#pragma once

// <( headers )>
#include <string>
#include <cstdint>

// <( windows )>
#include <Windows.h>

namespace console
{

	
	// < struct >
	struct color_t
	{
		std::uint8_t r{};
		std::uint8_t g{};
		std::uint8_t b{};
	};

	struct rect_t
	{
		short left{};
		short top{};
		short right{};
		short bottom{};
	};

	struct font_t
	{
		std::wstring face_name{ L"Consolas" };
		short weight{ static_cast<short>(FW_NORMAL) };
		short size_x{ 0 };
		short size_y{ 16 };
	};

	struct cursor_info_t
	{
		std::uint32_t size{ 25 };
		bool visible{ true };
	};

	struct window_info_t
	{
		std::string title{};
		rect_t window_rect{};
		rect_t buffer_rect{};
		COORD buffer_size{};
		COORD cursor_pos{};
		cursor_info_t cursor{};
		WORD attributes{};
		COORD maximum_window_size{};
		std::uint32_t code_page{};
	};

	// < setup >
	void setup();
	void shutdown();

	// < other shit >
	void set_size(short width, short height);
	void set_window_size(short width, short height);
	void set_buffer_size(short width, short height);
	void set_position(short x, short y);
	void set_title(const std::string& title);
	void set_title(const std::wstring& title);
	void set_icon(HICON icon);

	void set_font(const font_t& font);
	void set_font(const std::wstring& face_name, short size_y, short weight = FW_NORMAL);

	void set_transparency_alpha(std::uint8_t alpha);

	void set_code_page(std::uint32_t cp);
	void set_mode(DWORD mode);
	void set_output_mode(DWORD mode);
	void set_input_mode(DWORD mode);

	void set_fullscreen(bool enabled);
	void set_console_window(HWND hwnd);
	void set_resizable(bool enabled);
	void set_scrollable(bool enabled);

	void clear();
	void clear(color_t background);

	color_t get_foreground_color();
	color_t get_background_color();

	// < colors >
	void set_foreground_color(std::uint8_t r, std::uint8_t g, std::uint8_t b);
	void set_background_color(std::uint8_t r, std::uint8_t g, std::uint8_t b);

	void set_color(WORD attributes);
	void set_color(color_t foreground, color_t background);
	void set_foreground_color(color_t color);
	void set_background_color(color_t color);

	void move_window(short x, short y, short width, short height);

	// < center >
	void center_window();
	void center_window(short width, short height);

	// < flash >
	void flash_window();
	void flash_window(std::uint32_t count, bool invert);

	// < cursor >
	void show_cursor();
	void hide_cursor();

	void set_cursor_size(std::uint32_t size);
	void set_cursor_visible(bool visible);
	void set_cursor_position(short x, short y);

	// < getting >
	COORD get_cursor_position();
	WORD get_attributes();
	std::uint32_t get_code_page();
	window_info_t get_window_info();
	HWND get_window_handle();
	HANDLE get_output_handle();
	HANDLE get_input_handle();

	bool is_fullscreen();

} // console
