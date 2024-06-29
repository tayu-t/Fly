#pragma once
#include"Game_Effect.h"
#include"Shot0.h"

class Map_Object;
class Play;

class Reflecter
{
private:
	bool now_reflect;//今反射板があるかどうか

	Vec2 pos;//反射板を描画する中心座標
	RectF hit_r;//反射板の当たり判定
	int reflecter_w = 15;//幅
	int reflecter_h = 64;//高さ

	bool can_use = true;//今反射板を使えるか

	double use_timer = 3;//use_timerの間反射板が使える

	double cool_time = 1;//クールタイム

	double cool_timer = 0;//クールタイムを測る
public:

	void init_reflect(Vec2);

	void update(double,Vec2);

	void draw(double, double);

	RectF get_hit_r() { return hit_r; }

	bool get_now_reflect() { return now_reflect; }

	bool get_can_use() { return can_use; }
};


class Player
{
private:
	///staticがついているものは別々の継承間で共有したいデータ

	int tex_index{ 1 };//描画する画像の番号
	double tex_index_span{0.03};
	double tex_index_timer{ 0 };
	bool tex_bool{ true };//上下方向どちらに翼を動かすかどうか


	int spon_x;
	int spon_y;

	double c_x;
	double c_y;

	Vec2 tex_pos;//描画位置


	int HP;
	int HP_max;
	int normal_HP_max;//normal状態でのHP＿max;


	double shot_span = 0.15;//弾が出るスパン
	double shot_timer = 0;//shot_timer>=shot_spanのときに弾を出せる

	double wait_click = 0.3;//ゲームスタートしてからwait_click待ってから動ける

	double scene_del = 0;//前のフレームからの経過時間

	double gravity = 490;//重力加速度

	Vec2 speed{ 0,0 };//actorの速度

	Vec2 pre_pos;//1フレーム前のpos

	Vec2 start_pos;//スタート位置

	Vec2 pos;//texture(四角形)の左上の座標

	bool grounded;
	Vec2 ground_speed;

	Array<Game_Effect> effects;
	Array<std::shared_ptr<Shot>> shots;

	int have_star;//持っている星の数

	int dire;//向いている方向　右なら1,左なら-１

	RectF hit_r;//当たり判定

	bool live;//生きているかどうか

	double muteki_time = 0.7;//ダメージ受けた後この時間だけ無敵
	double muteki_timer = 0;

	double p_w{ 48 };//playerの幅
	double p_h{ 48 };//playerの高さ


	bool can_restart = false;//死んだ後にリスタートできるか

	double now_hp_rate;//今現在のHPの割合

	//反射板

//	Reflecter reflecter;


public:

	void init(int x, int y ,int hp);//初期化

	void update(double);
	void update_live();
	void update_die();

	void move();
	void attack();

	void draw(double, double);
	void draw_player();

	
	void draw_debug();//デバック用の描画

	//void hit_shot(Shot&);
	void hit_shot(std::shared_ptr<Shot>);//敵の弾に当たるかも

	void reset();

	void init_get_star();


	void damage(int d = 1);
	void muteki();

	void death(int, int);

	




	int get_spon_x() { return spon_x; }
	int get_spon_y() { return spon_y; }

	void set_grounded(bool t) { grounded = t; }
	bool get_grounded() { return grounded; }

	Vec2 get_pos() { return pos; }
	void set_pos(Vec2 p)
	{
		pos = p;
		hit_r = RectF{ pos,p_w,p_h };
	}

	void set_pre_pos(Vec2 a_p) { pre_pos = a_p; }
	Vec2 get_pre_pos() { return pre_pos; }


	void set_ground_speed(Vec2 s) { ground_speed = s; }

	RectF get_hit_r() { return hit_r; }
	int get_p_w() { return p_w; }
	int get_p_h() { return p_h; }


	Vec2 get_speed() { return speed; }
	Vec2 get_total_speed() { return speed + ground_speed; }
	void set_speed(Vec2 s) { speed = s; }

	int get_HP() { return HP; }

	bool get_live() { return live; }


	Array<std::shared_ptr<Shot>> get_shots() { return shots; };

	bool get_can_restart() { return can_restart; }

	int get_have_star() { return have_star; }


};

