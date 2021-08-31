#ifndef _VICE_TEMPORARY_FUNCTION_H_
#define _VICE_TEMPORARY_FUNCTION_H_

#include <windows.h>

#define VICE_NO_MIN 1
#define VICE_NO_MAX 247
#define READ_COIL_NUM_MAX 2000
#define READ_REGI_NUM_MAX 125
#define WRITE_COIL_NUM_MAX 1968
#define WRITE_REGI_NUM_MAX 123


#define BEGIN_ADDR 0x0000
#define END_ADDR 0x270F



enum RetCodeMeanAnalyse{CodeAnalyseSuc = 0, CodeMessNotEnoughLong = -1, CodeProtocolNotTrue = -2, CodeHeadAfterByteNotTrue = -3, CodeViceNoNotTrue = -4, CodeCantReadFunc = -5, CodeErrorOne = -6, CodeErrorTwoOrThree = -7, CodeBeginAddrErr = -8, CodeReadCoilNumErr = -9, CodeReadRegiNumErr = -10, CodeWriteCoilNumErr = -11, CodeWriteRegiNumErr = -12, CodeAddrAndCountNotTrue = -13, Code01ReqLenNotTrue = -14, Code03ReqLenNotTrue = -15, Code0FReqLenNotTrue = -16, Code10ReqLenNotTrue = -17, Code0FByteNotTrue = -18, Code10ByteNotTrue = -19, CodeCrcError = -20};


class ClaTempFuncVice
{
public:
	static unsigned short FunCombineTwoChar(byte preNum, byte afterNum);

private:

};

#endif _TEMPORARY_FUNCTION_H_