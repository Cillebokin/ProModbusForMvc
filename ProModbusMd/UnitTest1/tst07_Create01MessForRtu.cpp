#include "stdafx.h"
#include "CppUnitTest.h"

#include "MainCreateMessRtu.h"
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

		TEST_METHOD(tst07_Create01MessForRtu)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, MESS_CREATE_01_INI_PATH_RTU);
			//Assert::AreEqual(secNum,1);

			LPTSTR message = (LPTSTR)malloc(READ_SIZE);

			for (int i = 1; i <= secNum; i++)
			{
				CString realSecName;
				CString secTag;
				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				byte viceNo = (byte)GetPrivateProfileInt(realSecName, "viceNo", READ_INT_ERROR, MESS_CREATE_01_INI_PATH_RTU);
				byte funcCode = (byte)GetPrivateProfileInt(realSecName, "funcCode", READ_INT_ERROR, MESS_CREATE_01_INI_PATH_RTU);

				byte beginAddrHi = (byte)GetPrivateProfileInt(realSecName, "beginAddrHi", READ_INT_ERROR, MESS_CREATE_01_INI_PATH_RTU);
				byte beginAddrLo = (byte)GetPrivateProfileInt(realSecName, "beginAddrLo", READ_INT_ERROR, MESS_CREATE_01_INI_PATH_RTU);

				byte dataCountHi = (byte)GetPrivateProfileInt(realSecName, "dataCountHi", READ_INT_ERROR, MESS_CREATE_01_INI_PATH_RTU);
				byte dataCountLo = (byte)GetPrivateProfileInt(realSecName, "dataCountLo", READ_INT_ERROR, MESS_CREATE_01_INI_PATH_RTU);

				GetPrivateProfileString(realSecName, "message", READ_STRING_ERROR, message, READ_SIZE, MESS_CREATE_01_INI_PATH_RTU);

				int result = GetPrivateProfileInt(realSecName, "result", READ_INT_ERROR, MESS_CREATE_01_INI_PATH_RTU);

				vector<byte> mess0103Vec;
				ClaFuncForTest::FunStringToVectorChar(message, mess0103Vec);

				vector<byte> realMess0103Vec;
				int realResult = ClaCreateMessRtu::FunCreate01MessForRtu(viceNo, funcCode, beginAddrHi, beginAddrLo, dataCountHi, dataCountLo, realMess0103Vec);

				Assert::AreEqual(result, realResult);

				if (0 == result)
				{
					vector<byte>::iterator iter = mess0103Vec.begin();
					vector<byte>::iterator realIter = realMess0103Vec.begin();
					for (int i = 0; i < realMess0103Vec.size(); i++, iter++, realIter++)
					{
						Assert::AreEqual(*iter, *realIter);
					}
				}
				mess0103Vec.clear();
				realMess0103Vec.clear();
			}
			free(message);
		}
	};
}