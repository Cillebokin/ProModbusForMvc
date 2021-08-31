#ifndef _VICE_TCP_ANALYSE_MESS_H_
#define _VICE_TCP_ANALYSE_MESS_H_

#include <vector>
#include <windows.h>
#include "ViceTemporaryFunction.h"

using namespace std;

class ClaTcpAnalyseRequest
{
public:
	static int  FunTcpAnalyseMess(vector<byte> requestMess, byte viceNo);

private:
	static int FunCheckMBAP(byte protocolHi, byte protocolLo, byte afterByteHi, byte afterByteLo, int requestSize);
	static int FunCheckDataCount(byte funcCode, unsigned short dataCount);
	static int FunCheckMessLenIsTrue(vector<byte> requestMess);
};

#endif