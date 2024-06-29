#pragma once
#include"GameScene.h"
#include"Title.h"
#include"Play.h"
#include<memory>

class Game
{
private:

	//GameScene* Scenes[Scene_State_Num];
	std::unique_ptr<GameScene> Scenes[Scene_State_Num];

	Scene_State scene_index = Scene_State_Title;
	Scene_State next_scene_index = Scene_State_Title;

	double change_scene_counter = 0;
	double change_scene_hsv = 0;
	double change_scene_time = 0;

	bool init_flag = false;
public:
	Game();
	~Game();
	void set_change_scene(double a);
	void call_init(int n);

	void update();

	void draw();
	bool change_scene();
};

