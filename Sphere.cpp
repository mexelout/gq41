#include "Sphere.h"
#include "Common.h"
#include "ShaderDevise.h"
#include "Model.h"
#include "Vertex.h"

Sphere::Sphere(void) {
	world = Common::identity;
}

Sphere::~Sphere(void) {
}

Sphere* Sphere::init() {
	auto device = ShaderDevise::device();
	model = (new Model)->init(device, "models/sphere.x");

	return this;
}
void Sphere::update() {
}
void Sphere::draw() {
	auto device = ShaderDevise::device();

	D3DXMATERIAL* mat = (D3DXMATERIAL*)model->materials()->GetBufferPointer();
	int num_mat = model->numMaterials();
	device->SetFVF(D3DFVF_CUSTOMMODEL);
	LPD3DXMESH mesh = model->mesh();
	device->SetTransform(D3DTS_WORLD, &world);

	D3DMATERIAL9 tmp_mat;
	device->GetMaterial(&tmp_mat);

	for(int i = 0; i < num_mat; i++) {
		device->SetMaterial(&mat[i].MatD3D);
		mesh->DrawSubset(i);
	}

	device->SetMaterial(&tmp_mat);
}
void Sphere::release() {
	SAFE_RELEASE_DELETE(model);
}

D3DXMATRIX Sphere::getWorld() {
	return world;
}
