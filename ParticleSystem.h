#pragma once
#include <vector>
#include <d3dx9.h>
#include "ShaderPack.hpp"

class Particle;

class ParticleSystem {
public:
	ParticleSystem(void);
	~ParticleSystem(void);

	ParticleSystem* init();
	void update();
	void draw();
	void release();

	void fire(D3DXVECTOR3 pos = D3DXVECTOR3(0, 1, 0), D3DXVECTOR3 speed = D3DXVECTOR3(0, 0, 0), bool gravity = false);

	ParticleSystem* loadTexture(std::string filename);
	ParticleSystem* setMaxLife(int life);
	ParticleSystem* setStartScale(D3DXVECTOR3 scl);
	ParticleSystem* setEndScale(D3DXVECTOR3 scl);
	ParticleSystem* setAnimationAmount(int amount);
	ParticleSystem* setParticleAmount(int amount);
	ParticleSystem* setForceRange(D3DXVECTOR3 vec, float range);
private:
	std::vector<Particle*> particles;
	std::vector<LPDIRECT3DVERTEXBUFFER9> vtxs;

	std::string filename;
	int anm_num;
	int max_life;
	D3DXVECTOR3 start_scl;
	D3DXVECTOR3 end_scl;
	D3DXVECTOR3 force_vec;
	float force_range;

	ShaderPack ps_alpha_test_clip;
};

