// RO.h: interface for the CRO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RO_H__C1C5E597_B94D_4104_90F5_666D53C4F4BE__INCLUDED_)
#define AFX_RO_H__C1C5E597_B94D_4104_90F5_666D53C4F4BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "D:\lld_common\dsbob_common_se\plan_calc\pwrflow.h"
#include "../DNASPLIB/LLD.h"
#include "../DNASPLIB/pwrflow.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_Rdi - 无功臂，从电源点到任意节点的最短距离,和母线个数是对应的
//在做潮流的时候或潮流结束后直接写入
#define mx_rdi		1000
typedef struct G_Rdi
{
	int	ibs;//母线编号，优化编号后
	int	ibsp;//父节点的编号	
	int	ibr;//母线上游的线段编号（指针）
	double	dr;//节点的无功，从上游流入该节点的无功值
	double	v;//节点的电压
	double	r;//节点的上游支路r
	int pivot;//1 为中枢点，默认-1

	int	downstream[MXELE];//下游节点的序列
	int	n_downstream;//下游节点的个数

	double	linearRdi;//一次无功臂,从该节点一直到电源点Rk/Uk^2+Rk-1/Uk-1^2+...+R1/U1^2
	double	quanRdi;//二次无功臂，从该节点一直到电源点的全部电阻之和，Rk+Rk-1+...+R1

	//double	linearQ;//一次无功精确矩
	double	quanQ;//二次无功精确矩，如果是线路的末端，直接给0

	double	Qv;//补偿容量
	double	b_cosfi;//补偿前的功率因素
	double	b_v;//补偿前的电压
	double	a_cosfi;//补偿后的功率因素
	double	a_v;//补偿后的电压

} G_Rdi;

//
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////  G_JD - diagonal blocks of Jacobian matrix
//#define mx_jd		1000
//typedef struct G_JD_T   
//{
//    double H;
//    double N;
//    double J;
//    double L;
//
//} G_JD_T;
//
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////  G_JU - upper blocks of Jacobian matrix
////存放装置雅可比矩阵非对角元素的数组，下标从0开始
//#define mx_ju		1000
//typedef struct G_JU_T    
//{
//    int    j;//在矩阵中的列编号
//    int    d;//非对角元素在所对应的行中的位置，需要用下一个减得到行中的元素个数
//    int    k;//该元素的对应的线段的编号链表
//    double H;
//    double N;
//    double J;
//    double L;
//
//} G_JU_T;
//
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////  G_JL - lower blocks of Jacobian matrix
//#define mx_jl		1000
//typedef struct G_JL_T   
//{
//    int    j;
//    int    d;
//    int    k;
//    double H;
//    double N;
//    double J;
//    double L;
//
//} G_JL_T;

//
//class CRDI : public PWRFLOW  
//{
//public:
//	CRDI();
//	virtual ~CRO();
//
//public:
//	int	ibs;//母线编号，优化编号后
//	int	ibsp;//父节点的编号	
//	int	ibr;//母线上游的线段编号（指针）
//	double	dr;//节点的无功，从上游流入该节点的无功值
//	double	v;//节点的电压
//	double	r;//节点的上游支路r
//	int pivot;//1 为中枢点，默认-1
//
//	int	downstream[MXELE];//下游节点的序列
//	int	n_downstream;//下游节点的个数
//
//	double	quanRdi;//二次无功臂，从该节点一直到电源点的全部电阻之和，Rk+Rk-1+...+R1
//
//	double	quanQ;//二次无功精确矩，如果是线路的末端，直接给0
//
//	double	Qv;//补偿容量
//	double	b_cosfi;//补偿前的功率因素
//	double	b_v;//补偿前的电压
//	double	a_cosfi;//补偿后的功率因素
//	double	a_v;//补偿后的电压
//}


class CRO : public PWRFLOW  
{
public:
	CRO(DSBOB2* pDb, CLLD* m_pLLD);
	virtual ~CRO();
	CLLD* lld;

public:
	DSBOB2* dsbob2;
	int OnExeRO();

	//单位容量补偿设备投资Kc:100元/kVar;电费belta：0.5元/kW.h；全网最大负荷损耗小时数tiao：2000
	double alpha/*折旧维修率*/,garma/*投资回收率*/;
	double belta/*电价*/,Kc,/*单位容量补偿设备投资*/tiao/*全网最大负荷损耗小时数*/;

	//0 第一阶段没有成功，需要第二次补偿，1 成功 
	int firstRPO();

	void resetRdi();//初始化无功臂
	int getRdi();//计算节点的无功逻辑关系
	int getQuadQ();//计算无功二次精确矩

	int getPivot(int* pivot);//计算中枢点，在中枢点上进行优化,一次只有一个中枢点
	int getSite();//通过二次矩或网损微增率求取补偿点
	double get_Q(int g_wb_id);//根据补偿点求取最佳容量

//////////////////////////////////////////////////////////////////////////
//环形网络的网损微增率计算
//直接在雅可比矩阵上进行计算，需要先得到雅可比矩阵的每个元素内容，
//将雅可比矩阵做一个转置，然后求解高斯法得到相关的无功微增量

	int getTmattrice();//得到雅可比矩阵的转置矩阵
	int getArrayQ();//得到无功网损微增率，并排序
	double getEconomy();//得到经济当量

//	G_JD_T*	g_jd_t;
//	int		lv_jd_t;
//	G_JU_T*	g_ju_t;
//	int		lv_ju_t;
//	G_JL_T*	g_jl_t;
//	int		lv_jl_t;
	double  g_Q_inc[1000];//无功网损微增率
	

//////////////////////////////////////////////////////////////////////////
//辐射型配电网无功优化精确矩计算
	G_Rdi*	g_rdi;//反映无功的精确二次矩
	int		lv_rdi;	
	int	rdi_bs1/*二次无功精确矩最大的补偿点1*/,rdi_bs2,rdi_bs3;

	double g_wloss_b,g_rloss_b,g_wloss_a,g_rloss_a;

public:
	void InitWorkingArray_1(int type=1);//单独的无功优化初始化

	int isNeedRPO();//馈线是否需要进行无功优化
	//分析电压 0 电压完全合格，不需补偿，-1 不可能补偿到位 -2 可以补偿到位 -3 电压越高限

	
	//0 第二阶段没有成功，需要输出电压改善结果，1 成功 
	int secondRPO();
	
	
	//int  PutNDRDI(int g_wb_id/*节点优化后的编号*/);
	double getNDRDI(int g_wb_id/*节点优化后的编号*/);//计算每个节点的无功臂，准备用着无功精确矩
	int buildRdi();//做每个节点的无功臂
	int PutND_QuadQ();//计算无功二次精确矩
	double getND_DelQ(int g_wb_id/*节点优化后的编号*/);//计算节点一次无功矩
	int setCP(int rdi_bs2,double& Q,int num);//rdi_bs2补偿点,double Q 补偿容量
	int Sweep(int rdi_bs2, double Qv );//潮流校验

	//判断补偿后潮流二次计算得到的结果，如果还有越限
	//1 补偿后没有越限；0 补偿后电压越限
	int isVoltageViolate(int oragin=0);//默认不是原始电压
	
	double getCosfi(double w,double r);//求功率因素
	int putRPO(int lev=2);//输出无功优化的结果，默认为2阶段

};

#endif // !defined(AFX_RO_H__C1C5E597_B94D_4104_90F5_666D53C4F4BE__INCLUDED_)
