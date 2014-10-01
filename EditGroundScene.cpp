#include "EditGroundScene.h"
#include "ShaderDevise.h"
#include "Camera.h"
#include "Common.h"
#include "Input.h"
#include "GroundMesh.h"
#include "WaveMesh.h"


EditGroundScene::EditGroundScene(void) {
	camera_rot = D3DXVECTOR2(0, 0);
	camera_len = 0;
	ground_mesh = NULL;
	wave_mesh = NULL;
	idx = 0;
}
EditGroundScene::~EditGroundScene(void) {
}

EditGroundScene* EditGroundScene::init() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	camera_rot.x = camera_rot.y = (float)M_PI_2 / 2;
	camera_rot.y *= -1;
	camera_len = 1000;
	ground_mesh = (new GroundMesh)->init()->inputData("stage1");
	wave_mesh = (new WaveMesh)->init();
	Camera::setAt(D3DXVECTOR3(0, 0, 0));

	D3DMATERIAL9 mat = {0};
	mat.Power = 5;
	mat.Diffuse.r = mat.Diffuse.g = mat.Diffuse.b = mat.Diffuse.a = 1;
	mat.Ambient.r = mat.Ambient.g = mat.Ambient.b = mat.Ambient.a = 0.2f;
	mat.Emissive.r = mat.Emissive.g = mat.Emissive.b = mat.Emissive.a = 0.1f;
	mat.Specular.r = mat.Specular.g = mat.Specular.b = mat.Specular.a = 0.7f;
	device->SetMaterial(&mat);

	return this;
}
void EditGroundScene::update() {
	if(InputMouse::btn(InputMouse::Right, Input::Press)) {
		camera_rot.y += InputMouse::move().x/100;
		camera_rot.x += InputMouse::move().y/100;
	}

	float len = cosf(camera_rot.x);
	camera_len -= InputMouse::wheel() / 10;
	camera_len = (camera_len < 1) ? 1 : camera_len;
	if(camera_rot.x >= (M_PI_2-0.017f)) camera_rot.x = (float)(M_PI_2-0.017f);
	else if(camera_rot.x <= (-M_PI_2+0.017f)) camera_rot.x = (float)(-M_PI_2+0.017f);

	if(InputKeyboard::isKey(DIK_RETURN, Input::Trigger)) {
		ground_mesh->outputData("stage1");
	}

	Input::State state = (InputKeyboard::isKey(DIK_LSHIFT, Input::Press) ? (Input::Press) : (Input::Trigger));
	if(InputKeyboard::isKey(DIK_LEFT, state)) {
		idx--;
		if(idx < 0) idx = 0;
		if(idx % 128 == 127) idx++;
	}
	if(InputKeyboard::isKey(DIK_RIGHT, state)) {
		idx++;
		if(idx % 128 == 0) idx--;
	}
	if(InputKeyboard::isKey(DIK_UP, state)) {
		idx -= 128;
		if(idx < 0) idx += 128;
	}
	if(InputKeyboard::isKey(DIK_DOWN, state)) {
		idx+=128;
		if(idx > ground_mesh->horizontal * ground_mesh->vertical) idx -= 128;
	}
	if(InputKeyboard::isKey(DIK_Q, Input::Press)) {
		std::vector<float> v = ground_mesh->getMeshVertex();
		int id = idx - 128;
		float s_3 = sinf((float)M_PI/3);
		float s_2 = sinf((float)M_PI/2);
		float sub = 3;
		if(id >= 0) {
			if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] -= s_3 * s_3 * sub;
			v[id] -= s_3 * s_2 * sub;
			if((id + 1) % 128 != 0) v[id + 1] -= s_3 * s_3 * sub;
		}
		id += 128;
		if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] -= s_2 * s_3 * sub;
		v[id] -= s_2 * s_2 * sub;
		if((id + 1) % 128 != 0) v[id + 1] -= s_2 * s_3 * sub;
		id += 128;
		if(id < ground_mesh->horizontal * ground_mesh->vertical) {
			if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] -= s_3 * s_3 * sub;
			v[id] -= s_3 * s_2 * sub;
			if((id + 1) % 128 != 0) v[id + 1] -= s_3 * s_3 * sub;
		}

		ground_mesh->setMeshVertex(v);
		ground_mesh->calcNor();
	}
	if(InputKeyboard::isKey(DIK_W, Input::Press)) {
		std::vector<float> v = ground_mesh->getMeshVertex();
		int id = idx - 128;
		float s_3 = sinf((float)M_PI/3);
		float s_2 = sinf((float)M_PI/2);
		float sub = 3;
		if(id >= 0) {
			if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] += s_3 * s_3 * sub;
			v[id] += s_3 * s_2 * sub;
			if((id + 1) % 128 != 0) v[id + 1] += s_3 * s_3 * sub;
		}
		id += 128;
		if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] += s_2 * s_3 * sub;
		v[id] += s_2 * s_2 * sub;
		if((id + 1) % 128 != 0) v[id + 1] += s_2 * s_3 * sub;
		id += 128;
		if(id < ground_mesh->horizontal * ground_mesh->vertical) {
			if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] += s_3 * s_3 * sub;
			v[id] += s_3 * s_2 * sub;
			if((id + 1) % 128 != 0) v[id + 1] += s_3 * s_3 * sub;
		}

		ground_mesh->setMeshVertex(v);
		ground_mesh->calcNor();
	}

	if(InputKeyboard::isKey(DIK_A, Input::Press)) {
		std::vector<float> v = ground_mesh->getMeshVertex();
		int id = idx - 128;
		float s_3 = sinf((float)M_PI/3);
		float s_2 = sinf((float)M_PI/2);
		float sub = 3;
		float sum = 0;
		if(id >= 0) {
			if(id - 1 >= 0 && (id - 1) % 128 != 127) sum += v[id - 1];
			else sum += v[id];
			sum += v[id];
			if((id + 1) % 128 != 0) sum += v[id + 1];
			else sum += v[id];
		}
		id += 128;
		if(id - 1 >= 0 && (id - 1) % 128 != 127) sum += v[id - 1];
		else sum += v[id];
		sum += v[id];
		if((id + 1) % 128 != 0) sum += v[id + 1];
		else sum += v[id];
		id += 128;
		if(id < ground_mesh->horizontal * ground_mesh->vertical) {
			if(id - 1 >= 0 && (id - 1) % 128 != 127) sum += v[id - 1];
			else sum += v[id];
			sum += v[id];
			if((id + 1) % 128 != 0) sum += v[id + 1];
			else sum += v[id];
		}
		float avg = sum / 9;
		id = idx - 128;
		if(id >= 0) {
			if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] -= (v[id - 1] - avg) / 10;
			v[id] -= (v[id] - avg) / 10;
			if((id + 1) % 128 != 0) v[id + 1] -= (v[id + 1] - avg) / 10;
		}
		id += 128;
		if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] -= (v[id - 1] - avg) / 10;
		v[id] -= (v[id] - avg) / 10;
		if((id + 1) % 128 != 0) v[id + 1] -= (v[id + 1] - avg) / 10;
		id += 128;
		if(id < ground_mesh->horizontal * ground_mesh->vertical) {
			if(id - 1 >= 0 && (id - 1) % 128 != 127) v[id - 1] -= (v[id - 1] - avg) / 10;
			v[id] -= (v[id] - avg) / 10;
			if((id + 1) % 128 != 0) v[id + 1] -= (v[id + 1] - avg) / 10;
		}

		ground_mesh->setMeshVertex(v);
		ground_mesh->calcNor();
	}

	D3DXVECTOR3
		at((float)(idx%128)*ground_mesh->cell_size -ground_mesh->horizontal*ground_mesh->cell_size/2, 0, (float)(-idx/128)*ground_mesh->cell_size +ground_mesh->vertical*ground_mesh->cell_size/2),
		eye(at + D3DXVECTOR3(cosf(camera_rot.y)*len, sinf(camera_rot.x), sinf(camera_rot.y)*len)*camera_len);
	Camera::setAt(at);
	Camera::setEye(eye);
	wave_mesh->update();
}
void EditGroundScene::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0 );
	device->BeginScene();
	D3DXMATRIX world(Common::identity), view = Camera::view(), proj = Camera::projection();
	device->SetTransform(D3DTS_WORLD, &world);
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	{
		std::vector<float> f = ground_mesh->getMeshVertex();
		static float rot_y = 0;
		D3DXVECTOR3 at((float)(idx%128)*ground_mesh->cell_size -ground_mesh->horizontal*ground_mesh->cell_size/2+ground_mesh->cell_size, f[idx], (float)(-idx/128)*ground_mesh->cell_size +ground_mesh->vertical*ground_mesh->cell_size/2-ground_mesh->cell_size);
		CUSTOMVERTEX v[] = {
			CUSTOMVERTEX(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1), 0xff44ff44, D3DXVECTOR2(0, 0)),
			CUSTOMVERTEX(D3DXVECTOR3(-50, 100, 0), D3DXVECTOR3(0, 0, -1), 0xff44ff44, D3DXVECTOR2(0, 0)),
			CUSTOMVERTEX(D3DXVECTOR3(50, 100, 0), D3DXVECTOR3(0, 0, -1), 0xff44ff44, D3DXVECTOR2(0, 0)),
			CUSTOMVERTEX(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0,  1), 0xff44ff44, D3DXVECTOR2(0, 0)),
			CUSTOMVERTEX(D3DXVECTOR3(50, 100, 0), D3DXVECTOR3(0, 0,  1), 0xff44ff44, D3DXVECTOR2(0, 0)),
			CUSTOMVERTEX(D3DXVECTOR3(-50, 100, 0), D3DXVECTOR3(0, 0,  1), 0xff44ff44, D3DXVECTOR2(0, 0)),
		};

		D3DXMATRIX rot;
		rot_y += 0.02f;
 		D3DXMatrixRotationY(&rot, rot_y);
		for(int i = 0; i < 6; i++) {
			D3DXVECTOR4 tmp;
			D3DXVec3Transform(&tmp, &v[i].pos, &rot);
			v[i].pos = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
			v[i].pos += at;
			D3DXVec3Transform(&tmp, &v[i].nor, &rot);
			v[i].nor = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
		}

		device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, v, sizeof(CUSTOMVERTEX));
	}

	ground_mesh->draw();
	wave_mesh->draw();

	device->EndScene();
}
void EditGroundScene::release() {
	SAFE_RELEASE_DELETE(ground_mesh);
	SAFE_RELEASE_DELETE(wave_mesh);
}




