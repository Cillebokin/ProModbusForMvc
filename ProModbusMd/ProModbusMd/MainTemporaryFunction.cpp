#include "MainTemporaryFunction.h"

//������Char�ϲ���һ��Short
unsigned short ClaTempFunc::FunCombineTwoChar(byte preNum, byte afterNum)
{
	unsigned short tmp = afterNum;
	tmp = tmp | (preNum << 8);
	return tmp;
}