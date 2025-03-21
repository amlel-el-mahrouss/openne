/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#include <ArchKit/ArchKit.h>
#include <KernelKit/PCI/Device.h>

OpenNE::UInt pci_read_raw(OpenNE::UInt bar, OpenNE::UShort bus, OpenNE::UShort dev, OpenNE::UShort fun)
{
	OpenNE::UInt target = 0x80000000 | ((OpenNE::UInt)bus << 16) |
						  ((OpenNE::UInt)dev << 11) | ((OpenNE::UInt)fun << 8) |
						  (bar & 0xFC);

	OpenNE::HAL::rt_out32((OpenNE::UShort)OpenNE::PCI::PciConfigKind::ConfigAddress,
						  target);

	OpenNE::HAL::rt_wait_400ns();

	return OpenNE::HAL::rt_in32((OpenNE::UShort)OpenNE::PCI::PciConfigKind::ConfigData);
}

void pci_set_cfg_target(OpenNE::UInt bar, OpenNE::UShort bus, OpenNE::UShort dev, OpenNE::UShort fun)
{
	OpenNE::UInt target = 0x80000000 | ((OpenNE::UInt)bus << 16) |
						  ((OpenNE::UInt)dev << 11) | ((OpenNE::UInt)fun << 8) |
						  (bar & 0xFC);

	OpenNE::HAL::rt_out32((OpenNE::UShort)OpenNE::PCI::PciConfigKind::ConfigAddress,
						  target);

	OpenNE::HAL::rt_wait_400ns();
}

#define PCI_BAR_IO		 0x01
#define PCI_BAR_LOWMEM	 0x02
#define PCI_BAR_64		 0x04
#define PCI_BAR_PREFETCH 0x08

namespace OpenNE::PCI
{
	Device::Device(UShort bus, UShort device, UShort func, UInt32 bar)
		: fBus(bus), fDevice(device), fFunction(func), fBar(bar)
	{
	}

	Device::~Device() = default;

	UInt Device::Read(UInt bar, Size sz)
	{
		// Ensure aligned access by masking to 4-byte boundary
		pci_set_cfg_target(bar & 0xFC, fBus, fDevice, fFunction);

		// Read 4 bytes and shift out the correct value
		UInt data = HAL::rt_in32((UShort)PciConfigKind::ConfigData);

		if (sz == 4)
			return data;
		if (sz == 2)
			return (data >> ((bar & 2) * 8)) & 0xFFFF;
		if (sz == 1)
			return (data >> ((bar & 3) * 8)) & 0xFF;

		return (UShort)PciConfigKind::Invalid;
	}

	void Device::Write(UInt bar, UIntPtr data, Size sz)
	{
		pci_set_cfg_target(bar & 0xFC, fBus, fDevice, fFunction);

		if (sz == 4)
			HAL::rt_out32((UShort)PciConfigKind::ConfigData, (UInt)data);
		else if (sz == 2)
		{
			UInt temp = HAL::rt_in32((UShort)PciConfigKind::ConfigData);
			temp &= ~(0xFFFF << ((bar & 2) * 8));
			temp |= (data & 0xFFFF) << ((bar & 2) * 8);
			HAL::rt_out32((UShort)PciConfigKind::ConfigData, temp);
		}
		else if (sz == 1)
		{
			UInt temp = HAL::rt_in32((UShort)PciConfigKind::ConfigData);
			temp &= ~(0xFF << ((bar & 3) * 8));
			temp |= (data & 0xFF) << ((bar & 3) * 8);
			HAL::rt_out32((UShort)PciConfigKind::ConfigData, temp);
		}
	}

	UShort Device::DeviceId()
	{
		return (UShort)(pci_read_raw(0x0, fBus, fDevice, fFunction) >> 16);
	}

	UShort Device::VendorId()
	{
		return (UShort)(pci_read_raw(0x0, fBus, fDevice, fFunction) & 0xFFFF);
	}

	UShort Device::InterfaceId()
	{
		return (UShort)(pci_read_raw(0x0, fBus, fDevice, fFunction) >> 16);
	}

	UChar Device::Class()
	{
		return (UChar)(pci_read_raw(0x08, fBus, fDevice, fFunction) >> 24);
	}

	UChar Device::Subclass()
	{
		return (UChar)(pci_read_raw(0x08, fBus, fDevice, fFunction) >> 16);
	}

	UChar Device::ProgIf()
	{
		return (UChar)(pci_read_raw(0x08, fBus, fDevice, fFunction) >> 8);
	}

	UChar Device::HeaderType()
	{
		return (UChar)(pci_read_raw(0xC, fBus, fDevice, fFunction) >> 16);
	}

	void Device::EnableMmio(UInt32 bar_in)
	{
		UInt32 enable = Read((UShort)PciConfigKind::CommandReg, sizeof(UInt32));
		enable |= (1 << 1);

		Write((UShort)PciConfigKind::CommandReg, enable, sizeof(UInt32));
	}

	void Device::BecomeBusMaster(UInt32 bar_in)
	{
		UInt32 enable = Read((UShort)PciConfigKind::CommandReg, sizeof(UInt32));
		enable |= (1 << 2);

		Write((UShort)PciConfigKind::CommandReg, enable, sizeof(UInt32));
	}

	UIntPtr Device::Bar(UInt32 bar_in)
	{
		UInt32 bar = pci_read_raw(bar_in & 0xFC, fBus, fDevice, fFunction);

		if (bar & PCI_BAR_IO)
			return bar & ~0x03;

		if (bar & PCI_BAR_64)
		{
			UInt32 high = pci_read_raw((bar_in + 4) & 0xFC, fBus, fDevice, fFunction);
			return ((UIntPtr)high << 32) | (bar & ~0x0F);
		}

		return bar & ~0x0F;
	}

	UShort Device::Vendor()
	{
		UShort vendor = VendorId();

		if (vendor != (UShort)PciConfigKind::Invalid)
			fDevice = (UShort)Read(0x0, sizeof(UShort));

		return fDevice;
	}

	Device::operator bool()
	{
		return VendorId() != (UShort)PciConfigKind::Invalid;
	}
} // namespace OpenNE::PCI