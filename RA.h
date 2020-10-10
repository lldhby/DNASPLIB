// RA.h: interface for the CRA class.
//
//////////////////////////////////////////////////////////////////////
#include "../DNASPLIB/LLD.h"
#include "../DNASPLIB/dsbob2.h"
#include "../DNASPLIB/netmom.h"
#include "../DNASPLIB/tmpodrtb.h"

//FEMA���ɿ��Լ������NM1����
#include "../DNASPLIB/NM1.h"

#if !defined(AFX_RA_H__FA1D9B38_4553_4275_9A97_A19D3A7250E5__INCLUDED_)
#define AFX_RA_H__FA1D9B38_4553_4275_9A97_A19D3A7250E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
//�ɿ��Լ���ר�õ�Ԥ����
const int maxLoad=500;//�������ϵ���ฺ������
const int maxLoad_bak=2000;//�������ϵ���ฺ������
const int maxUnion=30;//�������ϵ�������翪������
const int maxEquipment=1000;//�������ϵ�����豸����
//const int maxPath=50;//�������ɵ���С·����

#define     EQUIPMENT_BREAKER	1	//�豸�Ƿ��Ѵ���
#define     EQUIPMENT_RELAY		2	//��������
#define     EQUIPMENT_LOADER	4	//���ɿ���,�����������Ķ�·��
#define     EQUIPMENT_DISCONN	8	//���뿪��
#define     EQUIPMENT_FUSE		16	//�۶���
#define		UNION_TRAN 5;//Ĭ�����翪�ص�ת��ԣ�� 5MVA

typedef struct _LD_RESULT//�ɿ��ԵĽ��
{
//	long	id;
	//int	type;//0-ϵͳ��1-���ߣ�2-�߶�
	
	char id[MAX_SEC_ID_LEN];//����id
	char fdid[MAX_SEC_ID_LEN];//��������
	
	double perFault;//������
	double perPlan;//������
	double perSum;//�ƻ�������

	double perFaultTime;//������*����ʱ��
	double perPlanTime;//������*����ʱ��
	double perSumTime;//������*����ʱ��

	double s;//����ֵ	

} _LD_RESULT;

//unsigned int __stdcall doMath(void *pPM);  
//////////////////////////////////////////////////////////////////////////
class CRA : public CNM1
{
public:
	CRA();
	CRA(DSBOB2* m_pDb, CLLD* m_pLLD);
	virtual ~CRA();
	CLLD* lld;

public:
	DSBOB2* dsbob2;

//	 sFileName[128];//�����Efile�ļ������е��̶߳�������д�ļ�

public:
	int OnMath(char* Efile,int type=0);//0Ϊ����Ҫ����nm1

//////////////////////////////////////////////////////////////////////////
//FEMA�������
	int doFEMA(int type=0);
	int WriteFEMA(int fdph);  


	typedef struct dev_
	{
		char id[64];
		char name[32];
	};
	dev_ devtest[31];
	//char name[32];
	char* OnGetName(char* id);
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//��С·�ɿ��Լ������
	int doCalc();  
	//������ĩ�ڵ�ţ������豸��ָ��
	int getEquipID(DSBOB2* dsbob2,int start, int end);
	int getUNCB_abun(DSBOB2* dsbob2,char* fdid,char* cb_id,double& p_abund,double& q_abund,double& v_abund);
	int equipCanTrans(DSBOB2* dsbob2,MINPATH* ldpath,int ldid,int cntload,MINPATH* bakpath,int cntbakpath,int eq_ind);
	int isDisconnect(DSBOB2* dsbob2,int* minPath_src,int start, int end);//ͨ�����ɵĽڵ�ţ��õ����ɵ���С·
	int getCBType(DSBOB2* dsbob2,int start, int end);
	int getFirstCB(DSBOB2* dsbob2,int* minPath,int p);//ͨ�����ɵĽڵ�ţ��õ����ɵ���С·
//////////////////////////////////////////////////////////////////////////

private:
};

#endif // !defined(AFX_RA_H__FA1D9B38_4553_4275_9A97_A19D3A7250E5__INCLUDED_)
