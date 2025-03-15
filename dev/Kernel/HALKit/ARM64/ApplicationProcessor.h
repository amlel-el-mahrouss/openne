/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#include <NewKit/Defines.h>
#include <HALKit/ARM64/Processor.h>

/************************************************** */
/*     INITIALIZE THE GIC ON CPU.              */
/************************************************** */

namespace OpenNE
{
	BOOL mp_initialize_gic(OpenNE::Void);
}