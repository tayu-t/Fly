#include "stdafx.h"
#include "Shot0.h"

Shot0::Shot0(Vec2 a_pos, int s_dire, Shot_Master shot_master)
{
	pos = a_pos;

	scale = 14.4;

	power = 1;

	master = shot_master;

	dire = s_dire;

	unit_vec = { dire,0 };

	acc = 500;
	speed = 200;

	hit_c = Circle{ pos,scale };
}

Shot0::Shot0(Vec2 a_pos, Vec2 s_unit_vec, Shot_Master shot_master)
{
	pos = a_pos;
	master = shot_master;
	unit_vec = s_unit_vec;

	power = 1;

	scale = 14.4;

	acc = 300;
	speed = 200;

	hit_c = Circle{ pos,scale };
}

void Shot0::update(double scene_deltatime)
{
	scene_del = scene_deltatime;

	exit_timer -= scene_del;

	if (crash == false)
	{
		move();
	}
	else
	{
		update_crash();
	}


	hit_c = Circle(pos, scale);
}

void Shot0::move()
{
	//speed += acc * scene_del;
	speed = 400;
	pos += unit_vec * speed * scene_del;
}

void Shot0::draw(double c_x, double c_y)
{
	Vec2 tex_pos = pos - Vec2{ c_x - 640,c_y - 480 };

	//画面外から出たらfalse
	if (pos.x - (c_x - 640) < -300 || pos.x - (c_x - 640) >= 1500) { exit = false; };

	if (crash)
	{
		draw_crash(c_x, c_y);
	}
	else
	{
		if (master == Shot_Master::player)
		{
			//Circle{ tex_pos,scale }.draw(Palette::Yellow);
			TextureAsset(U"Player_Shot").scaled(0.45).drawAt(tex_pos);
		}
		else if (master == Shot_Master::enemy)
		{
			Circle{ tex_pos,scale}.draw(Palette::Purple);
			TextureAsset(U"Enemy_Shot").scaled(0.45).drawAt(tex_pos);
		}
	}
}

