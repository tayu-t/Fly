#include "stdafx.h"
#include "Title.h"
Title::Title()
{
	scene_w = Scene::Width();
	scene_h = Scene::Height();
	change_scene = false;
}


void Title::init()
{
	scene_w = Scene::Width();
	scene_h = Scene::Height();
	change_scene = false;
}

void Title::update()
{
	double scene_del = Scene::DeltaTime();

	if (KeySpace.down())
	{

		//次のシーンへ
		change_scene = true;
		next_scene = Scene_State_Play;
	}

}

void Title::draw()
{
	TextureAsset(U"Sky").draw(0, 0);

	FontAsset(U"title2")(U"FLY").drawAt(scene_w / 2, 200, Palette::Black);
	FontAsset(U"title")(U"Press SpaceKey").drawAt(scene_w / 2, 700, Palette::Black);

}



