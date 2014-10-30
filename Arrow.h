#pragma once
#include <vector>
#include "model.h"
#include "Vertex.h"

class Arrow {
public:
	Arrow(void);
	~Arrow(void);
	Arrow* init();
	void update();
	void draw();
	void release();
	const bool collision();
	const D3DXVECTOR3& start();
	const D3DXVECTOR3& end();
	Arrow& start(D3DXVECTOR3& start);
	Arrow& end(D3DXVECTOR3& end);
	void hitTestPlane(const std::vector<CUSTOMVERTEX>& vertices);
private:
	void updateBuffer();
	LPDIRECT3DVERTEXBUFFER9 vbuf;
	D3DXMATRIX world;
	D3DXVECTOR3 _start;
	D3DXVECTOR3 _end;
	bool _collision;
};

