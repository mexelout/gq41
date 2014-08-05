#pragma once
#include <vector>
#include "model.h"
#include "Vertex.h"

class Arrow {
public:
	Arrow(void);
	~Arrow(void);
	Arrow* init(LPDIRECT3DDEVICE9 device);
	void update();
	void draw(LPDIRECT3DDEVICE9 device);
	void release();
	const bool collision();
	const D3DXVECTOR3& start();
	const D3DXVECTOR3& end();
	void hitTestPlane(const std::vector<CUSTOMVERTEX>& vertices);
private:
	LPDIRECT3DVERTEXBUFFER9 vbuf;
	LPD3DXCONSTANTTABLE _vs_constant_table;
	LPD3DXCONSTANTTABLE _ps_constant_table;
	LPDIRECT3DVERTEXSHADER9 _vs;
	LPDIRECT3DPIXELSHADER9 _ps;
	D3DXMATRIX world;
	D3DXVECTOR3 _start;
	D3DXVECTOR3 _end;
	bool _collision;
//	Model start;
//	Model end;
public:
//	CannonBullet* init(void);
	void draw(void);
};

