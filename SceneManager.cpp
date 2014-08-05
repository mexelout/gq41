#include "SceneManager.h"
#include "Scene.h"
#include "Common.h"
#include "DebugScene.h"
#include "Input.h"

Scene* SceneManager::scene;

SceneManager::SceneManager(void) {
	scene = NULL;
}

SceneManager::~SceneManager(void) {
}

void SceneManager::init() {
	scene = (new DebugScene)->init();
}
void SceneManager::update() {
	if(InputKeyboard::isKey(DIK_F12, Input::Trigger)) {
		setScene((new DebugScene)->init());
	}
	scene->update();
}
void SceneManager::draw() {
	scene->draw();
}
void SceneManager::release() {
	SAFE_RELEASE_DELETE(scene);
}
void SceneManager::setScene(Scene* scene) {
	SAFE_RELEASE_DELETE(SceneManager::scene);
	SceneManager::scene = scene;
}

