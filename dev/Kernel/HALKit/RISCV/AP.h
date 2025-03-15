/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

	File: AP.h
	Purpose: RISC-V hardware threads.

	Revision History:

	30/01/24: Added file (amlel)

------------------------------------------- */

#pragma once

#include <NewKit/Defines.h>

namespace OpenNE
{
	typedef Int64 hal_ap_kind;

	typedef struct HAL_HARDWARE_THREAD
	{
		OpenNE::UIntPtr fStartAddress;
		OpenNE::UInt8	fPrivleged : 1;
		OpenNE::UInt32	fPageMemoryFlags;
		hal_ap_kind		fIdentNumber;
	} HAL_HARDWARE_THREAD;

	/// @brief Set PC to specific hart.
	/// @param hart the hart
	/// @param epc the pc.
	/// @return
	EXTERN_C OpenNE::Void hal_set_pc_to_hart(HAL_HARDWARE_THREAD* hart, OpenNE::VoidPtr epc);

} // namespace OpenNE
