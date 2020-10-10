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
//  G_Rdi - �޹��ۣ��ӵ�Դ�㵽����ڵ����̾���,��ĸ�߸����Ƕ�Ӧ��
//����������ʱ�����������ֱ��д��
#define mx_rdi		1000
typedef struct G_Rdi
{
	int	ibs;//ĸ�߱�ţ��Ż���ź�
	int	ibsp;//���ڵ�ı��	
	int	ibr;//ĸ�����ε��߶α�ţ�ָ�룩
	double	dr;//�ڵ���޹�������������ýڵ���޹�ֵ
	double	v;//�ڵ�ĵ�ѹ
	double	r;//�ڵ������֧·r
	int pivot;//1 Ϊ����㣬Ĭ��-1

	int	downstream[MXELE];//���νڵ������
	int	n_downstream;//���νڵ�ĸ���

	double	linearRdi;//һ���޹���,�Ӹýڵ�һֱ����Դ��Rk/Uk^2+Rk-1/Uk-1^2+...+R1/U1^2
	double	quanRdi;//�����޹��ۣ��Ӹýڵ�һֱ����Դ���ȫ������֮�ͣ�Rk+Rk-1+...+R1

	//double	linearQ;//һ���޹���ȷ��
	double	quanQ;//�����޹���ȷ�أ��������·��ĩ�ˣ�ֱ�Ӹ�0

	double	Qv;//��������
	double	b_cosfi;//����ǰ�Ĺ�������
	double	b_v;//����ǰ�ĵ�ѹ
	double	a_cosfi;//������Ĺ�������
	double	a_v;//������ĵ�ѹ

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
////���װ���ſɱȾ���ǶԽ�Ԫ�ص����飬�±��0��ʼ
//#define mx_ju		1000
//typedef struct G_JU_T    
//{
//    int    j;//�ھ����е��б��
//    int    d;//�ǶԽ�Ԫ��������Ӧ�����е�λ�ã���Ҫ����һ�����õ����е�Ԫ�ظ���
//    int    k;//��Ԫ�صĶ�Ӧ���߶εı������
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
//	int	ibs;//ĸ�߱�ţ��Ż���ź�
//	int	ibsp;//���ڵ�ı��	
//	int	ibr;//ĸ�����ε��߶α�ţ�ָ�룩
//	double	dr;//�ڵ���޹�������������ýڵ���޹�ֵ
//	double	v;//�ڵ�ĵ�ѹ
//	double	r;//�ڵ������֧·r
//	int pivot;//1 Ϊ����㣬Ĭ��-1
//
//	int	downstream[MXELE];//���νڵ������
//	int	n_downstream;//���νڵ�ĸ���
//
//	double	quanRdi;//�����޹��ۣ��Ӹýڵ�һֱ����Դ���ȫ������֮�ͣ�Rk+Rk-1+...+R1
//
//	double	quanQ;//�����޹���ȷ�أ��������·��ĩ�ˣ�ֱ�Ӹ�0
//
//	double	Qv;//��������
//	double	b_cosfi;//����ǰ�Ĺ�������
//	double	b_v;//����ǰ�ĵ�ѹ
//	double	a_cosfi;//������Ĺ�������
//	double	a_v;//������ĵ�ѹ
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

	//��λ���������豸Ͷ��Kc:100Ԫ/kVar;���belta��0.5Ԫ/kW.h��ȫ����󸺺����Сʱ��tiao��2000
	double alpha/*�۾�ά����*/,garma/*Ͷ�ʻ�����*/;
	double belta/*���*/,Kc,/*��λ���������豸Ͷ��*/tiao/*ȫ����󸺺����Сʱ��*/;

	//0 ��һ�׶�û�гɹ�����Ҫ�ڶ��β�����1 �ɹ� 
	int firstRPO();

	void resetRdi();//��ʼ���޹���
	int getRdi();//����ڵ���޹��߼���ϵ
	int getQuadQ();//�����޹����ξ�ȷ��

	int getPivot(int* pivot);//��������㣬��������Ͻ����Ż�,һ��ֻ��һ�������
	int getSite();//ͨ�����ξػ�����΢������ȡ������
	double get_Q(int g_wb_id);//���ݲ�������ȡ�������

//////////////////////////////////////////////////////////////////////////
//�������������΢���ʼ���
//ֱ�����ſɱȾ����Ͻ��м��㣬��Ҫ�ȵõ��ſɱȾ����ÿ��Ԫ�����ݣ�
//���ſɱȾ�����һ��ת�ã�Ȼ������˹���õ���ص��޹�΢����

	int getTmattrice();//�õ��ſɱȾ����ת�þ���
	int getArrayQ();//�õ��޹�����΢���ʣ�������
	double getEconomy();//�õ����õ���

//	G_JD_T*	g_jd_t;
//	int		lv_jd_t;
//	G_JU_T*	g_ju_t;
//	int		lv_ju_t;
//	G_JL_T*	g_jl_t;
//	int		lv_jl_t;
	double  g_Q_inc[1000];//�޹�����΢����
	

//////////////////////////////////////////////////////////////////////////
//������������޹��Ż���ȷ�ؼ���
	G_Rdi*	g_rdi;//��ӳ�޹��ľ�ȷ���ξ�
	int		lv_rdi;	
	int	rdi_bs1/*�����޹���ȷ�����Ĳ�����1*/,rdi_bs2,rdi_bs3;

	double g_wloss_b,g_rloss_b,g_wloss_a,g_rloss_a;

public:
	void InitWorkingArray_1(int type=1);//�������޹��Ż���ʼ��

	int isNeedRPO();//�����Ƿ���Ҫ�����޹��Ż�
	//������ѹ 0 ��ѹ��ȫ�ϸ񣬲��貹����-1 �����ܲ�����λ -2 ���Բ�����λ -3 ��ѹԽ����

	
	//0 �ڶ��׶�û�гɹ�����Ҫ�����ѹ���ƽ����1 �ɹ� 
	int secondRPO();
	
	
	//int  PutNDRDI(int g_wb_id/*�ڵ��Ż���ı��*/);
	double getNDRDI(int g_wb_id/*�ڵ��Ż���ı��*/);//����ÿ���ڵ���޹��ۣ�׼�������޹���ȷ��
	int buildRdi();//��ÿ���ڵ���޹���
	int PutND_QuadQ();//�����޹����ξ�ȷ��
	double getND_DelQ(int g_wb_id/*�ڵ��Ż���ı��*/);//����ڵ�һ���޹���
	int setCP(int rdi_bs2,double& Q,int num);//rdi_bs2������,double Q ��������
	int Sweep(int rdi_bs2, double Qv );//����У��

	//�жϲ����������μ���õ��Ľ�����������Խ��
	//1 ������û��Խ�ޣ�0 �������ѹԽ��
	int isVoltageViolate(int oragin=0);//Ĭ�ϲ���ԭʼ��ѹ
	
	double getCosfi(double w,double r);//��������
	int putRPO(int lev=2);//����޹��Ż��Ľ����Ĭ��Ϊ2�׶�

};

#endif // !defined(AFX_RO_H__C1C5E597_B94D_4104_90F5_666D53C4F4BE__INCLUDED_)
