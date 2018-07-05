#include "StdAfx.h"
#include "XmlAnalyzeClass.h"
#include "CommDef.h"
#include "CheckResult.h"

#include <direct.h>
#include <io.h>

Config g_config;
DB_CON con;

CXmlAnalyzeClass::CXmlAnalyzeClass(void)
{
}

CXmlAnalyzeClass::~CXmlAnalyzeClass(void)
{
}

int CXmlAnalyzeClass::ReadConfig(char* db_host, char* db_user, char* db_password, char* db_dbname, int db_port)
{
	
	if(db_host != NULL)
		strcpy_s(g_config.db_host, 256, db_host);
	else
		return -1;
	if(db_user != NULL)
		strcpy_s(g_config.db_user, 256, db_user);
	else 
		return -2;

	if(db_password != NULL)
		strcpy_s(g_config.db_password, 256, db_password);
	else 
		return -3;

	if(db_dbname != NULL)
		strcpy_s(g_config.db_dbname, 256, db_dbname);
	else 
		return -4;

	itoa(db_port, g_config.db_port, 10);

	
	//	char path[256];
	//memset(path, 0, sizeof(char*));
	//char pathExe[256];
	//memset(pathExe, 0, sizeof(char*));
	//DWORD dwLen = GetModuleFileName(NULL, pathExe, 256);


	//char * pPatch = &pathExe[dwLen]; //szexepatch + dwLen;  
	//while (pPatch != NULL && *pPatch != '\\')  //计算程序的路径(倒数查找最后一个"\")
	//{
	//	pPatch--;
	//}

	////得到最后一个反斜扛之后再往后加1,并把它设置为0,这样后面的数据对于字符数组就会忽略
	//pPatch++;
	//*pPatch=0;

	//strcpy_s(path, 256, pathExe);
	//strcat_s(path, 256, "config.ini");
	////char str[100] = NULL; 
	//LPTSTR str = new char[100];
	//DWORD res = GetPrivateProfileString("Setting", "db_host", NULL, str , 256, path) ;
	//if(res>0){
	//	strcpy_s(g_config.db_host, 256, str);
	//}
	//res = GetPrivateProfileString("Setting", "db_user", NULL, str , 256, path) ;
	//if(res>0){
	//	strcpy_s(g_config.db_user, 256, str);
	//}
	//res = GetPrivateProfileString("Setting", "db_password", NULL, str , 256, path) ;
	//if(res>0){
	//	strcpy_s(g_config.db_password, 256, str);
	//}
	//res = GetPrivateProfileString("Setting", "db_dbname", NULL, str , 256, path) ;
	//if(res>0){
	//	strcpy_s(g_config.db_dbname, 256, str);
	//}
	//res = GetPrivateProfileString("Setting", "db_port", NULL, str , 256, path) ;
	//if(res>0){
	//	strcpy_s(g_config.db_port, 256, str);
	//	db_port = atoi(g_config.db_port);
	//}
	//delete str;
	return 0;
}

//解析xml获取工具类型 0-成功 -1~-2失败 -3-未获取到工具类型
int CXmlAnalyzeClass::GetToolType(TiXmlElement* root, XmlToolInfo &xmlToolInfo)
{
	memset(&xmlToolInfo, 0, sizeof(xmlToolInfo));

	int res=0;

	do{
		TiXmlNode *pShemeNode = root->FirstChild("scheme");
		if (pShemeNode == NULL)
		{
			res =  -1;
			break;
		}
		TiXmlNode *pToolInfoNode = pShemeNode->FirstChild("tool_info");
		if (pToolInfoNode == NULL)
		{
			res =  -2;
			break;
		}

		const char *toolcategory = pToolInfoNode->ToElement()->Attribute("toolcategory");
		if(toolcategory == NULL)
		{
			res = -3;
			break;
		}
		else
		{
			xmlToolInfo.toolType = atoi(toolcategory);
			strcpy_s(xmlToolInfo.toolTypeStr,20, toolcategory);
		}

		int nIpLen = 0;
		TiXmlNode *pOperationNode = pShemeNode->FirstChild("operation_info");
		if (pOperationNode != NULL)
		{
			const char *dest_ip = pOperationNode->ToElement()->Attribute("dest_ip");
			//        std::string ip;
			if (dest_ip)	//必须以;结尾以方便字符串精确查找
			{
				strcpy_s(xmlToolInfo.strDstIps, XML_MAX_DSTIP_LEN-1, dest_ip);
				nIpLen = strlen(xmlToolInfo.strDstIps);
				if (xmlToolInfo.strDstIps[nIpLen -1 ] != ';')
				{
					xmlToolInfo.strDstIps[nIpLen ] = ';';
					xmlToolInfo.strDstIps[nIpLen + 1] = 0;
				}
			}
		}
		TiXmlNode *pTaskInfoNode = pShemeNode->FirstChild("task_info");
		if(pTaskInfoNode != NULL)
		{
			const char *taskcode = pTaskInfoNode->ToElement()->Attribute("taskcode");
			if(taskcode != NULL)
			{
				strcpy(xmlToolInfo.taskCode, taskcode);
				StrtokCode(xmlToolInfo);
			}
			else
				strcpy(xmlToolInfo.taskCode, "");
		}
	}while(0);

	return res;
}

int CXmlAnalyzeClass::GetUToolType(TiXmlElement* root, XmlToolInfo &xmlToolInfo, char* xmlfile)
{
	memset(&xmlToolInfo, 0, sizeof(xmlToolInfo));

	int res=0;
	do{
		TiXmlNode *pShemeNode = root->FirstChild("scheme");
		if (pShemeNode == NULL)
		{
			res =  -31;
			break;
		}
		TiXmlNode *pToolInfoNode = pShemeNode->FirstChild("tool_info");
		if (pToolInfoNode == NULL)
		{
			res =  -32;
			break;
		}

		const char *toolcategory = pToolInfoNode->ToElement()->Attribute("toolcategory");
		if(toolcategory == NULL)
		{
			res = -33;
			break;
		}

		//将U口xml文件名以@分割
		char delims[] = "@";
		char *result = NULL; 
		result = strtok( xmlfile, delims );
		if( result == NULL ) {
			res = -34;
			break;
			//strcpy_s(xmlToolInfo.strPlanGuid, 200, result);
			
		}  
		result = strtok( NULL, delims );
		if( result != NULL ) {
			strcpy_s(xmlToolInfo.strPlanGuid, 200, result);
		}
		else{
			res = -35;
			break;
		}

		result = strtok( NULL, delims );
		if( result != NULL ) {
			strcpy_s(xmlToolInfo.strSysGuid, 200, result);
		}
		else{
			res = -36;
			break;
		}


		result = strtok( NULL, delims );
		if( result != NULL ) {
			strcpy_s(xmlToolInfo.strAssetGuid, 200, result);
		}
		else{
			res = -37;
			break;
		}

		result = strtok( NULL, delims );
		if( result != NULL ) {
			char* ptr = strrchr(result, '.');
			if(ptr == NULL)
			{
				res = -38;
				break;
			}
			*ptr = '\0';

			if(strcmp(result, toolcategory) == 0){
				strcpy_s(xmlToolInfo.toolTypeStr, 200, result);
				xmlToolInfo.toolType = atoi(result);
			}
			else{
				res = -39;
				break;
			}
		}
		else{
			res = -40;
			break;
		}

	}while(0);

	return res;
}

void CXmlAnalyzeClass::StrtokCode(XmlToolInfo& xmlToolInfo)
{
	char delims[] = "@";
	char *result = NULL; 
	result = strtok( xmlToolInfo.taskCode, delims );
	if( result != NULL ) {
		strcpy_s(xmlToolInfo.strPlanGuid, 200, result);
		//xmlToolInfo.strPlanGuid =  result;
	}           
	result = strtok( NULL, delims );
	if( result != NULL ) {
		strcpy_s(xmlToolInfo.strSysGuid, 200, result);
		//xmlToolInfo.strSysGuid = result;
	}
	result = strtok( NULL, delims );
	if( result != NULL ) {
		strcpy_s(xmlToolInfo.strAssetGuid, 200, result);
		//xmlToolInfo.strAssetGuid = result;
	}
}

//utf_8转gb2312

char* CXmlAnalyzeClass::ConvertUtf8ToGBK(const char* strUtf8, char* str) 
{
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL,0);
	unsigned short * wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL);
	strUtf8 = szGBK;

	if (strlen(strUtf8)>=COLLENGTH)
	{
		memcpy(str,strUtf8,COLLENGTH-1);
		str[COLLENGTH-1] = 0;
	}
	else
		strcpy_s(str, COLLENGTH,strUtf8);
	delete[] wszGBK;
	delete[] szGBK;
	return str;
}

void CXmlAnalyzeClass::selectChildNode(TiXmlNode* pNode, int flg)  
{
	if(pNode == NULL)  
	{  
		return;  
	}
	OptionInfo optionInfo;
	std::string optionNum = "";
	std::string option = "";
	const char* str = NULL;
	const char* pParentStr = NULL;
	TiXmlNode* pChildNode = NULL;
	TiXmlNode* pParentNode = NULL;
	int t = pNode->Type();
	if(t == TiXmlText::TINYXML_ELEMENT)
	{
		if ((!strcmp("option_number", pNode->Value())) || (!strcmp("item_result_number", pNode->Value())))
		{
			str = pNode->ToElement()->GetText();
			if (str != NULL && strcmp(str, "无数据"))
			{
				optionNum = std::string(str);
				optionInfo.optionNumber = optionNum;
				//获取父节点
				pParentNode = pNode->Parent();
				for (pChildNode = pParentNode->FirstChild();  pChildNode && (pChildNode->Type() != TiXmlText::TINYXML_TEXT) ; pChildNode = pChildNode->NextSibling())
				{
					if ((!strcmp(pChildNode->Value(), "option"))  || (!strcmp(pChildNode->Value(), "item_result")))
					{
						pParentStr = pChildNode->ToElement()->GetText();
						if(pParentStr != NULL)
						{
							if(flg == 0){
								option = std::string(pParentStr);
								optionInfo.option = option;
							}
							else{
								char str[COLLENGTH] = {0};
								option = std::string(ConvertUtf8ToGBK(pParentStr, str));
								optionInfo.option = option;
							}
							
						}
					}
				}
				m_OptionInfo.push_back(optionInfo);
			}
		} 
		else
		{
			for (pChildNode = pNode->FirstChild(); pChildNode && (pChildNode->Type() != TiXmlText::TINYXML_TEXT) ; pChildNode = pChildNode->NextSibling())
			{
				selectChildNode(pChildNode, flg);
			}
		}
	}
}

void CXmlAnalyzeClass::FindItemNumber(ToolFlawNum *pToolflaw, const char* pItemNumber, const char* pSum)
{
	if(pItemNumber == NULL)
	{
		return;
	}
	int i = 0;
	while(pToolflaw[i].pItemCode1[0])
	{
		if(!strcmp(pToolflaw[i].pItemNumber, pItemNumber))
		{
			strcpy(pToolflaw[i].num, pSum);
			return;
		}
		i++;
	}
}


void  CXmlAnalyzeClass::GetWebFlawNum(TiXmlNode* pRootNode)
{
	TiXmlNode *pPNode = pRootNode->FirstChild("ScanGroup");
	if(pPNode == NULL)
	{
		return;
	}
	TiXmlNode *pNode = pPNode->FirstChild("Statistics");
	if(pNode == NULL)
	{
		return;
	}

	TiXmlNode *pChildNode = NULL;
	TiXmlNode *pChildrNode = NULL;
	const char* ss = NULL;
	const char* ss1 = NULL;

	for (pChildNode = pNode->FirstChildElement(); pChildNode; pChildNode = pChildNode->NextSiblingElement())
	{
		if(pChildNode == NULL)
		{
			continue;
		}
		pChildrNode = pChildNode->FirstChild("item_name_number");

		if(pChildrNode != NULL)
		{
			ss = pChildrNode->ToElement()->GetText();
			pChildrNode = pChildNode->FirstChild("sum");
			ss1 = pChildrNode->ToElement()->GetText();
			FindItemNumber(m_WebFlawNum, ss, ss1);
		}
		
	}
}

void  CXmlAnalyzeClass::GetXtldFlawNum(TiXmlNode* pRootNode)
{
	TiXmlNode *pPNode = pRootNode->FirstChild("report");
	if(pPNode == NULL)
	{
		return;
	}

	TiXmlNode *pNode = pPNode->FirstChild("Statistics");
	if(pNode == NULL)
	{
		return;
	}

	TiXmlNode *pChildNode = NULL;
	TiXmlNode *pChildrNode = NULL;
	const char* ss = NULL;
	const char* ss1 = NULL;

	for (pChildNode = pNode->FirstChildElement(); pChildNode; pChildNode = pChildNode->NextSiblingElement())
	{
		if(pChildNode == NULL)
		{
			continue;
		}
		pChildrNode = pChildNode->FirstChild("item_name_number");
		if(pChildrNode !=NULL)
		{
			ss = pChildrNode->ToElement()->GetText();
			pChildrNode = pChildNode->FirstChild("sum");
			ss1 = pChildrNode->ToElement()->GetText();
			FindItemNumber(m_XtldFlawNum, ss, ss1);
		}
	}
}

void  CXmlAnalyzeClass::GetDbFlawNum(TiXmlNode* pRootNode)
{
	TiXmlNode *pPNode = pRootNode->FirstChild("report");
	if (pPNode == NULL)
	{
		return ;
	}
	TiXmlNode *pNode = pPNode->FirstChild("vulnerabity_amount");
	if(pNode == NULL)
	{
		return;
	}
	TiXmlNode *pChildNode = NULL;
	const char* ss = NULL;

	pChildNode = pNode->FirstChild("bufferoverflow_vulnerabity_amount");
	if(pChildNode != NULL)
	{
		ss = pChildNode->ToElement()->GetText();
		FindItemNumber(m_DbFlawNum, "013131000555102101", ss);
	}

	pChildNode = pNode->FirstChild("accesscontrol_vulnerabity_amount");
	if(pChildNode != NULL)
	{
		ss = pChildNode->ToElement()->GetText();
		FindItemNumber(m_DbFlawNum, "013131000555102102", ss);
	}

	pChildNode = pNode->FirstChild("PLSQL_vulnerabity_amount");
	if(pChildNode != NULL)
	{
		ss = pChildNode->ToElement()->GetText();
		FindItemNumber(m_DbFlawNum, "013131000555102103", ss);
	}
	
	pChildNode = pNode->FirstChild("bigauthority_vulnerabity_amount");
	if(pChildNode != NULL)
	{
		ss = pChildNode->ToElement()->GetText();
		FindItemNumber(m_DbFlawNum, "013131000555102104", ss);
	}

	pChildNode = pNode->FirstChild("rightflaw_vulnerabity_amount");
	if(pChildNode != NULL)
	{
		ss = pChildNode->ToElement()->GetText();
		FindItemNumber(m_DbFlawNum, "013131000555102105", ss);
	}
	
	pChildNode = pNode->FirstChild("accessbypass_vulnerabity_amount");
	if(pChildNode != NULL)
	{
		ss = pChildNode->ToElement()->GetText();
		FindItemNumber(m_DbFlawNum, "013131000555102106", ss);
	}
	

	pChildNode = pNode->FirstChild("other_vulnerabity_amount");
	if(pChildNode != NULL)
	{
		ss = pChildNode->ToElement()->GetText();
		FindItemNumber(m_DbFlawNum, "013131000555102107", ss);
	}
	

}


int CXmlAnalyzeClass::GetItemCode(TiXmlElement* root, int toolType, int flg)
{
	TiXmlNode *pNode = root;
	TiXmlNode *pSelectNode = NULL;
	m_OptionInfo.clear();
	selectChildNode(pNode, flg);

	if(toolType == DBTOOL_WZAQ_M)
	{
		GetWebFlawNum(pNode);
	}
	else if(toolType == DBTOOL_XTLD_M)
	{
		GetXtldFlawNum(pNode);
	}
	else if(toolType == DBTOOL_DB_M)
	{
		GetDbFlawNum(pNode);
	}
	else
		;
	return 0;
}

void CXmlAnalyzeClass::selectChildNodeWeakPwd(TiXmlNode* pNode)
{
	if(pNode == NULL)
	{
		return;
	}
	OptionInfo optionInfo;
	std::string optionNum = "";

	TiXmlNode *pNode1 = pNode->FirstChild("weakpwds");

	if(pNode1 == NULL)
	{
		return;
	}

	TiXmlNode *pChildNode = NULL;
	TiXmlNode *pChildrNode = NULL;
	const char* str;
	for (pChildNode = pNode1->FirstChildElement(); pChildNode; pChildNode = pChildNode->NextSiblingElement())
	{
		if(pChildNode == NULL)
		{
			continue;
		}
		
		for(pChildrNode = pChildNode->FirstChildElement(); pChildrNode; pChildrNode = pChildrNode->NextSiblingElement())
		{
			if(pChildrNode == NULL)
			{
				continue;
			}
			str = pChildrNode->ToElement()->Attribute("result_number");
			if(str != NULL && strcmp(str, "无数据"))
			{
				optionNum = std::string(str);
				optionInfo.optionNumber = optionNum;
				optionInfo.option = "";
				m_OptionInfo.push_back(optionInfo);
			}
		}
	}

}

int CXmlAnalyzeClass::GetItemCodeWeakPwd(TiXmlElement* root)
{

	TiXmlNode *pNode = root;
	//TiXmlNode *pSelectNode = NULL;
	m_OptionInfo.clear();
	selectChildNodeWeakPwd(pNode);
	return 0;
}

int CXmlAnalyzeClass::GetItemCodeBd(TiXmlElement* root, int flg)
{

	TiXmlNode *pNode = root;
	//TiXmlNode *pSelectNode = NULL;
	m_OptionInfo.clear();
	selectChildNode(pNode, flg);

	vector<OptionInfo>::iterator ite;
	bool bFlg1 = false;
	bool bFlg2 = false;
	for(ite=m_OptionInfo.begin();ite!=m_OptionInfo.end();)
	{
		if((ite->optionNumber == "003000000101101101e101") && (bFlg1 == false))
		{
			bFlg1 = true;
			++ite;
		}
		else if((ite->optionNumber == "003000000101101101e100") && (bFlg2 == false))
		{
			bFlg2 = true;
			++ite;
		}
		else
		{
			ite=m_OptionInfo.erase(ite);
		}
	}
	int nn =  m_OptionInfo.size();
	if(nn == 0)
	{
		OptionInfo pi;
		pi.optionNumber = "003000000101101101e100";
		pi.option = "";
		m_OptionInfo.push_back(pi);
	}
	return 0;
}

int CXmlAnalyzeClass::GetItemCodeMm(TiXmlElement* root, int flg)
{

	TiXmlNode *pNode = root;
	//TiXmlNode *pSelectNode = NULL;
	m_OptionInfo.clear();
	selectChildNode(pNode, flg);

	vector<OptionInfo>::iterator ite;
	bool bFlg1 = false;
	bool bFlg2 = false;
	for(ite=m_OptionInfo.begin();ite!=m_OptionInfo.end();)
	{
		if((ite->optionNumber == "004000000101101101e101") && (bFlg1 == false))
		{
			bFlg1 = true;
			++ite;
		}
		else if((ite->optionNumber == "004000000101101101e100") && (bFlg2 == false))
		{
			bFlg2 = true;
			++ite;
		}
		else
		{
			ite=m_OptionInfo.erase(ite);
		}
	}
	int nn =  m_OptionInfo.size();
	if(nn == 0)
	{
		OptionInfo pi;
		pi.optionNumber = "004000000101101101e100";
		pi.option = "";
		m_OptionInfo.push_back(pi);
	}
	return 0;
}

int CXmlAnalyzeClass::GetItemCodeWebshell(TiXmlElement* root, int flg)
{

	TiXmlNode *pNode = root;
	//TiXmlNode *pSelectNode = NULL;
	m_OptionInfo.clear();
	selectChildNode(pNode, flg);

	vector<OptionInfo>::iterator ite;
	bool bFlg1 = false;
	bool bFlg2 = false;
	bool bFlg3 = false;
	bool bFlg4 = false;
	bool bFlg5 = false;
	bool bFlg6 = false;
	bool bFlg7 = false;
	bool bFlg8 = false;
	bool bFlg9 = false;
	bool bFlg10 = false;
	int iFlg1 = 0, iFlg2 = 0, iFlg3 = 0, iFlg4 = 0, iFlg5 = 0, iFlg6 = 0, iFlg7 = 0, iFlg8 = 0, iFlg9 = 0, iFlg10 = 0;
	for(ite=m_OptionInfo.begin();ite!=m_OptionInfo.end();)
	{
		if((ite->optionNumber == "006000000101101101e100") && (bFlg1 == false))
		{
			bFlg1 = true;
			iFlg1++;
			++ite;
		}
		else if((ite->optionNumber == "006000000102101101e100") && (bFlg2 == false))
		{
			bFlg2 = true;
			iFlg2++;
			++ite;
		}
		else if((ite->optionNumber == "006000000103101101e100") && (bFlg3 == false))
		{
			bFlg3 = true;
			iFlg3++;
			++ite;
		}
		else if((ite->optionNumber == "006000000104101101e100") && (bFlg4 == false))
		{
			bFlg4 = true;
			iFlg4++;
			++ite;
		}
		else if((ite->optionNumber == "006000000105101101e100") && (bFlg5 == false))
		{
			bFlg5 = true;
			iFlg5++;
			++ite;
		}
		else if((ite->optionNumber == "006000000101101101e101") && (bFlg6 == false))
		{
			bFlg6 = true;
			iFlg6++;
			++ite;
		}
		else if((ite->optionNumber == "006000000102101101e101") && (bFlg7 == false))
		{
			bFlg7 = true;
			iFlg7++;
			++ite;
		}
		else if((ite->optionNumber == "006000000103101101e101") && (bFlg8 == false))
		{
			bFlg8 = true;
			iFlg8++;
			++ite;
		}
		else if((ite->optionNumber == "006000000104101101e101") && (bFlg9 == false))
		{
			bFlg9 = true;
			iFlg9++;
			++ite;
		}
		else if((ite->optionNumber == "006000000105101101e101") && (bFlg10 == false))
		{
			bFlg10 = true;
			iFlg10++;
			++ite;
		}
		else
		{
			ite=m_OptionInfo.erase(ite);
		}
	}
	if(bFlg1 == 0 && bFlg6 == 0)
	{
		OptionInfo pi;
		pi.optionNumber = "006000000101101101e100";
		pi.option = "";
		m_OptionInfo.push_back(pi);
	}
	if(bFlg2 == 0 && bFlg7 == 0)
	{
		OptionInfo pi;
		pi.optionNumber = "006000000102101101e100";
		pi.option = "";
		m_OptionInfo.push_back(pi);
	}
	if(bFlg3 == 0 && bFlg8 == 0)
	{
		OptionInfo pi;
		pi.optionNumber = "006000000103101101e100";
		pi.option = "";
		m_OptionInfo.push_back(pi);
	}
	if(bFlg4 == 0 && bFlg9 == 0)
	{
		OptionInfo pi;
		pi.optionNumber = "006000000104101101e100";
		pi.option = "";
		m_OptionInfo.push_back(pi);
	}
	if(bFlg5 == 0 && bFlg10 == 0)
	{
		OptionInfo pi;
		pi.optionNumber = "006000000105101101e100";
		pi.option = "";
		m_OptionInfo.push_back(pi);
	}
	return 0;
}


int CXmlAnalyzeClass::GetXmlData(TiXmlElement* root, int nToolType, int flg)
{
	if(nToolType == DBTOOL_WEAKPWD_M)
	{
		GetItemCodeWeakPwd(root);
	}
	else if(nToolType == DBTOOL_BD_M)
	{
		GetItemCodeBd(root, flg);
	}
	else if(nToolType == DBTOOL_MM_M)
	{
		GetItemCodeMm(root, flg);
	}
	else if(nToolType == DBTOOL_WBESHEEL_M)
	{
		GetItemCodeWebshell(root, flg);
	}
	else
	{
		GetItemCode(root, nToolType, flg);
	}
	return 0;
}


//数据库操作
//初始化
int CXmlAnalyzeClass::initDb()
{
	apr_pool_initialize();
	z_cate = stdout;

	con = CreateDBConnect();
	DBconnect(con, T_DB_CONNECT_NORMAL);

	DBexecute(con, "set names gbk;");
	return 0;
}

int CXmlAnalyzeClass::uninitDb()
{
	DBclose(con);
	return 0;
}

static std::multimap<std::string, CHECK_RESULT_DESC2> gs_hmCkRtDesc2;

int   CXmlAnalyzeClass::SysCheckResultJudge(char szCheckResult[32], char szSAG[16], CHECK_RESULT_DESC2 **pckRtDesc)
{
	int iResultLen;
	if ((NULL == szCheckResult) || (NULL == szSAG))
		return CHECK_RESULT_UNKNOWN;
	iResultLen = strlen(szCheckResult);
	if (((ITEM_RESULT_LEN_MIN != iResultLen)&&(ITEM_RESULT_LEN_MAX != iResultLen)) || (RESULT_SAG_LEN != strlen(szSAG)))
		return CHECK_RESULT_UNKNOWN;

	// 005XXXXXX101101101b200
	char szKey[64];
	char szCurItem[64];
	strncpy(szKey, szCheckResult, 3);
	szKey[3] = 0;
	strncpy(szKey+3, szCheckResult+9, 9);
	szKey[12] = 0;

	std::multimap<std::string, CHECK_RESULT_DESC2> :: const_iterator hm_iter;

	char szTmp[256] = "";
	hm_iter = gs_hmCkRtDesc2.find(szKey);

	DWORD dwCheckResult = CHECK_RESULT_UNKNOWN;
	*pckRtDesc = NULL;

	do 
	{
		if (hm_iter == gs_hmCkRtDesc2.end())
		{
			break;
		}

		const CHECK_RESULT_DESC2 &ckRtDesc = hm_iter->second;
		*pckRtDesc = (CHECK_RESULT_DESC2 *)(&(hm_iter->second));
		iResultLen = strlen(ckRtDesc.strLayer4CheckResult);

		if ((ITEM_RESULT_LEN_MIN != iResultLen) && (ITEM_RESULT_LEN_MAX != iResultLen))
		{
			hm_iter++;
			continue;
		}

		const char *pszCurCheckResult = (char*)ckRtDesc.strLayer4CheckResult;
		strncpy(szCurItem, pszCurCheckResult, 3);
		szCurItem[3] = 0;
		strncpy(szCurItem+3, pszCurCheckResult+9, 9);
		szCurItem[12] = 0;

		if (0 != strcmp(szKey, szCurItem))
		{
			break;
		}

		char szCurSag[16] = "";
		strncpy(szCurSag, (char*)ckRtDesc.strLayer4Type+4, 2);
		//if (0 == strnicmp(ckRtDesc.strLayer4Type.c_str()+4, szSAG, 2))
		if (strstr(szSAG, szCurSag))
		{
			int iOffset = 18;

			// 不适用
			if ('y' == szCheckResult[iOffset])
			{
				dwCheckResult = CHECK_RESULT_INADEQUACY;
				break;
			}
			else
			{
				if((pszCurCheckResult[iOffset] < 'a') 
					|| (pszCurCheckResult[iOffset] > 'f')   // 目前只有 a 到 f 和 y,没有看到有d开头的
					|| (szCheckResult[iOffset] != pszCurCheckResult[iOffset]))
				{
					dwCheckResult = CHECK_RESULT_UNKNOWN;
					break;
				}

				int iMatchValue = atoi(szCheckResult+iOffset+1);
				ckRtDesc.iMatchValue;
				// 1：小于
				// 2：小于等于
				// 3：等于
				// 4：大于等于
				// 5：大于
				switch(ckRtDesc.iMatchCondition)
				{
				case 1:
					if (iMatchValue < ckRtDesc.iMatchValue)
						dwCheckResult = CHECK_RESULT_CONFORM;
					else
						dwCheckResult = CHECK_RESULT_INCONFORM;
					break;
				case 2:
					if (iMatchValue <= ckRtDesc.iMatchValue)
						dwCheckResult = CHECK_RESULT_CONFORM;
					else
						dwCheckResult = CHECK_RESULT_INCONFORM;
					break;
				case 3:
					if (iMatchValue == ckRtDesc.iMatchValue)
						dwCheckResult = CHECK_RESULT_CONFORM;
					else
						dwCheckResult = CHECK_RESULT_INCONFORM;
					break;
				case 4:
					if (iMatchValue >= ckRtDesc.iMatchValue)
						dwCheckResult = CHECK_RESULT_CONFORM;
					else
						dwCheckResult = CHECK_RESULT_INCONFORM;
					break;
				case 5:
					if (iMatchValue > ckRtDesc.iMatchValue)
						dwCheckResult = CHECK_RESULT_CONFORM;
					else
						dwCheckResult = CHECK_RESULT_INCONFORM;
					break;
				default:
					dwCheckResult = CHECK_RESULT_UNKNOWN;
					break;
				}
			}

			break;
		}

		hm_iter++;
	} while (hm_iter != gs_hmCkRtDesc2.end());

	return dwCheckResult;
}


int   CXmlAnalyzeClass::SpeCheckResultJudge(char szCheckResult[32], CHECK_RESULT_DESC2 **pckRtDesc)
{
	//     if ((NULL == szCheckResult) || (22 != strlen(szCheckResult)))
	//         return CHECK_RESULT_UNKNOWN;
	int iResultLen;
	if (NULL == szCheckResult)
		return CHECK_RESULT_UNKNOWN;
	iResultLen = strlen(szCheckResult);
	if ((ITEM_RESULT_LEN_MIN != iResultLen) && (ITEM_RESULT_LEN_MAX != iResultLen))
		return CHECK_RESULT_UNKNOWN;

	// 005XXXXXX101101101b200
	char szKey[64];
	char szCurItem[64];
	strncpy(szKey, szCheckResult, 3);
	szKey[3] = 0;
	strncpy(szKey+3, szCheckResult+9, 9);
	szKey[12] = 0;

	std::multimap<std::string, CHECK_RESULT_DESC2> :: const_iterator hm_iter;

	char szTmp[256] = "";
	hm_iter = gs_hmCkRtDesc2.find(szKey);

	DWORD dwCheckResult = CHECK_RESULT_UNKNOWN;
	*pckRtDesc = NULL;

	do 
	{
		if (hm_iter == gs_hmCkRtDesc2.end())
		{
			break;
		}

		const CHECK_RESULT_DESC2 &ckRtDesc = hm_iter->second;
		*pckRtDesc = (CHECK_RESULT_DESC2 *)(&(hm_iter->second));
		iResultLen = strlen(ckRtDesc.strLayer4CheckResult);

		if ((ITEM_RESULT_LEN_MIN != iResultLen) && (ITEM_RESULT_LEN_MAX != iResultLen))
		{
			hm_iter++;
			continue;
		}

		const char *pszCurCheckResult = (char*)ckRtDesc.strLayer4CheckResult;
		strncpy(szCurItem, pszCurCheckResult, 3);
		szCurItem[3] = 0;
		strncpy(szCurItem+3, pszCurCheckResult+9, 9);
		szCurItem[12] = 0;

		if (0 != strcmp(szKey, szCurItem))
		{
			break;
		}

		int iOffset = 18;

		// 不适用
		if ('y' == szCheckResult[iOffset])
		{
			dwCheckResult = CHECK_RESULT_INADEQUACY;
			break;
		}
		else
		{
			if((pszCurCheckResult[iOffset] < 'a') 
				|| (pszCurCheckResult[iOffset] > 'f')   // 目前只有 a 到 f 和 y,没有看到有d开头的
				|| (szCheckResult[iOffset] != pszCurCheckResult[iOffset]))
			{
				dwCheckResult = CHECK_RESULT_UNKNOWN;
				break;
			}

			int iMatchValue = atoi(szCheckResult+iOffset+1);
			ckRtDesc.iMatchValue;
			// 1：小于
			// 2：小于等于
			// 3：等于
			// 4：大于等于
			// 5：大于
			switch(ckRtDesc.iMatchCondition)
			{
			case 1:
				if (iMatchValue < ckRtDesc.iMatchValue)
					dwCheckResult = CHECK_RESULT_CONFORM;
				else
					dwCheckResult = CHECK_RESULT_INCONFORM;
				break;
			case 2:
				if (iMatchValue <= ckRtDesc.iMatchValue)
					dwCheckResult = CHECK_RESULT_CONFORM;
				else
					dwCheckResult = CHECK_RESULT_INCONFORM;
				break;
			case 3:
				if (iMatchValue == ckRtDesc.iMatchValue)
					dwCheckResult = CHECK_RESULT_CONFORM;
				else
					dwCheckResult = CHECK_RESULT_INCONFORM;
				break;
			case 4:
				if (iMatchValue >= ckRtDesc.iMatchValue)
					dwCheckResult = CHECK_RESULT_CONFORM;
				else
					dwCheckResult = CHECK_RESULT_INCONFORM;
				break;
			case 5:
				if (iMatchValue > ckRtDesc.iMatchValue)
					dwCheckResult = CHECK_RESULT_CONFORM;
				else
					dwCheckResult = CHECK_RESULT_INCONFORM;
				break;
			default:
				dwCheckResult = CHECK_RESULT_UNKNOWN;
				break;
			}
			break;
		}

		//hm_iter++;
	} while (hm_iter != gs_hmCkRtDesc2.end());

	return dwCheckResult;
}

int ReDefiCheckPoint(int value)
{
	int res;
	if(value == CHECK_RESULT_CONFORM) //符合
	{
		res = CHECK_RESULT_CONFORM_DB;
	}
	else if(value == CHECK_RESULT_INCONFORM) // 不符合
	{
		res = CHECK_RESULT_INCONFORM_DB;
	}
	else if(value == CHECK_RESULT_INADEQUACY)// 不适用
	{
		res = CHECK_RESULT_INADEQUACY_DB;
	}
	else  // 未知
	{
		res = CHECK_RESULT_UNKNOWN_DB;
	}
	return res;
}

//获取sag
char* CXmlAnalyzeClass::GetSag(char* strSysGuid, char* sag)
{
	DB_RESULT	result;
	DB_ROW		row;
	//char sag[20] = {0};

	result = DBselect(con, "SELECT BUSSINESS_GRADING, SYSTEM_SERVICE_GRADING,SECURITY_GRADING  FROM t_rec_sys WHERE REC_SYS_GUID = '%s'", strSysGuid);

	while (NULL != (row = DBfetch(result)))
	{
		int sSag = atoi(row[0]);
		int aSag = atoi(row[1]);
		int gSag = atoi(row[2]);
		sprintf_s(sag, 20, "s%da%dg%d",sSag, aSag, gSag);
		break;
	}
	DBfree_result(result);
	return sag;

}

//通过sag得到系统等级
int CXmlAnalyzeClass::SAGCompare(int s, int a, int g)
{
	int max = 0;
	if(s < a)
		max = a;
	else
		max = s;
	if(max < g)
		max = g;
	return max;
}

//网站安全、数据库、系统漏洞加入填空问题 locate临时表
void CXmlAnalyzeClass::LocateItemCode_ResultTable(int toolType, char *sag, bool isWeb)
{
	int i = 0;
	if(isWeb)
	{
		while(m_WebFlawNum[i].pItemCode1[0])
		{
			DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_WebFlawNum[i].num, m_WebFlawNum[i].pItemCode4);
			i++;
		}
		return;
	}
	int level = SAGCompare(sag[1]-48, sag[3]-48, sag[5]-48);
	int code = 0;
	switch(toolType)
	{
		case DBTOOL_XTLD_M:
			if(level == 2)
			{
				while(m_XtldFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_XtldFlawNum[i].num, m_XtldFlawNum[i].pItemCode1);
					i++;
				}
			}
			else if(level == 3)
			{
				while(m_XtldFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_XtldFlawNum[i].num, m_XtldFlawNum[i].pItemCode2);
					i++;
				}
			}
			else if(level == 4)
			{
				while(m_XtldFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_XtldFlawNum[i].num, m_XtldFlawNum[i].pItemCode3);
					i++;
				}
			}
			else
				;
			break;
		case DBTOOL_WZAQ_M:
			if(level == 2)
			{

				while(m_WebFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_WebFlawNum[i].num, m_WebFlawNum[i].pItemCode1);
					i++;
				}
			}
			else if(level == 3)
			{
				while(m_WebFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_WebFlawNum[i].num, m_WebFlawNum[i].pItemCode2);
					i++;
				}
			}
			else if(level == 4)
			{
				while(m_WebFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_WebFlawNum[i].num, m_WebFlawNum[i].pItemCode3);
					i++;
				}
			}
			else
				;
			break;
		case DBTOOL_DB_M:
			if(level == 2)
			{
				while(m_DbFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_DbFlawNum[i].num, m_DbFlawNum[i].pItemCode1);
					i++;
				}
			}
			else if(level == 3)
			{
				while(m_DbFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_DbFlawNum[i].num, m_DbFlawNum[i].pItemCode2);
					i++;
				}
			}
			else if(level == 4)
			{
				while(m_DbFlawNum[i].pItemCode1[0])
				{
					DBexecute(con, "update itemcode_result set sum = '%s' where itemcode = '%s'", m_DbFlawNum[i].num, m_DbFlawNum[i].pItemCode3);
					i++;
				}
			}
			else
				;
			break;
		default:
			break;
	}
}

void CXmlAnalyzeClass::CheckPoint(int checkType)
{
	typedef std::pair<std::string, CHECK_RESULT_DESC2> CkRtDesc_Pair;
	CHECK_RESULT_DESC2 ckRtDesc;
	char szKey[64] = {0};
	const char* pszCkRt = NULL;

	DB_RESULT	result;
	DB_ROW		row;

	if(checkType == 1)
		result = DBselect(con, "SELECT item_type, result_type, result_value, match_type, match_value, match_condition FROM grxa_infosystem_check_result");
	if(checkType == 2)
		result = DBselect(con, "SELECT item_type, result_type, result_value, match_type, match_value, match_condition FROM grxa_spe_check_result");

	while (NULL != (row = DBfetch(result)))
	{
		memset(&ckRtDesc, 0, sizeof(CHECK_RESULT_DESC2));
		strcpy(ckRtDesc.strLayer4Type, row[0]);
		strcpy(ckRtDesc.strLayer4Check, row[1]);
		strcpy(ckRtDesc.strLayer4CheckResult, row[2]);
		ckRtDesc.iResult = atoi(row[3]);
		ckRtDesc.iMatchValue = atoi(row[4]);
		ckRtDesc.iMatchCondition = atoi(row[5]);
		pszCkRt = (char*)ckRtDesc.strLayer4Check;
		strncpy(szKey, pszCkRt, 3);
		szKey[3] = 0;
		strcat(szKey, pszCkRt+9);
		gs_hmCkRtDesc2.insert(CkRtDesc_Pair(szKey, ckRtDesc));
	}
	DBfree_result(result);
}


int CXmlAnalyzeClass::ItemcodeResultToDb(TiXmlElement* root, XmlToolInfo xmlToolInfo, char* sag)
{
	//每次导入前先清空临时表
	DBexecute(con, "delete from itemcode_result;");

	do 
	{
		//获取终端等不符合数目
		int nTtlNo = 0;
		int nErr;
		int nToolType = xmlToolInfo.toolType;

		//获取XML的具体不符合项数目
		TOOL_XML_RESULT tXmlResult;
		memset(&tXmlResult, 0, sizeof(tXmlResult));
		BOOL bOkResult = 0;
		if (nToolType == DBTOOL_BD_M || nToolType == DBTOOL_MM_M || nToolType == DBTOOL_WBESHEEL_M
			|| nToolType == DBTOOL_WEAKPWD_M || nToolType == DBTOOL_WZAQ_M
			|| nToolType == DBTOOL_DB_M || nToolType == DBTOOL_XTLD_M)
		{
			bOkResult = GetXmlResults(nToolType, root,  tXmlResult);
			if (bOkResult == FALSE)
			{
				nErr = -55;
				break;
			}
		}

		CHECK_RESULT_DESC2 *pckRtDesc = NULL;

		OptionInfo ic;
		int dwCkRt;

		std::vector<OptionInfo>::iterator it = m_OptionInfo.begin();
		std::vector<OptionInfo>::iterator itemEnd = m_OptionInfo.end();
		int ReDefiCkRt;

		
		if((strcmp(xmlToolInfo.strSysGuid, "0") != 0) && (strlen(xmlToolInfo.strSysGuid)!=0))
		{
			CheckPoint(1);
			for(;it != itemEnd; it++)
			{
				ic = *it;
				dwCkRt = SysCheckResultJudge((char*)ic.optionNumber.c_str(), sag, &pckRtDesc);
				ReDefiCkRt = ReDefiCheckPoint(dwCkRt);
				if(dwCkRt != CHECK_RESULT_UNKNOWN)
				{
					DBexecute(con, "replace into itemcode_result (plan_guid, system_guid, asset_guid, itemcode, result, res_string) values ('%s', '%s', '%s', '%s', %d, '%s');",
						xmlToolInfo.strPlanGuid, xmlToolInfo.strSysGuid, xmlToolInfo.strAssetGuid, pckRtDesc->strLayer4Type, ReDefiCkRt, ic.option.c_str());			

				}
				if (dwCkRt != CHECK_RESULT_CONFORM && dwCkRt != CHECK_RESULT_INADEQUACY)
					++nTtlNo;
			}

			//将填空问题更新到临时表
			LocateItemCode_ResultTable(nToolType, sag, false);

			DBexecute(con, "UPDATE t_plan_result w, itemcode_result t SET w.result_Check = t.result,w.is_Tool=1 , w.tool_result_counts = t.sum WHERE t.itemcode = w.item_Code AND t.asset_guid = w.asset_Guid AND t.plan_guid = w.plan_Guid AND t.system_guid = w.rec_Sys_Guid;");
		}
		else
		{
			CheckPoint(2);
			 for(;it != itemEnd; it++)
			 {
				 ic = *it;
				 dwCkRt = SpeCheckResultJudge((char*)ic.optionNumber.c_str(), &pckRtDesc);
				 ReDefiCkRt = ReDefiCheckPoint(dwCkRt);
				 if(dwCkRt != CHECK_RESULT_UNKNOWN)
				 {
					 DBexecute(con, "replace into itemcode_result (plan_guid, system_guid, asset_guid, itemcode, result, res_string) values ('%s', '%s', '%s', '%s', %d, '%s');",
						 xmlToolInfo.strPlanGuid, xmlToolInfo.strSysGuid, xmlToolInfo.strAssetGuid, pckRtDesc->strLayer4Type, ReDefiCkRt, ic.option.c_str());
				 }
				 if (dwCkRt != CHECK_RESULT_CONFORM && dwCkRt != CHECK_RESULT_INADEQUACY)
					 ++nTtlNo;
			 }
			LocateItemCode_ResultTable(nToolType, sag, true);
			DBexecute(con, "UPDATE t_plan_result w, itemcode_result t SET w.result_Check = t.result,w.is_Tool=1 , w.tool_result_counts = t.sum WHERE t.itemcode = w.item_Code AND  w.asset_Guid IS NULL AND t.plan_guid = w.plan_Guid AND w.rec_Sys_Guid IS NULL;");
		}
		
	} while (0);

	

	return 0;
}

//判断计划系统资产是否存在 0-不存在 1-存在
int CXmlAnalyzeClass::PlanIsExist(XmlToolInfo xmlToolInfo)
{
	DB_RESULT	result;
	DB_ROW		row;
	int res = 0;

	if((strcmp(xmlToolInfo.strSysGuid, "0") != 0) && (strlen(xmlToolInfo.strSysGuid)!=0))
	{
		result = DBselect(con, "SELECT * from t_plan_tool WHERE plan_Guid ='%s' AND asset_Guid ='%s' AND tool_Type='%s';",
			xmlToolInfo.strPlanGuid, xmlToolInfo.strAssetGuid, xmlToolInfo.toolTypeStr);
	}
	else
	{
		result = DBselect(con, "SELECT * from t_plan_tool WHERE plan_Guid ='%s' AND asset_Guid IS NULL AND tool_Type='%s';",
			xmlToolInfo.strPlanGuid,  xmlToolInfo.toolTypeStr);
	}

	
	if(NULL == (row = DBfetch(result)))
	{
		res = 0;
	}
	else
	{
		res = 1;
	}
	DBfree_result(result);
	return res;
}

//type:1网口自动导入  2U口
int CXmlAnalyzeClass::ImportCheckXml(char* xmlfile, char* htmlpath, int type)
{

	int nErr = 0;
	bool bRet = true;
	char htmlfile[MAX_PATH] = {0};

	TiXmlDocument doc(xmlfile);
	bool loadOkay = doc.LoadFile();
	if(!loadOkay)
	{
		return -1;
	}
	TiXmlElement* root = doc.RootElement();
	if (!root)
	{
		return -2;
	}

	TiXmlDeclaration* decl = doc.FirstChild()->ToDeclaration(); 
	const char *pEncode = decl->Encoding();
	int iE = 0;
	if(!_stricmp(pEncode, "utf-8")){
		iE = 1;
	}


	XmlToolInfo xmlToolInfo;
	do{

		if(type == 2){
			char uxmlfile[MAX_PATH] = {0};
			strcpy_s(uxmlfile, MAX_PATH, xmlfile);
			int uFalg =  GetUToolType(root, xmlToolInfo, uxmlfile);
			if(0 != uFalg){
				nErr = uFalg;
				break;
			}
		}
		else{
			if (0 != GetToolType(root, xmlToolInfo)) //通过解析XML获得
			{
				nErr = -11;
				break;
			}

		}

		if(type == 1)
		{
			int isExist = PlanIsExist(xmlToolInfo);
			if(!isExist)
			{
				nErr = -99;
				break;
			}
		}
		

		//获取sag
		char sagstr[20] = {0};
		char* sag = GetSag(xmlToolInfo.strSysGuid, sagstr);

		
		//strcpy_s(sagstr, 20, sag);

		int nToolType = xmlToolInfo.toolType;

		GetXmlData(root, nToolType, iE);
		ItemcodeResultToDb(root, xmlToolInfo, sag);

		//转html
		
		strcpy_s(htmlfile, MAX_PATH, htmlpath);

		strcat_s(htmlfile, MAX_PATH, "\\");
		strcat_s(htmlfile, MAX_PATH, xmlToolInfo.strPlanGuid);

		if(access(htmlfile, 0) == -1)
		{
			int flag = mkdir(htmlfile);
			if(flag != 0)
			{
				nErr = -12;
				break;
			}
		}

		sprintf_s(htmlfile, MAX_PATH, "%s\\%s@%s@%d.html", htmlfile, xmlToolInfo.strSysGuid, xmlToolInfo.strAssetGuid, xmlToolInfo.toolType);

		typedef int (*DLLFunc)(char*, char*, int);
		DLLFunc dllFunc;
		HINSTANCE hInstLibrary = LoadLibrary("ToolXml.dll");
		if (hInstLibrary == NULL)
		{
			FreeLibrary(hInstLibrary);
			nErr = -51;
			break;
		}
		dllFunc = (DLLFunc)GetProcAddress(hInstLibrary, "XmlToHtml");
		if (NULL == dllFunc)
		{
			FreeLibrary(hInstLibrary);
			nErr = -52;
			break;
		}
		if(dllFunc(xmlfile, htmlfile, nToolType) == -1)
		{
			FreeLibrary(hInstLibrary);
			nErr = -53;
			break;
		}
		FreeLibrary(hInstLibrary);

		

	}while (0);

	for(int i = 0; i < strlen(xmlfile);i++)
	{
		if(xmlfile[i] == '\\')
		{
			xmlfile[i] = '/';
		}
	}

	for(int i = 0; i < strlen(htmlfile);i++)
	{
		if(htmlfile[i] == '\\')
		{
			htmlfile[i] = '/';
		}
	}

	if((strcmp(xmlToolInfo.strSysGuid, "0") != 0) && (strlen(xmlToolInfo.strSysGuid)!=0))
	{
		DBexecute(con, "UPDATE t_plan_tool SET tool_Result_Xml_Path = '%s', tool_Result_Html_Path='%s', tool_Result=1 , errorcode=%d , notice=0 WHERE plan_Guid ='%s' AND asset_Guid ='%s' AND tool_Type='%s';",
			xmlfile, htmlfile, nErr, xmlToolInfo.strPlanGuid, xmlToolInfo.strAssetGuid, xmlToolInfo.toolTypeStr);
	}
	else
	{
		DBexecute(con, "UPDATE t_plan_tool SET tool_Result_Xml_Path = '%s', tool_Result_Html_Path='%s', tool_Result=1 , errorcode=%d , notice=0 WHERE plan_Guid ='%s' AND asset_Guid IS NULL AND tool_Type='%s';",
			xmlfile, htmlfile, nErr, xmlToolInfo.strPlanGuid, xmlToolInfo.toolTypeStr);
	}
	

	root->Clear();
	doc.Clear();

	return nErr;
}


