#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <string>
#include <d3dx9.h>

class Model {
public:
	Model(void);
	~Model(void);
	Model* init(LPDIRECT3DDEVICE9 device, std::string filename);
	LPDIRECT3DVERTEXBUFFER9 vertexes();
	LPDIRECT3DINDEXBUFFER9 indices();
	LPD3DXMESH mesh();
	int numFace();
	void release();
private:
	LPDIRECT3DVERTEXBUFFER9 _vertexes;
	LPDIRECT3DINDEXBUFFER9 _indices;
	LPD3DXMESH _mesh;
	int _num_vertex;
	int _num_face;
};

