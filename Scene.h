#pragma once

class Scene {
public:
	Scene(void);
	~Scene(void);
	virtual Scene* init() =0;
	virtual void update() =0;
	virtual void draw()   =0;
	virtual void release()=0;
private:

};

