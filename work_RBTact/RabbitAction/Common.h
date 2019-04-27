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

//bool型配列のすべての要素をtrueかfalseにする
void SetBoolArray(bool *flg, int element, bool TrueOrFalse);

//フラグを切り替える
void SwitchFlag(bool *ToFalse, bool *ToTrue);

//フラグの合否の反転
void ReverseFlag(bool *flg);

//bool型配列がtrueあるいはfalseである数を返す
int CountBoolArray(bool *flg, int element, bool search);

//ラープ関数
float Lerp(float x, float y, float LerpRate);

//コサインの値からシータ（角度）を取得
float Cos2Theta(float cos);

//文字列のすべてを終端文字にする関数
char * SetAllNull(char *str, int length);

//Push Start描画
void DrawHow2GoNext(int handle);

//クラスの前方宣言
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
