#include "CannonBullet.h"
#include "MeshField.h"
#include "ShaderDevise.h"
#include "Common.h"
#include "Vertex.h"

CannonBullet::CannonBullet(void) {
	pos = D3DXVECTOR3(0, 0, 0);
	speed = D3DXVECTOR3(0, 0, 0);
	is_hit = false;
	is_use = false;
}

CannonBullet::~CannonBullet(void) {
}

CannonBullet* CannonBullet::init(void) {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	Model::init(device, "models/cannon_bullet.x");
	return this;
}

void CannonBullet::update(void) {
	if(is_use) {
		pos += speed;
		if(!is_hit) {
			speed = D3DXVECTOR3(speed.x * 0.99f, speed.y, speed.z * 0.99f);
			speed.y -= 0.01f;
		} else {
			speed = D3DXVECTOR3(0, 0, 0);
		}
	}
}

void CannonBullet::draw(void) {
	if(is_use) {
		LPDIRECT3DDEVICE9 device = ShaderDevise::device();
		D3DMATERIAL9 tmp_mat = {0};
		device->GetMaterial(&tmp_mat);

		D3DXMATRIX world(Common::identity), translate;

		D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
		world = world * translate;

		device->SetTransform(D3DTS_WORLD, &world);

		LPD3DXMESH mesh = Model::mesh();
		D3DXMATERIAL* mat = (D3DXMATERIAL*)Model::materials()->GetBufferPointer();
		int num_mat = Model::numMaterials();
		std::vector<LPDIRECT3DTEXTURE9> tex = Model::textures();

		device->SetFVF(D3DFVF_CUSTOMMODEL);
		for(int i = 0; i < num_mat; i++) {
			device->SetTexture(0, tex[i]);
			device->SetMaterial(&mat[i].MatD3D);
			mesh->DrawSubset(i);
		}
		device->SetTexture(0, NULL);
		device->SetMaterial(&tmp_mat);
	}
}

void CannonBullet::release(void) {
	Model::release();
}

CannonBullet* CannonBullet::fire(D3DXVECTOR3& pos, D3DXVECTOR3& speed) {
	this->pos = pos;
	this->speed = speed;
	is_hit = false;
	is_use = true;
	return this;
}

CannonBullet* CannonBullet::hitCheckMeshField(MeshField* mesh_field) {
	if(is_use) {
		float height = mesh_field->getHeight(&pos);
		if(pos.y < height) is_hit = true;
		else is_hit = false;
		is_use = !is_hit;
	}
	return this;
}

bool CannonBullet::getHitFlag() {
	return is_hit;
}

bool CannonBullet::getUseFlag() {
	return is_use;
}