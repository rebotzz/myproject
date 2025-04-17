#include "dialogue_manager.h"
#include "resources_manager.h"
#include "scene_manager.h"
#include "game_system.h"
#include "bartend_system.h"
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
	if (GameSystem::instance()->get_mode() != GameSystem::Mode::Dialogue || SceneMgr::instance()->is_transition())
		return;

	// 鼠标点击/滚动响应，对话/历史
	// 历史框触发方式：1.滚轮上滑，2.按键图标
	dialog_box.on_input(event);
	//dialog_history.on_input(event);
}
void DialogMgr::on_update(float delta)
{
	if (GameSystem::instance()->get_mode() != GameSystem::Mode::Dialogue || SceneMgr::instance()->is_transition())
		return;

	// 解析执行脚本
	parse();

	// todo定时器更新，给个0.5s滴滴声，文本动态出现
	dialog_box.on_update(delta);
	//dialog_history.on_update(delta);

}
void DialogMgr::on_render(SDL_Renderer* renderer)
{
	if (showing_tip)
	{
		dialog_box.on_render(renderer);
	}

	if (GameSystem::instance()->get_mode() != GameSystem::Mode::Dialogue || SceneMgr::instance()->is_transition())
		return;

	// 渲染角色立绘，聊天框，文本
	dialog_box.on_render(renderer);
	//dialog_history.on_render(renderer);
}

void DialogMgr::parse()
{
	// 解析脚本，确定接下来执行什么.条件：对话没有结束，游戏目标没有达到
	if (!condition || !check_idx()) return;			// 不满足解析条件

	// if(命令)切换模式，设定结束目标。切换场景、播放音效
	// if(对白)继续对话
	const std::vector<std::string>& script = ResMgr::instance()->find_script(script_id);
	const std::string& text = script[idx++];

#ifdef DEBUG
	SDL_Log("parse script: %s\n", text.c_str());
#endif // DEBUG


	if (text.empty())return;
	// 指令
	if (text.back() == ']')
	{
		int pos = text.find(':');
		std::string cmd = text.substr(1, pos - 1);
		std::string opt = text.substr(pos + 1, std::string::npos);
		opt.pop_back();
		if (cmd == "describe")					// 场景切换过度：默认黑底白字描述
		{
			SceneMgr::instance()->transition_scene(opt, 5.0);
		}
		else if (cmd == "describe_image")		// 添加场景切换的图片
		{
			SceneMgr::instance()->set_transition_background(opt);
		}
		else if (cmd == "describe_position")	// 过场文字位置：center/bottom
		{
			SceneMgr::instance()->set_transition_text_position(opt);
		}
		else if (cmd == "scene")				// 切换场景
		{
			SceneMgr::instance()->switch_scene(opt);
		}
		else if (cmd == "music")				// 播放背景音乐	
		{
			GameSystem::instance()->switch_bgm(opt);
		}
		else if (cmd == "interlude")			// 播放音效
		{
			GameSystem::instance()->play_audio(opt);
		}
		else if (cmd == "game")					// 游戏操作模式,游戏目标可以为空; 目标(1.获取一定数量金币，2.调制某种酒)
		{
			GameSystem::instance()->set_mode(GameSystem::Mode::Operator);
			if (opt.find("coins") != std::string::npos)
			{
				int val = std::stoi(opt.substr(5, std::string::npos));
				GameSystem::instance()->set_coins_goal(val);
				enable_tips(true);
				set_tips(u8"Tips: 赚取硬币" + std::to_string(val));
			}
			else
			{
				GameSystem::instance()->set_drink_goal(opt);
				enable_tips(true);
				set_tips(u8"Tips: 调制一杯" + opt);
				BartendSystem::instance()->enable_meun(true);
			}
			condition = false;
		}
		else if (cmd == "next_script")	// 下一个执行脚本
		{
			script_id = opt;
			idx = 0;
		}
		else
		{
#ifdef DEBUG
			SDL_Log("unkown cmd: %s\n", cmd.c_str());
#endif // DEBUG
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
			else if (color == "C3") color_id = DialogBox::Color::C3;
			else if (color == "C4") color_id = DialogBox::Color::C4;
			else if (color == "C5") color_id = DialogBox::Color::C5;
			else if (color == "C6") color_id = DialogBox::Color::C6;

			dialog_box.set_dialog(dialog, npc_img, color_id);
		}
	}
}
void DialogMgr::set_idx(int val)
{
	idx = val;
}
bool DialogMgr::check_idx() const
{
	const std::vector<std::string>& script = ResMgr::instance()->find_script(script_id);
	return idx < script.size();
}

void DialogMgr::finish_goal()
{
	condition = true;
	enable_tips(false);
}

void DialogMgr::set_script_id(const std::string& id)
{
	script_id = id;
}
int DialogMgr::get_idx() const
{
	return idx;
}
const std::string& DialogMgr::get_script_id() const
{
	return script_id;
}

void DialogMgr::set_tips(const std::string& val)
{
	dialog_box.set_dialog(val, "", DialogBox::Color::C1);
}
void DialogMgr::enable_tips(bool flag)
{
	showing_tip = flag;
}


DialogMgr::DialogBox::DialogBox()
{
	color_map[static_cast<int>(Color::C1)] = { 200, 200, 200, 255 };
	color_map[static_cast<int>(Color::C2)] = { 50, 130, 240, 255 };
	color_map[static_cast<int>(Color::C3)] = { 60, 230, 230, 255 };
	color_map[static_cast<int>(Color::C4)] = { 70, 255, 90, 255 };
	color_map[static_cast<int>(Color::C5)] = { 255, 130, 70, 255 };
	color_map[static_cast<int>(Color::C6)] = { 255, 70, 170, 255 };
}

void DialogMgr::DialogBox::on_input(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		DialogMgr::instance()->finish_goal();
		break;
	}
}

void DialogMgr::DialogBox::on_update(float delta)
{
	// todo 或许可以加入特效/音效
}

void DialogMgr::DialogBox::on_render(SDL_Renderer* renderer)
{
	// 渲染角色立绘
	static int dialog_box_h = 100;
	SDL_Rect dstrect = { 0 };
	if (img != "??" && img != "Player" && !img.empty())
	{
		SDL_QueryTexture(ResMgr::instance()->find_texture(img), nullptr, nullptr, &dstrect.w, &dstrect.h);
		dstrect.x = 1280 / 2 - dstrect.w / 2 - 200;
		dstrect.y = 720 - dialog_box_h - dstrect.h;
		SDL_RenderCopy(renderer, ResMgr::instance()->find_texture(img), nullptr, &dstrect);
	}

	// 渲染对话框
	static SDL_Rect box_rect = { 0, 720 - dialog_box_h, 1280, dialog_box_h };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
	SDL_RenderFillRect(renderer, &box_rect);

	// 渲染对话文本
	SDL_Color text_color = color_map[static_cast<int>(color)];
	static SDL_Rect text_rect = { box_rect.x + 30, box_rect.y + 15, box_rect.w - 60, box_rect.h - 20 };
	int pos = text.find(':');
	std::string name = text.substr(0, pos + 1);
	std::string talk = text.substr(pos + 1, std::string::npos);
	render_textEx(renderer, { name, talk }, { text_color , {255,255,255,255} }, { text_rect.x, text_rect.y });
}

void DialogMgr::DialogBox::set_dialog(const std::string& text, const std::string& img, Color color)
{
	this->text = text;
	this->img = img;
	this->color = color;
}

