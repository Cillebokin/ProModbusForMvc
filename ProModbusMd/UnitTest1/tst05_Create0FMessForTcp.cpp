#include "stdafx.h"
#include "CppUnitTest.h"

#include "MainCreateMessTcp.h"
#include <vector>
#include <windows.h>
#include <atlstr.h>

#include "FunctionForTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(tst05_Create0FMessForTcp)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, MESS_CREATE_0F_INI_PATH_TCP);
			//Assert::AreEqual(secNum,1);

			LPTSTR message = (LPTSTR)malloc(READ_SIZE);
			LPTSTR data = (LPTSTR)malloc(READ_SIZE);

			for (int i = 1; i <= secNum; i++)
			{
				CString realSecName;
				CString secTag;
				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				byte messNoHi = GetPrivateProfileInt(realSecName, "messNoHi", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);
				byte messNoLo = (byte)GetPrivateProfileInt(realSecName, "messNoLo", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);
				byte protocolHi = (byte)GetPrivateProfileInt(realSecName, "protocolHi", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);
				byte protocolLo = (byte)GetPrivateProfileInt(realSecName, "protocolLo", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);
				byte afterByteHi = (byte)GetPrivateProfileInt(realSecName, "afterByteHi", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);
				byte afterByteLo = (byte)GetPrivateProfileInt(realSecName, "afterByteLo", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);

				byte viceNo = (byte)GetPrivateProfileInt(realSecName, "viceNo", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);
				byte funcCode = (byte)GetPrivateProfileInt(realSecName, "funcCode", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);

				byte beginAddrHi = (byte)GetPrivateProfileInt(realSecName, "beginAddrHi", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);
				byte beginAddrLo = (byte)GetPrivateProfileInt(realSecName, "beginAddrLo", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);

				byte dataCountHi = (byte)GetPrivateProfileInt(realSecName, "dataCountHi", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);
				byte dataCountLo = (byte)GetPrivateProfileInt(realSecName, "dataCountLo", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);

				GetPrivateProfileString(realSecName, "data", READ_STRING_ERROR, data, READ_SIZE, MESS_CREATE_0F_INI_PATH_TCP);

				GetPrivateProfileString(realSecName, "message", READ_STRING_ERROR, message, READ_SIZE, MESS_CREATE_0F_INI_PATH_TCP);

				int result = GetPrivateProfileInt(realSecName, "result", READ_INT_ERROR, MESS_CREATE_0F_INI_PATH_TCP);

				vector<byte> mess0F10Vec;
				ClaFuncForTest::FunStringToVectorChar(message, mess0F10Vec);
				vector<byte> data0F10Vec;
				ClaFuncForTest::FunStringToVectorChar(data, data0F10Vec);

				vector<byte> realMess0F10Vec;
				int realResult = ClaCreateMessTcp::FunCreate0FMessForTcp(messNoHi, messNoLo, protocolHi, protocolLo, afterByteHi, afterByteLo, viceNo, funcCode,
																		beginAddrHi, beginAddrLo, dataCountHi, dataCountLo, data0F10Vec, realMess0F10Vec);
				Assert::AreEqual(result, realResult);

				if (0 == result)
				{
					vector<byte>::iterator iter = mess0F10Vec.begin();
					vector<byte>::iterator realIter = realMess0F10Vec.begin();
					for (int i = 0; i < realMess0F10Vec.size(); i++, iter++, realIter++)
					{
						Assert::AreEqual(*iter, *realIter);
					}
				}
				data0F10Vec.clear();
				mess0F10Vec.clear();
				realMess0F10Vec.clear();
			}
			free(message);
			free(data);
		}
	};
}