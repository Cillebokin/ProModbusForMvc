#include "MainTcpAnalyseMess.h"
#include "CrcCheck.h"
#include "MainTemporaryFunction.h"

#pragma region privateFunction
int ClaTcpAnalyseReceive::FunCheckMBAP(	byte reqMessNoHi, byte reqMessNoLo, byte ansMessNoHi, byte ansMessNoLo,
										byte ansProtocolHi, byte ansProtocolLo,int ansSize, byte ansAfterByteHi, byte ansAfterByteLo)
{
	//判断响应报文的请求序号是否一致
	if (ansMessNoHi != reqMessNoHi || ansMessNoLo != reqMessNoLo)
		return CodeMessNoNotSame;//接收到的报文请求序列号不一致

	//判断响应报文的协议标识是否正确（00 00）
	if (ansProtocolHi != 0 || ansProtocolLo != 0)
		return CodeProtocolNotTrue;//接收到的报文协议标识有误

	//判断响应报文的其后字节数是否正确
	if (ansSize - 6 != ClaTempFunc::FunCombineTwoChar(ansAfterByteHi, ansAfterByteLo))
		return CodeHeadAfterByteNotTrue;//接收到的报文报头其后字节数不正确

	return CodeJustOK;
}

int ClaTcpAnalyseReceive::FunCheckMessForRead(vector<byte> requestMess, vector<byte> answerMess)
{
	//判断请求与响应的功能码是否一致
	if (answerMess.at(7) != requestMess.at(7))
		return CodeFuncNotSame;//响应报文的功能码与请求不一致

	//判断响应的字节数是否符合最低要求
	if (answerMess.size() < 10)
		return Code0103MessNotComplete;//(0x01/0x03)响应报文的字节数不正常或不完整

	unsigned short realRequestByte;
	unsigned short tmpByteCount;
	if (0x01 == answerMess.at(7))
	{
		realRequestByte = (ClaTempFunc::FunCombineTwoChar(requestMess.at(10), requestMess.at(11)) + 7) / 8;
		if (realRequestByte != (unsigned short)answerMess.at(8))
			return CodeDataCountNotSame;//(0x01/0x03)响应报文的数据字节数与请求无法对应

		tmpByteCount = answerMess.at(8);
		if (tmpByteCount == answerMess.size() - 9)//解析正确了
			return CodeReadCoilSuc;//解析正确
		else
			return CodeDataByteNotSame;//(0x01/0x03)响应报文的数据字节数不正确
	}
	else
	{
		realRequestByte = (ClaTempFunc::FunCombineTwoChar(requestMess.at(10), requestMess.at(11)) * 2);
		if (realRequestByte != (unsigned short)answerMess.at(8))
			return CodeDataCountNotSame;//(0x01/0x03)响应报文的数据字节数与请求无法对应

		tmpByteCount = answerMess.at(8);
		if (tmpByteCount == answerMess.size() - 9)
			return CodeReadRegiSuc;//解析正确
		else
			return CodeDataByteNotSame;//(0x01/0x03)响应报文的数据字节数不正确
	}
}

int ClaTcpAnalyseReceive::FunCheckMessForWrite(vector<byte> requestMess, vector<byte> answerMess)
{
	//判断请求与响应的功能码是否一致
	if (answerMess.at(7) != requestMess.at(7))
		return CodeFuncNotSame;//响应报文的功能码与请求不一致

	if (12 == answerMess.size())
	{
		unsigned short reqAddr = ClaTempFunc::FunCombineTwoChar(requestMess.at(8), requestMess.at(9));
		unsigned short ansAddr = ClaTempFunc::FunCombineTwoChar(answerMess.at(8), answerMess.at(9));
		if (reqAddr != ansAddr)
			return Code0f10AddrNotSame;//(0x0f/0x10)响应报文解析与预期响应不一致（请求地址与响应地址不一致）

		unsigned short reqCount = ClaTempFunc::FunCombineTwoChar(requestMess.at(10), requestMess.at(11));
		unsigned short ansCount = ClaTempFunc::FunCombineTwoChar(answerMess.at(10), answerMess.at(11));
		if (reqCount != ansCount)
			return Code0f10CountNotSame;//(0x0f/0x10)响应报文解析与预期响应不一致（请求数量与响应数量不一致）

		if (0x0F == answerMess.at(7))
			return CodeWriteCoilSuc;//线圈数据写入成功
		else
			return CodeWriteRegiSuc;//寄存器写入成功
	}
	else
	{
		return Code0f10LengthNotTrue;//(0x0f/0x10)响应报文长度不完整或不正确
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
			return CodeErrorOne;// 请求异常！异常码为：1

		return CodeErrorTwoOrThree;//请求异常！异常码为2或3
	}
	else
	{
		return CodeCantReadFunc;//无法被识别的功能码
	}
}
#pragma endregion

int ClaTcpAnalyseReceive::FunTcpAnalyseMess(vector<byte> requestMess, vector<byte> answerMess)
{
	//判断是否有发送请求报文
	if (requestMess.empty())
		return CodeNoRequest;//未发送请求就接收到了消息！此消息不予解析

	//判断接收到的报文长度是否正常(报头 6 + viceNo 1 + FuncCode 1)
	if (answerMess.size() < 9)
		return CodeLengthTooShot;// 接收到的报文长度不正常

	//判断MBAP是否正确
	int ret = FunCheckMBAP(requestMess.at(0), requestMess.at(1), answerMess.at(0), answerMess.at(1),answerMess.at(2), answerMess.at(3), answerMess.size(), answerMess.at(4), answerMess.at(5));
	if (0 != ret)
		return ret;

	//判断响应从站的从站编号是否与请求一致
	if (answerMess.at(6) != requestMess.at(6))
		return CodeViceNoNotSmae;//响应此请求的从站不正确

	return FunCheckAnotherFuncCode(requestMess, answerMess);
}