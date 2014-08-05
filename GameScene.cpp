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

GameScene::GameScene(void) {
	camera_rot = D3DXVECTOR2(0, 0);
	camera_len = 25;
	spara = NULL;
	ground_mesh = NULL;
	wave_mesh = NULL;
	effect_pos = D3DXVECTOR3(0, 0, 0);
	ship = NULL;
	cannon_bullet = NULL;
}

GameScene::~GameScene(void) {
}

GameScene* GameScene::init() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();

	camera_rot.y = (float)M_PI_2;
	spara = (new Spara())->init();
	ground_mesh = (new GroundMesh)->init();
	wave_mesh = (new WaveMesh)->init();

	bill_board = new Billboard*[100];
	for(int i = 0; i < 100; i++) {
		bill_board[i] = ((new Billboard)->init());
		bill_board[i]->setScale(D3DXVECTOR3(0, 0, 0));
	}

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

	{
		CUSTOMVERTEX v[] = {
			CUSTOMVERTEX(D3DXVECTOR3(-0.5f, 0.5f,  0), D3DXVECTOR3(0, 0, -1), 0xffffffff, D3DXVECTOR2(0, 0)),
			CUSTOMVERTEX(D3DXVECTOR3( 0.5f, 0.5f,  0), D3DXVECTOR3(0, 0, -1), 0xffffffff, D3DXVECTOR2(1, 0)),
			CUSTOMVERTEX(D3DXVECTOR3(-0.5f, -0.5f, 0), D3DXVECTOR3(0, 0, -1), 0xffffffff, D3DXVECTOR2(0, 1)),
			CUSTOMVERTEX(D3DXVECTOR3( 0.5f, -0.5f, 0), D3DXVECTOR3(0, 0, -1), 0xffffffff, D3DXVECTOR2(1, 1)),
		};
		device->CreateVertexBuffer(
			sizeof(CUSTOMVERTEX)*4,
			D3DUSAGE_WRITEONLY,
			D3DFVF_CUSTOMVERTEX,
			D3DPOOL_MANAGED,
			&vtx,
			NULL
		);
		CUSTOMVERTEX *data;
		vtx->Lock(0, 0, (void**)&data, 0);
		memcpy(data, v, sizeof(CUSTOMVERTEX)*4);
		vtx->Unlock();

	}

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

	// 船乗ってるフラグを用意する予定
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
		s = sinf(sinf(sa)*0.1f) * 5.1f;
		float p_s = sinf(sinf(sa - ship->amountChangeSwingAngle())*0.1f) * 5.1f;
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
			if(front_of_cannon < 2) {
				for(int i = 0; i < 100; i++) {
					if(!cannon_bullet[i]->getUseFlag()) {
						cannon_bullet[i]->fire(D3DXVECTOR3(tmp.x, tmp.y, tmp.z), D3DXVECTOR3(ship_front_vec.z*-2, 0.1f, ship_front_vec.x*2));
						break;
					}
				}
			} else {
				for(int i = 0; i < 100; i++) {
					if(!cannon_bullet[i]->getUseFlag()) {
						cannon_bullet[i]->fire(D3DXVECTOR3(tmp.x, tmp.y, tmp.z), D3DXVECTOR3(ship_front_vec.z*2, 0.1f, ship_front_vec.x*-2));
						break;
					}
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

	static float s_r = 0;
	s_r += 0.005f;
	effect_pos.x = cosf(s_r)*10 + 10;
	effect_pos.y = 2;
	effect_pos.z = sinf(s_r)*10 + 10;

	bill_board[0]->setPos(effect_pos);
	bill_board[0]->setScale(D3DXVECTOR3(1, 1, 1));

	for(int i = 99; i > 0; i--) {
		bill_board[i]->setPos(bill_board[i-1]->getPos());
	}

	for(int i = 0; i < 100; i++) {
		for(int j = i+1; j < 100; j++) {
			D3DXVECTOR3 vec[2];
			vec[0] = bill_board[i]->getPos() - Camera::eye();
			vec[1] = bill_board[j]->getPos() - Camera::eye();
			if(D3DXVec3LengthSq(&vec[0]) < D3DXVec3LengthSq(&vec[1])) {
				Billboard* tmp = bill_board[i];
				bill_board[i] = bill_board[j];
				bill_board[j] = tmp;
			}
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

	static D3DXCOLOR col(1, 1, 1, 1);
	static D3DXCOLOR sub(0.02f, 0.04f, 0.06f, 0);
	col -= sub;
	if(col.r < 0) {
		col.r = 0;
		sub.r *= -1;
	} else if(col.r > 1) {
		col.r = 1;
		sub.r *= -1;
	}
	if(col.g < 0) {
		col.g = 0;
		sub.g *= -1;
	} else if(col.g > 1) {
		col.g = 1;
		sub.g *= -1;
	}
	if(col.b < 0) {
		col.b = 0;
		sub.b *= -1;
	} else if(col.b > 1) {
		col.b = 1;
		sub.b *= -1;
	}

	ship->draw();
	for(int i = 0; i < 100; i++) {
		cannon_bullet[i]->draw();
	}

	ground_mesh->draw();
	wave_mesh->draw();

	for(int i = 0; i < 100; i++) {
		bill_board[i]->setColor(col);
		bill_board[i]->draw(vtx);
	}


	ship->drawGuide();

	device->EndScene();
	device->Present( NULL, NULL, NULL, NULL );
}
void GameScene::release() {
	SAFE_RELEASE(vtx);
	SAFE_RELEASE_DELETE(spara);
	SAFE_RELEASE_DELETE(ground_mesh);
	SAFE_RELEASE_DELETE(wave_mesh);
	SAFE_RELEASE_DELETE(ship);
	for(int i = 0; i < 100; i++) {
		SAFE_RELEASE_DELETE(cannon_bullet[i]);
	}
	SAFE_DELETE_ARRAY(cannon_bullet);
	for(int i = 0; i < 100; i++) {
		SAFE_RELEASE_DELETE(bill_board[i]);
	}
	SAFE_DELETE_ARRAY(bill_board);
}

