#ifndef _CREATE_MESS_RTU_H_
#define _CREATE_MESS_RTU_H_

#include <vector>
#include <windows.h>
#include "CrcCheck.h"
#include "MainTemporaryFunction.h"

using namespace std;

class ClaCreateMessRtu
{
public:
	static int FunCreateMessForRtu(	byte viceNo, byte funcCode, unsigned short beginAddr, unsigned short dataCount, 
									vector<byte> data, vector<byte> &message);

private:
	static int FunCheckAndInsertViceNo(byte viceNo, vector<byte> &message);
	static int FunCheckAndInsertFuncCode(byte funcCode, vector<byte> &message);
	static int FunCheckAndInsertBeginAddr(unsigned short beginAddr, vector<byte> &message);
	static int FunCheckAndInsertDataCount(byte funcCode, unsigned short dataCount, vector<byte> &message);
	static int FunCheckBeginAddrAddDataCount(unsigned short beginAddr, unsigned short dataCount);
	static void FunInsertCrc(vector<byte> &message);
	static int FunCheckAndInsertDataSize(byte funcCode, unsigned short dataCount, vector<byte> data, vector<byte> &message);
	static void FunInsertData(vector<byte> data, vector<byte> &message);
};

#endif