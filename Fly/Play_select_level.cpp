#include"Play.h"

void Play::init_select_level()
{
	wait_timer = 0.3;
}


void Play::update_select_level()
{

	for (int i = 0; i < level_num; i++)
	{
		//選択肢が押されたらplay開始
		if (select_level_box[i].leftClicked() && stage_infos[i].can_play == 1)
		{
			stage_index = i;
			change_play_scene(Play_Scene::play);
		}
	}

	if (wait_timer > 0)
	{
		wait_timer -= scene_del;
	}


	//Spaceキーがおされ、待ち時間も0になったら
	if (wait_timer <= 0)
	{
		if (KeySpace.down() || KeyEnter.down())
		{
			AudioAsset(U"decide").playOneShot(0.1);
			wait_timer = 0;
			change_play_scene(Play_Scene::play);
		}
	}

	if (KeyDown.down())
	{
		if (stage_index+1 < level_num && stage_infos[stage_index + 1].can_play == 1)
		{
			AudioAsset(U"select").playOneShot(0.1);
			stage_index++;
		}
	}

	

	if (KeyUp.down())
	{
		if (stage_index-1 >= 0 && stage_infos[stage_index - 1].can_play == 1)
		{
			AudioAsset(U"select").playOneShot(0.1);
			stage_index--;
		}
	}

	if (KeyRight.down())
	{
		if (stage_index + 4 < level_num && stage_infos[stage_index + 4].can_play == 1)
		{
			AudioAsset(U"select").playOneShot(0.1);
			stage_index += 4;
		}
	}

	if (KeyLeft.down())
	{
		if (stage_index - 4 >= 0 && stage_infos[stage_index - 4].can_play == 1)
		{
			AudioAsset(U"select").playOneShot(0.1);
			stage_index -= 4;
		}
	}
}

void Play::draw_select_level()
{
	RectF(100, 150, 1080, 790).draw(HSV(0, 0, 0, 0.7));

	FontAsset(U"title")(U"セレクトStage").drawAt(640, 50, Palette::White);
	for (int i = 0; i < level_num; i++)
	{
		select_level_box[i].draw(Palette::White);
		if (i <= 9)
		{
			FontAsset(U"menu")(U"Stage {}"_fmt(i)).drawAt(select_level_box[i].center() + Vec2{ 0,-30 }, Palette::Black);
		}
		else
		{
			FontAsset(U"menu")(U"StageEX").drawAt(select_level_box[i].center() + Vec2{ 0,-30 }, Palette::Black);
		}

		if (stage_infos[i].can_play == 0)
		{
			//遊べない
			select_level_box[i].draw(HSV(0, 0, 0, 0.5));
		}

		for (int j = 0; j < 3; j++)
		{
			Shape2D::Star(20, select_level_box[i].center() + Vec2{-40+ 45 * j ,30 }).drawFrame(3,Palette::Gray);
		}

		for (int j = 0; j < stage_infos[i].get_star_num; j++)
		{
			Shape2D::Star(20, select_level_box[i].center() + Vec2{ -40+45 * j ,30 }).draw(Palette::Red).drawFrame(0, Palette::Red);
		}
	}

	if (stage_infos[level_num-1].clear)
	{
		//最終ステージをクリアしたら
		FontAsset(U"menu")(U"Clear gg\nThank you \nfor playing").draw(select_level_box[level_num-1].pos + Vec2{ 0,200 }, Palette::Yellow);
	}

	select_level_box[stage_index].drawFrame(2, Palette::Red);

}
