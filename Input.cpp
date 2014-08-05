#include <string>
#include "Input.h"
#include "Common.h"

LPDIRECTINPUT8 Input::d_input = NULL;


Input::Input(void) {
}

Input::~Input(void) {
}

void Input::init(HINSTANCE inst, HWND hwnd) {
	if(!d_input) DirectInput8Create(inst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&d_input, NULL);
	InputKeyboard::init(hwnd);
	InputMouse::init(hwnd);
}

void Input::update() {
	InputKeyboard::update();
	InputMouse::update();
}

void Input::release() {
	InputKeyboard::release();
	InputMouse::release();

	SAFE_RELEASE(d_input);
}


