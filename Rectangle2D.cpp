#include "Rectangle2D.h"
#include "ShaderDevise.h"
#include "Common.h"
#include "Vertex.h"
#include "TextureManager.h"

Rectangle2D::Rectangle2D(void) {
	vtx = NULL;
	size = Common::vec2zero;
	tex_num = 0;
}


Rectangle2D::~Rectangle2D(void) {
}

Rectangle2D* Rectangle2D::init() {
	auto device = ShaderDevise::device();

	vtx = Common::plane();
	pos = Common::vec3zero;
	size = D3DXVECTOR2(256, 256);
	angle = 0;
	//tex_num = TextureManager::inst().loadTexture("filename", device);

	return this;
}

void Rectangle2D::update() {
}

void Rectangle2D::draw() {
	auto device = ShaderDevise::device();
	device->SetStreamSource(0, vtx, 0, sizeof(CUSTOMVERTEX));
	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	device->SetRenderState(D3DRS_LIGHTING, false);
	TextureManager::inst().applyTexture(tex_num, 0, device);
	D3DXMATRIX w, s, r, t;
	D3DXMatrixScaling(&s, size.x, size.y, 1);
	D3DXMatrixRotationZ(&r, angle);
	D3DXMatrixTranslation(&t, pos.x, pos.y,pos.z);
	w = s * r * t;
	device->SetTransform(D3DTS_WORLD, &w);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	device->SetRenderState(D3DRS_LIGHTING, true);
	TextureManager::inst().applyTexture(0, 0, device);
}

void Rectangle2D::release() {
	SAFE_RELEASE(vtx);
}

Rectangle2D* Rectangle2D::getPos(D3DXVECTOR3& pos) {
	pos = this->pos;
	return this;
}
Rectangle2D* Rectangle2D::getSize(D3DXVECTOR2& size) {
	size = this->size;
	return this;
}
Rectangle2D* Rectangle2D::getAngle(float& angle) {
	angle = this->angle;
	return this;
}
Rectangle2D* Rectangle2D::setPos(D3DXVECTOR3 pos) {
	this->pos = pos;
	return this;
}
Rectangle2D* Rectangle2D::setSize(D3DXVECTOR2 size) {
	this->size = size;
	return this;
}
Rectangle2D* Rectangle2D::setAngle(float angle) {
	this->angle = angle;
	return this;
}
Rectangle2D* Rectangle2D::setColor(D3DXCOLOR color) {
	CUSTOMVERTEX* data = (CUSTOMVERTEX*)Common::getVtxDataWithLock(vtx);

	data[0].color = color;
	data[1].color = color;
	data[2].color = color;
	data[3].color = color;

	vtx->Unlock();

	return this;
}
Rectangle2D* Rectangle2D::loadTexture(std::string filename) {
	return this;
}