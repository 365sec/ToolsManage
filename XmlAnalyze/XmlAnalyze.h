// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 XMLANALYZE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// XMLANALYZE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef XMLANALYZE_EXPORTS
#define XMLANALYZE_API __declspec(dllexport)
#else
#define XMLANALYZE_API __declspec(dllimport)
#endif




typedef struct _DBConnectInfo{
	char host[100];
	char user[100];
	char password[100];
	char dbname[100];
	int port;
}DBconnectInfo;

/*返回值-3 传入的xml文件路径找不到\，文件名错误
-1 xml文件load出错
-2 xml文件root为空
-11 xml解析工具类型出错
-12创建计划文件夹出错
-99 网口工具自动导入，计划不存在
-51、-52、-53xml转html出错

-31~-40U口工具导入文件名解析出错，其中-39为文件名中和文件中的工具类型不符
0成功
*/
XMLANALYZE_API int ImportCheckXml(char* xmlfile, char* htmlpath);
XMLANALYZE_API int initDb();
XMLANALYZE_API int uninitDb();

/*-1 host为空
-2 user为空
-3 password为空
-4 name为空
0成功
*/
XMLANALYZE_API int ReadConfigDb(DBconnectInfo *dbinfo);