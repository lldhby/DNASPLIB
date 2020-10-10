// RA.h: interface for the CRA class.
//
//////////////////////////////////////////////////////////////////////
#include "../DNASPLIB/LLD.h"
#include "../DNASPLIB/dsbob2.h"
#include "../DNASPLIB/netmom.h"
#include "../DNASPLIB/tmpodrtb.h"

//FEMA法可靠性计算基于NM1计算
#include "../DNASPLIB/NM1.h"

#if !defined(AFX_RA_H__FA1D9B38_4553_4275_9A97_A19D3A7250E5__INCLUDED_)
#define AFX_RA_H__FA1D9B38_4553_4275_9A97_A19D3A7250E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
//可靠性计算专用的预设量
const int maxLoad=500;//单条线上的最多负荷数量
const int maxLoad_bak=2000;//单条线上的最多负荷数量
const int maxUnion=30;//单条线上的最多联络开关数量
const int maxEquipment=1000;//单条线上的最多设备数量
//const int maxPath=50;//单个负荷的最小路长度

#define     EQUIPMENT_BREAKER	1	//设备是否已处理
#define     EQUIPMENT_RELAY		2	//保护开关
#define     EQUIPMENT_LOADER	4	//负荷开关,含不带保护的断路器
#define     EQUIPMENT_DISCONN	8	//隔离开关
#define     EQUIPMENT_FUSE		16	//熔断器
#define		UNION_TRAN 5;//默认联络开关的转供裕度 5MVA

typedef struct _LD_RESULT//可靠性的结果
{
//	long	id;
	//int	type;//0-系统；1-馈线；2-线段
	
	char id[MAX_SEC_ID_LEN];//负荷id
	char fdid[MAX_SEC_ID_LEN];//所属馈线
	
	double perFault;//故障率
	double perPlan;//故障率
	double perSum;//计划检修率

	double perFaultTime;//故障率*故障时间
	double perPlanTime;//故障率*故障时间
	double perSumTime;//检修率*检修时间

	double s;//负荷值	

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

//	 sFileName[128];//输出的Efile文件，所有的线程都结束后写文件

public:
	int OnMath(char* Efile,int type=0);//0为不需要计算nm1

//////////////////////////////////////////////////////////////////////////
//FEMA计算程序
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
//最小路可靠性计算程序
	int doCalc();  
	//给定首末节点号，返回设备的指针
	int getEquipID(DSBOB2* dsbob2,int start, int end);
	int getUNCB_abun(DSBOB2* dsbob2,char* fdid,char* cb_id,double& p_abund,double& q_abund,double& v_abund);
	int equipCanTrans(DSBOB2* dsbob2,MINPATH* ldpath,int ldid,int cntload,MINPATH* bakpath,int cntbakpath,int eq_ind);
	int isDisconnect(DSBOB2* dsbob2,int* minPath_src,int start, int end);//通过负荷的节点号，得到负荷的最小路
	int getCBType(DSBOB2* dsbob2,int start, int end);
	int getFirstCB(DSBOB2* dsbob2,int* minPath,int p);//通过负荷的节点号，得到负荷的最小路
//////////////////////////////////////////////////////////////////////////

private:
};

#endif // !defined(AFX_RA_H__FA1D9B38_4553_4275_9A97_A19D3A7250E5__INCLUDED_)
