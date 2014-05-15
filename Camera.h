#pragma once

#include <d3dx9.h>

class Camera {
public:
	Camera(void);
	~Camera(void);
	static void init();
	static const D3DXMATRIX& view();
	static const D3DXMATRIX& projection();
private:
	static D3DXVECTOR3 eye, at, up;
	static D3DXMATRIX _view, _projection;
};

