#include "TestScene.h"
#include "Common.h"
#include "ShaderDevise.h"
#include "Input.h"
#include "Particle.h"
#include "Camera.h"
#include "Grid.h"

TestScene::TestScene(void) {
	camera_rot = D3DXVECTOR2(0, 0);
	camera_len = 0;
	particle = NULL;
	anim_num = 0;
}
TestScene::~TestScene(void) {
}
TestScene* TestScene::init() {
	for(int i = 0; i < 8; i++) {
		vtx_buff_vec.push_back(Common::plane(D3DXVECTOR3(1, 1, 1), D3DXVECTOR2(1.0f/8.0f*i, 0), D3DXVECTOR2(1.0f/8.0f, 1)));
	}
	particle = (new Particle)->init();
	particle->loadTexture("textures/effect_bumb.png");
	particle->setMaxLife(12)
		->setLife(0)
		->setMinScl(D3DXVECTOR3(1, 1, 1))
		->setMaxScl(D3DXVECTOR3(1, 1, 1))
		->setAnimNumMax(8)
		->setUse(false);
	grid = (new Grid)->init();
	camera_rot.x = camera_rot.y = (float)M_PI_2 / 2;
	camera_len = 5;
	Camera::setAt(D3DXVECTOR3(0, 0, 0));
	return this;
}
void TestScene::update() {
	if(InputMouse::btn(InputMouse::Right, Input::Press)) {
		camera_rot.y += InputMouse::move().x/100;
		camera_rot.x += InputMouse::move().y/100;
	}
	float len = cosf(camera_rot.x);
	camera_len -= InputMouse::wheel() / 100;
	if(camera_rot.x >= (M_PI_2-0.017f)) camera_rot.x = (float)(M_PI_2-0.017f);
	else if(camera_rot.x <= (-M_PI_2+0.017f)) camera_rot.x = (float)(-M_PI_2+0.017f);
	Camera::setEye(D3DXVECTOR3(cosf(camera_rot.y)*len, sinf(camera_rot.x), sinf(camera_rot.y)*len)*camera_len);

	if(InputKeyboard::isKey(DIK_UP, Input::Trigger)) {
		particle->setLife(0)->setUse(true);
	}

	particle->update();
	grid->update();
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
	particle->draw(vtx_buff_vec[particle->getAnimNum()]);

	device->EndScene();
	device->Present( NULL, NULL, NULL, NULL );
}
void TestScene::release() {
	SAFE_RELEASE_DELETE(particle);
	SAFE_RELEASE_DELETE(grid);
	for each(LPDIRECT3DVERTEXBUFFER9 vtx in vtx_buff_vec) SAFE_RELEASE(vtx);
}
