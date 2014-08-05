#pragma once
#include "meshfield.h"

class GroundMesh : public MeshField {
public:
	GroundMesh(void);
	~GroundMesh(void);
	GroundMesh* init();
	void update();
	void draw();
	void release();
private:
};

