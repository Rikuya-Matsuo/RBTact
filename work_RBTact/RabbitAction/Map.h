#pragma once
#include "Dxlib.h"
#include "Common.h"
#include "Camera.h"

class Map
{
public:
	const float MAPCHIP_SIZE_W = 100.0f;
	const float MAPCHIP_SIZE_H = 100.0f;
	static const int MAP_WIDTH = 300;				//�^�C�������H
	static const int MAP_HEIGHT = 25;				//�^�C�������H
	const int GROUND_BLOCK_MASS = 3;				//�n�ʂ̓^�C�������H
	const int BLOCK_OFFSET_4DRAW = 0;		//�`��̂��߂̃I�t�Z�b�g(y��)
	const float MAP_HIGHEST = WINDOW_HEIGHT - MAPCHIP_SIZE_H * MAP_HEIGHT;
	const float HEIGHT_LENGTH = MAP_HEIGHT * MAPCHIP_SIZE_H;
	const float GROUND_Y = WINDOW_HEIGHT - MAPCHIP_SIZE_H * GROUND_BLOCK_MASS;

	//�}�b�v�`�b�v�̔z�u�p�^�[�����i�[
	//�q�[�v�̈�Ɋi�[���邽�߁Aint**�^�Œ�`
	int **mapPattern;
	int mapchipGraph[35];

	bool LoadPattern(const char *mapFile);
	void LoadChipGraph(int replayCnt);
	void Draw(Camera *camera);
};