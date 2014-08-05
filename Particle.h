#pragma once
#include "billboard.h"
class Particle : public Billboard {
public:
	Particle(void);
	~Particle(void);
	Particle* init();
	void update();
	void draw(LPDIRECT3DVERTEXBUFFER9 vtx);
	void release();
	int getAnimNum();
	Particle* setAnimNumMax(int anim_max);
	Particle* setUse(bool use);
	Particle* setMaxLife(int max_life);
	Particle* setLife(int life);
	Particle* setMaxScl(D3DXVECTOR3 max_scl);
	Particle* setMinScl(D3DXVECTOR3 min_scl);
private:
	int life;
	int max_life;
	int anim_max;
	D3DXVECTOR3 min_scl;
	D3DXVECTOR3 max_scl;
	bool is_use;
};

