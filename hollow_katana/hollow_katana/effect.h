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

class EffectTimeRun : public ParticleEffect
{
private:
	IMAGE frame;
	IMAGE* base = nullptr;

public:
	EffectTimeRun();
	~EffectTimeRun();
	void add_image(IMAGE& image);
};
