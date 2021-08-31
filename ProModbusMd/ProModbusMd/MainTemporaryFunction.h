#ifndef _MAIN_TEMPORARY_FUNCTION_H_
#define _MAIN_TEMPORARY_FUNCTION_H_

#include <windows.h>

#define VICE_NO_MIN 1
#define VICE_NO_MAX 247

#define BEGIN_ADDR 0x0000
#define END_ADDR 0x270F

#define READ_COIL_NUM_MAX 2000
#define READ_REGI_NUM_MAX 125
#define WRITE_COIL_NUM_MAX 1968
#define WRITE_REGI_NUM_MAX 123

enum RetCodeMeanAnalyse{	CodeNoRequest = -1, CodeLengthTooShot = -2, CodeMessNoNotSame = -3, CodeProtocolNotTrue = -4, CodeHeadAfterByteNotTrue = -5, CodeViceNoNotSmae = -6,
							CodeCantReadFunc = -7, CodeErrorOne = -8, CodeErrorTwoOrThree = -9, CodeFuncNotSame = -10,
							Code0103MessNotComplete = -12, CodeDataCountNotSame = -13, CodeDataByteNotSame = -14,
							Code0f10AddrNotSame = -16, Code0f10CountNotSame = -17, Code0f10LengthNotTrue = -18, CodeCrcError = -19,
							CodeJustOK = 0, CodeReadCoilSuc = 1, CodeReadRegiSuc = 2, CodeWriteCoilSuc = 3, CodeWriteRegiSuc = 4};


enum RetCodeMeanCreateMess{ CodeCreSuc = 0, CodeFuncCantRead = -1, CodeViceErr = -2, CodeBegiAddrErr = -3, CodeReadCoilNumErr = -4, CodeReadRegiNumErr = -5, CodeWriteCoilNumErr = -6, CodeWriteRegiNumErr = -7, CodeAddrAddCountErr = -8, CodeCountByteNoSame = -9, CodeCountByteSame = 9};

class ClaTempFunc
{
public:
	static unsigned short FunCombineTwoChar(byte preNum, byte afterNum);

private:

};


#endif