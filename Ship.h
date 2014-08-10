#pragma once
#include "model.h"

class Billboard;

class Ship : public Model {
public:
	Ship(void);
	~Ship(void);
	Ship* init();
	void update();
	void draw();
	void drawGuide();
	void release();
	D3DXMATRIX getWorld();
	// �����擾
	float getHeight(const D3DXVECTOR3& pos);
	// �����߂��ʂ��Z�o
	D3DXVECTOR3 hitCheck(const D3DXVECTOR3& pos);

	// ��C�̑O�ɋ���ԍ�
	int isFrontOfCannon(const D3DXVECTOR3& pos);

	// �t���[���̊ԂɈړ�������
	D3DXVECTOR3 amountChangeMove();
	// ��]�̕ω���
	float amountChangeAngle();
	// ����]�̕ω���
	float amountChangeSwingAngle(void);
	// ���W�擾
	D3DXVECTOR3 getPos();
	// ��]�ʎ擾
	float getAngle();
	// ����]�擾
	float getSwingAngle();
	// ���W�ݒ�
	Ship* setPos(D3DXVECTOR3 pos);
private:
	D3DXVECTOR3 pos;
	float speed;
	D3DXVECTOR3 move;
	float swing_angle;
	float angle;
	float angle_speed;
	float change_angle;
	float change_swing_angle;
	// �v���C���[�������C�̔ԍ�
	int player_front_of_cannon;

	Billboard* guide;

	LPDIRECT3DVERTEXBUFFER9 vtx;
};

