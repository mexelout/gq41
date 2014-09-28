#include "TestScene.h"
#include "Common.h"
#include "ShaderDevise.h"
#include "Input.h"
#include "Particle.h"
#include "Camera.h"
#include "Grid.h"
#include "ParticleSystem.h"
//#include "SoundManager.h"
#include "Rectangle2D.h"

TestScene::TestScene(void) {
	camera_rot = D3DXVECTOR2(0, 0);
	camera_len = 0;
	particle_system = NULL;
	grid = NULL;
	rectangle = NULL;
}
TestScene::~TestScene(void) {
}
TestScene* TestScene::init() {
	particle_system = (new ParticleSystem)->init();
	grid = (new Grid)->init();
	camera_rot.x = camera_rot.y = (float)M_PI_2 / 2;
	camera_len = 5;
	Camera::setAt(D3DXVECTOR3(0, 0, 0));
//	SoundManager::inst().init();
//	SoundManager::inst().play();

	rectangle = (new Rectangle2D)->init();

	return this;
}
void TestScene::update() {
	if(InputMouse::btn(InputMouse::Right, Input::Press)) {
		camera_rot.y += InputMouse::move().x/100;
		camera_rot.x += InputMouse::move().y/100;
	}
	float len = cosf(camera_rot.x);
	camera_len -= InputMouse::wheel() / 100;
	camera_len = (camera_len < 1) ? 1 : camera_len;
	if(camera_rot.x >= (M_PI_2-0.017f)) camera_rot.x = (float)(M_PI_2-0.017f);
	else if(camera_rot.x <= (-M_PI_2+0.017f)) camera_rot.x = (float)(-M_PI_2+0.017f);
	Camera::setEye(D3DXVECTOR3(cosf(camera_rot.y)*len, sinf(camera_rot.x), sinf(camera_rot.y)*len)*camera_len);

	if(InputKeyboard::isKey(DIK_UP, Input::Trigger)) {
		particle_system->fire();
	}
	particle_system->update();

	grid->update();

	rectangle->update();
}
void TestScene::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0 );
	device->BeginScene();
	D3DXMATRIX world(Common::identity), view = Camera::view(), proj = Camera::projection();
	device->SetTransform(D3DTS_WORLD, &world);
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	grid->draw();
	particle_system->draw();

	view = Common::identity;
	proj = Camera::ortho();

	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	rectangle->draw();

	device->EndScene();
	device->Present( NULL, NULL, NULL, NULL );
}
void TestScene::release() {
//	SoundManager::inst().release();
	SAFE_RELEASE_DELETE(particle_system);
	SAFE_RELEASE_DELETE(grid);
	SAFE_RELEASE_DELETE(rectangle);
}
