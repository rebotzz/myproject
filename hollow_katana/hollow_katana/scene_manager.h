#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "scene.h"



// ����������: ����
class SceneManager
{
private:
	static SceneManager* manager;											// ����
	std::unordered_map <std::string, std::shared_ptr<Scene>> scene_pool;	// ������
	std::shared_ptr<Scene> current_scene;									// ��ǰ����
	std::string cur_scene_id;
	std::string prev_scene_id;												// ��һ������
	bool need_init = true;													// �Ƿ���Ҫ��ʼ��
	Camera camera;															// �������


private:
	SceneManager();
	~SceneManager();

public:
	static SceneManager* instance();

	void set_entry_scene(const std::string& id);
	void switch_scene(std::string id);
	void register_scene(const std::string& id, const std::shared_ptr<Scene>& scene);

	void on_input(const ExMessage& msg);

	void on_update(float delta);		// debug: ������Ҫfloat

	void on_render()
	{
		current_scene->on_render();
	}

	// putimage_alpha_ex ����rect_dst
	// ���ｫrect_dst (x, y) �� camera (x, y) ��������,�����µ�����,
	// Ȼ������ж�����?
	// ��,�ܶ�������Ѿ���ʼ������,
	// �����޸Ĵ󲿷� render�ӿ�, ������ camera,Ȼ�����
	
	// ��ʱ����, ������һ�������,Ȼ�� extern �� putimage_alpha_ex, ��������޸ĺ������
	// Ȼ�������޸ĵ�������ȫ�ֵ�һ�� camera , ������һ�� get_camera()

	void switch_to_pre_scene()
	{
		if(!prev_scene_id.empty())
			switch_scene(prev_scene_id);
	}

	const std::string& get_pre_scene() const
	{
		return prev_scene_id;
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