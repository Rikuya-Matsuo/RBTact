//�@���t�@�N�^�����O�ς�
#pragma once
#include "DxLib.h"
#include "Coin.h"
#include "Camera.h"

class CoinGroup
{
public:
	const float GRAPH_EXTEND_RATE = 1.5f;		//�摜�̊g��\���{��

	Coin **coinArray;		//�R�C���̔z��
	int id;					//�R�C���O���[�v�̌̔ԍ�
	int se;					//���ʉ��n���h��
	int graph;				//�摜�n���h��
	int coinMassX;			//�R�C���̍s
	int coinMassY;			//�R�C���̗�
	float coinW;			//�摜�A�����蔻��̑傫��
	float coinH;			//�摜�A�����蔻��̑傫��
	

	void Load(FILE *r);							//�ǂݍ��݁i�R�C���̔z�u����txt�f�[�^�ŕۊǂ��Ă���j
	void Init(FILE *r, int coinAreaMass);		//�������B
	void Update(Camera *camera);				//�X�V�B
	void Draw(Camera *camera);					//�`��B
};