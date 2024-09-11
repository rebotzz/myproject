#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "scene.h"



// 场景管理器: 单例
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
		// putimage_alpha_ex 参数rect_dst
		// 这里将rect_dst (x, y) 与 camera (x, y) 坐标做差,生成新的坐标,
		// 然后绘制行动坐标?
		// 不,很多个画面已经开始绘制了,
		// 除非修改大部分 render接口, 都加上 camera,然后才行

		// 临时方案, 这里有一个摄像机,然后 extern 到 putimage_alpha_ex, 那里绘制修改后的坐标
		// 然后这里修改的坐标是全局的一个 camera , 或者用一个 get_camera()

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

	// 可以在状态节点里面调用这个摄像机震动
	// 只需要使用管理器 instance ...
	const Camera& get_camera() const
	{
		return camera;
	}

};