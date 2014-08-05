#pragma once
#include <d3dx9.h>

typedef struct CUSTOMVERTEX {
	CUSTOMVERTEX() {
		pos   = D3DXVECTOR3(0, 0, 0);
		nor   = D3DXVECTOR3(0, 0, 0);
		color = 0xffffffff;
		tex   = D3DXVECTOR2(0, 0);
	};
	CUSTOMVERTEX(D3DXVECTOR3 pos, D3DXVECTOR3 nor, DWORD color, D3DXVECTOR2 tex) {
		this->pos   = pos;
		this->nor   = nor;
		this->color = color;
		this->tex   = tex;
	};
	CUSTOMVERTEX(float px, float py, float pz, float nx, float ny, float nz, DWORD color, float tx, float ty) {
		pos   = D3DXVECTOR3(px, py, pz);
		nor   = D3DXVECTOR3(nx, ny, nz);
		this->color = color;
		tex   = D3DXVECTOR2(tx, ty);
	};

	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	DWORD color;
	D3DXVECTOR2 tex;
}CUSTOMVERTEX;

typedef struct CUSTOMVERTEX2D {
}CUSTOMVERTEX2D;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_CUSTOMVERTEX2D (D3DFVF_XYZRHW|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_CUSTOMMODEL (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

