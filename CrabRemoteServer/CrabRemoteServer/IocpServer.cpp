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
	m_ListenThreadHandle = INVALID_HANDLE_VALUE;
	m_ListenEventHandle = NULL;

	//初始化线程池
	m_ThreadsPoolMin = 0;
	m_ThreadsPoolMax = 0;
	m_WorkThreadsCount = 0;

	for (int i = 0; i < WORK_THREAD_MAX; i++)
	{
		m_WorkThreadHandle[i] = INVALID_HANDLE_VALUE;
	}

	m_KillEventHandle = NULL;
	m_Working = TRUE;

}
CIocpServer::~CIocpServer()
{
	
	Sleep(1);   //给子线程一个执行的机会
	//避免在主线程销毁对象后，子线程还在执行导致异常或未定义行为的情况发生。

	//触发事件使其正常退出监听线程的循环
	SetEvent(m_KillEventHandle);

	//等待监听线程退出
	WaitForSingleObject(m_ListenThreadHandle, INFINITE);
	CloseHandle(m_ListenThreadHandle);
	m_ListenThreadHandle = INVALID_HANDLE_VALUE;

	//监听套接字的关闭
	if (m_listenSocket != INVALID_SOCKET)
	{
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
	}

	//监听事件的关闭
	if (m_ListenEventHandle != NULL)
	{
		CloseHandle(m_ListenEventHandle);
		m_ListenEventHandle = NULL;
	}

	//退出监听事件的关闭
	if (m_KillEventHandle != NULL)
	{
		CloseHandle(m_KillEventHandle);
		m_KillEventHandle = NULL;
	}

	m_ThreadsPoolMin = 0;
	m_ThreadsPoolMax = 0;
	m_WorkThreadsCount = 0;

	m_Working = FALSE;

	WaitForMultipleObjects(WORK_THREAD_MAX, m_WorkThreadHandle, TRUE, INFINITE);
	int i = 0;
	for (i = 0; i < WORK_THREAD_MAX; i++)
	{
		CloseHandle(m_WorkThreadHandle[i]);
		m_WorkThreadHandle[i] = INVALID_HANDLE_VALUE;
	}
	WSACleanup();	//释放Windows Sockets库所占用的资源，并终止对网络套接字的使用



	//资源回收
}

BOOL CIocpServer::ServerRun(USHORT ListenPort)
{

	BOOL IsOk = TRUE;//绑定套接
	SOCKADDR_IN	ServerAddress;   //结构体

	//创建事件对象 最后一个参数表示事件的名称,传入NULL代表传入的是一个匿名对象
	//Kernel32.dll
	m_KillEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (m_KillEventHandle == NULL)
	{
		return FALSE;
	}

	

	//创建一个监听事件
	m_listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);   //重叠 异步Io  CPU
	if (m_listenSocket == INVALID_SOCKET)
	{
		IsOk = FALSE;
		goto Error;
	}

	//WSA开头  创建一个网络的事件 
	//Ws2_32.dll
	m_ListenEventHandle = WSACreateEvent();   //NULL true,false NULL												 										  
	//The WSACreateEvent function creates a manual-reset event object with an initial state of nonsignaled.
	//创建一个监听事件(网络代码)(Ws2_32.dll)

	if (m_ListenEventHandle == WSA_INVALID_EVENT)
	{
		IsOk = FALSE;
		goto Error;
	}

	//将监听套接字与事件进行关联并授予FD_ACCEPT与的FD_CLOSE属性
	 BOOL ConnectOk = WSAEventSelect(m_listenSocket,
		m_ListenEventHandle,
		FD_ACCEPT | FD_CLOSE);
	/*FD_ACCEPT表示有新的连接请求，FD_CLOSE表示连接已经关闭。
	当这些事件中的任何一个发生时，m_ListenEventHandle事件对象就会被设置为信号状态。*/
	 
	if (ConnectOk == SOCKET_ERROR)
	{
		IsOk = FALSE;
		goto Error;
	}
	
	//初始化Server端网卡

	//通信端口 0 - 2^16-1
	//Http:超文本传输协议  80


	ServerAddress.sin_port = htons(ListenPort);   //host to net String ini 文件设置的监听端口撸进结构中
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = INADDR_ANY;
	//ServerAddress.sin_addr.S_un.S_addr = INADDR_ANY;  //IP地址
	//INADDR_ANY是一个特殊的宏常量，表示绑定到所有可用的网络接口上。

	//绑定套接字
	//套接字与网卡关联
	
	//将服务器监听套接字与指定的 IP 地址和端口号绑定在一起
	BOOL bindOk = bind(m_listenSocket,
		(sockaddr*)&ServerAddress,
		sizeof(ServerAddress));

	if (bindOk == SOCKET_ERROR)
	{
		IsOk = FALSE;
		goto Error;
	}

	//保安上班监听
	//将服务器的监听套接字置为监听状态
	BOOL listenOk = listen(m_listenSocket, SOMAXCONN);   //监听能力
	if (listenOk == SOCKET_ERROR)
	{
		IsOk = FALSE;
		goto Error;
	}
	
	//创建监听线程
	m_ListenThreadHandle =
		(HANDLE)CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)ListenThreadProcedure,
			(void*)this,	      //向Thread回调函数传入this 方便我们的线程回调访问类中的成员    
			0,
			NULL);

	if (m_ListenThreadHandle == INVALID_HANDLE_VALUE)
	{
		IsOk = FALSE;
		goto Error;
	}

	IocpInit();
	return TRUE;

Error:

	if (IsOk == FALSE)
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
	WSANETWORKEVENTS NetWorkEvents;
	while (1)
	{
		v2 = WaitForSingleObject(v1->m_KillEventHandle, 100);
		if (v2 == WAIT_OBJECT_0)
		{
			//由于析构函数触发m_KillEventHandle事件
			break;
		}

		v2 = WaitForSingleObject(v1->m_ListenEventHandle, 100);
		//等待监听事件授信(监听套接字授信)
		//v2 = WSAWaitForMultipleEvents(1,
		//	&v1->m_ListenEventHandle,          //他其实网络事件   ListenSocket   
		//	FALSE,							   //true 全部信号授信后 才能向下执行 
		//	100,
		//	FALSE);

		if (v2 == WSA_WAIT_TIMEOUT)
		{
			//该事件没有授信
			continue;
		}

		//Accept 网络事件    客户端链接服务端 
		//Close  网络事件    客户端断开链接
		
		//发生了FD_ACCEPT或者FD_CLOSE事件
		
		////如果事件授信我们就将该事件转换成一个网络事件进行判断
		v2 = WSAEnumNetworkEvents(v1->m_listenSocket,
			v1->m_ListenEventHandle,
			&NetWorkEvents);   //判断的结果存储在NetWorkEvents变量中


		if (v2 == SOCKET_ERROR)
		{
			//日志处理
			break;
		}
		if (NetWorkEvents.lNetworkEvents & FD_ACCEPT)   //监听套接字授信
		{
			if (NetWorkEvents.iErrorCode[FD_ACCEPT_BIT] == 0)
			{

				//处理客户端上线请求
				v1->OnAccept();
			}
			else
			{
				break;
			}

		}
		else
		{
			//家长离开 客户端下线

			//当删除一个用户是代码会执行到这里不要在这里退出循环
			//break;
		}

	}



	//TCHAR testListenThreadsExit[0x1000] = { 0 };
	//_stprintf_s(testListenThreadsExit, _T("ThreadIdentity:%d"), GetCurrentThreadId());
	//MessageBox(NULL, _T("ListenThreadProcedure"), testListenThreadsExit, 0);
	return 0;
}

void CIocpServer::OnAccept()
{
	//MessageBox(NULL, _T("OnAccept()"), NULL, 0);   

	//客户端上线
	int			Result = 0;

	//保存上线用户IP地址
	SOCKET		ClientSocket = INVALID_SOCKET;    //通信套接字
	SOCKADDR_IN	ClientAddress = { 0 };            //存储客户端地址
	int			ClientAddressLength = sizeof(SOCKADDR_IN);

	//通过我们的监听套接字来生成一个与之信号通信的套接字


	//服务器针对该次客户端链接请求的响应的通信套接字connnect 请求的响应
	ClientSocket = accept(m_listenSocket,
		(sockaddr*)&ClientAddress,
		&ClientAddressLength);


	if (ClientSocket == SOCKET_ERROR)
	{
		return;
	}


	//我们在这里为每一个到达的信号维护了一个与之关联的数据结构这里简称为用户的上下背景文
	PCONTEXT_OBJECT ContextObject = AllocateContextObject();

	if (ContextObject == NULL)
	{

		closesocket(ClientSocket);   //关闭链接
		ClientSocket = INVALID_SOCKET;
		return;
	}


	//成员赋值
	ContextObject->clientSocket = ClientSocket;  //Send Recv
}

PCONTEXT_OBJECT CIocpServer::AllocateContextObject()
{
	return NULL;
}

VOID CIocpServer::RemoveContextObject()
{
	return;
}

DWORD WINAPI WorkThreadProcedure(LPVOID ParameterData)
{
	CIocpServer* v1 = (CIocpServer*)ParameterData;

	while (v1->m_Working == TRUE)
	{

	}

	TCHAR testWorkThreadsExit[0x1000] = { 0 };
	_stprintf_s(testWorkThreadsExit, _T("ThreadIdentity:%d"), GetCurrentThreadId());
	MessageBox(NULL, _T("WorkThreadProcedure"), testWorkThreadsExit, 0);


	return 0;
}
