/*
 *	========================================================
 *
 *	NetBoot
 * 	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved., all rights reserved.
 *
 * 	========================================================
 */

#pragma once

#include <NewKit/Defines.h>

#define kNetBootINetMagic		"NETB"
#define kNetBootINetMagicLength 4

/// @brief the internet header is used to download updates OTA.
typedef struct NetBootInternetHeader
{
	OpenNE::Char NB1; /// magic char 1 'N'
	OpenNE::Char NB2; /// magic char 2 'E'
	OpenNE::Char NB3; /// magic char 3 'T'
	OpenNE::Char NB4; /// magic char 4 'B'

	OpenNE::Char	PatchName[255];	  /// example: Modjo
	OpenNE::Int32	PatchLength;	  /// the patch length.
	OpenNE::Char	PatchTarget[255]; /// the target file.
	OpenNE::Boolean ImpliesROM;		  /// does it imply an EEPROM reprogram?
	OpenNE::Boolean Preflight;		  /// is it a preflight packet.
	OpenNE::Char	Data[];			  /// non preflight packet has a patch blob for a **PatchTarget**
} NetBootInternetHeader;
