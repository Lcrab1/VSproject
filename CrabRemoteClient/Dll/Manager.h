#pragma once
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "IocpClient.h"

using namespace std;

//抽象类  -- --- --- --- ---

class CIocpClient;
class CManager
{
public:
	CManager(CIocpClient* IocpClient);
	~CManager();

	virtual void HandleIo(PBYTE BufferData, ULONG_PTR BufferLength)
	{

	}
public:
	CIocpClient* m_IocpClient;   //通信类对象指针
};