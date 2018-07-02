// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XMLANALYZE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XMLANALYZE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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

/*����ֵ-3 �����xml�ļ�·���Ҳ���\���ļ�������
-1 xml�ļ�load����
-2 xml�ļ�rootΪ��
-11 xml�����������ͳ���
-12�����ƻ��ļ��г���
-99 ���ڹ����Զ����룬�ƻ�������
-51��-52��-53xmlתhtml����

-31~-40U�ڹ��ߵ����ļ���������������-39Ϊ�ļ����к��ļ��еĹ������Ͳ���
0�ɹ�
*/
XMLANALYZE_API int ImportCheckXml(char* xmlfile, char* htmlpath);
XMLANALYZE_API int initDb();
XMLANALYZE_API int uninitDb();

/*-1 hostΪ��
-2 userΪ��
-3 passwordΪ��
-4 nameΪ��
0�ɹ�
*/
XMLANALYZE_API int ReadConfigDb(DBconnectInfo *dbinfo);