#include "ViceRtuAnalyseMess.h"

#pragma region privateFunction
int ClaRtuAnalyseRequest::FunCheckCrc(vector<byte> requestMess)
{
	//�ȼ���˱��ĵ�crcУ�����Ƿ���ȷ
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
	//�ж����������Ƿ�Ϸ�
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

	//�ж�����Ĵ�վ�ǲ����Լ�
	if (viceNo != requestMess.at(0))
		return CodeViceNoNotTrue;

	//�жϹ������Ƿ�Ϸ�
	if (requestMess.at(1) != 0x01 && requestMess.at(1) != 0x03 && requestMess.at(1) != 0x0F && requestMess.at(1) != 0x10)
		return CodeCantReadFunc;//�޷���ʶ��Ĺ�����(!01,03,0F,10)

	//�ж������ַ�Ƿ�Ϸ�
	unsigned short beginAddr = ClaTempFuncVice::FunCombineTwoChar(requestMess.at(2), requestMess.at(3));
	if (beginAddr < BEGIN_ADDR || beginAddr> END_ADDR)
		return CodeBeginAddrErr;

	unsigned short dataCount = ClaTempFuncVice::FunCombineTwoChar(requestMess.at(4), requestMess.at(5));
	ret = FunCheckDataCount(requestMess.at(1), dataCount);
	if (0 != ret)
		return ret;

	//�жϵ�ַ�������Ƿ�Ϸ�
	if (beginAddr > END_ADDR - dataCount)
		return CodeAddrAndCountNotTrue;

	//�ж����������������ֽ����Ƿ���ʵ�ʱ��Ķ�Ӧ
	ret = FunCheckMessLenIsTrue(requestMess);
	if (0 != ret)
		return ret;

	return CodeAnalyseSuc;
}