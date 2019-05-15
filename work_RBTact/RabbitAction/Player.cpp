#include "Player.h"

void Player::InitForTitle()
{
	Load();
	x = TITLE_POSITION_X;
	y = TITLE_POSITION_Y;

	GetGraphSizeF(graph[0], &w, &h);
	w *= GRAPH_EXTEND_RATE;
	h *= GRAPH_EXTEND_RATE;

	graphNum4anim = 0;
	velocityX = velocityY = 0.0f;

	aliveFlag = true;
	jumpFlag = true;
	prevJumpCmd = false;
	hitEnemyFlag = false;
	reboundFlag = false;
	damageFlag = false;
	reverseDrawFlag = false;
	burningFlag = false;
	for (int i = 0; i < 2; i++)
	{
		kickFlag[i] = false;
	}
	jumpCmd = false;
	prevKickCmd = false;
}

void Player::Init(int replayCnt, Game *game)
{
	if (!replayCnt)
	{
		Load();
	}
	graphNum4anim = 0;
	GetGraphSizeF(graph[0], &w, &h);
	w *= GRAPH_EXTEND_RATE;
	h *= GRAPH_EXTEND_RATE;
	x = 500.0f;
	y = 500.0f;

	velocityX = velocityY = 0.0f;
	kickDistance = 0.0f;

	score = 0;
	hitPoint = HITPOINT_MAX;

	kickVectorX = KICK_SPEED_X;
	kickVectorY = KICK_SPEED_Y;

	kickAngle = DX_PI_F / 4;

	GetGraphSizeF(arrowGraph, &arrowGraphW, &arrowGraphH);

	aliveFlag = true;
	jumpFlag = true;
	prevJumpCmd = false;
	hitEnemyFlag = false;
	reboundFlag = false;
	damageFlag = false;
	reverseDrawFlag = false;
	burningFlag = false;
	for (int i = 0; i < 2; i++)
	{
		kickFlag[i] = false;
	}
	jumpCmd = false;
	prevKickCmd = false;
}

void Player::InitForResult(Effect *effect)
{
	x = WINDOW_WIDTH / 2 - w / 2;
	y = WINDOW_HEIGHT / 2;

	graphNum4anim = 0;

	kickDistance = effect->METEO_EFFECT_START_DISTANCE + 1;
	reverseDrawFlag = true;
	burningFlag = true;
	kickAngle = DX_PI_F / 6;
	kickFlag[0] = false;
	kickFlag[1] = true;
}

void Player::Load()
{
	LoadDivGraph("image/redBall_anim.png", 3, 3, 1, 32, 32, graph);
	arrowGraph = LoadGraph("image/arrowMark.png");
	jumpSound = LoadSoundMem("sound/se/puyon1.mp3");
	damageSound = LoadSoundMem("sound/se/tickle1.mp3");
	meteoSound = LoadSoundMem("sound/se/magic-flame2.mp3");
	ChangeVolumeSoundMem(175, jumpSound);
	ChangeVolumeSoundMem(175, damageSound);
	ChangeVolumeSoundMem(150, meteoSound);
}

void Player::UpdateControl(XINPUT_STATE *pad, Effect *effect, Game *game, ProgramControl *programCtrl)
{
	//リバウンドフラグの更新
	if (reboundFlag && (game->nowTime - reboundStartTime) > REBOUND_TIME)
	{
		reboundFlag = false;
	}

	//着地フラグの更新
	hitGroundFlag = false;

	//ダメージフラグの更新
	if (damageFlag && (game->nowTime - damageTime) > DAMAGE_REBOUND_TIME)
	{
		damageFlag = false;
	}

	//ヒットエネミーフラグの更新
	if (!graphNum4anim && hitEnemyFlag)
	{
		hitEnemyFlag = false;
	}

	//ジャンプコマンドフラグの更新
	if (jumpCmd && (game->nowTime - jumpCmdReleaseTime) > JUMP_COMMAND_OFF_TIME)
	{
		jumpCmd = false;
	}

	//HPが残っているかの判定
	if (hitPoint <= 0)
	{
		aliveFlag = false;
	}

	if (aliveFlag && !game->bossEventFlag)
	{
		//キックコマンドの処理
		if (jumpFlag)
		{
			if (CheckHitKey(KEY_INPUT_C) || pad->RightTrigger > 128)
			{
				if (!prevKickCmd)
				{
					prevKickCmd = true;
					//キックフラグ0がfalseなら0を、trueなら1をtrueに
					kickFlag[(!kickFlag[0]) ? 0 : 1] = true;
					//上の結果、1がtrueなら0をfalseに
					if (kickFlag[1])
					{
						effect->kickPrepareEffectExtend = effect->DEFAULT_KICK_GRAPH_EXTEND_RATE;
						kickFlag[0] = false;
					}
					//0がtrueならキック準備の初期化
					if (kickFlag[0])
					{
						/*
						kickVectorX = KICK_SPEED_X;
						kickVectorY = KICK_SPEED_Y;
						*/
						kickAngle = DX_PI_F / 4;
						kickReadyTime = GetNowCount();
					}
				}
			}
			else
			{
				prevKickCmd = false;
			}
		}
		else if (kickFlag[0])
		{
			//着地中にキックフラグ0がtrueになっているとキックフラグ1をtrueにできないため、予防。
			kickFlag[0] = false;
		}

		//キック準備段階の挙動
		if (kickFlag[0])
		{
			DrawFormatString(0, 0, GetColor(255, 0, 0), "キック準備中");

			//角度選択のプログラム
			KickDirectionControl(pad);
		}
		else
		{
			if (CheckHitKey(KEY_INPUT_SPACE) || pad->Buttons[XINPUT_BUTTON_A])
			{
				jumpCmd = true;
			}
			else if (prevJumpCmd)
			{
				jumpCmdReleaseTime = GetNowCount();
			}
			//キック実行
			//着地時にフラグをfalseに
			if (kickFlag[1])
			{
				if (!reverseDrawFlag)
				{
					velocityX = kickVectorX;
				}
				else
				{
					velocityX = -kickVectorX;
				}
				velocityY = kickVectorY;
			}
			else
			{
				if (!reboundFlag && !damageFlag && !game->bossEventFlag)
				{//移動処理
					if (CheckHitKey(KEY_INPUT_RIGHT) || pad->Buttons[XINPUT_BUTTON_DPAD_RIGHT] || pad->ThumbLX > PAD_STICK_DEADZONE)
					{
						velocityX += ACCELERATION_X;
						if (velocityX > MAX_SPEED_X)
						{
							velocityX = MAX_SPEED_X;
						}
					}
					if (CheckHitKey(KEY_INPUT_LEFT) || pad->Buttons[XINPUT_BUTTON_DPAD_LEFT] || pad->ThumbLX < -PAD_STICK_DEADZONE)
					{
						velocityX -= ACCELERATION_X;
						if (velocityX < -MAX_SPEED_X)
						{
							velocityX = -MAX_SPEED_X;
						}
					}

					//左右が同時に押されているか、どちらも押されていないかで
					//ブレーキをかける
					//キーボード操作を受けていない && パッドのスティック操作を受けていない && パッドのボタン操作を受けていない
					if ((!(CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT)) ||
						(CheckHitKey(KEY_INPUT_RIGHT) && CheckHitKey(KEY_INPUT_LEFT))) &&
						(!(pad->Buttons[XINPUT_BUTTON_DPAD_RIGHT] || pad->Buttons[XINPUT_BUTTON_DPAD_LEFT]) &&
						(pad->ThumbLX < PAD_STICK_DEADZONE && pad->ThumbLX > -PAD_STICK_DEADZONE)))
					{
						velocityX *= BRAKE_RATE;
						if (!reverseDrawFlag && velocityX < BRAKE_RATE)
						{
							velocityX = 0.0f;
						}
						if (reverseDrawFlag && velocityX > -BRAKE_RATE)
						{
							velocityX = 0.0f;
						}
					}
				}
			}

			//裏焼きするかどうかの判断
			if (velocityX < 0)
			{
				reverseDrawFlag = true;
			}
			if (velocityX > 0)
			{
				reverseDrawFlag = false;
			}

			//次フレームのため、このフレームでの真偽を格納
			prevJumpCmd = jumpCmd;
		}
	}
}

void Player::KickDirectionControl(XINPUT_STATE *pad)
{
	if (pad->Buttons[XINPUT_BUTTON_RIGHT_THUMB])
	{
		WaitTimer(0);
	}
	
	if (CheckHitKey(KEY_INPUT_LEFT) || pad->Buttons[XINPUT_BUTTON_DPAD_LEFT] || (ABUSOLUTE_VALUE(pad->ThumbLY) > PAD_STICK_DEADZONE && pad->ThumbLX < 0) || pad->ThumbLX < -PAD_STICK_DEADZONE)
	{
		reverseDrawFlag = true;
	}
	/*
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		reverseDrawFlag = true;
	}
	if (pad->Buttons[XINPUT_BUTTON_DPAD_LEFT])
	{
		reverseDrawFlag = true;
	}
	if ((ABUSOLUTE_VALUE(pad->ThumbLY) > PAD_STICK_DEADZONE && pad->ThumbLX < 0))
	{
		reverseDrawFlag = true;
	}
	if (pad->ThumbLX < -PAD_STICK_DEADZONE)
	{
		reverseDrawFlag = true;
	}
	*/

	if (CheckHitKey(KEY_INPUT_RIGHT) || pad->Buttons[XINPUT_BUTTON_DPAD_RIGHT] || (ABUSOLUTE_VALUE(pad->ThumbLY) > PAD_STICK_DEADZONE && pad->ThumbLX > 0) || pad->ThumbLX > PAD_STICK_DEADZONE)
	{
		reverseDrawFlag = false;
	}
	/*
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		reverseDrawFlag = false;
	}
	if (pad->Buttons[XINPUT_BUTTON_DPAD_RIGHT])
	{
		reverseDrawFlag = false;
	}
	if (ABUSOLUTE_VALUE(pad->ThumbLY) > PAD_STICK_DEADZONE && pad->ThumbLX > 0)
	{
		reverseDrawFlag = false;
	}
	if (pad->ThumbLX > PAD_STICK_DEADZONE)
	{
		reverseDrawFlag = false;
	}
	*/

	//キーボード、パッドのボタンによる操作
	{
		if ((CheckHitKey(KEY_INPUT_DOWN) && !CheckHitKey(KEY_INPUT_UP)) || pad->Buttons[XINPUT_BUTTON_DPAD_DOWN])
		{
			kickAngle += DX_PI_F / 180;
			if (kickAngle >= DX_PI_F / 2)
			{
				kickAngle -= DX_PI_F / 180;
			}
		}
		if (CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN) || pad->Buttons[XINPUT_BUTTON_DPAD_UP])
		{
			kickAngle -= DX_PI_F / 180;
			if (kickAngle <= DX_PI_F / 6)
			{
				kickAngle += DX_PI_F / 180;
			}
		}
	}

	//パッドのスティックによる操作
	if ((pad->ThumbLX > PAD_STICK_DEADZONE || pad->ThumbLX < -PAD_STICK_DEADZONE) ||
		(pad->ThumbLY > PAD_STICK_DEADZONE || pad->ThumbLY < -PAD_STICK_DEADZONE))
	{
		double cosine;

		double stickVectLen = GET_VECTOR_LENGTH((double)pad->ThumbLX, (double)pad->ThumbLY);

		//int baseX = (reverseDrawFlag) ? -32767 : 32767;
		int baseX = 32767;

		cosine = INNER_PRODUCT((double)pad->ThumbLX, (double)pad->ThumbLY, baseX, 0) / (stickVectLen * baseX);

		//角度を取得。
		kickAngle = acosf((float)cosine);

		if (kickAngle > DX_PI_F / 2)
		{
			reverseDrawFlag = true;
			kickAngle = -kickAngle + DX_PI_F;
		}

		//30°未満ないしは、スティックが上方向に入力されている時、角度を30°に
		if ((kickAngle < DX_PI_F / 6) || pad->ThumbLY >= 0)
		{
			kickAngle = DX_PI_F / 6;
		}
	}

	kickVectorX = cosf(kickAngle) * KICK_SPEED_X;
	kickVectorY = sinf(kickAngle) * KICK_SPEED_Y;
}

void Player::IncreaseKickDistance(Effect *effect)
{
	if (kickFlag[1])
	{
		bool prevFire = burningFlag;

		kickDistance += ABUSOLUTE_VALUE(velocityY);
		
		if (prevFire)
		{
			return;
		}

		if (kickDistance > effect->METEO_EFFECT_START_DISTANCE)
		{
			burningFlag = true;
		}

		if (!prevFire && burningFlag)
		{
			PlaySoundMem(meteoSound, DX_PLAYTYPE_BACK);
		}
	}
}

void Player::UpdateMoveX()
{
	//速度を実際にxに加算する
	if (kickFlag[0])
	{
		x += velocityX / SLOWMOTION_RATE;
	}
	else
	{
		x += velocityX;
	}
}

void Player::UpdateMoveY()
{
	//y座標にy速度を加算
	if (kickFlag[0])
	{
		//y += velocityY / SLOWMOTION_RATE;
		y += velocityY / powf(SLOWMOTION_RATE, 0.5f);
	}
	else
	{
		y += velocityY;
	}
}

void Player::Bound(float GroundY)
{
	if (kickFlag[0])
	{
		kickFlag[0] = false;
	}
	if (kickFlag[1])
	{
		kickFlag[1] = false;
	}
	jumpFlag = false;
	if (graphNum4anim == 0)
	{
		//x,y速度の記録
		MemoryVelocityXY();

		velocityY = 0.0f;
		graphNum4anim = 1;
	}
	else if (graphNum4anim == 1)
	{
		velocityY = 0.0f;
		graphNum4anim = 2;
	}
	else
	{
		y = GroundY - h;
		velocityX = tmpVelocityX;
		velocityY = (jumpCmd) ? -JUMP_POWER : -USUAL_JUMP_POWER;
		jumpCmd = false;
		jumpFlag = true;
		PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
		graphNum4anim = 0;
	}
}

void Player::ReboundAgainstWall(float WallEdgeX, bool checkLeft)
{
	int frame = 3;

	if (checkLeft)
	{
		if ((velocityX > ACCELERATION_X * frame) || (velocityX < -ACCELERATION_X * frame))
		{
			x = WallEdgeX;
			velocityX *= WALL_REBOUND_RATE;
			reboundFlag = true;
			reboundStartTime = GetNowCount();
		}
		else
		{
			/*y -= 1 / (2 * h / 2);
			x = WallEdgeX - (1 - (1 / (2 * w / 2)));
			velocityX = MAX_SPEED_X * 1.5f;
			velocityY = -USUAL_JUMP_POWER;
			jumpFlag = true;
			reboundFlag = true;
			reboundStartTime = GetNowCount();
			graphNum4anim = 0;*/
			OddBound(checkLeft, WallEdgeX, false);
		}
	}
	else
	{
		if ((velocityX > ACCELERATION_X * frame) || (velocityX < -ACCELERATION_X * frame))
		{
			x = WallEdgeX - w;
			velocityX *= WALL_REBOUND_RATE;
			reboundFlag = true;
			reboundStartTime = GetNowCount();
		}
		else
		{
			/*velocityX = -MAX_SPEED_X * 1.5f;
			velocityY = -USUAL_JUMP_POWER;
			jumpFlag = true;
			reboundFlag = true;
			reboundStartTime = GetNowCount();
			graphNum4anim = 0;*/
			OddBound(checkLeft, WallEdgeX, false);
		}
	}

	if (kickFlag[1])
	{
		ReverseFlag(&reverseDrawFlag);
	}
}

void Player::OddBound(bool boundToRight, float WallEdgeX, bool damageType)
{
	if (boundToRight)
	{
		y -= 1 / (2 * h / 2);
		x = WallEdgeX - (1 - (1 / (2 * w / 2)));
		velocityX = MAX_SPEED_X * 1.5f;
	}
	else
	{
		velocityX = -MAX_SPEED_X * 1.5f;
	}
	velocityY = -USUAL_JUMP_POWER;
	jumpFlag = true;
	if (kickFlag[0])
	{
		kickFlag[0] = false;
	}
	if (kickFlag[1])
	{
		kickFlag[1] = false;
	}
	//ダメージタイプならダメージフラグを、そうでないならリバウンドフラグをtrueに
	((damageType) ? damageFlag : reboundFlag) = true;
	//ダメージタイプならダメージを受けた時間を、そうでないならリバウンドを始めた時間を記録
	((damageType) ? damageTime : reboundStartTime) = GetNowCount();
	graphNum4anim = 0;
}

void Player::HitCeil(float ceilEdge)
{
	y = ceilEdge;
	if (velocityY < 0)
	{
		velocityY = 0;
	}
}

void Player::Fall(Game *game)
{
    graphNum4anim = 0;
    jumpFlag = true;
	if (kickFlag[0])
    {
		//velocityY += game->GRAVITY / SLOWMOTION_RATE;
		velocityY += game->GRAVITY / powf(SLOWMOTION_RATE, 0.5f);
	}
	else
	{
		velocityY += game->GRAVITY;
	}
}

void Player::Damage(bool leftsideOfEnemy, Effect *effect)
{
	hitPoint--;
	effect->SetDamageEffect(this);

	PlaySoundMem(damageSound, DX_PLAYTYPE_BACK);

	OddBound(leftsideOfEnemy, x, true);
}

void Player::DamageFromBoss(float partsX, Effect * effect)
{
	if (kickFlag[1])
	{
		kickFlag[1] = false;
	}
	
	x = partsX - w;
	velocityX = -MAX_SPEED_X * 1.5f;
	velocityY = -USUAL_JUMP_POWER;
	jumpFlag = true;

	graphNum4anim = 0;

	hitPoint--;
	effect->SetDamageEffect(this);

	PlaySoundMem(damageSound, DX_PLAYTYPE_BACK);

	damageFlag = true;
	damageTime = GetNowCount();
}

void Player::Draw(Effect *effect, Camera *camera, Game *game)
{
	if (!damageFlag || (damageFlag && !(((game->nowTime - damageTime) / 40) % 2)))
	{
		if (!reverseDrawFlag)
		{
			DrawExtendGraphF(x - camera->cameraOffsetX, y - camera->cameraOffsetY, x + w - camera->cameraOffsetX, y + h - camera->cameraOffsetY, graph[graphNum4anim], TRUE);
		}
		else
		{
			DrawExtendGraphF(x + w - camera->cameraOffsetX, y - camera->cameraOffsetY, x - camera->cameraOffsetX, y + h - camera->cameraOffsetY, graph[graphNum4anim], TRUE);
		}
	}

	if (kickFlag[0])
	{
		DrawRotaGraph2F((x + (w / 2) - camera->cameraOffsetX), (y + h / 2 - camera->cameraOffsetY), -w / 2, arrowGraphH / 2, 1.0f,
			(reverseDrawFlag) ? -kickAngle + DX_PI_F : kickAngle, arrowGraph, TRUE);
	}

	if (damageFlag)
	{
		effect->DrawDamageEffect(this, camera);
	}
}

void Player::Draw4Title(Camera * camera)
{
	DrawExtendGraphF(x - camera->cameraOffsetX, y - camera->cameraOffsetY, x + w - camera->cameraOffsetX, y + h - camera->cameraOffsetY, graph[graphNum4anim], TRUE);
}

void Player::DrawBurningBonus(Game *game, Camera * camera)
{
}

void Player::MemoryVelocityXY()
{
	tmpVelocityX = velocityX;
	tmpVelocityY = velocityY;
}

void Player::ResetKickDistance()
{
	kickDistance = 0.0f;
	burningFlag = false;
}

void Player::GameOver(Effect * effect)
{
	
}
