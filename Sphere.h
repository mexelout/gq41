#pragma once

#include <d3dx9.h>

class Model;

class Sphere {
public:
	Sphere(void);
	~Sphere(void);
	Sphere* init();
	void update();
	void draw();
	void release();

	D3DXMATRIX getWorld();
private:
	Model* model;
	D3DXMATRIX world;
};

