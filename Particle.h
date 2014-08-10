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
	bool getUse();
	bool fire(D3DXVECTOR3 pos = D3DXVECTOR3(0, 1, 0), D3DXVECTOR3 speed = D3DXVECTOR3(0, 1, 0), bool gravity = false);
	Particle* setAnimNumMax(int anim_max);
	Particle* setUse(bool use);
	Particle* setMaxLife(int max_life);
	Particle* setLife(int life);
	Particle* setStartScl(D3DXVECTOR3 scl);
	Particle* setEndScl(D3DXVECTOR3 scl);
	Particle* setSpeed(D3DXVECTOR3 speed);
	Particle* setGravity(bool gravity);
private:
	int life;
	int max_life;
	int anim_max;
	D3DXVECTOR3 end_scl;
	D3DXVECTOR3 start_scl;
	bool is_use;
	D3DXVECTOR3 speed;
	bool is_gravity;
};

