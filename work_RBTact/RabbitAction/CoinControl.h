#pragma once
#include "DxLib.h"
#include "Coin.h"
#include "Camera.h"

class CoinControl
{
public:
	const float GRAPH_EXTEND_RATE = 1.5f;

	Coin **coinArray;
	int id;
	int se;
	int graph;
	int coinMassX;
	int coinMassY;
	float coinW;
	float coinH;
	

	void Load(FILE *r);
	void Init(FILE *r, int coinAreaMass);
	void Update(Camera *camera);
	void Draw(Camera *camera);
};