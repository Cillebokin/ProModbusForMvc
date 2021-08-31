#include "ViceCreateMessRtu.h"

#pragma region privateFunction
void ClaCreateMessRtu::FunMixCrcAnsw(vector<byte> &answer)
{
	ClaCrcCheck *myClaCrcCheck = new ClaCrcCheck();
	unsigned int crcRealMess = myClaCrcCheck->ModbusCrc16(answer, answer.size());
	delete myClaCrcCheck;

	answer.push_back((byte)(crcRealMess >> 8));
	answer.push_back((byte)(crcRealMess));
}

void ClaCreateMessRtu::FunMixErrorAnsw(byte errCode, vector<byte> &answer)
{
	answer[1] = (byte)(answer.at(1) + 0x80);

	answer.push_back(errCode);

	FunMixCrcAnsw(answer);
}

void ClaCreateMessRtu::FunMixCommonAnswForRead(vector<byte> data, vector<byte> &answer)
{
	answer.push_back((byte)(data.size()));

	for (vector<byte>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		answer.push_back(*iter);
	}

	FunMixCrcAnsw(answer);
}

void ClaCreateMessRtu::FunMixCommonAnswForWrite(unsigned short beginAddr, unsigned short dataCount, vector<byte> &answer)
{
	byte beginAddrHi = (beginAddr >> 8);
	byte beginAddrLo = (beginAddr);
	answer.push_back(beginAddrHi);
	answer.push_back(beginAddrLo);

	byte dataCountHi = (dataCount >> 8);
	byte dataCountLo = (dataCount);
	answer.push_back(dataCountHi);
	answer.push_back(dataCountLo);

	FunMixCrcAnsw(answer);
}
#pragma endregion

void ClaCreateMessRtu::FunCreateAnswForRtu(int analyseCode, byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, vector<byte> data, vector<byte> &answer)
{
	answer.push_back(viceNo);
	answer.push_back(funcCode);

	//异常响应01
	if (analyseCode == CodeCantReadFunc)
	{
		FunMixErrorAnsw(1, answer);
		return;
	}

	//异常响应02
	if (analyseCode == CodeBeginAddrErr || analyseCode == CodeAddrAndCountNotTrue)
	{
		FunMixErrorAnsw(2, answer);
		return;
	}

	//异常响应03
	if (analyseCode == CodeReadCoilNumErr || analyseCode == CodeReadRegiNumErr || analyseCode == CodeWriteCoilNumErr || analyseCode == CodeWriteRegiNumErr)
	{
		FunMixErrorAnsw(3, answer);
		return;
	}

	//正常响应
	if (analyseCode == CodeAnalyseSuc)
	{
		if (0x01 == funcCode || 0x03 == funcCode)
		{
			FunMixCommonAnswForRead(data, answer);
		}
		else if (0x0F == funcCode || 0x10 == funcCode)
		{
			FunMixCommonAnswForWrite(beginAddr, dataCount, answer);
		}
	}
}