#pragma once
#include "scene.h"

#include <unordered_map>
#include <string>

// ������ϵ������������ - ���������� - ÿ�����������������


class SceneMgr
{
private:
	static SceneMgr* manager;

	std::unordered_map<std::string, Scene*> scene_pool;
	Scene* cur_scene = nullptr;
	std::string cur_scene_id;

private:
	SceneMgr();
	~SceneMgr() = default;

public:
	static SceneMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void add(const std::string& id, Scene* scene);
	void switch_scene(const std::string& id);
	void transition_scene(const std::string& transition_text, float transition_time = 3.0, const std::string& next_scene = "");

	bool is_transition() const;
	void set_transition_background(const std::string& tex_id);
};