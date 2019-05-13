#include "HitChecker.h"

bool HitChecker::CheckHitPlayerEnemy(Player *player, Enemy **enemy, Boss *boss, Effect *effect, Camera *camera, Map *map, EnemyControl *enemyCtrl, int enemyMass)
{
	if (player->damageFlag)
	{
		return false;
	}

	bool contact = false;		//�G�l�~�[�ƐڐG������
	for (int i = 0; i < enemyMass; i++)
	{
		//�v���C���[�ƃG�l�~�[���������Ă��邩�𔻒�
		//�G�l�~�[����ʓ��ɂ��邩�A�G�l�~�[�������Ă��邩
		if (enemy[i]->CheckInsideScreen(camera, map) && enemy[i]->existFlag)
		{
			int correctionRate = 4;			//�␳����
			//�v���C���[�������G�l�~�[�̕��͈͓̔� && Y���������Ă���i�v���C���[����̂ق��j
			if ((player->hitCheckerUD >= enemy[i]->x - player->w / correctionRate &&
				player->hitCheckerUD <= enemy[i]->x + enemy[i]->w + player->w / correctionRate) &&
			(player->y + player->h > enemy[i]->y &&
				player->y + player->h < enemy[i]->y + enemy[i]->h))
			{
				bool hitBoss = false;			//�{�X����̃_���[�W���󂯂���
				if (!player->hitEnemyFlag)
				{
					//���������G�l�~�[���{�X�^�C�v�ŁA���܂��r�o����Ă��Ȃ��̂̂Ƃ��A�����I�Ƀv���C���[�����_���[�W
					if (enemy[i]->movePattern == enemyCtrl->BOSS && !boss->partsSpritFlag[i])
					{
						player->DamageFromBoss(enemy[i]->x, effect);
						boss->SetAngerFlagTrue();
						hitBoss = true;
					}
					else
					{
						//�G�l�~�[���_���[�W���󂯂鏈��
						enemy[i]->Damage(player, effect, enemyCtrl);
						
						//�X�R�A���Z
						if (player->kickFlag[1])
						{
							player->score += player->KICK_ENEMY_SCORE;
						}
						else
						{
							player->score += player->HIT_ENEMY_SCORE;
						}

						//�a���L�b�N�{�[�i�X�̌v�Z
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
				//��x�ɕ����̓G�͍U���ł��Ȃ����߃u���[�N
				break;
			}
			//�v���C���[�������G�l�~�[�̕��͈̔͂̊O && X���������Ă��� && Y���������Ă���
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
				//��x�ɕ����̓G����U���͎󂯂Ȃ����߃u���[�N
				break;
			}
		}
	}

	return contact;
}

void HitChecker::CheckHitPlayerGround(Player * player, Map * map, Game * game)
{
	//�㉺�̓����蔻���x���W���߂�
	player->hitCheckerUD = player->x + player->w / 2.0f + player->velocityX;

	//�o�E���h���邩���Ȃ����̔���
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

		// �Ȃ������ŋ��߂邱�Ƃɂ����̂��낤���E�E�E
		// for�����O���������A�|���Ď肪�o���Ȃ��̂Ō�łɂ���
		for (int i = 0; i < map->MAP_HEIGHT; i++)
		{
			if (map->MAP_HIGHEST + (map->MAP_HEIGHT - (i)) * map->MAPCHIP_SIZE_H < player->y + fallSpeed + player->h)
			{
				groundBlockNumY = (map->MAP_HEIGHT - i >= 0 || map->MAP_HEIGHT - i < map->MAP_HEIGHT) ? (map->MAP_HEIGHT - i) : 0;
				break;
			}
		}
		int groundBlockNumX;
		// �Ȃ������ŋ��߂邱�Ƃɂ����̂��낤���E�E�E
		// for�����O���������A�|���Ď肪�o���Ȃ��̂Ō�łɂ���
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
				//�����蔻�������
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
	//���E�̓����蔻���y���W���߂�
	player->hitCheckerLR = player->y + player->h / 2.0f;
	//�㉺�̓����蔻���x���W���߂�
	player->hitCheckerUD = player->x + player->w / 2.0f + player->velocityX;


	//�o�E���h���邩���Ȃ���(���ɓ���������)�̔���
	CheckHitPlayerGround(player, map, game);

	//�V��ɓ����������̔���
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

	//�������̓����蔻��
	{
		// ���̓����蔻��
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

		// �E�̓����蔻��
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
	//�ǂŒ��˕Ԃ������A�v���C���[���L�b�N���s���ł���ꍇ�A���̂܂܏���˂������Ă��܂��̂ŗ\�h�B
	//���˕Ԃ������Ƃɂ�����x�A���Ƃ̓����蔻����s��
	if (player->reboundFlag && player->kickFlag[1])
	{
		CheckHitPlayerGround(player, map, game);
	}
}

void HitChecker::CheckHitEnemyGround(Enemy *enemy, EnemyControl *enemyCtrl,Map *map)
{
	{
		//���E�̓����蔻���y���W���߂�
		enemy->hitCheckerLR = enemy->y + enemy->h / 2.0f;
	
		//�㉺�̓����蔻���x���W���߂�
		enemy->hitCheckerUD = enemy->x + enemy->w / 2.0f + enemy->velocityX;
	
		//�����牽�Ԗڂ�
		int groundBlockNumY;
		// �Ȃ������ŋ��߂邱�Ƃɂ����̂��낤���E�E�E
		// for�����O���������A�|���Ď肪�o���Ȃ��̂Ō�łɂ���
		for (int i = 0; i < map->MAP_HEIGHT; i++)
		{
			if (map->MAP_HIGHEST + (map->MAP_HEIGHT - (i)) * map->MAPCHIP_SIZE_H < enemy->y + enemy->h)
			{
				groundBlockNumY = (map->MAP_HEIGHT - i >= 0 && map->MAP_HEIGHT - i < map->MAP_HEIGHT) ? (map->MAP_HEIGHT - i) : 0;
				break;
			}
		}
		int groundBlockNumX;
		// �Ȃ������ŋ��߂邱�Ƃɂ����̂��낤���E�E�E
		// for�����O���������A�|���Ď肪�o���Ȃ��̂Ō�łɂ���
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
	//���E�̓����蔻���y���W���߂�
	enemy->hitCheckerLR = enemy->y + enemy->h / 2.0f;

	//�G�l�~�[���猩�č��̕ǂ��`�F�b�N
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
	//�G�l�~�[���猩�ĉE�̕ǂ��`�F�b�N
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
	//�{�X���G�l�~�[��S�r�o && �{�X�{�̂Ɠ������Ă��鎞
	if (boss->allEmission && (player->x + player->w > boss->x && ((player->y > boss->y && player->y < boss->y + boss->PARTS_SIZE_H * boss->GRAPH_EXTEND_RATE * boss->PARTS_MASS_Y) ||
		(player->y + player->h > boss->y && player->y + player->h < boss->y + boss->PARTS_SIZE_H * boss->GRAPH_EXTEND_RATE * boss->PARTS_MASS_Y))))
	{
		player->ReboundAgainstWall(boss->x, false);
	}
}

void HitChecker::CheckHitPlayerItem(Player * player, Item * item)
{
	//�񕜃A�C�e��
	for (int i = 0; i < item->RECOVER_ITEM_MASS; i++)
	{
		//�v���C���[�ƃA�C�e�����������Ă���΂��̌̂̃A�C�e�����g��
		if (item->recoverExist[i] && (((player->x + player->w > item->recoverX[i] && player->x < item->recoverX[i]) ||
			player->x > item->recoverX[i] && player->x < item->recoverX[i] + item->recoverW) &&
			((player->y + player->h > item->recoverY[i] && player->y < item->recoverY[i]) ||
				player->y > item->recoverY[i] && player->y < item->recoverY[i] + item->recoverW)))
		{
			item->Use(player, true, i);
			//���x���f�U�C����A1�t���[���ɓ���A�C�e�����g�����Ƃ͂Ȃ��̂�break
			break;
		}
	}

	//�X�R�A�A�C�e��
	for (int i = 0; i < item->SCORE_ITEM_MASS; i++)
	{
		//�v���C���[�ƃA�C�e�����������Ă���΂��̌̂̃A�C�e�����g��
		if (item->scoreExist[i] && (((player->x + player->w > item->scoreX[i] && player->x < item->scoreX[i]) ||
			player->x > item->scoreX[i] && player->x < item->scoreX[i] + item->scoreW) &&
			((player->y + player->h > item->scoreY[i] && player->y < item->scoreY[i]) ||
				player->y > item->scoreY[i] && player->y < item->scoreY[i] + item->scoreW)))
		{
			item->Use(player, false, i);
			//���x���f�U�C����A1�t���[���ɓ���A�C�e�����g�����Ƃ͂Ȃ��̂�break
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

					//�v���C���[�ƃR�C���̓����蔻��
					if (hit && coinMaster->coinGroup[i].coinArray[j][k].exist)
					{
						//�Q�Ƃ���̂�se�Ȃ̂ŁA�Y������0�ł��_�C�W���[�u�I�i���o�J�j
						coinMaster->coinGroup[i].coinArray[j][k].OnObtain(player, effect, coinMaster->coinGroup);
					}
				}
			}
		}
	}
}
