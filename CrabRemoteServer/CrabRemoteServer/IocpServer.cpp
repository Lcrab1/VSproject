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
	InitializeCriticalSection(&m_CriticalSection);


	m_CompletionPortHandle = INVALID_HANDLE_VALUE;

	m_CurrentThreadsCount = 0;
	m_BusyThreadsCount = 0;

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
	

	if (m_CompletionPortHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_CompletionPortHandle);
		m_CompletionPortHandle = INVALID_HANDLE_VALUE;
	}

	DeleteCriticalSection(&m_CriticalSection);
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

	m_CompletionPortHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (m_CompletionPortHandle == NULL)   //创建完成端口
	{
		return FALSE;
	}

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
			CloseHandle(m_CompletionPortHandle);
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

		//v2 = WaitForSingleObject(v1->m_ListenEventHandle, 100);
		//等待监听事件授信(监听套接字授信)
		v2 = WSAWaitForMultipleEvents(1,
			&v1->m_ListenEventHandle,          //他其实网络事件   ListenSocket   
			FALSE,							   //true 全部信号授信后 才能向下执行 
			100,
			FALSE);

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
	BOOL        Ok;
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

	//关联内存
	//buf是个指针
	ContextObject->bufferReceive.buf = (char*)ContextObject->bufferData;
	ContextObject->bufferReceive.len = sizeof(ContextObject->bufferData);

	

	//将生成的通信套接字与完成端口句柄相关联
	HANDLE Handle = CreateIoCompletionPort((HANDLE)ClientSocket,
		m_CompletionPortHandle, (ULONG_PTR)ContextObject, 0);     //指针 完成Key
	//关联成功，返回值是m_CompletionPortHandle
	//关联失败，返回值是NULL


	if (Handle != m_CompletionPortHandle)
	{

		delete ContextObject;  //销毁对象
		ContextObject = NULL;

		if (ClientSocket != INVALID_SOCKET)
		{
			closesocket(ClientSocket);  //销毁套接字
			ClientSocket = INVALID_SOCKET;
		}

		return;
	}

	//保活机制

	//设置套接字的选项卡 Set KeepAlive 开启保活机制 SO_KEEPALIVE 
	//保持连接检测对方主机是否崩溃如果2小时内在此套接口的任一方向都没
	//有数据交换，TCP就自动给对方 发一个保持存活
	m_KeepAliveTime =3;
	const BOOL IsKeepAlive = TRUE;
	if (setsockopt(ContextObject->clientSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&IsKeepAlive, sizeof(IsKeepAlive)) != 0)
	{
	}

	//设置超时详细信息
	tcp_keepalive	KeepAlive;			
	KeepAlive.onoff = 1; // 启用保活					 //以下两个数据的单位都是毫秒
	KeepAlive.keepalivetime = m_KeepAliveTime;       //超过3分钟没有数据，就发送探测包
	KeepAlive.keepaliveinterval = 1000 * 10;         //重试间隔为10秒 Resend if No-Reply
	//控制套接字行为
	WSAIoctl
	(
		ContextObject->clientSocket,
		SIO_KEEPALIVE_VALS,
		&KeepAlive,
		sizeof(KeepAlive),
		NULL,
		0,
		(unsigned long*)&IsKeepAlive,
		0,
		NULL
	);

	//在做服务器时，如果发生客户端网线或断电等非正常断开的现象，如果服务器没有设置SO_KEEPALIVE选项，
	//则会一直不关闭SOCKET。因为上的的设置是默认两个小时时间太长了所以我们就修正这个值


	_CCriticalSection CriticalSection(&m_CriticalSection);  //Stack Object
	m_ConnectContextList.AddTail(ContextObject);     //插入到我们的内存列表中

	COVERLAPPEDEX* OverlappedEx = new COVERLAPPEDEX(IO_INITIALIZE);

	Ok = FALSE;
	//向完成端口中投递一个请求
	//工作线程会等待完成端口的完成状态
	Ok = PostQueuedCompletionStatus(m_CompletionPortHandle,
		0, (ULONG_PTR)ContextObject, &OverlappedEx->m_Overlapped);  //自己向自己的完成端口投递请求

	if ((!Ok && GetLastError() != ERROR_IO_PENDING))
	{
		//如果投递失败
		RemoveContextObject(ContextObject);
		return;
	}

	//该上线用户已完成了上线的请求
	//服务器向该用户投递PostRecv请求


	PostReceive(ContextObject);
}

PCONTEXT_OBJECT CIocpServer::AllocateContextObject()
{
	PCONTEXT_OBJECT ContextObject = NULL;   //分配一个对象

	//进入一个临界区
		//进入临界区
	//RAII技术
	_CCriticalSection CriticalSection(&m_CriticalSection);   //自定义封装了一个线程同步关键段工具
	//判断内存池是否为空

	//内存池
	if (m_FreeContextList.IsEmpty() == FALSE)
	{
		//内存池取内存
		ContextObject = m_FreeContextList.RemoveHead();
	}
	else
	{
		ContextObject = new CONTEXT_OBJECT;   //第一次客户端上下背景文生成的时候
	}

	if (ContextObject != NULL)
	{
		//初始化成员变量
		ContextObject->MemberInit();  //Clinet BufferData WSABuffer  WSABuffer  Array1 Array2 Array3
	}
	return ContextObject;
}

PCONTEXT_OBJECT CIocpServer::RemoveContextObject(PCONTEXT_OBJECT contextObject)
{
	_CCriticalSection CriticalSection(&m_CriticalSection);
	//在内存中查找该用户的上下背景文数据结构
	if (m_ConnectContextList.Find(contextObject))
	{
		//取消在当前套接字的异步IO 以前的未完成的异步请求全部立即取消   
		CancelIo((HANDLE)contextObject->clientSocket);    //会将该对象上没有得到完成的异步Io立即完成
		//关闭套接字
		closesocket(contextObject->clientSocket);
		contextObject->clientSocket = INVALID_SOCKET;

		//判断还有没有异步IO请求在当前套接字上
		while (!HasOverlappedIoCompleted((LPOVERLAPPED)contextObject))   //查看一下完成端口还有没有王浩的箱子
		{
			Sleep(1);
		}
		//将该内存结构回收至内存池
		MoveContextObjectToFreePool(contextObject);   //回收对象内存到内存池
	}
}

VOID CIocpServer::PostReceive(PCONTEXT_OBJECT contextObject)
{
	//向我们的刚上线的用户的投递一个接受数据的请求
	//如果该请求得到完成(用户发送数据)
	//工作线程(守候在完成端口)会响应并调用HandleIO函数
	COVERLAPPEDEX* OverlappedEx = new COVERLAPPEDEX(IO_RECEIVE);

	DWORD			ReturnLength;
	ULONG			Flags = MSG_PARTIAL;   //没有意义

	//函数返回但是请求没有得到完成
	int IsOk = WSARecv(contextObject->clientSocket,
		&contextObject->bufferReceive,   //接受数据的内存
		1,
		&ReturnLength,                      //Transfe	rBufferLength
		&Flags,
		&OverlappedEx->m_Overlapped,   //事件
		NULL);

	//返回值是错误 && 不是错误中的未完成
	//WSA_IO_PENDING表示操作将在未来完成
	if (IsOk == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		//请求发送错误
		RemoveContextObject(contextObject);   //完犊子  
	}

}

BOOL CIocpServer::HandleIo(PACKET_TYPE PacketType, PCONTEXT_OBJECT ContextObject, DWORD NumberOfBytesTransferred)
{
	BOOL v1 = FALSE;

	if (IO_INITIALIZE == PacketType)
	{
		MessageBox(NULL, _T("IO_INITIALIZE"), _T("IO_INITIALIZE"), 0);
	}

	if (IO_RECEIVE == PacketType)
	{

	}

	if (IO_SEND == PacketType)
	{

	}
	return v1;
}

VOID CIocpServer::MoveContextObjectToFreePool(CONTEXT_OBJECT* ContextObject)
{
	_CCriticalSection CriticalSection(&m_CriticalSection);

	POSITION Position = m_ConnectContextList.Find(ContextObject);
	if (Position)
	{

		ContextObject->m_ReceivedBufferDataCompressed.ClearArray();
		ContextObject->m_ReceivedBufferDataDecompressed.ClearArray();
		ContextObject->m_SendBufferDataCompressed.ClearArray();

		memset(ContextObject->bufferData, 0, PACKET_LENGTH);
		m_FreeContextList.AddTail(ContextObject);                         //回收至内存池
		m_ConnectContextList.RemoveAt(Position);                          //从内存结构中移除

	}
}

DWORD WINAPI WorkThreadProcedure(LPVOID ParameterData)
{
	CIocpServer* v1 = (CIocpServer*)ParameterData;
	HANDLE   CompletionPortHandle = v1->m_CompletionPortHandle;
	DWORD    NumberOfBytesTransferred = 0;
	LPOVERLAPPED     Overlapped = NULL;
	PCONTEXT_OBJECT  ContextObject = NULL;  //指针
	COVERLAPPEDEX* OverlappedEx = NULL;     //指针

		//原子锁
	InterlockedIncrement(&v1->m_CurrentThreadsCount);  //1  2
	InterlockedIncrement(&v1->m_BusyThreadsCount);     //1  2


	ULONG            v3 = 0;
	BOOL             IsOk1 = FALSE;

	while (v1->m_Working == TRUE)
	{
		InterlockedDecrement(&v1->m_BusyThreadsCount);

		BOOL IsOk2 = GetQueuedCompletionStatus(
			CompletionPortHandle,
			&NumberOfBytesTransferred,                    //完成多少数据
			(PULONG_PTR)&ContextObject,       //完成Key   获得的完成Key就是咱们投递过去的
			&Overlapped, 60000);

		DWORD LastError = GetLastError();
		OverlappedEx = CONTAINING_RECORD(Overlapped, COVERLAPPEDEX, m_Overlapped);  //数据强制转换


		v3 = InterlockedIncrement(&v1->m_BusyThreadsCount);

		//完成端口异常(强制退出)
		if (!IsOk2   &&     LastError != WAIT_TIMEOUT)
		{
			//关闭线程
			if (ContextObject &&   v1->m_Working == FALSE  &&   NumberOfBytesTransferred == 0)
			{
				//当对方的套接字发生了关闭	
				v1->RemoveContextObject(ContextObject);
			}
			continue;
		}

		//线程池调度
		if (!IsOk1)
		{

			//如果你在干活就判断一下是否可以再调度一个线程

			//分配一个新的线程到线程到线程池
			if (v3 == v1->m_CurrentThreadsCount)
			{

				if (v3 < v1->m_ThreadsPoolMax)//
				{


					if (ContextObject != NULL)   //你是否在干活
					{

						HANDLE ThreadHandle = (HANDLE)CreateThread(NULL,
							0,
							(LPTHREAD_START_ROUTINE)WorkThreadProcedure,
							(void*)v1,
							0,
							NULL);
						InterlockedIncrement(&v1->m_WorkThreadsCount);

						CloseHandle(ThreadHandle);
					}


				}
			}	

			//销毁一个线程从线程池
			if (!IsOk2  &&  LastError == WAIT_TIMEOUT)
			{
				if (ContextObject == NULL)
				{
						if (v1->m_CurrentThreadsCount > v1->m_ThreadsPoolMin)
						{
							break;   //销毁一个线程
						}
					IsOk2 = TRUE;
				}
			}
		}


		//处理完成的请求
		if (!IsOk1)
		{
			//当前线程要对完成请求进行处理
			//请求得到完成(两种请求 IO_INITIALIZE IO_RECEIVE )
			if (IsOk2 && OverlappedEx != NULL && ContextObject != NULL)
			{
				try
				{

					//请求得到完成的处理函数
					v1->HandleIo(OverlappedEx->m_PackType, ContextObject, NumberOfBytesTransferred);

					//没有释放内存
					ContextObject = NULL;

				}
				catch (...) {}
			}
		}

		if (OverlappedEx)
		{
			delete OverlappedEx;
			OverlappedEx = NULL;
		}

	}

	InterlockedDecrement(&v1->m_WorkThreadsCount);
	InterlockedDecrement(&v1->m_CurrentThreadsCount);
	InterlockedDecrement(&v1->m_BusyThreadsCount);

	TCHAR testWorkThreadsExit[0x1000] = { 0 };
	_stprintf_s(testWorkThreadsExit, _T("ThreadIdentity:%d"), GetCurrentThreadId());
	MessageBox(NULL, _T("WorkThreadProcedure"), testWorkThreadsExit, 0);


	return 0;
}
