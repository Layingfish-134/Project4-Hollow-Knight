#pragma once
#include<graphics.h>

#pragma comment(lib,"WINMM.lib")
#pragma comment(lib,"MSIMG32.lib")

struct Rect
{
	int x, y;
	int w, h;
};

inline void putimage_ex(IMAGE* img, const Rect* dst_rect, const Rect* src_rect=nullptr)
{
	static BLENDFUNCTION blend_func = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(GetWorkingImage()),dst_rect->x,
		dst_rect->y,dst_rect->w,dst_rect->h,GetImageHDC(img), 
		src_rect ? src_rect->x : 0,src_rect?src_rect->y:0,
		src_rect?src_rect->w:img->getwidth(),src_rect?src_rect->h:img->getheight(),
		blend_func);
}
inline void load_audio(LPCTSTR path, LPCTSTR id)
{
	TCHAR str_in[256];
	_stprintf_s(str_in, _T("open %s alias %s"), path, id);
	mciSendString(str_in, NULL, 0, NULL);
}

inline void play_audio(LPCTSTR id, bool is_loop)
{
	TCHAR str_cmd[256];
	_stprintf_s(str_cmd, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
	mciSendString(str_cmd, NULL, 0, NULL);
}

inline void stop_audio(LPCTSTR id)
{
	TCHAR str_cmd[256];
	_stprintf_s(str_cmd, _T("stop %s"), id);
	mciSendString(str_cmd, NULL, 0, NULL);
}

inline int range_random(int min_ran, int max_ran)
{
	return min_ran + rand() % (max_ran - min_ran + 1);
}