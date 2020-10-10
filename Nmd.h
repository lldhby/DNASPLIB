////////////////////////////////////////////////////////////////////////////////
//
//  NMD.H
////////////////////////////////////////////////////////////////////////////////
#if !defined(__NMD_H)
#define __NMD_H

#if _WIN32
#elif __linux__
#else
#endif

#include "../DNASPLIB/dsbob2.h"
#include "../DNASPLIB/LLD.h"

#define ST_ABB_LEN		5   //变电站的前缀99999

#define NMD_FAIL		0
#define NMD_SUCCEED		1

//#include <afxtempl.h>

////////////////////////////////////////////////////////////////////////////////
//  NMD
class  NMD
{
public:

	//constructor and destructor
	NMD(DSBOB2* m_pDb,CLLD* m_pLLD);
	~NMD();
	CLLD* lld;
	//execute validation
	//type-0 表示要把形成区段后的后缀删掉，1-表示不删，准备用来判断是否有环网
	//0-用来对规划网处理，1-用来对实时网进行处理。规划网的实测比较少
	int DoValidate();

public:	
	//int resetLDandSecID();
	int R_ZN_CB_RebuildPtr();
	
	//start validation
	int	ValidateStart();

	//end validation
	int	ValidateEnd();

	//validate st table

	//validate zn table
	int	ZN_Validate();

	//validate cb table
	int	CB_Validate();

	//validate bus table
	int BUS_Validate();

	//validate cp table
	int	CP_Validate();

	//validate ld table
	int	LD_Validate();

	//validate so table
	int	SO_Validate();

	//validate sec table
	int SEC_Validate();

	//validate xfmr table
	int	XFMR_Validate();

	//validate xf table
	int	XF_Validate();

	//lld edit 2016-1-4
	int	FD_Validate();

	//add by lld 2018-4-25
	int	TrimTable();

	//lld edit for multi-thread 2016-1-12
	int Insert_r_char_int(int type,int eq_ptr,int nd_ptr,char* nd_id);
	//int	RA_Validate();
	
	int ND_Insert(const char* nd_id, int* nd_p, int mask, const char* fdid, const char* ver);

	//insert r_nd_br table
	int ND_BR_Insert(int type, int nd_p, const char* nd_id, int ibr, const char* br_id, const char* fdid, const char* ver);

	//reset pointers in tables
	int ResetPtr();

	//reset pointers in co table
	int CO_ResetPtr();	

	//reset pointers in st table
	int	ST_ResetPtr();

	//reset pointers in xfmr table
	int XFMR_ResetPtr();	

	//reset pointers in fd table
	int ZN_ResetPtr();

	//reset pointers in cb table
	int CB_ResetPtr();	

	//reset pointers in so table
	int SO_ResetPtr();	

	//reset pointers in ld table
	int LD_ResetPtr();	

	//reset pointers in cp table
	//int CP_ResetPtr();	

	//reset pointers in bus table
	int BUS_ResetPtr();	

	//reset pointers in set table
	int SEC_ResetPtr();

	//build r_co_st table
	int R_CO_ST_Build();

	//build r_st_xfmr table
	int	R_ST_XFMR_Build();

	//build r_st_cb table
	int R_ST_CB_Build();

	//build r_st_so table
	int R_ST_SO_Build();

	//build r_st_ld table
	int R_ST_LD_Build();

	//build r_st_cp table
	int R_ST_CP_Build();

	//build r_st_bus table
	int R_ST_BUS_Build();

	//build r_st_zn table

	//build r_zn_ld table
	//int R_ZN_LD_Build();

	//build r_zn_sec table
	//int R_ZN_SEC_Build();

	//rebuild pointers in tables
	int RebuildPtr();

	//rebuild pointers in co table
	int CO_RebuildPtr();	

	//rebuild pointers in st table
	int ST_RebuildPtr();

	//rebuild pointers in xfmr table
	int XFMR_RebuildPtr();	

	//rebuild pointers in xf table
	int XF_RebuildPtr();	

	//rebuild pointers in cb table
	int CB_RebuildPtr();	

	//rebuild pointers in un table
	int SO_RebuildPtr();	

	//rebuild pointers in ld table
	int LD_RebuildPtr();	

	//rebuild pointers in cp table
	int CP_RebuildPtr();	

	//rebuild pointers in bus table
	int BUS_RebuildPtr();	

	//rebuild pointers in zn table
	int ZN_RebuildPtr();	

	//rebuild pointers in sec table
	int SEC_RebuildPtr();

	//rebuild pointers in nd table
	int ND_RebuildPtr();	

	//rebuild pointers in r_co_st table
	int R_CO_ST_RebuildPtr();

	//rebuild pointers in r_st_xfmr table
	int R_ST_XFMR_RebuildPtr();

	//rebuild pointers in r_xfmr_xf table
	int R_XFMR_XF_RebuildPtr();

	//rebuild pointers in r_st_cb table
	int R_ST_CB_RebuildPtr();

	//rebuild pointers in r_st_so table
	int R_ST_SO_RebuildPtr();

	//rebuild pointers in r_st_ld table
	int R_ST_LD_RebuildPtr();

	//rebuild pointers in r_st_cp table
	int R_ST_CP_RebuildPtr();

	//rebuild pointers in r_st_bus table
	int R_ST_BUS_RebuildPtr();

	//rebuild pointers in r_st_zn table

	//rebuild pointers in r_zn_ld table
	int R_ZN_LD_RebuildPtr();

	//rebuild pointers in r_zn_sec table
	int R_ZN_SEC_RebuildPtr();

	int R_ZN_CP_RebuildPtr();
	int R_ND_LD_RebuildPtr();
	
	//rebuild pointers in r_nd_cb table
	int R_ND_CB_RebuildPtr();

	//rebuild pointers in r_nd_xf table
	int R_ND_XF_RebuildPtr();

	//rebuild pointers in r_nd_zn table

	//rebuild pointers in r_nd_sec table
	int R_ND_SEC_RebuildPtr();
	int R_ND_CP_RebuildPtr();

	//build pointers to digital and ananlog table
	int	DIGITAL_ANALOG_PtrBuild();

	//set vl of nds
	int SetVL();
	int SetVL(int lld);//全部设成10kV，lld add 2012.04.09

	//通过一个给定的节点找到馈线ID，lld add 2018-4-28
	int OnGetFDByND(char* fdid,int ind);

	//谐波追踪，lld add 2020-4-23
	//结果放到secs[int]
	int traceHW(char* hwSub, vector<int>& secs);
 
private:
	//database object to be validated
	DSBOB2*				dsbob;
//实体读入表
	DTable<GLOBAL>*           global_tb;

	//DAS用
	DTable<ANALOG>*           analog_tb;
	DTable<DIGITAL>*          digital_tb;

	//计算调用前需要填满
	DTable<CO>*               co_tb;
	DTable<ST>*               st_tb;
	DTable<SO>*               so_tb;
	DTable<BUS>*              bus_tb;
	DTable<CB>*               cb_tb;
	DTable<SEC>*              sec_tb;
	DTable<LD>*               ld_tb;
	DTable<XFMR>*             xfmr_tb;
	DTable<TAPTY>*            tapty_tb;
	DTable<CP>*               cp_tb;
	DTable<ND>*               nd_tb;

	//在dsbob里面生成，nmd只是引用
	DTable<UN>*				  un_tb;//联络开关表
	DTable<FD>*               fd_tb;//馈线表

//由实体表生成的表
	DTable<R_ND_LD>*          r_nd_ld_tb;
	DTable<R_ND_CB>*          r_nd_cb_tb;
	DTable<R_ND_SEC>*         r_nd_sec_tb;
	DTable<R_ND_XF>*          r_nd_xf_tb;
	DTable<R_ND_EQ>*		  r_nd_eq_tb;//设备与节点的对应关系
	DTable<R_ND_CP>*		  r_nd_cp_tb;//设备与节点的对应关系

	DTable<ZN>*               zn_tb;
	DTable<R_ZN_LD>*          r_zn_ld_tb;
	DTable<R_ZN_SEC>*         r_zn_sec_tb;
	DTable<R_ZN_CB>*          r_zn_cb_tb;
	DTable<R_ZN_CP>*          r_zn_cp_tb;
	DTable<R_ZN_XF>*          r_zn_xf_tb;

	//lld add 2015-12-31
	DTable<R_FD_UN>*          r_fd_un_tb;//馈线联络
	DTable<R_FD_SO>*          r_fd_so_tb;
	DTable<R_FD_ZN>*		  r_fd_zn_tb;
	DTable<R_FD_EQ>*		  r_fd_eq_tb;
	DTable<R_FD_LD>*		  r_fd_ld_tb;

	//lld add 2018-5-14
	DTable<XF>*               xf_tb;
	DTable<R_XFMR_XF>*        r_xfmr_xf_tb;

	//lld add 2016-1-12
	DTable<EQ>*				  eq_tb;//全部的可靠性计算设备
	DTable<XF>*               xf_tb_t;//先临时，再正式
	DTable<R_XFMR_XF>*        r_xfmr_xf_tb_t;

//所有的关系表，含节点表都要在这里生成，做完后，反写到dsbob里面
	DTable<R_ND_LD>*	r_nd_ld_tb_t;
	DTable<R_ND_CB>*	r_nd_cb_tb_t;
	DTable<R_ND_XF>*	r_nd_xf_tb_t;
	DTable<R_ND_SEC>*	r_nd_sec_tb_t;
	DTable<R_ND_EQ>*	r_nd_eq_tb_t;
	DTable<R_ND_CP>*	r_nd_cp_tb_t;

	DTable<ZN>*			zn_tb_t;	
	DTable<R_ZN_LD>*	r_zn_ld_tb_t;
	DTable<R_ZN_SEC>*	r_zn_sec_tb_t;
	DTable<R_ZN_CP>*	r_zn_cp_tb_t;
	DTable<R_ZN_XF>*	r_zn_xf_tb_t;
	DTable<R_ZN_CB>*	r_zn_cb_tb_t;

	DTable<R_FD_UN>*	r_fd_un_tb_t;
	DTable<R_FD_SO>*	r_fd_so_tb_t;
	DTable<R_FD_LD>*	r_fd_ld_tb_t;	
	DTable<R_FD_ZN>*	r_fd_zn_tb_t;
	DTable<R_FD_EQ>*	r_fd_eq_tb_t;

	//需要临时给定区段表，做完后及时清除，减少内存占用
	DTable<EQ>*			eq_tb_t;

	//lld add 2019-6-3 for highvoltage distribution network
	DTable<R_CO_ST>*	r_co_st_tb;
	DTable<R_ST_XFMR>*	r_st_xfmr_tb;
	DTable<R_ST_CB>*	r_st_cb_tb;
	DTable<R_ST_SO>*	r_st_so_tb;
	DTable<R_ST_LD>*	r_st_ld_tb;
	DTable<R_ST_CP>*	r_st_cp_tb;
	DTable<R_ST_BUS>*	r_st_bus_tb;


private:

	//integer variables for dynamic array size
	int				lv_digital;
	int				lv_analog;
	int				lv_co;
	int				lv_st;
	int				lv_xfmr;
	int				lv_xf;
	int				lv_cb;
	int				lv_so;
	int				lv_fd;
	int				lv_un;
	int				lv_ld;
	int				lv_cp;
	int				lv_bus;
	int				lv_zn;
	int				lv_sec;
	int				lv_nd;
	int				lv_bs;
	int				lv_island;
	int				lv_global;
	int				lv_tapty;
	int				lv_lnty;
	int				lv_r_co_st;
	int				lv_r_st_xfmr;
	int				lv_r_xfmr_xf;
	int				lv_r_st_cb;
	int				lv_r_st_so;
	int				lv_r_st_ld;
	int				lv_r_st_cp;
	int				lv_r_st_bus;
//	int				lv_r_st_zn;
	int				lv_r_zn_ld;
	int				lv_r_zn_sec;
	int				lv_r_zn_cp;
	int				lv_r_zn_xf;
	int				lv_r_zn_cb;
	int				lv_r_nd_ld;
	int				lv_r_nd_cb;
	int				lv_r_nd_xf;
//	int				lv_r_nd_zn;
	int				lv_r_nd_sec;
	int				lv_r_nd_cp;
	int				lv_r_fd_un;
	int				lv_r_fd_so;
	int				lv_r_fd_ld;

	//lld by lld 2018-4-25
	int				lv_eq;
	int				lv_r_nd_eq;
	int				lv_r_fd_eq;
	int				lv_r_fd_zn;
	
	//pointer variables for dynamic arrays
	DIGITAL*		digital;
	ANALOG*			analog;
	CO*				co;
	ST*				st;
	XFMR*			xfmr;
	XF*				xf;
	CB*				cb;
	SO*				so;
	FD*				fd;
	UN*			    un;
	LD*				ld;
	CP*				cp;
	BUS*			bus;
	ZN*				zn;
	SEC*			sec;
	ND*				nd;
	BS*				bs;
	ISLAND*			island;
	GLOBAL*			global;
	TAPTY*			tapty;

	R_CO_ST*		r_co_st;
	R_ST_XFMR*		r_st_xfmr;
	R_XFMR_XF*		r_xfmr_xf;
	R_ST_CB*		r_st_cb;
	R_ST_SO*		r_st_so;
	R_ST_LD*		r_st_ld;
	R_ST_CP*		r_st_cp;
	R_ST_BUS*		r_st_bus;
//	R_ST_ZN*		r_st_zn;
	R_ZN_LD*		r_zn_ld;
	R_ZN_SEC*		r_zn_sec;
	R_ZN_CP*		r_zn_cp;
	R_ZN_XF*		r_zn_xf;
	R_ZN_CB*		r_zn_cb;
	R_ND_LD*		r_nd_ld;
	R_ND_CB*		r_nd_cb;
	R_ND_XF*		r_nd_xf;
	R_ND_EQ*		r_nd_eq;
//	R_ND_ZN*		r_nd_zn;
	R_ND_SEC*		r_nd_sec;
	R_ND_CP*		r_nd_cp;
	R_FD_UN*		r_fd_un;
	R_FD_ZN*		r_fd_zn;
	R_FD_SO*		r_fd_so;
	R_FD_EQ*		r_fd_eq;
	R_FD_LD*		r_fd_ld;
	R_FD_ND*		r_fd_nd;
};

#endif






