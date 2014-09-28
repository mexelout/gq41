#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <d3dx9.h>

class Common {
public:
	Common();
	~Common();
	static std::string debug(const char* fmt, ...);
	static DWORD ftodw(float f);
	static const float screen_width;
	static const float screen_height;
	static const float window_width;
	static const float window_height;
	static const D3DXMATRIX identity;
	static const D3DXVECTOR2 vec2zero;
	static const D3DXVECTOR3 vec3zero;
	static const D3DXVECTOR4 vec4zero;
	static float random(float min, float max);
	static int random(int min, int max);
	static LPDIRECT3DVERTEXBUFFER9 plane(D3DXVECTOR3 scl = D3DXVECTOR3(1, 1, 1), D3DXVECTOR2 uv_pos = D3DXVECTOR2(0, 0), D3DXVECTOR2 uv_scl = D3DXVECTOR2(1, 1));
private:
};

#define SAFE_RELEASE(p) {if(p){p->Release();p=NULL;}}
#define SAFE_RELEASE_DELETE(p) {if(p){p->release();delete p;p=NULL;}}
#define SAFE_DELETE(p) {if(p) {delete p; p=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p){delete[] p;p=NULL;}}
