#ifndef _VICE_CREATE_MESS_RTU_H_
#define _VICE_CREATE_MESS_RTU_H_

#include <vector>
#include <windows.h>
#include "ViceTemporaryFunction.h"
#include "CrcCheck.h"

using namespace std;

class ClaCreateMessRtu
{
public:
	static void FunCreateAnswForRtu(int analyseCode, byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, vector<byte> data, vector<byte> &answer);

private:
	static void FunMixErrorAnsw(byte errCode, vector<byte> &answer);
	static void FunMixCrcAnsw(vector<byte> &answer);
	static void FunMixCommonAnswForRead(vector<byte> data, vector<byte> &answer);
	static void FunMixCommonAnswForWrite(unsigned short beginAddr, unsigned short dataCount, vector<byte> &answer);
};

#endif