#pragma once
#include "Dxlib.h"
#include "Common.h"
#include "Camera.h"

class Sky
{
public:
	const int SKYCHIP_SIZE_H = 32;
	const int SKYCHIP_SIZE_W = 32;
	const int STARCHIP_NUM = 3;
	const float OFFSET_X = -150.0f;
	const float DISTANT_VIEW_SCROLL_RATE = 0.5f;

	int graph[12];

	void Load(int replayCnt);
	void Draw(Camera *camera);
	void Draw4Rule();
};