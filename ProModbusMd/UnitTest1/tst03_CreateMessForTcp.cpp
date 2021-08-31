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

		TEST_METHOD(tst03_CreateMessForTcp)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, CREATE_MESS_TCP_INI_PATH);
			//Assert::AreEqual(secNum,1);

			LPTSTR message = (LPTSTR)malloc(READ_SIZE);
			LPTSTR data = (LPTSTR)malloc(READ_SIZE);

			for (int i = 1; i <= secNum; i++)
			{
				CString realSecName;
				CString secTag;
				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				byte messNoHi = GetPrivateProfileInt(realSecName, "messNoHi", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);
				byte messNoLo = (byte)GetPrivateProfileInt(realSecName, "messNoLo", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);

				byte viceNo = (byte)GetPrivateProfileInt(realSecName, "viceNo", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);
				byte funcCode = (byte)GetPrivateProfileInt(realSecName, "funcCode", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);

				byte beginAddrHi = (byte)GetPrivateProfileInt(realSecName, "beginAddrHi", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);
				byte beginAddrLo = (byte)GetPrivateProfileInt(realSecName, "beginAddrLo", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);

				byte dataCountHi = (byte)GetPrivateProfileInt(realSecName, "dataCountHi", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);
				byte dataCountLo = (byte)GetPrivateProfileInt(realSecName, "dataCountLo", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);

				GetPrivateProfileString(realSecName, "message", READ_STRING_ERROR, message, READ_SIZE, CREATE_MESS_TCP_INI_PATH);

				GetPrivateProfileString(realSecName, "data", READ_STRING_ERROR, data, READ_SIZE, CREATE_MESS_TCP_INI_PATH);

				int result = GetPrivateProfileInt(realSecName, "result", READ_INT_ERROR, CREATE_MESS_TCP_INI_PATH);

				vector<byte> messVec;
				ClaFuncForTest::FunStringToVectorChar(message, messVec);

				vector<byte> dataVec;
				ClaFuncForTest::FunStringToVectorChar(data, dataVec);

				vector<byte> realMessVec;
				int realResult = ClaCreateMessTcp::FunCreateMessForTcp(ClaFuncForTest::FunCombineTwoChar(messNoHi, messNoLo), viceNo, funcCode, ClaFuncForTest::FunCombineTwoChar(beginAddrHi, beginAddrLo), ClaFuncForTest::FunCombineTwoChar(dataCountHi, dataCountLo), dataVec, realMessVec);
			
				Assert::AreEqual(result, realResult);

				if (0 == result)
				{
					vector<byte>::iterator iter = messVec.begin();
					vector<byte>::iterator realIter = realMessVec.begin();
					for (int i = 0; i < realMessVec.size(); i++, iter++, realIter++)
					{
						Assert::AreEqual(*iter, *realIter);
					}
				}
				messVec.clear();
				dataVec.clear();
				realMessVec.clear();
			}
			free(message);
			free(data);
		}
	};
}