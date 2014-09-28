#pragma once
#include <vector>
#include <d3dx9.h>

class MeshField {
public:
	MeshField(void);
	~MeshField(void);
	virtual MeshField* init();
	virtual void update();
	virtual void draw();
	virtual void release();
	float getHeight(const D3DXVECTOR3* pos);
protected:
	void calcNor();
	LPDIRECT3DVERTEXBUFFER9 vtx;
	LPDIRECT3DINDEXBUFFER9 idx;
	LPDIRECT3DTEXTURE9 texture;

	int horizontal, vertical, primitive_num;
	float cell_size;
	D3DCOLOR color;
	std::vector<D3DXVECTOR3> nors;
	// 頂点メッシュの高さの設定
	void setMeshVertex(std::vector<float> vertices, D3DXVECTOR2 uv_offset = D3DXVECTOR2(0, 0));
	// 頂点メッシュの高さの取得
	std::vector<float> getMeshVertex();
};

