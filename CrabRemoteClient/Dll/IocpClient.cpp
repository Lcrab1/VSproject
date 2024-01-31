#include "pch.h"
#include "IocpClient.h"

CIocpClient::CIocpClient()
{
	//初始化套接字类库
	WSADATA v1;
	if (WSAStartup(MAKEWORD(2, 2), &v1) != 0)
	{
		return;
	}
	
	//通信套接字
	m_ClientSocket = INVALID_SOCKET;

	m_WorkThreadHandle = NULL;
	m_EventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CIocpClient::~CIocpClient()
{
	if (m_ClientSocket != INVALID_SOCKET)
	{
		closesocket(m_ClientSocket);
		m_ClientSocket = INVALID_SOCKET;
	}

	//关闭工作线程句柄
	if (m_WorkThreadHandle != NULL)
	{
		CloseHandle(m_WorkThreadHandle);
		m_WorkThreadHandle = NULL;
	}
	//关闭事件
	if (m_EventHandle != NULL)
	{
		CloseHandle(m_EventHandle);
		m_EventHandle = NULL;
	}


	WSACleanup();
}

BOOL CIocpClient::ConnectServer(char* ServerAddress, unsigned short ConnectPort)
{
	//生成一个通信套接字
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ClientSocket == SOCKET_ERROR)
	{
		return FALSE;
	}
	//构造sockaddr_in结构也就是Server的IPAddress结构
	sockaddr_in	v1;
	v1.sin_family = AF_INET;
	v1.sin_port = htons(ConnectPort);
	v1.sin_addr.S_un.S_addr = inet_addr(ServerAddress);
	//链接服务器
	BOOL connectOk = connect(m_ClientSocket, (SOCKADDR*)&v1, sizeof(sockaddr_in));
	BOOL lastError = GetLastError();
	if (connectOk == SOCKET_ERROR)
	{
		int LastError = WSAGetLastError();
		if (m_ClientSocket != INVALID_SOCKET)
		{
			closesocket(m_ClientSocket);
			m_ClientSocket = INVALID_SOCKET;
		}
		return FALSE;
	}

	//	m_WorkThreadHandle = (HANDLE)CreateThread(NULL, 0,
	//		(LPTHREAD_START_ROUTINE)WorkThreadProcedure, (LPVOID)this, 0, NULL);   //接收数据
	return 0;
}
