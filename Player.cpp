#include "Player.h"
#include "Camera.h"
#include "ShaderDevise.h"
#include "Input.h"
#include "Common.h"


Player::Player(void) {
	_pos = _speed = D3DXVECTOR3(0, 0, 0);
	angle = 0;
	is_grounded = false;
	ground_pos_y = 0;
}

Player::~Player(void) {
}

Player* Player::init() {
	return this;
}

void Player::update() {
	updateAction();
}

void Player::draw() {
}

void Player::release() {
	Model::release();
}

void Player::updateAction() {
	D3DXVECTOR3 dir(Camera::dir());
	dir.y = 0;
	D3DXVec3Normalize(&dir, &dir);
	bool is_down_key = false;
	float mv_angle = 0;

	if(InputKeyboard::isKey(DIK_W, Input::Press)) {
		if(InputKeyboard::isKey(DIK_D, Input::Press)) {
			mv_angle = D3DX_PI*-0.25f;
		} else if(InputKeyboard::isKey(DIK_A, Input::Press)) {
			mv_angle = D3DX_PI* 0.25f;
		} else {
			mv_angle = D3DX_PI*0.0f;
		}
		is_down_key = true;
	} else if(InputKeyboard::isKey(DIK_S, Input::Press)) {
		if(InputKeyboard::isKey(DIK_D, Input::Press)) {
			mv_angle = D3DX_PI*-0.75f;
		} else if(InputKeyboard::isKey(DIK_A, Input::Press)) {
			mv_angle = D3DX_PI*0.75f;
		} else {
			mv_angle = D3DX_PI*1.0f;
		}
		is_down_key = true;
	} else if(InputKeyboard::isKey(DIK_D, Input::Press)) {
		mv_angle = -D3DX_PI*0.5f;
		is_down_key = true;
	} else if(InputKeyboard::isKey(DIK_A, Input::Press)) {
		mv_angle = D3DX_PI*0.5f;
		is_down_key = true;
	}

	if(is_down_key) {
		float s = sinf(mv_angle), c = cosf(mv_angle);
		float x = dir.x * c - dir.z * s, z = dir.x * s + dir.z * c;
		_pos.x += x*0.1f;
		_pos.z += z*0.1f;
		float d = D3DXVec3Dot(&D3DXVECTOR3(1, 0, 0), &D3DXVECTOR3(x, 0, z));
		if(d > 1) d = 1; else if(d < -1) d = -1;
		float ang = acosf(d);
		if(-z > 0) ang *= -1;
		angle = ang;
	}
}

void Player::updateGravity() {
	static float game_speed = 0.5f;
	_speed.y -= (4.9f * 0.016f * game_speed);

	float new_y = _pos.y - _speed.y;
	if(ground_pos_y + 1 < new_y) {
		is_grounded = false;
	}

	if(InputKeyboard::isKey(DIK_SPACE, Input::Trigger) && is_grounded) {
		_speed.y = 0.8f * game_speed;
		is_grounded = false;
	}

	if(is_grounded) {
		_speed.y = -10 * game_speed;
	}
	
	_pos.y += _speed.y;
	
	is_grounded = false;
	if(ground_pos_y > _pos.y) {
		_pos.y = ground_pos_y;
		_speed.y = 0;
		is_grounded = true;
	}
}


D3DXVECTOR3 Player::pos() {
	return _pos;
}

void Player::calcGround(std::array<CUSTOMVERTEX, 4> vtx) {
	std::array<D3DXVECTOR3, 4> vecs;
	for(int i = 0; i < 4; i++) vecs[i] = _pos - vtx[i].pos;
	float y1 = D3DXVec3Cross(&D3DXVECTOR3(), &(vtx[2].pos - vtx[0].pos), &vecs[2])->y;
	float y2 = D3DXVec3Cross(&D3DXVECTOR3(), &(vtx[1].pos - vtx[2].pos), &vecs[1])->y;
	float y3 = D3DXVec3Cross(&D3DXVECTOR3(), &(vtx[0].pos - vtx[1].pos), &vecs[0])->y;
	if(y1 <= 0 && y2 <= 0 && y3 <= 0) {
		ground_pos_y = vtx[0].pos.y - ((vtx[0].nor.x * (_pos.x - vtx[0].pos.x) + vtx[0].nor.z * (_pos.z - vtx[0].pos.z)) / vtx[0].nor.y);
	} else {
		y1 = D3DXVec3Cross(&D3DXVECTOR3(), &(vtx[1].pos - vtx[3].pos), &vecs[1])->y;
		y2 = D3DXVec3Cross(&D3DXVECTOR3(), &(vtx[2].pos - vtx[1].pos), &vecs[2])->y;
		y3 = D3DXVec3Cross(&D3DXVECTOR3(), &(vtx[3].pos - vtx[2].pos), &vecs[3])->y;
		if(y1 <= 0 && y2 <= 0 && y3 <= 0) {
			ground_pos_y = vtx[3].pos.y - ((vtx[3].nor.x * (_pos.x - vtx[3].pos.x) + vtx[3].nor.z * (_pos.z - vtx[3].pos.z)) / vtx[3].nor.y);
		}
	}
}

D3DXMATRIX Player::getWorld() {
	D3DXMATRIX world, rot, translate;
	D3DXMatrixIdentity(&world);
	D3DXMatrixRotationAxis(&rot, &D3DXVECTOR3(0, 1, 0), -angle - (float)M_PI_2);
	world *= rot;
	D3DXMatrixTranslation(&translate, _pos.x, _pos.y, _pos.z);
	world *= translate;
	return world;
}

// ポジション設定
void Player::setPos(D3DXVECTOR3 pos) {
	this->_pos = pos;
}


// 今居るの地面の高さを設定
void Player::setGroundHeight(float ground_height) {
	ground_pos_y = ground_height;
}

bool Player::getGrounded() {
	return is_grounded?true:false;
}

