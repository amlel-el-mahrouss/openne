/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#include <ArchKit/ArchKit.h>
#include <KernelKit/UserProcessScheduler.h>
#include <NewKit/KString.h>
#include <POSIXKit/signal.h>

STATIC BOOL kIsScheduling = NO;

/// @brief Handle GPF fault.
/// @param rsp
EXTERN_C void idt_handle_gpf(Kernel::UIntPtr rsp)
{
	auto process = Kernel::UserProcessScheduler::The().CurrentProcess();

	if (process.Leak().Status != Kernel::ProcessStatusKind::kRunning)
	{
		while (YES)
			;
	}

	kIsScheduling = NO;

	kout << "KTrace: GPF.\r";

	process.Leak().ProcessSignal.SignalArg		= 0UL;
	process.Leak().ProcessSignal.SignalID		= SIGKILL;
	process.Leak().ProcessSignal.PreviousStatus = process.Leak().Status;

	kout << "KTrace: SIGKILL.\r";

	process.Leak().Status = Kernel::ProcessStatusKind::kKilled;

	process.Leak().Crash();
}

/// @brief Handle page fault.
/// @param rsp
EXTERN_C void idt_handle_pf(Kernel::UIntPtr rsp)
{
	auto process = Kernel::UserProcessScheduler::The().CurrentProcess();

	if (process.Leak().Status != Kernel::ProcessStatusKind::kRunning)
		return;

	kIsScheduling = NO;

	kout << "KTrace: Page Fault.\r";

	process.Leak().ProcessSignal.SignalArg		= 0UL;
	process.Leak().ProcessSignal.SignalID		= SIGKILL;
	process.Leak().ProcessSignal.PreviousStatus = process.Leak().Status;

	kout << "KTrace: SIGKILL.\r";

	process.Leak().Status = Kernel::ProcessStatusKind::kKilled;

	process.Leak().Crash();
}

/// @brief Handle scheduler interrupt.
EXTERN_C void idt_handle_scheduler(Kernel::UIntPtr rsp)
{
	static Kernel::Int64 try_count_before_brute = 100000UL;

	while (kIsScheduling)
	{
		--try_count_before_brute;

		if (try_count_before_brute < 1)
			break;
	}

	try_count_before_brute = 100000UL;
	kIsScheduling		   = YES;

	kout << "KTrace: Timer IRQ (Scheduler Notification).\r";
	Kernel::UserProcessHelper::StartScheduling();

	kIsScheduling = NO;
}

/// @brief Handle math fault.
/// @param rsp
EXTERN_C void idt_handle_math(Kernel::UIntPtr rsp)
{
	auto process = Kernel::UserProcessScheduler::The().CurrentProcess();

	if (process.Leak().Status != Kernel::ProcessStatusKind::kRunning)
		return;

	kIsScheduling = NO;

	kout << "KTrace: Math error (division by zero?)\r";

	process.Leak().ProcessSignal.SignalArg		= 0UL;
	process.Leak().ProcessSignal.SignalID		= SIGKILL;
	process.Leak().ProcessSignal.PreviousStatus = process.Leak().Status;

	kout << "KTrace: SIGKILL.\r";

	process.Leak().Status = Kernel::ProcessStatusKind::kKilled;

	process.Leak().Crash();
}

/// @brief Handle any generic fault.
/// @param rsp
EXTERN_C void idt_handle_generic(Kernel::UIntPtr rsp)
{
	auto process = Kernel::UserProcessScheduler::The().CurrentProcess();

	if (process.Leak().Status != Kernel::ProcessStatusKind::kRunning)
		return;

	kIsScheduling = NO;

	kout << "KTrace: Generic Process Fault.\r";

	process.Leak().ProcessSignal.SignalArg		= 0UL;
	process.Leak().ProcessSignal.SignalID		= SIGKILL;
	process.Leak().ProcessSignal.PreviousStatus = process.Leak().Status;

	kout << "KTrace: SIGKILL.\r";

	process.Leak().Status = Kernel::ProcessStatusKind::kKilled;

	process.Leak().Crash();
}

EXTERN_C Kernel::Void idt_handle_breakpoint(Kernel::UIntPtr rcx, Kernel::UIntPtr rsp)
{
	auto process = Kernel::UserProcessScheduler::The().CurrentProcess();

	if (process.Leak().Status != Kernel::ProcessStatusKind::kRunning)
	{
		/// Log KTrace data.

		kout << "KTrace: RCX: " << Kernel::hex_number(rcx) << endl;
		kout << "KTrace: RSP: " << Kernel::hex_number(rsp) << endl;
		kout << "KTrace: CR2: " << Kernel::hex_number((Kernel::UIntPtr)hal_read_cr2()) << endl;

		while (YES)
			;
	}

	kIsScheduling = NO;

	kout << "KTrace: RCX: " << Kernel::hex_number(rcx) << endl;
	kout << "KTrace: SIGTRAP.\r";

	process.Leak().ProcessSignal.SignalArg = rcx;
	process.Leak().ProcessSignal.SignalID  = SIGTRAP;

	process.Leak().ProcessSignal.PreviousStatus = process.Leak().Status;

	kout << "KTrace: SIGKILL.\r";

	process.Leak().Status = Kernel::ProcessStatusKind::kFrozen;
}

/// @brief Handle #UD fault.
/// @param rsp
EXTERN_C void idt_handle_ud(Kernel::UIntPtr rsp)
{
	auto process = Kernel::UserProcessScheduler::The().CurrentProcess();

	if (process.Leak().Status != Kernel::ProcessStatusKind::kRunning)
		return;

	kIsScheduling = NO;

	kout << "KTrace: Undefined Opcode.\r";

	process.Leak().ProcessSignal.SignalArg		= 0UL;
	process.Leak().ProcessSignal.SignalID		= SIGKILL;
	process.Leak().ProcessSignal.PreviousStatus = process.Leak().Status;

	kout << "KTrace: SIGKILL.\r";

	process.Leak().Status = Kernel::ProcessStatusKind::kKilled;

	process.Leak().Crash();
}

/// @brief Enter syscall from assembly.
/// @param stack the stack pushed from assembly routine.
/// @return nothing.
EXTERN_C Kernel::Void hal_system_call_enter(Kernel::UIntPtr rcx_syscall_index, Kernel::UIntPtr rdx_syscall_struct)
{
	if (rcx_syscall_index < kSyscalls.Count())
	{
		kout << "syscall: Enter Syscall.\r";

		if (kSyscalls[rcx_syscall_index].fHooked)
		{
			if (kSyscalls[rcx_syscall_index].fProc)
			{
				(kSyscalls[rcx_syscall_index].fProc)((Kernel::VoidPtr)rdx_syscall_struct);
			}
			else
			{
				kout << "syscall: syscall isn't valid at all! (is nullptr)\r";
			}
		}
		else
		{
			kout << "syscall: syscall isn't hooked at all! (is set to false)\r";
		}

		kout << "syscall: Exit Syscall.\r";
	}
}

/// @brief Enter Kernel call from assembly (DDK only).
/// @param stack the stack pushed from assembly routine.
/// @return nothing.
EXTERN_C Kernel::Void hal_kernel_call_enter(Kernel::UIntPtr rcx_kerncall_index, Kernel::UIntPtr rdx_kerncall_struct)
{
	if (rcx_kerncall_index < kKerncalls.Count())
	{
		kout << "kerncall: Enter Kernel Call List.\r";

		if (kKerncalls[rcx_kerncall_index].fHooked)
		{
			if (kKerncalls[rcx_kerncall_index].fProc)
			{
				(kKerncalls[rcx_kerncall_index].fProc)((Kernel::VoidPtr)rdx_kerncall_struct);
			}
			else
			{
				kout << "kerncall: Kernel call isn't valid at all! (is nullptr)\r";
			}
		}
		else
		{
			kout << "kerncall: Kernel call isn't hooked at all! (is set to false)\r";
		}

		kout << "kerncall: Exit Kernel Call List.\r";
	}
}
