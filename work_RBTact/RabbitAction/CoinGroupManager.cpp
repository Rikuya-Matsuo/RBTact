#include "CoinGroupManager.h"

void CoinGroupManager::Init()
{
	coinGroup = new CoinGroup[COIN_AREA_MASS];

	FILE *r;
	fopen_s(&r, FILE_NAME, "r");

	for (int i = 0; i < COIN_AREA_MASS; i++)
	{
		coinGroup[i].Init(r, i);
	}

	fclose(r);
}

void CoinGroupManager::Update(Camera *camera)
{
	for (int i = 0; i < COIN_AREA_MASS; i++)
	{
		coinGroup[i].Update(camera);
	}
}

void CoinGroupManager::Draw(Camera *camera)
{
	for (int i = 0; i < COIN_AREA_MASS; i++)
	{
		coinGroup[i].Draw(camera);
	}
}

void CoinGroupManager::DeletePointers()
{
	if (coinGroup != NULL)
	{
		for (int i = 0; i < COIN_AREA_MASS; i++)
		{
			for (int j = 0; j < coinGroup[i].coinMassY; j++)
			{
				delete[]coinGroup[i].coinArray[j];
				coinGroup[i].coinArray[j] = NULL;
			}
		}

		delete[]coinGroup;
		coinGroup = NULL;
	}
}
