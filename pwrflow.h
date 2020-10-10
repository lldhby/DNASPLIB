//*****************************************************************************
//
//  PWRFLOW.H
//  Class definition for Distribution Load Flow Application
//
//*****************************************************************************
#if !defined(__PWRFLOW_H)
#define __PWRFLOW_H
#if _WIN32
#elif __linux__
#else
#endif

#include "../DNASPLIB/dsbob2.h"
#include "../DNASPLIB/LLD.h"

#define PWRFLOW_FAIL	0
#define PWRFLOW_SUCCEED	1
#define	MXELE			27

//#define	REPAIRDEFAULTTIME	0.2		//故障后维修恢复时间，小时
//#define	TRANSLOADTIME		0.02	//故障后转供时间，小时

#define g_wbase			100.0 //基准功率MW
//基准电压=10kV,基准电流10/1.732=5.7736kA，基准电阻=1欧姆

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_WG - generator data 
#define mx_wg			20
typedef struct G_WG    
{
    int    logic;
    int    ibs;
    int    regbs;
    int    slackavr;
    int    kswitch;
    double vtarget;
    double wmx;
    double rmx;
    double wmn;
    double rmn;
    double w;
    double r;
    double v;
    double a;

	//短路容量
    double ss;
	double r1;//电源点的正序电阻值
	double x1;//电源点的正序电抗值
	double r0;//电源点的零序电阻值，可能是小电阻，接地变的电阻，为小电阻接地准备
	double x0;//电源点的零序电抗值，可能是消弧线圈的电抗值，也可能是接地变的电抗值

} G_WG;
		
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_WB - bus data 
#define mx_wb		500
typedef struct G_WB        
{
    int    tio; //节点优化编号 前 的编号
    int    toi; //节点优化编号 后 的编号
    int    ja;
    int    jb;	//前推回代计算中用在表示这个节点的上游支路
    int    jc;	//上游节点
    double v;
    double a;
    double nw;	//固有值，注入
    double nr;
    double dv;	//累加值，下游累加
    double da;
    double dw;	//累加值，下游功率累加到这个节点上的值，表示节点流进线段的首端功率值
	double dr;	//**无功累加到这个节点的总加值，无功优化的时候有用
    double wloss;//节点流进线段的有功网损，无功优化使用	
    double rloss;//节点流进线段的无功网损，无功优化使用	

	double v1;
	double a1;
	double v2;
	double a2;
	double v0;
	double a0;
	double va;
	double aa;
	double vb;
	double ab;
	double vc;
	double ac;

//	double rdi;//计算母线的二次无功负荷矩，无功优化的时候使用
//	double qv;//无功优化的时候使用需要补偿的电容器容量
//	double b_v;//无功优化的时候使用需要补偿前的电压
//	double b_cosfi;//无功优化的时候使用需要补偿前的功率因素

} G_WB;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_WLD - load data 
#define mx_wld		150
typedef struct G_WLD    
{
    int    logic;
    int    ibs;
    double w;
    double r;

} G_WLD;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_WBR - branch data 
#define mx_wbr		500
typedef struct G_WBR    
{
    int    type;
    int    logic;
    int    ibs;
    int    zbs;
    double r;
    double x;
    double b;
	double r0;//零序
	double x0;
	double b0;

	int	   fp;//无功优化节点的支路
} G_WBR;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_ZBR - zero branch data 
#define mx_zbr		500
typedef struct G_ZBR    
{
    int    logic;//支路编号
    int    ibs;//首节点=末节点
    double w;//有功
    double r;//
    double v;//电压幅值
    double a;//
    double i;//
	
	int	flag;//标志位

} G_ZBR;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_WXF - transformer data (non-rating ratio)
#define mx_wxf		100
typedef struct G_WXF
{
	int	   ibr;
	double t;
	double g1;
	double b1;
	double g2;
	double b2;

} G_WXF;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_WSH - shunt data 
#define mx_wsh		100
typedef struct G_WSH   
{
    int    logic;
    int    ibs;
    double r;
    double rnom;

} G_WSH;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_YU - upper trangular admittance matrix
#define mx_yu		1000
typedef struct G_YU   
{
    int    i;
    int    j;
    int    p;
    int    d;
    double g;
    double b;

} G_YU;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_JD - diagonal blocks of Jacobian matrix
#define mx_jd		1000
typedef struct G_JD   
{
    double H;
    double N;
    double J;
    double L;

} G_JD;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_JU - upper blocks of Jacobian matrix
//按顺序存放导纳矩阵非对角元素的数组，下标从0开始
#define mx_ju		1000
typedef struct G_JU    
{
    int    j;//在矩阵中的列编号
    int    d;//非对角元素在所对应的行中的位置，需要用下一个减得到行中的元素个数
    int    k;//该元素的对应的线段的编号链表
    double H;
    double N;
    double J;
    double L;

} G_JU;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_JL - lower blocks of Jacobian matrix
#define mx_jl		1000
typedef struct G_JL   
{
    int    j;
    int    d;
    int    k;
    double H;
    double N;
    double J;
    double L;

} G_JL;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_JB - upper blocks of Jacobian matrix after Gauss method
#define mx_jb		1000
typedef struct G_JB    
{
    int    j;
    int    d;
    double H;
    double N;
    double J;
    double L;

} G_JB;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_RX - break point sensivity matrix
#define mx_rx		10
typedef struct G_RX    
{
    double r;
    double x;

} G_RX;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_CTL - control parameters
#define mx_ctl		1
typedef struct G_CTL
{
	int	   id;
	int	   method;
	int	   itermx;
	float  vtol;
	time_t start;
	float  second;
	int	   iter;
	char   convg[4];

} G_CTL;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_ITER - iteration information
#define mx_iter		500
typedef struct G_ITER
{
	int	   id;
	float  mxv;
	int	   bsv;
	int	   nbsv;

} G_ITER;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  PWRFLOW - power flow calculation class
class  PWRFLOW 
{
public:

	//constructor and destructor
	PWRFLOW();
	PWRFLOW(DSBOB2* m_pDb, CLLD* m_pLLD);
	~PWRFLOW();
	CLLD* lld;
	//interface for power flow calculation
	int  ExecutePwrflow();//全部拓扑岛都做
	//如果有岛的数据，那么就针对特定的岛进行处理
	//type=0,只计算纯电源点，type=1,将联络开关也虚拟成电源点
	int  ExecuteIslandPwrflow(int iisland,int type=0);//执行单个岛的潮流

	//通过电源点的量测对负荷进行分配
	//变压器的功率因素取0.85，线损取5%
	double g_scale_P/*负荷分配时的有功比例*/,g_scale_Q/*负荷分配时的无功比例*/;
	int OnDistributeLoadData();
	int PutUnResult();//将联络开关的信息写入，在n-1和可靠性计算时有用

private:

	//internal processing methods
	int  OrderBusNumberC();//半动态优化编号
//	int  ConstructSM();
//	int  ExecuteSeidel();
//	int  CorrectBPPower();
	int  InitJacobianA();
	int  InitJacobianB();
	int  FormJacobian();
	int  GetSlackPower(int g_slackbs);
	int  SetSlackPowerA();
	int  SetSlackPowerB();
	int  SetSlackPowerC();
	int  GetBusPower(int i, double* w, double* r, int s = 0);

	int  PutBusVoltage_1(int type);
	int  PutLineFlow_1(int type);
	int  PutSystemPower_1(int type);

public:

	//variables
	int		g_method;
	int		g_mxiter0;
	int		g_mxiter;
	int		g_niter0;
	int		g_niter;
	double	g_epsmx0;
	double	g_epsmx;
	double	g_eps0;
	double	g_eps_last;
	int		g_island;
	int		g_bsmx;//本次计算的最大母线数
	double	g_eps;
	int		g_pvbsnum;
	int		g_loopnum;
	int		g_slackbsnum;//平衡节点的个数
	int		g_slackbs;
	int		g_slackgen;
	G_WG*	g_wg;
	int		lv_wg;
	G_WB*	g_wb;
	int		lv_wb;
	G_WLD*	g_wld;
	int		lv_wld;
	G_WBR*	g_wbr;
	int		lv_wbr;
	G_WXF*  g_wxf;
	int	    lv_wxf;
	G_WSH*	g_wsh;
	int		lv_wsh;
	G_YU*	g_yu;
	int		lv_yu;
	G_JD*	g_jd;
	int		lv_jd;
	G_JU*	g_ju;
	int		lv_ju;
	G_JL*	g_jl;
	int		lv_jl;
	G_JB*	g_jb;
	int		lv_jb;
	G_RX	(*g_rx)[mx_rx];
	int		lv_rx;
	G_CTL*	g_ctl;
	G_ITER*	g_iter;
	int		lv_iter;

	double	g_r;
	double	g_x;
	double	g_wloss;
	double	g_rloss;

	double maxVoltage;
	double minVoltage;

	int mathtype;//潮流计算的类型，0-潮流；1-为无功优化；
	int g_ld_distr;//是否进行负荷分配，在无功优化的时候是不需要进行分配的
	double g_vl;//电压等级，10,20,35,6kV

	//虚拟的无功优化节点,lld add 2019-7-27
	std::vector<int> cpbss;//虚拟节点的母线编号
	std::vector<float> cps;//虚拟节点的无功注入值

	//处理0阻抗支路，lld add 2015-4-30
	G_ZBR*	g_zbr;
	int		lv_zbr;


protected:
	void InitWorkingArray();
	int  GetNetworkData(int type=0);

	int  GaussElimination();
	int  SolveEquation();

	int  GetSourceData(int type=0);
	int  GetBranchData();
	int  GetLoadData();
	int  GetShuntData();

	int  OrderBusNumberA();
	int  ExecuteSweep();

	int  OrderBusNumberB();
	int  ExecuteNewton();

	int  CheckConvergence();

	//输出潮流计算结果，后面可能还有继承
	//0-正常收敛；2-不收敛；4-联络开关供残余网络
	int  PutFlowResult(int type=0);

	int  FormAdmittance();
	int  FormAdmittance0();
	int  ExecuteSeidel();

	//database object
	DSBOB2*				dsbob2;
};

#endif