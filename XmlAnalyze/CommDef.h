#ifndef COMM_DEF_H__
#define COMM_DEF_H__

//ϵͳ��־��������
#define SYSLOG_OPR_ADDRCRD		1		//�������ݼ�¼
#define SYSLOG_OPR_MODRCRD		2		//�޸����ݼ�¼
#define SYSLOG_OPR_DELRCRD		3		//ɾ�����ݼ�¼
#define SYSLOG_OPR_IMPRTRCRD		4		//���뷽ʽ�������ݼ�¼
#define SYSLOG_OPR_EXPTRCRD		5		//��¼����
#define SYSLOG_OPR_COPYRCRD		7		//��¼����
#define SYSLOG_OPR_PRNTRCRD		8		//��¼��ӡ
#define SYSLOG_OPR_LOGIN		11		//ϵͳ��¼
#define SYSLOG_OPR_LOGOFF		12		//ϵͳע��

#define SYSLOG_MDL_LOGIN			1		//ϵͳ��¼
#define SYSLOG_MDL_PLAN			2		//�ƻ�����
#define SYSLOG_MDL_TMPLT			3		//ģ�����
#define SYSLOG_MDL_ZSK				4		//֪ʶ�����
#define SYSLOG_MDL_TOOLCHECK		5		//���߼��
#define SYSLOG_MDL_RESULTRPT		6		//������������
#define SYSLOG_MDL_NOTICE			7		//֪ͨͨ��

//#define SYSLOG_ADDRCRD		4

//ϵͳ��־����
#define SYSLOG_TP_LOGINOUT		1		//ϵͳ��¼
#define SYSLOG_TP_RECORD		2		//��¼����

//֪ͨ����- �����ֵ� - ��鸽������˵��
#define LOG_NTC_AUDIO				1
#define LOG_NTC_VIDEO				2
#define LOG_NTC_DOC				3
#define LOG_NTC_OTHER				9
#define LOG_NTC_ATCH10	  			10	//�Ѿ���������ҵ���簲ȫ�ļ��ͱ�׼��������
#define LOG_NTC_ATCH11				11	//�������ؿ�չ�˴ΰ�ȫ���Ĺ�����������
#define LOG_NTC_ATCH12				12	//�ϴμ�鹤���з��ֵ����⸽��
#define LOG_NTC_ATCH13				13	//���������ѹ����

#define LOG_NTC_YTTZ 				15	//Լ̸֪ͨ��
#define LOG_NTC_TJZD 				16	//ͣ������֪ͨ��
#define LOG_NTC_XQZG 				17	//��������֪ͨ��
#define LOG_NTC_FKYJ				18	//�������
#define LOG_NTC_CFJD 				19	//��������������
#define LOG_NTC_QKTB 				20	//������ͨ����

#define LOG_NTC_JCTZ				40	//���֪ͨ��
#define LOG_NTC_JDJCR 				41	//�ල����¼
#define LOG_NTC_SJFW 				51	//�ϼ�����
#define LOG_NTC_RCGZED				91	//�ճ���������ģ�� -1-	�����ĵ�
#define LOG_NTC_RCGZEX				92	//�ճ���������ģ�� -2-	���ӱ�
#define LOG_NTC_ONTC 				99	//����֪ͨ

//ȫ�ֺ� 
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

#define	TREEPLAN_ITEM_MGR			81		//��Ϣϵͳ������
#define	TREEPLAN_ITEM_DEV			82		//��Ϣϵͳ�豸�ڵ�
#define	TREEPLAN_ITEM_DEVTCH		83		//��Ϣϵͳ�豸������

//���ݿ���ֵ
#define     TOOL_TYPE_MAXNUM       14

#define     DBTOOL_MAX_MEMN_M      15    //���ػ������󹤾���Ŀ

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

//���ݿ��м��ƻ�����ĺ�
#define		DB_PLAN_STATE_CREATE		0
#define		DB_PLAN_STATE_RUN			1
#define		DB_PLAN_STATE_FINISH		4

//�����й��ߴ���ֵ
#define     TOOL_WINDOWS_M        1    //windows
#define     TOOL_BD_M             2    //����
#define     TOOL_MM_M             3    //ľ��
#define     TOOL_WBESHEEL_M       4     //�������
#define     TOOL_LINUX_M          5    //linux
#define     TOOL_WLSB_M           6     //�����豸
#define     TOOL_AQSB_M           7     //��ȫ�豸
#define     TOOL_WEAKPWD_M        8     //������
#define     TOOL_DB_M             9   //���ݿ�
#define     TOOL_WZAQ_M           10    //��վ��ȫ
#define     TOOL_XTLD_M           11    //ϵͳ©��
#define     TOOL_OTHER_M          99  //����
#define     TOOL_SLEVEL_NET_M             15    //����
#define     TOOL_SLEVEL_MAC_M             16    //����
#define     TOOL_SLEVEL_APLY_M             17    //Ӧ��

#define     CHK_STATUS_UNSELECTED          '0'    //δѡ��
#define     CHK_STATUS_CHECKWAIT           '1'    //����
#define     CHK_STATUS_CHECKFINISH         '2'    //�����
#define     CHK_STATUS_NOAPPLY             '4'    //������


#define     ITEM_RESULT_LEN_MIN         22                       // һ�������ȣ��磺011XXXXXX333101124a002
#define     ITEM_RESULT_LEN_MAX         (ITEM_RESULT_LEN_MIN+1)  // ĩβ��s�Ľ�����ȣ��磺011XXXXXX333101124a002s
#define     RESULT_SAG_LEN              6   // s3a3g3����

// �����
#define     CHECK_RESULT_CONFORM        1   // ����
#define     CHECK_RESULT_INCONFORM      2   // ������
#define     CHECK_RESULT_INADEQUACY     3   // ������
#define     CHECK_RESULT_UNKNOWN        4   // δ֪

//��������ݿ�
#define     CHECK_RESULT_CONFORM_DB        1   // ����
#define     CHECK_RESULT_INCONFORM_DB      0   // ������
#define     CHECK_RESULT_INADEQUACY_DB     2   // ������
#define     CHECK_RESULT_UNKNOWN_DB        3   // δ֪

//���ݿ����Ա
#define     DB_USERTYPE_AUTID       102

//�����ͨ��
#define WM_FORM_HOME1 WM_USER+101


#endif
