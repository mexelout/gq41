#include "Grid.h"
#include "ShaderDevise.h"
#include "Vertex.h"
#include "Common.h"

Grid::Grid(void) {
	block_size = D3DXVECTOR2(0, 0);
	size = 0;
	vtx = NULL;
}
Grid::~Grid(void) {
}
Grid* Grid::init() {
	return setBlockSize(D3DXVECTOR2(1, 1))->setSize(4);
}
void Grid::update() {
}
void Grid::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	device->SetRenderState(D3DRS_LIGHTING, false);

	device->SetStreamSource(0, vtx, 0, sizeof(CUSTOMVERTEX));
	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	device->SetTransform(D3DTS_WORLD, &D3DXMATRIX(Common::identity));
	device->DrawPrimitive(D3DPT_LINELIST, 0, (size+1)*2*2);

	device->SetRenderState(D3DRS_LIGHTING, true);
}
void Grid::release() {
	block_size = D3DXVECTOR2(0, 0);
	size = 0;
	SAFE_RELEASE(vtx);
}
Grid* Grid::setBlockSize(D3DXVECTOR2 size) {
	this->block_size = size;
	createGrid();
	return this;
}
Grid* Grid::setSize(int size) {
	this->size = size;
	createGrid();
	return this;
}
Grid* Grid::createGrid() {
	if(block_size == D3DXVECTOR2(0, 0) || size == 0) return this;
	SAFE_RELEASE(vtx);

	LPDIRECT3DDEVICE9 device = ShaderDevise::device();

	// Žg‚¢‚Ü‚í‚·‚Ì‚Å•Û‘¶
	int v_num = size + 1;

	device->CreateVertexBuffer(
		sizeof(CUSTOMVERTEX)*v_num*4,
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&vtx,
		NULL
	);

	CUSTOMVERTEX *data;
	vtx->Lock(0, 0, (void**)&data, 0);
	for(int i = 0, len = v_num*2; i < len; i+=2) {
		int ofs = i / 2;
		D3DXVECTOR3 p(-size*block_size.x/2+ofs*block_size.x, 0, -size*block_size.y/2);
		D3DXVECTOR2 uv((float)ofs / (float)size, 0);
		data[i] = CUSTOMVERTEX(p, D3DXVECTOR3(0, 0, -1), 0xffffffff, uv);
		p.z *= -1;
		uv.y = 1;
		data[i+1] = CUSTOMVERTEX(p, D3DXVECTOR3(0, 0, -1), 0xffffffff, uv);
	}
	for(int i = 0, len = v_num*2; i < len; i+=2) {
		int ofs = i / 2;
		D3DXVECTOR3 p(-size*block_size.x/2, 0, -size*block_size.y/2+ofs*block_size.y);
		D3DXVECTOR2 uv(0, (float)ofs / (float)size);
		data[v_num*2+i] = CUSTOMVERTEX(p, D3DXVECTOR3(0, 0, -1), 0xffffffff, uv);
		p.x *= -1;
		uv.x = 1;
		data[v_num*2+i+1] = CUSTOMVERTEX(p, D3DXVECTOR3(0, 0, -1), 0xffffffff, uv);
	}
	vtx->Unlock();
	return this;
}
