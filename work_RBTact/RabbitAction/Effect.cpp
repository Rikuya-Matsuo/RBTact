#include "Effect.h"

void Effect::InitKickEffect(int replayCnt)
{
	if (!replayCnt)
	{
		kickEff = LoadGraph("effectImg/kickFilter.png");
	}

	kickEffExtend = DEFAULT_KICK_GRAPH_EXTEND_RATE;
}

void Effect::DrawKickEffect(Player *player, Camera *camera)
{
	DrawRotaGraphF(player->x + player->w / 2 - camera->cameraOffsetX, player->y + player->h / 2 - camera->cameraOffsetY, kickEffExtend, 0.0, kickEff, TRUE);
	
	if (kickEffExtend < MAX_KICK_EFFECT_EXTEND_RATE)
	{
		kickEffExtend += INCRESE_EXTEND_MASS;
	}
}


void Effect::InitMeteoEffect(int replayCnt)
{
	if (!replayCnt)
	{
		LoadDivGraph("effectImg/flame_parts1.png", METEO_EFFECT_ANIMATION_MASS, 3, 3, 1200, 1200, meteoEff);
		LoadDivGraph("effectImg/explore01.png", HIT_METEO_EFFECT_ANIMATION_MASS, 10, 1, 240, 240, hitMeteoEff);
	}

	meteoAnimNum = 0;
	hitMeteoAnimNum = 0;

	drawExploreFlag = false;
}

void Effect::DrawMeteoEffect(Player * player, Camera * camera)
{
	//キックの距離が一定の値に達しているとき、描画
	if (player->burningFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		if (player->kickFlag[1])
		{
			DrawRotaGraphF(player->x + player->w / 2 - camera->cameraOffsetX, player->y + player->h / 2 - camera->cameraOffsetY, METEO_EFFECT_EXTEND_RATE, (player->reverseDrawFlag) ? DX_PI / 2 - player->kickAngle : player->kickAngle - DX_PI / 2,
					meteoEff[meteoAnimNum], FALSE);
		}
		else
		{
			float cosine;
			float angle;

			float playerVectLen = (float)GET_VECTOR_LENGTH(player->velocityX, player->velocityY);

			//int baseX = (reverseDrawFlag) ? -32767 : 32767;
			int baseY = 32767;

			cosine = INNER_PRODUCT(player->velocityX, player->velocityY, 0, baseY) / (playerVectLen * baseY);

			//角度を取得。
			angle = acosf(cosine);

			DrawRotaGraphF(player->x + player->w / 2 - camera->cameraOffsetX, player->y + player->h / 2 - camera->cameraOffsetY, METEO_EFFECT_EXTEND_RATE, (!player->reverseDrawFlag) ? -angle : angle,
				meteoEff[meteoAnimNum], FALSE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//アニメーション番号を更新。９以上になるなら０に戻す
	meteoAnimNum++;
	if (meteoAnimNum >= METEO_EFFECT_ANIMATION_MASS)
	{
		meteoAnimNum = 0;
	}
}

void Effect::SetHitMeteoEffect(Enemy *enemy, Player *player)
{
	drawExploreFlag = (player->burningFlag) ? true : false;
	hitMeteoAnimNum = 0;

	exploreX = enemy->x;
	exploreY = enemy->y;
	drawCnt = 0;
}

void Effect::DrawHitMeteoEffect(Enemy * enemy, Camera * camera)
{
	if (drawExploreFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

		DrawExtendGraphF(exploreX - HIT_METEO_EFFECT_OFFSET - camera->cameraOffsetX, exploreY - HIT_METEO_EFFECT_OFFSET - camera->cameraOffsetY,
			exploreX + enemy->w + HIT_METEO_EFFECT_OFFSET - camera->cameraOffsetX, exploreY + enemy->h + HIT_METEO_EFFECT_OFFSET - camera->cameraOffsetY, hitMeteoEff[hitMeteoAnimNum], TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		drawCnt++;
		if (drawCnt >= 3)
		{
			hitMeteoAnimNum++;
			drawCnt = 0;
		}
		if (hitMeteoAnimNum >= HIT_METEO_EFFECT_ANIMATION_MASS)
		{
			drawExploreFlag = false;
		}
	}
}


void Effect::InitStarEffect(int replayCnt)
{
	if (!replayCnt)
	{
		starEff = LoadGraph("effectImg/star.png");
	}
	for (int i = 0; i < STAR_MAX_MASS; i++)
	{
		starX[i] = 0;
		starY[i] = 0;
		starVelX[i] = 0;
		starVelY[i] = 0;
	}
}

void Effect::SetRandomStarVelocities()
{
	for (int i = 0; i < ((criticalEffectFlag) ? STAR_MAX_MASS : STAR_NORMAL_MASS); i++)
	{
		starVelX[i] = (float)(GetRand(STAR_MAX_VELOCITY) - STAR_MAX_VELOCITY_OFFSET);
		starVelY[i] = (float)(-GetRand(STAR_MAX_VELOCITY));
	}
}

void Effect::SetStarXY4Enemy(Enemy *enemy, Player *player)
{
	criticalEffectFlag = player->kickFlag[1];
	for (int i = 0; i < ((criticalEffectFlag) ? STAR_MAX_MASS : STAR_NORMAL_MASS); i++)
	{
		starX[i] = enemy->x + enemy->w / 2;
		starY[i] = enemy->y;
	}
	SetRandomStarVelocities();
}

void Effect::UpdateStar()
{
	for (int i = 0; i < ((criticalEffectFlag) ? STAR_MAX_MASS : STAR_NORMAL_MASS); i++)
	{
		starX[i] += starVelX[i];
		starVelY[i] += STAR_GRAVITY;
		starY[i] += starVelY[i];
	}
}

void Effect::DrawStarEffect(Camera * camera)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	for (int i = 0; i < ((criticalEffectFlag) ? STAR_MAX_MASS : STAR_NORMAL_MASS); i++)
	{
		DrawGraphF(starX[i] - camera->cameraOffsetX, starY[i] - camera->cameraOffsetY, starEff, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



void Effect::InitSmokeEffect(int replayCnt)
{
	if (!replayCnt)
	{
		LoadDivGraph("effectImg/enemyDown.png", 10, 10, 1, 120, 120, smokeEff);
	}
	for (int i = 0; i < SMOKE_MAX_MASS; i++)
	{
		smokeX[i] = 0.0f;
		smokeY[i] = 0.0f;
		smokeAnimNum[i] = -1;
		smokeFlag[i] = false;
	}
	smokeNumber = 0;
}

void Effect::SetSmokeEffect(Enemy *enemy)
{
	if (smokeNumber + 1 >= SMOKE_MAX_MASS)
	{
		smokeNumber = 0;
	}
	else
	{
		smokeNumber++;
	}

	smokeX[smokeNumber] = enemy->x + enemy->w / 2;
	smokeY[smokeNumber] = enemy->y + enemy->h / 2;
	smokeAnimNum[smokeNumber] = -1;
	smokeFlag[smokeNumber] = true;
	smokeTimer[smokeNumber] = GetNowCount();
}

void Effect::UpdateSmoke(EnemyControl *enemyCtrl)
{
	for (int i = 0; i < SMOKE_MAX_MASS; i++)
	{
		if (smokeFlag[i])
		{
			smokeAnimNum[i]++;
			//ちょうどいいタイミングでエネミーのやられサウンドを再生
			if (smokeAnimNum[i] == 5)
			{
				PlaySoundMem(enemyCtrl->downSound, DX_PLAYTYPE_BACK);
			}
			if (smokeAnimNum[i] == 10)
			{
				smokeFlag[i] = false;
			}
		}
	}
}

void Effect::DrawSmokeEffect(Camera *camera)
{
	for (int i = 0; i < SMOKE_MAX_MASS; i++)
	{
		if (smokeFlag[i])
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			DrawRotaGraphF(smokeX[smokeNumber] - camera->cameraOffsetX, smokeY[smokeNumber] - camera->cameraOffsetY, 1.5f, 0, smokeEff[smokeAnimNum[smokeNumber]], TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void Effect::InitDamageEffect(int replayCnt)
{
	if (!replayCnt)
	{
		damageEff = LoadGraph("effectImg/playerDamage.png");
	}
	damageEffX = 0.0f;
	damageEffY = 0.0f;
}

void Effect::SetDamageEffect(Player * player)
{
	damageEffX = player->x + player->w / 2;
	damageEffY = player->y + player->h / 2;
}

void Effect::DrawDamageEffect(Player *player, Camera * camera)
{
	if (player->damageFlag)
	{
		DrawRotaGraphF(damageEffX - camera->cameraOffsetX, damageEffY - camera->cameraOffsetY, 1.0f, 0, damageEff, TRUE);
	}
}


void Effect::InitPlayerDown()
{
	blackSqrGraph = LoadGraph("effectImg/black32x32.png");
	gameOverSetFlag = false;
}

void Effect::SetPlayerDown(Player * player)
{
	int i;
	for (i = 0; i < STAR_MAX_MASS; i++)
	{
		//星エフェクトの位置をセット
		starX[i] = player->x + player->w / 2;
		starY[i] = player->y + player->h / 2;
		//星エフェクトの飛ぶそれぞれの方向をセット
		starVelX[i] = STAR_MAX_VELOCITY * cosf(i * (DX_PI_F * 2 / STAR_MAX_MASS));
		starVelY[i] = STAR_MAX_VELOCITY * sinf(i * (DX_PI_F * 2 / STAR_MAX_MASS));
	}

	criticalEffectFlag = true;
	gameOverSetFlag = true;

	gameOverTime = GetNowCount();
}

void Effect::UpdatePlayerDown(StateFlags *state, Game * game, Camera *camera)
{
	int i;
	for (i = 0; i < STAR_MAX_MASS; i++)
	{
		starVelY[i] += STAR_GRAVITY;
		starX[i] += starVelX[i];
		starY[i] += starVelY[i];
	}

	DrawStarEffect(camera);

	//GAMEOVER_PERFORMANCE_COUNTミリ秒かけて、段々ブラックアウト(255を100%として、
	//少しずつパーセンテージを上げていく)
	blackOutRate =
		(255 / 100) * ((game->nowTime - gameOverTime) * 100 / game->GAMEOVER_PERFORMANCE_COUNT);
	if (blackOutRate > 255)
	{
		blackOutRate = 255;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackOutRate);

	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, blackSqrGraph, FALSE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (blackOutRate == 255)
	{
		game->gameOver = true;
	}
}


void Effect::InitCoinEffect(int replayCnt)
{
	if (!replayCnt)
	{
		LoadDivGraph("effectImg/getCoin.png", COIN_EFFECT_ANIMATION_MASS, 10, 1, 120, 120, coinEff);
	}
	
	coinEffAnimNum = -1;
	coinEffTimeFlag = false;
}

void Effect::UpdateCoinEffect()
{
	coinEffAnimNum++;

	if (coinEffAnimNum > COIN_EFFECT_ANIMATION_MASS)
	{
		coinEffAnimNum = -1;
		coinEffTimeFlag = false;
	}
}

void Effect::DrawCoinEffect(Player *player, Camera * camera)
{
	if (coinEffAnimNum != -1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawRotaGraphF(player->x + player->w / 2.0f - camera->cameraOffsetX, player->y + player->h / 2.0f - camera->cameraOffsetY, COIN_EFFECT_EXTEND_RATE, 0.0, coinEff[coinEffAnimNum], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


void Effect::InitGameClear()
{
	whiteSqrGraph = LoadGraph("effectImg/white32x32.png");
	gameClearSetFlag = false;
}

void Effect::SetGameClear()
{
	gameClearSetFlag = true;
	gameClearTime = GetNowCount();
}

void Effect::UpdateGameClear(StateFlags *state, Game *game)
{
	//GAMEOVER_PERFORMANCE_COUNTミリ秒かけて、段々ホワイトアウト(255を100%として、
	//少しずつパーセンテージを上げていく)
	whiteOutRate =
		(255 / 100) * ((game->nowTime - gameClearTime) * 100 / game->GAMEOVER_PERFORMANCE_COUNT);
	if (whiteOutRate > 255)
	{
		whiteOutRate = 255;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, whiteOutRate);

	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, whiteSqrGraph, FALSE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (whiteOutRate == 255)
	{
		game->gameClear = true;
	}
}