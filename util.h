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