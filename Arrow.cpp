#include "Arrow.h"
#include "Camera.h"
#include "Vertex.h"
#include "ShaderDevise.h"
#include "Input.h"

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
		CUSTOMVERTEX(_start, D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 0)),
		CUSTOMVERTEX(_end, D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 0)),
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
	CUSTOMVERTEX *data;
	vbuf->Lock(0, sizeof(CUSTOMVERTEX)*2, (void**)&data, 0);

	float angle = 0;
	bool is_down = false;

	if(InputKeyboard::isKey(DIK_LEFT, Input::Press)) {
		if(InputKeyboard::isKey(DIK_UP, Input::Press)) {
			angle = D3DX_PI * 0.25f;
		} else if(InputKeyboard::isKey(DIK_DOWN, Input::Press)) {
			angle = D3DX_PI * 1.75f;
		} else {
			angle = 0;
		}
		is_down = true;
	} else if(InputKeyboard::isKey(DIK_RIGHT, Input::Press)) {
		if(InputKeyboard::isKey(DIK_UP, Input::Press)) {
			angle = D3DX_PI * 0.75f;
		} else if(InputKeyboard::isKey(DIK_DOWN, Input::Press)) {
			angle = D3DX_PI * 1.25f;
		} else {
			angle = D3DX_PI;
		}
		is_down = true;
	} else if(InputKeyboard::isKey(DIK_UP, Input::Press)) {
		angle = D3DX_PI * 0.5f;
		is_down = true;
	} else if(InputKeyboard::isKey(DIK_DOWN, Input::Press)) {
		angle = D3DX_PI * 1.5f;
		is_down = true;
	}

	if(is_down) {
		_end.x += cosf(angle) * 0.1f;
		_end.y += sinf(angle) * 0.1f;
	}

	data[1].pos = _end;

	D3DXVECTOR3 zero(0, 0, 0);
	D3DXVECTOR3 vec0(_start - zero), vec1(_end - zero);
	D3DXVECTOR3 nor(0, 1, 0);

	float v0 = D3DXVec3Dot(&vec0, &nor);
	float v1 = D3DXVec3Dot(&vec1, &nor);


	if((v0 >= 0 && v1 < 0) || v0 < 0 && v1 >= 0) {

		data[0].color = 0xffff8888;
		data[1].color = 0xffff8888;
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

void Arrow::hitTestPlane(const std::vector<CUSTOMVERTEX>& vertices) {

}

//CannonBullet* Arrow::init(void)
//{
//}


void Arrow::draw(void)
{
}
