#include "stdafx.h"
#include "Play.h"


Play::Play()
{


	scene_w = Scene::Width();
	scene_h = Scene::Height();
	change_scene = false;

	//各ステージの情報を受け取る
	String csv_name = U"csv/Stage_info.csv";//読み込むファイルの名前

	//CSVファイル
	CSV stage_info(csv_name);
	if (!stage_info) {
		throw Error(U"Failed to load 'stage_data.csv'");
	}

	for (int i = 0; i < level_num; i++)
	{

		//レベル選択用の四角形
		int box_set_x = 200 + int(i / 4) * 300;//boxの左端のx座標

		int box_set_y = 200 + 180 * int(i % 4);

		select_level_box[i] = Rect(box_set_x, box_set_y, 250, 130);



		//ステージ情報の取得
		stage_infos[i].clear = Parse<int>(stage_info[i+1][1]);
		stage_infos[i].get_star_num = Parse<int>(stage_info[i+1][2]);
		stage_infos[i].time_limit = Parse<int>(stage_info[i + 1][3]);
		stage_infos[i].player_hp = Parse<int>(stage_info[i + 1][4]);
		stage_infos[i].can_play = Parse<int>(stage_info[i + 1][5]);
	}
}

Play::~Play()
{
	

	//各ステージの情報を保存
	String csv_name = U"csv/Stage_info.csv";//読み込むファイルの名前

	//CSVファイル
	CSV stage_info(csv_name);
	if (!stage_info) {
		throw Error(U"Failed to load 'stage_data.csv'");
	}

	for (int i = 0; i < level_num; i++)
	{
		//ステージ情報の取得
		stage_info[i+1][1] = Format(stage_infos[i].clear);
		stage_info[i + 1][2] = Format(stage_infos[i].get_star_num);
		stage_info[i + 1][5] = Format(stage_infos[i].can_play);
	}

	stage_info.save(csv_name);
	
}

void Play::init()
{
	play_scene = Play_Scene::select_level;

	change_scene = false;
	for (int x = 0; x < stage_x; x++)
	{
		for (int y = 0; y < stage_y; y++)
		{
			stage[y][x] = std::make_shared<Empty>(x, y);
		}
	}
}

void Play::update()
{
	scene_del = Scene::DeltaTime();


	if (shake_counter > 0)
	{
		Shake_counter();
	}

	

	switch (play_scene)
	{
	case  Play_Scene::select_level:
		update_select_level();
		break;
	case Play_Scene::play:
		update_play();
		break;
	case Play_Scene::map_make:
		update_map_make();
		break;
	case Play::Play_Scene::goal:
		update_goal();
		break;
	default:
		break;
	}
}

void Play::draw()
{
	TextureAsset(U"Sky").draw(0, 0);

	switch (play_scene)
	{
	case Play_Scene::select_level:
		draw_select_level();
		break;
	case Play_Scene::play:
		draw_play();
		break;
	case Play_Scene::map_make:
		draw_map_make();
		break;
	case Play::Play_Scene::goal:
		draw_goal();
		break;
	default:
		break;
	}
}

void Play::change_play_scene(Play_Scene next_play_scene)
{
	play_scene = next_play_scene;

	/// 初期化 

	switch (play_scene)
	{
	case Play::Play_Scene::select_level:
		init_select_level();
		break;
	case Play::Play_Scene::play:
		init_play();
		break;
	case Play::Play_Scene::map_make:
		init_map_make();
		break;
	case Play::Play_Scene::goal:
		init_goal();
		break;
	default:
		break;
	}
}
