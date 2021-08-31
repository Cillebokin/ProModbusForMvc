#include "stdafx.h"
#include "FunctionForTest.h"

byte ClaFuncForTest::FunCharToHex(byte tmpChar)
{
	byte reChar;

	if (tmpChar >= 'a' && tmpChar <= 'z')
		reChar = tmpChar - 'a' + 10;
	else if (tmpChar >= 'A' && tmpChar <= 'Z')
		reChar = tmpChar - 'A' + 10;
	else
		reChar = tmpChar - '0';

	return reChar;
}

void ClaFuncForTest::FunStringToVectorChar(LPTSTR reStr, std::vector<byte> &tmpVec)
{
	for (int j = 0; j < strlen(reStr); j = j + 2)
	{
		byte tmpChar1 = (byte)FunCharToHex(reStr[j]);
		byte tmpChar2 = (byte)FunCharToHex(reStr[j + 1]);

		byte resulChar = tmpChar2 | (tmpChar1 << 4);

		tmpVec.push_back(resulChar);
	}
}

//将两个Char合并成一个Short
unsigned short ClaFuncForTest::FunCombineTwoChar(byte preNum, byte afterNum)
{
	unsigned short tmp = afterNum;
	tmp = tmp | (preNum << 8);
	return tmp;
}