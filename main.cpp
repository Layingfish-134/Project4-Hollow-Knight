#define _CRT_SECURE_NO_WARNINGS 1
#include"util.h"
#include"resourcesmanager.h"

#include<graphics.h>
#include<chrono>
#include<thread>

static inline void draw_background()
{
	static IMAGE* img_background = ResourcesManager::instance()->find_image("background");
	static Rect rect_dst
	{
		(getwidth() - img_background->getwidth()) / 2,
		(getheight() - img_background->getheight()) / 2,
		img_background->getwidth(),
		img_background->getheight(),
	};
	putimage_ex(img_background, &rect_dst);
}


int main()
{
	using namespace std::chrono;
	HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);
	SetWindowText(hwnd, _T("Hollow Knight"));

	ExMessage msg;
	BeginBatchDraw();

	bool is_quitted = false;

	try
	{
		ResourcesManager::instance()->load();
	}
	catch ( const LPCTSTR id)
	{
		TCHAR error_msg[512];
		_stprintf_s(error_msg, _T("%s 无法被加载"), id);
		MessageBox(hwnd, error_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
	}

	const nanoseconds frame_duration(1000000000/144);
	steady_clock::time_point last_tick = steady_clock::now();

	while (!is_quitted)
	{

		while (peekmessage(&msg))
		{


		}
		//处理消息

		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);


		//处理更新
		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		draw_background();

		//处理绘图
		FlushBatchDraw();
		
		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
		if (sleep_duration > nanoseconds(0))
			std::this_thread::sleep_for(sleep_duration);

	}

	EndBatchDraw();

	return 0;
}