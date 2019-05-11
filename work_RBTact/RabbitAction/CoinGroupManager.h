//　リファクタリング済み
#pragma once
#include "Camera.h"
#include "CoinGroup.h"

class CoinGroupManager
{
public:
	static const int COIN_AREA_MASS = 3;				//コイン配置エリアの数
	const char *FILE_NAME = "resourceTxt/coin.txt";		//コイン配置状況を書いてあるファイルの名前

	CoinGroup *coinCtrl;

	void Init();
	void Update(Camera *camera);
	void Draw(Camera *camera);
	void DeletePointers();
};
