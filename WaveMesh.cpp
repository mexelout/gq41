#include "WaveMesh.h"
#include "ShaderDevise.h"
#include "Vertex.h"
#include "Common.h"

WaveMesh::WaveMesh(void) {
}

WaveMesh::~WaveMesh(void) {
}

WaveMesh* WaveMesh::init() {
	color = 0x880000ff;
	cell_size = 30;
	MeshField::init();
	old_mesh_height = getMeshVertex();
	std::vector<float> mesh_height = getMeshVertex();

	for(int i = 0; i < 200; i++) {
		if(i % 2) {
			int lottery = Common::random(0, 5);
			float amp = -1.8f;
			int x = Common::random(2, horizontal-2), z = Common::random(2, vertical-2);
			mesh_height[(z-1)*horizontal+x-1] += amp*0.5f;
			mesh_height[(z-1)*horizontal+x] += amp*0.707f;
			mesh_height[(z-1)*horizontal+x+1] += amp*0.5f;
			mesh_height[z*horizontal+x-1] += amp*0.707f;
			mesh_height[z*horizontal+x] += amp;
			mesh_height[z*horizontal+x+1] += amp*0.707f;
			mesh_height[(z+1)*horizontal+x-1] += amp*0.5f;
			mesh_height[(z+1)*horizontal+x] += amp*0.707f;
			mesh_height[(z+1)*horizontal+x+1] += amp*0.5f;
		}

		float	A,B,C;
		float	f[4];
		std::vector<float> ynew;
		for(int i=0;i<vertical;++i) {
			for(int j=0;j<horizontal;++j) {
				if(i==vertical-1)	f[0]=mesh_height[i*horizontal+j];
				else				f[0]=mesh_height[(i+1)*horizontal+j];
				if(i==0)			f[1]=mesh_height[i*horizontal+j];
				else				f[1]=mesh_height[(i-1)*horizontal+j];
				if(j==horizontal-1)	f[2]=mesh_height[i*horizontal+j];
				else				f[2]=mesh_height[i*horizontal+j+1];
				if(j==0)			f[3]=mesh_height[i*horizontal+j];
				else				f[3]=mesh_height[i*horizontal+j-1];
				A = mesh_height[i*horizontal+j];
				B = old_mesh_height[i*horizontal+j];
				C = 0.5f;
				ynew.push_back(C*(f[0]+f[1]+f[2]+f[3])+(2.0f-4.0f*C)*A-B);

				float decrease = 0.995f;
				ynew[i*horizontal+j]*=decrease;
			}
		}
		for(int i=0;i<vertical;++i) {
			for(int j=0;j<horizontal;++j) {
				old_mesh_height[i*horizontal+j] = mesh_height[i*horizontal+j];
				mesh_height[i*horizontal+j] = ynew[i*horizontal+j];
			}
		}
	}
	setMeshVertex(mesh_height);
	calcNor();

	return this;
}
void WaveMesh::update() {
	float	A,B,C;
	float	f[4];

	static int w_count = 0;
	w_count++;

	if(w_count == 2) {
		int lottery = Common::random(0, 5);
		float amp = -1.4f;
		if(lottery == 0) addForce(60 + Common::random(-2, 2), 2, amp);
		else if(lottery == 1) addForce(50 + Common::random(-2, 2), 2, amp);
		else if(lottery == 2) addForce(40 + Common::random(-2, 2), 2, amp);
		else if(lottery == 3) addForce(30 + Common::random(-2, 2), 2, amp);
		else if(lottery == 4) addForce(20 + Common::random(-2, 2), 2, amp);
		else if(lottery == 5) addForce(10 + Common::random(-2, 2), 2, amp);
		w_count = 0;
	}

	std::vector<float> ynew;
	std::vector<float> mesh_height = getMeshVertex();

	for(int i=0;i<vertical;++i) {
		for(int j=0;j<horizontal;++j) {
			if(i==vertical-1)	f[0]=mesh_height[i*horizontal+j];
			else				f[0]=mesh_height[(i+1)*horizontal+j];
			if(i==0)			f[1]=mesh_height[i*horizontal+j];
			else				f[1]=mesh_height[(i-1)*horizontal+j];
			if(j==horizontal-1)	f[2]=mesh_height[i*horizontal+j];
			else				f[2]=mesh_height[i*horizontal+j+1];
			if(j==0)			f[3]=mesh_height[i*horizontal+j];
			else				f[3]=mesh_height[i*horizontal+j-1];
			A = mesh_height[i*horizontal+j];
			B = old_mesh_height[i*horizontal+j];
			C = 0.005f;
			ynew.push_back(C*(f[0]+f[1]+f[2]+f[3])+(2.0f-4.0f*C)*A-B);

			float decrease = 0.995f;
			ynew[i*horizontal+j]*=decrease;
		}
	}
	for(int i=0;i<vertical;++i) {
		for(int j=0;j<horizontal;++j) {
			old_mesh_height[i*horizontal+j] = mesh_height[i*horizontal+j];
			mesh_height[i*horizontal+j] = ynew[i*horizontal+j];
		}
	}
	setMeshVertex(mesh_height);
	calcNor();
	MeshField::update();
}
void WaveMesh::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	D3DMATERIAL9 tmp_mat, mat = {0};
	device->GetMaterial(&tmp_mat);
	mat.Diffuse.r = mat.Diffuse.g = mat.Diffuse.b = mat.Diffuse.a = 1;
	mat.Ambient.r = mat.Ambient.g = mat.Ambient.b = mat.Ambient.a = 0.0f;
	mat.Specular.r = mat.Specular.g = mat.Specular.b = mat.Specular.a = 1;
	mat.Power = 10;
	device->SetMaterial(&mat);
	MeshField::draw();
	device->SetMaterial(&tmp_mat);
}
void WaveMesh::release() {
	MeshField::release();
}

void WaveMesh::addForce(int x, int z, float amp) {
	std::vector<float> mesh_height = getMeshVertex();
	mesh_height[(z-1)*horizontal+x-1] += amp*0.5f;
	mesh_height[(z-1)*horizontal+x] += amp*0.707f;
	mesh_height[(z-1)*horizontal+x+1] += amp*0.5f;
	mesh_height[z*horizontal+x-1] += amp*0.707f;
	mesh_height[z*horizontal+x] += amp;
	mesh_height[z*horizontal+x+1] += amp*0.707f;
	mesh_height[(z+1)*horizontal+x-1] += amp*0.5f;
	mesh_height[(z+1)*horizontal+x] += amp*0.707f;
	mesh_height[(z+1)*horizontal+x+1] += amp*0.5f;
	setMeshVertex(mesh_height);
}
