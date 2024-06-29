#include"Play.h"


void Play::init_play()
{
	enemies.clear();
	play_timer = 0;

	for (int x = 0; x < stage_x; x++)
	{
		for (int y = 0; y < stage_y; y++)
		{
			stage[y][x] = std::make_shared<Empty>();
		}
	}

	remaing_time = stage_infos[stage_index].time_limit;

	stage_loding();

	Shake_counter();
	c_x = player.get_pos().x + shake;
	c_y = player.get_pos().y + shake;


	

}

void Play::stage_loding()
{

	/*
	csvファイルでの数字の扱い方
	例えば数字が352となっているとき
	オブジェクトの種類が３
	move_typeが５
	move_amount2
	となるように保存する

	例外
	Playerのスポーン位置は-1
	空は0
	Enemyは401　　4で敵　01で敵の種類

	Beltの時は513 で5はベルト　右回り : 0 左回り 1  1の位で回転スピード

	ただしplayer_sponは-1で保存
	*/


	String csv_name = U"csv/Stage{}.csv"_fmt(stage_index);//読み込むファイルの名前

	//CSVファイル
	CSV stage_data(csv_name);
	if (!stage_data) {
		throw Error(U"Failed to load 'stage_data.csv'");
	}

	//縦
	for (int y = 0; y < stage_y; y++)
	{
		int x = 0;
		while (Parse<int>(stage_data[y][x]) != -2)
		{
			bool decide = false;//ブロックが決まったか


			//playerのスポーン
			if (Parse<int>(stage_data[y][x]) == Object_Kind_Player_Spon)
			{
				decide = true;
			
				player.init(x, y,stage_infos[stage_index].player_hp);
				stage[y][x] = std::make_shared<Player_Spon>(x,y);
	
			}

			//何もない
			if (decide == false && Parse<int>(stage_data[y][x]) == Object_Kind_Empty)
			{
				decide = true;
				stage[y][x] = std::make_shared<Empty>(x, y);
			}

			//Goal
			if (decide == false && Parse<int>(stage_data[y][x]) == Object_Kind_Goal)
			{
				decide = true;
				stage[y][x] = std::make_shared<Goal>(x, y);
			}

			//Star
			if (decide == false && Parse<int>(stage_data[y][x]) == Object_Kind_Star)
			{
				decide = true;
				stage[y][x] = std::make_shared<Star>(x, y);
			}

			int csv_number = Parse<int>(stage_data[y][x]);//csvに入っている数字

			int csv_object_kind = int(csv_number / 100);//入っている数字の100の位

			int csv_object_move_type = int((csv_number - csv_object_kind * 100) / 10);//入っている数字の10の位

			int csv_object_move_amount = csv_number % 10;//入っている数字の1の位

			//足場
			if (decide == false && csv_object_kind == Object_Kind_Scaffold)
			{
				decide = true;
				stage[y][x] = std::make_shared<Scaffold>(x, y, csv_object_move_type, csv_object_move_amount);
			}

			//トゲ
			if (decide == false && csv_object_kind == Object_Kind_Spike)
			{
				decide = true;
				stage[y][x] = std::make_shared<Spike>(x, y, csv_object_move_type, csv_object_move_amount);
			
			}

			//ベルト
			if (decide == false && csv_object_kind == Object_Kind_Belt)
			{
				decide = true;
				stage[y][x] = std::make_shared<Belt>(x, y, csv_object_move_type, csv_object_move_amount);

			}

			//敵のスポーン
			if (decide == false && csv_object_kind == Object_Kind_Enemy_Spon)
			{
				int k = csv_number - Object_Kind_Enemy_Spon*100;//スポーンする敵の種類



				stage[y][x] = std::make_shared<Enemy_Spon>(x, y,k);

				
				switch (k)
				{
				case 0:
					enemies << std::make_shared<Enemy0>(x,y);
					break;
				case 1:
					enemies << std::make_shared<Enemy1>(x,y);
					break;
				case 2:
					enemies << std::make_shared<Enemy2>(x,y);
					break;
				case 3:
					enemies << std::make_shared<Enemy0>(x,y);
					break;
				default:
					enemies << std::make_shared<Enemy0>(x,y);
					break;
				}
				
			}

			x++;
		}
	}

}

void Play::update_play()
{

	if (KeyS.pressed())
	{
		//scene_del /= 2;
	}


	player.update(scene_del);
	if (player.get_HP() <= 0)
	{
		player_death();
	}

	if (player.get_live() == false && player.get_can_restart())
	{
		//playerが死んだ演出が終わったら
		reset();
	}

	for (int x = 0; x < stage_x; x++)
	{
		for (int y = 0; y < stage_y; y++)
		{
			stage[y][x]->update(scene_del,player.get_pos());
		}
	}

	
	for (auto& enemy : enemies)
	{
		if (enemy->get_exit())
		{
			enemy->update(scene_del, player.get_pos());
		}
	}


	update_play_intersect_p();
	update_play_intersect_e();

	update_play_system();


}

void Play::update_play_system()
{
	c_x += (player.get_pos().x - c_x) / 16;
	c_y += (player.get_pos().y+150 - c_y) / 16;

	//カメラの揺れ
	
	Shake_counter();
	c_x += shake;
	c_y += shake;

	play_timer += scene_del;

	remaing_time -= scene_del;
	//残り時間0になったら死ぬ
	if (remaing_time < 0.5)
	{
		player_death();
	}


	//map_makeにする
	if (debug && KeyM.down() && play_scene == Play_Scene::play)
	{
		init_map_make();
	}


	//select_levelに戻るためのボタン
	if (KeyQ.down())
	{
		change_play_scene(Play_Scene::select_level);
	}
}


void Play::draw_play()
{
	for (int x = 0; x < stage_x; x++)
	{
		for (int y = 0; y < stage_y; y++)
		{
			stage[y][x]->draw(c_x, c_y, false);
		}
	}

	
	draw_play_explain();

	//残り時間が少なくなったらカウントダウンを出す
	if (remaing_time < 11)
	{

		double hsv_a = -0.3 + (remaing_time - int(remaing_time));

		hsv_a = Max(0.0, hsv_a);

		FontAsset(U"count")(int(remaing_time)).drawAt(scene_w / 2, scene_h / 2, HSV(1, 1, 1, hsv_a));
	}


	player.draw(c_x, c_y);

	for (auto& enemy : enemies)
	{
		if (enemy->get_exit())
		{
			enemy->draw(c_x, c_y);
		}
	}


	//select_levelに戻るためのボタンを描画





	//FontAsset(U"menu")(U"Level:{}"_fmt(stage_index)).draw(0, 0, Palette::Black);

	FontAsset(U"menu")(U"残り時間:{}s"_fmt(int(remaing_time))).draw(0, 100, Palette::Black);

	draw_play_debug();
}

void Play::draw_play_debug()
{
	/*
	FontAsset(U"menu")(U"[M]:Map_Make").draw(0, 150, Palette::Black);
	FontAsset(U"menu")(U"[Q]:Select_Levelに戻る").draw(0, 200,Palette::Black);
	FontAsset(U"menu")(U"[S]:スロウモーション").draw(0, 250, Palette::Black);
	*/
}

void Play::reset()
{
	remaing_time = stage_infos[stage_index].time_limit;

	player.reset();
	for (auto& enemy : enemies)
	{
		enemy->reset();
	}

	for (int x = 0; x < stage_x; x++)
	{
		for (int y = 0; y < stage_y; y++)
		{

			stage[y][x]->reset();
		}
	}


}

void Play::player_goal()
{
	change_play_scene(Play_Scene::goal);
}


void Play::draw_play_explain()
{

	FontAsset(U"menu")(U"[Q]:ステージセレクトに戻る").draw(0, 150,Palette::Black);

	if (stage_index == 0)
	{

		//操作方法の説明

		Vec2 draw_pos = Vec2{ 3,1 }*64 - Vec2{ c_x - 640,c_y - 480 };
		RectF explain1_box = RectF(draw_pos, 370, 150);
		explain1_box.draw(HSV(0, 0, 0, 0.4));
		FontAsset(U"menu")(U"[→]右上にジャンプ\n[←]左上にジャンプ").drawAt(explain1_box.center());


		draw_pos = Vec2{ 8,5 }*64 - Vec2{ c_x - 640,c_y - 480 };
		RectF explain2_box = RectF(draw_pos, 290, 150);
		explain2_box.draw(HSV(0, 0, 0, 0.4));
		FontAsset(U"menu")(U"トゲに当たると\nダメージを\n受けるぞ").draw(explain2_box.x, explain2_box.y);


		draw_pos = Vec2{ 19,2 }*64 - Vec2{ c_x - 640,c_y - 480 };
		RectF explain3_box = RectF(draw_pos, 310, 100);
		explain3_box.draw(HSV(0, 0, 0, 0.4));
		FontAsset(U"menu")(U"[Space]で弾を\n出して倒そう！").draw(explain3_box.x, explain3_box.y);


		draw_pos = Vec2{ -4,3 }*64 - Vec2{ c_x - 640,c_y - 480 };
		RectF explain4_box = RectF(draw_pos, 256, 100);
		explain4_box.draw(HSV(0, 0, 0, 0.4));
		FontAsset(U"menu")(U"制限時間内に\nクリアしよう").draw(explain4_box.x, explain4_box.y);
	}

	if (stage_index == 1)
	{
		Vec2 draw_pos = Vec2{ 3,2 }*64 - Vec2{ c_x - 640,c_y - 480 };
		RectF explain1_box = RectF(draw_pos, 300, 100);
		explain1_box.draw(HSV(0, 0, 0, 0.4));
		FontAsset(U"menu")(U"弾の反動を\n利用しよう!").drawAt(explain1_box.center());
	}
}
