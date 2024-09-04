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
	{"sword_left",	_T(R"(resources\enemy\sword_left\%d.png)"), 3},

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
	return GetImageBuffer(img);
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


void ResourcesManager::flip_image(IMAGE* src, IMAGE* dst, int num)
{
	int width = src->getwidth(), height = src->getheight();
	dst->Resize(width, height);
	DWORD* img_src_buffer = GetImageBuffer(src);
	DWORD* img_dst_buffer = GetImageBuffer(dst);
	// image在逻辑上是由像素点(R,G,B)构成的二维数组,内存上是一维数组
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int src_idx = y * width + width - 1 - x;
			int dst_idx = y * width + x;
			img_dst_buffer[dst_idx] = img_src_buffer[src_idx];
		}
	}
}

void ResourcesManager::flip_image(const std::string& id_src, const std::string& id_dst, int num)
{

}
void ResourcesManager::flip_atlas(const std::string& id_src, const std::string& id_dst)
{

}








void ResourcesManager::load()
{
	for (auto& info : image_info_list)
	{
		IMAGE* image = new IMAGE;
		loadimage(image, info.path);
		if (!check_image_valid(image))
			throw info.path;

		image_pool[info.id] = image;
	}


}