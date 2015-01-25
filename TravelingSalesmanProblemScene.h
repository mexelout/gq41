#pragma
#include "Scene.h"
#include <vector>

class StaticPlane;

class Arrow;

class TravelingSalesmanProblemScene : public Scene {
public:
	TravelingSalesmanProblemScene();
	~TravelingSalesmanProblemScene();
	TravelingSalesmanProblemScene* init();
	void update();
	void draw();
	void release();
private:
	StaticPlane* p;
	StaticPlane** points;

	Arrow** a;

	D3DXVECTOR2 camera_rot;
	float camera_len;
};
// end file