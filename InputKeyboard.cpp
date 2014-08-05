#include "Input.h"
#include "Common.h"

LPDIRECTINPUTDEVICE8 InputKeyboard::device = NULL;

BYTE InputKeyboard::press_key_state[KEY_NUM] = {0};
BYTE InputKeyboard::trigger_key_state[KEY_NUM] = {0};
BYTE InputKeyboard::release_key_state[KEY_NUM] = {0};
BYTE InputKeyboard::repeat_key_state[KEY_NUM] = {0};

InputKeyboard::InputKeyboard(void) {
}

InputKeyboard::~InputKeyboard(void) {
}

void InputKeyboard::init(HWND hwnd) {
	if(device == NULL && d_input != NULL) {
		// ここからキーボードの設定
		d_input->CreateDevice(GUID_SysKeyboard, &device, NULL);
		device->SetDataFormat(&c_dfDIKeyboard);
		device->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		device->Acquire();
	}
}

void InputKeyboard::update() {
	HRESULT hr = 0;
	BYTE prev_key_state[KEY_NUM];
	memcpy(prev_key_state, press_key_state, sizeof press_key_state);
	hr = device->GetDeviceState(KEY_NUM, press_key_state);
	if(SUCCEEDED(hr)) {
		for(int i = 0; i < KEY_NUM; i++) {
			press_key_state[i] = (press_key_state[i] & 0x80) == 0x80;
			trigger_key_state[i] = (prev_key_state[i] == 0 && press_key_state[i] == 1);
			release_key_state[i] = (prev_key_state[i] == 1 && press_key_state[i] == 0);
		}
	} else {
		device->Acquire();
	}
}

void InputKeyboard::release() {
	if(device) {
		device->Unacquire();
		device->Release();
		device = NULL;
	}
}

BOOL InputKeyboard::isKey(BYTE dik, State state) {
	switch(state) {
		case Press:
			return (press_key_state[dik]);
		case Trigger:
			return (trigger_key_state[dik]);
		case Release:
			return (release_key_state[dik]);
		default:
			return 0;
	};
}

