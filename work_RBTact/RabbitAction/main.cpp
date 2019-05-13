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
	//��ʃ��[�h�̐ݒ�
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16);				//�𑜓x��1920*1080�Acolor��16bit��
	ChangeWindowMode(TRUE);										//�E�B���h�E���[�h��

	//DX���C�u����������
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	//�O���t�B�b�N�`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	//�E�B���h�E�̖��O
	SetMainWindowText("�]�������炽���̐Ԃ��{�[����������");

	//�t�H���g�̐ݒ�
	LPCSTR engFontPath = "Font/mini-wakuwaku.otf";
	if (!(AddFontResourceEx(engFontPath, FR_PRIVATE, NULL) > 0))
	{
		//�t�H���g�ǂݍ��݃G���[����
		return -1;
	}
	ChangeFont("mini-�킭�킭");

	//�t�H���g�T�C�Y�̐ݒ�
	SetFontSize(72);

	//�����̏����l��ݒ�
	{
		DATEDATA date;
		GetDateTime(&date);

		int seed = date.Day + date.Hour + date.Min + date.Sec;

		SRand(seed);

		//�v���C�񐔋L�^�f�[�^�ɓ��t���L��
		FILE *a;
		fopen_s(&a, "resourceTxt/playMass.txt", "a");
		if (a == NULL)
		{
			fclose(a);
		}
		else
		{
			fprintf_s(a, "%d��\n", date.Day);

			fclose(a);
		}
	}

	//�W���C�p�b�h�̐ݒ�(XBOX360�R���g���[���[����)
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

	//�Q�[���̐i�s��Ԃ����ׂ�false�ɂ�����A�^�C�g����ʂ̏�����true��
	phase_p->InitGameState(&stateFlags);
	stateFlags.state[TITLE][0] = true;

	//�^�C�g���Ńn�C�X�R�A�����L���O��\�����邽�߁A�����L���O�f�[�^��ǂݍ���
	result_p->LoadScore();

	//---------------------------------//
	//�Q�[�����[�v
	//---------------------------------//
	while (true)
	{
		//�p�b�h���̎擾
		GetJoypadXInputState(DX_INPUT_PAD1, &pad);

		//----------------------------//
		// �^�C�g��
		//----------------------------//
		if (stateFlags.state[TITLE][0] || stateFlags.state[TITLE][1])
		{
			//�^�C�g���̏�����
			if (stateFlags.state[TITLE][0])
			{
				if (title_p->Init(programControl.replayCnt, player_p, &camera, &map, &sky) && map.LoadPattern("map/map01..csv"))
				{
					StopSoundMem(result_p->shortBgm);
					SwitchFlag(&stateFlags.state[TITLE][0], &stateFlags.state[TITLE][1]);
				}
			}
			//�A�b�v�f�[�g
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
		// ���[������
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
		// �Q�[�����
		//----------------------------//
		else if (stateFlags.state[GAME][0] || stateFlags.state[GAME][1])
		{
			if (stateFlags.state[GAME][0])
			{
				//�e�평����
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

				//����̎�t
				if (player_p->aliveFlag && !game_p->stopUpdate)
				{
					player_p->UpdateControl(&pad, effect_p, game_p, &programControl);
				}

				//�v���C���[�ƃu���b�N�Ƃ̓����蔻��
				if (player_p->aliveFlag && !game_p->stopUpdate)
				{
					hitChk.CheckHitPlayerBlock(player_p, &map, game_p);
				}

				//�A�C�e�����̍X�V
				item_p->Update(game_p);

				//�v���C���[�ƃA�C�e���̓����蔻��
				hitChk.CheckHitPlayerItem(player_p, item_p);

				//�R�C�����̍X�V
				coinMaster.Update(&camera);

				//�v���C���[�ƃR�C���̓����蔻��
				hitChk.CheckHitPlayerCoin(player_p, &coinMaster, effect_p);

				//�R�C���G�t�F�N�g�̍X�V
				if (effect_p->coinEffTimeFlag)
				{
					effect_p->UpdateCoinEffect();
				}

				//�G�l�~�[�ƃu���b�N�Ƃ̓����蔻��
				{
					//�ʏ�^�C�v�G�l�~�[
					for (int i = 0; i < enemyCtrl.ENEMY_MASS; i++)
					{
						hitChk.CheckHitEnemyBlock(enemy_p[i], &enemyCtrl, &map);
					}

					//�{�X�^�C�v�G�l�~�[
					for (int i = 0; i < boss_p->PARTS_MASS_ALL; i++)
					{
						if ((bossParts_p[i]->CheckInsideScreen(&camera, &map) && !boss_p->partsSplitFlag[i]) ||
						boss_p->partsSplitFlag[i])
						{
							hitChk.CheckHitEnemyBlock(bossParts_p[i], &enemyCtrl, &map);
						}
					}
				}

				//�v���C���[�ƃG�l�~�[�Ƃ̓����蔻��
				if (player_p->aliveFlag)
				{
					//�ʏ�^�G�l�~�[
					contactPE = hitChk.CheckHitPlayerEnemy(player_p, enemy_p, boss_p, effect_p, &camera, &map, &enemyCtrl, enemyCtrl.ENEMY_MASS);

					//�{�X�^�C�v�G�l�~�[
					if (!contactPE)
					{
						contactPE = hitChk.CheckHitPlayerEnemy(player_p, bossParts_p, boss_p, effect_p, &camera, &map, &enemyCtrl, boss_p->PARTS_MASS_ALL);
					}
				}

				//�{�X�̍X�V
				if (!game_p->bossEventFlag && game_p->bossTimeFlag)
				{
					boss_p->Update(bossParts_p, game_p, &map);
				}

				//�{�X���S�ẴG�l�~�[��r�o������A�{�X�p�̕���Ƀv���C���[�����Ȃ�����B
				hitChk.CheckHitPlayerBoss(player_p, boss_p);

				if (!game_p->stopUpdate)
				{
					//�G�l�~�[�̍X�V
					for (int i = 0; i < enemyCtrl.ENEMY_MASS; i++)
					{
						enemy_p[i]->Update(player_p, &enemyCtrl, effect_p, game_p);
					}

					//�{�X�^�C�v�̃G�l�~�[�̍X�V
					if (game_p->bossTimeFlag)
					{
						//�K����0-->���Ƃ�EnemyControl�N���X�Ɉڂ��ׂ�����
						enemy_p[0]->UpdateBossType(bossParts_p, boss_p, player_p, effect_p, game_p);
					}
				}

				//���j���ꂽ�G�l�~�[�̉��G�t�F�N�g�̍X�V
				effect_p->UpdateSmoke(&enemyCtrl);


#ifdef DEBUG
				//�R�}���h�Ńu���C�N�|�C���g�ցi�f�o�b�O�p�j
				if (programControl.CheckHitDebugCommand(&pad))
				{
					WaitTimer(0);
				}

#endif DEBUG

				//�o�E���h�������A���炩�̌����ŏ����Ȃ��Ȃ�A���̂܂܏�֏���čs���Ă��܂��A
				//�����鏸�V�o�O��h�����߁A�����G�l�~�[������ł��Ȃ��Ƃ��ɂ͏d�͂�L����
				if (!player_p->hitGroundFlag && !contactPE)
				{
					player_p->graphNum4anim = 0;
				}

				//�v���C���[���ςɐ[���Ƃ���܂ōs�����Ƃ��A�n��ɖ߂�
				if (player_p->y > map.GROUND_Y)
				{
					player_p->y = map.GROUND_Y - player_p->h;

					player_p->Bound(map.GROUND_Y);
				}

                //�J�����X�N���[��
                //�L�b�N�������͎��s���Ȃ�
                if (!game_p->stopUpdate)
                {
                    camera.ScrollScreenX(player_p);
                    camera.ScrollScreenY(player_p);
                }

				//�L�b�N�����ɓ����Ă��Ȃ����y�����Z--->�����Ă���Ή��Z���Ȃ�
				if (player_p->aliveFlag && !game_p->stopUpdate)
				{
					//if (!player_p->kickFlag[0])
					{
						//�L�b�N���s���̂Ƃ��A���̋������L�^����֐�
						player_p->IncreaseKickDistance(effect_p);
						//�v���C���[�̎��ۂ̈ړ�
						player_p->UpdateMoveY();
						player_p->UpdateMoveX();
					}
					//�L�b�N�����ɓ����Ă���΃v���C���[�̉摜���󒆂̂��̂ɂ���
					if (player_p->kickFlag[0])
					{
						player_p->graphNum4anim = 0;
					}
				}

				//�{�X�C�x���g����
				if (player_p->x + player_p->w > game_p->BOSS_BATTLE_LINE_X)
				{
					if (!game_p->bossTimeFlag && !game_p->bossEventFlag && !player_p->jumpFlag)
					{
						game_p->InitBossEvent(&map, &camera);
						//���łɃ{�X�p�[�c�S�r�o��̕��p�u���b�N��������(�́M*�U)�ͯ
						game_p->InitCloseBossArea(&map);
					}
					if (game_p->bossTimeFlag && game_p->bossEventFlag)
					{
						game_p->UpdateBossEvent(player_p, boss_p, &map, &camera);
					}
				}

				//�{�X�C�x���g�I������
				if (!game_p->bossEventFlag && game_p->bossAppear)
				{
					game_p->UpdateBossEventEnd();
				}

                //��̕`��
                sky.Draw(&camera);

				//�}�b�v�`��
				map.Draw(&camera);

				//�{�X�����Ƃ̎d�؂��`��
				if (game_p->bossEventFlag && !game_p->gateBlockFallen)
				{
					game_p->DrawGateBlock(&map, &camera);
				}

				//�{�X�����S�ɕ��􂵂���̕��u���b�N�`��
				if (boss_p->allEmission)
				{
					game_p->DrawCloseBlock(&map, &camera);
				}

				//�A�C�e���̕`��
				item_p->Draw(&camera);

				//�R�C���̕`��
				coinMaster.Draw(&camera);

				//�L�b�N�̃G�t�F�N�g�̕`��
				if (player_p->kickFlag[0] && player_p->aliveFlag)
				{
					effect_p->DrawKickEffect(player_p, &camera);
				}

				//�v���C���[�`��
				if (player_p->aliveFlag)
				{
					//�v���C���[���g�̕`��
					player_p->Draw(effect_p, &camera, game_p);
					
					//�a���L�b�N�̕`��
					effect_p->DrawMeteoEffect(player_p, &camera);

					//�R�C���l���G�t�F�N�g�̕`��
					if (effect_p->coinEffTimeFlag)
					{
						effect_p->DrawCoinEffect(player_p, &camera);
					}
				}

				//�G�l�~�[�`��
				for (int i = 0; i < enemyCtrl.ENEMY_MASS; i++)
				{
					enemy_p[i]->Draw(effect_p, &camera, &map);
				}

				//�{�X�ɑ�����G�l�~�[��`��
				if (game_p->bossTimeFlag)
				{
					for (int i = 0; i < boss_p->PARTS_MASS_ALL; i++)
					{
						bossParts_p[i]->Draw(effect_p, &camera, &map);
					}
				}

				//�{�X�`��
				if (game_p->bossTimeFlag)
				{
					boss_p->Draw(&camera);
				}

				//�t�B���^�[�`��
				filter.Draw(&sky, &map, player_p, enemy_p, &enemyCtrl, bossParts_p, boss_p, effect_p, &camera, game_p);

				//UI�`��
				ui.DrawUI(player_p, item_p, &camera, game_p, boss_p);

				//�C�x���g�V�[���Ƃ��ł悭���邠�̍����т̕`��
				if (game_p->bossEventFlag || (!game_p->bossEventFlag && game_p->bossAppear))
				{
					game_p->DrawEventBand();
				}

				//�Q�[���I�[�o�[���o
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

				//�Q�[���N���A���o
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

				//�R�}���h�Ńu���C�N�|�C���g�ցi�f�o�b�O�p�j
				if (programControl.CheckHitDebugCommand(&pad))
				{
					WaitTimer(0);
				}

#endif DEBUG

				//���j���ꂽ�G�l�~�[�̏�ɉ��G�t�F�N�g��`��
				effect_p->DrawSmokeEffect(&camera);

				//�����G�t�F�N�g�̕`��
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
		// ���U���g���
		//----------------------------//
        else if (stateFlags.state[RESULT][0] || stateFlags.state[RESULT][1])
        {
            if (stateFlags.state[RESULT][0])
            {
                if (result_p->Init(programControl.replayCnt))
                {
					result_p->LoadScore();
					result_p->SaveScore(player_p->score);

					//�ǂ̂��炢�̐l�����v���C���Ă��ꂽ�̂��A�L�O�ɒm�肽���̂��I
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
					//�q�[�v�̈�̃}�b�v�`�b�v�z�u�f�[�^��ԋp
					for (int i = 0; i < map.MAP_HEIGHT; i++)
					{
						delete[]map.mapPattern[i];
						map.mapPattern[i] = NULL;
					}
					delete[]map.mapPattern;
					map.mapPattern = NULL;

					//�R�C���̃C���X�^���X�̃����������
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

				//�f�o�b�O�p
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

		//esc�L�[�ŃQ�[�����[�v���甲����
		//���邢�́A�G���[�����ŃQ�[�����[�v���甲����
		if (programControl.CheckHitEsc() || ProcessMessage() == -1)
		{
			break;
		}
	}

	//�}�b�v�z�u�f�[�^�̃������ԋp
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

	//�R�C���̃C���X�^���X�̃����������
	coinMaster.DeletePointers();

	//�G�l�~�[�N���X�z��̃������ԋp
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

	//�{�X�N���X�̃������ԋp
	delete boss_p;
	boss_p = NULL;

	//�v���C���[�N���X�̃������ԋp
	delete player_p;
	player_p = NULL;

	//�G�t�F�N�g�N���X�̃������ԋp
	delete effect_p;
	effect_p = NULL;

	//�A�C�e���N���X�̃������ԋp
	delete item_p;
	item_p = NULL;

	//�e��t�F�C�Y�N���X�̃������ԋp
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

	//�t�H���g�f�[�^�̉��
	RemoveFontResourceEx(engFontPath, FR_PRIVATE, NULL);

	DxLib_End();
	return 0;
}