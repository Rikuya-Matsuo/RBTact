#include "Coin.h"

void Coin::Update(Camera * camera)
{
	CheckInSideScreen(camera);
}

void Coin::Obtained(Player * player, Effect *effect, CoinGroup *coinCtrl)
{
	player->score += ADDED_SCORE;
	exist = false;
	effect->coinEffTimeFlag = true;
	PlaySoundMem(coinCtrl->se, DX_PLAYTYPE_BACK);
}

bool Coin::CheckInSideScreen(Camera * camera)
{
	if (exist && (x > camera->cameraOffsetX - DRAW_MARGIN && x < camera->cameraOffsetX + WINDOW_WIDTH + DRAW_MARGIN) &&
		(y > camera->cameraOffsetY - DRAW_MARGIN && y < camera->cameraOffsetY + WINDOW_HEIGHT + DRAW_MARGIN))
	{
		inScreen = true;
	}
	else
	{
		inScreen = false;
	}

	return inScreen;
}
