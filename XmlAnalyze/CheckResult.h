#ifndef XML_CHECK_RESULT_H
#define XML_CHECK_RESULT_H

#include <Windows.h>
#include "tinyxml/tinyxml.h"


// 木马，病毒，webshell
// <result>
//     <scheme>
//         <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="virusScan.exe" toolname="病毒检查工具"  toolcategory="003" toolversion="2.0.0.1.1" />
//         <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="TrojanScan.exe" toolname="木马检查工具" toolcategory="004" toolversion="2.0.0.1.1" />
//         <tool_info corpcode="GUORUIXINAN" corpname="" toolcode="WBSL2.0" toolname="网站恶意代码检查工具" toolcategory="006" toolversion="v2.0" />    
//     </scheme>
//     <summary ScannedDirctories="313" ScannedFiles="3715" ScannedData="49.84 MB" ElapsedTime="22 sec (0 m 22 sec)" InfectedFiles="3589" />
// </result>
// 
// 弱口令
// <result>
//     <scheme>
//         <tool_info corpcode="JSGRXAKJYXGS" corpname="" toolcode="弱口令检查工具.exe" toolname="弱口令检查工具" toolcategory="009" toolversion="V2.0.0.0" />
//     </scheme>
//     <summary ElapsedTime="00:01:51" WeakPwdCnts="3" ScannedService="13" />
// <result>
// 
//     
// 系统漏扫
// <result>
//     <scheme>
//         <tool_info toolversion="V2.0" toolname="漏洞扫描检查工具" toolcode="CpLeakScan.exe" toolcategory="011" corpname="" corpcode="GABDSYJS"/>
//     </scheme>
// 	<report>
// 		<Statistics>
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
// 		</Statistics>
// 	</report>
// </result>
//         
// 网站安全
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
// 
// 
// 数据库漏扫
// <result>
//     <scheme>
//         <tool_info toolversion="V2.0" toolcategory="013" corpcode="" corpname="" toolname="数据库安全检查工具" toolcode="DBScan" />
//     </scheme>
//     <report>
//         <vulnerabity_amount>
//             <PLSQL_vulnerabity_amount>0</PLSQL_vulnerabity_amount>
//             <safetyinformation_vulnerabity_amount>0</safetyinformation_vulnerabity_amount>
//             <accesscontrol_vulnerabity_amount>0</accesscontrol_vulnerabity_amount>
//             <accessbypass_vulnerabity_amount>0</accessbypass_vulnerabity_amount>
//             <bufferoverflow_vulnerabity_amount>0</bufferoverflow_vulnerabity_amount>
//             <rightflaw_vulnerabity_amount>0</rightflaw_vulnerabity_amount>
//             <bigauthority_vulnerabity_amount>0</bigauthority_vulnerabity_amount>
//             <denialofserviceattack_vulnerabity>1</denialofserviceattack_vulnerabity>
//             <crosssiteattack_vulnerabity>0</crosssiteattack_vulnerabity>
//             <other_vulnerabity_amount>5</other_vulnerabity_amount>
//         </vulnerabity_amount>
//     </report>
// </result>
typedef struct  T_SCAN_STATISTICS
{
    int iHighNum;
    int iMiddleNum;
    int iLowNum;
    int iInfoNum;
    int iTotalNum;
    //T_SCAN_STATISTICS() { iHighNum=0; iMiddleNum=0; iLowNum=0; iInfoNum=0; iTotalNum=0; }
}SCAN_STATISTICS;

enum E_TOOL_TYPE{
    TOOL_TYPE_VIRUS                     = 3, 
    TOOL_TYPE_TROJAN                    = 4, 
    TOOL_TYPE_WEBSHELL                  = 6, 
    TOOL_TYPE_WEAK_PASSWORD             = 9,
    TOOL_TYPE_SYSTEM_VULNERABILITY_SCAN = 11, 
    TOOL_TYPE_WEB_VULNERABILITY_SCAN    = 12, 
    TOOL_TYPE_DB_VULNERABILITY_SCAN     = 13, 
    TOOL_TYPE_UNKNOWN                   = 255
};

// <PLSQL_vulnerabity_amount>0</PLSQL_vulnerabity_amount>
// <safetyinformation_vulnerabity_amount>0</safetyinformation_vulnerabity_amount>
// <accesscontrol_vulnerabity_amount>0</accesscontrol_vulnerabity_amount>
// <accessbypass_vulnerabity_amount>0</accessbypass_vulnerabity_amount>
// <bufferoverflow_vulnerabity_amount>0</bufferoverflow_vulnerabity_amount>
// <rightflaw_vulnerabity_amount>0</rightflaw_vulnerabity_amount>
// <bigauthority_vulnerabity_amount>0</bigauthority_vulnerabity_amount>
// <denialofserviceattack_vulnerabity>1</denialofserviceattack_vulnerabity>
// <crosssiteattack_vulnerabity>0</crosssiteattack_vulnerabity>
// <other_vulnerabity_amount>5</other_vulnerabity_amount>
typedef struct T_DB_VULNERABILITY_SCAN
{
    int iPlsqlNum;
    int iSafetyinformationNum;
    int iAccesscontrolNum;
    int iAccessbypassNum;
    int iBufferoverflowNum;
    int iRightflawNum;
    int iBigauthorityNum;
    int iDenialofserviceattackNum;
    int iCrosssiteattackNum;
    int iOtherNum;
    int iTotalNum;
}DB_VULNERABILITY_SCAN;

typedef struct T_TOOL_XML_RESULT 
{
    int eToolType;

        // <high>2</high>
        // <middle>50</middle>
        // <low>11</low>
        // <info>4</info>
        // <sum>67</sum>
            int iHighNum;
            int iMiddleNum;
            int iLowNum;
            int iInfoNum;
            int iTotalNum;



}TOOL_XML_RESULT;


BOOL    GetXmlResults(int iToolType, TiXmlElement* root,  TOOL_XML_RESULT& txr);

#endif  // XML_CHECK_RESULT_H
