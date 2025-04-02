#pragma once
#include "scene.h"

#include <unordered_map>
#include <string>

// ������ϵ������������ - ���� - �������


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