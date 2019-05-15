#pragma once
#include <math.h>
#include "DxLib.h"
#include "Common.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"

class Effect
{
public:
	//////////////////////////////////////
	//�@�Œ�l
	//////////////////////////////////////
	//�L�b�N�֘A�Œ�l
	const double DEFAULT_KICK_GRAPH_EXTEND_RATE = 1.0;		//�G�t�F�N�g�̕\���{���̏����l
	const double INCRESE_EXTEND_MASS			= 1.0;		//�G�t�F�N�g�̕\���{���̕ω���
	const int MAX_KICK_EFFECT_EXTEND_RATE		= 40;		//�G�t�F�N�g�̕\���{���̌��E�l
	//�a���L�b�N�i�v���C���[������Z���L�b�N�j�֘A�Œ�l
	static const int METEO_EFFECT_ANIMATION_MASS = 9;			//�a���L�b�N�̃A�j���[�V�����̃R�}��
	static const int HIT_METEO_EFFECT_ANIMATION_MASS = 10;		//�a���L�b�N�q�b�g���̃G�t�F�N�g�̃A�j���[�V�����̐�
	const float METEO_EFFECT_START_DISTANCE = 250.0f;			//�G�t�F�N�g�̕`������鋗��
	const float HIT_METEO_EFFECT_OFFSET = 350.0f;				//�����G�t�F�N�g���ǂ̂��炢�傫�����邩
	const double METEO_EFFECT_EXTEND_RATE = 0.5;				//�a���G�t�F�N�g�̉摜�̊g�嗦
	//�G�l�~�[��_���[�W�֘A�Œ�l
	static const int STAR_MAX_MASS = 10;								//��x�̔�_���[�W�Ŏg�����̍ő吔
	const int STAR_NORMAL_MASS = 3;										//�ʏ�̓��ݕt�����Ɏg�����̐�
	const int STAR_MAX_VELOCITY = 30;									//���̑��x�̃����_�������̐U�ꕝ
	const int STAR_MAX_VELOCITY_OFFSET = STAR_MAX_VELOCITY / 2;			//���̑��x�́i-STAR_MAX_VELOCITY_OFFSET �` STAR_MAX_VELOCITY - STAR_MAX_VELOCITY_OFFSET�j�̊ԂŃ����_���ɐݒ肳���
	const float STAR_GRAVITY = 1.5f;									//���̃G�t�F�N�g�ɂ�����d��
	//�G�l�~�[���j�֘A�Œ�l
	static const int SMOKE_MAX_MASS = 2;								//���G�t�F�N�g�̍ő�\����
	//�R�C���l���֘A�Œ�l
	static const int COIN_EFFECT_ANIMATION_MASS = 10;					//�R�C���l���G�t�F�N�g�̃A�j���[�V�����̃R�}��
	const float COIN_EFFECT_EXTEND_RATE = 2.75f;						//�R�C���l���G�t�F�N�g�̉摜�g�嗦

	//////////////////////////////////////
	//�@�ϐ�
	//////////////////////////////////////
	//�v���C���[�̃L�b�N�����Ɏg������
	int kickPrepareEffect;				//�L�b�N�������̃G�t�F�N�g�̃n���h��
	double kickPrepareEffectExtend;		//�L�b�N�������̃G�t�F�N�g�̉摜�g�嗦

	//�L�b�N�����̃X�s�[�h�Ɏ��������Ɏg�����́i�a���L�b�N�ƌď̂���j
	int meteoEffect[METEO_EFFECT_ANIMATION_MASS];	//�a���L�b�N�̃G�t�F�N�g�A�j���[�V�����̃n���h��
	int meteoAnimNum;								//���̃t���[���ŕ\������a���L�b�N�̃A�j���[�V�����ԍ�

	//�a���L�b�N�q�b�g���Ɏg������
	int hitMeteoEffect[HIT_METEO_EFFECT_ANIMATION_MASS];	//�a���L�b�N�������������̃G�t�F�N�g�̃n���h��
	int hitMeteoAnimNum;									//���̃t���[���ŕ\������a���L�b�N�q�b�g���̃A�j���[�V�����ԍ�
	int drawCnt;											//�����R�}��`�悵���񐔁B�i���R�}���\��������A���̃R�}�ɕς���j
	float hitMeteoEffectPosX;								//�\��������W(x)
	float hitMeteoEffectPosY;								//�\��������W(y)
	bool drawHitMeteoEffectFlag;							//�\�����邩�ۂ��̃t���O

	//�G�l�~�[�̔�_���[�W���A�v���C���[HP0���Ɏg������
	int starEffect;
	float starX[STAR_MAX_MASS];
	float starY[STAR_MAX_MASS];
	float starVelX[STAR_MAX_MASS];
	float starVelY[STAR_MAX_MASS];
	bool criticalEffectFlag;

	//�v���C���[HP0���Ɏg������
	int blackSqrGraph;
	int gameOverTime;
	int blackOutRate;
	bool gameOverSetFlag;

	//�G�l�~�[��HP���O�ɂȂ����Ƃ��Ɏg������
	int smokeEffect[10];
	int smokeAnimNum[SMOKE_MAX_MASS];	//�G�t�F�N�g�̃A�j���[�V�����ԍ�
	int smokeTimer[SMOKE_MAX_MASS];		//�G�t�F�N�g�`��Ɏg���^�C�}�[
	int smokeNumber;	//�G�t�F�N�g�̔ԍ��BSMOKE_MAX_MASS�������`����\�ɂ��邽�߁B
	float smokeX[SMOKE_MAX_MASS];
	float smokeY[SMOKE_MAX_MASS];
	bool smokeFlag[SMOKE_MAX_MASS];

	//�v���C���[�̔�_���[�W���Ɏg������
	int damageEffect;
	float damageEffectX;
	float damageEffectY;

	//�R�C���l�����Ɏg������
	int coinEffect[COIN_EFFECT_ANIMATION_MASS];
	int coinEffectAnimNum;
	bool coinEffectTimeFlag;		//true���ɃG�t�F�N�g�Đ�

	//�Q�[���N���A���Ɏg������
	int whiteSqrGraph;
	int gameClearTime;
	int whiteOutRate;
	bool gameClearSetFlag;

	//////////////////////////////////////
	//�@�֐�
	//////////////////////////////////////
	//�L�b�N�֘A�֐��Q
	void InitKickEffect(int replayCnt);
	void DrawKickEffect(Player *player, Camera *camera);

	//�a���L�b�N�֘A�֐��R
	void InitMeteoEffect(int replayCnt);
	void DrawMeteoEffect(Player *player, Camera *camera);
	void SetHitMeteoEffect(Enemy *enemy, Player *player);
	void DrawHitMeteoEffect(Enemy *enemy, Camera *camera);

	//�G�l�~�[��_���[�W�֘A�֐��Q
	void InitStarEffect(int replayCnt);
	void SetRandomStarVelocities();
	void SetStarXY4Enemy(Enemy *enemy, Player *player);
	void UpdateStar();
	void DrawStarEffect(Camera *camera);

	//�G�l�~�[���j�֘A�֐��Q
	void InitSmokeEffect(int replayCnt);
	void SetSmokeEffect(Enemy *enemy);
	void UpdateSmoke(EnemyControl *enemyCtrl);
	void DrawSmokeEffect(Camera *camera);

	//�v���C���[��_���[�W�֘A�֐��Q
	void InitDamageEffect(int replayCnt);
	void SetDamageEffect(Player *player);
	void DrawDamageEffect(Player *player, Camera *camera);

	//�v���C���[HP0�֘A�֐��Q
	void InitPlayerDown();
	void SetPlayerDown(Player *player);
	void UpdatePlayerDown(StateFlags *state, Game *game, Camera *camera);

	//�R�C���l���֘A�֐��Q
	void InitCoinEffect(int replayCnt);
	void UpdateCoinEffect();
	void DrawCoinEffect(Player *player, Camera *camera);

	//�Q�[���N���A�֘A�֐��Q
	void InitGameClear();
	void SetGameClear();
	void UpdateGameClear(StateFlags *state, Game *game);
};