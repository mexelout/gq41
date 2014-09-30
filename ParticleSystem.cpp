#include "ParticleSystem.h"
#include "Particle.h"
#include "Common.h"
#include "ShaderDevise.h"

ParticleSystem::ParticleSystem(void) {
	force_vec = D3DXVECTOR3(0, 0, 0);
	force_range = 0;
	filename = "";
	anm_num = 0;
	max_life = 0;
	start_scl = D3DXVECTOR3(0, 0, 0);
	end_scl = D3DXVECTOR3(0, 0, 0);
}
ParticleSystem::~ParticleSystem(void) {
}

ParticleSystem* ParticleSystem::init() {
	// alphaが0.5f以下になったら点を打たないピクセルシェーダ
	ps_alpha_test_clip.loadFunc(ShaderDevise::device(), "alpha_test_clip.hlsl", "psAlphaTestClip", "ps_2_0");
	return loadTexture("textures/effect_bumb.png")
			->setMaxLife(12)
			->setEndScale(D3DXVECTOR3(1, 1, 1))
			->setStartScale(D3DXVECTOR3(1, 1, 1))
			->setAnimationAmount(8)
			->setParticleAmount(10);
}
void ParticleSystem::update() {
	for each(Particle* p in particles) p->update();
}
void ParticleSystem::draw() {
	auto device = ShaderDevise::device();
	device->SetPixelShader(ps_alpha_test_clip.ps);
	for each(Particle* p in particles)
		p->draw(vtxs[p->getAnimNum()]);
	device->SetPixelShader(NULL);
}
void ParticleSystem::release() {
	for each(Particle* p in particles) SAFE_RELEASE_DELETE(p);
	particles.clear();
	for each(LPDIRECT3DVERTEXBUFFER9 v in vtxs) SAFE_RELEASE(v);
	vtxs.clear();
	std::vector<LPDIRECT3DVERTEXBUFFER9>().swap (vtxs);
	ps_alpha_test_clip.release();
}

void ParticleSystem::fire(D3DXVECTOR3 pos, D3DXVECTOR3 speed, bool gravity) {
	for each(Particle* p in particles) {
		if(p->fire(pos, speed, gravity)) break;
	}
}

ParticleSystem* ParticleSystem::loadTexture(std::string filename) {
	this->filename = filename;
	for each(Particle* p in particles) p->loadTexture(filename.c_str());
	return this;
}
ParticleSystem* ParticleSystem::setAnimationAmount(int amount) {
	for each(LPDIRECT3DVERTEXBUFFER9 v in vtxs) SAFE_RELEASE(v);
	vtxs.clear();
	for(int i = 0; i < amount; i++) {
		vtxs.push_back(Common::plane(D3DXVECTOR3(1, 1, 1), D3DXVECTOR2(1.0f/amount*i, 0), D3DXVECTOR2(1.0f/amount, 1)));
	}
	anm_num = amount;
	for each(Particle* p in particles) p->setAnimNumMax(amount);
	return this;
}
ParticleSystem* ParticleSystem::setMaxLife(int life) {
	max_life = life;
	for each(Particle* p in particles) p->setMaxLife(max_life);
	return this;
}
ParticleSystem* ParticleSystem::setStartScale(D3DXVECTOR3 scl) {
	start_scl = scl;
	for each(Particle* p in particles) p->setStartScl(start_scl);
	return this;
}
ParticleSystem* ParticleSystem::setEndScale(D3DXVECTOR3 scl) {
	end_scl = scl;
	for each(Particle* p in particles) p->setEndScl(end_scl);
	return this;
}
ParticleSystem* ParticleSystem::setForceRange(D3DXVECTOR3 vec, float range) {
	force_vec = vec;
	force_range = range;
	return this;
}
ParticleSystem* ParticleSystem::setParticleAmount(int amount) {
	for each(Particle* p in particles) SAFE_RELEASE_DELETE(p);
	particles.clear();
	for(int i = 0; i < amount; i++) {
		particles.push_back((new Particle)->init());
		particles[i]->loadTexture(filename.c_str());
		particles[i]->setMaxLife(max_life)
			->setLife(0)
			->setEndScl(end_scl)
			->setStartScl(start_scl)
			->setAnimNumMax(anm_num)
			->setUse(false);
	}
	return this;
}
