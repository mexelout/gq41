#include "StaticPlane.h"
#include "ShaderDevise.h"
#include "Vertex.h"
#include "Camera.h"
#include "Common.h"

StaticPlane::StaticPlane(void) {
	_vtx = NULL;
	_world = Common::identity;
}

StaticPlane::~StaticPlane(void) {
}

StaticPlane* StaticPlane::init() {
	auto device = ShaderDevise::device();

	_vtx = Common::plane();
	CUSTOMVERTEX* data = (CUSTOMVERTEX*)Common::getVtxDataWithLock(_vtx);

	data[0] = CUSTOMVERTEX(-0.5f, 0,  0.5f, 0, 1, 0, 0xffffffff, 0, 0);
	data[1] = CUSTOMVERTEX( 0.5f, 0,  0.5f, 0, 1, 0, 0xffffffff, 1, 0);
	data[2] = CUSTOMVERTEX(-0.5f, 0, -0.5f, 0, 1, 0, 0xffffffff, 0, 1);
	data[3] = CUSTOMVERTEX( 0.5f, 0, -0.5f, 0, 1, 0, 0xffffffff, 1, 1);

	_vtx->Unlock();

	return this;
}

void StaticPlane::update() {
}

void StaticPlane::draw() {
	auto device = ShaderDevise::device();
	device->SetFVF(D3DFVF_CUSTOMVERTEX);

	D3DXMATRIX view = Camera::view(), proj = Camera::projection();

	device->SetTransform(D3DTS_WORLD, &_world);
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	device->SetStreamSource(0, _vtx, 0, sizeof(CUSTOMVERTEX));
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

void StaticPlane::release() {
	SAFE_RELEASE(_vtx);
}

const D3DXMATRIX& StaticPlane::world() {
	return _world;
}
StaticPlane& StaticPlane::world(D3DXMATRIX& world) {
	_world = world;
	return *this;
}
LPDIRECT3DVERTEXBUFFER9 StaticPlane::vtx() {
	return _vtx;
}

