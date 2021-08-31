#include "ViceTcpAnalyseMess.h"

#pragma region privateFunction
int ClaTcpAnalyseRequest::FunCheckMBAP(byte protocolHi, byte protocolLo, byte afterByteHi, byte afterByteLo, int requestSize)
{
	//�ж���Ӧ���ĵ�Э���ʶ�Ƿ���ȷ��00 00��
	if (protocolHi != 0 || protocolLo != 0)
		return CodeProtocolNotTrue;//���յ��ı���Э���ʶ����

	//�ж���Ӧ���ĵ�����ֽ����Ƿ���ȷ
	if (requestSize - 6 != ClaTempFuncVice::FunCombineTwoChar(afterByteHi, afterByteLo))
		return CodeHeadAfterByteNotTrue;//���յ��ı��ı�ͷ����ֽ�������ȷ

	return CodeAnalyseSuc;
}

int ClaTcpAnalyseRequest::FunCheckDataCount(byte funcCode, unsigned short dataCount)
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

int ClaTcpAnalyseRequest::FunCheckMessLenIsTrue(vector<byte> requestMess)
{
	//�ж����������������ֽ����Ƿ���ʵ�ʱ��Ķ�Ӧ
	unsigned short realDataByte;
	switch (requestMess.at(7))
	{
	case 0x01:
		if (12 != requestMess.size())
			return Code01ReqLenNotTrue;
		break;

	case 0x03:
		if (12 != requestMess.size())
			return Code03ReqLenNotTrue;
		break;

	case 0x0F:
		realDataByte = (ClaTempFuncVice::FunCombineTwoChar(requestMess.at(10), requestMess.at(11)) + 7) / 8;
		if (12 + 1 + realDataByte != requestMess.size())
			return Code0FReqLenNotTrue;

		if (realDataByte != requestMess.at(12))
			return Code0FByteNotTrue;
		break;

	case 0x10:
		realDataByte = ClaTempFuncVice::FunCombineTwoChar(requestMess.at(10), requestMess.at(11)) * 2;
		if (12 + 1 + realDataByte != requestMess.size())
			return Code10ReqLenNotTrue;

		if (realDataByte != requestMess.at(12))
			return Code10ByteNotTrue;
		break;
	}
	return CodeAnalyseSuc;
}
#pragma endregion

int ClaTcpAnalyseRequest::FunTcpAnalyseMess(vector<byte> requestMess, byte viceNo)
{
	//�ж��������Ƿ������С�޶�
	if (requestMess.size() < 12)
		return CodeMessNotEnoughLong;

	int ret = FunCheckMBAP(requestMess.at(2), requestMess.at(3), requestMess.at(4), requestMess.at(5), requestMess.size());
	if (0 != ret)
		return ret;

	//�ж�����Ĵ�վ�ǲ����Լ�
	if (viceNo != requestMess.at(6))
		return CodeViceNoNotTrue;

	//�жϹ������Ƿ�Ϸ�
	if (requestMess.at(7) != 0x01 && requestMess.at(7) != 0x03 && requestMess.at(7) != 0x0F && requestMess.at(7) != 0x10)
		return CodeCantReadFunc;//�޷���ʶ��Ĺ�����(!01,03,0F,10)

	//�ж������ַ�Ƿ�Ϸ�
	unsigned short beginAddr = ClaTempFuncVice::FunCombineTwoChar(requestMess.at(8), requestMess.at(9));
	if (beginAddr < BEGIN_ADDR || beginAddr> END_ADDR)
		return CodeBeginAddrErr;

	//�ж����������Ƿ�Ϸ�
	unsigned short dataCount = ClaTempFuncVice::FunCombineTwoChar(requestMess.at(10), requestMess.at(11));
	ret = FunCheckDataCount(requestMess.at(7), dataCount);
	if (0 != ret)
		return ret;

	//�жϵ�ַ�������Ƿ�Ϸ�
	if (beginAddr > END_ADDR - dataCount)
		return CodeAddrAndCountNotTrue;

	ret = FunCheckMessLenIsTrue(requestMess);
	if (0 != ret)
		return ret;

	return CodeAnalyseSuc;
}