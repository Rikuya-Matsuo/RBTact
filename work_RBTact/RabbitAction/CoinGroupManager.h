//�@���t�@�N�^�����O�ς�
#pragma once
#include "Camera.h"
#include "CoinGroup.h"

class CoinGroupManager
{
public:
	static const int COIN_AREA_MASS = 3;				//�R�C���z�u�G���A�̐�
	const char *FILE_NAME = "resourceTxt/coin.txt";		//�R�C���z�u�󋵂������Ă���t�@�C���̖��O

	CoinGroup *coinCtrl;

	void Init();
	void Update(Camera *camera);
	void Draw(Camera *camera);
	void DeletePointers();
};
