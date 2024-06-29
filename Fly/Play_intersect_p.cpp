#include"Play.h"

void Play::update_play_intersect_p()
{
	update_play_intersect_p_s();
	update_play_intersect_pshot_s();
	update_play_intersect_pshot_e();
	update_play_intersect_p_e();
}

void Play::update_play_intersect_p_s()
{
	bool check_grounded = false;
	

	//playerとobject
	int check_sx_p = int(player.get_pos().x / 64) - 6;
	int check_sy_p = int(player.get_pos().y / 64) - 6;
	int check_fx_p = int(player.get_pos().x / 64) + 7;
	int check_fy_p = int(player.get_pos().y / 64) + 7;

	for (int x = Max(0, check_sx_p); x < Min(stage_x, check_fx_p); x++)
	{
		for (int y = Max(0, check_sy_p); y < Min(stage_y, check_fy_p); y++)
		{

			if (player.get_hit_r().intersects(stage[y][x]->get_hit_r()))
			{

				//足場またはトゲ
				//if (stage[y][x]->get_object_kind() == Object_Kind_Scaffold|| stage[y][x]->get_object_kind() == Object_Kind_Spike)
				if(stage[y][x]->get_can_hit())
				{

					//とげならダメージ
					if (stage[y][x]->get_object_kind() == Object_Kind_Spike)
					{
						shake_counter = 6;
						player.damage();
					}




					//押し返される前のpos
					Vec2 player_pre_pos = player.get_pos();

					double e = 0.2;//反発係数

					//どの部分に当たったか確かめる

					//補正値はplayerのRectがobjectのRectにめり込んだときも計算できるようにするため

					//ブロックの左上の座標
					Vec2 stage_rect_point = Vec2{ stage[y][x]->get_hit_r().x ,stage[y][x]->get_hit_r().y };

					//playerとstagerectの相対速度
					Vec2 p_r_speed_delta = player.get_total_speed() - stage[y][x]->get_speed();


					bool hit_y = false;
					bool hit_x = false;

					//playerがあったところのブロックの面のposを記録
					//上面、左面に当たったのなら左上
					//下面、右面に当たったのなら右下
					//
					//

					Vec2 hit_line = { 0,0 };

					////////////上下の判定//////////////////

					//もともとplayerの下面がblockの上にいたがこのフレームでは重なっている
					if (player.get_pre_pos().y + player.get_p_h() <= stage[y][x]->get_pre_pos().y)
					{
						

						//下に移動
						if (p_r_speed_delta.y >= 0)
						{
							hit_y = true;

							//上辺に当たる
							hit_line = stage[y][x]->get_pos();
							check_grounded = true;
							//player.set_ground_speed(stage[y][x]->get_fu_speed());//未来の速度を渡す


							if (stage[y][x]->get_object_kind() == Object_Kind_Belt)
							{
								//ベルトコンベアの時はplayerを指定の方向に移動（飛ばすのでspeedを変える）
								player.set_speed(stage[y][x]->get_fu_speed());
							
							}
							else
							{
								player.set_ground_speed(stage[y][x]->get_fu_speed());//未来の速度を渡す

							}


							//上に反射
							player.set_pos(Vec2{ player.get_pos().x ,stage_rect_point.y - player.get_p_h() });
							player.set_speed(Vec2{ player.get_speed().x,0 });
						}
					}

					//もともとplayerの上面がブロックの下にいたがこのフレームでは重なっている
					if (player.get_pre_pos().y >= stage[y][x]->get_pre_pos().y + stage[y][x]->get_hit_r().h)
					{
						
						double rate;
						double player_line_s = player.get_pos().x;
						double player_line_f = player.get_pos().x + player.get_p_w();


						double block_line_s = stage[y][x]->get_hit_r().x;
						double block_line_f = stage[y][x]->get_hit_r().x + stage[y][x]->get_hit_r().w;

						double p_dis = player_line_f - player_line_s;

						double b_dis = block_line_f - block_line_s;

						//線分の含有量(一致率)を調べる
						if (block_line_s >= player_line_s && block_line_f >= player_line_f)
						{
							double minasu = block_line_f - player_line_f;

							rate = (player_line_f - block_line_s) / p_dis;
						}
						else if (block_line_s >= player_line_s && block_line_f <= player_line_f)
						{
							rate = b_dis / p_dis;
						}
						else if (block_line_s <= player_line_s && block_line_f <= player_line_f)
						{
							double mainasu = player_line_s - block_line_s;
							rate = (block_line_f - player_line_s) / p_dis;
						}
						else if (block_line_s <= player_line_s && block_line_f >= player_line_f)
						{
							rate = 1;
						}
						//上に移動
						if (rate >= 0.01&&p_r_speed_delta.y <= 0)
						{
							hit_y = true;

							//下辺に当たる
							hit_line = stage[y][x]->get_pos() + Vec2{ stage[y][x]->get_hit_r().w,stage[y][x]->get_hit_r().h };
							//下に反射
							player.set_pos(Vec2{ player.get_pos().x ,1+stage[y][x]->get_hit_r().y + stage[y][x]->get_hit_r().h });
							player.set_speed(Vec2{ player.get_speed().x,-e * player.get_speed().y });
						}
					}

					//////////////////////////////////////

					///左右の判定////////////////
					if (hit_y == false)
					{
						


						//右に移動
						//2つ目の条件がないとplayerがブロックをすり抜ける
						//具体的に言うとplayerが乗っているブロックが向きを切り替えるときに
						//playerのtotal_speedが一瞬進行方向とは逆になるためすり抜ける
						//

						if (p_r_speed_delta.x > 0 && player.get_pre_pos().x + player.get_hit_r().w <= stage[y][x]->get_pre_pos().x)
						{
							hit_x = true;

							//左辺に当たる
							hit_line = stage[y][x]->get_pos();
						
							//左に反射
							player.set_pos(Vec2{ stage_rect_point.x - player.get_hit_r().w,player.get_pos().y });
							player.set_speed(Vec2{ -e * p_r_speed_delta.x,player.get_speed().y });
						}

						//左に移動
						if (p_r_speed_delta.x < 0 && player.get_pre_pos().x >=  stage[y][x]->get_pre_pos().x+ stage[y][x]->get_hit_r().w)
						{
							hit_x = true;
						
							//右辺に当たる
							hit_line = stage[y][x]->get_pos() + Vec2{ stage[y][x]->get_hit_r().w,stage[y][x]->get_hit_r().h };
						
							//右に反射
							player.set_pos(Vec2{ stage_rect_point.x + stage[y][x]->get_hit_r().w,player.get_pos().y });
							player.set_speed(Vec2{ -e * p_r_speed_delta.x,player.get_speed().y });

						}
	

					}

					///はさまれたかどうか
					Vec2 player_pos_delta = player.get_pos() - player_pre_pos;

					int check_sx_p2 = int(player.get_pos().x / 64) - 6;
					int check_sy_p2 = int(player.get_pos().y / 64) - 6;
					int check_fx_p2 = int(player.get_pos().x / 64) + 7;
					int check_fy_p2 = int(player.get_pos().y / 64) + 7;

					for (int check_x = Max(0, check_sx_p2); check_x < Min(stage_x, check_fx_p2); check_x++)
					{
						for (int check_y = Max(0, check_sy_p2); check_y < Min(stage_y, check_fy_p2); check_y++)
						{
							//当たっている
							if (stage[check_y][check_x]->get_can_hit())
							{

								if (player.get_hit_r().intersects(stage[check_y][check_x]->get_hit_r()))
								{
									
									//縦方向に挟まれる
									if (hit_y)
									{
										double player_line_s = player.get_pos().x;
										double player_line_f = player.get_pos().x + player.get_p_w();


										double block_line_s = Max(stage[y][x]->get_hit_r().x, stage[check_y][check_x]->get_hit_r().x);
										double block_line_f = Min(stage[y][x]->get_hit_r().x + stage[y][x]->get_hit_r().w, stage[check_y][check_x]->get_hit_r().x + stage[check_y][check_x]->get_hit_r().w);

										if (player_line_f < block_line_s && player_line_s > block_line_f)
										{
											//はさまれていない
										}
										else if (block_line_f > block_line_s)
										{
											
											//今当たっているブロックと前に当たったブロックの隙間がplayerの幅未満
											//下に移動していて上に押し返された

											double rate{ 0 };

											double p_dis = player_line_f - player_line_s;

											double b_dis = block_line_f - block_line_s;

											//線分の含有量(一致率)を調べる
											//playerの線分に対してのブロックたちの線分の割合
											//これがないと謎の死を遂げる。（地面に着地しているときに、回転しているブロックに当たると死ぬ）
											if (block_line_s >= player_line_s && block_line_f >= player_line_f)
											{
												double minasu = block_line_f - player_line_f;

												rate = (player_line_f - block_line_s) / p_dis;
											}
											else if (block_line_s >= player_line_s && block_line_f <= player_line_f)
											{
												rate = b_dis / p_dis;
											}
											else if (block_line_s <= player_line_s && block_line_f <= player_line_f)
											{
												double mainasu = player_line_s - block_line_s;
												rate = (block_line_f - player_line_s) / p_dis;
											}
											else if (block_line_s <= player_line_s && block_line_f >= player_line_f)
											{
												rate = 1;
											}

											//今当たっているブロックと前に当たったブロックの隙間がplayerの幅未満
											//下に移動していて上に押し返された

											double check_rate = 0.05;

											/*
											if ((stage[y][x]->get_move_type() == 3 || stage[check_y][check_x]->get_move_type() == 3))
											{
												//回転運動のときはcheck_rateを大きくしないと死ぬ
												check_rate = 0.05;
											}*/

											if (rate >= check_rate)
											{

												if (p_r_speed_delta.y > 0)
												{

													double gap = hit_line.y - (stage[check_y][check_x]->get_hit_r().y + stage[check_y][check_x]->get_hit_r().h);
													if (gap > 0.1 && gap < player.get_hit_r().h)
													{
														player_death();
													}

												}

												//上に移動していて下に押し返された
												if (p_r_speed_delta.y < 0)
												{

													double gap = (stage[check_y][check_x]->get_hit_r().y) - hit_line.y;
													if (gap > 0.1 && gap < player.get_hit_r().h)
													{
														player_death();
													}

												}
											}
											
										}
										else if (block_line_f == block_line_s)
										{
											
											bool can_death = false;
											
											double gap = 0;//隙間
											//下に移動していて上に押し返された
											if (p_r_speed_delta.y > 0)
											{
												gap =  hit_line.y - (stage[check_y][check_x]->get_hit_r().y + stage[check_y][check_x]->get_hit_r().h);
											}
											//上に移動していて下に押し返された
											if (p_r_speed_delta.y < 0)
											{
												double gap = (stage[check_y][check_x]->get_hit_r().y) - hit_line.y;
											}

											//playerの線分が2つのブロックをまたいでいるか

											if (stage[y][x]->get_pos().x < stage[check_y][check_x]->get_pos().x)
											{
												if (player_line_s > stage[y][x]->get_pos().x && player_line_s < stage[y][x]->get_pos().x + stage[y][x]->get_hit_r().w)
												{
													if (gap >=0.1 &&player_line_f > stage[check_y][check_x]->get_pos().x && player_line_f < stage[check_y][check_x]->get_pos().x + stage[check_y][check_x]->get_hit_r().w)
													{
														can_death = true;
													}
												}
											}

											if (stage[check_y][check_x]->get_pos().x < stage[y][x]->get_pos().x)
											{
												if (player_line_s > stage[check_y][check_x]->get_pos().x && player_line_s < stage[check_y][check_x]->get_pos().x + stage[check_y][check_x]->get_hit_r().w)
												{
													if (gap >= 0.1&&player_line_f > stage[y][x]->get_pos().x && player_line_f < stage[y][x]->get_pos().x + stage[y][x]->get_hit_r().w)
													{
														can_death = true;
													}
												}
											}
											

											if (can_death) { player_death(); }
											
										}

									}
									
									
									//横方向に挟まれる
									if (hit_x)
									{

										double player_line_s = player.get_pos().y + 5;
										double player_line_f = player.get_pos().y + player.get_p_h() - 5;


										double block_line_s = Max(stage[y][x]->get_hit_r().y, stage[check_y][check_x]->get_hit_r().y);
										double block_line_f = Min(stage[y][x]->get_hit_r().y + stage[y][x]->get_hit_r().h, stage[check_y][check_x]->get_hit_r().y + stage[check_y][check_x]->get_hit_r().h);



										if (player_line_f < block_line_s || player_line_s > block_line_f)
										{

										}
										else if (block_line_f > block_line_s)
										{
											//右に移動していて左に押し返された
											if (p_r_speed_delta.x > 0)
											{
												double gap = hit_line.x - (stage[check_y][check_x]->get_hit_r().x + stage[check_y][check_x]->get_hit_r().w);
												if (gap > 0.1 && gap < player.get_hit_r().w)
												{
													player_death();
												}
											}

											//左に移動していて右に押し返された
											if (p_r_speed_delta.x < 0)
											{
												double gap = (stage[check_y][check_x]->get_hit_r().x) - hit_line.x;
												if (gap > 0.1 && gap < player.get_hit_r().w)
												{
													player_death();
												}
											}
										}
										else if (block_line_f == block_line_s)
										{
											double gap{ 0 };

											//右に移動していて左に押し返された
											if (p_r_speed_delta.x > 0)
											{
												gap = hit_line.x - (stage[check_y][check_x]->get_hit_r().x + stage[check_y][check_x]->get_hit_r().w);
											}

											//左に移動していて右に押し返された
											if (p_r_speed_delta.x < 0)
											{
												gap = (stage[check_y][check_x]->get_hit_r().x) - hit_line.x;
											}
											bool can_death = false;

											if (gap > 0.1)
											{

												if (stage[y][x]->get_pos().y < stage[check_y][check_x]->get_pos().y)
												{
													if (player_line_s > stage[y][x]->get_pos().y && player_line_s < stage[y][x]->get_pos().y + stage[y][x]->get_hit_r().h)
													{
														if (player_line_f > stage[check_y][check_x]->get_pos().y && player_line_f < stage[check_y][check_x]->get_pos().y + stage[check_y][check_x]->get_hit_r().h)
														{
															can_death = true;
														}
													}
												}

												if (stage[check_y][check_x]->get_pos().y < stage[y][x]->get_pos().y)
												{
													if (player_line_s > stage[check_y][check_x]->get_pos().y && player_line_s < stage[check_y][check_x]->get_pos().y + stage[check_y][check_x]->get_hit_r().h)
													{
														if (player_line_f > stage[y][x]->get_pos().y && player_line_f < stage[y][x]->get_pos().y + stage[y][x]->get_hit_r().h)
														{
															can_death = true;
														}
													}
												}
											}
											if (can_death) { player_death(); }
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
					player_goal();
				}


				//星
				if (stage[y][x]->get_object_kind() == Object_Kind_Star)
				{
					//ゲットしたら見えなくなる
					if (stage[y][x]->get_can_see())
					{
						player.init_get_star();
						stage[y][x]->set_can_see(false);
					}
				}
			}
		}


	}



	if (check_grounded)
	{
		player.set_grounded(true);
		
	}
	else
	{
		player.set_grounded(false);
		player.set_ground_speed(Vec2{ 0,0 });
	}
}

void Play::update_play_intersect_p_e()
{
	for (auto& enemy : enemies)
	{
		if (enemy->get_live())
		{
			if (player.get_hit_r().intersects(enemy->get_hit_r()))
			{
				shake_counter = 6;
				player.damage();
			}
		}
	}
}

void Play::update_play_intersect_pshot_s()
{
	Array<std::shared_ptr<Shot>> p_shots;

	p_shots = player.get_shots();

	//playerが弾を持っているなら行う


	for (auto& p_shot : p_shots)
	{
		
		if (p_shot->get_crash() == false)
		{
			int check_pos_x = int(p_shot->get_pos().x / 64);
			int check_pos_y = int(p_shot->get_pos().y / 64);

			for (int x = Max(0, check_pos_x - 5); x < Min(stage_x, check_pos_x + 5); x++)
			{
				for (int y = Max(0, check_pos_y - 5); y < Min(stage_y, check_pos_y + 5); y++)
				{
					if (stage[y][x]->get_can_hit())
					{
						if (p_shot->get_hit_c().intersects(stage[y][x]->get_hit_r()))
						{
							p_shot->crash_init();
							break;
						}
					}
				}
			}
		}
		
	}
}

void Play::update_play_intersect_pshot_e()
{
	Array<std::shared_ptr<Shot>> p_shots;

	p_shots = player.get_shots();

	//playerが弾を持っているなら行う
	
		
	for (auto& p_shot : p_shots)
	{
		if (p_shot->get_crash() == false)
		{
			for (auto& enemy : enemies)
			{
					
				if (enemy->get_live())
				{
					if (p_shot->get_hit_c().intersects(enemy->get_hit_r()))
					{
						enemy->damage(p_shot->get_power());
						p_shot->crash_init();
					}

				}
			}
		}
	}
	
}

void Play::player_death()
{
	if (player.get_live())
	{
		int respon_x = 0;//生き返るところのx座標
		int respon_y = 0;//生き返るところのx座標

		for (int x = 0; x < stage_x; x++)
		{
			for (int y = 0; y < stage_y; y++)
			{
				if (stage[y][x]->get_object_kind() == Object_Kind_Player_Spon)
				{
					respon_x = x;
					respon_y = y;
					break;
				}
			}
		}

		player.death(respon_x, respon_y);
	}
}
