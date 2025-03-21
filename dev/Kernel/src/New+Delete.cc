/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#include <KernelKit/MemoryMgr.h>
#include <NewKit/New.h>

void* operator new[](size_t sz)
{
	if (sz == 0)
		++sz;

	return OpenNE::mm_new_heap(sz, true, false);
}

void* operator new(size_t sz)
{
	if (sz == 0)
		++sz;

	return OpenNE::mm_new_heap(sz, true, false);
}

void operator delete[](void* ptr)
{
	if (ptr == nullptr)
		return;

	OpenNE::mm_delete_heap(ptr);
}

void operator delete(void* ptr)
{
	if (ptr == nullptr)
		return;

	OpenNE::mm_delete_heap(ptr);
}

void operator delete(void* ptr, size_t sz)
{
	if (ptr == nullptr)
		return;

	OPENNE_UNUSED(sz);

	OpenNE::mm_delete_heap(ptr);
}
