#pragma once
#include "Scene.h"
#include "Plane.h"

class MultiPlane : public Scene {
public:
	MultiPlane(void);
	~MultiPlane(void);
	MultiPlane* init();
	void update();
	void draw();
	void release();
private:
	Plane* plane1;
	Plane* plane2;
	Plane* plane3;
	Plane* plane4;
	Plane* plane5;
	LPDIRECT3DVERTEXSHADER9 vertex_shader;
	LPD3DXCONSTANTTABLE vs_constant_table;
};

