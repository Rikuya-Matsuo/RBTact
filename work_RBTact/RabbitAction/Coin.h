//�@���t�@�N�^�����O�ς�
#pragma once
#include "Dxlib.h"
#include "CoinGroup.h"
#include "Player.h"
#include "Effect.h"

class Coin
{
public:
	const int ADDED_SCORE = 10;		//�擾���ꂽ�Ƃ��ɒǉ������X�R�A�̑傫��

	float x;			//���W
	float y;			//���W
	bool exist;			//�t�B�[���h��ɑ��݂��Ă��邩�B�܂�͎擾����Ă��Ȃ���
	bool inScreen;		//��ʏ�ɑ��݂��Ă��邩�B�v����ɕ`�悷�邩�ǂ���

	void Update(Camera *camera);												//�X�V�B
	void Obtained(Player *player, Effect *effect, CoinGroup *coinCtrl);		//���肳���
	bool CheckInSideScreen(Camera *camera);										//��ʓ��ɑ��݂��Ă��邩�̃`�F�b�N
};
