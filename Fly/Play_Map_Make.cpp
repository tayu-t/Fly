#include"Play.h"

void Play::init_map_make()
{
	reset();
	map_make_option_now = false;
	play_scene = Play_Scene::map_make;
}


void Play::update_map_make()
{
	//Qキーでやめる
	if (KeyQ.down() && map_make_option_now == false)
	{
		play_scene = Play_Scene::play;
		save();
	}

	//カメラの移動
	if (KeyRight.pressed())
	{
		c_x += 400 * scene_del;
	}
	if (KeyLeft.pressed())
	{
		c_x -= 400 * scene_del;
	}
	if (KeyDown.pressed())
	{
		c_y += 400 * scene_del;
	}
	if (KeyUp.pressed())
	{
		c_y -= 400 * scene_del;
	}

	map_make_change();

}

void Play::map_make_change()
{
	double cursor_x = Cursor::Pos().x + (c_x - scene_w / 2);
	double cursor_y = Cursor::Pos().y + (c_y-scene_h/2);

	if (KeyO.down())
	{
		if (map_make_option_now == false)
		{
			map_make_option_now = true;

		}
		else
		{
			map_make_option_now = false;
		}
	}

	if (map_make_wait_time > 0)
	{
		map_make_wait_time -= scene_del;
	}

	if (map_make_wait_time < 0) { map_make_wait_time = 0; }

		//選択した場所の種類を変える
	if (MouseL.pressed() && map_make_option_now == false &&map_make_wait_time == 0)
	{

		//カーソルが選択可能の範囲内にいるか確認
		bool cursor_in_range = false;
		if (cursor_x >= 0 && cursor_x <= stage_x * 64 && cursor_y >= 0 && cursor_y <= stage_y * 64)
		{
			cursor_in_range = true;
		}


		//範囲内にいるなら変える
		if (cursor_in_range)
		{
			int select_m_x = int(cursor_x / 64);
			int select_m_y = int(cursor_y / 64);

	
			
			//その場所に敵がいたら消す
			enemies.remove_if([&](std::shared_ptr<Enemy> enemy)
				{
					bool del = false;
					if (enemy->get_spon_x() == select_m_x)
					{
						if (enemy->get_spon_y() == select_m_y)
						{
							del = true;
						}
					}

					return del;
			});

			
			

				
			


			//ブロックの追加


			//空
			if (select_object_kind == Object_Kind_Empty)
			{
				stage[select_m_y][select_m_x] = std::make_shared<Empty>(select_m_x,select_m_y);
			}

			//足場
			if (select_object_kind == Object_Kind_Scaffold)
			{
				stage[select_m_y][select_m_x] = std::make_shared<Scaffold>(select_m_x, select_m_y, select_move_type, select_move_amount);
			}

			//トゲ
			if (select_object_kind == Object_Kind_Spike)
			{
				stage[select_m_y][select_m_x] = std::make_shared<Spike>(select_m_x, select_m_y, select_move_type, select_move_amount);

			}

			//ベルト
			if (select_object_kind == Object_Kind_Belt)
			{
				stage[select_m_y][select_m_x] = std::make_shared<Belt>(select_m_x, select_m_y, select_move_type, select_move_amount);
			}

			//星
			if (select_object_kind == Object_Kind_Star)
			{
				stage[select_m_y][select_m_x] = std::make_shared<Star>(select_m_x, select_m_y);

			}


			//player_spon
			if (select_object_kind == Object_Kind_Player_Spon)
			{
				for (int x = 0; x < stage_x; x++)
				{
					for (int y = 0; y < stage_y; y++)
					{
						if (stage[y][x]->get_object_kind() == Object_Kind::Object_Kind_Player_Spon)
						{
							stage[y][x] = std::make_shared<Empty>(x,y);

						}
					}
				}

				stage[select_m_y][select_m_x] = std::make_shared<Player_Spon>(select_m_x,select_m_y);
			}

			//goal
			if (select_object_kind == Object_Kind_Goal)
			{

				for (int x = 0; x < stage_x; x++)
				{
					for (int y = 0; y < stage_y; y++)
					{
						//ゴールなら空にする
						if (stage[y][x]->get_object_kind() == Object_Kind_Goal)
						{
							stage[y][x] = std::make_shared<Empty>(x, y);
						}
						
					}
				}

				stage[select_m_y][select_m_x] = std::make_shared<Goal>(select_m_x, select_m_y);

			}

			//enemy_spon
			if (select_object_kind == Object_Kind_Enemy_Spon)
			{

				switch (select_enemy_kind)
				{
				case 0:
					enemies << std::make_shared<Enemy0>(select_m_x, select_m_y);
					break;
				case 1:
					enemies << std::make_shared<Enemy1>(select_m_x, select_m_y);
					break;
				case 2:
					enemies << std::make_shared<Enemy2>(select_m_x, select_m_y);
					break;
				case 3:
					enemies << std::make_shared<Enemy0>(select_m_x, select_m_y);
					break;
				default:
					break;
				}

				
				

				stage[select_m_y][select_m_x] = std::make_shared<Enemy_Spon>(select_m_x, select_m_y,select_enemy_kind);
			}


		}

	}
}

void Play::draw_map_make()
{
	for (int x = 0; x < stage_x; x++)
	{
		for (int y = 0; y < stage_y; y++)
		{
			stage[y][x]->draw(c_x, c_y,true);
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



	//枠線の描画
	for (int x = 0; x < stage_x; x++)
	{
		for (int y = 0; y < stage_y; y++)
		{
			Vec2 tex_pos = 64 * Vec2{ x,y } - Vec2{ c_x - scene_w / 2,c_y - scene_h / 2 };

			RectF(tex_pos, 64, 64).drawFrame(2,Palette::White);
		}
	}

	//Button
	if (map_make_option_now)
	{
		Rect(0, 0, scene_w, scene_h).draw(HSV(0, 0, 0, 0.7));
		if (SimpleGUI::Button(U"空", Vec2{ 400, 10 }, 100))
		{
			map_make_wait_time = 0.5;
			select_object_kind = Object_Kind_Empty;
		}

		if (SimpleGUI::Button(U"足場", Vec2{ 510, 10 }, 100))
		{
			map_make_wait_time = 0.5;
			select_object_kind = Object_Kind_Scaffold;
		}

		if (SimpleGUI::Button(U"トゲ", Vec2{ 620, 10 }, 100))
		{
			map_make_wait_time = 0.2;
			select_object_kind = Object_Kind_Spike;
		}

		if (SimpleGUI::Button(U"スポーン位置", Vec2{ 730, 10 }, 165))
		{
			map_make_wait_time = 0.5;
			select_object_kind = Object_Kind_Player_Spon;
		}

		if (SimpleGUI::Button(U"Goal", Vec2{ 905, 10 }, 100))
		{
			map_make_wait_time = 0.5;
			select_object_kind = Object_Kind_Goal;
		}

		if (SimpleGUI::Button(U"Enemy", Vec2{ 1015, 10 }, 100))
		{
			map_make_wait_time = 0.5;
			select_object_kind = Object_Kind_Enemy_Spon;
		}

		if (SimpleGUI::Button(U"ベルト", Vec2{ 1125, 10 }, 100))
		{
			map_make_wait_time = 0.2;
			select_object_kind = Object_Kind_Belt;
		}

		if (SimpleGUI::Button(U"星", Vec2{ 1125, 70 }, 100))
		{
			map_make_wait_time = 0.2;
			select_object_kind = Object_Kind_Star;
		}
		

		FontAsset(U"menu")(U"動き方:").draw(400, 200);
		
		if (SimpleGUI::Button(U"静", Vec2{ 400, 250 }, 40))
		{
			select_move_type = 0;
		}


		if (SimpleGUI::Button(U"↔", Vec2{ 450, 250 }, 40))
		{
			select_move_type = 1;
		}

		if (SimpleGUI::Button(U"↕", Vec2{ 500, 250 }, 40))
		{
			select_move_type = 2;
		}

		if (SimpleGUI::Button(U"○", Vec2{ 550, 250 }, 40))
		{
			select_move_type = 3;
		}

		if (SimpleGUI::Button(U"近動↔", Vec2{ 600, 250 }, 60))
		{
			select_move_type = 4;
		}

		if (SimpleGUI::Button(U"近動↕", Vec2{ 670, 250 }, 70))
		{
			select_move_type = 5;
		}

		FontAsset(U"menu")(U"動く量:").draw(400, 400);

		if (SimpleGUI::Button(U"0", Vec2{ 400, 450 }, 40))
		{
			select_move_amount = 0;
		}


		if (SimpleGUI::Button(U"1", Vec2{ 450, 450 }, 40))
		{
			select_move_amount = 1;
		}

		if (SimpleGUI::Button(U"2", Vec2{ 500, 450 }, 40))
		{
			select_move_amount = 2;
		}

		if (SimpleGUI::Button(U"3", Vec2{ 550, 450 }, 40))
		{
			select_move_amount = 3;
		}

		if (SimpleGUI::Button(U"4", Vec2{ 600, 450 }, 40))
		{
			select_move_amount = 4;
		}

		if (SimpleGUI::Button(U"5", Vec2{ 650, 450 }, 40))
		{
			select_move_amount = 5;
		}


		FontAsset(U"menu")(U"敵の種類:").draw(400, 600);

		if (SimpleGUI::Button(U"0", Vec2{ 400, 650 }, 40))
		{
			select_enemy_kind = 0;
		}


		if (SimpleGUI::Button(U"1", Vec2{ 450, 650 }, 40))
		{
			select_enemy_kind = 1;
		}

		if (SimpleGUI::Button(U"2", Vec2{ 500, 650 }, 40))
		{
			select_enemy_kind = 2;
		}

		if (SimpleGUI::Button(U"3", Vec2{ 550, 650 }, 40))
		{
			select_enemy_kind = 3;
		}
	}

	draw_map_make_debug();
}

void Play::draw_map_make_debug()
{
	FontAsset(U"menu")(U"[M]:Map_Make").draw(0, 0, Palette::Black);
	FontAsset(U"menu")(U"[O]:Map_Make_Option").draw(0, 40, Palette::Black);
	FontAsset(U"menu")(U"動き方:{}"_fmt(select_move_type)).draw(0, 100, Palette::Black);
	FontAsset(U"menu")(U"動く量:{}"_fmt(select_move_amount)).draw(0, 150, Palette::Black);
	FontAsset(U"menu")(U"敵の種類:{}"_fmt(select_enemy_kind)).draw(0, 200, Palette::Black);

}

void Play::save()
{
	//CSVにデータを保存
	String csv_name = U"csv/Stage{}.csv"_fmt(stage_index);//読み込むファイルの名前
	CSV stage_data(csv_name);
	if (!stage_data) {
		throw Error(U"Failed to load 'stage_data.csv'");
	}


	for (int y = 0; y < stage_y; y++)
	{
		for (int x = 0; x <= stage_x; x++)
		{
			if (x < stage_x)
			{

				//空白
				if (stage[y][x]->get_object_kind() == Object_Kind_Empty)
				{
					stage_data[y][x] = Format(Object_Kind_Empty);
				}

				//playerのスポーン位置
				if (stage[y][x]->get_object_kind() == Object_Kind_Player_Spon)
				{
					stage_data[y][x] = Format(Object_Kind_Player_Spon);
				}

				//Goal
				if (stage[y][x]->get_object_kind() == Object_Kind_Goal)
				{
					stage_data[y][x] = Format(Object_Kind_Goal);
				}

				//Star
				if (stage[y][x]->get_object_kind() == Object_Kind_Star)
				{
					stage_data[y][x] = Format(Object_Kind_Star);
				}

				int save_number = 0;//csvに保存する数字

				save_number += stage[y][x]->get_object_kind() * 100 + stage[y][x]->get_move_type() * 10 + stage[y][x]->get_move_amount();

				//足場
				if (stage[y][x]->get_object_kind() == Object_Kind_Scaffold)
				{
					stage_data[y][x] = Format(save_number);
				}
				//トゲ
				if (stage[y][x]->get_object_kind() == Object_Kind_Spike)
				{
					stage_data[y][x] = Format(save_number);
				}

				//Belt
				if (stage[y][x]->get_object_kind() == Object_Kind_Belt)
				{

					stage_data[y][x] = Format(save_number);
				}

				//敵のスポーン
				if (stage[y][x]->get_object_kind() == Object_Kind_Enemy_Spon)
				{
					int enemy_spon_save_number = 400 + stage[y][x]->get_enemy_kind();
					stage_data[y][x] = Format(enemy_spon_save_number);
				}

			}
			else
			{
				stage_data[y][x] = Format(-2);
			}
		}
	}
	

	stage_data.save(csv_name);
}
