#include "Enemy.h"

void Enemy::InitBossType(int i, int *enemy0graph, Boss *boss, EnemyControl * enemyCtrl, Map *map)
{
	id = i;

	velocityX = velocityY = 0.0f;

	hitPoint = BOSSTYPE_HITPOINT_MAX;
	existFlag = true;
	onGroundFlag = false;
	directionRight = (id % 2) ? false : true;

	if (!i)
	{
		LoadDivGraph("image/enemy01.png", ANIM_GRAPH_NUM, 3, 1, 32, 32, graph);
	}
	else
	{
		for (int i = 0; i < ANIM_GRAPH_NUM; i++)
		{
			graph[i] = enemy0graph[i];
		}
	}
	animNum = 0;

	movePattern = enemyCtrl->BOSS;

	int bx, by;
	bx = i % boss->PARTS_MASS_Y;
	by = i / boss->PARTS_MASS_X;

	x = boss->POSITION_X + bx * (boss->PARTS_SIZE_W * boss->GRAPH_EXTEND_RATE);
	y = map->MAP_HIGHEST + by * (boss->PARTS_SIZE_H * boss->GRAPH_EXTEND_RATE);

	GetGraphSizeF(graph[0], &w, &h);
	w *= GRAPH_EXTEND_RATE;
	h *= GRAPH_EXTEND_RATE;
}

void Enemy::Update(Player *player, EnemyControl *enemyCtrl, Effect *effect, Game *game)
{
	if (existFlag)
	{
		if (!onGroundFlag)
		{
			if (player->kickFlag[0])
			{
				velocityY += game->GRAVITY / powf(player->SLOWMOTION_RATE, 0.5f);
				//velocityY += game->GRAVITY / player->SLOWMOTION_RATE;
				//物理法則より
				//velocityY += game->GRAVITY / (player->SLOWMOTION_RATE * 2);
			}
			else
			{
				velocityY += game->GRAVITY;
			}
		}

		if (onGroundFlag)
		{
			if (movePattern == enemyCtrl->JUMP && (game->nowTime - onGroundTime) > JUMP_ACTION_COUNT)
			{
		 		Jump(player);
			}
			else if (movePattern == enemyCtrl->HIGH_SPEED)
			{
				if ((game->nowTime - dashActionTime) > DASH_ACTION_COUNT)
				{
					Dash(player, game);
				}
				else
				{
					velocityX = 0.0f;
				}
			}
		}

		x += velocityX;
		if (player->kickFlag[0])
		{
			y += velocityY / powf(player->SLOWMOTION_RATE, 0.5f);
			//y += velocityY / player->SLOWMOTION_RATE;
			//y += velocityY;
		}
		else
		{
			y += velocityY;
		}

	}
	if (animNum == 1)
	{
		effect->UpdateStar();
		if ((game->nowTime - damageTime) > CRUSH_COUNT)
		{
			animNum = 2;
		}
	}
	else if (animNum == 2)
	{
		if (hitPoint <= 0 && existFlag)
		{
			effect->SetSmokeEffect(this);
			existFlag = false;
		}
		else
		{
			effect->UpdateStar();
			if ((game->nowTime - damageTime) > DAMAGE_COUNT)
			{
				animNum = 0;
			}
		}
	}
}

void Enemy::UpdateBossType(Enemy **enemy, Boss * boss, Player *player, Effect *effect, Game *game)
{
	int i, j;
	for (i = 0; i < boss->PARTS_MASS_Y; i++)
	{
		for (j = 0; j < boss->PARTS_MASS_X; j++)
		{
			int id = j + i * boss->PARTS_MASS_X;

			if (!boss->sprit[id])
			{
				enemy[id]->x = boss->x + j * boss->PARTS_SIZE_W * boss->GRAPH_EXTEND_RATE;
				enemy[id]->y = boss->y + i * boss->PARTS_SIZE_H * boss->GRAPH_EXTEND_RATE;
			}
			else
			{
				if (!enemy[id]->onGroundFlag)
				{
					if (player->kickFlag[0])
					{
						//重力
						enemy[id]->velocityY += game->GRAVITY / powf(player->SLOWMOTION_RATE, 0.5f);
						enemy[id]->x += enemy[id]->velocityX / player->SLOWMOTION_RATE;
						enemy[id]->y += enemy[id]->velocityY / powf(player->SLOWMOTION_RATE, 0.5f);
					}
					else
					{
						//重力
						enemy[id]->velocityY += game->GRAVITY;
						enemy[id]->x += enemy[id]->velocityX;
						enemy[id]->y += enemy[id]->velocityY;
					}
				}
				else
				{
					//エネミー着地後の挙動の関数
					enemy[id]->MoveBossType(game);
				}
			}

			if (enemy[id]->animNum == 1)
			{
				effect->UpdateStar();
				if ((game->nowTime - enemy[id]->damageTime) > enemy[id]->CRUSH_COUNT)
				{
					enemy[id]->animNum = 2;
				}
			}
			else if (enemy[id]->animNum == 2)
			{
				if (enemy[id]->hitPoint <= 0 && enemy[id]->existFlag)
				{
					effect->SetSmokeEffect(enemy[id]);
					boss->existEnemyPartsMass--;
					enemy[id]->existFlag = false;
				}
				else
				{
					effect->UpdateStar();
					if ((game->nowTime - enemy[id]->damageTime) > enemy[id]->DAMAGE_COUNT)
					{
						enemy[id]->animNum = 0;
					}
				}
			}
		}
	}

}

void Enemy::Jump(Player *player)
{
	velocityY = -JUMP_POWER;
	
	onGroundFlag = false;
}

void Enemy::Dash(Player *player, Game *game)
{
	if (game->nowTime - dashStopTime > DASH_INTERVAL)
	{
		if (directionRight)
		{
			velocityX = DASH_VELOCITY_X;
			if (player->kickFlag[0])
			{
				velocityX /= player->SLOWMOTION_RATE;
			}
			dashLength += velocityX;
			if (dashLength >= DASH_RANGE)
			{
				velocityX -= dashLength - DASH_RANGE;
				dashLength = 0.0f;
				ReverseFlag(&directionRight);
				dashCnt++;
				dashStopTime = GetNowCount();
			}
		}
		else
		{
			velocityX = -DASH_VELOCITY_X;
			if (player->kickFlag[0])
			{
				velocityX /= player->SLOWMOTION_RATE;
			}
			dashLength -= velocityX;
			if (dashLength >= DASH_RANGE)
			{
				velocityX -= -(dashLength - DASH_RANGE);
				dashLength = 0.0f;
				ReverseFlag(&directionRight);
				dashCnt++;
				dashStopTime = GetNowCount();
			}
		}

		if (dashCnt >= DASH_NUMBER_OF_TIMES)
		{
			dashCnt = 0;
			dashActionTime = GetNowCount();
		}
	}
	else
	{
		velocityX = 0.0f;
	}
}

void Enemy::MoveBossType(Game * game)
{
	onGroundFlag = false;
	velocityY = -BOSSTYPE_JUMP_POWER;
	if (directionRight)
	{
		velocityX = BOSSTYPE_VELOCITY_X;
	}
	else
	{
		velocityX = -BOSSTYPE_VELOCITY_X;
	}
}

void Enemy::HitWall(float wallEdge, bool leftSideWall)
{
	if (leftSideWall)
	{
		x = wallEdge;
	}
	else
	{
		x = wallEdge - w;
	}

	velocityX *= -1;
	ReverseFlag(&directionRight);
}

void Enemy::Damage(Player *player, Effect *effect, EnemyControl *enemyCtrl)
{
	if (player->kickFlag[1])
	{
		hitPoint -= 3;
	}
	else
	{
		hitPoint--;
	}
	animNum = 1;

	PlaySoundMem(enemyCtrl->damageSound, DX_PLAYTYPE_BACK);

	effect->SetStarXY4Enemy(this, player);
	effect->SetHitMeteoEffect(this, player);

	damageTime = GetNowCount();
}

void Enemy::Draw(Effect *effect, Camera *camera, Map *map)
{
	if (CheckInsideScreen(camera, map) && existFlag)
	{
		if (!directionRight)
		{
			DrawExtendGraphF(x - camera->cameraOffsetX, y - camera->cameraOffsetY, x + w - camera->cameraOffsetX, y + h - camera->cameraOffsetY, graph[animNum], TRUE);
		}
		else
		{
			DrawExtendGraphF(x + w - camera->cameraOffsetX, y - camera->cameraOffsetY, x - camera->cameraOffsetX, y + h - camera->cameraOffsetY, graph[animNum], TRUE);
		}
	}
	if (animNum)
	{
		effect->DrawStarEffect(camera);
	}
}

bool Enemy::CheckInsideScreen(Camera *camera, Map *map)
{
	bool ret;

	if ((x + w > camera->cameraOffsetX - DRAW_MARGIN && x < camera->cameraOffsetX + WINDOW_WIDTH + DRAW_MARGIN) &&
		(y + h > camera->cameraOffsetY + map->HEIGHT_LENGTH + map->MAP_HIGHEST - WINDOW_HEIGHT - DRAW_MARGIN && y < camera->cameraOffsetY + WINDOW_HEIGHT + DRAW_MARGIN))
	{
		ret = true;
	}
	else
	{
		ret = false;
	}

	return ret;
}
