
#include "Plane.h"
#include "Camera.h"

Plane::Plane(void) {
	vbuf = NULL;
	D3DXMatrixIdentity(&world);
}

Plane::~Plane(void) {
}

Plane* Plane::init(LPDIRECT3DDEVICE9 device) {
	device->CreateVertexBuffer(
		sizeof(CUSTOMVERTEX)*4,
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&vbuf,
		NULL
		);

	CUSTOMVERTEX vtx[] = {
		{D3DXVECTOR3(-1, 0,  1), D3DXVECTOR3(0, 1, 0), 0x00ffffff, D3DXVECTOR2(0, 0)},
		{D3DXVECTOR3( 1, 0,  1), D3DXVECTOR3(0, 1, 0), 0x00ffffff, D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(-1, 0, -1), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3( 1, 0, -1), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 1)}
	};

	void *data;
	vbuf->Lock(0, sizeof(CUSTOMVERTEX)*4, &data, 0);
	memcpy(data, vtx, sizeof(CUSTOMVERTEX)*4);
	vbuf->Unlock();

	LPD3DXBUFFER code;
	LPD3DXBUFFER error;

	D3DXCompileShaderFromFile("shader.fx", NULL, NULL,
								"vertexShaderTexture", "vs_2_0", 0,
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
								"pixelShaderTexture", "ps_2_0", 0,
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

	D3DXCreateTextureFromFile(device, "textures/default.bmp", &texture);

	angle = (D3DX_PI / 180.0f * 2);
	//D3DXMATRIX rot;
	//D3DXMatrixRotationAxis(&rot, &D3DXVECTOR3(1, 0, 0), (D3DX_PI / 180.0f * -90));
	//world *= rot;

	return this;
}

void Plane::update() {
}

void Plane::draw(LPDIRECT3DDEVICE9 device) {
	device->SetVertexShader(_vs);
	device->SetPixelShader(_ps);
	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	const D3DXMATRIX& view = Camera::view(), projection = Camera::projection();
	//D3DXMATRIX rot;
	//D3DXMatrixRotationAxis(&rot, &D3DXVECTOR3(0, 1, 0), angle);
	//world *= rot;
	D3DXMATRIX world_view_projection = world * view * projection;
	_vs_constant_table->SetMatrix(device, "g_world_view_projection", &world_view_projection);
	_vs_constant_table->SetMatrix(device, "g_world", &world);
	device->SetTexture(0, texture);
	device->SetStreamSource(0, vbuf, 0, sizeof(CUSTOMVERTEX));
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	device->SetTexture(0, NULL);
	device->SetVertexShader(NULL);
	device->SetPixelShader(NULL);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Plane::release() {
	vbuf->Release();
	_vs->Release();
	_vs_constant_table->Release();
	_ps->Release();
	_ps_constant_table->Release();
	texture->Release();
	delete this;
}

void Plane::setCollision(bool col) {
}
