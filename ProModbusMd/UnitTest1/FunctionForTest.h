#ifndef _FUNCTION_FOR_TEST_H_
#define _FUNCTION_FOR_TEST_H_

#include <vector>
#include <windows.h>

#define READ_SIZE 520

#define READ_INT_ERROR 255
#define READ_STRING_ERROR "ERROR"

#define TCP_INI_PATH "../config/FunTcpMainAnalyseMessData.ini"
#define RTU_INI_PATH "../config/FunRtuMainAnalyseMessData.ini"

#define CREATE_MESS_TCP_INI_PATH "../config/FunCreateMessageForTcp.ini"
#define CREATE_MESS_RTU_INI_PATH "../config/FunCreateMessageForRtu.ini"

#define TCP_VICE_ANAL_INI_PATH "../config/FunTcpViceAnalyseMessData.ini"
#define RTU_VICE_ANAL_INI_PATH "../config/FunRtuViceAnalyseMessData.ini"

#define CREATE_ANSWER_TCP_INI_PATH "../config/FunCreateAnswerForTcp.ini"
#define CREATE_ANSWER_RTU_INI_PATH "../config/FunCreateAnswerForRtu.ini"

class ClaFuncForTest
{
public:
	static byte FunCharToHex(byte tmpChar);

	static void FunStringToVectorChar(LPTSTR reStr, std::vector<byte> &tmpVec);

	static unsigned short FunCombineTwoChar(byte preNum, byte afterNum);

private:

};

#endif