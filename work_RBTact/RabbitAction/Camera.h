#pragma once
#include "Dxlib.h"
#include "Player.h"
#include "Common.h"

class Camera
{
public:
	const float SCROLL_LINE_X_PLUS = WINDOW_WIDTH / 2.0f;
	const float SCROLL_LINE_X_MINUS = 495.0f;
	const float SCROLL_LINE_Y_UPPER = 150.0f;
	const float SCROLL_LINE_Y_DOWNER = 900.0f;
	const float LERP_RATE_X = 0.1f;
	const float BOSS_TIME_CAMERA_OFFSET_X = 27995.0f;
	const float TITLE_X = 15700.0f;
	const float TITLE_Y = -40.0f;

    float aimCameraOffsetX;
    float aimCameraOffsetY;
	float cameraOffsetX;
	float cameraOffsetY;

	void Init4Title();
	void Init();
	void AddCameraOffsetX(Player *player);
	void ScrollScreenX(Player *player);
	void AddCameraOffsetY(Player *player);
	void ScrollScreenY(Player *player);
};