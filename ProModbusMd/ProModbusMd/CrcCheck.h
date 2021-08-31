#ifndef _CRC_CHECK_H_
#define _CRC_CHECK_H_

#include <vector>
#include "windows.h"

using namespace std;

class ClaCrcCheck
{
public:
	ClaCrcCheck();
	unsigned int ModbusCrc16(vector<byte> requestMess, unsigned int usDataLen);

private:
	unsigned char auchCRCHi[256];
	unsigned char auchCRCLo[256];
};

#endif