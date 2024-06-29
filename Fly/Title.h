#pragma once
#include "GameScene.h"
class Title :
    public GameScene
{
private:
	double scene_w;
	double scene_h;
public:
	Title();
	~Title() {};
	void init();
	void update();
	void draw();
};

