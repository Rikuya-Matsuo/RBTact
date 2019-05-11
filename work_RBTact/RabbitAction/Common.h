//　リファクタリング済み
#pragma once
#include <math.h>
#include "Dxlib.h"

//絶対値の取得
#define ABUSOLUTE_VALUE(number) (number > 0) ? number : -number
//ベクトルの長さの取得
#define GET_VECTOR_LENGTH(x, y) pow((x * x) + (y * y), 0.5)
//ベクトルの内積の取得
#define INNER_PRODUCT(vx1, vy1, vx2, vy2) (vx1 * vx2 + vy1 * vy2)

const int WINDOW_WIDTH = 1920;		//画面の横幅
const int WINDOW_HEIGHT = 1080;		//画面の縦幅
const int BGM_VOLUME = 175;			//BGMの音量
const float DRAW_MARGIN = 150.0f;	//描画の余裕。画面外でもこの範囲なら描画する。

class ProgramControl
{
public:
	int replayCnt = 0;			//ゲーム起動時から何回リプレイされたか。これによってロード回数を減らす。
	bool prevEnter = false;		//前のフレームでエンターキーが押されたかのフラグ

	bool CheckHitGoNext(XINPUT_STATE *pad);				//次の画面に移行する操作がされたかのチェック
	bool CheckHitEsc();									//エスケープが押されたかのチェック
	bool CheckHitDebugCommand(XINPUT_STATE *pad);		//デバッグコマンドが押されたかのチェック
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
class CoinGroup;
class CoinGroupManager;
class HitChecker;
