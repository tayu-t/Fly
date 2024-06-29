#include "stdafx.h"
#include"Game.h"

Game::Game()
{
	Scenes[Scene_State_Title] = std::make_unique<Title>();
	Scenes[Scene_State_Play] = std::make_unique<Play>();

}

Game::~Game()
{
	//delete Scenes[Scene_State_Title];
	//delete Scenes[Scene_State_Play];
}

void Game::set_change_scene(double a = 5)
{
	change_scene_time = a;
	change_scene_counter = a;
	init_flag = false;
}

bool Game::change_scene()
{
	double scene_del = Scene::DeltaTime();
	double t;
	change_scene_counter -= scene_del;
	t = -1 * (change_scene_counter - change_scene_time);
	if (change_scene_counter > 0)
	{
		change_scene_hsv = ((change_scene_time / 2) - 2 * (t - change_scene_time / 2) * (t - change_scene_time / 2) / change_scene_time) / (change_scene_time / 2);
		if (change_scene_counter < change_scene_time / 2)
		{
			if (scene_index != next_scene_index)
			{
				scene_index = next_scene_index;
			}
			if (init_flag == false)
			{
				call_init(scene_index);
			}
		}
		return true;
	}
	else
	{
		change_scene_counter = 0;
		return false;
	}
}

void Game::call_init(int next_scene)
{
	init_flag = true;
	Scenes[next_scene]->init();
}

void Game::update()
{
	//マウス消す
	//Cursor::RequestStyle(CursorStyle::Hidden);

	if (change_scene() == false)
	{
		Scenes[scene_index]->update();

		if (Scenes[scene_index]->get_change_scene() == true)
		{
			next_scene_index = Scenes[scene_index]->get_next_scene();
			set_change_scene();
		}
	}
}


void Game::draw()
{
	Scenes[scene_index]->draw();

	if (change_scene())
	{
		Rect(0, 0, Scene::Width(), Scene::Height()).draw(HSV(0, 0, 0, change_scene_hsv));
		if (change_scene_hsv >= 0.85)
		{
			FontAsset(U"title")(U"なうろーでぃんぐ").drawAt(Scene::Center());
		}
	}
}
