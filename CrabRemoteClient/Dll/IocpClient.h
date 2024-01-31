#pragma once

#include<winsock2.h>
#include<windows.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"WS2_32.lib")

class CIocpClient
{
public:
	CIocpClient();
	~CIocpClient();
	BOOL ConnectServer(char* ServerAddress, unsigned short ConnectPort);



	SOCKET m_ClientSocket;
	HANDLE m_WorkThreadHandle;
	HANDLE m_EventHandle;
};

