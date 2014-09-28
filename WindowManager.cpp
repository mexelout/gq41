#include "WindowManager.h"

WindowManager::WindowManager() {
	wnd = NULL;
	instance = NULL;
}
WindowManager::~WindowManager() {
}

void WindowManager::setWnd(HWND wnd) {
	this->wnd = wnd;
}
void WindowManager::setInst(HINSTANCE inst) {
	this->instance = inst;
}
HINSTANCE WindowManager::getInst() {
	return instance;
}
HWND WindowManager::getWnd() {
	return wnd;
}
