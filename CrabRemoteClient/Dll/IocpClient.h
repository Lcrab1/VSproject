#pragma once

#include<winsock2.h>
#include<windows.h>
#include<iostream>
#include"CArray.h"
#include"zconf.h"
#include"zlib.h"

using namespace std;
#pragma comment(lib,"WS2_32.lib")
#define PACKET_LENGTH 0x2000
#define PACKET_HEADER_LENGTH 13
#define PACKET_FLAG_LENGTH 5
#define MAX_SEND_BUFFER 0x2000

class CIocpClient
{
public:
	CIocpClient();
	~CIocpClient();
	BOOL ConnectServer(char* ServerAddress, unsigned short ConnectPort);
	static DWORD WINAPI WorkThreadProcedure(LPVOID ParameterData);
	BOOL IsReceiving()
	{
		return m_IsReceiving;
	}
	VOID Disconnect();
	VOID OnReceiving(char* BufferData, ULONG BufferLength);
	int OnSending(char* BufferData, ULONG BufferLength);
	BOOL SendWithSplit(char* BufferData, ULONG BufferLength, ULONG SplitLength);

private:
	BOOL m_IsReceiving = TRUE;
	CArray_ m_SendCompressedBufferData;
	char m_PacketHeaderFlag[PACKET_FLAG_LENGTH];

public:
	SOCKET m_ClientSocket;
	HANDLE m_WorkThreadHandle;
	HANDLE m_EventHandle;
};

