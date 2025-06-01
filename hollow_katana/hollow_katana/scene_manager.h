#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <queue>
#include "scene.h"
#include "camera.h"


// ����������: ����
class SceneManager
{
private:
	static SceneManager* manager;											// ����
	std::unordered_map <std::string, std::shared_ptr<Scene>> scene_pool;	// ������
	std::shared_ptr<Scene> current_scene;									// ��ǰ����
	std::string cur_scene_id;												// ��ǰ����id
	std::string prev_scene_id;												// ��һ������id
	bool need_init = true;													// �Ƿ���Ҫ��ʼ��
	Camera camera;															// �������

	// ������
	// ���������������ڶ�̬�����ͷų���
	std::unordered_map<std::string, std::function<void()>> scene_loader;
	std::deque<std::string> scene_cache;	// ����������У�����һ������������������������ͷ������
	const int SCENE_CACHE_SIZE = 4;			// ���������������


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

	void switch_to_pre_scene()
	{
		if(!prev_scene_id.empty())
			switch_scene(prev_scene_id);
	}

	const std::string& get_pre_scene() const
	{
		return prev_scene_id;
	}

	void set_transition_next_scene(const std::string& id);

	// ��ʱ���������, extern��putimage_alpha_ex
	Camera* get_camera()
	{
		return &camera;
	}

};