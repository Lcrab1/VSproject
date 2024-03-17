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
    void XkCreateProcess(PBYTE bufferData, ULONG_PTR BufferLength);
    void XkOpenProcess(PBYTE bufferData, ULONG_PTR BufferLength);
};

