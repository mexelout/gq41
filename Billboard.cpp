#include "Billboard.h"
#include "Vertex.h"
#include "Camera.h"
#include "Common.h"
#include "ShaderDevise.h"

Billboard::Billboard(void) {
	texture = 0;
	color = D3DXCOLOR(1, 1, 1, 1);
	pos = D3DXVECTOR3(0, 0, 0);
	scl = D3DXVECTOR3(1, 1, 1);
	is_disp = true;
}

Billboard::~Billboard(void) {
}

Billboard* Billboard::init() {
	D3DXCreateTextureFromFile(ShaderDevise::device(), "textures/effect.png", &texture);
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	return this;
}
void Billboard::update() {
}
void Billboard::draw(LPDIRECT3DVERTEXBUFFER9 vtx) {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	D3DXMATRIX world, scl, transform, view = Camera::view(), projection = Camera::projection();
	D3DXMATRIX inv_view = getInvView(pos);
	D3DXMatrixScaling(&scl, this->scl.x, this->scl.y, this->scl.z);
	D3DXMatrixTranslation(&transform, pos.x, pos.y, pos.z);
	D3DXMatrixIdentity(&world);
	world *= scl;
	world *= inv_view;
	world *= transform;

	device->SetTransform(D3DTS_WORLD, &world);
	device->SetStreamSource(0, vtx, 0, sizeof(CUSTOMVERTEX));
	device->SetTexture(0, texture);
	device->SetRenderState(D3DRS_LIGHTING, false);

	D3DMATERIAL9 mat, tmpMat;
	ZeroMemory(&mat, sizeof(D3DMATERIAL9));
	mat.Diffuse.r = mat.Diffuse.g = mat.Diffuse.b = mat.Diffuse.a = 1;
	mat.Ambient.r = mat.Ambient.g = mat.Ambient.b = mat.Ambient.a = 1;
	mat.Specular.r = mat.Specular.g = mat.Specular.b = mat.Specular.a = 0;
	mat.Emissive.r = color.r;
	mat.Emissive.g = color.g;
	mat.Emissive.b = color.b;
	mat.Emissive.a = color.a;
	mat.Power = 5;
	device->GetMaterial(&tmpMat);
	device->SetMaterial(&mat);

	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	device->SetMaterial(&tmpMat);
	device->SetTexture(0, 0);
	device->SetRenderState(D3DRS_LIGHTING, true);

}
void Billboard::release() {
	SAFE_RELEASE(texture);
}


// 逆行列作成メソッド
D3DXMATRIX Billboard::getInvView(const D3DXVECTOR3& pos) {
	D3DXMATRIX tmp;
	D3DXMatrixLookAtLH(&tmp, &Camera::eye(), &pos, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(&tmp, NULL, &tmp);
	tmp._41 = 0.0f;
	tmp._42 = 0.0f;
	tmp._43 = 0.0f;
	return tmp;
}

// 座標設定
void Billboard::setPos(const D3DXVECTOR3& pos) {
	this->pos = pos;
}
const D3DXVECTOR3 Billboard::getPos(void) {
	return pos;
}

// 色設定
void Billboard::setColor(D3DXCOLOR color) {
	this->color = color;
}

// スケール設定
void Billboard::setScale(D3DXVECTOR3 scl) {
	this->scl = scl;
}


const D3DXVECTOR3& Billboard::getScale(void) {
	return scl;
}


// 表示の
void Billboard::setDisp(bool disp_flag) {
	is_disp = disp_flag;
}

// 表示するテクスチャの読み込み
void Billboard::loadTexture(const char* filename) {
	SAFE_RELEASE(texture);
	D3DXCreateTextureFromFile(ShaderDevise::device(), filename, &texture);
}
