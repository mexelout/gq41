#include "SceneManager.h"
#include "Scene.h"
#include "Common.h"
#include "DebugScene.h"
#include "Input.h"
#include "ShaderDevise.h"
#include "Rectangle2D.h"
#include "Camera.h"

#include <process.h>

unsigned __stdcall loadingThread(void *p) {
	Scene** n_scene = (Scene**)p;

	(*n_scene)->init();
	SceneManager::setScene(*n_scene);

	_endthreadex(0);
	return 0;
}

Scene* SceneManager::scene = NULL;
Scene* SceneManager::next_scene = NULL;
HANDLE SceneManager::loading_thread = NULL;
Rectangle2D* SceneManager::loading_rect = NULL;
Rectangle2D* SceneManager::loading_background = NULL;
D3DXVECTOR3 SceneManager::loading_position = Common::vec3zero;
float SceneManager::loading_alpha = 0;
bool SceneManager::loading_end = false;

SceneManager::SceneManager(void) {
}

SceneManager::~SceneManager(void) {
}

void SceneManager::init() {
	InputMouse::hideCursor();
	if(loading_rect == NULL) loading_rect = (new Rectangle2D)->init()->setSize(D3DXVECTOR2(425, 281))->loadTexture("textures/pirate_ship_clip_art.png");
	if(loading_background == NULL) loading_background = (new Rectangle2D)->init()->setSize(D3DXVECTOR2(Common::window_width, Common::window_height));
	setNextScene(new DebugScene);
}
void SceneManager::update() {
	if(next_scene == NULL) {
		if(InputKeyboard::isKey(DIK_ESCAPE, Input::Trigger)) {
			setNextScene(new DebugScene);
		}
		if(scene && loading_end == true)
			scene->update();
	}

	if(next_scene) {
		if(loading_end) {
			loading_alpha -= 0.05f;
			if(loading_alpha < 0) {
				loading_alpha = 0;
				next_scene = NULL;
			}
		} else {
			loading_alpha += 0.05f;
			if(loading_alpha > 1) {
				loading_alpha = 1;
			}
		}

		loading_position.x -= 3.f;
		loading_background->setColor(D3DXCOLOR(1, 1, 1, loading_alpha));
		loading_rect->setPos(loading_position)->setColor(D3DXCOLOR(1, 1, 1, loading_alpha));
		//loading_angle += 0.05f;

	}
}
void SceneManager::draw() {
	auto device = ShaderDevise::device();

	if(scene && loading_end == true) scene->draw();
	if(next_scene) {

		device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
		device->BeginScene();

		D3DXMATRIX world(Common::identity), view(Common::identity), proj(Camera::ortho());

		device->SetTransform(D3DTS_WORLD, &world);
		device->SetTransform(D3DTS_VIEW, &view);
		device->SetTransform(D3DTS_PROJECTION, &proj);

		loading_background->draw();
		loading_rect->draw();

		/*view = Camera::view();
		proj = Camera::projection();
		device->SetTransform(D3DTS_VIEW, &view);
		device->SetTransform(D3DTS_PROJECTION, &proj);*/


		device->EndScene();
	}

	device->Present(NULL, NULL, NULL, NULL);
}
void SceneManager::release() {
	SAFE_RELEASE_DELETE(scene);
	SAFE_RELEASE_DELETE(loading_background);
	SAFE_RELEASE_DELETE(loading_rect);
}
void SceneManager::setNextScene(Scene* scene) {
	next_scene = scene;
	loading_position = D3DXVECTOR3(342, -174, 0);
	loading_alpha = 0;
	loading_end = false;
	loading_thread = (HANDLE)_beginthreadex(NULL, 0, loadingThread, &next_scene, 0, NULL);
}

void SceneManager::setScene(Scene* scene) {
	SAFE_RELEASE_DELETE(SceneManager::scene);
	SceneManager::scene = scene;
	CloseHandle(loading_thread);
	loading_thread = NULL;
	loading_end = true;
}
