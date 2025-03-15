/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

	File: BootString.cc
	Purpose: BootZ string library

	Revision History:



------------------------------------------- */

#include <BootKit/Platform.h>
#include <BootKit/Protocol.h>
#include <BootKit/BootKit.h>

/// BUGS: 0

/////////////////////////////////////////////////////////////////////////////////////////////////////////

OpenNE::SizeT Boot::BCopyMem(CharacterTypeUTF16* dest, CharacterTypeUTF16* src, const OpenNE::SizeT len)
{
	if (!dest || !src)
		return 0;

	SizeT index = 0UL;
	for (; index < len; ++index)
	{
		dest[index] = src[index];
	}

	return index;
}

OpenNE::SizeT Boot::BStrLen(const CharacterTypeUTF16* ptr)
{
	if (!ptr)
		return 0;

	OpenNE::SizeT cnt = 0;

	while (*ptr != (CharacterTypeUTF16)0)
	{
		++ptr;
		++cnt;
	}

	return cnt;
}

OpenNE::SizeT Boot::BSetMem(CharacterTypeUTF16* src, const CharacterTypeUTF16 byte, const OpenNE::SizeT len)
{
	if (!src)
		return 0;

	OpenNE::SizeT cnt = 0UL;

	while (*src != 0)
	{
		if (cnt > len)
			break;

		*src = byte;
		++src;

		++cnt;
	}

	return cnt;
}

OpenNE::SizeT Boot::BSetMem(CharacterTypeUTF8* src, const CharacterTypeUTF8 byte, const OpenNE::SizeT len)
{
	if (!src)
		return 0;

	OpenNE::SizeT cnt = 0UL;

	while (*src != 0)
	{
		if (cnt > len)
			break;

		*src = byte;
		++src;

		++cnt;
	}

	return cnt;
}
