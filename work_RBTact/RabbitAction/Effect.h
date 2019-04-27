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
	//�a���L�b�N�֘A�Œ�l
	static const int METEO_EFFECT_ANIMATION_MASS = 9;
	static const int HIT_METEO_EFFECT_ANIMATION_MASS = 10;
	const float METEO_EFFECT_START_DISTANCE = 250.0f;			//�G�t�F�N�g�̕`������鋗��
	const float HIT_METEO_EFFECT_OFFSET = 350.0f;				//�����G�t�F�N�g���ǂ̂��炢�傫�����邩
	const double METEO_EFFECT_EXTEND_RATE = 0.5;
	//�G�l�~�[��_���[�W�֘A�Œ�l
	static const int STAR_MAX_MASS = 10;						//��x�̔�_���[�W�Ŏg�����̍ő吔
	const int STAR_NORMAL_MASS = 3;							//�ʏ�̓��ݕt�����Ɏg�����̐�
	const int STAR_MAX_VELOCITY = 30;
	const int STAR_MAX_VELOCITY_OFFSET = STAR_MAX_VELOCITY / 2;
	const float STAR_GRAVITY = 1.5f;
	//�G�l�~�[���j�֘A�Œ�l
	static const int SMOKE_MAX_MASS = 2;					//���G�t�F�N�g�̍ő�\����
	//�R�C���l���֘A�Œ�l
	static const int COIN_EFFECT_ANIMATION_MASS = 10;
	const float COIN_EFFECT_EXTEND_RATE = 2.75f;

	//////////////////////////////////////
	//�@�ϐ�
	//////////////////////////////////////
	//�v���C���[�̃L�b�N�Ɏg������
	int kickEff;
	double kickEffExtend;

	//�L�b�N�����̃X�s�[�h�Ɏ��������Ɏg�����́i�a���L�b�N�ƌď̂���j
	int meteoEff[METEO_EFFECT_ANIMATION_MASS];
	int meteoAnimNum;

	//�a���L�b�N�q�b�g���Ɏg������
	int hitMeteoEff[HIT_METEO_EFFECT_ANIMATION_MASS];
	int hitMeteoAnimNum;
	int drawCnt;			//�����R�}��`�悵����
	float exploreX;
	float exploreY;
	bool drawExploreFlag;

	//�G�l�~�[�̔�_���[�W���A�v���C���[HP0���Ɏg������
	int starEff;
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
	int smokeEff[10];
	int smokeAnimNum[SMOKE_MAX_MASS];	//�G�t�F�N�g�̃A�j���[�V�����ԍ�
	int smokeTimer[SMOKE_MAX_MASS];		//�G�t�F�N�g�`��Ɏg���^�C�}�[
	int smokeNumber;	//�G�t�F�N�g�̔ԍ��BSMOKE_MAX_MASS�������`����\�ɂ��邽�߁B
	float smokeX[SMOKE_MAX_MASS];
	float smokeY[SMOKE_MAX_MASS];
	bool smokeFlag[SMOKE_MAX_MASS];

	//�v���C���[�̔�_���[�W���Ɏg������
	int damageEff;
	float damageEffX;
	float damageEffY;

	//�R�C���l�����Ɏg������
	int coinEff[COIN_EFFECT_ANIMATION_MASS];
	int coinEffAnimNum;
	bool coinEffTimeFlag;		//true���ɃG�t�F�N�g�Đ�

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