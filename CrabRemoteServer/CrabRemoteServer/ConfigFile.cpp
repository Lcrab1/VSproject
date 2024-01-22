﻿#include "pch.h"
#include "ConfigFile.h"

ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

BOOL ConfigFile::ConfigFileInit()
{
		TCHAR  fileFullPath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, fileFullPath, MAX_PATH);   //获得当前可执行文件的绝对路径

		TCHAR* v1 = NULL;

		//fileFullPath
		//Z:\_CConfigFile\Debug\_CConfigFile.exe
		
		//Z:\_CConfigFile\Debug\_CConfigFile.ini
		v1 = _tcsstr(fileFullPath, _T("."));   //查找
		if (v1 != NULL)
		{
			*v1 = _T('\0');
			_tcscat_s(fileFullPath, _T(".ini"));
		}
		//根据路径创建文件对象
		//创建一个ini文件
		HANDLE fileHandle = CreateFile(fileFullPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE,   //独占属性
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_HIDDEN, NULL);   //同步  异步   

		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		//保存ini文件路径到类成员变量中
		m_fileFullPath = fileFullPath;

		//计算文件大小
		
		ULONG HighLength = 0;
		ULONG LowLength = GetFileSize(fileHandle, &HighLength);  //获得文件的大小
		//GetFileSize函数可以获取64位值，其中高32位存储在HighLength里，返回低32位并存储在LowLength里
		if (LowLength > 0 || HighLength > 0)   //不是空文件  
		{

			//不是第一次打开该文件
			CloseHandle(fileHandle);
			return FALSE;
		}
		//文件被创建出来了第一次

		//[Key]
		//SubKey = KeyValue
		BOOL writeSuccess = WritePrivateProfileString(_T("Settings"), _T("ListenPort"), _T("2356"), fileFullPath);
	
		if (writeSuccess == FALSE)
		{
			//获取写操作失败原因
			BOOL failReason = GetLastError();
		}
		writeSuccess = WritePrivateProfileString(_T("Settings"), _T("MaxConnections"), _T("10"), fileFullPath);
		if (writeSuccess == FALSE)
		{
			BOOL failReason = GetLastError();
		}
		CloseHandle(fileHandle);
		return TRUE;
}
