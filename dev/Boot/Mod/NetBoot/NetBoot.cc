/*
 *	========================================================
 *
 *	NetBoot
 * 	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.
 *
 * 	========================================================
 */

#include <Mod/NetBoot/NetBoot.h>
#include <BootKit/BootKit.h>

EXTERN_C Int32 ModuleMain(OpenNE::HEL::BootInfoHeader* handover)
{
#ifdef __OPENNE_AMD64__
	return kEfiFail;
#else
	return kEfiOk;
#endif
}
