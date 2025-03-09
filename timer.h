#pragma once
#include<functional>
class Timer
{
public:
	Timer()=default;
	~Timer()=default;
public:
	void reset()
	{
		pass_time = 0;
		is_shotted = false;
	}

	void set_oneshot(bool flag)
	{
		one_shot = flag;
	}

	void set_wait_time(float ms)
	{
		wait_time = ms;
	}

	void pause()
	{
		is_paused = true;
	}

	void resume()
	{
		is_paused = false;
	}

	void set_callback(std::function<void()>callback)
	{
		this->on_timeout = callback;
	}
public:
	void on_update(float delta)
	{
		if (is_paused)
			return;

		pass_time += delta;

		if (pass_time >= wait_time)
		{
			bool can_shot = !one_shot || (one_shot && !is_shotted) && on_timeout;
			is_shotted = true;
			if (can_shot)
			{
				on_timeout();
			}
			pass_time -= wait_time;
		}
	}
private:
	float pass_time = 0;
	float wait_time = 0;
	bool is_paused = false;
	bool is_shotted = false;
	bool one_shot = false;//单次触发
	std::function <void()>on_timeout;
};
