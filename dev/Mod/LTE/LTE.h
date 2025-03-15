/* -------------------------------------------

Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved..

File: LTE\LTE.h.
Purpose: LTE Standard Library.

------------------------------------------- */

#ifndef _INC_NETWORK_LTE_H_
#define _INC_NETWORK_LTE_H_

#include <NewKit/Defines.h>
#include <NewKit/KString.h>

/// @brief Long Term Evolution I/O routines.

/// @brief Turn on SIM slot.
OpenNE::Boolean lte_turn_on_sim(OpenNE::Int32 simSlot);

/// @brief Turn off SIM slot.
OpenNE::Boolean lte_turn_off_sim(OpenNE::Int32 simSlot);

/// @brief Send AT command.
OpenNE::Boolean lte_send_at_command(OpenNE::Char* buf,
									OpenNE::Size  bufReadSz,
									OpenNE::Int32 simSlot);

OpenNE::Boolean lte_write_sim_file(OpenNE::Char*   file,
								   OpenNE::VoidPtr buf,
								   OpenNE::Size	   bufSz,
								   OpenNE::Size	   offset,
								   OpenNE::Int32   simSlot);

OpenNE::VoidPtr lte_read_sim_file(OpenNE::Char* file,
								  OpenNE::Size	bufSz,
								  OpenNE::Size	offset,
								  OpenNE::Int32 simSlot);

#endif // ifndef _INC_NETWORK_LTE_H_
