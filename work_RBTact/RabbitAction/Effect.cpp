#include "Effect.h"

void Effect::InitKickEffect(int replayCnt)
{
	if (!replayCnt)
	{
		kickPrepareEffect = LoadGraph("effectImg/kickFilter.png");
	}

	kickPrepareEffectExtend = DEFAULT_KICK_GRAPH_EXTEND_RATE;
}

void Effect::DrawKickEffect(Player *player, Camera *camera)
{
	DrawRotaGraphF(player->x + player->w / 2 - camera->cameraOffsetX, player->y + player->h / 2 - camera->cameraOffsetY, kickPrepareEffectExtend, 0.0, kickPrepareEffect, TRUE);
	
	if (kickPrepareEffectExtend < MAX_KICK_EFFECT_EXTEND_RATE)
	{
		kickPrepareEffectExtend += INCRESE_EXTEND_MASS;
	}
}


void Effect::InitMeteoEffect(int replayCnt)
{
	if (!replayCnt)
	{
		LoadDivGraph("effectImg/flame_parts1.png", METEO_EFFECT_ANIMATION_MASS, 3, 3, 1200, 1200, meteoEffect);
		LoadDivGraph("effectImg/explore01.png", HIT_METEO_EFFECT_ANIMATION_MASS, 10, 1, 240, 240, hitMeteoEffect);
	}

	meteoAnimNum = 0;
	hitMeteoAnimNum = 0;

	drawHitMeteoEffectFlag = false;
}

void Effect::DrawMeteoEffect(Player * player, Camera * camera)
{
	//�L�b�N�̋��������̒l�ɒB���Ă���Ƃ��A�`��
	if (player->burningFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		if (player->kickFlag[1])
		{
			DrawRotaGraphF(player->x + player->w / 2 - camera->cameraOffsetX, player->y + player->h / 2 - camera->cameraOffsetY, METEO_EFFECT_EXTEND_RATE, (player->reverseDrawFlag) ? DX_PI / 2 - player->kickAngle : player->kickAngle - DX_PI / 2,
					meteoEffect[meteoAnimNum], FALSE);
		}
		else
		{
			float cosine;
			float angle;

			float playerVectLen = (float)GET_VECTOR_LENGTH(player->velocityX, player->velocityY);

			//int baseX = (reverseDrawFlag) ? -32767 : 32767;
			int baseY = 32767;

			cosine = INNER_PRODUCT(player->velocityX, player->velocityY, 0, baseY) / (playerVectLen * baseY);

			//�p�x���擾�B
			angle = acosf(cosine);

			DrawRotaGraphF(player->x + player->w / 2 - camera->cameraOffsetX, player->y + player->h / 2 - camera->cameraOffsetY, METEO_EFFECT_EXTEND_RATE, (!player->reverseDrawFlag) ? -angle : angle,
				meteoEffect[meteoAnimNum], FALSE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//�A�j���[�V�����ԍ����X�V�B�X�ȏ�ɂȂ�Ȃ�O�ɖ߂�
	meteoAnimNum++;
	if (meteoAnimNum >= METEO_EFFECT_ANIMATION_MASS)
	{
		meteoAnimNum = 0;
	}
}

void Effect::SetHitMeteoEffect(Enemy *enemy, Player *player)
{
	drawHitMeteoEffectFlag = (player->burningFlag) ? true : false;
	hitMeteoAnimNum = 0;

	hitMeteoEffectPosX = enemy->x;
	hitMeteoEffectPosY = enemy->y;
	drawCnt = 0;
}

void Effect::DrawHitMeteoEffect(Enemy * enemy, Camera * camera)
{
	if (drawHitMeteoEffectFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

		DrawExtendGraphF(hitMeteoEffectPosX - HIT_METEO_EFFECT_OFFSET - camera->cameraOffsetX, hitMeteoEffectPosY - HIT_METEO_EFFECT_OFFSET - camera->cameraOffsetY,
			hitMeteoEffectPosX + enemy->w + HIT_METEO_EFFECT_OFFSET - camera->cameraOffsetX, hitMeteoEffectPosY + enemy->h + HIT_METEO_EFFECT_OFFSET - camera->cameraOffsetY, hitMeteoEffect[hitMeteoAnimNum], TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		drawCnt++;
		if (drawCnt >= 3)
		{
			hitMeteoAnimNum++;
			drawCnt = 0;
		}
		if (hitMeteoAnimNum >= HIT_METEO_EFFECT_ANIMATION_MASS)
		{
			drawHitMeteoEffectFlag = false;
		}
	}
}


void Effect::InitStarEffect(int replayCnt)
{
	if (!replayCnt)
	{
		starEffect = LoadGraph("effectImg/star.png");
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
		DrawGraphF(starX[i] - camera->cameraOffsetX, starY[i] - camera->cameraOffsetY, starEffect, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



void Effect::InitSmokeEffect(int replayCnt)
{
	if (!replayCnt)
	{
		LoadDivGraph("effectImg/enemyDown.png", 10, 10, 1, 120, 120, smokeEffect);
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
			//���傤�ǂ����^�C�~���O�ŃG�l�~�[�̂���T�E���h���Đ�
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
			DrawRotaGraphF(smokeX[smokeNumber] - camera->cameraOffsetX, smokeY[smokeNumber] - camera->cameraOffsetY, 1.5f, 0, smokeEffect[smokeAnimNum[smokeNumber]], TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void Effect::InitDamageEffect(int replayCnt)
{
	if (!replayCnt)
	{
		damageEffect = LoadGraph("effectImg/playerDamage.png");
	}
	damageEffectX = 0.0f;
	damageEffectY = 0.0f;
}

void Effect::SetDamageEffect(Player * player)
{
	damageEffectX = player->x + player->w / 2;
	damageEffectY = player->y + player->h / 2;
}

void Effect::DrawDamageEffect(Player *player, Camera * camera)
{
	if (player->damageFlag)
	{
		DrawRotaGraphF(damageEffectX - camera->cameraOffsetX, damageEffectY - camera->cameraOffsetY, 1.0f, 0, damageEffect, TRUE);
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
		//���G�t�F�N�g�̈ʒu���Z�b�g
		starX[i] = player->x + player->w / 2;
		starY[i] = player->y + player->h / 2;
		//���G�t�F�N�g�̔�Ԃ��ꂼ��̕������Z�b�g
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

	//GAMEOVER_PERFORMANCE_COUNT�~���b�����āA�i�X�u���b�N�A�E�g(255��100%�Ƃ��āA
	//�������p�[�Z���e�[�W���グ�Ă���)
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
		LoadDivGraph("effectImg/getCoin.png", COIN_EFFECT_ANIMATION_MASS, 10, 1, 120, 120, coinEffect);
	}
	
	coinEffectAnimNum = -1;
	coinEffectTimeFlag = false;
}

void Effect::UpdateCoinEffect()
{
	coinEffectAnimNum++;

	if (coinEffectAnimNum > COIN_EFFECT_ANIMATION_MASS)
	{
		coinEffectAnimNum = -1;
		coinEffectTimeFlag = false;
	}
}

void Effect::DrawCoinEffect(Player *player, Camera * camera)
{
	if (coinEffectAnimNum != -1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawRotaGraphF(player->x + player->w / 2.0f - camera->cameraOffsetX, player->y + player->h / 2.0f - camera->cameraOffsetY, COIN_EFFECT_EXTEND_RATE, 0.0, coinEffect[coinEffectAnimNum], TRUE);
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
	//GAMEOVER_PERFORMANCE_COUNT�~���b�����āA�i�X�z���C�g�A�E�g(255��100%�Ƃ��āA
	//�������p�[�Z���e�[�W���グ�Ă���)
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