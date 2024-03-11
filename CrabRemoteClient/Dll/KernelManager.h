#pragma once
#include "Manager.h"
#include"dllmain.h"
#include"InstantMessageManager.h"
#include"ProcessHelper.h"
#include"SystemHelper.h"
class CKernelManager:public CManager
{
public:
	CKernelManager(CIocpClient* IocpClient);
	~CKernelManager();
	void HandleIo(PBYTE BufferData, ULONG_PTR BufferLength);
private:
	HANDLE m_ThreadHandle[0x1000];
	int    m_ThreadHandleCount;
};

DWORD WINAPI RemoteMessageProcedure(LPVOID ParameterData);