#pragma once
class InputKeyboard : public Input {
public:

	static void init(HWND hwnd);
	static void update();
	static void release();
	static BOOL isKey(BYTE dik, State state);
private:
	InputKeyboard(void);
	~InputKeyboard(void);

	static LPDIRECTINPUTDEVICE8 device;

	// キーボードに必要な変数
	static BYTE press_key_state[KEY_NUM];
	static BYTE trigger_key_state[KEY_NUM];
	static BYTE release_key_state[KEY_NUM];
	static BYTE repeat_key_state[KEY_NUM];
};

