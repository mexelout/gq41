#pragma once
#include "scene.h"
class DebugScene : public Scene {
public:
	DebugScene(void);
	~DebugScene(void);
	DebugScene* init();
	void update();
	void draw();
	void release();
private:
	int current;
};

