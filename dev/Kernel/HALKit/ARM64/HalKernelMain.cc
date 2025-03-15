/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#include <ArchKit/ArchKit.h>
#include <Mod/CoreGfx/FBMgr.h>
#include <FirmwareKit/Handover.h>
#include <KernelKit/FileMgr.h>
#include <KernelKit/MemoryMgr.h>
#include <KernelKit/PEFCodeMgr.h>
#include <KernelKit/UserProcessScheduler.h>
#include <NewKit/Json.h>
#include <KernelKit/CodeMgr.h>
#include <Mod/ACPI/ACPIFactoryInterface.h>
#include <NetworkKit/IPC.h>
#include <HALKit/ARM64/Processor.h>
#include <CFKit/Property.h>

EXTERN_C void hal_init_platform(
	OpenNE::HEL::BootInfoHeader* handover_hdr)
{

	/************************************************** */
	/*     INITIALIZE AND VALIDATE HEADER.              */
	/************************************************** */

	kHandoverHeader = handover_hdr;

	if (kHandoverHeader->f_Magic != kHandoverMagic &&
		kHandoverHeader->f_Version != kHandoverVersion)
	{
		return;
	}

	/************************************** */
	/*     INITIALIZE BIT MAP.              */
	/************************************** */

	kKernelBitMpSize  = kHandoverHeader->f_BitMapSize;
	kKernelBitMpStart = reinterpret_cast<OpenNE::VoidPtr>(
		reinterpret_cast<OpenNE::UIntPtr>(kHandoverHeader->f_BitMapStart));

	/// @note do initialize the interrupts after it.

	OpenNE::mp_initialize_gic();

	while (YES)
	{
	}
}
