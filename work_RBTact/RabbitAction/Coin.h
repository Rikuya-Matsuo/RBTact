#pragma once
#include "Dxlib.h"
#include "CoinControl.h"
#include "Player.h"
#include "Effect.h"

class Coin
{
public:
	const int ADDED_SCORE = 10;

	float x;
	float y;
	bool exist;
	bool inScreen;

	void Update(Camera *camera);
	void Obtained(Player *player, Effect *effect, CoinControl *coinCtrl);		//“üŽè‚³‚ê‚é
	bool CheckInSideScreen(Camera *camera);
};