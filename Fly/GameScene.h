#pragma once
#include"Common.h"

class GameScene
{
protected:
	//画面の揺れ
	double shake{ 0 };
	double shake_counter{ 0 };

	double scene_del{0};

	bool change_scene = false;

	Scene_State next_scene = Scene_State_Title;
public:
	virtual ~GameScene() {};

	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	void Shake_counter();
	bool get_change_scene() { return change_scene; }
	Scene_State get_next_scene() { return next_scene; }
};

