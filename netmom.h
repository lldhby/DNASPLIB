////////////////////////////////////////////////////////////////////////////////
//
//  NETMOM.H
//  Class definitions of distribution network model
//�����������ģ��
////////////////////////////////////////////////////////////////////////////////
#if !defined(__NETMOM_H)
#define __NETMOM_H

//#include "afxwin.h"
#include <stdio.h>  
#include "string.h"
//using std::string;

////////////////////////////////////////////////////////////////////////////////
//	length definition of object ID and DESCR
#define     MAX_DIGITAL_ID_LEN		80		//ң��ID�ŵ���󳤶�
#define     MAX_DIGITAL_DESCR_LEN	64		//ң��ID��������󳤶�
#define     MAX_ANALOG_ID_LEN		120		//ң��
#define     MAX_ANALOG_DESCR_LEN	64		
#define     MAX_CO_ID_LEN			80		//��˾
#define     MAX_CO_DESCR_LEN		64				
#define     MAX_ST_ID_LEN			80		//��վ	
#define     MAX_ST_DESCR_LEN		64				
#define     MAX_ST_ABB_LEN			16 		//��վ��д,99999	
#define     MAX_XFMR_ID_LEN			80		//��ѹ��	
#define     MAX_XFMR_DESCR_LEN		64			
#define     MAX_XF_ID_LEN			80		//����
#define     MAX_XF_DESCR_LEN		64		
#define	    MAX_CB_ID_LEN			80		//����
#define	    MAX_CB_DESCR_LEN		64			
#define     MAX_SO_ID_LEN			80		//��Դ
#define	    MAX_SO_DESCR_LEN		64		
#define	    MAX_LD_ID_LEN			120		//����
#define     MAX_LD_DESCR_LEN		64		
#define     MAX_CP_ID_LEN			80		//������
#define     MAX_CP_DESCR_LEN		64		
#define     MAX_BUS_ID_LEN			80		//ĸ��
#define     MAX_BUS_DESCR_LEN		64
#define     MAX_ZN_ID_LEN			80		//����
#define     MAX_ZN_DESCR_LEN		64
#define     MAX_SEC_ID_LEN			120		//�߶�
#define     MAX_SEC_DESCR_LEN		128
#define     MAX_ND_ID_LEN			240		//�ڵ�,һ����������2���߻������
#define     MAX_TAPTY_ID_LEN		80		//��ѹ����ͷ����
#define     MAX_LNTY_ID_LEN			80		//��·����

#define     MAX_VER_LEN			    50		//�滮�汾���
#define     MAX_FDID_LEN			256		//�߶�
#define     MAX_EQUIPID_LEN			120		//�߶�
#define     MAX_UNIN_ID_SUM			400		//�߶�
#define     MAX_EQUIP_DESCR_LEN		120		//�߶�

#define     MAX_LOW_VOLTAGE_LEN		500		//�����ϵ͵�ѹ��������

////////////////////////////////////////////////////////////////////////////////
//  value definition of mask operation variable
#define     ST_SUBST				1             
#define     ST_SWIST				2		
#define     ST_SO					1		
#define     ST_CP					2		
#define     ST_XFMR					4		
#define     ST_LD					8		
#define     ST_CB					16		
#define     ST_ZN					32		
#define     XFMR_REMOVE 			1		
#define     XFMR_RMVENABL       	2		
#define     XFMR_NRMDEAD       		1
#define     XFMR_INRMOPEN			2
#define     XFMR_INRMGRND			4
#define     XFMR_ZNRMOPEN			8
#define     XFMR_ZNRMGRND			16
#define     XFMR_LIVE				32
#define     XFMR_DEAD				64
#define     XFMR_AMBIG				128
#define     XFMR_UKNOWN				256
#define     XFMR_HOPEN				512
#define     XFMR_HCONN				1024
#define     XFMR_HGRND				2048
#define     XFMR_HCMDMAN			4096
#define     XFMR_MOPEN				8192
#define     XFMR_MCONN				16384
#define     XFMR_MGRND				32768
#define     XFMR_MCMDMAN			65536
#define     XFMR_LOPEN				131072
#define     XFMR_LCONN				262144
#define     XFMR_LGRND				524288
#define     XFMR_LCMDMAN			1048576
#define     XFMR_ABNRML				2097152
#define     XF_REMOVE				1
#define     XF_RMVENABL				2
#define     XF_NRMDEAD				1
#define     XF_INRMOPEN				2
#define     XF_INRMGRND				4
#define     XF_ZNRMOPEN				8
#define     XF_ZNRMGRND				16
#define     XF_LIVE					32
#define     XF_DEAD					64
#define     XF_AMBIG				128
#define     XF_UNKNOWN				256
#define     XF_IOPEN				512
#define     XF_ICONN				1024
#define     XF_IGRND				2048
#define     XF_ICMDMAN				4096
#define     XF_ZOPEN				8192
#define     XF_ZCONN				16384
#define     XF_ZGRND				32768
#define     XF_ZCMDMAN				65536
#define     XF_ABNRML				131072
#define     CB_NMLOPEN				1//���翪��
#define     CB_FORCST				2
#define     CB_BREAKER				4
#define     CB_MOTODIS				8
#define     CB_SWITCH				16
#define     CB_DEADBUS				32
#define     CB_AUTO					64
#define     CB_OPEN					128
#define     CB_OPANABL				256
#define     CB_FDHCB				512
#define     CB_FDSCB				1024	
#define     CB_FDCCB				2048
#define		CB_FDLCB				4096
#define		CB_N_ISOLATE			8192
#define		CB_N_RESTORE			16384

#define     CB_NRMDEAD				1
#define     CB_INRMOPEN				2
#define     CB_INRMGRND				4
#define     CB_ZNRMOPEN				8
#define     CB_ZNRMGRND				16
#define     CB_LIVE					32
#define     CB_DEAD					64
#define     CB_AMBIG				128
#define     CB_UNKNOWN				256
#define     CB_IOPEN				512
#define     CB_ICONN				1024
#define     CB_IGRND				2048
#define     CB_ICMDMAN				4096
#define     CB_ZOPEN				8192
#define     CB_ZGRND				16384
#define     CB_ZCMDMAN				32768
#define     CB_ABNRMAL				65536
#define		CB_FLASH				131072		//for fdir

#define     SO_REMOVE				1
#define     SO_RMVENABL				2
#define     SO_OPEN					4
#define     SO_NRMDEAD				1
#define     SO_NRMOPEN				2
#define     SO_NRMGRND				4
#define     SO_LIVE					8
#define     SO_DEAD					16
#define     SO_AMBIG				32
#define     SO_UNKNOWN				64
#define     SO_IOPEN				128
#define     SO_CONN					256
#define     SO_GRND					512
#define     SO_CMDMAN				1024
#define     SO_ABNRML				2048
#define     LD_REMOVE				1
#define     LD_RMVENABL				2
#define     LD_OPEN					4
#define     LD_ZNLD					8
#define     LD_NRMDEAD				1
#define     LD_NRMOPEN				2
#define     LD_NRMGRND				4
#define     LD_LIVE					8
#define     LD_DEAD					16
#define     LD_AMBIG				32
#define     LD_UNKNOWN				64
#define     LD_IOPEN				128
#define     LD_CONN					256
#define     LD_GRND					512
#define     LD_CMDMAN				1024
#define     LD_ABNRML				2048
#define     LD_FAULT				4096		//for fdir
#define		LD_LEVEL0				8192		//�ؼ�����
#define		LD_LEVEL1				16384		//һ������
#define		LD_LEVEL2 				32768		//��������
#define		LD_LEVEL3				65536		//��������
#define		LD_SO					131072		//���Դ

#define     CP_REMOVE				1
#define     CP_RMVENABL				2
#define     CP_OPEN					4
#define     CP_NRMDEAD				1
#define     CP_NRMOPEN				2
#define     CP_NRMGRND				4
#define     CP_LIVE					8
#define     CP_DEAD					16
#define     CP_AMBIG				32
#define     CP_UNKNOWN				64
#define     CP_IOPEN				128
#define     CP_CONN					256
#define     CP_GRND					512
#define     CP_CMDMAN				1024
#define     CP_ABNRML				2048
#define     CP_ZOPEN				4096
#define     BUS_NRMDEAD				1
#define     BUS_NRMOPEN				2
#define     BUS_NRMGRND				4
#define     BUS_LIVE				8
#define     BUS_DEAD				16
#define     BUS_AMBIG				32
#define     BUS_UNKNOWN				64
#define     BUS_OPEN				128
#define     BUS_CONN				256
#define     BUS_GRND				512
#define     BUS_CMDMAN				1024
#define     BUS_ABNRML				2048
#define		BUS_FAULT				4096		//for fdir
#define		BUS_FLASH				8192		//for fdir

#define		ZN_REMOVE				1
#define		ZN_RMVENABL				2
#define     ZN_LOWCAP				4

#define		ZN_NRMDEAD				1
#define     ZN_INRMOPEN				2
#define     ZN_INRMGRND				4
#define     ZN_ZNRMOPEN				8
#define     ZN_ZNRMGRND				16
#define     ZN_LIVE					32
#define     ZN_DEAD					64
#define     ZN_AMBIG				128
#define     ZN_UNKNOWN				256
#define     ZN_IOPEN				512
#define     ZN_ICONN				1024
#define     ZN_IGRND				2048
#define     ZN_ICMDMAN				4096
#define     ZN_ZOPEN				8192
#define     ZN_ZCONN				16384
#define     ZN_ZGRND				32768
#define     ZN_ZCMDMAN				65536
#define     ZN_ABNRML				131072
#define		ZN_FAULT				262144		//for fdir
#define		ZN_FLASH				524288		//for fdir

#define     SEC_REMOVE				1
#define     SEC_RMVENABL			2
#define     SEC_ZPRIME				4
#define     SEC_NRMDEAD				1
#define     SEC_INRMOPEN			2
#define     SEC_INRMGRND			4
#define     SEC_ZNRMOPEN			8
#define     SEC_ZNRMGRND			16
#define     SEC_LIVE				32
#define     SEC_DEAD				64
#define     SEC_AMBIG				128
#define     SEC_UNKNOWN				256
#define     SEC_IOPEN				512
#define     SEC_ICONN				1024
#define     SEC_IGRND				2048
#define     SEC_ICMDMAN				4096
#define     SEC_ZOPEN				8192
#define     SEC_ZCONN				16384
#define     SEC_ZGRND				32768
#define     SEC_ZCMDMAN				65536
#define     SEC_ABNRML				131072
#define		SEC_FAULT				262144		//for fdir
#define		SEC_FLASH				524288		//for fdir

#define		ND_DEAD					1
#define     ND_OPEN					2		
#define     ND_SO					4		
#define     ND_LD					8		
#define     ND_CP					16		
#define     ND_ZN					32
#define		ND_REG					64
#define		ND_GRND					128
#define     ND_UNPROC				256	
#define     ND_MAIN					512	//�������ϵĽڵ�	

#define     BS_DEAD					1
#define     BS_OPEN					2
#define     BS_UNREG				4
#define     BS_REGERR				8
#define     BS_VHI					16
#define     BS_VLO					32
#define     BS_REF					64
#define     BS_UNION				128//ĸ���Ϲҵ������翪��

#define     BS_PRI					1
#define     BS_AVAIL				2

////////////////////////////////////////////////////////////////////////////////
//  flag definition of table name
#define		_DIGITAL				1
#define		_ANALOG					2
#define		_CO						3
#define		_ST						4
#define		_XFMR					5
#define		_XF						6
#define		_CB						7
#define		_SO						8
#define		_LD						9
#define		_CP						10
#define		_BUS					11
#define		_ZN						12
#define		_SEC					13
#define		_ND						14
#define		_BS						15
#define		_ISLAND					16
#define		_GLOBAL					17
#define		_TAPTY					18
#define		_LNTY					19
#define		_R_CO_ST				20
#define		_R_ST_XFMR				21
#define		_R_XFMR_XF				22
#define		_R_ST_CB				23
#define		_R_ST_SO				24
#define		_R_ST_LD				25
#define		_R_ST_CP				26
#define		_R_ST_BUS				27
#define		_R_ST_ZN				28
#define		_R_ZN_LD				29
#define		_R_ZN_SEC				30
#define		_R_ZN_CP				42
#define		_R_ZN_XF				43
#define		_R_ZN_CB				35
#define		_R_ND_CB				31
#define		_R_ND_XF				32
#define		_R_ND_ZN				33
#define		_R_ND_SEC				34
#define		_FDS					36
#define		_RA					    37
#define		_R_FD_ZN				38
#define		_R_FD_LD				39
#define		_R_FD_ND				40
#define		_GRDSEC					41//�Ե�֧·

#define ZN_ID_LEN 7

//�ɿ��Լ���ʹ��
#define     LD_FIRST				1	//һ������
#define     LD_SECOND				2	//��������
#define     LD_THIRD				4	//��������
#define     LD_MULTI				8	//���Դ

#define FEMA_ID_LEN  2000
#define mx_bs_sc  	  1000//һ�����������1000������ĸ��
#define MAX_MINPATH_LEN 1000


//equip 
//�豸���� add by lld 2017-12-29 for cb optimize
#define Q_ACLINESEGMENT			1	
#define Q_INSULATION			2		//�ܿվ�Ե��
#define Q_CABLE					4	
#define Q_CONNLINE				32768   //���迹֧·

#define Q_MAINLINE				8		//������
#define Q_FORKLINE				16		//��֧��
#define Q_LOAD					32		//����
#define Q_BREAKER				64
#define Q_BREAKERRELAY			128 	//�������Ŀ���
#define Q_LOADBREAKSWITCH		256 
#define Q_DISCONNECTOR			512
#define Q_FUSE					1024
#define Q_BREAKERFEEDER			2048	//���߿���
#define Q_UNIONCB				4096	//���翪��
#define Q_OPENCB				8192	//��λ����
#define Q_SECTCB				16384	//�ֶο���

#define MAX_ID_DLL				64		//DLL�����ID���ֵ
#define MAX_DESCR_DLL			128		//DLL�����ID���ֵ
#define MAX_IDLIST				1024	//DLL�����ID���ֵ
#define MAX_MIANND_LD			3200	//���ڵ��ϹҽӸ���ID���б���|����


const int max_switch_ld = 200;//���϶����θ����б����л����������л�ʱ������⣩
//const int max_fault_ld = 200;	//���϶�ʧ�縺���б�
//const int max_out_ld = 200;//���϶�������Ҫת���ĸ����б�
//const int max_trans_ld = 200;//���϶����ο���ת���ĸ����б�

//lld add 2018-5-9
//const int Q_EQ_UP_DISCONNECTOR	=	1;	//�豸���θ��뿪���ǵ�բ 


////////////////////////////////////////////////////////////////////////////////
//  size definition of each table
//const mx_analog=lld.mx_analog_test;
/*
#define		mx_digital_a		80000	//���ֻ��50000��ң�ŵ�
#define		mx_digital			40000	//���ֻ��50000��ң�ŵ�
#define		mx_analog_a			10000
#define		mx_analog			2000
#define		mx_co				1
#define		mx_st_a				6//000
#define		mx_st				6//000
#define		mx_xfmr				10
#define		mx_xf				10
#define		mx_cb_a				80000
#define		mx_cb				30000
#define		mx_so_a				5000
#define		mx_so				1000
#define		mx_ld_a				10000
#define		mx_ld				3000
#define		mx_cp_a				500
#define		mx_cp				50
#define		mx_bus_a			6000
#define		mx_bus				3000
#define		mx_zn				15000
#define		mx_sec_a			50000
#define		mx_sec				20000
#define		mx_nd				30000
#define		mx_bs				10000
#define		mx_island			1000
#define		mx_global			1
#define		mx_tapty			20
#define		mx_lnty				20
#define		mx_r_co_st			6000
#define		mx_r_st_xfmr		10
#define		mx_r_xfmr_xf		10
#define		mx_r_st_cb			2//0000
#define		mx_r_st_so			1//000
#define		mx_r_st_ld			1
#define		mx_r_st_cp			1//00
#define		mx_r_st_bus			5//000
#define		mx_r_st_zn			1//5000
#define		mx_r_zn_ld			20000
#define		mx_r_zn_sec			20000
#define		mx_r_nd_cb			50000
#define		mx_r_nd_ld			20000
#define		mx_r_nd_xf			10
#define		mx_r_nd_zn			10
#define		mx_r_nd_sec			30000
#define		mx_feeder			1000
*/
//


//@Num	mRID	name	pathName	highkV	lowkV	Substation	BaseVoltage	
//���	��ʶ	����ԭ��	��׼��·����ѹ�ȼ�ȫ��	��ѹ����	��ѹ����	������վ��ʶ	��׼��ѹ��ʶ	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  VL //: public CObject 
{
public:

	char	mRID[MAX_ANALOG_ID_LEN];
	char	name[MAX_ANALOG_DESCR_LEN];
	double	highkV;
	double	lowkV;
	char	BaseVoltage[MAX_ST_ID_LEN];
	char    Substation[MAX_ST_ID_LEN];
public:

	//constructors
	VL();
	VL(const char* m_mRID);

	//assign value operator
	const VL& operator = (const VL& m_VL);

	//compare operator ==
	int operator == (const VL& m_VL) const
	{
		return strcmp(mRID, m_VL.mRID) == 0;
	}

	//compare operator <
	int operator < (const VL& m_VL) const
	{
		return strcmp(mRID, m_VL.mRID) < 0;
	}

protected:

	//DECLARE_SERIAL( VL )
};

//��С·�࣬����������С·�����翪�ص���Դ�����С·��
class  MINPATH //: public CObject
{
public:
	int	id;//��С·�ı�ţ������߼�ͬ��
	int type;//��С·�����ͣ�0Ϊ���ɣ�1Ϊ���ã�2Ϊ����

	char	eq_id[MAX_SEC_ID_LEN];//�豸ID�ţ������Ǹ��ɻ����翪�� 
	int	ind;	//�ڵ�
	double	s;		//��������õ������ڹ��ʣ����翪��Ϊԣ�ȣ�����Ϊ���ڹ���ֵ
	double	monkV;	//��ѹ���������϶������ڹ���(���ڹ���)
	int	minpath[MAX_MINPATH_LEN];//���ɵ���С·��������200���ڵ� 
	int	pathlen;//��С·�ĳ��� 
	
	int cntbakpath;//������С·��������һ�����ɵ�����ж��������С·�����ɵ�ר��
	
	int q;	//��־λ
	int level;	//���ɵĵȼ���һ�ฺ�ɣ���Ҫ�������ฺ�ɣ�һ�㣩�����ฺ�ɣ�ũ����

	int equippath[MAX_MINPATH_LEN];//��С·�ϵ��豸�б�

	int	bakpath[MAX_MINPATH_LEN];//���ɵı�����С·�������30�����翪�أ�ÿ��200���ڵ� 
	int	bakpathlen;//������С·�ĳ��� 
	
	int bakCBnd;//�������翪�صĽڵ�ţ���Ҫ������������翪��	
	double bakAbund;//�������翪�ص�ԣ��
	
	//���翪�ص���Դ���·��
	int	unso[MAX_MINPATH_LEN];//���翪�ص���Դ���·��
	int unso_len;
	int	unso_reverse[MAX_MINPATH_LEN];//��Դ�㵽���翪�صķ���·��
	int unso_reverse_len;
public:

	//constructors
	MINPATH();
	MINPATH(const int m_id);

	void Reset();

	//assign value operator
	const MINPATH& operator = (const MINPATH& m_MINPATH);

	//compare operator ==
	int operator == (const MINPATH& m_MINPATH) const
	{
		return id==m_MINPATH.id;
	}

	//compare operator <
	int operator < (const MINPATH& m_MINPATH) const
	{
		return id<m_MINPATH.id;
	}

	//read or write object
 //   //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( EQUIP )
};

//CIME�ļ��еķ��������
class  CIMEDATA //: public CObject
{
public:
	char	id[256];//�豸���� 
	double  data;//�豸��ң��ֵ,��ң��ֵ

	int	type;//0ң��  1ң��

public:

	//constructors
	CIMEDATA();
	CIMEDATA(const char* m_id);

	//assign value operator
	const CIMEDATA& operator = (const CIMEDATA& m_CIMEDATA);

	//compare operator ==
	int operator == (const CIMEDATA& m_CIMEDATA) const
	{
		return strcmp(id,m_CIMEDATA.id) == 0;
	}

	//compare operator <
	int operator < (const CIMEDATA& m_CIMEDATA) const
	{
		return strcmp(id,m_CIMEDATA.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( EQUIP )
};

//����С·�ɿ��Լ���ʱ����Ҫ�����������������������ϵ��豸���н��д�����Լӿ��������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  EQ //: public CObject
{
public:
	int i_id;//��ţ���������Ϳ�������

	int	type;//�豸���� 0-breaker��1-loadbreakswitch��2-disconnector��3-fuse��
	//4-���翪�أ�5-���������أ�16-���߿��أ�10-��·�Σ��ܿջ���£���20-����
	int		idph;//ԭ�豸�������
	char	id[MAX_EQUIPID_LEN];//ԭ�豸ID�� 
	char	fdid[MAX_EQUIPID_LEN];//���翪�����豸�� 
	int	ind;	//�׽ڵ�
	int	iznd;	//ĩ�ڵ�
	int q;	//��־λ
	int island;	//��־λ
	int r_zn;//�豸�������εĹ�ϵ��lld add 2014-7-18
	int r_fd;//�豸�������εĹ�ϵ��lld add 2014-7-18
	
	int	minpath[MAX_MINPATH_LEN];//�豸����С·��������50���ڵ� 
	int	idpath[MAX_MINPATH_LEN]; //�豸����Դ����豸ID�����У�������50���豸��id�� 
	int pathlen;

	double prob_fault;//�豸������
	double repairTime;//�豸�޸�ʱ��
	
	double prob_check;//�豸�����ʣ�������������
	double checkTime;//�豸����ʱ��
	
	double switchTime;//�豸�л�����ʱ�䣬�����Դ���Ǳ������ػ��۶�������û�и���ʱ��
	double transTime;//ת��ʱ��	
	
	double lambda;//�����ʣ��������������ڱ�ʶ��������ص��豸Ϊ��������	
	double lambda_sum;//��ͣ����(���ƻ�ͣ��)���������������ڱ�ʶ��������ص��豸Ϊ��������	
	double fema;//���Ϻ�����������������ڱ�ʶ��������ص��豸Ϊ��������	
	double fema_sum;//���Ϻ��(���ƻ�ͣ��)���������������ڱ�ʶ��������ص��豸Ϊ��������	
	int	   ld_cnt_loss;//ʧ���û���������
	double ENS;	//��Ӱ���û�ƽ��ȱ������ (kWh/��)
	double w_loss;//ƽ��ʧ�縺��(kW)

	//lld add 2018-05-01
	int UpSwitchload[max_switch_ld];//���϶����θ����б��л���
	int UpNonOutload[max_switch_ld];//���϶����θ����б����л���ֱ���ٶϣ�
	int UpPlanSwitchload[max_switch_ld];//Ԥ�������θ����б�Ҫ��ͣ�粿�ָ��ɣ�����բ����ֱ����
	int UpPlanNonOutload[max_switch_ld];//Ԥ�������θ����б���ͣ�縺�ɣ�
	int Faultload[max_switch_ld];	//���϶�ʧ�縺���б�
	int NeedTrload[max_switch_ld];//���϶�������Ҫת���ĸ����б�
	int NowTrload[max_switch_ld];//���϶����ο���ת���ĸ����б�
	
	int nUpSwitchload;
	int nUpNonOutload;
	int nUpPlanSwitchload;
	int nUpPlanNonOutload;
	int nFaultload;
	int nNeedTrload;
	int nNowTrload;

	char tranfdlist[MAX_UNIN_ID_SUM];//�豸�������Ϻ��ת�������б�
	char closeunlist[MAX_UNIN_ID_SUM];//�豸�������Ϻ�� �պ� ���翪���б�
	char opencblist[MAX_UNIN_ID_SUM]; //�豸�������Ϻ�� �� �ֶο����б�

public:

	//constructors
	EQ();
	EQ(const int m_id);
	void Reset();//��Ҫ�����еĽ��г�ʼ��

	//assign value operator
	const EQ& operator = (const EQ& m_EQUIP);

	//compare operator ==
	int operator == (const EQ& m_EQUIP) const
	{
		return i_id == m_EQUIP.i_id;
	}

	//compare operator <
	int operator < (const EQ& m_EQUIP) const
	{
		return i_id < m_EQUIP.i_id;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( EQUIP )
};


class  R_ND_EQ //: public CObject
{
public:
	long	id;//��ϵ��������ţ����������Էַ�����

	char	nd_id[MAX_ND_ID_LEN];
	int		i_id;
	int		nd_id_ptr;
	int		equip_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ND_EQ();
	R_ND_EQ(const char* m_nd_id, const int m_equip_id);

	//assign value operator
	const R_ND_EQ& operator = (const R_ND_EQ& m_r_nd_equip);

	//compare operator ==
	int operator == (const R_ND_EQ& m_r_nd_equip) const
	{
 		return id == m_r_nd_equip.id;
	}

	//compare operator <
	int operator < (const R_ND_EQ& m_r_nd_equip) const
	{
 		return id < m_r_nd_equip.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const int GetChildId() const
	{
		return i_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return equip_id_ptr;
	}

	//read or write object
//    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_EQ )
};


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  DIGITAL //: public CObject
{
public:

	char	id[MAX_DIGITAL_ID_LEN];
	char	descr[MAX_DIGITAL_DESCR_LEN];
	int		value;
	int		quality;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];
	int fds;

public:

	//constructors
	DIGITAL();
	DIGITAL(const char* m_id);

	//assign value operator 
	const DIGITAL& operator = (const DIGITAL& m_digital);

	//compare operator ==
	int operator == (const DIGITAL& m_digital) const
	{
		return strcmp(id, m_digital.id) == 0;
	}

	//compare operator <
	int operator < (const DIGITAL& m_digital) const
	{
		return strcmp(id, m_digital.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	

protected:

	 //DECLARE_SERIAL( DIGITAL )
};

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ANALOG //: public CObject 
{
public:

	char	id[MAX_ANALOG_ID_LEN];
	char	descr[MAX_ANALOG_DESCR_LEN];
	int		analogType;
	double	value;
	int		quality;
	char	loc[MAX_ANALOG_ID_LEN];
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];
    int     fds;	
public:

	//constructors
	ANALOG();
	ANALOG(const char* m_id);

	//assign value operator
	const ANALOG& operator = (const ANALOG& m_analog);

	//compare operator ==
	int operator == (const ANALOG& m_analog) const
	{
		return strcmp(id, m_analog.id) == 0;
	}

	//compare operator <
	int operator < (const ANALOG& m_analog) const
	{
		return strcmp(id, m_analog.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	

protected:

	 //DECLARE_SERIAL( ANALOG )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  CO //: public CObject 
{
public:

	char	id[MAX_CO_ID_LEN];
	char 	descr[MAX_CO_DESCR_LEN];
	float 	wloss;
	float	rloss;
	float	wso;
	float 	rso;
	float	wload;
	float	rload;
	float	cap;
	float	reac;
	int		r_st;
	
    char    ver[MAX_VER_LEN];

public:

	//constructors
	CO();
	CO(const char* m_id);

	//assign value operator
	const CO& operator = (const CO& m_co);

	//compare operator ==
	int	operator == (const CO& m_co) const
	{
		return strcmp(id, m_co.id) == 0;
	}

	//compare operator <
	int operator < (const CO& m_co) const
	{
		return strcmp(id, m_co.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( CO )
};



//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ST //: public CObject
{
public:

	char	id[MAX_ST_ID_LEN];
	char 	descr[MAX_ST_DESCR_LEN];
	char	abb[MAX_ST_ABB_LEN];
	int		type;
	int		qcomp;//Ԫ����־λ�����ֻ�б��վ����
	float 	wso;
	float 	rso;
	float	wload;
	float 	rload;
	int		r_co;
	int		r_xfmr;
	int		r_so;
	int		r_cb;
	int		r_ld;
	int		r_cp;
	int		r_bus;
	int		r_zn;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];

	double prob_fault;//������
	float repairTime;//5.5Сʱ �����޸�ʱ��;

	int fds;

public:

	//constructors
	ST();
	ST(const char* m_id);

	//assign value operator
	const ST& operator = (const ST& m_st);

	//compare operator ==
	int operator == (const ST& m_st) const
	{
		return strcmp(id, m_st.id) == 0;
	}

	//compare operator <
	int operator < (const ST& m_st) const
	{
		return strcmp(id, m_st.id) < 0;
	}

	//read or write object
//     //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ST )
};



//
class  XFMR //: public CObject
{
public:

	char	id[MAX_XFMR_ID_LEN];
	char	descr[MAX_XFMR_DESCR_LEN];
	int		type;
	char	st[MAX_ST_ID_LEN];
	double	rh;//����
	double	xh;
	double	rm;
	double	xm;
	double	rl;
	double	xl;
	double	r0h;//����
	double	x0h;
	double	r0m;
	double	x0m;
	double	r0l;
	double	x0l;
	double	kvnomh;
	double	kvnomm;
	double	kvnoml;
	double	mvah;
	double	mvam;
	double	mval;
	char	algph[MAX_ANALOG_ID_LEN];
	int		ialgph;
	char	algqh[MAX_ANALOG_ID_LEN];
	int		ialgqh;
	char	algih[MAX_ANALOG_ID_LEN];
	int		ialgih;
	char	algth[MAX_ANALOG_ID_LEN];
	int		ialgth;
	char	algpm[MAX_ANALOG_ID_LEN];
	int		ialgpm;
	char	algqm[MAX_ANALOG_ID_LEN];
	int		ialgqm;
	char	algim[MAX_ANALOG_ID_LEN];
	int		ialgim;
	char	algtm[MAX_ANALOG_ID_LEN];
	int		ialgtm;
	char	algpl[MAX_ANALOG_ID_LEN];
	int		ialgpl;
	char	algql[MAX_ANALOG_ID_LEN];
	int		ialgql;
	char	algil[MAX_ANALOG_ID_LEN];
	int		ialgil;
	char	algtl[MAX_ANALOG_ID_LEN];
	int		ialgtl;
	char	hnd[MAX_ND_ID_LEN];
	char	mnd[MAX_ND_ID_LEN];
	char	lnd[MAX_ND_ID_LEN];
	char	htapty[MAX_TAPTY_ID_LEN];
	char	mtapty[MAX_TAPTY_ID_LEN];
	char	ltapty[MAX_TAPTY_ID_LEN];
	int		htap;
	int		mtap;
	int		ltap;
	int		q;
	int		topflags;
	int		r_st;
	int		r_xf;
	//lld add for distribution network planning platform 2012.03.17
	char    ver[MAX_VER_LEN];
	char    fdid[MAX_SEC_ID_LEN];
	//lld add 2018-5-14
	double prob_fault;//������
	double repairTime;//5.5Сʱ �����޸�ʱ��;

public:

	//constructors
	XFMR();
	XFMR(const char* m_id);

	//assign value operator
	const XFMR& operator = (const XFMR& m_xfmr);

	//compare operator
	int operator == (const XFMR& m_xfmr) const
	{
		return strcmp(id, m_xfmr.id) == 0;
	}

	//compare operator <
	int operator < (const XFMR& m_xfmr) const
	{
		return strcmp(id, m_xfmr.id) < 0;
	}

	//read or write object
	//void Serialize(CArchive& ar);

protected:

	//DECLARE_SERIAL( XFMR )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  XF //: public CObject
{
public:

	char	id[MAX_XF_ID_LEN];
	char	descr[MAX_XF_DESCR_LEN];
	int		itapty;//һ�β��ͷ���ͺ�
	int		iztapty;
	int		tap;//һ�β��ͷλ��
	int		ztap;
	float  	kvnom;//��·���
	float	zkvnom;//��·��İٷֱ�
	char	nd[MAX_ND_ID_LEN];
	char	znd[MAX_ND_ID_LEN];
	int		ind;
	int		iznd;
	int		ibs;
	int		zbs;
	float	mvanom;//�������
	int		q;
	int		topflags;
	float	r1;
	float	x1;
	float	r0;
	float	x0;
	int		ialgip;
	int		ialgiq;
	int		ialgii;
	int		ialgit;
	int		ialgzp;
	int		ialgzq;
	int		ialgzi;
	int		ialgzt;
	float	w;
	float	r_;//����翹
	float	i;
	float	wz;//�޹����⣬��ѹ��������ʱ��
	float	rz;
	float	iz;
	int		r_xfmr;
	int		r_zn;
	//lld add for distribution network planning platform 2012.03.17
	char    ver[MAX_VER_LEN];
	char    fdid[MAX_FDID_LEN];

	char    vl[MAX_FDID_LEN];//����ĵ�ѹ�ȼ�
	float	basevoltage;
	char    tapty[MAX_FDID_LEN];//����ĳ�ͷ

	float v_b;
	float adeg_b;
	float v_e;
	float adeg_e;

public:

	//constructors
	XF();
	XF(const char* m_id);

	//assign value operator
	const XF& operator = (const XF& m_xf);

	//compare operator ==
	int operator == (const XF& m_xf) const
	{
		return strcmp(id, m_xf.id) == 0;
	}

	//compare operator <
	int operator < (const XF& m_xf) const
	{
		return strcmp(id, m_xf.id) < 0;
	}


protected:

	//DECLARE_SERIAL( XF )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  CB //: public CObject
{
public:

	char	id[MAX_CB_ID_LEN];
	char	descr[MAX_CB_DESCR_LEN];
	char	nd[MAX_ND_ID_LEN];
	char	znd[MAX_ND_ID_LEN];
	int		ind;
	int		iznd;
	char	digital[MAX_DIGITAL_ID_LEN];
	int		idigital;
	int		q;
	int		topflags;
	int		r_st;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��

	int		maincb;//�������ϵĿ��أ�Ĭ����0
	int		cbtype;//�������� 0-breaker��1-loadbreakswitch��2-disconnector��3-fuse
	int fds;

	float prob_fault;//������
	float repairTime;//5.5Сʱ �����޸�ʱ��;

	char    vl[MAX_FDID_LEN];//����ĵ�ѹ�ȼ�

//	float prob_check;//����ͣ����
//	float checkTime;//5.5Сʱ �豸����ʱ��;

//	float switchTime;//1.5Сʱ���˹�Ĭ��ֵ�� ���ϼ�������Ŀ����л�ʱ�䣬�ڼ����θ���δ����;
//	float transTime;//0.5Сʱ���˹�Ĭ��ֵ��������ת��·������ת����  �ǹ��϶λָ����磨ת���� ���ز���ʱ�䣬�ڼ������ѹ�
public:

	//constructors
	CB();
	CB(const char* m_id);

	//assign value operator
	const CB& operator = (const CB& m_cb);

	//compare operator ==
	int operator == (const CB& m_cb) const
	{
		return strcmp(id, m_cb.id) == 0;
	}

	//compare operator <
	int operator < (const CB& m_cb) const
	{
		return strcmp(id, m_cb.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( CB )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  SO //: public CObject
{
public:

	char	id[MAX_SO_ID_LEN];
	char	descr[MAX_SO_DESCR_LEN];
	char    nd[MAX_ND_ID_LEN];
	char    vl[MAX_ND_ID_LEN];
	int		ind;
	int    	ibs;
	int    	iisland;
	char    algp[MAX_ANALOG_ID_LEN];
	int    	ialgp;
	char    algq[MAX_ANALOG_ID_LEN];
	int    	ialgq;
	char    algi[MAX_ANALOG_ID_LEN];
	int    	ialgi;
	int  	q;
	int    	topflags;
	int		slackavr;
	float   vtarget;
	float   w;
	float   r;
	float	i;
	float	v;//ΪPV�ڵ�׼��
	int    	r_st;
	int    	r_fd;
	float	ss;//��·����
	float	sw;//�ӵص���
	float	sr;//�ӵص翹

	float  I_value;//�й�ʵ��ֵ
	float  cap_I;//���ݵ���

	int		isAnalog_P;//�׶˵��й��Ƿ������⣬������Ϊ1��������Ϊ0
	int		isAnalog_Q;//�׶˵��޹��Ƿ������⣬������Ϊ1��������Ϊ0
	double  fScaleP;//��λ���ɵ��й������������Ҫ����5%����������������
	double  fScaleQ;//��λ���ɵ��޹��������

    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];

	float maxcurrent;//�������������������ԣ��,��λ��A
	int nm1;//�Ƿ����ȫ��·ͨ��n-1(�׶ι���)
	
	float	pLoss;//����
	float	qLoss;

	int fds;

	//�ɿ��Լ���ָ��
	float fault_ltime;//���Ϻ���λ����ʱ��
	float fault_stime;//�����л�ʱ�䣨�Ѻ���λ����ʱ�䣩
	float fault_ttime;//����ת��ʱ�䣨�Ѻ���λ����ʱ�䣩

	float plan_stime;//�ƻ��л�ʱ�䣨�Ѻ���λ����ʱ�䣩
	float plan_ttime;//�ƻ�ת��ʱ�䣨�Ѻ���λ����ʱ�䣩

	float plan_ltime;//���Ϻ���λ����ʱ��
	float plan_prob;//�ܿ��߼ƻ�ͣ����
	float plan_rtime;//�ܿ��߼ƻ��޸�ʱ��

	//lld add 2018-4-22
	float prob_fault;//������
	float repairTime;//5.5Сʱ �����޸�ʱ��;

//	double plan_overhead_prob;//�ܿ��߼ƻ�ͣ����
//	float plan_overhead_rtime;//�ܿ��߼ƻ��޸�ʱ��
//	double plan_overhead_ins_prob;//�ܿվ�Ե��
//	float plan_overhead_ins_rtime;
//	double plan_cable_prob;//����
//	float plan_cable_rtime;
	float basevoltage;
	
public:

	//constructors
	SO();
	SO(const char* m_id);

	//assign value operator
	const SO& operator = (const SO& m_so);

	//compare operator ==
	int operator == (const SO& m_so) const
	{
		return strcmp(id, m_so.id) == 0;
	}

	//compare operator <
	int operator < (const SO& m_so) const
	{
		return strcmp(id, m_so.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	

protected:

	 //DECLARE_SERIAL( SO )
};

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  FD //: public CObject
{
public:
	int		i_id;//���ɵ��豸һ��Ҫ����Ȼָ��

	char	id[MAX_SO_ID_LEN];
	char    vl[MAX_ND_ID_LEN];
	int		q;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];

	float maxcurrent;//�������������������ԣ��,��λ��A
	
	int nm1;//�Ƿ����ȫ��·ͨ��n-1(�׶ι���)
	int cnt_un;//���翪�ص�����
	int cnt_zn;//���翪�ص�����
	int cnt_ld;//���翪�ص�����
	int cnt_eq;//���翪�ص�����
	int cnt_nd;//����ڵ������
	int cnt_so;//��Դ������

	int r_un;//�����߼�֮��Ĺ�ϵ
	int r_zn;//�͵�Դ֮��Ĺ�ϵ�����߿��԰��������Դ��Ŀǰ��һ��һ
	int r_so;//�͵�Դ֮��Ĺ�ϵ�����߿��԰��������Դ��Ŀǰ��һ��һ
	int r_eq;//�͵�Դ֮��Ĺ�ϵ�����߿��԰��������Դ��Ŀǰ��һ��һ
	int r_ld;//�͵�Դ֮��Ĺ�ϵ�����߿��԰��������Դ��Ŀǰ��һ��һ
	int r_nd;//�͵�Դ֮��Ĺ�ϵ�����߿��԰��������Դ��Ŀǰ��һ��һ

	//�ɿ��Լ���ָ��
	float fault_ltime;//���Ϻ���λ����ʱ��
	float fault_stime;//�����л�ʱ�䣨�Ѻ���λ����ʱ�䣩
	float fault_ttime;//����ת��ʱ�䣨�Ѻ���λ����ʱ�䣩

	float plan_ltime;//���Ϻ���λ����ʱ��
	float plan_prob;//�ܿ��߼ƻ�ͣ����
	float plan_rtime;//�ܿ��߼ƻ��޸�ʱ��

	float plan_stime;//�ƻ��л�ʱ�䣨�Ѻ�����ʱ�䣩
	float plan_ttime;//�ƻ�ת��ʱ�䣨�Ѻ�����ʱ�䣩

	float I_value;//�׶�ʵ�����

public:

	//constructors
	FD();
	FD(const int m_id);

	//assign value operator
	const FD& operator = (const FD& m_fd);

	//compare operator ==
	int operator == (const FD& m_fd) const
	{
		//return i_id == m_fd.i_id;

		return strcmp(id, m_fd.id) == 0;
	}

	//compare operator <
	int operator < (const FD& m_fd) const
	{
		//return i_id < m_fd.i_id;

		return strcmp(id, m_fd.id) < 0;
	}

	void Reset();
	//read or write object
    ////void Serialize( CArchive& ar );
	

protected:

	 //DECLARE_SERIAL( FD )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  LD //: public CObject
{
public:

	char	id[MAX_LD_ID_LEN];
	char	descr[MAX_LD_DESCR_LEN];
	long	type;
	char	nd[MAX_ND_ID_LEN];
	char	vl[MAX_ND_ID_LEN];
	int		ind;
	int		ibs;
	char    algp[MAX_ANALOG_ID_LEN];
	int		ialgp;
	char    algq[MAX_ANALOG_ID_LEN];
	int		ialgq;
	char	algi[MAX_ANALOG_ID_LEN];
	int		ialgi;
	int		q;
	int		topflags;
	float   w;
	float   r;
	float   wm;
	float   rm;
	int		r_st;
	int		r_zn;
	int		r_fd;
	
	int		level;//���ɵȼ���1-һ�����ɣ�2-�������ɣ�4-�������ɣ�8-���Դ�û�

    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[512];//���ڶ��Դ�������������Ҫ����

	int fds;
//�Ƿ���Ҫ�޹��������޹�����

	float prob_fault;//������
	float repairTime;//5.5Сʱ �����޸�ʱ��;
//	float prob_check;//����ͣ����
//	float checkTime;//5.5Сʱ �豸����ʱ��;
	float basevoltage;
public:

	//constructors
	LD();
	LD(const char* m_id);

	//assign value operator
	const LD& operator = (const LD& m_ld);

	//compare operator ==
	int operator == (const LD& m_ld) const
	{
		return strcmp(id, m_ld.id) == 0;
	}

	//compare operator <
	int operator < (const LD& m_ld) const
	{
		return strcmp(id, m_ld.id) < 0;
	}

	//read or write object
//    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( LD )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  CP //: public CObject
{
public:

	char	id[MAX_CP_ID_LEN];
	char	descr[MAX_CP_DESCR_LEN];
	char	nd[MAX_ND_ID_LEN];
	char	znd[MAX_ND_ID_LEN];
	int		ind;
	int		ibs;
	int		iznd;
	int		zbs;
	char    algq[MAX_ANALOG_ID_LEN];
	int		ialgq;
	int		q;
	int		topflags;
	float	mrnom;
	double   r;

	double   w;
	double   r_;
	double   wz;
	double   rz;

	double   v_b;
	double   adeg_b;
	double   v_e;
	double   adeg_e;
	double   i;

	int		r_st;
	int		r_zn;

	
    //lld add for distribution network planning platform 2012.03.17
    char    vl[MAX_ID_DLL];
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];

	int fds;
	float basevoltage;
public:

	//constructors
	CP();
	CP(const char* m_id);

	//assign value operator
	const CP& operator = (const CP& m_cp);

	//compare operator ==
	int operator == (const CP& m_cp) const
	{
		return strcmp(id, m_cp.id) == 0;
	}

	//compare operator <
	int operator < (const CP& m_cp) const
	{
		return strcmp(id, m_cp.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( CP )
};

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  BUS //: public CObject
{
public:

	char	id[MAX_BUS_ID_LEN];
	char    descr[MAX_BUS_DESCR_LEN];
	char    nd[MAX_ND_ID_LEN];
	int		ind;
	int		ibs;
	char    algv[MAX_ANALOG_ID_LEN];
	int		ialgv;
	int		topflags;
	float	vtarget;
	int		vl;//���ѹֵ�����������ֵ����
	float	vhlim;
	float	vllim;
	float   v;
	float   adeg;
	int		r_st;
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];

	int fds;
	float basevoltage;
public:

	//constructors
	BUS();
	BUS(const char* m_id);

	//assign value operator
	const BUS& operator = (const BUS& m_bus);

	//compare operator ==
	int operator == (const BUS& m_bus) const
	{
		return strcmp(id, m_bus.id) == 0;
	}

	//compare operator <
	int operator < (const BUS& m_bus) const
	{
		return strcmp(id, m_bus.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( BUS )
};	

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ZN //: public CObject
{
public:
	long     id;//���θĳ����͵�Ŀ����Ϊ�˱��ڲ���ʱ�����˳�����

	char    descr[MAX_ZN_DESCR_LEN];
	int		type;
	
	int		q;
	int		topflags;

	int		r_st;
	int		r_fd;//�����ߵĹ�ϵ
	int		r_ld;
	int		r_sec;
	int		r_cb;//���ζ�Ӧ�ı߽翪�ص��׶˿���,��һ������
	int		r_cp;
	int		r_xf;

	int		cbptr;//���ν��뿪�ص�ָ��
	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];

	//lld add 2013-3-13
	char	unfd_idList[MAX_UNIN_ID_SUM];//һ�����ε��������翪���б�֮���ã��ֿ�

	//lld add 2014-08-26
	char closeunlist[MAX_UNIN_ID_SUM];//���߶η������Ϻ�� �պ� ���翪���б�
	char opencblist[MAX_UNIN_ID_SUM];//���߶η������Ϻ�� �� �ֶο����б�
	
//	char transSec[FEMA_ID_LEN];// ����ת���߶�
//	char lossSec[FEMA_ID_LEN];//������ʧ�߶�



	int		inner_C;//�������û�
	double	inner_P;//�������й�
	int		next_C;	//���������û�
	double	next_P;	//���������й�(����������)
	int		lossC;	//�����Լ������μ�����һ����ʧ���û�,���β���ʧ
	int		transC;	//���ο���ת�����û�
	double	lossP;	//�����Լ������μ�����һ����ʧ���û�,���β���ʧ
	double	transP;	//���ο���ת�����û�
	
	int	fault_oper_type;	//�������豸���Ϻ�����ĺ������
	int plan_oper_type;		//���α��豸�ƻ����޺�����ĺ������

public:

	//constructors
	ZN();
	ZN(const int m_id);
	void Reset();//��Ҫ�����еĽ��г�ʼ��

	//assign value operator
	const ZN& operator = (const ZN& m_zn);

	//compare operator ==
	int operator == (const ZN& m_zn) const
	{
		return id == m_zn.id;
	}

	//compare operator <
	int operator < (const ZN& m_zn) const
	{
		return id < m_zn.id;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ZN )
};

//�ڽ������࣬���ߵļ��ϣ�һ������һ����һ���ڽ����߼�
//���翪����ֻ�������Ǹ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  UN //: public CObject
{
public:

	char    id[MAX_SEC_ID_LEN];
	int		idph;//���ص�������
//	char    descr[MAX_SEC_ID_LEN];
	char	left[MAX_SEC_ID_LEN];//�������ID
	char	right[MAX_SEC_ID_LEN];
	char	l_nd[MAX_SEC_ID_LEN];//��ڵ��
	char	r_nd[MAX_SEC_ID_LEN];
	
	int		q;
	int		topflags;

	int		r_fd;	//���߼������ߵĹ�ϵ

	double l_abund_P;//�й�ԣ��,��
	double l_abund_Q;//�޹�ԣ��
	double l_abund_V;//��͵�ѹ
	
	double r_abund_P;//�й�ԣ�ȣ���
	double r_abund_Q;//�޹�ԣ��
	double r_abund_V;//��͵�ѹ

    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];//�汾��

public:

	//constructors
	UN();
	UN(const char* m_id);

	//assign value operator
	const UN& operator = (const UN& m_UN);

	//compare operator ==
	int operator == (const UN& m_UN) const
	{
		return strcmp(id, m_UN.id) == 0;
	}

	//compare operator <
	int operator < (const UN& m_UN) const
	{
		return strcmp(id, m_UN.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ZN )
};

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  SEC //: public CObject
{
public:

	char	id[MAX_SEC_ID_LEN];
	char    descr[MAX_SEC_DESCR_LEN];
	char    nd[MAX_ND_ID_LEN];
	char    znd[MAX_ND_ID_LEN];
	int		ind;
	int		iznd;
	int		ibs;
	int		zbs;
	float	r1;
	float	x1;
	float   b1;
	float	r0;
	float	x0;
	float   b0;
	int		q;
	int		topflags;
	char	algip[MAX_ANALOG_ID_LEN];
	int		ialgip;
	char	algiq[MAX_ANALOG_ID_LEN];
	int		ialgiq;
	char	algii[500];//��ʱ������Žڵ���׶˶�·����
	int		ialgii;
	char	algzp[MAX_ANALOG_ID_LEN];
	int		ialgzp;
	char	algzq[MAX_ANALOG_ID_LEN];
	int		ialgzq;
	char	algzi[500];//��ʱ������Žڵ��ĩ�˶�·����
	int		ialgzi;
	float	w;
	float	r;
	float	i;
	float	wz;
	float	rz;
	float	iz;
	int		r_zn;

	float	lossP;//һ����ʧ�й������������ڵ��й���
	float	transP;//����ת���й�
	int		lossC;//һ����ʧ�û������������ڵ��û���
	int		transC;//����ת���û�

	int		lC_zn;//�������û�
	float	lP_zn;//���϶�ʧ�縺��(MW)
	int		lC_znnext;//�������û�
	float	lP_znnext;//�ǹ��϶���ת�Ƹ���(MW)

	char vl[64];

	float frPerLen;//2.2(���£�Ĭ��Ϊ��/��/ÿǧ��)  ���߶ι�����
	float switchTime;//1.5Сʱ���˹�Ĭ��ֵ�� ���ϼ�������Ŀ����л�ʱ�䣬�ڼ����θ���δ����;
	float transTime;//0.5Сʱ���˹�Ĭ��ֵ��������ת��·������ת����  �ǹ��϶λָ����磨ת���� ���ز���ʱ�䣬�ڼ������ѹ�
//�磬���ο���������ͣ��;

	float prob_fault;//������
	float repairTime;//5.5Сʱ �����޸�ʱ��;
	
//	float prob_check;//����ͣ����
//	float checkTime;//5.5Сʱ �豸����ʱ��;

	char tranfdlist[MAX_UNIN_ID_SUM];//���߶η������Ϻ��ת�������б�

	//lld add 2014-08-26
	char closeunlist[MAX_UNIN_ID_SUM];//���߶η������Ϻ�� �պ� ���翪���б�
	char opencblist[MAX_UNIN_ID_SUM];//���߶η������Ϻ�� �� �ֶο����б�
	
		
//	char transSec[FEMA_ID_LEN];// ����ת���߶�
//	char lossSec[FEMA_ID_LEN];//������ʧ�߶�

	
    //lld add for distribution network planning platform 2012.03.17
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_SEC_ID_LEN];

	float faultratio;//������ռ�ı���

	int fds;

	float v_b;//�׽ڵ��ѹ
	float adeg_b;//�׽ڵ����

	float v_e;//ĩ�ڵ��ѹ
	float adeg_e;//ĩ�ڵ����

	//lld add for chengdu run&monitor system 2015-11-19
	char	st_b[MAX_SEC_ID_LEN];//��·���׶˳�վ
	char	st_e[MAX_SEC_ID_LEN];//��·��ĩ�˳�վ

	char	sep_b[MAX_SEC_ID_LEN];//��·���׶˲��
	char	sep_e[MAX_SEC_ID_LEN];//��·��ĩ�˲��

	float len;
	float	basevoltage;//��ȼ��ĵ�����Ҫ�л�׼��ѹ�ȼ����б���ֵ�Ĺ���

public:

	//constructors
	SEC();
	SEC(const char* m_id);

	//assign value operator
	const SEC& operator = (const SEC& m_sec);

	//compare operator ==
	int operator == (const SEC& m_sec) const
	{
		return strcmp(id, m_sec.id) == 0;
	}

	//compare operator <
	int operator < (const SEC& m_sec) const
	{
		return strcmp(id, m_sec.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( SEC )
};	

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  SECP //: ��·�˵�
{
public:

	char	id[MAX_SEC_ID_LEN];
	char    descr[MAX_SEC_DESCR_LEN];
	char    nd[MAX_ND_ID_LEN];
	char	fdid[MAX_SEC_ID_LEN];//�����߶�
	char	st[MAX_SEC_ID_LEN];//�����߶�
	float	w;//�й�����
	int		w_flag;//�й�����״̬
	float	r;
	int		r_flag;

	int		topflags;
	char	baseVL[MAX_SEC_ID_LEN];//�����߶�

public:

	//constructors
	SECP();
	SECP(const char* m_id);

	//assign value operator
	const SECP& operator = (const SECP& m_secp);

	//compare operator ==
	int operator == (const SECP& m_secp) const
	{
		return strcmp(id, m_secp.id) == 0;
	}

	//compare operator <
	int operator < (const SECP& m_secp) const
	{
		return strcmp(id, m_secp.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( SEC )
};	

//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ND //: public CObject
{
public:

	char	id[MAX_ND_ID_LEN];
	int		vl;
	int		q;
	int		ibs;
	int     primebs;
	int     fch;//forechain,ǰ��
	int		bch;//backchain�����
	int		r_cb;
	int		r_xf;
	int		r_zn;
	int		r_sec;
	int		r_ld;
	int		r_eq;//�ڵ���豸�Ĺ�ϵ
	//int		r_fd;//�ڵ�����ߵĹ�ϵ
	//�ڵ�͵������Ĺ�ϵ��������������lld add 2019-7-21 
	int		r_cp;

	//DNASP����
	double	v;//�ڵ��ѹ�ķ�ֵ
	double	adeg;//�ڵ��ѹ�����

	//һ��һ������
	double	w;
	double  len;
	char	ldList[MAX_MIANND_LD];//��|�����ĸ���ID���б�lld add 2018-1-1
	char	UpCBID[MAX_ID_DLL];//�ڵ�����
	char	UnionCBID[MAX_ID_DLL];//�ڵ�����

    char    fdid[MAX_SEC_ID_LEN];//�ڵ�����������
    char    ver[MAX_VER_LEN];//�ڵ�İ汾��

public:
	
	//constructors
	ND();
	ND(const char* m_id);
	
	//assign value operator
	const ND& operator = (const ND& m_nd);

	//compare operator ==
	int operator == (const ND& m_nd) const
	{
		return strcmp(id, m_nd.id) == 0;
	}

	//compare operator <
	int operator < (const ND& m_nd) const
	{
		return strcmp(id, m_nd.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ND )
};


//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  BS //: public CObject
{
public:

	int		id;
	int		iisland;
	int		q;
	int		qq;
	int		fbs;//ǰ����ָ��
	int		bbs;//������ָ��
	int		pnd;//ͷ����
	float	kv;
	float	v;
	float	adeg;
	float	w;
	float	r;

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];

	double  Qv;//ĸ���ϵĲ�������
	char	sc[500];

public:

	//constructors
	BS();
	BS(int m_id);
	
	//assign value operator
	const BS& operator = (const BS& m_bs);

	//compare operator ==
	int operator == (const BS& m_bs) const
	{
		return id == m_bs.id;
	}

	//compare operator <
	int operator < (const BS& m_bs) const
	{
		return id < m_bs.id;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( BS )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  ISLAND //: public CObject
{
public:

	int		id;
	int		refbs;
	int		reffd;//���������������ߣ����㵥Ԫ�������ǵ�Դ��
	float	refminv;//���������͵�ѹ
	float	refmaxv;//���������ߵ�ѹ
	float   wload;
	float   rload;
	float   wso;
	float   rso;

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_VER_LEN];
public:

	//constructors
	ISLAND();
	ISLAND(int m_id);
	
	//assign value operator
	const ISLAND& operator = (const ISLAND& m_island);

	//compare operator ==
	int operator == (const ISLAND& m_island) const
	{
		return id == m_island.id;
	}

	//compare operator <
	int operator < (const ISLAND& m_island) const
	{
		return id < m_island.id;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( ISLAND )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  GLOBAL //: public CObject
{
public:

	int		id;			//id��
	int		nullbs;		//��ĸ����
	int		nextbs;		//
	//time_t  tptime;		//���������ʱ��
	//time_t	timedb;
	int		statusdb;	//���ݿ�״̬
	int		error;		//�������
	int		dberrnum;	//���ݿ�������
	
public:

	//constructors
	GLOBAL();
	GLOBAL(int m_id);
	
	//assign value operator
	const GLOBAL& operator = (const GLOBAL& m_global);

	//compare operator ==
	int operator == (const GLOBAL& m_global) const
	{
		return id == m_global.id;
	}

	//compare operator <
	int operator < (const GLOBAL& m_global) const
	{
		return id < m_global.id;
	}

	//read or write object
    //void Serialize( CArchive& ar );
	
protected:

	 //DECLARE_SERIAL( GLOBAL )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  TAPTY //: public CObject
{
public:

	char	id[MAX_TAPTY_ID_LEN];
	int		mn;
	int		mx;
	int		nom;
	float	step;

public:

	//constructors
	TAPTY();
	TAPTY(const char* m_id);
	
	//assign value operator
	const TAPTY& operator = (const TAPTY& m_tapty);

	//compare operator ==
	int operator == (const TAPTY& m_tapty) const
	{
		return strcmp(id, m_tapty.id) == 0;
	}

	//compare operator <
	int operator < (const TAPTY& m_tapty) const
	{
		return strcmp(id, m_tapty.id) < 0;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( TAPTY )
};

//��˾�͵�վ֮��Ĺ�ϵ��
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_CO_ST //: public CObject
{
public:

	char	co_id[MAX_CO_ID_LEN];	//��˾��ID��
	char	st_id[MAX_ST_ID_LEN];	//���վ��ID��
	int		co_id_ptr;				//��˾ID�ŵ�ָ��
	int		st_id_ptr;				//���վID�ŵ�ָ��
	int		fptr;					//ǰ��ָ��
	int		bptr;					//����ָ��	

public:

	//constructors
	R_CO_ST();
	R_CO_ST(const char* m_co_id, const char* m_st_id);

	//assign value operator
	const R_CO_ST& operator = (const R_CO_ST& m_r_co_st);

	//compare operator ==
	int operator == (const R_CO_ST& m_r_co_st) const
	{
		return strcmp(st_id, m_r_co_st.st_id) == 0 && 
			   strcmp(co_id, m_r_co_st.co_id) == 0;
	}

	//compare operator <
	int operator < (const R_CO_ST& m_r_co_st) const
	{
		int		cmpval;

		cmpval = strcmp(co_id, m_r_co_st.co_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(st_id, m_r_co_st.st_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return co_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return st_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return co_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return st_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_CO_ST )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_XFMR //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	xfmr_id[MAX_XFMR_ID_LEN];
	int		st_id_ptr;
	int		xfmr_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_XFMR();
	R_ST_XFMR(const char* m_st_id, const char* m_xfmr_id);

	//assign value operator
	const R_ST_XFMR& operator = (const R_ST_XFMR& m_r_st_xfmr);

	//compare operator ==
	int operator == (const R_ST_XFMR& m_r_st_xfmr) const
	{
		return strcmp(xfmr_id, m_r_st_xfmr.xfmr_id) == 0 && 
			   strcmp(st_id, m_r_st_xfmr.st_id) == 0;
	}
	
	//compare operator <
	int operator < (const R_ST_XFMR& m_r_st_xfmr) const
	{
		int		cmpval;

		cmpval = strcmp(st_id, m_r_st_xfmr.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(xfmr_id, m_r_st_xfmr.xfmr_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return xfmr_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return xfmr_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_XFMR )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_XFMR_XF //: public CObject
{
public:
	long	id;

	char	xfmr_id[MAX_XFMR_ID_LEN];
	char	xf_id[MAX_XF_ID_LEN];
	int		xfmr_id_ptr;
	int		xf_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_XFMR_XF();
	R_XFMR_XF(const char* m_xfmr_id, const char* m_xf_id);

	//assign value operator
	const R_XFMR_XF& operator = (const R_XFMR_XF& m_r_xfmr_xf);

	//compare operator ==
	int operator == (const R_XFMR_XF& m_r_xfmr_xf) const
	{
		return id == m_r_xfmr_xf.id;
//		return strcmp(xf_id, m_r_xfmr_xf.xf_id) == 0 && 
//			   strcmp(xfmr_id, m_r_xfmr_xf.xfmr_id) == 0;
	}
	
	//compare operator <
	int operator < (const R_XFMR_XF& m_r_xfmr_xf) const
	{
		return id < m_r_xfmr_xf.id;

//		int		cmpval;
//
//		cmpval = strcmp(xfmr_id, m_r_xfmr_xf.xfmr_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(xf_id, m_r_xfmr_xf.xf_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return xfmr_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return xf_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return xfmr_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return xf_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_XFMR_XF )
};



//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_CB //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	cb_id[MAX_CB_ID_LEN];
	int		st_id_ptr;
	int		cb_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_CB();
	R_ST_CB(const char* m_st_id, const char* m_cb_id);

	//assign value operator
	const R_ST_CB& operator = (const R_ST_CB& m_r_st_cb);

	//compare operator ==
	int operator == (const R_ST_CB& m_r_st_cb) const
	{
		return strcmp(cb_id, m_r_st_cb.cb_id) == 0 && 
			   strcmp(st_id, m_r_st_cb.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_CB& m_r_st_cb) const
	{
		int		cmpval;

		cmpval = strcmp(st_id, m_r_st_cb.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(cb_id, m_r_st_cb.cb_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cb_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cb_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_CB )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_SO //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	so_id[MAX_SO_ID_LEN];
	int		st_id_ptr;
	int		so_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_SO();
	R_ST_SO(const char* m_st_id, const char* m_so_id);

	//assign value operator
	const R_ST_SO& operator = (const R_ST_SO& m_r_st_so);

	//compare operator ==
	int operator == (const R_ST_SO& m_r_st_so) const
	{
		return strcmp(so_id, m_r_st_so.so_id) == 0 && 
			   strcmp(st_id, m_r_st_so.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_SO& m_r_st_so) const
	{
		int		cmpval;

		cmpval = strcmp(st_id, m_r_st_so.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(so_id, m_r_st_so.so_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return so_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return so_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_SO )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_LD //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	ld_id[MAX_LD_ID_LEN];
	int		st_id_ptr;
	int		ld_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_LD();
	R_ST_LD(const char* m_st_id, const char* m_ld_id);

	//assign value operator
	const R_ST_LD& operator = (const R_ST_LD& m_r_st_ld);

	//compare operator ==
	int operator == (const R_ST_LD& m_r_st_ld) const
	{
		return strcmp(ld_id, m_r_st_ld.ld_id) == 0 && 
			   strcmp(st_id, m_r_st_ld.st_id) == 0;
	}

	//compare operator < 
	int operator < (const R_ST_LD& m_r_st_ld) const
	{
		int		cmpval;

		cmpval = strcmp(st_id, m_r_st_ld.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(ld_id, m_r_st_ld.ld_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return ld_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return ld_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_LD )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_CP //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	cp_id[MAX_CP_ID_LEN];
	int		st_id_ptr;
	int		cp_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_CP();
	R_ST_CP(const char* m_st_id, const char* m_cp_id);

	//assign value operator
	const R_ST_CP& operator = (const R_ST_CP& m_r_st_cp);

	//compare operator ==
	int operator == (const R_ST_CP& m_r_st_cp) const
	{
		return strcmp(cp_id, m_r_st_cp.cp_id) == 0 && 
			   strcmp(st_id, m_r_st_cp.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_CP& m_r_st_cp) const
	{
		int cmpval;

		cmpval = strcmp(st_id, m_r_st_cp.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(cp_id, m_r_st_cp.cp_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cp_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cp_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_CP )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_BUS //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	bus_id[MAX_BUS_ID_LEN];
	int		st_id_ptr;
	int		bus_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_BUS();
	R_ST_BUS(const char* m_st_id, const char* m_bus_id);

	//assign value operator
	const R_ST_BUS& operator = (const R_ST_BUS& m_r_st_bus);

	//compare operator ==
	int operator == (const R_ST_BUS& m_r_st_bus) const
	{
		return strcmp(bus_id, m_r_st_bus.bus_id) == 0 && 
			   strcmp(st_id, m_r_st_bus.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_BUS& m_r_st_bus) const
	{
		int		cmpval;
		
		cmpval = strcmp(st_id, m_r_st_bus.st_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(bus_id, m_r_st_bus.bus_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return bus_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return bus_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_BUS )
};

/*
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ST_ZN //: public CObject
{
public:

	char	st_id[MAX_ST_ID_LEN];
	char	zn_id[MAX_ZN_ID_LEN];
	int		st_id_ptr;
	int		zn_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ST_ZN();
	R_ST_ZN(const char* m_st_id, const char* m_zn_id);

	//assign value operator
	const R_ST_ZN& operator = (const R_ST_ZN& m_r_st_zn);

	//compare operator ==
	int operator == (const R_ST_ZN& m_r_st_zn) const
	{
		return strcmp(zn_id, m_r_st_zn.zn_id) == 0 && 
			   strcmp(st_id, m_r_st_zn.st_id) == 0;
	}

	//compare operator <
	int operator < (const R_ST_ZN& m_r_st_zn) const
	{
		int		cmpval;

		//���жϳ�վID�Ƿ�һ��
		cmpval = strcmp(st_id, m_r_st_zn.st_id);
		if(cmpval == 0)
		{	
			//���ж�����ID�Ƿ�һ��
			cmpval = strcmp(zn_id, m_r_st_zn.zn_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return st_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return zn_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return st_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return zn_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ST_ZN )
};
*/


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ZN_LD //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	ld_id[MAX_LD_ID_LEN];
	int		zn_id_ptr;
	int		ld_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��

public:

	//constructors
	R_ZN_LD();
	R_ZN_LD(const char* m_zn_id, const char* m_ld_id);

	//assign value operator
	const R_ZN_LD& operator = (const R_ZN_LD& m_r_zn_ld);

	//compare operator ==
	int operator == (const R_ZN_LD& m_r_zn_ld) const
	{
		return id == m_r_zn_ld.id;
//		return strcmp(ld_id, m_r_zn_ld.ld_id) == 0 && 
//			   strcmp(zn_id, m_r_zn_ld.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_LD& m_r_zn_ld) const
	{
		return id < m_r_zn_ld.id;
//		int		cmpval;
//
//		cmpval = strcmp(zn_id, m_r_zn_ld.zn_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(ld_id, m_r_zn_ld.ld_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return ld_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return ld_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ZN_LD )
};

class  R_ZN_EQ //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	eq_id[MAX_LD_ID_LEN];
	int		zn_id_ptr;
	int		eq_id_ptr;
	int		fptr;
	int		bptr;
public:

	//constructors
	R_ZN_EQ();
	R_ZN_EQ(const char* m_zn_id, const char* m_eq_id);

	//assign value operator
	const R_ZN_EQ& operator = (const R_ZN_EQ& m_r_zn_eq);

	//compare operator ==
	int operator == (const R_ZN_EQ& m_r_zn_eq) const
	{
		return id == m_r_zn_eq.id;
	}

	//compare operator <
	int operator < (const R_ZN_EQ& m_r_zn_eq) const
	{
		return id < m_r_zn_eq.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get chieq id
	const char* GetChildId() const
	{
		return eq_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get chieq ptr
	int GetChildPtr() const
	{
		return eq_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ZN_EQ )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ZN_SEC //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	sec_id[MAX_SEC_ID_LEN];
	int		zn_id_ptr;
	int		sec_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��

public:

	//constructors
	R_ZN_SEC();
	R_ZN_SEC(const char* m_zn_id, const char* m_sec_id);

	//assign value operator
	const R_ZN_SEC& operator = (const R_ZN_SEC& m_r_zn_sec);

	//compare operator ==
	int operator == (const R_ZN_SEC& m_r_zn_sec) const
	{
		return id == m_r_zn_sec.id;
//		return strcmp(sec_id, m_r_zn_sec.sec_id) == 0 && 
//			   strcmp(zn_id, m_r_zn_sec.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_SEC& m_r_zn_sec) const
	{
		return id < m_r_zn_sec.id;
//		int		cmpval;
//
//		cmpval = strcmp(zn_id, m_r_zn_sec.zn_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(sec_id, m_r_zn_sec.sec_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return sec_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return sec_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ZN_SEC )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ZN_CP //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	cp_id[MAX_CP_ID_LEN];
	int		zn_id_ptr;
	int		cp_id_ptr;
	int		fptr;
	int		bptr;
	char    ver[MAX_VER_LEN];
	char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��

public:

	//constructors
	R_ZN_CP();
	R_ZN_CP(const char* m_zn_id, const char* m_cp_id);

	//assign value operator
	const R_ZN_CP& operator = (const R_ZN_CP& m_r_zn_cp);

	//compare operator ==
	int operator == (const R_ZN_CP& m_r_zn_cp) const
	{
		return id == m_r_zn_cp.id;
		//		return strcmp(cp_id, m_r_zn_cp.cp_id) == 0 && 
		//			   strcmp(zn_id, m_r_zn_cp.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_CP& m_r_zn_cp) const
	{
		return id < m_r_zn_cp.id;
		//		int		cmpval;
		//
		//		cmpval = strcmp(zn_id, m_r_zn_cp.zn_id);
		//		if(cmpval == 0)
		//		{
		//			cmpval = strcmp(cp_id, m_r_zn_cp.cp_id);
		//		}
		//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cp_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cp_id_ptr;
	}

	//read or write object
	//void Serialize( CArchive& ar );

protected:

	//DECLARE_SERIAL( R_ZN_cp )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ZN_XF //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	xf_id[MAX_XF_ID_LEN];
	int		zn_id_ptr;
	int		xf_id_ptr;
	int		fptr;
	int		bptr;
	char    ver[MAX_VER_LEN];
	char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��

public:

	//constructors
	R_ZN_XF();
	R_ZN_XF(const char* m_zn_id, const char* m_xf_id);

	//assign value operator
	const R_ZN_XF& operator = (const R_ZN_XF& m_r_zn_xf);

	//compare operator ==
	int operator == (const R_ZN_XF& m_r_zn_xf) const
	{
		return id == m_r_zn_xf.id;
		//		return strcmp(xf_id, m_r_zn_xf.xf_id) == 0 && 
		//			   strcmp(zn_id, m_r_zn_xf.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_XF& m_r_zn_xf) const
	{
		return id < m_r_zn_xf.id;
		//		int		cmpval;
		//
		//		cmpval = strcmp(zn_id, m_r_zn_xf.zn_id);
		//		if(cmpval == 0)
		//		{
		//			cmpval = strcmp(xf_id, m_r_zn_xf.xf_id);
		//		}
		//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return xf_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return xf_id_ptr;
	}

	//read or write object
	//void Serialize( CArchive& ar );

protected:

	//DECLARE_SERIAL( R_ZN_xf )
};

//ͨ������-�߽翪�ر�õ���صı߽翪��
class  R_ZN_CB //: public CObject
{
public:
	long	id;

	char	zn_id[MAX_ZN_ID_LEN];
	char	cb_id[MAX_SEC_ID_LEN];
	int		zn_id_ptr;
	int		cb_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��

public:

	//constructors
	R_ZN_CB();
	R_ZN_CB(const char* m_zn_id, const char* m_cb_id);

	//assign value operator
	const R_ZN_CB& operator = (const R_ZN_CB& m_r_zn_cb);

	//compare operator ==
	int operator == (const R_ZN_CB& m_r_zn_cb) const
	{
		return id == m_r_zn_cb.id;
//		return strcmp(cb_id, m_r_zn_cb.cb_id) == 0 && 
//			   strcmp(zn_id, m_r_zn_cb.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_ZN_CB& m_r_zn_cb) const
	{
		return id < m_r_zn_cb.id;

//		int		cmpval;
//
//		cmpval = strcmp(zn_id, m_r_zn_cb.zn_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(cb_id, m_r_zn_cb.cb_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return zn_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cb_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return zn_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cb_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ZN_CB )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_LD //: public CObject
{
public:
	long	id;
	
	char	nd_id[MAX_ND_ID_LEN];
	char	ld_id[MAX_LD_ID_LEN];
	int		nd_id_ptr;
	int		ld_id_ptr;
	int		fptr;
	int		bptr;

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��
	
public:
	
	//constructors
	R_ND_LD();
	R_ND_LD(const char* m_nd_id, const char* m_ld_id);
	
	//assign value operator
	const R_ND_LD& operator = (const R_ND_LD& m_r_nd_ld);
	
	//compare operator ==
	int operator == (const R_ND_LD& m_r_nd_ld) const
	{
		return id == m_r_nd_ld.id;
//		return strcmp(ld_id, m_r_nd_ld.ld_id) == 0 && 
//			strcmp(nd_id, m_r_nd_ld.nd_id) == 0;
	}
	
	//compare operator <
	int operator < (const R_ND_LD& m_r_nd_ld) const
	{
		return id < m_r_nd_ld.id;
//		int		cmpval;
//		
//		cmpval = strcmp(nd_id, m_r_nd_ld.nd_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(ld_id, m_r_nd_ld.ld_id);
//		}
//		return cmpval < 0;
	}
	
	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}
	
	//get child id
	const char* GetChildId() const
	{
		return ld_id;
	}
	
	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}
	
	//get child ptr
	int GetChildPtr() const
	{
		return ld_id_ptr;
	}
	
	//read or write object
    //void Serialize( CArchive& ar );
	
protected:
	
	//DECLARE_SERIAL( R_ND_CB )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_CB //: public CObject
{
public:
	long	id;

	char	nd_id[MAX_ND_ID_LEN];
	char	cb_id[MAX_CB_ID_LEN];
	int		nd_id_ptr;
	int		cb_id_ptr;
	int		fptr;
	int		bptr;

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��
public:

	//constructors
	R_ND_CB();
	R_ND_CB(const char* m_nd_id, const char* m_cb_id);

	//assign value operator
	const R_ND_CB& operator = (const R_ND_CB& m_r_nd_cb);

	//compare operator ==
	int operator == (const R_ND_CB& m_r_nd_cb) const
	{
		return id == m_r_nd_cb.id;
//		return strcmp(cb_id, m_r_nd_cb.cb_id) == 0 && 
//			   strcmp(nd_id, m_r_nd_cb.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_CB& m_r_nd_cb) const
	{
		return id < m_r_nd_cb.id;
//		int		cmpval;
//
//		cmpval = strcmp(nd_id, m_r_nd_cb.nd_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(cb_id, m_r_nd_cb.cb_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cb_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cb_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_CB )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_XF //: public CObject
{
public:
	long	id;

	char	nd_id[MAX_ND_ID_LEN];
	char	xf_id[MAX_XF_ID_LEN];
	int		nd_id_ptr;
	int		xf_id_ptr;
	int		fptr;
	int		bptr;

	char	ver[MAX_VER_LEN];
	char	fdid[MAX_FDID_LEN];

public:

	//constructors
	R_ND_XF();
	R_ND_XF(const char* m_nd_id, const char* m_xf_id);

	//assign value operator
	const R_ND_XF& operator = (const R_ND_XF& m_r_nd_xf);

	//compare operator ==
	int operator == (const R_ND_XF& m_r_nd_xf) const
	{
		return id == m_r_nd_xf.id;
//		return strcmp(xf_id, m_r_nd_xf.xf_id) == 0 && 
//			   strcmp(nd_id, m_r_nd_xf.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_XF& m_r_nd_xf) const
	{
		return id < m_r_nd_xf.id;

//		int		cmpval;
//
//		cmpval = strcmp(nd_id, m_r_nd_xf.nd_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(xf_id, m_r_nd_xf.xf_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return xf_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return xf_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_XF )
};

/*

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_ZN //: public CObject
{
public:

	char	nd_id[MAX_ND_ID_LEN];
	char	zn_id[MAX_ZN_ID_LEN];
	int		nd_id_ptr;
	int		zn_id_ptr;
	int		fptr;
	int		bptr;

public:

	//constructors
	R_ND_ZN();
	R_ND_ZN(const char* m_nd_id, const char* m_zn_id);

	//assign value operator
	const R_ND_ZN& operator = (const R_ND_ZN& m_r_nd_zn);

	//compare operator ==
	int operator == (const R_ND_ZN& m_r_nd_zn) const
	{
		return strcmp(zn_id, m_r_nd_zn.zn_id) == 0 && 
			   strcmp(nd_id, m_r_nd_zn.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_ZN& m_r_nd_zn) const
	{
		int		cmpval;

		cmpval = strcmp(nd_id, m_r_nd_zn.nd_id);
		if(cmpval == 0)
		{
			cmpval = strcmp(zn_id, m_r_nd_zn.zn_id);
		}
		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return zn_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return zn_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_ZN )
};

*/

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_SEC //: public CObject
{
public:
	long	id;

	char	nd_id[MAX_ND_ID_LEN];
	char	sec_id[MAX_SEC_ID_LEN];
	int		nd_id_ptr;
	int		sec_id_ptr;
	int		fptr;//ͬ���ĸ��׳��ֵ�ǰһ��λ��
	int		bptr;//ͬ���ĸ��׳��ֵĺ�һ��λ�ã�ͨ�����������Կ��ٶ�λ�����׵����ж���

    char    ver[MAX_VER_LEN];
    char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��

public:

	//constructors
	R_ND_SEC();
	R_ND_SEC(const char* m_nd_id, const char* m_sec_id);

	//assign value operator
	const R_ND_SEC& operator = (const R_ND_SEC& m_r_nd_sec);

	//compare operator ==
	int operator == (const R_ND_SEC& m_r_nd_sec) const
	{
		return id == m_r_nd_sec.id;
//		return strcmp(sec_id, m_r_nd_sec.sec_id) == 0 && 
//			   strcmp(nd_id, m_r_nd_sec.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_SEC& m_r_nd_sec) const
	{
		return id < m_r_nd_sec.id;

//		int		cmpval;
//
//		cmpval = strcmp(nd_id, m_r_nd_sec.nd_id);
//		if(cmpval == 0)
//		{
//			cmpval = strcmp(sec_id, m_r_nd_sec.sec_id);
//		}
//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return sec_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return sec_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_ND_SEC )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_ND_CP //: public CObject
{
public:
	long	id;

	char	nd_id[MAX_ND_ID_LEN];
	char	cp_id[MAX_CP_ID_LEN];
	int		nd_id_ptr;
	int		cp_id_ptr;
	int		fptr;//ͬ���ĸ��׳��ֵ�ǰһ��λ��
	int		bptr;//ͬ���ĸ��׳��ֵĺ�һ��λ�ã�ͨ�����������Կ��ٶ�λ�����׵����ж���

	char    ver[MAX_VER_LEN];
	char    fdid[MAX_FDID_LEN];//ͨ��������Եõ����翪�أ����һ���˿�������2�����ߣ�һ�������翪��

public:

	//constructors
	R_ND_CP();
	R_ND_CP(const char* m_nd_id, const char* m_cp_id);

	//assign value operator
	const R_ND_CP& operator = (const R_ND_CP& m_r_nd_cp);

	//compare operator ==
	int operator == (const R_ND_CP& m_r_nd_cp) const
	{
		return id == m_r_nd_cp.id;
		//		return strcmp(cp_id, m_r_nd_cp.cp_id) == 0 && 
		//			   strcmp(nd_id, m_r_nd_cp.nd_id) == 0;
	}

	//compare operator <
	int operator < (const R_ND_CP& m_r_nd_cp) const
	{
		return id < m_r_nd_cp.id;

		//		int		cmpval;
		//
		//		cmpval = strcmp(nd_id, m_r_nd_cp.nd_id);
		//		if(cmpval == 0)
		//		{
		//			cmpval = strcmp(cp_id, m_r_nd_cp.cp_id);
		//		}
		//		return cmpval < 0;
	}

	//get parent id
	const char* GetParentId() const
	{
		return nd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return cp_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return nd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return cp_id_ptr;
	}

	//read or write object
	//void Serialize( CArchive& ar );

protected:

	//DECLARE_SERIAL( R_ND_cp )
};



//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_BS_SEC //: public CObject
{
public:
	long	id;

	char	bs_id[MAX_ND_ID_LEN];
	char	sec_id[MAX_SEC_ID_LEN];
	int		bs_id_ptr;
	int		sec_id_ptr;
	int		fptr;//ͬ���ĸ��׳��ֵ�ǰһ��λ��
	int		bptr;//ͬ���ĸ��׳��ֵĺ�һ��λ�ã�ͨ�����������Կ��ٶ�λ�����׵����ж���

public:

	//constructors
	R_BS_SEC();
	R_BS_SEC(const char* m_bs_id, const char* m_sec_id);

	//assign value operator
	const R_BS_SEC& operator = (const R_BS_SEC& m_r_bs_sec);

	//compare operator ==
	int operator == (const R_BS_SEC& m_r_bs_sec) const
	{
		return id == m_r_bs_sec.id;
	}

	//compare operator <
	int operator < (const R_BS_SEC& m_r_bs_sec) const
	{
		return id < m_r_bs_sec.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return bs_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return sec_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return bs_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return sec_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_bs_SEC )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_UN //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	char	un_id[MAX_CB_ID_LEN];
	int		fd_id_ptr;
	int		un_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_UN();
	R_FD_UN(const char* m_fd_id, const char* m_un_id);

	//assign value operator
	const R_FD_UN& operator = (const R_FD_UN& m_R_FD_UN);

	//compare operator ==
	int operator == (const R_FD_UN& m_R_FD_UN) const
	{
		return id == m_R_FD_UN.id;
//		return strcmp(sec_id, m_R_FD_UN.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_UN.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_UN& m_R_FD_UN) const
	{
		return id < m_R_FD_UN.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get chiun id
	const char* GetChildId() const
	{
		return un_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get chiun ptr
	int GetChildPtr() const
	{
		return un_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_UN )
};

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_LD //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	char	ld_id[MAX_CB_ID_LEN];
	int		fd_id_ptr;
	int		ld_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_LD();
	R_FD_LD(const char* m_fd_id, const char* m_ld_id);

	//assign value operator
	const R_FD_LD& operator = (const R_FD_LD& m_R_FD_LD);

	//compare operator ==
	int operator == (const R_FD_LD& m_R_FD_LD) const
	{
		return id == m_R_FD_LD.id;
//		return strcmp(sec_id, m_R_FD_LD.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_LD.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_LD& m_R_FD_LD) const
	{
		return id < m_R_FD_LD.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return ld_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return ld_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_LD )
};

class  R_FD_ND //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	char	nd_id[MAX_CB_ID_LEN];
	int		fd_id_ptr;
	int		nd_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_ND();
	R_FD_ND(const char* m_fd_id, const char* m_nd_id);

	//assign value operator
	const R_FD_ND& operator = (const R_FD_ND& m_R_FD_ND);

	//compare operator ==
	int operator == (const R_FD_ND& m_R_FD_ND) const
	{
		return id == m_R_FD_ND.id;
//		return strcmp(sec_id, m_R_FD_ND.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_ND.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_ND& m_R_FD_ND) const
	{
		return id < m_R_FD_ND.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get chiND id
	const char* GetChildId() const
	{
		return nd_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get chiND ptr
	int GetChildPtr() const
	{
		return nd_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_ND )
};
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_EQ //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	int		eq_id;
	int		fd_id_ptr;
	int		eq_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_EQ();
	R_FD_EQ(const char* m_fd_id, int m_eq_id);

	//assign value operator
	const R_FD_EQ& operator = (const R_FD_EQ& m_R_FD_EQ);

	//compare operator ==
	int operator == (const R_FD_EQ& m_R_FD_EQ) const
	{
		return id == m_R_FD_EQ.id;
//		return strcmp(sec_id, m_R_FD_EQ.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_EQ.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_EQ& m_R_FD_EQ) const
	{
		return id < m_R_FD_EQ.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get child id
	const int GetChildId() const
	{
		return eq_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return eq_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_EQ )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_ZN //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_SEC_ID_LEN];
	int		zn_id;
	int		fd_id_ptr;
	int		zn_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_ZN();
	R_FD_ZN(const char* m_fd_id, const int m_zn_id);

	//assign value operator
	const R_FD_ZN& operator = (const R_FD_ZN& m_R_FD_ZN);

	//compare operator ==
	int operator == (const R_FD_ZN& m_R_FD_ZN) const
	{
		return id == m_R_FD_ZN.id;
//		return strcmp(sec_id, m_R_FD_ZN.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_ZN.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_ZN& m_R_FD_ZN) const
	{
		return id < m_R_FD_ZN.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get child id
	const int GetChildId() const
	{
		return zn_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return zn_id_ptr;
	}

	//read or write object
    ////void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_ZN )
};


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class  R_FD_SO //: public CObject
{
public:
	long	id;

	char	fd_id[MAX_ZN_ID_LEN];
	char	so_id[MAX_SEC_ID_LEN];
	int		fd_id_ptr;
	int		so_id_ptr;
	int		fptr;
	int		bptr;
    char    ver[MAX_VER_LEN];

public:

	//constructors
	R_FD_SO();
	R_FD_SO(const char* m_fd_id, const char* m_so_id);

	//assign value operator
	const R_FD_SO& operator = (const R_FD_SO& m_R_FD_SO);

	//compare operator ==
	int operator == (const R_FD_SO& m_R_FD_SO) const
	{
		return id == m_R_FD_SO.id;
//		return strcmp(sec_id, m_R_FD_SO.sec_id) == 0 && 
//			   strcmp(zn_id, m_R_FD_SO.zn_id) == 0;
	}

	//compare operator <
	int operator < (const R_FD_SO& m_R_FD_SO) const
	{
		return id < m_R_FD_SO.id;
	}

	//get parent id
	const char* GetParentId() const
	{
		return fd_id;
	}

	//get child id
	const char* GetChildId() const
	{
		return so_id;
	}

	//get parent ptr
	int GetParentPtr() const
	{
		return fd_id_ptr;
	}

	//get child ptr
	int GetChildPtr() const
	{
		return so_id_ptr;
	}

	//read or write object
    //void Serialize( CArchive& ar );

protected:

	 //DECLARE_SERIAL( R_FD_SO )
};

#endif