#include "Boss.h"

void Boss::Init(Map *map, int replayCnt)
{
	if (!replayCnt)
	{
		LoadDivGraph("image/boss.png", PARTS_MASS_ALL, PARTS_MASS_X, PARTS_MASS_Y, PARTS_SIZE_W, PARTS_SIZE_H, bossGraph);
		LoadDivGraph("image/bossAnger.png", PARTS_MASS_ALL, PARTS_MASS_X, PARTS_MASS_Y, PARTS_SIZE_W, PARTS_SIZE_H, angerGraph);
	}

	alive = true;
	angerFlag = false;
	for (int i = 0; i < PARTS_MASS_ALL; i++)
	{
		partsSplitFlag[i] = false;
	}
	emission = true;
	allEmission = false;

	x = POSITION_X;
	y = map->MAP_HIGHEST;
	existEnemyPartsMass = 0;
	emissionInTheStep = 0;
	splitTimes = 0;
	for (int i = 0; i < EMISSION_STEP; i++)
	{
		int mass;
		if (i == 0)
		{
			mass = 1;
		}
		else if (i == 1)
		{
			mass = 2;
		}
		else if (i == 2)
		{
			mass = 2;
		}
		else if (i == 3)
		{
			mass = 3;
		}
		else if (i == 4)
		{
			mass = 2;
		}
		else if (i == 5)
		{
			mass = 1;
		}
		else if (i == 6)
		{
			mass = 2;
		}
		else if (i == 7)
		{
			mass = 2;
		}
		else if (i == 8)
		{
			mass = 3;
		}
		else if (i == 9)
		{
			mass = 2;
		}
		else if (i == 10)
		{
			mass = 1;
		}
		else if (i == 11)
		{
			mass = 4;
		}
		else
		{
			mass = 0;
		}
		emissionMass[i] = mass;
	}
	splitTime = GetNowCount();
}

void Boss::Update(Enemy **enemy, Game *game, Map *map)
{
	//怒り表示フラグの更新
	if (angerFlag && (game->nowTime - angerTime) > ANGER_COUNT)
	{
		angerFlag = false;
	}

	//排出したエネミーが誰もいなくなったら排出のタイミングである
	if (!existEnemyPartsMass)
	{
		emission = true;
	}

	//時間による条件 && すべて出し切っていない && 排出した数が排出すべき数に達していない && 排出のタイミングである
	if ((game->nowTime - splitTime) > SPRIT_COUNT && !allEmission &&
		existEnemyPartsMass < emissionMass[splitTimes] && emission)
	{
		for (int i = 0; i < PARTS_MASS_ALL; i++)
		{
			//まだ排出されていない個体を走査
			if (!partsSplitFlag[i])
			{
				partsSplitFlag[i] = true;
				enemy[i]->velocityX = enemy[i]->POPUP_VELOCITY_X;
				enemy[i]->velocityY = enemy[i]->POPUP_VELOCITY_Y;

				enemy[i]->x += enemy[i]->velocityX;
				enemy[i]->y += enemy[i]->velocityY;

				existEnemyPartsMass++;
				emissionInTheStep++;
				//排出した量が排出すべき量以上になったとき
				if (emissionInTheStep >= emissionMass[splitTimes])
				{
					emission = false;
					emissionInTheStep = 0;
					splitTimes++;
					if (splitTimes >= EMISSION_STEP)
					{
						allEmission = true;
						allSplitTime = GetNowCount();
					}
				}
				break;
			}
		}
		splitTime = GetNowCount();
	}

	if (allEmission && (game->nowTime - allSplitTime) > CLOSE_COUNT && !CheckEnemyOnBossArea(enemy))
	{
		game->UpdateCloseBossArea(map);
	}

	TimeOut(game);

	//全て排出した後、エネミーが全滅したとき、ボスは撃破された。
	if (!existEnemyPartsMass && allEmission)
	{
		alive = false;
	}
}

void Boss::SetAngerFlagTrue()
{
	angerFlag = true;
	angerTime = GetNowCount();
}

bool Boss::CheckEnemyOnBossArea(Enemy ** enemy)
{
	bool ret = false;

	for (int i = 0; i < PARTS_MASS_ALL; i++)
	{
		//エネミーが生きていて、ボスエリアの奥に居ればretをtrueに
		if (enemy[i]->existFlag && enemy[i]->x + enemy[i]->w > x)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void Boss::Draw(Camera *camera)
{
	for (int ly = 0; ly < PARTS_MASS_Y; ly++)
	{
		for (int lx = 0; lx < PARTS_MASS_X; lx++)
		{
			if (!partsSplitFlag[lx + (ly * PARTS_MASS_X)])
			{
				int graphHandle = (angerFlag) ? angerGraph[lx + (ly * PARTS_MASS_X)] : bossGraph[lx + (ly * PARTS_MASS_X)];

				DrawExtendGraphF(x + lx * PARTS_SIZE_W * GRAPH_EXTEND_RATE - camera->cameraOffsetX, y + ly * PARTS_SIZE_H * GRAPH_EXTEND_RATE - camera->cameraOffsetY,
					x + (lx + 1) * PARTS_SIZE_W * GRAPH_EXTEND_RATE - camera->cameraOffsetX, y + (ly + 1) * PARTS_SIZE_H * GRAPH_EXTEND_RATE - camera->cameraOffsetY,
					graphHandle, TRUE);
			}
		}
	}
}

void Boss::TimeOut(Game * game)
{
	int time = (game->nowTime - appearTime);
	//ボス戦の時間が時間制限を超えたとき
	if (time > TIME_LIMIT)
	{
		//タイムアウト演出時間が過ぎていないとき
		if (time < TIME_LIMIT + TIME_OUT_COUNT)
		{
			game->stopUpdate = true;
		}
		else
		{
			game->gameOver = true;
		}
	}
}
