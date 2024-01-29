#pragma once
#include <Windows.h>
#include <iostream>
#include <winsock2.h>    //通信Socket2  套接字  
using namespace std;
#pragma comment(lib,"Ws2_32.lib")   //kernel32.dll User32.dll  Ws2_32.dll 加载这个库为使用通信接口
#define WORK_THREAD_MAX 2
//线程回调函数的创建法1
DWORD WINAPI WorkThreadProcedure(LPVOID ParameterData);
class CIocpServer   //IoCompletePort
{
public:
	CIocpServer();
	~CIocpServer();
	BOOL ServerRun(USHORT ListenPort);
	BOOL IocpInit();
	//线程回调函数的创建法2
	static DWORD WINAPI ListenThreadProcedure(LPVOID ParameterData);

public:
	//监听套接字
	SOCKET m_listenSocket;

	HANDLE m_KillEventHandle;    //当该事件对象授信 销毁资源

	//线程池
	ULONG m_ThreadsPoolMin;	
	ULONG m_ThreadsPoolMax;

	//工作线程
	HANDLE m_WorkThreadHandle[WORK_THREAD_MAX];
	ULONG m_WorkThreadsCount;

	//监听线程
	HANDLE m_ListenThreadHandle;   


};
