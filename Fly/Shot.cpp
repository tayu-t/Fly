#include "stdafx.h"
#include "Shot.h"

Shot::Shot(Vec2 a_pos,int s_dire,Shot_Master shot_master)
{
	pos = a_pos;

	power = 1;

	master = shot_master;

	dire = s_dire;

	unit_vec = { dire,0 };

	acc = 500;
	speed = 200;

	hit_c = Circle{ pos,scale };
}

Shot::Shot(Vec2 a_pos,Vec2 s_unit_vec, Shot_Master shot_master)
{
	pos = a_pos;
	master = shot_master;
	unit_vec = s_unit_vec;

	power = 1;

	acc = 300;
	speed = 200;

	hit_c = Circle{ pos,scale };
}


void Shot::update_crash()
{
	crash_timer += scene_del;
	if (crash_timer >= 0.3)
	{
		exit = false;
	}
}

void Shot::crash_init()
{
	if (exit_timer < 0)
	{
		crash = true;
		crash_timer = 0;
	}
}


void Shot::draw_crash(double c_x, double c_y)
{
	Vec2 tex_pos = pos - Vec2{ c_x - 640,c_y - 480 };
	// イージング
	double e = EaseOutExpo(crash_timer);
	Circle{ tex_pos, (e * 30) }.draw(HSV(1, 1, 1, 0.7));
}
