#include "WaveScene.h"
#include "ShaderDevise.h"
#include "Camera.h"
#include "Common.h"
#include "Input.h"
#include "Spara.h"

WaveScene::WaveScene(void) {
	vertex_shader = NULL;
	vs_constant_table = NULL;
	camera_rot = D3DXVECTOR2(0, 0);
	camera_len = 25;
}

WaveScene::~WaveScene(void) {
}

WaveScene* WaveScene::init() {
	/*
	LPD3DXBUFFER code;
	LPD3DXBUFFER error;

	D3DXCompileShaderFromFile("shader.fx", NULL, NULL,
								"vertexShader3D", "vs_2_0", 0,
								&code, &error, &vs_constant_table);

	if(error) {
		OutputDebugString((LPCSTR)error->GetBufferPointer());
		MessageBox(NULL, "shader error.\ncheck output string message.", "error", MB_OK);
		error->Release();
		delete this;
		return NULL;
	}

	ShaderDevise::device()->CreateVertexShader((DWORD*)code->GetBufferPointer(), &vertex_shader);
	code->Release();
	*/

	t = 0;
	one_radian = (float)M_PI / 180.0f;

	for(int i = 0; i < 10; i++) {
		vtx.push_back(CUSTOMVERTEX(-10.0f + i * 5, calcHeight(t+i), -1, 0, 1, 0, 0xffffffff, 0.00f + i * 0.25f, 0));
		vtx.push_back(CUSTOMVERTEX(-10.0f + i * 5, calcHeight(t+i), 1, 0, 1, 0, 0xffffffff, 0.00f + i * 0.25f, 1));
	}

	for(int i = 0, len = vtx.size()/2-1; i < len; i++) {
		nors.push_back(vtx[(i+1)*2].pos - vtx[i*2].pos);
		D3DXVec3Normalize(&nors[i], &nors[i]);
	}

	//camera_rot.x = (float)M_PI_4;
	camera_rot.y = (float)M_PI_2;

	spara = (new Spara())->init();

	InputMouse::hideCursor();

	return this;
}
void WaveScene::update() {
	t++;
	// 頂点の計算
	for(int i = 0, len = vtx.size()/2; i < len; i++) {
		vtx[i*2].pos.y = calcHeight(t + i * 45);
		vtx[i*2+1].pos.y = calcHeight(t + i * 45);
	}
	// 面の法線の計算
	for(int i = 0, len = nors.size(); i < len; i++) {
		nors[i] = (vtx[(i+1)*2].pos - vtx[i*2].pos);
		D3DXVec3Normalize(&nors[i], &nors[i]);
	}
	// 頂点法線の計算
	for(int i = 0, len = vtx.size()/2; i < len; i++) {
		if(i == 0) {
			vtx[i*2].nor = nors[i];
			vtx[i*2+1].nor = nors[i];
		} else if(i == len-1) {
			vtx[i*2].nor = nors[i-1];
			vtx[i*2+1].nor = nors[i-1];
		} else {
			vtx[i*2].nor = ((nors[i] + nors[i-1])/2);
			vtx[i*2+1].nor = ((nors[i] + nors[i-1])/2);
		}
	}

	float len = cosf(camera_rot.x);
	camera_rot.y -= InputMouse::move().x/100;
	camera_rot.x -= InputMouse::move().y/100;
	if(camera_rot.x >= (M_PI_2-0.017f)) camera_rot.x = (float)(M_PI_2-0.017f);
	else if(camera_rot.x <= (-M_PI_2+0.017f)) camera_rot.x = (float)(-M_PI_2+0.017f);
	Camera::setEye(spara->pos());
	Camera::setAt(spara->pos() + (D3DXVECTOR3(cosf(camera_rot.y)*len, sinf(camera_rot.x), sinf(camera_rot.y)*len)));

	spara->setRotate(camera_rot.x, camera_rot.y);
	spara->update();

	Camera::setEye(spara->pos());
	Camera::setAt(spara->pos() + (D3DXVECTOR3(cosf(camera_rot.y)*len, sinf(camera_rot.x), sinf(camera_rot.y)*len)));
}
void WaveScene::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();

	device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	device->BeginScene();

	D3DXVECTOR4 light_dir(0, 0.4f, -1, 0);
	D3DXVec4Normalize(&light_dir, &light_dir);
	D3DXVECTOR4 ambient_color(0.8f, 0.8f, 0.8f, 1);
	//vs_constant_table->SetVector(ShaderDevise::device(), "g_light_direction", &light_dir);
	//vs_constant_table->SetVector(ShaderDevise::device(), "g_ambient", &ambient_color);

	D3DXMATRIX world, view = Camera::view(), proj = Camera::projection();
	D3DXMatrixIdentity(&world);

	//D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(5, 5, 5), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));

	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetTransform(D3DTS_WORLD, &world);
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	device->SetFVF(D3DFVF_CUSTOMVERTEX);

	//device->SetVertexShader(_vs);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vtx.size()/2+1, &vtx[0], sizeof CUSTOMVERTEX);
	//vs_constant_table->SetMatrix(device, "g_world_view_projection", &world_view_projection);

	spara->draw();

	device->EndScene();
	device->Present( NULL, NULL, NULL, NULL );
}
void WaveScene::release() {
	if(vertex_shader) vertex_shader->Release();
	if(vs_constant_table) vs_constant_table->Release();

	SAFE_RELEASE_DELETE(spara);
}

float WaveScene::calcHeight(int t) {
	return (sinf(((float)t)*one_radian)*5);
}