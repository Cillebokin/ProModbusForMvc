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

		TEST_METHOD(tst04_Create03MessForTcp)
		{
			// TODO:  �ڴ�������Դ���
			int secNum = GetPrivateProfileInt("Section0", "count", -1, MESS_CREATE_03_INI_PATH_TCP);
			//Assert::AreEqual(secNum,1);

			LPTSTR message = (LPTSTR)malloc(READ_SIZE);

			for (int i = 1; i <= secNum; i++)
			{
				CString realSecName;
				CString secTag;
				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				byte messNoHi = GetPrivateProfileInt(realSecName, "messNoHi", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);
				byte messNoLo = (byte)GetPrivateProfileInt(realSecName, "messNoLo", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);
				byte protocolHi = (byte)GetPrivateProfileInt(realSecName, "protocolHi", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);
				byte protocolLo = (byte)GetPrivateProfileInt(realSecName, "protocolLo", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);
				byte afterByteHi = (byte)GetPrivateProfileInt(realSecName, "afterByteHi", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);
				byte afterByteLo = (byte)GetPrivateProfileInt(realSecName, "afterByteLo", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);

				byte viceNo = (byte)GetPrivateProfileInt(realSecName, "viceNo", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);
				byte funcCode = (byte)GetPrivateProfileInt(realSecName, "funcCode", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);

				byte beginAddrHi = (byte)GetPrivateProfileInt(realSecName, "beginAddrHi", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);
				byte beginAddrLo = (byte)GetPrivateProfileInt(realSecName, "beginAddrLo", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);

				byte dataCountHi = (byte)GetPrivateProfileInt(realSecName, "dataCountHi", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);
				byte dataCountLo = (byte)GetPrivateProfileInt(realSecName, "dataCountLo", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);

				GetPrivateProfileString(realSecName, "message", READ_STRING_ERROR, message, READ_SIZE, MESS_CREATE_03_INI_PATH_TCP);

				int result = GetPrivateProfileInt(realSecName, "result", READ_INT_ERROR, MESS_CREATE_03_INI_PATH_TCP);

				vector<byte> Mess0103Vec;
				ClaFuncForTest::FunStringToVectorChar(message, Mess0103Vec);

				vector<byte> realMess0103Vec;
				int realResult = ClaCreateMessTcp::FunCreate03MessForTcp(messNoHi, messNoLo, protocolHi, protocolLo, afterByteHi, afterByteLo, viceNo, funcCode,
					beginAddrHi, beginAddrLo, dataCountHi, dataCountLo, realMess0103Vec);
				Assert::AreEqual(result, realResult);

				if (0 == result)
				{
					vector<byte>::iterator iter = Mess0103Vec.begin();
					vector<byte>::iterator realIter = realMess0103Vec.begin();
					for (int i = 0; i < realMess0103Vec.size(); i++, iter++, realIter++)
					{
						Assert::AreEqual(*iter, *realIter);
					}
				}
				Mess0103Vec.clear();
				realMess0103Vec.clear();
			}

			free(message);
		}
	};
}