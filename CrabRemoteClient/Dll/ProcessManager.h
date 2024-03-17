#pragma once
#include "Manager.h"
#include"Common.h"
class CProcessManager :
    public CManager
{
public:
    CProcessManager(CIocpClient* IocpClient);
    ~CProcessManager();
    BOOL SendClientProcessList();
    void HandleIo(PBYTE BufferData, ULONG_PTR BufferLength);
};

