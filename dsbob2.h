// DSBOB2.h: interface for the DSBOB2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSBOB2_H__8417E7D5_DBF5_4B7D_8DE4_6FFC1F6E5094__INCLUDED_)
#define AFX_DSBOB2_H__8417E7D5_DBF5_4B7D_8DE4_6FFC1F6E5094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _WIN32
//#include "C:\DNASP\DPCAS4.1\WIN\calcdll\lib.h"
//#include "../DPCAS4.1/WINDLL/calcdll/lib.h"
//#include "../DNASPLIB/netmom.h"		// Network model definition
//#include "../DNASPLIB/tmpodrtb.h"	// Table management template for database object
//#include "../DNASPLIB/LLD.h"
#elif __linux__
#else
#endif

#include "../DNASPLIB/netmom.h"		// Network model definition
#include "../DNASPLIB/tmpodrtb.h"	// Table management template for database object
#include "../DNASPLIB/LLD.h"

//电压等级，因为涉及到的电压等级很少，不需要做一个类来维护
typedef struct BASEVL
{
	//@Num	mRID	name	nomkV
	//序号	标识	基准电压名	基准电压
	int	Num;		//序号
	char mRID[80];	//标识
	char name[80];	//基准电压名
	double nomkV;	//基准电压
} BASEVL;

typedef struct NM1_RESULT//n-1的结果
{
	long	id;
	char fdid[MAX_ID_DLL];//馈线id
	char secid[MAX_ID_DLL];//馈线段id
	double Faultload;//故障段失电负荷(MW)
	double Trload;//非故障段需转移负荷(MW)
	double NowTrload;//非故障段实际转移负荷(MW)
	double PowerLostRatio;//非故障段失电负荷所占总负荷的比例%
	char unionList[MAX_UNIN_ID_SUM];// 转供电源
	char secResult;//线段通过n-1
	char LineResult;//整条线通过n-1

	char transSec[FEMA_ID_LEN];
	char lossSec[FEMA_ID_LEN];
	//char zoneLoss[FEMA_ID_LEN];
} NM1_RESULT;

typedef struct RO_RESULT//无功补偿的结果
{
	long	id;
	char fdid[80];//馈线id
	char ndid[80];//馈线上节点id

	double Qv;//补偿容量
	char b_cosfi[10];//补偿前功率因素
	char a_cosfi[10];//补偿后功率因素

	double b_v;//补偿前电压
	double a_v;//补偿后电压

} RO_RESULT;

typedef struct RA_RESULT//可靠性的结果
{
//	long	id;
	int	type;//0-系统；1-馈线；2-线段
	
	char eqid[MAX_SEC_ID_LEN];//负荷id
	char fdid[MAX_SEC_ID_LEN];//所属馈线
	
	double ASAI_Fault;//可靠率(只考虑故障停电%)
	double SAIFI_Fault;//系统平均停电频率(次/年)
	double SAIDI_Fault;//系统平均停电时间(考虑故障停电与预安排停电，小时/年)
	double ENS_Fault;//系统平均停电时间(考虑故障停电，小时/年)

	double ASAI_Sum;//可靠率(考虑故障停电与预安排停电%)
	double SAIFI_Sum;//系统平均停电频率(次/年)
	double SAIDI_Sum;//系统平均停电时间(考虑故障停电与预安排停电，小时/年)
	double ENS_Sum;//系统平均停电时间(考虑故障停电，小时/年)

} RA_RESULT;

//馈线的分段信息
typedef struct SECT_INFO
{
	string fdid;         //馈线ID
	string fdname;
	string version;      //版本号
	string calcDate;     //计算日期
	int sect_id;         //分段序号
	string cut;          //断点
	int sumLD;
	int sumVip;
	vector<string> idList;//分段的负荷ID号集合
	vector<string> secList;//分段的线段ID号集合
	double sumRatedS;
	double sumW;
	double sumLen;
	int sumUN;

	//参数
	int cntsect;//分段数；
				//权重
	double wW;  //负荷大小
	double wLD; //负荷个数
	double wLen;//分段长度
	double wUN; //联络开关约束
	double wVIP;//重要用户约束
	double wSecW;//分段负荷大小约束
				 //优化后结果
	double ruoW;  //负荷大小
	double ruoLD; //负荷个数
	double ruoLen;//分段长度
	double punishUN; //联络开关约束
	double punishVIP;//重要用户约束
	double punishSecW;//分段负荷大小约束

}SECT_INFO;

//unsigned int __stdcall doReadDB(void* pThread);  

class DSBOB2 
{
public:
	DSBOB2();
	DSBOB2(CLLD* m_lld);
	virtual ~DSBOB2();

	CLLD* lld;

	int getDevType(char * line, char * devType, int& n_line, int &flag_b);
	//根据传入的sSQL语句，构造含邻接馈线的新的SQL语句
	//CString getAdjSQL(CString lFdid);

	void inputmem(int type=0);
	void deletemem();//删除临时开的内存
	
	int ReadAnalogData();
	int ReadDigitalData();

	//构造读数据的语句
	//读网络版

//T0	1	1	1	0.02
//T1	5	1	3	0.025
//T2	3	1	2	0.050000001
//T3	17	1	9	0.0125
//T4	5	1	2	0.025
//T5	7	1	5	0.025
//T6	7	1	4	0.025

	//lld edit 2018-4-25
#if _WIN32
	//int ReadData(
	//	   SOT* so,int cntSO,
	//	   CBT* cb,int cntCB,
	//	   BUST* bs,int cntBUS,
	//	   SECT* sec,int cntSEC,
	//	   XFMRT* xfmr,int cntXFMR,
	//	   TAPTYT* tapty,int cntTAPTY,
	//	   LDT* ld,int cntLD,
	//	   CPT* cp,int cntCP,
	//	   NDT* nd,int cntND
	//			);
#endif

	//从文本文件里面读数据，该文件的格式为自定义，非E文件
	//主要针对厦门linux临时用
	int ReadDataFromFile(char * FileName, char * optParam);

	//读取E格式数据文件,如果是短路电流，类型为1，接地计算用
	int ReadDataFromEfile(char * FileName);
	//从CIME里面读数据，高压配电网用
	int ReadCIME(char * filename);
	//拓扑分析专用，只需填写线路表即可，里面含有首末节点，节点的类型
	int ReadDataFromTPfile(char * sFileName, char * traceSub);


	DTable<FD>* fd_tb_t;
	int OnGetPhByfdid(char* fdid,int type=0);
	int OnGetBsByND(char * nd);

	//用线程来读数据
	//int ReadDataByThread();
	
//	//将潮流计算结果输出到efile文件中，传给人机界面显示,type==0,输出全部的数据，其它为几个岛
	int WriteOutfile_pf(char* sFileName,int type=100);

	//输出短路电流，1.0版本为3个结果，以后成熟后全部用2.0进行输出
	//int WriteOutfile_ra(char* sFileName);
	int WriteOutfile_nm1(char* sFileName);	

	int WriteOutfile_ro(char* sFileName);

	int WriteOutfile_optsect(char* sFileName, int sect_id);
//	int WriteOutfile_optsect(char* sFileName, optSect* pOptSect);

	
public:
	NM1_RESULT nm1_result[5000];
	RO_RESULT ro_result[2];
	RA_RESULT ra_result[500];//每条馈线最多200个负荷
	SECT_INFO fdsect[6];
	char outFileName[128];

	BASEVL	basevl[10];//配电网大概只有110,66,35,10,20,6等几个电压等级
	//通过电压等级代码得到电压等级的实际标准值
	//mrID-电压等级标识，默认为type=0；如果基准电压等级，默认为1
	float getVLBymrID(char * mrID, int type = 0);

public:

	//在这里声明，在读入数据里得到实体表，在nmd得到关系表
	//table objects for network model database
	DTable<GLOBAL>*           global_tb;

	DTable<ANALOG>*           analog_tb;
	DTable<DIGITAL>*          digital_tb;

	DTable<CO>*               co_tb;
	DTable<ST>*               st_tb;
	DTable<SO>*               so_tb;
	DTable<BUS>*              bus_tb;
	DTable<CB>*               cb_tb;
	DTable<SEC>*              sec_tb;
	DTable<LD>*               ld_tb;
	DTable<XFMR>*             xfmr_tb;
	DTable<XF>*               xf_tb;
	DTable<TAPTY>*            tapty_tb;
	DTable<CP>*               cp_tb;

	DTable<ISLAND>*           island_tb;
	DTable<BS>*               bs_tb;
	
	DTable<ND>*               nd_tb;
	//lld add 2019-5-31
	DTable<ND>*               nd_tb_t;
	DTable<BUS>*              bus_tb_t;
	DTable<CB>*               cb_tb_t;
	DTable<SEC>*              sec_tb_t;
	DTable<LD>*               ld_tb_t;
	DTable<XFMR>*             xfmr_tb_t;
	DTable<XF>*               xf_tb_t;
	DTable<SO>*               so_tb_t;
	DTable<CP>*               cp_tb_t;
	DTable<ST>*               st_tb_t;
	DTable<VL>*               vl_tb_t;
	DTable<VL>*               vl_tb;
	DTable<SECP>*             secp_tb_t;
	DTable<TAPTY>*            tapty_tb_t;

	DTable<R_CO_ST>*	r_co_st_tb;
	DTable<R_ST_XFMR>*	r_st_xfmr_tb;
	DTable<R_ST_CB>*	r_st_cb_tb;
	DTable<R_ST_SO>*	r_st_so_tb;
	DTable<R_ST_LD>*	r_st_ld_tb;
	DTable<R_ST_CP>*	r_st_cp_tb;
	DTable<R_ST_BUS>*	r_st_bus_tb;
	//lld end add 2019-5-31

	DTable<R_ND_LD>*          r_nd_ld_tb;
	DTable<R_ND_CB>*          r_nd_cb_tb;
	DTable<R_ND_SEC>*         r_nd_sec_tb;
	DTable<R_ND_CP>*         r_nd_cp_tb;
	DTable<R_ND_XF>*          r_nd_xf_tb;
	DTable<R_XFMR_XF>*        r_xfmr_xf_tb;

	DTable<ZN>*               zn_tb;
	DTable<R_ZN_LD>*          r_zn_ld_tb;
	DTable<R_ZN_SEC>*         r_zn_sec_tb;
	DTable<R_ZN_CB>*          r_zn_cb_tb;
	DTable<R_ZN_CP>*          r_zn_cp_tb;
	DTable<R_ZN_XF>*          r_zn_xf_tb;

	//lld add 2015-12-31
	DTable<UN>*				  un_tb;//联络开关表
	DTable<FD>*               fd_tb;//馈线表
	DTable<R_FD_UN>*          r_fd_un_tb;//馈线联络
	DTable<R_FD_SO>*          r_fd_so_tb;
	DTable<R_FD_ZN>*		  r_fd_zn_tb;
	DTable<R_FD_EQ>*		  r_fd_eq_tb;
	DTable<R_FD_LD>*		  r_fd_ld_tb;

	//lld add 2016-1-12
	DTable<EQ>*				  eq_tb;//全部的可靠性计算设备
	DTable<R_ND_EQ>*		  r_nd_eq_tb;//设备与节点的对应关系
};

#endif // !defined(AFX_DSBOB2_H__8417E7D5_DBF5_4B7D_8DE4_6FFC1F6E5094__INCLUDED_)
