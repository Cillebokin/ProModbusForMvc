#include "stdafx.h"
#include "CppUnitTest.h"

#include "MainTcpAnalyseMess.h"
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
		
		TEST_METHOD(tst01_TcpAnalyse)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, TCP_INI_PATH);
			//Assert::AreEqual(secNum,76);
			
			LPTSTR txStr = (LPTSTR)malloc(READ_SIZE);
			LPTSTR rxStr = (LPTSTR)malloc(READ_SIZE);
			int result;

			for (int i = 1; i <= secNum; i++)
			{
				vector<byte> requestMess;
				vector<byte> answerMess;

				CString realSecName;
				CString secTag;
				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				GetPrivateProfileString(realSecName, "tx", READ_STRING_ERROR, txStr, READ_SIZE, TCP_INI_PATH);
				GetPrivateProfileString(realSecName, "rx", READ_STRING_ERROR, rxStr, READ_SIZE, TCP_INI_PATH);
				result = GetPrivateProfileInt(realSecName, "result", READ_INT_ERROR, TCP_INI_PATH);

				ClaFuncForTest::FunStringToVectorChar(txStr, requestMess);
				ClaFuncForTest::FunStringToVectorChar(rxStr, answerMess);
				int analyseResult = ClaTcpAnalyseReceive::FunTcpAnalyseMess(requestMess, answerMess);
				
				Assert::AreEqual(analyseResult, result);

				requestMess.clear();
				answerMess.clear();
			}
			free(txStr);
			free(rxStr);
		}
	};
}