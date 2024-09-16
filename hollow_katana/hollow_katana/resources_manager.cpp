#include "resources_manager.h"
#include "util.h"
#include "audio_manager.h"


struct ImageResInfo
{
	std::string id;
	LPCTSTR path;
};

struct AtlasResInfo
{
	std::string id;
	LPCTSTR path;
	int num_frame = 0;
};

static const std::vector<ImageResInfo> image_info_list =
{
	// UI
	{"background_katana",			_T(R"(resources\background_1.png)")},
	{"background_hollow",			_T(R"(resources\background_2.png)")},
	{"ui_heart",					_T(R"(resources\ui_heart.png)")},

	// 玩家素材
	{"player_attack_right",			_T(R"(resources\player\attack.png)")},
	{"player_dead_right",			_T(R"(resources\player\dead.png)")},
	{"player_fall_right",			_T(R"(resources\player\fall.png)")},
	{"player_idle_right",			_T(R"(resources\player\idle.png)")},
	{"player_jump_right",			_T(R"(resources\player\jump.png)")},
	{"player_run_right",			_T(R"(resources\player\run.png)")},
	{"player_roll_right",			_T(R"(resources\player\roll.png)")},

	{"player_vfx_attack_down",		_T(R"(resources\player\vfx_attack_down.png)")},
	{"player_vfx_attack_left",		_T(R"(resources\player\vfx_attack_left.png)") },
	{"player_vfx_attack_right",		_T(R"(resources\player\vfx_attack_right.png)")},
	{"player_vfx_attack_up",		_T(R"(resources\player\vfx_attack_up.png)") },
	{"player_vfx_jump",				_T(R"(resources\player\vfx_jump.png)")},
	{"player_vfx_land",				_T(R"(resources\player\vfx_land.png)") },
	{"sword_hit_right",				_T(R"(resources\effect\sword_hit.png)") },

	// 混叠图片
	{ "effect_mixed_blue_1",		_T(R"(resources\effect\mixed_image\blue_1.png)") },
	{ "effect_mixed_blue_2",		_T(R"(resources\effect\mixed_image\blue_2.png)") },
	{ "effect_mixed_red",			_T(R"(resources\effect\mixed_image\red.png)") },

	// 文本底色
	{ "test_background_black",		_T(R"(resources\effect\mixed_image\test_background_black.png)") },

	// 龙王特效:刀光,冲刺,
	{"dragon_vfx_attack_up",			_T(R"(resources\enemy\dragon_king\vfx\vfx_attack_up.png)")},
	{"dragon_vfx_attack_down",			_T(R"(resources\enemy\dragon_king\vfx\vfx_attack_down.png)")},
	{"dragon_vfx_attack_left",		_T(R"(resources\enemy\dragon_king\vfx\vfx_attack_left.png)")},
	{"dragon_vfx_attack_right",		_T(R"(resources\enemy\dragon_king\vfx\vfx_attack_right.png)")},
	
	{"dragon_vfx_dash_tail_left",				_T(R"(resources\enemy\dragon_king\vfx\dash_tail.png)") },
	{"dragon_vfx_dash_line_right",				_T(R"(resources\enemy\dragon_king\vfx\dash_line.png)") },

	{"menu",				_T(R"(resources\menu.png)") },
	{"menu_introduce",		_T(R"(resources\menu_introduce.png)") },
};


static const std::vector<AtlasResInfo> atlas_info_list =
{
	// 敌人:大黄蜂
	{"barb_break",	_T(R"(resources\enemy\hornet\barb_break\%d.png)"),  3},
	{"barb_loose",	_T(R"(resources\enemy\hornet\barb_loose\%d.png)"),  5},
	{"silk",		_T(R"(resources\enemy\hornet\silk\%d.png)"),		9},
	{"sword_left",	_T(R"(resources\enemy\hornet\sword\%d.png)"),		3},

	{"enemy_hornet_aim_left",			    _T(R"(resources\enemy\hornet\aim\%d.png)"),				9},
	{"enemy_hornet_dash_in_air_left",	    _T(R"(resources\enemy\hornet\dash_in_air\%d.png)"),     2},
	{"enemy_hornet_dash_on_floor_left",     _T(R"(resources\enemy\hornet\dash_on_floor\%d.png)"),   2},
	{"enemy_hornet_fall_left",			    _T(R"(resources\enemy\hornet\fall\%d.png)"),			4},
	{"enemy_hornet_idle_left",			    _T(R"(resources\enemy\hornet\idle\%d.png)"),			6},
	{"enemy_hornet_jump_left",			    _T(R"(resources\enemy\hornet\jump\%d.png)"),			8},

	{"enemy_hornet_run_left",			    _T(R"(resources\enemy\hornet\run\%d.png)"),				 8},
	{"enemy_hornet_squat_left",				_T(R"(resources\enemy\hornet\squat\%d.png)"),		    10},
	{"enemy_hornet_throw_barb_left",	    _T(R"(resources\enemy\hornet\throw_barb\%d.png)"),	     8},
	{"enemy_hornet_throw_silk_left",	    _T(R"(resources\enemy\hornet\throw_silk\%d.png)"),	    17},
	{"enemy_hornet_throw_sword_left",	    _T(R"(resources\enemy\hornet\throw_sword\%d.png)"),     16},
		   
	{"enemy_hornet_vfx_dash_in_air_left",	_T(R"(resources\enemy\hornet\vfx_dash_in_air\%d.png)"),  5},
	{"enemy_hornet_vfx_dash_on_floor_left", _T(R"(resources\enemy\hornet\vfx_dash_on_floor\%d.png)"),6},

	// 角色新增动画 zero跳舞
	{"player_dance_right",  _T(R"(resources\player\dance\%d.png)"), 12},

	// 敌人:龙王
	{"enemy_dragon_king_idle_right",		_T(R"(resources\enemy\dragon_king\idle\%d.png)"),	12},
	{"enemy_dragon_king_prepare_right",		_T(R"(resources\enemy\dragon_king\prepare\%d.png)"),2},
	{"enemy_dragon_king_attack_right",		_T(R"(resources\enemy\dragon_king\attack\%d.png)"),	9},
	{"enemy_dragon_king_jump_right",		_T(R"(resources\enemy\dragon_king\jump\%d.png)"),	4},
	{"enemy_dragon_king_run_right",			_T(R"(resources\enemy\dragon_king\run\%d.png)"),	10},
	{"enemy_dragon_king_fall_right",		_T(R"(resources\enemy\dragon_king\fall\%d.png)"),	13},
	{"fire_dash_left",  _T(R"(resources\effect\fire_dash\%d.png)"),		10},
	{"fire_dash_down",  _T(R"(resources\effect\fire_dash_down\%d.png)"),10},
	{"fire_bullet",_T(R"(resources\effect\fire_bullet\%d.png)"),	13},

	// 新加粒子特效
	{"particle_vfx_hit_left",			  _T(R"(resources\effect\hit\%d.png)"),			4},
	{"particle_vfx_hurt_yellow_right",	  _T(R"(resources\effect\watercolor\%d.png)"),	6},
	{"particle_vfx_hurt",	  _T(R"(resources\effect\hurt\%d.png)"),		18},
	{"particle_vfx_electric",	  _T(R"(resources\effect\electric\%d.png)"), 10},
	{"particle_vfx_electric_right",	  _T(R"(resources\effect\electric_right\%d.png)"), 5},
	{"particle_vfx_fire_left",		  _T(R"(resources\effect\fire\%d.png)"),	6},
	{"particle_vfx_leaves",  _T(R"(resources\effect\leaves\%d.png)"),		 19},
	{"ui_choose_left",  _T(R"(resources\effect\ui_choose\%d.png)"),		 11},
	{"light_bug",  _T(R"(resources\effect\other\bug\%d.png)"),		 8},
	{"light",  _T(R"(resources\effect\other\light\%d.png)"),		 16},
	{"little_knight",  _T(R"(resources\effect\other\little_knight\%d.png)"), 5},

};

static inline bool check_image_valid(IMAGE* img)
{
	// vector中的IMAGE在堆上,IMAGE默认构造没有初始化缓存指针,所以辅助图片尺寸判断
	return GetImageBuffer(img) && img->getheight() && img->getwidth();
}

ResourcesManager* ResourcesManager::manager = nullptr;
ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() = default;

ResourcesManager* ResourcesManager::instance()
{
	if (!manager)
		manager = new ResourcesManager();

	return manager;
}

IMAGE* ResourcesManager::find_image(const std::string& id)
{
	auto iter = image_pool.find(id);
	if (iter == image_pool.end())
		return nullptr;

	return iter->second;
}

Atlas* ResourcesManager::find_atlas(const std::string& id)
{
	auto iter = atlas_pool.find(id);
	if (iter == atlas_pool.end())
		return nullptr;

	return iter->second;
}


void ResourcesManager::flip_image(IMAGE* image_src, IMAGE* image_dst, int num)
{
	int width = image_src->getwidth();
	int height = image_src->getheight();
	int width_frame = width / num;
	image_dst->Resize(width, height);

	for (int i = 0; i < num; ++i)
	{
		// image在逻辑上是由像素点(R,G,B)构成的二维数组,内存上是一维数组
		DWORD* img_src_buffer = GetImageBuffer(image_src);
		DWORD* img_dst_buffer = GetImageBuffer(image_dst);

		int x_left = width_frame * i;
		int x_right = width_frame * (i + 1);
		for (int y = 0; y < height; ++y)
		{
			for (int x = x_left; x < x_right; ++x)
			{
				int src_idx = y * width + x;
				int dst_idx = y * width + x_right - 1 - (x - x_left);
				img_dst_buffer[dst_idx] = img_src_buffer[src_idx];
			}
		}
	}

}

void ResourcesManager::flip_image(const std::string& id_src, const std::string& id_dst, int num)
{
	IMAGE* image_dst = new IMAGE;
	flip_image(image_pool[id_src], image_dst, num);
	image_pool[id_dst] = image_dst;
}

void ResourcesManager::flip_atlas(const std::string& id_src, const std::string& id_dst)
{
	Atlas* atlas_src = atlas_pool[id_src];
	Atlas* atlas_dst = new Atlas;
	for (int i = 0; i < atlas_pool[id_src]->get_size(); ++i)
	{
		IMAGE image_flipped;
		IMAGE* image_src = atlas_src->get_image(i);
		flip_image(image_src, &image_flipped);
		atlas_dst->add_image(image_flipped);
	}
	atlas_pool[id_dst] = atlas_dst;
}



void ResourcesManager::load()
{
	// 加载图片
	for (auto& info : image_info_list)
	{
		IMAGE* image = new IMAGE();
		loadimage(image, info.path);
		if (!check_image_valid(image))
			throw info.path;

		image_pool[info.id] = image;
	}

	for (auto& info : atlas_info_list)
	{
		Atlas* atlas = new Atlas();
		atlas->load(info.path, info.num_frame);

		for (int i = 0; i < atlas->get_size(); ++i)
		{
			IMAGE* image = atlas->get_image(i);

			if (!check_image_valid(image))
				throw info.path;
		}

		atlas_pool[info.id] = atlas;
	}

	// 增加翻转图片
	// 玩家
	flip_image("player_attack_right", "player_attack_left", 5);
	flip_image("player_dead_right", "player_dead_left", 6);
	flip_image("player_fall_right", "player_fall_left", 5);
	flip_image("player_idle_right", "player_idle_left", 5);
	flip_image("player_jump_right", "player_jump_left", 5);
	flip_image("player_run_right", "player_run_left", 10);
	flip_image("player_roll_right", "player_roll_left", 7);
	flip_atlas("player_dance_right", "player_dance_left");
	flip_image("sword_hit_right", "sword_hit_left", 6);
	
	// 敌人:大黄蜂
	flip_atlas("sword_left", "sword_right");
	flip_atlas("enemy_hornet_aim_left", "enemy_hornet_aim_right");
	flip_atlas("enemy_hornet_dash_in_air_left", "enemy_hornet_dash_in_air_right");
	flip_atlas("enemy_hornet_dash_on_floor_left", "enemy_hornet_dash_on_floor_right");
	flip_atlas("enemy_hornet_fall_left", "enemy_hornet_fall_right");

	flip_atlas("enemy_hornet_idle_left", "enemy_hornet_idle_right");
	flip_atlas("enemy_hornet_jump_left", "enemy_hornet_jump_right");
	flip_atlas("enemy_hornet_run_left", "enemy_hornet_run_right");
	flip_atlas("enemy_hornet_squat_left", "enemy_hornet_squat_right");
	flip_atlas("enemy_hornet_throw_barb_left", "enemy_hornet_throw_barb_right");
	flip_atlas("enemy_hornet_throw_silk_left", "enemy_hornet_throw_silk_right");
	flip_atlas("enemy_hornet_throw_sword_left", "enemy_hornet_throw_sword_right");

	flip_atlas("enemy_hornet_vfx_dash_in_air_left", "enemy_hornet_vfx_dash_in_air_right");
	flip_atlas("enemy_hornet_vfx_dash_on_floor_left", "enemy_hornet_vfx_dash_on_floor_right");

	// 敌人:龙王
	flip_atlas("enemy_dragon_king_idle_right", "enemy_dragon_king_idle_left");
	flip_atlas("enemy_dragon_king_prepare_right", "enemy_dragon_king_prepare_left");
	flip_atlas("enemy_dragon_king_attack_right", "enemy_dragon_king_attack_left");
	flip_atlas("enemy_dragon_king_jump_right", "enemy_dragon_king_jump_left");
	flip_atlas("enemy_dragon_king_run_right", "enemy_dragon_king_run_left");
	flip_atlas("enemy_dragon_king_fall_right", "enemy_dragon_king_fall_left");

	flip_image("dragon_vfx_dash_tail_left", "dragon_vfx_dash_tail_right", 11);
	flip_image("dragon_vfx_dash_line_right", "dragon_vfx_dash_line_left", 11);
	flip_atlas("fire_dash_left", "fire_dash_right");
	flip_atlas("particle_vfx_electric_right", "particle_vfx_electric_left");
	flip_atlas("particle_vfx_fire_left", "particle_vfx_fire_right");

	// 特效
	flip_atlas("particle_vfx_hit_left", "particle_vfx_hit_right");
	flip_atlas("particle_vfx_hurt_yellow_right", "particle_vfx_hurt_yellow_left");
	flip_atlas("ui_choose_left", "ui_choose_right");

	// 混叠图片,改变图片原有色彩(注意图片大小别越界)
	blend_atlas("particle_vfx_hurt_yellow_right", "particle_vfx_hurt_red_right", "effect_mixed_red");
	blend_atlas("particle_vfx_hurt_yellow_left", "particle_vfx_hurt_red_left", "effect_mixed_red");



	// 加载音频
	AudioManager* audio_player = AudioManager::instance();
	audio_player->load_audio_ex(_T(R"(resources\audio\barb_break.mp3)"), _T("barb_break"));
	audio_player->load_audio_ex(_T(R"(resources\audio\bullet_time.mp3)"), _T("bullet_time"));
	audio_player->load_audio_ex(_T(R"(resources\audio\reverse_time.mp3)"), _T("reverse_time"));

	audio_player->load_audio_ex(_T(R"(resources\audio\enemy_dash.mp3)"), _T("enemy_dash"));
	audio_player->load_audio_ex(_T(R"(resources\audio\enemy_run.mp3)"), _T("enemy_run"));
	audio_player->load_audio_ex(_T(R"(resources\audio\enemy_hurt_1.mp3)"), _T("enemy_hurt_1"));
	audio_player->load_audio_ex(_T(R"(resources\audio\enemy_hurt_2.mp3)"), _T("enemy_hurt_2"));
	audio_player->load_audio_ex(_T(R"(resources\audio\enemy_hurt_3.mp3)"), _T("enemy_hurt_3"));
	audio_player->load_audio_ex(_T(R"(resources\audio\enemy_throw_barbs.mp3)"), _T("enemy_throw_barbs"));
	audio_player->load_audio_ex(_T(R"(resources\audio\enemy_throw_silk.mp3)"), _T("enemy_throw_silk"));
	audio_player->load_audio_ex(_T(R"(resources\audio\enemy_throw_sword.mp3)"), _T("enemy_throw_sword"));
	audio_player->load_audio_ex(_T(R"(resources\audio\hornet_say_dash.mp3)"), _T("hornet_say_dash"));
	audio_player->load_audio_ex(_T(R"(resources\audio\hornet_say_throw_sword.mp3)"), _T("hornet_say_throw_sword"));
	audio_player->load_audio_ex(_T(R"(resources\audio\hornet_say_throw_silk.mp3)"), _T("hornet_say_throw_silk"));

	audio_player->load_audio_ex(_T(R"(resources\audio\player_attack_1.mp3)"), _T("player_attack_1"));
	audio_player->load_audio_ex(_T(R"(resources\audio\player_attack_2.mp3)"), _T("player_attack_2"));
	audio_player->load_audio_ex(_T(R"(resources\audio\player_attack_3.mp3)"), _T("player_attack_3"));
	audio_player->load_audio_ex(_T(R"(resources\audio\player_dead.mp3)"), _T("player_dead"));
	audio_player->load_audio_ex(_T(R"(resources\audio\player_hurt.mp3)"), _T("player_hurt"));
	audio_player->load_audio_ex(_T(R"(resources\audio\player_jump.mp3)"), _T("player_jump"));
	audio_player->load_audio_ex(_T(R"(resources\audio\player_land.mp3)"), _T("player_land"));
	audio_player->load_audio_ex(_T(R"(resources\audio\player_roll.mp3)"), _T("player_roll"));
	audio_player->load_audio_ex(_T(R"(resources\audio\player_run.mp3)"), _T("player_run"));

	audio_player->load_audio_ex(_T(R"(resources\audio\sword_hit_1.mp3)"), _T("sword_hit_1"));
	audio_player->load_audio_ex(_T(R"(resources\audio\sword_hit_2.mp3)"), _T("sword_hit_2"));
	audio_player->load_audio_ex(_T(R"(resources\audio\sword_hit_2.mp3)"), _T("sword_hit_2"));

	audio_player->load_audio_ex(_T(R"(resources\audio\run_loop.wav)"), _T("run_loop"));
	audio_player->load_audio_ex(_T(R"(resources\audio\jump.wav)"), _T("jump"));
	audio_player->load_audio_ex(_T(R"(resources\audio\land.wav)"), _T("land"));

	audio_player->load_audio_ex(_T(R"(resources\audio\dash.wav)"), _T("dash"));
	audio_player->load_audio_ex(_T(R"(resources\audio\fall.mp3)"), _T("fall"));
	audio_player->load_audio_ex(_T(R"(resources\audio\fire.mp3)"), _T("fire_loop"));

	audio_player->load_audio_ex(_T(R"(resources\audio\electric.mp3)"), _T("electric"));
	audio_player->load_audio_ex(_T(R"(resources\audio\wind.mp3)"), _T("wind"));
	audio_player->load_audio_ex(_T(R"(resources\audio\fire_bullet.mp3)"), _T("fire_bullet"));

	audio_player->load_audio_ex(_T(R"(resources\audio\bgm_0.mp3)"), _T("bgm_0"));
	audio_player->load_audio_ex(_T(R"(resources\audio\bgm_1.mp3)"), _T("bgm_1"));
	audio_player->load_audio_ex(_T(R"(resources\audio\bgm_2.mp3)"), _T("bgm_2"));

	audio_player->load_audio_ex(_T(R"(resources\audio\ui_confirm.wav)"), _T("ui_confirm"));
	audio_player->load_audio_ex(_T(R"(resources\audio\ui_switch.wav)"), _T("ui_switch"));
	audio_player->load_audio_ex(_T(R"(resources\audio\hornet_dialogue.wav)"), _T("hornet_dialogue"));
	audio_player->load_audio_ex(_T(R"(resources\audio\hornet_final_yell.wav)"), _T("hornet_final_yell"));
	audio_player->load_audio_ex(_T(R"(resources\audio\hornet_dead.wav)"), _T("hornet_dead"));
	audio_player->load_audio_ex(_T(R"(resources\audio\play_tape.mp3)"), _T("play_tape"));

	// 加载导入字体
	AddFontResourceEx(_T("resources/font/IPix.ttf"), FR_PRIVATE, nullptr);
	settextstyle(25, 0, _T("IPix"));
	setbkmode(TRANSPARENT);
}


void ResourcesManager::blend_atlas(const std::string& id_src, const std::string& id_dst, const std::string& base, float ratio)
{
	IMAGE* image_base = image_pool[base];
	Atlas* atlas_src = atlas_pool[id_src];
	Atlas* atlas_dst = new Atlas;
	for (int i = 0; i < atlas_pool[id_src]->get_size(); ++i)
	{
		IMAGE image_blended;
		IMAGE* image_src = atlas_src->get_image(i);
		bend_image(image_src, &image_blended, image_base, ratio);
		atlas_dst->add_image(image_blended);
	}
	atlas_pool[id_dst] = atlas_dst;
}

void  ResourcesManager::bend_image(IMAGE* src, IMAGE* dst, IMAGE* base, float blend_ratio)
{
	int width = src->getwidth(), height = src->getheight();
	dst->Resize(width, height);
	DWORD* img_src_buffer = GetImageBuffer(src);
	DWORD* img_dst_buffer = GetImageBuffer(dst);
	DWORD* img_base_buffer = GetImageBuffer(base);

	if (base->getwidth() < width || base->getheight() < height)
		throw std::invalid_argument("bend_image base图片过小");

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int idx = y * width + x;
			DWORD color_base = img_base_buffer[idx];
			DWORD color_src = img_src_buffer[idx];
			// 对非透明像素点混叠
			if ((color_src & 0xff000000) >> 24)
			{
				// colorbuff在内存存储顺序是B G R, 所以交换 R B
				BYTE r = (BYTE)(BYTE(GetBValue(color_src)) * blend_ratio + BYTE(GetBValue(color_base)) * (1 - blend_ratio));
				BYTE g = (BYTE)(BYTE(GetGValue(color_src)) * blend_ratio + BYTE(GetGValue(color_base)) * (1 - blend_ratio));
				BYTE b = (BYTE)(BYTE(GetRValue(color_src)) * blend_ratio + BYTE(GetRValue(color_base)) * (1 - blend_ratio));

				// 加上透明通道构成新的像素点	255(不透明)
				img_dst_buffer[idx] = (DWORD)BGR(RGB(r, g, b)) | (DWORD)((BYTE)0xff << 24);
			}
		}
	}
}