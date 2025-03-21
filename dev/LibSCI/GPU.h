/* -------------------------------------------

Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

File: GPU.h
Purpose: GFX System Calls.

------------------------------------------- */

#ifndef SCIKIT_GPU_H
#define SCIKIT_GPU_H

#include <SCI.h>

struct GPUCmdBuffer;

/// ------------------------------------------------------------------------------------------ //
/// @brief GPU API.
/// ------------------------------------------------------------------------------------------ //

/// ------------------------------------------------------------------------------------------ //
/// @brief Command buffer structure type.
/// ------------------------------------------------------------------------------------------ //
struct GPUCmdBuffer final
{
	VoidPtr FrameData;
	SizeT	FrameDataSz;
	SizeT	FrameLayer;
	Bool	IsGPGPUData;
};

typedef VoidPtr GPUObject;

IMPORT_C GPUObject GPUNewFromDeviceName(_Input const Char* device_name);

IMPORT_C SInt32 GPUDisposeDevice(GPUObject gpu_handle, Bool cancel_all, Bool flush_all);

IMPORT_C SInt32 GPUSendCmdBufferListWithCnt(GPUCmdBuffer** cmd_list, SizeT cmd_list_cnt);

#endif // ifndef SCIKIT_GPU_H
