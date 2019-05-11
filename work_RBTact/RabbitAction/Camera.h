//　リファクタリング済み
#pragma once
#include "Dxlib.h"
#include "Player.h"
#include "Common.h"

class Camera
{
public:
	const float SCROLL_LINE_X_PLUS = WINDOW_WIDTH / 2.0f;		//右へスクロールするライン
	const float SCROLL_LINE_X_MINUS = 495.0f;					//左へスクロールするライン
	const float SCROLL_LINE_Y_UPPER = 150.0f;					//上へスクロールするライン
	const float SCROLL_LINE_Y_DOWNER = 900.0f;					//下へスクロールするライン
	const float LERP_RATE_X = 0.1f;								//カメラスクロールのラープの倍率
	const float BOSS_TIME_CAMERA_OFFSET_X = 27995.0f;			//ボス戦突入時のカメラオフセット
	const float TITLE_X = 15700.0f;								//タイトル画面で表示するフィールドのカメラオフセット
	const float TITLE_Y = -40.0f;								//上に同じ（y座標）

    float aimCameraOffsetX;						//cameraOffsetXの目標とする値。ラープ時にここを目指す
    float aimCameraOffsetY;						//cameraOffsetYの目標とする値。ラープ時にここを目指す
	float cameraOffsetX;						//ステージスタートからカメラをどのくらい水平方向にずらしたか
	float cameraOffsetY;						//ステージスタートからカメラをどのくらい鉛直方向にずらしたか

	void Init4Title();								//タイトル画面での初期化
	void Init();									//ゲーム画面での初期化
	void AddCameraOffsetX(Player *player);			//cameraOffsetXに値を追加する関数。
	void ScrollScreenX(Player *player);				//メインではこちらを呼ぶ。カプセル化は後の課題。
	void AddCameraOffsetY(Player *player);			//cameraOffsetYに値を追加する関数。
	void ScrollScreenY(Player *player);				//メインではこちらを呼ぶ。カプセル化は後の課題。
};