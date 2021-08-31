#include "MainTcpAnalyseMess.h"
#include "CrcCheck.h"
#include "MainTemporaryFunction.h"

#pragma region privateFunction
int ClaTcpAnalyseReceive::FunCheckMBAP(	byte reqMessNoHi, byte reqMessNoLo, byte ansMessNoHi, byte ansMessNoLo,
										byte ansProtocolHi, byte ansProtocolLo,int ansSize, byte ansAfterByteHi, byte ansAfterByteLo)
{
	//�ж���Ӧ���ĵ���������Ƿ�һ��
	if (ansMessNoHi != reqMessNoHi || ansMessNoLo != reqMessNoLo)
		return CodeMessNoNotSame;//���յ��ı����������кŲ�һ��

	//�ж���Ӧ���ĵ�Э���ʶ�Ƿ���ȷ��00 00��
	if (ansProtocolHi != 0 || ansProtocolLo != 0)
		return CodeProtocolNotTrue;//���յ��ı���Э���ʶ����

	//�ж���Ӧ���ĵ�����ֽ����Ƿ���ȷ
	if (ansSize - 6 != ClaTempFunc::FunCombineTwoChar(ansAfterByteHi, ansAfterByteLo))
		return CodeHeadAfterByteNotTrue;//���յ��ı��ı�ͷ����ֽ�������ȷ

	return CodeJustOK;
}

int ClaTcpAnalyseReceive::FunCheckMessForRead(vector<byte> requestMess, vector<byte> answerMess)
{
	//�ж���������Ӧ�Ĺ������Ƿ�һ��
	if (answerMess.at(7) != requestMess.at(7))
		return CodeFuncNotSame;//��Ӧ���ĵĹ�����������һ��

	//�ж���Ӧ���ֽ����Ƿ�������Ҫ��
	if (answerMess.size() < 10)
		return Code0103MessNotComplete;//(0x01/0x03)��Ӧ���ĵ��ֽ���������������

	unsigned short realRequestByte;
	unsigned short tmpByteCount;
	if (0x01 == answerMess.at(7))
	{
		realRequestByte = (ClaTempFunc::FunCombineTwoChar(requestMess.at(10), requestMess.at(11)) + 7) / 8;
		if (realRequestByte != (unsigned short)answerMess.at(8))
			return CodeDataCountNotSame;//(0x01/0x03)��Ӧ���ĵ������ֽ����������޷���Ӧ

		tmpByteCount = answerMess.at(8);
		if (tmpByteCount == answerMess.size() - 9)//������ȷ��
			return CodeReadCoilSuc;//������ȷ
		else
			return CodeDataByteNotSame;//(0x01/0x03)��Ӧ���ĵ������ֽ�������ȷ
	}
	else
	{
		realRequestByte = (ClaTempFunc::FunCombineTwoChar(requestMess.at(10), requestMess.at(11)) * 2);
		if (realRequestByte != (unsigned short)answerMess.at(8))
			return CodeDataCountNotSame;//(0x01/0x03)��Ӧ���ĵ������ֽ����������޷���Ӧ

		tmpByteCount = answerMess.at(8);
		if (tmpByteCount == answerMess.size() - 9)
			return CodeReadRegiSuc;//������ȷ
		else
			return CodeDataByteNotSame;//(0x01/0x03)��Ӧ���ĵ������ֽ�������ȷ
	}
}

int ClaTcpAnalyseReceive::FunCheckMessForWrite(vector<byte> requestMess, vector<byte> answerMess)
{
	//�ж���������Ӧ�Ĺ������Ƿ�һ��
	if (answerMess.at(7) != requestMess.at(7))
		return CodeFuncNotSame;//��Ӧ���ĵĹ�����������һ��

	if (12 == answerMess.size())
	{
		unsigned short reqAddr = ClaTempFunc::FunCombineTwoChar(requestMess.at(8), requestMess.at(9));
		unsigned short ansAddr = ClaTempFunc::FunCombineTwoChar(answerMess.at(8), answerMess.at(9));
		if (reqAddr != ansAddr)
			return Code0f10AddrNotSame;//(0x0f/0x10)��Ӧ���Ľ�����Ԥ����Ӧ��һ�£������ַ����Ӧ��ַ��һ�£�

		unsigned short reqCount = ClaTempFunc::FunCombineTwoChar(requestMess.at(10), requestMess.at(11));
		unsigned short ansCount = ClaTempFunc::FunCombineTwoChar(answerMess.at(10), answerMess.at(11));
		if (reqCount != ansCount)
			return Code0f10CountNotSame;//(0x0f/0x10)��Ӧ���Ľ�����Ԥ����Ӧ��һ�£�������������Ӧ������һ�£�

		if (0x0F == answerMess.at(7))
			return CodeWriteCoilSuc;//��Ȧ����д��ɹ�
		else
			return CodeWriteRegiSuc;//�Ĵ���д��ɹ�
	}
	else
	{
		return Code0f10LengthNotTrue;//(0x0f/0x10)��Ӧ���ĳ��Ȳ���������ȷ
	}
}

int ClaTcpAnalyseReceive::FunCheckAnotherFuncCode(vector<byte> requestMess, vector<byte> answerMess)
{
	if (0x01 == answerMess.at(7) || 0x03 == answerMess.at(7))
	{
		return FunCheckMessForRead(requestMess, answerMess);
	}
	else if (0x0F == answerMess.at(7) || 0x10 == answerMess.at(7))
	{
		return FunCheckMessForWrite(requestMess, answerMess);
	}
	else if (0x81 == answerMess.at(7) || 0x83 == answerMess.at(7) || 0x8F == answerMess.at(7) || 0x90 == answerMess.at(7))
	{
		byte tmpFunc = answerMess.at(7) - 0x80;
		if (tmpFunc != requestMess.at(7))
			return CodeErrorOne;// �����쳣���쳣��Ϊ��1

		return CodeErrorTwoOrThree;//�����쳣���쳣��Ϊ2��3
	}
	else
	{
		return CodeCantReadFunc;//�޷���ʶ��Ĺ�����
	}
}
#pragma endregion

int ClaTcpAnalyseReceive::FunTcpAnalyseMess(vector<byte> requestMess, vector<byte> answerMess)
{
	//�ж��Ƿ��з���������
	if (requestMess.empty())
		return CodeNoRequest;//δ��������ͽ��յ�����Ϣ������Ϣ�������

	//�жϽ��յ��ı��ĳ����Ƿ�����(��ͷ 6 + viceNo 1 + FuncCode 1)
	if (answerMess.size() < 9)
		return CodeLengthTooShot;// ���յ��ı��ĳ��Ȳ�����

	//�ж�MBAP�Ƿ���ȷ
	int ret = FunCheckMBAP(requestMess.at(0), requestMess.at(1), answerMess.at(0), answerMess.at(1),answerMess.at(2), answerMess.at(3), answerMess.size(), answerMess.at(4), answerMess.at(5));
	if (0 != ret)
		return ret;

	//�ж���Ӧ��վ�Ĵ�վ����Ƿ�������һ��
	if (answerMess.at(6) != requestMess.at(6))
		return CodeViceNoNotSmae;//��Ӧ������Ĵ�վ����ȷ

	return FunCheckAnotherFuncCode(requestMess, answerMess);
}