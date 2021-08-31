#include "stdafx.h"
#include "CppUnitTest.h"

#include "ViceRtuAnalyseMess.h"
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

		TEST_METHOD(tst12_RtuAnalyse)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, RTU_VICE_ANAL_INI_PATH);
			//Assert::AreEqual(secNum,2);

			LPTSTR message = (LPTSTR)malloc(READ_SIZE);
			
			for (int i = 1; i <= secNum; i++)
			{
				CString realSecName;
				CString secTag;
				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				GetPrivateProfileString(realSecName, "message", READ_STRING_ERROR, message, READ_SIZE, RTU_VICE_ANAL_INI_PATH);
				byte viceNo = (byte)GetPrivateProfileInt(realSecName, "viceNo", READ_INT_ERROR, RTU_VICE_ANAL_INI_PATH);
				int result = GetPrivateProfileInt(realSecName, "result", READ_INT_ERROR, RTU_VICE_ANAL_INI_PATH);

				vector<byte> messReqVec;
				ClaFuncForTest::FunStringToVectorChar(message, messReqVec);

				int realResult = ClaRtuAnalyseRequest::FunRtuAnalyseMess(messReqVec, viceNo);

				Assert::AreEqual(result, realResult);

				messReqVec.clear();
			}
			free(message);
		}
	};
}