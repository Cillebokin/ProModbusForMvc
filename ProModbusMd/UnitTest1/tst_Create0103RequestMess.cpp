#include "stdafx.h"
#include "CppUnitTest.h"

#include "CreateMess.h"
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

		TEST_METHOD(TestMethodForCreate0103RequestMess)
		{
			// TODO:  在此输入测试代码
			int secNum = GetPrivateProfileInt("Section0", "count", -1, MESS_CREATE_0103_INI_PATH);
			//Assert::AreEqual(secNum,1);
			
			LPTSTR message = (LPTSTR)malloc(READ_SIZE);

			CString realSecName;
			CString secTag;

			for (int i = 1; i <= secNum; i++)
			{
				secTag.Format(_T("%d"), i);
				realSecName = "Section" + secTag;

				int messNoHi = GetPrivateProfileInt(realSecName, "messNoHi", 254, MESS_CREATE_0103_INI_PATH);
				byte messNoLo = (byte)GetPrivateProfileInt(realSecName, "messNoLo", 255, MESS_CREATE_0103_INI_PATH);
				byte protocolHi = (byte)GetPrivateProfileInt(realSecName, "protocolHi", 255, MESS_CREATE_0103_INI_PATH);
				byte protocolLo = (byte)GetPrivateProfileInt(realSecName, "protocolLo", 255, MESS_CREATE_0103_INI_PATH);
				byte afterByteHi = (byte)GetPrivateProfileInt(realSecName, "afterByteHi", 255, MESS_CREATE_0103_INI_PATH);
				byte afterByteLo = (byte)GetPrivateProfileInt(realSecName, "afterByteLo", 255, MESS_CREATE_0103_INI_PATH);
				
				byte viceNo = (byte)GetPrivateProfileInt(realSecName, "viceNo", 255, MESS_CREATE_0103_INI_PATH);
				byte funcCode = (byte)GetPrivateProfileInt(realSecName, "funcCode", 255, MESS_CREATE_0103_INI_PATH);
				
				byte beginAddrHi = (byte)GetPrivateProfileInt(realSecName, "beginAddrHi", 255, MESS_CREATE_0103_INI_PATH);
				byte beginAddrLo = (byte)GetPrivateProfileInt(realSecName, "beginAddrLo", 255, MESS_CREATE_0103_INI_PATH);
				
				byte dataCountHi = (byte)GetPrivateProfileInt(realSecName, "dataCountHi", 255, MESS_CREATE_0103_INI_PATH);
				byte dataCountLo = (byte)GetPrivateProfileInt(realSecName, "dataCountLo", 255, MESS_CREATE_0103_INI_PATH);
				
				GetPrivateProfileString(realSecName, "message", "ERROR", message, READ_SIZE, MESS_CREATE_0103_INI_PATH);
			
				int result = GetPrivateProfileInt(realSecName, "result", 255, MESS_CREATE_0103_INI_PATH);

				vector<byte> Mess0103Vec;
				FunStringToVectorChar(message, Mess0103Vec);

				vector<byte> realMess0103Vec;
				int realResult = ClaCreateMess::FunCreate0103Mess(	messNoHi, messNoLo,
																	protocolHi, protocolLo,
																	afterByteHi, afterByteLo,
																	viceNo, funcCode,
																	beginAddrHi, beginAddrLo,
																	dataCountHi, dataCountLo,
																	realMess0103Vec);
				Assert::AreEqual(result, realResult);
				
				if (0 == result)
				{
					vector<byte>::iterator iter = Mess0103Vec.begin();
					vector<byte>::iterator realIter = realMess0103Vec.begin();
					for (int i = 0; i < realMess0103Vec.size(); i++, iter++, realIter++)
					{
						Assert::AreEqual(*iter,*realIter);
					}
				}
				Mess0103Vec.clear();
				realMess0103Vec.clear();
			}

			free(message);
		}
	};
}