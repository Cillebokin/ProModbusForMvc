#ifndef _VICE_RTU_ANALYSE_MSS_H_
#define _VICE_RTU_ANALYSE_MSS_H_

#include <vector>
#include <windows.h>
#include "CrcCheck.h"
#include "ViceTemporaryFunction.h"

using namespace std;

class ClaRtuAnalyseRequest
{
public:
	static int  FunRtuAnalyseMess(vector<byte> requestMess, byte viceNo);

private:
	static int FunCheckCrc(vector<byte> receiveMess);
	static int FunCheckDataCount(byte funcCode, unsigned short dataCount);
	static int FunCheckMessLenIsTrue(vector<byte> requestMess);
};

#endif