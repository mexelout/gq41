#pragma once

#include <d3dx9.h>
#include <string.h>
#include "Common.h"

typedef struct ShaderPack {
	ShaderPack() {
		constant_table = NULL;
		vs = NULL;
		ps = NULL;
	}
	void loadFunc(LPDIRECT3DDEVICE9 device, char* filename, char* funcname, char* version) {
		LPD3DXBUFFER code = NULL;
		LPD3DXBUFFER error = NULL;

		// vertical
		D3DXCompileShaderFromFile(filename, NULL, NULL,
									funcname, version, 0,
									&code, &error, &constant_table);

		if(error) {
			OutputDebugString((LPCSTR)error->GetBufferPointer());
			error->Release();
			release();
			return;
		}

		if(strstr(version, "vs")) {
			device->CreateVertexShader((DWORD*)code->GetBufferPointer(), &vs);
		} else {
			device->CreatePixelShader((DWORD*)code->GetBufferPointer(), &ps);
		}
		code->Release();
	}
	void release() {
		SAFE_RELEASE(constant_table);
		SAFE_RELEASE(vs);
		SAFE_RELEASE(ps);
	}
	LPD3DXCONSTANTTABLE constant_table;
	LPDIRECT3DVERTEXSHADER9 vs;
	LPDIRECT3DPIXELSHADER9 ps;
}ShaderPack;