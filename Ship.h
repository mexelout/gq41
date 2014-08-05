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
	// ‚‚³æ“¾
	float getHeight(const D3DXVECTOR3& pos);
	// ‰Ÿ‚µ–ß‚·—Ê‚ğZo
	D3DXVECTOR3 hitCheck(const D3DXVECTOR3& pos);

	// ‘å–C‚Ì‘O‚É‹‚é”Ô†
	int isFrontOfCannon(const D3DXVECTOR3& pos);

	// ƒtƒŒ[ƒ€‚ÌŠÔ‚ÉˆÚ“®‚µ‚½—Ê
	D3DXVECTOR3 amountChangeMove();
	// ‰ñ“]‚Ì•Ï‰»—Ê
	float amountChangeAngle();
	// ‰¡‰ñ“]‚Ì•Ï‰»—Ê
	float amountChangeSwingAngle(void);
	// À•Wæ“¾
	D3DXVECTOR3 getPos();
	// ‰ñ“]—Êæ“¾
	float getAngle();
	// ‰¡‰ñ“]æ“¾
	float getSwingAngle();
private:
	D3DXVECTOR3 pos;
	float speed;
	D3DXVECTOR3 move;
	float swing_angle;
	float angle;
	float angle_speed;
	float change_angle;
	float change_swing_angle;
	// ƒvƒŒƒCƒ„[‚ª‹‚é‘å–C‚Ì”Ô†
	int player_front_of_cannon;

	Billboard* guide;

	LPDIRECT3DVERTEXBUFFER9 vtx;
};

