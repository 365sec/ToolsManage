#include "stdafx.h"
#include "config.h"
#include "XmlAnalyzeClass.h"

extern Config g_config;


#ifdef _WIN32
FILE *z_cate;
#elif __linux__
zlog_category_t *z_cate;
#endif

char *db_host = g_config.db_host;
char *db_user = g_config.db_user;
char *db_password = g_config.db_password;
char *db_dbname = g_config.db_dbname;
char *db_schema = NULL;
char *db_socket = NULL;
int db_port = atoi(g_config.db_port);

