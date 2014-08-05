#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <string>
#include <d3dx9.h>
#include <vector>

class Model {
public:
	Model(void);
	~Model(void);
	Model* init(LPDIRECT3DDEVICE9 device, std::string filename);
	LPDIRECT3DVERTEXBUFFER9 vertexes();
	LPDIRECT3DINDEXBUFFER9 indices();
	LPD3DXMESH mesh();
	LPD3DXBUFFER materials();
	int numMaterials();
	int numFace();
	void release();
	void drawVolumeShadow();
	std::vector<LPDIRECT3DTEXTURE9> textures();
private:
	LPDIRECT3DVERTEXBUFFER9 _vertexes;
	LPDIRECT3DINDEXBUFFER9 _indices;
	LPD3DXMESH _mesh;
	LPD3DXBUFFER _materials;
	std::vector<LPDIRECT3DTEXTURE9> _textures;
	int _num_materials;
	int _num_vertex;
	int _num_face;
};

