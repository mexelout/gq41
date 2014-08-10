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
	// 高さ取得
	float getHeight(const D3DXVECTOR3& pos);
	// 押し戻す量を算出
	D3DXVECTOR3 hitCheck(const D3DXVECTOR3& pos);

	// 大砲の前に居る番号
	int isFrontOfCannon(const D3DXVECTOR3& pos);

	// フレームの間に移動した量
	D3DXVECTOR3 amountChangeMove();
	// 回転の変化量
	float amountChangeAngle();
	// 横回転の変化量
	float amountChangeSwingAngle(void);
	// 座標取得
	D3DXVECTOR3 getPos();
	// 回転量取得
	float getAngle();
	// 横回転取得
	float getSwingAngle();
	// 座標設定
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
	// プレイヤーが居る大砲の番号
	int player_front_of_cannon;

	Billboard* guide;

	LPDIRECT3DVERTEXBUFFER9 vtx;
};

