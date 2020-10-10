// SC.h: interface for the CSC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SC_H__2D1BE096_4FA7_4086_A8B1_4FAC70B40C28__INCLUDED_)
#define AFX_SC_H__2D1BE096_4FA7_4086_A8B1_4FAC70B40C28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _WIN32
#elif __linux__
#else
#endif

#include "../DNASPLIB/LLD.h"
#include "../DNASPLIB/pwrflow.h"

#include "string.h"
#include <fstream>
using namespace std;

//故障描述,通过这个类表征发生短路的计算母线
//所有的故障都可以归算到计算母线上
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  FAULT
{
public:

	char 	id[MAX_BUS_ID_LEN];	//假定所有的故障都发生在母线上，配电自动化的最小单元
	char     descr[MAX_BUS_DESCR_LEN];

	//	case 1:  //single phase short to ground	单相短路接地
	//	case 2:  //two phase short circuit	两相短路
	//	case 3:	 //three phase short circuit	三相短路
	//	case 4:  //two phase short to ground	两相短路接地
	int		type;//短路故障类型，

	int		ibs;//故障点的计算母线，这个是主要的计算参数

	char 	error[20];//获取故障类型的错误描述
	time_t	start;	//故障开始时间
	float	second;	//秒
	float	r;
	float	x;
	float	r0;//零序
	float	x0;
	float	v;//故障点的稳态电压
	float	a;
	float	i1;//正序
	float	a1;
	float	i2;//负序
	float	a2;
	float	i0;//零序
	float	a0;
	float	ia;//A相电流/电压
	float	aa;
	float	ib;
	float	ab;
	float	ic;
	float	ac;
};

//因子表的上三角
#define mx_fu		1000
typedef struct G_FU   
{
    int    i;	//行
    int    j;	//列
    int    p;	//列的链表，从第一个开始，一直循环到最后
    int    d;	//对角
    double g;	//
    double b;	//

} G_FU;

class CSC : public PWRFLOW  
{
public:
	DSBOB2*	dsbob2;

	CSC(DSBOB2* m_pDb, CLLD* m_pLLD);
	virtual ~CSC();
	CLLD* lld;

public:
	//矩阵因子表
	G_FU*	g_fu;
	int		lv_fu;

	//序网的阻抗值
	vector<double> fr;
	vector<double> fx;
	vector<double> fr0;
	vector<double> fx0;

	FAULT*	fault;
	
	ofstream outfile;//流文件输出结果
	string bs_sc[mx_bs_sc];//节点短路电流字符串，以计算母线编号为序


public:
	//在潮流计算类里面已经有形成导纳矩阵的程序
	//直接计算短路电流就可以了

	//根据导纳矩阵形成因子表
	int FormFactorTable();
	int SolveStatic(int type=1);//默认正序	
	int Calc3Series();
	int CalculateFaultCurrent3(int faulttype,char  * output);

	int OnMathSC(char * Efile_name,int type=0);
	int PutResult();

};

#endif // !defined(AFX_SC_H__2D1BE096_4FA7_4086_A8B1_4FAC70B40C28__INCLUDED_)
