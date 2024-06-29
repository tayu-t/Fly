#include "stdafx.h"
#include "Game_Effect.h"

Game_Effect::Game_Effect(Vec2 effect_pos, Effect_Kind effect_kind)
{
	now_effect = true;
	pos = effect_pos;
	kind = effect_kind;

	switch (kind)
	{
	case Effect_Kind::empty:
		break;
	case Effect_Kind::death:
		be_wait_effect = true;
		break;
	case Effect_Kind::get_star:
		pos = Vec2{ effect_pos.x,effect_pos.y };
		be_wait_effect = false;
		break;
	default:
		break;
	}
}

void Game_Effect::update(double scene_delta_time)
{
	scene_del = scene_delta_time;
	switch (kind)
	{
	case Effect_Kind::empty:
		break;
	case Effect_Kind::death:
		update_death_effect();
		break;
	case Effect_Kind::get_star:
		update_get_star_effect();
		break;
	default:
		break;
	}
}

void Game_Effect::draw(double c_x, double c_y)
{
	switch (kind)
	{
	case Effect_Kind::empty:
		break;
	case Effect_Kind::death:
		draw_death_effect(c_x, c_y);
		break;
	case Effect_Kind::get_star:
		draw_get_star_effect(c_x,c_y);
		break;
	default:
		break;
	}
}

void Game_Effect::update_death_effect()
{
	act_timer += scene_del;

	//発生から0.5秒たったら消す
	if (act_timer > 0.7)
	{
		now_effect = false;
		exit = false;
	}
}

void Game_Effect::update_get_star_effect()
{
	act_timer += scene_del;

	Vec2 target_pos = Vec2{25,65 };

	pos += (target_pos - pos) / 16;

	//画面左端にいったら消す
	if (sqrt((target_pos.x - pos.x)* (target_pos.x - pos.x) +(target_pos.y - pos.y) * (target_pos.y - pos.y)) <= 10)
	{
		now_effect = false;
		exit = false;
	}
}

void Game_Effect::draw_death_effect(double c_x, double c_y)
{
	Vec2 tex_pos = pos - Vec2{ c_x-640,c_y -480};
	// イージング
	double e = EaseOutExpo(act_timer);

	Circle{ tex_pos, (e * 100) }.drawFrame((30.0 * (1.0 - e)), HSV(1,1,1,0.7));
}

void Game_Effect::draw_get_star_effect(double c_x, double c_y)
{
	Circle(pos, 20).draw(HSV(60, 1, 1, 0.9));
	Circle(pos, 10).draw(HSV(0,0.1, 1, 0.9));
}
