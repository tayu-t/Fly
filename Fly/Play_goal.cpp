#include"Play.h"

void Play::init_goal()
{
	goal_logo_pos = Vec2{ scene_w/2,-100 };

	//クリア情報
	stage_infos[stage_index].clear = 1;

	if (stage_infos[stage_index].get_star_num <= player.get_have_star())
	{
		stage_infos[stage_index].get_star_num = player.get_have_star();
	}

	if (stage_index + 1 < level_num)
	{
		//次のレベルを遊べるようにする
		stage_infos[stage_index + 1].can_play = 1;
	}



	/*
	//各ステージの情報を保存
	String csv_name2 = U"csv/Stage_info.csv";//読み込むファイルの名前

	//CSVファイル
	CSV stage_info(csv_name2);
	if (!stage_info) {
		throw Error(U"Failed to load 'stage_data.csv'");
	}

	int clear = stage_infos[stage_index].clear;
	int get_star = stage_infos[stage_index].get_star_num;

	//ステージ情報の取得
	stage_info[stage_index+1][1] = Format(clear);
	stage_info[stage_index+1][2] = Format(get_star);
	

	stage_info.save(csv_name2);
	*/
}

void Play::update_goal()
{

	//徐々に下降していく
	goal_logo_pos.y += (300-goal_logo_pos.y) / 8;

	if (KeySpace.down())
	{
		change_play_scene(Play_Scene::select_level);
	}


}

void Play::draw_goal()
{
	for (int x = 0; x < stage_x; x++)
	{
		for (int y = 0; y < stage_y; y++)
		{
			stage[y][x]->draw(c_x, c_y, false);
		}
	}

	player.draw(c_x, c_y);

	for (auto& enemy : enemies)
	{
		if (enemy->get_exit())
		{
			enemy->draw(c_x, c_y);
		}
	}



	Rect(0, 0, scene_w, scene_h).draw(HSV(0, 0, 0, 0.7));

	//Goalのロゴを描画
	FontAsset(U"title2")(U"GOAL!!").drawAt(goal_logo_pos,Palette::Yellow);
	
	FontAsset(U"menu")(U"クリアタイム: {}秒"_fmt(int(stage_infos[stage_index].time_limit-remaing_time))).drawAt(scene_w / 2, 500,Palette::White);

	FontAsset(U"menu")(U"Press SpaceKey: レベル選択へ").drawAt(scene_w / 2, 650,Palette::White);

	//星の外枠
	for (int i = 0; i < 3; i++)
	{
		Shape2D::Star(50, Vec2{ scene_w / 2 - 200 + 200 * i,850.0 }).drawFrame(3, Palette::Yellow);
		
	}

	//星の中身
	for (int i = 0; i < player.get_have_star(); i++)
	{
		Shape2D::Star(50, Vec2{ scene_w / 2 - 200 + 200 * i,850.0 }).draw(Palette::Yellow);
	}
}
