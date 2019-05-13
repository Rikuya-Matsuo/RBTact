#include "HitChecker.h"

bool HitChecker::CheckHitPlayerEnemy(Player *player, Enemy **enemy, Boss *boss, Effect *effect, Camera *camera, Map *map, EnemyControl *enemyCtrl, int enemyMass)
{
	if (player->damageFlag)
	{
		return false;
	}

	bool contact = false;		//エネミーと接触したか
	for (int i = 0; i < enemyMass; i++)
	{
		//プレイヤーとエネミーが当たっているかを判定
		//エネミーが画面内にいるか、エネミーが生きているか
		if (enemy[i]->CheckInsideScreen(camera, map) && enemy[i]->existFlag)
		{
			int correctionRate = 4;			//補正割合
			//プレイヤー中央がエネミーの幅の範囲内 && Yが当たっている（プレイヤーが上のほう）
			if ((player->hitCheckerUD >= enemy[i]->x - player->w / correctionRate &&
				player->hitCheckerUD <= enemy[i]->x + enemy[i]->w + player->w / correctionRate) &&
			(player->y + player->h > enemy[i]->y &&
				player->y + player->h < enemy[i]->y + enemy[i]->h))
			{
				bool hitBoss = false;			//ボスからのダメージを受けたか
				if (!player->hitEnemyFlag)
				{
					//当たったエネミーがボスタイプで、かつまだ排出されていない個体のとき、強制的にプレイヤー側がダメージ
					if (enemy[i]->movePattern == enemyCtrl->BOSS && !boss->partsSpritFlag[i])
					{
						player->DamageFromBoss(enemy[i]->x, effect);
						boss->SetAngerFlagTrue();
						hitBoss = true;
					}
					else
					{
						//エネミーがダメージを受ける処理
						enemy[i]->Damage(player, effect, enemyCtrl);
						
						//スコア加算
						if (player->kickFlag[1])
						{
							player->score += player->KICK_ENEMY_SCORE;
						}
						else
						{
							player->score += player->HIT_ENEMY_SCORE;
						}

						//彗星キックボーナスの計算
						if (player->burningFlag)
						{
							player->burningStompTime = GetNowCount();
							player->score += player->BURNING_BONUS;
							player->burningTextX = player->x;
							player->burningTextY = player->y - 10;
						}
					}
				}
				player->hitEnemyFlag = true;
				if (!hitBoss)
				{
					player->Bound(enemy[i]->y);
				}
				contact = true;
				//一度に複数の敵は攻撃できないためブレーク
				break;
			}
			//プレイヤー中央がエネミーの幅の範囲の外 && Xが当たっている && Yが当たっている
			else if ((player->hitCheckerUD < enemy[i]->x - player->w / correctionRate ||
				player->hitCheckerUD > enemy[i]->x + enemy[i]->w + player->w / correctionRate) &&
				((player->x > enemy[i]->x &&
					player->x < enemy[i]->x + enemy[i]->w) ||
					(player->x + player->w > enemy[i]->x &&
						player->x + player->w < enemy[i]->x + enemy[i]->w)) &&
				(player->hitCheckerLR > enemy[i]->y &&
					player->hitCheckerLR < enemy[i]->y + enemy[i]->h))
			{
				bool hitLeftsideOfEnemy = false;
				if (player->x > enemy[i]->x &&
					player->x < enemy[i]->x + enemy[i]->w)
				{
					hitLeftsideOfEnemy = true;
				}

				player->Damage(hitLeftsideOfEnemy, effect);
				if (enemy[i]->movePattern == enemyCtrl->BOSS && !boss->partsSpritFlag[i])
				{
					boss->SetAngerFlagTrue();
				}
				contact = true;
				//一度に複数の敵から攻撃は受けないためブレーク
				break;
			}
		}
	}

	return contact;
}

void HitChecker::CheckHitPlayerGround(Player * player, Map * map, Game * game)
{
	//上下の当たり判定のx座標を定める
	player->hitCheckerUD = player->x + player->w / 2.0f + player->velocityX;

	//バウンドするかしないかの判定
	{
		int groundBlockNumY;
		///float fallSpeed = (player->jumpFlag) ? player->velocityY : 0.0f;
		float fallSpeed = 0.0f;
		/*
		if (player->jumpFlag)
		{
			fallSpeed = player->velocityY;
			if (player->kickFlag[0])
			{
				fallSpeed /= player->SLOWMOTION_RATE;
			}
		}
		*/

		// なぜ走査で求めることにしたのだろうか・・・
		// for文を外したいが、怖くて手が出せないので後でにする
		for (int i = 0; i < map->MAP_HEIGHT; i++)
		{
			if (map->MAP_HIGHEST + (map->MAP_HEIGHT - (i)) * map->MAPCHIP_SIZE_H < player->y + fallSpeed + player->h)
			{
				groundBlockNumY = (map->MAP_HEIGHT - i >= 0 || map->MAP_HEIGHT - i < map->MAP_HEIGHT) ? (map->MAP_HEIGHT - i) : 0;
				break;
			}
		}
		int groundBlockNumX;
		// なぜ走査で求めることにしたのだろうか・・・
		// for文を外したいが、怖くて手が出せないので後でにする
		for (int i = 0; i < map->MAP_WIDTH; i++)
		{
			if ((i + 1) * map->MAPCHIP_SIZE_W > player->hitCheckerUD)
			{
				groundBlockNumX = i;
				break;
			}
		}
		if ((groundBlockNumY >= 0 && groundBlockNumY < map->MAP_HEIGHT) && (groundBlockNumX >= 0 && groundBlockNumX < map->MAP_WIDTH))
		{
			if (map->mapPattern[groundBlockNumY][groundBlockNumX] >= 0 || 
				(map->mapPattern[groundBlockNumY][groundBlockNumX] == -1 && map->MAP_HIGHEST + groundBlockNumY * map->MAPCHIP_SIZE_H > map->GROUND_Y))
			{
				//当たり判定条件式
				if (map->MAP_HIGHEST + groundBlockNumY * map->MAPCHIP_SIZE_H < player->y + player->h + fallSpeed &&
					map->MAP_HIGHEST + (groundBlockNumY + 1) * map->MAPCHIP_SIZE_H > player->y + player->h + fallSpeed)
				{
					if ((!player->graphNum4anim && player->jumpFlag) || (!player->jumpFlag))
					{
						player->Bound(map->MAP_HIGHEST + (groundBlockNumY * map->MAPCHIP_SIZE_H));
						player->ResetKickDistance();
						player->hitGroundFlag = true;
					}
					else if (!player->hitEnemyFlag)
					{
						player->Fall(game);
					}
				}
			}
			else if (!player->hitEnemyFlag)
			{
				player->Fall(game);
			}
		}
	}
}

void HitChecker::CheckHitPlayerBlock(Player *player, Map *map, Game *game)
{
	//左右の当たり判定のy座標を定める
	player->hitCheckerLR = player->y + player->h / 2.0f;
	//上下の当たり判定のx座標を定める
	player->hitCheckerUD = player->x + player->w / 2.0f + player->velocityX;


	//バウンドするかしないか(床に当たったか)の判定
	CheckHitPlayerGround(player, map, game);

	//天井に当たったかの判定
	{
		int upperBlockNumX;
		upperBlockNumX = (int)(player->hitCheckerUD / map->MAPCHIP_SIZE_W);

		int upperBlockNumY;
		upperBlockNumY = (int)((player->y - map->MAP_HIGHEST) / map->MAPCHIP_SIZE_H);
		if (upperBlockNumY < 0 || upperBlockNumY > map->MAP_HEIGHT)
		{
			upperBlockNumY = 0;
		}

		if (map->mapPattern[upperBlockNumY][upperBlockNumX] >= 0)
		{
			player->HitCeil((upperBlockNumY + 1) * map->MAPCHIP_SIZE_H + map->MAP_HIGHEST);
		}
	}

	//横方向の当たり判定
	{
		// 左の当たり判定
		{
			int leftBlockNumX;
			leftBlockNumX = (int)((player->x) / map->MAPCHIP_SIZE_W);

			int leftBlockNumY;
			leftBlockNumY = (int)(((player->hitCheckerLR) - map->MAP_HIGHEST) / map->MAPCHIP_SIZE_H);
			if (leftBlockNumY < 0 || leftBlockNumY > map->MAP_HEIGHT)
			{
				leftBlockNumY = 0;
			}

			if (map->mapPattern[leftBlockNumY][leftBlockNumX] >= 0)
			{
				player->ReboundAgainstWall(((leftBlockNumX + 1) * map->MAPCHIP_SIZE_W), true);
			}
		}

		// 右の当たり判定
		{
			int rightBlockNumX;
			rightBlockNumX = (int)((player->x + player->w) / map->MAPCHIP_SIZE_W);

			int rightBlockNumY;
			rightBlockNumY = (int)(((player->hitCheckerLR) - map->MAP_HIGHEST) / map->MAPCHIP_SIZE_H);
			if (rightBlockNumY < 0 || rightBlockNumY > map->MAP_HEIGHT)
			{
				rightBlockNumY = 0;
			}

			if (map->mapPattern[rightBlockNumY][rightBlockNumX] >= 0)
			{
				player->ReboundAgainstWall((rightBlockNumX * map->MAPCHIP_SIZE_W), false);
			}
		}
	}
	//壁で跳ね返った時、プレイヤーがキック実行中である場合、そのまま床を突き抜けてしまうので予防。
	//跳ね返ったあとにもう一度、床との当たり判定を行う
	if (player->reboundFlag && player->kickFlag[1])
	{
		CheckHitPlayerGround(player, map, game);
	}
}

void HitChecker::CheckHitEnemyGround(Enemy *enemy, EnemyControl *enemyCtrl,Map *map)
{
	{
		//左右の当たり判定のy座標を定める
		enemy->hitCheckerLR = enemy->y + enemy->h / 2.0f;
	
		//上下の当たり判定のx座標を定める
		enemy->hitCheckerUD = enemy->x + enemy->w / 2.0f + enemy->velocityX;
	
		//下から何番目か
		int groundBlockNumY;
		// なぜ走査で求めることにしたのだろうか・・・
		// for文を外したいが、怖くて手が出せないので後でにする
		for (int i = 0; i < map->MAP_HEIGHT; i++)
		{
			if (map->MAP_HIGHEST + (map->MAP_HEIGHT - (i)) * map->MAPCHIP_SIZE_H < enemy->y + enemy->h)
			{
				groundBlockNumY = (map->MAP_HEIGHT - i >= 0 && map->MAP_HEIGHT - i < map->MAP_HEIGHT) ? (map->MAP_HEIGHT - i) : 0;
				break;
			}
		}
		int groundBlockNumX;
		// なぜ走査で求めることにしたのだろうか・・・
		// for文を外したいが、怖くて手が出せないので後でにする
		for (int i = 0; i < map->MAP_WIDTH; i++)
		{
			if ((i + 1) * map->MAPCHIP_SIZE_W > enemy->hitCheckerUD)
			{
				groundBlockNumX = i;
				break;
			}
		}
		if (map->mapPattern[groundBlockNumY][groundBlockNumX] >= 0 && (!enemy->onGroundFlag))
		{
			enemy->velocityY = 0.0f;
			enemy->y = map->MAP_HIGHEST + (groundBlockNumY * map->MAPCHIP_SIZE_H) - enemy->h;
			enemy->onGroundTime = GetNowCount();
			enemy->onGroundFlag = true;
		}
	}
}

void HitChecker::CheckHitEnemyWall(Enemy * enemy, EnemyControl * enemyCtrl, Map * map)
{
	//左右の当たり判定のy座標を定める
	enemy->hitCheckerLR = enemy->y + enemy->h / 2.0f;

	//エネミーから見て左の壁をチェック
	if (enemy->velocityX < 0)
	{
		int wallBlockNumX = (int)(enemy->x / map->MAPCHIP_SIZE_W);
		int wallBlockNumY = (int)((enemy->hitCheckerLR - map->MAP_HIGHEST) / map->MAPCHIP_SIZE_H);

		float wallEdgeX = map->MAPCHIP_SIZE_W * (wallBlockNumX + 1);

		if (map->mapPattern[wallBlockNumY][wallBlockNumX] >= 0)
		{
			if (enemy->movePattern == enemyCtrl->BOSS)
			{
				enemy->HitWall(wallEdgeX, true);
			}
		}
	}
	//エネミーから見て右の壁をチェック
	else if (enemy->velocityX > 0)
	{
		int wallBlockNumX = (int)((enemy->x + enemy->w) / map->MAPCHIP_SIZE_W);
		int wallBlockNumY = (int)((enemy->hitCheckerLR - map->MAP_HIGHEST) / map->MAPCHIP_SIZE_H);

		float wallEdgeX = map->MAPCHIP_SIZE_W * wallBlockNumX;

		if (map->mapPattern[wallBlockNumY][wallBlockNumX] >= 0)
		{
			if (enemy->movePattern == enemyCtrl->BOSS)
			{
				enemy->HitWall(wallEdgeX, false);
			}
		}
	}
}

void HitChecker::CheckHitEnemyBlock(Enemy * enemy, EnemyControl * enemyCtrl, Map * map)
{
	CheckHitEnemyGround(enemy, enemyCtrl, map);

	CheckHitEnemyWall(enemy, enemyCtrl, map);
}

void HitChecker::CheckHitEnemyBoss(Enemy * enemy, Boss * boss)
{
	if (enemy->x + enemy->w > boss->x && boss->partsSpritFlag[enemy->id])
	{
		enemy->HitWall(boss->x, false);
	}
}

void HitChecker::CheckHitPlayerBoss(Player * player, Boss * boss)
{
	//ボスがエネミーを全排出 && ボス本体と当たっている時
	if (boss->allEmission && (player->x + player->w > boss->x && ((player->y > boss->y && player->y < boss->y + boss->PARTS_SIZE_H * boss->GRAPH_EXTEND_RATE * boss->PARTS_MASS_Y) ||
		(player->y + player->h > boss->y && player->y + player->h < boss->y + boss->PARTS_SIZE_H * boss->GRAPH_EXTEND_RATE * boss->PARTS_MASS_Y))))
	{
		player->ReboundAgainstWall(boss->x, false);
	}
}

void HitChecker::CheckHitPlayerItem(Player * player, Item * item)
{
	//回復アイテム
	for (int i = 0; i < item->RECOVER_ITEM_MASS; i++)
	{
		//プレイヤーとアイテムが当たっていればその個体のアイテムを使う
		if (item->recoverExist[i] && (((player->x + player->w > item->recoverX[i] && player->x < item->recoverX[i]) ||
			player->x > item->recoverX[i] && player->x < item->recoverX[i] + item->recoverW) &&
			((player->y + player->h > item->recoverY[i] && player->y < item->recoverY[i]) ||
				player->y > item->recoverY[i] && player->y < item->recoverY[i] + item->recoverW)))
		{
			item->Use(player, true, i);
			//レベルデザイン上、1フレームに二つもアイテムを使うことはないのでbreak
			break;
		}
	}

	//スコアアイテム
	for (int i = 0; i < item->SCORE_ITEM_MASS; i++)
	{
		//プレイヤーとアイテムが当たっていればその個体のアイテムを使う
		if (item->scoreExist[i] && (((player->x + player->w > item->scoreX[i] && player->x < item->scoreX[i]) ||
			player->x > item->scoreX[i] && player->x < item->scoreX[i] + item->scoreW) &&
			((player->y + player->h > item->scoreY[i] && player->y < item->scoreY[i]) ||
				player->y > item->scoreY[i] && player->y < item->scoreY[i] + item->scoreW)))
		{
			item->Use(player, false, i);
			//レベルデザイン上、1フレームに二つもアイテムを使うことはないのでbreak
			break;
		}
	}
}

void HitChecker::CheckHitPlayerCoin(Player * player, CoinGroupManager *coinMaster, Effect *effect)
{
	for (int i = 0; i < coinMaster->COIN_AREA_MASS; i++)
	{
		for (int j = 0; j < coinMaster->coinGroup[i].coinMassY; j++)
		{
			for (int k = 0; k < coinMaster->coinGroup[i].coinMassX; k++)
			{
				if (coinMaster->coinGroup[i].coinArray[j][k].inScreen)
				{
					bool hit = (((player->x + player->w > coinMaster->coinGroup[i].coinArray[j][k].x && player->x < coinMaster->coinGroup[i].coinArray[j][k].x) ||
						player->x > coinMaster->coinGroup[i].coinArray[j][k].x && player->x < coinMaster->coinGroup[i].coinArray[j][k].x + coinMaster->coinGroup[i].coinW) &&
						((player->y + player->h > coinMaster->coinGroup[i].coinArray[j][k].y && player->y < coinMaster->coinGroup[i].coinArray[j][k].y) ||
							player->y > coinMaster->coinGroup[i].coinArray[j][k].y && player->y < coinMaster->coinGroup[i].coinArray[j][k].y + coinMaster->coinGroup[i].coinH));

					//プレイヤーとコインの当たり判定
					if (hit && coinMaster->coinGroup[i].coinArray[j][k].exist)
					{
						//参照するのはseなので、添え字は0でもダイジョーブ！（おバカ）
						coinMaster->coinGroup[i].coinArray[j][k].OnObtain(player, effect, coinMaster->coinGroup);
					}
				}
			}
		}
	}
}
