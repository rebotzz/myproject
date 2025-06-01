#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <queue>
#include "scene.h"
#include "camera.h"


// 场景管理器: 单例
class SceneManager
{
private:
	static SceneManager* manager;											// 单例
	std::unordered_map <std::string, std::shared_ptr<Scene>> scene_pool;	// 场景池
	std::shared_ptr<Scene> current_scene;									// 当前场景
	std::string cur_scene_id;												// 当前场景id
	std::string prev_scene_id;												// 上一个场景id
	bool need_init = true;													// 是否需要初始化
	Camera camera;															// 主摄像机

	// 其他：
	// 场景加载器，用于动态加载释放场景
	std::unordered_map<std::string, std::function<void()>> scene_loader;
	std::deque<std::string> scene_cache;	// 场景缓存队列，缓存一定数量场景，超过数量后就释放最早的
	const int SCENE_CACHE_SIZE = 4;			// 场景缓存队列容量


private:
	SceneManager();
	~SceneManager();

public:
	static SceneManager* instance();

	void set_entry_scene(const std::string& id);
	void switch_scene(std::string id);
	void register_scene(const std::string& id, const std::shared_ptr<Scene>& scene);

	void on_input(const ExMessage& msg);

	void on_update(float delta);		// debug: 这里需要float

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

	// 临时摄像机方案, extern到putimage_alpha_ex
	Camera* get_camera()
	{
		return &camera;
	}

};