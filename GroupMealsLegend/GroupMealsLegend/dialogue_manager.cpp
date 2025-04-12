#include "dialogue_manager.h"
#include "resources_manager.h"
#include "scene_manager.h"
#include "game_system.h"
#include "kits.h"

DialogMgr* DialogMgr::manager = nullptr;

DialogMgr* DialogMgr::instance()
{
	if (nullptr == manager)
	{
		manager = new DialogMgr();
	}
	return manager;
}

void DialogMgr::on_input(const SDL_Event& event)
{
	// 鼠标点击/滚动响应，对话/历史
	// 历史框触发方式：1.滚轮上滑，2.按键图标
	dialog_box.on_input(event);
	dialog_history.on_input(event);
}
void DialogMgr::on_update(float delta)
{
	// 定时器更新，给个0.5s滴滴声，文本动态出现
	dialog_box.on_update(delta);
	dialog_history.on_update(delta);

}
void DialogMgr::on_render(SDL_Renderer* renderer)
{
	// 渲染角色立绘，聊天框，文本
	dialog_box.on_render(renderer);
	dialog_history.on_render(renderer);
}

void DialogMgr::parse()		
{
	if (!condition || !check_idx()) return;			// 不满足解析条件

	// 解析脚本，确定接下来执行什么



	// 循环解析，直到一个耗时长的事件?直到条件不满足？
	// 条件：对话没有结束，游戏目标没有达到
	// 所以，解析之前先判断是一次执行的目标是否达成。

	const std::vector<std::string>& script = ResMgr::instance()->find_script(script_id);
	const std::string& text = script[idx++];

	// 指令
	if (text.back() == ']')
	{
		int pos = text.find(':');
		std::string cmd = text.substr(1, pos - 1);
		std::string opt = text.substr(pos + 1, text.size() - pos - 1);
		if (cmd == "describe")
		{
			// 切换到过度场景：黑底白字描述
			// 或者不用切换场景，直接覆盖渲染？
			SceneMgr::instance()->transition_scene(opt, 5.0);
		}
		else if (cmd == "scene")
		{
			SceneMgr::instance()->switch_scene(opt);
		}
		else if (cmd == "music")		
		{
			// todo
			GameSystem::instance()->switch_bgm(opt);
		}
		else if (cmd == "game")
		{
			GameSystem::instance()->set_mode(GameSystem::Mode::Operator);
			condition = false;
		}
		else if (cmd == "game_goal")
		{
			goal = opt;
		}
		else
		{
			SDL_Log("unkown cmd: %s\n", cmd.c_str());
		}
	}
	// 文本
	else
	{
		GameSystem::instance()->set_mode(GameSystem::Mode::Dialogue);
		condition = false;

		int left = text.find('['), right = text.find(']');
		if (left != std::string::npos && right != std::string::npos)
		{
			std::string npc_color = text.substr(left + 1, right - left - 1);
			std::string npc_img = npc_color.substr(0, npc_color.find(':'));
			std::string color = npc_color.substr(npc_color.find(':') + 1, std::string::npos);
			std::string dialog = text.substr(right + 1, std::string::npos);

			DialogBox::Color color_id = DialogBox::Color::C1;
			if (color == "C1") color_id = DialogBox::Color::C1;
			else if (color == "C2") color_id = DialogBox::Color::C2;
			else if(color == "C3") color_id = DialogBox::Color::C3;

			dialog_box.set_dialog(dialog, npc_img, color_id);
		}
	}

	// if(命令)切换模式，设定结束目标。切换场景、播放音效
	// if(对白)继续对话
}
void DialogMgr::set_idx(int val)
{

}
bool DialogMgr::check_idx() const
{
	const std::vector<std::string>& script = ResMgr::instance()->find_script(script_id);
	return idx < script.size();
}

void DialogMgr::finish_goal()
{
	condition = true;
}
const std::string& DialogMgr::get_goal() const
{
	return goal;
}

//const std::string& DialogMgr::get_next_text() const
//{
//	if (check_idx())
//	{
//
//	}
//
//}

void DialogMgr::DialogBox::on_input(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		//SDL_Point point = { event.motion.x, event.motion.y };
		//if (SDL_PointInRect(&point, &rect))
		//{
		//	if (finished)
		//	{
		//		text = std::move(next_text);
		//		next_text = DialogMgr::instance()->get_next_text();
		//	}
		//	else
		//	{

		//	}
		//}
		DialogMgr::instance()->finish_goal();

		break;

	}
}

void DialogMgr::DialogBox::on_update(float delta)
{
	// todo 或许可以加入特效
}

void DialogMgr::DialogBox::on_render(SDL_Renderer* renderer)
{
	// 渲染角色立绘
	static int dialog_box_h = 100;

	SDL_Rect dstrect = { 0 };
	SDL_QueryTexture(ResMgr::instance()->find_texture(img_id), nullptr, nullptr, &dstrect.w, &dstrect.h);
	dstrect.x = 1280 / 2 - dstrect.w / 2;
	dstrect.y = 720 - dialog_box_h - dstrect.h;
	SDL_RenderCopy(renderer, ResMgr::instance()->find_texture(img_id), nullptr, &dstrect);

	// 渲染对话框
	static SDL_Rect box_rect = { 0, 720 - dialog_box_h, 1280, dialog_box_h };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
	SDL_RenderDrawRect(renderer, &box_rect);
	
	// 渲染对话文本
	SDL_Color text_color;
	switch (color)
	{
	case Color::C1:
		text_color = { 255, 255, 255, 255 };
		break;
	case Color::C2:
		text_color = { 255, 255, 255, 255 };
		break;
	case Color::C3:
		text_color = { 255, 255, 255, 255 };
		break;
	}
	render_text(renderer, text, dstrect, text_color);
}

void DialogMgr::DialogBox::set_dialog(const std::string& text, const std::string& img, Color color)
{
	this->text = text;
	this->img_id = img;
	this->color = color;
}


//void DialogMgr::DialogHistory::on_update(float delta)
//{
//
//}
//
//void DialogMgr::DialogHistory::on_render(SDL_Renderer* renderer)
//{
//
//}