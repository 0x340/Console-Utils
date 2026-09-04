#include "console.hpp"

// <( windows )>

namespace console
{
	namespace detail
	{
		// < state >
		static HANDLE output_handle{ GetStdHandle(STD_OUTPUT_HANDLE) };
		static HANDLE input_handle{ GetStdHandle(STD_INPUT_HANDLE) };
		static HWND window_handle{ GetConsoleWindow() };
		static bool fullscreen{ false };
	}

	void setup()
	{
		detail::output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		detail::input_handle = GetStdHandle(STD_INPUT_HANDLE);
		detail::window_handle = GetConsoleWindow();
		//
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);

		DWORD mode{};

		if (GetConsoleMode(detail::input_handle, &mode))
		{
			mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
			//
			SetConsoleMode(detail::input_handle, mode);
		}
		//
		if (GetConsoleMode(detail::output_handle, &mode))
		{
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			mode |= DISABLE_NEWLINE_AUTO_RETURN;
			//
			SetConsoleMode(detail::output_handle, mode);
		}
	}

	void shutdown()
	{
		detail::output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		detail::input_handle = GetStdHandle(STD_INPUT_HANDLE);
		detail::window_handle = GetConsoleWindow();
	}

	void set_size(short width, short height)
	{
		COORD size{};
		size.X = width;
		size.Y = height;
		//
		SMALL_RECT rect{};
		rect.Left = 0;
		rect.Top = 0;
		rect.Right = static_cast<short>(width - 1);
		rect.Bottom = static_cast<short>(height - 1);
		//
		SetConsoleScreenBufferSize(detail::output_handle, size);
		SetConsoleWindowInfo(detail::output_handle, TRUE, &rect);
	}

	void set_window_size(short width, short height)
	{
		SMALL_RECT rect{};
		rect.Left = 0;
		rect.Top = 0;
		rect.Right = static_cast<short>(width - 1);
		rect.Bottom = static_cast<short>(height - 1);
		//
		SetConsoleWindowInfo(detail::output_handle, TRUE, &rect);
	}

	void set_buffer_size(short width, short height)
	{
		COORD size{};
		size.X = width;
		size.Y = height;
		//
		SetConsoleScreenBufferSize(detail::output_handle, size);
	}

	void set_position(short x, short y)
	{
		SetWindowPos(detail::window_handle, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void set_title(const std::string& title)
	{
		SetConsoleTitleA(title.c_str());
	}

	void set_title(const std::wstring& title)
	{
		SetConsoleTitleW(title.c_str());
	}

	void set_icon(HICON icon)
	{
		SendMessage(detail::window_handle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
		SendMessage(detail::window_handle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
	}

	void set_font(const font_t& font)
	{
		CONSOLE_FONT_INFOEX font_info{};
		font_info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		font_info.nFont = 0;
		font_info.dwFontSize.X = font.size_x;
		font_info.dwFontSize.Y = font.size_y;
		font_info.FontWeight = font.weight;
		//
		wcscpy_s(font_info.FaceName, font.face_name.c_str());
		//
		SetCurrentConsoleFontEx(detail::output_handle, FALSE, &font_info);
	}

	void set_font(const std::wstring& face_name, short size_y, short weight)
	{
		font_t font{};
		font.face_name = face_name;
		font.size_y = size_y;
		font.weight = weight;
		//
		set_font(font);
	}

	void set_transparency_alpha(std::uint8_t alpha)
	{
		SetWindowLong(detail::window_handle, GWL_EXSTYLE, GetWindowLong(detail::window_handle, GWL_EXSTYLE) | WS_EX_LAYERED);
		//
		SetLayeredWindowAttributes(detail::window_handle, 0, alpha, LWA_ALPHA);
	}

	void set_cursor_size(std::uint32_t size)
	{
		CONSOLE_CURSOR_INFO cursor_info{};
		cursor_info.dwSize = static_cast<DWORD>(size);
		cursor_info.bVisible = TRUE;
		//
		SetConsoleCursorInfo(detail::output_handle, &cursor_info);
	}

	void set_cursor_visible(bool visible)
	{
		CONSOLE_CURSOR_INFO cursor_info{};
		cursor_info.dwSize = 25;
		cursor_info.bVisible = visible ? TRUE : FALSE;
		//
		SetConsoleCursorInfo(detail::output_handle, &cursor_info);
	}

	void set_cursor_position(short x, short y)
	{
		COORD pos{};
		pos.X = x;
		pos.Y = y;
		//
		SetConsoleCursorPosition(detail::output_handle, pos);
	}

	void set_color(WORD attributes)
	{
		SetConsoleTextAttribute(detail::output_handle, attributes);
	}

	void set_color(color_t foreground, color_t background)
	{
		WORD fg_attr{};

		if (foreground.r > 127)
		{
			fg_attr |= FOREGROUND_RED;
		}
		//
		if (foreground.g > 127)
		{
			fg_attr |= FOREGROUND_GREEN;
		}
		//
		if (foreground.b > 127)
		{
			fg_attr |= FOREGROUND_BLUE;
		}

		WORD bg_attr{};

		if (background.r > 127)
		{
			bg_attr |= BACKGROUND_RED;
		}
		//
		if (background.g > 127)
		{
			bg_attr |= BACKGROUND_GREEN;
		}
		//
		if (background.b > 127)
		{
			bg_attr |= BACKGROUND_BLUE;
		}
		//
		SetConsoleTextAttribute(detail::output_handle, fg_attr | bg_attr);
	}

	void set_foreground_color(color_t color)
	{
		WORD attr{};

		if (color.r > 127)
		{
			attr |= FOREGROUND_RED;
		}
		//
		if (color.g > 127)
		{
			attr |= FOREGROUND_GREEN;
		}
		//
		if (color.b > 127)
		{
			attr |= FOREGROUND_BLUE;
		}

		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		SetConsoleTextAttribute(detail::output_handle, (csbi.wAttributes & 0xF0) | attr);
	}

	void set_background_color(color_t color)
	{
		WORD attr{};

		if (color.r > 127)
		{
			attr |= BACKGROUND_RED;
		}
		//
		if (color.g > 127)
		{
			attr |= BACKGROUND_GREEN;
		}
		//
		if (color.b > 127)
		{
			attr |= BACKGROUND_BLUE;
		}

		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		SetConsoleTextAttribute(detail::output_handle, (csbi.wAttributes & 0x0F) | attr);
	}

	void set_foreground_color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
	{
		set_foreground_color({ r, g, b });
	}

	void set_background_color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
	{
		set_background_color({ r, g, b });
	}

	void set_code_page(std::uint32_t cp)
	{
		SetConsoleCP(static_cast<UINT>(cp));
		SetConsoleOutputCP(static_cast<UINT>(cp));
	}

	void set_mode(DWORD mode)
	{
		SetConsoleMode(detail::output_handle, mode);
	}

	void set_output_mode(DWORD mode)
	{
		SetConsoleMode(detail::output_handle, mode);
	}

	void set_input_mode(DWORD mode)
	{
		SetConsoleMode(detail::input_handle, mode);
	}

	void set_fullscreen(bool enabled)
	{
		if (enabled)
		{
			COORD largest{ GetLargestConsoleWindowSize(detail::output_handle) };
			//
			SMALL_RECT rect{};
			rect.Left = 0;
			rect.Top = 0;
			rect.Right = static_cast<short>(largest.X - 1);
			rect.Bottom = static_cast<short>(largest.Y - 1);
			//
			SetConsoleScreenBufferSize(detail::output_handle, largest);
			SetConsoleWindowInfo(detail::output_handle, TRUE, &rect);
		}
		else
		{
			set_size(120, 30);
		}
		//
		detail::fullscreen = enabled;
	}

	void set_console_window(HWND hwnd)
	{
		detail::window_handle = hwnd;
	}

	void set_resizable(bool enabled)
	{
		HMENU h_menu{ GetSystemMenu(detail::window_handle, FALSE) };

		if (h_menu)
		{
			if (!enabled)
			{
				RemoveMenu(h_menu, SC_SIZE, MF_BYCOMMAND);
				RemoveMenu(h_menu, SC_MAXIMIZE, MF_BYCOMMAND);
			}
			else
			{
				AppendMenu(h_menu, MF_SEPARATOR, 0, nullptr);
				AppendMenu(h_menu, MF_STRING, SC_SIZE, L"Size");
				AppendMenu(h_menu, MF_STRING, SC_MAXIMIZE, L"Maximize");
			}
		}
	}

	void set_scrollable(bool enabled)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		COORD buffer_size{};
		buffer_size.X = static_cast<short>(csbi.srWindow.Right - csbi.srWindow.Left + 1);

		if (!enabled)
		{
			buffer_size.Y = static_cast<short>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
		}
		else
		{
			buffer_size.Y = 9999;
		}
		//
		SetConsoleScreenBufferSize(detail::output_handle, buffer_size);
	}

	void clear()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		DWORD written{};
		COORD origin{};
		origin.X = 0;
		origin.Y = 0;
		//
		FillConsoleOutputCharacterW(detail::output_handle, L' ', csbi.dwSize.X * csbi.dwSize.Y, origin, &written);
		FillConsoleOutputAttribute(detail::output_handle, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, origin, &written);
		//
		SetConsoleCursorPosition(detail::output_handle, origin);
	}

	void clear(color_t background)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		WORD attr{};

		if (background.r > 127)
		{
			attr |= BACKGROUND_RED;
		}
		//
		if (background.g > 127)
		{
			attr |= BACKGROUND_GREEN;
		}
		//
		if (background.b > 127)
		{
			attr |= BACKGROUND_BLUE;
		}

		DWORD written{};
		COORD origin{};
		origin.X = 0;
		origin.Y = 0;
		//
		FillConsoleOutputCharacterW(detail::output_handle, L' ', csbi.dwSize.X * csbi.dwSize.Y, origin, &written);
		FillConsoleOutputAttribute(detail::output_handle, attr, csbi.dwSize.X * csbi.dwSize.Y, origin, &written);
		//
		SetConsoleCursorPosition(detail::output_handle, origin);
	}

	COORD get_cursor_position()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		return { csbi.dwCursorPosition };
	}

	WORD get_attributes()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		return { csbi.wAttributes };
	}

	std::uint32_t get_code_page()
	{
		return { GetConsoleOutputCP() };
	}

	window_info_t get_window_info()
	{
		window_info_t info{};
		//
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		info.buffer_size = csbi.dwSize;
		info.cursor_pos = csbi.dwCursorPosition;
		info.attributes = csbi.wAttributes;
		info.maximum_window_size = csbi.dwMaximumWindowSize;
		//
		info.window_rect.left = csbi.srWindow.Left;
		info.window_rect.top = csbi.srWindow.Top;
		info.window_rect.right = csbi.srWindow.Right;
		info.window_rect.bottom = csbi.srWindow.Bottom;
		//
		info.buffer_rect.left = 0;
		info.buffer_rect.top = 0;
		info.buffer_rect.right = static_cast<short>(csbi.dwSize.X - 1);
		info.buffer_rect.bottom = static_cast<short>(csbi.dwSize.Y - 1);
		//
		CONSOLE_CURSOR_INFO cursor_info{};
		GetConsoleCursorInfo(detail::output_handle, &cursor_info);
		//
		info.cursor.size = cursor_info.dwSize;
		info.cursor.visible = cursor_info.bVisible != FALSE;
		//
		info.code_page = GetConsoleOutputCP();
		//
		char title_buf[256]{};
		GetConsoleTitleA(title_buf, sizeof(title_buf));
		info.title = title_buf;
		//
		return { info };
	}

	HWND get_window_handle()
	{
		return { detail::window_handle };
	}

	HANDLE get_output_handle()
	{
		return { detail::output_handle };
	}

	HANDLE get_input_handle()
	{
		return { detail::input_handle };
	}

	bool is_fullscreen()
	{
		return { detail::fullscreen };
	}

	color_t get_foreground_color()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		WORD attr{ static_cast<WORD>(csbi.wAttributes & 0x0F) };
		//
		color_t color{};
		color.r = (attr & FOREGROUND_RED) ? 255 : 0;
		color.g = (attr & FOREGROUND_GREEN) ? 255 : 0;
		color.b = (attr & FOREGROUND_BLUE) ? 255 : 0;
		//
		return { color };
	}

	color_t get_background_color()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		GetConsoleScreenBufferInfo(detail::output_handle, &csbi);
		//
		WORD attr{ static_cast<WORD>((csbi.wAttributes & 0xF0) >> 4) };
		//
		color_t color{};
		color.r = (attr & BACKGROUND_RED) ? 255 : 0;
		color.g = (attr & BACKGROUND_GREEN) ? 255 : 0;
		color.b = (attr & BACKGROUND_BLUE) ? 255 : 0;
		//
		return { color };
	}

	void move_window(short x, short y, short width, short height)
	{
		SetWindowPos(detail::window_handle, nullptr, x, y, width, height, SWP_NOZORDER);
	}

	void center_window()
	{
		RECT screen_rect{};
		GetWindowRect(GetDesktopWindow(), &screen_rect);
		//
		RECT window_rect{};
		GetWindowRect(detail::window_handle, &window_rect);
		//
		short x{ static_cast<short>((screen_rect.right - (window_rect.right - window_rect.left)) / 2) };
		short y{ static_cast<short>((screen_rect.bottom - (window_rect.bottom - window_rect.top)) / 2) };
		//
		SetWindowPos(detail::window_handle, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void center_window(short width, short height)
	{
		RECT screen_rect{};
		GetWindowRect(GetDesktopWindow(), &screen_rect);
		//
		short x{ static_cast<short>((screen_rect.right - width) / 2) };
		short y{ static_cast<short>((screen_rect.bottom - height) / 2) };
		//
		SetWindowPos(detail::window_handle, nullptr, x, y, width, height, SWP_NOZORDER);
	}

	void flash_window()
	{
		FlashWindow(detail::window_handle, TRUE);
	}

	void flash_window(std::uint32_t count, bool invert)
	{
		FLASHWINFO fwi{};
		fwi.cbSize = sizeof(FLASHWINFO);
		fwi.hwnd = detail::window_handle;
		fwi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		fwi.uCount = static_cast<UINT>(count);
		fwi.dwTimeout = 0;
		//
		FlashWindowEx(&fwi);
	}

	void show_cursor()
	{
		set_cursor_visible(true);
	}

	void hide_cursor()
	{
		set_cursor_visible(false);
	}

} // console
