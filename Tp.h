////////////////////////////////////////////////////////////////////////////////
//
//  TP.H
//  Class definition for distribution topology processing
//
////////////////////////////////////////////////////////////////////////////////
#if !defined(__TP_H)
#define __TP_H

#if _WIN32
#elif __linux__
#else
#endif

#include "../DNASPLIB/LLD.h"
#include "../DNASPLIB/netmom.h"
#include "../DNASPLIB/dsbob2.h"
#include "../DNASPLIB/tmpodrtb.h"

#define TP_FAIL		0
#define	TP_SUCCEED	1

//一个岛里面的最大分支数
//#define g_mxnbr 20
#define g_mxnbr 1000
////////////////////////////////////////////////////////////////////////////////
// TP  合并开关节点、聚合母线、母线分岛，为潮流计算做准备
// 最小路的搜索需要在计算母线前做，需要保留开关的相关信息

class TP
{
public:

	//constructor and destructor
	TP(DSBOB2* m_pDb, CLLD* m_pLLD);
	~TP();
	CLLD* lld;

	//complete topology processing
	int tp_allnew();
	int tp_result();//输出辐射网拓扑结果，分电源点
	
private:

	//internal processing  methods
	int tp_table_count();
	int tp_table_get();
	int tp_nd_sort();
	int tp_bs_sort();
	int tp_bs_neighbor();
	int tp_comp_status();	
	int tp_trim_cord();

	//通过首末节点得到设备id
	void getEqID(char* eqid, int knd, int kknd);
private:

	//variables
	int g_bs;
	int g_unit;
	int g_shunt;
	int g_nbr;
	int g_z_br[g_mxnbr];

private:

	//record count variables
	int	lv_digital;
	int	lv_analog;
	int	lv_co;
	//int	lv_st;
	int	lv_xfmr;
	int	lv_xf;
	int	lv_cb;
	int	lv_so;
	int	lv_ld;
	int	lv_cp;
	int	lv_bus;
	//int	lv_zn;
	int	lv_sec;
	int	lv_nd;
	int	lv_bs;
	int	lv_island;
	int	lv_global;
	int	lv_tapty;
	int	lv_lnty;
//	int lv_r_co_st;
//	int	lv_r_st_xfmr;
	int	lv_r_xfmr_xf;
//	int	lv_r_st_cb;
//	int	lv_r_st_so;
//	int	lv_r_st_ld;
//	int	lv_r_st_cp;
//	int	lv_r_st_bus;
//	int	lv_r_st_zn;
//	int	lv_r_zn_ld;
//	int	lv_r_zn_sec;
	int	lv_r_nd_cb;
//	int	lv_r_nd_xf;
//	int	lv_r_nd_zn;
	int	lv_r_nd_sec;

private:

	//pointer variables
	DIGITAL*	digital;
	ANALOG*		analog;
	CO*			co;
//	ST*			st;
	XFMR*		xfmr;
	XF*			xf;
	CB*			cb;
	SO*			so;
	LD*			ld;
	CP*			cp;
	BUS*		bus;
//	ZN*			zn;
	SEC*		sec;
	ND*			nd;
	BS*			bs;
	ISLAND*		island;
	GLOBAL*		global;
	TAPTY*		tapty;
//	R_CO_ST*	r_co_st;
//	R_ST_XFMR*	r_st_xfmr;
	R_XFMR_XF*	r_xfmr_xf;
//	R_ST_CB*	r_st_cb;
//	R_ST_SO*	r_st_so;
//	R_ST_LD*	r_st_ld;
//	R_ST_CP*	r_st_cp;
//	R_ST_BUS*	r_st_bus;
//	R_ST_ZN*	r_st_zn;
	//R_ZN_LD*	r_zn_ld;
	//R_ZN_SEC*	r_zn_sec;
	R_ND_CB*	r_nd_cb;
	R_ND_XF*	r_nd_xf;
//	R_ND_ZN*	r_nd_zn;
	R_ND_SEC*	r_nd_sec;

private:
	//int ClearTopflags();

	//database object
	DSBOB2*				dsbob2;
	DTable<DIGITAL>*	digital_tb;
	DTable<ANALOG>*		analog_tb;
	DTable<CO>*			co_tb;
//	DTable<ST>*			st_tb;
	DTable<XFMR>*		xfmr_tb;
	DTable<XF>*			xf_tb;
	DTable<CB>*			cb_tb;
	DTable<SO>*			so_tb;
	DTable<LD>*			ld_tb;
	DTable<CP>*			cp_tb;
	DTable<BUS>*		bus_tb;
	//DTable<ZN>*			zn_tb;
	DTable<SEC>*		sec_tb;
	DTable<ND>*			nd_tb;
	DTable<BS>*			bs_tb;
	DTable<ISLAND>*		island_tb;
	DTable<GLOBAL>*		global_tb;
	DTable<TAPTY>*		tapty_tb;
//	DTable<R_CO_ST>*	r_co_st_tb;
//	DTable<R_ST_XFMR>*	r_st_xfmr_tb;
	DTable<R_XFMR_XF>*	r_xfmr_xf_tb;
//	DTable<R_ST_CB>*	r_st_cb_tb;
//	DTable<R_ST_SO>*	r_st_so_tb;
//	DTable<R_ST_LD>*	r_st_ld_tb;
//	DTable<R_ST_CP>*	r_st_cp_tb;
//	DTable<R_ST_BUS>*	r_st_bus_tb;
//	DTable<R_ST_ZN>*	r_st_zn_tb;
	//DTable<R_ZN_LD>*	r_zn_ld_tb;
	//DTable<R_ZN_SEC>*	r_zn_sec_tb;
	//DTable<R_ZN_CB>*	r_zn_cb_tb;

	DTable<R_ND_CB>*	r_nd_cb_tb;
	DTable<R_ND_XF>*	r_nd_xf_tb;
//	DTable<R_ND_ZN>*	r_nd_zn_tb;
	DTable<R_ND_SEC>*	r_nd_sec_tb;
	DTable<R_ND_CP>*	r_nd_cp_tb;
	DTable<R_ND_LD>*	r_nd_ld_tb;

	//事先并不知道有多少个母线和岛，需要用临时的进行处理
	DTable<BS>*			bs_tb_t;
	DTable<ISLAND>*		island_tb_t;

};

#endif