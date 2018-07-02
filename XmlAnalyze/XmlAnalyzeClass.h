#pragma once

#include "tinyxml/tinyxml.h"

#include <vector>
#include <string>
#include <map>
#include "db.h"
#include "tdb.h"
using namespace std;

#define     DBTOOL_WINDOWS_M        1    //windows
#define     DBTOOL_LINUX_M          2    //linux
#define     DBTOOL_BD_M             3    //����
#define     DBTOOL_MM_M             4    //ľ��
#define     DBTOOL_WLSB_M           5     //�����豸
#define     DBTOOL_WBESHEEL_M       6     //�������
#define     DBTOOL_RESERVE_M        7     //�������Ա��������
#define     DBTOOL_AQSB_M           8     //��ȫ�豸
#define     DBTOOL_WEAKPWD_M        9     //������
#define     DBTOOL_SQL_M            10    //SQLע��
#define     DBTOOL_XTLD_M           11    //ϵͳ©��
#define     DBTOOL_WZAQ_M           12    //��վ��ȫ
#define     DBTOOL_DB_M             13   //���ݿ�
#define     DBTOOL_OTHER_M          99   //����


#define XML_MAX_DSTIP_LEN		320

typedef struct T_XML_TOOLINFO{
	int toolType;
	char toolTypeStr[20];
	//vector<std::string> toolIp;
	char strDstIps[XML_MAX_DSTIP_LEN];	//������;��β�Է����ַ�����ȷ����
	char taskCode[200];
	char strPlanGuid[200];
	char strSysGuid[200];
	char strAssetGuid[200];
}XmlToolInfo;

typedef struct _OPTIONINFO{
	std::string option;
	std::string optionNumber;
}OptionInfo;

typedef struct TOOL_FLAWNUM_T
{
	char pItemCode1[100];   //����itemcode
	char pItemCode2[100];   //����itemcode
	char pItemCode3[100];  //�ļ�itemcode
	char pItemCode4[100];  //������վ��itemcode��ֻ����վ�У����ݿ��ϵͳ©��Ϊ��
	char pItemNumber[100]; //xml�ж�Ӧ��item_number
	char num[10];        //��Ŀ
}ToolFlawNum;

typedef struct T_CHECK_RESULT_DESC2
{
	char strLayer4Type[32];
	char strLayer4Check[32];
	char strLayer4CheckResult[32];
	int    iResult;
	int    iMatchValue;
	int    iMatchCondition;
}CHECK_RESULT_DESC2;


typedef struct _CONFIG{
	char db_host[256];
	char db_user[256];
	char db_password[256];
	char db_dbname[256];
	char db_port[256];
}Config;
extern Config g_config;
extern DB_CON con;

class CXmlAnalyzeClass
{
public:
	CXmlAnalyzeClass(void);
	~CXmlAnalyzeClass(void);

	int ReadConfig(char* db_host, char* db_user, char* db_password, char* db_dbname, int db_port);
	int GetUToolType(TiXmlElement* root, XmlToolInfo &xmlToolInfo, char* xmlfile);
	int GetToolType(TiXmlElement* root, XmlToolInfo &xmlToolInfo);
	void StrtokCode(XmlToolInfo& xmlToolInfo);
	int GetItemCode(TiXmlElement* root, int nToolType);
	int GetXmlData(TiXmlElement* root, int nToolType);
	int ImportCheckXml(char* xmlfile, char* htmlpath, int type);
	void selectChildNode(TiXmlNode* pNode);
	void GetDbFlawNum(TiXmlNode* pRootNode);
	void GetXtldFlawNum(TiXmlNode* pRootNode);
	void GetWebFlawNum(TiXmlNode* pRootNode);
	void FindItemNumber(ToolFlawNum *pToolflaw, const char* pItemNumber, const char* pSum);
	int GetItemCodeWeakPwd(TiXmlElement* root);
	int GetItemCodeMm(TiXmlElement* root);
	int GetItemCodeWebshell(TiXmlElement* root);
	int GetItemCodeBd(TiXmlElement* root);
	void selectChildNodeWeakPwd(TiXmlNode* pNode);

	void CheckPoint(int checkType);
	int SpeCheckResultJudge(char szCheckResult[32], CHECK_RESULT_DESC2 **pckRtDesc);
	int SysCheckResultJudge(char szCheckResult[32], char szSAG[16], CHECK_RESULT_DESC2 **pckRtDesc);
	char* GetSag(char* strSysGuid, char *sag);
	int SAGCompare(int s, int a, int g);
	int ItemcodeResultToDb(TiXmlElement* root, XmlToolInfo xmlToolInfo, char* sag);
	void LocateItemCode_ResultTable(int toolType, char *sag, bool isWeb);
	int initDb();
	int uninitDb();
	int PlanIsExist(XmlToolInfo xmlToolInfo);

public:
	vector<OptionInfo> m_OptionInfo;

	ToolFlawNum m_XtldFlawNum[200];
	ToolFlawNum m_DbFlawNum[200];
	ToolFlawNum m_WebFlawNum[200];
};
