#include "MainRtuAnalyseMess.h"
#include "CrcCheck.h"
#include "MainTemporaryFunction.h"

#pragma region privateFunction
int ClaRtuAnalyseReceive::FunCheckCrc(vector<byte> receiveMess)
{
	//�ȼ���˱��ĵ�crcУ�����Ƿ���ȷ
	ClaCrcCheck *myClaCrcCheck = new ClaCrcCheck();
	unsigned int crcMess = (receiveMess.at(receiveMess.size() - 2) << 8) | receiveMess.at(receiveMess.size() - 1);
	receiveMess.pop_back();
	receiveMess.pop_back();
	unsigned int crcRealMess = myClaCrcCheck->ModbusCrc16(receiveMess, receiveMess.size());
	delete myClaCrcCheck;
	if (crcMess != crcRealMess)
		return CodeCrcError;

	return CodeJustOK;
}

int ClaRtuAnalyseReceive::FunCheckMessForRead(vector<byte> requestMess, vector<byte> receiveMess)
{
	//�ж���������Ӧ�Ĺ������Ƿ�һ��
	if (receiveMess.at(1) != requestMess.at(1))
		return CodeFuncNotSame;//��Ӧ���ĵĹ�����������һ��

	if (0x01 == receiveMess.at(1))
	{
		//�ж���Ӧ���ֽ����Ƿ�������Ҫ��
		if (receiveMess.size() < 6)
			return Code0103MessNotComplete;//(0x01/0x03)��Ӧ���ĵ��ֽ���������������

		//���ж���Ӧ���ĵ��ֽ����Ƿ�������������Ӧ
		unsigned short realRequestByte = (ClaTempFunc::FunCombineTwoChar(requestMess.at(4), requestMess.at(5)) + 7) / 8;
		if (realRequestByte != (unsigned short)receiveMess.at(2))
			return CodeDataCountNotSame;//(0x01/0x03)��Ӧ���ĵ������ֽ����������޷���Ӧ

		if (receiveMess.at(2) == receiveMess.size() - 5)
			return CodeReadCoilSuc;//������ȷ
		else
			return CodeDataByteNotSame;//(0x01/0x03)��Ӧ���ĵ������ֽ�������ȷ
	}
	else
	{
		//�ж���Ӧ���ֽ����Ƿ�������Ҫ��
		if (receiveMess.size() < 7)
			return Code0103MessNotComplete;//(0x01/0x03)��Ӧ���ĵ��ֽ���������������

		//���ж���Ӧ���ĵ��ֽ����Ƿ�������������Ӧ
		unsigned short realRequestByte = (ClaTempFunc::FunCombineTwoChar(requestMess.at(4), requestMess.at(5)) * 2);
		if (realRequestByte != (unsigned short)receiveMess.at(2))
			return CodeDataCountNotSame;//(0x01/0x03)��Ӧ���ĵ������ֽ����������޷���Ӧ

		if (receiveMess.at(2) == receiveMess.size() - 5)
			return CodeReadRegiSuc;//������ȷ
		else
			return CodeDataByteNotSame;//(0x01/0x03)��Ӧ���ĵ������ֽ�������ȷ
	}
}

int ClaRtuAnalyseReceive::FunCheckMessForWrite(vector<byte> requestMess, vector<byte> receiveMess)
{
	//�ж���������Ӧ�Ĺ������Ƿ�һ��
	if (receiveMess.at(1) != requestMess.at(1))
		return CodeFuncNotSame;//��Ӧ���ĵĹ�����������һ��

	if (8 == receiveMess.size())
	{
		unsigned short reqAddr = ClaTempFunc::FunCombineTwoChar(requestMess.at(2), requestMess.at(3));
		unsigned short ansAddr = ClaTempFunc::FunCombineTwoChar(receiveMess.at(2), receiveMess.at(3));
		if (reqAddr != ansAddr)
			return Code0f10AddrNotSame;//(0x0f/0x10)��Ӧ���Ľ�����Ԥ����Ӧ��һ�£������ַ����Ӧ��ַ��һ�£�

		unsigned short reqCount = ClaTempFunc::FunCombineTwoChar(requestMess.at(4), requestMess.at(5));
		unsigned short ansCount = ClaTempFunc::FunCombineTwoChar(receiveMess.at(4), receiveMess.at(5));
		if (reqCount != ansCount)
			return Code0f10CountNotSame;//(0x0f/0x10)��Ӧ���Ľ�����Ԥ����Ӧ��һ�£�������������Ӧ������һ�£�

		if (0x0F == receiveMess.at(1))
			return CodeWriteCoilSuc;//��Ȧ����д��ɹ�
		else
			return CodeWriteRegiSuc;//�Ĵ�������д��ɹ�
	}
	else
	{
		return Code0f10LengthNotTrue;//(0x0f/0x10)��Ӧ���ĳ��Ȳ���������ȷ
	}
}

int ClaRtuAnalyseReceive::FunCheckAnotherFuncCode(vector<byte> requestMess, vector<byte> receiveMess)
{
	if (0x01 == receiveMess.at(1) || 0x03 == receiveMess.at(1))
	{
		return FunCheckMessForRead(requestMess, receiveMess);
	}
	else if (0x0F == receiveMess.at(1) || 0x10 == receiveMess.at(1))
	{
		return FunCheckMessForWrite(requestMess, receiveMess);
	}
	else if (0x81 == receiveMess.at(1) || 0x83 == receiveMess.at(1) || 0x8F == receiveMess.at(1) || 0x90 == receiveMess.at(1))
	{
		byte tmpFunc = receiveMess.at(1) - 0x80;
		if (tmpFunc != requestMess.at(1))
			return CodeErrorOne;// �����쳣���쳣��Ϊ��1

		return CodeErrorTwoOrThree;//�����쳣���쳣��Ϊ2��3
	}
	else
	{
		return CodeCantReadFunc;//�޷���ʶ��Ĺ�����
	}
}
#pragma endregion

int ClaRtuAnalyseReceive::FunRtuAnalyseMess(vector<byte> requestMess, vector<byte> receiveMess)
{
	//�ж��Ƿ��з���������
	if (requestMess.empty())
		return CodeNoRequest;//δ��������ͽ��յ�����Ϣ������Ϣ�������

	//�жϽ��յ��ı��ĳ����Ƿ�����
	if (receiveMess.size() < 5)
		return CodeLengthTooShot;// ���յ��ı��ĳ��Ȳ�����

	//����CRC
	if(CodeJustOK != FunCheckCrc(receiveMess))
		return CodeCrcError;

	//�ж���Ӧ��վ�Ĵ�վ����Ƿ�������һ��
	if (receiveMess.at(0) != requestMess.at(0))
		return CodeViceNoNotSmae;//��Ӧ������Ĵ�վ����ȷ

	return FunCheckAnotherFuncCode(requestMess, receiveMess);
}