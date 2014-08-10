#pragma once
#include <string>
#include "meshfield.h"

class EditGroundScene;

class GroundMesh : public MeshField {
	friend EditGroundScene;
public:

	GroundMesh(void);
	~GroundMesh(void);
	GroundMesh* init();
	void update();
	void draw();
	void release();
	GroundMesh* inputData(std::string dataname);
	GroundMesh* outputData(std::string dataname);
private:
};

