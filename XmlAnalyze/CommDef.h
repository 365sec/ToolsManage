#ifndef COMM_DEF_H__
#define COMM_DEF_H__

//系统日志操作类型
#define SYSLOG_OPR_ADDRCRD		1		//增加数据记录
#define SYSLOG_OPR_MODRCRD		2		//修改数据记录
#define SYSLOG_OPR_DELRCRD		3		//删除数据记录
#define SYSLOG_OPR_IMPRTRCRD		4		//导入方式增加数据记录
#define SYSLOG_OPR_EXPTRCRD		5		//记录导出
#define SYSLOG_OPR_COPYRCRD		7		//记录复制
#define SYSLOG_OPR_PRNTRCRD		8		//记录打印
#define SYSLOG_OPR_LOGIN		11		//系统登录
#define SYSLOG_OPR_LOGOFF		12		//系统注销

#define SYSLOG_MDL_LOGIN			1		//系统登录
#define SYSLOG_MDL_PLAN			2		//计划管理
#define SYSLOG_MDL_TMPLT			3		//模板管理
#define SYSLOG_MDL_ZSK				4		//知识库管理
#define SYSLOG_MDL_TOOLCHECK		5		//工具检查
#define SYSLOG_MDL_RESULTRPT		6		//检查结果报告管理
#define SYSLOG_MDL_NOTICE			7		//通知通报

//#define SYSLOG_ADDRCRD		4

//系统日志类型
#define SYSLOG_TP_LOGINOUT		1		//系统登录
#define SYSLOG_TP_RECORD		2		//记录更改

//通知类型- 数据字典 - 检查附件类型说明
#define LOG_NTC_AUDIO				1
#define LOG_NTC_VIDEO				2
#define LOG_NTC_DOC				3
#define LOG_NTC_OTHER				9
#define LOG_NTC_ATCH10	  			10	//已经发布的行业网络安全文件和标准描述附件
#define LOG_NTC_ATCH11				11	//公安机关开展此次安全检查的工作背景附件
#define LOG_NTC_ATCH12				12	//上次检查工作中发现的问题附件
#define LOG_NTC_ATCH13				13	//检查结果附件压缩包

#define LOG_NTC_YTTZ 				15	//约谈通知书
#define LOG_NTC_TJZD 				16	//停机整顿通知书
#define LOG_NTC_XQZG 				17	//限期整改通知书
#define LOG_NTC_FKYJ				18	//反馈意见
#define LOG_NTC_CFJD 				19	//行政处罚决定书
#define LOG_NTC_QKTB 				20	//检查情况通报书

#define LOG_NTC_JCTZ				40	//检查通知书
#define LOG_NTC_JDJCR 				41	//监督检查记录
#define LOG_NTC_SJFW 				51	//上级发文
#define LOG_NTC_RCGZED				91	//日常工作电子模板 -1-	电子文档
#define LOG_NTC_RCGZEX				92	//日常工作电子模板 -2-	电子表单
#define LOG_NTC_ONTC 				99	//其它通知

//全局宏 
#define DB_GUID_LEN	40
#define DBZSK_ITEMCODE_L			48

#define	TREEPLAN_NODE_ROOT		100
#define	TREEPLAN_NODE_PLAN		101
#define	TREEPLAN_NODE_ORGH		111
#define	TREEPLAN_NODE_ORGU		112
#define	TREEPLAN_NODE_SYSR		114
#define	TREEPLAN_NODE_WEB 		115
#define	TREEPLAN_NODE_SPE 		116
#define	TREEPLAN_NODE_OTHER		119
#define	TREEPLAN_NODE_SYSNAME		121
#define	TREEPLAN_NODE_DEVR		131
#define	TREEPLAN_NODE_DEVNAME		141

#define	TREEPLAN_ITEM_MGR			81		//信息系统管理项
#define	TREEPLAN_ITEM_DEV			82		//信息系统设备节点
#define	TREEPLAN_ITEM_DEVTCH		83		//信息系统设备技术项

//数据库中值
#define     TOOL_TYPE_MAXNUM       14

#define     DBTOOL_MAX_MEMN_M      15    //本地缓存的最大工具数目

#define     DBTOOL_WINDOWS_M        1    //windows
#define     DBTOOL_LINUX_M          2    //linux
#define     DBTOOL_BD_M             3    //病毒
#define     DBTOOL_MM_M             4    //木马
#define     DBTOOL_WLSB_M           5     //网络设备
#define     DBTOOL_WBESHEEL_M       6     //恶意代码
#define     DBTOOL_RESERVE_M        7     //保留，以便后续扩充
#define     DBTOOL_AQSB_M           8     //安全设备
#define     DBTOOL_WEAKPWD_M        9     //弱口令
#define     DBTOOL_SQL_M            10    //SQL注入
#define     DBTOOL_XTLD_M           11    //系统漏洞
#define     DBTOOL_WZAQ_M           12    //网站安全
#define     DBTOOL_DB_M             13   //数据库
#define     DBTOOL_OTHER_M          99   //其它

//数据库中检查计划任务的宏
#define		DB_PLAN_STATE_CREATE		0
#define		DB_PLAN_STATE_RUN			1
#define		DB_PLAN_STATE_FINISH		4

//界面中工具代表值
#define     TOOL_WINDOWS_M        1    //windows
#define     TOOL_BD_M             2    //病毒
#define     TOOL_MM_M             3    //木马
#define     TOOL_WBESHEEL_M       4     //恶意代码
#define     TOOL_LINUX_M          5    //linux
#define     TOOL_WLSB_M           6     //网络设备
#define     TOOL_AQSB_M           7     //安全设备
#define     TOOL_WEAKPWD_M        8     //弱口令
#define     TOOL_DB_M             9   //数据库
#define     TOOL_WZAQ_M           10    //网站安全
#define     TOOL_XTLD_M           11    //系统漏洞
#define     TOOL_OTHER_M          99  //其它
#define     TOOL_SLEVEL_NET_M             15    //网络
#define     TOOL_SLEVEL_MAC_M             16    //主机
#define     TOOL_SLEVEL_APLY_M             17    //应用

#define     CHK_STATUS_UNSELECTED          '0'    //未选择
#define     CHK_STATUS_CHECKWAIT           '1'    //待查
#define     CHK_STATUS_CHECKFINISH         '2'    //已完成
#define     CHK_STATUS_NOAPPLY             '4'    //不适用


#define     ITEM_RESULT_LEN_MIN         22                       // 一般结果长度，如：011XXXXXX333101124a002
#define     ITEM_RESULT_LEN_MAX         (ITEM_RESULT_LEN_MIN+1)  // 末尾带s的结果长度，如：011XXXXXX333101124a002s
#define     RESULT_SAG_LEN              6   // s3a3g3长度

// 检查结果
#define     CHECK_RESULT_CONFORM        1   // 符合
#define     CHECK_RESULT_INCONFORM      2   // 不符合
#define     CHECK_RESULT_INADEQUACY     3   // 不适用
#define     CHECK_RESULT_UNKNOWN        4   // 未知

//检查结果数据库
#define     CHECK_RESULT_CONFORM_DB        1   // 符合
#define     CHECK_RESULT_INCONFORM_DB      0   // 不符合
#define     CHECK_RESULT_INADEQUACY_DB     2   // 不适用
#define     CHECK_RESULT_UNKNOWN_DB        3   // 未知

//数据库审计员
#define     DB_USERTYPE_AUTID       102

//窗体间通信
#define WM_FORM_HOME1 WM_USER+101


#endif
