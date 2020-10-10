// optSect.h: interface for the CReliabilty class.
//最小路可靠性计算，author：phd. lld 2004-12-12
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RELIABLE_H__B4F4A5EB_DA9C_4A8B_B90A_5808CD262AA2__INCLUDED_)
#define AFX_RELIABLE_H__B4F4A5EB_DA9C_4A8B_B90A_5808CD262AA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../DNASPLIB/dsbob2.h"
#include "../DNASPLIB/netmom.h"
#include "../DNASPLIB/tmpodrtb.h"

//FEMA法可靠性计算基于NM1计算
#include "../DNASPLIB/NM1.h"
#include "../DNASPLIB/RA.h"

#include <algorithm>
#include <functional>
#include <vector> 
#include <ctime>
#include <cstdlib> 
using namespace std;


//可靠性计算专用的预设量
//const int maxLoad=500;//单条线上的最多负荷数量
//const int maxLoad_bak=2000;//单条线上的最多负荷数量
//const int maxUnion=50;//单条线上的最多联络开关数量
//const int maxEquipment=2000;//单条线上的最多设备数量
//const int maxPath=50;//单个负荷的最小路长度

#define     EQUIPMENT_BREAKER	1	//设备是否已处理
#define     EQUIPMENT_RELAY		2	//保护开关
#define     EQUIPMENT_LOADER	4	//负荷开关,含不带保护的断路器
#define     EQUIPMENT_DISCONN	8	//隔离开关
#define     EQUIPMENT_FUSE		16	//熔断器

#define UNION_TRAN 5;//默认联络开关的转供裕度 5MVA

const int maxMainND=2000;//单条线上的最多负荷数量

//////////////////////////////////////////////////////////////////////////
//最小路计算可靠性

//负荷最小路，每个负荷点有唯一的最小路，该数组的长度为馈线上的全部负荷的个数
typedef struct PATH_LOAD//搜索所有负荷
{
	int i_id;//序号，用于排序和快速搜索
	char	id[MAX_SEC_ID_LEN];//设备ID号 
	int	ind;	//节点
	int ptr;	//负荷指针，通过这个指针可以直接找到负荷
	double	s;		//潮流计算得到的视在功率
	double	monkV;	//变压器的铭牌上都是视在功率(视在功率)
	int	minpath[MAX_MINPATH_LEN];//负荷的最小路，存最多的200个节点 
	int	pathlen;//最小路的长度 
	
	int cntbakpath;//备用最小路的条数，一个负荷点可能有多个备用最小路
	
	int q;	//标志位
	int level;	//负荷的等级，一类负荷（重要），二类负荷（一般），三类负荷（农网）

	int equippath[MAX_MINPATH_LEN];//最小路上的设备列表
	
} PATH_LOAD;

//联络开关到电源点的路径
//长度为联络开关的个数
typedef struct PATH_UNION//搜索所有联络开关
{
	char	id[MAX_SEC_ID_LEN];//设备ID号 
	int	ind;	//首节点
	int ptr;	//开关指针，通过这个指针可以直接找到负荷
	double	Abun;	//联络开关的裕度
	
	int	pathlen;//备用最小路的长度 
	int	mainpath[MAX_MINPATH_LEN];//联络开关对应的主干线节点，存最多30个联络开关，每个50个节点 
	int	equipmainpath[MAX_MINPATH_LEN];//联络开关对应的主干线节点，存最多30个联络开关，每个50个节点 
		
	int q;	//标志位
	
} PATH_UNION;

//主干线上的节点的集合
typedef struct PATH_MAINLINE
{
	int	ind;	//节点号
	int	equip_ind_l;//左 设备节点号 
	int	equip_ind_r;//右 设备节点号 
	char	equip_id_l[MAX_SEC_ID_LEN];//左 设备ID号 
	char	equip_id_r[MAX_SEC_ID_LEN];//右 设备ID号 

	int sumLD;//总的负荷个数
	char ldList[MAX_MIANND_LD];
	double  sumP;//归集到节点的负荷大小
	double  w; 	 //流过这个节点的负荷大小，由潮流计算得出
	double  sumQ;//归集到节点的负荷大小
	double  len;//归集到节点到上一个节点的长度

	char	UpCBID[MAX_ID_DLL];
	char	UnionCBID[MAX_ID_DLL];

	int isValid;//主节点是否有效，如果联络开关到主节点之间都没有负荷，则是无效的主节点
	
} PATH_MAINLINE;

typedef struct stru_MainDev
{
	int eq_id;
	int type;//10-线路，1-普通开关，2-出线开关，3-普通联络开关，4-节点，5-最远联络开关（主联络开关）
	int idph;//设备原始物理ID号
	char id[64];//设备原始ID号
	int ind;//设备原始物理ID号
	int iznd;//设备原始物理ID号
	double len;//线路的长度
	int x1;
	int y1;
	int x2;
	int y2;
};
typedef struct stru_MainND
{
	int type;//一级节点，二级节点，三级节点，...
	bool nd_up;//节点在主干的上面
	int nd_mapH;//节点的基准线

	int ind;//设备原始物理ID号
	char id[128];//节点的ID号
	int x1;
	int y1;
	int sumLD;//负荷个数
	//char ldList[1024];
	//double w;
	char UpCBID[128];//是否有分支开关
	char UnionCBID[128];//是否有联络开关
};

class optSect : public CNM1  
{
public:
	optSect();

	optSect(DSBOB2* m_pDb, CLLD* m_pLLD);
	virtual ~optSect();
	CLLD* lld;

//////////////////////////////////////////////////////////////////////////
//架空线分段开关优化，2017-12-27
	ND* nd_t;
	int cntMainLine;//主干线上节点的个数
	PATH_MAINLINE path_mainline[maxMainND];
	PATH_MAINLINE path_mainline1[maxMainND];

//鱼骨图，2018-11-22
	void initMainDev(stru_MainDev& mainDev);
	vector<stru_MainDev> mainDevList;//主设备
	vector<stru_MainND> mainNDList;//主节点
	int mapW, mapH, deltaH;//最大的幅面，800，600
	int find_ind;
	//bool findx(stru_MainND &task);

	//在mainND里面查找节点，如果有，得到主节点的位置信息
	int isExistMainND(int knd);


////////////////////////////////////////////////////
	int OnOptSect(int type, char* mainline);//计算分段开关的优化
	int OnBuildMainLine(int soid,int& cntLD,double& sumLen,double& sumP, char* mainline);
	int OnGetNDSumPQ(int main_ind,int l,int r,double& sumP,double& sumQ,char* idList);
	double OnGetRealFlow(int ind,int before);
	int isInMainLine(int knd);
	int isInMainLine(int knd,PATH_MAINLINE* path,int len);
	bool isCB(int knd,int kknd,char* cbID);
	double isSEC(int knd,int kknd,double& len);

	//1.均匀线路，2.橄榄型；3.哑铃型；4.杠杆型
	int fdShape;
	int OnAnalyseShape();
	int OnSetMainLine(PATH_MAINLINE* path_mainline,int cntMainLine,int sond);

	int nSect;
	double w_cntLd, w_w, w_len,p_un, p_vip, p_avgLD,loadrate;
	int sond;
	int island;
	void OnTP(int* cuts, int cntCut, int isOpt=0);

	void getRandom(int** cuts);
	
	int optCalc(char* mainline_name);

	//准备结果输出
	int sect_id;
	int cut[5];
//////////////////////////////////////////////////////////////////////////

public:
	char	sFileName[120];//输出文件名
	ofstream o;
		
	double  pmutation;                      /* 变异概率 */
	int    popsize;                        /* 种群大小  */
	int    maxgen;                         /* 最大世代数   */

	//////////////////////////////////////////////////////////////////////////
	//遗传算法
	struct individual                       /* 个体*/
	{
		double   *chrom;                    /* 染色体 */
		double   fitness;                   /* 个体适应度*/
		double   thick;                     /*个体浓度*/
	};
	struct bestever                         /* 最佳个体*/
	{
		double   *chrom;                    /* 最佳个体染色体*/
		double   fitness;                   /* 最佳个体适应度 */
		int      generation;                /* 最佳个体生成代 */
	};

	struct individual *oldpop;             /* 当前代种群 */
	struct individual *newpop;             /* 新一代种群 */
	struct individual *temp;

	struct individual bestthick;          /* 最大浓度的个体 */
	struct bestever bestfit;               /* 最佳个体 */
	double sumfitness;                     /* 种群中个体适应度累计 */
	double max;                            /* 种群中个体最大适应度 */
	double avg;                            /* 种群中个体平均适应度 */
	double min;                            /* 种群中个体最小适应度 */
	double  pcross;                         /* 交叉概率 */
	int    nmutation;                      /* 当前代变异发生次数 */
	int    chromsize;                      /* 存储一染色体所需字节数 */

//	int    fn;                             //复型变量的个数
//	int    in;                             //整形变量的个数

	int    gen;                            /* 当前世代数 */
	// int    run;                            /* 当前运行次数 */
	// int    maxruns;                        /* 总运行次数   */
	int    ncross;                         /* 当前代交叉发生次数 */

	double psel[200];                         //按照排名顺序分配选择概率
	double sumpsel;                       //选择概率总和
	double vlow[100];                            //实型控制变量的上限
	double vhigh[100];                           //实型控制变量的下限
//	int    ilow[1000];                            //整型控制变量的上限
//	int    ihigh[1000];                           //整型控制变量的下限

	int nbest;                              //最佳个体保持不变的代数

	/* 随机数发生器使用的静态变量 */
	double static_r;

public:
	int type;//计算类型,0为普通算法，1为遗传算法

private:
	double random();   //产生一个0～1之间的随机数
	double randf(double low, double high);  //产生一个low,high之间的随机浮点数
	int randi(int low,int high); //产生一个low,high之间的随机整数
	
	void initGA();//相当于构造函数
	void initmalloc();
	
	int isValidChrome(individual *pop,int type);
	int getValidgen(individual *pop);
	int editChrome(individual *pop);
	int isNew(individual *pop, int size);

	void freeGA();
	void initpop();
	void minfunction(struct individual * critter);//形成目标函数
	void pselect(double);//按照排名顺序分配选择概率
	void qksortlittle(struct individual *,int,int);//对新一代种群排序
	void statistics(struct individual *);
	void genresult();

	void make_ga();
	void generation();
	int select();
	int flip(double);
	void crossover(double *, double *, double *, double *);
	void mutation(double *);
	void addrandpop(double r);//r为淘汰率
	void qksortbig(individual *pop,int left,int right);
	void THICK(double T);

	void safebest();

	//遗传算法结束
	//////////////////////////////////////////////////////////////////////////
	//针对每一条馈线，准备可靠性计算

//////////////////////////////////////////////////////////////////////////
private:
//////////////////////////////////////////////////////////////////////////
	int cntload;//负荷
	PATH_LOAD path_load[maxLoad];//一条线上最多200个负荷
	int cntunion;//联络开关
	PATH_UNION path_union[maxUnion];//一条馈线上最多30个联络开关
	int cntEquipment;
//	PATH_EQUIPMENT path_equipment[maxEquipment];//具体到每条馈线上的所有设备，包括开关和线段
//////////////////////////////////////////////////////////////////////////

//时间测试
	long t_getCrossPoint,t_equipCanToptSectns,t_getFirstCB,t_buildMinPath,t_InMinPath;

private:
	//计算最小路、备用最小路、每个设备到电源点的路径

	void InitPath();

	//核心类  输入源节点，输出联络开关指针,负荷的个数，负荷的数组
	int buildMinPath(int innd, int sond=-1);	
	
	int comp_vector(int* src,int* comp,int len,int& ele,bool bReverse = false);

	//src源向量，comp1,返回分叉的位置（从源开始数）,ele为元素
	//是否需要向量转置
	int comp_vector(int* src,int* comp,int& ele);//比较2个向量
	int ele_vector(int* src, int ele, int len=0);//ele是否在向量中，是返回位置，否返回-1
	//向量中两个元素进行比较，先出现返回1，后出现返回2，都没有，返回-1
	int ele_comp(int* src, int ele1, int ele2);
	
//	//将一个向量进行翻置
//	int vector_reverse(int* src,int* dst,int len);

//	//设备的位置，0-非最小路，1-最小路
//	int quipment_pos(PATH_LOAD minpath,int p1,int Equipmenti_ind,int Equipmenti_iznd);
	
	//给定设备的到电源点最小路，负荷点的最小路，求取它们的交点
	int getCrossPoint(int* src,int* comp,int& cross_nd,int& pos_src,int& pos_parent);


//传入：ldnd负荷的节点号
	//返回：minPath负荷点到电源点的最小路径
	PATH_LOAD getMinPath(int ldnd);//通过负荷的节点号，得到负荷的最小路
	
	//判断负荷点，设备故障后是否可以转供
	int equipCanTrans(int ldnd,int main_iznd);//通过负荷点得到可行最小路

	int getFirstCB(int* minPath,int p=0);//给定一个最小路，得到最小路上的第一个开关，返回开关首节点，否-1
	//判断开关是否在最小路上,是，返回1，否-1

	//inPath给定的一条备用路径，ldnd给定负荷，unionid, abund备用路径的联络开关盒裕度
	int isDisconnect(int* minPath_src,int start, int end);//通过负荷的节点号，得到负荷的最小路

//////////////////////////////////////////////////////////////////////////
	//给定首末节点号，返回开关的类型，找不到或不是开关返回-1
	int getCBType(int start, int end);
	
	//给定首末节点号，返回设备的指针
	int getEquipID(int start, int end);
	//设备直接连在最小路上
	int InMinPath(PATH_LOAD t_path_load,int i_id);



private:
	//internal processing  methods
	int rs_table_count();
	int rs_table_get();

private:
	//database object
	DSBOB2*				dsbob2;

private:
	CB*			cb;
	SO*			so;
	LD*			ld;
	SEC*		sec;
	ND*			nd;
	R_ND_CB*	r_nd_cb;
	R_ND_XF*	r_nd_xf;
	R_ND_SEC*	r_nd_sec;
	R_ND_LD*	r_nd_ld;

private:
	DTable<CB>*			cb_tb;
	DTable<SO>*			so_tb;
	DTable<LD>*			ld_tb;
	DTable<SEC>*		sec_tb;
	DTable<ND>*			nd_tb;
	DTable<R_ND_CB>*	r_nd_cb_tb;
	DTable<R_ND_XF>*	r_nd_xf_tb;
	DTable<R_ND_SEC>*	r_nd_sec_tb;
	DTable<R_ND_LD>*	r_nd_ld_tb;
//////////////////////////////////////////////////////////////////////////
	DTable<EQ>*      equip_tb;
	DTable<R_ND_EQ>*		r_nd_equip_tb;

	EQ* equip;
	R_ND_EQ*	r_nd_equip;
	int first;
	int prevph;
	int insert_equip(int cntEquipment,char* equipid,int equipidph, int ind,int iznd,int type,
							  double prob_fault, double repairTime,double prob_check=-1, double checkTime=-1);
	int insert_r_nd_equip(ND* nd_t,char* nd_id,int knd,int equip_id);
//////////////////////////////////////////////////////////////////////////

private:
	//record count variables
	int	lv_cb;
	int	lv_so;
	int	lv_ld;
	int	lv_sec;
	int	lv_nd;
	int	lv_r_nd_cb;
	int	lv_r_nd_xf;
	int	lv_r_nd_sec;
	int	lv_r_nd_ld;
};

#endif // !defined(AFX_RELIABLE_H__B4F4A5EB_DA9C_4A8B_B90A_5808CD262AA2__INCLUDED_)
