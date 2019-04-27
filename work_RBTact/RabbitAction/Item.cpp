#include "Item.h"

void Item::Init(Map *map)
{
	//回復アイテム用
	{
		recoverGraph = LoadGraph("image/heart01.png");
		GetGraphSizeF(recoverGraph, &recoverW, &recoverH);
		recoverW *= GRAPH_EXTEND_RATE;
		recoverH *= GRAPH_EXTEND_RATE;

		recoverSound = LoadSoundMem("sound/se/decision22.mp3");
		
		usedRecover = false;

		int i, j;
		char recoverPosition[6] = "";
		char tmp;
		FILE *recoverData;

		fopen_s(&recoverData, "resourceTxt/recoverItem.txt", "r");
		if (recoverData == NULL)
		{
			for (int i = 0; i < RECOVER_ITEM_MASS; i++)
			{
				recoverX[i] = 0.0f;
				recoverY[i] = 0.0f;
			}
			return;
		}

		for (i = 0; i < RECOVER_ITEM_MASS; i++)
		{
			j = 0;
			strncpyDx(recoverPosition, "", 6);
			while ((tmp = fgetc(recoverData)) != ',')
			{
				recoverPosition[j++] = tmp;
			}
			recoverX[i] = (float)atoi(recoverPosition);

			j = 0;
			strncpyDx(recoverPosition, "", 6);
			while ((tmp = fgetc(recoverData)) != '\n')
			{
				recoverPosition[j++] = tmp;
			}
			recoverY[i] = map->MAP_HIGHEST + atoi(recoverPosition);

			recoverExist[i] = true;
		}

		fclose(recoverData);
	}

	//スコアアイテム用
	{
		scoreGraph = LoadGraph("image/crown00.png");
		GetGraphSizeF(scoreGraph, &scoreW, &scoreH);
		scoreW *= GRAPH_EXTEND_RATE;
		scoreH *= GRAPH_EXTEND_RATE;

		scoreSound = LoadSoundMem("sound/se/button82.mp3");

		usedScore = false;

		int i, j;
		char tmp;
		char scorePosition[6] = "";

		FILE *scoreData;
		fopen_s(&scoreData, "resourceTxt/scoreItem.txt", "r");
		if (scoreData == NULL)
		{
			for (i = 0; i < SCORE_ITEM_MASS; i++)
			{
				scoreX[i] = 0.0f;
				scoreY[i] = 0.0f;
				return;
			}
		}

		for (i = 0; i < SCORE_ITEM_MASS; i++)
		{
			j = 0;
			strncpyDx(scorePosition, "", 6);
			while ((tmp = fgetc(scoreData)) != ',')
			{
				scorePosition[j++] = tmp;
			}
			scoreX[i] = (float)atoi(scorePosition);

			j = 0;
			strncpyDx(scorePosition, "", 6);
			while ((tmp = fgetc(scoreData)) != '\n')
			{
				scorePosition[j++] = tmp;
			}
			scoreY[i] = map->MAP_HIGHEST + atoi(scorePosition);

			scoreExist[i] = true;
		}
		
		fclose(scoreData);
	}
}

void Item::Update(Game *game)
{
	//usedフラグの更新
	if ((usedRecover || usedScore) && (game->nowTime - usedTime) > USED_STRING_DRAW_COUNT)
	{
		usedRecover = false;
		usedScore = false;
	}
}

void Item::Use(Player *player, bool recover, int id)
{
	if (recover)
	{
		//一定の値が回復するver
		/*
		player->hitPoint += RECOVER_INCREASE_MASS;
		if (player->hitPoint > player->HITPOINT_MAX)
		{
			player->hitPoint = player->HITPOINT_MAX;
		}
		*/
		//全回復ver
		player->hitPoint = player->HITPOINT_MAX;

		usedStringX = recoverX[id] + USED_STRING_OFFSET_X;
		usedStringY = recoverY[id] + USED_STRING_OFFSET_Y;
		PlaySoundMem(recoverSound, DX_PLAYTYPE_BACK);
		usedRecover = true;
		recoverExist[id] = false;
	}
	else
	{
		player->score += SCORE_INCREASE_MASS;
		usedStringX = scoreX[id] + USED_STRING_OFFSET_X;
		usedStringY = scoreY[id] + USED_STRING_OFFSET_Y;
		PlaySoundMem(scoreSound, DX_PLAYTYPE_BACK);
		usedScore = true;
		scoreExist[id] = false;
	}

	usedTime = GetNowCount();
}

void Item::Draw(Camera * camera)
{
	for (int i = 0; i < RECOVER_ITEM_MASS; i++)
	{
		//回復アイテム描画
		if (recoverExist[i])
		{
			DrawExtendGraphF(recoverX[i] - camera->cameraOffsetX, recoverY[i] - camera->cameraOffsetY, recoverX[i] + recoverW - camera->cameraOffsetX, recoverY[i] + recoverH - camera->cameraOffsetY,
				recoverGraph, TRUE);
		}

		//スコアアイテム描画
		if (scoreExist[i])
		{
			DrawExtendGraphF(scoreX[i] - camera->cameraOffsetX, scoreY[i] - camera->cameraOffsetY, scoreX[i] + scoreW - camera->cameraOffsetX, scoreY[i] + scoreH - camera->cameraOffsetY, 
				scoreGraph, TRUE);
		}
	}
}
