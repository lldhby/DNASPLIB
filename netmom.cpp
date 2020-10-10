// netmom.cpp: //IMPLEMENTation of the netmom class.
//
//////////////////////////////////////////////////////////////////////
#if _WIN32
#include "../DNASPLIB/netmom.h"
#elif __linux__
//#include "D:/lld_common/DPCAS4.1/netmom.h"
#else
#endif

#include "../DNASPLIB/netmom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char  THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


VL::VL()
{
	mRID[0] = 0;
	name[0] = 0;
	highkV = 0;
	lowkV = 0;
	BaseVoltage[0] = 0;
	Substation[0] = 0;
}

const VL& VL::operator = (const VL& m_VL)
{
	strcpy(mRID, m_VL.mRID);
	strcpy(name, m_VL.name);
	strcpy(BaseVoltage, m_VL.BaseVoltage);
	strcpy(Substation, m_VL.Substation);
	highkV = m_VL.highkV;
	lowkV = m_VL.lowkV;

	return*this;
}

VL::VL(const char * m_mRID)
{
	strcpy(mRID, m_mRID);
	name[0] = 0;
	highkV = 0;
	lowkV = 0;
	BaseVoltage[0] = 0;
	Substation[0] = 0;
}
MINPATH::MINPATH()
{
	int i = 0;
	id=-1;//最小路的编号，物理逻辑同名
	type=-1;//最小路的类型，0为负荷，1为备用，2为联络

	eq_id[0]=0;//设备ID号，可以是负荷或联络开关 
	ind=-1;	//节点
	s=0;		//潮流计算得到的视在功率，联络开关为裕度，负荷为视在功率值
	monkV=0;	//变压器的铭牌上都是视在功率(视在功率)
	for (i=0; i<MAX_MINPATH_LEN; i++)
		minpath[i]=-1;//负荷的最小路，存最多的200个节点 
	pathlen=0;//最小路的长度 
	cntbakpath=0;//备用最小路的条数，一个负荷点可能有多个备用最小路，负荷点专用
	
	for (i=0; i<MAX_MINPATH_LEN; i++)
		equippath[i]=-1;//负荷的最小路，存最多的200个节点 

	for (i=0; i<MAX_MINPATH_LEN; i++)
		bakpath[i]=-1;//负荷的最小路，存最多的200个节点 
	bakpathlen=0;//备用最小路的长度 
	bakAbund=0;//备用联络开关的裕度
	
	//联络开关到电源点的路径
	for (i=0; i<MAX_MINPATH_LEN; i++)
		unso[i]=-1;//负荷的最小路，存最多的200个节点 
	for (i=0; i<MAX_MINPATH_LEN; i++)
		unso_reverse[i]=-1;//负荷的最小路，存最多的200个节点 
	unso_reverse_len=0;
	unso_len=0;

	q=0;	//标志位
	level=0;	//负荷的等级，一类负荷（重要），二类负荷（一般），三类负荷（农网）
}

void MINPATH::Reset()
{
	int i = 0;
	id=-1;//最小路的编号，物理逻辑同名
	type=-1;//最小路的类型，0为负荷，1为备用，2为联络

	eq_id[0]=0;//设备ID号，可以是负荷或联络开关 
	ind=-1;	//节点
	s=0;		//潮流计算得到的视在功率，联络开关为裕度，负荷为视在功率值
	monkV=0;	//变压器的铭牌上都是视在功率(视在功率)
	for (i=0; i<MAX_MINPATH_LEN; i++)
		minpath[i]=-1;//负荷的最小路，存最多的200个节点 
	pathlen=0;//最小路的长度 
	cntbakpath=0;//备用最小路的条数，一个负荷点可能有多个备用最小路，负荷点专用
	
	for (i=0; i<MAX_MINPATH_LEN; i++)
		equippath[i]=-1;//负荷的最小路，存最多的200个节点 

	for (i=0; i<MAX_MINPATH_LEN; i++)
		bakpath[i]=-1;//负荷的最小路，存最多的200个节点 
	bakpathlen=0;//备用最小路的长度 
	bakAbund=0;//备用联络开关的裕度
	
	//联络开关到电源点的路径
	for (i=0; i<MAX_MINPATH_LEN; i++)
		unso[i]=-1;//负荷的最小路，存最多的200个节点 
	for (i=0; i<MAX_MINPATH_LEN; i++)
		unso_reverse[i]=-1;//负荷的最小路，存最多的200个节点 

	unso_reverse_len=0;
	unso_len=0;
	q=0;	//标志位
	level=0;	//负荷的等级，一类负荷（重要），二类负荷（一般），三类负荷（农网）
}

MINPATH::MINPATH(const int m_id)
{
	int i = 0;
	id=m_id;//最小路的编号，物理逻辑同名
	type=-1;//最小路的类型，0为负荷，1为备用，2为联络

	eq_id[0]=0;//设备ID号，可以是负荷或联络开关 
	ind=-1;	//节点
	s=0;		//潮流计算得到的视在功率，联络开关为裕度，负荷为视在功率值
	monkV=0;	//变压器的铭牌上都是视在功率(视在功率)
	for (i=0; i<MAX_MINPATH_LEN; i++)
		minpath[i]=-1;//负荷的最小路，存最多的200个节点 
	pathlen=0;//最小路的长度 
	cntbakpath=0;//备用最小路的条数，一个负荷点可能有多个备用最小路，负荷点专用
	
	for (i=0; i<MAX_MINPATH_LEN; i++)
		equippath[i]=-1;//负荷的最小路，存最多的200个节点 

	for (i=0; i<MAX_MINPATH_LEN; i++)
		bakpath[i]=-1;//负荷的最小路，存最多的200个节点 
	bakpathlen=0;//备用最小路的长度 
	bakAbund=0;//备用联络开关的裕度
	
	//联络开关到电源点的路径
	for (i=0; i<MAX_MINPATH_LEN; i++)
		unso[i]=-1;//负荷的最小路，存最多的200个节点 
	for (i=0; i<MAX_MINPATH_LEN; i++)
		unso_reverse[i]=-1;//负荷的最小路，存最多的200个节点 

	unso_reverse_len=0;
	unso_len=0;
	q=0;	//标志位
	level=0;	//负荷的等级，一类负荷（重要），二类负荷（一般），三类负荷（农网）
}

const MINPATH& MINPATH::operator = (const MINPATH& m_MINPATH)
{
	int i = 0;
	id=m_MINPATH.id;//最小路的编号，物理逻辑同名
	type=m_MINPATH.type;//最小路的类型，0为负荷，1为备用，2为联络

	strcpy(eq_id,m_MINPATH.eq_id);//设备ID号，可以是负荷或联络开关 
	ind=m_MINPATH.ind;	//节点
	s=m_MINPATH.s;		//潮流计算得到的视在功率，联络开关为裕度，负荷为视在功率值
	monkV=m_MINPATH.monkV;	//变压器的铭牌上都是视在功率(视在功率)
	for (i=0; i<MAX_MINPATH_LEN; i++)
		minpath[i]=m_MINPATH.minpath[i];//负荷的最小路，存最多的200个节点 
	pathlen=m_MINPATH.pathlen;//最小路的长度 
	cntbakpath=m_MINPATH.cntbakpath;//备用最小路的条数，一个负荷点可能有多个备用最小路，负荷点专用
	
	for (i=0; i<MAX_MINPATH_LEN; i++)
		equippath[i]=m_MINPATH.equippath[i];//负荷的最小路，存最多的200个节点 

	for (i=0; i<MAX_MINPATH_LEN; i++)
		bakpath[i]=m_MINPATH.bakpath[i];//负荷的最小路，存最多的200个节点 
	bakpathlen=m_MINPATH.bakpathlen;//备用最小路的长度 
	bakAbund=m_MINPATH.bakAbund;//备用联络开关的裕度
	
	//联络开关到电源点的路径
	for (i=0; i<MAX_MINPATH_LEN; i++)
		unso[i]=m_MINPATH.unso[i];//负荷的最小路，存最多的200个节点 
	for (i=0; i<MAX_MINPATH_LEN; i++)
		unso_reverse[i]=m_MINPATH.unso_reverse[i];//负荷的最小路，存最多的200个节点 

	unso_reverse_len=m_MINPATH.unso_reverse_len;
	unso_len=m_MINPATH.unso_len;

	q=m_MINPATH.q;	//标志位
	level=m_MINPATH.level;	//负荷的等级，一类负荷（重要），二类负荷（一般），三类负荷（农网）

	return* this;
}




//单条馈线上的设备，用于可靠性计算
CIMEDATA::CIMEDATA()
{
	id[0] = 0;//设备ID号 
	data = 0;
	type = -1;
}

CIMEDATA::CIMEDATA(const char * m_id)
{
	strcpy(id,m_id);//设备ID号 
	data = 0;
	type = -1;
}

const CIMEDATA& CIMEDATA::operator = (const CIMEDATA& m_CIMEDATA)
{
	strcpy(id,m_CIMEDATA.id);
	data = m_CIMEDATA.data;//序号，用于排序和快速搜索
	type = m_CIMEDATA.type;//设备类型 0-breaker；1-loadbreakswitch；2-disconnector；3-fuse；5-带保护开关；10-线路；20-负荷

	return* this;
}

/*
void CIMEDATA::Serialize( CArchive& ar )
{
	if( ar.IsStoring() )
	{

	}
	else
	{

	}
}
*/

//单条馈线上的设备，用于可靠性计算
EQ::EQ()
{
	int i = 0;
 	i_id = -1;//序号，用于排序和快速搜索

	type = -1;//设备类型 0-breaker；1-loadbreakswitch；2-disconnector；3-fuse；5-带保护开关；10-线路；20-负荷
 	idph = -1;//序号，用于排序和快速搜索
	id[0] = 0;//设备ID号 
	fdid[0] = 0;//设备ID号 
	ind = -1;	//首节点
	iznd = -1;	//末节点
	q = 0;	//标志位
	island = 0;
	r_fd = -1;
	r_zn = -1;

	for (i=0; i<MAX_MINPATH_LEN; i++)
		minpath[i] = -1;//设备的最小路，存最多的50个节点 
	pathlen = 0;

	prob_fault=0;//设备故障率
	repairTime=0;//设备修复时间
	
	prob_check=0;//设备检修率
	checkTime=0;//设备检修时间
	
	switchTime=0;//设备切换隔离时间
	transTime=0;//转供时间	

	fema = 0;
	fema_sum = 0;
	lambda = 0;
	lambda_sum = 0;

	ld_cnt_loss = 0;//失电用户数（户）
	ENS = 0;	//受影响用户平均缺供电量 (kWh/户)
	w_loss = 0;//平均失电负荷(kW)

	nUpSwitchload=0;
	nUpNonOutload=0;
	nUpPlanSwitchload=0;
	nUpPlanNonOutload=0;
	nFaultload=0;
	nNeedTrload=0;
	nNowTrload=0;
	for (i=0; i<max_switch_ld; i++)
	{
		UpSwitchload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpNonOutload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpPlanSwitchload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpPlanNonOutload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		Faultload[i] = -1;	//故障段失电负荷列表
		NeedTrload[i] = -1;//故障段下游需要转供的负荷列表
		NowTrload[i] = -1;//故障段下游可以转供的负荷列表
	}

	tranfdlist[0] = 0;//设备发生故障后的转供馈线列表
	closeunlist[0] = 0;//设备发生故障后的 闭合 联络开关列表
	opencblist[0] = 0; //设备发生故障后的 打开 分段开关列表
}

void EQ::Reset()
{
	int i = 0;
 	i_id = -1;//序号，用于排序和快速搜索

	type = -1;//设备类型 0-breaker；1-loadbreakswitch；2-disconnector；3-fuse；5-带保护开关；10-线路；20-负荷
 	idph = -1;//序号，用于排序和快速搜索
	id[0] = 0;//设备ID号 
	fdid[0] = 0;//设备ID号 
	ind = -1;	//首节点
	iznd = -1;	//末节点
	q = 0;	//标志位
	island = 0;
	r_fd = -1;
	r_zn = -1;

	for (i=0; i<MAX_MINPATH_LEN; i++)
		minpath[i] = -1;//设备的最小路，存最多的50个节点 
	pathlen = 0;

	prob_fault=0;//设备故障率
	repairTime=0;//设备修复时间
	
	prob_check=0;//设备检修率
	checkTime=0;//设备检修时间
	
	switchTime=0;//设备切换隔离时间
	transTime=0;//转供时间	

	fema = 0;
	fema_sum = 0;
	lambda = 0;
	lambda_sum = 0;

	ld_cnt_loss = 0;//失电用户数（户）
	ENS = 0;	//受影响用户平均缺供电量 (kWh/户)
	w_loss = 0;//平均失电负荷(kW)

	nUpSwitchload=0;
	nUpNonOutload=0;
	nUpPlanSwitchload=0;
	nUpPlanNonOutload=0;
	nFaultload=0;
	nNeedTrload=0;
	nNowTrload=0;
	for (i=0; i<max_switch_ld; i++)
	{
		UpSwitchload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpNonOutload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpPlanSwitchload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpPlanNonOutload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		Faultload[i] = -1;	//故障段失电负荷列表
		NeedTrload[i] = -1;//故障段下游需要转供的负荷列表
		NowTrload[i] = -1;//故障段下游可以转供的负荷列表
	}

	tranfdlist[0] = 0;//设备发生故障后的转供馈线列表
	closeunlist[0] = 0;//设备发生故障后的 闭合 联络开关列表
	opencblist[0] = 0; //设备发生故障后的 打开 分段开关列表
}

EQ::EQ(const int m_id)
{
	int i = 0;
 	i_id = m_id;//序号，用于排序和快速搜索

	type = -1;//设备类型 0-breaker；1-loadbreakswitch；2-disconnector；3-fuse；5-带保护开关；10-线路；20-负荷
	fdid[0] = 0;//设备ID号 
 	idph = -1;//序号，用于排序和快速搜索
	id[0] = 0;//设备ID号 
	ind = -1;	//首节点
	iznd = -1;	//末节点
	q = 0;	//标志位
	island = 0;
	r_fd = -1;
	r_zn = -1;
	for (i=0; i<MAX_MINPATH_LEN; i++)
		minpath[i] = -1;//设备的最小路，存最多的50个节点 
	pathlen = 0;

	prob_fault=0;//设备故障率
	repairTime=0;//设备修复时间
	
	prob_check=0;//设备检修率
	checkTime=0;//设备检修时间
	
	switchTime=0;//设备切换隔离时间
	transTime=0;//转供时间	

	fema = 0;
	fema_sum = 0;
	lambda = 0;
	lambda_sum = 0;

	ld_cnt_loss = 0;//失电用户数（户）
	ENS = 0;	//受影响用户平均缺供电量 (kWh/户)
	w_loss = 0;//平均失电负荷(kW)

	nUpSwitchload=0;
	nUpNonOutload=0;
	nUpPlanSwitchload=0;
	nUpPlanNonOutload=0;
	nFaultload=0;
	nNeedTrload=0;
	nNowTrload=0;
	for (i=0; i<max_switch_ld; i++)
	{
		UpSwitchload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpNonOutload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpPlanSwitchload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpPlanNonOutload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		Faultload[i] = -1;	//故障段失电负荷列表
		NeedTrload[i] = -1;//故障段下游需要转供的负荷列表
		NowTrload[i] = -1;//故障段下游可以转供的负荷列表
	}

	tranfdlist[0] = 0;//设备发生故障后的转供馈线列表
	closeunlist[0] = 0;//设备发生故障后的 闭合 联络开关列表
	opencblist[0] = 0; //设备发生故障后的 打开 分段开关列表
}

const EQ& EQ::operator = (const EQ& m_EQUIP)
{
	int i = 0;
 	idph = m_EQUIP.idph;//序号，用于排序和快速搜索
	strcpy(id,m_EQUIP.id);
	strcpy(fdid,m_EQUIP.fdid);

 	i_id = m_EQUIP.i_id;//序号，用于排序和快速搜索

	type = m_EQUIP.type;//设备类型 0-breaker；1-loadbreakswitch；2-disconnector；3-fuse；5-带保护开关；10-线路；20-负荷
	ind = m_EQUIP.ind;	//首节点
	iznd = m_EQUIP.iznd;	//末节点
	q = m_EQUIP.q;	//标志位
	island = m_EQUIP.island;	//标志位
	r_fd = m_EQUIP.r_fd;	//末节点
	r_zn = m_EQUIP.r_zn;	//末节点

	for (i=0; i<MAX_MINPATH_LEN; i++)
		minpath[i] = m_EQUIP.minpath[i];//设备的最小路，存最多的50个节点 
	pathlen = m_EQUIP.pathlen;

	prob_fault=m_EQUIP.prob_fault;//设备故障率
	repairTime=m_EQUIP.repairTime;//设备修复时间
	
	prob_check=m_EQUIP.prob_check;//设备检修率
	checkTime=m_EQUIP.checkTime;//设备检修时间
	
	switchTime=m_EQUIP.switchTime;//设备切换隔离时间
	transTime=m_EQUIP.transTime;//转供时间	

	fema=m_EQUIP.fema;//	
	fema_sum=m_EQUIP.fema_sum;//	
	lambda=m_EQUIP.lambda;//	
	lambda_sum=m_EQUIP.lambda_sum;//	

	ld_cnt_loss = m_EQUIP.ld_cnt_loss;//	
	ENS = m_EQUIP.ENS;//	
	w_loss = m_EQUIP.w_loss;//	

	nUpSwitchload=m_EQUIP.nUpSwitchload;
	nUpNonOutload=m_EQUIP.nUpNonOutload;
	nUpPlanSwitchload=m_EQUIP.nUpPlanSwitchload;
	nUpPlanNonOutload=m_EQUIP.nUpPlanNonOutload;
	nFaultload=m_EQUIP.nFaultload;
	nNeedTrload=m_EQUIP.nNeedTrload;
	nNowTrload=m_EQUIP.nNowTrload;
	for (i=0; i<max_switch_ld; i++)
	{
		UpSwitchload[i] = m_EQUIP.UpSwitchload[i];//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpNonOutload[i] = m_EQUIP.UpNonOutload[i];//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpPlanSwitchload[i] = m_EQUIP.UpPlanSwitchload[i];//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		UpPlanNonOutload[i] = m_EQUIP.UpPlanNonOutload[i];//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
		Faultload[i] = m_EQUIP.Faultload[i];	//故障段失电负荷列表
		NeedTrload[i] = m_EQUIP.NeedTrload[i];//故障段下游需要转供的负荷列表
		NowTrload[i] = m_EQUIP.NowTrload[i];//故障段下游可以转供的负荷列表
	}
	strcpy(tranfdlist, m_EQUIP.tranfdlist);//设备发生故障后的转供馈线列表
	strcpy(closeunlist, m_EQUIP.closeunlist);//设备发生故障后的 闭合 联络开关列表
	strcpy(opencblist, m_EQUIP.opencblist); //设备发生故障后的 打开 分段开关列表

	return* this;
}


R_ND_EQ::R_ND_EQ()
{
	id = -1;

    nd_id[0] = 0;
    i_id = -1;
    nd_id_ptr = -1;
    equip_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ND_EQ& R_ND_EQ::operator = (const R_ND_EQ& m_r_nd_equip)
{
	id = m_r_nd_equip.id;
	strcpy(nd_id,m_r_nd_equip.nd_id);
	i_id = m_r_nd_equip.i_id;
	nd_id_ptr = m_r_nd_equip.nd_id_ptr;
	equip_id_ptr = m_r_nd_equip.equip_id_ptr;
	fptr = m_r_nd_equip.fptr;
	bptr = m_r_nd_equip.bptr;
	return* this;
}

R_ND_EQ::R_ND_EQ(const char * m_nd_id, const int m_equip_id)
{
	id = -1;
    strcpy(nd_id,m_nd_id);
    i_id=m_equip_id;
    nd_id_ptr = -1;
    equip_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

/////////////////////////////////////////////////////////////////////////
// class ANALOG //IMPLEMENTation

//IMPLEMENT_SERIAL(ANALOG,CObject,0)

ANALOG::ANALOG()
{
    id[0] = 0;
    descr[0] = 0;
    value = 0;
    quality = 0;
    analogType = 0;
    loc[0] = 0;
    
    ver[0] = 0;
    fdid[0]=0;
    fds=0;
}

const ANALOG& ANALOG::operator = (const ANALOG& m_ANALOG)
{
	strcpy(id,m_ANALOG.id);
	strcpy(descr,m_ANALOG.descr);
	value = m_ANALOG.value;
	quality = m_ANALOG.quality;
	analogType = m_ANALOG.analogType;
	strcpy(loc,m_ANALOG.loc);
	
	strcpy(ver,m_ANALOG.ver);
	strcpy(fdid,m_ANALOG.fdid);
	fds = m_ANALOG.fds;

	return* this;
}

ANALOG::ANALOG(const char * m_id)
{
    strcpy(id, m_id);
    descr[0] = 0;
    value = 0.0;
    quality = 0;
	analogType = 0;
    loc[0] = 0;
    ver[0] = 0;

    fdid[0]=0;
    fds=0;

}

/////////////////////////////////////////////////////////////////////////
// class BS //IMPLEMENTation

//IMPLEMENT_SERIAL(BS,CObject,0)

BS::BS()
{
    id = 0;
    iisland = -2;
    q = 0;
    qq = 0;
    fbs = -1;
    bbs = -1;
    pnd = 0;
    kv = 0;
    v = 0.0;
    adeg = 00.0;
    w = 0;
    r = 0;

    ver[0] = 0;
    fdid[0] = 0;

	Qv =0;
}

const BS& BS::operator = (const BS& m_BS)
{
	id = m_BS.id;
	iisland = m_BS.iisland;
	q = m_BS.q;
	qq = m_BS.qq;
	fbs = m_BS.fbs;
	bbs = m_BS.bbs;
	pnd = m_BS.pnd;
	kv = m_BS.kv;
	v = m_BS.v;
	adeg = m_BS.adeg;
	w = m_BS.w;
	r = m_BS.r;

	Qv = m_BS.Qv;

    strcpy(ver, m_BS.ver);
    strcpy(fdid, m_BS.fdid);

	return* this;
}

BS::BS(const int m_id)
{
    id = m_id;
    iisland = -2;
    q = 0;
    qq = 0;
    fbs = -1;
    bbs = -1;
    pnd = 0;
    kv = 0.0;
    v = 0.0;
    adeg = 0.0;
    w = 0.0;
    r = 0.0;
    ver[0] = 0;
    fdid[0] = 0;
	Qv =0;

}

/////////////////////////////////////////////////////////////////////////
// class BUS //IMPLEMENTation

//IMPLEMENT_SERIAL(BUS,CObject,0)

BUS::BUS()
{
    id[0] = 0;
    descr[0] = 0;
    nd[0] = 0;
    ind = -1;
    ibs = -1;
    algv[0] = 0;
    ialgv = -1;
    topflags = 0;
    vtarget = 0;
    vl = 10;
    vhlim = 999;
    vllim = 0;
    v = 1.0;
    adeg = 0.0;
    r_st = -1;

    ver[0] = 0;
    fdid[0]=0;

	fds=0;
	basevoltage = 0;
}

const BUS& BUS::operator = (const BUS& m_BUS)
{
	strcpy(id,m_BUS.id);
	strcpy(descr,m_BUS.descr);
	strcpy(nd,m_BUS.nd);
	ind = m_BUS.ind;
	ibs = m_BUS.ibs;
	strcpy(algv,m_BUS.algv);
	ialgv = m_BUS.ialgv;
	topflags = m_BUS.topflags;
	vtarget = m_BUS.vtarget;
	vl = m_BUS.vl;
	vhlim = m_BUS.vhlim;
	vllim = m_BUS.vllim;
	v = m_BUS.v;
	adeg = m_BUS.adeg;
	r_st = m_BUS.r_st;
	
	strcpy(ver,m_BUS.ver);
	strcpy(fdid,m_BUS.fdid);

	fds = m_BUS.fds;
	
	basevoltage = m_BUS.basevoltage;
	return* this;
}
BUS::BUS(const char * m_id)
{
    strcpy(id, m_id);
    descr[0] = 0;
    nd[0] = 0;
    ind = -1;
    ibs = -1;
    algv[0] = 0;
    ialgv = -1;
    topflags = 0;
    vtarget = 0.0;
    vl = 10;
    vhlim = 999;
    vllim = 0.0;
    v = 1.0;
    adeg = 0.0;
    r_st = -1;

    ver[0] = 0;
    fdid[0]=0;

	fds=0;
	basevoltage = 0;
}

/////////////////////////////////////////////////////////////////////////
// class CB //IMPLEMENTation

//IMPLEMENT_SERIAL(CB,CObject,0)

CB::CB()
{
    id[0] = 0;
    descr[0] = 0;
    nd[0] = 0;
    znd[0] = 0;
    ind = -1;
    iznd = -1;
    digital[0] = 0;
    idigital = -1;
    q = 0;
    topflags = 0;
    r_st = -1;

    ver[0] = 0;
    fdid[0]=0;

	maincb = 0;
	fds=0;
	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;
	cbtype = -1;
	vl[0] = 0;
}

const CB& CB::operator = (const CB& m_CB)
{
	strcpy(id,m_CB.id);
	strcpy(descr,m_CB.descr);
	strcpy(nd,m_CB.nd);
	strcpy(znd,m_CB.znd);
	ind = m_CB.ind;
	iznd = m_CB.iznd;
	strcpy(digital,m_CB.digital);
	idigital = m_CB.idigital;
	q = m_CB.q;
	topflags = m_CB.topflags;
	r_st = m_CB.r_st;
	
	strcpy(ver,m_CB.ver);
	strcpy(fdid,m_CB.fdid);

	maincb = m_CB.maincb;
	fds = m_CB.fds;

	prob_fault=m_CB.prob_fault;//故障率
	repairTime=m_CB.repairTime;//5.5小时 故障修复时间;
	cbtype=m_CB.cbtype;//5.5小时 故障修复时间;
	
	strcpy(vl, m_CB.vl);
	return* this;
}

CB::CB(const char * m_id)
{
    strcpy(id, m_id);
    descr[0] = 0;
    nd[0] = 0;
    znd[0] = 0;
    ind = -1;
    iznd = -1;
    digital[0] = 0;
    idigital = -1;
    q = 0;
    topflags = 0;
    r_st = -1;

    ver[0] = 0;
    fdid[0]=0;

	maincb = 0;
	fds=0;
	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;
	cbtype = -1;
	vl[0] = 0;
}

/////////////////////////////////////////////////////////////////////////
// class CO //IMPLEMENTation

//IMPLEMENT_SERIAL(CO,CObject,0)

CO::CO()
{
    id[0] = 0;
    descr[0] = 0;
    wloss = 0;
    rloss = 0;
    wso = 0;
    rso = 0;
    wload = 0;
    rload = 0;
    cap = 0;
    reac = 0;
    r_st = -1;

    ver[0] = 0;
}

const CO& CO::operator = (const CO& m_CO)
{
	strcpy(id,m_CO.id);
	strcpy(descr,m_CO.descr);
	wloss = m_CO.wloss;
	rloss = m_CO.rloss;
	wso = m_CO.wso;
	rso = m_CO.rso;
	wload = m_CO.wload;
	rload = m_CO.rload;
	cap = m_CO.cap;
	reac = m_CO.reac;
	r_st = m_CO.r_st;
	
	strcpy(ver,m_CO.ver);
	
	return* this;
}

CO::CO(const char * m_id)
{
    strcpy(id, m_id);
    descr[0] = 0;
    wloss = 0.0;
    rloss = 0.0;
    wso = 0.0;
    rso = 0.0;
    wload = 0.0;
    rload = 0.0;
    cap = 0.0;
    reac = 0.0;
    r_st = -1;
    
    ver[0] = 0;
}

/////////////////////////////////////////////////////////////////////////
// class CP //IMPLEMENTation

//IMPLEMENT_SERIAL(CP,CObject,0)

CP::CP()
{
   id[0] = 0;
    descr[0] = 0;
    nd[0] = 0;
	znd[0] = 0;
	ind = -1;
    ibs = -1;
	iznd = -1;
	zbs = -1;
	algq[0] = 0;
    ialgq = -1;
    q = 0;
    topflags = 0;
    mrnom = 0;
    r = 0;
    r_st = -1;
	r_zn = -1;

	w = 0;
	r_ = 0;
	wz = 0;
	rz = 0;

	v_b = 0;
	adeg_b = 0;
	v_e = 0;
	adeg_e = 0;
	i = 0;

    vl[0] = 0;
    ver[0] = 0;
    fdid[0]=0;

	fds=0;
	basevoltage = 0;
}

const CP& CP::operator = (const CP& m_CP)
{
	strcpy(id,m_CP.id);
	strcpy(descr,m_CP.descr);
	strcpy(nd,m_CP.nd);
	strcpy(znd, m_CP.znd);
	ind = m_CP.ind;
	ibs = m_CP.ibs;
	iznd = m_CP.iznd;
	zbs = m_CP.zbs;
	strcpy(algq,m_CP.algq);
	ialgq = m_CP.ialgq;
	q = m_CP.q;
	topflags = m_CP.topflags;
	mrnom = m_CP.mrnom;
	r = m_CP.r;
	r_st = m_CP.r_st;
	r_zn = m_CP.r_zn;
	
	w = m_CP.w;
	wz = m_CP.wz;
	r_ = m_CP.r_;
	rz = m_CP.rz;
	v_b = m_CP.v_b;
	adeg_b = m_CP.adeg_b;
	v_e = m_CP.v_e;
	adeg_e = m_CP.adeg_e;
	i = m_CP.i;

	strcpy(vl,m_CP.vl);
	strcpy(ver,m_CP.ver);
	strcpy(fdid,m_CP.fdid);
	
	fds = m_CP.fds;
	basevoltage = m_CP.basevoltage;
	return* this;
}
CP::CP(const char* m_id)
{
    strcpy(id, m_id);
    descr[0] = 0;
	nd[0] = 0;
	znd[0] = 0;
	ind = -1;
	ibs = -1;
	iznd = -1;
	zbs = -1;
	algq[0] = 0;
    ialgq = -1;
    q = 0;
    topflags = 0;
    mrnom = 0.0;
    r = 0.0;
    r_st = -1;
	r_zn = -1;

	w = 0;
	r_ = 0;
	wz = 0;
	rz = 0;

	v_b = 0;
	adeg_b = 0;
	v_e = 0;
	adeg_e = 0;

    vl[0] = 0;
    ver[0] = 0;
    fdid[0]=0;

	fds=0;
	basevoltage = 0;
}

/////////////////////////////////////////////////////////////////////////
// class DIGITAL //IMPLEMENTation

//IMPLEMENT_SERIAL(DIGITAL,CObject,0)

DIGITAL::DIGITAL()
{
    id[0] = 0;
    descr[0] = 0;
    value = 0;
    quality = 0;

    ver[0] = 0;
    fdid[0]=0;
	fds=0;

}

const DIGITAL& DIGITAL::operator = (const DIGITAL& m_DIGITAL)
{
	strcpy(id,m_DIGITAL.id);
	strcpy(descr,m_DIGITAL.descr);
	value = m_DIGITAL.value;
	quality = m_DIGITAL.quality;
	
	strcpy(ver,m_DIGITAL.ver);
	strcpy(fdid,m_DIGITAL.fdid);
	
	fds = m_DIGITAL.fds;
	return* this;
}
DIGITAL::DIGITAL(const char* m_id)
{
    strcpy(id, m_id);
    descr[0] = 0;
    value = 0;
    quality = 0;

    ver[0] = 0;
    fdid[0]=0;
	fds=0;
}

/////////////////////////////////////////////////////////////////////////
// class GLOBAL //IMPLEMENTation

//IMPLEMENT_SERIAL(GLOBAL,CObject,0)

GLOBAL::GLOBAL()
{
    id = 0;
    nullbs = 0;
    nextbs = 0;
//    tptime = CTime::GetCurrentTime().GetTime();
  //  timedb = CTime::GetCurrentTime().GetTime();
    statusdb = 0;
    error = 0;
    dberrnum = 0;
}

const GLOBAL& GLOBAL::operator = (const GLOBAL& m_GLOBAL)
{
	id = m_GLOBAL.id;
	nullbs = m_GLOBAL.nullbs;
	nextbs = m_GLOBAL.nextbs;
	//tptime = m_GLOBAL.tptime;
	//timedb = m_GLOBAL.timedb;
	statusdb = m_GLOBAL.statusdb;
	error = m_GLOBAL.error;
	dberrnum = m_GLOBAL.dberrnum;
	return* this;
}

GLOBAL::GLOBAL(const int m_id)
{
    id = m_id;
    nullbs = 0;
    nextbs = 0;
//    tptime = CTime::GetCurrentTime().GetTime();
  //  timedb = CTime::GetCurrentTime().GetTime();
    statusdb = 0;
    error = 0;
    dberrnum = 0;
}

/////////////////////////////////////////////////////////////////////////
// class ISLAND //IMPLEMENTation

//IMPLEMENT_SERIAL(ISLAND,CObject,0)

ISLAND::ISLAND()
{
    id = 0;
    refbs = 0;
    reffd = 0;
    refminv = 0;
	refmaxv = 100000;
	wload = 0;
    rload = 0;
    wso = 0;
    rso = 0;
    
    ver[0]=0;
    fdid[0]=0;

}

const ISLAND& ISLAND::operator = (const ISLAND& m_ISLAND)
{
	id = m_ISLAND.id;
	refbs = m_ISLAND.refbs;
	reffd = m_ISLAND.reffd;
	refminv = m_ISLAND.refminv;
	refmaxv = m_ISLAND.refmaxv;
	wload = m_ISLAND.wload;
	rload = m_ISLAND.rload;
	wso = m_ISLAND.wso;
	rso = m_ISLAND.rso;
	
	strcpy(ver,m_ISLAND.ver);
	strcpy(fdid,m_ISLAND.fdid);
	
	return* this;
}

ISLAND::ISLAND(const int m_id)
{
    id = m_id;
    refbs = 0;
	reffd = 0;
	refminv = 0;
	refmaxv = 100000;
	wload = 0.0;
    rload = 0.0;
    wso = 0.0;
    rso = 0.0;
    ver[0]=0;
    fdid[0]=0;
}

/////////////////////////////////////////////////////////////////////////
// class UN //IMPLEMENTation

//IMPLEMENT_SERIAL(UN,CObject,0)

UN::UN()
{
    id[0] = 0;
	idph = 0;
//    descr[0] = 0;
    left[0] = 0;
    right[0] = 0;
    l_nd[0] = 0;
    r_nd[0] = 0;
    q = 0;
    topflags = 0;
	
	r_fd = -1;

	l_abund_P = 5;//默认是5的裕度
	l_abund_Q = 0;
	l_abund_V = 1;
	r_abund_P = 5;
	r_abund_Q = 0;
	r_abund_V = 1;
	
    ver[0]=0;
}

const UN& UN::operator = (const UN& m_UN)
{
	strcpy(id , m_UN.id);
//	strcpy(descr,m_UN.descr);
	strcpy(left,m_UN.left);
	strcpy(right,m_UN.right);
	strcpy(l_nd,m_UN.l_nd);
	strcpy(r_nd,m_UN.r_nd);

	idph = m_UN.idph;
	q = m_UN.q;
	topflags = m_UN.topflags;
	r_fd = m_UN.r_fd;	
	l_abund_P = m_UN.l_abund_P;	
	l_abund_Q = m_UN.l_abund_Q;	
	l_abund_V = m_UN.l_abund_V;	
	r_abund_P = m_UN.r_abund_P;	
	r_abund_Q = m_UN.r_abund_Q;	
	r_abund_V = m_UN.r_abund_V;	

	strcpy(ver,m_UN.ver);
	
	return* this;
}

UN::UN(const char* m_id)
{
    strcpy(id,m_id);
//   descr[0] = 0;
	idph = 0;
    left[0] = 0;
    right[0] = 0;
    l_nd[0] = 0;
    r_nd[0] = 0;
    q = 0;
    topflags = 0;
	r_fd = -1;
	l_abund_P = 5;
	l_abund_Q = 0;
	l_abund_V = 1;
	r_abund_P = 5;
	r_abund_Q = 0;
	r_abund_V = 1;
}

/////////////////////////////////////////////////////////////////////////
// class LD //IMPLEMENTation

//IMPLEMENT_SERIAL(LD,CObject,0)

LD::LD()
{
    id[0] = 0;
    descr[0] = 0;
    type = 0;
    nd[0] = 0;
    vl[0] = 0;
    ind = -1;
    ibs = -1;
    algp[0] = 0;
    ialgp = -1;
    algq[0] = 0;
    ialgq = -1;
    algi[0] = 0;
    ialgi = -1;
    q = 0;
    topflags = 0;
	level = 0;
    w = 0;
    r = 0;
    wm = 0;
    rm = 0;
    r_st = -1;
    r_zn = -1;
    r_fd = -1;

    ver[0] = 0;
    fdid[0]=0;

	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;

	fds=0;
	basevoltage = 0;
}

const LD& LD::operator = (const LD& m_LD)
{
	strcpy(id,m_LD.id);
	strcpy(descr,m_LD.descr);
	type = m_LD.type;
	strcpy(nd,m_LD.nd);
	strcpy(vl,m_LD.vl);
	ind = m_LD.ind;
	ibs = m_LD.ibs;
	strcpy(algp,m_LD.algp);
	ialgp = m_LD.ialgp;
	strcpy(algq,m_LD.algq);
	ialgq = m_LD.ialgq;
	strcpy(algi,m_LD.algi);
	ialgi = m_LD.ialgi;
	q = m_LD.q;
	topflags = m_LD.topflags;
	level = m_LD.level;
	w = m_LD.w;
	r = m_LD.r;
	wm = m_LD.wm;
	rm = m_LD.rm;
	r_st = m_LD.r_st;
	r_zn = m_LD.r_zn;
	r_fd = m_LD.r_fd;
	
	strcpy(ver,m_LD.ver);
	strcpy(fdid,m_LD.fdid);
	
	fds = m_LD.fds;
	prob_fault=m_LD.prob_fault;//故障率
	repairTime=m_LD.repairTime;//5.5小时 故障修复时间;
	basevoltage = m_LD.basevoltage;

	return* this;
}
LD::LD(const char* m_id)
{
    strcpy(id, m_id);
    descr[0] = 0;
    type = 0;
    nd[0] = -1;
    vl[0] = 0;
    ind = -1;
    ibs = 0;
    algp[0] = 0;
    ialgp = -1;
    algq[0] = 0;
    ialgq = -1;
    algi[0] = 0;
    ialgi = -1;
    q = 0;
    topflags = 0;
 	level = 0;
    w = 0.0;
    r = 0.0;
    wm = 0.0;
    rm = 0.0;
    r_st = -1;
    r_zn = -1;
    r_fd = -1;

    ver[0] = 0;
    fdid[0]=0;

	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;
	fds=0;
	basevoltage = 0;
}


/////////////////////////////////////////////////////////////////////////
// class ND //IMPLEMENTation

//IMPLEMENT_SERIAL(ND,CObject,0)

ND::ND()
{
    id[0] = 0;
    vl = 0;
    q = 0;
    ibs = -1;
    primebs = 0;
    fch = -1;
    bch = -1;
    r_cb = -1;
    r_zn = -1;
    r_xf = -1;
    r_sec = -1;
	r_ld = -1;
	r_eq = -1;
	r_cp = -1;

	v = 0;//节点电压的幅值
	adeg = 0;//节点电压的相角
	
	w = 0;
	len = 0;
	ldList[0] = 0;

	UpCBID[0] = 0;//节点名称
	UnionCBID[0] = 0;//节点名称
    fdid[0] = 0;
    ver[0] = 0;
}

const ND& ND::operator = (const ND& m_ND)
{
	strcpy(id,m_ND.id);
	vl = m_ND.vl;
	q = m_ND.q;
	ibs = m_ND.ibs;
	primebs = m_ND.primebs;
	fch = m_ND.fch;
	bch = m_ND.bch;
	r_cb = m_ND.r_cb;
	r_zn = m_ND.r_zn;
	r_xf = m_ND.r_xf;
	r_sec = m_ND.r_sec;
	r_ld = m_ND.r_ld;
	r_eq = m_ND.r_eq;
	r_cp = m_ND.r_cp;
	
	v = m_ND.v;
	adeg = m_ND.adeg;

	w = m_ND.w;
	len = m_ND.len;
	strcpy(ldList, m_ND.ldList);
	strcpy(UpCBID, m_ND.UpCBID);
	strcpy(UnionCBID, m_ND.UnionCBID);
	
	strcpy(fdid,m_ND.fdid);
	strcpy(ver,m_ND.ver);

	return* this;
}

ND::ND(const char* m_id)
{
    strcpy(id, m_id);
    vl = 0;
    q = 0;
    ibs = -1;
    primebs = 0;
    fch = -1;
    bch = -1;
    r_cb = -1;
    r_zn = -1;
    r_xf = -1;
    r_sec = -1;
	r_ld = -1;
 	r_eq = -1;
	r_cp = -1;

	v = 0;//节点电压的幅值
	adeg = 0;//节点电压的相角

	w = 0;
	len = 0;
	ldList[0] = 0;

	UpCBID[0] = 0;//节点名称
	UnionCBID[0] = 0;//节点名称

    fdid[0] = 0;
    ver[0] = 0;
}

/////////////////////////////////////////////////////////////////////////
// class R_CO_ST //IMPLEMENTation

//IMPLEMENT_SERIAL(R_CO_ST,CObject,0)

R_CO_ST::R_CO_ST()
{
    co_id[0] = 0;
    st_id[0] = 0;
    co_id_ptr = -1;
    st_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_CO_ST& R_CO_ST::operator = (const R_CO_ST& m_R_CO_ST)
{
	strcpy(co_id,m_R_CO_ST.co_id);
	strcpy(st_id,m_R_CO_ST.st_id);
	co_id_ptr = m_R_CO_ST.co_id_ptr;
	st_id_ptr = m_R_CO_ST.st_id_ptr;
	fptr = m_R_CO_ST.fptr;
	bptr = m_R_CO_ST.bptr;
	return* this;
}

R_CO_ST::R_CO_ST(const char* m_co_id,const char* m_st_id)
{
    strcpy(co_id, m_co_id);
    strcpy(st_id,m_st_id);
    co_id_ptr = -1;
    st_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

/////////////////////////////////////////////////////////////////////////
// class R_ND_LD //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ND_LD,CObject,0)

R_ND_LD::R_ND_LD()
{
	id = -1;
    nd_id[0] = 0;
    ld_id[0] = 0;
    nd_id_ptr = -1;
    ld_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}

const R_ND_LD& R_ND_LD::operator = (const R_ND_LD& m_R_ND_LD)
{
	id = m_R_ND_LD.id;
	strcpy(nd_id,m_R_ND_LD.nd_id);
	strcpy(ld_id,m_R_ND_LD.ld_id);
	nd_id_ptr = m_R_ND_LD.nd_id_ptr;
	ld_id_ptr = m_R_ND_LD.ld_id_ptr;
	fptr = m_R_ND_LD.fptr;
	bptr = m_R_ND_LD.bptr;

	strcpy(fdid,m_R_ND_LD.fdid);
	strcpy(ver,m_R_ND_LD.ver);
	
	return* this;
}

R_ND_LD::R_ND_LD(const char* m_nd_id, const char* m_ld_id)
{
	id = -1;
    strcpy(nd_id,m_nd_id);
    strcpy(ld_id,m_ld_id);
    nd_id_ptr = -1;
    ld_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;
}


/////////////////////////////////////////////////////////////////////////
// class R_ND_CB //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ND_CB,CObject,0)

R_ND_CB::R_ND_CB()
{
	id = -1;

    nd_id[0] = 0;
    cb_id[0] = 0;
    nd_id_ptr = -1;
    cb_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;
}

const R_ND_CB& R_ND_CB::operator = (const R_ND_CB& m_R_ND_CB)
{
	id = m_R_ND_CB.id;
	strcpy(nd_id,m_R_ND_CB.nd_id);
	strcpy(cb_id,m_R_ND_CB.cb_id);
	nd_id_ptr = m_R_ND_CB.nd_id_ptr;
	cb_id_ptr = m_R_ND_CB.cb_id_ptr;
	fptr = m_R_ND_CB.fptr;
	bptr = m_R_ND_CB.bptr;

	strcpy(fdid,m_R_ND_CB.fdid);
	strcpy(ver,m_R_ND_CB.ver);


	return* this;
}
R_ND_CB::R_ND_CB(const char* m_nd_id, const char* m_cb_id)
{
	id = -1;
    strcpy(nd_id,m_nd_id);
    strcpy(cb_id,m_cb_id);
    nd_id_ptr = -1;
    cb_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}



/////////////////////////////////////////////////////////////////////////
// class R_ND_SEC //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ND_SEC,CObject,0)

R_ND_SEC::R_ND_SEC()
{
	id = -1;
    nd_id[0] = 0;
    sec_id[0] = 0;
    nd_id_ptr = -1;
    sec_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}

const R_ND_SEC& R_ND_SEC::operator = (const R_ND_SEC& m_R_ND_SEC)
{
	id = m_R_ND_SEC.id;
	strcpy(nd_id,m_R_ND_SEC.nd_id);
	strcpy(sec_id,m_R_ND_SEC.sec_id);
	nd_id_ptr = m_R_ND_SEC.nd_id_ptr;
	sec_id_ptr = m_R_ND_SEC.sec_id_ptr;
	fptr = m_R_ND_SEC.fptr;
	bptr = m_R_ND_SEC.bptr;

	strcpy(fdid,m_R_ND_SEC.fdid);
	strcpy(ver,m_R_ND_SEC.ver);
	
	return* this;
}
R_ND_SEC::R_ND_SEC(const char* m_nd_id,const char* m_sec_id)
{
	id = -1;
    strcpy(nd_id,m_nd_id);
    strcpy(sec_id,m_sec_id);
    nd_id_ptr = -1;
    sec_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}

/////////////////////////////////////////////////////////////////////////
R_ND_CP::R_ND_CP()
{
	id = -1;
	nd_id[0] = 0;
	cp_id[0] = 0;
	nd_id_ptr = -1;
	cp_id_ptr = -1;
	fptr = -1;
	bptr = -1;

	fdid[0] = 0;
	ver[0] = 0;

}

const R_ND_CP& R_ND_CP::operator = (const R_ND_CP& m_R_ND_cp)
{
	id = m_R_ND_cp.id;
	strcpy(nd_id, m_R_ND_cp.nd_id);
	strcpy(cp_id, m_R_ND_cp.cp_id);
	nd_id_ptr = m_R_ND_cp.nd_id_ptr;
	cp_id_ptr = m_R_ND_cp.cp_id_ptr;
	fptr = m_R_ND_cp.fptr;
	bptr = m_R_ND_cp.bptr;

	strcpy(fdid, m_R_ND_cp.fdid);
	strcpy(ver, m_R_ND_cp.ver);

	return*this;
}
R_ND_CP::R_ND_CP(const char * m_nd_id, const char * m_cp_id)
{
	id = -1;
	strcpy(nd_id, m_nd_id);
	strcpy(cp_id, m_cp_id);
	nd_id_ptr = -1;
	cp_id_ptr = -1;
	fptr = -1;
	bptr = -1;

	fdid[0] = 0;
	ver[0] = 0;

}
// class R_BS_SEC //IMPLEMENTation

//IMPLEMENT_SERIAL(R_bs_SEC,CObject,0)

R_BS_SEC::R_BS_SEC()
{
	id = -1;
    bs_id[0] = 0;
    sec_id[0] = 0;
    bs_id_ptr = -1;
    sec_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_BS_SEC& R_BS_SEC::operator = (const R_BS_SEC& m_R_bs_SEC)
{
	id = m_R_bs_SEC.id;
	strcpy(bs_id,m_R_bs_SEC.bs_id);
	strcpy(sec_id,m_R_bs_SEC.sec_id);
	bs_id_ptr = m_R_bs_SEC.bs_id_ptr;
	sec_id_ptr = m_R_bs_SEC.sec_id_ptr;
	fptr = m_R_bs_SEC.fptr;
	bptr = m_R_bs_SEC.bptr;

	return* this;
}

R_BS_SEC::R_BS_SEC(const char* m_bs_id,const char* m_sec_id)
{
	id = -1;
    strcpy(bs_id,m_bs_id);
    strcpy(sec_id,m_sec_id);
    bs_id_ptr = -1;
    sec_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}


/////////////////////////////////////////////////////////////////////////
// class R_ND_XF //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ND_XF,CObject,0)

R_ND_XF::R_ND_XF()
{
	id = -1;
    nd_id[0] = 0;
    xf_id[0] = 0;
    nd_id_ptr = -1;
    xf_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}

const R_ND_XF& R_ND_XF::operator = (const R_ND_XF& m_R_ND_XF)
{
	id = m_R_ND_XF.id;
	strcpy(nd_id,m_R_ND_XF.nd_id);
	strcpy(xf_id,m_R_ND_XF.xf_id);
	nd_id_ptr = m_R_ND_XF.nd_id_ptr;
	xf_id_ptr = m_R_ND_XF.xf_id_ptr;
	fptr = m_R_ND_XF.fptr;
	bptr = m_R_ND_XF.bptr;

	strcpy(fdid,m_R_ND_XF.fdid);
	strcpy(ver,m_R_ND_XF.ver);

	return* this;
}
R_ND_XF::R_ND_XF(const char* m_nd_id,const char* m_xf_id)
{
	id = -1;
    strcpy(nd_id,m_nd_id);
    strcpy(xf_id,m_xf_id);
    nd_id_ptr = -1;
    xf_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}

/////////////////////////////////////////////////////////////////////////
// class R_ND_ZN //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ND_ZN,CObject,0)
/*

R_ND_ZN::R_ND_ZN()
{
    nd_id[0] = 0;
    zn_id[0] = 0;
    nd_id_ptr = -1;
    zn_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ND_ZN& R_ND_ZN::operator = (const R_ND_ZN& m_R_ND_ZN)
{
	strcpy(nd_id,m_R_ND_ZN.nd_id);
	strcpy(zn_id,m_R_ND_ZN.zn_id);
	nd_id_ptr = m_R_ND_ZN.nd_id_ptr;
	zn_id_ptr = m_R_ND_ZN.zn_id_ptr;
	fptr = m_R_ND_ZN.fptr;
	bptr = m_R_ND_ZN.bptr;
	return* this;
}
R_ND_ZN::R_ND_ZN(const char* m_nd_id,const char* m_zn_id)
{
    strcpy(nd_id,m_nd_id);
    strcpy(zn_id,m_zn_id);
    nd_id_ptr = -1;
    zn_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

void R_ND_ZN::Serialize( CArchive& ar )
{
	if( ar.IsStoring() )
	{
		ar.Write(nd_id,20);
		ar.Write(zn_id,20);
		ar << nd_id_ptr;
		ar << zn_id_ptr;
		ar << fptr;
		ar << bptr;
	}
	else
	{
		ar.Read(nd_id,20);
		ar.Read(zn_id,20);
		ar >> nd_id_ptr;
		ar >> zn_id_ptr;
		ar >> fptr;
		ar >> bptr;
	}
}
*/
/////////////////////////////////////////////////////////////////////////
// class R_ST_BUS //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ST_BUS,CObject,0)

R_ST_BUS::R_ST_BUS()
{
    st_id[0] = 0;
    bus_id[0] = 0;
    st_id_ptr = -1;
    bus_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ST_BUS& R_ST_BUS::operator = (const R_ST_BUS& m_R_ST_BUS)
{
	strcpy(st_id,m_R_ST_BUS.st_id);
	strcpy(bus_id,m_R_ST_BUS.bus_id);
	st_id_ptr = m_R_ST_BUS.st_id_ptr;
	bus_id_ptr = m_R_ST_BUS.bus_id_ptr;
	fptr = m_R_ST_BUS.fptr;
	bptr = m_R_ST_BUS.bptr;
	return* this;
}
R_ST_BUS::R_ST_BUS(const char* m_st_id,const char* m_bus_id)
{
    strcpy(st_id,m_st_id);
    strcpy(bus_id,m_bus_id);
    st_id_ptr = -1;
    bus_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

/////////////////////////////////////////////////////////////////////////
// class R_ST_CB //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ST_CB,CObject,0)

R_ST_CB::R_ST_CB()
{
    st_id[0] = 0;
    cb_id[0] = 0;
    st_id_ptr = -1;
    cb_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ST_CB& R_ST_CB::operator = (const R_ST_CB& m_R_ST_CB)
{
	strcpy(st_id,m_R_ST_CB.st_id);
	strcpy(cb_id,m_R_ST_CB.cb_id);
	st_id_ptr = m_R_ST_CB.st_id_ptr;
	cb_id_ptr = m_R_ST_CB.cb_id_ptr;
	fptr = m_R_ST_CB.fptr;
	bptr = m_R_ST_CB.bptr;
	return* this;
}
R_ST_CB::R_ST_CB(const char* m_st_id,const char* m_cb_id)
{
    strcpy(st_id,m_st_id);
    strcpy(cb_id,m_cb_id);
    st_id_ptr = -1;
    cb_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

/////////////////////////////////////////////////////////////////////////
// class R_ST_CP //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ST_CP,CObject,0)

R_ST_CP::R_ST_CP()
{
    st_id[0] = 0;
    cp_id[0] = 0;
    st_id_ptr = -1;
    cp_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ST_CP& R_ST_CP::operator = (const R_ST_CP& m_R_ST_CP)
{
	strcpy(st_id,m_R_ST_CP.st_id);
	strcpy(cp_id,m_R_ST_CP.cp_id);
	st_id_ptr = m_R_ST_CP.st_id_ptr;
	cp_id_ptr = m_R_ST_CP.cp_id_ptr;
	fptr = m_R_ST_CP.fptr;
	bptr = m_R_ST_CP.bptr;
	return* this;
}
R_ST_CP::R_ST_CP(const char* m_st_id,const char* m_cp_id)
{
    strcpy(st_id,m_st_id);
    strcpy(cp_id,m_cp_id);
    st_id_ptr = -1;
    cp_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}
/////////////////////////////////////////////////////////////////////////
// class R_ST_LD //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ST_LD,CObject,0)

R_ST_LD::R_ST_LD()
{
    st_id[0] = 0;
    ld_id[0] = 0;
    st_id_ptr = -1;
    ld_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ST_LD& R_ST_LD::operator = (const R_ST_LD& m_R_ST_LD)
{
	strcpy(st_id,m_R_ST_LD.st_id);
	strcpy(ld_id,m_R_ST_LD.ld_id);
	st_id_ptr = m_R_ST_LD.st_id_ptr;
	ld_id_ptr = m_R_ST_LD.ld_id_ptr;
	fptr = m_R_ST_LD.fptr;
	bptr = m_R_ST_LD.bptr;
	return* this;
}
R_ST_LD::R_ST_LD(const char* m_st_id,const char* m_ld_id)
{
    strcpy(st_id,m_st_id);
    strcpy(ld_id,m_ld_id);
    st_id_ptr = -1;
    ld_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

/////////////////////////////////////////////////////////////////////////
// class R_ST_SO //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ST_SO,CObject,0)

R_ST_SO::R_ST_SO()
{
    st_id[0] = 0;
    so_id[0] = 0;
    st_id_ptr = -1;
    so_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ST_SO& R_ST_SO::operator = (const R_ST_SO& m_R_ST_SO)
{
	strcpy(st_id,m_R_ST_SO.st_id);
	strcpy(so_id,m_R_ST_SO.so_id);
	st_id_ptr = m_R_ST_SO.st_id_ptr;
	so_id_ptr = m_R_ST_SO.so_id_ptr;
	fptr = m_R_ST_SO.fptr;
	bptr = m_R_ST_SO.bptr;
	return* this;
}
R_ST_SO::R_ST_SO(const char* m_st_id,const char* m_so_id)
{
    strcpy(st_id,m_st_id);
    strcpy(so_id,m_so_id);
    st_id_ptr = -1;
    so_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

/////////////////////////////////////////////////////////////////////////
// class R_ST_XFMR //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ST_XFMR,CObject,0)

R_ST_XFMR::R_ST_XFMR()
{
    st_id[0] = 0;
    xfmr_id[0] = 0;
    st_id_ptr = -1;
    xfmr_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ST_XFMR& R_ST_XFMR::operator = (const R_ST_XFMR& m_R_ST_XFMR)
{
	strcpy(st_id,m_R_ST_XFMR.st_id);
	strcpy(xfmr_id,m_R_ST_XFMR.xfmr_id);
	st_id_ptr = m_R_ST_XFMR.st_id_ptr;
	xfmr_id_ptr = m_R_ST_XFMR.xfmr_id_ptr;
	fptr = m_R_ST_XFMR.fptr;
	bptr = m_R_ST_XFMR.bptr;
	return* this;
}
R_ST_XFMR::R_ST_XFMR(const char* m_st_id,const char* m_xfmr_id)
{
    strcpy(st_id,m_st_id);
    strcpy(xfmr_id,m_xfmr_id);
    st_id_ptr = -1;
    xfmr_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

//void R_ST_XFMR::Serialize( CArchive& ar )
//{
//	if( ar.IsStoring() )
//	{
//		ar.Write(st_id,20);
//		ar.Write(xfmr_id,20);
//		ar << st_id_ptr;
//		ar << xfmr_id_ptr;
//		ar << fptr;
//		ar << bptr;
//	}
//	else
//	{
//		ar.Read(st_id,20);
//		ar.Read(xfmr_id,20);
//		ar >> st_id_ptr;
//		ar >> xfmr_id_ptr;
//		ar >> fptr;
//		ar >> bptr;
//	}
//}
/////////////////////////////////////////////////////////////////////////
// class R_ST_ZN //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ST_ZN,CObject,0)
/*
R_ST_ZN::R_ST_ZN()
{
    st_id[0] = 0;
    zn_id[0] = 0;
    st_id_ptr = -1;
    zn_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ST_ZN& R_ST_ZN::operator = (const R_ST_ZN& m_R_ST_ZN)
{
	strcpy(st_id,m_R_ST_ZN.st_id);
	strcpy(zn_id,m_R_ST_ZN.zn_id);
	st_id_ptr = m_R_ST_ZN.st_id_ptr;
	zn_id_ptr = m_R_ST_ZN.zn_id_ptr;
	fptr = m_R_ST_ZN.fptr;
	bptr = m_R_ST_ZN.bptr;
	return* this;
}
R_ST_ZN::R_ST_ZN(const char* m_st_id,const char* m_zn_id)
{
    strcpy(st_id,m_st_id);
    strcpy(zn_id,m_zn_id);
    st_id_ptr = -1;
    zn_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

void R_ST_ZN::Serialize( CArchive& ar )
{
	if( ar.IsStoring() )
	{
		ar.Write(st_id,20);
		ar.Write(zn_id,20);
		ar << st_id_ptr;
		ar << zn_id_ptr;
		ar << fptr;
		ar << bptr;
	}
	else
	{
		ar.Read(st_id,20);
		ar.Read(zn_id,20);
		ar >> st_id_ptr;
		ar >> zn_id_ptr;
		ar >> fptr;
		ar >> bptr;
	}
}
*/
/////////////////////////////////////////////////////////////////////////
// class R_XFMR_XF //IMPLEMENTation

//IMPLEMENT_SERIAL(R_XFMR_XF,CObject,0)

R_XFMR_XF::R_XFMR_XF()
{
	id = -1;
    xfmr_id[0] = 0;
    xf_id[0] = 0;
    xfmr_id_ptr = -1;
    xf_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_XFMR_XF& R_XFMR_XF::operator = (const R_XFMR_XF& m_R_XFMR_XF)
{
	id = m_R_XFMR_XF.id;
	strcpy(xfmr_id,m_R_XFMR_XF.xfmr_id);
	strcpy(xf_id,m_R_XFMR_XF.xf_id);
	xfmr_id_ptr = m_R_XFMR_XF.xfmr_id_ptr;
	xf_id_ptr = m_R_XFMR_XF.xf_id_ptr;
	fptr = m_R_XFMR_XF.fptr;
	bptr = m_R_XFMR_XF.bptr;
	return* this;
}

R_XFMR_XF::R_XFMR_XF(const char* m_xfmr_id,const char* m_xf_id)
{
	id = -1;
    strcpy(xfmr_id,m_xfmr_id);
    strcpy(xf_id,m_xf_id);
    xfmr_id_ptr = -1;
    xf_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

/////////////////////////////////////////////////////////////////////////
// class R_ZN_LD //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ZN_LD,CObject,0)

R_ZN_LD::R_ZN_LD()
{
	id = -1;
    zn_id[0] = 0;
    ld_id[0] = 0;
    zn_id_ptr = -1;
    ld_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0] = 0;
	ver[0] = 0;
}

const R_ZN_LD& R_ZN_LD::operator = (const R_ZN_LD& m_R_ZN_LD)
{
	id = m_R_ZN_LD.id;
	strcpy(zn_id,m_R_ZN_LD.zn_id);
	strcpy(ld_id,m_R_ZN_LD.ld_id);
	zn_id_ptr = m_R_ZN_LD.zn_id_ptr;
	ld_id_ptr = m_R_ZN_LD.ld_id_ptr;
	fptr = m_R_ZN_LD.fptr;
	bptr = m_R_ZN_LD.bptr;

	strcpy(fdid,m_R_ZN_LD.fdid);
	strcpy(ver,m_R_ZN_LD.ver);
	
	return* this;
}
R_ZN_LD::R_ZN_LD(const char* m_zn_id,const char* m_ld_id)
{
	id = -1;
    strcpy(zn_id,m_zn_id);
    strcpy(ld_id,m_ld_id);
    zn_id_ptr = -1;
    ld_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0] = 0;
	ver[0] = 0;
}

/////////////////////////////////////////////////////////////////////////
// class R_ZN_EQ //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ZN_EQ,CObject,0)

R_ZN_EQ::R_ZN_EQ()
{
	id = -1;
    zn_id[0] = 0;
    eq_id[0] = 0;
    zn_id_ptr = -1;
    eq_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

const R_ZN_EQ& R_ZN_EQ::operator = (const R_ZN_EQ& m_R_ZN_EQ)
{
	id = m_R_ZN_EQ.id;
	strcpy(zn_id,m_R_ZN_EQ.zn_id);
	strcpy(eq_id,m_R_ZN_EQ.eq_id);
	zn_id_ptr = m_R_ZN_EQ.zn_id_ptr;
	eq_id_ptr = m_R_ZN_EQ.eq_id_ptr;
	fptr = m_R_ZN_EQ.fptr;
	bptr = m_R_ZN_EQ.bptr;
	
	return* this;
}
R_ZN_EQ::R_ZN_EQ(const char* m_zn_id,const char* m_eq_id)
{
	id = -1;
    strcpy(zn_id,m_zn_id);
    strcpy(eq_id,m_eq_id);
    zn_id_ptr = -1;
    eq_id_ptr = -1;
    fptr = -1;
    bptr = -1;
}

/////////////////////////////////////////////////////////////////////////
// class R_ZN_SEC //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ZN_SEC,CObject,0)

R_ZN_SEC::R_ZN_SEC()
{
	id = -1;
    zn_id[0] = 0;
    sec_id[0] = 0;
    zn_id_ptr = -1;
    sec_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}

const R_ZN_SEC& R_ZN_SEC::operator = (const R_ZN_SEC& m_R_ZN_SEC)
{
	id = m_R_ZN_SEC.id;
	strcpy(zn_id,m_R_ZN_SEC.zn_id);
	strcpy(sec_id,m_R_ZN_SEC.sec_id);
	zn_id_ptr = m_R_ZN_SEC.zn_id_ptr;
	sec_id_ptr = m_R_ZN_SEC.sec_id_ptr;
	fptr = m_R_ZN_SEC.fptr;
	bptr = m_R_ZN_SEC.bptr;

	strcpy(fdid,m_R_ZN_SEC.fdid);
	strcpy(ver,m_R_ZN_SEC.ver);
	
	return* this;
}

R_ZN_SEC::R_ZN_SEC(const char* m_zn_id,const char* m_sec_id)
{
	id = -1;
    strcpy(zn_id,m_zn_id);
    strcpy(sec_id,m_sec_id);
    zn_id_ptr = -1;
    sec_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}

/////////////////////////////////////////////////////////////////////////
// class R_ZN_cp //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ZN_cp,CObject,0)

R_ZN_CP::R_ZN_CP()
{
	id = -1;
	zn_id[0] = 0;
	cp_id[0] = 0;
	zn_id_ptr = -1;
	cp_id_ptr = -1;
	fptr = -1;
	bptr = -1;

	fdid[0] = 0;
	ver[0] = 0;

}

const R_ZN_CP& R_ZN_CP::operator = (const R_ZN_CP& m_R_ZN_cp)
{
	id = m_R_ZN_cp.id;
	strcpy(zn_id, m_R_ZN_cp.zn_id);
	strcpy(cp_id, m_R_ZN_cp.cp_id);
	zn_id_ptr = m_R_ZN_cp.zn_id_ptr;
	cp_id_ptr = m_R_ZN_cp.cp_id_ptr;
	fptr = m_R_ZN_cp.fptr;
	bptr = m_R_ZN_cp.bptr;

	strcpy(fdid, m_R_ZN_cp.fdid);
	strcpy(ver, m_R_ZN_cp.ver);

	return*this;
}

R_ZN_CP::R_ZN_CP(const char * m_zn_id, const char * m_cp_id)
{
	id = -1;
	strcpy(zn_id, m_zn_id);
	strcpy(cp_id, m_cp_id);
	zn_id_ptr = -1;
	cp_id_ptr = -1;
	fptr = -1;
	bptr = -1;

	fdid[0] = 0;
	ver[0] = 0;

}

/////////////////////////////////////////////////////////////////////////
// class R_ZN_XF //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ZN_xf,CObject,0)

R_ZN_XF::R_ZN_XF()
{
	id = -1;
	zn_id[0] = 0;
	xf_id[0] = 0;
	zn_id_ptr = -1;
	xf_id_ptr = -1;
	fptr = -1;
	bptr = -1;

	fdid[0] = 0;
	ver[0] = 0;

}

const R_ZN_XF& R_ZN_XF::operator = (const R_ZN_XF& m_R_ZN_xf)
{
	id = m_R_ZN_xf.id;
	strcpy(zn_id, m_R_ZN_xf.zn_id);
	strcpy(xf_id, m_R_ZN_xf.xf_id);
	zn_id_ptr = m_R_ZN_xf.zn_id_ptr;
	xf_id_ptr = m_R_ZN_xf.xf_id_ptr;
	fptr = m_R_ZN_xf.fptr;
	bptr = m_R_ZN_xf.bptr;

	strcpy(fdid, m_R_ZN_xf.fdid);
	strcpy(ver, m_R_ZN_xf.ver);

	return*this;
}

R_ZN_XF::R_ZN_XF(const char * m_zn_id, const char * m_xf_id)
{
	id = -1;
	strcpy(zn_id, m_zn_id);
	strcpy(xf_id, m_xf_id);
	zn_id_ptr = -1;
	xf_id_ptr = -1;
	fptr = -1;
	bptr = -1;

	fdid[0] = 0;
	ver[0] = 0;

}

/////////////////////////////////////////////////////////////////////////
// class R_ZN_CB //IMPLEMENTation

//IMPLEMENT_SERIAL(R_ZN_CB,CObject,0)

R_ZN_CB::R_ZN_CB()
{
	id = -1;
    zn_id[0] = 0;
    cb_id[0] = 0;
    zn_id_ptr = -1;
    cb_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;

}

const R_ZN_CB& R_ZN_CB::operator = (const R_ZN_CB& m_R_ZN_CB)
{
	id = m_R_ZN_CB.id;
	strcpy(zn_id,m_R_ZN_CB.zn_id);
	strcpy(cb_id,m_R_ZN_CB.cb_id);
	zn_id_ptr = m_R_ZN_CB.zn_id_ptr;
	cb_id_ptr = m_R_ZN_CB.cb_id_ptr;
	fptr = m_R_ZN_CB.fptr;
	bptr = m_R_ZN_CB.bptr;

	strcpy(fdid,m_R_ZN_CB.fdid);
	strcpy(ver,m_R_ZN_CB.ver);
	
	return* this;
}

R_ZN_CB::R_ZN_CB(const char* m_zn_id,const char* m_cb_id)
{
	id = -1;
    strcpy(zn_id,m_zn_id);
    strcpy(cb_id,m_cb_id);
    zn_id_ptr = -1;
    cb_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	fdid[0]=0;
	ver[0]=0;
}

/////////////////////////////////////////////////////////////////////////
// class R_FD_UN //IMPLEMENTation

//IMPLEMENT_SERIAL(R_FD_UN,CObject,0)

R_FD_UN::R_FD_UN()
{
	id = -1;
    fd_id[0] = 0;
    un_id[0] = 0;
    fd_id_ptr = -1;
    un_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;
}

const R_FD_UN& R_FD_UN::operator = (const R_FD_UN& m_R_FD_UN)
{
	id = m_R_FD_UN.id;
	strcpy(fd_id,m_R_FD_UN.fd_id);
	strcpy(un_id,m_R_FD_UN.un_id);
	fd_id_ptr = m_R_FD_UN.fd_id_ptr;
	un_id_ptr = m_R_FD_UN.un_id_ptr;
	fptr = m_R_FD_UN.fptr;
	bptr = m_R_FD_UN.bptr;

	strcpy(ver,m_R_FD_UN.ver);
	
	return* this;
}

R_FD_UN::R_FD_UN(const char* m_fd_id,const char* m_un_id)
{
	id = -1;
    strcpy(fd_id,m_fd_id);
    strcpy(un_id,m_un_id);
    fd_id_ptr = -1;
    un_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;

}


/////////////////////////////////////////////////////////////////////////
// class R_FD_UN //IMPLEMENTation

//IMPLEMENT_SERIAL(R_FD_UN,CObject,0)

R_FD_LD::R_FD_LD()
{
	id = -1;
    fd_id[0] = 0;
    ld_id[0] = 0;
    fd_id_ptr = -1;
    ld_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;
}

const R_FD_LD& R_FD_LD::operator = (const R_FD_LD& m_R_FD_LD)
{
	id = m_R_FD_LD.id;
	strcpy(fd_id,m_R_FD_LD.fd_id);
	strcpy(ld_id,m_R_FD_LD.ld_id);
	fd_id_ptr = m_R_FD_LD.fd_id_ptr;
	ld_id_ptr = m_R_FD_LD.ld_id_ptr;
	fptr = m_R_FD_LD.fptr;
	bptr = m_R_FD_LD.bptr;

	strcpy(ver,m_R_FD_LD.ver);
	
	return* this;
}

R_FD_LD::R_FD_LD(const char* m_fd_id,const char* m_ld_id)
{
	id = -1;
    strcpy(fd_id,m_fd_id);
    strcpy(ld_id,m_ld_id);
    fd_id_ptr = -1;
    ld_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;

}

R_FD_ND::R_FD_ND()
{
	id = -1;
    fd_id[0] = 0;
    nd_id[0] = 0;
    fd_id_ptr = -1;
    nd_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;
}

const R_FD_ND& R_FD_ND::operator = (const R_FD_ND& m_R_FD_ND)
{
	id = m_R_FD_ND.id;
	strcpy(fd_id,m_R_FD_ND.fd_id);
	strcpy(nd_id,m_R_FD_ND.nd_id);
	fd_id_ptr = m_R_FD_ND.fd_id_ptr;
	nd_id_ptr = m_R_FD_ND.nd_id_ptr;
	fptr = m_R_FD_ND.fptr;
	bptr = m_R_FD_ND.bptr;

	strcpy(ver,m_R_FD_ND.ver);
	
	return* this;
}

R_FD_ND::R_FD_ND(const char* m_fd_id,const char* m_nd_id)
{
	id = -1;
    strcpy(fd_id,m_fd_id);
    strcpy(nd_id,m_nd_id);
    fd_id_ptr = -1;
    nd_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;

}


R_FD_EQ::R_FD_EQ()
{
	id = -1;
    fd_id[0] = 0;
    eq_id = 0;
    fd_id_ptr = -1;
    eq_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;
}

const R_FD_EQ& R_FD_EQ::operator = (const R_FD_EQ& m_R_FD_EQ)
{
	id = m_R_FD_EQ.id;
	strcpy(fd_id,m_R_FD_EQ.fd_id);
	eq_id=m_R_FD_EQ.eq_id;
	fd_id_ptr = m_R_FD_EQ.fd_id_ptr;
	eq_id_ptr = m_R_FD_EQ.eq_id_ptr;
	fptr = m_R_FD_EQ.fptr;
	bptr = m_R_FD_EQ.bptr;

	strcpy(ver,m_R_FD_EQ.ver);
	
	return* this;
}

R_FD_EQ::R_FD_EQ(const char* m_fd_id,const int m_eq_id)
{
	id = -1;
    strcpy(fd_id,m_fd_id);
    eq_id=m_eq_id;
    fd_id_ptr = -1;
    eq_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;

}

R_FD_ZN::R_FD_ZN()
{
	id = -1;
    fd_id[0] = 0;
    zn_id = 0;
    fd_id_ptr = -1;
    zn_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;
}

const R_FD_ZN& R_FD_ZN::operator = (const R_FD_ZN& m_R_FD_ZN)
{
	id = m_R_FD_ZN.id;
	strcpy(fd_id,m_R_FD_ZN.fd_id);
	zn_id = m_R_FD_ZN.zn_id;
	fd_id_ptr = m_R_FD_ZN.fd_id_ptr;
	zn_id_ptr = m_R_FD_ZN.zn_id_ptr;
	fptr = m_R_FD_ZN.fptr;
	bptr = m_R_FD_ZN.bptr;

	strcpy(ver,m_R_FD_ZN.ver);
	
	return* this;
}

R_FD_ZN::R_FD_ZN(const char* m_fd_id,const int m_zn_id)
{
	id = -1;
    strcpy(fd_id,m_fd_id);
    zn_id=m_zn_id;
    fd_id_ptr = -1;
    zn_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;

}
/////////////////////////////////////////////////////////////////////////
// class R_FD_SO //IMPLEMENTation

//IMPLEMENT_SERIAL(R_FD_SO,CObject,0)

R_FD_SO::R_FD_SO()
{
	id = -1;
    fd_id[0] = 0;
    so_id[0] = 0;
    fd_id_ptr = -1;
    so_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;
}

const R_FD_SO& R_FD_SO::operator = (const R_FD_SO& m_R_FDS_FD)
{
	id = m_R_FDS_FD.id;
	strcpy(fd_id,m_R_FDS_FD.fd_id);
	strcpy(so_id,m_R_FDS_FD.so_id);
	fd_id_ptr = m_R_FDS_FD.fd_id_ptr;
	so_id_ptr = m_R_FDS_FD.so_id_ptr;
	fptr = m_R_FDS_FD.fptr;
	bptr = m_R_FDS_FD.bptr;

	strcpy(ver,m_R_FDS_FD.ver);
	
	return* this;
}

R_FD_SO::R_FD_SO(const char* m_fd_id,const char* m_so_id)
{
	id = -1;
    strcpy(fd_id,m_fd_id);
    strcpy(so_id,m_so_id);
    fd_id_ptr = -1;
    fd_id_ptr = -1;
    fptr = -1;
    bptr = -1;

	ver[0]=0;

}


/////////////////////////////////////////////////////////////////////////
// class SEC //IMPLEMENTation

//IMPLEMENT_SERIAL(SEC,CObject,0)

SEC::SEC()
{
    id[0] = 0;
    descr[0] = 0;
    nd[0] = 0;
    znd[0] = 0;
    ind = -1;
    iznd = -1;
    ibs = -1;
    zbs = -1;
    r1 = 0.0;
    x1 = 0.0;
    b1 = 0.0;
	r0 = 0.0;
	x0 = 0.0;
	b0 = 0.0;
	q = 0;
    topflags = 0;
	algip[0] = 0;
	ialgip = -1;
	algiq[0] = 0;
	ialgiq = -1;
	algii[0] = 0;
	ialgii = -1;
	algzp[0] = 0;
	ialgzp = -1;
	algzq[0] = 0;
	ialgzq = -1;
	algzi[0] = 0;
	ialgzi = -1;
    w = 0;
    r = 0;
    i = 0;
    wz = 0;
    rz = 0;
    iz = 0;
    r_zn = -1;

	lossP=0;
	transP=0;
	lossC=0;
	transC=0;

	lP_zn=0;
	lC_zn=0;
	lP_znnext=0;
	lC_znnext=0;
	
	vl[0]=0;

    ver[0] = 0;
    fdid[0]=0;

	frPerLen = 0;
//	switchTime = 0;
//	transTime = 0;
//	repairTime = 0;

	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;

	tranfdlist[0] = 0;
//	transSec[0]=0;
//	lossSec[0]=0;
	closeunlist[0]=0;//馈线段发生故障后的 闭合 联络开关列表
	opencblist[0] = 0;//馈线段发生故障后的 打开 分段开关列表

	faultratio = 0;

	fds=0;

	v_b=0;//首节点电压
	adeg_b=0;//首节点相角

	v_e=0;//末节点电压
	adeg_e=0;//末节点相角

    st_b[0] = 0;
    st_e[0] = 0;
    sep_b[0] = 0;
    sep_e[0] = 0;

	len=0;
	basevoltage = 0;
}

const SEC& SEC::operator = (const SEC& m_SEC)
{
	strcpy(id,m_SEC.id);
	strcpy(descr,m_SEC.descr);
	strcpy(nd,m_SEC.nd);
	strcpy(znd,m_SEC.znd);
	ind = m_SEC.ind;
	iznd = m_SEC.iznd;
	ibs = m_SEC.ibs;
	zbs = m_SEC.zbs;
	r1 = m_SEC.r1;
	x1 = m_SEC.x1;
	b1 = m_SEC.b1;
	r0 = m_SEC.r0;
	x0 = m_SEC.x0;
	b0 = m_SEC.b0;
	q = m_SEC.q;
	topflags = m_SEC.topflags;
	strcpy(algip, m_SEC.algip);
	ialgip = m_SEC.ialgip;
	strcpy(algiq, m_SEC.algiq);
	ialgiq = m_SEC.ialgiq;
	strcpy(algii, m_SEC.algii);
	ialgii = m_SEC.ialgii;
	strcpy(algzp, m_SEC.algzp);
	ialgzp = m_SEC.ialgzp;
	strcpy(algzq, m_SEC.algzq);
	ialgzq = m_SEC.ialgzq;
	strcpy(algzi, m_SEC.algzi);
	ialgzi = m_SEC.ialgzi;
	w = m_SEC.w;
	r = m_SEC.r;
	i = m_SEC.i;
	wz = m_SEC.wz;
	rz = m_SEC.rz;
	iz = m_SEC.iz;
	r_zn = m_SEC.r_zn;

	lossP=m_SEC.lossP;
	transP=m_SEC.transP;
	lossC=m_SEC.lossC;
	transC=m_SEC.transC;

	lC_zn=m_SEC.lC_zn;
	lP_zn=m_SEC.lP_zn;
	lC_znnext=m_SEC.lC_znnext;
	lP_znnext=m_SEC.lP_znnext;

	strcpy(vl,m_SEC.vl);
	
	strcpy(ver,m_SEC.ver);
	strcpy(fdid,m_SEC.fdid);

	frPerLen=m_SEC.frPerLen;
	prob_fault=m_SEC.prob_fault;
	repairTime=m_SEC.repairTime;
	
	strcpy(tranfdlist,m_SEC.tranfdlist);
	strcpy(closeunlist, m_SEC.closeunlist);
	strcpy(opencblist, m_SEC.opencblist);

	faultratio = m_SEC.faultratio;

	fds = m_SEC.fds;

	strcpy(st_b,m_SEC.st_b);
	strcpy(st_e,m_SEC.st_e);
	
	strcpy(sep_b,m_SEC.sep_b);
	strcpy(sep_e,m_SEC.sep_e);

	len = m_SEC.len;
	basevoltage = m_SEC.basevoltage;

	return* this;
}

SEC::SEC(const char  * m_id)
{
    strcpy(id,m_id);
    descr[0] = 0;
    nd[0] = 0;
    znd[0] = 0;
    ind = -1;
    iznd = -1;
    ibs = -1;
    zbs = -1;
	r1 = 0.0;
	x1 = 0.0;
	b1 = 0.0;
	r0 = 0.0;
	x0 = 0.0;
	b0 = 0.0;
    q = 0;
    topflags = 0;
	algip[0] = 0;
	ialgip = -1;
	algiq[0] = 0;
	ialgiq = -1;
	algii[0] = 0;
	ialgii = -1;
	algzp[0] = 0;
	ialgzp = -1;
	algzq[0] = 0;
	ialgzq = -1;
	algzi[0] = 0;
	ialgzi = -1;
    w = 0.0;
    r = 0.0;
    i = 0.0;
    wz = 0.0;
    rz = 0.0;
    iz = 0.0;
    r_zn = -1;

	lossP=0;
	transP=0;
	lossC=0;
	transC=0;

	lP_zn=0;
	lC_zn=0;
	lP_znnext=0;
	lC_znnext=0;
	
	vl[0] =0;

    ver[0] = 0;
    fdid[0]=0;

	frPerLen = 0;
	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;

	tranfdlist[0] = 0;
//	transSec[0]=0;
//	lossSec[0]=0;
	closeunlist[0] = 0;//馈线段发生故障后的 闭合 联络开关列表
	opencblist[0] = 0;//馈线段发生故障后的 打开 分段开关列表

	faultratio = 0;

	fds=0;

	v_b=0;//首节点电压
	adeg_b=0;//首节点相角

	v_e=0;//末节点电压
	adeg_e=0;//末节点相角

    st_b[0] = 0;
    st_e[0] = 0;
    sep_b[0] = 0;
    sep_e[0] = 0;
	len=0;
	basevoltage = 0;
}

//SECP
SECP::SECP()
{
    id[0] = 0;
    descr[0] = 0;
    nd[0] = 0;
    fdid[0]=0;
    st[0]=0;

	w=0;
	w_flag=0;
	r=0;
	r_flag=0;
	topflags=0;
	baseVL[0]=0;//所属线段
}

SECP::SECP(const char* m_id)
{
    strcpy(id,m_id);
    descr[0] = 0;
    nd[0] = 0;
    fdid[0]=0;
    st[0]=0;

	w=0;
	w_flag=0;
	r=0;
	r_flag=0;
	topflags=0;
	baseVL[0] = 0;//所属线段
}

const SECP& SECP::operator = (const SECP& m_secp)
{
	strcpy(id,m_secp.id);
	strcpy(descr,m_secp.descr);
	strcpy(nd,m_secp.nd);
	strcpy(fdid,m_secp.fdid);
	strcpy(st,m_secp.st);
	strcpy(baseVL, m_secp.baseVL);

	w = m_secp.w;
	r = m_secp.r;
	w_flag = m_secp.w_flag;
	r_flag = m_secp.r_flag;
	topflags = m_secp.topflags;

	return* this;
}


/////////////////////////////////////////////////////////////////////////
// class SO //IMPLEMENTation

//IMPLEMENT_SERIAL(SO,CObject,0)

SO::SO()
{
    id[0] = 0;
    descr[0] = 0;
    nd[0] = 0;
    vl[0] = 0;
    ind = -1;
    ibs = -1;
    iisland = -1;
    algp[0] = 0;
    ialgp = -1;
    algq[0] = 0;
    ialgq = -1;
    algi[0] = 0;
    ialgi = -1;
    q = 0;
    topflags = 0;
	vtarget = 0;
	slackavr = 1;
    w = 0;
    r = 0;
    i = 0;
    v = 0;
    r_st = -1;
    r_fd = -1;
	ss = 0;
	sw = 0;
	sr = 0;

    ver[0] = 0;
    fdid[0]=0;

	I_value = 0;
	cap_I = 0;
	isAnalog_P = 0;
	isAnalog_Q = 0;
	fScaleP = 0.0;
	fScaleQ = 0.0;

	maxcurrent = 0;
	nm1 = 0;

	fds=0;

	pLoss = 0;
	qLoss = 0;

	fault_ltime = 0;//add by lld 2016-1-27
	fault_stime=0;
	fault_ttime=0;
	plan_prob=0;
	plan_rtime=0;

	plan_ltime = 0;//add by lld 2018-5-10
	plan_stime=0;
	plan_ttime=0;

	prob_fault = 0;
	repairTime = 0;
	basevoltage = 0;
}

const SO& SO::operator = (const SO& m_SO)
{
	strcpy(id,m_SO.id);
	strcpy(descr,m_SO.descr);
	strcpy(nd,m_SO.nd);
	strcpy(vl,m_SO.vl);
	ind = m_SO.ind;
	ibs = m_SO.ibs;
	iisland = m_SO.iisland;
	strcpy(algp,m_SO.algp);
	ialgp = m_SO.ialgp;
	strcpy(algq,m_SO.algq);
	ialgq = m_SO.ialgq;
	strcpy(algi,m_SO.algi);
	ialgi = m_SO.ialgi;
	q = m_SO.q;
	topflags = m_SO.topflags;
	slackavr = m_SO.slackavr;
	vtarget = m_SO.vtarget;
	w = m_SO.w;
	r = m_SO.r;
	i = m_SO.i;
	v = m_SO.v;
	r_st = m_SO.r_st;
	r_fd = m_SO.r_fd;
	ss = m_SO.ss;
	sw = m_SO.sw;
	sr = m_SO.sr;

	I_value = m_SO.I_value;
	cap_I = m_SO.cap_I;

	isAnalog_P = m_SO.isAnalog_P;
	isAnalog_Q = m_SO.isAnalog_Q;
	fScaleP = m_SO.fScaleP;
	fScaleQ = m_SO.fScaleQ;

	maxcurrent = m_SO.maxcurrent;
	nm1 = m_SO.nm1;
	
	strcpy(ver,m_SO.ver);
	strcpy(fdid,m_SO.fdid);
	
	fds = m_SO.fds;

	pLoss = m_SO.pLoss;
	qLoss = m_SO.qLoss;

	fault_ltime=m_SO.fault_ltime;
	fault_stime=m_SO.fault_stime;
	fault_ttime=m_SO.fault_ttime;
	plan_ltime=m_SO.plan_ltime;
	plan_prob=m_SO.plan_prob;
	plan_rtime=m_SO.plan_rtime;

	plan_stime=m_SO.plan_stime;
	plan_ttime=m_SO.plan_ttime;

	prob_fault=m_SO.prob_fault;
	repairTime=m_SO.repairTime;

	basevoltage = m_SO.basevoltage;

	return* this;
}
SO::SO(const char* m_id)
{
    id[0] = 0;
    descr[0] = 0;
    nd[0] = 0;
    vl[0] = 0;
    ind = -1;
    ibs = -1;
    iisland = -1;
    algp[0] = 0;
    ialgp = -1;
    algq[0] = 0;
    ialgq = -1;
    algi[0] = 0;
    ialgi = -1;
    q = 0;
    topflags = 0;
    slackavr = 1;//默认是PQ节点
	vtarget = 0;
    w = 0.0;
    r = 0.0;
    i = 0.0;
    v = 0;
    r_st = -1;
    r_fd = -1;
	ss = 0.0;
	sw = 0.0;
	sr = 0.0;

    ver[0] = 0;
    fdid[0]=0;

	I_value = 0;
	cap_I = 0;
	isAnalog_P = 0;
	isAnalog_Q = 0;
	fScaleP = 0.0;
	fScaleQ = 0.0;
	
	maxcurrent = 0;
	nm1 = 0;
	fds=0;
	pLoss = 0;
	qLoss = 0;

	fault_ltime = 0;
	fault_stime=0;
	fault_ttime=0;
	plan_ltime = 0;//add by lld 2018-5-10
	plan_prob=0;
	plan_rtime=0;
	plan_stime=0;
	plan_ttime=0;

	prob_fault = 0;
	repairTime = 0;
	basevoltage = 0;
}


/////////////////////////////////////////////////////////////////////////
// class FD //IMPLEMENTation

//IMPLEMENT_SERIAL(FD,CObject,0)

FD::FD()
{
	i_id = -1;

    id[0] = 0;
	vl[0]=0;
	q=0;
	
    ver[0]=0;

	maxcurrent =0;//最大电流，用这个来计算裕度,单位是A
	nm1=0;//是否可以全线路通过n-1(首段故障)
	cnt_un=0;
	cnt_ld=0;
	cnt_zn=0;
	cnt_eq=0;
	cnt_nd=0;
	cnt_so = 0;

	r_un=-1;
	r_zn=-1;
	r_so = -1;
	r_ld = -1;
	r_eq = -1;
	r_nd = -1;

	plan_prob=0;
	plan_rtime=0;

	fault_ltime = 0;//add by lld 2016-1-27
	fault_stime=0;
	fault_ttime=0;

	plan_ltime = 0;//add by lld 2018-5-10
	plan_stime=0;
	plan_ttime=0;

	I_value = 0;
}

const FD& FD::operator = (const FD& m_FD)
{
	i_id = m_FD.i_id;
	
	strcpy(id,m_FD.id);
	strcpy(vl,m_FD.vl);
	maxcurrent = m_FD.maxcurrent;
	nm1 = m_FD.nm1;
	cnt_un = m_FD.cnt_un;
	cnt_zn = m_FD.cnt_zn;
	cnt_ld = m_FD.cnt_ld;
	cnt_eq = m_FD.cnt_eq;
	cnt_nd = m_FD.cnt_nd;
	cnt_so = m_FD.cnt_so;
	r_un = m_FD.r_un;
	r_zn = m_FD.r_zn;
	r_so = m_FD.r_so;
	r_ld = m_FD.r_ld;
	r_eq = m_FD.r_eq;
	r_nd = m_FD.r_nd;

	fault_ltime=m_FD.fault_ltime;
	fault_stime=m_FD.fault_stime;
	fault_ttime=m_FD.fault_ttime;
	plan_ltime=m_FD.plan_ltime;
	plan_stime=m_FD.plan_stime;
	plan_ttime=m_FD.plan_ttime;

	plan_prob=m_FD.plan_prob;
	plan_rtime=m_FD.plan_rtime;


	I_value=m_FD.I_value;

	strcpy(ver,m_FD.ver);
	return* this;
}

FD::FD(const int m_id)
{
	i_id = m_id;

    id[0]=0;
	vl[0]=0;
	q=0;
	
    ver[0]=0;

	maxcurrent =0;//最大电流，用这个来计算裕度,单位是A
	nm1=0;//是否可以全线路通过n-1(首段故障)
	cnt_un=0;
	cnt_ld=0;
	cnt_zn=0;
	cnt_eq=0;
	cnt_nd=0;
	cnt_so = 0;

	r_un=-1;
	r_zn=-1;
	r_so = -1;
	r_ld = -1;
	r_eq = -1;
	r_nd = -1;

	plan_prob=0;
	plan_rtime=0;

	fault_ltime = 0;//add by lld 2016-1-27
	fault_stime=0;
	fault_ttime=0;

	plan_ltime = 0;//add by lld 2018-5-10
	plan_stime=0;
	plan_ttime=0;

	I_value = 0;

}

void FD::Reset()
{
	i_id = -1;
	
    id[0] = 0;
	vl[0]=0;
	q=0;
	
    ver[0]=0;

	maxcurrent =0;//最大电流，用这个来计算裕度,单位是A

	plan_prob=0;
	plan_rtime=0;

	fault_ltime = 0;//add by lld 2016-1-27
	fault_stime=0;
	fault_ttime=0;

	plan_ltime = 0;//add by lld 2018-5-10
	plan_stime=0;
	plan_ttime=0;

	nm1=0;//是否可以全线路通过n-1(首段故障)
	cnt_un=0;
	cnt_ld=0;
	cnt_zn=0;
	cnt_eq=0;
	cnt_nd=0;

	r_un=-1;
	r_zn=-1;
	r_so = -1;
	r_ld = -1;
	r_eq = -1;
}

/////////////////////////////////////////////////////////////////////////
// class ST //IMPLEMENTation

//IMPLEMENT_SERIAL(ST,CObject,0)


ST::ST()
{
    id[0] = 0;
    descr[0] = 0;
    abb[0] = 0;
    type = 0;
    qcomp = 0;
    wso = 0;
    rso = 0;
    wload = 0;
    rload = 0;
    r_co = -1;
    r_xfmr = -1;
    r_so = -1;
    r_cb = -1;
    r_ld = -1;
    r_cp = -1;
    r_bus = -1;
    r_zn = -1;

    ver[0] = 0;
    fdid[0]=0;

	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;

	fds=0;
}

const ST& ST::operator = (const ST& m_ST)
{
	strcpy(id,m_ST.id);
	strcpy(descr,m_ST.descr);
	strcpy(abb,m_ST.abb);
	type = m_ST.type;
	qcomp = m_ST.qcomp;
	wso = m_ST.wso;
	rso = m_ST.rso;
	wload = m_ST.wload;
	rload = m_ST.rload;
	r_co = m_ST.r_co;
	r_xfmr = m_ST.r_xfmr;
	r_so = m_ST.r_so;
	r_cb = m_ST.r_cb;
	r_ld = m_ST.r_ld;
	r_cp = m_ST.r_cp;
	r_bus = m_ST.r_bus;
	r_zn = m_ST.r_zn;
	
	strcpy(ver,m_ST.ver);
	strcpy(fdid,m_ST.fdid);

	fds = m_ST.fds;

	prob_fault = m_ST.prob_fault;
	repairTime = m_ST.repairTime;
	
	return* this;
}
ST::ST(const char* m_id)
{
    id[0] = 0;
    descr[0] = 0;
    abb[0] = 0;
    type = 0;
    qcomp = 0;
    wso = 0.0;
    rso = 0.0;
    wload = 0.0;
    rload = 0.0;
    r_co = -1;
    r_xfmr = -1;
    r_so = -1;
    r_cb = -1;
    r_ld = -1;
    r_cp = -1;
    r_bus = -1;
    r_zn = -1;

    ver[0] = 0;
    fdid[0]=0;

	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;

	fds=0;
}


/////////////////////////////////////////////////////////////////////////
// class TAPTY //IMPLEMENTation

//IMPLEMENT_SERIAL(TAPTY,CObject,0)

TAPTY::TAPTY()
{
    id[0] = 0;
    mn = 0;
    mx = 0;
    nom = 0;
    step = 0;
}

const TAPTY& TAPTY::operator = (const TAPTY& m_TAPTY)
{
	strcpy(id,m_TAPTY.id);
	mn = m_TAPTY.mn;
	mx = m_TAPTY.mx;
	nom = m_TAPTY.nom;
	step = m_TAPTY.step;
	return* this;
}
TAPTY::TAPTY(const char* m_id)
{
    strcpy(id,m_id);
    mn = 0;
    mx = 0;
    nom = 0;
    step = 0.0;
}

/////////////////////////////////////////////////////////////////////////
// class XF //IMPLEMENTation

//IMPLEMENT_SERIAL(XF,CObject,0)

XF::XF()
{
	id[0] = 0;
	descr[0] = 0;
	itapty = -1;
	iztapty = -1;
	tap = 0;
	ztap = 0;
	kvnom = 0;
	zkvnom = 0;
	nd[0] = 0;
	znd[0] = 0;
	ind = -1;
	iznd = -1;
	ibs = -1;
	zbs = -1;
	mvanom = 0;
	q = 0;
	topflags = 0;
	r1 = 0;
	x1 = 0;
	r0 = 0;
	x0 = 0;
	ialgip = -1;
	ialgiq = -1;
	ialgii = -1;
	ialgit = -1;
	ialgzp = -1;
	ialgzq = -1;
	ialgzi = -1;
	ialgzt = -1;
	w = 0;
	r_ = 0;
	i = 0;
	wz = 0;
	rz = 0;
	iz = 0;
	r_xfmr = -1;
	r_zn = -1;

	fdid[0] = 0;
	ver[0] = 0;

	vl[0] = 0;
	basevoltage = 0;
	tapty[0] = 0;

	 v_b=0;
	 adeg_b=0;
	 v_e=0;
	 adeg_e=0;

}

const XF& XF::operator = (const XF& m_XF)
{
	strcpy(id, m_XF.id);
	strcpy(descr, m_XF.descr);
	itapty = m_XF.itapty;
	iztapty = m_XF.iztapty;
	tap = m_XF.tap;
	ztap = m_XF.ztap;
	kvnom = m_XF.kvnom;
	zkvnom = m_XF.zkvnom;
	strcpy(nd, m_XF.nd);
	strcpy(znd, m_XF.znd);
	ind = m_XF.ind;
	iznd = m_XF.iznd;
	ibs = m_XF.ibs;
	zbs = m_XF.zbs;
	mvanom = m_XF.mvanom;
	q = m_XF.q;
	topflags = m_XF.topflags;
	r1 = m_XF.r1;
	x1 = m_XF.x1;
	r0 = m_XF.r0;
	x0 = m_XF.x0;
	ialgip = m_XF.ialgip;
	ialgiq = m_XF.ialgiq;
	ialgii = m_XF.ialgii;
	ialgit = m_XF.ialgit;
	ialgzp = m_XF.ialgzp;
	ialgzq = m_XF.ialgzq;
	ialgzi = m_XF.ialgzi;
	ialgzt = m_XF.ialgzt;
	w = m_XF.w;
	r_ = m_XF.r_;
	i = m_XF.i;
	wz = m_XF.wz;
	rz = m_XF.rz;
	iz = m_XF.iz;
	r_xfmr = m_XF.r_xfmr;
	r_zn = m_XF.r_zn;

	basevoltage = m_XF.basevoltage;

	strcpy(fdid, m_XF.fdid);
	strcpy(ver, m_XF.ver);
	strcpy(vl, m_XF.vl);
	strcpy(tapty, m_XF.tapty);

	v_b = m_XF.v_b;
	adeg_b = m_XF.adeg_b;
	v_e = m_XF.v_e;
	adeg_e = m_XF.adeg_e;

	return*this;
}
XF::XF(const char* m_id)
{
	strcpy(id, m_id);
	descr[0] = 0;
	itapty = -1;
	iztapty = -1;
	tap = 0;
	ztap = 0;
	kvnom = 0.0;
	zkvnom = 0.0;
	nd[0] = 0;
	znd[0] = 0;
	ind = -1;
	iznd = -1;
	ibs = -1;
	zbs = -1;
	mvanom = 0.0;
	q = 0;
	topflags = 0;
	r1 = 0.0;
	x1 = 0.0;
	r0 = 0.0;
	x0 = 0.0;
	ialgip = -1;
	ialgiq = -1;
	ialgii = -1;
	ialgit = -1;
	ialgzp = -1;
	ialgzq = -1;
	ialgzi = -1;
	ialgzt = -1;
	w = 0.0;
	r_ = 0.0;
	i = 0.0;
	wz = 0.0;
	rz = 0.0;
	iz = 0.0;
	r_xfmr = -1;
	r_zn = -1;

	fdid[0] = 0;
	ver[0] = 0;
	vl[0] = 0;
	basevoltage = 0;
	tapty[0] = 0;
	v_b = 0;
	adeg_b = 0;
	v_e = 0;
	adeg_e = 0;
}


/////////////////////////////////////////////////////////////////////////
// class XFMR //IMPLEMENTation

//IMPLEMENT_SERIAL(XFMR,CObject,0)

XFMR::XFMR()
{
	id[0] = 0;
	descr[0] = 0;
	type = 0;
	st[0] = 0;
	rh = 0;
	xh = 0;
	rm = 0;
	xm = 0;
	rl = 0;
	xl = 0;
	r0h = 0;
	x0h = 0;
	r0m = 0;
	x0m = 0;
	r0l = 0;
	x0l = 0;
	kvnomh = 110;
	kvnomm = 35;
	kvnoml = 10;
	mvah = -1;
	mvam = -1;
	mval = -1;
	algph[0] = 0;
	ialgph = -1;
	algqh[0] = 0;
	ialgqh = -1;
	algih[0] = 0;
	ialgih = -1;
	algth[0] = 0;
	ialgth = -1;
	algpm[0] = 0;
	ialgpm = -1;
	algqm[0] = 0;
	ialgqm = -1;
	algim[0] = 0;
	ialgim = -1;
	algtm[0] = 0;
	ialgtm = -1;
	algpl[0] = 0;
	ialgpl = -1;
	algql[0] = 0;
	ialgql = -1;
	algil[0] = 0;
	ialgil = -1;
	algtl[0] = 0;
	ialgtl = -1;
	hnd[0] = 0;
	mnd[0] = 0;
	lnd[0] = 0;
	htapty[0] = 0;
	mtapty[0] = 0;
	ltapty[0] = 0;
	htap = -1;
	mtap = -1;
	ltap = -1;
	q = 0;
	topflags = 0;
	r_st = -1;
	r_xf = -1;

	ver[0] = 0;
	fdid[0] = 0;

	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;
}

const XFMR& XFMR::operator = (const XFMR& m_XFMR)
{
	strcpy(id, m_XFMR.id);
	strcpy(descr, m_XFMR.descr);
	type = m_XFMR.type;
	strcpy(st, m_XFMR.st);
	rh = m_XFMR.rh;
	xh = m_XFMR.xh;
	rm = m_XFMR.rm;
	xm = m_XFMR.xm;
	rl = m_XFMR.rl;
	xl = m_XFMR.xl;
	r0h = m_XFMR.r0h;
	x0h = m_XFMR.x0h;
	r0m = m_XFMR.r0m;
	x0m = m_XFMR.x0m;
	r0l = m_XFMR.r0l;
	x0l = m_XFMR.x0l;
	kvnomh = m_XFMR.kvnomh;
	kvnomm = m_XFMR.kvnomm;
	kvnoml = m_XFMR.kvnoml;
	mvah = m_XFMR.mvah;
	mvam = m_XFMR.mvam;
	mval = m_XFMR.mval;
	strcpy(algph, m_XFMR.algph);
	ialgph = m_XFMR.ialgph;
	strcpy(algqh, m_XFMR.algqh);
	ialgqh = m_XFMR.ialgqh;
	strcpy(algih, m_XFMR.algih);
	ialgih = m_XFMR.ialgih;
	strcpy(algth, m_XFMR.algth);
	ialgth = m_XFMR.ialgth;
	strcpy(algpm, m_XFMR.algpm);
	ialgpm = m_XFMR.ialgpm;
	strcpy(algqm, m_XFMR.algqm);
	ialgqm = m_XFMR.ialgqm;
	strcpy(algim, m_XFMR.algim);
	ialgim = m_XFMR.ialgim;
	strcpy(algtm, m_XFMR.algtm);
	ialgtm = m_XFMR.ialgtm;
	strcpy(algpl, m_XFMR.algpl);
	ialgpl = m_XFMR.ialgpl;
	strcpy(algql, m_XFMR.algql);
	ialgql = m_XFMR.ialgql;
	strcpy(algil, m_XFMR.algil);
	ialgil = m_XFMR.ialgil;
	strcpy(algtl, m_XFMR.algtl);
	ialgtl = m_XFMR.ialgtl;
	strcpy(hnd, m_XFMR.hnd);
	strcpy(mnd, m_XFMR.mnd);
	strcpy(lnd, m_XFMR.lnd);
	strcpy(htapty, m_XFMR.htapty);
	strcpy(mtapty, m_XFMR.mtapty);
	strcpy(ltapty, m_XFMR.ltapty);
	htap = m_XFMR.htap;
	mtap = m_XFMR.mtap;
	ltap = m_XFMR.ltap;
	q = m_XFMR.q;
	topflags = m_XFMR.topflags;
	r_st = m_XFMR.r_st;
	r_xf = m_XFMR.r_xf;

	prob_fault = m_XFMR.prob_fault;
	repairTime = m_XFMR.repairTime;
	strcpy(ver, m_XFMR.ver);
	strcpy(fdid, m_XFMR.fdid);

	return*this;
}

XFMR::XFMR(const char* m_id)
{
	strcpy(id, m_id);
	descr[0] = 0;
	type = 0;
	st[0] = 0;
	rh = 0.0;
	xh = 0.0;
	rm = 0.0;
	xm = 0.0;
	rl = 0.0;
	xl = 0.0;
	r0h = 0;
	x0h = 0;
	r0m = 0;
	x0m = 0;
	r0l = 0;
	x0l = 0;
	kvnomh = 110;
	kvnomm = 35;
	kvnoml = 10;
	mvah = 100.0;
	mvam = 100.0;
	mval = 100.0;
	algph[0] = 0;
	ialgph = -1;
	algqh[0] = 0;
	ialgqh = -1;
	algih[0] = 0;
	ialgih = -1;
	algth[0] = 0;
	ialgth = -1;
	algpm[0] = 0;
	ialgpm = -1;
	algqm[0] = 0;
	ialgqm = -1;
	algim[0] = 0;
	ialgim = -1;
	algtm[0] = 0;
	ialgtm = -1;
	algpl[0] = 0;
	ialgpl = -1;
	algql[0] = 0;
	ialgql = -1;
	algil[0] = 0;
	ialgil = -1;
	algtl[0] = 0;
	ialgtl = -1;
	hnd[0] = 0;
	mnd[0] = 0;
	lnd[0] = 0;
	htapty[0] = 0;
	mtapty[0] = 0;
	ltapty[0] = 0;
	htap = -1;
	mtap = -1;
	ltap = -1;
	q = 0;
	topflags = 0;
	r_st = -1;
	r_xf = -1;

	ver[0] = 0;
	fdid[0] = 0;
	prob_fault=0;//故障率
	repairTime=0;//5.5小时 故障修复时间;
}

/////////////////////////////////////////////////////////////////////////
// class ZN //IMPLEMENTation

//IMPLEMENT_SERIAL(ZN,CObject,0)

ZN::ZN()
{
    id = -1;
    descr[0] = 0;
    type = 0;

    q = 0;
    topflags = 0;
    r_st = -1;
    r_ld = -1;
    r_sec = -1;
    r_cb = -1;
	r_fd = -1;
	r_cp = -1;
	r_xf = -1;

    ver[0] = 0;
    fdid[0]=0;

	unfd_idList[0]=0;
	opencblist[0]=0;
	closeunlist[0] = 0;

	inner_C=-1;//区段内用户
	inner_P=-1;//区段内有功
	next_C=-1;//区段下游用户
	next_P=-1;//区段下游有功(不含本区段)
	lossC =-1;
	lossP =-1;
	transC =-1;
	transP =-1;

	cbptr = -1;
	
	fault_oper_type = -1;
	plan_oper_type = -1;
}

void ZN::Reset()
{
	id = -1;
	descr[0] = 0;
	type = 0;

	q = 0;
	topflags = 0;
	r_st = -1;
	r_ld = -1;
	r_sec = -1;
	r_cb = -1;
	r_fd = -1;
	r_cp = -1;
	r_xf = -1;

	ver[0] = 0;
	fdid[0] = 0;

	unfd_idList[0] = 0;
	opencblist[0] = 0;
	closeunlist[0] = 0;

	inner_C = -1;//区段内用户
	inner_P = -1;//区段内有功
	next_C = -1;//区段下游用户
	next_P = -1;//区段下游有功(不含本区段)
	lossC = -1;
	lossP = -1;
	transC = -1;
	transP = -1;

	cbptr = -1;

	fault_oper_type = -1;
	plan_oper_type = -1;
}

const ZN& ZN::operator = (const ZN& m_ZN)
{
	id = m_ZN.id;
	strcpy(descr,m_ZN.descr);
	type = m_ZN.type;

	q = m_ZN.q;
	topflags = m_ZN.topflags;
	r_st = m_ZN.r_st;
	r_ld = m_ZN.r_ld;
	r_sec = m_ZN.r_sec;
	r_cb = m_ZN.r_cb;
	r_fd = m_ZN.r_fd;
	r_cp = m_ZN.r_cp;
	r_xf = m_ZN.r_xf;

	strcpy(ver,m_ZN.ver);
	strcpy(fdid,m_ZN.fdid);

	strcpy(unfd_idList,m_ZN.unfd_idList);
	strcpy(opencblist,m_ZN.opencblist);

	inner_C=m_ZN.inner_C;//区段内用户
	inner_P=m_ZN.inner_P;//区段内有功
	next_C=m_ZN.next_C;//区段下游用户
	next_P=m_ZN.next_P;//区段下游有功(不含本区段)	
	fault_oper_type = m_ZN.fault_oper_type;
	plan_oper_type = m_ZN.plan_oper_type;

	cbptr=m_ZN.cbptr;//区段下游有功(不含本区段)	

	return* this;
}

ZN::ZN(const int m_id)
{
    id =-1;
    descr[0] = 0;
    type = 0;

    q = 0;
    topflags = 0;
    r_st = -1;
    r_ld = -1;
    r_sec = -1;
    r_cb = -1;
	r_fd = -1;
	r_cp = -1;

    ver[0] = 0;
    fdid[0]=0;

	unfd_idList[0]=0;
	opencblist[0]=0;
//	transSec[0]=0;
//	lossSec[0]=0;

	inner_C=-1;//区段内用户
	inner_P=-1;//区段内有功
	next_C=-1;//区段下游用户
	next_P=-1;//区段下游有功(不含本区段)
	lossC =-1;
	lossP =-1;
	transC =-1;
	transP =-1;

	cbptr = -1;

	fault_oper_type = -1;
	plan_oper_type = -1;
}


