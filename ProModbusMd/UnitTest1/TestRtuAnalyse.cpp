#include "stdafx.h"
#include "CppUnitTest.h"

#include "RtuAnalyseMess.h"
#include <vector>
#include <windows.h>
#include <atlstr.h>

#include "FunctionForTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		//RTU从
		TEST_METHOD(TestMethodForRtuAnalyse)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, RTU_INI_PATH);
			//Assert::AreEqual(secNum,48);
			
			LPTSTR rxStr = (LPTSTR)malloc(READ_SIZE);

			for (int i = 1; i <= secNum; i++)
			{
				string realSecName = "Section" + i;

				GetPrivateProfileString(realSecName.c_str(), "rx", "ERROR", rxStr, READ_SIZE, RTU_INI_PATH);

				int viceNo = GetPrivateProfileInt(realSecName.c_str(), "viceNo", 256, RTU_INI_PATH);
				int result = GetPrivateProfileInt(realSecName.c_str(), "result", 256, RTU_INI_PATH);

				vector<byte> requestMess;
				FunStringToVectorChar(rxStr, requestMess);

				//01 01 00 00 00 20 ** **
				
				int analyseResult = ClaRtuViceAnalyseAnswer::FunRtuAnalyseMess(requestMess, viceNo);

				Assert::AreEqual(result, analyseResult);

				requestMess.clear();
			}
			free(rxStr);
		}
	};
}