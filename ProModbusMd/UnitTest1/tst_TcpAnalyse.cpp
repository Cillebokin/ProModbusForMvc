#include "stdafx.h"
#include "CppUnitTest.h"

#include "TcpAnalyseMess.h"
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
		
		TEST_METHOD(TestMethodForTcpAnalyse)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, TCP_INI_PATH);
			//Assert::AreEqual(secNum,76);

			CString realSecName;
			CString secTag;
			LPTSTR txStr = (LPTSTR)malloc(READ_SIZE);
			LPTSTR rxStr = (LPTSTR)malloc(READ_SIZE);
			int result;

			for (int i = 3; i <= 3; i++)
			{
				std::vector<byte> requestMess;
				std::vector<byte> answerMess;

				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;
				GetPrivateProfileString(realSecName, "tx", "ERROR", txStr, READ_SIZE, TCP_INI_PATH);
				GetPrivateProfileString(realSecName, "rx", "ERROR", rxStr, READ_SIZE, TCP_INI_PATH);
				result = GetPrivateProfileInt(realSecName, "result", 256, TCP_INI_PATH);

				FunStringToVectorChar(txStr,requestMess);
				FunStringToVectorChar(rxStr,answerMess);
				int analyseResult = ClaTcpMainAnalyseAnswer::FunTcpAnalyseMess(requestMess, answerMess);
				
				Assert::AreEqual(analyseResult, result);

				requestMess.clear();
				answerMess.clear();
			}
			free(txStr);
			free(rxStr);
		}
	};
}