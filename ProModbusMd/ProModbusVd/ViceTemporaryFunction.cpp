#include "ViceTemporaryFunction.h"

//������Char�ϲ���һ��Short
unsigned short ClaTempFuncVice::FunCombineTwoChar(byte preNum, byte afterNum)
{
	unsigned short tmp = afterNum;
	tmp = tmp | (preNum << 8);
	return tmp;
}