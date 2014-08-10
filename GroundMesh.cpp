#define _CRT_SECURE_NO_WARNINGS
#include "GroundMesh.h"
#include "ShaderDevise.h"
#include "Common.h"

GroundMesh::GroundMesh(void) {
}

GroundMesh::~GroundMesh(void) {
}

GroundMesh* GroundMesh::init() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	cell_size = 15;
	vertical *= 2;
	horizontal *= 2;
	MeshField::init();

	std::vector<float> vertices;
	for(int i = 0; i < vertical; i++) {
		for(int j = 0; j < horizontal; j++) {
			vertices.push_back(0);
		}
	}

	for(int a = 0; a < 10; a++) {
		for(int i = 0; i < vertical; i++) {
			for(int j = 0; j < horizontal; j++) {
				vertices[i*horizontal+j] += (float)(rand()%100)-50.0f;
			}
		}
		for(int k = 0; k < 3; k++) { 
			for(int i = 0; i < vertical; i++) {
				for(int j = 0; j < horizontal; j++) {
					float left, right, front, back;
					if(j == 0) left = vertices[i*horizontal+j];
					else left = vertices[i*horizontal+j-1];
					if(j == horizontal-1) right = vertices[i*horizontal+j];
					else right = vertices[i*horizontal+j+1];
					if(i == 0) front = vertices[i*horizontal+j];
					else front = vertices[(i-1)*horizontal+j];
					if(i == vertical-1) back = vertices[i*horizontal+j];
					else back = vertices[(i+1)*horizontal+j];
					vertices[i*horizontal+j] = ((vertices[i*horizontal+j] + left + right + front + back) / 4.8f);
				}
			}
		}
	}

	setMeshVertex(vertices);
	calcNor();

	D3DXCreateTextureFromFile(device, "textures/ground.png", &texture);
	return this;
}
void GroundMesh::update() {
	MeshField::update();
}
void GroundMesh::draw() {
	MeshField::draw();
}
void GroundMesh::release() {
	MeshField::release();
}
GroundMesh* GroundMesh::inputData(std::string dataname) {
	FILE* fp = NULL;
	if(dataname.find(".dat", 0)) {
		dataname += ".dat";
	}
	if(dataname.find("dat/", 0)) {
		dataname = "dat/" + dataname;
	}
	fp = fopen(dataname.c_str(),"rb");
	if(fp) {
		int size = 0;
		fread(&size, sizeof(int), 1, fp);
		std::vector<float> v;
		v.resize(size);
		for(int i = 0; i < size; i++)
			fread(&v[i], sizeof(float), 1, fp);
		setMeshVertex(v);
		calcNor();
		fclose(fp);
	}
	return this;
}
GroundMesh* GroundMesh::outputData(std::string dataname) {
	FILE* fp = NULL;
	if(dataname.find(".dat", 0)) {
		dataname += ".dat";
	}
	if(dataname.find("dat/", 0)) {
		dataname = "dat/" + dataname;
	}
	fp = fopen(dataname.c_str(),"wb");
	if(fp) {
		std::vector<float> v = getMeshVertex();

		int size = v.size();
		fwrite(&size, sizeof(int), 1, fp);
		for each(float f in v) fwrite(&f, sizeof(float), 1, fp);

		fclose(fp);
	}
	return this;
}

