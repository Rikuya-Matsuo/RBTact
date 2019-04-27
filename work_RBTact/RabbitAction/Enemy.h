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
	const float			DASH_VELOCITY_X			= 25.0f;	//�_�b�V���^�C�v�̑���
	const float			DASH_RANGE				= 300.0f;	//�_�b�V���^�C�v�̈ړ��͈�
	const float			POPUP_VELOCITY_X		= -15.0f;	//�{�X�̃G�l�~�[�̃|�b�v�A�b�v����
	const float			POPUP_VELOCITY_Y		= -13.0f;	//�{�X�̃G�l�~�[�̃|�b�v�A�b�v����
	const float			BOSSTYPE_JUMP_POWER		= 8.0f;
	const float			BOSSTYPE_VELOCITY_X		= 5.0f;
	const int			DAMAGE_COUNT			= 1000;		//�ڂ��o�b�e���̎���
	const int			CRUSH_COUNT				= 100;		//�Ԃ�鎞��
	const int			JUMP_ACTION_COUNT		= 2500;		//���n����W�����v�^�C�v�G�l�~�[���s������܂ł̎���
	const int			DASH_ACTION_COUNT		= 2500;
	const int			DASH_INTERVAL			= 300;		//�_�b�V�����A�~�܂鎞��
	const int			DASH_NUMBER_OF_TIMES	= 3;		//���̃A�N�V�����Ń_�b�V�������
	const int			BOSSTYPE_ACTION_COUNT	= 100;
	const int			HITPOINT_MAX			= 3;
	const int			BOSSTYPE_HITPOINT_MAX	= 3;

	float x, y;
	float velocityX, velocityY;
	float w, h;
	float hitCheckerLR, hitCheckerUD;
	float dashLength;						//�_�b�V���^�C�v�p
	int id;									//�G�l�~�[��ID�i�̔ԍ��j
	int movePattern;
	int graph[ANIM_GRAPH_NUM];
	int animNum;
	int onGroundTime;
	int damageTime;
	int hitPoint;
	int dashActionTime;					//�_�b�V���^�C�v�G�l�~�[���A�N�V���������邽�߂̃J�E���^�[�Ɏg�p
	int dashStopTime;
	int dashCnt;						//�_�b�V���^�C�v�G�l�~�[�����̃A�N�V�����Ń_�b�V�������񐔂��i�[
	bool existFlag;
	bool onGroundFlag;
	bool directionRight;			//�E�����������Ȃ�true�A���Ȃ�false

	//void Init(int i, int *enemy0graph, int animGraphNum, EnemyControl *enemyCtrl, Map *map);
	void InitBossType(int i, int *enemy0graph, Boss *boss, EnemyControl * enemyCtrl, Map *map);
	void Update(Player *player, EnemyControl *enemyCtrl, Effect *effect, Game *game);
	void UpdateBossType(Enemy **enemy, Boss *boss, Player *player, Effect *effect, Game *game);
	void Jump(Player *player);						//�W�����v�^�C�v�̋���
	void Dash(Player *player, Game *game);				//�n�C�X�s�[�h�^�C�v�̋���
	void MoveBossType(Game *game);		//�{�X�^�C�v�̋���
	void HitWall(float wallEdge, bool leftSideWall);
	void Damage(Player *player, Effect *effect, EnemyControl *enemyCtrl);
	void Draw(Effect *effect, Camera *camera, Map *map);
	bool CheckInsideScreen(Camera *camera, Map *map);
};