/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#include <ArchKit/ArchKit.h>
#include <KernelKit/UserProcessScheduler.h>
#include <KernelKit/HardwareThreadScheduler.h>
#include <KernelKit/CodeMgr.h>
#include <Mod/ACPI/ACPIFactoryInterface.h>
#include <NetworkKit/IPC.h>
#include <CFKit/Property.h>
#include <Mod/CoreGfx/TextMgr.h>

EXTERN_C OpenNE::VoidPtr kInterruptVectorTable[];
EXTERN_C OpenNE::VoidPtr mp_user_switch_proc;
EXTERN_C OpenNE::Char mp_user_switch_proc_stack_begin[];

EXTERN_C OpenNE::rtl_ctor_kind __CTOR_LIST__[];
EXTERN_C OpenNE::VoidPtr __DTOR_LIST__;

EXTERN_C OpenNE::Void rtl_kernel_main(OpenNE::SizeT argc, char** argv, char** envp, OpenNE::SizeT envp_len);

STATIC OpenNE::Void hal_init_cxx_ctors()
{
	for (OpenNE::SizeT i = 0U; i < OpenNE::UserProcessScheduler::The().CurrentTeam().AsArray().Count(); ++i)
	{
		OpenNE::UserProcessScheduler::The().CurrentTeam().AsArray()[i]		  = OpenNE::UserProcess();
		OpenNE::UserProcessScheduler::The().CurrentTeam().AsArray()[i].Status = OpenNE::ProcessStatusKind::kKilled;
	}

	OpenNE::UserProcessScheduler::The().CurrentTeam().mProcessCount = 0UL;

	for (OpenNE::SizeT index = 0UL; __CTOR_LIST__[index] != __DTOR_LIST__; ++index)
	{
		OpenNE::rtl_ctor_kind constructor_cxx = (OpenNE::rtl_ctor_kind)__CTOR_LIST__[index];
		constructor_cxx();
	}
}

/// @brief Kernel init procedure.
EXTERN_C void hal_init_platform(
	OpenNE::HEL::BootInfoHeader* handover_hdr)
{
	kHandoverHeader = handover_hdr;

	if (kHandoverHeader->f_Magic != kHandoverMagic &&
		kHandoverHeader->f_Version != kHandoverVersion)
	{
		return;
	}

	hal_init_cxx_ctors();

	/************************************** */
	/*     INITIALIZE BIT MAP.              */
	/************************************** */

	kKernelBitMpSize  = kHandoverHeader->f_BitMapSize;
	kKernelBitMpStart = reinterpret_cast<OpenNE::VoidPtr>(
		reinterpret_cast<OpenNE::UIntPtr>(kHandoverHeader->f_BitMapStart));

	/************************************** */
	/*     INITIALIZE GDT AND SEGMENTS. */
	/************************************** */

	STATIC CONST auto kGDTEntriesCount = 6;

	/* GDT, mostly descriptors for user and kernel segments. */
	STATIC OpenNE::HAL::Detail::OPENNE_GDT_ENTRY ALIGN(0x08) kGDTArray[kGDTEntriesCount] = {
		{.fLimitLow = 0, .fBaseLow = 0, .fBaseMid = 0, .fAccessByte = 0x00, .fFlags = 0x00, .fBaseHigh = 0},   // Null entry
		{.fLimitLow = 0x0, .fBaseLow = 0, .fBaseMid = 0, .fAccessByte = 0x9A, .fFlags = 0xAF, .fBaseHigh = 0}, // Kernel code
		{.fLimitLow = 0x0, .fBaseLow = 0, .fBaseMid = 0, .fAccessByte = 0x92, .fFlags = 0xCF, .fBaseHigh = 0}, // Kernel data
		{.fLimitLow = 0x0, .fBaseLow = 0, .fBaseMid = 0, .fAccessByte = 0xFA, .fFlags = 0xAF, .fBaseHigh = 0}, // User code
		{.fLimitLow = 0x0, .fBaseLow = 0, .fBaseMid = 0, .fAccessByte = 0xF2, .fFlags = 0xCF, .fBaseHigh = 0}, // User data
	};

	// Load memory descriptors.
	OpenNE::HAL::RegisterGDT gdt_reg;

	gdt_reg.Base  = reinterpret_cast<OpenNE::UIntPtr>(kGDTArray);
	gdt_reg.Limit = (sizeof(OpenNE::HAL::Detail::OPENNE_GDT_ENTRY) * kGDTEntriesCount) - 1;

	//! GDT will load hal_read_init after it successfully loads the segments.
	OpenNE::HAL::GDTLoader gdt_loader;
	gdt_loader.Load(gdt_reg);

	OpenNE::ke_panic(RUNTIME_CHECK_BOOTSTRAP);
}

EXTERN_C OpenNE::Void hal_real_init(OpenNE::Void) noexcept
{
	rtl_kernel_main(0, nullptr, nullptr, 0);

	OpenNE::HAL::mp_get_cores(kHandoverHeader->f_HardwareTables.f_VendorPtr);

	OpenNE::HAL::Register64 idt_reg;

	idt_reg.Base = (OpenNE::UIntPtr)kInterruptVectorTable;

	OpenNE::HAL::IDTLoader idt_loader;

	idt_loader.Load(idt_reg);

	while (YES)
	{
		continue;
	}
}
