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
	const float SLOWMOTION_RATE		= 100.0f;		//�L�b�N�������̃X���[���[�V�����{��
	const float KICK_SPEED_X		= 35.0f;
	const float KICK_SPEED_Y		= 35.0f;
	const float WALL_REBOUND_RATE	= -0.30f;		//�ǂƂ̔����W��
	const int	REBOUND_TIME		= 500;				//�ǂƔ���������A���~���b������󂯕t���Ȃ���
	const int	DAMAGE_REBOUND_TIME = 200;
	const int	JUMP_COMMAND_OFF_TIME	= 750;
	const int	BURNING_TEXT_OFF_COUNT	= 2000;
	const int	HITPOINT_MAX			= 10;
	const int	HIT_ENEMY_SCORE			= 100;
	const int	KICK_ENEMY_SCORE		= 500;
	const int	BURNING_BONUS			= 100;
	const int	PAD_STICK_DEADZONE		= 15000;		//�p�b�h�̃X�e�B�b�N�̖����͈�
	

	float x, y;
	float w, h;
	float tmpVelocityX, tmpVelocityY;
	float velocityX, velocityY;
	float hitCheckerLR;			//���E�̓����蔻����Ƃ�y���W
	float hitCheckerUD;			//�㉺�̓����蔻����Ƃ�x���W
	float kickVectorX, kickVectorY;		//���}�[�N�̊e�����̃x�N�g��
	float kickDistance;					//�L�b�N��������
	float arrowGraphW, arrowGraphH;
	float kickAngle;
	float burningTextX;
	float burningTextY;
	int score;
	int arrowGraph;							//���}�[�N�̃n���h��
	int graph[3];
	int graphNum4anim;		//�A�j���[�V�����p�A�O���t�B�b�N�̓Y�����ԍ����i�[
	int kickReadyTime;		//�L�b�N�t���O0��true�ɂȂ����Ƃ��Ɏ��Ԃ��擾
	int kickStartTime;		//�L�b�N�t���O1��true�ɂȂ����Ƃ��Ɏ��Ԃ��擾
	int jumpCmdReleaseTime;
	int reboundStartTime;
	int damageTime;
	int burningStompTime;
	int hitPoint;
	int jumpSound;
	int damageSound;
	int meteoSound;
	bool aliveFlag;
	bool jumpFlag;			//�W�����v�����ǂ������i�[
	bool hitGroundFlag;		//���u���b�N�ɓ������Ă���t���[������true
	bool kickFlag[2];		//0�ŃL�b�N�����@1�ŃL�b�N���s ���n���ɗ�����false��
	bool burningFlag;
	bool prevKickCmd;
	bool jumpCmd;
	bool prevJumpCmd;
	bool reboundFlag;		//�ǂƂ̔��������ǂ������i�[
	bool damageFlag;		//�G�l�~�[����_���[�W���󂯂��e��������Ƃ�true;
	bool hitEnemyFlag;		//�G�l�~�[�ւ̍U�������������Ƃ�true��
	bool reverseDrawFlag;	//���Ă����邩�ۂ��̃t���O		true-->�������@false-->�E����

	void InitForTitle();		//�^�C�g����ʗp�̏������֐�
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