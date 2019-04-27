#pragma once
#include "DxLib.h"
#include "Common.h"
#include "Camera.h"
#include "EnemyControl.h"
#include "Boss.h"
#include "Map.h"
#include "Effect.h"

class Enemy
{
public:
	static const int	ANIM_GRAPH_NUM			= 3;
	const float			GRAPH_EXTEND_RATE		= 3.0f;
	const float			JUMP_POWER				= 18.0f;
	const float			DASH_VELOCITY_X			= 25.0f;	//ダッシュタイプの速さ
	const float			DASH_RANGE				= 300.0f;	//ダッシュタイプの移動範囲
	const float			POPUP_VELOCITY_X		= -15.0f;	//ボスのエネミーのポップアップ方向
	const float			POPUP_VELOCITY_Y		= -13.0f;	//ボスのエネミーのポップアップ方向
	const float			BOSSTYPE_JUMP_POWER		= 8.0f;
	const float			BOSSTYPE_VELOCITY_X		= 5.0f;
	const int			DAMAGE_COUNT			= 1000;		//目がバッテンの時間
	const int			CRUSH_COUNT				= 100;		//つぶれる時間
	const int			JUMP_ACTION_COUNT		= 2500;		//着地からジャンプタイプエネミーが行動するまでの時間
	const int			DASH_ACTION_COUNT		= 2500;
	const int			DASH_INTERVAL			= 300;		//ダッシュ中、止まる時間
	const int			DASH_NUMBER_OF_TIMES	= 3;		//一回のアクションでダッシュする回数
	const int			BOSSTYPE_ACTION_COUNT	= 100;
	const int			HITPOINT_MAX			= 3;
	const int			BOSSTYPE_HITPOINT_MAX	= 3;

	float x, y;
	float velocityX, velocityY;
	float w, h;
	float hitCheckerLR, hitCheckerUD;
	float dashLength;						//ダッシュタイプ用
	int id;									//エネミーのID（個体番号）
	int movePattern;
	int graph[ANIM_GRAPH_NUM];
	int animNum;
	int onGroundTime;
	int damageTime;
	int hitPoint;
	int dashActionTime;					//ダッシュタイプエネミーがアクションをするためのカウンターに使用
	int dashStopTime;
	int dashCnt;						//ダッシュタイプエネミーが一回のアクションでダッシュした回数を格納
	bool existFlag;
	bool onGroundFlag;
	bool directionRight;			//右方向を向くならtrue、左ならfalse

	//void Init(int i, int *enemy0graph, int animGraphNum, EnemyControl *enemyCtrl, Map *map);
	void InitBossType(int i, int *enemy0graph, Boss *boss, EnemyControl * enemyCtrl, Map *map);
	void Update(Player *player, EnemyControl *enemyCtrl, Effect *effect, Game *game);
	void UpdateBossType(Enemy **enemy, Boss *boss, Player *player, Effect *effect, Game *game);
	void Jump(Player *player);						//ジャンプタイプの挙動
	void Dash(Player *player, Game *game);				//ハイスピードタイプの挙動
	void MoveBossType(Game *game);		//ボスタイプの挙動
	void HitWall(float wallEdge, bool leftSideWall);
	void Damage(Player *player, Effect *effect, EnemyControl *enemyCtrl);
	void Draw(Effect *effect, Camera *camera, Map *map);
	bool CheckInsideScreen(Camera *camera, Map *map);
};