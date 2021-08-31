#ifndef _MAIN_RTU_ANALYSE_MESS_H_
#define _MAIN_RTU_ANALYSE_MESS_H_

#include <vector>
#include <windows.h>


using namespace std;

class ClaRtuAnalyseReceive
{
public:
	static int FunRtuAnalyseMess(vector<byte> requestMess, vector<byte> receiveMess);

private:
	static int FunCheckCrc(vector<byte> receiveMess);

	static int FunCheckMessForRead(vector<byte> requestMess, vector<byte> receiveMess);
	static int FunCheckMessForWrite(vector<byte> requestMess, vector<byte> receiveMess);

	static int FunCheckAnotherFuncCode(vector<byte> requestMess, vector<byte> receiveMess);
};

#endif