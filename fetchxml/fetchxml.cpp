// watchxml.cpp : �������̨Ӧ�ó������ڵ㡣
//


//gcc -lstdc++ -lpthread FileWatch.cpp RdWrIni.cpp watchxml.cpp -o FileWatch

#include "stdafx.h"
#include "FileWatch.h"
#include "RdWrIni.h"
#include<json/json.h>
#include "util.h"
#include "zlog.h"
#include "XmlAnalyze.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <dlfcn.h>
#endif

#define __VERSION__    1.0
#define __COMPANY__  "昌吉州互联网信息办公室"
#define __PRODUCTNAME__   "等保检查任务调度系统"
#define     WINDOWS_TOOL        1    //windows

int start_task(std::string file_path, std::string exepath, void *item);
int parser(std::string file_path, std::string htmldir,  void *item);

FW::FileWatch filewatch;
DBconnectInfo db_config;

BOOL MyFreeLibrary (void* hLibModule)
{
#ifdef _WIN32
	return FreeLibrary ((HMODULE)hLibModule);
#else
	return dlclose(hLibModule);
#endif
}

#ifdef _WIN32
HINSTANCE hInstLibrary;
#elif __linux__
void* hInstLibrary;
#endif

DWORD ReadConfigDir()
{
	Json::Reader  reader;
	Json::Value   value;
	char *buffer = NULL;
	filewatch.ReadFile("conf.json", &buffer);
	if(buffer){
		if(reader.parse(buffer, value))
		{
			Json::Value arrayObj = value["task_xml_dir"];
			for (unsigned int i = 0; i < arrayObj.size(); i++)
			{
				FW::WatchItem item;
				item.name = arrayObj[i]["name"].asString();
				item.watchdir = arrayObj[i]["watch_dir"].asString();
				item.backdir = arrayObj[i]["backup_dir"].asString();
				strcpy(item.watchinfo.exepath, arrayObj[i]["exec_file"].asString().c_str());
				item.extension = arrayObj[i]["extention"].asString();
				item.mon_type = FW::WatchItem::TASK_MON;
				item.parser = start_task;
				item.watchinfo.max_scantime = arrayObj[i]["max_scantime"].asInt();
				item.watchinfo.max_process = arrayObj[i]["max_process"].asInt();
				filewatch.AddWatchItem(item);
			}
			arrayObj = value["result_xml_dir"];
			for (unsigned int i = 0; i < arrayObj.size(); i++)
			{
				FW::WatchItem item;
				item.watchdir = arrayObj[i]["watch_dir"].asString();
				item.backdir = arrayObj[i]["backup_dir"].asString();
				strcpy(item.htmldir, arrayObj[i]["html_dir"].asString().c_str());
				item.extension = arrayObj[i]["extention"].asString();
				item.mon_type = FW::WatchItem::RESULT_MON;
				item.parser = parser;
				filewatch.AddWatchItem(item);
			}
			arrayObj = value["AutoMonitTaskFile"];
			filewatch.SetScanInterval(arrayObj.asInt());
			arrayObj = value["db_host"];
			strcpy(db_config.host, arrayObj.asString().c_str());
			arrayObj = value["db_user"];
			strcpy(db_config.user, arrayObj.asString().c_str());
			arrayObj = value["db_password"];
			strcpy(db_config.password, arrayObj.asString().c_str());
			arrayObj = value["db_dbname"];
			strcpy(db_config.dbname, arrayObj.asString().c_str());
			arrayObj = value["db_port"];
			db_config.port = arrayObj.asInt();
		}
		free(buffer);
	}
	return 0;
}

int start_task(std::string file_path, std::string exepath, void *item)
{
	char cmd[1024] = {0};
	sprintf(cmd, "%s %s", exepath.c_str(), file_path.c_str());
	FW::WatchItem *witem = (FW::WatchItem *)item;
	HANDLE hProcess = dcreateprocess(cmd);
	if(hProcess)
	{
		zlog_info("启动任务%s 命令:%s", witem->name.c_str(), cmd);
		WaitForSingleObject(hProcess, INFINITE);
		zlog_info("任务%s扫描完成", witem->name.c_str());
		CloseHandle(hProcess);
	}
	else{
		zlog_error("dcreateprocess failed. cmd=%s \n", cmd);
	}
	return 0;
}

int parser(std::string file_path, std::string htmldir, void *item)
{
	char cFile[256];
	char pcFileName[256];
	char xmlFile[256];
	char htmlFile[256];
	char htmlpath[256];

	memset(xmlFile, 0, 256);
	memset(cFile, 0, 256);
	memset(pcFileName, 0, 256);
	memset(htmlFile, 0, 256);
	memset(htmlpath, 0, 256);
	strcpy_s(xmlFile, 256, file_path.c_str());
	strcpy_s(htmlFile, 256, htmldir.c_str());
#ifdef _WIN32
	int pos = file_path.find_last_of("\\");
#else
	int pos = file_path.find_last_of("/");
#endif
	std::string s =file_path.substr(pos+1);

	strcpy_s(cFile, 256, s.c_str());

	char *pTemp=NULL;
	pTemp = strrchr(cFile, '.');
	memset(pTemp, 0, strlen(pTemp));
	strcpy_s(pcFileName, 256, cFile);
	strcat_s(pcFileName, 256, ".html");
	//strcat_s(htmlFile, 256, "/");
	//strcat_s(htmlFile, 256, pcFileName);
	_fullpath(htmlpath,htmlFile,1024);
	zlog_info("转换XML xmlFile:%s  ---> htmlFile:%s", xmlFile, htmlpath);

	
	int ret = ImportCheckXml(xmlFile, htmlpath);
	if(ret){
		zlog_info("转换XML失败! 文件:%s 错误码:%d", xmlFile, ret);
	}

	return 0;
}

void usage(){

}

int main(int argc, char* argv[])
{
	char pathExe[256];
	memset(pathExe, 0, 256);
	GetExePath(pathExe);
	filewatch.SetApppath(pathExe);

	zlog_open_log(LOG_TYPE_CONSOLE, ZLOG_LEVEL_DEBUG, "fetchxml_log.log");

	zlog_info("当前工作目录: %s", pathExe);

	if(GetProcessidFromName("Deamon.exe")){
		Sleep(1000);
	}
	else{
		dcreateprocess("Deamon.exe");
		zlog_info("启动守护进程Deamon");
	}

	//初始化文件监控模块
	memset(&db_config, 0, sizeof(db_config));
	ReadConfigDir();

	//初始化数据库和日志配置信息
	printf("host : %s\n", db_config.host);
	printf("user : %s\n", db_config.user);
	printf("password : %s\n", db_config.password);
	printf("dbname : %s\n", db_config.dbname);
	printf("port : %d\n", db_config.port);

	ReadConfigDb(&db_config);
	initDb();

	filewatch.StartWatch();
	for(;;){
		filewatch.MySleep(10);
	}
	
	return 0;
}
