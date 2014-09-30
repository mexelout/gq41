#pragma once
#include <d3dx9.h>
#include <string>

class Rectangle2D {
public:
	Rectangle2D(void);
	~Rectangle2D(void);

	virtual Rectangle2D* init();
	virtual void update();
	virtual void draw();
	virtual void release();

	Rectangle2D* getPos(D3DXVECTOR3& pos);
	Rectangle2D* getSize(D3DXVECTOR2& size);
	Rectangle2D* getAngle(float& angle);
	Rectangle2D* setPos(D3DXVECTOR3 pos);
	Rectangle2D* setSize(D3DXVECTOR2 size);
	Rectangle2D* setAngle(float angle);
	Rectangle2D* setColor(D3DXCOLOR color);
	Rectangle2D* loadTexture(std::string filename);

protected:
	LPDIRECT3DVERTEXBUFFER9 vtx;
	D3DXVECTOR3 pos;
	D3DXVECTOR2 size;
	float angle;
	UINT tex_num;
};

