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

	// �����/������Ӧ���Ի�/��ʷ
	// ��ʷ�򴥷���ʽ��1.�����ϻ���2.����ͼ��
	dialog_box.on_input(event);
	//dialog_history.on_input(event);
}
void DialogMgr::on_update(float delta)
{
	if (GameSystem::instance()->get_mode() != GameSystem::Mode::Dialogue || SceneMgr::instance()->is_transition())
		return;

	// ����ִ�нű�
	parse();

	// todo��ʱ�����£�����0.5s�ε������ı���̬����
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

	// ��Ⱦ��ɫ���棬������ı�
	dialog_box.on_render(renderer);
	//dialog_history.on_render(renderer);
}

void DialogMgr::parse()
{
	// �����ű���ȷ��������ִ��ʲô.�������Ի�û�н�������ϷĿ��û�дﵽ
	if (!condition || !check_idx()) return;			// �������������

	// if(����)�л�ģʽ���趨����Ŀ�ꡣ�л�������������Ч
	// if(�԰�)�����Ի�
	const std::vector<std::string>& script = ResMgr::instance()->find_script(script_id);
	const std::string& text = script[idx++];

#ifdef DEBUG
	SDL_Log("parse script: %s\n", text.c_str());
#endif // DEBUG


	if (text.empty())return;
	// ָ��
	if (text.back() == ']')
	{
		int pos = text.find(':');
		std::string cmd = text.substr(1, pos - 1);
		std::string opt = text.substr(pos + 1, std::string::npos);
		opt.pop_back();
		if (cmd == "describe")					// �����л����ȣ�Ĭ�Ϻڵװ�������
		{
			SceneMgr::instance()->transition_scene(opt, 5.0);
		}
		else if (cmd == "describe_image")		// ��ӳ����л���ͼƬ
		{
			SceneMgr::instance()->set_transition_background(opt);
		}
		else if (cmd == "describe_position")	// ��������λ�ã�center/bottom
		{
			SceneMgr::instance()->set_transition_text_position(opt);
		}
		else if (cmd == "scene")				// �л�����
		{
			SceneMgr::instance()->switch_scene(opt);
		}
		else if (cmd == "music")				// ���ű�������	
		{
			GameSystem::instance()->switch_bgm(opt);
		}
		else if (cmd == "interlude")			// ������Ч
		{
			GameSystem::instance()->play_audio(opt);
		}
		else if (cmd == "game")					// ��Ϸ����ģʽ,��ϷĿ�����Ϊ��; Ŀ��(1.��ȡһ��������ң�2.����ĳ�־�)
		{
			GameSystem::instance()->set_mode(GameSystem::Mode::Operator);
			if (opt.find("coins") != std::string::npos)
			{
				int val = std::stoi(opt.substr(5, std::string::npos));
				GameSystem::instance()->set_coins_goal(val);
				enable_tips(true);
				set_tips(u8"Tips: ׬ȡӲ��" + std::to_string(val));
			}
			else
			{
				GameSystem::instance()->set_drink_goal(opt);
				enable_tips(true);
				set_tips(u8"Tips: ����һ��" + opt);
				BartendSystem::instance()->enable_meun(true);
			}
			condition = false;
		}
		else if (cmd == "next_script")	// ��һ��ִ�нű�
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
	// �ı�
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
	// todo ������Լ�����Ч/��Ч
}

void DialogMgr::DialogBox::on_render(SDL_Renderer* renderer)
{
	// ��Ⱦ��ɫ����
	static int dialog_box_h = 100;
	SDL_Rect dstrect = { 0 };
	if (img != "??" && img != "Player" && !img.empty())
	{
		SDL_QueryTexture(ResMgr::instance()->find_texture(img), nullptr, nullptr, &dstrect.w, &dstrect.h);
		dstrect.x = 1280 / 2 - dstrect.w / 2 - 200;
		dstrect.y = 720 - dialog_box_h - dstrect.h;
		SDL_RenderCopy(renderer, ResMgr::instance()->find_texture(img), nullptr, &dstrect);
	}

	// ��Ⱦ�Ի���
	static SDL_Rect box_rect = { 0, 720 - dialog_box_h, 1280, dialog_box_h };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
	SDL_RenderFillRect(renderer, &box_rect);

	// ��Ⱦ�Ի��ı�
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

