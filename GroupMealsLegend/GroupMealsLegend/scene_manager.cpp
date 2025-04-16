#include "scene_manager.h"
#include "concrete_scene.h"

#include "game_system.h"

SceneMgr* SceneMgr::manager = nullptr;

SceneMgr* SceneMgr::instance()
{
	if (!manager)
	{
		manager = new SceneMgr();
	}
	return manager;
}

SceneMgr::SceneMgr()
{
	scene_pool["day"] = new DayScene();
	scene_pool["night"] = new NightScene();
	scene_pool["transition"] = new TransitionScene();
	cur_scene = scene_pool["day"];
	cur_scene_id = "day";
	cur_scene->on_enter();
}

void SceneMgr::on_input(const SDL_Event& event)
{
	// 只渲染，不接收输入、更新；为了对话在场景之上提供支持。
	if (cur_scene_id != "transition" && GameSystem::Mode::Dialogue == GameSystem::instance()->get_mode()) return;

	if (cur_scene)
		cur_scene->on_input(event);
}
void SceneMgr::on_update(float delta)
{
	if (cur_scene_id != "transition" && GameSystem::Mode::Dialogue == GameSystem::instance()->get_mode()) return;

	if (cur_scene)
		cur_scene->on_update(delta);
}
void SceneMgr::on_render(SDL_Renderer* renderer)
{
	if (cur_scene)
		cur_scene->on_render(renderer);
}

void SceneMgr::add(const std::string& id, Scene* scene)
{
	scene_pool[id] = scene;
}

void SceneMgr::switch_scene(const std::string& id)
{
	SDL_Log("switch_scene: %s\n", id.c_str());

	if (cur_scene)
		cur_scene->on_exit();
	cur_scene = scene_pool[id];
	cur_scene_id = id;
	if (cur_scene)
		cur_scene->on_enter();
}

void SceneMgr::transition_scene(const std::string& transition_text, float transition_time, const std::string& next_scene)
{
	TransitionScene* trans = dynamic_cast<TransitionScene*>(scene_pool["transition"]);
	trans->set_text(transition_text);
	trans->set_wait_time(transition_time);
	trans->set_next_scene(next_scene == "" ? cur_scene_id : next_scene);
	switch_scene("transition");
}

bool SceneMgr::is_transition() const
{
	return "transition" == cur_scene_id;
}

void SceneMgr::set_transition_background(const std::string& tex_id)
{
	TransitionScene* trans = dynamic_cast<TransitionScene*>(scene_pool["transition"]);
	trans->set_background(tex_id);
}
