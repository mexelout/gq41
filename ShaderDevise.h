#include <Windows.h>
#pragma once
class ShaderDevise {
public:
	ShaderDevise(void);
	~ShaderDevise(void);
	ShaderDevise* init(int w, int h, HWND hWnd);
	void release();

	static LPDIRECT3DDEVICE9 device();
private:
	static LPDIRECT3D9 _d3d;
	static LPDIRECT3DDEVICE9 _device;
};

