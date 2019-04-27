#pragma once
#include "Dxlib.h"
#include "Common.h"
#include "Camera.h"

class Map
{
public:
	const float MAPCHIP_SIZE_W = 100.0f;
	const float MAPCHIP_SIZE_H = 100.0f;
	static const int MAP_WIDTH = 300;				//タイル何個分？
	static const int MAP_HEIGHT = 25;				//タイル何個分？
	const int GROUND_BLOCK_MASS = 3;				//地面はタイル何個分？
	const int BLOCK_OFFSET_4DRAW = 0;		//描画のためのオフセット(y軸)
	const float MAP_HIGHEST = WINDOW_HEIGHT - MAPCHIP_SIZE_H * MAP_HEIGHT;
	const float HEIGHT_LENGTH = MAP_HEIGHT * MAPCHIP_SIZE_H;
	const float GROUND_Y = WINDOW_HEIGHT - MAPCHIP_SIZE_H * GROUND_BLOCK_MASS;

	//マップチップの配置パターンを格納
	//ヒープ領域に格納するため、int**型で定義
	int **mapPattern;
	int mapchipGraph[35];

	bool LoadPattern(const char *mapFile);
	void LoadChipGraph(int replayCnt);
	void Draw(Camera *camera);
};