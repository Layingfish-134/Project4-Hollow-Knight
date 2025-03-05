#pragma once
#include"util.h"
#include"atlas.h"
#include"Vector2.h"
#include"timer.h"

#include<functional>

class Animation
{
public:
	Animation()
	{
		timer.set_oneshot(false);
		timer.set_callback([&]()
			{
				idx_frame++;
				if (idx_frame >= frame_list.size())
				{
					idx_frame = is_loop ? 0 : frame_list.size() - 1;
					if (!is_loop && idx_frame == frame_list.size() - 1)
					{
						on_finished();
					}
				}
			}
		);
	}
	~Animation();
public:
	enum class AnchorMode
	{
		Centered,
		Bottomcentered
	};
public:
	void reset()
	{
		timer.reset();

		idx_frame = 0;
	}

	void set_interval(float ms)
	{
		timer.set_wait_time(ms);
	}

	void set_anchor_mode(AnchorMode mode)
	{
		anchor_mode = mode;
	}

	void set_loop(bool flag)
	{
		is_loop = flag;
	}

	void set_position(const Vector2& pos)
	{
		position = pos;
	}

	void set_on_finished(std::function<void()> on_finished)
	{
		this->on_finished = on_finished;
	}
    
	void add_frame(IMAGE* img, int num_frame)
	{
		int w = img->getwidth();
		int h = img->getheight();
		int frame_w = w / num_frame;

		for (int i = 0; i < num_frame; i++)
		{
			Rect rect;
			rect.x = frame_w * i;
			rect.y = 0;
			rect.w = frame_w;
			rect.h = h;
			frame_list.emplace_back(rect, img);
		}
	}

	void add_frame(Atlas* atlas)
	{
		for (int i = 0; i < atlas->get_size(); i++)
		{
			IMAGE* img = atlas->get_image(i);
			Rect rect;
			rect.x = 0;
			rect.y = 0;
			rect.h = img->getheight();
			rect.w = img->getwidth();;
			frame_list.emplace_back(rect, img);
		}
	}
public:
	void on_update(int delta)
	{
		timer.on_update(delta);
	}

	void on_render()
	{
		const Frame& frame = frame_list[idx_frame];
         

		Rect dst_rect;
		dst_rect.x = (int)position.x - frame.src_rect.w / 2;
		dst_rect.y = (anchor_mode == AnchorMode::Centered) ?
			position.y - frame.src_rect.h / 2 : (int)position.y - frame.src_rect.h;
		dst_rect.w = frame.src_rect.w;
		dst_rect.h = frame.src_rect.h;

		putimage_ex(frame.img, &dst_rect, &frame.src_rect);
	}
private:
	struct Frame
	{
		Rect src_rect;
		IMAGE* img = nullptr;

		Frame() = default;
		Frame(const Rect& rect,IMAGE* im):src_rect(rect),img(im){}
		~Frame() = default;
	};
private:
	Timer timer;
	Vector2 position;
	size_t idx_frame = 0;
	bool is_loop = true;
	std::vector<Frame> frame_list;
	std::function<void()> on_finished = nullptr;
	AnchorMode anchor_mode = AnchorMode::Centered;
};
