#pragma once
#include "DxLib.h"
#include "Enemy.h"

class EnemyControl
{
public:
	enum Type
	{
		NORMAL,
		JUMP,
		HIGH_SPEED,
		BOSS,
		INVALID_EnemyType
	};

	static const int ENEMY_MASS = 41;
	const char *FILE_NAME = "resourceTxt/enemy.txt";

	int existMass;
	int damageSound;
	int downSound;

	void Init(Enemy **enemy);
	void LoadPosition(const char *fileName, Enemy **enemy);
};