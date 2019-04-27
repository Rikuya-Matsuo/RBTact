#include "CoinMaster.h"

void CoinMaster::Init()
{
	coinCtrl = new CoinControl[COIN_AREA_MASS];

	FILE *r;
	fopen_s(&r, FILE_NAME, "r");

	for (int i = 0; i < COIN_AREA_MASS; i++)
	{
		coinCtrl[i].Init(r, i);
	}

	fclose(r);
}

void CoinMaster::Update(Camera *camera)
{
	for (int i = 0; i < COIN_AREA_MASS; i++)
	{
		coinCtrl[i].Update(camera);
	}
}

void CoinMaster::Draw(Camera *camera)
{
	for (int i = 0; i < COIN_AREA_MASS; i++)
	{
		coinCtrl[i].Draw(camera);
	}
}

void CoinMaster::DeletePointers()
{
	if (coinCtrl != NULL)
	{
		for (int i = 0; i < COIN_AREA_MASS; i++)
		{
			for (int j = 0; j < coinCtrl[i].coinMassY; j++)
			{
				delete[]coinCtrl[i].coinArray[j];
				coinCtrl[i].coinArray[j] = NULL;
			}
		}

		delete[]coinCtrl;
		coinCtrl = NULL;
	}
}
