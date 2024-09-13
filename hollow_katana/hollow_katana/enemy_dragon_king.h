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
//	Animation animation_vfx_slash_left;								// 攻击刀光特效动画,左
//	Animation animation_vfx_slash_right;							// 攻击刀光特效动画,右
//	Animation* current_slash_animation = nullptr;					// 当前刀光动画
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