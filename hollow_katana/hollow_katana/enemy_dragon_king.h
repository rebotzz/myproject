//#pragma once
//#include "character.h"
//
//class EnemyDragonKing : public Character
//{
//private:
//	bool is_dashing = false;
//
//	AnimationGroup animation_dash_vfx;
//	Animation* current_dash_animation = nullptr;
//
//	Animation animation_vfx_slash_left;								// ����������Ч����,��
//	Animation animation_vfx_slash_right;							// ����������Ч����,��
//	Animation* current_slash_animation = nullptr;					// ��ǰ���⶯��
//
//
//
//public:
//	EnemyDragonKing();
//	~EnemyDragonKing();
//	virtual void on_hurt() override;
//
//	virtual void on_input(const ExMessage& msg) override { };
//	virtual void on_update(float delta) override;
//	virtual void on_render() override;
//
//	void on_dash();
//
//
//};