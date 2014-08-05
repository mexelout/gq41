#pragma once

class Scene;

class SceneManager {
public:
	static void init();
	static void update();
	static void draw();
	static void release();
	static void setScene(Scene* scene);
private:
	SceneManager(void);
	~SceneManager(void);
	static Scene* scene;
};

