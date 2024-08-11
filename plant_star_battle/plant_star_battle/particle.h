#pragma once
#include "atlas.h"
#include "util.h"

// 粒子系统: 粒子 + 粒子发射器
// 粒子类
class Particle
{
private:
	Atlas* _atlas;			// 动画帧图片
	int _timer = 0;			// 动画播放发计时器
	int _lifespan = 0;		// 单帧粒子动画持续时长
	int _idx_frame = 0;		// 帧索引,当前正在播放的动画帧
	Vector2 _position;		// 粒子的世界坐标位置
	bool _valid = true;		// 粒子对象是否有效

public:
	Particle() = default;
	Particle(Atlas* atlas, const Vector2& position, int lifespan)
		:_atlas(atlas), _position(position), _lifespan(lifespan) {}
	~Particle() = default;

	void set_position(float x, float y)
	{
		_position.x = x;
		_position.y = y;
	}

	void set_lifespan(int ms)
	{
		_lifespan = ms;
	}

	bool check_valid() const
	{
		return _valid;
	}

	void on_update(int ms)
	{
		_timer += ms;
		if (_timer >= _lifespan)
		{
			_timer = 0;
			_idx_frame++;
			if (_idx_frame >= _atlas->get_size())
			{
				_idx_frame = _atlas->get_size() - 1;
				_valid = false;
			}
		}
	}

	void on_draw(const Camera& camera) const
	{
		// 粒子一旦生成后,世界坐标位置不随玩家改变
		putimage_alpha(camera, (int)_position.x, (int)_position.y, _atlas->get_image(_idx_frame));
	}
};