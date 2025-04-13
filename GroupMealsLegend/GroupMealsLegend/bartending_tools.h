#pragma once
#include "region.h"
#include "meal.h"
#include "timer.h"
#include "animation.h"
#include <unordered_map>
#include <vector>


// 调酒原料
class RawMaterial : public Region
{
protected:
	int count = 0;						// 原料加入份量
	SDL_Color color;					// 绘制颜色
	Meal meal = Meal::None;				// 原料类型

public:
	RawMaterial(Meal _meal, SDL_Rect _rect, SDL_Color _color);

	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_down() override;
	virtual void on_cursor_up() override;
};

class Adelhyde : public RawMaterial
{
public:
	Adelhyde() :RawMaterial(Meal::Adelhyde, { 810,330, 145, 115 }, { 225, 40, 65, 255 }) {}
};

class BronsonExt : public RawMaterial
{
public:
	BronsonExt() :RawMaterial(Meal::BronsonExt, { 960,330, 145, 115 }, { 255, 200, 60, 255 }) {}
};

class PwdDelta : public RawMaterial
{
public:
	PwdDelta() :RawMaterial(Meal::PwdDelta, { 1110,330, 145, 115 }, { 135, 165, 225, 255 }) {}
};

class Flanergide : public RawMaterial
{
public:
	Flanergide() :RawMaterial(Meal::Flanergide, { 810,450, 145, 115 }, { 160, 200, 115, 255 }) {}
};

class Karmotrine : public RawMaterial
{
public:
	Karmotrine() :RawMaterial(Meal::Karmotrine, { 1110,450, 145, 115 }, { 200, 230, 230, 255 }) {}
};

// 调酒菜单
class BartendMeun 
{
private:
	Meal meal = Meal::None;

public:
	//virtual void on_render(SDL_Renderer* renderer) override;
	//virtual void on_cursor_down() override;
	//virtual void on_cursor_up() override;
};


// 调酒瓶子
class BartendBottle : public Region
{
	// 定时器，瓶子摇晃动画，提交材料时瓶子打开图片，记录瓶子内部材料
	// 确定最后生成的饮料种类: tries树？	那就先不弄两倍材料的大杯了
private:
	struct Node
	{
	public:
		Meal drink = Meal::Unkown;
		std::unordered_map<Meal, Node*> nexts;	// maybe之后确定材料数目后，改为array, 有序，

	public:
		Node() = default;
		Node(Meal target):drink(target) {}
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
	Meal meal = Meal::None;						// 最终饮料成果
	std::vector<Meal> materials;				// 材料列表
	Timer timer_shake;							// 摇晃定时器
	Animation anim;								// 摇晃动画
	SDL_Texture* tex_open = nullptr;			// 瓶子打开图片
	SDL_Texture* tex_close = nullptr;			// 瓶子关闭图片
	Status status = Status::Init;

public:
	BartendBottle();	// 初始化Trie树

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;

	// 清空调酒瓶
	void reset();
	void shake();
	void stop_shaking();
	Meal get_drink();		// 先排序材料，然后trie树
};

// 按钮
class Button : public Region
{

};

// 重做按钮
class RedoDrink : public Button
{

};


// 调制按钮/停止摇晃按钮/上酒按钮  可以归为一类，用不同状态区分
// 摇晃按钮
class ShakeBottle : public Button
{
	
};

// 停止摇晃按钮
class StopShaking : public Button
{
	// 检验原料，确定生成的酒种类
};

// 上酒按钮
class SubmitDrink : public Button
{

};

// 加冰按钮

// 陈化按钮

