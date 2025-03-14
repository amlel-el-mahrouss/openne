/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#include <ArchKit/ArchKit.h>
#include <KernelKit/DebugOutput.h>
#include <NewKit/Utils.h>
#include <NewKit/New.h>
#include <Mod/CoreGfx/FBMgr.h>
#include <Mod/CoreGfx/TextMgr.h>

namespace OpenNE
{
	enum CommStatus
	{
		kStateInvalid,
		kStateReady	   = 0xCF,
		kStateTransmit = 0xFC,
		kStateCnt	   = 3
	};

	namespace Detail
	{
		constexpr Int16 kPort  = 0x3F8;
		static Int32	kState = kStateInvalid;

		/// @brief Init COM1.
		/// @return
		template <Int16 PORT>
		bool hal_serial_init() noexcept
		{
			if (kState == kStateReady || kState == kStateTransmit)
				return true;

			HAL::rt_out8(PORT + 1, 0x00); // Disable all interrupts
			HAL::rt_out8(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
			HAL::rt_out8(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
			HAL::rt_out8(PORT + 1, 0x00); //                  (hi byte)
			HAL::rt_out8(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
			HAL::rt_out8(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
			HAL::rt_out8(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
			HAL::rt_out8(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
			HAL::rt_out8(PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if
										  // serial returns same byte)

			// Check if serial is faulty (i.e: not same byte as sent)
			if (HAL::rt_in8(PORT) != 0xAE)
			{
				ke_panic(RUNTIME_CHECK_HANDSHAKE);
			}

			kState = kStateReady;

			// If serial is not faulty set it in normal operation mode
			// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
			HAL::rt_out8(PORT + 4, 0x0F);

			return true;
		}
	} // namespace Detail

	TerminalDevice::~TerminalDevice() = default;

	EXTERN_C void ke_io_write(const Char* bytes)
	{
#ifdef __DEBUG__
		Detail::hal_serial_init<Detail::kPort>();

		if (!bytes || Detail::kState != kStateReady)
			return;

		if (*bytes == 0)
			return;

		Detail::kState = kStateTransmit;

		SizeT index = 0;
		SizeT len	= 0;

		index = 0;
		len	  = rt_string_len(bytes, 255);

		static int x = kFontSizeX, y = kFontSizeY;

		static BOOL important = YES;

		while (index < len)
		{
			if (bytes[index] == '\r')
				HAL::rt_out8(Detail::kPort, '\r');

			HAL::rt_out8(Detail::kPort, bytes[index] == '\r' ? '\n' : bytes[index]);

			char tmp_str[2];
			tmp_str[0] = bytes[index];
			tmp_str[1] = 0;

			if (bytes[index] == '*')
			{
				if (important)
					important = NO;
				else
					important = YES;

				++index;

				continue;
			}

			fb_render_string(tmp_str, y, x, (!important) ? RGB(0xff, 0xff, 0xff) : RGB(0x00, 0x00, 0xff));

			if (bytes[index] == '\r')
			{
				y += kFontSizeY;
				x = kFontSizeX;
			}

			x += kFontSizeX;

			if (y > kHandoverHeader->f_GOP.f_Height)
			{
				y = kFontSizeY;

				fb_init();

				FBDrawInRegion(fb_get_clear_clr(), FB::UIAccessibilty::Height(), FB::UIAccessibilty::Width(),
							   0, 0);

				fb_clear();
			}

			++index;
		}

		Detail::kState = kStateReady;
#endif // __DEBUG__
	}

	EXTERN_C void ke_io_read(const Char* bytes)
	{
#ifdef __DEBUG__
		Detail::hal_serial_init<Detail::kPort>();

		if (!bytes || Detail::kState != kStateReady)
			return;

		Detail::kState = kStateTransmit;

		SizeT index = 0;

		///! TODO: Look on how to wait for the UART to complete.
		while (true)
		{
			auto in = HAL::rt_in8(Detail::kPort);

			///! If enter pressed then break.
			if (in == 0xD)
			{
				break;
			}

			if (in < '0' || in < 'A' || in < 'a')
			{
				if (in != '@' || in != '!' || in != '?' || in != '.' || in != '/' ||
					in != ':')
				{
					continue;
				}
			}

			((char*)bytes)[index] = in;

			++index;
		}

		((char*)bytes)[index] = 0;

		Detail::kState = kStateReady;
#endif // __DEBUG__
	}

	TerminalDevice TerminalDevice::The() noexcept
	{
		TerminalDevice out(OpenNE::ke_io_write, OpenNE::ke_io_read);
		return out;
	}

} // namespace OpenNE