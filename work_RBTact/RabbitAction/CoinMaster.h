#pragma once
#include "Camera.h"
#include "CoinControl.h"

class CoinMaster
{
public:
	static const int COIN_AREA_MASS = 3;
	const char *FILE_NAME = "resourceTxt/coin.txt";

	CoinControl *coinCtrl;

	void Init();
	void Update(Camera *camera);
	void Draw(Camera *camera);
	void DeletePointers();
};
