/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

/**
	@brief The Explicit Partition Map scheme.
*/

#ifndef FIRMWAREKIT_EPM_H
#define FIRMWAREKIT_EPM_H

#include <NewKit/Defines.h>

#define kEPMNameLength		 (32)
#define kEPMFilesystemLength (16)
#define kEPMMagicLength		 (5)

/* @brief AMD64 magic for EPM */
#define kEPMMagic86 "EPMAM"

/* @brief RISC-V magic for EPM */
#define kEPMMagicRISCV "EPMRV"

/* @brief ARM magic for EPM */
#define kEPMMagicARM "EPMAR"

/* @brief POWER magic for EPM */
#define kEPMMagicPPC "EPMPC"

/* @brief Invalid magic for EPM */
#define kEPMMagicError "EPM??"

#ifdef __OPENNE_AMD64__
#define kEPMMagic kEPMMagic86
#else
#ifdef __OPENNE_ARM64__
#define kEPMMagic kEPMMagicARM
#else
#define kEPMMagic kEPMMagicError
#endif
#endif

///! @brief partition must start at this address.
///! Anything below is reserved for Data backup by the Main OS.
#define kEPMPartBlockLba (sizeof(EPM_PART_BLOCK))

///! @brief Current EPM revision.
#define kEPMRevisionBcd (0x0100)

/// !@brief EPM boot block address.
#define kEPMBootBlockLba (0U)

#define kEPMReserveLen (401)

struct EPM_PART_BLOCK;

/* The first 0 > 128 addresses of a disk contains these headers. */

/**
 * @brief The EPM boot block.
 * @note NumBlock and LbaStart are ignored on some platforms.
 */
struct PACKED EPM_PART_BLOCK
{
	OpenNE::Char  Magic[kEPMMagicLength];
	OpenNE::Char  Name[kEPMNameLength];
	OpenNE::Int32 Version;
	OpenNE::Int64 NumBlocks;
	OpenNE::Int64 SectorSz;
	OpenNE::Int64 LbaStart; // base offset
	OpenNE::Int64 LbaEnd;	// addition of lba_start to get the end of partition.
	OpenNE::Int16 Kind;
	OpenNE::Int32 FsVersion;
	OpenNE::Char  Fs[kEPMFilesystemLength]; /* OpenNeFS, NeFS, ffs2... */
	OpenNE::Char  Reserved[kEPMReserveLen]; // to fill a full sector.
};

///! @brief Version kind enum.
///! @brief Use in boot block version field.

enum
{
	kEPMInvalid	  = 0x00,
	kEPMGenericOS = 0xcf, // Generic OS
	kEPMLinux	  = 0x8f, // Linux on EPM
	kEPMBSD		  = 0x9f, // Berkeley Soft. Distribution
	kEPMOneOS	  = 0x1f, // This OS.
	kEPMInvalidOS = 0xff,
};

typedef struct EPM_PART_BLOCK BOOT_BLOCK_STRUCT;

#endif // ifndef FIRMWAREKIT_EPM_H
