#define DEBUG

#include "Dxlib.h"
#include "Common.h"
#include "Phase.h"
#include "Camera.h"
#include "HitChecker.h"
#include "Map.h"
#include "Sky.h"
#include "Effect.h"
#include "Item.h"
#include "Coin.h"
#include "CoinGroup.h"
#include "CoinGroupManager.h"
#include "player.h"
#include "Enemy.h"
#include "EnemyControl.h"
#include "Boss.h"
#include "UI.h"
#include "Filter.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//画面モードの設定
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16);				//解像度を1920*1080、colorを16bitに
	ChangeWindowMode(TRUE);										//ウィンドウモードに

	//DXライブラリ初期化
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	//グラフィック描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//ウィンドウの名前
	SetMainWindowText("転生したらただの赤いボールだった件");

	//フォントの設定
	LPCSTR engFontPath = "Font/mini-wakuwaku.otf";
	if (!(AddFontResourceEx(engFontPath, FR_PRIVATE, NULL) > 0))
	{
		//フォント読み込みエラー処理
		return -1;
	}
	ChangeFont("mini-わくわく");

	//フォントサイズの設定
	SetFontSize(72);

	//乱数の初期値を設定
	{
		DATEDATA date;
		GetDateTime(&date);

		int seed = date.Day + date.Hour + date.Min + date.Sec;

		SRand(seed);

		//プレイ回数記録データに日付を記入
		FILE *a;
		fopen_s(&a, "resourceTxt/playMass.txt", "a");
		if (a == NULL)
		{
			fclose(a);
		}
		else
		{
			fprintf_s(a, "%d日\n", date.Day);

			fclose(a);
		}
	}

	//ジョイパッドの設定(XBOX360コントローラー限定)
	XINPUT_STATE pad;
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.35);

	ProgramControl programControl;
	StateFlags stateFlags;
	Camera camera;
	Phase *phase_p;
	phase_p = new Phase;
	Title *title_p;
	title_p = new Title;
	Rule *rule_p;
	rule_p = new Rule;
	Game *game_p;
	game_p = new Game;
    Result *result_p;
	result_p = new Result;
	HitChecker hitChk;
	Map map;
    Sky sky;
	UI ui;
	EnemyControl enemyCtrl;
	CoinGroupManager coinMaster;
	Item *item_p;
	item_p = new Item;
	Effect *effect_p;
	effect_p = new Effect;
	Player *player_p;
	player_p = new Player;
	Enemy *enemy_p[enemyCtrl.ENEMY_MASS];
	for (int i = 0; i < enemyCtrl.ENEMY_MASS; i++)
	{
		enemy_p[i] = new Enemy;
	}
	Boss *boss_p;
	boss_p = new Boss;
	Enemy *bossParts_p[boss_p->PARTS_MASS_ALL];
	for (int i = 0; i < boss_p->PARTS_MASS_ALL; i++)
	{
		bossParts_p[i] = new Enemy;
	}
	Filter filter;

	//ゲームの進行状態をすべてfalseにした後、タイトル画面の準備をtrueに
	phase_p->InitGameState(&stateFlags);
	stateFlags.state[TITLE][0] = true;

	//タイトルでハイスコアランキングを表示するため、ランキングデータを読み込む
	result_p->LoadScore();

	//---------------------------------//
	//ゲームループ
	//---------------------------------//
	while (true)
	{
		//パッド情報の取得
		GetJoypadXInputState(DX_INPUT_PAD1, &pad);

		//----------------------------//
		// タイトル
		//----------------------------//
		if (stateFlags.state[TITLE][0] || stateFlags.state[TITLE][1])
		{
			//タイトルの初期化
			if (stateFlags.state[TITLE][0])
			{
				if (title_p->Init(programControl.replayCnt, player_p, &camera, &map, &sky) && map.LoadPattern("map/map01..csv"))
				{
					StopSoundMem(result_p->shortBgm);
					SwitchFlag(&stateFlags.state[TITLE][0], &stateFlags.state[TITLE][1]);
				}
			}
			//アップデート
			if (stateFlags.state[TITLE][1])
			{
				if (programControl.CheckHitDebugCommand(&pad))
				{
					WaitTimer(0);
				}

				title_p->Update(&hitChk, player_p, &map, game_p);

				title_p->Draw(player_p, &map, &sky, &camera, result_p);

				if (programControl.CheckHitGoNext(&pad))
				{
					if (!programControl.prevEnter)
					{
						if (title_p->moviePlayFlag)
						{
							title_p->ChangeDemo2Title(player_p);
						}
						else
						{
							SwitchFlag(&stateFlags.state[TITLE][1], &stateFlags.state[TITLE + 1][0]);
						}
						PlaySoundMem(title_p->sceneChangeSound, DX_PLAYTYPE_BACK);
						programControl.prevEnter = true;
					}
				}
				else
				{
					programControl.prevEnter = false;
				}
			}
		}
		//----------------------------//
		// ルール説明
		//----------------------------//
		else if (stateFlags.state[RULE][0] || stateFlags.state[RULE][1])
		{
			if (stateFlags.state[RULE][0])
			{
				if (rule_p->Init(programControl.replayCnt))
				{
					SwitchFlag(&stateFlags.state[RULE][0], &stateFlags.state[RULE][1]);
				}
			}
			if (stateFlags.state[RULE][1])
			{
				rule_p->Update(&sky);

				DrawHow2GoNext(title_p->how2goNext);

				ScreenFlip();

				if (programControl.CheckHitGoNext(&pad))
				{
					if (!programControl.prevEnter)
					{
						programControl.prevEnter = true;
						PlaySoundMem(rule_p->sceneChangeSound, DX_PLAYTYPE_BACK);
						StopSoundMem(title_p->bgm);
						WaitTimer(1000);
						SwitchFlag(&stateFlags.state[RULE][1], &stateFlags.state[RULE + 1][0]);
					}
				}
				else
				{
					programControl.prevEnter = false;
				}
			}
		}
		//----------------------------//
		// ゲーム画面
		//----------------------------//
		else if (stateFlags.state[GAME][0] || stateFlags.state[GAME][1])
		{
			if (stateFlags.state[GAME][0])
			{
				//各種初期化
				if (game_p->Init(programControl.replayCnt))
				{
					map.LoadChipGraph(programControl.replayCnt);
                    sky.Load(programControl.replayCnt);
					effect_p->InitKickEffect(programControl.replayCnt);
					effect_p->InitMeteoEffect(programControl.replayCnt);
					effect_p->InitStarEffect(programControl.replayCnt);
					effect_p->InitSmokeEffect(programControl.replayCnt);
					effect_p->InitDamageEffect(programControl.replayCnt);
					effect_p->InitPlayerDown();
					effect_p->InitCoinEffect(programControl.replayCnt);
					effect_p->InitGameClear();
					item_p->Init(&map);
					coinMaster.Init();
					player_p->Init(programControl.replayCnt, game_p);
					camera.Init();
					enemyCtrl.Init(enemy_p);
					boss_p->Init(&map, programControl.replayCnt);
					for (int i = 0; i < boss_p->PARTS_MASS_ALL; i++)
					{
						bossParts_p[i]->InitBossType(i, bossParts_p[0]->graph, boss_p, &enemyCtrl, &map);
					}
					filter.Init();
					SwitchFlag(&stateFlags.state[GAME][0], &stateFlags.state[GAME][1]);
				}
			}
			if (stateFlags.state[GAME][1])
			{
				game_p->Update(&camera);
				bool contactPE = false;

				//操作の受付
				if (player_p->aliveFlag && !game_p->stopUpdate)
				{
					player_p->UpdateControl(&pad, effect_p, game_p, &programControl);
				}

				//プレイヤーとブロックとの当たり判定
				if (player_p->aliveFlag && !game_p->stopUpdate)
				{
					hitChk.CheckHitPlayerBlock(player_p, &map, game_p);
				}

				//アイテム情報の更新
				item_p->Update(game_p);

				//プレイヤーとアイテムの当たり判定
				hitChk.CheckHitPlayerItem(player_p, item_p);

				//コイン情報の更新
				coinMaster.Update(&camera);

				//プレイヤーとコインの当たり判定
				hitChk.CheckHitPlayerCoin(player_p, &coinMaster, effect_p);

				//コインエフェクトの更新
				if (effect_p->coinEffTimeFlag)
				{
					effect_p->UpdateCoinEffect();
				}

				//エネミーとブロックとの当たり判定
				{
					//通常タイプエネミー
					for (int i = 0; i < enemyCtrl.ENEMY_MASS; i++)
					{
						hitChk.CheckHitEnemyBlock(enemy_p[i], &enemyCtrl, &map);
					}

					//ボスタイプエネミー
					for (int i = 0; i < boss_p->PARTS_MASS_ALL; i++)
					{
						if ((bossParts_p[i]->CheckInsideScreen(&camera, &map) && !boss_p->partsSplitFlag[i]) ||
						boss_p->partsSplitFlag[i])
						{
							hitChk.CheckHitEnemyBlock(bossParts_p[i], &enemyCtrl, &map);
						}
					}
				}

				//プレイヤーとエネミーとの当たり判定
				if (player_p->aliveFlag)
				{
					//通常型エネミー
					contactPE = hitChk.CheckHitPlayerEnemy(player_p, enemy_p, boss_p, effect_p, &camera, &map, &enemyCtrl, enemyCtrl.ENEMY_MASS);

					//ボスタイプエネミー
					if (!contactPE)
					{
						contactPE = hitChk.CheckHitPlayerEnemy(player_p, bossParts_p, boss_p, effect_p, &camera, &map, &enemyCtrl, boss_p->PARTS_MASS_ALL);
					}
				}

				//ボスの更新
				if (!game_p->bossEventFlag && game_p->bossTimeFlag)
				{
					boss_p->Update(bossParts_p, game_p, &map);
				}

				//ボスが全てのエネミーを排出した後、ボス用の舞台にプレイヤーを入れなくする。
				hitChk.CheckHitPlayerBoss(player_p, boss_p);

				if (!game_p->stopUpdate)
				{
					//エネミーの更新
					for (int i = 0; i < enemyCtrl.ENEMY_MASS; i++)
					{
						enemy_p[i]->Update(player_p, &enemyCtrl, effect_p, game_p);
					}

					//ボスタイプのエネミーの更新
					if (game_p->bossTimeFlag)
					{
						//適当に0-->あとでEnemyControlクラスに移すべきかも
						enemy_p[0]->UpdateBossType(bossParts_p, boss_p, player_p, effect_p, game_p);
					}
				}

				//撃破されたエネミーの煙エフェクトの更新
				effect_p->UpdateSmoke(&enemyCtrl);


#ifdef DEBUG
				//コマンドでブレイクポイントへ（デバッグ用）
				if (programControl.CheckHitDebugCommand(&pad))
				{
					WaitTimer(0);
				}

#endif DEBUG

				//バウンド処理中、何らかの原因で床がなくなり、そのまま上へ上って行ってしまう、
				//いわゆる昇天バグを防ぐため、床もエネミーも踏んでいないときには重力を有効化
				if (!player_p->hitGroundFlag && !contactPE)
				{
					player_p->graphNum4anim = 0;
				}

				//プレイヤーが変に深いところまで行ったとき、地上に戻す
				if (player_p->y > map.GROUND_Y)
				{
					player_p->y = map.GROUND_Y - player_p->h;

					player_p->Bound(map.GROUND_Y);
				}

                //カメラスクロール
                //キック準備中は実行しない
                if (!game_p->stopUpdate)
                {
                    camera.ScrollScreenX(player_p);
                    camera.ScrollScreenY(player_p);
                }

				//キック準備に入っていなければyを加算--->入っていれば加算しない
				if (player_p->aliveFlag && !game_p->stopUpdate)
				{
					//if (!player_p->kickFlag[0])
					{
						//キック実行中のとき、その距離を記録する関数
						player_p->IncreaseKickDistance(effect_p);
						//プレイヤーの実際の移動
						player_p->UpdateMoveY();
						player_p->UpdateMoveX();
					}
					//キック準備に入っていればプレイヤーの画像を空中のものにする
					if (player_p->kickFlag[0])
					{
						player_p->graphNum4anim = 0;
					}
				}

				//ボスイベント発動
				if (player_p->x + player_p->w > game_p->BOSS_BATTLE_LINE_X)
				{
					if (!game_p->bossTimeFlag && !game_p->bossEventFlag && !player_p->jumpFlag)
					{
						game_p->InitBossEvent(&map, &camera);
						//ついでにボスパーツ全排出後の閉鎖用ブロックも初期化(∀｀*ゞ)ﾃﾍｯ
						game_p->InitCloseBossArea(&map);
					}
					if (game_p->bossTimeFlag && game_p->bossEventFlag)
					{
						game_p->UpdateBossEvent(player_p, boss_p, &map, &camera);
					}
				}

				//ボスイベント終了処理
				if (!game_p->bossEventFlag && game_p->bossAppear)
				{
					game_p->UpdateBossEventEnd();
				}

                //空の描画
                sky.Draw(&camera);

				//マップ描画
				map.Draw(&camera);

				//ボス部屋との仕切りを描画
				if (game_p->bossEventFlag && !game_p->gateBlockFallen)
				{
					game_p->DrawGateBlock(&map, &camera);
				}

				//ボスが完全に分裂した後の閉鎖ブロック描画
				if (boss_p->allEmission)
				{
					game_p->DrawCloseBlock(&map, &camera);
				}

				//アイテムの描画
				item_p->Draw(&camera);

				//コインの描画
				coinMaster.Draw(&camera);

				//キックのエフェクトの描画
				if (player_p->kickFlag[0] && player_p->aliveFlag)
				{
					effect_p->DrawKickEffect(player_p, &camera);
				}

				//プレイヤー描画
				if (player_p->aliveFlag)
				{
					//プレイヤー自身の描画
					player_p->Draw(effect_p, &camera, game_p);
					
					//彗星キックの描画
					effect_p->DrawMeteoEffect(player_p, &camera);

					//コイン獲得エフェクトの描画
					if (effect_p->coinEffTimeFlag)
					{
						effect_p->DrawCoinEffect(player_p, &camera);
					}
				}

				//エネミー描画
				for (int i = 0; i < enemyCtrl.ENEMY_MASS; i++)
				{
					enemy_p[i]->Draw(effect_p, &camera, &map);
				}

				//ボスに属するエネミーを描画
				if (game_p->bossTimeFlag)
				{
					for (int i = 0; i < boss_p->PARTS_MASS_ALL; i++)
					{
						bossParts_p[i]->Draw(effect_p, &camera, &map);
					}
				}

				//ボス描画
				if (game_p->bossTimeFlag)
				{
					boss_p->Draw(&camera);
				}

				//フィルター描画
				filter.Draw(&sky, &map, player_p, enemy_p, &enemyCtrl, bossParts_p, boss_p, effect_p, &camera, game_p);

				//UI描画
				ui.DrawUI(player_p, item_p, &camera, game_p, boss_p);

				//イベントシーンとかでよくあるあの黒い帯の描画
				if (game_p->bossEventFlag || (!game_p->bossEventFlag && game_p->bossAppear))
				{
					game_p->DrawEventBand();
				}

				//ゲームオーバー演出
				if (!player_p->aliveFlag)
				{
					if (!effect_p->gameOverSetFlag)
					{
						effect_p->SetPlayerDown(player_p);
					}
					else
					{
						effect_p->UpdatePlayerDown(&stateFlags, game_p, &camera);
					}
				}

				//ゲームクリア演出
				if (!boss_p->alive)
				{
					if (!effect_p->gameClearSetFlag)
					{
						effect_p->SetGameClear();
					}
					else
					{
						effect_p->UpdateGameClear(&stateFlags, game_p);
					}
				}

#ifdef DEBUG

				//コマンドでブレイクポイントへ（デバッグ用）
				if (programControl.CheckHitDebugCommand(&pad))
				{
					WaitTimer(0);
				}

#endif DEBUG

				//撃破されたエネミーの上に煙エフェクトを描画
				effect_p->DrawSmokeEffect(&camera);

				//爆発エフェクトの描画
				effect_p->DrawHitMeteoEffect(enemy_p[0], &camera);

				ScreenFlip();

				if (game_p->gameOver || game_p->gameClear)
				{
					player_p->score += (game_p->gameClear) ? boss_p->BONUS_SCORE : 0;
					game_p->SetResult(result_p);
					SwitchFlag(&stateFlags.state[GAME][1], &stateFlags.state[GAME + 1][0]);
				}
			}
		}
		//----------------------------//
		// リザルト画面
		//----------------------------//
        else if (stateFlags.state[RESULT][0] || stateFlags.state[RESULT][1])
        {
            if (stateFlags.state[RESULT][0])
            {
                if (result_p->Init(programControl.replayCnt))
                {
					result_p->LoadScore();
					result_p->SaveScore(player_p->score);

					//どのくらいの人数がプレイしてくれたのか、記念に知りたいのだ！
					FILE *a;
					fopen_s(&a, "resourceTxt/playMass.txt", "a");
					if (a == NULL)
					{
						fclose(a);
					}
					else
					{
						fprintf_s(a, "%d\n", programControl.replayCnt + 1);

						fclose(a);
					}

					StopSoundMem(game_p->normalBgm);
					StopSoundMem(game_p->bossBgm);
					//ヒープ領域のマップチップ配置データを返却
					for (int i = 0; i < map.MAP_HEIGHT; i++)
					{
						delete[]map.mapPattern[i];
						map.mapPattern[i] = NULL;
					}
					delete[]map.mapPattern;
					map.mapPattern = NULL;

					//コインのインスタンスのメモリを解放
					coinMaster.DeletePointers();

					if (game_p->gameClear)
					{
						camera.Init();
						player_p->InitForResult(effect_p);
					}

                    SwitchFlag(&stateFlags.state[RESULT][0], &stateFlags.state[RESULT][1]);
                }
            }
            if (stateFlags.state[RESULT][1])
            {
                result_p->Update(player_p, boss_p, effect_p, &camera, game_p);

				DrawHow2GoNext(title_p->how2goNext);

				ScreenFlip();

#ifdef DEBUG

				//デバッグ用
				if (programControl.CheckHitDebugCommand(&pad))
				{
					WaitTimer(0);
				}

#endif DEBUG

                if (programControl.CheckHitGoNext(&pad))
                {
                    if (!programControl.prevEnter)
                    {
                        programControl.prevEnter = true;
                        SwitchFlag(&stateFlags.state[RESULT][1], &stateFlags.state[TITLE][0]);
						programControl.replayCnt++;
                    }
                }
                else
                {
                    programControl.prevEnter = false;
                }
            }
        }

		//escキーでゲームループから抜ける
		//あるいは、エラー発生でゲームループから抜ける
		if (programControl.CheckHitEsc() || ProcessMessage() == -1)
		{
			break;
		}
	}

	//マップ配置データのメモリ返却
	if (map.mapPattern != NULL)
	{
		for (int i = 0; i < map.MAP_HEIGHT; i++)
		{
			delete[]map.mapPattern[i];
			map.mapPattern[i] = NULL;
		}
		delete[]map.mapPattern;
		map.mapPattern = NULL;
	}

	//コインのインスタンスのメモリを解放
	coinMaster.DeletePointers();

	//エネミークラス配列のメモリ返却
	for (int i = 0; i < enemyCtrl.ENEMY_MASS; i++)
	{
		delete enemy_p[i];
		enemy_p[i] = NULL;
	}

	for (int i = 0; i < boss_p->PARTS_MASS_ALL; i++)
	{
		delete bossParts_p[i];
		bossParts_p[i] = NULL;
	}

	//ボスクラスのメモリ返却
	delete boss_p;
	boss_p = NULL;

	//プレイヤークラスのメモリ返却
	delete player_p;
	player_p = NULL;

	//エフェクトクラスのメモリ返却
	delete effect_p;
	effect_p = NULL;

	//アイテムクラスのメモリ返却
	delete item_p;
	item_p = NULL;

	//各種フェイズクラスのメモリ返却
	delete phase_p;
	phase_p = NULL;

	delete title_p;
	title_p = NULL;

	delete rule_p;
	rule_p = NULL;

	delete game_p;
	game_p = NULL;

	delete result_p;
	result_p = NULL;

	//フォントデータの解放
	RemoveFontResourceEx(engFontPath, FR_PRIVATE, NULL);

	DxLib_End();
	return 0;
}