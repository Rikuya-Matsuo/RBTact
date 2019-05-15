//�@���t�@�N�^�����O�ς�
#pragma once
#include <math.h>
#include "Dxlib.h"

//��Βl�̎擾
#define ABUSOLUTE_VALUE(number) (number > 0) ? number : -number
//�x�N�g���̒����̎擾
#define GET_VECTOR_LENGTH(x, y) pow((x * x) + (y * y), 0.5)
//�x�N�g���̓��ς̎擾
#define INNER_PRODUCT(vx1, vy1, vx2, vy2) (vx1 * vx2 + vy1 * vy2)

const int WINDOW_WIDTH = 1920;		//��ʂ̉���
const int WINDOW_HEIGHT = 1080;		//��ʂ̏c��
const int BGM_VOLUME = 175;			//BGM�̉���
const float DRAW_MARGIN = 150.0f;	//�`��̗]�T�B��ʊO�ł����͈̔͂Ȃ�`�悷��B

class ProgramControl
{
public:
	int replayCnt = 0;			//�Q�[���N�������牽�񃊃v���C���ꂽ���B����ɂ���ă��[�h�񐔂����炷�B
	bool prevEnter = false;		//�O�̃t���[���ŃG���^�[�L�[�������ꂽ���̃t���O

	bool CheckHitGoNext(XINPUT_STATE *pad);				//���̉�ʂɈڍs���鑀�삪���ꂽ���̃`�F�b�N
	bool CheckHitEsc();									//�G�X�P�[�v�������ꂽ���̃`�F�b�N
	bool CheckHitDebugCommand(XINPUT_STATE *pad);		//�f�o�b�O�R�}���h�������ꂽ���̃`�F�b�N
};

//bool�^�z��̂��ׂĂ̗v�f��true��false�ɂ���
void SetBoolArray(bool *flg, int element, bool TrueOrFalse);

//�t���O��؂�ւ���
void SwitchFlag(bool *ToFalse, bool *ToTrue);

//�t���O�̍��ۂ̔��]
void ReverseFlag(bool *flg);

//bool�^�z��true���邢��false�ł��鐔��Ԃ�
int CountBoolArray(bool *flg, int element, bool search);

//���[�v�֐�
float Lerp(float x, float y, float LerpRate);

//�R�T�C���̒l����V�[�^�i�p�x�j���擾
float Cos2Theta(float cos);

//������̂��ׂĂ��I�[�����ɂ���֐�
char * SetAllNull(char *str, int length);

//Push Start�`��
void DrawHow2GoNext(int handle);

//�N���X�̑O���錾
class Player;
class Enemy;
class EnemyControl;
class StateFlags;
class Game;
class Result;
class Camera;
class Effect;
class Map;
class Sky;
class Boss;
class Item;
class Coin;
class CoinGroup;
class CoinGroupManager;
class HitChecker;
