#pragma once
#include "MeshField.h"

class WaveMesh : public MeshField {
public:
	WaveMesh(void);
	~WaveMesh(void);
	WaveMesh* init();
	void update();
	void draw();
	void release();
	// amp�͋���
	void addForce(int x, int z, float amp);
private:
	std::vector<float> old_mesh_height;
	D3DXVECTOR2 uv_offset;
};

