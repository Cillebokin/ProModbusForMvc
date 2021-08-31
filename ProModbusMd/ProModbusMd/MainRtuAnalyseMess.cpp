#include "MainRtuAnalyseMess.h"
#include "CrcCheck.h"
#include "MainTemporaryFunction.h"

#pragma region privateFunction
int ClaRtuAnalyseReceive::FunCheckCrc(vector<byte> receiveMess)
{
	//先计算此报文的crc校验码是否正确
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
	//判断请求与响应的功能码是否一致
	if (receiveMess.at(1) != requestMess.at(1))
		return CodeFuncNotSame;//响应报文的功能码与请求不一致

	if (0x01 == receiveMess.at(1))
	{
		//判断响应的字节数是否符合最低要求
		if (receiveMess.size() < 6)
			return Code0103MessNotComplete;//(0x01/0x03)响应报文的字节数不正常或不完整

		//先判断响应报文的字节数是否和请求的数量对应
		unsigned short realRequestByte = (ClaTempFunc::FunCombineTwoChar(requestMess.at(4), requestMess.at(5)) + 7) / 8;
		if (realRequestByte != (unsigned short)receiveMess.at(2))
			return CodeDataCountNotSame;//(0x01/0x03)响应报文的数据字节数与请求无法对应

		if (receiveMess.at(2) == receiveMess.size() - 5)
			return CodeReadCoilSuc;//解析正确
		else
			return CodeDataByteNotSame;//(0x01/0x03)响应报文的数据字节数不正确
	}
	else
	{
		//判断响应的字节数是否符合最低要求
		if (receiveMess.size() < 7)
			return Code0103MessNotComplete;//(0x01/0x03)响应报文的字节数不正常或不完整

		//先判断响应报文的字节数是否和请求的数量对应
		unsigned short realRequestByte = (ClaTempFunc::FunCombineTwoChar(requestMess.at(4), requestMess.at(5)) * 2);
		if (realRequestByte != (unsigned short)receiveMess.at(2))
			return CodeDataCountNotSame;//(0x01/0x03)响应报文的数据字节数与请求无法对应

		if (receiveMess.at(2) == receiveMess.size() - 5)
			return CodeReadRegiSuc;//解析正确
		else
			return CodeDataByteNotSame;//(0x01/0x03)响应报文的数据字节数不正确
	}
}

int ClaRtuAnalyseReceive::FunCheckMessForWrite(vector<byte> requestMess, vector<byte> receiveMess)
{
	//判断请求与响应的功能码是否一致
	if (receiveMess.at(1) != requestMess.at(1))
		return CodeFuncNotSame;//响应报文的功能码与请求不一致

	if (8 == receiveMess.size())
	{
		unsigned short reqAddr = ClaTempFunc::FunCombineTwoChar(requestMess.at(2), requestMess.at(3));
		unsigned short ansAddr = ClaTempFunc::FunCombineTwoChar(receiveMess.at(2), receiveMess.at(3));
		if (reqAddr != ansAddr)
			return Code0f10AddrNotSame;//(0x0f/0x10)响应报文解析与预期响应不一致（请求地址与响应地址不一致）

		unsigned short reqCount = ClaTempFunc::FunCombineTwoChar(requestMess.at(4), requestMess.at(5));
		unsigned short ansCount = ClaTempFunc::FunCombineTwoChar(receiveMess.at(4), receiveMess.at(5));
		if (reqCount != ansCount)
			return Code0f10CountNotSame;//(0x0f/0x10)响应报文解析与预期响应不一致（请求数量与响应数量不一致）

		if (0x0F == receiveMess.at(1))
			return CodeWriteCoilSuc;//线圈数据写入成功
		else
			return CodeWriteRegiSuc;//寄存器数据写入成功
	}
	else
	{
		return Code0f10LengthNotTrue;//(0x0f/0x10)响应报文长度不完整或不正确
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
			return CodeErrorOne;// 请求异常！异常码为：1

		return CodeErrorTwoOrThree;//请求异常！异常码为2或3
	}
	else
	{
		return CodeCantReadFunc;//无法被识别的功能码
	}
}
#pragma endregion

int ClaRtuAnalyseReceive::FunRtuAnalyseMess(vector<byte> requestMess, vector<byte> receiveMess)
{
	//判断是否有发送请求报文
	if (requestMess.empty())
		return CodeNoRequest;//未发送请求就接收到了消息！此消息不予解析

	//判断接收到的报文长度是否正常
	if (receiveMess.size() < 5)
		return CodeLengthTooShot;// 接收到的报文长度不正常

	//计算CRC
	if(CodeJustOK != FunCheckCrc(receiveMess))
		return CodeCrcError;

	//判断响应从站的从站编号是否与请求一致
	if (receiveMess.at(0) != requestMess.at(0))
		return CodeViceNoNotSmae;//响应此请求的从站不正确

	return FunCheckAnotherFuncCode(requestMess, receiveMess);
}