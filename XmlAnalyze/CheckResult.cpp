#include "stdafx.h"
#include "CommDef.h"
#include "CheckResult.h"



BOOL    GetDBScanResult(TiXmlNode* pSubElement, const char* pSubElementName, TOOL_XML_RESULT& ss)
{
    const char* pTxt;
    TiXmlNode* pItem;
    TiXmlNode* pSubItem;
    const unsigned short LEVEL_HIGH    = 0xdfb8;
    const unsigned short LEVEL_MIDDLE  = 0xd0d6;
    const unsigned short LEVEL_LOW     = 0xcdb5;
    const unsigned short LEVEL_INFO    = 0xc5d0;
    unsigned short nLevel;

    pItem = pSubElement->FirstChild( pSubElementName );
    if (NULL == pItem)
        return FALSE;

    pItem = pItem->FirstChild( "item" );
    if (NULL == pItem)
        return FALSE;

    do 
    {
        pSubItem = pItem->FirstChild( "item_threat" );
        if (pSubItem && (pTxt = pSubItem->ToElement()->GetText()))
        {
            nLevel = *(short *)(pTxt);
            switch (nLevel)
            {
            case LEVEL_HIGH:
                ss.iHighNum++;
                break;
            case LEVEL_MIDDLE:
                ss.iMiddleNum++;
                break;
            case LEVEL_LOW:
                ss.iLowNum++;
                break;
            case LEVEL_INFO:
                ss.iInfoNum++;
                break;
            default:
                break;
            }
        }

        pItem = pItem->NextSibling( "item" );
    } while (pItem);

    return TRUE;
}

BOOL    GetXmlResults(int iToolType, TiXmlElement* root,  TOOL_XML_RESULT& txr)
{
    // Read information from xml file.  

    // define xml file path, as follow , we use relative path,  
    // but you can use absolute path also.  
    //const char* filepath = "phonebookdata.xml";  
   
    char szDstBuf[1024];
    char* pszHeadBuf;
    int   iHeadBufLen;

      
    // <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="virusScan.exe" toolname="病毒检查工具" toolcategory="003" toolversion="2.0.0.1.1" />
    // <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="TrojanScan.exe" toolname="木马检查工具" toolcategory="004" toolversion="2.0.0.1.1" />
    // <tool_info corpcode="GUORUIXINAN" corpname="" toolcode="WBSL2.0" toolname="国瑞信安网站恶意代码检查工具" toolcategory="006" toolversion="v2.0" />
    // <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="弱口令检查工具.exe" toolname="弱口令检查工具" toolcategory="009" toolversion="V2.0.0.0" />
    // <tool_info toolversion="V2.0" toolname="漏洞扫描检查工具" toolcode="CpLeakScan.exe" toolcategory="011" corpname="" corpcode="GABDSYJS"/>
    // <tool_info corpcode='GABDSYJS' corpname='' toolcategory='012' toolcode='CpWebScan.exe' toolname='网站安全检查工具' toolversion='2.0.6.6'>
    // <tool_info toolversion="V2.0" toolcategory="013" corpcode="" corpname="" toolname="数据库安全检查工具" toolcode="DBScan" />
  
	TiXmlNode* pSubElement;
	TiXmlElement* pElementInfo;
	TiXmlNode*  pItem;
	const char* pTxt = NULL;

    switch(iToolType)
    {
    case DBTOOL_BD_M://TOOL_TYPE_VIRUS:
    case DBTOOL_MM_M://TOOL_TYPE_TROJAN:
    case DBTOOL_WBESHEEL_M://TOOL_TYPE_WEBSHELL:
        {
            // <result>
            //     <scheme>
            //         <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="virusScan.exe" toolname="病毒检查工具"   toolcategory="003" toolversion="2.0.0.1.1" />
            //         <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="TrojanScan.exe" toolname="木马检查工具"  toolcategory="004" toolversion="2.0.0.1.1" />
            //         <tool_info corpcode="GUORUIXINAN"  corpname="" toolcode="WBSL2.0" toolname="网站恶意代码检查工具" toolcategory="006" toolversion="v2.0" />    
            //     </scheme>
            //     <summary ScannedDirctories="313" ScannedFiles="3715" ScannedData="49.84 MB" ElapsedTime="22 sec (0 m 22 sec)" InfectedFiles="3589" />
            // </result>
            txr.eToolType = iToolType;

            pSubElement = root->FirstChild( "summary" );
            if (NULL == pSubElement)
                return FALSE;

            pElementInfo = pSubElement->ToElement();
            pTxt = pElementInfo->Attribute("InfectedFiles");
            if (NULL == pTxt)
                return FALSE;

            txr.iTotalNum = atoi(pTxt);

            break;
        }
    case DBTOOL_WEAKPWD_M://TOOL_TYPE_WEAK_PASSWORD:
        {
            // <result>
            //     <scheme>
            //         <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="弱口令检查工具.exe" toolname="弱口令检查工具" toolcategory="009" toolversion="V2.0.0.0" />
            //     </scheme>
            //     <summary ElapsedTime="00:01:51" WeakPwdCnts="3" ScannedService="13" />
            // <result>
            txr.eToolType = iToolType;

            pSubElement = root->FirstChild( "summary" );
            if (NULL == pSubElement)
                return FALSE;

            pElementInfo = pSubElement->ToElement();
            pTxt = pElementInfo->Attribute("WeakPwdCnts");
            if (NULL == pTxt)
                return FALSE;

            txr.iTotalNum = atoi(pTxt);

            break;
        }
    case DBTOOL_XTLD_M://TOOL_TYPE_SYSTEM_VULNERABILITY_SCAN:
        {
            // <result>
            //     <scheme>
            //         <tool_info toolversion="V2.0" toolname="漏洞扫描检查工具" toolcode="CpLeakScan.exe" toolcategory="011" corpname="" corpcode="GABDSYJS"/>
            //     </scheme>
            // 	   <report>
            // 		 <Statistics>
            // 			<item>
            // 				<item_name_number>0</item_name_number>
            // 				<item_name>合计</item_name>
            // 				<item_Code>Total</item_Code>
            // 				<high>2</high>
            // 				<middle>50</middle>
            // 				<low>11</low>
            // 				<info>4</info>
            // 				<sum>67</sum>
            // 			</item>
            // 		 </Statistics>
            // 	   </report>
            // </result>
            txr.eToolType = iToolType;

            pSubElement = root->FirstChild( "report" );
            if (NULL == pSubElement)
                return FALSE;

            pItem = pSubElement->FirstChild( "Statistics" );
            if (NULL == pItem)
                return FALSE;

            pItem = pItem->FirstChild( "item" );
            if (NULL == pItem)
                return FALSE;

            BOOL bFind = FALSE;

            while (pItem)
            {
                TiXmlNode* pSubItem = pItem->FirstChild( "item_Code" );
                if (pSubItem)
                {
                    pTxt = pSubItem->ToElement()->GetText();
                    if (pTxt && (0 == stricmp(pTxt, "Total")))
                    {
                        bFind = TRUE;
                        pSubItem = pItem->FirstChild( "high" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iHighNum = atoi(pTxt);
                        }
                        pSubItem = pItem->FirstChild( "middle" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iMiddleNum = atoi(pTxt);
                        }
                        pSubItem = pItem->FirstChild( "low" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iLowNum = atoi(pTxt);
                        }
                        pSubItem = pItem->FirstChild( "info" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iInfoNum = atoi(pTxt);
                        }
                        pSubItem = pItem->FirstChild( "sum" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iTotalNum = atoi(pTxt);
                        }
                        break;
                    }
                }

                pItem = pItem->NextSibling( "item" );
            }

            if (!bFind)
                return FALSE;

            break;
        }
    case DBTOOL_WZAQ_M://TOOL_TYPE_WEB_VULNERABILITY_SCAN:
        {
            // <result>
            //     <scheme>
            //         <tool_info corpcode='GABDSYJS' corpname='' toolcategory='012' toolcode='CpWebScan.exe' toolname='网站安全检查工具' toolversion='2.0.6.6'>
            //     </scheme>
            // 	<ScanGroup>
            // 		<Statistics>
            // 			<Item>
            // 				<item_name_number>0</item_name_number>
            // 				<item_name>合计</item_name>
            // 				<item_Code>Total</item_Code>
            // 				<high>7</high>
            // 				<middle>2</middle>
            // 				<low>23</low>
            // 				<info>4</info>
            // 				<sum>36</sum>
            // 			</Item>
            // 		</Statistics>
            // 	</ScanGroup>
            // </result>
            txr.eToolType = iToolType;

            pSubElement = root->FirstChild( "ScanGroup" );
            if (NULL == pSubElement)
                return FALSE;

            pItem = pSubElement->FirstChild( "Statistics" );
            if (NULL == pItem)
                return FALSE;

            pItem = pItem->FirstChild( "Item" );
            if (NULL == pItem)
                return FALSE;

            BOOL bFind = FALSE;

            while (pItem)
            {
                TiXmlNode* pSubItem = pItem->FirstChild( "item_Code" );
                if (pSubItem)
                {
                    pTxt = pSubItem->ToElement()->GetText();
                    if (pTxt && (0 == stricmp(pTxt, "Total")))
                    {
                        bFind = TRUE;
                        pSubItem = pItem->FirstChild( "high" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iHighNum = atoi(pTxt);
                        }
                        pSubItem = pItem->FirstChild( "middle" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iMiddleNum = atoi(pTxt);
                        }
                        pSubItem = pItem->FirstChild( "low" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iLowNum = atoi(pTxt);
                        }
                        pSubItem = pItem->FirstChild( "info" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iInfoNum = atoi(pTxt);
                        }
                        pSubItem = pItem->FirstChild( "sum" );
                        if (pSubItem)
                        {
                            if (pTxt = pSubItem->ToElement()->GetText())
                                txr.iTotalNum = atoi(pTxt);
                        }
                        break;
                    }
                }

                pItem = pItem->NextSibling( "Item" );
            }

            if (!bFind)
                return FALSE;

            break;
        }
    case DBTOOL_DB_M://TOOL_TYPE_DB_VULNERABILITY_SCAN:
        {
            // <result>
            //     <report>        
            //         <vulnerabity>
            //             <vulnerabity />
            //             <PLSQL_vulnerabity />
            //             <safetyinformation_vulnerabity />
            //             <accesscontrol_vulnerabity />
            //             <accessbypass_vulnerabity />
            //             <bufferoverflow_vulnerabity />
            //             <rightflaw_vulnerabity />
            //             <bigauthority_vulnerabity />
            //             <denialofserviceattack_vulnerabity />
            //             <crosssiteattack_vulnerabity />
            //             <other_vulnerabity>
            //                 <item>
            //                     <item_vulnerabity />
            //                     <item_cvss />
            //                     <item_cve>无</item_cve>
            //                     <item_threat>中</item_threat>
            //                     <item_description />
            //                     <item_recommendation />
            //                 </item>
            //             </other_vulnerabity>
            //         </vulnerabity>
            //     </report>
            // </result>

            txr.eToolType = (E_TOOL_TYPE)iToolType;

            pSubElement = root->FirstChild( "report" );
            if (NULL == pSubElement)
                return FALSE;

            pSubElement = pSubElement->FirstChild( "vulnerabity" );
            if (NULL == pSubElement)
                return FALSE;

            // <vulnerabity />
            // <PLSQL_vulnerabity />
            // <safetyinformation_vulnerabity />
            // <accesscontrol_vulnerabity />
            // <accessbypass_vulnerabity />
            // <bufferoverflow_vulnerabity />
            // <rightflaw_vulnerabity />
            // <bigauthority_vulnerabity />
            // <denialofserviceattack_vulnerabity />
            // <crosssiteattack_vulnerabity />
            // <other_vulnerabity>
            GetDBScanResult(pSubElement, "vulnerabity", txr);
            GetDBScanResult(pSubElement, "PLSQL_vulnerabity", txr);
            GetDBScanResult(pSubElement, "safetyinformation_vulnerabity", txr);
            GetDBScanResult(pSubElement, "accesscontrol_vulnerabity", txr);
            GetDBScanResult(pSubElement, "accessbypass_vulnerabity", txr);
            GetDBScanResult(pSubElement, "bufferoverflow_vulnerabity", txr);
            GetDBScanResult(pSubElement, "rightflaw_vulnerabity", txr);
            GetDBScanResult(pSubElement, "bigauthority_vulnerabity", txr);
            GetDBScanResult(pSubElement, "denialofserviceattack_vulnerabity", txr);
            GetDBScanResult(pSubElement, "crosssiteattack_vulnerabity", txr);
            GetDBScanResult(pSubElement, "other_vulnerabity", txr);
            txr.iTotalNum = txr.iHighNum + txr.iMiddleNum + txr.iLowNum + txr.iInfoNum;

            break;
        }
        break;
    default:
        txr.eToolType = DBTOOL_OTHER_M;//TOOL_TYPE_UNKNOWN;
        return FALSE;
    }

    return TRUE;
}

