#pragma once

#include <Windows.h>
#include <d3dx9.h>

class Scene;
class Rectangle2D;

class SceneManager {
public:
	static void init();
	static void update();
	static void draw();
	static void release();
	static void setNextScene(Scene* scene);
	static void setScene(Scene* scene);

private:
	SceneManager(void);
	~SceneManager(void);
	static Scene* scene;
	static Scene* next_scene;

	static HANDLE loading_thread;
	static Rectangle2D* loading_rect;
	static Rectangle2D* loading_background;
	static D3DXVECTOR3 loading_position;
	static float loading_alpha;
	static bool loading_end;
};

