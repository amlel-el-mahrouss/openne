/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

	File: PE.h
	Purpose: Portable Executable for Kernel.

	Revision History:

	30/01/24: Added file (amlel)

------------------------------------------- */

#ifndef __KERNELKIT_INC_PE_H__
#define __KERNELKIT_INC_PE_H__

#include <NewKit/Defines.h>

#define kPeSignature 0x00004550

#define kPeDLLBase 0x4000000
#define kPeEXEBase 0x1000000

#define kPeMagic32 0x010b
#define kPeMagic64 0x020b

#define kPeMachineAMD64 0x8664
#define kPeMachineARM64 0xaa64

typedef struct LDR_EXEC_HEADER final
{
	OpenNE::UInt32 mSignature;
	OpenNE::UInt16 mMachine;
	OpenNE::UInt16 mNumberOfSections;
	OpenNE::UInt32 mTimeDateStamp;
	OpenNE::UInt32 mPointerToSymbolTable;
	OpenNE::UInt32 mNumberOfSymbols;
	OpenNE::UInt16 mSizeOfOptionalHeader;
	OpenNE::UInt16 mCharacteristics;
} LDR_EXEC_HEADER, *LDR_EXEC_HEADER_PTR;

typedef struct LDR_OPTIONAL_HEADER final
{
	OpenNE::UInt16 mMagic; // 0x010b - PE32, 0x020b - PE32+ (64 bit)
	OpenNE::UInt8  mMajorLinkerVersion;
	OpenNE::UInt8  mMinorLinkerVersion;
	OpenNE::UInt32 mSizeOfCode;
	OpenNE::UInt32 mSizeOfInitializedData;
	OpenNE::UInt32 mSizeOfUninitializedData;
	OpenNE::UInt32 mAddressOfEntryPoint;
	OpenNE::UInt32 mBaseOfCode;
	OpenNE::UInt32 mBaseOfData;
	OpenNE::UInt32 mImageBase;
	OpenNE::UInt32 mSectionAlignment;
	OpenNE::UInt32 mFileAlignment;
	OpenNE::UInt16 mMajorOperatingSystemVersion;
	OpenNE::UInt16 mMinorOperatingSystemVersion;
	OpenNE::UInt16 mMajorImageVersion;
	OpenNE::UInt16 mMinorImageVersion;
	OpenNE::UInt16 mMajorSubsystemVersion;
	OpenNE::UInt16 mMinorSubsystemVersion;
	OpenNE::UInt32 mWin32VersionValue;
	OpenNE::UInt32 mSizeOfImage;
	OpenNE::UInt32 mSizeOfHeaders;
	OpenNE::UInt32 mCheckSum;
	OpenNE::UInt16 mSubsystem;
	OpenNE::UInt16 mDllCharacteristics;
	OpenNE::UInt32 mSizeOfStackReserve;
	OpenNE::UInt32 mSizeOfStackCommit;
	OpenNE::UInt32 mSizeOfHeapReserve;
	OpenNE::UInt32 mSizeOfHeapCommit;
	OpenNE::UInt32 mLoaderFlags;
	OpenNE::UInt32 mNumberOfRvaAndSizes;
} LDR_OPTIONAL_HEADER, *LDR_OPTIONAL_HEADER_PTR;

typedef struct LDR_SECTION_HEADER final
{
	OpenNE::Char   mName[8];
	OpenNE::UInt32 mVirtualSize;
	OpenNE::UInt32 mVirtualAddress;
	OpenNE::UInt32 mSizeOfRawData;
	OpenNE::UInt32 mPointerToRawData;
	OpenNE::UInt32 mPointerToRelocations;
	OpenNE::UInt32 mPointerToLineNumbers;
	OpenNE::UInt16 mNumberOfRelocations;
	OpenNE::UInt16 mNumberOfLinenumbers;
	OpenNE::UInt32 mCharacteristics;
} LDR_SECTION_HEADER, *LDR_SECTION_HEADER_PTR;

enum kExecDataDirParams
{
	kExecExport,
	kExecImport,
	kExecInvalid,
	kExecCount,
};

typedef struct LDR_EXPORT_DIRECTORY
{
	OpenNE::UInt32 mCharacteristics;
	OpenNE::UInt32 mTimeDateStamp;
	OpenNE::UInt16 mMajorVersion;
	OpenNE::UInt16 mMinorVersion;
	OpenNE::UInt32 mName;
	OpenNE::UInt32 mBase;
	OpenNE::UInt32 mNumberOfFunctions;
	OpenNE::UInt32 mNumberOfNames;
	OpenNE::UInt32 mAddressOfFunctions; // export table rva
	OpenNE::UInt32 mAddressOfNames;
	OpenNE::UInt32 mAddressOfNameOrdinal; // ordinal table rva
} LDR_EXPORT_DIRECTORY, *LDR_EXPORT_DIRECTORY_PTR;

typedef struct LDR_IMPORT_DIRECTORY
{
	union {
		OpenNE::UInt32 mCharacteristics;
		OpenNE::UInt32 mOriginalFirstThunk;
	};
	OpenNE::UInt32 mTimeDateStamp;
	OpenNE::UInt32 mForwarderChain;
	OpenNE::UInt32 mNameRva;
	OpenNE::UInt32 mThunkTableRva;
} LDR_IMPORT_DIRECTORY, *LDR_IMPORT_DIRECTORY_PTR;

typedef struct LDR_DATA_DIRECTORY
{
	OpenNE::UInt32 VirtualAddress;
	OpenNE::UInt32 Size;
} LDR_DATA_DIRECTORY, *LDR_DATA_DIRECTORY_PTR;

typedef struct LDR_IMAGE_HEADER
{
	LDR_EXEC_HEADER		mHeader;
	LDR_OPTIONAL_HEADER mOptHdr;
} LDR_IMAGE_HEADER, *LDR_IMAGE_HEADER_PTR;

enum
{
	eUserSection  = 0x00000020,
	cPEResourceId = 0xFFaadd00,
};

#endif /* ifndef __KERNELKIT_INC_PE_H__ */
