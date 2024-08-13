#pragma once
#include <functional>
#include <easyx.h>
#include "vector2.h"
#include "player_id.h"
#include "camera.h"

extern bool is_debug;

// �ӵ�����
// ִ���߼�: �ӵ���ײ�󴥷���ײִ���߼�,Ȼ���ӵ���Ч,����ʼ�����ӵ����鶯��,���������ӵ���ɾ��
class Bullet
{
protected:
	Vector2 _size;						// �ӵ���ײ��ߴ�
	Vector2 _position;					// �ӵ���ײ��λ��
	Vector2 _velocity;					// �ӵ��ٶ�
	int _damage = 10;					// �ӵ��˺�
	bool _valid = true;					// �ӵ���ײ����Ƿ���Ч
	bool _can_remove = false;			// �ӵ��Ƿ��Ƴ�
	std::function<void()> _callback;	// �ӵ���ײ�ص��߼�
	PlayerID _target_id = PlayerID::P1;	// �ӵ���ײĿ�����id,���ֵ���

	// ���ӵ�ͼƬ�ߴ�����ײ�ߴ粻ͬʱʹ�����²���
	Vector2 _img_size;					// �ӵ�ͼƬ��С
	Vector2 _render_offset;				// �ӵ���ײ��������ȾͼƬ��ƫ��(ͳһ����)

public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val)
	{
		_damage = val;
	}

	int get_damage() const
	{
		return _damage;
	}

	void set_size(float x, float y)
	{
		_size.x = x, _size.y = y;
	}

	const Vector2& get_size() const
	{
		return _size;
	}

	void set_img_size(float x, float y)
	{
		_img_size.x = x, _img_size.y = y;
	}

	const Vector2& get_img_size() const
	{
		return _img_size;
	}

	void set_collide_target(PlayerID target)
	{
		_target_id = target;
	}

	PlayerID get_collide_target() const
	{
		return _target_id;
	}

	void set_velocity(float x, float y)
	{
		_velocity.x = x;
		_velocity.y = y;
	}
	void set_velocity(const Vector2& velocity)
	{
		_velocity = velocity;
	}

	const Vector2& get_velocity() const
	{
		return _velocity;
	}

	void set_position(float x, float y)
	{
		_position.x = x;
		_position.y = y;
	}
	void set_position(const Vector2& position)
	{
		_position = position;
	}

	const Vector2& get_position() const
	{
		return _position;
	}

	virtual void set_valid(bool flag)
	{
		_valid = flag;
	}

	bool get_valid() const
	{
		return _valid;
	}

	bool check_can_remove() const
	{
		return _can_remove;
	}

	void set_callback(std::function<void()> callback)
	{
		_callback = callback;
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		// �ж��ӵ������Ƿ��������ײ������
		float bullet_mid_x = _position.x + _size.x / 2;
		float bullet_mid_y = _position.y + _size.y / 2;
		return bullet_mid_x > position.x && bullet_mid_x < position.x + size.x
			&& bullet_mid_y > position.y && bullet_mid_y < position.y + size.y;
	}

	virtual void on_collide()
	{
		if (_callback)
			_callback();
	}

	virtual void on_draw(const Camera& camera) const
	{
		// �ӵ���ײ���߿�
		if (is_debug)
		{
			setlinecolor(RGB(255, 255, 255));
			setfillcolor(RGB(255, 51, 51));
			rectangle(camera, (int)_position.x, (int)_position.y, (int)(_position.x + _size.x), (int)(_position.y + _size.y));
			solidcircle(camera, (int)(_position.x + _size.x / 2), (int)(_position.y + _size.y / 2), 5);
		}
	}

	virtual void on_update(int interval_ms)
	{}

protected:

	// ����Ƿ񳬹���Ļ�߽�,�����ӵ�һֱ����
	bool check_if_exceeds_screen(const Camera& camera)
	{
		Vector2 pos_camera = camera.get_position();
		return _position.x + _size.x - pos_camera.x < 0 || _position.x - pos_camera.x > getwidth()
			|| _position.y - pos_camera.y > getheight() || _position.y + _size.y - pos_camera.y < 0;
	}

	bool check_if_exceeds_screen()
	{
		return _position.x + _size.x < 0 || _position.x > getwidth()
			|| _position.y > getheight() || _position.y + _size.y < 0;
	}

	// ��ײ�������Ƿ��ཻ
	bool check_collision_rect(const Vector2& position, const Vector2& size)
	{
		bool is_x_collide = (max(_position.x + _size.x, position.x + size.x) - min(_position.x, position.x))
			< (size.x + _size.x);
		bool is_y_collide = (max(_position.y + _size.y, position.y + size.y) - min(_position.y, position.y))
			< (size.y + _size.y);

		return is_x_collide && is_y_collide;
	}
};