#pragma once
#include <math.h>
#include "Dxlib.h"
#include "Common.h"
#include "Phase.h"
#include "Camera.h"
#include "Effect.h"

class Player
{
public:
	const float TITLE_POSITION_X = 16550.0f;
	const float TITLE_POSITION_Y = 550.0f;
	const float GRAPH_EXTEND_RATE	= 3.0f;
	const float USUAL_JUMP_POWER	= 20.0f;
	const float JUMP_POWER			= 30.0f;
	const float ACCELERATION_X		= 0.60f;
	const float MAX_SPEED_X			= 7.0f;
	const float BRAKE_RATE			= 0.920f;
	const float SLOWMOTION_RATE		= 100.0f;		//キック準備時のスローモーション倍率
	const float KICK_SPEED_X		= 35.0f;
	const float KICK_SPEED_Y		= 35.0f;
	const float WALL_REBOUND_RATE	= -0.30f;		//壁との反発係数
	const int	REBOUND_TIME		= 500;				//壁と反発した後、何ミリ秒操作を受け付けないか
	const int	DAMAGE_REBOUND_TIME = 200;
	const int	JUMP_COMMAND_OFF_TIME	= 750;
	const int	BURNING_TEXT_OFF_COUNT	= 2000;
	const int	HITPOINT_MAX			= 10;
	const int	HIT_ENEMY_SCORE			= 100;
	const int	KICK_ENEMY_SCORE		= 500;
	const int	BURNING_BONUS			= 100;
	const int	PAD_STICK_DEADZONE		= 15000;		//パッドのスティックの無効範囲
	

	float x, y;
	float w, h;
	float tmpVelocityX, tmpVelocityY;
	float velocityX, velocityY;
	float hitCheckerLR;			//左右の当たり判定をとるy座標
	float hitCheckerUD;			//上下の当たり判定をとるx座標
	float kickVectorX, kickVectorY;		//矢印マークの各方向のベクトル
	float kickDistance;					//キックした距離
	float arrowGraphW, arrowGraphH;
	float kickAngle;
	float burningTextX;
	float burningTextY;
	int score;
	int arrowGraph;							//矢印マークのハンドル
	int graph[3];
	int graphNum4anim;		//アニメーション用、グラフィックの添え字番号を格納
	int kickReadyTime;		//キックフラグ0がtrueになったときに時間を取得
	int kickStartTime;		//キックフラグ1がtrueになったときに時間を取得
	int jumpCmdReleaseTime;
	int reboundStartTime;
	int damageTime;
	int burningStompTime;
	int hitPoint;
	int jumpSound;
	int damageSound;
	int meteoSound;
	bool aliveFlag;
	bool jumpFlag;			//ジャンプ中かどうかを格納
	bool hitGroundFlag;		//床ブロックに当たっているフレームだけtrue
	bool kickFlag[2];		//0でキック準備　1でキック実行 着地時に両方をfalseに
	bool burningFlag;
	bool prevKickCmd;
	bool jumpCmd;
	bool prevJumpCmd;
	bool reboundFlag;		//壁との反発中かどうかを格納
	bool damageFlag;		//エネミーからダメージを受けた影響があるときtrue;
	bool hitEnemyFlag;		//エネミーへの攻撃が成功したときtrueに
	bool reverseDrawFlag;	//裏焼きするか否かのフラグ		true-->左向き　false-->右向き

	void InitForTitle();		//タイトル画面用の初期化関数
	void Init(int replayCnt, Game *game);
	void InitForResult(Effect *effect);
	void Load();
	void UpdateControl(XINPUT_STATE *pad, Effect *effect, Game *game, ProgramControl *programCtrl);
	void KickDirectionControl(XINPUT_STATE *pad);
	void IncreaseKickDistance(Effect *effect);
	void UpdateMoveX();
	void UpdateMoveY();
	void Bound(float GroundY);
	void ReboundAgainstWall(float WallEdgeX, bool checkLeft);
	void OddBound(bool checkLeft, float WallEdgeX, bool damageType);
	void HitCeil(float ceilEdge);
	void Fall(Game *game);
	void Damage(bool leftsideOfEnemy, Effect *effect);
	void DamageFromBoss(float partsX, Effect *effect);
	void Draw(Effect *effect, Camera *camera, Game *game);
	void Draw4Title(Camera *camera);
	void DrawBurningBonus(Game *game, Camera *camera);
	void MemoryVelocityXY();
	void ResetKickDistance();
	void GameOver(Effect *effect);
};