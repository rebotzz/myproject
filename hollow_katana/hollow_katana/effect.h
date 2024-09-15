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

class EffectJump : public ParticleEffect
{
public:
	EffectJump();
};

class EffectLand : public ParticleEffect
{
public:
	EffectLand();
};


class EffectLeaves : public ParticleEffect
{
public:
	EffectLeaves();
};

class EffectElectric : public ParticleEffect
{
public:
	EffectElectric();
};

class EffectElectricAxis : public ParticleEffect
{
public:
	EffectElectricAxis(bool is_left);
};

class EffectFire : public ParticleEffect
{
public:
	EffectFire(bool is_left);
};

class EffectDashLine : public ParticleEffect
{
public:
	EffectDashLine(bool is_left);
};

class EffectSwordHit : public ParticleEffect
{
public:
	EffectSwordHit(bool is_left);
};

class EffectUiChoose : public ParticleEffect
{
public:
	EffectUiChoose(bool is_left);
};

class EffectHurt2 : public ParticleEffect
{
public:
	EffectHurt2();
};
