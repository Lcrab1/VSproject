#include "pch.h"
#include "KernelManager.h"
#include "Common.h"
CKernelManager::CKernelManager(CIocpClient* IocpClient) :CManager(IocpClient)
{

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
	}
}