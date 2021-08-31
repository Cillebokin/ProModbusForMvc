#include "MainCreateMessRtu.h"

#pragma region privateFunction
int ClaCreateMessRtu::FunCheckAndInsertViceNo(byte viceNo, vector<byte> &message)
{
	if (viceNo < VICE_NO_MIN || viceNo >VICE_NO_MAX)
		return CodeViceErr;

	message.push_back(viceNo);

	return CodeCreSuc;
}

int ClaCreateMessRtu::FunCheckAndInsertFuncCode(byte funcCode, vector<byte> &message)
{
	if (!(0x01 == funcCode || 0x03 == funcCode || 0x0F == funcCode || 0x10 == funcCode))
		return CodeFuncCantRead;

	message.push_back(funcCode);

	return CodeCreSuc;
}

int ClaCreateMessRtu::FunCheckAndInsertBeginAddr(unsigned short beginAddr, vector<byte> &message)
{
	if (beginAddr < BEGIN_ADDR || beginAddr> END_ADDR)
		return CodeBegiAddrErr;

	byte beginAddrHi = (beginAddr >> 8);
	byte beginAddrLo = (beginAddr);
	message.push_back(beginAddrHi);
	message.push_back(beginAddrLo);

	return CodeCreSuc;
}

int ClaCreateMessRtu::FunCheckAndInsertDataCount(byte funcCode, unsigned short dataCount, vector<byte> &message)
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

int ClaCreateMessRtu::FunCheckBeginAddrAddDataCount(unsigned short beginAddr, unsigned short dataCount)
{
	if (beginAddr > END_ADDR - dataCount)
		return CodeAddrAddCountErr;

	return CodeCreSuc;
}

void ClaCreateMessRtu::FunInsertCrc(vector<byte> &message)
{
	ClaCrcCheck *myClaCrcCheck = new ClaCrcCheck();
	unsigned int crcRealMess = myClaCrcCheck->ModbusCrc16(message, message.size());
	delete myClaCrcCheck;

	message.push_back((byte)(crcRealMess >> 8));
	message.push_back((byte)(crcRealMess));
}

int ClaCreateMessRtu::FunCheckAndInsertDataSize(byte funcCode, unsigned short dataCount, vector<byte> data, vector<byte> &message)
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
		FunInsertCrc(message);
		return CodeCreSuc;
	}
}

void ClaCreateMessRtu::FunInsertData(vector<byte> data, vector<byte> &message)
{
	message.push_back((byte)(data.size()));
	for (vector<byte>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		message.push_back(*iter);
	}
}
#pragma endregion

int ClaCreateMessRtu::FunCreateMessForRtu(byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, vector<byte> data, vector<byte> &message)
{
	int ret = FunCheckAndInsertViceNo(viceNo, message);
	if (0 != ret)
		return ret;

	ret = FunCheckAndInsertFuncCode(funcCode, message);
	if (0 != ret)
		return ret;

	ret = FunCheckAndInsertBeginAddr(beginAddr, message);
	if (0 != ret)
		return ret;

	ret = FunCheckAndInsertDataCount(funcCode, dataCount, message);
	if (0 != ret)
		return ret;

	ret = FunCheckBeginAddrAddDataCount( beginAddr, dataCount);
	if (0 != ret)
		return ret;

	ret = FunCheckAndInsertDataSize(funcCode, dataCount, data, message);
	if (CodeCountByteSame != ret)
		return ret;

	FunInsertData(data, message);

	FunInsertCrc(message);

	return CodeCreSuc;
}