#include"Play.h"
void Play::update_play_intersect_e()
{
	update_play_intersect_e_s();
	update_play_intersect_eshot_s();
	update_play_intersect_eshot_p();
	update_play_intersect_eshot_e();
}

void Play::update_play_intersect_e_s()
{
	for (auto& enemy : enemies)
	{
		if (enemy->get_exit())
		{
			bool check_grounded = false;
			//enemyとobject
			int check_sx_p = int(enemy->get_pos().x / 64) - 6;
			int check_sy_p = int(enemy->get_pos().y / 64) - 6;
			int check_fx_p = int(enemy->get_pos().x / 64) + 7;
			int check_fy_p = int(enemy->get_pos().y / 64) + 7;

			for (int x = Max(0, check_sx_p); x < Min(stage_x, check_fx_p); x++)
			{
				for (int y = Max(0, check_sy_p); y < Min(stage_y, check_fy_p); y++)
				{

					if (enemy->get_hit_r().intersects(stage[y][x]->get_hit_r()))
					{
						
							//if (stage[y][x]->get_object_kind() == Object_Kind_Scaffold|| stage[y][x]->get_object_kind() == Object_Kind_Spike)
						if (stage[y][x]->get_can_hit())
						{
							//とげなら死ぬ
							if (stage[y][x]->get_object_kind() == Object_Kind_Spike)
							{
								enemy_damage(enemy);
							}


							//押し返される前のpos
							Vec2 enemy_pre_pos = enemy->get_pos();

							double e = 0.2;//反発係数

							//どの部分に当たったか確かめる


							//ブロックの左上の座標
							Vec2 stage_rect_point = Vec2{ stage[y][x]->get_hit_r().x ,stage[y][x]->get_hit_r().y };

							//playerとstagerectの相対速度
							Vec2 e_r_speed_delta = enemy->get_total_speed() - stage[y][x]->get_speed();


							bool hit_y = false;
							bool hit_x = false;


							//上面、左面に当たったのなら左上
							//下面、右面に当たったのなら右下
							//
							//

							Vec2 hit_line = { 0,0 };

							////////////上下の判定//////////////////


							if (enemy->get_pre_pos().y + enemy->get_e_h() <= stage[y][x]->get_pre_pos().y)
							{
								hit_y = true;

								//下に移動
								if (e_r_speed_delta.y >= 0)
								{
									//上辺に当たる
									hit_line = stage[y][x]->get_pos();
									check_grounded = true;
									enemy->set_ground_speed(stage[y][x]->get_fu_speed());//未来の速度を渡す

									//上に反射
									enemy->set_pos(Vec2{ enemy->get_pos().x ,stage_rect_point.y - enemy->get_e_h() });
									enemy->set_speed(Vec2{ enemy->get_speed().x,0 });
								}
							}


							if (enemy->get_pre_pos().y >= stage[y][x]->get_pre_pos().y + stage[y][x]->get_hit_r().h)
							{
								hit_y = true;

								//上に移動
								if (e_r_speed_delta.y <= 0)
								{
									//下辺に当たる
									hit_line = stage[y][x]->get_pos() + Vec2{ stage[y][x]->get_hit_r().w,stage[y][x]->get_hit_r().h };
									//下に反射
									enemy->set_pos(Vec2{ enemy->get_pos().x ,stage[y][x]->get_hit_r().y + stage[y][x]->get_hit_r().h });
									enemy->set_speed(Vec2{ enemy->get_speed().x,-e * enemy->get_speed().y });
								}
							}

							//////////////////////////////////////

							///左右の判定////////////////
							if (hit_y == false)
							{
								hit_x = true;
								//右に移動
								if (e_r_speed_delta.x >= 0)
								{
									//左辺に当たる
									hit_line = stage[y][x]->get_pos();
									//左に反射
									enemy->set_pos(Vec2{ stage_rect_point.x - enemy->get_hit_r().w,enemy->get_pos().y });
									enemy->set_speed(Vec2{ -e * e_r_speed_delta.x,enemy->get_speed().y });
								}

								//左に移動
								if (e_r_speed_delta.x <= 0)
								{
									//右辺に当たる
									hit_line = stage[y][x]->get_pos() + Vec2{ stage[y][x]->get_hit_r().w,stage[y][x]->get_hit_r().h };
									double rect_w = stage[y][x]->get_hit_r().w;
									//右に反射
									enemy->set_pos(Vec2{ stage_rect_point.x + rect_w,enemy->get_pos().y });
									enemy->set_speed(Vec2{ -e * e_r_speed_delta.x,enemy->get_speed().y });

								}

							}

							///はさまれたかどうか
							Vec2 enemy_pos_delta = enemy->get_pos() - enemy_pre_pos;

							int check_sx_p2 = int(enemy->get_pos().x / 64) - 6;
							int check_sy_p2 = int(enemy->get_pos().y / 64) - 6;
							int check_fx_p2 = int(enemy->get_pos().x / 64) + 7;
							int check_fy_p2 = int(enemy->get_pos().y / 64) + 7;

							for (int check_x = Max(0, check_sx_p2); check_x < Min(stage_x, check_fx_p2); check_x++)
							{
								for (int check_y = Max(0, check_sy_p2); check_y < Min(stage_y, check_fy_p2); check_y++)
								{
									//当たっている
									if (stage[check_y][check_x]->get_can_hit())
									{
										if (enemy->get_hit_r().intersects(stage[check_y][check_x]->get_hit_r()))
										{

											Vec2 e_r_speed_delta2 = enemy_pos_delta / scene_del - stage[check_y][check_x]->get_speed();

											int b_x = int((stage[check_y][check_x]->get_pos().x) / 64);
											int b_y = int((stage[check_y][check_x]->get_pos().y) / 64);

											b_x = x;
											b_y = y;
											//縦方向に挟まれる
											if (b_x == x && hit_y)
											{
												double enemy_line_s = enemy->get_pos().x + 5;
												double enemy_line_f = enemy->get_pos().x + enemy->get_e_w() - 5;


												double block_line_s = Max(stage[y][x]->get_hit_r().x, stage[check_y][check_x]->get_hit_r().x);
												double block_line_f = Min(stage[y][x]->get_hit_r().x + stage[y][x]->get_hit_r().w, stage[check_y][check_x]->get_hit_r().x + stage[check_y][check_x]->get_hit_r().x);

												if (enemy_line_f < block_line_s && enemy_line_s > block_line_f)
				 								{

												}
												else if (block_line_f > block_line_s)
												{
													//今当たっているブロックと前に当たったブロックの隙間がplayerの幅未満
													//下に移動していて上に押し返された
													if (e_r_speed_delta.y > 0)
													{

														double gap = hit_line.y - (stage[check_y][check_x]->get_hit_r().y + stage[check_y][check_x]->get_hit_r().h);
														if (gap > 0.1 && gap < enemy->get_hit_r().h)
														{
															enemy_damage(enemy);
														}

													}

													//上に移動していて下に押し返された
													if (e_r_speed_delta.y < 0)
													{

														double gap = (stage[check_y][check_x]->get_hit_r().y) - hit_line.y;
														if (gap > 0.1 && gap < enemy->get_hit_r().h)
														{
															enemy_damage(enemy);
														}

													}
												}
												else if (block_line_f == block_line_s)
												{
													bool can_death = false;
													if (stage[y][x]->get_pos().x < stage[check_y][check_x]->get_pos().x)
													{
														if (enemy_line_s > stage[y][x]->get_pos().x && enemy_line_s < stage[y][x]->get_pos().x + stage[y][x]->get_hit_r().w)
														{
															if (enemy_line_f > stage[check_y][check_x]->get_pos().x && enemy_line_f < stage[check_y][check_x]->get_pos().x + stage[check_y][check_x]->get_hit_r().w)
															{
																can_death = true;
															}
														}
													}

													if (stage[check_y][check_x]->get_pos().x < stage[y][x]->get_pos().x)
													{
														if (enemy_line_s > stage[check_y][check_x]->get_pos().x && enemy_line_s < stage[check_y][check_x]->get_pos().x + stage[check_y][check_x]->get_hit_r().w)
														{
															if (enemy_line_f > stage[y][x]->get_pos().x && enemy_line_f < stage[y][x]->get_pos().x + stage[y][x]->get_hit_r().w)
															{
																can_death = true;
															}
														}
													}

													if (can_death) { enemy_damage(enemy); }
												}

											}

											//横方向に挟まれる
											if (b_y == y && hit_x)
											{

												double enemy_line_s = enemy->get_pos().y + 5;
												double enemy_line_f = enemy->get_pos().y + enemy->get_e_h() - 5;


												double block_line_s = Max(stage[y][x]->get_hit_r().y, stage[check_y][check_x]->get_hit_r().y);
												double block_line_f = Min(stage[y][x]->get_hit_r().y + stage[y][x]->get_hit_r().h, stage[check_y][check_x]->get_hit_r().y + stage[check_y][check_x]->get_hit_r().h);



												if (enemy_line_f < block_line_s || enemy_line_s > block_line_f)
												{

												}
												else if (block_line_f > block_line_s)
												{
													//右に移動していて左に押し返された
													if (e_r_speed_delta.x > 0)
													{
														double gap = hit_line.x - (stage[check_y][check_x]->get_hit_r().x + stage[check_y][check_x]->get_hit_r().w);
														if (gap > 0.1 && gap < enemy->get_hit_r().w)
														{
															enemy_damage(enemy);
														}
													}

													//左に移動していて右に押し返された
													if (e_r_speed_delta.x < 0)
													{
														double gap = (stage[check_y][check_x]->get_hit_r().x) - hit_line.x;
														if (gap > 0.1 && gap < enemy->get_hit_r().w)
														{
															enemy_damage(enemy);
														}
													}
												}
												else if (block_line_f == block_line_s)
												{
													bool can_death = false;
													if (stage[y][x]->get_pos().y < stage[check_y][check_x]->get_pos().y)
													{
														if (enemy_line_s > stage[y][x]->get_pos().y && enemy_line_s < stage[y][x]->get_pos().y + stage[y][x]->get_hit_r().h)
														{
															if (enemy_line_f > stage[check_y][check_x]->get_pos().y && enemy_line_f < stage[check_y][check_x]->get_pos().y + stage[check_y][check_x]->get_hit_r().h)
															{
																can_death = true;
															}
														}
													}

													if (stage[check_y][check_x]->get_pos().y < stage[y][x]->get_pos().y)
													{
														if (enemy_line_s > stage[check_y][check_x]->get_pos().y && enemy_line_s < stage[check_y][check_x]->get_pos().y + stage[check_y][check_x]->get_hit_r().h)
														{
															if (enemy_line_f > stage[y][x]->get_pos().y && enemy_line_f < stage[y][x]->get_pos().y + stage[y][x]->get_hit_r().h)
															{
																can_death = true;
															}
														}
													}

													if (can_death) { enemy_damage(enemy); }
												}

											}
										}
									}

								}
							}
						}

						//ゴール
						if (stage[y][x]->get_object_kind() == Object_Kind_Goal)
						{

							enemy_damage(enemy);
						}
					}
				}
			}


			if (check_grounded)
			{
				enemy->set_grounded(true);
			}
			else
			{
				enemy->set_grounded(false);
				enemy->set_ground_speed(Vec2{ 0,0 });
			}
		}
	}
}

void Play::update_play_intersect_eshot_s()
{
	for (auto& enemy : enemies)
	{
		Array<std::shared_ptr<Shot>> e_shots;

		e_shots = enemy->get_shots();


		for (auto& e_shot : e_shots)
		{
			if (e_shot->get_crash() == false)
			{
				int check_pos_x = int(e_shot->get_pos().x / 64);
				int check_pos_y = int(e_shot->get_pos().y / 64);

				for (int x = Max(0, check_pos_x - 5); x < Min(stage_x, check_pos_x + 5); x++)
				{
					for (int y = Max(0, check_pos_y - 5); y < Min(stage_y, check_pos_y + 5); y++)
					{
						if (stage[y][x]->get_can_hit())
						{
							if (e_shot->get_hit_c().intersects(stage[y][x]->get_hit_r()))
							{
								e_shot->crash_init();
								break;
							}
						}
					}
				}
			}
		}

		e_shots.clear();
		
	}
}

void Play::update_play_intersect_eshot_p()
{
	for (auto& enemy : enemies)
	{
		Array<std::shared_ptr<Shot>> e_shots;

		e_shots = enemy->get_shots();

		for (auto& e_shot : e_shots)
		{

			if (e_shot->get_crash() == false)
			{
				if (player.get_live())
				{
					/*
					Vec2 e_shot_pos = e_shot->get_pos();

					Vec2 player_pos = player.get_pos();

					if (sqrt((e_shot_pos.x - player_pos.x) * (e_shot_pos.x - player_pos.x) + (e_shot_pos.y - player_pos.y) * (e_shot_pos.y - player_pos.y)) < 80)
					{
						player.hit_shot(e_shot);

					}*/
					if(player.get_hit_r().intersects(e_shot->get_hit_c()))
					{
						shake_counter = 6;
						player.damage();
						e_shot->crash_init();
					}
				}
			}
		}

		e_shots.clear();
		
	}
}

void Play::update_play_intersect_eshot_e()
{
	for (auto& enemy : enemies)
	{
		Array<std::shared_ptr<Shot>> e_shots;

		e_shots = enemy->get_shots();

		for (auto& e_shot : e_shots)
		{
			if (e_shot->get_crash() == false)
			{
				//破裂していない

				//反射された弾なら敵に当たる
				if (e_shot->get_reflected() == true)
				{

					for (auto& enemy : enemies)
					{
						if (enemy->get_live())
						{
							//生きている

							if (enemy->get_hit_r().intersects(e_shot->get_hit_c()))
							{
								enemy->damage(e_shot->get_power());
								e_shot->crash_init();
							}
						}
					}
				}
			}
		}
		
	}
}

void Play::enemy_damage(std::shared_ptr<Enemy> enemy)
{
	if (enemy->get_live())
	{
		enemy->damage(1);
	}
}
