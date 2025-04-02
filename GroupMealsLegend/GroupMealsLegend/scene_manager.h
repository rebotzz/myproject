#pragma once
#include "scene.h"

#include <unordered_map>
#include <string>

// 包含关系：场景管理器 - 场景 - 多个区域


class SceneMgr
{
private:
	static SceneMgr* manager;

	std::unordered_map<std::string, Scene*> scene_pool;
	Scene* cur_scene = nullptr;

private:
	SceneMgr() = default;
	~SceneMgr() = default;

public:
	static SceneMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void add(const std::string& id, Scene* scene);
	void switch_scene(const std::string& id);
};