////////////////////////////////////////////////////////////////////////////////
//
//  NETMOM.H
//  Class definitions of distribution network model
//定义配电网络模型
////////////////////////////////////////////////////////////////////////////////
#if !defined(__NETMOM_H)
#define __NETMOM_H

//#include "afxwin.h"
#include <stdio.h>  
#include "string.h"
//using std::string;

////////////////////////////////////////////////////////////////////////////////
//	length definition of object ID and DESCR
#define     MAX_DIGITAL_ID_LEN		80		//遥信ID号的最大长度
#define     MAX_DIGITAL_DESCR_LEN	64		//遥信ID描述的最大长度
#define     MAX_ANALOG_ID_LEN		120		//遥测
#define     MAX_ANALOG_DESCR_LEN	64		
#define     MAX_CO_ID_LEN			80		//公司
#define     MAX_CO_DESCR_LEN		64				
#define     MAX_ST_ID_LEN			80		//厂站	
#define     MAX_ST_DESCR_LEN		64				
#define     MAX_ST_ABB_LEN			16 		//厂站缩写,99999	
#define     MAX_XFMR_ID_LEN			80		//变压器	
#define     MAX_XFMR_DESCR_LEN		64			
#define     MAX_XF_ID_LEN			80		//绕组
#define     MAX_XF_DESCR_LEN		64		
#define	    MAX_CB_ID_LEN			80		//开关
#define	    MAX_CB_DESCR_LEN		64			
#define     MAX_SO_ID_LEN			80		//电源
#define	    MAX_SO_DESCR_LEN		64		
#define	    MAX_LD_ID_LEN			120		//负荷
#define     MAX_LD_DESCR_LEN		64		
#define     MAX_CP_ID_LEN			80		//电容器
#define     MAX_CP_DESCR_LEN		64		
#define     MAX_BUS_ID_LEN			80		//母线
#define     MAX_BUS_DESCR_LEN		64
#define     MAX_ZN_ID_LEN			80		//区域
#define     MAX_ZN_DESCR_LEN		64
#define     MAX_SEC_ID_LEN			120		//线段
#define     MAX_SEC_DESCR_LEN		128
#define     MAX_ND_ID_LEN			240		//节点,一个可能属于2条线或多条线
#define     MAX_TAPTY_ID_LEN		80		//变压器抽头类型
#define     MAX_LNTY_ID_LEN			80		//线路类型

#define     MAX_VER_LEN			    50		//规划版本编号
#define     MAX_FDID_LEN			256		//线段
#define     MAX_EQUIPID_LEN			120		//线段
#define     MAX_UNIN_ID_SUM			400		//线段
#define     MAX_EQUIP_DESCR_LEN		120		//线段

#define     MAX_LOW_VOLTAGE_LEN		500		//馈线上低电压的最大个数

////////////////////////////////////////////////////////////////////////////////
//  value definition of mask operation variable
#define     ST_SUBST				1             
#define     ST_SWIST				2		
#define     ST_SO					1		
#define     ST_CP					2		
#define     ST_XFMR					4		
#define     ST_LD					8		
#define     ST_CB					16		
#define     ST_ZN					32		
#define     XFMR_REMOVE 			1		
#define     XFMR_RMVENABL       	2		
#define     XFMR_NRMDEAD       		1
#define     XFMR_INRMOPEN			2
#define     XFMR_INRMGRND			4
#define     XFMR_ZNRMOPEN			8
#define     XFMR_ZNRMGRND			16
#define     XFMR_LIVE				32
#define     XFMR_DEAD				64
#define     XFMR_AMBIG				128
#define     XFMR_UKNOWN				256
#define     XFMR_HOPEN				512
#define     XFMR_HCONN				1024
#define     XFMR_HGRND				2048
#define     XFMR_HCMDMAN			4096
#define     XFMR_MOPEN				8192
#define     XFMR_MCONN				16384
#define     XFMR_MGRND				32768
#define     XFMR_MCMDMAN			65536
#define     XFMR_LOPEN				131072
#define     XFMR_LCONN				262144
#define     XFMR_LGRND				524288
#define     XFMR_LCMDMAN			1048576
#define     XFMR_ABNRML				2097152
#define     XF_REMOVE				1
#define     XF_RMVENABL				2
#define     XF_NRMDEAD				1
#define     XF_INRMOPEN				2
#define     XF_INRMGRND				4
#define     XF_ZNRMOPEN				8
#define     XF_ZNRMGRND				16
#define     XF_LIVE					32
#define     XF_DEAD					64
#define     XF_AMBIG				128
#define     XF_UNKNOWN				256
#define     XF_IOPEN				512
#define     XF_ICONN				1024
#define     XF_IGRND				2048
#define     XF_ICMDMAN				4096
#define     XF_ZOPEN				8192
#define     XF_ZCONN				16384
#define     XF_ZGRND				32768
#define     XF_ZCMDMAN				65536
#define     XF_ABNRML				131072
#define     CB_NMLOPEN				1//联络开关
#define     CB_FORCST				2
#define     CB_BREAKER				4
#define     CB_MOTODIS				8
#define     CB_SWITCH				16
#define     CB_DEADBUS				32
#define     CB_AUTO					64
#define     CB_OPEN					128
#define     CB_OPANABL				256
#define     CB_FDHCB				512
#define     CB_FDSCB				1024	
#define     CB_FDCCB				2048
#define		CB_FDLCB				4096
#define		CB_N_ISOLATE			8192
#define		CB_N_RESTORE			16384

#define     CB_NRMDEAD				1
#define     CB_INRMOPEN				2
#define     CB_INRMGRND				4
#define     CB_ZNRMOPEN				8
#define     CB_ZNRMGRND				16
#define     CB_LIVE					32
#define     CB_DEAD					64
#define     CB_AMBIG				128
#define     CB_UNKNOWN				256
#define     CB_IOPEN				512
#define     CB_ICONN				1024
#define     CB_IGRND				2048
#define     CB_ICMDMAN				4096
#define     CB_ZOPEN				8192
#define     CB_ZGRND				16384
#define     CB_ZCMDMAN				32768
#define     CB_ABNRMAL				65536
#define		CB_FLASH				131072		//for fdir

#define     SO_REMOVE				1
#define     SO_RMVENABL				2
#define     SO_OPEN					4
#define     SO_NRMDEAD				1
#define     SO_NRMOPEN				2
#define     SO_NRMGRND				4
#define     SO_LIVE					8
#define     SO_DEAD					16
#define     SO_AMBIG				32
#define     SO_UNKNOWN				64
#define     SO_IOPEN				128
#define     SO_CONN					256
#define     SO_GRND					512
#define     SO_CMDMAN				1024
#define     SO_ABNRML				2048
#define     LD_REMOVE				1
#define     LD_RMVENABL				2
#define     LD_OPEN					4
#define     LD_ZNLD					8
#define     LD_NRMDEAD				1
#define     LD_NRMOPEN				2
#define     LD_NRMGRND				4
#define     LD_LIVE					8
#define     LD_DEAD					16
#define     LD_AMBIG				32
#define     LD_UNKNOWN				64
#define     LD_IOPEN				128
#define     LD_CONN					256
#define     LD_GRND					512
#define     LD_CMDMAN				1024
#define     LD_ABNRML				2048
#define     LD_FAULT				4096		//for fdir
#define		LD_LEVEL0				8192		//特级负荷
#define		LD_LEVEL1				16384		//一级负荷
#define		LD_LEVEL2 				32768		//二级负荷
#define		LD_LEVEL3				65536		//三级负荷
#define		LD_SO					131072		//多电源

#define     CP_REMOVE				1
#define     CP_RMVENABL				2
#define     CP_OPEN					4
#define     CP_NRMDEAD				1
#define     CP_NRMOPEN				2
#define     CP_NRMGRND				4
#define     CP_LIVE					8
#define     CP_DEAD					16
#define     CP_AMBIG				32
#define     CP_UNKNOWN				64
#define     CP_IOPEN				128
#define     CP_CONN					256
#define     CP_GRND					512
#define     CP_CMDMAN				1024
#define     CP_ABNRML				2048
#define     CP_ZOPEN				4096
#define     BUS_NRMDEAD				1
#define     BUS_NRMOPEN				2
#define     BUS_NRMGRND				4
#define     BUS_LIVE				8
#define     BUS_DEAD				16
#define     BUS_AMBIG				32
#define     BUS_UNKNOWN				64
#define     BUS_OPEN				128
#define     BUS_CONN				256
#define     BUS_GRND				512
#define     BUS_CMDMAN				1024
#define     BUS_ABNRML				2048
#define		BUS_FAULT				4096		//for fdir
#define		BUS_FLASH				8192		//for fdir

#define		ZN_REMOVE				1
#define		ZN_RMVENABL				2
#define     ZN_LOWCAP				4

#define		ZN_NRMDEAD				1
#define     ZN_INRMOPEN				2
#define     ZN_INRMGRND				4
#define     ZN_ZNRMOPEN				8
#define     ZN_ZNRMGRND				16
#define     ZN_LIVE					32
#define     ZN_DEAD					64
#define     ZN_AMBIG				128
#define     ZN_UNKNOWN				256
#define     ZN_IOPEN				512
#define     ZN_ICONN				1024
#define     ZN_IGRND				2048
#define     ZN_ICMDMAN				4096
#define     ZN_ZOPEN				8192
#define     ZN_ZCONN				16384
#define     ZN_ZGRND				32768
#define     ZN_ZCMDMAN				65536
#define     ZN_ABNRML				131072
#define		ZN_FAULT				262144		//for fdir
#define		ZN_FLASH				524288		//for fdir

#define     SEC_REMOVE				1
#define     SEC_RMVENABL			2
#define     SEC_ZPRIME				4
#define     SEC_NRMDEAD				1
#define     SEC_INRMOPEN			2
#define     SEC_INRMGRND			4
#define     SEC_ZNRMOPEN			8
#define     SEC_ZNRMGRND			16
#define     SEC_LIVE				32
#define     SEC_DEAD				64
#define     SEC_AMBIG				128
#define     SEC_UNKNOWN				256
#define     SEC_IOPEN				512
#define     SEC_ICONN				1024
#define     SEC_IGRND				2048
#define     SEC_ICMDMAN				4096
#define     SEC_ZOPEN				8192
#define     SEC_ZCONN				16384
#define     SEC_ZGRND				32768
#define     SEC_ZCMDMAN				65536
#define     SEC_ABNRML				131072
#define		SEC_FAULT				262144		//for fdir
#define		SEC_FLASH				524288		//for fdir

#define		ND_DEAD					1
#define     ND_OPEN					2		
#define     ND_SO					4		
#define     ND_LD					8		
#define     ND_CP					16		
#define     ND_ZN					32
#define		ND_REG					64
#define		ND_GRND					128
#define     ND_UNPROC				256	
#define     ND_MAIN					512	//主干线上的节点	

#define     BS_DEAD					1
#define     BS_OPEN					2
#define     BS_UNREG				4
#define     BS_REGERR				8
#define     BS_VHI					16
#define     BS_VLO					32
#define     BS_REF					64
#define     BS_UNION				128//母线上挂的有联络开关

#define     BS_PRI					1
#define     BS_AVAIL				2

////////////////////////////////////////////////////////////////////////////////
//  flag definition of table name
#define		_DIGITAL				1
#define		_ANALOG					2
#define		_CO						3
#define		_ST						4
#define		_XFMR					5
#define		_XF						6
#define		_CB						7
#define		_SO						8
#define		_LD						9
#define		_CP						10
#define		_BUS					11
#define		_ZN						12
#define		_SEC					13
#define		_ND						14
#define		_BS						15
#define		_ISLAND					16
#define		_GLOBAL					17
#define		_TAPTY					18
#define		_LNTY					19
#define		_R_CO_ST				20
#define		_R_ST_XFMR				21
#define		_R_XFMR_XF				22
#define		_R_ST_CB				23
#define		_R_ST_SO				24
#define		_R_ST_LD				25
#define		_R_ST_CP				26
#define		_R_ST_BUS				27
#define		_R_ST_ZN				28
#define		_R_ZN_LD				29
#define		_R_ZN_SEC				30
#define		_R_ZN_CP				42
#define		_R_ZN_XF				43
#define		_R_ZN_CB				35
#define		_R_ND_CB				31
#define		_R_ND_XF				32
#define		_R_ND_ZN				33
#define		_R_ND_SEC				34
#define		_FDS					36
#define		_RA					    37
#define		_R_FD_ZN				38
#define		_R_FD_LD				39
#define		_R_FD_ND				40
#define		_GRDSEC					41//对地支路

#define ZN_ID_LEN 7

//可靠性计算使用
#define     LD_FIRST				1	//一级负荷
#define     LD_SECOND				2	//二级负荷
#define     LD_THIRD				4	//三级负荷
#define     LD_MULTI				8	//多电源

#define FEMA_ID_LEN  2000
#define mx_bs_sc  	  1000//一条馈线上最多1000个计算母线
#define MAX_MINPATH_LEN 1000


//equip 
//设备类型 add by lld 2017-12-29 for cb optimize
#define Q_ACLINESEGMENT			1	
#define Q_INSULATION			2		//架空绝缘线
#define Q_CABLE					4	
#define Q_CONNLINE				32768   //零阻抗支路

#define Q_MAINLINE				8		//主干线
#define Q_FORKLINE				16		//分支线
#define Q_LOAD					32		//负荷
#define Q_BREAKER				64
#define Q_BREAKERRELAY			128 	//带保护的开关
#define Q_LOADBREAKSWITCH		256 
#define Q_DISCONNECTOR			512
#define Q_FUSE					1024
#define Q_BREAKERFEEDER			2048	//出线开关
#define Q_UNIONCB				4096	//联络开关
#define Q_OPENCB				8192	//分位开关
#define Q_SECTCB				16384	//分段开关

#define MAX_ID_DLL				64		//DLL传入的ID最大值
#define MAX_DESCR_DLL			128		//DLL传入的ID最大值
#define MAX_IDLIST				1024	//DLL传入的ID最大值
#define MAX_MIANND_LD			3200	//主节点上挂接负荷ID号列表，用|隔开


const int max_switch_ld = 200;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
//const int max_fault_ld = 200;	//故障段失电负荷列表
//const int max_out_ld = 200;//故障段下游需要转供的负荷列表
//const int max_trans_ld = 200;//故障段下游可以转供的负荷列表

//lld add 2018-5-9
//const int Q_EQ_UP_DISCONNECTOR	=	1;	//设备上游隔离开关是刀闸 


////////////////////////////////////////////////////////////////////////////////
//  size definition of each table
//const mx_analog=lld.mx_analog_test;
/*
#define		mx_digital_a		80000	//最多只有50000个遥信点
#define		mx_digital			40000	//最多只有50000个遥信点
#define		mx_analog_a			10000
#define		mx_analog			2000
#define		mx_co				1
#define		mx_st_a				6//000
#define		mx_st				6//000
#define		mx_xfmr				10
#define		mx_xf				10
#define		mx_cb_a				80000
#define		mx_cb				30000
#define		mx_so_a				5000
#define		mx_so				1000
#define		mx_ld_a				10000
#define		mx_ld				3000
#define		mx_cp_a				500
#define		mx_cp				50
#define		mx_bus_a			6000
#define		mx_bus				3000
#define		mx_zn				15000
#define		mx_sec_a			50000
#define		mx_sec				20000
#define		mx_nd				30000
#define		mx_bs				10000
#define		mx_island			1000
#define		mx_global			1
#define		mx_tapty			20
#define		mx_lnty				20
#define		mx_r_co_st			6000
#define		mx_r_st_xfmr		10
#define		mx_r_xfmr_xf		10
#define		mx_r_st_cb			2//0000
#define		mx_r_st_so			1//000
#define		mx_r_st_ld			1
#define		mx_r_st_cp			1//00
#define		mx_r_st_bus			5//000
#define		mx_r_st_zn			1//5000
#define		mx_r_zn_ld			20000
#define		mx_r_zn_sec			20000
#define		mx_r_nd_cb			50000
#define		mx_r_nd_ld			20000
#define		mx_r_nd_xf			10
#define		mx_r_nd_zn			10
#define		mx_r_nd_sec			30000
#define		mx_feeder			1000
*/
//


//@Num	mRID	name	pathName	highkV	lowkV	Substation	BaseVoltage	
//序号	标识	中文原名	标准带路径电压等级全名	电压上限	电压下限	所属厂站标识	基准电压标识	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  VL //: public CObject 
{
public:

	char	mRID[MAX_ANALOG_ID_LEN];
	char	name[MAX_ANALOG_DESCR_LEN];
	double	highkV;
	double	lowkV;
	char	BaseVoltage[MAX_ST_ID_LEN];
	char    Substation[MAX_ST_ID_LEN];
public:

	//constructors
	VL();
	VL(const char* m_mRID);

	//assign value operator
	const VL& operator = (const VL& m_VL);

	//compare operator ==
	int operator == (const VL& m_VL) const
	{
		return strcmp(mRID, m_VL.mRID) == 0;
	}

	//compare operator <
	int operator < (const VL& m_VL) const
	{
		return strcmp(mRID, m_VL.mRID) < 0;
	}

protected:

	//DECLARE_SERIAL( VL )
};

//最小路类，包括备用最小路，联络开关到电源点的最小路等
class  MINPATH //: public CObject
{
public:
	int	id;//最小路的编号，物理逻辑同名
	int type;//最小路的类型，0为负荷，1为备用，2为联络

	char	eq_id[MAX_SEC_ID_LEN];//设备ID号，可以是负荷或联络开关 
	int	ind;	//节点
	double	s;		//潮流计算得到的视在功率，联络开关为裕度，负荷为视在功率值
	double	monkV;	//变压器的铭牌上都是视在功率(视在功率)
	int	minpath[MAX_MINPATH_LEN];//负荷的最小路，存最多的200个节点 
	int	pathlen;//最小路的长度 
	
	int cntbakpath;//备用最小路的条数，一个负荷点可能有多个备用最小路，负荷点专用
	
	int q;	//标志位
	int level;	//负荷的等级，一类负荷（重要），二类负荷（一般），三类负荷（农网）

	int equippath[MAX_MINPATH_LEN];//最小路上的设备列表

	int	bakpath[MAX_MINPATH_LEN];//负荷的备用最小路，存最多30个联络开关，每个200个节点 
	int	bakpathlen;//备用最小路的长度 
	
	int bakCBnd;//备用联络开关的节点号，需要搜索到这个联络开关	
	double bakAbund;//备用联络开关的裕度
	
	//联络开关到电源点的路径
	int	unso[MAX_MINPATH_LEN];//联络开关到电源点的路径
	int unso_len;
	int	unso_reverse[MAX_MINPATH_LEN];//电源点到联络开关的方向路径
	int unso_reverse_len;
public:

	//constructors
	MINPATH();
	MINPATH(const int m_id);

	void Reset();

	//assign value operator
	const MINPATH& operator = (const MINPATH& m_MINPATH);

	//compare operator ==
	int operator == (const MINPATH& m_MINPATH) const
	{
		return id==m_MINPATH.id;
	}

	//compare operator <
	int operator < (const MINPATH& m_MINPATH) const
	{
		return id<m_MINPATH.id;
	}

	//read or write object
 //   //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( EQUIP )
};

//CIME文件中的分离的数据
class  CIMEDATA //: public CObject
{
public:
	char	id[256];//设备名称 
	double  data;//设备的遥测值,或遥信值

	int	type;//0遥测  1遥信

public:

	//constructors
	CIMEDATA();
	CIMEDATA(const char* m_id);

	//assign value operator
	const CIMEDATA& operator = (const CIMEDATA& m_CIMEDATA);

	//compare operator ==
	int operator == (const CIMEDATA& m_CIMEDATA) const
	{
		return strcmp(id,m_CIMEDATA.id) == 0;
	}

	//compare operator <
	int operator < (const CIMEDATA& m_CIMEDATA) const
	{
		return strcmp(id,m_CIMEDATA.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( EQUIP )
};

//在最小路可靠性计算时，需要大量的搜索，将单条馈线上的设备集中进行处理可以加快这个进程
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  EQ //: public CObject
{
public:
	int i_id;//序号，用于排序和快速搜索

	int	type;//设备类型 0-breaker；1-loadbreakswitch；2-disconnector；3-fuse；
	//4-联络开关；5-带保护开关；16-出线开关；10-线路段（架空或电缆）；20-负荷
	int		idph;//原设备的物理号
	char	id[MAX_EQUIPID_LEN];//原设备ID号 
	char	fdid[MAX_EQUIPID_LEN];//联络开关在设备内 
	int	ind;	//首节点
	int	iznd;	//末节点
	int q;	//标志位
	int island;	//标志位
	int r_zn;//设备所属区段的关系，lld add 2014-7-18
	int r_fd;//设备所属区段的关系，lld add 2014-7-18
	
	int	minpath[MAX_MINPATH_LEN];//设备的最小路，存最多的50个节点 
	int	idpath[MAX_MINPATH_LEN]; //设备到电源点的设备ID号序列，存最多的50个设备的id号 
	int pathlen;

	double prob_fault;//设备故障率
	double repairTime;//设备修复时间
	
	double prob_check;//设备检修率，根据馈线折算
	double checkTime;//设备检修时间
	
	double switchTime;//设备切换隔离时间，如果电源侧是保护开关或熔断器，则没有隔离时间
	double transTime;//转供时间	
	
	double lambda;//故障率，用来做薄弱环节辨识，后果严重的设备为薄弱环节	
	double lambda_sum;//总停运率(含计划停电)，用来做薄弱环节辨识，后果严重的设备为薄弱环节	
	double fema;//故障后果，用来做薄弱环节辨识，后果严重的设备为薄弱环节	
	double fema_sum;//故障后果(含计划停电)，用来做薄弱环节辨识，后果严重的设备为薄弱环节	
	int	   ld_cnt_loss;//失电用户数（户）
	double ENS;	//受影响用户平均缺供电量 (kWh/户)
	double w_loss;//平均失电负荷(kW)

	//lld add 2018-05-01
	int UpSwitchload[max_switch_ld];//故障段上游负荷列表（切换）
	int UpNonOutload[max_switch_ld];//故障段上游负荷列表（不切换，直接速断）
	int UpPlanSwitchload[max_switch_ld];//预安排上游负荷列表（要先停电部分负荷），刀闸不能直接拉
	int UpPlanNonOutload[max_switch_ld];//预安排上游负荷列表（不停电负荷）
	int Faultload[max_switch_ld];	//故障段失电负荷列表
	int NeedTrload[max_switch_ld];//故障段下游需要转供的负荷列表
	int NowTrload[max_switch_ld];//故障段下游可以转供的负荷列表
	
	int nUpSwitchload;
	int nUpNonOutload;
	int nUpPlanSwitchload;
	int nUpPlanNonOutload;
	int nFaultload;
	int nNeedTrload;
	int nNowTrload;

	char tranfdlist[MAX_UNIN_ID_SUM];//设备发生故障后的转供馈线列表
	char closeunlist[MAX_UNIN_ID_SUM];//设备发生故障后的 闭合 联络开关列表
	char opencblist[MAX_UNIN_ID_SUM]; //设备发生故障后的 打开 分段开关列表

public:

	//constructors
	EQ();
	EQ(const int m_id);
	void Reset();//需要将现有的进行初始化

	//assign value operator
	const EQ& operator = (const EQ& m_EQUIP);

	//compare operator ==
	int operator == (const EQ& m_EQUIP) const
	{
		return i_id == m_EQUIP.i_id;
	}

	//compare operator <
	int operator < (const EQ& m_EQUIP) const
	{
		return i_id < m_EQUIP.i_id;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( EQUIP )
};


class  R_ND_EQ //: public CObject
{
public:
	long	id;//关系表的索引号，用它来做对分法搜索

	char	nd_id[MAX_ND_ID_LEN];
	int		i_id;
	int		nd_id_ptr;
	int		equip_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ND_EQ();
	R_ND_EQ(const char* m_nd_id, const int m_equip_id);

	//assign value operator
	const R_ND_EQ& operator = (const R_ND_EQ& m_r_nd_equip);

	//compare operator ==
	int operator == (const R_ND_EQ& m_r_nd_equip) const
	{
 		return id == m_r_nd_equip.id;
	}

	//compare operator <
	int operator < (const R_ND_EQ& m_r_nd_equip) const
	{
 		return id < m_r_nd_equip.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const int GetChildId() const
	{
		return i_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return equip_id_ptr;
	}

	//read or write object
//    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_EQ )
};


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  DIGITAL //: public CObject
{
public:

	char	id[MAX_DIGITAL_ID_LEN];
	char	descr[MAX_DIGITAL_DESCR_LEN];
	int		value;
	int		quality;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];
	int fds;

public:

	//constructors
	DIGITAL();
	DIGITAL(const char* m_id);

	//assign value operator 
	const DIGITAL& operator = (const DIGITAL& m_digital);

	//compare operator ==
	int operator == (const DIGITAL& m_digital) const
	{
		return strcmp(id, m_digital.id) == 0;
	}

	//compare operator <
	int operator < (const DIGITAL& m_digital) const
	{
		return strcmp(id, m_digital.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	

protected:

	 //DECLARE_SERIAL( DIGITAL )
};

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ANALOG //: public CObject 
{
public:

	char	id[MAX_ANALOG_ID_LEN];
	char	descr[MAX_ANALOG_DESCR_LEN];
	int		analogType;
	double	value;
	int		quality;
	char	loc[MAX_ANALOG_ID_LEN];
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];
    int     fds;	
public:

	//constructors
	ANALOG();
	ANALOG(const char* m_id);

	//assign value operator
	const ANALOG& operator = (const ANALOG& m_analog);

	//compare operator ==
	int operator == (const ANALOG& m_analog) const
	{
		return strcmp(id, m_analog.id) == 0;
	}

	//compare operator <
	int operator < (const ANALOG& m_analog) const
	{
		return strcmp(id, m_analog.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	

protected:

	 //DECLARE_SERIAL( ANALOG )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  CO //: public CObject 
{
public:

	char	id[MAX_CO_ID_LEN];
	char 	descr[MAX_CO_DESCR_LEN];
	float 	wloss;
	float	rloss;
	float	wso;
	float 	rso;
	float	wload;
	float	rload;
	float	cap;
	float	reac;
	int		r_st;
	
    char    ver[MAX_VER_LEN];

public:

	//constructors
	CO();
	CO(const char* m_id);

	//assign value operator
	const CO& operator = (const CO& m_co);

	//compare operator ==
	int	operator == (const CO& m_co) const
	{
		return strcmp(id, m_co.id) == 0;
	}

	//compare operator <
	int operator < (const CO& m_co) const
	{
		return strcmp(id, m_co.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( CO )
};



//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ST //: public CObject
{
public:

	char	id[MAX_ST_ID_LEN];
	char 	descr[MAX_ST_DESCR_LEN];
	char	abb[MAX_ST_ABB_LEN];
	int		type;
	int		qcomp;//元件标志位，这个只有变电站才有
	float 	wso;
	float 	rso;
	float	wload;
	float 	rload;
	int		r_co;
	int		r_xfmr;
	int		r_so;
	int		r_cb;
	int		r_ld;
	int		r_cp;
	int		r_bus;
	int		r_zn;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];

	double prob_fault;//故障率
	float repairTime;//5.5小时 故障修复时间;

	int fds;

public:

	//constructors
	ST();
	ST(const char* m_id);

	//assign value operator
	const ST& operator = (const ST& m_st);

	//compare operator ==
	int operator == (const ST& m_st) const
	{
		return strcmp(id, m_st.id) == 0;
	}

	//compare operator <
	int operator < (const ST& m_st) const
	{
		return strcmp(id, m_st.id) < 0;
	}

	//read or write object
//     //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ST )
};



//
class  XFMR //: public CObject
{
public:

	char	id[MAX_XFMR_ID_LEN];
	char	descr[MAX_XFMR_DESCR_LEN];
	int		type;
	char	st[MAX_ST_ID_LEN];
	double	rh;//正序
	double	xh;
	double	rm;
	double	xm;
	double	rl;
	double	xl;
	double	r0h;//零序
	double	x0h;
	double	r0m;
	double	x0m;
	double	r0l;
	double	x0l;
	double	kvnomh;
	double	kvnomm;
	double	kvnoml;
	double	mvah;
	double	mvam;
	double	mval;
	char	algph[MAX_ANALOG_ID_LEN];
	int		ialgph;
	char	algqh[MAX_ANALOG_ID_LEN];
	int		ialgqh;
	char	algih[MAX_ANALOG_ID_LEN];
	int		ialgih;
	char	algth[MAX_ANALOG_ID_LEN];
	int		ialgth;
	char	algpm[MAX_ANALOG_ID_LEN];
	int		ialgpm;
	char	algqm[MAX_ANALOG_ID_LEN];
	int		ialgqm;
	char	algim[MAX_ANALOG_ID_LEN];
	int		ialgim;
	char	algtm[MAX_ANALOG_ID_LEN];
	int		ialgtm;
	char	algpl[MAX_ANALOG_ID_LEN];
	int		ialgpl;
	char	algql[MAX_ANALOG_ID_LEN];
	int		ialgql;
	char	algil[MAX_ANALOG_ID_LEN];
	int		ialgil;
	char	algtl[MAX_ANALOG_ID_LEN];
	int		ialgtl;
	char	hnd[MAX_ND_ID_LEN];
	char	mnd[MAX_ND_ID_LEN];
	char	lnd[MAX_ND_ID_LEN];
	char	htapty[MAX_TAPTY_ID_LEN];
	char	mtapty[MAX_TAPTY_ID_LEN];
	char	ltapty[MAX_TAPTY_ID_LEN];
	int		htap;
	int		mtap;
	int		ltap;
	int		q;
	int		topflags;
	int		r_st;
	int		r_xf;
	//lld add for distribution network planning platform 2012.03.17
	char    ver[MAX_VER_LEN];
	char    fdid[MAX_SEC_ID_LEN];
	//lld add 2018-5-14
	double prob_fault;//故障率
	double repairTime;//5.5小时 故障修复时间;

public:

	//constructors
	XFMR();
	XFMR(const char* m_id);

	//assign value operator
	const XFMR& operator = (const XFMR& m_xfmr);

	//compare operator
	int operator == (const XFMR& m_xfmr) const
	{
		return strcmp(id, m_xfmr.id) == 0;
	}

	//compare operator <
	int operator < (const XFMR& m_xfmr) const
	{
		return strcmp(id, m_xfmr.id) < 0;
	}

	//read or write object
	//void Serialize(CArchive& ar);

protected:

	//DECLARE_SERIAL( XFMR )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  XF //: public CObject
{
public:

	char	id[MAX_XF_ID_LEN];
	char	descr[MAX_XF_DESCR_LEN];
	int		itapty;//一次侧抽头类型号
	int		iztapty;
	int		tap;//一次侧抽头位置
	int		ztap;
	float  	kvnom;//短路损耗
	float	zkvnom;//短路损耗百分比
	char	nd[MAX_ND_ID_LEN];
	char	znd[MAX_ND_ID_LEN];
	int		ind;
	int		iznd;
	int		ibs;
	int		zbs;
	float	mvanom;//零序电阻
	int		q;
	int		topflags;
	float	r1;
	float	x1;
	float	r0;
	float	x0;
	int		ialgip;
	int		ialgiq;
	int		ialgii;
	int		ialgit;
	int		ialgzp;
	int		ialgzq;
	int		ialgzi;
	int		ialgzt;
	float	w;
	float	r_;//零序电抗
	float	i;
	float	wz;//无功量测，高压网计算临时用
	float	rz;
	float	iz;
	int		r_xfmr;
	int		r_zn;
	//lld add for distribution network planning platform 2012.03.17
	char    ver[MAX_VER_LEN];
	char    fdid[MAX_FDID_LEN];

	char    vl[MAX_FDID_LEN];//绕组的电压等级
	float	basevoltage;
	char    tapty[MAX_FDID_LEN];//绕组的抽头

	float v_b;
	float adeg_b;
	float v_e;
	float adeg_e;

public:

	//constructors
	XF();
	XF(const char* m_id);

	//assign value operator
	const XF& operator = (const XF& m_xf);

	//compare operator ==
	int operator == (const XF& m_xf) const
	{
		return strcmp(id, m_xf.id) == 0;
	}

	//compare operator <
	int operator < (const XF& m_xf) const
	{
		return strcmp(id, m_xf.id) < 0;
	}


protected:

	//DECLARE_SERIAL( XF )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  CB //: public CObject
{
public:

	char	id[MAX_CB_ID_LEN];
	char	descr[MAX_CB_DESCR_LEN];
	char	nd[MAX_ND_ID_LEN];
	char	znd[MAX_ND_ID_LEN];
	int		ind;
	int		iznd;
	char	digital[MAX_DIGITAL_ID_LEN];
	int		idigital;
	int		q;
	int		topflags;
	int		r_st;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关

	int		maincb;//主干线上的开关，默认是0
	int		cbtype;//开关类型 0-breaker；1-loadbreakswitch；2-disconnector；3-fuse
	int fds;

	float prob_fault;//故障率
	float repairTime;//5.5小时 故障修复时间;

	char    vl[MAX_FDID_LEN];//绕组的电压等级

//	float prob_check;//检修停运率
//	float checkTime;//5.5小时 设备检修时间;

//	float switchTime;//1.5小时（人工默认值） 故障检测与隔离的开关切换时间，期间上游负荷未供电;
//	float transTime;//0.5小时（人工默认值，假设有转供路径可以转供）  非故障段恢复供电（转供） 开关操作时间，期间上游已供
public:

	//constructors
	CB();
	CB(const char* m_id);

	//assign value operator
	const CB& operator = (const CB& m_cb);

	//compare operator ==
	int operator == (const CB& m_cb) const
	{
		return strcmp(id, m_cb.id) == 0;
	}

	//compare operator <
	int operator < (const CB& m_cb) const
	{
		return strcmp(id, m_cb.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( CB )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  SO //: public CObject
{
public:

	char	id[MAX_SO_ID_LEN];
	char	descr[MAX_SO_DESCR_LEN];
	char    nd[MAX_ND_ID_LEN];
	char    vl[MAX_ND_ID_LEN];
	int		ind;
	int    	ibs;
	int    	iisland;
	char    algp[MAX_ANALOG_ID_LEN];
	int    	ialgp;
	char    algq[MAX_ANALOG_ID_LEN];
	int    	ialgq;
	char    algi[MAX_ANALOG_ID_LEN];
	int    	ialgi;
	int  	q;
	int    	topflags;
	int		slackavr;
	float   vtarget;
	float   w;
	float   r;
	float	i;
	float	v;//为PV节点准备
	int    	r_st;
	int    	r_fd;
	float	ss;//短路容量
	float	sw;//接地电阻
	float	sr;//接地电抗

	float  I_value;//有功实测值
	float  cap_I;//电容电流

	int		isAnalog_P;//首端的有功是否有量测，有量测为1，无量测为0
	int		isAnalog_Q;//首端的无功是否有量测，有量测为1，无量测为0
	double  fScaleP;//单位负荷的有功折算比例，需要按照5%的线损来进行折算
	double  fScaleQ;//单位负荷的无功折算比例

    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];

	float maxcurrent;//最大电流，用这个来计算裕度,单位是A
	int nm1;//是否可以全线路通过n-1(首段故障)
	
	float	pLoss;//线损
	float	qLoss;

	int fds;

	//可靠性计算指标
	float fault_ltime;//故障含定位隔离时间
	float fault_stime;//故障切换时间（已含定位隔离时间）
	float fault_ttime;//故障转供时间（已含定位隔离时间）

	float plan_stime;//计划切换时间（已含定位隔离时间）
	float plan_ttime;//计划转供时间（已含定位隔离时间）

	float plan_ltime;//故障含定位隔离时间
	float plan_prob;//架空线计划停电率
	float plan_rtime;//架空线计划修复时间

	//lld add 2018-4-22
	float prob_fault;//故障率
	float repairTime;//5.5小时 故障修复时间;

//	double plan_overhead_prob;//架空线计划停电率
//	float plan_overhead_rtime;//架空线计划修复时间
//	double plan_overhead_ins_prob;//架空绝缘线
//	float plan_overhead_ins_rtime;
//	double plan_cable_prob;//电缆
//	float plan_cable_rtime;
	float basevoltage;
	
public:

	//constructors
	SO();
	SO(const char* m_id);

	//assign value operator
	const SO& operator = (const SO& m_so);

	//compare operator ==
	int operator == (const SO& m_so) const
	{
		return strcmp(id, m_so.id) == 0;
	}

	//compare operator <
	int operator < (const SO& m_so) const
	{
		return strcmp(id, m_so.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	

protected:

	 //DECLARE_SERIAL( SO )
};

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  FD //: public CObject
{
public:
	int		i_id;//生成的设备一定要用自然指针

	char	id[MAX_SO_ID_LEN];
	char    vl[MAX_ND_ID_LEN];
	int		q;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];

	float maxcurrent;//最大电流，用这个来计算裕度,单位是A
	
	int nm1;//是否可以全线路通过n-1(首段故障)
	int cnt_un;//联络开关的数量
	int cnt_zn;//联络开关的数量
	int cnt_ld;//联络开关的数量
	int cnt_eq;//联络开关的数量
	int cnt_nd;//联络节点的数量
	int cnt_so;//电源的数量

	int r_un;//和馈线集之间的关系
	int r_zn;//和电源之间的关系，馈线可以包括多个电源，目前是一对一
	int r_so;//和电源之间的关系，馈线可以包括多个电源，目前是一对一
	int r_eq;//和电源之间的关系，馈线可以包括多个电源，目前是一对一
	int r_ld;//和电源之间的关系，馈线可以包括多个电源，目前是一对一
	int r_nd;//和电源之间的关系，馈线可以包括多个电源，目前是一对一

	//可靠性计算指标
	float fault_ltime;//故障含定位隔离时间
	float fault_stime;//故障切换时间（已含定位隔离时间）
	float fault_ttime;//故障转供时间（已含定位隔离时间）

	float plan_ltime;//故障含定位隔离时间
	float plan_prob;//架空线计划停电率
	float plan_rtime;//架空线计划修复时间

	float plan_stime;//计划切换时间（已含隔离时间）
	float plan_ttime;//计划转供时间（已含隔离时间）

	float I_value;//首端实测电流

public:

	//constructors
	FD();
	FD(const int m_id);

	//assign value operator
	const FD& operator = (const FD& m_fd);

	//compare operator ==
	int operator == (const FD& m_fd) const
	{
		//return i_id == m_fd.i_id;

		return strcmp(id, m_fd.id) == 0;
	}

	//compare operator <
	int operator < (const FD& m_fd) const
	{
		//return i_id < m_fd.i_id;

		return strcmp(id, m_fd.id) < 0;
	}

	void Reset();
	//read or write object
    ////void Serialize( CArchive& ar );
	

protected:

	 //DECLARE_SERIAL( FD )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  LD //: public CObject
{
public:

	char	id[MAX_LD_ID_LEN];
	char	descr[MAX_LD_DESCR_LEN];
	long	type;
	char	nd[MAX_ND_ID_LEN];
	char	vl[MAX_ND_ID_LEN];
	int		ind;
	int		ibs;
	char    algp[MAX_ANALOG_ID_LEN];
	int		ialgp;
	char    algq[MAX_ANALOG_ID_LEN];
	int		ialgq;
	char	algi[MAX_ANALOG_ID_LEN];
	int		ialgi;
	int		q;
	int		topflags;
	float   w;
	float   r;
	float   wm;
	float   rm;
	int		r_st;
	int		r_zn;
	int		r_fd;
	
	int		level;//负荷等级，1-一级负荷，2-二级负荷，4-三级负荷，8-多电源用户

    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[512];//存在多电源的情况，这里需要扩大

	int fds;
//是否需要无功补偿，无功补偿

	float prob_fault;//故障率
	float repairTime;//5.5小时 故障修复时间;
//	float prob_check;//检修停运率
//	float checkTime;//5.5小时 设备检修时间;
	float basevoltage;
public:

	//constructors
	LD();
	LD(const char* m_id);

	//assign value operator
	const LD& operator = (const LD& m_ld);

	//compare operator ==
	int operator == (const LD& m_ld) const
	{
		return strcmp(id, m_ld.id) == 0;
	}

	//compare operator <
	int operator < (const LD& m_ld) const
	{
		return strcmp(id, m_ld.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( LD )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  CP //: public CObject
{
public:

	char	id[MAX_CP_ID_LEN];
	char	descr[MAX_CP_DESCR_LEN];
	char	nd[MAX_ND_ID_LEN];
	char	znd[MAX_ND_ID_LEN];
	int		ind;
	int		ibs;
	int		iznd;
	int		zbs;
	char    algq[MAX_ANALOG_ID_LEN];
	int		ialgq;
	int		q;
	int		topflags;
	float	mrnom;
	double   r;

	double   w;
	double   r_;
	double   wz;
	double   rz;

	double   v_b;
	double   adeg_b;
	double   v_e;
	double   adeg_e;
	double   i;

	int		r_st;
	int		r_zn;

	
    //lld add for distribution network planning platform 2012.03.17
    char    vl[MAX_ID_DLL];
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];

	int fds;
	float basevoltage;
public:

	//constructors
	CP();
	CP(const char* m_id);

	//assign value operator
	const CP& operator = (const CP& m_cp);

	//compare operator ==
	int operator == (const CP& m_cp) const
	{
		return strcmp(id, m_cp.id) == 0;
	}

	//compare operator <
	int operator < (const CP& m_cp) const
	{
		return strcmp(id, m_cp.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( CP )
};

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  BUS //: public CObject
{
public:

	char	id[MAX_BUS_ID_LEN];
	char    descr[MAX_BUS_DESCR_LEN];
	char    nd[MAX_ND_ID_LEN];
	int		ind;
	int		ibs;
	char    algv[MAX_ANALOG_ID_LEN];
	int		ialgv;
	int		topflags;
	float	vtarget;
	int		vl;//额定电压值，不参与标幺值计算
	float	vhlim;
	float	vllim;
	float   v;
	float   adeg;
	int		r_st;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];

	int fds;
	float basevoltage;
public:

	//constructors
	BUS();
	BUS(const char* m_id);

	//assign value operator
	const BUS& operator = (const BUS& m_bus);

	//compare operator ==
	int operator == (const BUS& m_bus) const
	{
		return strcmp(id, m_bus.id) == 0;
	}

	//compare operator <
	int operator < (const BUS& m_bus) const
	{
		return strcmp(id, m_bus.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( BUS )
};	

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ZN //: public CObject
{
public:
	long     id;//区段改成整型的目的是为了便于插入时候进行顺序插入

	char    descr[MAX_ZN_DESCR_LEN];
	int		type;
	
	int		q;
	int		topflags;

	int		r_st;
	int		r_fd;//和馈线的关系
	int		r_ld;
	int		r_sec;
	int		r_cb;//区段对应的边界开关的首端开关,第一个开关
	int		r_cp;
	int		r_xf;

	int		cbptr;//区段进入开关的指针
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];

	//lld add 2013-3-13
	char	unfd_idList[MAX_UNIN_ID_SUM];//一个区段的下游联络开关列表，之间用，分开

	//lld add 2014-08-26
	char closeunlist[MAX_UNIN_ID_SUM];//馈线段发生故障后的 闭合 联络开关列表
	char opencblist[MAX_UNIN_ID_SUM];//馈线段发生故障后的 打开 分段开关列表
	
//	char transSec[FEMA_ID_LEN];// 下游转供线段
//	char lossSec[FEMA_ID_LEN];//下游损失线段



	int		inner_C;//区段内用户
	double	inner_P;//区段内有功
	int		next_C;	//区段下游用户
	double	next_P;	//区段下游有功(不含本区段)
	int		lossC;	//区段自己和下游加起来一起损失的用户,上游不损失
	int		transC;	//下游可以转供的用户
	double	lossP;	//区段自己和下游加起来一起损失的用户,上游不损失
	double	transP;	//下游可以转供的用户
	
	int	fault_oper_type;	//区段内设备故障后，引起的后果类型
	int plan_oper_type;		//区段被设备计划检修后，引起的后果类型

public:

	//constructors
	ZN();
	ZN(const int m_id);
	void Reset();//需要将现有的进行初始化

	//assign value operator
	const ZN& operator = (const ZN& m_zn);

	//compare operator ==
	int operator == (const ZN& m_zn) const
	{
		return id == m_zn.id;
	}

	//compare operator <
	int operator < (const ZN& m_zn) const
	{
		return id < m_zn.id;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ZN )
};

//邻接馈线类，馈线的集合，一条馈线一定有一个邻接馈线集
//联络开关是只，馈线是父
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  UN //: public CObject
{
public:

	char    id[MAX_SEC_ID_LEN];
	int		idph;//开关的物理编号
//	char    descr[MAX_SEC_ID_LEN];
	char	left[MAX_SEC_ID_LEN];//左侧馈线ID
	char	right[MAX_SEC_ID_LEN];
	char	l_nd[MAX_SEC_ID_LEN];//左节点号
	char	r_nd[MAX_SEC_ID_LEN];
	
	int		q;
	int		topflags;

	int		r_fd;	//馈线集与馈线的关系

	double l_abund_P;//有功裕度,左
	double l_abund_Q;//无功裕度
	double l_abund_V;//最低电压
	
	double r_abund_P;//有功裕度，右
	double r_abund_Q;//无功裕度
	double r_abund_V;//最低电压

    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];//版本号

public:

	//constructors
	UN();
	UN(const char* m_id);

	//assign value operator
	const UN& operator = (const UN& m_UN);

	//compare operator ==
	int operator == (const UN& m_UN) const
	{
		return strcmp(id, m_UN.id) == 0;
	}

	//compare operator <
	int operator < (const UN& m_UN) const
	{
		return strcmp(id, m_UN.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ZN )
};

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  SEC //: public CObject
{
public:

	char	id[MAX_SEC_ID_LEN];
	char    descr[MAX_SEC_DESCR_LEN];
	char    nd[MAX_ND_ID_LEN];
	char    znd[MAX_ND_ID_LEN];
	int		ind;
	int		iznd;
	int		ibs;
	int		zbs;
	float	r1;
	float	x1;
	float   b1;
	float	r0;
	float	x0;
	float   b0;
	int		q;
	int		topflags;
	char	algip[MAX_ANALOG_ID_LEN];
	int		ialgip;
	char	algiq[MAX_ANALOG_ID_LEN];
	int		ialgiq;
	char	algii[500];//临时用来存放节点的首端短路电流
	int		ialgii;
	char	algzp[MAX_ANALOG_ID_LEN];
	int		ialgzp;
	char	algzq[MAX_ANALOG_ID_LEN];
	int		ialgzq;
	char	algzi[500];//临时用来存放节点的末端短路电流
	int		ialgzi;
	float	w;
	float	r;
	float	i;
	float	wz;
	float	rz;
	float	iz;
	int		r_zn;

	float	lossP;//一定损失有功（包括区段内的有功）
	float	transP;//可以转供有功
	int		lossC;//一定损失用户（包括区段内的用户）
	int		transC;//可以转供用户

	int		lC_zn;//区段内用户
	float	lP_zn;//故障段失电负荷(MW)
	int		lC_znnext;//区段内用户
	float	lP_znnext;//非故障段需转移负荷(MW)

	char vl[64];

	float frPerLen;//2.2(电缆，默认为次/年/每千米)  馈线段故障率
	float switchTime;//1.5小时（人工默认值） 故障检测与隔离的开关切换时间，期间上游负荷未供电;
	float transTime;//0.5小时（人工默认值，假设有转供路径可以转供）  非故障段恢复供电（转供） 开关操作时间，期间上游已供
//电，下游可能已永久停电;

	float prob_fault;//故障率
	float repairTime;//5.5小时 故障修复时间;
	
//	float prob_check;//检修停运率
//	float checkTime;//5.5小时 设备检修时间;

	char tranfdlist[MAX_UNIN_ID_SUM];//馈线段发生故障后的转供馈线列表

	//lld add 2014-08-26
	char closeunlist[MAX_UNIN_ID_SUM];//馈线段发生故障后的 闭合 联络开关列表
	char opencblist[MAX_UNIN_ID_SUM];//馈线段发生故障后的 打开 分段开关列表
	
		
//	char transSec[FEMA_ID_LEN];// 下游转供线段
//	char lossSec[FEMA_ID_LEN];//下游损失线段

	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];

	float faultratio;//故障所占的比例

	int fds;

	float v_b;//首节点电压
	float adeg_b;//首节点相角

	float v_e;//末节点电压
	float adeg_e;//末节点相角

	//lld add for chengdu run&monitor system 2015-11-19
	char	st_b[MAX_SEC_ID_LEN];//线路的首端厂站
	char	st_e[MAX_SEC_ID_LEN];//线路的末端厂站

	char	sep_b[MAX_SEC_ID_LEN];//线路的首端测点
	char	sep_e[MAX_SEC_ID_LEN];//线路的末端测点

	float len;
	float	basevoltage;//多等级的电网需要有基准电压等级进行标幺值的归算

public:

	//constructors
	SEC();
	SEC(const char* m_id);

	//assign value operator
	const SEC& operator = (const SEC& m_sec);

	//compare operator ==
	int operator == (const SEC& m_sec) const
	{
		return strcmp(id, m_sec.id) == 0;
	}

	//compare operator <
	int operator < (const SEC& m_sec) const
	{
		return strcmp(id, m_sec.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( SEC )
};	

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  SECP //: 线路端点
{
public:

	char	id[MAX_SEC_ID_LEN];
	char    descr[MAX_SEC_DESCR_LEN];
	char    nd[MAX_ND_ID_LEN];
	char	fdid[MAX_SEC_ID_LEN];//所属线段
	char	st[MAX_SEC_ID_LEN];//所属线段
	float	w;//有功量测
	int		w_flag;//有功量测状态
	float	r;
	int		r_flag;

	int		topflags;
	char	baseVL[MAX_SEC_ID_LEN];//所属线段

public:

	//constructors
	SECP();
	SECP(const char* m_id);

	//assign value operator
	const SECP& operator = (const SECP& m_secp);

	//compare operator ==
	int operator == (const SECP& m_secp) const
	{
		return strcmp(id, m_secp.id) == 0;
	}

	//compare operator <
	int operator < (const SECP& m_secp) const
	{
		return strcmp(id, m_secp.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( SEC )
};	

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ND //: public CObject
{
public:

	char	id[MAX_ND_ID_LEN];
	int		vl;
	int		q;
	int		ibs;
	int     primebs;
	int     fch;//forechain,前驱
	int		bch;//backchain，后继
	int		r_cb;
	int		r_xf;
	int		r_zn;
	int		r_sec;
	int		r_ld;
	int		r_eq;//节点和设备的关系
	//int		r_fd;//节点和馈线的关系
	//节点和电容器的关系，串联电容器，lld add 2019-7-21 
	int		r_cp;

	//DNASP增加
	double	v;//节点电压的幅值
	double	adeg;//节点电压的相角

	//一线一案增加
	double	w;
	double  len;
	char	ldList[MAX_MIANND_LD];//用|隔开的负荷ID号列表，lld add 2018-1-1
	char	UpCBID[MAX_ID_DLL];//节点名称
	char	UnionCBID[MAX_ID_DLL];//节点名称

    char    fdid[MAX_SEC_ID_LEN];//节点所属的馈线
    char    ver[MAX_VER_LEN];//节点的版本号

public:
	
	//constructors
	ND();
	ND(const char* m_id);
	
	//assign value operator
	const ND& operator = (const ND& m_nd);

	//compare operator ==
	int operator == (const ND& m_nd) const
	{
		return strcmp(id, m_nd.id) == 0;
	}

	//compare operator <
	int operator < (const ND& m_nd) const
	{
		return strcmp(id, m_nd.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ND )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  BS //: public CObject
{
public:

	int		id;
	int		iisland;
	int		q;
	int		qq;
	int		fbs;//前向链指针
	int		bbs;//后向链指针
	int		pnd;//头结点号
	float	kv;
	float	v;
	float	adeg;
	float	w;
	float	r;

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];

	double  Qv;//母线上的补偿容量
	char	sc[500];

public:

	//constructors
	BS();
	BS(int m_id);
	
	//assign value operator
	const BS& operator = (const BS& m_bs);

	//compare operator ==
	int operator == (const BS& m_bs) const
	{
		return id == m_bs.id;
	}

	//compare operator <
	int operator < (const BS& m_bs) const
	{
		return id < m_bs.id;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( BS )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ISLAND //: public CObject
{
public:

	int		id;
	int		refbs;
	int		reffd;//岛所关联的是馈线（计算单元），不是电源点
	float	refminv;//岛里面的最低电压
	float	refmaxv;//岛里面的最高电压
	float   wload;
	float   rload;
	float   wso;
	float   rso;

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_VER_LEN];
public:

	//constructors
	ISLAND();
	ISLAND(int m_id);
	
	//assign value operator
	const ISLAND& operator = (const ISLAND& m_island);

	//compare operator ==
	int operator == (const ISLAND& m_island) const
	{
		return id == m_island.id;
	}

	//compare operator <
	int operator < (const ISLAND& m_island) const
	{
		return id < m_island.id;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ISLAND )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  GLOBAL //: public CObject
{
public:

	int		id;			//id号
	int		nullbs;		//空母线数
	int		nextbs;		//
	//time_t  tptime;		//拓扑所需的时间
	//time_t	timedb;
	int		statusdb;	//数据库状态
	int		error;		//错误个数
	int		dberrnum;	//数据库错误个数
	
public:

	//constructors
	GLOBAL();
	GLOBAL(int m_id);
	
	//assign value operator
	const GLOBAL& operator = (const GLOBAL& m_global);

	//compare operator ==
	int operator == (const GLOBAL& m_global) const
	{
		return id == m_global.id;
	}

	//compare operator <
	int operator < (const GLOBAL& m_global) const
	{
		return id < m_global.id;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( GLOBAL )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  TAPTY //: public CObject
{
public:

	char	id[MAX_TAPTY_ID_LEN];
	int		mn;
	int		mx;
	int		nom;
	float	step;

public:

	//constructors
	TAPTY();
	TAPTY(const char* m_id);
	
	//assign value operator
	const TAPTY& operator = (const TAPTY& m_tapty);

	//compare operator ==
	int operator == (const TAPTY& m_tapty) const
	{
		return strcmp(id, m_tapty.id) == 0;
	}

	//compare operator <
	int operator < (const TAPTY& m_tapty) const
	{
		return strcmp(id, m_tapty.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( TAPTY )
};

//公司和电站之间的关系表
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_CO_ST //: public CObject
{
public:

	char	co_id[MAX_CO_ID_LEN];	//公司的ID号
	char	st_id[MAX_ST_ID_LEN];	//变电站的ID号
	int		co_id_ptr;				//公司ID号的指针
	int		st_id_ptr;				//变电站ID号的指针
	int		fptr;					//前向指针
	int		bptr;					//后向指针	

public:

	//constructors
	R_CO_ST();
	R_CO_ST(const char* m_co_id, const char* m_st_id);

	//assign value operator
	const R_CO_ST& operator = (const R_CO_ST& m_r_co_st);

	//compare operator ==
	int operator == (const R_CO_ST& m_r_co_st) const
	{
		return strcmp(st_id, m_r_co_st.st_id) == 0 && 
			   strcmp(co_id, m_r_co_st.co_id) == 0;
	}

	//compare operator <
	int operator < (const R_CO_ST& m_r_co_st) const
	{
		int		cmpval;

		cmpval = strcmp(co_id, m_r_co_st.co_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(st_id, m_r_co_st.st_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return co_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return st_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return co_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return st_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_CO_ST )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_XFMR //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	xfmr_id[MAX_XFMR_ID_LEN];
	int		st_id_ptr;
	int		xfmr_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_XFMR();
	R_ST_XFMR(const char* m_st_id, const char* m_xfmr_id);

	//assign value operator
	const R_ST_XFMR& operator = (const R_ST_XFMR& m_r_st_xfmr);

	//compare operator ==
	int operator == (const R_ST_XFMR& m_r_st_xfmr) const
	{
		return strcmp(xfmr_id, m_r_st_xfmr.xfmr_id) == 0 && 
			   strcmp(st_id, m_r_st_xfmr.st_id) == 0;
	}
	
	//compare operator <
	int operator < (const R_ST_XFMR& m_r_st_xfmr) const
	{
		int		cmpval;

		cmpval = strcmp(st_id, m_r_st_xfmr.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(xfmr_id, m_r_st_xfmr.xfmr_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return xfmr_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return xfmr_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_XFMR )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_XFMR_XF //: public CObject
{
public:
	long	id;

	char	xfmr_id[MAX_XFMR_ID_LEN];
	char	xf_id[MAX_XF_ID_LEN];
	int		xfmr_id_ptr;
	int		xf_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_XFMR_XF();
	R_XFMR_XF(const char* m_xfmr_id, const char* m_xf_id);

	//assign value operator
	const R_XFMR_XF& operator = (const R_XFMR_XF& m_r_xfmr_xf);

	//compare operator ==
	int operator == (const R_XFMR_XF& m_r_xfmr_xf) const
	{
		return id == m_r_xfmr_xf.id;
//		return strcmp(xf_id, m_r_xfmr_xf.xf_id) == 0 && 
//			   strcmp(xfmr_id, m_r_xfmr_xf.xfmr_id) == 0;
	}
	
	//compare operator <
	int operator < (const R_XFMR_XF& m_r_xfmr_xf) const
	{
		return id < m_r_xfmr_xf.id;

//		int		cmpval;
//
//		cmpval = strcmp(xfmr_id, m_r_xfmr_xf.xfmr_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(xf_id, m_r_xfmr_xf.xf_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return xfmr_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return xf_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return xfmr_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return xf_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_XFMR_XF )
};



//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_CB //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	cb_id[MAX_CB_ID_LEN];
	int		st_id_ptr;
	int		cb_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_CB();
	R_ST_CB(const char* m_st_id, const char* m_cb_id);

	//assign value operator
	const R_ST_CB& operator = (const R_ST_CB& m_r_st_cb);

	//compare operator ==
	int operator == (const R_ST_CB& m_r_st_cb) const
	{
		return strcmp(cb_id, m_r_st_cb.cb_id) == 0 && 
			   strcmp(st_id, m_r_st_cb.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_CB& m_r_st_cb) const
	{
		int		cmpval;

		cmpval = strcmp(st_id, m_r_st_cb.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(cb_id, m_r_st_cb.cb_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cb_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cb_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_CB )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_SO //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	so_id[MAX_SO_ID_LEN];
	int		st_id_ptr;
	int		so_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_SO();
	R_ST_SO(const char* m_st_id, const char* m_so_id);

	//assign value operator
	const R_ST_SO& operator = (const R_ST_SO& m_r_st_so);

	//compare operator ==
	int operator == (const R_ST_SO& m_r_st_so) const
	{
		return strcmp(so_id, m_r_st_so.so_id) == 0 && 
			   strcmp(st_id, m_r_st_so.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_SO& m_r_st_so) const
	{
		int		cmpval;

		cmpval = strcmp(st_id, m_r_st_so.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(so_id, m_r_st_so.so_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return so_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return so_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_SO )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_LD //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	ld_id[MAX_LD_ID_LEN];
	int		st_id_ptr;
	int		ld_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_LD();
	R_ST_LD(const char* m_st_id, const char* m_ld_id);

	//assign value operator
	const R_ST_LD& operator = (const R_ST_LD& m_r_st_ld);

	//compare operator ==
	int operator == (const R_ST_LD& m_r_st_ld) const
	{
		return strcmp(ld_id, m_r_st_ld.ld_id) == 0 && 
			   strcmp(st_id, m_r_st_ld.st_id) == 0;
	}

	//compare operator < 
	int operator < (const R_ST_LD& m_r_st_ld) const
	{
		int		cmpval;

		cmpval = strcmp(st_id, m_r_st_ld.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(ld_id, m_r_st_ld.ld_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return ld_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return ld_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_LD )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_CP //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	cp_id[MAX_CP_ID_LEN];
	int		st_id_ptr;
	int		cp_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_CP();
	R_ST_CP(const char* m_st_id, const char* m_cp_id);

	//assign value operator
	const R_ST_CP& operator = (const R_ST_CP& m_r_st_cp);

	//compare operator ==
	int operator == (const R_ST_CP& m_r_st_cp) const
	{
		return strcmp(cp_id, m_r_st_cp.cp_id) == 0 && 
			   strcmp(st_id, m_r_st_cp.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_CP& m_r_st_cp) const
	{
		int cmpval;

		cmpval = strcmp(st_id, m_r_st_cp.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(cp_id, m_r_st_cp.cp_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cp_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cp_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_CP )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_BUS //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	bus_id[MAX_BUS_ID_LEN];
	int		st_id_ptr;
	int		bus_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_BUS();
	R_ST_BUS(const char* m_st_id, const char* m_bus_id);

	//assign value operator
	const R_ST_BUS& operator = (const R_ST_BUS& m_r_st_bus);

	//compare operator ==
	int operator == (const R_ST_BUS& m_r_st_bus) const
	{
		return strcmp(bus_id, m_r_st_bus.bus_id) == 0 && 
			   strcmp(st_id, m_r_st_bus.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_BUS& m_r_st_bus) const
	{
		int		cmpval;
		
		cmpval = strcmp(st_id, m_r_st_bus.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(bus_id, m_r_st_bus.bus_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return bus_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return bus_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_BUS )
};

/*
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_ZN //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	zn_id[MAX_ZN_ID_LEN];
	int		st_id_ptr;
	int		zn_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_ZN();
	R_ST_ZN(const char* m_st_id, const char* m_zn_id);

	//assign value operator
	const R_ST_ZN& operator = (const R_ST_ZN& m_r_st_zn);

	//compare operator ==
	int operator == (const R_ST_ZN& m_r_st_zn) const
	{
		return strcmp(zn_id, m_r_st_zn.zn_id) == 0 && 
			   strcmp(st_id, m_r_st_zn.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_ZN& m_r_st_zn) const
	{
		int		cmpval;

		//先判断厂站ID是否一样
		cmpval = strcmp(st_id, m_r_st_zn.st_id);
		if(cmpval == 0)
		{	
			//再判断区段ID是否一样
			cmpval = strcmp(zn_id, m_r_st_zn.zn_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return zn_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return zn_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_ZN )
};
*/


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ZN_LD //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	ld_id[MAX_LD_ID_LEN];
	int		zn_id_ptr;
	int		ld_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关

public:

	//constructors
	R_ZN_LD();
	R_ZN_LD(const char* m_zn_id, const char* m_ld_id);

	//assign value operator
	const R_ZN_LD& operator = (const R_ZN_LD& m_r_zn_ld);

	//compare operator ==
	int operator == (const R_ZN_LD& m_r_zn_ld) const
	{
		return id == m_r_zn_ld.id;
//		return strcmp(ld_id, m_r_zn_ld.ld_id) == 0 && 
//			   strcmp(zn_id, m_r_zn_ld.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_LD& m_r_zn_ld) const
	{
		return id < m_r_zn_ld.id;
//		int		cmpval;
//
//		cmpval = strcmp(zn_id, m_r_zn_ld.zn_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(ld_id, m_r_zn_ld.ld_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return ld_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return ld_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ZN_LD )
};

class  R_ZN_EQ //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	eq_id[MAX_LD_ID_LEN];
	int		zn_id_ptr;
	int		eq_id_ptr;
	int		fptr;
	int		bptr;
public:

	//constructors
	R_ZN_EQ();
	R_ZN_EQ(const char* m_zn_id, const char* m_eq_id);

	//assign value operator
	const R_ZN_EQ& operator = (const R_ZN_EQ& m_r_zn_eq);

	//compare operator ==
	int operator == (const R_ZN_EQ& m_r_zn_eq) const
	{
		return id == m_r_zn_eq.id;
	}

	//compare operator <
	int operator < (const R_ZN_EQ& m_r_zn_eq) const
	{
		return id < m_r_zn_eq.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get chieq id
	const char* GetChildId() const
	{
		return eq_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get chieq ptr
	int GetChildPtr() const
	{
		return eq_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ZN_EQ )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ZN_SEC //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	sec_id[MAX_SEC_ID_LEN];
	int		zn_id_ptr;
	int		sec_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关

public:

	//constructors
	R_ZN_SEC();
	R_ZN_SEC(const char* m_zn_id, const char* m_sec_id);

	//assign value operator
	const R_ZN_SEC& operator = (const R_ZN_SEC& m_r_zn_sec);

	//compare operator ==
	int operator == (const R_ZN_SEC& m_r_zn_sec) const
	{
		return id == m_r_zn_sec.id;
//		return strcmp(sec_id, m_r_zn_sec.sec_id) == 0 && 
//			   strcmp(zn_id, m_r_zn_sec.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_SEC& m_r_zn_sec) const
	{
		return id < m_r_zn_sec.id;
//		int		cmpval;
//
//		cmpval = strcmp(zn_id, m_r_zn_sec.zn_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(sec_id, m_r_zn_sec.sec_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return sec_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return sec_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ZN_SEC )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ZN_CP //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	cp_id[MAX_CP_ID_LEN];
	int		zn_id_ptr;
	int		cp_id_ptr;
	int		fptr;
	int		bptr;
	char    ver[MAX_VER_LEN];
	char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关

public:

	//constructors
	R_ZN_CP();
	R_ZN_CP(const char* m_zn_id, const char* m_cp_id);

	//assign value operator
	const R_ZN_CP& operator = (const R_ZN_CP& m_r_zn_cp);

	//compare operator ==
	int operator == (const R_ZN_CP& m_r_zn_cp) const
	{
		return id == m_r_zn_cp.id;
		//		return strcmp(cp_id, m_r_zn_cp.cp_id) == 0 && 
		//			   strcmp(zn_id, m_r_zn_cp.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_CP& m_r_zn_cp) const
	{
		return id < m_r_zn_cp.id;
		//		int		cmpval;
		//
		//		cmpval = strcmp(zn_id, m_r_zn_cp.zn_id);
		//		if(cmpval == 0)
		//		{
		//			cmpval = strcmp(cp_id, m_r_zn_cp.cp_id);
		//		}
		//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cp_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cp_id_ptr;
	}

	//read or write object
	//void Serialize( CArchive& ar );

protected:

	//DECLARE_SERIAL( R_ZN_cp )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ZN_XF //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	xf_id[MAX_XF_ID_LEN];
	int		zn_id_ptr;
	int		xf_id_ptr;
	int		fptr;
	int		bptr;
	char    ver[MAX_VER_LEN];
	char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关

public:

	//constructors
	R_ZN_XF();
	R_ZN_XF(const char* m_zn_id, const char* m_xf_id);

	//assign value operator
	const R_ZN_XF& operator = (const R_ZN_XF& m_r_zn_xf);

	//compare operator ==
	int operator == (const R_ZN_XF& m_r_zn_xf) const
	{
		return id == m_r_zn_xf.id;
		//		return strcmp(xf_id, m_r_zn_xf.xf_id) == 0 && 
		//			   strcmp(zn_id, m_r_zn_xf.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_XF& m_r_zn_xf) const
	{
		return id < m_r_zn_xf.id;
		//		int		cmpval;
		//
		//		cmpval = strcmp(zn_id, m_r_zn_xf.zn_id);
		//		if(cmpval == 0)
		//		{
		//			cmpval = strcmp(xf_id, m_r_zn_xf.xf_id);
		//		}
		//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return xf_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return xf_id_ptr;
	}

	//read or write object
	//void Serialize( CArchive& ar );

protected:

	//DECLARE_SERIAL( R_ZN_xf )
};

//通过区段-边界开关表得到相关的边界开关
class  R_ZN_CB //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	cb_id[MAX_SEC_ID_LEN];
	int		zn_id_ptr;
	int		cb_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关

public:

	//constructors
	R_ZN_CB();
	R_ZN_CB(const char* m_zn_id, const char* m_cb_id);

	//assign value operator
	const R_ZN_CB& operator = (const R_ZN_CB& m_r_zn_cb);

	//compare operator ==
	int operator == (const R_ZN_CB& m_r_zn_cb) const
	{
		return id == m_r_zn_cb.id;
//		return strcmp(cb_id, m_r_zn_cb.cb_id) == 0 && 
//			   strcmp(zn_id, m_r_zn_cb.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_CB& m_r_zn_cb) const
	{
		return id < m_r_zn_cb.id;

//		int		cmpval;
//
//		cmpval = strcmp(zn_id, m_r_zn_cb.zn_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(cb_id, m_r_zn_cb.cb_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cb_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cb_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ZN_CB )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_LD //: public CObject
{
public:
	long	id;
	
	char	nd_id[MAX_ND_ID_LEN];
	char	ld_id[MAX_LD_ID_LEN];
	int		nd_id_ptr;
	int		ld_id_ptr;
	int		fptr;
	int		bptr;

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关
	
public:
	
	//constructors
	R_ND_LD();
	R_ND_LD(const char* m_nd_id, const char* m_ld_id);
	
	//assign value operator
	const R_ND_LD& operator = (const R_ND_LD& m_r_nd_ld);
	
	//compare operator ==
	int operator == (const R_ND_LD& m_r_nd_ld) const
	{
		return id == m_r_nd_ld.id;
//		return strcmp(ld_id, m_r_nd_ld.ld_id) == 0 && 
//			strcmp(nd_id, m_r_nd_ld.nd_id) == 0;
	}
	
	//compare operator <
	int operator < (const R_ND_LD& m_r_nd_ld) const
	{
		return id < m_r_nd_ld.id;
//		int		cmpval;
//		
//		cmpval = strcmp(nd_id, m_r_nd_ld.nd_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(ld_id, m_r_nd_ld.ld_id);
//		}
//		return cmpval < 0;
	}
	
	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}
	
	//get child id
	const char* GetChildId() const
	{
		return ld_id;
	}
	
	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}
	
	//get child ptr
	int GetChildPtr() const
	{
		return ld_id_ptr;
	}
	
	//read or write object
    //void Serialize( CArchive& ar );
	
protected:
	
	//DECLARE_SERIAL( R_ND_CB )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_CB //: public CObject
{
public:
	long	id;

	char	nd_id[MAX_ND_ID_LEN];
	char	cb_id[MAX_CB_ID_LEN];
	int		nd_id_ptr;
	int		cb_id_ptr;
	int		fptr;
	int		bptr;

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关
public:

	//constructors
	R_ND_CB();
	R_ND_CB(const char* m_nd_id, const char* m_cb_id);

	//assign value operator
	const R_ND_CB& operator = (const R_ND_CB& m_r_nd_cb);

	//compare operator ==
	int operator == (const R_ND_CB& m_r_nd_cb) const
	{
		return id == m_r_nd_cb.id;
//		return strcmp(cb_id, m_r_nd_cb.cb_id) == 0 && 
//			   strcmp(nd_id, m_r_nd_cb.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_CB& m_r_nd_cb) const
	{
		return id < m_r_nd_cb.id;
//		int		cmpval;
//
//		cmpval = strcmp(nd_id, m_r_nd_cb.nd_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(cb_id, m_r_nd_cb.cb_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cb_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cb_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_CB )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_XF //: public CObject
{
public:
	long	id;

	char	nd_id[MAX_ND_ID_LEN];
	char	xf_id[MAX_XF_ID_LEN];
	int		nd_id_ptr;
	int		xf_id_ptr;
	int		fptr;
	int		bptr;

	char	ver[MAX_VER_LEN];
	char	fdid[MAX_FDID_LEN];

public:

	//constructors
	R_ND_XF();
	R_ND_XF(const char* m_nd_id, const char* m_xf_id);

	//assign value operator
	const R_ND_XF& operator = (const R_ND_XF& m_r_nd_xf);

	//compare operator ==
	int operator == (const R_ND_XF& m_r_nd_xf) const
	{
		return id == m_r_nd_xf.id;
//		return strcmp(xf_id, m_r_nd_xf.xf_id) == 0 && 
//			   strcmp(nd_id, m_r_nd_xf.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_XF& m_r_nd_xf) const
	{
		return id < m_r_nd_xf.id;

//		int		cmpval;
//
//		cmpval = strcmp(nd_id, m_r_nd_xf.nd_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(xf_id, m_r_nd_xf.xf_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return xf_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return xf_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_XF )
};

/*

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_ZN //: public CObject
{
public:

	char	nd_id[MAX_ND_ID_LEN];
	char	zn_id[MAX_ZN_ID_LEN];
	int		nd_id_ptr;
	int		zn_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ND_ZN();
	R_ND_ZN(const char* m_nd_id, const char* m_zn_id);

	//assign value operator
	const R_ND_ZN& operator = (const R_ND_ZN& m_r_nd_zn);

	//compare operator ==
	int operator == (const R_ND_ZN& m_r_nd_zn) const
	{
		return strcmp(zn_id, m_r_nd_zn.zn_id) == 0 && 
			   strcmp(nd_id, m_r_nd_zn.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_ZN& m_r_nd_zn) const
	{
		int		cmpval;

		cmpval = strcmp(nd_id, m_r_nd_zn.nd_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(zn_id, m_r_nd_zn.zn_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return zn_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return zn_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_ZN )
};

*/

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_SEC //: public CObject
{
public:
	long	id;

	char	nd_id[MAX_ND_ID_LEN];
	char	sec_id[MAX_SEC_ID_LEN];
	int		nd_id_ptr;
	int		sec_id_ptr;
	int		fptr;//同样的父亲出现的前一个位置
	int		bptr;//同样的父亲出现的后一个位置，通过这个链表可以快速定位到父亲的所有儿子

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关

public:

	//constructors
	R_ND_SEC();
	R_ND_SEC(const char* m_nd_id, const char* m_sec_id);

	//assign value operator
	const R_ND_SEC& operator = (const R_ND_SEC& m_r_nd_sec);

	//compare operator ==
	int operator == (const R_ND_SEC& m_r_nd_sec) const
	{
		return id == m_r_nd_sec.id;
//		return strcmp(sec_id, m_r_nd_sec.sec_id) == 0 && 
//			   strcmp(nd_id, m_r_nd_sec.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_SEC& m_r_nd_sec) const
	{
		return id < m_r_nd_sec.id;

//		int		cmpval;
//
//		cmpval = strcmp(nd_id, m_r_nd_sec.nd_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(sec_id, m_r_nd_sec.sec_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return sec_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return sec_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_SEC )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_CP //: public CObject
{
public:
	long	id;

	char	nd_id[MAX_ND_ID_LEN];
	char	cp_id[MAX_CP_ID_LEN];
	int		nd_id_ptr;
	int		cp_id_ptr;
	int		fptr;//同样的父亲出现的前一个位置
	int		bptr;//同样的父亲出现的后一个位置，通过这个链表可以快速定位到父亲的所有儿子

	char    ver[MAX_VER_LEN];
	char    fdid[MAX_FDID_LEN];//通过这个可以得到联络开关，如果一个人开关属于2条馈线，一定是联络开关

public:

	//constructors
	R_ND_CP();
	R_ND_CP(const char* m_nd_id, const char* m_cp_id);

	//assign value operator
	const R_ND_CP& operator = (const R_ND_CP& m_r_nd_cp);

	//compare operator ==
	int operator == (const R_ND_CP& m_r_nd_cp) const
	{
		return id == m_r_nd_cp.id;
		//		return strcmp(cp_id, m_r_nd_cp.cp_id) == 0 && 
		//			   strcmp(nd_id, m_r_nd_cp.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_CP& m_r_nd_cp) const
	{
		return id < m_r_nd_cp.id;

		//		int		cmpval;
		//
		//		cmpval = strcmp(nd_id, m_r_nd_cp.nd_id);
		//		if(cmpval == 0)
		//		{
		//			cmpval = strcmp(cp_id, m_r_nd_cp.cp_id);
		//		}
		//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cp_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cp_id_ptr;
	}

	//read or write object
	//void Serialize( CArchive& ar );

protected:

	//DECLARE_SERIAL( R_ND_cp )
};



//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_BS_SEC //: public CObject
{
public:
	long	id;

	char	bs_id[MAX_ND_ID_LEN];
	char	sec_id[MAX_SEC_ID_LEN];
	int		bs_id_ptr;
	int		sec_id_ptr;
	int		fptr;//同样的父亲出现的前一个位置
	int		bptr;//同样的父亲出现的后一个位置，通过这个链表可以快速定位到父亲的所有儿子

public:

	//constructors
	R_BS_SEC();
	R_BS_SEC(const char* m_bs_id, const char* m_sec_id);

	//assign value operator
	const R_BS_SEC& operator = (const R_BS_SEC& m_r_bs_sec);

	//compare operator ==
	int operator == (const R_BS_SEC& m_r_bs_sec) const
	{
		return id == m_r_bs_sec.id;
	}

	//compare operator <
	int operator < (const R_BS_SEC& m_r_bs_sec) const
	{
		return id < m_r_bs_sec.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return bs_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return sec_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return bs_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return sec_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_bs_SEC )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_UN //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	char	un_id[MAX_CB_ID_LEN];
	int		fd_id_ptr;
	int		un_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_UN();
	R_FD_UN(const char* m_fd_id, const char* m_un_id);

	//assign value operator
	const R_FD_UN& operator = (const R_FD_UN& m_R_FD_UN);

	//compare operator ==
	int operator == (const R_FD_UN& m_R_FD_UN) const
	{
		return id == m_R_FD_UN.id;
//		return strcmp(sec_id, m_R_FD_UN.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_UN.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_UN& m_R_FD_UN) const
	{
		return id < m_R_FD_UN.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get chiun id
	const char* GetChildId() const
	{
		return un_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get chiun ptr
	int GetChildPtr() const
	{
		return un_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_UN )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_LD //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	char	ld_id[MAX_CB_ID_LEN];
	int		fd_id_ptr;
	int		ld_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_LD();
	R_FD_LD(const char* m_fd_id, const char* m_ld_id);

	//assign value operator
	const R_FD_LD& operator = (const R_FD_LD& m_R_FD_LD);

	//compare operator ==
	int operator == (const R_FD_LD& m_R_FD_LD) const
	{
		return id == m_R_FD_LD.id;
//		return strcmp(sec_id, m_R_FD_LD.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_LD.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_LD& m_R_FD_LD) const
	{
		return id < m_R_FD_LD.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return ld_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return ld_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_LD )
};

class  R_FD_ND //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	char	nd_id[MAX_CB_ID_LEN];
	int		fd_id_ptr;
	int		nd_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_ND();
	R_FD_ND(const char* m_fd_id, const char* m_nd_id);

	//assign value operator
	const R_FD_ND& operator = (const R_FD_ND& m_R_FD_ND);

	//compare operator ==
	int operator == (const R_FD_ND& m_R_FD_ND) const
	{
		return id == m_R_FD_ND.id;
//		return strcmp(sec_id, m_R_FD_ND.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_ND.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_ND& m_R_FD_ND) const
	{
		return id < m_R_FD_ND.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get chiND id
	const char* GetChildId() const
	{
		return nd_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get chiND ptr
	int GetChildPtr() const
	{
		return nd_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_ND )
};
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_EQ //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	int		eq_id;
	int		fd_id_ptr;
	int		eq_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_EQ();
	R_FD_EQ(const char* m_fd_id, int m_eq_id);

	//assign value operator
	const R_FD_EQ& operator = (const R_FD_EQ& m_R_FD_EQ);

	//compare operator ==
	int operator == (const R_FD_EQ& m_R_FD_EQ) const
	{
		return id == m_R_FD_EQ.id;
//		return strcmp(sec_id, m_R_FD_EQ.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_EQ.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_EQ& m_R_FD_EQ) const
	{
		return id < m_R_FD_EQ.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get child id
	const int GetChildId() const
	{
		return eq_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return eq_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_EQ )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_ZN //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	int		zn_id;
	int		fd_id_ptr;
	int		zn_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_ZN();
	R_FD_ZN(const char* m_fd_id, const int m_zn_id);

	//assign value operator
	const R_FD_ZN& operator = (const R_FD_ZN& m_R_FD_ZN);

	//compare operator ==
	int operator == (const R_FD_ZN& m_R_FD_ZN) const
	{
		return id == m_R_FD_ZN.id;
//		return strcmp(sec_id, m_R_FD_ZN.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_ZN.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_ZN& m_R_FD_ZN) const
	{
		return id < m_R_FD_ZN.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get child id
	const int GetChildId() const
	{
		return zn_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return zn_id_ptr;
	}

	//read or write object
    ////void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_ZN )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_SO //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_ZN_ID_LEN];
	char	so_id[MAX_SEC_ID_LEN];
	int		fd_id_ptr;
	int		so_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_SO();
	R_FD_SO(const char* m_fd_id, const char* m_so_id);

	//assign value operator
	const R_FD_SO& operator = (const R_FD_SO& m_R_FD_SO);

	//compare operator ==
	int operator == (const R_FD_SO& m_R_FD_SO) const
	{
		return id == m_R_FD_SO.id;
//		return strcmp(sec_id, m_R_FD_SO.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_SO.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_SO& m_R_FD_SO) const
	{
		return id < m_R_FD_SO.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return so_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return so_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_SO )
};

#endif