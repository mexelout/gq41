#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

#include <Windows.h>
#include <stdarg.h>
#include "Common.h"
#include "ShaderDevise.h"
#include "Vertex.h"

const float Common::screen_width = (float)GetSystemMetrics(SM_CXSCREEN);
const float Common::screen_height = (float)GetSystemMetrics(SM_CYSCREEN );
const float Common::window_width = 16 * 70;
const float Common::window_height = 9 * 70;

const D3DXMATRIX Common::identity = *D3DXMatrixIdentity(&D3DXMATRIX());
const D3DXVECTOR2 Common::vec2zero(0, 0);
const D3DXVECTOR3 Common::vec3zero(0, 0, 0);
const D3DXVECTOR4 Common::vec4zero(0, 0, 0, 0);

Common::Common() {

}
Common::~Common() {
}

std::string Common::debug(const char* fmt, ...) {
	va_list ap;
	std::string rs;

	va_start(ap, fmt);

	char check = 0;

	for(int i = 0, len = strlen(fmt); i < len; ++i) {
		check = fmt[i];
		if(check == '%') {
			++i;
			check = fmt[i];
			switch(check) {
				case 'c': {
					char c = va_arg(ap, int);
					rs += c;
					break;
				}
				case 'd': {
					int _ = va_arg(ap, int);
					rs += std::to_string((_Longlong)_);
					break;
				}
				case 'f': {
					rs += std::to_string((long double)va_arg(ap, double));
					break;
				}
				case 's': {
					rs += va_arg(ap, char*);
					break;
				}
				case 'b': {
					rs += (va_arg(ap, bool)) ? "true" : "false";
					break;
				}
			}
		} else {
			rs += check;
		}
	}
	rs += "\n";

	va_end(ap);
	OutputDebugString(rs.c_str());

	return rs;
}

DWORD Common::ftodw(float f) {
	return *((DWORD*)&f);
}

float Common::random(float min, float max) {
	return min + (rand()*(max-min+1.0f)/(1.0f+RAND_MAX));
}

int Common::random(int min, int max) {
	return (int)(random((float)min, (float)max));
}

LPDIRECT3DVERTEXBUFFER9 Common::plane(D3DXVECTOR3 scl, D3DXVECTOR2 uv_pos, D3DXVECTOR2 uv_scl) {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	LPDIRECT3DVERTEXBUFFER9 vtx;
	device->CreateVertexBuffer(
		sizeof(CUSTOMVERTEX)*4,
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&vtx,
		NULL
	);

	CUSTOMVERTEX *data;
	vtx->Lock(0, 0, (void**)&data, 0);
	data[0] = CUSTOMVERTEX(D3DXVECTOR3(-0.5f*scl.x,  0.5f*scl.y,  0), D3DXVECTOR3(0, 0, -1), 0xffffffff, uv_pos);
	data[1] = CUSTOMVERTEX(D3DXVECTOR3( 0.5f*scl.x,  0.5f*scl.y,  0), D3DXVECTOR3(0, 0, -1), 0xffffffff, D3DXVECTOR2(uv_pos.x + uv_scl.x, uv_pos.y));
	data[2] = CUSTOMVERTEX(D3DXVECTOR3(-0.5f*scl.x, -0.5f*scl.y, 0), D3DXVECTOR3(0, 0, -1), 0xffffffff, D3DXVECTOR2(uv_pos.x, uv_pos.y + uv_scl.y));
	data[3] = CUSTOMVERTEX(D3DXVECTOR3( 0.5f*scl.x, -0.5f*scl.y, 0), D3DXVECTOR3(0, 0, -1), 0xffffffff, D3DXVECTOR2(uv_pos.x + uv_scl.x, uv_pos.y + uv_scl.y));
	vtx->Unlock();

	return vtx;
}

void* Common::getVtxDataWithLock(LPDIRECT3DVERTEXBUFFER9 vtx) {
	CUSTOMVERTEX* data;
	vtx->Lock(0, 0, (void**)&data, 0);
	return (void*)data;
}

// end line