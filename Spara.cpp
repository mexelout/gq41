#include "Spara.h"
#include "ShaderDevise.h"
#include "Common.h"
#include "Camera.h"
#include "Input.h"

Spara::Spara(void) {
	vec = D3DXVECTOR2(0, 0);
}

Spara::~Spara(void) {
}

Spara* Spara::init() {
	Model::init(ShaderDevise::device(), "models/robo_arm.x");
	_pos.x = -3;
	_pos.z = 0;
	return this;
}
void Spara::update() {
	D3DXVECTOR3 dir(Camera::dir());
	dir.y = 0;
	D3DXVec3Normalize(&dir, &dir);
	bool is_down_key = false;
	float mv_angle = 0;

	if(InputKeyboard::isKey(DIK_W, Input::Press)) {
		if(InputKeyboard::isKey(DIK_D, Input::Press)) {
			mv_angle = D3DX_PI*-0.25f;
		} else if(InputKeyboard::isKey(DIK_A, Input::Press)) {
			mv_angle = D3DX_PI* 0.25f;
		} else {
			mv_angle = D3DX_PI*0.0f;
		}
		is_down_key = true;
	} else if(InputKeyboard::isKey(DIK_S, Input::Press)) {
		if(InputKeyboard::isKey(DIK_D, Input::Press)) {
			mv_angle = D3DX_PI*-0.75f;
		} else if(InputKeyboard::isKey(DIK_A, Input::Press)) {
			mv_angle = D3DX_PI*0.75f;
		} else {
			mv_angle = D3DX_PI*1.0f;
		}
		is_down_key = true;
	} else if(InputKeyboard::isKey(DIK_D, Input::Press)) {
		mv_angle = -D3DX_PI*0.5f;
		is_down_key = true;
	} else if(InputKeyboard::isKey(DIK_A, Input::Press)) {
		mv_angle = D3DX_PI*0.5f;
		is_down_key = true;
	}

	if(!InputMouse::btn(InputMouse::Right, Input::Press))
	if(is_down_key) {
		float s = sinf(mv_angle), c = cosf(mv_angle);
		float x = dir.x * c - dir.z * s, z = dir.x * s + dir.z * c;
		float pow = (float)InputKeyboard::isKey(DIK_LSHIFT, Input::Press);
		if(InputKeyboard::isKey(DIK_LCONTROL, Input::Press)) pow *= 4;
		_pos.x += x*(0.1f + pow * 0.125f);
		_pos.z += z*(0.1f + pow* 0.125f);
	}
}
void Spara::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();

	D3DMATERIAL9 tmp_mat = {0};
	device->GetMaterial(&tmp_mat);

	D3DXMATRIX world, rotX, rotY, translate;
	D3DXMatrixIdentity(&world);

	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&translate, 0.75f, -0.75f, 2);
	world *= translate;
	D3DXMatrixRotationX(&rotX, -vec.x);
	D3DXMatrixRotationY(&rotY, -vec.y + (float)M_PI_2);
	D3DXMatrixTranslation(&translate, _pos.x, _pos.y+2.0f, _pos.z);
	world = world * rotX * rotY * translate;

	device->SetTransform(D3DTS_WORLD, &world);

	LPD3DXMESH mesh = Model::mesh();
	D3DXMATERIAL* mat = (D3DXMATERIAL*)Model::materials()->GetBufferPointer();
	int num_mat = Model::numMaterials();
	std::vector<LPDIRECT3DTEXTURE9> tex = Model::textures();

	device->SetFVF(D3DFVF_CUSTOMMODEL);
	for(int i = 0; i < num_mat; i++) {
		device->SetTexture(0, tex[i]);
		device->SetMaterial(&mat[i].MatD3D);
//		mesh->DrawSubset(i);
	}
	device->SetTexture(0, NULL);
	device->SetMaterial(&tmp_mat);
}
void Spara::release() {
	Player::release();
}

void Spara::setRotate(float x, float y) {
	vec.x = x;
	vec.y = y;
}
