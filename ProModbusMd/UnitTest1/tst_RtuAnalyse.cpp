#include "stdafx.h"
#include "CppUnitTest.h"

#include "RtuAnalyseMess.h"
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

		TEST_METHOD(TestMethodForRtuAnalyse)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, RTU_INI_PATH);
			//Assert::AreEqual(secNum,48);

			LPCSTR realSecName;
			CString secTag;
			LPTSTR rxStr = (LPTSTR)malloc(READ_SIZE);
			byte viceNo;
			int result;

			for (int i = 1; i <= secNum; i++)
			{
				std::vector<byte> requestMess;

				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				GetPrivateProfileString(realSecName, "rx", "ERROR", rxStr, READ_SIZE, RTU_INI_PATH);
				viceNo = (byte)GetPrivateProfileInt(realSecName, "viceNo", 256, RTU_INI_PATH);
				result = GetPrivateProfileInt(realSecName, "result", 256, RTU_INI_PATH);

				FunStringToVectorChar(rxStr, requestMess);
				int analyseResult = ClaRtuViceAnalyseAnswer::FunRtuAnalyseMess(requestMess, viceNo);

				Assert::AreEqual(result, analyseResult);

				requestMess.clear();
			}
			free(rxStr);
		}
	};
}