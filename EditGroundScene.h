#pragma once
#include "scene.h"
#include "Vertex.h"
#include <vector>

class GroundMesh;
class WaveMesh;

class EditGroundScene : public Scene {
public:
	EditGroundScene(void);
	~EditGroundScene(void);
	EditGroundScene* init();
	void update();
	void draw();
	void release();

private:
	D3DXVECTOR2 camera_rot;
	float camera_len;

	GroundMesh* ground_mesh;
	WaveMesh* wave_mesh;

	int idx;
};

