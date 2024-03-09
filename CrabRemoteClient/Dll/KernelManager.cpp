#include "pch.h"
#include "KernelManager.h"
#include "Common.h"
CKernelManager::CKernelManager(CIocpClient* IocpClient) :CManager(IocpClient)
{
	int    m_ThreadHandleCount = 0;
	memset(m_ThreadHandle, 0, sizeof(m_ThreadHandle));
}
CKernelManager::~CKernelManager()
{

}

void CKernelManager::HandleIo(PBYTE BufferData, ULONG_PTR BufferLength)
{
	BYTE IsToken;

	switch (BufferData[0])
	{
	case CLIENT_GET_OUT_REQUIRE:
	{

		IsToken = CLIENT_GET_OUT_REPLY;

		m_IocpClient->OnSending((char*)&IsToken, 1);

		break;
	}
	case CLIENT_REMOTE_MESSAGE_REQUIRE:
	{

		//启动一个线程
		m_ThreadHandle[m_ThreadHandleCount++] = CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE)RemoteMessageProcedure,
			NULL, 0, NULL);


		break;
	}
	}
}

DWORD WINAPI RemoteMessageProcedure(LPVOID ParameterData)
{
	//建立一个新的连接
	CIocpClient	IocpClient;   //新的链接

	if (!IocpClient.ConnectServer(__ServerAddress, __ConnectPort))   //产生一个新的链接  while(接受  )  m_Manger->HandiO
		return -1;
	CInstantMessageManager	InstantMessageManager(&IocpClient);



	//等待服务器弹出窗口
	IocpClient.WaitingForEvent();  //一个事件等待
}