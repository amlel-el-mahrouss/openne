/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <Mod/AHCI/AHCI.h>

class BootDeviceSATA final
{
public:
	explicit BootDeviceSATA() noexcept;
	~BootDeviceSATA() = default;

	OPENNE_COPY_DEFAULT(BootDeviceSATA);

	struct SATATrait final
	{
		OpenNE::SizeT	mBase{1024};
		OpenNE::Boolean mErr{false};
		OpenNE::Boolean mDetected{false};

		operator bool()
		{
			return !this->mErr;
		}
	};

	operator bool()
	{
		return this->Leak().mDetected;
	}

	BootDeviceSATA& Read(OpenNE::WideChar* Buf, const OpenNE::SizeT& SecCount);
	BootDeviceSATA& Write(OpenNE::WideChar* Buf, const OpenNE::SizeT& SecCount);

	SATATrait& Leak();

private:
	SATATrait mTrait;
};

#define kAHCISectorSz 4096
