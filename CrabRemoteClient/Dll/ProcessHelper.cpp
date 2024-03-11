#include "pch.h"
#include "ProcessHelper.h"


int EnableSeDebugPrivilege(HANDLE ProcessHandle, BOOL IsEnable, LPCTSTR RequireLevel)
{
	DWORD  LastError;
	HANDLE TokenHandle = 0;

	if (!OpenProcessToken(ProcessHandle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))
	{
		LastError = GetLastError();
		if (TokenHandle)
			CloseHandle(TokenHandle);
		return LastError;
	}
	TOKEN_PRIVILEGES TokenPrivileges;
	memset(&TokenPrivileges, 0, sizeof(TOKEN_PRIVILEGES));
	LUID v1;
	if (!LookupPrivilegeValue(NULL, RequireLevel, &v1))
	{
		LastError = GetLastError();
		CloseHandle(TokenHandle);
		return LastError;
	}
	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Luid = v1;
	if (IsEnable)
		TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		TokenPrivileges.Privileges[0].Attributes = 0;
	AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	LastError = GetLastError();
	CloseHandle(TokenHandle);
	return LastError;
}


//判断进程位数
BOOL XkIsWow64Process(HANDLE ProcessHandle, BOOL* isWow64Process)
{
	//获得ntdll模块的函数
	HMODULE	Kernel32ModuleBase = NULL;

	Kernel32ModuleBase = GetModuleHandle(_T("kernel32.dll"));
	if (Kernel32ModuleBase == NULL)
	{
		return FALSE;
	}
	typedef BOOL(__stdcall* LPFN_ISWOW64PROCESS)(HANDLE ProcessHandle, BOOL* isWow64Process);
	LPFN_ISWOW64PROCESS  v1 = NULL;
	v1 = (LPFN_ISWOW64PROCESS)GetProcAddress(Kernel32ModuleBase, "IsWow64Process");

	if (v1 == NULL)
	{
		goto Exit;
	}
	//关机
	v1(ProcessHandle, isWow64Process);
Exit:
	if (Kernel32ModuleBase != NULL)
	{
		FreeLibrary(Kernel32ModuleBase);
		Kernel32ModuleBase = NULL;
	}

	return TRUE;
}

BOOL EnumProcessByToolHelp32(vector<PROCESS_INFORMATION_ITEM>& ProcessInfo)
{

	BOOL v1 = FALSE;
	HANDLE   SnapshotHandle = NULL;
	HANDLE   ProcessHandle = NULL;
	char     isWow64Process[20] = { 0 };
	PROCESSENTRY32  ProcessEntry32;   //官方
	PROCESS_INFORMATION_ITEM    v2 = { 0 }; //自定义
	TCHAR  ProcessFullPath[MAX_PATH] = { 0 };
	HMODULE ModuleHandle = NULL;
	DWORD ReturnLength = 0;
	ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);

	//快照句柄
	SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (SnapshotHandle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	//得到第一个进程顺便判断一下系统快照是否成功
	if (Process32First(SnapshotHandle, &ProcessEntry32))
	{
		do
		{
			//打开进程并返回句柄  //4 system
			ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
				FALSE, ProcessEntry32.th32ProcessID);   //打开目标进程  
												  //	
			if (ProcessHandle == NULL)// 权限太高 - 降低打开
			{
				ProcessHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
					FALSE, ProcessEntry32.th32ProcessID);   //打开目标进程

				if (ProcessHandle == NULL)
				{
					memcpy(ProcessFullPath, _T("打开进程失败"), _tcslen(_T("打开进程失败")));

					memcpy(isWow64Process, _T("无法判断"), _tcslen(_T("无法判断")));
					goto Label;

				}

			}
			//判断目标进程的位数

			if (XkIsWow64Process(ProcessHandle, &v1) == TRUE)
			{
				if (v1)
				{
					memcpy(isWow64Process, _T("32位"), _tcslen(_T("32位")));
				}
				else
				{
					memcpy(isWow64Process, _T("64位"), _tcslen(_T("64位")));
				}
			}
			else
			{
				memcpy(isWow64Process, _T("无法判断"), _tcslen(_T("无法判断")));
			}

			//通过进程句柄获得第一个模块句柄信息

			//加一个扩展库
			ReturnLength = GetModuleFileNameEx(ProcessHandle, ModuleHandle,
				ProcessFullPath,
				sizeof(ProcessFullPath));

			if (ReturnLength == 0)
			{
				//如果失败
				RtlZeroMemory(ProcessFullPath, MAX_PATH);

				QueryFullProcessImageName(ProcessHandle, 0, ProcessFullPath, &ReturnLength);	// 更推荐使用这个函数
				if (ReturnLength == 0)
				{
					memcpy(ProcessFullPath, _T("枚举信息失败"), _tcslen(_T("枚举信息失败")));
				}
			}
		Label:
			ZeroMemory(&v2, sizeof(v2));

			v2.ProcessIdentity = (HANDLE)ProcessEntry32.th32ProcessID;
			memcpy(v2.ProcessImageName, ProcessEntry32.szExeFile, (_tcslen(ProcessEntry32.szExeFile) + 1) * sizeof(TCHAR));
			memcpy(v2.ProcessFullPath, ProcessFullPath, (_tcslen(ProcessFullPath) + 1) * sizeof(TCHAR));
			memcpy(v2.isWow64Process, isWow64Process, (_tcslen(isWow64Process) + 1) * sizeof(TCHAR));
			ProcessInfo.push_back(v2);

			if (ProcessHandle != NULL)
			{
				CloseHandle(ProcessHandle);
				ProcessHandle = NULL;
			}

		} while (Process32Next(SnapshotHandle, &ProcessEntry32));
	}
	else
	{
		CloseHandle(SnapshotHandle);

		return FALSE;
	}

	CloseHandle(SnapshotHandle);

	return ProcessInfo.size() > 0 ? TRUE : FALSE;
}