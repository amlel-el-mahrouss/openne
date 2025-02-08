/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#ifdef __NEOSKRNL__
#include <NewKit/Defines.h>
#endif // ifdef __NEOSKRNL__

#define OPENNE_COPY_DELETE(KLASS)            \
	KLASS& operator=(const KLASS&) = delete; \
	KLASS(const KLASS&)			   = delete;

#define OPENNE_COPY_DEFAULT(KLASS)            \
	KLASS& operator=(const KLASS&) = default; \
	KLASS(const KLASS&)			   = default;

#define OPENNE_MOVE_DELETE(KLASS)       \
	KLASS& operator=(KLASS&&) = delete; \
	KLASS(KLASS&&)			  = delete;

#define OPENNE_MOVE_DEFAULT(KLASS)       \
	KLASS& operator=(KLASS&&) = default; \
	KLASS(KLASS&&)			  = default;
