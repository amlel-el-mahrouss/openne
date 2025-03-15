/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

	File: HPET.h
	Purpose: HPET builtin.

	Revision History:

------------------------------------------- */

#pragma once

#include <NewKit/Defines.h>
#include <Mod/ACPI/ACPI.h>

namespace OpenNE
{
	struct PACKED HPETAddressStructure final
	{
		OpenNE::UInt8  AddressSpaceId; // 0 - system memory, 1 - system I/O
		OpenNE::UInt8  RegisterBitWidth;
		OpenNE::UInt8  RegisterBitOffset;
		OpenNE::UInt8  Reserved;
		OpenNE::UInt64 Address;
	};

	struct PACKED HPETHeader final : public SDT
	{
		OpenNE::UInt8		 HardwareRevId;
		OpenNE::UInt8		 ComparatorCount : 5;
		OpenNE::UInt8		 CounterSize : 1;
		OpenNE::UInt8		 Reserved : 1;
		OpenNE::UInt8		 LegacyReplacement : 1;
		OpenNE::UInt16		 PciVendorId;
		HPETAddressStructure Address;
		OpenNE::UInt8		 HpetNumber;
		OpenNE::UInt16		 MinimumTick;
		OpenNE::UInt8		 PageProtection;
	};

} // namespace OpenNE
