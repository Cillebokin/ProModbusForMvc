#ifndef _CREATE_MESS_TCP_H_
#define _CREATE_MESS_TCP_H_

#include <vector>
#include <windows.h>
#include "CrcCheck.h"
#include "MainTemporaryFunction.h"

using namespace std;

class ClaCreateMessTcp
{
public:
	static int FunCreateMessForTcp(	unsigned short messNo, byte viceNo, byte funcCode, unsigned short beginAddr,
									unsigned short dataCount, vector<byte> data, vector<byte> &message);
	

private:
	static void FunInsertMessNo(unsigned short messNo, vector<byte> &message);
	static void FunInsertProtocol(vector<byte> &message);
	static int FunCheckAndInsertAfterByte(byte funcCode, vector<byte> data, vector<byte> &message);
	static int FunCheckAndInsertViceNo(byte viceNo, vector<byte> &message);
	static void FunInsertFuncCode(byte funcCode, vector<byte> &message);
	static int FunCheckAndInsertBeginAddr(unsigned short beginAddr, vector<byte> &message);
	static int FunCheckAndInsertDataCount(byte funcCode, unsigned short dataCount, vector<byte> &message);
	static int FunCheckBeginAddrAddDataCount(unsigned short beginAddr, unsigned short dataCount);
	static int FunCheckAndInsertDataSize(byte funcCode, unsigned short dataCount, vector<byte> data);
	static void FunInsertData(vector<byte> data, vector<byte> &message);
	
};

#endif