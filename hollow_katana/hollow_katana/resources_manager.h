#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <easyx.h>
#include "atlas.h"

// ��Դ����������
class ResourcesManager
{
private:
	static ResourcesManager* manager;
	std::unordered_map<std::string, IMAGE*> image_pool;
	std::unordered_map<std::string, Atlas*> atlas_pool;

private:
	ResourcesManager();
	~ResourcesManager();

	void flip_image(IMAGE* src, IMAGE* dst, int num = 1);
	void flip_image(const std::string& id_src, const std::string& id_dst, int num = 1);
	void flip_atlas(const std::string& id_src, const std::string& id_dst);

public:
	static ResourcesManager* instance();
	void load();
	IMAGE* find_image(const std::string& id);
	Atlas* find_atlas(const std::string& id);
};