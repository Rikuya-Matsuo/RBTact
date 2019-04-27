#pragma once
#include <math.h>
#include "Dxlib.h"

#define ABUSOLUTE_VALUE(number) (number > 0) ? number : -number
#define GET_VECTOR_LENGTH(x, y) pow((x * x) + (y * y), 0.5)
#define INNER_PRODUCT(vx1, vy1, vx2, vy2) (vx1 * vx2 + vy1 * vy2)

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
//const int BGM_VOLUME = 0;
const int BGM_VOLUME = 175;
const float DRAW_MARGIN = 150.0f;

class ProgramControl
{
public:
	int replayCnt = 0;
	bool prevEnter = false;

	bool CheckHitGoNext(XINPUT_STATE *pad);
	bool CheckHitEsc();
	bool CheckHitDebugCmd(XINPUT_STATE *pad);
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
class CoinControl;
class CoinMaster;
class HitChecker;
