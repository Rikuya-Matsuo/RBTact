#include "Map.h"

bool Map::LoadPattern(const char *mapFile)
{
	int i, j, k;

	//二次元配列の動的確保
	mapPattern = new int*[MAP_HEIGHT];
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		mapPattern[i] = new int[MAP_WIDTH];
	}

	//マップデータの読み込み
	char tmp;
	FILE *fp;
	fopen_s(&fp, mapFile, "r");
	if (fp == NULL)
	{
		return false;
	}
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			char chipType[3] = "";
			k = 0;
			while ((tmp = fgetc(fp)) != '\n' && tmp != ',')
			{
				chipType[k++] = tmp;
			}
			mapPattern[i][j] = atoi(chipType);
		}
	}

	fclose(fp);

	return true;
}

void Map::LoadChipGraph(int replayCnt)
{
	if (!replayCnt)
	{
		LoadDivGraph("image/mapchip00.png", 35, 10, 4, 32, 32, mapchipGraph);
	}
}

void Map::Draw(Camera *camera)
{
	int i, j;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		//ブロックが描画範囲内かどうかのチェック（ｙ）
		if (i * MAPCHIP_SIZE_H > (MAP_HEIGHT * MAPCHIP_SIZE_H) - WINDOW_HEIGHT + camera->cameraOffsetY - DRAW_MARGIN &&
			i * MAPCHIP_SIZE_H < (MAP_HEIGHT * MAPCHIP_SIZE_H) + camera->cameraOffsetY + DRAW_MARGIN)
		{
			for (j = 0; j < MAP_WIDTH; j++)
			{
				//ブロックが描画範囲内かどうかのチェック（ｘ）
				if (j * MAPCHIP_SIZE_W > camera->cameraOffsetX - DRAW_MARGIN && j * MAPCHIP_SIZE_W < WINDOW_WIDTH + camera->cameraOffsetX + DRAW_MARGIN)
				{
					//スタート時の左の壁は非表示にする
					if (!(j * MAPCHIP_SIZE_W < 200 && mapPattern[i][j] == 10))
					{
						DrawExtendGraphF(j * MAPCHIP_SIZE_W - camera->cameraOffsetX, MAP_HIGHEST - camera->cameraOffsetY + (i * MAPCHIP_SIZE_H),
							(j + 1) * MAPCHIP_SIZE_W - camera->cameraOffsetX, MAP_HIGHEST - camera->cameraOffsetY + ((i + 1) * MAPCHIP_SIZE_H), mapchipGraph[mapPattern[i][j]], FALSE);
					}
				}
				//ブロックのＸ座標が範囲よりも右に達したとき、ループを終了
				else if (j * MAPCHIP_SIZE_W > WINDOW_WIDTH + camera->cameraOffsetX + DRAW_MARGIN)
				{
					break;
				}
			}
		}
		//ブロックのＹ座標が範囲よりも下に達したとき、ループを終了
		else if (i * MAPCHIP_SIZE_H > (MAP_HEIGHT * MAPCHIP_SIZE_H) + camera->cameraOffsetY + DRAW_MARGIN)
		{
			break;
		}
	}

	//草原を画面左の奥まで描画して、あたかも大地はまだ続いているように見せる
	if (camera->cameraOffsetX - DRAW_MARGIN < 0)
	{
		for (int bx = 0; -(bx + 1) * MAPCHIP_SIZE_W > (camera->cameraOffsetX - DRAW_MARGIN); bx++)
		{
			for (int by = 0; by < GROUND_BLOCK_MASS; by++)
			{
				DrawExtendGraphF(-(bx + 1) * MAPCHIP_SIZE_W - camera->cameraOffsetX, MAP_HIGHEST + (MAP_HEIGHT - GROUND_BLOCK_MASS + by) * MAPCHIP_SIZE_H - camera->cameraOffsetY,
					-bx * MAPCHIP_SIZE_W - camera->cameraOffsetX, MAP_HIGHEST + (MAP_HEIGHT - GROUND_BLOCK_MASS + (by + 1)) * MAPCHIP_SIZE_H - camera->cameraOffsetY, mapchipGraph[0], FALSE);
			}
		}
	}
}
