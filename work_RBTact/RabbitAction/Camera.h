//�@���t�@�N�^�����O�ς�
#pragma once
#include "Dxlib.h"
#include "Player.h"
#include "Common.h"

class Camera
{
public:
	const float SCROLL_LINE_X_PLUS = WINDOW_WIDTH / 2.0f;		//�E�փX�N���[�����郉�C��
	const float SCROLL_LINE_X_MINUS = 495.0f;					//���փX�N���[�����郉�C��
	const float SCROLL_LINE_Y_UPPER = 150.0f;					//��փX�N���[�����郉�C��
	const float SCROLL_LINE_Y_DOWNER = 900.0f;					//���փX�N���[�����郉�C��
	const float LERP_RATE_X = 0.1f;								//�J�����X�N���[���̃��[�v�̔{��
	const float BOSS_TIME_CAMERA_OFFSET_X = 27995.0f;			//�{�X��˓����̃J�����I�t�Z�b�g
	const float TITLE_X = 15700.0f;								//�^�C�g����ʂŕ\������t�B�[���h�̃J�����I�t�Z�b�g
	const float TITLE_Y = -40.0f;								//��ɓ����iy���W�j

    float aimCameraOffsetX;						//cameraOffsetX�̖ڕW�Ƃ���l�B���[�v���ɂ�����ڎw��
    float aimCameraOffsetY;						//cameraOffsetY�̖ڕW�Ƃ���l�B���[�v���ɂ�����ڎw��
	float cameraOffsetX;						//�X�e�[�W�X�^�[�g����J�������ǂ̂��炢���������ɂ��炵����
	float cameraOffsetY;						//�X�e�[�W�X�^�[�g����J�������ǂ̂��炢���������ɂ��炵����

	void Init4Title();								//�^�C�g����ʂł̏�����
	void Init();									//�Q�[����ʂł̏�����
	void AddCameraOffsetX(Player *player);			//cameraOffsetX�ɒl��ǉ�����֐��B
	void ScrollScreenX(Player *player);				//���C���ł͂�������ĂԁB�J�v�Z�����͌�̉ۑ�B
	void AddCameraOffsetY(Player *player);			//cameraOffsetY�ɒl��ǉ�����֐��B
	void ScrollScreenY(Player *player);				//���C���ł͂�������ĂԁB�J�v�Z�����͌�̉ۑ�B
};