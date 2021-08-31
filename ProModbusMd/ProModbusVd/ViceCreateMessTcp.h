#ifndef _VICE_CREATE_MESS_TCP_H_
#define _VICE_CREATE_MESS_TCP_H_

#include <vector>
#include <windows.h>
#include "ViceTemporaryFunction.h"

using namespace std;

class ClaCreateMessTcp
{
public:
	static void FunCreateAnswForTcp(int analyseCode, unsigned short messNo, byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, vector<byte> data, vector<byte> &answer);

private:
	static void FunMixErrorAnsw(byte viceNo, byte funcCode, byte errCode, vector<byte> &answer);
	static void FunMixCommonAnswForRead(byte viceNo, byte funcCode, vector<byte> data, vector<byte> &answer);
	static void FunMixCommonAnswForWrite(byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, vector<byte> &answer);
};

#endif