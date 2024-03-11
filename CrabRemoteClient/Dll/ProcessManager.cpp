#include "pch.h"
#include "ProcessManager.h"
#include "ProcessHelper.h"



CProcessManager::CProcessManager(CIocpClient* IocpClient) :CManager(IocpClient)
{
	//回传数据包到服务器

	EnableSeDebugPrivilege(GetCurrentProcess(), TRUE, SE_DEBUG_NAME);

	//当前客户端扫描所有正在运行的进程信息

	SendClientProcessList();



}
CProcessManager::~CProcessManager()
{
	_tprintf(_T("~CProcessManager()\r\n"));
	EnableSeDebugPrivilege(GetCurrentProcess(), FALSE, SE_DEBUG_NAME);
}

void CProcessManager::HandleIo(PBYTE BufferData, ULONG_PTR BufferLength)
{

}
BOOL CProcessManager::SendClientProcessList()
{

	BOOL  IsOk = FALSE;
	/*	DWORD Offset = 1;
		DWORD v1 = 0;
		ULONG ItemCount = 0;
		char* BufferData = NULL;
		vector<PROCESS_ITEM_INFORMATION> ProcessItemInfoVector;
		vector<PROCESS_ITEM_INFORMATION>::iterator i;
		if (m_IsWow64Process == NULL)
		{
			return IsOk;
		}
		if (SeEnumProcessByToolHelp32(ProcessItemInfoVector) == FALSE)
		{
			return IsOk;
		}
		BufferData = (char*)LocalAlloc(LPTR, 0x1000);
		if (BufferData == NULL)
		{
			goto Exit;
		}
		BufferData[0] = CLIENT_PROCESS_MANAGER_REPLY;

		//遍历模板数据
		//[CLIENT_PROCESS_MANAGER_REPLY][ProcessID][ProcessImageName\0][ProcessFullPath\0][位数\0][ProcessID]......
		for (i = ProcessItemInfoVector.begin(); i != ProcessItemInfoVector.end(); i++)
		{
			v1 = sizeof(HANDLE) +
				lstrlen(i->ProcessImageName) + lstrlen(i->ProcessFullPath) + lstrlen(i->IsWow64Process) + 3;
			// 缓冲区太小，再重新分配下
			if (LocalSize(BufferData) < (Offset + v1))
			{
				BufferData = (char*)LocalReAlloc(BufferData, (Offset + v1),
					LMEM_ZEROINIT | LMEM_MOVEABLE);
			}

			memcpy(BufferData + Offset, &(i->ProcessID), sizeof(HANDLE));
			Offset += sizeof(HANDLE);
			memcpy(BufferData + Offset, i->ProcessImageName, lstrlen(i->ProcessImageName) + 1);
			Offset += lstrlen(i->ProcessImageName) + 1;
			memcpy(BufferData + Offset, i->ProcessFullPath, lstrlen(i->ProcessFullPath) + 1);
			Offset += lstrlen(i->ProcessFullPath) + 1;
			memcpy(BufferData + Offset, i->IsWow64Process, lstrlen(i->IsWow64Process) + 1);
			Offset += lstrlen(i->IsWow64Process) + 1;
		}
		m_IOCPClient->OnSending((char*)BufferData, LocalSize(BufferData));
		IsOk = TRUE;
	Exit:
		if (BufferData != NULL)
		{
			LocalFree(BufferData);
			BufferData = NULL;
		}*/
	return IsOk;
}




