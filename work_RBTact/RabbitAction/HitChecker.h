#pragma once
#include "Common.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "Item.h"
#include "CoinGroupManager.h"
#include "CoinGroup.h"
#include "Coin.h"

class HitChecker
{
public:

	bool CheckHitPlayerEnemy(Player *player, Enemy **enemy, Boss *boss, Effect *effect, Camera *camera, Map *map, EnemyControl *enemyCtrl, int enemyMass);			//エネミーとの接触があればtrueを返す

	void CheckHitPlayerGround(Player *player, Map *map, Game *game);

	void CheckHitPlayerBlock(Player *player, Map *map, Game *game);

	void CheckHitEnemyGround(Enemy *enemy, EnemyControl *enemyCtrl, Map *map);

	void CheckHitEnemyWall(Enemy *enemy, EnemyControl *enemyCtrl, Map *map);

	void CheckHitEnemyBlock(Enemy *enemy, EnemyControl *enemyCtrl, Map *map);		//上の二つをいっぺんにやってくれるや～つ

	void CheckHitEnemyBoss(Enemy *enemy, Boss *boss);

	void CheckHitPlayerBoss(Player *player, Boss *boss);

	void CheckHitPlayerItem(Player *player, Item *item);

	void CheckHitPlayerCoin(Player *player, CoinGroupManager *coinMaster, Effect *effect);
};