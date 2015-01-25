#pragma once
#include <Windows.h>
#include "Singleton.h"

class ShaderDevise : public Singleton<ShaderDevise> {
private:
	friend Singleton;
public:
	ShaderDevise(void);
	~ShaderDevise(void);
	static ShaderDevise* init(int w, int h, HWND hWnd);
	static LPDIRECT3DDEVICE9 device();
	static void release();

	static void setLight(int num, D3DLIGHT9& l);
	static D3DLIGHT9& getLight(int num);

	static void drawText(const char* text);
	static void resetOffsetY();

private:
	LPDIRECT3D9 _d3d;
	LPDIRECT3DDEVICE9 _device;

	D3DXVECTOR3 light_vec[8];
	D3DLIGHT9 light[8];

	LPD3DXFONT font;
	int offset_text_y;

};

