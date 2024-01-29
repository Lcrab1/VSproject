#include "pch.h"
#include "IocpServer.h"



CIocpServer::CIocpServer()
{
	//套接字类库的初始化
	WSADATA  v1 = { 0 };

	//指定要初始化的Windows Sockets库的版本。
	//具体来说，MAKEWORD(2, 2)将参数2和参数2合并为一个WORD类型的值，即0x0202，表示使用Winsock API版本2.2
	if (WSAStartup(MAKEWORD(2, 2), &v1) != 0)	//第一个参数是低位，第二个参数是高位
		
	{
		//初始化失败
	}
	m_listenSocket = INVALID_SOCKET;

	//初始化线程池
	m_ThreadsPoolMin = 0;
	m_ThreadsPoolMax = 0;
	m_WorkThreadsCount = 0;

	for (int i = 0; i < WORK_THREAD_MAX; i++)
	{
		m_WorkThreadHandle[i] = INVALID_HANDLE_VALUE;
	}

	m_KillEventHandle = NULL;

}
CIocpServer::~CIocpServer()
{
	WSACleanup();	//释放Windows Sockets库所占用的资源，并终止对网络套接字的使用
}

BOOL CIocpServer::ServerRun(USHORT ListenPort)
{

	BOOL IsOk = TRUE;//绑定套接
	SOCKADDR_IN	ServerAddress;   //结构体


	//创建监听套接字Address Family  

	//应用   Http  
	//表示
	//会话
	//传输   TCP(Stream)  UDP(datagram)   
	//网络   IP  寻址
	//数据链路
	//物理
	

	//创建一个监听事件
	m_listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);   //重叠 异步Io  CPU
	if (m_listenSocket == INVALID_SOCKET)
	{
		goto Error;
	}

	//初始化Server端网卡

	//通信端口 0 - 2^16-1
	//Http:超文本传输协议  80


	ServerAddress.sin_port = htons(ListenPort);   //host to net String ini 文件设置的监听端口撸进结构中
	ServerAddress.sin_family = AF_INET;
	//ServerAddress.sin_addr.s_addr = INADDR_ANY;
	ServerAddress.sin_addr.S_un.S_addr = INADDR_ANY;  //IP地址
	//INADDR_ANY是一个特殊的宏常量，表示绑定到所有可用的网络接口上。

	//绑定套接字
	//套接字与网卡关联
	
	//将服务器监听套接字与指定的 IP 地址和端口号绑定在一起
	IsOk = bind(m_listenSocket,
		(sockaddr*)&ServerAddress,
		sizeof(ServerAddress));

	if (IsOk == SOCKET_ERROR)
	{
		goto Error;
	}

	//保安上班监听
	//将服务器的监听套接字置为监听状态
	IsOk = listen(m_listenSocket, SOMAXCONN);   //监听能力
	if (IsOk == SOCKET_ERROR)
	{
		goto Error;
	}


	IocpInit();

Error:;

	if (IsOk != TRUE)
	{
		if (m_listenSocket != INVALID_SOCKET)
		{
			closesocket(m_listenSocket);
			m_listenSocket = INVALID_SOCKET;
		}
	}
	return 0;
}

BOOL CIocpServer::IocpInit()
{
	//定义一个官方结构
	SYSTEM_INFO SystemInfo;
	//获得PC中有几核
	GetSystemInfo(&SystemInfo);      //获取系统信息

	//线程池
	m_ThreadsPoolMin = 1;
	m_ThreadsPoolMax = SystemInfo.dwNumberOfProcessors * 2;

	//测试最大线程数
	/*TCHAR testThreadsPoolMax[0x1000] = { 0 };
	_stprintf_s(testThreadsPoolMax, "m_ThreadsPoolMax=%d", m_ThreadsPoolMax);
	MessageBox(NULL, testThreadsPoolMax, testThreadsPoolMax, 0);*/
	
	//启动两个工作线程
	ULONG  WorkThreadCount = WORK_THREAD_MAX;
	HANDLE WorkThreadHandle = NULL;

	for (int i = 0; i < WorkThreadCount; i++)
	{
		//线程句柄也不关心
		m_WorkThreadHandle[i] = (HANDLE)CreateThread(NULL,	             //创建工作线程目的是处理投递到完成端口中的任务			
			0,
			(LPTHREAD_START_ROUTINE)WorkThreadProcedure,
			(void*)this,
			0,
			NULL);   //不关心线程Identity

		if (m_WorkThreadHandle[i] == NULL)
		{
			//CloseHandle(m_CompletionPortHandle);
			return FALSE;
		}
		m_WorkThreadsCount++;
	}



	return TRUE;
}

DWORD WINAPI CIocpServer::ListenThreadProcedure(LPVOID ParameterData)
{
	CIocpServer* v1 = (CIocpServer*)ParameterData;
	DWORD    v2 = 0;

	while (1)
	{
		v2 = WaitForSingleObject(v1->m_KillEventHandle, 100);
		if (v2 == WAIT_OBJECT_0)
		{
			//由于析构函数触发m_KillEventHandle事件
			break;
		}


	}
	return 0;
}

DWORD WINAPI WorkThreadProcedure(LPVOID ParameterData)
{
	return 0;
}
