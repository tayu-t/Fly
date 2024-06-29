# include <Siv3D.hpp>
#include"Game.h"

/*
* 作成したレベル
* 0
* 1
* 2
* 3
* 4
* 5
* 6
* 7
* 8
* 9
*/




void init()
{
	Scene::SetBackground(Palette::Skyblue);
	Scene::Resize(1280, 960);//64*20,64*15,  640,480
	Scene::SetResizeMode(ResizeMode::Keep);
	//Window::Resize(1280, 960);
	//Window::SetFullscreen(true);

	//Font
	FontAsset::Register(U"title", 80, Resource(U"font/Retro.ttf"));
	FontAsset::Register(U"title2", 160, Resource(U"font/Retro.ttf"));
	FontAsset::Register(U"menu", 40, Resource(U"font/Retro.ttf"));
	FontAsset::Register(U"sentence", 20, Resource(U"font/Retro.ttf"));
	FontAsset::Register(U"sentence2", 30, Resource(U"font/Retro.ttf"));
	FontAsset::Register(U"count", 390, Resource(U"font/Retro.ttf"));

	//Texture
	TextureAsset::Register(U"Bard_r_1", Resource(U"gazou/Bard_r_1.png"));
	TextureAsset::Register(U"Bard_r_2", Resource(U"gazou/Bard_r_2.png"));
	TextureAsset::Register(U"Bard_r_3", Resource(U"gazou/Bard_r_3.png"));
	TextureAsset::Register(U"Bard_r_4", Resource(U"gazou/Bard_r_4.png"));



	TextureAsset::Register(U"Bard_l_1", Resource(U"gazou/Bard_l_1.png"));
	TextureAsset::Register(U"Bard_l_2", Resource(U"gazou/Bard_l_2.png"));
	TextureAsset::Register(U"Bard_l_3", Resource(U"gazou/Bard_l_3.png"));
	TextureAsset::Register(U"Bard_l_4", Resource(U"gazou/Bard_l_4.png"));

	TextureAsset::Register(U"Enemy0_l", Resource(U"gazou/enemy0_l.png"));
	TextureAsset::Register(U"Enemy0_r", Resource(U"gazou/enemy0_r.png"));

	TextureAsset::Register(U"Player_Shot", Resource(U"gazou/Player_Shot.png"), TextureDesc::Mipped);
	TextureAsset::Register(U"Enemy_Shot", Resource(U"gazou/Enemy_Shot.png"));

	TextureAsset::Register(U"heart", Resource(U"gazou/heart.png"));
	TextureAsset::Register(U"heartframe", Resource(U"gazou/heartframe.png"));
	TextureAsset::Register(U"Spike", Resource(U"gazou/Spike.png"));
	TextureAsset::Register(U"Scaffold", Resource(U"gazou/Scaffold.png"));
	TextureAsset::Register(U"Goal", Resource(U"gazou/Goal.png"));

	TextureAsset::Register(U"Belt_r_1", Resource(U"gazou/Belt_r_1.png"));
	TextureAsset::Register(U"Belt_r_2", Resource(U"gazou/Belt_r_2.png"));
	TextureAsset::Register(U"Belt_r_3", Resource(U"gazou/Belt_r_3.png"));
	TextureAsset::Register(U"Belt_r_4", Resource(U"gazou/Belt_r_4.png"));

	TextureAsset::Register(U"Belt_l_1", Resource(U"gazou/Belt_l_1.png"));
	TextureAsset::Register(U"Belt_l_2", Resource(U"gazou/Belt_l_2.png"));
	TextureAsset::Register(U"Belt_l_3", Resource(U"gazou/Belt_l_3.png"));
	TextureAsset::Register(U"Belt_l_4", Resource(U"gazou/Belt_l_4.png"));

	TextureAsset::Register(U"Sky", Resource(U"gazou/Sky.png"));

	//Audio
	AudioAsset::Register(U"bgm", Resource(U"audio/bgm.mp3"),Loop::Yes);
	AudioAsset::Register(U"select", Resource(U"audio/select.mp3"));
	AudioAsset::Register(U"decide", Resource(U"audio/decide.mp3"));
}

void Main()
{
	
	init();
	Game game;
	AudioAsset(U"bgm").setVolume(0.3);
	AudioAsset(U"bgm").play();
	while (System::Update())
	{
		game.update();
		game.draw();
	}
}
