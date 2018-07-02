// XmlAnalyze.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "XmlAnalyze.h"
#include "XmlAnalyzeClass.h"


XMLANALYZE_API int ImportCheckXml(char* xmlfile, char* htmlpath)
{
	CXmlAnalyzeClass xac;
	int type;
	

	char* ptr = strrchr(xmlfile, '\\');
	if(ptr == NULL)
	{
		return -3;
	}
	char fileName[MAX_PATH] = {0};
	strcpy_s(fileName, MAX_PATH, ptr+1);

	int flag = strncmp(fileName, "U@", 2);
	if(flag == 0)
	{
		type = 2;
	}
	else
	{
		type=1;
	}

	int res = xac.ImportCheckXml(xmlfile, htmlpath, type);
	return res;
}

XMLANALYZE_API int ReadConfigDb(DBconnectInfo *dbinfo)
{
	CXmlAnalyzeClass xac;
	xac.ReadConfig(dbinfo->host, dbinfo->user, dbinfo->password, dbinfo->dbname, dbinfo->port);
	return 0;
}

XMLANALYZE_API int initDb()
{
	CXmlAnalyzeClass xac;
	xac.initDb();
	return 0;
}

XMLANALYZE_API int uninitDb()
{
	CXmlAnalyzeClass xac;
	xac.uninitDb();
	return 0;
}