
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Common.h"
#include "ShaderDevise.h"
#include "Vertex.h"


ShaderDevise::ShaderDevise(void) {
}

ShaderDevise::~ShaderDevise(void) {
}

ShaderDevise* ShaderDevise::init(int w, int h, HWND hWnd) {
	ShaderDevise& sd(ShaderDevise::inst());

	if(!(sd._d3d = Direct3DCreate9(D3D_SDK_VERSION))) return 0;

	D3DPRESENT_PARAMETERS d3dpp = {w, h, D3DFMT_UNKNOWN, 0, D3DMULTISAMPLE_NONE, 0,
	D3DSWAPEFFECT_DISCARD,NULL,TRUE,TRUE,D3DFMT_D24S8,0,0}; 

	if( FAILED(sd._d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &sd._device))) {
		if( FAILED(sd._d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &sd._device))) {
			if( FAILED(sd._d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &sd._device))) {
				if( FAILED(sd._d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &sd._device))) {
					sd._d3d->Release();
					sd._d3d = NULL;
					sd._device = NULL;
					return 0;
				}
			}
		}
	}

	sd._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	sd._device->SetRenderState(D3DRS_LIGHTING, TRUE);
	sd._device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	sd._device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	sd._device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	sd._device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	sd._device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);  
 	sd._device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	sd._device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	sd._device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	sd._device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	ZeroMemory(&sd.light, sizeof(D3DLIGHT9)*8);
	sd.light[0].Type			= D3DLIGHT_DIRECTIONAL;
	sd.light[0].Diffuse.r = 0.5f;
	sd.light[0].Diffuse.g = 0.5f;
	sd.light[0].Diffuse.b = 0.5f;
	sd.light[0].Diffuse.a = 1.0f;
	sd.light[0].Specular.r	= sd.light[0].Specular.g = sd.light[0].Specular.b = sd.light[0].Specular.a = 1.0f;
	sd.light[0].Ambient.r = 0.098f;
	sd.light[0].Ambient.g = 0.098f;
	sd.light[0].Ambient.b = 0.439f;
	sd.light[0].Ambient.a = 1.0f;
	
	D3DXVECTOR3 vecDir(-0.76f, -0.52f, -0.31f);
	D3DXVec3Normalize((D3DXVECTOR3*)&sd.light[0].Direction, &vecDir);
	sd._device->SetLight(0, &sd.light[0]);
	sd._device->LightEnable(0, TRUE);
	sd.light_vec[0] = sd.light[0].Direction;
	sd._device->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DMATERIAL9 mat = {
		1, 1, 1, 1,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.8f, 0.8f, 0.8f, 1.0f,
		0, 0, 0, 0,
		5
	};

	sd._device->SetMaterial(&mat);

	// ƒtƒHƒ“ƒg‚Ì¶¬
	int fontsize = 24;
	D3DXFONT_DESC lf = {fontsize, 0, 0, 1, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
	PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "‚l‚r ƒSƒVƒbƒN"};
	if(FAILED(D3DXCreateFontIndirect(sd._device, &lf, &sd.font))){
		return 0;
	}

	return &sd;
}

LPDIRECT3DDEVICE9 ShaderDevise::device() {
	return ShaderDevise::inst()._device;
}

void ShaderDevise::release() {
	ShaderDevise& sd(ShaderDevise::inst());
	SAFE_RELEASE(sd.font);
	SAFE_RELEASE(sd._device);
	SAFE_RELEASE(sd._d3d);
}

void ShaderDevise::setLight(int num, D3DLIGHT9& l) {
	ShaderDevise& sd(ShaderDevise::inst());
	sd.light[num] = l;
	sd._device->SetLight(num, &sd.light[num]);
}
D3DLIGHT9& ShaderDevise::getLight(int num) {
	ShaderDevise& sd(ShaderDevise::inst());
	return sd.light[num];
}

void ShaderDevise::drawText(const char* text) {
	ShaderDevise& sd(ShaderDevise::inst());
	// •`‰æ
	RECT r = {0, sd.offset_text_y, 0, 0};
	sd.font->DrawText(NULL, text, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);
	sd.offset_text_y += 24;
}

void ShaderDevise::resetOffsetY() {
	ShaderDevise& sd(ShaderDevise::inst());
	sd.offset_text_y = 0;
}
