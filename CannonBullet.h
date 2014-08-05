#pragma once
#include "model.h"

class MeshField;

class CannonBullet : public Model {
public:
	CannonBullet(void);
	~CannonBullet(void);
	CannonBullet* init(void);
	void update(void);
	void draw(void);
	void release(void);
	CannonBullet* fire(D3DXVECTOR3& pos, D3DXVECTOR3& speed);
	CannonBullet* hitCheckMeshField(MeshField* mesh_field);
	bool getHitFlag();
	bool getUseFlag();
private:
	D3DXVECTOR3 pos;
	D3DXVECTOR3 speed;
	bool is_hit;
	bool is_use;
};

