#include "Camera.h"

void Camera::InitForTitle()
{
	cameraOffsetX = TITLE_X;
	cameraOffsetY = TITLE_Y;
}

void Camera::Init()
{
    aimCameraOffsetX = cameraOffsetX = 0.0f;
    aimCameraOffsetY = cameraOffsetY = 0.0f;
}

void Camera::AddCameraOffsetX(Player *player)
{
    if (player->x + player->w + player->velocityX > SCROLL_LINE_X_PLUS + cameraOffsetX)
    {
        aimCameraOffsetX = cameraOffsetX + Lerp(SCROLL_LINE_X_PLUS + cameraOffsetX, player->x + player->w + player->velocityX, LERP_RATE_X);
    }
    else if (player->x + player->velocityX < SCROLL_LINE_X_MINUS + cameraOffsetX)
    {
        aimCameraOffsetX = cameraOffsetX + Lerp(SCROLL_LINE_X_MINUS + cameraOffsetX, player->x + player->velocityX, LERP_RATE_X);
    }
	cameraOffsetX = aimCameraOffsetX;
}

void Camera::ScrollScreenX(Player *player)
{
	if (player->x + player->w + player->velocityX > SCROLL_LINE_X_PLUS + cameraOffsetX ||
		player->x + player->velocityX < SCROLL_LINE_X_MINUS + cameraOffsetX)
	{
		AddCameraOffsetX(player);
	}
}

void Camera::AddCameraOffsetY(Player *player)
{
	if (player->kickFlag[0])
	{
		aimCameraOffsetY = cameraOffsetY + player->velocityY / powf(player->SLOWMOTION_RATE, 0.5);
	}
	else
	{
		aimCameraOffsetY = cameraOffsetY + player->velocityY;
	}

	if (aimCameraOffsetY > 0)
	{
		aimCameraOffsetY = 0;
	}

	cameraOffsetY = aimCameraOffsetY;
}

void Camera::ScrollScreenY(Player *player)
{
	//着地中はスクロールしない
	if (((player->y + player->velocityY < SCROLL_LINE_Y_UPPER + cameraOffsetY ||
		player->y + player->h + player->velocityY > SCROLL_LINE_Y_DOWNER + cameraOffsetY) && player->jumpFlag) ||
        player->y + player->h > SCROLL_LINE_Y_DOWNER + cameraOffsetY && !player->jumpFlag)
	{
		AddCameraOffsetY(player);
	}
}