#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "scene.h"



// ����������: ����
class SceneManager
{
private:
	static SceneManager* manager;
	std::unordered_map <std::string, std::shared_ptr<Scene>> scene_pool;
	std::shared_ptr<Scene> current_scene;
	bool need_init = true;
	bool is_shake = false;
	Camera camera;


private:
	SceneManager();
	~SceneManager();

public:
	static SceneManager* instance();

	void set_entry_scene(const std::string& id);
	void switch_scene(const std::string& id);
	void register_scene(const std::string& id, const std::shared_ptr<Scene>& scene);

	void on_input(ExMessage& msg)
	{
		current_scene->on_input(msg);
	}

	void on_update(int delta_time);

	void on_render(const Camera& camera)
	{
		// putimage_alpha_ex ����rect_dst
		// ���ｫrect_dst (x, y) �� camera (x, y) ��������,�����µ�����,
		// Ȼ������ж�����?
		// ��,�ܶ�������Ѿ���ʼ������,
		// �����޸Ĵ󲿷� render�ӿ�, ������ camera,Ȼ�����

		// ��ʱ����, ������һ�������,Ȼ�� extern �� putimage_alpha_ex, ��������޸ĺ������
		// Ȼ�������޸ĵ�������ȫ�ֵ�һ�� camera , ������һ�� get_camera()

		current_scene->on_render(camera);
	}


	void set_camera_position(const Vector2& position)
	{
		camera.set_position(position);
	}

	const Vector2& get_camera_position() const
	{
		return camera.get_position();
	}

	void camera_shake(float strength, float duration)
	{
		camera.shake(strength, duration);
	}

	// ������״̬�ڵ������������������
	// ֻ��Ҫʹ�ù����� instance ...
	const Camera& get_camera() const
	{
		return camera;
	}

};