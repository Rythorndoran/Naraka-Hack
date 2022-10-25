#pragma once
#include <windows.h>
#include <stdio.h>
#include <dwmapi.h> 
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "Dwrite")

#pragma comment(lib, "Dwmapi.lib") 
#pragma comment(lib, "d2d1.lib")

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

class Nvidia
{
public:
	auto window_set_style()-> void;
	auto window_set_transparency()-> void;
	auto window_set_top_most()-> void;
	auto retrieve_window()->HWND;
	auto window_init()->BOOL;
	auto d2d_shutdown()-> void;
	auto init_d2d()->BOOL;
	auto begin_scene()-> void;
	auto end_scene()-> void;
	auto clear_scene()-> void;
	auto draw_text_white(int x, int y, const wchar_t* str, ...)-> void;
	auto draw_text_purple(int x, int y, const wchar_t* str, ...) -> void;
	auto draw_text_blue(int x, int y, const wchar_t* str, ...) -> void;
	auto draw_text_red(int x, int y, const wchar_t* str, ...)-> void;
	auto draw_text_green(int x, int y, const wchar_t* str, ...) -> void;
	auto draw_text_yellow(int x, int y, const wchar_t* str, ...) -> void;
	auto draw_line(int x, int y, int x2, int y2, float thickness, ...) -> void;
	auto draw_rect(float left, float top, float right, float bottom, float thickness) ->void;
	auto clear_screen()-> void;
};