#pragma once
class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	BOOL ConfigFileInit();
	
public:
	//可能要改为_tstring
	CString m_fileFullPath;

};

