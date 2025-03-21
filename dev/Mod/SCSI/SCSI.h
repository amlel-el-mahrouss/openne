/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#include <NewKit/Defines.h>

/// @file SCSI.h
/// @brief Serial SCSI driver.

template <int PacketBitLen>
using scsi_packet_type = OpenNE::UInt16[PacketBitLen];
