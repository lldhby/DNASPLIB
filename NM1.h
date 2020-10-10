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

const int MAX_ZNNO =  20;//各区段存在的最大边界开关节点，现设置的20个，需要时可修改
const int MAX_UN = 20;//最大联络开关
const int MAX_LD = 150;//最多负荷
const int MAX_CP = 10;//最多cp
const int MAX_SEC = 300;//最多线段
const int MAX_CB = 400;//最多开关
const int MAX_ND = 500;//最多节点

const int MAX_UION_ID = 4000;//最多的联络开关总数所占的长度
const int MAX_UION_COMPOSITE = 200;//最多的联络开关组合数
//const int MAX_RA_CNT = 10000;//最多的负荷在可靠性计算中的个数

//配电网的n-1实际上是个多目标组合优化问题
//目标：尽可能转供多的重要用户和尽可能转供多的负荷
//约束：馈线的容量约束和潮流约束（电压不能越限）
//支路交换法可以求出次优解，不一定是全局最优解，但速度快，属于贪心算法
//每次切分供电岛时，尽可能做到每个到的负荷电源平衡而且要均匀
//将全部的负荷点和联络（备用电源点）都要折算到主干线路上

//配电网的n-1不同于主网，需要解决故障后残余网络的转供
//也不同于网络重构，仅仅只需要在故障后尽可能多的转供负荷，不一定要优化处理
//借鉴支路交换法，但某个联络开关加入后，转供不能成功，每次加入一个联络开关，解环
//每增加一个联络开关后，不一定能转供走足够的负荷

//让每个联络开关尽量多的转供负荷，特别是重要用户的负荷
//在残余网络的拓扑分析中，将主干线路（包括主干的开关）全部分析出来

//残余网络的分布式电源的结构
typedef struct NM1_SOLIST
{
	int	nd;//电源节点编号

	int soptr;//电源的物理号
	char secid[MAX_SEC_ID_LEN];//所连的馈线段id
	double p;//(MW)，	有功出力
	double q;//(MVar)，无功出力
	double v;//(kV)，电压值
	double pq;//MVA，电源视在功率裕度
} NM1_SOLIST;

//残余网络的联络开关结构
typedef struct NM1_UNLIST
{
	int	nd;//联络节点编号，开关在岛内一侧的编号
	int main_nd;//联络开关折算到主干线上的节点

//	 unid[MAX_CB_ID_LEN];//联络开关id
	int cbptr;//联络开关的物理号
	char secid[MAX_SEC_ID_LEN];//所连的馈线段id
//	int secptr;//所连馈线的物理号
	double p_abund;//裕度(MW)，	有功裕度
	double q_abund;//裕度(MVar)，无功裕度
	double v_abund;//裕度(MW)，电压值
	double pq_abund;//联络开关的实在功率裕度
} NM1_UNLIST;

//残余网络的所有分段开关
typedef struct NM1_CBLIST
{
	int	cb_ptr;//分段开关的指针（物理号）
//	 cb_id[MAX_CB_ID_LEN];//分段开关的编号
//	int island;//分段开关所属的残余岛编号
	int	normalOpen;//属于常开的分段开关
} NM1_CBLIST;

//残余网络内的所有负荷
typedef struct NM1_LDLIST
{
	int	ld_ptr;//负荷点的指针
//	 ld_id[MAX_LD_ID_LEN];//负荷编号
	int nd;//负荷的节点编号
	int main_nd;//负荷所属的主干线上的节点
	
	double p_ld;//有功(MW)
	double q_ld;//无功(MVar)
	double pq_ld;//视在功率
	int	soSum;//电源点的个数，主要处理双电源用
	int	vip;//电源点的重要程度，0-一般用户；1-三级负荷；2-二级负荷；3-一级负荷
} NM1_LDLIST;

//残余网络内的所有电容器
typedef struct NM1_CPLIST
{
	int	cp_ptr;//电容器的指针
	int nd;//cp的节点编号
	
	int main_nd;//电容器所属的主干线上的节点
	double r;//有功(MW)
} NM1_CPLIST;

//残余网络内的所有节点
typedef struct NM1_NDLIST
{
	int	nd_ptr;//负荷点的指针
//	 nd_id[MAX_ND_ID_LEN];//负荷编号
	int ibs;//节点的母线编号，在原有网络中的编号
	int primebs;//节点的临时母线编号，不破坏原有的网络

	double ld_sum;//节点上的等值负荷总数
	double so_sum;//节点上的等值电源总数
} NM1_NDLIST;

//残余网络内的所有支路及其阻抗
typedef struct NM1_SECLIST
{
	int	sec_ptr;//负荷点的指针
//	 sec_id[MAX_SEC_ID_LEN];//负荷编号
	int nd;//首节点
	int znd;//末节点
	double r;//电阻
	double x;//电抗，潮流计算的时候使用

	double pq;//馈线段内流动的视在功率(MW)，在计算切分开关的时候有用
} NM1_SECLIST;

//残余网络内的所有电气岛
typedef struct NM1_ISLANDLIST
{
	int	island;//岛编号
	
	NM1_SOLIST island_solist[MAX_UN];//一个岛内最多存在10个分布式电源点
	int soCount;//岛内联络开关的个数

	NM1_UNLIST island_unlist[MAX_UN];//一个岛内最多存在10个联络开关
	int unCount;//岛内联络开关的个数

	NM1_LDLIST island_ldlist[MAX_LD];//一个岛内最多存在100个负荷点
	int ldCount;//岛里面的负荷个数

	NM1_CPLIST island_cplist[MAX_CP];//一个岛内最多存在10个电容器补偿点
	int cpCount;//岛里面的负荷个数

	NM1_SECLIST island_seclist[MAX_SEC];//一个岛内最多存在100个线段
	int secCount;//岛里面的负荷个数

	NM1_CBLIST island_cblist[MAX_CB];//一个岛内最多存在100个线段
	int cbCount;//岛里面的负荷个数

	NM1_NDLIST island_ndlist[MAX_ND];//一个岛内最多存在100个线段
	int ndCount;//岛里面的负荷个数

	int canTrans;//岛内的负荷是否可以转供
	char trans_unList[MAX_UION_ID];//岛内转供负荷的联络开关列表
	int validate;//该岛是否已经校验过了

} NM1_ISLANDLIST;

//两个联络开关之间的最小路
typedef struct NM1_Minpath
{
	int cbptr[MAX_CB];//最小路上的开关，开关的交换靠它来实现
//	int secptr[100];//最小路上的支路，支路电流的靠它来比较
	
	int cbCount;//开关数量
//	int secCount;//支路数量
	//double loopR;//最小路上的电阻值
	//double delV;//电压差

	int composite_nd[MAX_ND];//最小路上的节点集合，其中每个节点都折有负荷和电源
//	int composite_fch[100];//最小路上的节点的前向链
//	int composite_bch[100];//最小路上的节点的后向链

	double composite_so[MAX_ND];//最小路上的节点的折合发电，电源为正
	double composite_ld[MAX_ND];//最小路上的节点的折合负荷，负荷为负
	int composite_ld_vip[MAX_ND];//最小路上的重要用户等级，默认是0

	int comCount;

	int island;//负荷所属的残余岛编号
} NM1_Minpath;

//////////////////////////////////////////////////////////////////////////
//可靠性计算结果，对单条馈线段扫描完成后，需要填写该结果
typedef struct RA_Feeder
{
	int ld_id;//负荷编号
	
	short out;//负荷损失

	short swit;//负荷可以切换
	int	up_cb;//上游开关的性质，0-breaker;1-loadbreaker;2-disconnector;3-fuse;5-relay breaker

	short tran;//负荷可以转供
	float tranprobality;//负荷转供概率,对于分布式电源，不是所有的都能转供

	short mutiso;//多电源用户
	int sec_id;//负荷所属的馈线ID
} RA_Feeder;
//可靠性计算结果定义结束
//////////////////////////////////////////////////////////////////////////

//从潮流程序继承后，所有的潮流程序的类都可以在这里使用
class CNM1 : public PWRFLOW  
{
public:
	DSBOB2* dsbob2;
	CLLD* lld;

	CNM1();
	CNM1(DSBOB2* m_pDb,CLLD* m_pLLD);
	virtual ~CNM1();

//////////////////////////////////////////////////////////////////////////
//新的n-1程序，考虑全部的设备，2018-4-21

//1.一个设备故障后：
//	（1）这个设备需要隔开，能不能隔开？不能隔开，全部停电
//	（2）如果能隔开，隔开区域内的负荷是多少？（影响负荷）
//	（3）隔开后，残余网络内的负荷能否转供？
//以这个设备为起点，做一个全部的搜索，一直到所有的电源点，所有的联络开关为止
	//保证每次分析都是一条馈线，2018-4-29
	//潮流计算结束后，给每条馈线设置裕度，也给联络开关设置相关对侧的裕度
	int OnSetAbund();	
	
	char pathFlow[128];
	int fdph;
	char fdid[MAX_ID_DLL];

	int i_island;//岛的编号
	//DTable<ISLAND>*       island_tb_fault;//每一个设备故障后搜索到的岛
	DTable<EQ>*			  eq_tb_isolate;//全部的隔离设备，主要是各类开关
	int					  lv_eq_tb_isolate;
	//通过节点号得到设备
	int OnGetDevByNDs(DSBOB2* dsbob2,int ind,int iznd);

	//通过ID号得到设备
	int OnGetDevByID(DTable<EQ>* eq_tb, char * id);

	//给定馈线物理号和联络开关物理号，得到联络开关的裕度
	double OnGetUnCBAbund(int fdph,int unph, char * oppfdid,double& p_abund,double& q_abund,double& v_abund);

	//给定电源点，终止节点，搜索类型，找到上游岛，type=0 保护开关 1切换开关
	int OnGetUpIsland(int sond,int end_nd,int eqph,int type);
	
	//初始化岛
	int OninitialIsland(int ifd);

//	int	nUpSwitchload;
//	int	nUpNonOutload;
//	int nFaultload;
//	int nNeedTrload;
//	int nNowTrload;

//	int UpStreamload[max_switch_ld];//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
//	int Faultload[max_switch_ld];	//故障段失电负荷列表
//	int NeedTrload[max_switch_ld];//故障段下游需要转供的负荷列表
//	int NowTrload[max_switch_ld];//故障段下游可以转供的负荷列表
	char tranfdlist[MAX_UNIN_ID_SUM];//设备发生故障后的转供馈线列表
	char closeunlist[MAX_UNIN_ID_SUM];//设备发生故障后的 闭合 联络开关列表
	char opencblist[MAX_UNIN_ID_SUM]; //设备发生故障后的 打开 分段开关列表

	//执行nm1程序，全量的N-1
	int OnExeNM1(char * path=NULL);
	int OnWholeFeederNM1(int ifd);//全馈线的N-1
	int OnDevNM1(int ifd);//单个设备的N-1
	int OnWriteNM1(int ifd);
	
	int OnAnalyseIsolateCB(int eqid,int start,int oppsite);//搜索每一个设备的隔离开关，只能是开关，起隔离作用的

	int OnGetIsland(int eqph,int sond);//隔离后的岛，包括死岛和活岛

	int OnAnalyseIsland(int eqph,int fdph);//分析活岛和死岛，获得可以转供的负荷

	//int OnIslandFlow(int eqph, int fdph);//对每个岛进行潮流计算
	
//////////////////////////////////////////////////////////////////////////


//残余网络的输出结果
	//////////////////////////////////////////////////////////////////////////
	//故障段失电负荷(MW) 非故障段需转移负荷(MW)	非故障段实际转移负荷(MW) 转供电源
	int	zn_ld_cnt;//区段内的负荷个数
	double	zn_ld_pq;//区段内的负荷视在功率
	int	zn_multi_ld_cnt;//区段内多电源的个个数
	double	zn_multi_ld_pq;//区段内多电源的视在功率
	//////////////////////////////////////////////////////////////////////////
	int	downstream_ld_cnt;//区段下游的负荷个数
	double	downstream_ld_pq;//区段下游的负荷视在功率
	int	downstream_multi_ld_cnt;//区段下游多电源的个个数
	double	downstream_multi_ld_pq;//区段下游多电源的视在功率
	int	downstream_trans_ld_cnt;//区段下游可以转的负荷个数（含多电源）
	double	downstream_trans_ld_pq;//区段下游可以转的负荷视在功率（含多电源）
	
	char unfd_idList[MAX_UION_ID];//区段下游联络馈线列表
	
	char un_idList[MAX_UION_ID]; //区段下游联络开关“合”列表
	char open_idList[MAX_UION_ID];//区段下游分段开关“分”列表

	char transSec[FEMA_ID_LEN];// 下游转供线段（开关分（包括联络） ）
	char lossSec[FEMA_ID_LEN];//下游损失线段(分段开关合（分段,不含联络）)

//////////////////////////////////////////////////////////////////////////
//用于可靠性计算的结果,lld 2017-3-18
public:
	int sec_ana;//正在分析的馈线段
	int up_cb;//线段上游开关的性质
//	int ra_cnt;//本次计算所有的可靠性中间结果
//	RA_Feeder ra_feeder[MAX_RA_CNT];
	int ldlist[5];//一次最多计算含有5000个负荷
//////////////////////////////////////////////////////////////////////////

public:
	//给定联络开关的个数，得到两两组合数量	
	int getUNcomb(int unCount,int* before,int* next);

	//so_id 电源id, cb_id,联络开关id，返回这个联络开关对侧馈线的负荷裕度
	double getUNCB_abun(char* fdid,char* so_OPP_id,char* cb_id=NULL);

//计算配电网的n-1，在潮流计算结束后，处理这个转供的问题。lld 2013-2-2
//转供应以区段为核心，重点在于下游的区段是否可以转供掉
	int PutNM1Data();

	int PutRiskData();

	//对转供进行切分，搜索联络，彻底解决n-1。2013-3-30
//	int OnSearchUnion(int noextbs,int* pextbs,int* pupextbs,\
//		double& deltap,double& deltaq,int& deltaC,\
//		double& sumP,double& sumQ,int& sumC,\
//		double& lossP,double& lossQ,int& lossC,\
//		double& deltaMultiP,double& deltaMultiQ,int& deltaMultiC,char* unfd_idList);

	//给定联络开关（含裕度）和断开的分段开关，残余网络中的负荷个数，求拓扑
	//unSum联络开关数量,pun联络开关首节点, pun_联络开关末节点, unRebund联络开关裕度
	//cbSum路径中断开开关个数,pcb断开开关的指针,ldSum路径中负荷个数, tranSumld可以转供的负荷数
	int OnSearchTP(int unSum,int* pun, int* pun_, double* unRebund, \
					int cbSum,int* pcb,	int ldSum, double& tranSumld
					);

public:
	//采用支路交换法求取转供和转供路径
	//**必须有足够的裕度；**必须不能出现低电压

	//第一步要对残余网络进行拓扑全局分析，找到各个岛内的情况，是否有孤立点和多电源等存在
	//需要假设残余网络内的所有分段开关都要合闸
	//1.如果存在孤立点，那么可以肯定是不能通过n-1；
	//2.如果存在多电源，需要挨个进行电源的支路交换接环；
	//3.把所有的环都解开后，进行潮流校验	
	
	//1.得到参与网络中的最大转供裕度联络点
	//2.用这个联络点作为电源，将参与网络中的所有的开关闭合，求取潮流校验值，判断是否越界
	//***还需要判断负荷点是否都已转，如果没有，则说明有孤岛存在，还需要进一步处理

	//3.如果潮流校验越界，取第二大的裕度加入到网络中，选取环路中的一个分段开关打开，拆环；
	//4.计算潮流，如果还不行，找下一个联络加入，如果联络开关在同一个地方，不能加入；
	//5.最后可以得到校验值

	//

	//对残余网络的搜索也要遵循拓扑分析的套路
	NM1_ISLANDLIST islandlist[MAX_UN];//一个残余网络内最多有联络开关个电气岛
	int	g_island;//残余岛编号
	
public:

	//输入：馈线id号，联络开关id号，联络开关的对侧节点号
	//输出：对侧馈线id，联络开关有功裕度p，无功裕度q，对侧节点电压
	int getUNCB_abun(char* fdid,char* cb_id,char* nd_id,char* fdid_abund,double& p_abund,double& q_abund,double& v_abund);
	//得到联络开关的序列，其中包括联络开关的裕度值，有些在同一个母线的联络开关不能加入
	//在搜索时，还需要得到所有的分段开关集合和所有的负荷点的集合

//对残余网络进行拓扑扫描，得到：
	//1.拓扑岛；
	//2.多电源；
	//填充三个结构，得到分析出来的实际结果
	int residueAnalyze(int noextbs,int* pextbs,int* pupextbs);
	//得到残余网络的结构
	int getResidueStruct(int noextbs,int* pextbs,int* pupextbs);
	//对联络开关进行排序，将裕度最大的排在最前面
	int trim_unlist(int island);
	int sort_unlist(NM1_UNLIST* unlist,int cnt);
	
	//对节点进行排序，将小的放在前面，准备二分法查找
	int trim_ndlist(int island);

	//初步分析，根据拓扑岛和联络开关的裕度进行分析
	//排除明显的不能转供路径
	int AnanyseIsland();
	int isNeedDivide(int island);//对单个岛进行分析,判断是否需要切分
	int divideIsland(int island);//切分岛

	//给定两个联络开关unCB1和unCB2，找到其最短路径
	int getMiminalPath(int island, int unCB1,int unCB2,NM1_Minpath& minpath);
	int setCompositeND(int src_nd, int* com_nd, int cnt);

	int setBreakCB(int island, NM1_Minpath minpath,int& bk_cb,double& retPQ);//设定解环的开关
	//给定一个现有的岛和两个联络开关，解环，形成两个新岛
	int setNewIsland(int old_island, int un_cb,int new_island=NULL);//设定解环的开关

	
	//分析低电压，要调用潮流计算软件
	int voltageAnanyse();
	int SetNetworkData(int island,int* loop = NULL);//默认是辐射网
	int islandTreeFlow(int island);//辐射网的潮流计算，针对单个电源岛
	int islandLoopFlow(int island,int* loop);//辐射网的潮流计算，针对单个电源岛
	int getbs(int island,int nd);
	int islandTPAnalyse(int island,int so_nd);//对给定的岛进行拓扑分析

	//得到导致环网的两个联络开关unCB1和unCB2;j为加入的联络开关序号
	int getLoopunCB(int island, int& unCB1,int& unCB2);
	//给定一个联络开关，搜索网络，查看是否存在和其它联络开关形成环网
	int exist_unCB(int island,int unCB1);


	//最后输出的时候将拆分的岛输出来即可
	int AnanyseResult(int eqph);

	int getPartTrans(int eqph,int island, int& ld_cnt, double& ld_pq,int* transSec,int* lossSec,int& cntTransSec, int& cntLossSec);


};

#endif // !defined(AFX_NM1_H__984F6C62_9471_4ADB_85C1_0C973334F1C5__INCLUDED_)
