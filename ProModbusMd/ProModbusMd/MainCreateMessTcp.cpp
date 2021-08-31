#include "MainCreateMessTcp.h"

#pragma region privateFunction
void ClaCreateMessTcp::FunInsertMessNo(unsigned short messNo, vector<byte> &message)
{
	byte messNoHi = (messNo >> 8);
	byte messNoLo = (messNo);
	message.push_back(messNoHi);
	message.push_back(messNoLo);
}

void ClaCreateMessTcp::FunInsertProtocol(vector<byte> &message)
{
	message.push_back(0);
	message.push_back(0);
}

int ClaCreateMessTcp::FunCheckAndInsertAfterByte(byte funcCode, vector<byte> data, vector<byte> &message)
{
	if (0x01 == funcCode || 0x03 == funcCode)
	{
		message.push_back(0);
		message.push_back(6);
	}
	else if (0x0F == funcCode || 0x10 == funcCode)
	{
		byte afterByteHi = ((6 + 1 + data.size()) >> 8);
		byte afterByteLo = ((6 + 1 + data.size()));
		message.push_back(afterByteHi);
		message.push_back(afterByteLo);
	}
	else
		return CodeFuncCantRead;

	return CodeCreSuc;
}

int ClaCreateMessTcp::FunCheckAndInsertViceNo(byte viceNo, vector<byte> &message)
{
	if (viceNo < VICE_NO_MIN || viceNo >VICE_NO_MAX)
		return CodeViceErr;

	message.push_back(viceNo);

	return CodeCreSuc;
}

void ClaCreateMessTcp::FunInsertFuncCode(byte funcCode, vector<byte> &message)
{
	message.push_back(funcCode);
}

int ClaCreateMessTcp::FunCheckAndInsertBeginAddr(unsigned short beginAddr, vector<byte> &message)
{
	if (beginAddr < BEGIN_ADDR || beginAddr> END_ADDR)
		return CodeBegiAddrErr;

	byte beginAddrHi = (beginAddr >> 8);
	byte beginAddrLo = (beginAddr);
	message.push_back(beginAddrHi);
	message.push_back(beginAddrLo);

	return CodeCreSuc;
}

int ClaCreateMessTcp::FunCheckAndInsertDataCount(byte funcCode, unsigned short dataCount, vector<byte> &message)
{
	switch (funcCode)
	{
	case 0x01:
		if (dataCount < 1 || dataCount > READ_COIL_NUM_MAX)
			return CodeReadCoilNumErr;
		break;

	case 0x03:
		if (dataCount < 1 || dataCount > READ_REGI_NUM_MAX)
			return CodeReadRegiNumErr;
		break;

	case 0x0F:
		if (dataCount < 1 || dataCount > WRITE_COIL_NUM_MAX)
			return CodeWriteCoilNumErr;
		break;

	case 0x10:
		if (dataCount < 1 || dataCount > WRITE_REGI_NUM_MAX)
			return CodeWriteRegiNumErr;
		break;
	}

	byte dataCountHi = (dataCount >> 8);
	byte dataCountLo = (dataCount);
	message.push_back(dataCountHi);
	message.push_back(dataCountLo);

	return CodeCreSuc;
}

int ClaCreateMessTcp::FunCheckBeginAddrAddDataCount(unsigned short beginAddr, unsigned short dataCount)
{
	if (beginAddr > END_ADDR - dataCount)
		return CodeAddrAddCountErr;

	return CodeCreSuc;
}

int ClaCreateMessTcp::FunCheckAndInsertDataSize(byte funcCode, unsigned short dataCount, vector<byte> data)
{
	if (0x0F == funcCode)
	{
		unsigned short realRequestByte;
		realRequestByte = (dataCount + 7) / 8;
		if (realRequestByte != (unsigned short)(data.size()))
			return CodeCountByteNoSame;
		
		return CodeCountByteSame;
	}
	else if (0x10 == funcCode)
	{
		unsigned short realRequestByte;
		realRequestByte = dataCount * 2;
		if (realRequestByte != (unsigned short)(data.size()))
			return CodeCountByteNoSame;

		return CodeCountByteSame;
	}
	else
	{
		return CodeCreSuc;
	}
}

void ClaCreateMessTcp::FunInsertData(vector<byte> data, vector<byte> &message)
{
	message.push_back((byte)(data.size()));
	for (vector<byte>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		message.push_back(*iter);
	}
}
#pragma endregion

int ClaCreateMessTcp::FunCreateMessForTcp(unsigned short messNo, byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, vector<byte> data, vector<byte> &message)
{
	FunInsertMessNo(messNo, message);

	FunInsertProtocol(message);

	int ret = FunCheckAndInsertAfterByte(funcCode, data, message);
	if (0 != ret)
		return ret;

	ret = FunCheckAndInsertViceNo(viceNo, message);
	if (0 != ret)
		return ret;

	FunInsertFuncCode(funcCode, message); 

	
	ret = FunCheckAndInsertBeginAddr(beginAddr, message);
	if (0 != ret)
		return ret;

	ret = FunCheckAndInsertDataCount(funcCode, dataCount, message);
	if (0 != ret)
		return ret;

	ret = FunCheckBeginAddrAddDataCount(beginAddr, dataCount);
	if (0 != ret)
		return ret;

	ret = FunCheckAndInsertDataSize(funcCode, dataCount, data);
	if (CodeCountByteSame != ret)
		return ret;

	FunInsertData(data, message);

	return CodeCreSuc;
}