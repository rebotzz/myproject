#pragma once
#include "region.h"
#include "meal.h"
#include "timer.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <array>

// 调酒瓶子
class BartendBottle : public Region
{
private:
	struct Node
	{
	public:
		Meal drink = Meal::Unkown;				// 饮料类型
		std::unordered_map<Meal, Node*> nexts;	// k-v: 原料-下一个节点

	public:
		Node() = default;
		Node(Meal target) :drink(target) {}
	};
	class TrieTree
	{
	private:
		Node* root = nullptr;
	public:
		TrieTree();
		~TrieTree();
		// 检测配方是否正确，返回饮料种类
		Meal check(const std::vector<Meal>& material_list);
		// 添加新的配方
		void add_branch(std::vector<Meal>& material_list, Meal target, bool any_karmotrine = false);
		void destroy(Node* node);
	};
	enum class Status
	{
		Init, Shaking, SevereShaking, Done,		// 状态：初始，摇晃、剧烈摇晃、完成
	};

private:
	TrieTree tree;								// 前缀树
	Meal drink = Meal::None;					// 最终饮料成果
	std::vector<Meal> materials;				// 材料列表
	Timer timer_shake;							// 摇晃定时器
	SDL_Texture* tex_open = nullptr;			// 瓶子打开图片
	SDL_Texture* tex_close = nullptr;			// 瓶子关闭图片
	Status status = Status::Init;				// 调酒过程状态

	// 瓶子摇晃动画
	Timer timer_anim;							// 摇晃动画定时器
	std::array<double, 4> frame_list = { 0.0, 30.0, 0.0, -30.0 };	// 摇晃角度
	int frame_idx = 0;							// 摇晃动画当前帧
	const float SHAKE_FRAME_DELTA = 0.25f;		// 摇晃动画帧间隔
	const float SHAKE_CD = 2.5f;				// 调制到剧烈摇晃间隔

public:
	BartendBottle();							// 初始化Trie树

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;

	void reset();								// 清空调酒瓶
	void modulate();							// 调制饮料

private:
	void shake();
	void stop_shaking();
	Meal get_drink();		// 先排序材料，然后trie树
};






