#pragma once
#include "region.h"
#include "meal.h"
#include "timer.h"
#include "animation.h"
#include "button.h"
#include <unordered_map>
#include <vector>
#include <string>


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
		Meal check(const std::vector<Meal>& arr);
		// 添加新的配方
		void add_branch(const std::vector<Meal>& arr, Meal meal, bool any_karmotrine = false);
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
	Animation anim;								// 摇晃动画
	SDL_Texture* tex_open = nullptr;			// 瓶子打开图片
	SDL_Texture* tex_close = nullptr;			// 瓶子关闭图片
	Status status = Status::Init;				// 调酒过程状态

public:
	BartendBottle();	// 初始化Trie树

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;

	
	void reset();		// 清空调酒瓶
	void modulate();	// 调制饮料

private:
	void shake();
	void stop_shaking();
	Meal get_drink();		// 先排序材料，然后trie树
};





// 调酒菜单		todo
class BartendMeun
{
private:
	Meal meal = Meal::None;

public:
	//virtual void on_render(SDL_Renderer* renderer) override;
	//virtual void on_cursor_down() override;
	//virtual void on_cursor_up() override;
};
