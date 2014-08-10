#include "Particle.h"
#include "ShaderDevise.h"

Particle::Particle(void) {
	life = 0;
	max_life = 0;
	anim_max = 0;
	end_scl = D3DXVECTOR3(0, 0, 0);
	start_scl = D3DXVECTOR3(0, 0, 0);
	is_use = false;
	pos = D3DXVECTOR3(0, 0, 0);
	speed = D3DXVECTOR3(0, 0, 0);
	is_gravity = false;
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
		scl = end_scl + (start_scl - end_scl) * ((max_life - life) / (float)max_life);
		if(life >= max_life) {
			is_use = false;
			life = 0;
		}
		pos += speed;
		if(is_gravity)
			speed.y -= 0.0016f;
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
bool Particle::getUse() {
	return is_use;
}
bool Particle::fire(D3DXVECTOR3 pos, D3DXVECTOR3 speed, bool gravity) {
	if(is_use) return false;
	this->pos = pos;
	this->speed = speed;
	is_gravity = gravity;
	return is_use = true;
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
Particle* Particle::setStartScl(D3DXVECTOR3 scl) {
	this->start_scl = scl;
	return this;
}
Particle* Particle::setEndScl(D3DXVECTOR3 scl) {
	this->end_scl = scl;
	return this;
}
Particle* Particle::setSpeed(D3DXVECTOR3 speed) {
	this->speed = speed;
	return this;
}
Particle* Particle::setGravity(bool gravity) {
	this->is_gravity = gravity;
	return this;
}

