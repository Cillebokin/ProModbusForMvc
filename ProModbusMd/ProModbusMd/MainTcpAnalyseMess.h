#ifndef _MAIN_TCP_ANALYSE_MESS_H_
#define _MAIN_TCP_ANALYSE_MESS_H_

#include <vector>
#include <windows.h>

using namespace std;

class ClaTcpAnalyseReceive
{
public:
	static int FunTcpAnalyseMess(vector<byte> requestMess, vector<byte> answerMess);

private:
	static int FunCheckMBAP(byte reqMessNoHi, byte reqMessNoLo, byte ansMessNoHi, byte ansMessNoLo, byte ansProtocolHi, byte ansProtocolLo, int ansSize, byte ansAfterByteHi, byte ansAfterByteLo);
	static int FunCheckMessForRead(vector<byte> requestMess, vector<byte> answerMess);
	static int FunCheckMessForWrite(vector<byte> requestMess, vector<byte> answerMess);
	static int FunCheckAnotherFuncCode(vector<byte> requestMess, vector<byte> answerMess);

};

#endif