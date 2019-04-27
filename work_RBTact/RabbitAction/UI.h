#pragma once
#include "DxLib.h"
#include "Common.h"
#include "Phase.h"
#include "Player.h"
#include "Boss.h"
#include "Item.h"
#include "Camera.h"

class UI
{
public:
	const unsigned int PLAYER_UI_COLOR = GetColor(0, 140, 0);

	void DrawPlayerHitPoint(Player *player);
	void DrawPlayerScore(Player *player);
	void DrawItemEfficacy(Item *item, Camera *camera);
	void DrawBurningBonus(Player *player, Game *game, Camera *camera);
	void DrawBossTimeLimit(Game *game, Boss *boss);
	void DrawUI(Player *player, Item *item, Camera *camera, Game *game, Boss *boss);
};