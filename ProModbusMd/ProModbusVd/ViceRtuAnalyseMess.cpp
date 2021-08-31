#include "ViceRtuAnalyseMess.h"

#pragma region privateFunction
int ClaRtuAnalyseRequest::FunCheckCrc(vector<byte> requestMess)
{
	//先计算此报文的crc校验码是否正确
	ClaCrcCheck *myClaCrcCheck = new ClaCrcCheck();
	unsigned int crcMess = (requestMess.at(requestMess.size() - 2) << 8) | requestMess.at(requestMess.size() - 1);
	requestMess.pop_back();
	requestMess.pop_back();
	unsigned int crcRealMess = myClaCrcCheck->ModbusCrc16(requestMess, requestMess.size());
	delete myClaCrcCheck;

	if (crcMess != crcRealMess)
		return CodeCrcError;

	return CodeAnalyseSuc;
}

int ClaRtuAnalyseRequest::FunCheckDataCount(byte funcCode, unsigned short dataCount)
{
	//判断请求数量是否合法
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
	return CodeAnalyseSuc;
}

int ClaRtuAnalyseRequest::FunCheckMessLenIsTrue(vector<byte> requestMess)
{
	unsigned short realDataByte;
	switch (requestMess.at(1))
	{
	case 0x01:
		if (8 != requestMess.size())
			return Code01ReqLenNotTrue;
		break;

	case 0x03:
		if (8 != requestMess.size())
			return Code03ReqLenNotTrue;
		break;

	case 0x0F:
		realDataByte = (ClaTempFuncVice::FunCombineTwoChar(requestMess.at(4), requestMess.at(5)) + 7) / 8;
		if (8 + 1 + realDataByte != requestMess.size())
			return Code0FReqLenNotTrue;

		if (realDataByte != requestMess.at(6))
			return Code0FByteNotTrue;
		break;

	case 0x10:
		realDataByte = ClaTempFuncVice::FunCombineTwoChar(requestMess.at(4), requestMess.at(5)) * 2;
		if (8 + 1 + realDataByte != requestMess.size())
			return Code10ReqLenNotTrue;

		if (realDataByte != requestMess.at(6))
			return Code10ByteNotTrue;
		break;
	}
	return CodeAnalyseSuc;
}
#pragma endregion

int ClaRtuAnalyseRequest::FunRtuAnalyseMess(vector<byte> requestMess, byte viceNo)
{
	if (requestMess.size() < 8)
		return CodeMessNotEnoughLong;

	int ret = FunCheckCrc(requestMess);
	if (0 != ret)
		return ret;

	//判断请求的从站是不是自己
	if (viceNo != requestMess.at(0))
		return CodeViceNoNotTrue;

	//判断功能码是否合法
	if (requestMess.at(1) != 0x01 && requestMess.at(1) != 0x03 && requestMess.at(1) != 0x0F && requestMess.at(1) != 0x10)
		return CodeCantReadFunc;//无法被识别的功能码(!01,03,0F,10)

	//判断请求地址是否合法
	unsigned short beginAddr = ClaTempFuncVice::FunCombineTwoChar(requestMess.at(2), requestMess.at(3));
	if (beginAddr < BEGIN_ADDR || beginAddr> END_ADDR)
		return CodeBeginAddrErr;

	unsigned short dataCount = ClaTempFuncVice::FunCombineTwoChar(requestMess.at(4), requestMess.at(5));
	ret = FunCheckDataCount(requestMess.at(1), dataCount);
	if (0 != ret)
		return ret;

	//判断地址加数量是否合法
	if (beginAddr > END_ADDR - dataCount)
		return CodeAddrAndCountNotTrue;

	//判断请求数量和数据字节数是否与实际报文对应
	ret = FunCheckMessLenIsTrue(requestMess);
	if (0 != ret)
		return ret;

	return CodeAnalyseSuc;
}