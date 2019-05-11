//　リファクタリング済み
#pragma once
#include "DxLib.h"
#include "Coin.h"
#include "Camera.h"

class CoinGroup
{
public:
	const float GRAPH_EXTEND_RATE = 1.5f;		//画像の拡大表示倍率

	Coin **coinArray;		//コインの配列
	int id;					//コイングループの個体番号
	int se;					//効果音ハンドル
	int graph;				//画像ハンドル
	int coinMassX;			//コインの行
	int coinMassY;			//コインの列
	float coinW;			//画像、当たり判定の大きさ
	float coinH;			//画像、当たり判定の大きさ
	

	void Load(FILE *r);							//読み込み（コインの配置情報はtxtデータで保管している）
	void Init(FILE *r, int coinAreaMass);		//初期化。
	void Update(Camera *camera);				//更新。
	void Draw(Camera *camera);					//描画。
};