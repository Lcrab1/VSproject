#pragma once
#include <Windows.h>
#include <iostream>
#include <winsock2.h>    //通信Socket2  套接字  
#include"CArray.h"
using namespace std;
#pragma comment(lib,"Ws2_32.lib")   //kernel32.dll User32.dll  Ws2_32.dll 加载这个库为使用通信接口
#define WORK_THREAD_MAX 2
#define PACKET_LENGTH 0x2000

typedef struct _CONTEXT_OBJECT_
{
	SOCKET clientSocket;
	WSABUF bufferReceive;
	WSABUF bufferSend;
	char bufferData[PACKET_LENGTH];


	//数据的传送使用压缩算法
	CArray_ m_ReceivedBufferDataCompressed;
	CArray_ m_ReceivedBufferDataDecompressed;
	CArray_ m_SendBufferDataCompressed;

	int DialogID;
	HANDLE DialogHandle;

	//VOID MemberInit()
	//{
	//	clientSocket = INVALID_SOCKET;
	//	memset(bufferData, 0, sizeof(char) * PACKET_DATA);
	//	memset(&bufferReceive, 0, sizeof(WSABUF));
	//	memset(&bufferSend, 0, sizeof(WSABUF));
	//	DialogHandle = NULL;
	//	DialogID = 0;

	//}
}CONTEXT_OBJECT,*PCONTEXT_OBJECT;

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
	void OnAccept();

	PCONTEXT_OBJECT AllocateContextObject();
	VOID RemoveContextObject();
public:
	//监听套接字
	SOCKET m_listenSocket;



	//线程池
	ULONG m_ThreadsPoolMin;	
	ULONG m_ThreadsPoolMax;

	//工作线程
	HANDLE m_WorkThreadHandle[WORK_THREAD_MAX];
	ULONG m_WorkThreadsCount;

	//监听线程
	HANDLE m_ListenThreadHandle;   
	HANDLE m_ListenEventHandle;

	BOOL m_Working;
	HANDLE m_KillEventHandle;    //当该事件对象授信 销毁资源
};
