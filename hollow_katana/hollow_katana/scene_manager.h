#pragma once
#include <memory>
#include <unordered_map>
#include <string>
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

	// putimage_alpha_ex 参数rect_dst
	// 这里将rect_dst (x, y) 与 camera (x, y) 坐标做差,生成新的坐标,
	// 然后绘制行动坐标?
	// 不,很多个画面已经开始绘制了,
	// 除非修改大部分 render接口, 都加上 camera,然后才行
	
	// 临时方案, 这里有一个摄像机,然后 extern 到 putimage_alpha_ex, 那里绘制修改后的坐标
	// 然后这里修改的坐标是全局的一个 camera , 或者用一个 get_camera()

	void switch_to_pre_scene()
	{
		if(!prev_scene_id.empty())
			switch_scene(prev_scene_id);
	}

	const std::string& get_pre_scene() const
	{
		return prev_scene_id;
	}

	Scene* get_transition_scene()
	{
		return scene_pool["transition_scene"].get();
	}

	Camera* get_camera() 
	{
		return &camera;
	}

};