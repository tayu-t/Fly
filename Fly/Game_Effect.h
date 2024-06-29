#pragma once

enum class Effect_Kind
{
	empty,
	death,
	get_star,
};

class Game_Effect
{
private:
	Vec2 pos{ 0,0 };//エフェクトの位置
	bool now_effect = true;//今エフェクトがあるかどうか
	Effect_Kind kind = Effect_Kind::empty;//エフェクトの種類

	bool be_wait_effect = false;//エフェクトが終了するまでほかのものが止まるかどうか


	bool exit = true;

	double scene_del{ 0 };

	double act_timer = 0;//エフェクトが発生してからの時間

public:

	Game_Effect(Vec2,Effect_Kind);

	void update(double scene_delta_time);
	void update_death_effect();
	void update_get_star_effect();

	void draw(double, double);
	void draw_death_effect(double,double);
	void draw_get_star_effect(double, double);

	bool get_be_wait_effect() { return be_wait_effect; }
	Effect_Kind get_kind() { return kind; }
	bool get_exit() { return exit; }
};

