#pragma once

#include <Windows.h>
#include "Singleton.h"

class WindowManager : public Singleton<WindowManager> {
private:
	friend Singleton;
	WindowManager();
	~WindowManager();
public:
	void setWnd(HWND wnd);
	void setInst(HINSTANCE inst);
	HINSTANCE getInst();
	HWND getWnd();
private:
	HINSTANCE instance;
	HWND wnd;
};
