#include"Enemy.h"
#include "Enemy0.h"
#include<memory>

Enemy0::Enemy0(int x, int y)
{
	spon_x = x;
	spon_y = y;



	pos = Vec2{ spon_x,spon_y }*64 + Vec2{ (64 - e_w) / 2,(64 - e_h) / 2 };//texture(四角形)の左上の座標
									
	pre_pos = pos;//1フレーム前のpos

	live = true;

	e_w = 32;
	e_h = 48;

	reset();
}

void Enemy0::update(double scene_delta, Vec2 p_pos)
{
	scene_del = scene_delta;
	player_pos = p_pos;

	for (auto& e : effects)
	{
		//effect
		e.update(scene_del);
	}
	for (auto& shot : shots)
	{
		//弾
		shot->update(scene_del);
	}

	switch (live)
	{
	case true:
		update_live();
		break;
	case false:
		update_die();
		break;
	}

	effects.remove_if([&](Game_Effect effect) {return effect.get_exit() == false; });

	shots.remove_if([&](std::shared_ptr<Shot> shot) {
		return shot->get_exit() == false;
		});
}

void Enemy0::update_live()
{
	move();
	hit_r = RectF{ pos,e_w,e_h };

	if (pos.x > player_pos.x)
	{
		//左向き
		dire = -1;
	}
	else
	{
		//右向き
		dire = 1;
	}

	//playerが近づいたら弾を出す
	if (sqrt((pos.x - player_pos.x) * (pos.x - player_pos.x) + (pos.y - player_pos.y) * (pos.y - player_pos.y)) <= 600)
	{
		shot_timer += scene_del;
		if (shot_timer >= shot_span)
		{
			shot_timer = 0;

			shots << std::make_shared<Shot0>(pos + Vec2{ e_w / 2,e_h / 2 }, dire, Shot_Master::enemy);

		}
	}
}

void Enemy0::move()
{
	//重力
	if (speed.y <= 200)
	{
		speed.y += gravity * scene_del;
	}

	pre_pos = pos;//1フレーム前のposの更新
	pos += speed * scene_del;//posの更新

	if (grounded)
	{
		pos += ground_speed * scene_del;
		speed.x *= 0.7;
	}
}

void Enemy0::draw(double c_x, double c_y)
{
	Vec2 tex_pos = pos - Vec2{ c_x - 640,c_y - 480 };

	if (live)
	{
		//RectF(tex_pos, e_w, e_h).draw(Palette::White);

		if (dire == -1)
		{
			//左
			TextureAsset(U"Enemy0_l").draw(tex_pos);
		}
		else
		{
			TextureAsset(U"Enemy0_r").draw(tex_pos);
		}
	}

	for (auto& e : effects)
	{
		e.draw(c_x, c_y);
	}

	for (auto& shot : shots)
	{
		shot->draw(c_x, c_y);
	}


}

void Enemy0::reset()
{

	live = true;//生きているかどうか
	exit = true;

	shots.clear();
	effects.clear();

	pre_pos = pos;//1フレーム前のpos




	HP = 1;

	shot_span = 1.7;//弾が出るスパン

	shot_timer = 0;//shot_timer>=shot_spanのときに弾を出せる

	gravity = 490;//重力加速度

	shot_timer = 0;//shot_timer>=shot_spanのときに弾を出せる


	pos = Vec2{ spon_x,spon_y }*64 + Vec2{ (64 - e_w) / 2,(64 - e_h) / 2 };//texture(四角形)の左上の座標

	Start_HP = HP;
	speed = { 0,0 };//enemyの速度
	hit_r = RectF{ pos,e_w,e_h };
}
