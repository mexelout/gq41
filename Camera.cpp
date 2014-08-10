#include "Camera.h"
#include "Common.h"

D3DXVECTOR3 Camera::_eye(3, 3, 3), Camera::_at(0, 0, 0), Camera::_up(0, 1, 0);
D3DXMATRIX Camera::_view, Camera::_projection;

Camera::Camera(void) {
}

Camera::~Camera(void) {
}

void Camera::init() {
	D3DXMatrixLookAtLH(
		&_view,
		&_eye,
		&_at,
		&_up
	);

	// ŽË‰e•ÏŠ·
	D3DXMatrixPerspectiveFovLH(&_projection, D3DXToRadian(45), Common::window_width/Common::window_height, 0.1f, 10000.0f);
}
const D3DXMATRIX& Camera::view() {
	static float angle = 0;
//	angle += (D3DX_PI / 180 * 1);
//	eye.x = cosf(angle) * 3;
//	eye.z = sinf(angle) * 3;

	D3DXMatrixLookAtLH(
		&_view,
		&_eye,
		&_at,
		&_up
	);

	return _view;
}
const D3DXMATRIX& Camera::projection() {
	return _projection;
}

void Camera::setEye(D3DXVECTOR3 eye) {
	_eye = eye;
}

void Camera::setAt(D3DXVECTOR3 at) {
	_at = at;
}

D3DXVECTOR3 Camera::eye() {
	return _eye;
}

D3DXVECTOR3 Camera::at() {
	return _at;
}

D3DXVECTOR3 Camera::dir() {
	return (_at - _eye);
}
