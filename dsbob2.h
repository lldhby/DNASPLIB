// DSBOB2.h: interface for the DSBOB2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSBOB2_H__8417E7D5_DBF5_4B7D_8DE4_6FFC1F6E5094__INCLUDED_)
#define AFX_DSBOB2_H__8417E7D5_DBF5_4B7D_8DE4_6FFC1F6E5094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _WIN32
//#include "C:\DNASP\DPCAS4.1\WIN\calcdll\lib.h"
//#include "../DPCAS4.1/WINDLL/calcdll/lib.h"
//#include "../DNASPLIB/netmom.h"		// Network model definition
//#include "../DNASPLIB/tmpodrtb.h"	// Table management template for database object
//#include "../DNASPLIB/LLD.h"
#elif __linux__
#else
#endif

#include "../DNASPLIB/netmom.h"		// Network model definition
#include "../DNASPLIB/tmpodrtb.h"	// Table management template for database object
#include "../DNASPLIB/LLD.h"

//��ѹ�ȼ�����Ϊ�漰���ĵ�ѹ�ȼ����٣�����Ҫ��һ������ά��
typedef struct BASEVL
{
	//@Num	mRID	name	nomkV
	//���	��ʶ	��׼��ѹ��	��׼��ѹ
	int	Num;		//���
	char mRID[80];	//��ʶ
	char name[80];	//��׼��ѹ��
	double nomkV;	//��׼��ѹ
} BASEVL;

typedef struct NM1_RESULT//n-1�Ľ��
{
	long	id;
	char fdid[MAX_ID_DLL];//����id
	char secid[MAX_ID_DLL];//���߶�id
	double Faultload;//���϶�ʧ�縺��(MW)
	double Trload;//�ǹ��϶���ת�Ƹ���(MW)
	double NowTrload;//�ǹ��϶�ʵ��ת�Ƹ���(MW)
	double PowerLostRatio;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
	char unionList[MAX_UNIN_ID_SUM];// ת����Դ
	char secResult;//�߶�ͨ��n-1
	char LineResult;//������ͨ��n-1

	char transSec[FEMA_ID_LEN];
	char lossSec[FEMA_ID_LEN];
	//char zoneLoss[FEMA_ID_LEN];
} NM1_RESULT;

typedef struct RO_RESULT//�޹������Ľ��
{
	long	id;
	char fdid[80];//����id
	char ndid[80];//�����Ͻڵ�id

	double Qv;//��������
	char b_cosfi[10];//����ǰ��������
	char a_cosfi[10];//������������

	double b_v;//����ǰ��ѹ
	double a_v;//�������ѹ

} RO_RESULT;

typedef struct RA_RESULT//�ɿ��ԵĽ��
{
//	long	id;
	int	type;//0-ϵͳ��1-���ߣ�2-�߶�
	
	char eqid[MAX_SEC_ID_LEN];//����id
	char fdid[MAX_SEC_ID_LEN];//��������
	
	double ASAI_Fault;//�ɿ���(ֻ���ǹ���ͣ��%)
	double SAIFI_Fault;//ϵͳƽ��ͣ��Ƶ��(��/��)
	double SAIDI_Fault;//ϵͳƽ��ͣ��ʱ��(���ǹ���ͣ����Ԥ����ͣ�磬Сʱ/��)
	double ENS_Fault;//ϵͳƽ��ͣ��ʱ��(���ǹ���ͣ�磬Сʱ/��)

	double ASAI_Sum;//�ɿ���(���ǹ���ͣ����Ԥ����ͣ��%)
	double SAIFI_Sum;//ϵͳƽ��ͣ��Ƶ��(��/��)
	double SAIDI_Sum;//ϵͳƽ��ͣ��ʱ��(���ǹ���ͣ����Ԥ����ͣ�磬Сʱ/��)
	double ENS_Sum;//ϵͳƽ��ͣ��ʱ��(���ǹ���ͣ�磬Сʱ/��)

} RA_RESULT;

//���ߵķֶ���Ϣ
typedef struct SECT_INFO
{
	string fdid;         //����ID
	string fdname;
	string version;      //�汾��
	string calcDate;     //��������
	int sect_id;         //�ֶ����
	string cut;          //�ϵ�
	int sumLD;
	int sumVip;
	vector<string> idList;//�ֶεĸ���ID�ż���
	vector<string> secList;//�ֶε��߶�ID�ż���
	double sumRatedS;
	double sumW;
	double sumLen;
	int sumUN;

	//����
	int cntsect;//�ֶ�����
				//Ȩ��
	double wW;  //���ɴ�С
	double wLD; //���ɸ���
	double wLen;//�ֶγ���
	double wUN; //���翪��Լ��
	double wVIP;//��Ҫ�û�Լ��
	double wSecW;//�ֶθ��ɴ�СԼ��
				 //�Ż�����
	double ruoW;  //���ɴ�С
	double ruoLD; //���ɸ���
	double ruoLen;//�ֶγ���
	double punishUN; //���翪��Լ��
	double punishVIP;//��Ҫ�û�Լ��
	double punishSecW;//�ֶθ��ɴ�СԼ��

}SECT_INFO;

//unsigned int __stdcall doReadDB(void* pThread);  

class DSBOB2 
{
public:
	DSBOB2();
	DSBOB2(CLLD* m_lld);
	virtual ~DSBOB2();

	CLLD* lld;

	int getDevType(char * line, char * devType, int& n_line, int &flag_b);
	//���ݴ����sSQL��䣬���캬�ڽ����ߵ��µ�SQL���
	//CString getAdjSQL(CString lFdid);

	void inputmem(int type=0);
	void deletemem();//ɾ����ʱ�����ڴ�
	
	int ReadAnalogData();
	int ReadDigitalData();

	//��������ݵ����
	//�������

//T0	1	1	1	0.02
//T1	5	1	3	0.025
//T2	3	1	2	0.050000001
//T3	17	1	9	0.0125
//T4	5	1	2	0.025
//T5	7	1	5	0.025
//T6	7	1	4	0.025

	//lld edit 2018-4-25
#if _WIN32
	//int ReadData(
	//	   SOT* so,int cntSO,
	//	   CBT* cb,int cntCB,
	//	   BUST* bs,int cntBUS,
	//	   SECT* sec,int cntSEC,
	//	   XFMRT* xfmr,int cntXFMR,
	//	   TAPTYT* tapty,int cntTAPTY,
	//	   LDT* ld,int cntLD,
	//	   CPT* cp,int cntCP,
	//	   NDT* nd,int cntND
	//			);
#endif

	//���ı��ļ���������ݣ����ļ��ĸ�ʽΪ�Զ��壬��E�ļ�
	//��Ҫ�������linux��ʱ��
	int ReadDataFromFile(char * FileName, char * optParam);

	//��ȡE��ʽ�����ļ�,����Ƕ�·����������Ϊ1���ӵؼ�����
	int ReadDataFromEfile(char * FileName);
	//��CIME��������ݣ���ѹ�������
	int ReadCIME(char * filename);
	//���˷���ר�ã�ֻ����д��·���ɣ����溬����ĩ�ڵ㣬�ڵ������
	int ReadDataFromTPfile(char * sFileName, char * traceSub);


	DTable<FD>* fd_tb_t;
	int OnGetPhByfdid(char* fdid,int type=0);
	int OnGetBsByND(char * nd);

	//���߳���������
	//int ReadDataByThread();
	
//	//�����������������efile�ļ��У������˻�������ʾ,type==0,���ȫ�������ݣ�����Ϊ������
	int WriteOutfile_pf(char* sFileName,int type=100);

	//�����·������1.0�汾Ϊ3��������Ժ�����ȫ����2.0�������
	//int WriteOutfile_ra(char* sFileName);
	int WriteOutfile_nm1(char* sFileName);	

	int WriteOutfile_ro(char* sFileName);

	int WriteOutfile_optsect(char* sFileName, int sect_id);
//	int WriteOutfile_optsect(char* sFileName, optSect* pOptSect);

	
public:
	NM1_RESULT nm1_result[5000];
	RO_RESULT ro_result[2];
	RA_RESULT ra_result[500];//ÿ���������200������
	SECT_INFO fdsect[6];
	char outFileName[128];

	BASEVL	basevl[10];//��������ֻ��110,66,35,10,20,6�ȼ�����ѹ�ȼ�
	//ͨ����ѹ�ȼ�����õ���ѹ�ȼ���ʵ�ʱ�׼ֵ
	//mrID-��ѹ�ȼ���ʶ��Ĭ��Ϊtype=0�������׼��ѹ�ȼ���Ĭ��Ϊ1
	float getVLBymrID(char * mrID, int type = 0);

public:

	//�������������ڶ���������õ�ʵ�����nmd�õ���ϵ��
	//table objects for network model database
	DTable<GLOBAL>*           global_tb;

	DTable<ANALOG>*           analog_tb;
	DTable<DIGITAL>*          digital_tb;

	DTable<CO>*               co_tb;
	DTable<ST>*               st_tb;
	DTable<SO>*               so_tb;
	DTable<BUS>*              bus_tb;
	DTable<CB>*               cb_tb;
	DTable<SEC>*              sec_tb;
	DTable<LD>*               ld_tb;
	DTable<XFMR>*             xfmr_tb;
	DTable<XF>*               xf_tb;
	DTable<TAPTY>*            tapty_tb;
	DTable<CP>*               cp_tb;

	DTable<ISLAND>*           island_tb;
	DTable<BS>*               bs_tb;
	
	DTable<ND>*               nd_tb;
	//lld add 2019-5-31
	DTable<ND>*               nd_tb_t;
	DTable<BUS>*              bus_tb_t;
	DTable<CB>*               cb_tb_t;
	DTable<SEC>*              sec_tb_t;
	DTable<LD>*               ld_tb_t;
	DTable<XFMR>*             xfmr_tb_t;
	DTable<XF>*               xf_tb_t;
	DTable<SO>*               so_tb_t;
	DTable<CP>*               cp_tb_t;
	DTable<ST>*               st_tb_t;
	DTable<VL>*               vl_tb_t;
	DTable<VL>*               vl_tb;
	DTable<SECP>*             secp_tb_t;
	DTable<TAPTY>*            tapty_tb_t;

	DTable<R_CO_ST>*	r_co_st_tb;
	DTable<R_ST_XFMR>*	r_st_xfmr_tb;
	DTable<R_ST_CB>*	r_st_cb_tb;
	DTable<R_ST_SO>*	r_st_so_tb;
	DTable<R_ST_LD>*	r_st_ld_tb;
	DTable<R_ST_CP>*	r_st_cp_tb;
	DTable<R_ST_BUS>*	r_st_bus_tb;
	//lld end add 2019-5-31

	DTable<R_ND_LD>*          r_nd_ld_tb;
	DTable<R_ND_CB>*          r_nd_cb_tb;
	DTable<R_ND_SEC>*         r_nd_sec_tb;
	DTable<R_ND_CP>*         r_nd_cp_tb;
	DTable<R_ND_XF>*          r_nd_xf_tb;
	DTable<R_XFMR_XF>*        r_xfmr_xf_tb;

	DTable<ZN>*               zn_tb;
	DTable<R_ZN_LD>*          r_zn_ld_tb;
	DTable<R_ZN_SEC>*         r_zn_sec_tb;
	DTable<R_ZN_CB>*          r_zn_cb_tb;
	DTable<R_ZN_CP>*          r_zn_cp_tb;
	DTable<R_ZN_XF>*          r_zn_xf_tb;

	//lld add 2015-12-31
	DTable<UN>*				  un_tb;//���翪�ر�
	DTable<FD>*               fd_tb;//���߱�
	DTable<R_FD_UN>*          r_fd_un_tb;//��������
	DTable<R_FD_SO>*          r_fd_so_tb;
	DTable<R_FD_ZN>*		  r_fd_zn_tb;
	DTable<R_FD_EQ>*		  r_fd_eq_tb;
	DTable<R_FD_LD>*		  r_fd_ld_tb;

	//lld add 2016-1-12
	DTable<EQ>*				  eq_tb;//ȫ���Ŀɿ��Լ����豸
	DTable<R_ND_EQ>*		  r_nd_eq_tb;//�豸��ڵ�Ķ�Ӧ��ϵ
};

#endif // !defined(AFX_DSBOB2_H__8417E7D5_DBF5_4B7D_8DE4_6FFC1F6E5094__INCLUDED_)
