#pragma once
#include "region.h"
#include "meal.h"
#include "timer.h"
#include "animation.h"
#include "button.h"
#include <unordered_map>
#include <vector>
#include <string>


// ����ƿ��
class BartendBottle : public Region
{
private:
	struct Node
	{
	public:
		Meal drink = Meal::Unkown;				// ��������
		std::unordered_map<Meal, Node*> nexts;	// k-v: ԭ��-��һ���ڵ�

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
	Meal drink = Meal::None;					// �������ϳɹ�
	std::vector<Meal> materials;				// �����б�
	Timer timer_shake;							// ҡ�ζ�ʱ��
	Animation anim;								// ҡ�ζ���
	SDL_Texture* tex_open = nullptr;			// ƿ�Ӵ�ͼƬ
	SDL_Texture* tex_close = nullptr;			// ƿ�ӹر�ͼƬ
	Status status = Status::Init;				// ���ƹ���״̬

public:
	BartendBottle();	// ��ʼ��Trie��

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;

	
	void reset();		// ��յ���ƿ
	void modulate();	// ��������

private:
	void shake();
	void stop_shaking();
	Meal get_drink();		// ��������ϣ�Ȼ��trie��
};





// ���Ʋ˵�		todo
class BartendMeun
{
private:
	Meal meal = Meal::None;

public:
	//virtual void on_render(SDL_Renderer* renderer) override;
	//virtual void on_cursor_down() override;
	//virtual void on_cursor_up() override;
};
