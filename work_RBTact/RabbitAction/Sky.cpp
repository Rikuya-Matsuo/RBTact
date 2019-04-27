#include "Sky.h"

void Sky::Load(int replayCnt)
{
	if (!replayCnt)
	{
		LoadDivGraph("image/skychip.png", 12, 4, 3, 32, 32, graph);
	}
}

void Sky::Draw(Camera *camera)
{
    int yMax = 80;
    int xMax = 550;

	int cloudIntervalX = 30;
    int cloudIntervalY = 25;
    for (int y = 0; y < yMax; y++)
    {
        int chipNumber;
        for (int x = 0; x < xMax; x++)
        {
            if (y < (yMax / 10) * 5)
            {
                if (x % cloudIntervalX < 4 && (y < (yMax / 10) * 6 && y >(yMax / 10) * 2 && y % cloudIntervalY < 2))
                {
                    if (!(y % cloudIntervalY))
                    {
                        chipNumber = 8 + (x % cloudIntervalX);
                    }
                    else if (y % cloudIntervalY == 1)
                    {
                        chipNumber = 4 + (x % cloudIntervalX);
                    }
                }
                else
                {
                    chipNumber = 0;
                }
            }
            else if (y == (yMax / 10) * 5)
            {
                chipNumber = 1;
            }
            else if (y > (yMax / 10) * 5 && y < (yMax / 10) * 7)
            {
                chipNumber = 2;
            }
            else
            {
                chipNumber = 3;
            }

			//��ʓ��Ƃ��̕t�߂̃`�b�v�����`��
			bool drawFlag = (x * SKYCHIP_SIZE_W + OFFSET_X > camera->cameraOffsetX * DISTANT_VIEW_SCROLL_RATE - DRAW_MARGIN &&
							x * SKYCHIP_SIZE_W + OFFSET_X < camera->cameraOffsetX * DISTANT_VIEW_SCROLL_RATE + WINDOW_WIDTH + DRAW_MARGIN) &&
							(WINDOW_HEIGHT - (y + 1) * SKYCHIP_SIZE_H > camera->cameraOffsetY - DRAW_MARGIN &&
							WINDOW_HEIGHT - (y + 1) * SKYCHIP_SIZE_H < camera->cameraOffsetY + WINDOW_HEIGHT + DRAW_MARGIN);
			if (drawFlag)
            {
				DrawGraph(x * SKYCHIP_SIZE_W - (int)(camera->cameraOffsetX * DISTANT_VIEW_SCROLL_RATE) + (int)OFFSET_X, WINDOW_HEIGHT - (y + 1) * SKYCHIP_SIZE_H - (int)camera->cameraOffsetY,
							graph[chipNumber], FALSE);
			}
			//����ȏ�E�ɕ`�悷��K�v���Ȃ���΃u���[�N���Ď��̗��
			else if (x * SKYCHIP_SIZE_W + OFFSET_X > camera->cameraOffsetX * DISTANT_VIEW_SCROLL_RATE + WINDOW_WIDTH + DRAW_MARGIN)
			{
				break;
			}
        }
    }
}

void Sky::Draw4Rule()
{
	//�摜�̏�[����ʏc���𒴂���Ȃ�`��I��
	for (int i = 0; i * SKYCHIP_SIZE_H < WINDOW_HEIGHT; i++)
	{
		//�摜�̍��[����ʉ����𒴂���Ȃ�`��I��
		for (int j = 0; j * SKYCHIP_SIZE_W < WINDOW_WIDTH; j++)
		{
			DrawGraph(j * SKYCHIP_SIZE_W, i * SKYCHIP_SIZE_H, graph[STARCHIP_NUM], FALSE);
		}
	}
}
