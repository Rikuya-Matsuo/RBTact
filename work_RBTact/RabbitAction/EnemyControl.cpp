#include "EnemyControl.h"

void EnemyControl::Init(Enemy **enemy)
{
	//音声関係
	damageSound = LoadSoundMem("sound/se/strike1.mp3");
	downSound = LoadSoundMem("sound/se/shrink1.mp3");
	ChangeVolumeSoundMem(175, damageSound);
	ChangeVolumeSoundMem(175, downSound);

	//エネミー情報読み込み
	LoadPosition(FILE_NAME, enemy);

	for (int i = 0; i < ENEMY_MASS; i++)
	{
		enemy[i]->id = i;

		enemy[i]->velocityX = enemy[i]->velocityY = 0.0f;

		if (enemy[i]->id <= 2)
		{
			enemy[i]->hitPoint = enemy[i]->id + 1;
		}
		else
		{
			enemy[i]->hitPoint = enemy[i]->HITPOINT_MAX;
		}
		enemy[i]->existFlag = true;
		enemy[i]->onGroundFlag = false;

		if (!i)
		{
			LoadDivGraph("image/enemy01.png", enemy[i]->ANIM_GRAPH_NUM, 3, 1, 32, 32, enemy[i]->graph);
		}
		else
		{
			for (int j = 0; j < enemy[i]->ANIM_GRAPH_NUM; j++)
			{
				enemy[i]->graph[j] = enemy[0]->graph[j];
			}
		}
		enemy[i]->animNum = 0;

		//ハイスピードタイプに関する初期化
		if (enemy[i]->movePattern == HIGH_SPEED)
		{
			enemy[i]->dashCnt = 0;
			enemy[i]->dashLength = 0.0f;
			enemy[i]->dashStopTime = GetNowCount();
			enemy[i]->dashActionTime = GetNowCount();
			enemy[i]->directionRight = true;
		}
		//ジャンプタイプに関する初期化
		else if (enemy[i]->movePattern == JUMP)
		{
			enemy[i]->directionRight = false;
		}
		else if (enemy[i]->movePattern == NORMAL)
		{
			enemy[i]->directionRight = false;
		}

		GetGraphSizeF(enemy[i]->graph[0], &enemy[i]->w, &enemy[i]->h);
		enemy[i]->w *= enemy[i]->GRAPH_EXTEND_RATE;
		enemy[i]->h *= enemy[i]->GRAPH_EXTEND_RATE;
	}
}

void EnemyControl::LoadPosition(const char * fileName, Enemy ** enemy)
{
	FILE *r;
	fopen_s(&r, fileName, "r");
	if (r == NULL)
	{
		printfDx("読み込みエラー");
		for (int i = 0; i < ENEMY_MASS; i++)
		{
			enemy[i]->x = 700.0f * (i + 1);
			enemy[i]->y = 0.0f;
			enemy[i]->movePattern = NORMAL;
		}
		return;
	}

	int i = 0;
	char tmpX[7], tmpY[6], tmpType[3];

	for (i = 0; i < ENEMY_MASS; i++)
	{
		fgets(tmpX, 7, r);
		enemy[i]->x = (float)atof(tmpX);
		strncpy_s(tmpX, 6, "", 5);

		fgets(tmpY, 6, r);
		enemy[i]->y = (float)atof(tmpY);
		strncpy_s(tmpY, 5, "", 4);

		fgets(tmpType, 3, r);
		toupper(tmpType[0]);
		if (tmpType[0] == 'N')
		{
			enemy[i]->movePattern = NORMAL;
		}
		else if (tmpType[0] == 'J')
		{
			enemy[i]->movePattern = JUMP;
		}
		else if (tmpType[0] == 'H')
		{
			enemy[i]->movePattern = HIGH_SPEED;
		}
		else
		{
			enemy[i]->movePattern = NORMAL;
		}
	}

	fclose(r);
}
