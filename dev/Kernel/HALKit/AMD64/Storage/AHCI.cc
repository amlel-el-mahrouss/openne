/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

/**
 * @file AHCI.cc
 * @author Amlal EL Mahrouss (amlalelmahrouss@icloud.com)
 * @brief AHCI driver.
 * @version 0.1
 * @date 2024-02-02
 *
 * @Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.
 *
 */

#include <KernelKit/UserProcessScheduler.h>
#include <KernelKit/LPC.h>

#include <Mod/ATA/ATA.h>
#include <Mod/AHCI/AHCI.h>
#include <KernelKit/PCI/Iterator.h>
#include <NewKit/Utils.h>
#include <KernelKit/LockDelegate.h>

#ifdef __AHCI__

#define kHBAErrTaskFile (1 << 30)
#define kHBACmdGhc		(1 << 31)
#define kHBACmdAE		(0x80000000)
#define kHBAPxCmdST		(0x0001)
#define kHBAPxCmdFre	(0x0010)
#define kHBAPxCmdFR		(0x4000)
#define kHBAPxCmdCR		(0x8000)

#define kSATALBAMode (1 << 6)

#define kSATASRBsy (0x80)
#define kSATASRDrq (0x08)

#define kSATAPortCnt (0x20)

#define kSATAProgIfAHCI (0x01)
#define kSATASubClass	(0x06)
#define kSATABar5		(0x24)

using namespace OpenNE;

STATIC PCI::Device kDevice;
STATIC HbaMem* kSATA					   = nullptr;
STATIC SizeT kSATAPortIdx		   = 0UL;
STATIC Lba kCurrentDiskSectorCount = 0UL;
STATIC Char kCurrentDiskModel[50]  = {"UNKNOWN SATA DRIVE"};

template <BOOL Write, BOOL CommandOrCTRL, BOOL Identify>
static Void drvi_std_input_output(UInt64 lba, UInt8* buffer, SizeT sector_sz, SizeT size_buffer) noexcept;

static Int32 drvi_find_cmd_slot(HbaPort* port) noexcept;

static Void drvi_calculate_disk_geometry() noexcept;

static Void drvi_calculate_disk_geometry() noexcept
{
	kCurrentDiskSectorCount = 0UL;

	UInt8 identify_data[512] = {};

	drvi_std_input_output<NO, NO, YES>(0, identify_data, 0, 512);

	kCurrentDiskSectorCount = (identify_data[61] << 16) | identify_data[60];

	for (int i = 0; i < 20; i++)
	{
		kCurrentDiskModel[i * 2]	 = identify_data[27 + i] >> 8;
		kCurrentDiskModel[i * 2 + 1] = identify_data[27 + i] & 0xFF;
	}

	kCurrentDiskModel[40] = '\0';

	kout << "Drive Model: " << kCurrentDiskModel << endl;

	kout << "Disk Size: " << number(drv_get_size()) << endl;
	kout << "Disk Sector Count: " << number(kCurrentDiskSectorCount) << endl;
}

/// @brief Initializes an AHCI disk.
/// @param pi the amount of kSATA that have been detected.
/// @return if the disk was successfully initialized or not.
Boolean drv_std_init(UInt16& pi)
{
	using namespace OpenNE;

	PCI::Iterator iterator(Types::PciDeviceKind::MassStorageController);

	for (SizeT device_index = 0; device_index < OPENNE_BUS_COUNT; ++device_index)
	{
		kDevice = iterator[device_index].Leak(); // And then leak the reference.

		// if SATA and then interface is AHCI...
		if (kDevice.Subclass() == kSATASubClass &&
			kDevice.ProgIf() == kSATAProgIfAHCI)
		{
			kDevice.EnableMmio(kSATABar5);		// Enable the memory index_byte/o for this ahci device.
			kDevice.BecomeBusMaster(kSATABar5); // Become bus master for this ahci device, so that we can control it.

			HbaMem* mem_ahci = (HbaMem*)kDevice.Bar(kSATABar5);

			kout << hex_number((UIntPtr)mem_ahci) << endl;

			UInt32 ports_implemented = mem_ahci->Pi;
			UInt16 ahci_index		 = 0;

			const UInt16 kMaxPortsImplemented = kSATAPortCnt;
			const UInt32 kSATASignature		  = 0x00000101;
			const UInt8	 kSATAPresent		  = 0x03;
			const UInt8	 kSATAIPMActive		  = 0x01;

			while (ahci_index < kMaxPortsImplemented)
			{
				if (ports_implemented)
				{
					UInt8 ipm = (mem_ahci->Ports[ahci_index].Ssts >> 8) & 0x0F;
					UInt8 det = mem_ahci->Ports[ahci_index].Ssts & 0x0F;

					HAL::mm_map_page(mem_ahci, mem_ahci, HAL::kMMFlagsWr);

					if (mem_ahci->Ports[ahci_index].Sig == kSATASignature && det == 3 && ipm == 1 &&
						(mem_ahci->Ports[ahci_index].Ssts & 0xF))
					{
						kout << "Port is implemented as SATA.\r";

						kSATAPortIdx = ahci_index;
						kSATA		 = mem_ahci;

						// Restart the HBA.

						kSATA->Ports[kSATAPortIdx].Cmd &= ~(kHBAPxCmdST | kHBAPxCmdFre); // Disable command and FIS reception

						while (kSATA->Ports[kSATAPortIdx].Cmd & kHBAPxCmdCR)
							; // Wait for controller to stop

						kSATA->Ports[kSATAPortIdx].Serr = 0xFFFFFFFF; // Clear errors
						kSATA->Ports[kSATAPortIdx].Is	= 0xFFFFFFFF; // Clear interrupts

						kSATA->Ports[kSATAPortIdx].Cmd |= kHBAPxCmdFre; // Re-enable FIS reception
						kSATA->Ports[kSATAPortIdx].Cmd |= kHBAPxCmdST;	// Start command engine

						kSATA->Ghc |= kHBACmdAE; // Enable AHCI mode

						HAL::rt_wait_400ns();

						drvi_calculate_disk_geometry();

						return YES;
					}
				}

				ports_implemented >>= 1;
				++ahci_index;
			}
		}
	}

	return No;
}

Boolean drv_std_detected(Void)
{
	return kDevice.DeviceId() != 0xFFFF && kCurrentDiskSectorCount > 0;
}

Void drv_std_write(UInt64 lba, Char* buffer, SizeT sector_sz, SizeT size_buffer)
{
	drvi_std_input_output<YES, YES, NO>(lba, (UInt8*)buffer, sector_sz, size_buffer);
}

Void drv_std_read(UInt64 lba, Char* buffer, SizeT sector_sz, SizeT size_buffer)
{
	drvi_std_input_output<NO, YES, NO>(lba, (UInt8*)buffer, sector_sz, size_buffer);
}

static Int32 drvi_find_cmd_slot(HbaPort* port) noexcept
{
	UInt32 slots = port->Ci;

	for (Int32 i = 0; i < kSATAPortCnt; i++)
	{
		if ((slots & i) == 0)
			return i;

		slots >>= 1;
	}

	return -1;
}

template <BOOL Write, BOOL CommandOrCTRL, BOOL Identify>
static Void drvi_std_input_output(UInt64 lba, UInt8* buffer, SizeT sector_sz, SizeT size_buffer) noexcept
{
	const SizeT slot = drvi_find_cmd_slot(&kSATA->Ports[kSATAPortIdx]);

	if (slot == -1)
		return;

	if (size_buffer > mib_cast(4))
		ke_panic(RUNTIME_CHECK_FAILED, "AHCI only supports < 4mb DMA transfers per PRD.");

	if (!Write)
		rt_set_memory(buffer, 0, size_buffer);

	HbaCmdHeader* command_header = ((HbaCmdHeader*)((UInt64)(kSATA->Ports[kSATAPortIdx].Clb)));

	MUST_TRY(command_header != nullptr);

	command_header->Cfl			 = sizeof(FisRegH2D) / sizeof(UInt32);
	command_header->Write		 = Write;
	command_header->Prdtl		 = 1;
	command_header->Atapi		 = 0;
	command_header->Prefetchable = 1;

	HbaCmdTbl* command_table = ((HbaCmdTbl*)((UInt64)(command_header->Ctba)));

	rt_set_memory(command_table, 0, sizeof(HbaCmdTbl));

	MUST_PASS(command_table);

	auto phys_dma_buf = HAL::hal_get_phys_address((VoidPtr)buffer);

	command_table->Prdt[0].Dba	= ((UInt32)(UInt64)phys_dma_buf & 0xFFFFFFFF);
	command_table->Prdt[0].Dbau = (((UInt64)phys_dma_buf << 32));
	command_table->Prdt[0].Dbc	= ((size_buffer)-1) | (1 << 31);

	FisRegH2D* h2d_fis = (FisRegH2D*)((UInt64)&command_table->Cfis);

	h2d_fis->FisType   = kFISTypeRegH2D;
	h2d_fis->CmdOrCtrl = CommandOrCTRL;
	h2d_fis->Command   = Write ? kAHCICmdWriteDmaEx : kAHCICmdReadDmaEx;

	if (Identify)
		h2d_fis->Command = kAHCICmdIdentify;

	h2d_fis->Lba0 = (lba & 0xFF);
	h2d_fis->Lba1 = (lba >> 8) & 0xFF;
	h2d_fis->Lba2 = (lba >> 16) & 0xFF;
	h2d_fis->Lba3 = (lba >> 24) & 0xFF;
	h2d_fis->Lba4 = (lba >> 32) & 0xFF;
	h2d_fis->Lba5 = (lba >> 40) & 0xFF;

	h2d_fis->Device = kSATALBAMode;

	// 28-bit LBA mode, fis is done being configured.

	h2d_fis->CountLow  = sector_sz & 0xFF;
	h2d_fis->CountHigh = (sector_sz >> 8) & 0xFF;

	if (kSATA->Is & kHBAErrTaskFile)
		ke_panic(RUNTIME_CHECK_BAD_BEHAVIOR, "AHCI Read disk failure, faulty component.");

	while (kSATA->Ports[kSATAPortIdx].Cmd & (kHBAPxCmdCR | kHBAPxCmdFR))
		; // Ensure controller is idle

	kSATA->Ports[kSATAPortIdx].Cmd |= kHBAPxCmdFre;
	kSATA->Ports[kSATAPortIdx].Ci = (1 << slot);

	while (YES)
	{
		if (kSATA->Ports[kSATAPortIdx].Ci == 0)
			break;

		kout << "PxCI: " << hex_number(kSATA->Ports[kSATAPortIdx].Ci) << endl;
		kout << "PxCMD: " << hex_number(kSATA->Ports[kSATAPortIdx].Cmd) << endl;

		if (kSATA->Is & kHBAErrTaskFile)
			ke_panic(RUNTIME_CHECK_BAD_BEHAVIOR, "AHCI Read disk failure, faulty component.");
	}
}

/***
	 @brief Gets the number of sectors inside the drive.
	 @return Sector size in bytes.
  */
SizeT drv_get_sector_count()
{
	return kCurrentDiskSectorCount;
}

/// @brief Get the drive size.
/// @return Disk size in bytes.
SizeT drv_get_size()
{
	return (drv_get_sector_count()) * kAHCISectorSize;
}

#endif // ifdef __AHCI__