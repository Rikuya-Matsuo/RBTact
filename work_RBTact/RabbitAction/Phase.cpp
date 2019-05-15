#include "Phase.h"

void Phase::InitGameState(StateFlags *stateFlags)
{
	for (int i = 0; i < INVALID_GameState; i++)
	{
		SetBoolArray(stateFlags->state[i], PREPARE_SEQUENCE_NUMBER, false);
	}
}

bool Title::Init(int replayCnt, Player *player, Camera *camera, Map *map, Sky *sky)
{
	bool ret;

	player->InitForTitle();
	camera->InitForTitle();
	map->LoadChipGraph(replayCnt);
	sky->Load(replayCnt);

	if (!replayCnt)
	{
		graph = LoadGraph("image/title.png");
		how2goNext = LoadGraph("image/pushStart.png");
		bgm = LoadBGM("sound/bgm/LittleBravers.mp3");
		sceneChangeSound = LoadSoundMem("sound/se/decision22.mp3");
		demoMovie = LoadGraph("movie/demo00.mp4");
		ChangeVolumeSoundMem(BGM_VOLUME, bgm);
		ChangeMovieVolumeToGraph(0, demoMovie);

		creditBGM = LoadGraph("image/creditBgm.png");
		creditSE = LoadGraph("image/creditSE.png");
	}

	GetGraphSizeF(graph, &w, &h);
	w *= TITLE_GRAPH_EXTEND_RATE;
	h *= TITLE_GRAPH_EXTEND_RATE;

	alphaRate = 0;
	alphaIncrease = true;

	x = 0.0f;
	y = (float)(WINDOW_HEIGHT - h) / 2.0f - 125.0f;

	rRate = 1.0f;
	gRate = 0.0f;
	bRate = 0.0f;
	colorChangeState = R2B;
	ret = true;

	titleStartTime = GetNowCount();
	moviePlayFlag = false;

	SeekMovieToGraph(demoMovie, 0);

	blackOutRate = 0;

	if (graph < 0)
	{
		ret = false;
	}

	return ret;
}

void Title::Update(HitChecker * hitChk, Player * player, Map * map, Game * game)
{
	nowTime = GetNowCount();
	moviePlayFlag = (nowTime - titleStartTime) > MOVIE_START_COUNT;

	if (!CheckSoundMem(bgm))
	{
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	}
	
	if (moviePlayFlag)
	{
		PlayMovieToGraph(demoMovie, DX_PLAYTYPE_BACK);
	}
	else
	{
		hitChk->CheckHitPlayerGround(player, map, game);

		player->UpdateMoveY();
	}
}

void Title::Draw(Player *player, Map *map, Sky *sky, Camera *camera, Result *result)
{
	ClearDrawScreen();

	if (moviePlayFlag)
	{
		DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, demoMovie, FALSE);

		if (TellMovieToGraph(demoMovie) > 101500)
		{
			blackOutRate += BLACK_OUT_INCREASE;

			if (blackOutRate > 255)
			{
				blackOutRate = 255;
			}

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackOutRate);

			DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(0,0,0), TRUE);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			if (blackOutRate >= 255)
			{
				ChangeDemo2Title(player);
			}
		}
	}
	else
	{
		//空、マップ、プレイヤーの描画
		sky->Draw(camera);
		map->Draw(camera);
		player->Draw4Title(camera);
		DrawExtendGraphF(x, y, x + w, y + h, graph, TRUE);

		//ランキング一位の色更新
		{
			if (colorChangeState == R2B)
			{
				rRate -= COLOR_RATE_INCREASE;
				bRate += COLOR_RATE_INCREASE;
				if (bRate > 1.0f)
				{
					rRate = 0.0f;
					bRate = 1.0f;
					colorChangeState++;
				}
			}
			else if (colorChangeState == B2G)
			{
				bRate -= COLOR_RATE_INCREASE;
				gRate += COLOR_RATE_INCREASE;
				if (gRate > 1.0f)
				{
					bRate = 0.0f;
					gRate = 1.0f;
					colorChangeState++;
				}
			}
			else if (colorChangeState == G2R)
			{
				gRate -= COLOR_RATE_INCREASE;
				rRate += COLOR_RATE_INCREASE;
				if (rRate > 1.0f)
				{
					gRate = 0.0f;
					rRate = 1.0f;
					colorChangeState++;
				}
			}

			if (colorChangeState >= INVALID_ColorState)
			{
				colorChangeState = R2B;
			}
		}

		//ランキングの描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

		DrawBoxAA(WINDOW_WIDTH - 550, 50, WINDOW_WIDTH - 10, 650, GetColor(255, 255, 255), TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawFormatString(WINDOW_WIDTH - 550, 80, GetColor(0, 0, 0), "RANKING!");
		for (int i = 0; i < result->RANKING_MASS; i++)
		{
			if (!i && result->ranking[i])
			{
				int R = (int)(COLOR_PRAM_MAX * rRate);
				int G = (int)(COLOR_PRAM_MAX * gRate);
				int B = (int)(COLOR_PRAM_MAX * bRate);
				DrawFormatString(WINDOW_WIDTH - 550, 80 + GetFontSize() * (i + 1) + 30, GetColor(R, G, B), "No.%d %d", i + 1, result->ranking[i]);
			}
			else
			{
				DrawFormatString(WINDOW_WIDTH - 550, 80 + GetFontSize() * (i + 1) + 30, GetColor(0, 0, 0), "No.%d %d", i + 1, result->ranking[i]);
			}
		}

		//クレジット表記
		//漢字表記不可のため、画像表示で行う必要がある
		{
			int creditW, creditH;
			GetGraphSize(creditSE, &creditW, &creditH);

			DrawGraph(WINDOW_WIDTH - creditW, WINDOW_HEIGHT - creditH * 2, creditBGM, TRUE);
			DrawGraph(WINDOW_WIDTH - creditW, WINDOW_HEIGHT - creditH, creditSE, TRUE);
		}
	}

	//Push Startの点滅（？）描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaRate);
	DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 250, 1, 0, how2goNext, TRUE);
	(alphaIncrease) ? alphaRate += 5 : alphaRate -= 5;
	if (alphaRate > 255 || alphaRate < 0)
	{
		ReverseFlag(&alphaIncrease);
		alphaRate = (alphaRate > 255) ? 255 : 0;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ScreenFlip();
}

void Title::ChangeDemo2Title(Player *player)
{
	//Push Startの点滅をリセット
	alphaIncrease = true;
	alphaRate = 0;

	//タイトル画面表示時間を取得しなおす
	titleStartTime = GetNowCount();

	//デモムービーの再生フラグをfalseに
	moviePlayFlag = false;

	//ムービーを巻き戻す
	SeekMovieToGraph(demoMovie, 0);

	//ブラックアウトの倍率を０に戻す
	blackOutRate = 0;

	//プレイヤーの状態を初期化
	player->InitForTitle();

	//デモムービーのデータを読み直す
	ReloadDemo();
}

void Title::ReloadDemo()
{
	DeleteGraph(demoMovie);
	demoMovie = LoadGraph("movie/demo00.mp4");
}

bool Rule::Init(int replayCnt)
{
	bool ret;

	if (!replayCnt)
	{
		sceneChangeSound = LoadSoundMem("sound/se/decision22.mp3");
		for (int i = 0; i < ITEM_KIND; i++)
		{
			//アイテム画像読み込み
			char fileName[64];
			if (!i)
			{
				strcpy_s(fileName, 64, "image/heart01.png");
			}
			else if (i == 1)
			{
				strcpy_s(fileName, 64, "image/crown00.png");
			}
			else
			{
				strcpy_s(fileName, 64, "image/coin.png");
			}
			itemGraphes[i] = LoadGraph(fileName);
		}
		
		//動画読み込み
		for (int i = 0; i < MOVIE_MASS; i++)
		{
			char fileName[64];
			//ついでに再生開始位置記録
			int seekTime = 0;
			if (!i)
			{
				strcpy_s(fileName, 64, "movie/move01.mp4");
				movieX[i] = WINDOW_WIDTH / 3 - MOVIE_WIDTH / 2;
				movieSeekTime[i] = 1000;
				movieEndPoint[i] = 5500;
			}
			else if (i == 1)
			{
				strcpy_s(fileName, 64, "movie/jump01.mp4");
				movieX[i] = (WINDOW_WIDTH / 3) * 2 - MOVIE_WIDTH / 2;
				movieSeekTime[i] = 3000;
				movieEndPoint[i] = 8000;
			}
			else if (i == 2)
			{
				strcpy_s(fileName, 64, "movie/kickPrepare01.mp4");
				movieX[i] = (WINDOW_WIDTH / 6) * (1 + (i - 2) * 2) - MOVIE_WIDTH / 2;
				//(i - 2)とは、下の段では何番目の映像化を算出する式。(0から数えて)
				movieSeekTime[i] = 2500;
				movieEndPoint[i] = 6500;
			}
			else if (i == 3)
			{
				strcpy_s(fileName, 64, "movie/kickDirection01.mp4");
				movieX[i] = (WINDOW_WIDTH / 6) * (1 + (i - 2) * 2) - MOVIE_WIDTH / 2;
				movieSeekTime[i] = 2500;
				movieEndPoint[i] = 6500;
			}
			else if (i == 4)
			{
				strcpy_s(fileName, 64, "movie/kickExecution01.mp4");
				movieX[i] = (WINDOW_WIDTH / 6) * (1 + (i - 2) * 2) - MOVIE_WIDTH / 2;
				movieSeekTime[i] = 3000;
				movieEndPoint[i] = 7500;
			}
			movieHandle[i] = LoadGraph(fileName);
			SetMovieVolumeToGraph(0, movieHandle[i]);
			SeekMovieToGraph(movieHandle[i], movieSeekTime[i]);
		}

		//ボタン類画像読み込み
		lStickGraph = LoadGraph("image/LStick.png");
		arrowGraph = LoadGraph("image/blackArrow.png");
		rTriggerGraph = LoadGraph("image/RTrigger.png");
		aButtonGraph = LoadGraph("image/AButton.png");
	}

	for (int i = 0; i < ITEM_MASS / ITEM_KIND; i++)
	{
		for (int j = 0; j < INVALID_ItemKind; j++)
		{
			itemX[j][i] = (j + (i * ITEM_KIND)) * ITEMS_MARGIN;
		}
	}

	ret = true;

	return ret;
}

void Rule::Update(Sky *sky)
{
	ClearDrawScreen();

	sky->Draw4Rule();

	UpdateItems();

	DrawItems();

	DrawMovieAndIcon();

	//ScreenFlip();
}

void Rule::UpdateItems()
{
	for (int i = 0; i < ITEM_MASS / ITEM_KIND; i++)
	{
		for (int j = 0; j < INVALID_ItemKind; j++)
		{
			itemX[j][i] += ITEMS_SPEED;
			//その個体が画面を超えたら、個体の右端が画面の左端と重なるように戻す
			if (itemX[j][i] > WINDOW_WIDTH)
			{
				itemX[j][i] = (float)-ITEMS_SIZE;
			}
		}
	}
}

void Rule::DrawItems()
{
	for (int i = 0; i < ITEM_MASS / ITEM_KIND; i++)
	{
		for (int j = 0; j < INVALID_ItemKind; j++)
		{
			DrawExtendGraphF(itemX[j][i], ITEMS_Y, itemX[j][i] + ITEMS_SIZE, ITEMS_Y + ITEMS_SIZE, itemGraphes[j], TRUE);
		}
	}
}

void Rule::DrawMovieAndIcon()
{
	for (int i = 0; i < MOVIE_MASS; i++)
	{
		//ムービーの更新
		PlayMovieToGraph(movieHandle[i], DX_PLAYTYPE_BACK);
		
		//ムービーY座標の確定
		int movieY;
		if (i < 2)
		{
			movieY = MOVIE_Y_UPPER;
		}
		else
		{
			movieY = MOVIE_Y_DOWNER;
		}
		//ムービー描画
		DrawExtendGraph(movieX[i], movieY, movieX[i] + MOVIE_WIDTH, movieY + MOVIE_HEIGHT,
						movieHandle[i], FALSE);

		//ムービーに即したボタン画像の描画
		if (!i)
		{
			int stickPosX = (movieX[i] + MOVIE_WIDTH / 2) - BUTTON_SIZE / 2;
			DrawExtendGraph(stickPosX, movieY - BUTTON_SIZE, stickPosX + BUTTON_SIZE, movieY, lStickGraph, TRUE);
			DrawExtendGraph(stickPosX, movieY - BUTTON_SIZE, stickPosX - BUTTON_SIZE, movieY, arrowGraph, TRUE);
			DrawExtendGraph(stickPosX + BUTTON_SIZE, movieY - BUTTON_SIZE, stickPosX + BUTTON_SIZE * 2, movieY, arrowGraph, TRUE);
		}
		else if (i == 1)
		{
			int aX = (movieX[i] + MOVIE_WIDTH / 2) - BUTTON_SIZE / 2;
			DrawExtendGraph(aX, movieY - BUTTON_SIZE, aX + BUTTON_SIZE, movieY, aButtonGraph, TRUE);
		}
		else if (i == 2 || i == 4)
		{
			int RTX = (movieX[i] + MOVIE_WIDTH / 2) - BUTTON_SIZE / 2;
			DrawExtendGraph(RTX, movieY - BUTTON_SIZE, RTX + BUTTON_SIZE, movieY, rTriggerGraph, TRUE);
		}
		else if (i == 3)
		{
			int stickPosX = (movieX[i] + MOVIE_WIDTH / 2) - BUTTON_SIZE / 2;
			DrawExtendGraph(stickPosX, movieY - BUTTON_SIZE, stickPosX + BUTTON_SIZE, movieY, lStickGraph, TRUE);
		}

		//ムービーが特定の位置まで再生したとき、再開したいところまで巻き戻す
		if (TellMovieToGraph(movieHandle[i]) > movieEndPoint[i])
		{
			SeekMovieToGraph(movieHandle[i], movieSeekTime[i]);
		}
	}

	//キックの流れの矢印描画
	for (int i = 0; i < 2; i++)
	{
		int x = (WINDOW_WIDTH / 3) * (i + 1) - BUTTON_SIZE / 2;
		DrawExtendGraph(x, MOVIE_Y_DOWNER - BUTTON_SIZE, x + BUTTON_SIZE, MOVIE_Y_DOWNER, arrowGraph, TRUE);
	}
}

bool Game::Init(int replayCnt)
{
	bool ret = true;

	if (!replayCnt)
	{
		normalBgm = LoadBGM("sound/bgm/BeHappy.mp3");
		bossBgm = LoadBGM("sound/bgm/Kimihamou.mp3");
		blackBandGraph = LoadGraph("effectImg/black32x32.png");
		ChangeVolumeSoundMem(BGM_VOLUME, normalBgm);
		ChangeVolumeSoundMem(BGM_VOLUME, bossBgm);
	}

	blackBandUpper = 0 - BLACK_BAND_H;
	blackBandDowner = WINDOW_HEIGHT;

	gameOver = false;
	gameClear = false;
	bossTimeFlag = false;
	bossEventFlag = false;
	stopUpdate = false;
	gameStartTime = GetNowCount();

	return ret;
}

void Game::Update(Camera *camera)
{
	//BGM関連
	{
		if (!CheckSoundMem(normalBgm) && !CheckSoundMem(bossBgm))
		{
			if (!bossTimeFlag)
			{
				PlaySoundMem(normalBgm, DX_PLAYTYPE_LOOP);
			}
			else
			{
				PlaySoundMem(bossBgm, DX_PLAYTYPE_LOOP);
			}
		}
		//ノーマルのBGMが特定の時点で最初に戻る（ループ）
		if (!bossTimeFlag && GetSoundCurrentTime(normalBgm) >= 117250)
		{
			StopSoundMem(normalBgm);
			SetSoundCurrentTime(400, normalBgm);
			PlaySoundMem(normalBgm, DX_PLAYTYPE_LOOP, FALSE);
		}
	}

	ClearDrawScreen();

	nowTime = GetNowCount();
}

void Game::InitBossEvent(Map *map, Camera *camera)
{
	gateBlockX = 280 * map->MAPCHIP_SIZE_W;
	gateBlockY = map->MAP_HIGHEST;
	gateBlockFallSpeed = 0.0f;
	StopSoundMem(normalBgm);
	bossRoomEnterTime = 0;
	gateBlockFallen = false;
	bossAppear = false;
	bossTimeFlag = true;
	bossEventFlag = true;
	int i;
	//ボス部屋前のブロックの一部を消去する。
	for (i = 0; i < BOSSROOM_GATE_BLOCK_HEIGHT; i++)
	{
		map->mapPattern[i][280] = -1;
	}
}

void Game::UpdateBossEvent(Player *player, Boss *boss, Map *map, Camera * camera)
{
	//ボス部屋まで自動で進む
	if (player->x + player->w < 28600)
	{
		player->velocityX = 0.0f;
		player->x += player->MAX_SPEED_X;
	}
	//カメラの調整
	else if (camera->cameraOffsetX < camera->BOSS_TIME_CAMERA_OFFSET_X)
	{
		camera->cameraOffsetX += player->MAX_SPEED_X;
		if (!(camera->cameraOffsetX < camera->BOSS_TIME_CAMERA_OFFSET_X))
		{
			camera->cameraOffsetX = camera->BOSS_TIME_CAMERA_OFFSET_X;
		}
	}
	//タイマーを起動
	else if (!bossRoomEnterTime)
	{
		bossRoomEnterTime = GetNowCount();
	}
	//タイマーが一定値以上になったらプレイヤーをボス部屋に隔離
	else if ((nowTime - bossRoomEnterTime) >= ISOLATE_COUNT && !gateBlockFallen)
	{
		gateBlockFallSpeed += GRAVITY;
		gateBlockY += gateBlockFallSpeed;

		//隔離用ブロックが地面に落ちたらフラグを立てる
		if (gateBlockY + BOSSROOM_GATE_BLOCK_HEIGHT * map->MAPCHIP_SIZE_H >
			map->MAP_HIGHEST + (map->MAP_HEIGHT - map->GROUND_BLOCK_MASS) * map->MAPCHIP_SIZE_H)
		{
			gateBlockFallen = true;
			int i;
			for (i = 0; i < BOSSROOM_GATE_BLOCK_HEIGHT; i++)
			{
				map->mapPattern[i + 4][280] = 10;
			}
		}
	}
	else if ((nowTime - bossRoomEnterTime) >= BOSS_APPEAR_COUNT && !bossAppear)
	{
		boss->y += BOSS_FALL_SPEED;
		//着地後、ボス出現のフラグをtrueに
		if (boss->y + (boss->PARTS_SIZE_H * boss->GRAPH_EXTEND_RATE * boss->PARTS_MASS_Y) >
			map->MAP_HIGHEST + (map->MAP_HEIGHT - map->GROUND_BLOCK_MASS - boss->FLOAT_BLOCK_MASS) * map->MAPCHIP_SIZE_H)
		{
			boss->y = map->MAP_HIGHEST + (map->MAP_HEIGHT - map->GROUND_BLOCK_MASS - boss->FLOAT_BLOCK_MASS) * map->MAPCHIP_SIZE_H -
				(boss->PARTS_SIZE_H * boss->GRAPH_EXTEND_RATE * boss->PARTS_MASS_Y);
			bossAppear = true;
			boss->appearTime = GetNowCount();
		}
	}
	else if (gateBlockFallen && bossAppear)
	{
		//bossEventFlag = false;
	}

	if (bossEventFlag)
	{
		if (!bossAppear)
		{
			blackBandUpper += BAND_SPEED;
			if (blackBandUpper > 0)
			{
				blackBandUpper = 0;
			}

			blackBandDowner -= BAND_SPEED;
			if (blackBandDowner < WINDOW_HEIGHT - BLACK_BAND_H)
			{
				blackBandDowner = WINDOW_HEIGHT - BLACK_BAND_H;
			}
		}
		else
		{
			bool bandEndUp = false;
			blackBandUpper -= BAND_SPEED;
			if (blackBandUpper < -BLACK_BAND_H)
			{
				blackBandUpper = -BLACK_BAND_H;
				bandEndUp = true;
			}

			bool bandEndDown = false;
			blackBandDowner += BAND_SPEED;
			if (blackBandDowner > WINDOW_HEIGHT)
			{
				blackBandDowner = WINDOW_HEIGHT;
				bandEndDown = true;
			}

			if (bandEndUp && bandEndDown)
			{
				bossEventFlag = false;
			}
		}
	}
}

void Game::UpdateBossEventEnd()
{
	blackBandUpper -= BAND_SPEED;
	if (blackBandUpper < -BLACK_BAND_H)
	{
		blackBandUpper = -BLACK_BAND_H;
	}

	blackBandDowner += BAND_SPEED;
	if (blackBandDowner > WINDOW_HEIGHT)
	{
		blackBandDowner = WINDOW_HEIGHT;
	}
}

void Game::InitCloseBossArea(Map *map)
{
	for (int i = 0; i < 5; i++)
	{
		closeBlockX[i] = (map->MAP_WIDTH - 7 + i) * map->MAPCHIP_SIZE_W;
		closeBlockY[i] = map->MAP_HIGHEST - BOSSAREA_CLOSE_BLOCK_HEIGHT * map->MAPCHIP_SIZE_H;
		closeBlockFallSpeed[i] = 0.0f;
	}
	bossAreaClosed = false;
}

void Game::UpdateCloseBossArea(Map * map)
{
	if (!bossAreaClosed)
	{
		for (int i = 0; i < 5; i++)
		{
			closeBlockFallSpeed[i] += GRAVITY;
			closeBlockY[i] += closeBlockFallSpeed[i];
			if (closeBlockY[i] + BOSSAREA_CLOSE_BLOCK_HEIGHT * map->MAPCHIP_SIZE_H > map->MAP_HIGHEST + (map->MAP_HEIGHT - (map->GROUND_BLOCK_MASS + 3)) * map->MAPCHIP_SIZE_H)
			{
				closeBlockY[i] = ((map->MAP_HEIGHT - (map->GROUND_BLOCK_MASS + 3)) - BOSSAREA_CLOSE_BLOCK_HEIGHT) * map->MAPCHIP_SIZE_H;
				for (int j = 0; j < map->MAP_HEIGHT - (map->GROUND_BLOCK_MASS + 3); j++)
				{
					map->mapPattern[j][map->MAP_WIDTH - 7 + i] = 10;
					if (i == 4)
					{
						bossAreaClosed = true;
					}
				}
			}
		}
	}
}

void Game::DrawGateBlock(Map *map, Camera *camera)
{
	int i;
	for (i = 0; i < BOSSROOM_GATE_BLOCK_HEIGHT; i++)
	{
		DrawExtendGraphF(gateBlockX - camera->cameraOffsetX, gateBlockY + (i * map->MAPCHIP_SIZE_H) - camera->cameraOffsetY,
		gateBlockX + map->MAPCHIP_SIZE_W - camera->cameraOffsetX, gateBlockY + ((i + 1) * map->MAPCHIP_SIZE_H) - camera->cameraOffsetY, map->mapchipGraph[10], FALSE);
	}
}

void Game::DrawCloseBlock(Map * map, Camera * camera)
{
	if (!bossAreaClosed)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < BOSSAREA_CLOSE_BLOCK_HEIGHT; j++)
			{
				DrawExtendGraphF(closeBlockX[i] - camera->cameraOffsetX, closeBlockY[i] + j * map->MAPCHIP_SIZE_H - camera->cameraOffsetY,
							closeBlockX[i] + map->MAPCHIP_SIZE_W - camera->cameraOffsetX, closeBlockY[i] + (j + 1) * map->MAPCHIP_SIZE_H - camera->cameraOffsetY,
							map->mapchipGraph[10], FALSE);
			}
		}
	}
}

void Game::DrawEventBand()
{
	DrawExtendGraphF(0, blackBandUpper, (float)WINDOW_WIDTH, blackBandUpper + BLACK_BAND_H, blackBandGraph, FALSE);
	DrawExtendGraphF(0, blackBandDowner, (float)WINDOW_WIDTH, blackBandDowner + BLACK_BAND_H, blackBandGraph, FALSE);
}

void Game::SetResult(Result *result)
{
	if (gameOver)
	{
		strcpyDx(result->resultStr, "GAME OVER");
	}
	else
	{
		strcpyDx(result->resultStr, "GAME CLEAR");
	}
}

bool Result::Init(int replayCnt)
{
    bool ret;

	if (!replayCnt)
	{
		BGgraph = LoadGraph("image/resultBG00.jpg");
		shortBgm = LoadSoundMem("sound/bgm/Let's_Joy.mp3");
		ChangeVolumeSoundMem(BGM_VOLUME, shortBgm);
	}

	rankInNum = 0;

	alphaRate = 0;
	alphaIncrease = true;

	playBgm = false;

    ret = true;

    return ret;
}

void Result::Update(Player *player, Boss *boss, Effect *effect, Camera *camera, Game *game)
{
    ClearDrawScreen();

	if (!playBgm)
	{
		PlaySoundMem(shortBgm, DX_PLAYTYPE_BACK);
		playBgm = true;
	}

	if (game->gameClear)
	{
		DrawExtendGraph(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, BGgraph, FALSE);

		player->Draw(effect, camera, game);

		effect->DrawMeteoEffect(player, camera);
	}

	//DrawFormatString(WINDOW_WIDTH / 2 - GetDrawFormatStringWidth(resultStr) / 2, WINDOW_HEIGHT / 2 - 200, GetColor(255, 0, 0), resultStr);
	DrawFormatString(100, 300, GetColor(255, 0, 0), resultStr);

	//ランキング描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

	DrawBoxAA(WINDOW_WIDTH - 550, 50, WINDOW_WIDTH - 10, 650, GetColor(255, 255, 255), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(WINDOW_WIDTH - 500, 80, GetColor(0, 0, 0), "RANKING!");
	for (int i = 0; i < RANKING_MASS; i++)
	{
		unsigned int color;
		if (newRecord && i == rankInNum - 1)
		{
			color = GetColor(255, 0, 0);

			alphaRate += (alphaIncrease) ? ALPHA_INCREASE_MASS : -ALPHA_INCREASE_MASS;
			if (alphaRate > 255)
			{
				alphaRate = 255;
				alphaIncrease = false;
			}
			else if (alphaRate < 0)
			{
				alphaRate = 0;
				alphaIncrease = true;
			}
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaRate);
		}
		else
		{
			color = GetColor(0,0,0);
		}
		DrawFormatString(WINDOW_WIDTH - 550, 80 + GetFontSize() * (i + 1) + 30, color, "No.%d %d", i + 1, ranking[i]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (!boss->alive)
	{
		char text[256];
		sprintf_s(text, 256, "BOSS BUSTER BONUS! +%d", boss->BONUS_SCORE);
		DrawFormatString((WINDOW_WIDTH - GetDrawFormatStringWidth(text)) / 2, WINDOW_HEIGHT / 2 + 200, GetColor(255, 0,0), text);
	}

	if (newRecord)
	{
		DrawFormatString(WINDOW_WIDTH / 2 - GetDrawFormatStringWidth("NEW RECORD! You are No.%d", rankInNum) / 2, WINDOW_HEIGHT / 2 + 300, GetColor(255, 0, 0), "NEW RECORD! You are No.%d", rankInNum);
	}

    //ScreenFlip();
}

void Result::LoadScore()
{
	int i;

	FILE *r;
	fopen_s(&r, RANKING_FILE_NAME, "r");
	if (r == NULL)
	{
		for (i = 0; i < RANKING_MASS; i++)
		{
			ranking[i] = 0;
		}
	}

	char rankingStr[RANKING_MASS][RANKING_STR_MAX];
	i = 0;
	
	while (fgets(rankingStr[i], RANKING_STR_MAX, r) && i < RANKING_MASS)
	{
		if (r == NULL)
		{
			for (i; i < RANKING_MASS; i++)
			{
				ranking[i] = 0;
			}
			break;
		}
		ranking[i] = atoi(rankingStr[i]);
		i++;
	}

	fclose(r);
}

void Result::SaveScore(int score)
{
	newRecord = false;
	int i;

	for (i = 0; i < RANKING_MASS; i++)
	{
		if (score >= ranking[i])
		{
			int j;
			for (j = 0; i < RANKING_MASS - 1 - j; j++)
			{
				ranking[RANKING_MASS - 1 - j] = ranking[RANKING_MASS - 2 - j];
			}

			ranking[i] = score;
			newRecord = true;
			break;
		}
	}

	if (!newRecord)
	{
		return;
	}

	rankInNum = i + 1;
	FILE *w;
	fopen_s(&w, RANKING_FILE_NAME, "w");
	if (w == NULL)
	{
		printfDx("スコアのセーブに失敗");
		return;
	}

	for (i = 0; i < RANKING_MASS; i++)
	{
		fprintf(w, "%d\n", ranking[i]);
	}

	fclose(w);
}
