//　リファクタリング済み
#pragma once
#include "Dxlib.h"
#include "CoinGroup.h"
#include "Player.h"
#include "Effect.h"

class Coin
{
public:
	const int ADDED_SCORE = 10;		//取得されたときに追加されるスコアの大きさ

	float x;			//座標
	float y;			//座標
	bool exist;			//フィールド上に存在しているか。つまりは取得されていないか
	bool inScreen;		//画面上に存在しているか。要するに描画するかどうか

	void Update(Camera *camera);												//更新。
	void Obtained(Player *player, Effect *effect, CoinGroup *coinCtrl);		//入手される
	bool CheckInSideScreen(Camera *camera);										//画面内に存在しているかのチェック
};
