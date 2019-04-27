#pragma once
#include "DxLib.h"
#include "Common.h"
#include "Camera.h"
#include "Map.h"

class Item
{
public:
	static const int RECOVER_ITEM_MASS = 3;
	static const int SCORE_ITEM_MASS = 3;
	const int RECOVER_INCREASE_MASS = 3;
	const int SCORE_INCREASE_MASS = 10000;
	const int USED_STRING_DRAW_COUNT = 2000;
	const float GRAPH_EXTEND_RATE = 3.0f;
	const float USED_STRING_OFFSET_X = -1.5f;
	const float USED_STRING_OFFSET_Y = -3.0f;

	int recoverGraph;
	int scoreGraph;
	int usedTime;
	int recoverSound;
	int scoreSound;
	float recoverX[RECOVER_ITEM_MASS];
	float recoverY[RECOVER_ITEM_MASS];
	float recoverW, recoverH;
	float scoreX[SCORE_ITEM_MASS];
	float scoreY[SCORE_ITEM_MASS];
	float scoreW, scoreH;
	float usedStringX;
	float usedStringY;
	bool recoverExist[RECOVER_ITEM_MASS];
	bool scoreExist[SCORE_ITEM_MASS];
	bool usedRecover;
	bool usedScore;

	void Init(Map *map);
	void Update(Game *game);
	void Use(Player *player, bool recover, int id);		//使うアイテムが回復ならtrue、スコアならfalse
	void Draw(Camera *camera);
};