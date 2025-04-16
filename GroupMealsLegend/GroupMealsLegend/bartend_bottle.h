#pragma once
#include "region.h"
#include "meal.h"
#include "timer.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <array>

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
		Meal check(const std::vector<Meal>& material_list);
		// ����µ��䷽
		void add_branch(std::vector<Meal>& material_list, Meal target, bool any_karmotrine = false);
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
	SDL_Texture* tex_open = nullptr;			// ƿ�Ӵ�ͼƬ
	SDL_Texture* tex_close = nullptr;			// ƿ�ӹر�ͼƬ
	Status status = Status::Init;				// ���ƹ���״̬

	// ƿ��ҡ�ζ���
	Timer timer_anim;							// ҡ�ζ�����ʱ��
	std::array<double, 4> frame_list = { 0.0, 30.0, 0.0, -30.0 };	// ҡ�νǶ�
	int frame_idx = 0;							// ҡ�ζ�����ǰ֡
	const float SHAKE_FRAME_DELTA = 0.25f;		// ҡ�ζ���֡���
	const float SHAKE_CD = 2.5f;				// ���Ƶ�����ҡ�μ��

public:
	BartendBottle();							// ��ʼ��Trie��

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;

	void reset();								// ��յ���ƿ
	void modulate();							// ��������

private:
	void shake();
	void stop_shaking();
	Meal get_drink();		// ��������ϣ�Ȼ��trie��
};






