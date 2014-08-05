#pragma once

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <Windows.h>
#include <dinput.h>

// ÉLÅ[ÇÃêî
const DWORD KEY_NUM = 256ul;

class Input {
public:
	typedef enum {
		Press,
		Trigger,
		Release,
		Max
	}State;
	Input(void);
	~Input(void);
	static void init(HINSTANCE inst, HWND hwnd);
	static void update();
	static void release();
protected:
	static LPDIRECTINPUT8 d_input;

};

#include "InputKeyboard.h"
#include "InputMouse.h"
