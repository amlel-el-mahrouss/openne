/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

namespace OpenNE
{
	template <class T>
	T* DMAWrapper::operator->()
	{
		return fAddress;
	}

	template <class T>
	T* DMAWrapper::Get(const UIntPtr offset)
	{
		return reinterpret_cast<T*>((UIntPtr)fAddress + offset);
	}
} // namespace OpenNE
