#include "Model.h"
#include "Vertex.h"

Model::Model(void) {
	_vertexes = NULL;
	_indices = NULL;
	_num_face = 0;
	_mesh = NULL;
}

Model::~Model(void) {
}

Model* Model::init(LPDIRECT3DDEVICE9 device, std::string filename) {
	D3DXLoadMeshFromX(
		filename.c_str(),
		0,
		device,
		NULL,
		NULL,
		NULL,
		NULL,
		&_mesh);
	_num_face = (int)_mesh->GetNumFaces();
	_mesh->GetVertexBuffer(&_vertexes);
	_mesh->GetIndexBuffer(&_indices);
	return this;
}

LPDIRECT3DVERTEXBUFFER9 Model::vertexes() {
	return _vertexes;
}

LPDIRECT3DINDEXBUFFER9 Model::indices() {
	return _indices;
}

LPD3DXMESH Model::mesh() {
	return _mesh;
}

int Model::numFace() {
	return _num_face;
}

void Model::release() {
	if(_vertexes) _vertexes->Release();
	if(_indices) _indices->Release();
	if(_mesh) _mesh->Release();
}