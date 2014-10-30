#include <array>
#include "DebugScene.h"
#include "Common.h"
#include "ShaderDevise.h"
#include "Input.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "TestScene.h"
#include "EditGroundScene.h"
#include "WindowManager.h"
#include "TravelingSalesmanProblemScene.h"

DebugScene::DebugScene(void) {
	current = 0;
}

DebugScene::~DebugScene(void) {
	std::vector<std::string>().swap(menu);
}

DebugScene* DebugScene::init() {
	menu.push_back("Game");
	menu.push_back("EditGround");
	menu.push_back("TravelingSalesmanProblem");
	menu.push_back("Test");
	menu.push_back("Exit");
	return this;
}
void DebugScene::update() {
	if(InputKeyboard::isKey(DIK_UP, Input::Trigger)) {
		if(current > 0) {
			current -= 1;
		} else {
			current = (int)menu.size()-1;
		}
	}
	if(InputKeyboard::isKey(DIK_DOWN, Input::Trigger)) {
		if(current < (int)menu.size()-1) {
			current += 1;
		} else {
			current = 0;
		}
	}
	if(InputKeyboard::isKey(DIK_RETURN, Input::Trigger)) {
		switch(current) {
			case 0: SceneManager::setNextScene((new GameScene)); break;
			case 1: SceneManager::setNextScene((new EditGroundScene)); break;
			case 2: SceneManager::setNextScene((new TravelingSalesmanProblemScene)); break;
			case 3: SceneManager::setNextScene((new TestScene)); break;
			default: DestroyWindow(WindowManager::inst().getWnd()); break;
		}
	}
}
void DebugScene::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0 );
	device->BeginScene();

	for(int i = 0, len = menu.size(); i < len; i++) {
		ShaderDevise::drawText((menu[i] + ((i == current) ? "*" : "")).c_str());
	}

	device->EndScene();
}
void DebugScene::release() {
	DebugScene::~DebugScene();
}




