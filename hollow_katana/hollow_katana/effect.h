#pragma once
#include "particle_effect.h"

class EffectHit : public ParticleEffect
{
public:
	EffectHit(bool is_left);
};

class EffectHurt : public ParticleEffect
{
public:
	EffectHurt(bool is_left);
};

// 需要图片混叠的特效
class EffectBlend : public ParticleEffect
{
protected:
	IMAGE frame;
	IMAGE* base = nullptr;
	float blend_ratio = 0.55f;

public:
	EffectBlend();
	void add_image(IMAGE& image);
};

class EffectBulletTime : public EffectBlend
{
public:
	EffectBulletTime();
};

class EffectRoll : public EffectBlend
{
public:
	EffectRoll();
};

// 对话文本,暂时用粒子特效实现,重写更新,渲染逻辑
class EffectText : public ParticleEffect
{
private:
	Timer timer;
	LPCTSTR text = nullptr;
	IMAGE* background = nullptr;
	COLORREF text_color;
	bool enable_background = true;
	int offset_x = 0;
	int offset_y = 0;

public:
	EffectText(LPCTSTR text, float delta = 1.5f, COLORREF color = RGB(255,255,255));
	virtual void on_update(float delta) override;
	virtual void on_render()  override;
	void set_enable_background(bool flag) { enable_background = flag; }
	void set_text_offset(int x, int y) { offset_x = x, offset_y = y; }
};