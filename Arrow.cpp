#include "Arrow.h"
#include "Camera.h"
#include "Vertex.h"
#include "ShaderDevise.h"
#include "Input.h"
#include "Common.h"

Arrow::Arrow(void) {
	world = Common::identity;
	_start = _end = Common::vec3zero;
	vbuf = NULL;
}

Arrow::~Arrow(void) {
}

Arrow* Arrow::init() {
	auto device = ShaderDevise::device();
	_start = D3DXVECTOR3(0, 2,  0);
	_end = D3DXVECTOR3( 0, -2,  0);

	if(vbuf == NULL) device->CreateVertexBuffer(sizeof(CUSTOMVERTEX)*4, D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &vbuf, NULL);
	updateBuffer();

	return this;
}
void Arrow::update() {
}
void Arrow::draw() {
	auto device = ShaderDevise::device();

	device->SetTransform(D3DTS_WORLD, &world);
	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetStreamSource(0, vbuf, 0, sizeof(CUSTOMVERTEX));
	device->DrawPrimitive(D3DPT_LINELIST, 0, 2);
	device->SetRenderState(D3DRS_LIGHTING, true);

}
void Arrow::release() {
	SAFE_RELEASE(vbuf);
}
const bool Arrow::collision() {
	return _collision;
}
const D3DXVECTOR3& Arrow::start() {
	return _start;
}
const D3DXVECTOR3& Arrow::end() {
	return _end;
}
Arrow& Arrow::start(D3DXVECTOR3& start) {
	_start = start;
	updateBuffer();
	return *this;
}
Arrow& Arrow::end(D3DXVECTOR3& end) {
	_end = end;
	updateBuffer();
	return *this;
}
void Arrow::hitTestPlane(const std::vector<CUSTOMVERTEX>& vertices) {

}
void Arrow::updateBuffer() {
	CUSTOMVERTEX* data = (CUSTOMVERTEX*)Common::getVtxDataWithLock(vbuf);
	data[0] = CUSTOMVERTEX(_start, Common::vec3zero, 0xffff0000, Common::vec2zero);
	data[1] = CUSTOMVERTEX(_end, Common::vec3zero, 0xff0000ff, Common::vec2zero);
	D3DXVECTOR3 dir(_end - _start);
	D3DXVec3Normalize(&dir, &dir);
	D3DXVECTOR3 rev_dir(dir*-1);
	data[2] = CUSTOMVERTEX(_end + rev_dir + D3DXVECTOR3(0, 1, 0), Common::vec3zero, 0xff0000ff, Common::vec2zero);
	data[3] = CUSTOMVERTEX(_end, Common::vec3zero, 0xff0000ff, Common::vec2zero);
	vbuf->Unlock();
}
