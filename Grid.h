#pragma once
#include <d3dx9.h>

class Grid {
public:
	Grid(void);
	~Grid(void);
	Grid* init();
	void update();
	void draw();
	void release();
	Grid* setBlockSize(D3DXVECTOR2 size);
	Grid* setSize(int size);
private:
	D3DXVECTOR2 block_size;
	int size;
	LPDIRECT3DVERTEXBUFFER9 vtx;
	Grid* createGrid();
};

