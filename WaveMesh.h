#pragma once
#include "MeshField.h"

class WaveMesh : MeshField {
public:
	WaveMesh(void);
	~WaveMesh(void);
	WaveMesh* init();
	void update();
	void draw();
	void release();
	// amp‚Í‹­‚³
	void addForce(int x, int z, float amp);
private:
	std::vector<float> old_mesh_height;
};

