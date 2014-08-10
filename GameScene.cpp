#include "GameScene.h"
#include "ShaderDevise.h"
#include "Camera.h"
#include "Common.h"
#include "Input.h"
#include "Spara.h"
#include "MeshField.h"
#include "Billboard.h"
#include "Ship.h"
#include "GroundMesh.h"
#include "WaveMesh.h"
#include "CannonBullet.h"
#include "ParticleSystem.h"

GameScene::GameScene(void) {
	camera_rot = D3DXVECTOR2(0, 0);
	camera_len = 25;
	spara = NULL;
	ground_mesh = NULL;
	wave_mesh = NULL;
	ship = NULL;
	cannon_bullet = NULL;
	particle_system = NULL;
}

GameScene::~GameScene(void) {
}

GameScene* GameScene::init() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();

	camera_rot.y = (float)M_PI_2;
	spara = (new Spara())->init();
	ground_mesh = (new GroundMesh)->init()->inputData("stage1");
	wave_mesh = (new WaveMesh)->init();

	particle_system = (new ParticleSystem)->init()->setStartScale(D3DXVECTOR3(2, 2, 2))->setEndScale(D3DXVECTOR3(2, 2, 2));

	InputMouse::hideCursor();
	InputMouse::fixed(true);

	ship = (new Ship)->init();

	D3DMATERIAL9 mat = {0};
	mat.Power = 5;
	mat.Diffuse.r = mat.Diffuse.g = mat.Diffuse.b = mat.Diffuse.a = 1;
	mat.Ambient.r = mat.Ambient.g = mat.Ambient.b = mat.Ambient.a = 0.7f;
	mat.Emissive.r = mat.Emissive.g = mat.Emissive.b = mat.Emissive.a = 0.1f;
	mat.Specular.r = mat.Specular.g = mat.Specular.b = mat.Specular.a = 0.2f;
	device->SetMaterial(&mat);

	cannon_bullet = new CannonBullet*[100];
	for(int i = 0; i < 100; i++)
		cannon_bullet[i] = (new CannonBullet)->init();


	return this;
}
void GameScene::update() {
	float len = cosf(camera_rot.x);
	camera_rot.y -= InputMouse::move().x/100;
	camera_rot.x -= InputMouse::move().y/100;
	if(camera_rot.x >= (M_PI_2-0.017f)) camera_rot.x = (float)(M_PI_2-0.017f);
	else if(camera_rot.x <= (-M_PI_2+0.017f)) camera_rot.x = (float)(-M_PI_2+0.017f);

	D3DXVECTOR3 ppos = spara->pos();
	Camera::setAt(ppos + (D3DXVECTOR3(cosf(camera_rot.y)*len, sinf(camera_rot.x), sinf(camera_rot.y)*len)));
	{
		static float length = 1.0f;
		if(InputMouse::btn(InputMouse::Right, Input::Press)) {
			length += (50 - length) * 0.1f;
			D3DXVECTOR3 dir(ppos - Camera::at());
			D3DXVec3Normalize(&dir, &dir);
			dir *= length;
			Camera::setEye(Camera::at() + dir);
		} else {
			length += (1.0f - length) * 0.1f;
			D3DXVECTOR3 dir(ppos - Camera::at());
			D3DXVec3Normalize(&dir, &dir);
			dir *= length;
			Camera::setEye(ppos);
		}
	}

	spara->setRotate(camera_rot.x, camera_rot.y);
	spara->update();

	ppos = spara->pos();

	ship->update();
	{
		D3DXVECTOR3 ship_pos(ship->getPos());
		float height = wave_mesh->getHeight(&ship_pos) + 1;
		float sub = height - ship_pos.y;
		ship_pos.y += sub / 50;
		ship->setPos(ship_pos);
	}

	particle_system->update();

	// 船乗ってるフラグを用意する予定(すぐじゃないよ)
	if(1) {
		float ca = ship->amountChangeAngle();
		float sa = ship->getSwingAngle();
		camera_rot.y -= ca;
		D3DXVECTOR3 m = (ship->amountChangeMove());
		ppos += m;
		float pos_y = ppos.y;
		float c, s;
		D3DXVECTOR3 ship_to_ppos(ppos - ship->getPos()), ship_to_ppos_yzero(ship_to_ppos.x, 0, ship_to_ppos.z);
		c = cosf(-ca);
		s = sinf(-ca);
		ship_to_ppos = D3DXVECTOR3(ship_to_ppos.x * c - ship_to_ppos.z * s, 0, ship_to_ppos.x * s + ship_to_ppos.z * c);
		s = sinf(sinf(sa)*0.05f) * 5.1f;
		float p_s = sinf(sinf(sa - ship->amountChangeSwingAngle())*0.05f) * 5.1f;
		float ship_angle = ship->getAngle() - (float)M_PI_2;
		D3DXVECTOR3 ship_front_vec(-cosf(ship_angle), 0, sinf(ship_angle));
		D3DXVECTOR3 prev_ship_front_vec(-cosf(ship_angle) * p_s, 0, sinf(ship_angle) * p_s);
		ppos = ship->getPos() + ship_to_ppos - (ship_front_vec * s - prev_ship_front_vec);
		spara->setGroundHeight(ship->getHeight(ppos));
		ppos += ship->hitCheck(ppos);
		ppos.y = pos_y;
		spara->setPos(ppos);
		int front_of_cannon = ship->isFrontOfCannon(ppos);
		if(front_of_cannon != -1 && InputMouse::btn(InputMouse::Left, Input::Trigger)) {
			D3DXMATRIX world(ship->getWorld());
			D3DXVECTOR3 v[] = {
				D3DXVECTOR3( 4.5f, 4.8f, - 7.69f),
				D3DXVECTOR3( 4.5f, 4.8f, -11.3f),
				D3DXVECTOR3(-4.5f, 4.8f, -11.3f),
				D3DXVECTOR3(-4.5f, 4.8f, - 7.69f),
			};
			D3DXVECTOR4 tmp;
			D3DXVec3Transform(&tmp, &v[front_of_cannon], &world);
			for(int i = 0; i < 100; i++) {
				if(!cannon_bullet[i]->getUseFlag()) {
					D3DXVECTOR3 fire_pos(tmp.x, tmp.y, tmp.z), fire_vec; 
					if(front_of_cannon < 2) {
						fire_vec = D3DXVECTOR3(ship_front_vec.z*-2, 0.1f, ship_front_vec.x*2);
					} else {
						fire_vec = D3DXVECTOR3(ship_front_vec.z*2, 0.1f, ship_front_vec.x*-2);
					}
					cannon_bullet[i]->fire(fire_pos, fire_vec);
					particle_system->fire(fire_pos + fire_vec*0.1f, fire_vec*0.1f+D3DXVECTOR3(((rand()%10)/100.0f-0.05f), ((rand()%10)/100.0f-0.05f), ((rand()%10)/100.0f-0.05f)));
					particle_system->fire(fire_pos + fire_vec*0.1f, fire_vec*0.1f+D3DXVECTOR3(((rand()%10)/100.0f-0.05f), ((rand()%10)/100.0f-0.05f), ((rand()%10)/100.0f-0.05f)));
					particle_system->fire(fire_pos + fire_vec*0.1f, fire_vec*0.1f+D3DXVECTOR3(((rand()%10)/100.0f-0.05f), ((rand()%10)/100.0f-0.05f), ((rand()%10)/100.0f-0.05f)));
					break;
				}
			}
		}
	}

	spara->updateGravity();


	Camera::setAt(spara->pos() + (D3DXVECTOR3(cosf(camera_rot.y)*len, sinf(camera_rot.x)+2.0f, sinf(camera_rot.y)*len)));
	{
		static float length = 1.0f;
		if(InputMouse::btn(InputMouse::Right, Input::Press)) {
			length += (50 - length) * 0.1f;
			D3DXVECTOR3 dir((spara->pos()+D3DXVECTOR3(0, 2.0f, 0) - Camera::at()));
			D3DXVec3Normalize(&dir, &dir);
			dir *= length;
			Camera::setEye(Camera::at() + dir);
		} else {
			length += (1.0f - length) * 0.1f;
			D3DXVECTOR3 dir((spara->pos()+D3DXVECTOR3(0, 2.0f, 0) - Camera::at()));
			D3DXVec3Normalize(&dir, &dir);
			dir *= length;
			Camera::setEye(Camera::at() + dir);

		}
	}


	wave_mesh->update();

	for(int i = 0; i < 100; i++) {
		cannon_bullet[i]->update();
		cannon_bullet[i]->hitCheckMeshField((MeshField*)wave_mesh);
	}
}
void GameScene::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();

	device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff191970, 1.0f, 0 );
	device->BeginScene();

	D3DXMATRIX world(Common::identity), view = Camera::view(), proj = Camera::projection();

	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetTransform(D3DTS_WORLD, &world);
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	spara->draw();

	ship->draw();
	for(int i = 0; i < 100; i++) {
		cannon_bullet[i]->draw();
	}

	ground_mesh->draw();
	wave_mesh->draw();


	particle_system->draw();
	ship->drawGuide();


	device->EndScene();
	device->Present( NULL, NULL, NULL, NULL );
}
void GameScene::release() {
	SAFE_RELEASE_DELETE(particle_system);
	SAFE_RELEASE_DELETE(spara);
	SAFE_RELEASE_DELETE(ground_mesh);
	SAFE_RELEASE_DELETE(wave_mesh);
	SAFE_RELEASE_DELETE(ship);
	for(int i = 0; i < 100; i++) {
		SAFE_RELEASE_DELETE(cannon_bullet[i]);
	}
	SAFE_DELETE_ARRAY(cannon_bullet);
}

