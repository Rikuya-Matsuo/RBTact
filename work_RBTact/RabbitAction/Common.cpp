#include "Common.h"

bool ProgramControl::CheckHitGoNext(XINPUT_STATE * pad)
{
	bool ret = false;

	if (CheckHitKey(KEY_INPUT_RETURN) || pad->Buttons[XINPUT_BUTTON_START] || pad->Buttons[XINPUT_BUTTON_A])
	{
		ret = true;
	}

	return ret;
}

bool ProgramControl::CheckHitEsc()
{
	bool ret = false;
	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ret = true;
	}

	return ret;
}

bool ProgramControl::CheckHitDebugCmd(XINPUT_STATE *pad)
{
	bool ret = false;
	if ((CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_BACK)) || (pad->Buttons[XINPUT_BUTTON_RIGHT_THUMB]))
	{
		ret = true;
	}

	return ret;
}

void SetBoolArray(bool *flg, int element, bool TrueOrFalse)
{
	for (int i = 0; i < element; i++)
	{
		flg[i] = TrueOrFalse;
	}
}

void SwitchFlag(bool *ToFalse, bool *ToTrue)
{
	*ToFalse = false;
	*ToTrue = true;
}

void ReverseFlag(bool *flg)
{
	if (*flg)
	{
		*flg = false;
	}
	else
	{
		*flg = true;
	}
}

int CountBoolArray(bool * flg, int element, bool search)
{
	int ret = 0;

	for (int i = 0; i < element; i++)
	{
		if (flg[i] == search)
		{
			ret++;
		}
	}

	return ret;
}

float Lerp(float x, float y, float LerpRate)
{
	float ret = (y - x) * LerpRate;

	return ret;
}

float Cos2Theta(float cos)
{
	float ret;

	ret = acosf(cos);

	return ret;
}

char * SetAllNull(char * str, int length)
{
	for (int i = 0; i < length; i++)
	{
		str[i] = '\0';
	}
	return str;
}

void DrawHow2GoNext(int handle)
{
	int w, h;
	int divideRate = 2;
	GetGraphSize(handle, &w, &h);
	DrawExtendGraph(WINDOW_WIDTH - (w / divideRate), 0, WINDOW_WIDTH, h / divideRate, handle, TRUE);
}
