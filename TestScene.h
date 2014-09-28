#pragma once
#include "scene.h"
#include <vector>
#include <d3dx9.h>

class Particle;
class Grid;
class ParticleSystem;
class Rectangle2D;


class TestScene : public Scene {
public:
	TestScene(void);
	~TestScene(void);
	TestScene* init();
	void update();
	void draw();
	void release();

	Grid* grid;

	D3DXVECTOR2 camera_rot;
	float camera_len;

	ParticleSystem* particle_system;

	Rectangle2D* rectangle;
};

