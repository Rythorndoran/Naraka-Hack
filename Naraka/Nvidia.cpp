#include "Nvidia.h"
#include <comdef.h>
#include <corecrt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)

static HWND win;

/*
Window functions
*/

auto Nvidia::window_set_style() -> void {
    int i = 0;

    i = (int)GetWindowLong(win, -20);

    SetWindowLongPtr(win, -20, (LONG_PTR)(i | 0x20));
}

auto Nvidia::window_set_transparency() -> void {
    MARGINS margin;
    UINT opacity_flag, color_key_flag, color_key = 0;
    UINT opacity = 0;

    margin.cyBottomHeight = -1;
    margin.cxLeftWidth = -1;
    margin.cxRightWidth = -1;
    margin.cyTopHeight = -1;

    DwmExtendFrameIntoClientArea(win, &margin);

    opacity_flag = 0x02;
    color_key_flag = 0x01;
    color_key = 0x000000;
    opacity = 0xFF;

    SetLayeredWindowAttributes(win, color_key, opacity, opacity_flag);
}

auto Nvidia::window_set_top_most() -> void {
    SetWindowPos(win, HWND_TOPMOST, 0, 0, 0, 0, 0x0002 | 0x0001);
}

auto Nvidia::retrieve_window() -> HWND { return win; }

// Hijacking method down here.

auto Nvidia::window_init() -> BOOL {
    win = FindWindowW(L"CEF-OSC-WIDGET", L"NVIDIA GeForce Overlay");
    if (!win)
        return FALSE;

    Nvidia::window_set_style();
    Nvidia::window_set_transparency();
    Nvidia::window_set_top_most();

    ShowWindow(win, SW_SHOW);

    return TRUE;
}

/*
Overlay functions
*/

ID2D1Factory* d2d_factory;
ID2D1HwndRenderTarget* tar;
IDWriteFactory* write_factory;
ID2D1SolidColorBrush* brush;
ID2D1SolidColorBrush* blue_brush;
ID2D1SolidColorBrush* purple_brush;
ID2D1SolidColorBrush* red_brush;
ID2D1SolidColorBrush* green_brush;
ID2D1SolidColorBrush* yellow_brush;
IDWriteTextFormat* format;

auto Nvidia::d2d_shutdown() -> void {
    // Release
    tar->Release();
    write_factory->Release();
    brush->Release();
    red_brush->Release();
    blue_brush->Release();
    green_brush->Release();
    yellow_brush->Release();
    purple_brush->Release();
    d2d_factory->Release();
}

auto Nvidia::init_d2d() -> BOOL {
    HRESULT ret;
    RECT rc;

    // Initialize D2D here
    ret = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);
    if (FAILED(ret))
        return FALSE;

    ret =
        DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
            (IUnknown**)(&write_factory));
    if (FAILED(ret))
        return FALSE;

    write_factory->CreateTextFormat(
        L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, 14.0, L"zh-cn", &format);

    GetClientRect(Nvidia::retrieve_window(), &rc);

    ret = d2d_factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,
                D2D1_ALPHA_MODE_PREMULTIPLIED)),
        D2D1::HwndRenderTargetProperties(
            Nvidia::retrieve_window(),
            D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
        &tar);
    if (FAILED(ret))
        return FALSE;

    tar->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
    tar->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &red_brush);
    tar->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LimeGreen), &green_brush);
    tar->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &blue_brush);
    tar->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &yellow_brush);
    tar->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple), &purple_brush);

    return TRUE;
}

auto Nvidia::begin_scene() -> void { tar->BeginDraw(); }

auto Nvidia::end_scene() -> void { tar->EndDraw(); }

auto Nvidia::clear_scene() -> void { tar->Clear(); }

auto Nvidia::draw_text_white(int x, int y, const wchar_t* str, ...) -> void {
    int len = 0;
    wchar_t buf[256];

    // if (!draw) // no need for it.
    //	 return;

    va_list arg_list;
    va_start(arg_list, str);
    wvsprintf(buf, str, arg_list);
    va_end(arg_list);
    len = wcslen(buf);

    tar->DrawText(buf, len, format, D2D1::RectF(x, y, 1920, 1080), brush,
        D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}

auto Nvidia::draw_text_purple(int x, int y, const wchar_t* str, ...) -> void {
    int len = 0;
    wchar_t buf[256];

    // if (!draw) // no need for it.
    //	 return;

    va_list arg_list;
    va_start(arg_list, str);
    wvsprintf(buf, str, arg_list);
    va_end(arg_list);
    len = wcslen(buf);

    tar->DrawText(buf, len, format, D2D1::RectF(x, y, 1920, 1080), purple_brush,
        D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}

auto Nvidia::draw_text_blue(int x, int y, const wchar_t* str, ...) -> void {
    int len = 0;
    wchar_t buf[256];

    // if (!draw) // no need for it.
    //	 return;

    va_list arg_list;
    va_start(arg_list, str);
    wvsprintf(buf, str, arg_list);
    va_end(arg_list);
    len = wcslen(buf);

    tar->DrawText(buf, len, format, D2D1::RectF(x, y, 1920, 1080), blue_brush,
        D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}


auto Nvidia::draw_text_red(int x, int y, const wchar_t* str, ...) -> void {
    int len = 0;
    wchar_t buf[256];

    // if (!draw) // no need for it.
    //	 return;

    va_list arg_list;
    va_start(arg_list, str);
    wvsprintf(buf, str, arg_list);
    va_end(arg_list);
    len = wcslen(buf);

    tar->DrawText(buf, len, format, D2D1::RectF(x, y, 1920, 1080), red_brush,
        D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}

auto Nvidia::draw_text_green(int x, int y, const wchar_t* str, ...) -> void {
    int len = 0;
    wchar_t buf[256];

    // if (!draw) // no need for it.
    //	 return;

    va_list arg_list;
    va_start(arg_list, str);
    wvsprintf(buf, str, arg_list);
    va_end(arg_list);
    len = wcslen(buf);

    tar->DrawText(buf, len, format, D2D1::RectF(x, y, 1920, 1080), green_brush,
        D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}

auto Nvidia::draw_text_yellow(int x, int y, const wchar_t* str, ...) -> void {
    int len = 0;
    wchar_t buf[256];

    // if (!draw) // no need for it.
    //	 return;

    va_list arg_list;
    va_start(arg_list, str);
    wvsprintf(buf, str, arg_list);
    va_end(arg_list);
    len = wcslen(buf);

    tar->DrawText(buf, len, format, D2D1::RectF(x, y, 1920, 1080), yellow_brush,
        D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}

auto Nvidia::draw_line(int x, int y, int x2, int y2, float thickness, ...)-> void {
    tar->DrawLine(D2D1::Point2F(x, y), D2D1::Point2F(x2, y2), red_brush, thickness);
}

auto Nvidia::draw_rect(float left, float top, float right, float bottom, float thickness) -> void{
    tar->DrawRectangle(D2D1::RectF(left,top, left + right, top + bottom), red_brush,thickness);
}
 
auto Nvidia::clear_screen() -> void {
    Nvidia::begin_scene();
    Nvidia::clear_scene();
    Nvidia::end_scene();
}