/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#ifndef KIB
#define KIB(X) (OpenNE::UInt64)((X) / 1024)
#endif

#ifndef kib_cast
#define kib_cast(X) (OpenNE::UInt64)((X)*1024)
#endif

#ifndef MIB
#define MIB(X) (OpenNE::UInt64)((OpenNE::UInt64)KIB(X) / 1024)
#endif

#ifndef mib_cast
#define mib_cast(X) (OpenNE::UInt64)((OpenNE::UInt64)kib_cast(X) * 1024)
#endif

#ifndef GIB
#define GIB(X) (OpenNE::UInt64)((OpenNE::UInt64)MIB(X) / 1024)
#endif

#ifndef gib_cast
#define gib_cast(X) (OpenNE::UInt64)((OpenNE::UInt64)mib_cast(X) * 1024)
#endif

#ifndef TIB
#define TIB(X) (OpenNE::UInt64)((OpenNE::UInt64)GIB(X) / 1024)
#endif

#ifndef tib_cast
#define tib_cast(X) ((OpenNE::UInt64)gib_cast(X) * 1024)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)              \
	(((sizeof(a) / sizeof(*(a))) / \
	  (static_cast<OpenNE::Size>(!(sizeof(a) % sizeof(*(a)))))))
#endif

#define DEPRECATED ATTRIBUTE(deprecated)

#ifndef ALIGN
#define ALIGN(X) __attribute__((aligned(X)))
#endif // #ifndef ALIGN

#ifndef ATTRIBUTE
#define ATTRIBUTE(X) __attribute__((X))
#endif // #ifndef ATTRIBUTE

#ifndef __OPENNE_VER__
#define __OPENNE_VER__ (2024)
#endif // !__OPENNE_VER__

#ifndef EXTERN
#define EXTERN extern
#endif

#ifndef EXTERN_C
#define EXTERN_C extern "C"
#endif

#ifndef MAKE_ENUM
#define MAKE_ENUM(NAME) \
	enum NAME           \
	{
#endif

#ifndef END_ENUM
#define END_ENUM() \
	}              \
	;
#endif

#ifndef MAKE_STRING_ENUM
#define MAKE_STRING_ENUM(NAME) \
	namespace NAME             \
	{
#endif

#ifndef ENUM_STRING
#define ENUM_STRING(NAME, VAL) inline constexpr const char* e##NAME = VAL
#endif

#ifndef END_STRING_ENUM
#define END_STRING_ENUM() }
#endif

#ifndef rtl_alloca
#define rtl_alloca(sz) __builtin_alloca(sz)
#endif // #ifndef rtl_alloca

#ifndef CANT_REACH
#define CANT_REACH() __builtin_unreachable()
#endif

#define kInvalidAddress 0xFBFBFBFBFBFBFBFB
#define kBadAddress		0x0000000000000000
#define kMaxAddr		0xFFFFFFFFFFFFFFFF
#define kPathLen		0x100

#define PACKED	ATTRIBUTE(packed)
#define NO_EXEC ATTRIBUTE(noexec)

#define EXTERN extern
#define STATIC static

#define CONST const

#define STRINGIFY(X)	 #X
#define OPENNE_UNUSED(X) ((OpenNE::Void)X)

#ifndef RGB
#define RGB(R, G, B) (OpenNE::UInt32)(R | G << 0x8 | B << 0x10)
#endif // !RGB

#ifdef __OPENNE_AMD64__
#define dbg_break_point() asm volatile("int $3")
#else
#define dbg_break_point() ((void)0)
#endif

/// @brief A simple try and die macro.
#define MUST_TRY(EXPR)     \
	if (!(EXPR))           \
	{                      \
		MUST_PASS(EXPR);   \
		dbg_break_point(); \
	}

#define RTL_ENDIAN(address, value)                              \
	(((reinterpret_cast<OpenNE::Char*>(address)[0]) == (value)) \
		 ? (OpenNE::Endian::kEndianBig)                         \
		 : (OpenNE::Endian::kEndianLittle))

#define Yes true
#define No	false

#define YES true
#define NO	false

#define TRUE  true
#define FALSE false

#define BOOL OpenNE::Boolean

#ifdef RTL_INIT_OBJECT
#undef RTL_INIT_OBJECT
#endif // ifdef RTL_INIT_OBJECT

#define RTL_INIT_OBJECT(OBJ, TYPE, ...) TYPE OBJ = TYPE(__VA_ARGS__)
