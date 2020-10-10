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

//#define	REPAIRDEFAULTTIME	0.2		//���Ϻ�ά�޻ָ�ʱ�䣬Сʱ
//#define	TRANSLOADTIME		0.02	//���Ϻ�ת��ʱ�䣬Сʱ

#define g_wbase			100.0 //��׼����MW
//��׼��ѹ=10kV,��׼����10/1.732=5.7736kA����׼����=1ŷķ

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

	//��·����
    double ss;
	double r1;//��Դ����������ֵ
	double x1;//��Դ�������翹ֵ
	double r0;//��Դ����������ֵ��������С���裬�ӵر�ĵ��裬ΪС����ӵ�׼��
	double x0;//��Դ�������翹ֵ��������������Ȧ�ĵ翹ֵ��Ҳ�����ǽӵر�ĵ翹ֵ

} G_WG;
		
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_WB - bus data 
#define mx_wb		500
typedef struct G_WB        
{
    int    tio; //�ڵ��Ż���� ǰ �ı��
    int    toi; //�ڵ��Ż���� �� �ı��
    int    ja;
    int    jb;	//ǰ�ƻش����������ڱ�ʾ����ڵ������֧·
    int    jc;	//���νڵ�
    double v;
    double a;
    double nw;	//����ֵ��ע��
    double nr;
    double dv;	//�ۼ�ֵ�������ۼ�
    double da;
    double dw;	//�ۼ�ֵ�����ι����ۼӵ�����ڵ��ϵ�ֵ����ʾ�ڵ������߶ε��׶˹���ֵ
	double dr;	//**�޹��ۼӵ�����ڵ���ܼ�ֵ���޹��Ż���ʱ������
    double wloss;//�ڵ������߶ε��й������޹��Ż�ʹ��	
    double rloss;//�ڵ������߶ε��޹������޹��Ż�ʹ��	

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

//	double rdi;//����ĸ�ߵĶ����޹����ɾأ��޹��Ż���ʱ��ʹ��
//	double qv;//�޹��Ż���ʱ��ʹ����Ҫ�����ĵ���������
//	double b_v;//�޹��Ż���ʱ��ʹ����Ҫ����ǰ�ĵ�ѹ
//	double b_cosfi;//�޹��Ż���ʱ��ʹ����Ҫ����ǰ�Ĺ�������

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
	double r0;//����
	double x0;
	double b0;

	int	   fp;//�޹��Ż��ڵ��֧·
} G_WBR;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  G_ZBR - zero branch data 
#define mx_zbr		500
typedef struct G_ZBR    
{
    int    logic;//֧·���
    int    ibs;//�׽ڵ�=ĩ�ڵ�
    double w;//�й�
    double r;//
    double v;//��ѹ��ֵ
    double a;//
    double i;//
	
	int	flag;//��־λ

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
//��˳���ŵ��ɾ���ǶԽ�Ԫ�ص����飬�±��0��ʼ
#define mx_ju		1000
typedef struct G_JU    
{
    int    j;//�ھ����е��б��
    int    d;//�ǶԽ�Ԫ��������Ӧ�����е�λ�ã���Ҫ����һ�����õ����е�Ԫ�ظ���
    int    k;//��Ԫ�صĶ�Ӧ���߶εı������
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
	int  ExecutePwrflow();//ȫ�����˵�����
	//����е������ݣ���ô������ض��ĵ����д���
	//type=0,ֻ���㴿��Դ�㣬type=1,�����翪��Ҳ����ɵ�Դ��
	int  ExecuteIslandPwrflow(int iisland,int type=0);//ִ�е������ĳ���

	//ͨ����Դ�������Ը��ɽ��з���
	//��ѹ���Ĺ�������ȡ0.85������ȡ5%
	double g_scale_P/*���ɷ���ʱ���й�����*/,g_scale_Q/*���ɷ���ʱ���޹�����*/;
	int OnDistributeLoadData();
	int PutUnResult();//�����翪�ص���Ϣд�룬��n-1�Ϳɿ��Լ���ʱ����

private:

	//internal processing methods
	int  OrderBusNumberC();//�붯̬�Ż����
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
	int		g_bsmx;//���μ�������ĸ����
	double	g_eps;
	int		g_pvbsnum;
	int		g_loopnum;
	int		g_slackbsnum;//ƽ��ڵ�ĸ���
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

	int mathtype;//������������ͣ�0-������1-Ϊ�޹��Ż���
	int g_ld_distr;//�Ƿ���и��ɷ��䣬���޹��Ż���ʱ���ǲ���Ҫ���з����
	double g_vl;//��ѹ�ȼ���10,20,35,6kV

	//������޹��Ż��ڵ�,lld add 2019-7-27
	std::vector<int> cpbss;//����ڵ��ĸ�߱��
	std::vector<float> cps;//����ڵ���޹�ע��ֵ

	//����0�迹֧·��lld add 2015-4-30
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

	//���������������������ܻ��м̳�
	//0-����������2-��������4-���翪�ع���������
	int  PutFlowResult(int type=0);

	int  FormAdmittance();
	int  FormAdmittance0();
	int  ExecuteSeidel();

	//database object
	DSBOB2*				dsbob2;
};

#endif