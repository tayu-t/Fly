#pragma once

enum Object_Kind
{
	Object_Kind_Player_Spon = -1,
	Object_Kind_Empty = 0,
	Object_Kind_Scaffold = 1,
	Object_Kind_Spike = 2,
	Object_Kind_Goal = 3,
	Object_Kind_Enemy_Spon = 4,
	Object_Kind_Belt = 5,
	Object_Kind_Star = 6,
};

class Map_Object
{
protected:
	bool change = false;//空がブロックに変化したか

	double init_stop_time = 0.5;//最初の瞬間はブロックが動かない

	double scene_del{ 0 };

	int move_type{ 0 };//ブロックの動き方
	int move_amount{ 0 };

	bool move_switch = false;//ブロックのmove内でいろいろな用途に使う

	int object_w{ 64 };//objectの横幅
	int object_h{ 64 };//objectの縦幅

	Vec2 delta_pos{0,0};//前のフレームとのposの差

	Vec2 fu_speed{ 0,0 };
	Vec2 speed{ 0,0 };//ブロックの速度

	Vec2 m_pos{ 0,0 };//{3,4}のようなstage配列における要素

	//m_pos*Vec2{obect_w,object_h}で求められるTextureを表示する位置
	Vec2 pos{ 0,0 };
	Vec2 s_pos{ 0,0 };//start時のpos
	Vec2 pre_pos{ 0,0 };

	Object_Kind first_object_kind = Object_Kind_Scaffold;//csvファイルに書いてあるやつ
	Object_Kind object_kind = Object_Kind_Scaffold;

	int enemy_kind = 0;//敵のスポーン位置のだった時どの敵をスポーンさせるか

	bool can_hit{true};//当たり判定を持つか

	bool can_see = true;//見えるかどうか

	RectF hit_r;//当たり判定

public:
	virtual ~Map_Object() {};

	virtual void update(double,Vec2) {};
	virtual void draw(double, double) {};
	virtual void draw(double,double,bool) {};

	virtual void reset() {};

	//virtual bool created_block() { return false; };//空がブロックになる

	bool created_block();


	RectF get_hit_r() { return hit_r; }

	int get_move_type() { return move_type; }
	int get_move_amount() { return move_amount; }

	Object_Kind get_first_object_kind() { return first_object_kind; }
	Object_Kind get_object_kind() { return object_kind; }

	int get_enemy_kind() { return enemy_kind; }

	Vec2 get_pos() { return pos; }
	Vec2 get_pre_pos() { return pre_pos; }
	Vec2 get_m_pos() { return m_pos; }
	Vec2 get_delta_pos() { return delta_pos; }
	Vec2 get_speed() { return speed; }
	Vec2 get_fu_speed() { return fu_speed; }
	bool get_can_hit() { return can_hit; }

	bool get_can_see() { return can_see; }
	void set_can_see(bool t) { can_see = t; }
};

class Player_Spon :
	public Map_Object
{
public:
	Player_Spon(int x, int y);
	void draw(double c_x, double c_y, bool now_map_make);
};

class Enemy_Spon :
	public Map_Object
{
private:
public:
	Enemy_Spon(int x, int y,int k);
	void draw(double c_x, double c_y,bool now_map_make);
};

class Goal :
	public Map_Object
{
public:
	Goal(int, int);
	void draw(double c_x, double c_y, bool now_map_make);
};

class Star :
	public Map_Object
{
private:
	double color{0};
	double color_change_span{0.2};
	double color_change_timer{ 0 };
public:
	Star(int, int);
	void update(double scene_del, Vec2);
	void reset();
	void draw(double c_x, double c_y, bool now_map_make);
};

