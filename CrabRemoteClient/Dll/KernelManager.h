#pragma once
#include "Manager.h"
class CKernelManager:public CManager
{
public:
	CKernelManager(CIocpClient* IocpClient);
	~CKernelManager();
	void HandleIo(PBYTE BufferData, ULONG_PTR BufferLength);
private:
};

