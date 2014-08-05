#include "Particle.h"
#include "ShaderDevise.h"

Particle::Particle(void) {
	life = 0;
	max_life = 0;
	min_scl = D3DXVECTOR3(0, 0, 0);
	max_scl = D3DXVECTOR3(0, 0, 0);
	is_use = false;
}

Particle::~Particle(void) {
}

Particle* Particle::init() {
	Billboard::init();
	return this;
}
void Particle::update() {
	if(is_use) {
		life++;
		scl = min_scl + (max_scl - min_scl) * ((max_life - life) / (float)max_life);
		if(life >= max_life) {
			is_use = false;
		}
	}
	Billboard::update();
}
void Particle::draw(LPDIRECT3DVERTEXBUFFER9 vtx) {
	if(is_use) {
		Billboard::draw(vtx);
	}
}
void Particle::release() {
	Billboard::release();
}

int Particle::getAnimNum() {
	int num = (int)((float)life / (float)max_life * (anim_max));
	if(num >= anim_max) num -= 1;
	return num;
}
Particle* Particle::setAnimNumMax(int anim_max) {
	this->anim_max = anim_max;
	return this;
}
Particle* Particle::setUse(bool use) {
	is_use = use;
	return this;
}
Particle* Particle::setMaxLife(int max_life) {
	this->max_life = max_life;
	return this;
}
Particle* Particle::setLife(int life) {
	this->life = life;
	return this;
}
Particle* Particle::setMaxScl(D3DXVECTOR3 max_scl) {
	this->max_scl = max_scl;
	return this;
}
Particle* Particle::setMinScl(D3DXVECTOR3 min_scl) {
	this->min_scl = min_scl;
	return this;
}

