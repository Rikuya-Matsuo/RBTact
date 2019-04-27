#include "Filter.h"

void Filter::Init()
{
	screen = MakeScreen(WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
}

void Filter::Draw(Sky *sky, Map *map, Player *player, Enemy **enemy, EnemyControl *enemyCtrl, Enemy **bossEnemy, Boss *boss, Effect *eff, Camera * camera, Game *game)
{
	SetDrawScreen(screen);

	sky->Draw(camera);

	map->Draw(camera);

	if (game->bossEventFlag && !game->gateBlockFallen)
	{
		game->DrawGateBlock(map, camera);
	}

	player->Draw(eff, camera, game);

	eff->DrawMeteoEffect(player, camera);

	//コイン獲得エフェクトの描画
	if (eff->coinEffTimeFlag)
	{
		eff->DrawCoinEffect(player, camera);
	}

	for (int i = 0; i < enemyCtrl->ENEMY_MASS; i++)
	{
		enemy[i]->Draw(eff, camera, map);
	}

	if (game->bossTimeFlag)
	{
		for (int i = 0; i < boss->PARTS_MASS_ALL; i++)
		{
			bossEnemy[i]->Draw(eff, camera, map);
		}

		boss->Draw(camera);
	}

	GraphFilter(screen, DX_GRAPH_FILTER_GAUSS, 16, 2000);

	SetDrawScreen(DX_SCREEN_BACK);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 50);

	DrawGraph(0, 0, screen, FALSE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
