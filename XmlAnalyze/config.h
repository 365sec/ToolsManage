#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <fcntl.h>
//#include <pthread.h>
#include "apr_pools.h"
#include "apr_strings.h"
#include "apr_thread_mutex.h"
#ifdef _WIN32
#include  "zlog.h"
#elif __linux__
#include <zlog.h>
#endif
//#include "atomic.h"

#ifdef _WIN32
extern FILE * z_cate;
#elif __linux__
extern zlog_category_t *z_cate;
#endif


extern char *db_host;
extern char *db_user;
extern char *db_password;
extern char *db_dbname;
extern char *db_schema;
extern char *db_socket;
extern int db_port;

#endif
