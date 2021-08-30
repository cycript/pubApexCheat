#pragma once
#include "XorStr.h"
#include "imgui/imgui.h"
#include "imgui/imgui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#include <string>
#include <thread>
#include "ApexLegends.h"
#include "CFG.h"
#include "Maths.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include "HTTPDownloader.hpp"
#include <curl.h>
#include "resource1.h"
#include <sstream>






inline string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

#define GREEN ImColor(0, 255, 0)
#define RED ImColor(255, 0, 0)
#define BLUE ImColor(0, 0, 255)
#define ORANGE ImColor(255, 165, 0)
#define WHITE ImColor(255, 255, 255)

static ImGuiStyle imGuiStyle;
static bool fontIsInstalled = false;
static int currentMenu = 0;
static bool pushCalled = false;
static bool executedOnce = false;


typedef struct visuals
{
	bool box = true;
	bool line = true;
	bool distance = true;
	bool healthbar = true;
	bool shieldbar = true;
	bool name = true;
}visuals;










class Overlay
{
public:
	LONG nv_default = WS_POPUP | WS_CLIPSIBLINGS;
	LONG nv_default_in_game = nv_default | WS_DISABLED;
	LONG nv_edit = nv_default_in_game | WS_VISIBLE;
	LONG nv_ex_default = WS_EX_TOOLWINDOW;
	LONG nv_ex_edit = nv_ex_default | WS_EX_LAYERED | WS_EX_TRANSPARENT;
	LONG nv_ex_edit_menu = nv_ex_default | WS_EX_TRANSPARENT;
	bool bESP;
	bool bItemGlow;
	bool bPlayerGlow;
	bool bAimbot;
	float flMaxDist;
	float flsmooth;
	float flFov;
	int dwbone;
	int dwwidth;
	int dwheight;
	bool bLeftclick = false;
	bool bIns = false;
	bool bMenu = false;
	visuals v;
	bool bActive;
	HWND overlayHWND;
	void Start(HWND Overlay_hwnd);
	DWORD CreateOverlay();
	void Clear();
	int getWidth();
	int getHeight();
	void RenderInfo();
	void RenderMenu();
	void RenderEsp(int dwEntity);
	void ClickThrough(bool v);
	void DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width);
	void DrawBox(ImColor color, float x, float y, float w, float h);
	void Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect);
	void DrawStrokeText(float x, float y, const char* str);
	float DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center);
	void DrawText1(float x, float y, const char* str, ImVec4 color);
	void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags);
	void ProgressBar(float x, float y, float w, float h, int value, int v_max, ImColor barColor, bool Outlined, ImColor Outlinecolor = ImColor(0,0,0));
	void String(ImVec2 pos, ImColor color, const char* text);
	void DrawCircle(float x, float y, float radius, ImVec4 color, int segments = 0);
	void DrawFilledCircle(float x, float y, float radius, ImVec4 color, int segments = 0 );
	void DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness);
	void RectFilled(float x0, float y0, float x1, float y1, ImU32 color, float rounding, int rounding_corners_flags);
	void RectOutlined(float x0, float y0, float x1, float y1, ImU32 color, float rounding, int rounding_corners_flags);

private:
	bool isInitialized = true;
	bool isVisible = true;
};