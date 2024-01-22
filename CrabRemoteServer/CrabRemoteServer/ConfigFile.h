﻿#pragma once
#include<string>
#ifdef UNICODE
typedef std::wstring _tstring;
#else
typedef std::string _tstring;
#endif


class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	//初始化配置文件
	BOOL ConfigFileInit();
	//获取配置文件
	BOOL GetCongfigFileData(_tstring& Key, _tstring& SubKey, int& BufferData);
	BOOL SetConfigFileData(_tstring& Key, _tstring& SubKey, int& BufferData);
	
public:

	_tstring m_fileFullPath;

};

