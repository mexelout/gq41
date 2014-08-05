#pragma once
#include "player.h"
class Spara : public Player {
public:
	Spara(void);
	~Spara(void);
	Spara* init();
	void update();
	void draw();
	void release();
	void setRotate(float x, float y);
private:
	D3DXVECTOR2 vec;
};

