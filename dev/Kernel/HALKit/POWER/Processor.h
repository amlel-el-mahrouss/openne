/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

	Purpose: POWER processor header.

------------------------------------------- */

#pragma once

#include <NewKit/Defines.h>
#include <NewKit/Utils.h>

#define NoOp()			 asm volatile("mr 0, 0")
#define kHalPPCAlignment __attribute__((aligned(4)))

namespace OpenNE::HAL
{
	typedef UIntPtr Reg;

	/// @brief Stack frame (as retrieved from assembly.)
	struct PACKED StackFrame final
	{
		Reg R8{0};
		Reg R9{0};
		Reg R10{0};
		Reg R11{0};
		Reg R12{0};
		Reg R13{0};
		Reg R14{0};
		Reg R15{0};
		Reg SP{0};
		Reg BP{0};
	};

	typedef StackFrame* StackFramePtr;

	inline void rt_halt()
	{
		while (true)
		{
			NoOp(); // no oop.
		}
	}

	inline void rt_cli()
	{
		NoOp(); // no oop
	}
} // namespace OpenNE::HAL

EXTERN_C OpenNE::Void int_handle_math(OpenNE::UIntPtr sp);
EXTERN_C OpenNE::Void int_handle_pf(OpenNE::UIntPtr sp);

/// @brief Set TLB.
OpenNE::Bool hal_set_tlb(OpenNE::UInt8 tlb, OpenNE::UInt32 epn, OpenNE::UInt64 rpn, OpenNE::UInt8 perms, OpenNE::UInt8 wimge, OpenNE::UInt8 ts, OpenNE::UInt8 esel, OpenNE::UInt8 tsize, OpenNE::UInt8 iprot);

/// @brief Write TLB.
OpenNE::Void hal_write_tlb(OpenNE::UInt32 mas0, OpenNE::UInt32 mas1, OpenNE::UInt32 mas2, OpenNE::UInt32 mas3, OpenNE::UInt32 mas7);

/// @brief Flush TLB.
EXTERN_C OpenNE::Void hal_flush_tlb();
