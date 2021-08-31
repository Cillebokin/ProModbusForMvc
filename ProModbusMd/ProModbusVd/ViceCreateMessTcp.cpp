#include "ViceCreateMessTcp.h"

#pragma region privateFunction
void ClaCreateMessTcp::FunMixErrorAnsw(byte viceNo, byte funcCode, byte errCode, vector<byte> &answer)
{
	answer.push_back(0);
	answer.push_back(3);

	answer.push_back(viceNo);

	answer.push_back((byte)(funcCode + 0x80));

	answer.push_back(errCode);
}

void ClaCreateMessTcp::FunMixCommonAnswForRead(byte viceNo, byte funcCode, vector<byte> data, vector<byte> &answer)
{
	answer.push_back((byte)0);
	answer.push_back((byte)(2 + 1 + data.size()));

	answer.push_back(viceNo);
	answer.push_back(funcCode);

	answer.push_back((byte)(data.size()));
	for (vector<byte>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		answer.push_back(*iter);
	}
}

void ClaCreateMessTcp::FunMixCommonAnswForWrite(byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, vector<byte> &answer)
{
	answer.push_back((byte)0);
	answer.push_back((byte)6);

	answer.push_back(viceNo);
	answer.push_back(funcCode);

	byte beginAddrHi = (beginAddr >> 8);
	byte beginAddrLo = (beginAddr);
	answer.push_back(beginAddrHi);
	answer.push_back(beginAddrLo);

	byte dataCountHi = (dataCount >> 8);
	byte dataCountLo = (dataCount);
	answer.push_back(dataCountHi);
	answer.push_back(dataCountLo);
}
#pragma endregion

void ClaCreateMessTcp::FunCreateAnswForTcp(int analyseCode, unsigned short messNo, byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, vector<byte> data, vector<byte> &answer)
{
	byte messNoHi = (messNo >> 8);
	byte messNoLo = (messNo);
	answer.push_back(messNoHi);
	answer.push_back(messNoLo);

	answer.push_back(0);
	answer.push_back(0);

	//异常响应01
	if (analyseCode == CodeCantReadFunc)
	{
		FunMixErrorAnsw(viceNo, funcCode, 1, answer);
		return;
	}

	//异常响应02
	if (analyseCode == CodeBeginAddrErr || analyseCode == CodeAddrAndCountNotTrue)
	{
		FunMixErrorAnsw(viceNo, funcCode, 2, answer);
		return;
	}

	//异常响应03
	if (analyseCode == CodeReadCoilNumErr || analyseCode == CodeReadRegiNumErr || analyseCode == CodeWriteCoilNumErr || analyseCode == CodeWriteRegiNumErr)
	{
		FunMixErrorAnsw(viceNo, funcCode, 3, answer);
		return;
	}

	//正常响应
	if (analyseCode == CodeAnalyseSuc)
	{
		if (0x01 == funcCode || 0x03 == funcCode)
		{
			FunMixCommonAnswForRead(viceNo, funcCode,data, answer);
		}
		else if (0x0F == funcCode || 0x10 == funcCode)
		{
			FunMixCommonAnswForWrite(viceNo, funcCode, beginAddr, dataCount, answer);
		}
	}
}