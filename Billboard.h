#pragma once
#include <d3dx9.h>

class Billboard {
public:
	Billboard(void);
	~Billboard(void);
	virtual Billboard* init();
	virtual void update();
	virtual void draw(LPDIRECT3DVERTEXBUFFER9 vtx);
	virtual void release();

	// ç¿ïWê›íË
	void setPos(const D3DXVECTOR3& pos);
	const D3DXVECTOR3 getPos(void);

	void setColor(D3DXCOLOR color);
	void setScale(D3DXVECTOR3 scl);
	const D3DXVECTOR3& getScale(void);
	void setDisp(bool disp_flag);
	void loadTexture(const char* filename);
	static D3DXMATRIX getInvView(const D3DXVECTOR3& pos);
protected:
	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR3 pos;
	D3DXCOLOR color;
	bool is_disp;
	D3DXVECTOR3 scl;
};

