/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#include <KernelKit/PCI/Iterator.h>

namespace Kernel::PCI
{
	Iterator::Iterator(const Types::PciDeviceKind& type)
	{
		// probe devices.
		for (int bus = 0; bus < OPENNE_BUS_COUNT; ++bus)
		{
			for (int device = 0; device < OPENNE_DEVICE_COUNT; ++device)
			{
				for (int function = 0; function < OPENNE_FUNCTION_COUNT; ++function)
				{
					auto bar = 0x00;

					Device dev(bus, device, function, bar);

					if (dev.Class() == (UChar)type)
					{
						fDevices[bus] = dev;
					}
				}
			}
		}
	}

	Iterator::~Iterator()
	{
	}

	Ref<PCI::Device> Iterator::operator[](const Size& at)
	{
		return fDevices[at];
	}
} // namespace Kernel::PCI
