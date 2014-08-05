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
		D3DXMESH_SYSTEMMEM,
		device,
		NULL,
		&_materials,
		NULL,
		(DWORD*)&_num_materials,
		&_mesh);

	LPD3DXMESH tmp;
	_mesh->CloneMeshFVF(_mesh->GetOptions(), D3DFVF_CUSTOMMODEL, device, &tmp);
	_mesh->Release();
	_mesh = tmp;

	_num_face = (int)_mesh->GetNumFaces();
	_mesh->GetVertexBuffer(&_vertexes);
	_mesh->GetIndexBuffer(&_indices);

	D3DXMATERIAL *material = ( D3DXMATERIAL* )( _materials -> GetBufferPointer() );
	for(int i = 0; i < _num_materials; i++) {
		LPDIRECT3DTEXTURE9 tex = NULL;
		material[i].MatD3D.Ambient = material[i].MatD3D.Diffuse;
		if(material[i].pTextureFilename) {
			D3DXCreateTextureFromFile(device, material[i].pTextureFilename, &tex);
		} else {
			tex = NULL;
		}
		_textures.push_back(tex);
	}

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
	for each(LPDIRECT3DTEXTURE9 tex in _textures)
		if(tex) tex->Release();
	if(_mesh) _mesh->Release();
}

LPD3DXBUFFER Model::materials() {
	return _materials;
}

int Model::numMaterials() {
	return _num_materials;
}

void drawVolumeShadow() {
	
}

std::vector<LPDIRECT3DTEXTURE9> Model::textures() {
	return _textures;
}
