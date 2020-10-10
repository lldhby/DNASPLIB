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

//��������,ͨ����������������·�ļ���ĸ��
//���еĹ��϶����Թ��㵽����ĸ����
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  FAULT
{
public:

	char 	id[MAX_BUS_ID_LEN];	//�ٶ����еĹ��϶�������ĸ���ϣ�����Զ�������С��Ԫ
	char     descr[MAX_BUS_DESCR_LEN];

	//	case 1:  //single phase short to ground	�����·�ӵ�
	//	case 2:  //two phase short circuit	�����·
	//	case 3:	 //three phase short circuit	�����·
	//	case 4:  //two phase short to ground	�����·�ӵ�
	int		type;//��·�������ͣ�

	int		ibs;//���ϵ�ļ���ĸ�ߣ��������Ҫ�ļ������

	char 	error[20];//��ȡ�������͵Ĵ�������
	time_t	start;	//���Ͽ�ʼʱ��
	float	second;	//��
	float	r;
	float	x;
	float	r0;//����
	float	x0;
	float	v;//���ϵ����̬��ѹ
	float	a;
	float	i1;//����
	float	a1;
	float	i2;//����
	float	a2;
	float	i0;//����
	float	a0;
	float	ia;//A�����/��ѹ
	float	aa;
	float	ib;
	float	ab;
	float	ic;
	float	ac;
};

//���ӱ��������
#define mx_fu		1000
typedef struct G_FU   
{
    int    i;	//��
    int    j;	//��
    int    p;	//�е������ӵ�һ����ʼ��һֱѭ�������
    int    d;	//�Խ�
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
	//�������ӱ�
	G_FU*	g_fu;
	int		lv_fu;

	//�������迹ֵ
	vector<double> fr;
	vector<double> fx;
	vector<double> fr0;
	vector<double> fx0;

	FAULT*	fault;
	
	ofstream outfile;//���ļ�������
	string bs_sc[mx_bs_sc];//�ڵ��·�����ַ������Լ���ĸ�߱��Ϊ��


public:
	//�ڳ��������������Ѿ����γɵ��ɾ���ĳ���
	//ֱ�Ӽ����·�����Ϳ�����

	//���ݵ��ɾ����γ����ӱ�
	int FormFactorTable();
	int SolveStatic(int type=1);//Ĭ������	
	int Calc3Series();
	int CalculateFaultCurrent3(int faulttype,char  * output);

	int OnMathSC(char * Efile_name,int type=0);
	int PutResult();

};

#endif // !defined(AFX_SC_H__2D1BE096_4FA7_4086_A8B1_4FAC70B40C28__INCLUDED_)
