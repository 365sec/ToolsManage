#ifndef _FILEWATCH_H
#define _FILEWATCH_H

#include <vector>
#include <string>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#include <process.h>
#elif __linux__
#include <pthread.h>
#endif

namespace FW{

#define SUCESSED   1
#define FAILED		2
#define WATCH_TIMEOUT  5

	typedef struct _ProcessInfo{
		HANDLE hProcess;
		time_t start_time;
	}ProcessInfo;

	typedef int (*parser_file)(std::string file_path, std::string apppath, void *item);

	typedef struct WatchItem_{
		std::string name;
		std::string watchdir;
		std::string backdir;
		std::string extension;

		enum{
			TASK_MON = 0,
			RESULT_MON
		}mon_type;

		union {
			struct{
				char exepath[256];
				int max_scantime;  //最长扫描时间 （分钟）
				int max_process;  //最大进程数目
				std::vector<ProcessInfo> *watchprocess;
			}watchinfo;
			char htmldir[256];
		};
		parser_file parser;


	}WatchItem;

	class FileWatch
	{
	private:
		std::vector<WatchItem> _watch_list;
		std::string _backupdir;
		std::string _apppath;
		int _scan_interval;
	public:
		FileWatch();
		int AddWatchItem(WatchItem item);
		int StartWatch();
		int Run(int i);
		void Work(int i);
		void ListDirEx(WatchItem* item);
		int HandleFile(std::string filepath, std::string filename, WatchItem* item, std::string htmldir);
		int ReadFile(const char *filename, char **buf);
#ifdef _WIN32
		static unsigned int __stdcall thread_fun(PVOID arg);
#elif __linux__
		static void * thread_fun(void * arg);
#endif
		time_t TimeClock();
		void MySleep(int second);
		void SetBackupDir(std::string backupdir);
		std::string GetBackupDir();
		void SetApppath(std::string apppath){_apppath = apppath;};
		void SetScanInterval(int scan_interval){_scan_interval = scan_interval;};
	};

	typedef struct _ThreadArg{
		FileWatch *_watch;
		int i;
	}ThreadArg;
}
#endif
