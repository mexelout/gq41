#pragma once
#include <vector>
#include "scene.h"
#include "Vertex.h"

class Spara;

class WaveScene : public Scene {
public:
	WaveScene(void);
	~WaveScene(void);
	WaveScene* init();
	void update();
	void draw();
	void release();

private:
	float calcHeight(int t);

	LPDIRECT3DVERTEXSHADER9 vertex_shader;
	LPD3DXCONSTANTTABLE vs_constant_table;

	std::vector<CUSTOMVERTEX> vtx;
	std::vector<D3DXVECTOR3> nors;

	int t;
	float one_radian;

	D3DXVECTOR2 camera_rot;
	float camera_len;

	Spara* spara;
};

