#pragma once
#include "GameScene.h"

#include"Player.h"

#include"Map_Object.h"
#include"Scaffold.h"
#include"Spike.h"
#include"Shot.h"
#include"Belt.h"
#include"Empty.h"


#include"Enemy.h"
#include"Enemy0.h"
#include"Enemy1.h"
#include"Enemy2.h"


#include<memory>


struct Stage_Infomation
{
	//各ステージの情報
	int clear = 0;//クリアしたかどうか0:未 1:クリア
	int get_star_num{ 0 };//取得した星の数
	double time_limit{ 0 };//制限時間
	int player_hp{ 0 };//playerのHP
	int can_play{ 0 };//遊べるかどうか0:遊べない 1:遊べる

};

class Play :
    public GameScene
{
private:
	enum class Play_Scene
	{
		select_level,
		play,
		map_make,
		goal,
	};


	Play_Scene play_scene = Play_Scene::select_level;

	int debug_flag{ 0 };

	double scene_w;//シーンの幅
	double scene_h;//シーンの高さ

	double c_x{0};//カメラのx座標
	double c_y{0};


	double play_timer{ 0 };//play時間


	double wait_timer{ 0 };//待ち時間として使う

	//stageの番号
	int stage_index = 0;
	static constexpr int level_num = 11;

	double remaing_time{ 0 };//残り時間

	Rect select_level_box[level_num];

	Stage_Infomation stage_infos[level_num];


	//bool play_scene_init = false;//stageの初期化したかどうか


	//Actor
	Player player;
	Array<std::shared_ptr<Enemy>> enemies;


	//map_make系
	bool map_make_option_now = false;//今map_makeのオプションをひらいているか
	Object_Kind select_object_kind = Object_Kind_Empty;//置くオブジェクトの種類
	int select_move_type = 0;//動く時の種類
	int select_move_amount = 0;//変化する量
	int select_enemy_kind = 0;//敵の種類

	double map_make_wait_time = 0;//ブロックを選択したと同時にそのブロックをマップ上においてしまうのを防ぐ


	//goal系
	Vec2 goal_logo_pos{ 0,0 };//ゴールのロゴの位置

	bool debug{ true };
public:

	//stage
	 std::shared_ptr<Map_Object> stage[25][60];//アクセスしやすいように
	int stage_x = 60;
	int stage_y = 25;


	Play();
	~Play();

	void init();
	void change_play_scene(Play_Scene next_play_scene);//次のシーンに行く時に呼ぶ
	void update();
	void draw();


	//レベル選択/////////////////////////////
	void init_select_level();

	void update_select_level();

	void draw_select_level();


	/// Play///////////////////////////

	void player_death();
	void enemy_damage(std::shared_ptr<Enemy>);

	void player_goal();

	void init_play();//各ステージの最初に呼ぶ
	void stage_loding();//ステージデータを読み込む

	void update_play();//game操作中に行う
	void update_play_system();//カメラとか
	void update_play_intersect_p();//playerの当たり判定はここで呼ぶ
	void update_play_intersect_p_s();//playerとstageの当たり判定の計算
	void update_play_intersect_pshot_s();//playerの弾とstageの当たり判定の計算
	void update_play_intersect_pshot_e();//playerの弾とenemyの当たり判定の計算
	void update_play_intersect_p_e();//playerとenemyの当たり判定

	void update_play_intersect_e();//enemyの当たり判定はここで呼ぶ
	void update_play_intersect_eshot_s();//enemyの弾とstageの当たり判定の計算
	void update_play_intersect_eshot_p();//enemyの弾とplayerの当たり判定の計算
	void update_play_intersect_eshot_e();//enemyの弾とenemyの当たり判定の計算
	void update_play_intersect_e_s();//enemyとstageの当たり判定


	void draw_play_explain();//stage０のときに書く説明書
	void draw_play();
	void draw_play_debug();

	void reset();//全てを初期値に戻す


	/// map_make///////////////////////////////////
	void init_map_make();
	void update_map_make();//map制作
	void map_make_change();//マップを変化させる
	void save();//変化させたマップを保存
	void draw_map_make();
	void draw_map_make_debug();

	/// Goal ///////////////////////////////

	void init_goal();
	void update_goal();
	void draw_goal();

	
};

