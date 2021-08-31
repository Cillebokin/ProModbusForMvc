#include "MainTemporaryFunction.h"

//将两个Char合并成一个Short
unsigned short ClaTempFunc::FunCombineTwoChar(byte preNum, byte afterNum)
{
	unsigned short tmp = afterNum;
	tmp = tmp | (preNum << 8);
	return tmp;
}