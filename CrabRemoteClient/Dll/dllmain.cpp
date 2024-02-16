// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include<tchar.h>
#include<iostream>
#include"IocpClient.h"
using namespace std;


char __ServerAddress[MAX_PATH] = { 0 };
USHORT __ConnectPort = 0;

DWORD WINAPI WorkThreadProcedure(LPVOID ParameterData);


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void ClientRun(char* ServerAddrss, USHORT ConnectPort)
{
    memcpy(__ServerAddress, ServerAddrss, strlen(ServerAddrss));
    __ConnectPort = ConnectPort;

    //启动一个工作线程
    HANDLE ThreadHandle = CreateThread(NULL, 0,
        (LPTHREAD_START_ROUTINE)WorkThreadProcedure,
        NULL, 0, NULL);
   // int LastError = WSAGetLastError();
    //等待工作线程的正常退出
    WaitForSingleObject(ThreadHandle, INFINITE);

    _tprintf(_T("Client Bye Bye!!!\r\n"));

    if (ThreadHandle != NULL) {
        CloseHandle(ThreadHandle);
    }

}

DWORD WINAPI WorkThreadProcedure(LPVOID ParameterData) 
{

    //启动一个客户端的通信类
    CIocpClient IocpClient; //触发构造函数
    BOOL ok = FALSE;


    while (1) {
        if (ok == TRUE) {
            break;
        }
        DWORD TickCount = GetTickCount64();
        if (!IocpClient.ConnectServer(__ServerAddress, __ConnectPort)) {
            continue;
        }


        //SendLoginInformation(&IocpClient, GetTickCount() - TickCount);


        Sleep(INFINITE); 
    }


    return 0;
}