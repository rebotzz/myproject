#include "resources_manager.h"
#include "util.h"

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
	{"background",					_T(R"(resources\background.png)")},
	{"ui_heart",					_T(R"(resources\ui_heart.png)")},

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
	{"player_vfx_land",				_T(R"(resources\player\vfx_land.png)") }
};


static const std::vector<AtlasResInfo> atlas_info_list =
{
	{"barb_break",	_T(R"(resources\enemy\barb_break\%d.png)"), 3},
	{"barb_loose",	_T(R"(resources\enemy\barb_loose\%d.png)"), 5},
	{"silk",		_T(R"(resources\enemy\silk\%d.png)"),		9},
	{"sword_left",	_T(R"(resources\enemy\sword\%d.png)"),		3},

	{"enemy_aim_left",			  _T(R"(resources\enemy\aim\%d.png)"),			 9},
	{"enemy_dash_in_air_left",	  _T(R"(resources\enemy\dash_in_air\%d.png)"),   2},
	{"enemy_dash_on_floor_left",  _T(R"(resources\enemy\dash_on_floor\%d.png)"), 2},
	{"enemy_fall_left",			  _T(R"(resources\enemy\fall\%d.png)"),			 4},
	{"enemy_idle_left",			  _T(R"(resources\enemy\idle\%d.png)"),			 6},
	{"enemy_jump_left",			  _T(R"(resources\enemy\jump\%d.png)"),			 8},

	{"enemy_run_left",			  _T(R"(resources\enemy\run\%d.png)"),			 8},
	{"enemy_squat_left",		  _T(R"(resources\enemy\squat\%d.png)"),		10},
	{"enemy_throw_barb_left",	  _T(R"(resources\enemy\throw_barb\%d.png)"),	 8},
	{"enemy_throw_silk_left",	  _T(R"(resources\enemy\throw_silk\%d.png)"),	17},
	{"enemy_throw_sword_left",	  _T(R"(resources\enemy\throw_sword\%d.png)"),  16},

	{"enemy_vfx_dash_in_air_left",	  _T(R"(resources\enemy\vfx_dash_in_air\%d.png)"),    5},
	{"enemy_vfx_dash_on_floor_left",  _T(R"(resources\enemy\vfx_dash_on_floor\%d.png)"),  6},
};

static inline bool check_image_valid(IMAGE* img)
{
	// vector中的IMAGE图片缓冲区好像有bug,默认不是nullptr,所以辅助图片尺寸判断
	return GetImageBuffer(img) && img->getheight() &&img->getwidth();
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
		atlas->load_from_file(info.path, info.num_frame);

		for (int i = 0; i < atlas->get_size(); ++i)
		{
			IMAGE* image = atlas->get_image(i);

			if (!check_image_valid(image))
				throw info.path;
		}

		atlas_pool[info.id] = atlas;
	}

	// 增加翻转图片
	flip_image("player_attack_right", "player_attack_left", 5);
	flip_image("player_dead_right",   "player_dead_left",   6);
	flip_image("player_fall_right",   "player_fall_left",   5);
	flip_image("player_idle_right",   "player_idle_left",   5);
	flip_image("player_jump_right",   "player_jump_left",   5);
	flip_image("player_run_right",    "player_run_left",   10);
	flip_image("player_roll_right",   "player_roll_left",   7);

	flip_atlas("sword_left",					"sword_right");					
	flip_atlas("enemy_aim_left",				"enemy_aim_right");				
	flip_atlas("enemy_dash_in_air_left",		"enemy_dash_in_air_right");		
	flip_atlas("enemy_dash_on_floor_left",		"enemy_dash_on_floor_right");		
	flip_atlas("enemy_fall_left",				"enemy_fall_right");				

	flip_atlas("enemy_jump_left",				"enemy_jump_right");				
	flip_atlas("enemy_run_left",				"enemy_run_right");				
	flip_atlas("enemy_squat_left",				"enemy_squat_right");				
	flip_atlas("enemy_throw_barb_left",			"enemy_throw_barb_right");			
	flip_atlas("enemy_throw_silk_left",			"enemy_throw_silk_right");			
	flip_atlas("enemy_throw_sword_left",		"enemy_throw_sword_right");		

	flip_atlas("enemy_vfx_dash_in_air_left",	"enemy_vfx_dash_in_air_right");	
	flip_atlas("enemy_vfx_dash_on_floor_left",  "enemy_vfx_dash_on_floor_right");  

	// 加载音频
	load_audio(_T(R"(resources\audio\bgm.mp3)"),		 _T("bgm"));
	load_audio(_T(R"(resources\audio\barb_break.mp3)"),  _T("barb_break"));
	load_audio(_T(R"(resources\audio\bullet_time.mp3)"), _T("bullet_time"));

	load_audio(_T(R"(resources\audio\enemy_dash.mp3)"),        _T("enemy_dash"));
	load_audio(_T(R"(resources\audio\enemy_run.mp3)"),         _T("enemy_run"));
	load_audio(_T(R"(resources\audio\enemy_hurt_1.mp3)"),      _T("enemy_hurt_1"));
	load_audio(_T(R"(resources\audio\enemy_hurt_2.mp3)"),      _T("enemy_hurt_2"));
	load_audio(_T(R"(resources\audio\enemy_hurt_3.mp3)"),      _T("enemy_hurt_3"));
	load_audio(_T(R"(resources\audio\enemy_throw_barbs.mp3)"), _T("enemy_throw_barbs"));
	load_audio(_T(R"(resources\audio\enemy_throw_silk.mp3)"),  _T("enemy_throw_silk"));
	load_audio(_T(R"(resources\audio\enemy_throw_sword.mp3)"), _T("enemy_throw_sword"));

	load_audio(_T(R"(resources\audio\player_attack_1.mp3)"),   _T("player_attack_1"));
	load_audio(_T(R"(resources\audio\player_attack_2.mp3)"),   _T("player_attack_2"));
	load_audio(_T(R"(resources\audio\player_attack_3.mp3)"),   _T("player_attack_3"));
	load_audio(_T(R"(resources\audio\player_dead.mp3)"),       _T("player_dead"));
	load_audio(_T(R"(resources\audio\player_hurt.mp3)"),       _T("player_hurt"));
	load_audio(_T(R"(resources\audio\player_jump.mp3)"),       _T("player_jump"));
	load_audio(_T(R"(resources\audio\player_land.mp3)"),       _T("player_land"));
	load_audio(_T(R"(resources\audio\player_roll.mp3)"),       _T("player_roll"));
	load_audio(_T(R"(resources\audio\player_run.mp3)"),        _T("player_run"));

}