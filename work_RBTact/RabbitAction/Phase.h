#pragma once
#include "Dxlib.h"
#include "Common.h"
#include "Camera.h"
#include "Boss.h"
#include "Player.h"
#include "Map.h"
#include "HitChecker.h"
#include "Sky.h"

const int PREPARE_SEQUENCE_NUMBER = 2;

enum GameState
{
	TITLE,
	RULE,
	GAME,
	RESULT,
	INVALID_GameState
};

enum ItemKind
{
	HEART,
	CROWN,
	COIN,
	INVALID_ItemKind
};

enum ColorState
{
	R2B,
	B2G,
	G2R,
	INVALID_ColorState
};

class StateFlags
{
public:
	//ゲームの進行段階を記録するフラグ
	//二次元目の添え字：0--->Init　1--->Update　を行う
	bool state[INVALID_GameState][PREPARE_SEQUENCE_NUMBER];

};

class Phase
{
public:

	//ゲームの進行状態をすべてfalseにする。
	void InitGameState(StateFlags *stateFlags);



};

class Title
{
public:
	const float TITLE_GRAPH_EXTEND_RATE = 0.8f;
	const float COLOR_RATE_INCREASE = 0.05f;
	const int	MOVIE_START_COUNT = 30000;
	const int	BLACK_OUT_INCREASE = 1;
	const int	COLOR_PRAM_MAX = 255;

	int titleStartTime;
	int nowTime;

	int graph;		//タイトルロゴ画像
	int bgm;
	int how2goNext;	//Push Startの表示
	int sceneChangeSound;
	int alphaRate;
	bool alphaIncrease;

	int demoMovie;
	bool moviePlayFlag;		//デモムービーが再生中ならtrue

	int blackOutRate;

	int creditBGM;
	int creditSE;

	float x, y;
	float w, h;

	int colorChangeState;
	float rRate;
	float gRate;
	float bRate;

	//タイトル画面の初期化
	bool Init(int replayCnt, Player *player, Camera *camera, Map *map, Sky *sky);
	void Update(HitChecker *hitChk, Player *player, Map *map, Game *game);
	void Draw(Player *player, Map *map, Sky *sky, Camera *camera, Result *result);
	void ChangeDemo2Title(Player *player);
	void ReloadDemo();
};

class Rule
{
public:
	//アイテム関連
	static const int ITEM_KIND = 3;
	static const int ITEM_MASS = 6;
	const int ITEMS_SIZE = 64;
	const float ITEMS_MARGIN = WINDOW_WIDTH / ITEM_MASS;		//アイテム同士の間隔
	const float ITEMS_Y = (float)WINDOW_HEIGHT / 2.0f - ITEMS_SIZE / 2.0f;
	const float ITEMS_SPEED = 3.0f;
	//ムービー関連
	const int MOVIE_WIDTH = 512;
	const int MOVIE_HEIGHT = MOVIE_WIDTH * 3 / 4;
	static const int MOVIE_MASS = 5;
	const int MOVIE_Y_UPPER = 100;
	const int MOVIE_Y_DOWNER = 700;
	//ボタン関連
	const int BUTTON_SIZE = 100;

	int sceneChangeSound;

	int movieHandle[MOVIE_MASS];
	int movieX[MOVIE_MASS];
	int movieEndPoint[MOVIE_MASS];
	int movieSeekTime[MOVIE_MASS];

	int lStickGraph;		//えるすてぃっくぐらふ
	int arrowGraph;
	int aButtonGraph;
	int rTriggerGraph;

	int itemGraphes[ITEM_KIND];
	float itemX[INVALID_ItemKind][ITEM_MASS / ITEM_KIND];

	bool Init(int replayCnt);

	void Update(Sky *sky);

	void UpdateItems();
	void DrawItems();

	void DrawMovieAndIcon();
};

class Game
{
public:
	const float GRAVITY = 1.0f;
	const float BOSS_BATTLE_LINE_X = 26300;		//プレイヤーがこのX軸を越えたら、ボスイベント発生
	const float BOSS_FALL_SPEED = 10.0f;		//ボス登場演出時のボスの落下速度
	const float BLACK_BAND_H = 125;
	const float BAND_SPEED = 3.0f;				//黒い帯の動く速さ
	const int BOSSROOM_GATE_BLOCK_HEIGHT = 18;	//ボス部屋前の仕切りブロックの数
	const int BOSSAREA_CLOSE_BLOCK_HEIGHT = 15;
	const int ISOLATE_COUNT = 2000;
	const int BOSS_APPEAR_COUNT = 3000;
	const int GAMEOVER_PERFORMANCE_COUNT = 3000;

	int normalBgm;
	int bossBgm;
	int playerDownTime;
	int gameStartTime;
	int nowTime;
	int bossRoomEnterTime;
	int blackBandGraph;
	float gateBlockX;		//ボス部屋との仕切りブロックの位置
	float gateBlockY;		//ボス部屋との仕切りブロックの位置
	float closeBlockX[5];
	float closeBlockY[5];
	float gateBlockFallSpeed;
	float closeBlockFallSpeed[5];
	float blackBandUpper;		//イベント中の黒い帯の位置
	float blackBandDowner;
	bool gateBlockFallen;
	bool bossAreaClosed;
	bool bossEventFlag;		//ボスイベント中であるかのフラグ
	bool bossTimeFlag;		//ボスとの戦闘中であるかのフラグ(イベント時もtrue)
	bool bossAppear;
	bool gameOver;
	bool gameClear;
	bool stopUpdate;


	bool Init(int replayCnt);

	void Update(Camera *camera);

	void InitBossEvent(Map *map, Camera *camera);
	void UpdateBossEvent(Player *player, Boss *boss, Map *map, Camera *camera);
	void UpdateBossEventEnd();
	void InitCloseBossArea(Map *map);
	void UpdateCloseBossArea(Map *map);
	void DrawGateBlock(Map *map, Camera *camera);
	void DrawCloseBlock(Map *map, Camera *camera);
	void DrawEventBand();

	void SetResult(Result *result);
};

class Result
{
public:
	static const int STRING_MAX_LENGTH = 256;
	static const int RANKING_MASS = 5;
	static const int RANKING_STR_MAX = 10;
	const int ALPHA_INCREASE_MASS = 8;
	const char * RANKING_FILE_NAME = "resourceTxt/ranking.txt";

    char resultStr[STRING_MAX_LENGTH];
	int ranking[RANKING_MASS];
	int rankInNum;
	int BGgraph;
	int shortBgm;
	int alphaRate;
	bool playBgm;		//ｂｇｍを再生したかを格納。true--->した　false--->まだしていない
	bool newRecord;
	bool alphaIncrease;

    bool Init(int replayCnt);

    void Update(Player *player, Boss *boss, Effect *effect, Camera *camera, Game *game);

	void LoadScore();

	void SaveScore(int score);
};