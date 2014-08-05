#pragma once
#include "scene.h"
#include "Vertex.h"
#include <vector>

class Spara;
class GroundMesh;
class Billboard;
class Ship;
class WaveMesh;
class Model;
class CannonBullet;

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

	Billboard** bill_board;

	Ship* ship;

	D3DXVECTOR3 effect_pos;

	CannonBullet** cannon_bullet;

	LPDIRECT3DVERTEXBUFFER9 vtx;
};

