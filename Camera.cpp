#include "Camera.h"

D3DXVECTOR3 Camera::eye(3, 3, 3), Camera::at(0, 0, 0), Camera::up(0, 1, 0);
D3DXMATRIX Camera::_view, Camera::_projection;

Camera::Camera(void) {
}

Camera::~Camera(void) {
}

void Camera::init() {
	D3DXMatrixLookAtLH(
		&_view,
		&eye,
		&at,
		&up
	);

	// ŽË‰e•ÏŠ·
	D3DXMatrixPerspectiveFovLH(&_projection, D3DXToRadian(45), 640.0f/480.0f, 1.0f, 10000.0f);
}
const D3DXMATRIX& Camera::view() {
	static float angle = 0;
//	angle += (D3DX_PI / 180 * 1);
//	eye.x = cosf(angle) * 3;
//	eye.z = sinf(angle) * 3;

	D3DXMatrixLookAtLH(
		&_view,
		&eye,
		&at,
		&up
	);

	return _view;
}
const D3DXMATRIX& Camera::projection() {
	return _projection;
}
