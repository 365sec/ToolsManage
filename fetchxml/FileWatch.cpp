#include "stdafx.h"
#include "FileWatch.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "zlog.h"

#ifdef _WIN32
#elif __linux__
#include <sys/types.h>
#include <dirent.h>
#endif

namespace FW{

	FileWatch::FileWatch()
	{
		_scan_interval = WATCH_TIMEOUT;
	}

	int FileWatch::AddWatchItem(WatchItem item)
	{
		_watch_list.push_back(item);

		return SUCESSED;
	}

	int FileWatch::StartWatch()
	{
		for(int i = 0; i< _watch_list.size(); i++){
			ThreadArg *pArg = new ThreadArg;
			pArg->i = i;
			pArg->_watch = this;
	#ifdef _WIN32
			HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, FileWatch::thread_fun, pArg, 0, NULL);
			if(!handle) return FAILED;
	#elif __linux__
			int ret = 0;
			pthread_t ntid;
			ret = pthread_create(&ntid, NULL, FileWatch::thread_fun, pArg);
			printf("创建线程thread_fun");
			if(ret) return FAILED;
	#endif
		}

		return SUCESSED;
	}

#ifdef _WIN32
	unsigned int __stdcall FileWatch::thread_fun(PVOID arg)
#elif __linux__
	void * FileWatch::thread_fun(void * arg)
#endif
	{
		ThreadArg *pArg = (ThreadArg *)arg;
		WatchItem* item = (WatchItem*)&(pArg->_watch->_watch_list[pArg->i]);
		zlog_info("线程%d启动,监控目录:%s", pArg->i, item->watchdir.c_str());
		(pArg->_watch)->Run(pArg->i);
		return 0;
	}

	int FileWatch::Run(int i)
	{
		long int sec1, sec2, nextcheck;
		int sleeptime;

		sec1 = TimeClock();
		sec2 = sec1;

		sleeptime = _scan_interval - sec1 % _scan_interval;
		for (;;)
		{
			MySleep(sleeptime);

			sec1 = TimeClock();
			Work(i);
			sec2 = TimeClock();

			nextcheck = sec1 - sec1 % _scan_interval + _scan_interval;
			if (0 > (sleeptime = nextcheck - sec2))
				sleeptime = 0;
		}
		return 0;

	}

	void FileWatch::Work(int i)
	{
		ListDirEx(&(_watch_list[i]));
		// 处理退出的进程
	}

	void FileWatch::ListDirEx(WatchItem* item)
	{
		std::string dir = item->watchdir;
		std::string extension = item->extension;
		parser_file parser = item->parser;
		std::string backdir = item->backdir;
		//std::string htmldir = item->htmldir;
		
#ifdef _WIN32
		WIN32_FIND_DATAA  findData = { 0 };
		std::string strFindPath = dir + "\\*.*";
		HANDLE hFindFine = FindFirstFileA(strFindPath.c_str(), &findData);
		if (INVALID_HANDLE_VALUE == hFindFine){
			//printf("Error:%d", GetLastError());
			return ;
		}
		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (findData.cFileName[0] == '.')
					continue;
				//std::string strNeedFindPaht = dir + "\\" + findData.cFileName;
				//ListDirEx(strNeedFindPaht, extension, parser, backdir, htmldir);
			}
			else
			{
				char currentdir[1024]={0},fullpath[1024]={0};
				sprintf(currentdir, "%s", (dir + "\\" + findData.cFileName).c_str());
				_fullpath(fullpath,currentdir,1024);
				if(item->mon_type == WatchItem::TASK_MON){
					HandleFile(fullpath, findData.cFileName, item, item->watchinfo.exepath);
				}else{
					HandleFile(fullpath, findData.cFileName, item, item->htmldir);
				}
			}
		} while (FindNextFileA(hFindFine, &findData));
		FindClose(hFindFine);
#elif __linux__
		DIR               *pDir;
		struct dirent  *ent;
		std::string      childpath;

		pDir=opendir(dir.c_str());

		while((ent=readdir(pDir))!=NULL)
		{
			if(ent->d_type & DT_DIR)
			{
				if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
					continue;

				//childpath = dir + "/" + ent->d_name;
				//ListDirEx(childpath, extension, parser, backdir, htmldir);

			}
			else
			{
				childpath = dir + "/" + ent->d_name;
				HandleFile(childpath, ent->d_name, item, htmldir);
			}
		}
#endif
	}

	int FileWatch::HandleFile(std::string filepath, std::string filename, WatchItem* item, std::string _dir)
	{
		std::string extension = item->extension;
		parser_file parser = item->parser;
		std::string backdir = item->backdir;

		std::string::size_type position;
		position = filepath.rfind(".");
		if (position != std::string::npos) {
			std::string ext = filepath.substr(position+1);
			if(ext == extension){
					if(parser) parser(filepath, _dir, item);
#ifdef _WIN32
					if(GetFileAttributes(filepath.c_str()) != -1){ //存在 工具没有删除
						MoveFileEx(filepath.c_str(), (backdir+ "\\" + filename).c_str(), MOVEFILE_REPLACE_EXISTING);
						zlog_info("备份未删除XML文件 %s ---> %s.", filepath.c_str(), (backdir+ "\\" + filename).c_str());
					}
#elif __linux__
					rename(filepath.c_str(), (backdir+ "/" + filename).c_str());
					zlog_info("备份未删除XML文件 %s ---> %s.", filepath.c_str(), (backdir+ "/" + filename).c_str());
#endif
			}
		}
		return 0;
	}

	time_t FileWatch::TimeClock()
	{
		return time(NULL);
	}

	void FileWatch::MySleep(int second)
	{
		MySleepSecond(second);
	}

	void FileWatch::SetBackupDir(std::string backupdir)
	{
		_backupdir = backupdir;
	}

	std::string FileWatch::GetBackupDir()
	{
		return _backupdir;
	}

	int FileWatch::ReadFile(const char *filename, char **buf)
	{
		FILE *fp = 0;
		int file_len = 0;
		int ret = 1;
		char *buffer = NULL;

		if((fp=fopen(filename, "r"))== NULL){
			return 0;
		}
		fseek(fp, 0L, SEEK_END);
		file_len = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		buffer = *buf = (char*)malloc(file_len);
		memset(buffer, 0, file_len);
		while(ret){
			ret = fread(buffer, 1024, 1, fp);
			if(ret == -1){
				fclose(fp);
				return 0;
			}
			buffer +=ret*1024;
		}
		fclose(fp);
		return file_len;
	}
}

