#include "Arrow.h"
#include "Camera.h"
#include "Vertex.h"
#include "ShaderDevise.h"

Arrow::Arrow(void) {
	D3DXMatrixIdentity(&world);
	_start = D3DXVECTOR3(0, 2,  0);
	_end = D3DXVECTOR3( 0, -2,  0);
}

Arrow::~Arrow(void) {
}

Arrow* Arrow::init(LPDIRECT3DDEVICE9 device) {
	device->CreateVertexBuffer(
		sizeof(CUSTOMVERTEX)*2,
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&vbuf,
		NULL
		);

	CUSTOMVERTEX vtx[] = {
		{_start, D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 0)},
		{_end, D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 0)},
	};

	void *data;
	vbuf->Lock(0, sizeof(CUSTOMVERTEX)*2, &data, 0);
	memcpy(data, vtx, sizeof(CUSTOMVERTEX)*2);
	vbuf->Unlock();

	LPD3DXBUFFER code;
	LPD3DXBUFFER error;

	D3DXCompileShaderFromFile("shader.fx", NULL, NULL,
								"vertexShaderNotLight", "vs_2_0", 0,
								&code, &error, &_vs_constant_table);

	if(error) {
		OutputDebugString((LPCSTR)error->GetBufferPointer());
		vbuf->Release();
		error->Release();
		return 0;
	}

	device->CreateVertexShader((DWORD*)code->GetBufferPointer(), &_vs);
	code->Release();

	D3DXCompileShaderFromFile("shader.fx", NULL, NULL,
								"pixelShader3D", "ps_2_0", 0,
								&code, &error, &_ps_constant_table);

	if(error) {
		OutputDebugString((LPCSTR)error->GetBufferPointer());
		vbuf->Release();
		_vs_constant_table->Release();
		_vs->Release();
		error->Release();
		return 0;
	}

	device->CreatePixelShader((DWORD*)code->GetBufferPointer(), &_ps);
	code->Release();

	return this;
}
void Arrow::update() {
	static int count = 0;
	CUSTOMVERTEX *data;
	vbuf->Lock(0, sizeof(CUSTOMVERTEX)*2, (void**)&data, 0);

	count++;
	if(count % 120 < 60)
		_end.y += 0.06125f;
	else
		_end.y -= 0.06125f;

	data[1].pos = _end;

	D3DXVECTOR3 zero(0, 0, 0);
	D3DXVECTOR3 vec0(_start - zero), vec1(_end - zero);
	D3DXVECTOR3 nor(0, 1, 0);

	float v0 = D3DXVec3Dot(&vec0, &nor);
	float v1 = D3DXVec3Dot(&vec1, &nor);


	if((v0 >= 0 && v1 < 0) || v0 < 0 && v1 >= 0) {
		data[0].color = 0xff800080;
		data[1].color = 0xff800080;
	} else {
		data[0].color = 0xffffffff;
		data[1].color = 0xffffffff;
	}


	vbuf->Unlock();

}
void Arrow::draw(LPDIRECT3DDEVICE9 device) {
	device->SetVertexShader(_vs);
	device->SetPixelShader(_ps);
	device->SetFVF(D3DFVF_CUSTOMVERTEX);

	const D3DXMATRIX& view = Camera::view(), projection = Camera::projection();

	D3DXMATRIX world_view_projection = world * view * projection;
	_vs_constant_table->SetMatrix(device, "g_world_view_projection", &world_view_projection);
	_vs_constant_table->SetMatrix(device, "g_world", &world);
	device->SetStreamSource(0, vbuf, 0, sizeof(CUSTOMVERTEX));
	device->DrawPrimitive(D3DPT_LINESTRIP, 0, 1);
	device->SetVertexShader(NULL);
	device->SetPixelShader(NULL);

}
void Arrow::release() {
	vbuf->Release();
	_vs->Release();
	_vs_constant_table->Release();
	_ps->Release();
	_ps_constant_table->Release();
	delete this;
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