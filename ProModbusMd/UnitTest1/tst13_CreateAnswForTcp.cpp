#include "stdafx.h"
#include "CppUnitTest.h"

#include "ViceCreateMessTcp.h"
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

		TEST_METHOD(tst13_CreateAnswForTcp)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, CREATE_ANSWER_TCP_INI_PATH);
			//Assert::AreEqual(secNum,1);

			LPTSTR request = (LPTSTR)malloc(READ_SIZE);
			LPTSTR data = (LPTSTR)malloc(READ_SIZE);
			LPTSTR answer = (LPTSTR)malloc(READ_SIZE);

			for (int i = 1; i <= secNum; i++)
			{
				CString realSecName;
				CString secTag;
				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				GetPrivateProfileString(realSecName, "request", READ_STRING_ERROR, request, READ_SIZE, CREATE_ANSWER_TCP_INI_PATH);
				GetPrivateProfileString(realSecName, "data", READ_STRING_ERROR, data, READ_SIZE, CREATE_ANSWER_TCP_INI_PATH);
				GetPrivateProfileString(realSecName, "answer", READ_STRING_ERROR, answer, READ_SIZE, CREATE_ANSWER_TCP_INI_PATH);

				int analyseCode = GetPrivateProfileInt(realSecName, "analyseCode", READ_INT_ERROR, CREATE_ANSWER_TCP_INI_PATH);

				vector<byte> requestVec;
				ClaFuncForTest::FunStringToVectorChar(request, requestVec);
				vector<byte> dataVec;
				ClaFuncForTest::FunStringToVectorChar(data, dataVec);
				vector<byte> answerVec;
				ClaFuncForTest::FunStringToVectorChar(answer, answerVec); 

				vector<byte> realAnswerVec;
				if (0 == analyseCode || -5 == analyseCode || -8 == analyseCode || -13 == analyseCode || -9 == analyseCode || -10 == analyseCode || -11 == analyseCode || -12 == analyseCode)
				{
					
					ClaCreateMessTcp::FunCreateAnswForTcp(analyseCode, ClaTempFuncVice::FunCombineTwoChar(requestVec.at(0), requestVec.at(1)), requestVec.at(6), requestVec.at(7), ClaTempFuncVice::FunCombineTwoChar(requestVec.at(8), requestVec.at(9)), ClaTempFuncVice::FunCombineTwoChar(requestVec.at(10), requestVec.at(11)), dataVec, realAnswerVec);

					Assert::AreEqual(answerVec.size(), realAnswerVec.size());
					for (vector<byte>::iterator iter = answerVec.begin(), realIter = realAnswerVec.begin();
						iter != answerVec.end() || realIter != realAnswerVec.end();
						iter++, realIter++)
					{
						Assert::AreEqual(*iter, *realIter);
					}
				}
				requestVec.clear();
				dataVec.clear();
				answerVec.clear();
				realAnswerVec.clear();
			}
			free(request);
			free(data);
			free(answer);
		}
	};
}