
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Common.h"
#include "ShaderDevise.h"
#include "Vertex.h"

#define self this

LPDIRECT3D9 ShaderDevise::_d3d;
LPDIRECT3DDEVICE9 ShaderDevise::_device;

LPD3DXCONSTANTTABLE ShaderDevise::vs_constant_table;
LPD3DXCONSTANTTABLE ShaderDevise::ps_constant_table;
LPDIRECT3DVERTEXSHADER9 ShaderDevise::vertex_shader;
LPDIRECT3DPIXELSHADER9 ShaderDevise::pixel_shader;
D3DLIGHT9 ShaderDevise::light;
LPD3DXFONT ShaderDevise::font;
int ShaderDevise::offset_text_y;

D3DXVECTOR3 ShaderDevise::light_vec;


ShaderDevise::ShaderDevise(void) {
}

ShaderDevise::~ShaderDevise(void) {
}

ShaderDevise* ShaderDevise::init(int w, int h, HWND hWnd) {
	if(!(_d3d = Direct3DCreate9(D3D_SDK_VERSION))) return 0;

	D3DPRESENT_PARAMETERS d3dpp = {w, h, D3DFMT_UNKNOWN, 0, D3DMULTISAMPLE_NONE, 0,
	D3DSWAPEFFECT_DISCARD,NULL,TRUE,TRUE,D3DFMT_D24S8,0,0}; 

	if( FAILED(_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &_device))) {
		if( FAILED(_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &_device))) {
			if( FAILED(_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &_device))) {
				if( FAILED(_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &_device))) {
					_d3d->Release();
					_d3d = NULL;
					_device = NULL;
					return 0;
				}
			}
		}
	}

	_device->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);
	_device->SetRenderState(D3DRS_LIGHTING , TRUE);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );  
 	_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	_device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	_device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = light.Diffuse.a = 1.0f;
	light.Specular.r = light.Specular.g = light.Specular.b = light.Specular.a = 0.6f;
	light.Ambient.r = light.Ambient.g = light.Ambient.b = light.Ambient.a = 0.3f;
	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(-1.0f, -1.0f, -0.1f);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	_device->SetLight( 0, &light );
	_device->LightEnable( 0, TRUE );
	light_vec = light.Direction;
	_device->SetRenderState(D3DRS_SPECULARENABLE, true);

	// ƒtƒHƒ“ƒg‚Ì¶¬
	int fontsize = 24;
	D3DXFONT_DESC lf = {fontsize, 0, 0, 1, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
	PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "‚l‚r ƒSƒVƒbƒN"};
	if(FAILED(D3DXCreateFontIndirect(_device, &lf, &font))){
		return 0;
	}

	return self;
}

LPDIRECT3DDEVICE9 ShaderDevise::device() {
	return _device;
}

void ShaderDevise::release() {
	SAFE_RELEASE(font);
	SAFE_RELEASE(vertex_shader);
	SAFE_RELEASE(pixel_shader);

	SAFE_RELEASE(vs_constant_table);
	SAFE_RELEASE(ps_constant_table);

	SAFE_RELEASE(_device);
	SAFE_RELEASE(_d3d);
}

void ShaderDevise::beginDepthShadow() {

}

void ShaderDevise::endDepthShadow() {
}

D3DXVECTOR3& ShaderDevise::getLightVec() {
	return light_vec;
}

void ShaderDevise::drawText(const char* text) {
	// •`‰æ
	RECT r = {0, offset_text_y, 0, 0};
	font->DrawText(NULL, text, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);
	offset_text_y += 24;
}

void ShaderDevise::resetOffsetY() {
	offset_text_y = 0;
}
