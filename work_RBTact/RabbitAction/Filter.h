#pragma once
#include "Dxlib.h"
#include "Common.h"
#include "Camera.h"
#include "Sky.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Effect.h"

class Filter
{
public:
	int screen;

	void Init();
	void Draw(Sky *sky, Map *map, Player *player, Enemy **enemy, EnemyControl *enemyCtrl, Enemy **bossEnemy, Boss *boss, Effect *eff, Camera * camera, Game *game);
};
