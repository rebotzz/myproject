#pragma once
#include "region.h"
#include "meal.h"
#include "timer.h"
#include "animation.h"
#include <unordered_map>
#include <vector>


// ����ԭ��
class RawMaterial : public Region
{
protected:
	int count = 0;						// ԭ�ϼ������
	SDL_Color color;					// ������ɫ
	Meal meal = Meal::None;				// ԭ������

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

// ���Ʋ˵�
class BartendMeun 
{
private:
	Meal meal = Meal::None;

public:
	//virtual void on_render(SDL_Renderer* renderer) override;
	//virtual void on_cursor_down() override;
	//virtual void on_cursor_up() override;
};


// ����ƿ��
class BartendBottle : public Region
{
	// ��ʱ����ƿ��ҡ�ζ������ύ����ʱƿ�Ӵ�ͼƬ����¼ƿ���ڲ�����
	// ȷ��������ɵ���������: tries����	�Ǿ��Ȳ�Ū�������ϵĴ���
private:
	struct Node
	{
	public:
		Meal drink = Meal::Unkown;
		std::unordered_map<Meal, Node*> nexts;	// maybe֮��ȷ��������Ŀ�󣬸�Ϊarray, ����

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
		// ����䷽�Ƿ���ȷ��������������
		Meal check(const std::vector<Meal>& arr);
		// ����µ��䷽
		void add_branch(const std::vector<Meal>& arr, Meal meal, bool any_karmotrine = false);

		void destroy(Node* node);
	};

	enum class Status
	{
		Init, Shaking, SevereShaking, Done,		// ״̬����ʼ��ҡ�Ρ�����ҡ�Ρ����
	};

private:
	TrieTree tree;								// ǰ׺��
	Meal meal = Meal::None;						// �������ϳɹ�
	std::vector<Meal> materials;				// �����б�
	Timer timer_shake;							// ҡ�ζ�ʱ��
	Animation anim;								// ҡ�ζ���
	SDL_Texture* tex_open = nullptr;			// ƿ�Ӵ�ͼƬ
	SDL_Texture* tex_close = nullptr;			// ƿ�ӹر�ͼƬ
	Status status = Status::Init;

public:
	BartendBottle();	// ��ʼ��Trie��

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;

	// ��յ���ƿ
	void reset();
	void shake();
	void stop_shaking();
	Meal get_drink();		// ��������ϣ�Ȼ��trie��
};

// ��ť
class Button : public Region
{

};

// ������ť
class RedoDrink : public Button
{

};


// ���ư�ť/ֹͣҡ�ΰ�ť/�Ͼư�ť  ���Թ�Ϊһ�࣬�ò�ͬ״̬����
// ҡ�ΰ�ť
class ShakeBottle : public Button
{
	
};

// ֹͣҡ�ΰ�ť
class StopShaking : public Button
{
	// ����ԭ�ϣ�ȷ�����ɵľ�����
};

// �Ͼư�ť
class SubmitDrink : public Button
{

};

// �ӱ���ť

// �»���ť

