//　リファクタリング済み
#pragma once
#include "DxLib.h"
#include "Common.h"
#include "Phase.h"
#include "Camera.h"
#include "Map.h"

class Boss
{
public:
	static const int PARTS_MASS_X = 5;
	static const int PARTS_MASS_Y = 5;
	static const int PARTS_MASS_ALL = PARTS_MASS_X * PARTS_MASS_Y;
	static const int EMISSION_STEP = 12;
	const int PARTS_SIZE_W = 32;
	const int PARTS_SIZE_H = 32;
	const int FLOAT_BLOCK_MASS = 3;			//何ブロック分浮遊させるか
	const int ANGER_COUNT = 500;
	const int SPRIT_COUNT = 800;
	const int CLOSE_COUNT = 2000;			//ボス全分裂から何ミリ秒後にボス用の舞台を閉鎖するか
	const int TIME_LIMIT = 180000;
	const int TIME_OUT_COUNT = 3000;
	const int BONUS_SCORE = 10000;
	const float GRAPH_EXTEND_RATE = 3.0f;
	const float POSITION_X = 29300;

	int bossGraph[PARTS_MASS_ALL];
	int angerGraph[PARTS_MASS_ALL];
	int angerTime;									//「攻撃無駄だよ画像」に差し替えた時間を格納
	int appearTime;									//出現した時間を格納
	int splitTime;									//分裂するタイミングを計るために使用する時間の変数
	int allSplitTime;								//すべてのパーツが分離した時間を格納
	int existEnemyPartsMass;						//分裂後のエネミーが画面上に存在する数
	int emissionInTheStep;							//その排出段階において何体排出したか
	int splitTimes;									//分裂した回数
	int emissionMass[EMISSION_STEP];				//目標とするエネミーの排出量
	float x, y;										//座標
	bool partsSplitFlag[PARTS_MASS_ALL];			//その番号のパーツが分裂したかのフラグ
	bool angerFlag;									//怒り画像に差し替えるときtrue
	bool emission;									//分裂すべき時にtrue
	bool allEmission;								//すべてのボス所属エネミーを排出したらtrueに
	bool alive;										//ボスが生きていればtrue

	void Init(Map *map, int replayCnt);					//初期化。
	void Update(Enemy **enemy, Game *game, Map *map);	//更新。
	void SetAngerFlagTrue();							//ボスの怒りフラグを立てる関数。同時にその時間も記録。
	bool CheckEnemyOnBossArea(Enemy **enemy);			//エネミーがボスエリア上にいるかどうかのチェック　true--->いる
	void Draw(Camera *camera);							//描画。
	void TimeOut(Game *game);							//制限時間が過ぎたときの処理
};