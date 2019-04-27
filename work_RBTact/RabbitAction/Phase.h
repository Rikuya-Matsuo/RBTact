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
	//�Q�[���̐i�s�i�K���L�^����t���O
	//�񎟌��ڂ̓Y�����F0--->Init�@1--->Update�@���s��
	bool state[INVALID_GameState][PREPARE_SEQUENCE_NUMBER];

};

class Phase
{
public:

	//�Q�[���̐i�s��Ԃ����ׂ�false�ɂ���B
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

	int graph;		//�^�C�g�����S�摜
	int bgm;
	int how2goNext;	//Push Start�̕\��
	int sceneChangeSound;
	int alphaRate;
	bool alphaIncrease;

	int demoMovie;
	bool moviePlayFlag;		//�f�����[�r�[���Đ����Ȃ�true

	int blackOutRate;

	int creditBGM;
	int creditSE;

	float x, y;
	float w, h;

	int colorChangeState;
	float rRate;
	float gRate;
	float bRate;

	//�^�C�g����ʂ̏�����
	bool Init(int replayCnt, Player *player, Camera *camera, Map *map, Sky *sky);
	void Update(HitChecker *hitChk, Player *player, Map *map, Game *game);
	void Draw(Player *player, Map *map, Sky *sky, Camera *camera, Result *result);
	void ChangeDemo2Title(Player *player);
	void ReloadDemo();
};

class Rule
{
public:
	//�A�C�e���֘A
	static const int ITEM_KIND = 3;
	static const int ITEM_MASS = 6;
	const int ITEMS_SIZE = 64;
	const float ITEMS_MARGIN = WINDOW_WIDTH / ITEM_MASS;		//�A�C�e�����m�̊Ԋu
	const float ITEMS_Y = (float)WINDOW_HEIGHT / 2.0f - ITEMS_SIZE / 2.0f;
	const float ITEMS_SPEED = 3.0f;
	//���[�r�[�֘A
	const int MOVIE_WIDTH = 512;
	const int MOVIE_HEIGHT = MOVIE_WIDTH * 3 / 4;
	static const int MOVIE_MASS = 5;
	const int MOVIE_Y_UPPER = 100;
	const int MOVIE_Y_DOWNER = 700;
	//�{�^���֘A
	const int BUTTON_SIZE = 100;

	int sceneChangeSound;

	int movieHandle[MOVIE_MASS];
	int movieX[MOVIE_MASS];
	int movieEndPoint[MOVIE_MASS];
	int movieSeekTime[MOVIE_MASS];

	int lStickGraph;		//���邷�Ă����������
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
	const float BOSS_BATTLE_LINE_X = 26300;		//�v���C���[������X�����z������A�{�X�C�x���g����
	const float BOSS_FALL_SPEED = 10.0f;		//�{�X�o�ꉉ�o���̃{�X�̗������x
	const float BLACK_BAND_H = 125;
	const float BAND_SPEED = 3.0f;				//�����т̓�������
	const int BOSSROOM_GATE_BLOCK_HEIGHT = 18;	//�{�X�����O�̎d�؂�u���b�N�̐�
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
	float gateBlockX;		//�{�X�����Ƃ̎d�؂�u���b�N�̈ʒu
	float gateBlockY;		//�{�X�����Ƃ̎d�؂�u���b�N�̈ʒu
	float closeBlockX[5];
	float closeBlockY[5];
	float gateBlockFallSpeed;
	float closeBlockFallSpeed[5];
	float blackBandUpper;		//�C�x���g���̍����т̈ʒu
	float blackBandDowner;
	bool gateBlockFallen;
	bool bossAreaClosed;
	bool bossEventFlag;		//�{�X�C�x���g���ł��邩�̃t���O
	bool bossTimeFlag;		//�{�X�Ƃ̐퓬���ł��邩�̃t���O(�C�x���g����true)
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
	bool playBgm;		//���������Đ����������i�[�Btrue--->�����@false--->�܂����Ă��Ȃ�
	bool newRecord;
	bool alphaIncrease;

    bool Init(int replayCnt);

    void Update(Player *player, Boss *boss, Effect *effect, Camera *camera, Game *game);

	void LoadScore();

	void SaveScore(int score);
};