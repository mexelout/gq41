#pragma once
#include <d3dx9.h>
class InputMouse : public Input {
public:
	typedef enum {
		Left,
		Center,
		Right
	}Button;

	InputMouse(void);
	~InputMouse(void);

	static void init(HWND hwnd);
	static void update();
	static void release();

	static D3DXVECTOR2 pos();
	static D3DXVECTOR2 move();
	static float wheel();
	static bool btn(Button b, State s);

	static void showCursor();
	static void hideCursor();

	static void fixed(bool flag);
private:
	static LPDIRECTINPUTDEVICE8 device;
	static D3DXVECTOR2 _pos;
	static D3DXVECTOR2 _move;
	static float _wheel;
	// "left center right"  with  "press trigger release"
	static bool _btn[3][Input::Max];
	static HWND _hwnd;
	static bool fixed_flag;
};

