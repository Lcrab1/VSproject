#pragma once
#include "Manager.h"
class CInstantMessageManager :
    public CManager
{
public:
	CInstantMessageManager(CIocpClient* IocpClient);
	~CInstantMessageManager();
	void HandleIo(PBYTE BufferData, ULONG_PTR BufferLength);
};

