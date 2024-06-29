#pragma once
#include"Game_Effect.h"
#include"Shot.h"
#include"Shot0.h"
#include<memory>

class Enemy
{
protected:
	int HP{ 4 };
	int Start_HP{ 0 };

	Array<Game_Effect> effects;
	int spon_x{0};
	int spon_y{ 0 };


	int dire = 1;//向いている方向　右なら1,左なら-１

	double shot_span = 0.7;//弾が出るスパン
	double shot_timer = 0;//shot_timer>=shot_spanのときに弾を出せる

	double scene_del = 0;//前のフレームからの経過時間

	double gravity = 490;//重力加速度

	Vec2 speed{ 0,0 };//actorの速度

	Vec2 pre_pos{ 600,200 };//1フレーム前のpos

	Vec2 pos{ 600,200 };//texture(四角形)の左上の座標

	Vec2 player_pos{ 0,0 };

	bool grounded = false;
	Vec2 ground_speed{ 0,0 };

	RectF hit_r = RectF{0,0,0,0};//当たり判定

	bool live = true;//生きているかどうか

	bool exit = true;

	Array<std::shared_ptr<Shot>> shots;

	double e_w{ 32 };//enemyの幅
	double e_h{ 48 };//enemyの高さ
public:
	Enemy() {};
	Enemy(int, int) {};

	virtual ~Enemy() {};

	virtual void update(double, Vec2 player_pos) {};

	virtual void draw(double, double) {};

	virtual void reset() {};

	void update_die();

	void death();

	void damage(int);


	/// get,set系

	int get_spon_x() { return spon_x; }
	int get_spon_y() { return spon_y; }

	Vec2 get_pos() { return pos; }
	void set_pos(Vec2 p)
	{
		pos = p;
		hit_r = RectF{ pos,e_w,e_h };
	}

	void set_pre_pos(Vec2 a_p) { pre_pos = a_p; }
	Vec2 get_pre_pos() { return pre_pos; }

	int get_e_w() { return e_w; }
	int get_e_h() { return e_h; }

	void set_ground_speed(Vec2 s) { ground_speed = s; }

	Vec2 get_speed() { return speed; }
	Vec2 get_total_speed() { return speed + ground_speed; }
	void set_speed(Vec2 s) { speed = s; }

	bool get_live() { return live; }

	void set_grounded(bool t) { grounded = t; }
	bool get_grounded() { return grounded; }

	RectF get_hit_r() { return hit_r; }

	bool get_exit() { return exit; }
	void set_exit(bool t) {exit = t; }


	Array<std::shared_ptr<Shot>> get_shots() { return shots; }


};

