#include "UI.h"

void UI::DrawPlayerHitPoint(Player * player)
{
	DrawFormatString(0, 5, PLAYER_UI_COLOR, "HP");

	for (int i = 0; i < player->hitPoint; i++)
	{
		int boxRed;
		int boxGreen;
		if (i < player->HITPOINT_MAX * 0.3f)
		{
			boxRed = 180;
			boxGreen = 0;
		}
		else if (i < player->HITPOINT_MAX * 0.6f)
		{
			boxRed = 180;
			boxGreen = 180;
		}
		else
		{
			boxRed = 0;
			boxGreen = 180;
		}
		DrawBoxAA((float)((GetFontSize() * 1.5f) + 5 + i * GetFontSize()), 5.0f, (float)((GetFontSize() * 1.5f) + (i + 1) * GetFontSize()), 5.0f + GetFontSize(),
			GetColor(boxRed, boxGreen, 0), TRUE);
	}
}

void UI::DrawPlayerScore(Player * player)
{
	DrawFormatString(0, 5 + GetFontSize() + 5, PLAYER_UI_COLOR, "SCORE:%6d", player->score);
}

void UI::DrawItemEfficacy(Item * item, Camera * camera)
{
	if (item->usedRecover)
	{
		DrawFormatStringF(item->usedStringX - camera->cameraOffsetX, item->usedStringY - camera->cameraOffsetY,
			GetColor(0, 180, 0), "HP MAX");
	}
	if (item->usedScore)
	{
		DrawFormatStringF(item->usedStringX - camera->cameraOffsetX, item->usedStringY - camera->cameraOffsetY,
			GetColor(0, 100, 100), "SCORE +%d", item->SCORE_INCREASE_MASS);
	}
}

void UI::DrawBurningBonus(Player * player, Game *game, Camera * camera)
{
	if ((game->nowTime - player->burningStompTime) < player->BURNING_TEXT_OFF_COUNT)
	{
		DrawFormatStringF(player->burningTextX - camera->cameraOffsetX, player->burningTextY - camera->cameraOffsetY, GetColor(128, 0, 0), "BURNING BONUS!\n+%d", player->BURNING_BONUS);
	}
}

void UI::DrawBossTimeLimit(Game * game, Boss * boss)
{
	if (game->bossTimeFlag && !game->bossEventFlag)
	{
		int countDownNum = (boss->TIME_LIMIT - (game->nowTime - boss->appearTime)) / 1000;
		SetFontSize(90);
		if ((game->nowTime - boss->appearTime) > boss->TIME_LIMIT)
		{
			DrawFormatString((WINDOW_WIDTH - GetDrawFormatStringWidth("%d", countDownNum)) / 2, 0, GetColor(255, 0, 0), "0");

			SetFontSize(256);
			char timeOut[32] = "TIME OUT!";
			int timeOutW, timeOutH, line;
			GetDrawFormatStringSize(&timeOutW, &timeOutH, &line, timeOut);
			DrawFormatString((WINDOW_WIDTH - timeOutW) / 2, (WINDOW_HEIGHT - timeOutH) / 2, GetColor(140, 0, 140), timeOut);
		}
		else
		{
			DrawFormatString((WINDOW_WIDTH - GetDrawFormatStringWidth("%d", countDownNum)) / 2, 0, GetColor(255, 0, 0), "%d", countDownNum);
		}
		SetFontSize(72);
	}
}

void UI::DrawUI(Player * player, Item * item, Camera * camera, Game *game, Boss *boss)
{
	SetFontSize(48);

	DrawItemEfficacy(item, camera);

	DrawBurningBonus(player, game, camera);

	DrawPlayerScore(player);

	DrawPlayerHitPoint(player);

	DrawBossTimeLimit(game, boss);

	SetFontSize(72);
}
