// NM1.h: interface for the CNM1 class.
//developed by Dr.lld 2004-2-5 for distribution network plannning platform
//ncepu 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NM1_H__984F6C62_9471_4ADB_85C1_0C973334F1C5__INCLUDED_)
#define AFX_NM1_H__984F6C62_9471_4ADB_85C1_0C973334F1C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../DNASPLIB/LLD.h"
#include "../DNASPLIB/dsbob2.h"
#include "../DNASPLIB/pwrflow.h"

const int MAX_ZNNO =  20;//�����δ��ڵ����߽翪�ؽڵ㣬�����õ�20������Ҫʱ���޸�
const int MAX_UN = 20;//������翪��
const int MAX_LD = 150;//��ฺ��
const int MAX_CP = 10;//���cp
const int MAX_SEC = 300;//����߶�
const int MAX_CB = 400;//��࿪��
const int MAX_ND = 500;//���ڵ�

const int MAX_UION_ID = 4000;//�������翪��������ռ�ĳ���
const int MAX_UION_COMPOSITE = 200;//�������翪�������
//const int MAX_RA_CNT = 10000;//���ĸ����ڿɿ��Լ����еĸ���

//�������n-1ʵ�����Ǹ���Ŀ������Ż�����
//Ŀ�꣺������ת�������Ҫ�û��;�����ת����ĸ���
//Լ�������ߵ�����Լ���ͳ���Լ������ѹ����Խ�ޣ�
//֧·����������������Ž⣬��һ����ȫ�����Ž⣬���ٶȿ죬����̰���㷨
//ÿ���зֹ��絺ʱ������������ÿ�����ĸ��ɵ�Դƽ�����Ҫ����
//��ȫ���ĸ��ɵ�����磨���õ�Դ�㣩��Ҫ���㵽������·��

//�������n-1��ͬ����������Ҫ������Ϻ���������ת��
//Ҳ��ͬ�������ع�������ֻ��Ҫ�ڹ��Ϻ󾡿��ܶ��ת�����ɣ���һ��Ҫ�Ż�����
//���֧·����������ĳ�����翪�ؼ����ת�����ܳɹ���ÿ�μ���һ�����翪�أ��⻷
//ÿ����һ�����翪�غ󣬲�һ����ת�����㹻�ĸ���

//��ÿ�����翪�ؾ������ת�����ɣ��ر�����Ҫ�û��ĸ���
//�ڲ�����������˷����У���������·���������ɵĿ��أ�ȫ����������

//��������ķֲ�ʽ��Դ�Ľṹ
typedef struct NM1_SOLIST
{
	int	nd;//��Դ�ڵ���

	int soptr;//��Դ�������
	char secid[MAX_SEC_ID_LEN];//���������߶�id
	double p;//(MW)��	�й�����
	double q;//(MVar)���޹�����
	double v;//(kV)����ѹֵ
	double pq;//MVA����Դ���ڹ���ԣ��
} NM1_SOLIST;

//������������翪�ؽṹ
typedef struct NM1_UNLIST
{
	int	nd;//����ڵ��ţ������ڵ���һ��ı��
	int main_nd;//���翪�����㵽�������ϵĽڵ�

//	 unid[MAX_CB_ID_LEN];//���翪��id
	int cbptr;//���翪�ص������
	char secid[MAX_SEC_ID_LEN];//���������߶�id
//	int secptr;//�������ߵ������
	double p_abund;//ԣ��(MW)��	�й�ԣ��
	double q_abund;//ԣ��(MVar)���޹�ԣ��
	double v_abund;//ԣ��(MW)����ѹֵ
	double pq_abund;//���翪�ص�ʵ�ڹ���ԣ��
} NM1_UNLIST;

//������������зֶο���
typedef struct NM1_CBLIST
{
	int	cb_ptr;//�ֶο��ص�ָ�루����ţ�
//	 cb_id[MAX_CB_ID_LEN];//�ֶο��صı��
//	int island;//�ֶο��������Ĳ��ൺ���
	int	normalOpen;//���ڳ����ķֶο���
} NM1_CBLIST;

//���������ڵ����и���
typedef struct NM1_LDLIST
{
	int	ld_ptr;//���ɵ��ָ��
//	 ld_id[MAX_LD_ID_LEN];//���ɱ��
	int nd;//���ɵĽڵ���
	int main_nd;//�����������������ϵĽڵ�
	
	double p_ld;//�й�(MW)
	double q_ld;//�޹�(MVar)
	double pq_ld;//���ڹ���
	int	soSum;//��Դ��ĸ�������Ҫ����˫��Դ��
	int	vip;//��Դ�����Ҫ�̶ȣ�0-һ���û���1-�������ɣ�2-�������ɣ�3-һ������
} NM1_LDLIST;

//���������ڵ����е�����
typedef struct NM1_CPLIST
{
	int	cp_ptr;//��������ָ��
	int nd;//cp�Ľڵ���
	
	int main_nd;//�������������������ϵĽڵ�
	double r;//�й�(MW)
} NM1_CPLIST;

//���������ڵ����нڵ�
typedef struct NM1_NDLIST
{
	int	nd_ptr;//���ɵ��ָ��
//	 nd_id[MAX_ND_ID_LEN];//���ɱ��
	int ibs;//�ڵ��ĸ�߱�ţ���ԭ�������еı��
	int primebs;//�ڵ����ʱĸ�߱�ţ����ƻ�ԭ�е�����

	double ld_sum;//�ڵ��ϵĵ�ֵ��������
	double so_sum;//�ڵ��ϵĵ�ֵ��Դ����
} NM1_NDLIST;

//���������ڵ�����֧·�����迹
typedef struct NM1_SECLIST
{
	int	sec_ptr;//���ɵ��ָ��
//	 sec_id[MAX_SEC_ID_LEN];//���ɱ��
	int nd;//�׽ڵ�
	int znd;//ĩ�ڵ�
	double r;//����
	double x;//�翹�����������ʱ��ʹ��

	double pq;//���߶������������ڹ���(MW)���ڼ����зֿ��ص�ʱ������
} NM1_SECLIST;

//���������ڵ����е�����
typedef struct NM1_ISLANDLIST
{
	int	island;//�����
	
	NM1_SOLIST island_solist[MAX_UN];//һ������������10���ֲ�ʽ��Դ��
	int soCount;//�������翪�صĸ���

	NM1_UNLIST island_unlist[MAX_UN];//һ������������10�����翪��
	int unCount;//�������翪�صĸ���

	NM1_LDLIST island_ldlist[MAX_LD];//һ������������100�����ɵ�
	int ldCount;//������ĸ��ɸ���

	NM1_CPLIST island_cplist[MAX_CP];//һ������������10��������������
	int cpCount;//������ĸ��ɸ���

	NM1_SECLIST island_seclist[MAX_SEC];//һ������������100���߶�
	int secCount;//������ĸ��ɸ���

	NM1_CBLIST island_cblist[MAX_CB];//һ������������100���߶�
	int cbCount;//������ĸ��ɸ���

	NM1_NDLIST island_ndlist[MAX_ND];//һ������������100���߶�
	int ndCount;//������ĸ��ɸ���

	int canTrans;//���ڵĸ����Ƿ����ת��
	char trans_unList[MAX_UION_ID];//����ת�����ɵ����翪���б�
	int validate;//�õ��Ƿ��Ѿ�У�����

} NM1_ISLANDLIST;

//�������翪��֮�����С·
typedef struct NM1_Minpath
{
	int cbptr[MAX_CB];//��С·�ϵĿ��أ����صĽ���������ʵ��
//	int secptr[100];//��С·�ϵ�֧·��֧·�����Ŀ������Ƚ�
	
	int cbCount;//��������
//	int secCount;//֧·����
	//double loopR;//��С·�ϵĵ���ֵ
	//double delV;//��ѹ��

	int composite_nd[MAX_ND];//��С·�ϵĽڵ㼯�ϣ�����ÿ���ڵ㶼���и��ɺ͵�Դ
//	int composite_fch[100];//��С·�ϵĽڵ��ǰ����
//	int composite_bch[100];//��С·�ϵĽڵ�ĺ�����

	double composite_so[MAX_ND];//��С·�ϵĽڵ���ۺϷ��磬��ԴΪ��
	double composite_ld[MAX_ND];//��С·�ϵĽڵ���ۺϸ��ɣ�����Ϊ��
	int composite_ld_vip[MAX_ND];//��С·�ϵ���Ҫ�û��ȼ���Ĭ����0

	int comCount;

	int island;//���������Ĳ��ൺ���
} NM1_Minpath;

//////////////////////////////////////////////////////////////////////////
//�ɿ��Լ��������Ե������߶�ɨ����ɺ���Ҫ��д�ý��
typedef struct RA_Feeder
{
	int ld_id;//���ɱ��
	
	short out;//������ʧ

	short swit;//���ɿ����л�
	int	up_cb;//���ο��ص����ʣ�0-breaker;1-loadbreaker;2-disconnector;3-fuse;5-relay breaker

	short tran;//���ɿ���ת��
	float tranprobality;//����ת������,���ڷֲ�ʽ��Դ���������еĶ���ת��

	short mutiso;//���Դ�û�
	int sec_id;//��������������ID
} RA_Feeder;
//�ɿ��Լ������������
//////////////////////////////////////////////////////////////////////////

//�ӳ�������̳к����еĳ���������඼����������ʹ��
class CNM1 : public PWRFLOW  
{
public:
	DSBOB2* dsbob2;
	CLLD* lld;

	CNM1();
	CNM1(DSBOB2* m_pDb,CLLD* m_pLLD);
	virtual ~CNM1();

//////////////////////////////////////////////////////////////////////////
//�µ�n-1���򣬿���ȫ�����豸��2018-4-21

//1.һ���豸���Ϻ�
//	��1������豸��Ҫ�������ܲ��ܸ��������ܸ�����ȫ��ͣ��
//	��2������ܸ��������������ڵĸ����Ƕ��٣���Ӱ�츺�ɣ�
//	��3�������󣬲��������ڵĸ����ܷ�ת����
//������豸Ϊ��㣬��һ��ȫ����������һֱ�����еĵ�Դ�㣬���е����翪��Ϊֹ
	//��֤ÿ�η�������һ�����ߣ�2018-4-29
	//������������󣬸�ÿ����������ԣ�ȣ�Ҳ�����翪��������ضԲ��ԣ��
	int OnSetAbund();	
	
	char pathFlow[128];
	int fdph;
	char fdid[MAX_ID_DLL];

	int i_island;//���ı��
	//DTable<ISLAND>*       island_tb_fault;//ÿһ���豸���Ϻ��������ĵ�
	DTable<EQ>*			  eq_tb_isolate;//ȫ���ĸ����豸����Ҫ�Ǹ��࿪��
	int					  lv_eq_tb_isolate;
	//ͨ���ڵ�ŵõ��豸
	int OnGetDevByNDs(DSBOB2* dsbob2,int ind,int iznd);

	//ͨ��ID�ŵõ��豸
	int OnGetDevByID(DTable<EQ>* eq_tb, char * id);

	//������������ź����翪������ţ��õ����翪�ص�ԣ��
	double OnGetUnCBAbund(int fdph,int unph, char * oppfdid,double& p_abund,double& q_abund,double& v_abund);

	//������Դ�㣬��ֹ�ڵ㣬�������ͣ��ҵ����ε���type=0 �������� 1�л�����
	int OnGetUpIsland(int sond,int end_nd,int eqph,int type);
	
	//��ʼ����
	int OninitialIsland(int ifd);

//	int	nUpSwitchload;
//	int	nUpNonOutload;
//	int nFaultload;
//	int nNeedTrload;
//	int nNowTrload;

//	int UpStreamload[max_switch_ld];//���϶����θ����б����л����������л�ʱ������⣩
//	int Faultload[max_switch_ld];	//���϶�ʧ�縺���б�
//	int NeedTrload[max_switch_ld];//���϶�������Ҫת���ĸ����б�
//	int NowTrload[max_switch_ld];//���϶����ο���ת���ĸ����б�
	char tranfdlist[MAX_UNIN_ID_SUM];//�豸�������Ϻ��ת�������б�
	char closeunlist[MAX_UNIN_ID_SUM];//�豸�������Ϻ�� �պ� ���翪���б�
	char opencblist[MAX_UNIN_ID_SUM]; //�豸�������Ϻ�� �� �ֶο����б�

	//ִ��nm1����ȫ����N-1
	int OnExeNM1(char * path=NULL);
	int OnWholeFeederNM1(int ifd);//ȫ���ߵ�N-1
	int OnDevNM1(int ifd);//�����豸��N-1
	int OnWriteNM1(int ifd);
	
	int OnAnalyseIsolateCB(int eqid,int start,int oppsite);//����ÿһ���豸�ĸ��뿪�أ�ֻ���ǿ��أ���������õ�

	int OnGetIsland(int eqph,int sond);//�����ĵ������������ͻ

	int OnAnalyseIsland(int eqph,int fdph);//���������������ÿ���ת���ĸ���

	//int OnIslandFlow(int eqph, int fdph);//��ÿ�������г�������
	
//////////////////////////////////////////////////////////////////////////


//���������������
	//////////////////////////////////////////////////////////////////////////
	//���϶�ʧ�縺��(MW) �ǹ��϶���ת�Ƹ���(MW)	�ǹ��϶�ʵ��ת�Ƹ���(MW) ת����Դ
	int	zn_ld_cnt;//�����ڵĸ��ɸ���
	double	zn_ld_pq;//�����ڵĸ������ڹ���
	int	zn_multi_ld_cnt;//�����ڶ��Դ�ĸ�����
	double	zn_multi_ld_pq;//�����ڶ��Դ�����ڹ���
	//////////////////////////////////////////////////////////////////////////
	int	downstream_ld_cnt;//�������εĸ��ɸ���
	double	downstream_ld_pq;//�������εĸ������ڹ���
	int	downstream_multi_ld_cnt;//�������ζ��Դ�ĸ�����
	double	downstream_multi_ld_pq;//�������ζ��Դ�����ڹ���
	int	downstream_trans_ld_cnt;//�������ο���ת�ĸ��ɸ����������Դ��
	double	downstream_trans_ld_pq;//�������ο���ת�ĸ������ڹ��ʣ������Դ��
	
	char unfd_idList[MAX_UION_ID];//�����������������б�
	
	char un_idList[MAX_UION_ID]; //�����������翪�ء��ϡ��б�
	char open_idList[MAX_UION_ID];//�������ηֶο��ء��֡��б�

	char transSec[FEMA_ID_LEN];// ����ת���߶Σ����ط֣��������磩 ��
	char lossSec[FEMA_ID_LEN];//������ʧ�߶�(�ֶο��غϣ��ֶ�,�������磩)

//////////////////////////////////////////////////////////////////////////
//���ڿɿ��Լ���Ľ��,lld 2017-3-18
public:
	int sec_ana;//���ڷ��������߶�
	int up_cb;//�߶����ο��ص�����
//	int ra_cnt;//���μ������еĿɿ����м���
//	RA_Feeder ra_feeder[MAX_RA_CNT];
	int ldlist[5];//һ�������㺬��5000������
//////////////////////////////////////////////////////////////////////////

public:
	//�������翪�صĸ������õ������������	
	int getUNcomb(int unCount,int* before,int* next);

	//so_id ��Դid, cb_id,���翪��id������������翪�ضԲ����ߵĸ���ԣ��
	double getUNCB_abun(char* fdid,char* so_OPP_id,char* cb_id=NULL);

//�����������n-1���ڳ�����������󣬴������ת�������⡣lld 2013-2-2
//ת��Ӧ������Ϊ���ģ��ص��������ε������Ƿ����ת����
	int PutNM1Data();

	int PutRiskData();

	//��ת�������з֣��������磬���׽��n-1��2013-3-30
//	int OnSearchUnion(int noextbs,int* pextbs,int* pupextbs,\
//		double& deltap,double& deltaq,int& deltaC,\
//		double& sumP,double& sumQ,int& sumC,\
//		double& lossP,double& lossQ,int& lossC,\
//		double& deltaMultiP,double& deltaMultiQ,int& deltaMultiC,char* unfd_idList);

	//�������翪�أ���ԣ�ȣ��ͶϿ��ķֶο��أ����������еĸ��ɸ�����������
	//unSum���翪������,pun���翪���׽ڵ�, pun_���翪��ĩ�ڵ�, unRebund���翪��ԣ��
	//cbSum·���жϿ����ظ���,pcb�Ͽ����ص�ָ��,ldSum·���и��ɸ���, tranSumld����ת���ĸ�����
	int OnSearchTP(int unSum,int* pun, int* pun_, double* unRebund, \
					int cbSum,int* pcb,	int ldSum, double& tranSumld
					);

public:
	//����֧·��������ȡת����ת��·��
	//**�������㹻��ԣ�ȣ�**���벻�ܳ��ֵ͵�ѹ

	//��һ��Ҫ�Բ��������������ȫ�ַ������ҵ��������ڵ�������Ƿ��й�����Ͷ��Դ�ȴ���
	//��Ҫ������������ڵ����зֶο��ض�Ҫ��բ
	//1.������ڹ����㣬��ô���Կ϶��ǲ���ͨ��n-1��
	//2.������ڶ��Դ����Ҫ�������е�Դ��֧·�����ӻ���
	//3.�����еĻ����⿪�󣬽��г���У��	
	
	//1.�õ����������е����ת��ԣ�������
	//2.������������Ϊ��Դ�������������е����еĿ��رպϣ���ȡ����У��ֵ���ж��Ƿ�Խ��
	//***����Ҫ�жϸ��ɵ��Ƿ���ת�����û�У���˵���йµ����ڣ�����Ҫ��һ������

	//3.�������У��Խ�磬ȡ�ڶ����ԣ�ȼ��뵽�����У�ѡȡ��·�е�һ���ֶο��ش򿪣��𻷣�
	//4.���㳱������������У�����һ��������룬������翪����ͬһ���ط������ܼ��룻
	//5.�����Եõ�У��ֵ

	//

	//�Բ������������ҲҪ��ѭ���˷�������·
	NM1_ISLANDLIST islandlist[MAX_UN];//һ��������������������翪�ظ�������
	int	g_island;//���ൺ���
	
public:

	//���룺����id�ţ����翪��id�ţ����翪�صĶԲ�ڵ��
	//������Բ�����id�����翪���й�ԣ��p���޹�ԣ��q���Բ�ڵ��ѹ
	int getUNCB_abun(char* fdid,char* cb_id,char* nd_id,char* fdid_abund,double& p_abund,double& q_abund,double& v_abund);
	//�õ����翪�ص����У����а������翪�ص�ԣ��ֵ����Щ��ͬһ��ĸ�ߵ����翪�ز��ܼ���
	//������ʱ������Ҫ�õ����еķֶο��ؼ��Ϻ����еĸ��ɵ�ļ���

//�Բ��������������ɨ�裬�õ���
	//1.���˵���
	//2.���Դ��
	//��������ṹ���õ�����������ʵ�ʽ��
	int residueAnalyze(int noextbs,int* pextbs,int* pupextbs);
	//�õ���������Ľṹ
	int getResidueStruct(int noextbs,int* pextbs,int* pupextbs);
	//�����翪�ؽ������򣬽�ԣ������������ǰ��
	int trim_unlist(int island);
	int sort_unlist(NM1_UNLIST* unlist,int cnt);
	
	//�Խڵ�������򣬽�С�ķ���ǰ�棬׼�����ַ�����
	int trim_ndlist(int island);

	//�����������������˵������翪�ص�ԣ�Ƚ��з���
	//�ų����ԵĲ���ת��·��
	int AnanyseIsland();
	int isNeedDivide(int island);//�Ե��������з���,�ж��Ƿ���Ҫ�з�
	int divideIsland(int island);//�зֵ�

	//�����������翪��unCB1��unCB2���ҵ������·��
	int getMiminalPath(int island, int unCB1,int unCB2,NM1_Minpath& minpath);
	int setCompositeND(int src_nd, int* com_nd, int cnt);

	int setBreakCB(int island, NM1_Minpath minpath,int& bk_cb,double& retPQ);//�趨�⻷�Ŀ���
	//����һ�����еĵ����������翪�أ��⻷���γ������µ�
	int setNewIsland(int old_island, int un_cb,int new_island=NULL);//�趨�⻷�Ŀ���

	
	//�����͵�ѹ��Ҫ���ó����������
	int voltageAnanyse();
	int SetNetworkData(int island,int* loop = NULL);//Ĭ���Ƿ�����
	int islandTreeFlow(int island);//�������ĳ������㣬��Ե�����Դ��
	int islandLoopFlow(int island,int* loop);//�������ĳ������㣬��Ե�����Դ��
	int getbs(int island,int nd);
	int islandTPAnalyse(int island,int so_nd);//�Ը����ĵ��������˷���

	//�õ����»������������翪��unCB1��unCB2;jΪ��������翪�����
	int getLoopunCB(int island, int& unCB1,int& unCB2);
	//����һ�����翪�أ��������磬�鿴�Ƿ���ں��������翪���γɻ���
	int exist_unCB(int island,int unCB1);


	//��������ʱ�򽫲�ֵĵ����������
	int AnanyseResult(int eqph);

	int getPartTrans(int eqph,int island, int& ld_cnt, double& ld_pq,int* transSec,int* lossSec,int& cntTransSec, int& cntLossSec);


};

#endif // !defined(AFX_NM1_H__984F6C62_9471_4ADB_85C1_0C973334F1C5__INCLUDED_)
