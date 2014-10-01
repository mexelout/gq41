#pragma once
#include "scene.h"
#include <string>
#include <vector>

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
	std::vector<std::string> menu;
};

