#include "CoinGroup.h"

void CoinGroup::Load(FILE *r)
{
	//*coin = new Coin **[COIN_AREA_MASS];

	if (r == NULL)
	{
		printfDx("�R�C�����ǂݍ��ݎ��s");
		return;
	}
	
	/*
	�t�@�C���̍\��
	/////////////////
	�����x���W
	�����y���W
	�R�C���̌�X
	�R�C���̌�Y
	�R�C���ƃR�C����x�̊Ԋu
	�R�C���ƃR�C����y�̊Ԋu
	/////////////////
	*/

	int step = 0;
	int coinAreaNum = 0;
	char buf[10];

	int areaX;
	int areaY;
	int marginX;
	int marginY;

	SetAllNull(buf, 10);

	while (fgets(buf, 10, r))
	{
		if (!step)
		{
			areaX = atoi(buf);
			step++;
		}
		else if (step == 1)
		{
			areaY = atoi(buf);
			step++;
		}
		else if (step == 2)
		{
			coinMassX = atoi(buf);
			step++;
		}
		else if (step == 3)
		{
			coinMassY = atoi(buf);
			step++;
		}
		else if (step == 4)
		{
			marginX = atoi(buf);
			step++;
		}
		else if (step == 5)
		{
			marginY = atoi(buf);
			step++;
		}
		else if (step == 6 && *buf == '\n')
		{
			////�C���X�^���X����
			//*coinArray[coinAreaNum] = new Coin *[coinMassY[coinAreaNum]];
			//for (int i = 0; i < coinMassY[coinAreaNum]; i++)
			//{
			//	*coinArray[i] = new Coin[coinMassX[coinAreaNum]];
			//}

			//for (int i = 0; i < coinMassY[coinAreaNum]; i++)
			//{
			//	for (int j = 0; j < coinMassX[coinAreaNum]; j++)
			//	{
			//		coinArray[i][j].x = areaX + (coinW + marginX) * j;
			//		coinArray[i][j].y = areaY + (coinH + marginY) * i;
			//	}
			//}

			//coinAreaNum++;
			//step = 0;

			//�C���X�^���X����
			coinArray = new Coin*[coinMassY];
			for (int i = 0; i < coinMassY; i++)
			{
				coinArray[i] = new Coin[coinMassX];
			}

			//���������o�[�Ɋi�[
			for (int i = 0; i < coinMassY; i++)
			{
				for (int j = 0; j < coinMassX; j++)
				{
					coinArray[i][j].x = areaX + (coinW + marginX) * j;
					coinArray[i][j].y = areaY + (coinH + marginY) * i;
				}
			}

			break;
		}
		else
		{
			printfDx("�R�C�����ǂݍ��ݎ��s");
			return;
		}

		SetAllNull(buf, 10);
	}

}

void CoinGroup::Init(FILE *r, int coinAreaMass)
{
	graph = LoadGraph("image/coin.png");
	se = LoadSoundMem("sound/se/coin03.mp3");
	
	GetGraphSizeF(graph, &coinW, &coinH);
	coinW *= GRAPH_EXTEND_RATE;
	coinH *= GRAPH_EXTEND_RATE;

	Load(r);

	for (int i = 0; i < coinMassY; i++)
	{
		for (int j = 0; j < coinMassX; j++)
		{
			coinArray[i][j].exist = true;
			coinArray[i][j].inScreen = true;
		}
	}
}

void CoinGroup::Update(Camera *camera)
{
	for (int j = 0; j < coinMassY; j++)
	{
		for (int k = 0; k < coinMassX; k++)
		{
			coinArray[j][k].Update(camera);
		}
	}
}

void CoinGroup::Draw(Camera *camera)
{
	for (int j = 0; j < coinMassY; j++)
	{
		for (int k = 0; k < coinMassX; k++)
		{
			if (coinArray[j][k].exist && coinArray[j][k].inScreen)
			{
				DrawExtendGraphF(coinArray[j][k].x - camera->cameraOffsetX, coinArray[j][k].y - camera->cameraOffsetY, coinArray[j][k].x + coinW - camera->cameraOffsetX, coinArray[j][k].y + coinH - camera->cameraOffsetY, graph, TRUE);
			}
		}
	}
}
