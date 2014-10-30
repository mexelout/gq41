#pragma once

#include <d3dx9.h>

class StaticPlane {
public:
	StaticPlane(void);
	~StaticPlane(void);
	StaticPlane* init();
	void update();
	void draw();
	void release();
	const D3DXMATRIX& world();
	StaticPlane& world(D3DXMATRIX& world);
	LPDIRECT3DVERTEXBUFFER9 vtx();
protected:
	LPDIRECT3DVERTEXBUFFER9 _vtx;
	D3DXMATRIX _world;
};

