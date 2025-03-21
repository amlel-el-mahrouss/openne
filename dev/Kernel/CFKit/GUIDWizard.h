/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#include <CFKit/GUIDWrapper.h>
#include <NewKit/Array.h>
#include <NewKit/ArrayList.h>
#include <NewKit/Defines.h>
#include <NewKit/ErrorOr.h>
#include <NewKit/Ref.h>
#include <NewKit/Stream.h>
#include <NewKit/KString.h>

namespace CFKit::XRN::Version1
{
	using namespace OpenNE;

	Ref<GUIDSequence*>			  cf_make_sequence(const ArrayList<UInt32>& seq);
	ErrorOr<Ref<OpenNE::KString>> cf_try_guid_to_string(Ref<GUIDSequence*>& guid);
} // namespace CFKit::XRN::Version1
