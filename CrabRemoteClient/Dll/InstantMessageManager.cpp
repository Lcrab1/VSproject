#include "pch.h"
#include "InstantMessageManager.h"
#include "Common.h"
CIocpClient* __IocpClient = NULL;
CInstantMessageManager::CInstantMessageManager(CIocpClient* IocpClient) :CManager(IocpClient)
{
	//回传数据包到服务器
	BYTE	IsToken = CLIENT_REMOTE_MESSAGE_REPLY;
	IocpClient->OnSending((char*)&IsToken, 1);



	__IocpClient = IocpClient;   //不应该的  消息回调  




	//该函数是父类中实现
	WaitingForDialogOpen();   //等待服务器执行



}
CInstantMessageManager::~CInstantMessageManager()
{
	_tprintf(_T("~CRemoteMessageManage()\r\n"));
}

void CInstantMessageManager::HandleIo(PBYTE BufferData, ULONG_PTR BufferLength)
{
}