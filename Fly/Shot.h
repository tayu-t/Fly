#pragma once

enum class Shot_Master
{
	player,
	enemy,
};

class Shot
{
protected:

	Shot_Master master{ Shot_Master::player };//弾の保持者

	double scene_del{ 0 };

	///弾の方向:右なら１,左なら-1
	int dire{ 0 };

	Vec2 unit_vec{0,0};//弾が進む方向の単位ベクトル

	double acc;//加速度

	double speed;//速さ

	bool reflected = false;//反射板で反射されたかどうか

	Vec2 pos{ 0,0 };//弾の中心座標
	int scale{ 12 };//弾の大きさ

	int power{ 2 };//弾のダメージ

	Circle hit_c;

	double exit_timer = 0.2;//この時間は消えない

	double crash_timer{ 0 };//破裂時のtimer

	bool crash{ false };//ぶつかったらはじける

	bool exit{ true };
public:
	Shot() {};
	Shot(Vec2 a_pos, int di,Shot_Master);
	Shot(Vec2 a_pos,Vec2 s_e, Shot_Master);

	virtual ~Shot() {};

	virtual void update(double) {};

	void update_crash();

	virtual void draw(double, double) {};
	void draw_crash(double,double);

	bool get_crash() { return crash; }
	void set_crash(bool t) { crash = t; }
	void crash_init();//破裂の準備

	Circle get_hit_c() { return hit_c; }


	Vec2 get_pos() { return pos; }
	void set_pos(Vec2 v) { pos = v; }

	int get_power() { return power; }
	bool get_exit() { return exit; }


	bool get_reflected() { return reflected; }
	void set_reflected(bool t) { reflected = t; }

	Vec2 get_unit_vec() { return unit_vec; }
	void set_unit_vec(Vec2 s) { unit_vec= s; }
};

