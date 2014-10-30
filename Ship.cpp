#include <array>
#include "Ship.h"
#include "ShaderDevise.h"
#include "Common.h"
#include "Vertex.h"
#include "Billboard.h"
#include "Input.h"

Ship::Ship(void) {
	pos = D3DXVECTOR3(0, 0, 0);
	angle = 0;
	swing_angle = 0;
	speed = 0;
	move = D3DXVECTOR3(0, 0, 0);
	change_swing_angle = 0.0f;
	vtx = NULL;
}


Ship::~Ship(void) {
}

Ship* Ship::init() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	Model::init(device, "models/Galleon16.x");
	angle = (float)M_PI_2 - 0.1f;
	angle_speed = 0;
	player_front_of_cannon = 0;
	guide = (new Billboard)->init();
	guide->loadTexture("textures/guide.png");
	guide->setScale(D3DXVECTOR3(0.25f, 0.25f, 0.25f));

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
void Ship::update() {
	float prev_angle = angle;
	float prev_swing_angle = swing_angle;
	D3DXVECTOR3 prev_pos(pos);
	swing_angle += 0.06f;

	if(InputMouse::btn(InputMouse::Right, Input::Press)) {
		speed += (0.0075f * (InputKeyboard::isKey(DIK_W, Input::Press)?1.0f:-0.25f));
		angle_speed += (0.0005f * (InputKeyboard::isKey(DIK_A, Input::Press)?-1.0f:(InputKeyboard::isKey(DIK_D, Input::Press)?1.0f:0.0f)));
	} else {
		speed -= 0.0025f;
	}
	angle_speed *= 0.989f;
	if(speed < 0) speed = 0;
	else if(speed > 1.9f) speed = 1.9f;
	if(angle_speed < -0.006f) angle_speed = -0.006f;
	else if(angle_speed > 0.006f) angle_speed = 0.006f;

	pos += D3DXVECTOR3(-cosf(angle-(float)M_PI_2), 0, sinf(angle-(float)M_PI_2))*speed;
	angle += angle_speed;

	change_angle = angle - prev_angle;
	change_swing_angle = swing_angle - prev_swing_angle;
	move = pos - prev_pos;
}
void Ship::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();

	D3DXMATRIX world(getWorld());

	device->SetTransform(D3DTS_WORLD, &world);

	LPD3DXMESH mesh = Model::mesh();
	D3DXMATERIAL* mat = (D3DXMATERIAL*)Model::materials()->GetBufferPointer();
	int num_mat = Model::numMaterials();
	std::vector<LPDIRECT3DTEXTURE9> tex = Model::textures();

	D3DMATERIAL9 tmp_mat = {0};
	device->GetMaterial(&tmp_mat);
	device->SetFVF(D3DFVF_CUSTOMMODEL);
	for(int i = 0; i < num_mat; i++) {
		device->SetTexture(0, tex[i]);
		device->SetMaterial(&mat[i].MatD3D);
		mesh->DrawSubset(i);
	}
	device->SetMaterial(&tmp_mat);
	device->SetTexture(0, NULL);


	// Ç†ÇΩÇËîªíËópÇÃÉÅÉÇ
	if(0) {
		// è∞
		CUSTOMVERTEX v[4] = {
			CUSTOMVERTEX(-4.5f, 4.2625f,   0, 0, 1, 0, 0x880000ff, 0, 0),
			CUSTOMVERTEX( 4.5f, 4.2625f,   0, 0, 1, 0, 0x880000ff, 0, 0),
			CUSTOMVERTEX(-4.5f, 4.2625f, -15, 0, 1, 0, 0x880000ff, 0, 0),
			CUSTOMVERTEX( 4.5f, 4.2625f, -15, 0, 1, 0, 0x880000ff, 0, 0),
		};
		for(int i = 0; i < 4; i++) { 
			D3DXVECTOR4 tmp;
			D3DXVec3Transform(&tmp, &v[i].pos, &world);
			v[i].pos.x = tmp.x;
			v[i].pos.y = tmp.y;
			v[i].pos.z = tmp.z;
		}
		D3DXMatrixIdentity(&world);
		device->SetTransform(D3DTS_WORLD, &world);

		device->SetFVF(D3DFVF_CUSTOMVERTEX);
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(CUSTOMVERTEX));
	}
	if(0) {
		// ï«
		CUSTOMVERTEX v[] = {
			CUSTOMVERTEX(-4.5f, 5.2625f,       0, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 4.5f, 5.2625f,       0, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 4.5f, 5.2625f,  -7.09f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 2.5f, 5.2625f,  -7.09f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 2.5f, 5.2625f,  -8.29f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 4.5f, 5.2625f,  -8.29f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 4.5f, 5.2625f,  -10.7f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 2.5f, 5.2625f,  -10.7f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 2.5f, 5.2625f,  -11.9f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 4.5f, 5.2625f,  -11.9f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX( 4.5f, 5.2625f,     -15, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-4.5f, 5.2625f,     -15, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-4.5f, 5.2625f,  -7.09f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-2.5f, 5.2625f,  -7.09f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-2.5f, 5.2625f,  -8.29f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-4.5f, 5.2625f,  -8.29f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-4.5f, 5.2625f,  -10.7f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-2.5f, 5.2625f,  -10.7f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-2.5f, 5.2625f,  -11.9f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-4.5f, 5.2625f,  -11.9f, 0, 1, 0, 0xffffffff, 0, 0),
			CUSTOMVERTEX(-4.5f, 5.2625f,       0, 0, 1, 0, 0xffffffff, 0, 0),
		};
		int len = sizeof(v) / sizeof(CUSTOMVERTEX);
		for(int i = 0; i < len; i++) { 
			D3DXVECTOR4 tmp;
			D3DXVec3Transform(&tmp, &v[i].pos, &world);
			v[i].pos.x = tmp.x;
			v[i].pos.y = tmp.y;
			v[i].pos.z = tmp.z;
		}
		D3DXMatrixIdentity(&world);
		device->SetTransform(D3DTS_WORLD, &world);

		device->SetFVF(D3DFVF_CUSTOMVERTEX);
		device->DrawPrimitiveUP(D3DPT_LINESTRIP, len-1, v, sizeof(CUSTOMVERTEX));
	}
}

void Ship::drawGuide() {
	if(player_front_of_cannon != -1) {
		guide->draw(vtx);
	}
}

void Ship::release() {
	SAFE_RELEASE(vtx);
	SAFE_RELEASE_DELETE(guide);
	Model::release();
}

D3DXMATRIX Ship::getWorld() {
	D3DXMATRIX world(Common::identity), rot_x, rot_y, translate;
	D3DXMatrixTranslation(&translate, this->pos.x, this->pos.y, this->pos.z);
	D3DXMatrixRotationX(&rot_x, sinf(swing_angle)*0.05f);
	D3DXMatrixRotationY(&rot_y, angle);
	world = world * rot_x * rot_y * translate;
	return world;
}

// çÇÇ≥éÊìæ
float Ship::getHeight(const D3DXVECTOR3& pos) {
	D3DXVECTOR3 v[4] = {
		D3DXVECTOR3(-4.5f, 4.2625f,   0),
		D3DXVECTOR3( 4.5f, 4.2625f,   0),
		D3DXVECTOR3(-4.5f, 4.2625f, -15),
		D3DXVECTOR3( 4.5f, 4.2625f, -15),
	};
	D3DXVECTOR3 nor;
	D3DXMATRIX world(getWorld()), rot_x, rot_y, translate;

	for(int i = 0; i < 4; i++) { 
		D3DXVECTOR4 tmp;
		D3DXVec3Transform(&tmp, &v[i], &world);
		v[i].x = tmp.x;
		v[i].y = tmp.y;
		v[i].z = tmp.z;
	}

	D3DXVec3Cross(&nor, &(v[1] - v[0]), &(v[2] - v[0]));
	D3DXVec3Normalize(&nor, &nor);

	std::array<D3DXVECTOR3, 4> vecs;
	for(int i = 0; i < 4; i++) vecs[i] = pos - v[i];
	float y1 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[2] - v[0]), &vecs[2])->y;
	float y2 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[1] - v[2]), &vecs[1])->y;
	float y3 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[0] - v[1]), &vecs[0])->y;
	if(y1 <= 0 && y2 <= 0 && y3 <= 0) {
		float y = v[0].y - ((nor.x * (pos.x - v[0].x) + nor.z * (pos.z - v[0].z)) / nor.y);
		return  y;
	} else {
		y1 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[1] - v[3]), &vecs[1])->y;
		y2 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[2] - v[1]), &vecs[2])->y;
		y3 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[3] - v[2]), &vecs[3])->y;
		if(y1 <= 0 && y2 <= 0 && y3 <= 0) {
			float y = v[3].y - ((nor.x * (pos.x - v[3].x) + nor.z * (pos.z - v[3].z)) / nor.y);
			return y;
		}
	}
	return 0;
}

// âüÇµñﬂÇ∑ó ÇéZèo
D3DXVECTOR3 Ship::hitCheck(const D3DXVECTOR3& pos) {
	D3DXVECTOR3 v[] = {
		D3DXVECTOR3(-4.5f, 0,      0),
		D3DXVECTOR3( 4.5f, 0,      0),
		D3DXVECTOR3( 4.5f, 0, -7.09f),
		D3DXVECTOR3( 2.5f, 0, -7.09f),
		D3DXVECTOR3( 2.5f, 0, -8.29f),
		D3DXVECTOR3( 4.5f, 0, -8.29f),
		D3DXVECTOR3( 4.5f, 0, -10.7f),
		D3DXVECTOR3( 2.5f, 0, -10.7f),
		D3DXVECTOR3( 2.5f, 0, -11.9f),
		D3DXVECTOR3( 4.5f, 0, -11.9f),
		D3DXVECTOR3( 4.5f, 0,    -15),
		D3DXVECTOR3(-4.5f, 0,    -15),
		D3DXVECTOR3(-4.5f, 0, -11.9f),
		D3DXVECTOR3(-2.5f, 0, -11.9f),
		D3DXVECTOR3(-2.5f, 0, -10.7f),
		D3DXVECTOR3(-4.5f, 0, -10.7f),
		D3DXVECTOR3(-4.5f, 0, -8.29f),
		D3DXVECTOR3(-2.5f, 0, -8.29f),
		D3DXVECTOR3(-2.5f, 0, -7.09f),
		D3DXVECTOR3(-4.5f, 0, -7.09f),
	};
	D3DXVECTOR3 nor;
	D3DXMATRIX world(getWorld());
	int num = sizeof(v) / sizeof(D3DXVECTOR3);
	for(int i = 0; i < num; i++) { 
		D3DXVECTOR4 tmp;
		D3DXVec3Transform(&tmp, &v[i], &world);
		v[i].x = tmp.x;
		v[i].y = 0;
		v[i].z = tmp.z;
	}

	D3DXVECTOR3 ret(0, 0, 0);
	D3DXVECTOR3 pos_y_zero(pos);
	pos_y_zero.y = 0;
	for(int i = 0; i < num; i++) {
		D3DXVECTOR3 line(v[(i+1)%num] - v[i]);
		D3DXVECTOR3 ctop(pos_y_zero - v[i]);
		D3DXVECTOR3 crs;
		D3DXVec3Cross(&crs, &line, &ctop);
		float len = 0;
		if(crs.y <= 0) continue;
		float t = (ctop.x*line.x+ctop.z*line.z)/(line.x*line.x+line.z*line.z);
		if(0 <= t && t <= 1) {
			D3DXVECTOR3 pcross(line*t+v[i]);
			D3DXVECTOR3 tpos(pos_y_zero);
			pcross.y = 0;
			tpos.y = 0;
			D3DXVECTOR3 d(tpos - pcross);
			len = D3DXVec3Length(&d);
			if(len < 1) {
				len = 1-len;
				D3DXVec3Normalize(&d, &d);
				d *= len;
				ret += d;
				continue;
			}
		}
		len = D3DXVec3LengthSq(&ctop);
		if(len < 1) {
			len = 1-len;
			D3DXVECTOR3 nor;
			D3DXVec3Normalize(&nor, &ctop);
			nor *= len;
			ret += nor;
			continue;
		}
	}
	return ret;
}

int Ship::isFrontOfCannon(const D3DXVECTOR3& pos) {
	D3DXVECTOR3 v[] = {
		D3DXVECTOR3(1.5f, 6, -7.69f),
		D3DXVECTOR3(1.5f, 6, -11.3f),
		D3DXVECTOR3(-1.5f, 6, -11.3f),
		D3DXVECTOR3(-1.5f, 6, -7.69f),
	};
	D3DXVECTOR3 pos_y_zero(pos);
	pos_y_zero.y = 0;
	D3DXMATRIX world(getWorld());

	int num = sizeof(v) / sizeof(D3DXVECTOR3);
	for(int i = 0; i < num; i++) {
		D3DXVECTOR4 tmp;
		D3DXVec3Transform(&tmp, &v[i], &world);
		v[i] = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
	}

	player_front_of_cannon = -1;
	for(int i = 0; i < num; i++) {
		D3DXVECTOR3 v_zero(v[i]);
		v_zero.y = 0;
		D3DXVECTOR3 ctop(pos_y_zero - v_zero);
		float len = D3DXVec3LengthSq(&ctop);
		if(len < 1) {
			player_front_of_cannon = i;
			break;
		}
	}

	if(player_front_of_cannon != -1) {
		D3DXVECTOR3 v2[] = {
			D3DXVECTOR3(2.5f, 6, -7.69f),
			D3DXVECTOR3(2.5f, 6, -11.3f),
			D3DXVECTOR3(-2.5f, 6, -11.3f),
			D3DXVECTOR3(-2.5f, 6, -7.69f),
		};
		for(int i = 0; i < num; i++) {
			D3DXVECTOR4 tmp;
			D3DXVec3Transform(&tmp, &v2[i], &world);
			v2[i] = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
		}

		guide->setPos(v2[player_front_of_cannon]);
	}

	return player_front_of_cannon;
}

// ÉtÉåÅ[ÉÄÇÃä‘Ç…à⁄ìÆÇµÇΩó 
D3DXVECTOR3 Ship::amountChangeMove() {
	return move;
}
// âÒì]ÇÃïœâªó 
float Ship::amountChangeAngle() {
	return change_angle;
}

// ç¿ïWéÌìó
D3DXVECTOR3 Ship::getPos() {
	return pos;
}


// ècâÒì]ÇÃïœâªó 
float Ship::amountChangeSwingAngle(void) {
	return change_swing_angle;
}

// âÒì]ó éÊìæ
float Ship::getAngle() {
	return angle;
}

// â°âÒì]éÊìæ
float Ship::getSwingAngle() {
	return swing_angle;
}

// ç¿ïWê›íË
Ship* Ship::setPos(D3DXVECTOR3 pos) {
	this->pos = pos;
	return this;
}

