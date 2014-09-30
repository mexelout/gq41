#pragma once
#include "scene.h"
#include "Vertex.h"
#include <vector>

class Spara;
class GroundMesh;
class Billboard;
class Ship;
class WaveMesh;
class CannonBullet;
class ParticleSystem;
class Rectangle2D;

class GameScene : public Scene {
public:
	GameScene(void);
	~GameScene(void);

	GameScene* init();
	void update();
	void draw();
	void release();

private:

	D3DXVECTOR2 camera_rot;
	float camera_len;

	Spara* spara;
	GroundMesh* ground_mesh;
	WaveMesh* wave_mesh;

	Ship* ship;

	ParticleSystem* particle_system;

	CannonBullet** cannon_bullet;

	Rectangle2D* camera_underwater_rectangle;
};

