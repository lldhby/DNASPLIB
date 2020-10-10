// optSect.h: interface for the CReliabilty class.
//最小路可靠性计算，author：phd. lld 2004-12-12
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RELIABLE_H__B4F4A5EB_DA9C_4A8B_B90A_5808CD262AA2__INCLUDED_)
#define AFX_RELIABLE_H__B4F4A5EB_DA9C_4A8B_B90A_5808CD262AA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _WIN32
#include "..\DPCAS4.1\WINDLL\calcdll\lib.h"
#elif __linux__
#else
#endif

#include "../DNASPLIB/pwrflow.h"
#include "../DNASPLIB/dsbob2.h"
#include "../DNASPLIB/netmom.h"
#include "../DNASPLIB/tmpodrtb.h"

#include <algorithm>
#include <functional>
#include <vector> 
#include <ctime>
#include <cstdlib> 
using namespace std;


class optCP : public PWRFLOW
{
public:
	optCP();

	optCP(DSBOB2* m_pDb, CLLD* m_pLLD);
	virtual ~optCP();
	CLLD* lld;

/////////////////////////////////////////////////////
public:
	std::vector<int> cpbss;
	std::vector<float> cps;

	std::vector<std::string> stay;//强制安装节点
	std::vector<std::string> unstay;//不能安装节点

	std::vector<std::string> possible_nds;//计算可以安装节点
	std::vector<std::int16_t> possible_r;//计算可以安装容量

//////////////////////////////////////////////////////////////////////////
public:
	DTable<SEC>*		sec_tb_t;//将临时表保存下来

#if _WIN32
	int optInitFlow(NDT* nd,SECT* sec, char* Efile_name);//初始化潮流,windows直接返回结果到界面
#endif

	int optInitFlow(char* Efile_name);//初始化潮流
	int optCalc(int type);
	int optSerialCP();//串联电容器
	int SerialCP(char* nd, char* secid, double r);//串联电容器
	int ParallelCP();//并联电容器

	int OnCalFlow(double& minV, double& maxV);
	int isValidate();

	int optResult();

	//////////////////////////////////////////////////////////////////////////
	void getRandom(int** cuts);

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
		int   *chrom;                    /* 染色体 */
		double   fitness;                   /* 个体适应度*/
		double   thick;                     /*个体浓度*/
	};
	struct bestever                         /* 最佳个体*/
	{
		int   *chrom;                    /* 最佳个体染色体*/
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

	int    fn;                             //复型变量的个数
	int    in;                             //整形变量的个数

	int    gen;                            /* 当前世代数 */
	// int    run;                            /* 当前运行次数 */
	// int    maxruns;                        /* 总运行次数   */
	int    ncross;                         /* 当前代交叉发生次数 */

	double psel[200];                         //按照排名顺序分配选择概率
	double sumpsel;                       //选择概率总和
	double vlow[100];                            //实型控制变量的上限
	double vhigh[100];                           //实型控制变量的下限
	int    ilow[1000];                            //整型控制变量的上限
	int    ihigh[1000];                           //整型控制变量的下限

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
	void crossover(int *, int *, int *, int *);
	void mutation(int *);
	void addrandpop(double r);//r为淘汰率
	void qksortbig(individual *pop,int left,int right);
	void THICK(double T);

	void safebest();

	//遗传算法结束
	//////////////////////////////////////////////////////////////////////////
	//针对每一条馈线，准备可靠性计算

//////////////////////////////////////////////////////////////////////////
private:
	//database object
	DSBOB2*				dsbob2;
//
//private:
//	CB*			cb;
//	SO*			so;
//	LD*			ld;
//	SEC*		sec;
//	ND*			nd;
//	R_ND_CB*	r_nd_cb;
//	R_ND_XF*	r_nd_xf;
//	R_ND_SEC*	r_nd_sec;
//	R_ND_LD*	r_nd_ld;
//	R_ND_CP*	r_nd_cp;
//
//private:
//	DTable<CB>*			cb_tb;
//	DTable<CP>*			cp_tb;
//	DTable<SO>*			so_tb;
//	DTable<LD>*			ld_tb;
//	DTable<SEC>*		sec_tb;
//	DTable<ND>*			nd_tb;
//	DTable<R_ND_CB>*	r_nd_cb_tb;
//	DTable<R_ND_XF>*	r_nd_xf_tb;
//	DTable<R_ND_SEC>*	r_nd_sec_tb;
//	DTable<R_ND_LD>*	r_nd_ld_tb;
//	DTable<R_ND_CP>*	r_nd_cp_tb;
//	//////////////////////////////////////////////////////////////////////////
//
//private:
//	//record count variables
//	int	lv_cb;
//	int	lv_so;
//	int	lv_ld;
//	int	lv_sec;
//	int	lv_nd;
//	int	lv_r_nd_cb;
//	int	lv_r_nd_xf;
//	int	lv_r_nd_sec;
//	int	lv_r_nd_ld;
//	int	lv_r_nd_cp;
};

#endif // !defined(AFX_RELIABLE_H__B4F4A5EB_DA9C_4A8B_B90A_5808CD262AA2__INCLUDED_)
