#define _CRT_SECURE_NO_WARNINGS 1
#include"util.h"
#include"resourcesmanager.h"
#include"collisionmanager.h"
#include"charactormanager.h"
#include"bullet-timer-manager.h"

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

static void draw_remain_hp()
{
	static IMAGE* img_ui_heart = ResourcesManager::instance()->find_image("ui_heart");
	Rect rect_dst = { 0,10,img_ui_heart->getwidth(),img_ui_heart->getheight() };
	for (int i = 0; i < CharactorManager::instance()->get_player()->get_hp(); i++)
	{
		rect_dst.x = 10 + i * 40;
		putimage_ex(img_ui_heart, &rect_dst);
	}

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
	play_audio(_T("bgm"), true);

	const nanoseconds frame_duration(1000000000/144);
	steady_clock::time_point last_tick = steady_clock::now();

	while (!is_quitted)
	{
	
		while (peekmessage(&msg))
		{

			CharactorManager::instance()->get_player()->on_input(msg);

		}
		//处理消息

		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);

		float scaled_delta = BulletTimeManager::instance()->on_update(delta.count());
		CharactorManager::instance()->get_player()->on_update(scaled_delta);
		//CharactorManager::instance()->on_update(delta.count());
		CollisionManager::instance()->process_collision();

		//处理更新
		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		draw_background();

		CharactorManager::instance()->on_render();
		CollisionManager::instance()->on_debug_render();
		draw_remain_hp();
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