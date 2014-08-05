#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include "ShaderDevise.h"
#include "Common.h"
#include "Vertex.h"
#include "Camera.h"
#include "Model.h"
#include "Plane.h"
#include "Arrow.h"
#include "Input.h"
#include "WaveScene.h"
#include "GameScene.h"
#include "DebugScene.h"
#include "SceneManager.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			return 0;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				DestroyWindow(hWnd);
			return 0;
		case WM_MOUSEMOVE:
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(133);
#endif
	MSG msg; HWND hWnd;
	WNDCLASSEX wcex ={sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, LoadCursor(NULL , IDC_ARROW),
	(HBRUSH)(COLOR_WINDOW+1), NULL, "cls_name", NULL};

	if(!RegisterClassEx(&wcex))
		return 0;

	int w = (int)Common::window_width, h = (int)Common::window_height;
	RECT clientRect = {0, 0, w, h};
	AdjustWindowRect( &clientRect, WS_OVERLAPPEDWINDOW, FALSE );

	if(!(hWnd = CreateWindow("cls_name", "GQ41", WS_OVERLAPPEDWINDOW, 0, 0,
		clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
		NULL, NULL, hInstance, NULL)))
		return 0;

	ShaderDevise* sd = (new ShaderDevise())->init(w, h, hWnd);

	ShowWindow(hWnd, nCmdShow);
	Camera::init();
	Input::init(hInstance, hWnd);

	SceneManager::init();

	DWORD fps = 60;
	// メッセージ ループ
	do{
		DWORD start_time, end_time, pass_time;
		start_time = timeGetTime();

		Input::update();

		if(InputKeyboard::isKey(DIK_R, Input::Press)) {
			fps = (fps != 60) ? fps + 1: fps;
		}
		if(InputKeyboard::isKey(DIK_T, Input::Press)) {
			fps = (fps != 0) ? fps - 1: fps;
		}

		if(fps) {
			SceneManager::update();
			SceneManager::draw();
			ShaderDevise::resetOffsetY();
		}

		end_time = timeGetTime();
		pass_time = end_time - start_time;

		DWORD _fps_ = (fps) ? fps : 60;
		DWORD wait_time = (1000 / _fps_ > pass_time) ? (1000 / _fps_ - pass_time) : 0;
		{
			DWORD start_time = timeGetTime();

			do {
				// メッセージ処理
				if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if(wait_time > 0) Sleep(1); // ちょっと休憩（CPUの占有率を下げるため）
			}while(timeGetTime() < wait_time + start_time); // wait_time だけ回る
		}

	} while(msg.message != WM_QUIT);

	SceneManager::release();
	Input::release();
	SAFE_RELEASE_DELETE(sd);

	return 0;
}