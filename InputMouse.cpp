#include "Input.h"
#include "Common.h"

LPDIRECTINPUTDEVICE8 InputMouse::device = NULL;
D3DXVECTOR2 InputMouse::_pos;
bool InputMouse::_btn[3][Input::Max];
D3DXVECTOR2 InputMouse::_move;
float InputMouse::_wheel = 0;
HWND InputMouse::_hwnd = NULL;
bool InputMouse::fixed_flag = false;

InputMouse::InputMouse(void) {
}


InputMouse::~InputMouse(void) {
}

void InputMouse::init(HWND hwnd) {
	if(device == NULL && d_input != NULL) {
		HRESULT hr = 0;
		hr = d_input->CreateDevice(GUID_SysMouse, &device, NULL);
		if(FAILED(hr)) {
			Common::debug("デバイスの生成に失敗");
			release();
			return;
		}
		hr = device->SetDataFormat(&c_dfDIMouse2);
		if(FAILED(hr)) {
			Common::debug("フォーマットの設定に失敗");
			release();
			return ;
		}
		hr = device->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE  | DISCL_FOREGROUND);
		if(FAILED(hr)) {
			Common::debug("コンパーレーティブレベルの設定に失敗");
			release();
			return;
		}
		DIPROPDWORD diprop;
		diprop.diph.dwSize  = sizeof(diprop); 
		diprop.diph.dwHeaderSize    = sizeof(diprop.diph); 
		diprop.diph.dwObj   = 0;
		diprop.diph.dwHow   = DIPH_DEVICE;
		diprop.dwData       = DIPROPAXISMODE_REL;
		//diprop.dwData       = DIPROPAXISMODE_ABS; // 絶対値モードの場合
		hr = device->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(hr)) {
			Common::debug("軸モードの設定に失敗");
			release();
			return;
		}
		// 入力制御開始
		device->Acquire();
		_hwnd = hwnd;
	}
}
void InputMouse::update() {
	POINT __pos;
	GetCursorPos(&__pos);
	ScreenToClient(_hwnd, &__pos);
	_pos.x = (float)__pos.x;
	_pos.y = (float)__pos.y;

	DIMOUSESTATE2 dims = {0};
	HRESULT hr = 0;

	bool prev_press[3] = {_btn[Left][Press], _btn[Center][Press], _btn[Right][Press]};

	hr= device->GetDeviceState(sizeof(DIMOUSESTATE2),&dims);
	if(SUCCEEDED(hr)) {
		_btn[Left][Press] = (dims.rgbButtons[0] != 0);
		_btn[Right][Press] = (dims.rgbButtons[1] != 0);
		_btn[Center][Press] = (dims.rgbButtons[2] != 0);
		_btn[Left][Trigger] = (prev_press[Left] == false && true == _btn[Left][Press]);
		_btn[Right][Trigger] = (prev_press[Right] == false && true  == _btn[Right][Press]);
		_btn[Center][Trigger] = (prev_press[Center] == false && true  == _btn[Center][Press]);
		_btn[Left][Release] = (prev_press[Left] == true && false  == _btn[Left][Press]);
		_btn[Right][Release] = (prev_press[Right] == true && false  == _btn[Right][Press]);
		_btn[Center][Release] = (prev_press[Center] == true && false  == _btn[Center][Press]);

		_move.x = (float)dims.lX;
		_move.y = (float)dims.lY;
		_wheel = (float)dims.lZ;
	} else 
		device->Acquire();

	if(fixed_flag) {
		RECT rect;
		GetWindowRect(_hwnd, &rect);

		SetCursorPos(rect.left + (int)Common::window_width/2, rect.top + (int)Common::window_height/2);
	}
}
void InputMouse::release() {
	if(device) {
		device->Unacquire();
		device->Release();
		device = NULL;
	}
}

D3DXVECTOR2 InputMouse::pos() {
	return _pos;
}

D3DXVECTOR2 InputMouse::move() {
	return _move;
}

float InputMouse::wheel() {
	return _wheel;
}

bool InputMouse::btn(Button b, State s) {
	return _btn[b][s];
}

void InputMouse::showCursor() {
	while(ShowCursor(true) <= 0) {}
}

void InputMouse::hideCursor() {
	while(ShowCursor(false) > 0) {}
}

void InputMouse::fixed(bool flag) {
	fixed_flag = flag;
}
