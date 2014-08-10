#pragma once
#include <Windows.h>
class Cylinder;

class ShaderDevise {
public:
	ShaderDevise(void);
	~ShaderDevise(void);
	ShaderDevise* init(int w, int h, HWND hWnd);
	static LPDIRECT3DDEVICE9 device();

	void release();

	void beginDepthShadow();
	void endDepthShadow();

	static D3DXVECTOR3& getLightVec();

	static void drawText(const char* text);
	static void resetOffsetY();
	static void setupVertexFog(float start, float end, DWORD color, DWORD mode, bool use_range, float density);
private:
	static LPDIRECT3D9 _d3d;
	static LPDIRECT3DDEVICE9 _device;
	static LPD3DXCONSTANTTABLE vs_constant_table;
	static LPD3DXCONSTANTTABLE ps_constant_table;
	static LPDIRECT3DVERTEXSHADER9 vertex_shader;
	static LPDIRECT3DPIXELSHADER9 pixel_shader;

	static D3DXVECTOR3 light_vec;

	static D3DLIGHT9 light;

	static LPD3DXFONT font;
	static int offset_text_y;

};

