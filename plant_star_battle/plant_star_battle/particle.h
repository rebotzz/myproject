#pragma once
#include "atlas.h"
#include "util.h"

// ����ϵͳ: ���� + ���ӷ�����
// ������
class Particle
{
private:
	Atlas* _atlas;			// ����֡ͼƬ
	int _timer = 0;			// �������ŷ���ʱ��
	int _lifespan = 0;		// ��֡���Ӷ�������ʱ��
	int _idx_frame = 0;		// ֡����,��ǰ���ڲ��ŵĶ���֡
	Vector2 _position;		// ���ӵ���������λ��
	bool _valid = true;		// ���Ӷ����Ƿ���Ч

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
		// ����һ�����ɺ�,��������λ�ò�����Ҹı�
		putimage_alpha(camera, (int)_position.x, (int)_position.y, _atlas->get_image(_idx_frame));
	}
};