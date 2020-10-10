// optSect.cpp: implementation of the CReliabilty class.
//
//////////////////////////////////////////////////////////////////////
#include "math.h"
#include "../DNASPLIB/optSect.h"
#include "../DNASPLIB/LLD.h"
//#include "stdafx.h"
//
//#if __GNUC__>2
//#include <ext/hash_set>
//#include <ext/hash_map>
//using namespace __gnu_cxx;
//#else
//#include <hash_set>
//#include <hash_map>
//using namespace stdext;
//#endif


#include <vector> 
#include <fstream>
using namespace std;

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void optSect::initMainDev(stru_MainDev& mainDev)
{
	mainDev.eq_id = -1;//设备原始物理ID号
	mainDev.type = -1;//0-线路，1-普通开关，2-出线开关，3-联络开关，4-节点
	mainDev.idph = -1;//设备原始物理ID号
	mainDev.id[0]=0;//设备原始ID号
	mainDev.ind=-1;//设备原始物理ID号
	mainDev.iznd = -1;//设备原始物理ID号
	mainDev.len = 0;
	mainDev.x1 = 0;
	mainDev.y1 = 0;
	mainDev.x2 = 0;
	mainDev.y2 = 0;
}

optSect::optSect()
{
}

optSect::optSect(DSBOB2* m_pDb, CLLD* m_pLLD) : CNM1(m_pDb, m_pLLD)
{
	dsbob2		= m_pDb;//将dsbob2传进来后就可以使用其全部的结果，包括潮流计算的结果
	lld = m_pLLD;

	if (lld->mx_cb > 0)
		cb_tb		= dsbob2->cb_tb;

	so_tb		= dsbob2->so_tb;
	ld_tb		= dsbob2->ld_tb;
	sec_tb		= dsbob2->sec_tb;
	nd_tb		= dsbob2->nd_tb;
	r_nd_cb_tb	= dsbob2->r_nd_cb_tb;
	r_nd_xf_tb	= dsbob2->r_nd_xf_tb;
	r_nd_sec_tb = dsbob2->r_nd_sec_tb;
	r_nd_ld_tb  = dsbob2->r_nd_ld_tb;

	equip = new EQ();

	equip_tb = new DTable<EQ>(maxEquipment);//先把遥测表的内存开出来
	r_nd_equip_tb = new DTable<R_ND_EQ>(5*maxEquipment);
}

optSect::~optSect()
{
	delete equip;
	//delete r_nd_equip;

	delete equip_tb;
	delete r_nd_equip_tb;
}

//比较2个相似向量,返回分叉点的序号，不是分叉点的编号
int optSect::comp_vector(int* src,int* comp,int len,int& ele,bool bReverse)
{
	int vec1[1000],vec2[1000];
	int pos1=-1,pos2=-1,i,j;
	
	if (bReverse=true)
	{	
		//先转置
		for(i=0; i<len; i++)
		{
			if ( (src[i] == -1) && (pos1<0) )
				pos1 = i;

			if ( (comp[i] == -1) && (pos2<0) )
				pos2 = i;
			if ( (pos1>0) && (pos2>0) )
				break;
		}

		j=0;
		for(i=pos1-1; i>=0; i--)
		{
			vec1[j++] = src[i];
		}
		for(i=pos1; i<1000; i++)
		{
			vec1[j++] = -1;
		}

		j=0;
		for(i=pos2-1; i>=0; i--)
		{
			vec2[j++] = comp[i];
		}
		for(i=pos2; i<1000; i++)
		{
			vec2[j++] = -1;
		}
	}

	for(i=0; i<len; i++)
	{
		if ((vec1[i] == -1))
		{
			ele=vec1[i-1];
			return i-1;
		}
			//return  -1;
		if ((vec2[i] == -1))
		{
			ele=vec2[i-1];
			return i-1;
		}

		if (vec1[i] != vec2[i])
		{
			ele=vec1[i-1];
			return i-1;
		}
	}
	
	return -1;


}

//比较2个相似向量,返回分叉点的序号，不是分叉点的编号
int optSect::comp_vector(int* src,int* comp,int& ele)
{
	for(int i=0; i<MAX_MINPATH_LEN; i++)
	{
		if ((src[i] == -1))
		{
			ele=src[i-1];
			return i-1;
		}
			//return  -1;
		if ((comp[i] == -1))
		{
			ele=comp[i-1];
			return i-1;
		}

		if (src[i] != comp[i])
		{
			ele=src[i-1];
			return i-1;
		}
	}
	
	return -1;
}

//向量中两个元素进行比较，先出现返回1，后出现返回2，都没用，返回-1
int optSect::ele_comp(int* src, int ele1, int ele2)
{
	for(int i=0; i<MAX_MINPATH_LEN; i++)
	{
		if (src[i] == ele1)
		{
			return 1;
		}
		if (src[i] == ele2)
		{
			return 2;
		}
	}
	
	return -1;
}

//ele是否在向量中，是返回向量的位置，否返回0
//需要考虑中间联络，也就是 设备最小路包括主干线的路径即可
int optSect::ele_vector(int* src, int ele, int len)
{

//这里可以采用对分法进行求解
//1.先个src[]进行排序

//2.对分法


	if (len > 0)
	{
//二分法
		int high,middle,low;

		int ndCount = len;

		high = ndCount - 1;
		low = 0;
		while(low <= high)
		{
			middle = (low + high) / 2;
			
			if (ele == src[middle])
			{  //找到,返回下标索引值
				return 1;
			}
			else if(ele < src[middle])
			{  //查找值在低半区
				high = middle - 1;
			}
			else
			{  //查找值在高半区
				low = middle + 1;
			}
		}
	}
	else
	{
		
	//	long tt=lld->getCurMi();

		if (ele == -1)//主干线已经没有了
		{
			//已经发现分叉点
			return -1;
		}
		else
		{
		}
	//	lld->eleInvector(src,MAX_MINPATH_LEN,ele);

		for(int i=0; i<MAX_MINPATH_LEN; i++)
		{
			if (src[i] == ele)
			{
				return i;
			}
		}
	//	long tt=lld->getCurMi();
	//	t_toptSectn += lld->getCurMi()-tt;
	}
	return -1;
}

//对路径进行初始化
void optSect::InitPath()
{
	equip_tb->Empty();
	r_nd_equip_tb->Empty();

	int i,j;
	for (i=0; i<maxLoad;i++)//每个负荷的最小路
	{
		path_load[i].i_id = -1;
		path_load[i].id[0] = 0;
		path_load[i].ind = -1;
		path_load[i].monkV = 0;
		path_load[i].ptr = -1;
		path_load[i].q = 0;//
		path_load[i].s = 0;
		path_load[i].level = 2;//默认为二级，一般负荷

		for (j=0; j<MAX_MINPATH_LEN;j++)
		{
			path_load[i].minpath[j] = -1;//最小路上的节点列表
			path_load[i].equippath[j] = -1;//最小路的设备列表
		}	

		path_load[i].cntbakpath = 0;
	}

	for (i=0; i<maxMainND;i++)//主干线上的节点
	{
		path_mainline[i].ind = -1;
		path_mainline[i].equip_id_l[0] = 0;//左边设备，靠近电源点的设备
		path_mainline[i].equip_id_r[0] = 0;
		path_mainline[i].equip_ind_l  = -1;
		path_mainline[i].equip_ind_r  = -1;
		path_mainline[i].sumLD = 0;
		path_mainline[i].sumP = 0;
		path_mainline[i].sumQ = 0;
		path_mainline[i].len = 0;
		path_mainline[i].UpCBID[0] = 0;//左边设备，靠近电源点的设备
		path_mainline[i].UnionCBID[0] = 0;
		path_mainline[i].ldList[0] = 0;
		path_mainline[i].isValid = 1;//主节点默认都是有效的
	}
	

	for (i=0; i<maxUnion;i++)//每个联络开关的最小路
	{
		path_union[i].id[0] = 0;
		path_union[i].ind = -1;
		path_union[i].Abun = 0;
		path_union[i].ptr = -1;
		path_union[i].q = 0;
		for (j=0; j<MAX_MINPATH_LEN;j++)
		{
			path_union[i].mainpath[j] = -1;
			path_union[i].equipmainpath[j] = -1;//联络开关到电源点的最小路
		}	
	}	

	//设备清零
	cntEquipment=cntload=cntunion=0;
}

//根据节点号，以及主干线列表，计算该节点真实的潮流
double optSect::OnGetRealFlow(int ind,int before)
{
	double ret = -1;
	ChildPtr1	*sec_p, *sec_p0;
//	ChildPtr1	*cb_p, *cb_p0;
	
	ND* nd = nd_tb->GetTable();
	sec_p = r_nd_sec_tb->FindChildPtr1((nd+ind)->r_sec);
	sec_p0 = sec_p;
	while(sec_p)
	{
		//find the opposite nd_t through this sec
		if( (before == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
			|| (before == sec_tb->GetDataByPh(sec_p->Ptr)->iznd) )
		{
			//直接找到线路段
			ret = fabs(sec_tb->GetDataByPh(sec_p->Ptr)->wz);
			break;
		}

		sec_p = sec_p->Next;
	}
	::FreeChildPtr(sec_p0);

	return ret;
}

//优化分段开关的配置，lld add 2017-12-27
//1.从馈线点开始扫描，一直找到联络开关，得到连续的主干线集合，如果指定了主干线，更好
//2.将所有分支线中的全部负荷归集到主干线
//3.对主干线进行分段，一般分三段，确认分段位置
//4.找到离分段点最近的开关，暂时设为分段开关
int optSect::OnOptSect(int type,char* mainline)
{
	int ret;

   	InitPath();

	int ind = so_tb->GetDataByLog(0)->ind;//搜索的起始节点编号，电源点
	sond = ind;

	int cntLD;//馈线上总的累计值
	double sumLen;
	double sumP;
	ret = OnBuildMainLine(so_tb->GetDataByPh(0)->ind,cntLD,sumLen, sumP, mainline);

	if ((ret<0) || (cntMainLine == 0))
		return -1;

	int ind_t=-1;
	double sum_w=0;
	double before_w=0;
	double after_w=0;
	for(int k = 0; k < cntMainLine; k++)
	{
		ind_t = path_mainline[k].ind;//节点号
		ind = ind_t;
		nd_tb->GetDataByPh(ind)->q |= ND_MAIN;//主节点
//		if(ind_t == 2)
//		{
//			int deby=0;
//		}
	
		//此处仅仅只是潮流负荷的累加，并不一定是真实的潮流值
		sum_w = nd_tb->GetDataByPh(ind_t)->w = path_mainline[k].sumQ;

		nd_tb->GetDataByPh(ind_t)->len = path_mainline[k].len;
		nd_tb->GetDataByPh(ind_t)->primebs = path_mainline[k].sumLD;
		strcpy(nd_tb->GetDataByPh(ind_t)->ldList , path_mainline[k].ldList);

		strcpy(nd_tb->GetDataByPh(ind_t)->UpCBID , path_mainline[k].UpCBID);
		strcpy(nd_tb->GetDataByPh(ind_t)->UnionCBID , path_mainline[k].UnionCBID);		
	}	

	//此处形成鱼骨图的相关位置信息




//		double ss = dsbob2->nd_tb->GetDataByPh(2)->w;

	return ret;
}

int optSect::isInMainLine(int knd,PATH_MAINLINE* path,int len)
{
	for(int l=0; l<len;l++)
	{
		if (path[l].ind == knd)
		{
			return l;
		}
	}
	return  -1;
}

int optSect::isInMainLine(int knd)
{
	for(int l=0; l<cntMainLine;l++)
	{
		if (path_mainline[l].ind == knd)
		{
			return l;
		}
	}
	return  -1;
}

//判断是否属于开关，返回开关的ID号
bool optSect::isCB(int knd,int kknd,char* cbID)
{
	bool bFindCB = false;
	char UpCBID[MAX_ID_DLL]; 
	UpCBID[0]=0;
	for (int i=0; i<equip_tb->GetCount();i++)
	{
		int ind = equip_tb->GetDataByLog(i)->ind;
		int iznd = equip_tb->GetDataByLog(i)->iznd;

		if ( ( (ind == knd) && (iznd == kknd) ) || ( (ind == kknd) && (iznd == knd) ) )
		{
			//如果是分段开关，将分段开关的ID号赋给前一个主节点
			if ( (equip_tb->GetDataByLog(i)->type & Q_BREAKER)
				|| (equip_tb->GetDataByLog(i)->type & Q_BREAKERRELAY)
				||(equip_tb->GetDataByLog(i)->type & Q_LOADBREAKSWITCH)
				||(equip_tb->GetDataByLog(i)->type & Q_FUSE)
				||(equip_tb->GetDataByLog(i)->type & Q_DISCONNECTOR))
			{
				strcpy(UpCBID,equip_tb->GetDataByLog(i)->id);
				bFindCB = true;
			}
			break;
		}
	}

	strcpy(cbID,UpCBID);
	return bFindCB;	
}

double optSect::isSEC(int knd,int kknd,double& len)
{
	char main_sec_id[MAX_ID_DLL];
	main_sec_id[0]=0;
	double w=0;

	for (int i=0; i<equip_tb->GetCount();i++)
	{
		int ind = equip_tb->GetDataByLog(i)->ind;
		int iznd = equip_tb->GetDataByLog(i)->iznd;

		if ( ( (ind == knd) && (iznd == kknd) ) || ( (ind == kknd) && (iznd == knd) ) )
		{
			strcpy(main_sec_id,equip_tb->GetDataByLog(i)->id);

			if (   (equip_tb->GetDataByLog(i)->type & Q_ACLINESEGMENT)
				|| (equip_tb->GetDataByLog(i)->type & Q_INSULATION)
				|| (equip_tb->GetDataByLog(i)->type & Q_CONNLINE)
				|| (equip_tb->GetDataByLog(i)->type & Q_CABLE) )
			{
				SEC* sect = new SEC(main_sec_id);
				int ttt = sec_tb->FindPh(*sect);
				delete sect;

				//线段已经处理过了，说明有几个负荷到达这个主节点
				if(sec_tb->GetDataByPh(ttt)->q & ND_UNPROC) //this so nd_t has not processed
				{
					sec_tb->GetDataByPh(ttt)->q &= (~ND_UNPROC);//将当前节点设为已处理
	 
					w = fabs(sec_tb->GetDataByPh(ttt)->w);
					if (w < fabs(sec_tb->GetDataByPh(ttt)->wz))
						w = fabs(sec_tb->GetDataByPh(ttt)->wz);//选最大的那个

					len = sec_tb->GetDataByPh(ttt)->len;
					return w;
				}
				else
					return -2;
			}
		}
	}

	return -1;	
}

//？？对于树枝状的主干线，如何界定馈线形状
//1.均匀线路，2.橄榄型；3.哑铃型；4.杠杆型
int optSect::OnAnalyseShape()
{
	if (cntMainLine == 0)
		return -1;

	int knd,kknd;
	int ret=1;
	int j,sect1lds,sect2lds,sect3lds;
	double len,len1,sect1w,sect2w,sect3w;
	//计算馈线的形态
	len1=0;

	j=0;
	knd = path_mainline[j].ind;
	while(j<cntMainLine)
	{
		kknd = path_mainline[j+1].ind;
		len=0;
		isSEC(knd,kknd,len);
		if (len>0)
			path_mainline[j].len = len;
		knd=kknd;
		len1 += len;
		j++;
	}
	
	j=1;
	double avg = len1/3;
	double tempW=0;
	int tempLDs = 0;
	len1=0;
	for(int ii=0; ii<cntMainLine;ii++)
	{
		len=path_mainline[ii].len;
		len1 += len;

		if ( (len1>avg) && (j<3))
		{
			if (j==1)
			{
				sect3w = tempW;
				sect3lds = tempLDs;
			}
			else if (j==2)
			{
				sect2w = tempW;
				sect2lds = tempLDs;
			}
			len1=len;
			tempW=path_mainline[ii].sumQ;
			tempLDs=path_mainline[ii].sumLD;
			j++;
		}
		else
		{
			tempW += path_mainline[ii].sumQ;
			tempLDs += path_mainline[ii].sumLD;
		}
	}
	sect1w = tempW;
	sect1lds = tempLDs;

	//归一化，加权
	double temp1 = 0.7*sect1w/(sect1w+sect2w+sect3w)+0.3*sect1lds/(sect1lds+sect2lds+sect3lds);
	double temp2 = 0.7*sect2w/(sect1w+sect2w+sect3w)+0.3*sect2lds/(sect1lds+sect2lds+sect3lds);
	double temp3 = 0.7*sect3w/(sect1w+sect2w+sect3w)+0.3*sect3lds/(sect1lds+sect2lds+sect3lds);

	if ( 
		((temp1 > 4*temp2)&& temp1>4*temp3)
	  ||((temp2 > 4*temp2)&& temp3>4*temp1)
	  )
	  ret = 4;//杠杆型
	
	if ( 
		((temp1 > 4*temp2)&& temp3>4*temp2)
	  //||((temp2 > 4*temp2)&& temp3>4*temp1)
	  )
	  ret = 3;//哑铃型

	if ( 
		((temp2 > 4*temp1)&& temp2>4*temp3)
	  //||((temp2 > 4*temp2)&& temp3>4*temp1)
	  )
	  ret = 2;//纺锤型
	
	return ret;
}

//set value to main node,such as P,lds,etc
int optSect::OnSetMainLine(PATH_MAINLINE* path_mainline,int cntMainLine,int sond)
{
	int knd,kknd,ret_ld;
	double w,ret_w,len;
	char cbID[64];
	char cbID_[64];
	bool existCB;
	int i;

	int lv_sec = dsbob2->sec_tb->GetCount();
	SEC* sec = dsbob2->sec_tb->GetTable();
	for(i = 0; i < lv_sec; i++)
	{
		(sec+i)->q |= ND_UNPROC;
	}

	for(i=0; i<cntload; i++)//个数
	{
		bool multiLoad=false;
		w=path_load[i].s;

		cbID[0]=0;
		knd = path_load[i].ind;//
		if ((ret_ld = isInMainLine(knd,path_mainline,cntMainLine)) != -1)
		{
			path_mainline[ret_ld].sumLD++;
			path_mainline[ret_ld].sumP += path_load[i].s;
			strcat(path_mainline[ret_ld].ldList,path_load[i].id);
			strcat(path_mainline[ret_ld].ldList,",");
			continue;
		}

		while (1)
		{
			kknd = (nd_t+knd)->bch;
			
			//所属的设备类别找到
			if (isCB(knd,kknd,cbID_))
			{
				strcpy(cbID,cbID_);
				existCB = true;
			}

			//最后一条线路上的有功值即可
			len=0;
			ret_w = isSEC(knd,kknd,len);
			if (ret_w == -2)//processed，多路负荷到一个节点 
			{
				multiLoad = true;
			}
			else if (ret_w >= 0)
			{
				if (ret_w > w)
					w = ret_w;
			}

			//load reached the main line
			if ((ret_ld = isInMainLine(kknd,path_mainline,cntMainLine)) != -1)
			{
				path_mainline[ret_ld].sumLD++;
				path_mainline[ret_ld].sumP = path_load[i].s;
				strcat(path_mainline[ret_ld].ldList,path_load[i].id);
				strcat(path_mainline[ret_ld].ldList,",");
				if (existCB)
					strcpy(path_mainline[ret_ld].UpCBID,cbID);
				if (!multiLoad && (w>0))
					path_mainline[ret_ld].sumQ += w;

				//path_mainline[ret_ld].len += len;
				break;
			}

			knd = kknd;//得到开关的上级节点号
			if (knd==sond)//回溯到输入节点
			{
				break;
			}
		}
	}

	return 1;
}

//1.构建主干线，从最远端的联络开关开始进行主干线的构建，或者最远端的馈线段开始构建
//*********************手工设主干线的优先级最高
//2.每一节点的联络信息要输出，而且节点的最近的上游分段开关要输出
//1.先确定主干线，然后用负荷进行回溯
//2.先确定一个最远的主干线，然后用其它的联络开关回溯，如果回溯到主干线的时候没有负荷，这直接将联络开关折算到主干点
int optSect::OnBuildMainLine(int sond,int& cntLD,double& sumLen,double& sumP, char* mainline)
{
	int ret =  1;
	int			i,j;
	int			ind, knd, kknd, kkndcb;

	char fdid_abund[MAX_SEC_ID_LEN];
	char opp_ndid[MAX_SEC_ID_LEN];
	double p_abund,q_abund,v_abund;

	double p,q;//功率
	
	int main_ind,main_iznd;//最后一个主干线的首末节点，带顺序
	char main_sec_id[MAX_SEC_ID_LEN];//最后一个主干线
	char main_sec_id_t[MAX_SEC_ID_LEN];//最后一个主干线
	main_ind=main_iznd=-1;
	main_sec_id[0]=0;
	main_sec_id_t[0]=0;
	
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;

	char fdid[MAX_SEC_ID_LEN];//馈线id，得到电源点的所属馈线
	fdid[0]=0;
	double fault_ltime = 0;

	bool isSetMainND=0;//判断是否已设定主干线

	//做整型向量,20行（20个联络开关），200列（联络开关的最小路有500个元素）
	int unioncb[50][500];
	int mainndmax=0;
	//折到每一个主节点上的负荷值
	int ret_ld=-1;
	double ret_w=-1;
	char cbID_[MAX_ID_DLL];
	char cbID[MAX_ID_DLL];
	bool existCB=false;

	//1.全景扫描
	first = 1;
	prevph = -1;

//	if (sond>=0)
//		fault_ltime = so_tb->GetDataByLog(sond)->fault_ltime;

	cntEquipment=cntload=cntunion=cntMainLine=0;
	equip_tb->Empty();//清空设备表
	r_nd_equip_tb->Empty();

//////////////////////////////////////////////////////////////////////////

	//read all nds into buffer
	lv_nd = nd_tb->GetCount();
	nd_t = new ND[lv_nd];
	nd_tb->CopyTable(nd_t);

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
		(nd_t + i)->r_eq |= -1;//在以前做区段的时候已经做了一遍了
	}

	ind = sond;//从电源点开始搜索
	//节点的bch为上级节点
	(nd_t+ind)->bch = ind;//从源点开始开始进行,将源点作为上一级记录下来
	
	//之前已经将所有的节点都置为未处理
	if((nd_t+ind)->q & ND_UNPROC) //this so nd_t has not processed
	{
		//拿当前节点向下扩展，找到所有和它相连的节点
		knd = ind;//当前节点赋给一个临时的变量
		(nd_t+knd)->fch = knd; //当前节点号赋给前向链 
		(nd_t+knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

		while (1)//还有扩展开关没有处理
		{
//			if(knd==16)
//			{
//				int debug=0;
//			}

			//通过节点的q可以判断出是不是负荷节点，q |= 8
			if ((nd_t+knd)->q & ND_LD) 
			{
				//得到负荷id
				ld_p = r_nd_ld_tb->FindChildPtr1((nd_t+knd)->r_ld);
				ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
				while(ld_p)
				{
					strcpy(path_load[cntload].id , ld_tb->GetDataByPh(ld_p->Ptr)->id);
					path_load[cntload].ind = ld_tb->GetDataByPh(ld_p->Ptr)->ind;
					path_load[cntload].ptr = ld_p->Ptr;

					path_load[cntload].q = 0;//非双电源用户

					path_load[cntload].level = ld_tb->GetDataByPh(ld_p->Ptr)->level;//一般负荷

					p = ld_tb->GetDataByPh(ld_p->Ptr)->w;
					q = ld_tb->GetDataByPh(ld_p->Ptr)->r;
					//s = sqrt(p*p+q*q); 
					path_load[cntload].s = p;//潮流视在功率,变压器铭牌上的功率是视在功率kVA，有功是kW

					cntload++;
//////////////////////////////////////////////////////////////////////////
					//增加表数据,lld 2014-3-18
					insert_equip(cntEquipment,ld_tb->GetDataByPh(ld_p->Ptr)->id, 
						ld_p->Ptr,ld_tb->GetDataByPh(ld_p->Ptr)->ind,-1,20, \
						ld_tb->GetDataByPh(ld_p->Ptr)->prob_fault, \
						fault_ltime+ld_tb->GetDataByPh(ld_p->Ptr)->repairTime);
						
					insert_r_nd_equip(nd_t,ld_tb->GetDataByPh(ld_p->Ptr)->id,knd,cntEquipment);
					
					equip->r_zn = -1;//lld 2014=07-22
					equip_tb->Insert(*equip);
					cntEquipment++;
//////////////////////////////////////////////////////////////////////////

					ld_p = ld_p->Next;
				}
				::FreeChildPtr(ld_p0);
			}
		

			//read cbs connected to this nd
			//从节点-开关关系表找到所有和这个节点相连的开关
			cb_p = r_nd_cb_tb->FindChildPtr1((nd_t+knd)->r_cb);
			cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
			while(cb_p)
			{
				//find the opposite nd_t through this cb
				//kknd表示开关的对侧节点，这里的节点都是整型的编号
				if(knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}

//			if(kkndcb==16)
//			{
//				int debug=0;
//			}
				int cb_q=cb_tb->GetDataByPh(cb_p->Ptr)->q;

				//如果是联络开关，停下来,只要是联络开关，不管是否悬空，都需要进行处理
				if ( (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//联络开关的q为1，常开
					//|| (cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB))  
					//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0) comment lld for FD 2017-12-28
					) 
				{
					//联络开关的后向链也需要记录下来，需要从联络开关处回溯
					if (kkndcb != -1)
					  (nd_t+kkndcb)->bch = knd;

					path_union[cntunion].ind = knd;
					
					strcpy(path_union[cntunion].id , cb_tb->GetDataByPh(cb_p->Ptr)->id);
					path_union[cntunion].ptr=cb_p->Ptr;
					
					fdid_abund[0]=0;
					opp_ndid[0]=0;
					p_abund=q_abund=v_abund=0;
					getUNCB_abun(fdid,cb_tb->GetDataByPh(cb_p->Ptr)->id,opp_ndid,fdid_abund,p_abund,q_abund,v_abund);
					
					if ( (p_abund==0) && (q_abund==0) && (v_abund==0) )
					{
						path_union[cntunion].Abun = UNION_TRAN;//找不到对侧的裕度，用默认值
					}
					else
						path_union[cntunion].Abun = sqrt(p_abund*p_abund + q_abund*q_abund);
				
					cntunion++;

					if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_BREAKER)
						type = 0;
					if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_LOADBREAKSWITCH)
						type = 1;
					if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_DISCONNECTOR)
						type = 2;
					if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_FUSE)
						type = 3;
					if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)
						type = 4;
					if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_BREAKERRELAY)
						type = 5;

					//type;//设备类型 0-breaker；1-loadbreakswitch；2-disconnector；3-fuse；4-联络开关；5-带保护开关；10-线路；20-负荷
							 //联络开关也要加入到设备里面
					insert_equip(cntEquipment,(cb_tb->GetDataByPh(cb_p->Ptr)->id), \
						cb_p->Ptr,knd,kkndcb,type, \
						cb_tb->GetDataByPh(cb_p->Ptr)->prob_fault, \
						fault_ltime+cb_tb->GetDataByPh(cb_p->Ptr)->repairTime);
						
					insert_r_nd_equip(nd_t,(nd_t+knd)->id,knd,cntEquipment);
					if(kkndcb!=-1)
						insert_r_nd_equip(nd_t,(nd_t+kkndcb)->id,kkndcb,cntEquipment);

					equip->r_zn = -1;//lld 2014=07-22
					equip_tb->Insert(*equip);

					cntEquipment++;

					//如果是从联络开关进行搜索，其它联络等同于普通悬空开关
					cb_p = cb_p->Next;
					continue;
				}

//printf("%s %d\n",cb_tb->GetDataByPh(cb_p->Ptr)->id,cb_tb->GetDataByPh(cb_p->Ptr)->q);
				//只有闭合的开关才需要向下扩展
				if ((cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB))
				{
					//断开开关的后向链不需要记录下来
					cb_p = cb_p->Next;
					continue;
				}

				//process this opposite nd
				//如果对侧节点 未处理==开关不参与拓扑推进
				if((nd_t+kkndcb)->q & ND_UNPROC)
				{
					//避免重复处理开关
					insert_equip(cntEquipment,(cb_tb->GetDataByPh(cb_p->Ptr)->id), \
						cb_p->Ptr,knd,kkndcb,cb_tb->GetDataByPh(cb_p->Ptr)->q, \
						cb_tb->GetDataByPh(cb_p->Ptr)->prob_fault, \
						fault_ltime+cb_tb->GetDataByPh(cb_p->Ptr)->repairTime);
						
					insert_r_nd_equip(nd_t,(nd_t+knd)->id,knd,cntEquipment);
					insert_r_nd_equip(nd_t,(nd_t+kkndcb)->id,kkndcb,cntEquipment);

					equip->r_zn = -1;//lld 2014=07-22
					equip_tb->Insert(*equip);

					cntEquipment++;

					//将这个节点的上一级记录下来，准备回溯
					(nd_t+kkndcb)->bch = knd;

					//将扩展的前向链 设成 找到的新节点的前向链
					(nd_t+kkndcb)->fch = (nd_t+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd_t+knd)->fch = kkndcb;
					//将找到的新节点设成已处理
					(nd_t+kkndcb)->q &= (~ND_UNPROC);
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this nd
			sec_p = r_nd_sec_tb->FindChildPtr1((nd_t+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//find the opposite nd_t through this sec
				if(knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
				{
					kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
				}
				else
				{
					kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;
				}

//							if(kknd==16)
//			{
//				int debug=0;
//			}


//printf("%s\n",sec_tb->GetDataByPh(sec_p->Ptr)->id);
				//process this opposite nd
				if((nd_t+kknd)->q & ND_UNPROC)
				{
					if (fdid[0] == 0)
						strcpy(fdid,sec_tb->GetDataByPh(sec_p->Ptr)->fdid);

					//计划停电
					double len = sec_tb->GetDataByPh(sec_p->Ptr)->len;//长度
					if (len == 1)
						len = 0;
					//add by lld 2017-5-13
					int sectype = sec_tb->GetDataByPh(sec_p->Ptr)->q;

					//是否为主干线
					if (sectype & Q_MAINLINE)
					{
						//以找到的最后一个主干线段为准
						main_ind = knd;
						main_iznd = kknd;
						strcpy(main_sec_id,sec_tb->GetDataByPh(sec_p->Ptr)->id);
						isSetMainND = true;//设定了主干线
					}

					//lld edit 2017-5-13
					insert_equip(cntEquipment,(sec_tb->GetDataByPh(sec_p->Ptr)->id), \
						sec_p->Ptr,knd,kknd,10, \
						sec_tb->GetDataByPh(sec_p->Ptr)->prob_fault, \
						fault_ltime+sec_tb->GetDataByPh(sec_p->Ptr)->repairTime,len,0);
						
					insert_r_nd_equip(nd_t,(nd_t+knd)->id,knd,cntEquipment);
					insert_r_nd_equip(nd_t,(nd_t+kknd)->id,kknd,cntEquipment);
					
					//将馈线段的所属区段加入到设备属性中,只有馈线段才有
					equip->r_zn = sec_tb->GetDataByPh(sec_p->Ptr)->r_zn;//lld 2014=07-22
					equip_tb->Insert(*equip);

					cntEquipment++;

					//循环链表的使用，每扩展一个新的节点，做2件事：
					//1.将上一个节点的前向链设为自己的前向链
					//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
					
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd_t+kknd)->fch = (nd_t+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd_t+knd)->fch = kknd;

					//将这个节点的上一级记录下来，准备回溯
					(nd_t+kknd)->bch = knd;

					//将找到的新节点设成已处理
					(nd_t+kknd)->q &= (~ND_UNPROC);
				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd，搜索完一层
			knd = (nd_t+knd)->fch;
			//循环到头，连起来了，表示全部走了一遍
			//拿另外的节点再开始搜索，一直到所有的节点都处理
			if(knd == ind)
			{
				break;
			}
		}
	}

	//对节点表进行更新，因为修改了节点的属性
	nd_tb->UpdateTable(nd_t);

	//ofstream outfile1("lld_equip.txt", ios::out);
	//int count = equip_tb->GetCount();
	//EQ* eq = equip_tb->GetTable();
	//outfile1<< "id       " << "nd    " << "znd       "<< "q     " << endl;
	//for (i = 0; i <count; i++)
	//{
	//	outfile1 << (eq + i)->id << "   " << (eq + i)->ind << "   " << (eq + i)->iznd << "   " << (eq + i)->idph << endl;
	//}

	//outfile1 << "    " << endl;
	//count = nd_tb->GetCount();
	//ND* eq2 = nd_tb->GetTable();
	//outfile1 << "id       " << "r_eq    " << "q     " << endl;
	//for (i = 0; i <count; i++)
	//{
	//	outfile1 << (eq2 + i)->id << "   " << (eq2 + i)->r_eq << "   " << (eq2 + i)->q << endl;
	//}

	//outfile1 << "    " << endl;

	//count = r_nd_equip_tb->GetCount();
	//R_ND_EQ* eq1 = r_nd_equip_tb->GetTable();
	//outfile1 << "id       " << "nd    " << "znd       " << "q     " << endl;
	//for (i = 0; i <count; i++)
	//{
	//	outfile1 << (eq1 + i)->id << "   " << (eq1 + i)->nd_id << "   " << (eq1 + i)->i_id << "   " << (eq1 + i)->fptr << "   " << (eq1 + i)->bptr << endl;
	//}
	//outfile1.close();


//全局搜索完毕后，直接回溯，得到每个负荷点到电源点的最小路
//将最小路以数组的形式返回
	double len=0;
	double len1=0;
//	double avg;//三段的长度
	double sect1w,sect2w,sect3w;//三段的功率总加
	int sect1lds,sect2lds,sect3lds;//三段的负荷个数总加
	
	double w=0; //潮流大小
	cntMainLine = 0;
	int lastMainLine = 0;//上一个有效的主节点
	char UpCBID_old[MAX_ID_DLL];//上游开关的ID
	UpCBID_old[0]=0;
	char UpCBID[MAX_ID_DLL];//上游开关的ID
	UpCBID[0]=0;
	char UnionCBID[MAX_ID_DLL];//上游开关的ID
	UnionCBID[0]=0;
	bool bFindCB=false;

	int devid = -1;
	double seclensum = 0;
	int cntcb = 0;
	EQ* equip;
	double high = 0;
	double maxhigh = 300;//最大高度
	double minhigh = 300;//最低高度
	int firstcb = 0;

	mapW = 900;
	mapH = 300;//基准线
	maxhigh = 300;//最大高度
	minhigh = 300;//最低高度
	double linklen = 0;//小短线的长度
	bool upType = true;//上一个设备属于开关

	double scaleX = 0;
	double scaleCB = 0;

	vector<int> procMainND;
	stru_MainND itMainND;

	int k = 0;
	int vector1[500], vector2[500];
	int ele, pos, farmax = 0, elemax;

	//***************************************************//
	//lld add 2018-12-04 for FishBoneMap
	//（1）主节点：电源点到全部联络开关的全部节点：mainND
	//（2）主设备：由主节点连接成的设备为主设备:mainDev
	//（3）一级设备，电源点到主联络开关的设备
			//如果不设定主联络开关则节点数最多的联络开关为主联络开关
	//（4）二级设备，只有一个节点挂接在主设备上的其它设备
	//（5）n级设备，挂接在n-1个主节点上的其它设备
	//***************************************************//

	//先判断是否有指定主干线
	//设定主干线相对比较简单，没有树形主干线的问题，一条直通线，从设定点到电源点
	double maxlen = 0;
	if (isSetMainND)
	{
		knd = main_iznd;//最后一段主干线末端开始进行追踪，手工只有一条主干线，无分叉
		if (isInMainLine(knd) == -1)
			path_mainline[cntMainLine++].ind = knd;
		//path_mainline[cntMainLine].equip_ind_r = knd;

		//从这个节点向上回溯
		//j=0;
		//int ind,iznd;
		while (1)
		{
			kknd = (nd_t+knd)->bch;
			
			if (isInMainLine(kknd) == -1)
				path_mainline[cntMainLine++].ind = kknd;

			if (equip_tb->GetDataByPh(getEquipID(kknd, knd))->type == 10)
			{
				maxlen += sec_tb->GetDataByPh(equip_tb->GetDataByPh(ind)->idph)->len;
			}
			else if (equip_tb->GetDataByPh(getEquipID(kknd, knd))->type == 20)
			{
			}
			else
				cntcb++;


			//需要累计长度和开关的个数
			//devid = getEquipID(knd, kknd);
			//equip = equip_tb->GetDataByPh(devid);

			//stru_MainDev mDev;
			//initMainDev(mDev);
			//mDev.eq_id = equip->i_id;
			//mDev.type = equip->type;
			//mDev.idph = equip->idph;
			//mDev.ind = equip->ind;
			//mDev.iznd = equip->iznd;
			//strcpy(mDev.id, equip->id);

			//mainDev.push_back(mDev);

			//maindev[cntmaindev++] = devid;//从 后面往前面排

			//if (equip->type & 10)
			//{
			//	seclensum += sec_tb->GetDataByPh(equip->idph)->len;
			//}
			//else if (equip->type == 20)
			//{
			//}
			//else
			//	cntcb++;

			knd = kknd;//得到开关的上级节点号
			if (knd==sond)//回溯到输入节点
			{
				break;
			}
		}

		//计算每个主干线节点上折算过来的全部信息，包括负荷个数，大小等
		OnSetMainLine(path_mainline,cntMainLine,sond);
		ret = fdShape = OnAnalyseShape();
		
		//做主干节点，主联络开关或设定的主干线上的节点
		mainNDList.clear();
		seclensum = 10;
		for (k = 0; k < cntMainLine; k++)//联络个数
		{
			stru_MainND t_mainND;
			strcpy(t_mainND.id, (nd_t + path_mainline[k].ind)->id);
			t_mainND.type = 1;
			t_mainND.ind = path_mainline[k].ind;
			t_mainND.x1 = 0;
			t_mainND.y1 = 0;
			t_mainND.sumLD = path_mainline[k].sumLD;
			strcpy(t_mainND.UpCBID, path_mainline[k].UpCBID);
			strcpy(t_mainND.UnionCBID, path_mainline[k].UnionCBID);

			mainNDList.push_back(t_mainND);
		}

		//从电源点开始做一颗树
		//两头各留10，左-10，右-790 开关占位50，
		scaleCB = 200 / (double)cntcb;//开关最多只200的空间,每个开关的大小
		high = 0.5*scaleCB / 2;	  //需要计算开关高宽比
		scaleX = (mapW - 200) / maxlen;//每米占的像素大小
		linklen = 0.05 * maxlen;
		//seclensum = 10;
		//bool up = true;//先上后下
		mainDevList.clear();

		for (i = 0; i < lv_nd; i++)
		{
			(nd_t + i)->q |= ND_UNPROC;
		}

		int ret2;
		ind = sond;//从电源点开始搜索
		(nd_t + ind)->bch = ind;//从源点开始开始进行,将源点作为上一级记录下来
		if ((ret = isExistMainND(ind)) != -1)
		{
			mainNDList[ret].type = 1;//电源点为一级节点
			mainNDList[ret].x1 = seclensum;
			mainNDList[ret].y1 = mapH;//主干线的全部y都在一个上面
		}
		if ((nd_t + ind)->q & ND_UNPROC) //this so nd_t has not processed
		{
			//拿当前节点向下扩展，找到所有和它相连的节点
			knd = ind;//当前节点赋给一个临时的变量
			(nd_t + knd)->fch = knd; //当前节点号赋给前向链 
			(nd_t + knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

			while (1)//还有扩展开关没有处理
			{
				cb_p = r_nd_cb_tb->FindChildPtr1((nd_t + knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while (cb_p)
				{
					if (knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					else
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;

					if (kkndcb == -1)
					{
						//如果是联络开关，需要加上
						if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)
						{
							//上一个设备是开关设备，增加一个短线,出口开关不加
							if ( (upType) && !(cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_BREAKERFEEDER))
							{
								stru_MainDev mDev;
								initMainDev(mDev);
								//增加一个短线设备
								mDev.type = 16;//短线
								mDev.len = linklen;
								strcpy(mDev.id, "link");//短线

								mDev.x1 = round(seclensum);
								mDev.y1 = mapH;
								seclensum += scaleX * mDev.len;
								mDev.x2 = round(seclensum);
								mDev.y2 = mapH;

								mainDevList.push_back(mDev);
							}
							upType = true;

							stru_MainDev mDev;
							initMainDev(mDev);
							mDev.eq_id = -1;
							mDev.type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;

							mDev.idph = cb_p->Ptr;
							mDev.ind = knd;
							mDev.iznd = kkndcb;
							strcpy(mDev.id, cb_tb->GetDataByPh(cb_p->Ptr)->id);

							mDev.x1 = round(seclensum);
							mDev.y1 = mapH - high;
							seclensum += scaleCB;
							mDev.x2 = round(seclensum);
							mDev.y2 = mapH + high;

							mainDevList.push_back(mDev);
						}

						cb_p = cb_p->Next;
						continue;
					}

					if ((nd_t + kkndcb)->q & ND_UNPROC)
					{
						//这个节点是否在主干上
						if ((ret = isExistMainND(kkndcb)) != -1)
						{
							//上一个设备是开关设备，增加一个短线
							if ((upType) && !(cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_BREAKERFEEDER))
							{
								stru_MainDev mDev;
								initMainDev(mDev);
								//增加一个短线设备
								mDev.type = 16;//短线
								mDev.len = linklen;
								strcpy(mDev.id, "link");//短线

								mDev.x1 = round(seclensum);
								mDev.y1 = mapH;
								seclensum += scaleX * mDev.len;
								mDev.x2 = round(seclensum);
								mDev.y2 = mapH;

								mainDevList.push_back(mDev);
							}
							upType = true;

							stru_MainDev mDev;
							initMainDev(mDev);
							mDev.eq_id = -1;
							mDev.type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;

							mDev.idph = cb_p->Ptr;
							mDev.ind = knd;
							mDev.iznd = kkndcb;
							strcpy(mDev.id, cb_tb->GetDataByPh(cb_p->Ptr)->id);
							if (mainNDList[ret].type == 1)
							{//在主干
								mDev.x1 = round(seclensum);
								mDev.y1 = mapH - high;
								seclensum += scaleCB;
								mDev.x2 = round(seclensum);
								mDev.y2 = mapH + high;

								mainNDList[ret].x1 = mDev.x2;
								mainNDList[ret].y1 = mapH;//主干线的全部y都在一个上面
							}
							mainDevList.push_back(mDev);
						}

						//将这个节点的上一级记录下来，准备回溯
						(nd_t + kkndcb)->bch = knd;

						//将扩展的前向链 设成 找到的新节点的前向链
						(nd_t + kkndcb)->fch = (nd_t + knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd_t + knd)->fch = kkndcb;
						//将找到的新节点设成已处理
						(nd_t + kkndcb)->q &= (~ND_UNPROC);
					}

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//read secs connected to this nd
				sec_p = r_nd_sec_tb->FindChildPtr1((nd_t + knd)->r_sec);
				sec_p0 = sec_p;
				while (sec_p)
				{
					//find the opposite nd_t through this sec
					if (knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
					else
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;

					if (kknd == -1)
					{
						//悬空线路不参与推进
						sec_p = sec_p->Next;
						continue;
					}

					if ((nd_t + kknd)->q & ND_UNPROC)
					{
						//这个节点是否在主干上
						if ((ret = isExistMainND(kknd)) != -1)
						{
							//if (upType)
							upType = false;

							stru_MainDev mDev;
							initMainDev(mDev);
							mDev.eq_id = -1;
							mDev.type = 10;
							mDev.idph = sec_p->Ptr;
							mDev.ind = knd;
							mDev.iznd = kknd;
							mDev.len = sec_tb->GetDataByPh(sec_p->Ptr)->len;
							if (mDev.len == -1)
								mDev.len = 0;
							strcpy(mDev.id, sec_tb->GetDataByPh(sec_p->Ptr)->id);
							if (mainNDList[ret].type == 1)
							{
								mDev.x1 = round(seclensum);
								mDev.y1 = mapH;
								seclensum += scaleX * mDev.len;
								mDev.x2 = round(seclensum);
								mDev.y2 = mapH;

								mainNDList[ret].x1 = mDev.x2;
								mainNDList[ret].y1 = mapH;//主干线的全部y都在一个上面

							}
							mainDevList.push_back(mDev);
						}

						(nd_t + kknd)->fch = (nd_t + knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd_t + knd)->fch = kknd;

						//将这个节点的上一级记录下来，准备回溯
						(nd_t + kknd)->bch = knd;

						//将找到的新节点设成已处理
						(nd_t + kknd)->q &= (~ND_UNPROC);
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);

				//move to next nd，搜索完一层
				knd = (nd_t + knd)->fch;
				//循环到头，连起来了，表示全部走了一遍
				//拿另外的节点再开始搜索，一直到所有的节点都处理
				if (knd == ind)
				{
					break;
				}
			}
		}

/*
		//从电源点开始做一棵树
		for (i = 0; i < lv_nd; i++)
			(nd_t + i)->q |= ND_UNPROC;

		ind = sond;//从电源点开始搜索
		(nd_t + ind)->bch = ind;//从源点开始开始进行,将源点作为上一级记录下来
		if ((ret = isExistMainND(ind)) != -1)
		{
			mainNDList[ret].type = 1;//电源点为一级节点
		}
		if ((nd_t + ind)->q & ND_UNPROC) //this so nd_t has not processed
		{
			//拿当前节点向下扩展，找到所有和它相连的节点
			knd = ind;//当前节点赋给一个临时的变量
			(nd_t + knd)->fch = knd; //当前节点号赋给前向链 
			(nd_t + knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

			while (1)//还有扩展开关没有处理
			{
				//从节点-开关关系表找到所有和这个节点相连的开关
				cb_p = r_nd_cb_tb->FindChildPtr1((nd_t + knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while (cb_p)
				{
					if (knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					else
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;

					if ((nd_t + kkndcb)->q & ND_UNPROC)
					{
						if ((ret = isExistMainND(kkndcb)) != -1)
						{
							stru_MainDev mDev;
							initMainDev(mDev);
							mDev.eq_id = -1;
							mDev.type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;

							if (upType)//上一个设备属于开关
								mDev.type |= 16;
							upType = true;

							mDev.idph = cb_p->Ptr;
							mDev.ind = knd;
							mDev.iznd = kkndcb;
							strcpy(mDev.id, cb_tb->GetDataByPh(cb_p->Ptr)->id);
							cntcb++;//增加一个开关
							mainDevList.push_back(mDev);
						}

						(nd_t + kkndcb)->bch = knd;
						(nd_t + kkndcb)->fch = (nd_t + knd)->fch;
						(nd_t + knd)->fch = kkndcb;
						(nd_t + kkndcb)->q &= (~ND_UNPROC);
					}

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				sec_p = r_nd_sec_tb->FindChildPtr1((nd_t + knd)->r_sec);
				sec_p0 = sec_p;
				while (sec_p)
				{
					if (knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
					else
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;

					if ((nd_t + kknd)->q & ND_UNPROC)
					{
						if ((ret = isExistMainND(kknd)) != -1)
						{
							upType = false;

							stru_MainDev mDev;
							initMainDev(mDev);
							mDev.eq_id = -1;
							mDev.type = 10;
							mDev.idph = sec_p->Ptr;
							mDev.ind = knd;
							mDev.iznd = kknd;
							strcpy(mDev.id, sec_tb->GetDataByPh(sec_p->Ptr)->id);
							mDev.len = sec_tb->GetDataByPh(sec_p->Ptr)->len;
							seclensum += mDev.len;
							mainDevList.push_back(mDev);
						}

						(nd_t + kknd)->fch = (nd_t + knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd_t + knd)->fch = kknd;

						//将这个节点的上一级记录下来，准备回溯
						(nd_t + kknd)->bch = knd;

						//将找到的新节点设成已处理
						(nd_t + kknd)->q &= (~ND_UNPROC);
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);

				//move to next nd，搜索完一层
				knd = (nd_t + knd)->fch;
				//循环到头，连起来了，表示全部走了一遍
				//拿另外的节点再开始搜索，一直到所有的节点都处理
				if (knd == ind)
					break;
			}
		}

		//根据累计长度和开关的个数计算每个开关、每米的占位
		//两头各留10，左-10，右-790 开关占位50，
		scaleCB = 200 / (double)cntcb;//开关最多只200的空间,每个开关的大小
   	    //需要计算高宽比
		high = 0.5*scaleCB / 2;//只有一条线
		scaleX = (mapW-200)/seclensum;//每米占的像素大小
		
		//直接从电源点开始输出
		seclensum = 10;
		for (i = 0; i < mainDevList.size(); i++)
		{
			//做设备的2个节点
			knd = mainDevList.at(i).ind;
			kknd = mainDevList.at(i).iznd;
			if ( (mainDevList[i].type == 10) && (mainDevList[i].len>0))
			{
				mainDevList[i].x1 = round(seclensum);
				mainDevList[i].y1 = mapH;
				if ((ret = isExistMainND(knd)) != -1)
				{
					mainNDList[ret].x1 = round(seclensum);
					mainNDList[ret].y1 = mapH;
				}

				seclensum += mainDevList[i].len;
				mainDevList[i].x2 = round(seclensum);
				mainDevList[i].y2 = mapH;
				if ((ret = isExistMainND(kknd)) != -1)
				{
					mainNDList[ret].x1 = round(seclensum);
					mainNDList[ret].y1 = mapH;
				}
			}
			else
			{
				//0 - breaker；1 - loadbreakswitch；2 - disconnector；3 - fuse
				if ((mainDevList[i].type == 0)
					|| (mainDevList[i].type == 1)
					|| (mainDevList[i].type == 2)
					|| (mainDevList[i].type == 3))
				{
					mainDevList.at(i).x1 = round(seclensum);
					mainDevList.at(i).y1 = mapH - high;
					if ((mapH - high) < minhigh)
						minhigh = mapH - high;
					if ((ret = isExistMainND(knd)) != -1)
					{
						mainNDList[ret].x1 = round(seclensum);
						mainNDList[ret].y1 = mapH - high;
					}
					seclensum += scaleCB;
					mainDevList.at(i).x2 = round(seclensum);
					mainDevList.at(i).y2 = mapH + high;
					if ((mapH + high) > minhigh)
						minhigh = mapH + high;
					if ((ret = isExistMainND(kknd)) != -1)
					{
						mainNDList[ret].x1 = round(seclensum);
						mainNDList[ret].y1 = mapH + high;
					}
				}
			}
		}
		*/
	}
	else
	{
		//lld add for tree mainline 2018-3-22
		//每个联络开关最小路去和别的最小路对比，找分叉点，然后从这个分叉点回溯	

		if (cntunion > 0)
		{
			//1.先分析形态
			for (i = 0;i < 50;i++)
				for (j = 0;j < 500;j++)
					unioncb[i][j] = -1;

			//get all uninpath
			int maxlen = 0;
			int maxindex = 0;

			for (i = 0; i < cntunion; i++)//联络个数
			{
				knd = path_union[i].ind;//得到联络的节点

				//从这个节点向上回溯
				j = 0;
				while (1)
				{
					kknd = (nd_t + knd)->bch;

					if ((kknd == knd) || (j >= 199))
						break;

					//把每个ind记录下来就是最小路的轨迹
					unioncb[i][j] = knd;
					path_union[i].mainpath[j++] = knd;

					knd = kknd;//得到开关的上级节点号
					if (knd == sond)//回溯到输入节点
					{
						unioncb[i][j] = knd;
						path_union[i].mainpath[j++] = knd;
						break;
					}
				}
				path_union[i].pathlen = j;//联络开关到电源点的最小路长度
				if (j > maxlen)
				{
					maxindex = i;
					maxlen = j;
				}
			}
			//
			for (i = 0; i < maxlen;i++)//主干线上的节点
			{
				path_mainline[i].ind = path_union[maxindex].mainpath[i];
			}
			cntMainLine = maxlen;
			OnSetMainLine(path_mainline, cntMainLine, sond);
			ret = fdShape = this->OnAnalyseShape();//分析最长的链路特征

			cntMainLine = 0;
			for (i = 0; i < maxMainND;i++)//主干线上的节点
			{
				path_mainline[i].ind = -1;
				path_mainline[i].equip_id_l[0] = 0;//左边设备，靠近电源点的设备
				path_mainline[i].equip_id_r[0] = 0;
				path_mainline[i].equip_ind_l = -1;
				path_mainline[i].equip_ind_r = -1;
				path_mainline[i].sumLD = 0;
				path_mainline[i].sumP = 0;
				path_mainline[i].sumQ = 0;
				path_mainline[i].UpCBID[0] = 0;//左边设备，靠近电源点的设备
				path_mainline[i].len = 0;
				path_mainline[i].UnionCBID[0] = 0;
				path_mainline[i].ldList[0] = 0;
				path_mainline[i].isValid = 1;//主节点默认都是有效的

				path_mainline1[i].ind = -1;
				path_mainline1[i].equip_id_l[0] = 0;//左边设备，靠近电源点的设备
				path_mainline1[i].equip_id_r[0] = 0;
				path_mainline1[i].equip_ind_l = -1;
				path_mainline1[i].equip_ind_r = -1;
				path_mainline1[i].sumLD = 0;
				path_mainline1[i].sumP = 0;
				path_mainline1[i].sumQ = 0;
				path_mainline1[i].UpCBID[0] = 0;//左边设备，靠近电源点的设备
				path_mainline1[i].len = 0;
				path_mainline1[i].UnionCBID[0] = 0;
				path_mainline1[i].ldList[0] = 0;
				path_mainline1[i].isValid = 1;//主节点默认都是有效的
			}

			//全部的链路进行累加，里面有很多重复的节点
			for (i = 0; i < cntunion; i++)//联络个数
			{
				for (j = 0; j < path_union[i].pathlen; j++)//联络个数
					path_mainline1[mainndmax++].ind = path_union[i].mainpath[j];
			}

			//将分支的负荷信息折算到主干线上
			OnSetMainLine(path_mainline1, mainndmax, sond);

			//////////////////////////////////////////////////////
			for (j = 0;j < 500;j++)
			{
				vector1[j] = -1;
				vector2[j] = -1;
			}
			for (i = 0; i < cntunion; i++)//联络个数
			{
				/////////////////////查看是否联络开关单纯做联络，上面没挂负荷
				farmax = 0;
				//1.先找到和其它联络最小路的最远交叉点
				for (k = 0; k < 500; k++)//联络个数
					vector1[k] = unioncb[i][k];

				//跟每一个其它联络开关路径比较，获取分叉点
				for (j = 0; j < cntunion; j++)//联络个数
				{
					if (j == i)
						continue;//是自己，跳过

					for (k = 0; k < 500; k++)//联络个数
						vector2[k] = unioncb[j][k];

					pos = comp_vector(vector1, vector2, 500, ele, true);
					if (pos > farmax)
					{
						farmax = pos;
						elemax = ele;//最远的分叉点
					}
				}

				bool bFindLD = false;

				//单纯回溯，看是否没有负荷点
				knd = path_union[i].ind;//得到联络的节点

				//判断是否负荷节点
				for (k = 0; k < mainndmax; k++)//联络个数
				{
					if ((path_mainline1[k].ind == knd) && (path_mainline1[k].sumLD > 0))
					{
						bFindLD = true;
						break;
					}
				}

				if (elemax != knd)
				{
					//从这个节点向上回溯
					j = 0;
					while (1)
					{
						j++;
						if (j > 1000)
							break;

						kknd = (nd_t + knd)->bch;

						if (kknd == elemax)
							break;//到了分叉点

						//判断是否负荷节点
						for (k = 0; k < mainndmax; k++)//
						{
							if ((path_mainline1[k].ind == kknd) && (path_mainline1[k].sumLD > 0))
							{
								bFindLD = true;
								break;
							}
						}
						knd = kknd;//得到开关的上级节点号
						if (knd == sond)//回溯到输入节点
						{
							break;
						}
					}
				}

				//从这个节点向上回溯，到最小的交叉点后，直接上去
				if (!bFindLD)//联络开关前没有负荷，直接把联络开关挂上去
				{
					knd = elemax;

					for (int l = 0; l < mainndmax;l++)
					{
						if (path_mainline1[l].ind == knd)
						{
							strcpy(path_mainline1[l].UnionCBID, path_union[i].id);
							break;
						}
					}
				}
				else
				{
					knd = path_union[i].ind;//得到联络的节点
					for (int l = 0; l < mainndmax;l++)
					{
						if (path_mainline1[l].ind == knd)
						{
							strcpy(path_mainline1[l].UnionCBID, path_union[i].id);
							break;
						}
					}
				}

				////////////////////查看是否联络开关单纯做联络，上面没挂负荷
				if (cntMainLine == 0)
				{
					//联络开关节点也有可能挂负荷
					for (k = 0; k < mainndmax; k++)
					{
						if (path_mainline1[k].ind == knd)
						{
							path_mainline[cntMainLine].sumLD = path_mainline1[k].sumLD;
							path_mainline[cntMainLine].sumP = path_mainline1[k].sumP;
							path_mainline[cntMainLine].sumQ = path_mainline1[k].sumQ;
							strcpy(path_mainline[cntMainLine].ldList, path_mainline1[k].ldList);
							strcpy(path_mainline[cntMainLine].UpCBID, path_mainline1[k].UpCBID);
							strcpy(path_mainline[cntMainLine].UnionCBID, path_mainline1[k].UnionCBID);
							break;
						}
					}

					path_mainline[cntMainLine++].ind = knd;
				}
				//联络开关的节点不在主干线上
				int ret1 = isInMainLine(knd);
				if (ret1 == -1)
				{
					//联络开关节点也有可能挂负荷
					for (k = 0; k < mainndmax; k++)//联络个数
					{
						if (path_mainline1[k].ind == knd)
						{
							path_mainline[cntMainLine].sumLD = path_mainline1[k].sumLD;
							path_mainline[cntMainLine].sumP = path_mainline1[k].sumP;
							path_mainline[cntMainLine].sumQ = path_mainline1[k].sumQ;
							strcpy(path_mainline[cntMainLine].ldList, path_mainline1[k].ldList);
							strcpy(path_mainline[cntMainLine].UpCBID, path_mainline1[k].UpCBID);
							strcpy(path_mainline[cntMainLine].UnionCBID, path_mainline1[k].UnionCBID);
							break;
						}
					}

					path_mainline[cntMainLine++].ind = knd;
				}
				else
				{
					for (k = 0; k < mainndmax; k++)//联络个数
					{
						if (path_mainline1[k].ind == knd)
						{
							path_mainline[ret1].sumLD = path_mainline1[k].sumLD;
							path_mainline[ret1].sumP = path_mainline1[k].sumP;
							path_mainline[ret1].sumQ = path_mainline1[k].sumQ;
							strcpy(path_mainline[ret1].ldList, path_mainline1[k].ldList);
							strcpy(path_mainline[ret1].UpCBID, path_mainline1[k].UpCBID);
							strcpy(path_mainline[ret1].UnionCBID, path_mainline1[k].UnionCBID);
							break;
						}
					}
				}

				//从这个节点向上回溯
				j = 0;
				while (1)
				{
					if (j++ >= 200)
						break;

					kknd = (nd_t + knd)->bch;
					if (isInMainLine(kknd) == -1)//主干线不包括这个节点，加到主干线集合
					{
						//联络开关节点也有可能挂负荷
						for (k = 0; k < mainndmax; k++)//联络个数
						{
							if (path_mainline1[k].ind == kknd)
							{
								path_mainline[cntMainLine].sumLD = path_mainline1[k].sumLD;
								path_mainline[cntMainLine].sumP = path_mainline1[k].sumP;
								path_mainline[cntMainLine].sumQ = path_mainline1[k].sumQ;
								strcpy(path_mainline[cntMainLine].ldList, path_mainline1[k].ldList);
								strcpy(path_mainline[cntMainLine].UpCBID, path_mainline1[k].UpCBID);
								strcpy(path_mainline[cntMainLine].UnionCBID, path_mainline1[k].UnionCBID);
								break;
							}
						}

						path_mainline[cntMainLine++].ind = kknd;
					}
					knd = kknd;//得到开关的上级节点号
					if (knd == sond)//回溯到输入节点
					{
						//path_union[i].mainpath[j++]=knd;
						break;
					}
				}
			}

			//lld add 2018-11-22 for fishbonemap
			//全部的主干线节点都填充到了path_mainline[cntMainLine++]里面
			for (k = 0; k < cntMainLine; k++)//联络个数
			{
				stru_MainND t_mainND;
				strcpy(t_mainND.id, (nd_t + path_mainline[k].ind)->id);
				t_mainND.type = -1;
				t_mainND.nd_up = true;//默认都在上
				t_mainND.nd_mapH = mapH;//

				t_mainND.ind = path_mainline[k].ind;
				t_mainND.x1 = 0;
				t_mainND.y1 = 0;
				t_mainND.sumLD = path_mainline[k].sumLD;
				strcpy(t_mainND.UpCBID, path_mainline[k].UpCBID);
				strcpy(t_mainND.UnionCBID, path_mainline[k].UnionCBID);

				mainNDList.push_back(t_mainND);
			}

			int perpath_1[50];
			int perpath_2[50];
			int perpathlen = 0;
			for (k = 0;k < cntunion;k++)
				perpath_1[k] = perpath_2[k] = path_union[k].pathlen;

			//对perpath_1[k]进行排序
			sort(perpath_2, perpath_2 + cntunion);

			double maxLength = 0;
			int maxTie, maxCBlen;
			cntcb = 0;
			int nd_type = 1;
			for (i = cntunion - 1;i >= 0;i--)
			{
				perpathlen = perpath_2[i];
				for (k = 0;k < cntunion;k++)
					if (path_union[k].pathlen == perpathlen)
					{
						if (i == cntunion - 1)
							maxTie = i;//主干

						//给该联络的全路径给一个值
						for (j = 0;j < perpathlen;j++)
							if (((ret = isExistMainND(path_union[k].mainpath[j])) != -1)
								&& (mainNDList[ret].type == -1))
								mainNDList[ret].type = nd_type;
						break;
					}
				nd_type++;
			}

			j == 0;
			maxLength = 0;
			ind = knd = path_union[maxTie].ind;
			while (1)
			{
				if (j++ >= 200)
					break;

				kknd = (nd_t + knd)->bch;
				int i_id = getEquipID(kknd, knd);
				if (i_id == -1)
				{
					knd = kknd;//得到开关的上级节点号
					continue;
				}
				if (equip_tb->GetDataByPh(i_id)->type == 10)
				{
					double lld = sec_tb->GetDataByPh(equip_tb->GetDataByPh(i_id)->idph)->len;
					maxLength += sec_tb->GetDataByPh(equip_tb->GetDataByPh(i_id)->idph)->len;
				}
				else if (equip_tb->GetDataByPh(getEquipID(kknd, knd))->type == 20)
				{
				}
				else
					cntcb++;

				knd = kknd;//得到开关的上级节点号
				if (knd == sond)//回溯到输入节点
				{
					//path_union[i].mainpath[j++]=knd;
					break;
				}
			}
			maxCBlen = cntcb;

			///////////////////////////////////////////////////////////////////////////////////
					//从电源点开始做一颗树
					//两头各留10，左-10，右-790 开关占位50，
			scaleCB = 500 / (double)maxCBlen;//开关最多只200的空间,每个开关的大小
			high = 0.5*scaleCB / 2;	  //需要计算开关高宽比
			scaleX = (mapW - 500) / maxLength;//每米占的像素大小
			seclensum = 10;
			int linklen = 100;//小短线的长度（米），非像素
			int ret2;//上级节点位置
			int rety;//上级节点的y值
			int extndlist[50];//一个节点最多扩展50个节点
			int extnds = 0;//扩展节点数

			bool up = true;//先上后下

			//先把每个主节点的层次划分出来，只做节点
			for (i = 0; i < lv_nd; i++)
				(nd_t + i)->q |= ND_UNPROC;

			ind = sond;//从电源点开始搜索
			(nd_t + ind)->bch = ind;//从源点开始开始进行,将源点作为上一级记录下来
			if ((nd_t + ind)->q & ND_UNPROC) //this so nd_t has not processed
			{
				//拿当前节点向下扩展，找到所有和它相连的节点
				knd = ind;//当前节点赋给一个临时的变量
				(nd_t + knd)->fch = knd; //当前节点号赋给前向链 
				(nd_t + knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

				while (1)//还有扩展开关没有处理
				{
					extnds = 0;
					ret2 = isExistMainND(knd);

					cb_p = r_nd_cb_tb->FindChildPtr1((nd_t + knd)->r_cb);
					cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
					while (cb_p)
					{
						if (knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
							kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
						else
							kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;

						if ((kkndcb == -1)
							|| (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB))
						{
							//extnds++;

							cb_p = cb_p->Next;
							continue;
						}

						if ((nd_t + kkndcb)->q & ND_UNPROC)
						{
							if ((ret = isExistMainND(kkndcb)) != -1)
								extndlist[extnds++] = ret;

							(nd_t + kkndcb)->fch = (nd_t + knd)->fch;
							(nd_t + knd)->fch = kkndcb;
							(nd_t + kkndcb)->q &= (~ND_UNPROC);
						}

						cb_p = cb_p->Next;
					}
					::FreeChildPtr(cb_p0);

					//read secs connected to this nd
					sec_p = r_nd_sec_tb->FindChildPtr1((nd_t + knd)->r_sec);
					sec_p0 = sec_p;
					while (sec_p)
					{
						//find the opposite nd_t through this sec
						if (knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
							kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
						else
							kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;

						if (kknd == -1)
						{
							//悬空线路不参与推进
							sec_p = sec_p->Next;
							continue;
						}

						if ((nd_t + kknd)->q & ND_UNPROC)
						{
							if ((ret = isExistMainND(kknd)) != -1)
								extndlist[extnds++] = ret;

							(nd_t + kknd)->fch = (nd_t + knd)->fch;
							(nd_t + knd)->fch = kknd;
							(nd_t + kknd)->q &= (~ND_UNPROC);
						}
						sec_p = sec_p->Next;
					}
					::FreeChildPtr(sec_p0);

					if (extnds >= 2)//说明有扩展节点，需要上下进行扩位
					{
						for (i = 0;i < extnds;i++)
						{
							if (mainNDList[extndlist[i]].type == mainNDList[ret2].type)
								continue;//和源节点一个等级

							mainNDList[extndlist[i]].type = mainNDList[ret2].type + 1;
							if (mainNDList[extndlist[i]].type == 2)
							{
								if (up)
									mainNDList[extndlist[i]].nd_up = true;
								else
									mainNDList[extndlist[i]].nd_up = false;
								up = !up;
							}
							else
							{
								mainNDList[extndlist[i]].nd_up = mainNDList[ret2].nd_up;
							}
						}
					}
					else
					{
						for (i = 0;i < extnds;i++)
							mainNDList[extndlist[i]].type = mainNDList[ret2].type;
					}

					//move to next nd，搜索完一层
					knd = (nd_t + knd)->fch;
					//循环到头，连起来了，表示全部走了一遍
					//拿另外的节点再开始搜索，一直到所有的节点都处理
					if (knd == ind)
					{
						break;
					}
				}
			}

			///////////////////////////////////////////////////////////////////////////
					//正式开始写位置信息
			mainDevList.clear();
			for (i = 0; i < lv_nd; i++)
				(nd_t + i)->q |= ND_UNPROC;

			ind = sond;//从电源点开始搜索
			(nd_t + ind)->bch = ind;//从源点开始开始进行,将源点作为上一级记录下来
			if ((ret = isExistMainND(ind)) != -1)
			{
				mainNDList[ret].x1 = round(seclensum);
				mainNDList[ret].y1 = mapH;//电源点基准高度
			}

			if ((nd_t + ind)->q & ND_UNPROC) //this so nd_t has not processed
			{
				//拿当前节点向下扩展，找到所有和它相连的节点
				knd = ind;//当前节点赋给一个临时的变量
				(nd_t + knd)->fch = knd; //当前节点号赋给前向链 
				(nd_t + knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

				while (1)//还有扩展开关没有处理
				{
					//得到上一级的节点的位置信息
					ret2 = isExistMainND(knd);
					extnds = 0;

					cb_p = r_nd_cb_tb->FindChildPtr1((nd_t + knd)->r_cb);
					cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
					while (cb_p)
					{
						if (knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
							kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
						else
							kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;

						//如果是联络开关，需要加上，联络开关的y值需要计算
						if ((kkndcb == -1)
							&&
							(cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB))
						{
							//上一个设备是开关设备，增加一个短线
							stru_MainDev mDev;
							initMainDev(mDev);

							stru_MainDev mDev_;
							if (upType)
							{
								initMainDev(mDev_);
								//增加一个短线设备
								mDev_.type = 16;//短线
								mDev_.len = linklen;
								strcpy(mDev_.id, "link");//短线

								mDev_.x1 = mainNDList[ret2].x1;
								mDev_.y1 = mainNDList[ret2].y1;
								mDev_.x2 = mDev_.x1 + round(scaleX * mDev_.len);
								mDev_.y2 = mainNDList[ret2].y1;

								mainDevList.push_back(mDev_);

								mDev.x1 = mDev_.x2;
							}
							else
								mDev.x1 = mainNDList[ret2].x1;

							upType = true;

							mDev.eq_id = -1;
							mDev.type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;

							mDev.idph = cb_p->Ptr;
							mDev.ind = knd;
							mDev.iznd = kkndcb;
							strcpy(mDev.id, cb_tb->GetDataByPh(cb_p->Ptr)->id);

							mDev.y1 = mainNDList[ret2].y1 - high;
							mDev.x2 = mDev.x1 + round(scaleCB);
							mDev.y2 = mainNDList[ret2].y1 + high;

							mainDevList.push_back(mDev);

							cb_p = cb_p->Next;
							continue;
						}

						if ((nd_t + kkndcb)->q & ND_UNPROC)
						{
							//这个节点是否在主干上，主要是找位置
							if ((ret = isExistMainND(kkndcb)) != -1)
							{
								stru_MainDev mDev;
								initMainDev(mDev);
								//上一个设备是开关设备，增加一个短线
								stru_MainDev mDev_;
								if (upType)
								{
									initMainDev(mDev_);
									//增加一个短线设备
									mDev_.type = 16;//短线
									mDev_.len = linklen;
									strcpy(mDev_.id, "link");//短线

									mDev_.x1 = mainNDList[ret2].x1;
									mDev_.y1 = mainNDList[ret2].y1;
									mDev_.x2 = mDev_.x1 + round(scaleX * mDev_.len);
									mDev_.y2 = mainNDList[ret2].y1;

									mainDevList.push_back(mDev_);

									mDev.x1 = mDev_.x2;
								}
								else
									mDev.x1 = mainNDList[ret2].x1;
								upType = true;

								mDev.eq_id = -1;
								mDev.type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;

								mDev.idph = cb_p->Ptr;
								mDev.ind = knd;
								mDev.iznd = kkndcb;
								strcpy(mDev.id, cb_tb->GetDataByPh(cb_p->Ptr)->id);
								if (mainNDList[ret].type == mainNDList[ret2].type)
								{
									mDev.y1 = mainNDList[ret2].y1 - high;
									mDev.x2 = mDev.x1 + round(scaleCB);
									mDev.y2 = mainNDList[ret2].y1 + high;

									mainNDList[ret].x1 = mDev.x2;
									mainNDList[ret].y1 = mainNDList[ret2].y1;//主干线的全部y都在一个上面
								}
								else//不在主干线上，开辟新的位置，交错开
								{
									//必须比扩展节点低一级才能进行累加
									if (mainNDList[ret].nd_up)
									{
										rety = mainNDList[ret2].y1 - 120;
										if (rety < minhigh)
											minhigh = rety;
									}
									else
									{
										rety = mainNDList[ret2].y1 + 120;
										if (rety > maxhigh)
											maxhigh = rety;
									}

									//补一条竖线
									stru_MainDev mDev_;
									initMainDev(mDev_);
									//增加一个短线设备
									mDev_.type = 16;//短线
									mDev_.len = 0;
									strcpy(mDev_.id, "link");//短线

									mDev_.x1 = mainNDList[ret2].x1;
									mDev_.y1 = mainNDList[ret2].y1;//上一个节点y值
									mDev_.x2 = mainNDList[ret2].x1;
									mDev_.y2 = rety;//修改后的y值
									mainDevList.push_back(mDev_);

									mDev.y1 = rety - high;
									mDev.x2 = mDev.x1 + round(scaleCB);
									mDev.y2 = rety + high;

									mainNDList[ret].x1 = mDev.x2;
									mainNDList[ret].y1 = rety;//主干线的全部y都在一个上面
								}
								mainDevList.push_back(mDev);
							}

							//将这个节点的上一级记录下来，准备回溯
							(nd_t + kkndcb)->bch = knd;

							//将扩展的前向链 设成 找到的新节点的前向链
							(nd_t + kkndcb)->fch = (nd_t + knd)->fch;
							//新节点 设成 扩展节点的前向链
							(nd_t + knd)->fch = kkndcb;
							//将找到的新节点设成已处理
							(nd_t + kkndcb)->q &= (~ND_UNPROC);
						}

						cb_p = cb_p->Next;
					}
					::FreeChildPtr(cb_p0);

					sec_p = r_nd_sec_tb->FindChildPtr1((nd_t + knd)->r_sec);
					sec_p0 = sec_p;
					while (sec_p)
					{
						if (knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
							kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
						else
							kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;

						if (kknd == -1)
						{
							//悬空线路不参与推进
							sec_p = sec_p->Next;
							continue;
						}

						if ((nd_t + kknd)->q & ND_UNPROC)
						{
							//if (strcmp(sec_tb->GetDataByPh(sec_p->Ptr)->id, "5-6") == 0)
							//{
							//	int debug = 0;
							//}
							//这个节点是否在主干上
							if ((ret = isExistMainND(kknd)) != -1)
							{
								upType = false;

								stru_MainDev mDev;
								initMainDev(mDev);
								mDev.eq_id = -1;
								mDev.type = 10;
								mDev.idph = sec_p->Ptr;
								mDev.ind = knd;
								mDev.iznd = kknd;
								mDev.len = sec_tb->GetDataByPh(sec_p->Ptr)->len;
								strcpy(mDev.id, sec_tb->GetDataByPh(sec_p->Ptr)->id);
								if (mainNDList[ret].type == mainNDList[ret2].type)
								{
									mDev.x1 = mainNDList[ret2].x1;
									mDev.y1 = mainNDList[ret2].y1;//上一个节点y值
									mDev.x2 = mDev.x1 + scaleX * mDev.len;
									mDev.y2 = mDev.y1;//修改后的y值

									mainNDList[ret].x1 = mDev.x2;
									mainNDList[ret].y1 = mDev.y2;//主干线的全部y都在一个上面
								}
								else//不在主干线上，开辟新的位置，交错开
								{
									//必须比扩展节点低一级才能进行累加
									if (mainNDList[ret].nd_up)
									{
										rety = mainNDList[ret2].y1 - 120;
										if (rety < minhigh)
											minhigh = rety;
									}
									else
									{
										rety = mainNDList[ret2].y1 + 120;
										if (rety > maxhigh)
											maxhigh = rety;
									}

									//补一条竖线
									stru_MainDev mDev_;
									initMainDev(mDev_);
									//增加一个短线设备
									mDev_.type = 16;//短线
									mDev_.len = 0;
									strcpy(mDev_.id, "link");//短线

									mDev_.x1 = mainNDList[ret2].x1;
									mDev_.y1 = mainNDList[ret2].y1;//上一个节点y值
									mDev_.x2 = mDev_.x1;
									mDev_.y2 = rety;//修改后的y值
									mainDevList.push_back(mDev_);

									mDev.x1 = mainNDList[ret2].x1;
									mDev.y1 = rety;
									mDev.x2 = mDev.x1 + scaleX * mDev.len;
									mDev.y2 = rety;

									mainNDList[ret].x1 = mDev.x2;
									mainNDList[ret].y1 = rety;//主干线的全部y都在一个上面
								}
								mainDevList.push_back(mDev);
							}

							(nd_t + kknd)->fch = (nd_t + knd)->fch;
							//新节点 设成 扩展节点的前向链
							(nd_t + knd)->fch = kknd;

							//将这个节点的上一级记录下来，准备回溯
							(nd_t + kknd)->bch = knd;

							//将找到的新节点设成已处理
							(nd_t + kknd)->q &= (~ND_UNPROC);
						}
						sec_p = sec_p->Next;
					}
					::FreeChildPtr(sec_p0);

					//move to next nd，搜索完一层
					knd = (nd_t + knd)->fch;
					//循环到头，连起来了，表示全部走了一遍
					//拿另外的节点再开始搜索，一直到所有的节点都处理
					if (knd == ind)
					{
						break;
					}
				}
			}
		}
	}

	if ( (!isSetMainND) && (cntunion==0))
		return -1;

	mapW = 0;
	for (i = 0; i < mainDevList.size(); i++)
	{//最远的联络开关的x值为最大的长度
		if ( (mainDevList[i].type == 6) || (mainDevList[i].type == 4))
		{
			if (mapW < mainDevList[i].x2)
				mapW = mainDevList[i].x2;
		}
	}

	deltaH = minhigh-120;//所有的高度都要减去这个数
	mapH = maxhigh + 120;

	double ret_p, ret_q;
	ret_p = ret_q = 0;
	char ldList[MAX_MIANND_LD];

	char id[64];
	ofstream outfile(mainline, ios::out);
	outfile << "<!System= Version=1.0 Code=UTF-8 Data=!>" << endl;

	//规范化计算结果

	outfile << "<map::w_h name" << "\"页面\"" << ">" << endl;
	outfile << "@	width	highlevel	minH	maxH" << endl;
	outfile << "/@	宽度	基准高度	最小高度	最大高度" << endl;
	outfile << "# " << mapW	<< "\t" << mapH << "\t" << minhigh << "\t"<< maxhigh << endl;
	outfile << "</map::w_h>" << endl;
	outfile << "" << endl;//空一格

	outfile << "<cb::mainnd name" << "\"开关\""<<">" << endl;
	outfile << "@	cb_id	x1	y1	x2	y2	type" << endl;
	outfile << "/@	开关编号	x1	y1	x2	y2 类别" << endl;
	for (i = 0; i < mainDevList.size(); i++)
	{
		if ((mainDevList[i].type == 0)
			|| (mainDevList[i].type == 6)//最远联络开关
			|| (mainDevList[i].type == 5)
			|| (mainDevList[i].type == 1)
			|| (mainDevList[i].type == 2)
			|| (mainDevList[i].type == 3)
			|| (mainDevList[i].type == 4)//union
			)
			outfile << "# " << mainDevList[i].id
			<<"\t"<<mainDevList[i].x1 << "\t" << mainDevList[i].y1- deltaH << "\t"
			<< mainDevList[i].x2 << "\t" << mainDevList[i].y2- deltaH << "\t"
			<< mainDevList[i].type <<"\n";
	}
	outfile << "</cb::mainnd>" << endl;

	char sLine[1000];
	outfile <<""<<endl;//空一格
	outfile << "<sec::mainnd name" << "\"线段\"" << ">" << endl;
	outfile << "@	sec_id	x1	y1	x2	y2	type" << endl;
	outfile << "/@	线段编号	x1	y1	x2	y2 类别" << endl;
	for (i = 0; i < mainDevList.size(); i++)
	{
		//
		sprintf(sLine, "# %s\t%d\t%d\t%d\t%d\t%d\t", mainDevList[i].id,
			mainDevList[i].x1, mainDevList[i].y1 - deltaH,
			mainDevList[i].x2, mainDevList[i].y2 - deltaH, mainDevList[i].type);
		if (((mainDevList[i].type == 10) || (mainDevList[i].type == 16)))
			outfile << sLine << endl;
			//outfile << "# " << mainDevList[i].id
			//<<"\t"<< mainDevList[i].x1 <<"\t"<<mainDevList[i].y1<<"\t"<< mainDevList[i].x2 << "\t" << mainDevList[i].y2 << "\t"<< mainDevList[i].type<<"\n";

	}
	outfile << "</sec::mainnd>" << endl;

	outfile << "" << endl;//空一格
	outfile << "<nd::mainnd name" << "\"节点\"" << ">" << endl;
	outfile << "@  nd_id	type     x1	y1 sumLD	UpCBID	    UnionCBID" << endl;
	outfile << "/@ 节点编号 类别(级) x1 y1 负荷个数 分支开关id	联络开关id" << endl;
	for (i = 0; i < mainNDList.size(); i++)
	{
		//只有负荷个数大于0的节点才显示在鱼骨图上
		if (mainNDList[i].sumLD > 0)
		{
			if (strlen(mainNDList[i].UpCBID) == 0)
				strcpy(mainNDList[i].UpCBID,"-");
			if (strlen(mainNDList[i].UnionCBID) == 0)
				strcpy(mainNDList[i].UnionCBID, "-");
			outfile << "# " << mainNDList[i].id << "\t"<<mainNDList[i].type
				<< "\t" << mainNDList[i].x1 << "\t" << mainNDList[i].y1 - deltaH
				<< "\t" << mainNDList[i].sumLD << "\t" << mainNDList[i].UpCBID
				<< "\t" << mainNDList[i].UnionCBID << "\n";
		}
	}
	outfile << "</nd::mainnd>" << endl;
	outfile.close();


	//outfile << "id    " << "   ldcount" << "    sumP" << "   " << "   ldList" << "  UnionCBID" << endl;

	ldList[0] = 0;
	for (i = 0; i < cntMainLine; i++)
	{
		//主节点上挂有负荷
		if (strlen(path_mainline[i].ldList) != 0)
		{
			strcpy(ldList, path_mainline[i].ldList);
			if (strlen(path_mainline[i].UpCBID) != 0)
			{
				strcat(ldList, "cbid=");
				strcat(ldList, path_mainline[i].UpCBID);
			}
			else
				//整理负荷列表字符串
				lld->delstrright(ldList, 1);

			strcpy(path_mainline[i].ldList, ldList);
		}

		strcpy(id, nd_tb->GetDataByPh(path_mainline[i].ind)->id);
//		outfile << id << "   -" << path_mainline[i].sumLD << "   -" << path_mainline[i].sumP << "   -" << path_mainline[i].sumQ << "   -" << path_mainline[i].ldList << "   -" << path_mainline[i].UnionCBID << endl;
	}

	//根据总的长度寻找分段开关的点，理论上是分三段
	//负荷个数
	cntLD = ret_ld;
	sumLen = len;
	sumP = ret_p;

	nd_tb->UpdateTable(nd_t);
	delete[] nd_t;
	nd_t = 0;

	//t_buildMinPath += lld->getCurMi() - t_begin;
	return 1;
}
//
//bool optSect::findx(stru_MainND &task)
//{
//	return task.ind == find_ind;
//}

int optSect::isExistMainND(int knd)
{
	int rr=-1;
	//find_ind = knd;
	//vector<stru_MainND>::iterator it = find_if(mainND.begin(), mainND.end(), findx);
	for (int ii = 0; ii < mainNDList.size();ii++)
	{
		if (mainNDList[ii].ind == knd)
		{
			return ii;
		}
	}
	

	//auto itrFind = find_if(mainND.begin(), mainND.end(), [](stru_MainND myStruct)
	//{
	//	return myStruct.ind == knd;
	//});

	//if (itrFind != mainND.end())
	//else

	
	//if (it != mainND.end())//找到了
	//{
	//	rr = distance(mainND.begin(), it);
	//}

	return rr;
}


//给定一个节点，左右节点，查找其带的全部折算负荷
int optSect::OnGetNDSumPQ(int main_ind,int l,int r,double& sumP,double& sumQ,char* ldList)
{
	int			i;
	int			ind, knd, kknd, kkndcb;

	bool isForkCB=false;//是否存在分支线开关
	char cbID[64]; 

	double p,q;//功率
	p=q=0;
	cntload =0;
	ldList[0] = 0;

	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;

	//read all nds into buffer
	lv_nd = nd_tb->GetCount();
	ND* nd_t = new ND[lv_nd];
	nd_tb->CopyTable(nd_t);

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

	ind = main_ind;//从电源点开始搜索
	//节点的bch为上级节点
	(nd_t+ind)->bch = ind;//从源点开始开始进行,将源点作为上一级记录下来
	
	//之前已经将所有的节点都置为未处理
	if((nd_t+ind)->q & ND_UNPROC) //this so nd_t has not processed
	{
		//拿当前节点向下扩展，找到所有和它相连的节点
		knd = ind;//当前节点赋给一个临时的变量
		(nd_t+knd)->fch = knd; //当前节点号赋给前向链 
		(nd_t+knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

		while (1)//还有扩展开关没有处理
		{
			if ( (knd!=l) && (knd!=r) ) //以电源点为起点，对负荷点进行追踪
			{
				//通过节点的q可以判断出是不是负荷节点，q |= 8
				if ((nd_t+knd)->q & ND_LD) 
				{
					//得到负荷id
					ld_p = r_nd_ld_tb->FindChildPtr1((nd_t+knd)->r_ld);
					ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
					while(ld_p)
					{
						//path_load[cntload].level = ld_tb->GetDataByPh(ld_p->Ptr)->level;//一般负荷
						p += ld_tb->GetDataByPh(ld_p->Ptr)->w;
						q += ld_tb->GetDataByPh(ld_p->Ptr)->r;

						strcat(ldList,ld_tb->GetDataByPh(ld_p->Ptr)->id);
						strcat(ldList,",");

						cntload++;

						ld_p = ld_p->Next;
					}
					::FreeChildPtr(ld_p0);
				}
			}

			//read cbs connected to this nd
			//从节点-开关关系表找到所有和这个节点相连的开关
			cb_p = r_nd_cb_tb->FindChildPtr1((nd_t+knd)->r_cb);
			cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
			while(cb_p)
			{
				//find the opposite nd_t through this cb
				//kknd表示开关的对侧节点，这里的节点都是整型的编号
				if(knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}

				if ( (kkndcb == l) || (kkndcb == r) )
				{
					cb_p = cb_p->Next;
					continue;
				}
					
//printf("%s %d\n",cb_tb->GetDataByPh(cb_p->Ptr)->id,cb_tb->GetDataByPh(cb_p->Ptr)->q);
				//如果是联络开关，停下来,只要是联络开关，不管是否悬空，都需要进行处理
				if ( ((cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//联络开关的q为1，常开
					|| (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_OPENCB))  //分位开关					
					) 
				{

					//如果是从联络开关进行搜索，其它联络等同于普通悬空开关
					cb_p = cb_p->Next;
					continue;
				}


				//process this opposite nd
				//如果对侧节点 未处理==开关不参与拓扑推进
				if((nd_t+kkndcb)->q & ND_UNPROC)
				{
					//将这个节点的上一级记录下来，准备回溯
					(nd_t+kkndcb)->bch = knd;

					//将扩展的前向链 设成 找到的新节点的前向链
					(nd_t+kkndcb)->fch = (nd_t+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd_t+knd)->fch = kkndcb;
					//将找到的新节点设成已处理
					(nd_t+kkndcb)->q &= (~ND_UNPROC);

					if (!isForkCB)
						strcpy(cbID,cb_tb->GetDataByPh(cb_p->Ptr)->id);
					isForkCB = true;
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this nd
			sec_p = r_nd_sec_tb->FindChildPtr1((nd_t+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//find the opposite nd_t through this sec
				if(knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
				{
					kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
				}
				else
				{
					kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;
				}

				if ( (kknd == l) || (kknd == r) )
				{
					sec_p = sec_p->Next;
					continue;
				}

//printf("%s\n",sec_tb->GetDataByPh(sec_p->Ptr)->id);
				//process this opposite nd
				if((nd_t+kknd)->q & ND_UNPROC)
				{
					//循环链表的使用，每扩展一个新的节点，做2件事：
					//1.将上一个节点的前向链设为自己的前向链
					//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
					
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd_t+kknd)->fch = (nd_t+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd_t+knd)->fch = kknd;

					//将这个节点的上一级记录下来，准备回溯
					(nd_t+kknd)->bch = knd;

					//将找到的新节点设成已处理
					(nd_t+kknd)->q &= (~ND_UNPROC);
				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd，搜索完一层
			knd = (nd_t+knd)->fch;
			//循环到头，连起来了，表示全部走了一遍
			//拿另外的节点再开始搜索，一直到所有的节点都处理
			if(knd == ind)
			{
				break;
			}
		}
	}
	//nd_tb->UpdateTable(nd_t);
	delete[] nd_t;
	nd_t = 0;

	sumP = p;
	sumQ = q;

	if (isForkCB)
	{
		strcat(ldList,"cbid=");
		strcat(ldList,cbID);
	}
	else	
		//整理负荷列表字符串
		lld->delstrright(ldList,1);

	return cntload;
}

int optSect::rs_table_count()
{
	if (lld->mx_cb > 0)
		lv_cb = cb_tb->GetCount();
	lv_so = so_tb->GetCount();
	lv_ld = ld_tb->GetCount();
	lv_sec = sec_tb->GetCount();
	lv_nd = nd_tb->GetCount();
	lv_r_nd_cb = r_nd_cb_tb->GetCount();
//	lv_r_nd_xf = r_nd_xf_tb->GetCount();
	lv_r_nd_sec = r_nd_sec_tb->GetCount();
	lv_r_nd_ld = r_nd_ld_tb->GetCount();

	return 1;
}

int optSect::rs_table_get()
{
//	int		i;

	//get cb table
	if (lld->mx_cb > 0)
		cb = cb_tb->GetTable();

	//get so table
	so = so_tb->GetTable();

	//get nd table
	nd = nd_tb->GetTable();

	//get ld table
	ld = ld_tb->GetTable();

	//get sec table
	sec = sec_tb->GetTable();
	
	//get r_nd_sec table
	r_nd_sec = r_nd_sec_tb->GetTable();
	r_nd_cb = r_nd_cb_tb->GetTable();
//	r_nd_xf = r_nd_xf_tb->GetTable();
	r_nd_ld = r_nd_ld_tb->GetTable();
	
	return 1;
}

int optSect::insert_equip(int cntEquipment,char* equipid,int equipidph,int ind,int iznd,int type,
							  double prob_fault, double repairTime,
							  double prob_check, double checkTime)
{
	equip->i_id = cntEquipment;
	strcpy(equip->id,equipid);
	equip->idph = equipidph;
	equip->ind = ind;
	equip->iznd = iznd;
	equip->type = type;
	equip->prob_fault = prob_fault;
	equip->repairTime = repairTime;	
	if (prob_check>=0)
	{
		equip->prob_check = prob_check;
		equip->checkTime = checkTime;
	}

	return 0;
}

int optSect::insert_r_nd_equip(ND* nd_t,char* nd_id,int knd,int equip_id)
{
	int		phpos, fptr;

	r_nd_equip = new R_ND_EQ;
	*r_nd_equip = R_ND_EQ(nd_id, equip_id);
	r_nd_equip->nd_id_ptr = knd;
	r_nd_equip->equip_id_ptr = equip_id;
//	phpos = nd_tb->GetDataByPh(knd)->r_equip;//得到节点表中和开关的关联信息
	phpos = (nd_t+knd)->r_eq;//得到节点表中和开关的关联信息
	if(phpos == -1)  //first connected cb of this nd节点没有连接开关
	{
		//节点是父，开关是子。
		//节点连的第一个开关指针（子），通过这个指针可以得到所有的这个节点连的开关
		//nd_tb->GetDataByPh(knd)->r_equip = r_nd_equip_tb->GetNext();
		(nd_t+knd)->r_eq = r_nd_equip_tb->GetNext();
	}
	else  //build cb chain for this nd，如果节点以前已经连接了开关
	{
		//将节点开关关系表中的位置向后挪动，准备新的节点关系指针插入
		fptr = r_nd_equip_tb->GetDataByPh(phpos)->fptr;
		while( fptr != -1 )  
		{
			phpos = fptr;
			fptr = r_nd_equip_tb->GetDataByPh(phpos)->fptr;
		}
		r_nd_equip_tb->GetDataByPh(phpos)->fptr = r_nd_equip_tb->GetNext();
		r_nd_equip->bptr = phpos;
	}
	//插入后返回指针链表的物理位置，便于下一个指针的插入
	r_nd_equip->id = r_nd_equip_tb->GetNext();//用逻辑位置做id号
	phpos = r_nd_equip_tb->Insert(*r_nd_equip);
	if(phpos == -1)//判断是否越界
	{
		return -1;
	}
	delete r_nd_equip;
	r_nd_equip = 0;

	return 1;
}

//返回最小路指针
PATH_LOAD optSect::getMinPath(int ldnd)//通过负荷的节点号，得到负荷的最小路
{
	PATH_LOAD ret;
	ret.i_id = -1;
	ret.id[0] = 0;

	for(int i=0; i<cntload; i++)
	{
		//找到负荷点
		if (path_load[i].ind==ldnd)//最小路
			return path_load[i];

	}

	return ret;
}

//将两个向量反向比较，返回源的顶点的位置
int optSect::getCrossPoint(int* src,int* comp,int& cross_nd,int& pos_src,int& pos_parent)
{
	//long tt=lld->getCurMi();

	int i,j,p,pos,t_src[MAX_MINPATH_LEN],t_comp[MAX_MINPATH_LEN];
	for (i=MAX_MINPATH_LEN-1; i>=0; i--)
	{
		t_src[i] = -1;
		t_comp[i] = -1;
	}
	
	pos = -1;
	j=0;
	for (i=MAX_MINPATH_LEN-1; i>=0; i--)
	{
		if (src[i] != -1)
		{
			t_src[j++] = src[i];
		
			if (pos == -1)
			{
				pos = i;
			}
		}
	}

	j=0;
	for (i=MAX_MINPATH_LEN-1; i>=0; i--)
	{
		if (comp[i] != -1)
			t_comp[j++] = comp[i];
	}

	int ret = comp_vector(t_src,t_comp,p);//比较2个向量，返回分叉点的序号
	cross_nd = p;
	pos_parent = ret;
	pos_src = pos - ret;

	//t_getCrossPoint += lld->getCurMi()-tt;

	return ret;
}

//给定首末节点号，返回开关的类型，找不到或不是开关返回-1
int optSect::getCBType(int start, int end)
{
	//负荷
	if ( (start==-1) || (end==-1))
		return -1;

	//设备不属于负荷
//	EQUIP* t_equip;

	int i_id,k,l,itype;
	int i[20],j[20],t[20],i_cnt,j_cnt;
	for(k=0; k<20; k++)
		i[k]=j[k]=t[k]=-1;
	
	ND* nd_t = nd_tb->GetTable();
	ChildPtr1	*equip_p, *equip_p0;
	//EQUIP* e_find=new EQUIP();
	
	k=0;
	equip_p = r_nd_equip_tb->FindChildPtr1((nd_t+start)->r_eq);
	equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
	while(equip_p)
	{
		//0,1,2,3,5 为开关
		itype = equip_tb->GetDataByPh(equip_p->Ptr)->type;
		if ( (itype == 0)//负荷
			|| (itype == 1) 
			|| (itype == 2) 
			|| (itype == 3) 
			|| (itype == 5) 
			)//线段
		{
			i_id = equip_tb->GetDataByPh(equip_p->Ptr)->i_id;
			i[k] = i_id;
			t[k++] = itype;
		}

		equip_p = equip_p->Next;
	}
	::FreeChildPtr(equip_p0);
	i_cnt = k;

	k=0;
	equip_p = r_nd_equip_tb->FindChildPtr1((nd_t+end)->r_eq);
	equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
	while(equip_p)
	{
		itype = equip_tb->GetDataByPh(equip_p->Ptr)->type;
		if ( (itype == 0)//负荷
			|| (itype == 1) 
			|| (itype == 2) 
			|| (itype == 3) 
			|| (itype == 5) 
			)//线段
		{
			i_id = equip_tb->GetDataByPh(equip_p->Ptr)->i_id;
			j[k++] = i_id;
		}

		equip_p = equip_p->Next;
	}
	::FreeChildPtr(equip_p0);
	j_cnt = k;

//连接的设备都含有同一个设备
	for (k=0; k<i_cnt; k++)
		for (l=0; l<j_cnt; l++)
		{
			if (i[k] == j[l])
			{
				return t[k];
			}
		}

	return -1;
}


//给定首末节点号，返回设备的设备id，找不到或不是开关返回-1
int optSect::getEquipID(int start, int end)
{
	//两端都有节点的设备
	int i_id,k,l;
	int i[20],j[20],t[20],i_cnt,j_cnt;
	ND* nd_t = nd_tb->GetTable();
	ChildPtr1	*equip_p, *equip_p0;

	//没有节点
	if ( (start==-1) && (end==-1) )
		return -1;

//////////////////////////////////////////////////////////////////////////
	//特殊处理负荷等单端元件，首末节点可能为-1
	i_id = -1;
	if ((start!=-1) && (end==-1)) 
	{
		i_id = start;
	}
		
	if ((start==-1) && (end!=-1))
	{
		i_id = end;
	}

	//肯定是单端元件
	if (i_id != -1)
	{
		equip_p = r_nd_equip_tb->FindChildPtr1((nd_t+i_id)->r_eq);
		equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
		while(equip_p)
		{
			//有可能这个设备
//			if ( (equip_tb->GetDataByPh(equip_p->Ptr)->ind != -1) 
//				&& (equip_tb->GetDataByPh(equip_p->Ptr)->iznd != -1) )
//			{
//			}
//			else
//				i_id = equip_tb->GetDataByPh(equip_p->Ptr)->i_id;

			//在进行设备写入的时候，已经规定了单端设备的末端为-1
			if (equip_tb->GetDataByPh(equip_p->Ptr)->iznd != -1) 
				i_id = equip_tb->GetDataByPh(equip_p->Ptr)->i_id;

			equip_p = equip_p->Next;
		}
		::FreeChildPtr(equip_p0);
	
		return i_id;
	}
//////////////////////////////////////////////////////////////////////////

	//准备处理双端元件
	for(k=0; k<20; k++)
		i[k]=j[k]=t[k]=-1;
	
	//EQUIP* e_find=new EQUIP();
	
	k=0;
	equip_p = r_nd_equip_tb->FindChildPtr1((nd_t+start)->r_eq);
	equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
	while(equip_p)
	{
		i_id = equip_tb->GetDataByPh(equip_p->Ptr)->i_id;
		i[k] = i_id;
		t[k++] = i_id;

		equip_p = equip_p->Next;
	}
	::FreeChildPtr(equip_p0);
	i_cnt = k;

	k=0;
	equip_p = r_nd_equip_tb->FindChildPtr1((nd_t+end)->r_eq);
	equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
	while(equip_p)
	{
		i_id = equip_tb->GetDataByPh(equip_p->Ptr)->i_id;
		j[k++] = i_id;

		equip_p = equip_p->Next;
	}
	::FreeChildPtr(equip_p0);
	j_cnt = k;

//连接的设备都含有同一个设备，这样的设备不会太多
	for (k=0; k<i_cnt; k++)
		for (l=0; l<j_cnt; l++)
		{
			if (i[k] == j[l])
			{
				return t[k];
			}
		}

	return -1;
}

//得到设备最小路上的到交叉点的开关，-1，没有开关；0，隔离开关；1，熔断器或带保护的断路器
int optSect::isDisconnect(int* minPath_src,int start, int end)//通过负荷的节点号，得到负荷的最小路
{
//	return 1;
//long time_begin;

	int i;
	int ind,iznd,type;
	int iFind = 0;
	if ( (start<0) || (end<0) )
		return 0;

	for(i=0; i<MAX_MINPATH_LEN; i++)
	{
		//注意这里有方向，靠近start的第一个开关
		ind = minPath_src[i];
		iznd = minPath_src[i+1];

		if (ind == start)//开始
		{
			iFind = 1;
		}
		
		if (iFind)
		{

		//time_begin = lld->getCurMi();
		
			//type = getEquip(ind,iznd)->type;
			type = getCBType(ind,iznd);
		//t_toptSectn += lld->getCurMi()-time_begin;

			if ( (type == 0)//断路器
			  || ( type == 5)//保护开关
			  || ( type == 1)//loadbreakswitch
			  || ( type == 2)//disconnector
			  || ( type == 3)//fuse
			   )
			{
				return type;
			}
		}
	
		if (iznd == end)//结束
		{
			iFind = 0;
			break;
		}
	}
		
	return -1;
}

int optSect::InMinPath(PATH_LOAD t_path_load,int i_id)
{	 
	int ind_Equipment,iznd_Equipment;
	int t_minPath[MAX_MINPATH_LEN];
	int len;

	len = 0;
	for (int ii=0;ii<MAX_MINPATH_LEN;ii++)
	{
		if ( t_path_load.minpath[ii] == -1 )
			break;
		t_minPath[ii] = t_path_load.minpath[ii]; 
		len++;
	}

	//对最小路进行排序,用于对分法
	lld->sortInt(t_minPath,len);

	EQ* equip = equip_tb->GetDataByPh(i_id);
	ind_Equipment = equip->ind;//设备的节点是有顺序的，从电源点开始向下
	iznd_Equipment = equip->iznd;

	if ((ele_vector(t_minPath,ind_Equipment,len)>=0)
		&&
		(ele_vector(t_minPath,iznd_Equipment,len)>=0))
		return iznd_Equipment;

	//只有一端挂在主干线上，返回单端主干线上的节点
	if (ele_vector(t_minPath,ind_Equipment,len)>=0)
		return ind_Equipment;

	if (ele_vector(t_minPath,iznd_Equipment,len)>=0) 
	{
		return iznd_Equipment;
	}
	
	return -1;
}

double optSect::random()   //产生一个0～1之间的随机数
{
	int m;
	double s,u,v,p;


	s=65536.0;
	u=2053.0;
	v=13849.0;

	static_r=u*(static_r)+v;
	m=(int)(static_r/s);
	static_r=static_r-m*s;
	p=static_r/s;
	return(p);
}

//产生一个low,high之间的随机浮点数
double optSect::randf(double low, double high)  
{
	double x=random();
	double i=x*(high-low)+low;
	return(i);
}

//产生一个[low,high]之间的随机整数
int optSect::randi(int low,int high)
{
	int i=high-low+1;
    int k= rand()%i+low;
	return(k);
}

void optSect::initGA()//相当于构造函数
{
	sumfitness=0;            /* 种群中个体适应度累计 */
    max=0;                   /* 种群中个体最大适应度 */
    avg=0;                   /* 种群中个体平均适应度 */
    min=0;                   /* 种群中个体最小适应度 */
	popsize = 0;               /* 种群大小  */
	chromsize = 0;             /* 存储一染色体所需字节数 */
	pcross = 0.8;                /* 交叉概率 */
	pmutation=0.05;             /* 变异概率 */
	gen = 0;                   /* 当前世代数 */
	maxgen=10;                /* 最大世代数   */
    nmutation=0;             /* 当前代变异发生次数 */
    ncross=0;                /* 当前代交叉发生次数 */
	sumpsel=0;
    nbest=0;                 //最佳个体保持不变的代数

	/* 随机数发生器使用的静态变量 */
    static_r=1.0;
    //求最小值特征，最佳个体初值置一个大数
	bestfit.fitness=9999999999999; 

	for(int i=0;i<100;i++)//染色体的上下限值
	{
		psel[i]=0;
		vlow[i]=0;
		vhigh[i]=0;
		//ilow[i]=0;
		//ihigh[i]=0;
	}
}

//
//为全局变量分配空间
void optSect::initmalloc()
{
	int i, j;

	nSect = 2;//默认分三段
	loadrate = 0;
	w_w = 0.7; w_cntLd = 0.2; w_len = 0.1;
	p_un = 0.7;	p_vip = 0.5;p_avgLD = 0.1;
	p_un = 0.7;	p_vip = 0.5;p_avgLD = 0.1;

	ifstream aa;               //定义输出文件
	aa.open("C:\\input.txt");     //作为输出文件打开
	if (aa.is_open())
	{
		//ifstream aa("C:\\DNASP\\雄安3.0\\DLL3.0\\plan_calcDLL_vc6\\plan_calcDLL_vc6\\input.txt");
		//从文本文件读取计算参数
		aa >> nSect >> nSect >> nSect >> nSect >> nSect >> nSect >> nSect;
		aa >> nSect >> w_cntLd >> w_w >> w_len >> p_un >> p_vip >> p_avgLD >> loadrate;
		aa.close();
	}
	else
	{
		loadrate = 0;
		w_w = 0.7; w_cntLd = 0.2; w_len = 0.1;
		p_un = 0.7;	p_vip = 0.5;p_avgLD = 0.1;
	}

	if (nSect == -1)
	{
		nSect = 2;//默认分三段
	}

	//需要计算优化分段数量
	//i = OnAnalyseShape();
	//1.均匀线路，2.橄榄型；3.哑铃型；4.杠杆型
	if (fdShape == 4)
	{
		if (loadrate == 0)
		{
			w_w = 0.2; w_cntLd = 0.1; w_len = 0.7;
		}
		else
		if (loadrate == 1)
		{
			w_w = 0.1; w_cntLd = 0.3; w_len = 0.6;
		}
		else
		{
			w_w = 0.4; w_cntLd = 0.2; w_len = 0.4;
		}
	}
	else
	//2.纺锤型，长度分段减弱
	//3.哑铃型，长度分段加强
	if ((fdShape == 2) || (fdShape == 3))
	{
		if (loadrate == 0)
		{
			w_w = 0.3; w_cntLd = 0.2; w_len = 0.5;
		}
		else
		if (loadrate == 1)
		{
			w_w = 0.1; w_cntLd = 0.4; w_len = 0.5;
		}
		else
		{
			w_w = 0.4; w_cntLd = 0.1; w_len = 0.5;
		}
	}
	else
	{
		if (loadrate == 0)
		{
			w_w = 0.7; w_cntLd = 0.2; w_len = 0.1;
		}
		else
		if (loadrate == 1)
		{
			w_w = 0.1; w_cntLd = 0.8; w_len = 0.1;
		}
		else
		{
			w_w = 0.8; w_cntLd = 0.1; w_len = 0.1;
		}
	}

	//染色体的长度需要计算
	chromsize = cntMainLine;
	popsize = 4*cntMainLine;

	//分配给当前代和新一代种群内存空间
    if((oldpop=new struct individual[popsize])==NULL)
		return;
	if((newpop=new struct individual[popsize])==NULL)
		return;

	//分配给染色体内存空间，最后两个是时户数和用户数等可靠性指标，可以累加计算可靠性
	for(j=0;j<popsize;j++)
	{
		if((oldpop[j].chrom=new double[chromsize])==NULL)
			return;
	    if((newpop[j].chrom=new double[chromsize])==NULL)
			return;
	}

	if((bestfit.chrom=new double[chromsize])==NULL)
		return;
	if((bestthick.chrom=new double[chromsize])==NULL)
		return;

/*	//给变量的上下限分配内存空间
    if((vlow=new double[fn])==NULL)
			nomemory("vlow");
	if((vhigh=new double[fn])==NULL)
			nomemory("vhigh");
    if((ilow=new int[in])==NULL)
			nomemory("ilow");
	if((ihigh=new int[in])==NULL)
			nomemory("ihigh");

	if ((psel = new double[popsize]) == NULL)
			nomemory("psel");
*/
}

void optSect::freeGA()//析构函数
{
	int i,j;
	for(i=0;i<popsize;i++)
	{
		//for (j = 0;j < chromsize;j++)
		//{
		//	o << oldpop[i].chrom[j] << " ";
		//}
		//o << endl;

		delete[] oldpop[i].chrom;
		delete[] newpop[i].chrom;
	}
	delete[] oldpop;
	delete[] newpop;
	delete[] bestfit.chrom;
	delete[] bestthick.chrom;	

/*	delete[] vlow;
	delete[] vhigh;
	delete[] ilow;
	delete[] ihigh;
	delete[] psel;
	*/
}

int optSect::getValidgen(individual *pop)
{
	int i, j, k;
	j = 0;
	i = -1;
	while (true)
	{
		if (j++ > 100)
			break;

		i = randi(0, chromsize);
		if ((i == 0) || (i == 1) || (i == chromsize - 2) || (i == chromsize - 1)|| (strlen(path_mainline[i].UnionCBID) > 0) )
			continue;
		else
		{
			//不能对于其中的任何一个为1的数
			for (k = 0;k < chromsize;k++)
			{
				if ((1 == pop->chrom[k]) && (i == pop->chrom[k]))
					continue;
			}
			break;
		}
	}
	
	return i;
}

//修复染色体
//不能是前后的4个，不能和中间的重复
int optSect::editChrome(individual *pop)
{
	int i,j,k;
	int chrom,pic,temp;
	j = 0;
	pic = 0;

	//在中间部分找一个基因进行处理
	for (i = 0;i < chromsize;i++)
	{
		temp = pop->chrom[i];
		if (temp == 1)
		{
			//if ((i == 0) || (i == 1)|| (i == chromsize - 2) || (i == chromsize - 1))
			//{
			//	pop->chrom[i] = 0;
			//	chrom = getValidgen(pop);
			//	if (chrom == -1)
			//		return -1;//没有办法修复
			//	pop->chrom[chrom] = 1;
			//}
			pic++;
		}
	}

	//大于或小于断点数量
	k = pic - nSect;
	if (k < 0)//需要补
	{
		k = -k;
		for (i = 0;i < k;i++)
		{
			pop->chrom[getValidgen(pop)] = 1;
		}
	}
	else if (k > 0)//需要减
	{
		while (true)
		{
			if (pop->chrom[0] == 1)
			{
				pop->chrom[0] = 0;
				if (--k == 0)
					break;
			}
			if (pop->chrom[1] == 1)
			{
				pop->chrom[1] = 0;
				if (--k == 0)
					break;
			}
			if (pop->chrom[chromsize - 2] == 1)
			{
				pop->chrom[chromsize - 2] = 0;
				if (--k == 0)
					break;
			}
			if (pop->chrom[chromsize - 1] == 1)
			{
				pop->chrom[chromsize - 1] = 0;
				if (--k == 0)
					break;
			}

			for (j = 2;j < chromsize-2;j++)
			{
				if (pop->chrom[j] == 1)
				{
					pop->chrom[j] = 0;
					if (--k == 0)
						break;
				}
			}
		}
	}
	return 1;
}

//有效 1，新2，有效且新3，无效0
int optSect::isValidChrome(individual *pop,int type)
{
	int i, j, k;
	int chrom, valid;

	//if (type == -1)
	//{
	//	if (editChrome(pop) == -1)
	//		return 0;
	//}
	//else
	{
		valid = 0;
		for (i = 0;i < chromsize;i++)
		{
			chrom = pop->chrom[i];
			if (chrom == 1)
			{
				//断点不能是出线开关或联络开关
				if ((i == 0) || (i == 1) || (i == chromsize - 2) || (i == chromsize - 1))
				{
					return 0;
				}
				if (strlen(path_mainline[i].UnionCBID) > 0)
					return 0;

				valid++;
			}
		}

		if (valid != nSect)//断点超过规定的节点数
			return 0;

		//判断是否重复
		if (type > 0)
		{
			int ret = isNew(pop, type);
			if (ret == 1)
				return 1;
			else
				return 0;
		}
	}
	
	return 1;
}

//给一个基因的位置都一样
int optSect::isNew(individual *pop,int size )
{
	int i, j, k;
	int chrom;

	int isNew = 0;//先假设染色体不是新的
	for (j = 0;j < size;j++)
	{
		//逐个基因比较
		for (i = 0;i < chromsize;i++)
		{
			chrom = pop->chrom[i];
			if (oldpop[j].chrom[i] != chrom)
			{//只要一个基因位和以前的不一样，就认为是新的
				isNew = 1;
				break;
			}
		}
		//全部比较完了，完全一样
		if (isNew == 0)
			break;
	}
	if (isNew == 0)
		return 0;
	else
		return 1;
}

//产生初始种群
void optSect::initpop()
{	
	int i,j,k,m,n,iFind,valid,l;
	int chrom;
	int ii = 0;
	int jj = 0;
	int* pic = new int[nSect];

	lv_so = so_tb->GetCount();
	SO* so = so_tb->GetTable();
	if(lv_so == 0)
	{
//		printf("there is no record in lv_so table\n");
		//lld->addLogLine("电源表中无记录!");
	
		return;
	}
		
	srand(time(0));
	vector<int> temp;
	for (i = 0; i < chromsize; ++i)
		temp.push_back(i + 1);

	//random_shuffle(temp.begin(), temp.end(), p_myrandom);
	//random_shuffle(temp.begin(), temp.end(), Print<int>);
	
	//先洗一次牌
	random_shuffle(temp.begin(), temp.end());
	m = 0;

	//染色体初始化，每个个体的染色体都不一样
	for (j = 0; j < popsize;j++)
	{
		k = 0;
		while (true)
		{
			if (k++ > 20000)//不能再有有效解
			{
				//复制上一条
				if (j>0)
					for (i = 0;i < chromsize;i++)
						oldpop[j].chrom[i] = oldpop[j-1].chrom[i];
				else
					for (i = 0;i < chromsize;i++)
						oldpop[j].chrom[i] = 0;

				break;
			}

			//lld->OnGetOneRandom(chromsize, nSect, pic);//获得一组不重复的随机数
													   
			//先写到染色体里面
			//解释成电气量
			for (i = 0;i < chromsize;i++)
				oldpop[j].chrom[i] = 0;

			//o << "生成：";//输出适应度，最小值特征：－
			for (n = 0;n < nSect;n++)
			{
				if (m >= chromsize-1)
				{
					//再洗一次牌
					m = 0;
					//temp.clear();
					//for (i = 0; i < chromsize; ++i)
					//	temp.push_back(i + 1);
					random_shuffle(temp.begin(), temp.end());
				}
				while (true)
				{
					if (m >= chromsize - 1)
						break;

					valid = temp[m++];
					if ((valid <= 1) || (valid >= chromsize))
						continue;
					else
					{
						oldpop[j].chrom[valid] = 1;
						break;
					}
				}
			}
//			break;
			
			//有效并且和以前的不重复，只针对初始值
			if (isValidChrome(&oldpop[j],j))
			{
				//o << "有效" << endl;//输出适应度，最小值特征：－
				//for (n = 0;n<chromsize;n++)
				//{
				//	o << oldpop[j].chrom[n] << " ";//输出随机变量
				//}

				minfunction(&oldpop[j]);//计算个体的适应度，放到个体的染色体中
				//o <<"适应度："<< oldpop[j].fitness << endl;//输出适应度，最小值特征：－
				break;
			}
			//else
			//	o << "无效" << endl;//输出适应度，最小值特征：－
		}
	}

	temp.clear();
	delete[] pic;

	o << "          原始值 " << endl;//输出适应度，最小值特征：－
	o << " ========================== " << endl;//输出适应度，最小值特征：－
	for (j = 0; j<popsize;j++)//输出随机种群
	{
		o << j + 1 << " ";
		for (i = 0;i<chromsize;i++)
		{
			o << oldpop[j].chrom[i] << " ";//输出随机变量
		}
		o << oldpop[j].fitness << endl;//输出适应度，最小值特征：－
	}


	//int ii = 0;
	//int jj = 0;
	//int** cuts = NULL;
	//cuts = (int**)calloc(popsize, sizeof(int*));
	//for (ii = 0; ii < popsize; ++ii)
	//{
	//	cuts[ii] = (int*)calloc(nSect, sizeof(int));
	//}
	//for (ii = 0; ii < popsize; ++ii)
	//	for (jj = 0; jj < nSect; ++jj)
	//		cuts[ii][jj] = -1;

	//getRandom(cuts);


	//l = 0;

	//	///////////////////////////////////
	//	//根据分段数，随机产生有效的染色体
	//	k = 0;//尝试找到有效染色体的次数
	//	while (true)
	//	{
	//		if (k > 20000)
	//		{
	//			break;
	//		}

	//		valid = 0;//默认是否是有效的
	//		m = 0;
	//		//随机产生两个断点
	//		getRandom(cuts);
	//		//while (true)
	//		//{
	//		//	if (m > nSect)
	//		//		break;
	//		//	chrom = randi(0, cutsize);
	//		//	iFind = 0;
	//		//	for (n = 0;n < m;n++)
	//		//	{
	//		//		if (cuts[n] == chrom)
	//		//		{
	//		//			iFind = 1;
	//		//			break;
	//		//		}
	//		//	}
	//		//	if (iFind == 0)
	//		//		cuts[m++] = chrom;
	//		//}
	//		//解释成电气量
	//		for (i = 0;i < chromsize;i++)
	//		{
	//			for (n = 0;n < m - 1;n++)
	//			{
	//				if (cuts[n] == i)
	//				{
	//					oldpop[j].chrom[i] = 1;
	//				}
	//				else
	//					oldpop[j].chrom[i] = 0;
	//			}
	//		}
	//		if (isValidChrome(&oldpop[j]))
	//		{
	//			valid = 1;
	//			break;
	//		}
	//		k++;

	//	}


	//	if (valid == 1)
	//		minfunction(&oldpop[j]);//计算个体的适应度，放到个体的染色体中
	//	else
	//	{
	//		l++;
	//		for (i = 0;i < chromsize;i++)
	//		{
	//			oldpop[j].chrom[i] = 0;
	//		}
	//		oldpop[j].fitness = 10000;//默认一个很大的适应度
	//	}
	//}

	/*int a, b, c;
	for (ii = 0; ii < retNum; ++ii)
	{
		a = *(*(intlist + ii) + 0);
		b = *(*(intlist + ii) + 1);
		c = *(*(intlist + ii) + 2);
		printf("%d %d %d\n", a, b, c);

	}*/


	//for (ii = 0; ii < popsize; ++ii)
	//{
	//	if (NULL != cuts[ii])
	//		free(cuts[ii]);
	//}
	//free(cuts);
	//cuts = 0;




	//delete[] cuts;
}

//从电源点开始进行拓扑分析，给每个设备赋上岛号
void optSect::OnTP(int* cuts,int cntCut,int isOpt)
{
	int			i, j;
	int			ind, knd, kknd, kkndcb;

	int sumLD=0;
	int sumVip=0;
	vector<string> idList;
	vector<string> secList;
	char tt[10];
	idList.clear();
	/*for (i = 0; i < maxLoad; i++)
	{
		sprintf(tt,"",i+1);
		idList.push_back(tt);
	}*/

	double sumRatedS=0;
	double sumW=0;
	double sumLen=0;
	int sumUN=0;

	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;

	//read all nds into buffer
	int lv_nd = nd_tb->GetCount();
	nd_t = new ND[lv_nd];
	nd_tb->CopyTable(nd_t);

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	for (i = 0; i < lv_nd; i++)
	{
		for (j = 0;j < cntCut;j++)
		{
			if (i==cuts[j])
				(nd_t + i)->q &= (~ND_UNPROC);//将当前节点设为已处理
			else
				(nd_t + i)->q |= ND_UNPROC;
		}
	}

	island = 0;//起始岛
	for (i = 0; i < lv_nd; i++)
	{
		if ((nd_t + i)->q & ND_UNPROC)  //this nd has not processed
		{
			ind = i;//从电源点开始搜索
		   //节点的bch为上级节点
			(nd_t + ind)->bch = ind;//从源点开始开始进行,将源点作为上一级记录下来
			//之前已经将所有的节点都置为未处理
			//拿当前节点向下扩展，找到所有和它相连的节点
			knd = ind;//当前节点赋给一个临时的变量
			(nd_t + knd)->fch = knd; //当前节点号赋给前向链 
			(nd_t + knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

			while (1)//还有扩展开关没有处理
			{
				if ((nd_t + knd)->q & ND_LD)
				{
					//得到负荷id
					ld_p = r_nd_ld_tb->FindChildPtr1((nd_t + knd)->r_ld);
					ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
					while (ld_p)
					{
						sumLD++;
						if (ld_tb->GetDataByPh(ld_p->Ptr)->q & LD_LEVEL1)
							sumVip++;
						sumW += ld_tb->GetDataByPh(ld_p->Ptr)->w;
						sumRatedS += ld_tb->GetDataByPh(ld_p->Ptr)->wm;
						idList.push_back(ld_tb->GetDataByPh(ld_p->Ptr)->id);

						ld_tb->GetDataByPh(ld_p->Ptr)->fds = island;//一般负荷
						ld_p = ld_p->Next;
					}
					::FreeChildPtr(ld_p0);
				}

				//read cbs connected to this nd
				//从节点-开关关系表找到所有和这个节点相连的开关
				cb_p = r_nd_cb_tb->FindChildPtr1((nd_t + knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while (cb_p)
				{
					//find the opposite nd_t through this cb
					//kknd表示开关的对侧节点，这里的节点都是整型的编号
					if (knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}

					//			if(kkndcb==16)
					//			{
					//				int debug=0;
					//			}
					int cb_q = cb_tb->GetDataByPh(cb_p->Ptr)->q;

					//如果是联络开关，停下来,只要是联络开关，不管是否悬空，都需要进行处理
					if ((cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//联络开关的q为1，常开
																	//|| (cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB))  
																	//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0) comment lld for FD 2017-12-28
						)
					{
						//联络开关的后向链也需要记录下来，需要从联络开关处回溯
						if (kkndcb != -1)
							(nd_t + kkndcb)->bch = knd;

						sumUN++;
						//如果是从联络开关进行搜索，其它联络等同于普通悬空开关
						cb_p = cb_p->Next;
						continue;
					}

					//printf("%s %d\n",cb_tb->GetDataByPh(cb_p->Ptr)->id,cb_tb->GetDataByPh(cb_p->Ptr)->q);
					//只有闭合的开关才需要向下扩展
					if ((cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB))
					{
						//断开开关的后向链不需要记录下来
						cb_p = cb_p->Next;
						continue;
					}

					//process this opposite nd
					//如果对侧节点 未处理==开关不参与拓扑推进
					if ((nd_t + kkndcb)->q & ND_UNPROC)
					{
						cb_tb->GetDataByPh(cb_p->Ptr)->fds = island;
						//将这个节点的上一级记录下来，准备回溯
						(nd_t + kkndcb)->bch = knd;

						//将扩展的前向链 设成 找到的新节点的前向链
						(nd_t + kkndcb)->fch = (nd_t + knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd_t + knd)->fch = kkndcb;
						//将找到的新节点设成已处理
						(nd_t + kkndcb)->q &= (~ND_UNPROC);
					}

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//read secs connected to this nd
				sec_p = r_nd_sec_tb->FindChildPtr1((nd_t + knd)->r_sec);
				sec_p0 = sec_p;
				while (sec_p)
				{
					//find the opposite nd_t through this sec
					if (knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
					{
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
					}
					else
					{
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;
					}

					//							if(kknd==16)
					//			{
					//				int debug=0;
					//			}


					//printf("%s\n",sec_tb->GetDataByPh(sec_p->Ptr)->id);
					//process this opposite nd
					if ((nd_t + kknd)->q & ND_UNPROC)
					{
						sec_tb->GetDataByPh(sec_p->Ptr)->fds = island;
						sumLen += sec_tb->GetDataByPh(sec_p->Ptr)->len;

						secList.push_back(sec_tb->GetDataByPh(sec_p->Ptr)->id);

						//将扩展的前向链 设成 找到的新节点的前向链
						(nd_t + kknd)->fch = (nd_t + knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd_t + knd)->fch = kknd;

						//将这个节点的上一级记录下来，准备回溯
						(nd_t + kknd)->bch = knd;

						//将找到的新节点设成已处理
						(nd_t + kknd)->q &= (~ND_UNPROC);
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);

				//move to next nd，搜索完一层
				knd = (nd_t + knd)->fch;
				//循环到头，连起来了，表示全部走了一遍
				//拿另外的节点再开始搜索，一直到所有的节点都处理
				if (knd == ind)
				{
					//统计最终的结果
					if (isOpt == 1)
					{
						dsbob2->fdsect[sect_id].sumLD = sumLD;
						dsbob2->fdsect[sect_id].sumVip = sumVip;
						dsbob2->fdsect[sect_id].sumRatedS = sumRatedS;
						dsbob2->fdsect[sect_id].sumW = sumW;
						dsbob2->fdsect[sect_id].sumLen = sumLen;
						(dsbob2->fdsect[sect_id].idList).clear();
						for (vector<string>::iterator it = idList.begin();it != idList.end(); it++) ///第二种调用方法  
						{
							if ((*it).length()>0)
								(dsbob2->fdsect[sect_id].idList).push_back(*it);
						}
						(dsbob2->fdsect[sect_id].secList).clear();
						for (vector<string>::iterator it = secList.begin();it != secList.end(); it++) ///第二种调用方法  
						{
							if ((*it).length()>0)
								(dsbob2->fdsect[sect_id].secList).push_back(*it);
						}
						sect_id++;

						sumLD = 0;
						sumVip = 0;
						idList.clear();
						secList.clear();
						sumRatedS = 0;
						sumW = 0;
						sumLen = 0;
						sumUN = 0;
					}

					island++;
					break;
				}
			}
		}
	}
	nd_t = 0;
	delete[] nd_t;
}

//形成目标函数,每条线100个计算结果
void optSect::minfunction(individual * critter)
{
	int i,j;
	int chrom;
	int nCut = 0;
	int* cut = new int[nSect];
	double* w = new double[nSect + 1];
	double* cntLd = new double[nSect + 1];
	double* len = new double[nSect + 1];
	for (i = 0;i < nSect+1;i++)
	{
		if (i<nSect)
			*(cut + i) = 0;
		*(w + i) = 0;
		*(cntLd + i) = 0;
		*(len + i) = 0;
	}

	//把染色体拆开
	for (i = 0;i < chromsize;i++)
	{
		chrom = critter->chrom[i];
		if (chrom == 1)
		{
			//断开点
			*(cut+ nCut++) = path_mainline[i].ind;
		}
	}

	//拓扑分析
	OnTP(cut, nCut);

	//分析每个岛里面的负荷个数、负荷大小、导线长度
	lv_ld = ld_tb->GetCount();
	ld = ld_tb->GetTable();
	for (i = 0; i < lv_ld; i++)
	{
		for (j = 0;j < island;j++)
		{
			if ((ld+i)->fds == j)
			{
				(*(cntLd+j))++;
				*(w+j) += (ld+i)->w;
			}
		}
	}
	lv_sec = sec_tb->GetCount();
	sec = sec_tb->GetTable();
	for (i = 0; i < lv_sec; i++)
	{
		for (j = 0;j < island;j++)
		{
			if ((sec+i)->fds == j)
			{
				*(len+j) += (sec+i)->len;
			}
		}
	}

	//计算结果赋给个体的适应度
	critter->fitness=w_w*lld->OnGetStandardDev(w, nSect + 1)
		+ w_cntLd * lld->OnGetStandardDev(cntLd, nSect + 1)
		+ w_len * lld->OnGetStandardDev(len, nSect + 1);

	delete[] cut;
	delete[] w;
	delete[] cntLd;
	delete[] len;
}

//计算种群统计数据
void optSect::statistics(individual *pop)
{
	sumfitness=0.0;
	min=pop[0].fitness;
	max=pop[0].fitness;
	//计算最大、最小和累计适应度
	for(int j=0;j<popsize;j++)
	{
		sumfitness=sumfitness+pop[j].fitness;
		if(pop[j].fitness>max) max=pop[j].fitness;
		if(pop[j].fitness<min) min=pop[j].fitness;
	}

	//最佳个体
	int L=0;//L为判断这一代是否有最佳个体，如果有,则L=1
	for(int k=0;k<popsize;k++)
	{
		if((bestfit.fitness-pop[k].fitness)>9.99e-8)//求最小值特征
		{ 
			L=1;//有最佳个体
			nbest=0;                         
		   for(int i=0;i<chromsize;i++)
		   {
			   bestfit.chrom[i]=pop[k].chrom[i];
		   }
		   bestfit.fitness=pop[k].fitness;
           bestfit.generation=gen;
		}
	}

	if(L==0)//这一代没有最佳个体
	{
		nbest++;
	}

	avg=sumfitness/popsize;//计算平均适应度
}

//按照排名顺序分配选择概率
void optSect::pselect(double q)
{
	double p=1-q;
	psel[0]=q;
	 sumpsel=psel[0];

	for(int i=1;i<=popsize-1;i++)
	{
		psel[i]=psel[i-1]*p;
		sumpsel+=psel[i];
	}
}

//快速排序法，从小到大排列，适合于求解最小值问题
void optSect::qksortlittle(individual *pop,int left,int right)
{
	int l,r,h;
	double pivot;
	struct individual temp1;
	l=left;
	r=right;
	h=(left+right)/2;
	double k;
	k=0;//如果为GA,取k=0;如果为IGA，取k=0.02
	pivot=pop[h].fitness+k*pop[h].thick;
	
	while(l<r)
	{
		while((pop[l].fitness+k*pop[l].thick)<pivot) ++l;//从小到大排序特征
		while((pop[r].fitness+k*pop[r].thick)>pivot) --r;//从小到大排序特征

		if(l>=r) break;

		temp1=pop[l];
		pop[l]=pop[r];
		pop[r]=temp1;
		if(l!=pivot) --r;
		if(r!=pivot) ++l;
	}
	if(l==r) l++;
	if(left<r) qksortlittle(pop,left,l-1);
	if(l<right) qksortlittle(pop,r+1,right);
}


//保留最优解
void optSect::safebest()
{
	int s=1;//s为判据
	for(int i=0;i<popsize;i++)
	{  //如果新一代中有上一代的最优解，则最优解不用再保留
		if(newpop[i].fitness==bestfit.fitness)
			 s=0; //最优解保留的判据
			break;
	}
 
	if(s!=0)//s!=0,则保留上一代的最优解
	{
		int k=randi(0,popsize-1);
		for(int j=0;j<chromsize;j++)
		{
			newpop[k].chrom[j]=bestfit.chrom[j];
		}
		newpop[k].fitness=bestfit.fitness;
	}
}

void optSect::make_ga()//遗传算法操作
{
	gen=1;
	//最佳个体保持20代或遗传代数大于最大代数时，结束迭代计算
	while((gen<=maxgen))
	{
		//产生新一代
		generation();
		
		//THICK(0);//计算这一代的个体浓度
		qksortbig(newpop,0,popsize-1);//按个体浓度从大到小排序
//		qksortlittle(newpop,0,popsize-1);
		safebest();//保留最优解
		qksortlittle(newpop,0,popsize-1);
		//计算新一代种群的适应度统计数据
		statistics(newpop);
		//输出新一代统计数据
		//genresult();

		//o2<<gen<<"  "<<bestfit.fitness<<endl;
		temp=oldpop;
		oldpop=newpop;
		newpop=temp;
		gen++;
	}
}

//输出种群统计结果
void optSect::genresult()
{
	o << endl;
	o << "       模拟计算统计报告" << endl;
	o << endl;
	o << "世代数＝" << gen << endl;
	o << "染色体" 
		<< "       适应度" << endl;
	o << endl;

	for (int i = 0; i<popsize;i++)
	{
		o << i + 1 << "         ";
		for (int j = 0;j<chromsize;j++)
		{
			o<< newpop[i].chrom[j] << " ";
		}
		o<< newpop[i].fitness << endl;//最小值特征：－
	}
	o << endl;
	o << "第" << gen << "代统计：" << endl;
	o << "总交叉操作数＝" << ncross << "  "
		<< "总变异操作数=" << nmutation << endl;
	o << "最小适应度＝" << min << "  "//最小值特征：－max
		<< "最大适应度＝" << max << "  "//最小值特征：－min
		<< "平均适应度＝" << avg << endl;//最小值特征：－
	o << "迄今发现最佳个体：所在代数=" << bestfit.generation << endl;
	o << endl;
	o << "适应度" << "    ";
	o <<  bestfit.fitness << endl;//最小值特征：－
	o << endl;

}

//进行一代遗传操作
void optSect::generation()
{
	int i,mate1,mate2,j=0,k,l;
	
  /* 选择, 交叉, 变异 */
	while(j<=popsize-2)
	{
		l = 0;
		while (true)
		{
			if (l++ > 200)
				break;
			/* 挑选交叉配对 */
			mate2 = mate1 = -1;
			while (mate1 == -1)
				mate1 = select();
			while (mate2 == -1)
				mate2 = select();
			while( (mate1 == mate2) || (mate2 == -1))
			{
				mate2 = select();
			}
			/* 交叉和变异 */
			crossover(oldpop[mate1].chrom, oldpop[mate2].chrom,
				newpop[j].chrom, newpop[j + 1].chrom);

			for (int k = 0;k <= 1;k++)
			{
				mutation(newpop[j + k].chrom);
			}
			if (isValidChrome(&newpop[j], 0) == 0)
				continue;
			if (isValidChrome(&newpop[j + 1], 0) == 0)
				continue;
			break;
		}

		if (l==101)
		{
			for (i = 0;i < chromsize;i++)
			{
				newpop[j].chrom[i] = oldpop[mate1].chrom[i];
			}
			for (i = 0;i < chromsize;i++)
			{
				newpop[j + 1].chrom[i] = oldpop[mate2].chrom[i];
			}
		}

		j = j + 2;
	}
//////////////////////////////////////////////////////////////////////////
	for(j=0; j<popsize;j++)//放到每个电源点里，将来累加的总的适应度
	{
		minfunction(&newpop[j]);
		//输出新个体的适应度
	}
}

//选择操作
int optSect::select()
{
	double pick,sum;
    pick=random();

    sum=0;
	int i;
	for(i=0;(sum<pick)&&(i<popsize);i++)
		sum+=psel[i]/sumpsel;

	return(i-1);
}

//以一定的概率产生0或1
int optSect::flip(double prob)
{
	double p;
	p=random();//每次调用都产生不同的随机数
	if(p<=prob)
		return(1);
	else
		return(0);
}

//由两个父体交叉产生两个子体
void optSect::crossover(double *parent1,double *parent2,
			      double *child1,double *child2)
{
	double jcross;
	if(flip(pcross))
	{
		ncross++;
		for(int i=0;i<chromsize;i++)
		{
			jcross=randf(-0.25,1.25);
			child1[i]=parent1[i]*jcross+parent2[i]*(1-jcross);
			if(child1[i]<vlow[i]) child1[i]=vlow[i];
			if(child1[i]>vhigh[i]) child1[i]=vhigh[i];
			jcross=randf(-0.25,1.25);
			child2[i]=parent2[i]*jcross+parent1[i]*(1-jcross);
			if(child2[i]<vlow[i]) child2[i]=vlow[i];
			if(child2[i]>vhigh[i]) child2[i]=vhigh[i];
		}

//		for(int i1=fn;i1<fn+in;i1++)
//		{
//			jcross=optSectndf(-0.25,1.25);
//			child1[i1]=parent1[i1]*jcross+parent2[i1]*(1-jcross);
//			child1[i1]=int(child1[i1]+0.5);
//			if(child1[i1]<ilow[i1-fn]) child1[i1]=ilow[i1-fn];
//			if(child1[i1]>ihigh[i1-fn]) child1[i1]=ihigh[i1-fn];
//			jcross=optSectndf(-0.25,1.25);
//			child2[i1]=parent2[i1]*jcross+parent1[i1]*(1-jcross);
//			child2[i1]=int(child2[i1]+0.5);
//			if(child2[i1]<ilow[i1-fn]) child2[i1]=ilow[i1-fn];
//			if(child2[i1]>ihigh[i1-fn]) child2[i1]=ihigh[i1-fn];
//		}

	}
	else
	{
		for(int k=0;k<chromsize;k++)
		{
			child1[k]=parent1[k];
			child2[k]=parent2[k];
		}
	}
}

//变异操作
void optSect::mutation(double *child)
//void optSect::mutation(individual *child)
{
	if(flip(pmutation))
	{
		int k,k1,j,old;
		j=chromsize-1;
		k=randi(0,j);
		//k1 = getValidgen(child);
		if(k<chromsize)
		{
			old = child[k];
			if (old == 0)
			{
				child[k] = 1;//randi(ilow[k],ihigh[k]);
			}
			else
			{
				child[k] = 0;//randi(ilow[k],ihigh[k]);
			}

		}
//		else
//		{
//			child[k]=optSectndi(ilow[k-fn],ihigh[k-fn]);
//		}
		nmutation++;
	}
}


//计算新抗体浓度
void optSect::THICK(double T)
{
//	for(int j=0;j<=99;j++)//矩阵初始化
//	{
//		for(int k=0;k<=99;k++)
//		{
//			distan[j][k]=0;
//			integ[j][k]=0;
//		}
//	}
//
//	for(int r=0;r<popsize;r++)
//	{
//		newpop[r].thick=0;
//	}
//
//	int optSectte=50;//电压增量的权系数
//	for(int i1=1;i1<popsize;i1++)//计算抗体之间的距离和结合力
//	{
//		for(int j1=i1+1;j1<=popsize;j1++)
//		{
//			double DV=0.0;
//			for(int k1=0;k1<N1;k1++)
//			{
//				DV+=pow(optSectte*(newpop[i1-1].chrom[k1]-newpop[j1-1].chrom[k1]),2);
//			}
//
//			double DC=0.0;
//			for(int k2=0;k2<ICQ;k2++)
//			{
//				DC+=pow(newpop[i1-1].chrom[k2+N1]-newpop[j1-1].chrom[k2+N1],2);
//			}
//
//			double DT=0.0;
//			int NT;
//			NT=chromsize-N1-ICQ;//NT表示变压器的个数
//			for(int k3=0;k3<NT;k3++)
//			{
//				DT+=pow(newpop[i1-1].chrom[k3+N1+ICQ]-newpop[j1-1].chrom[k3+N1+ICQ],2);
//			}
//
//			distan[i1][j1]=pow(DV+DC+DT,0.5);
//			integ[i1][j1]=1/(1+distan[i1][j1]);
//		}
//	}
//
//	//计算对称矩阵的另一半元素
//	for(int i2=2;i2<=popsize;i2++)
//	{
//		for(int j2=1;j2<=i2-1;j2++)
//		{
//			integ[i2][j2]=integ[j2][i2];
//		}
//	}
//	
//	
//	//计算抗体浓度
//	for(int i3=1;i3<=popsize;i3++)
//	{
//		for(int j3=1;j3<=popsize;j3++)
//		{
//			if(integ[i3][j3]>=T)
//			{
//				newpop[i3-1].thick+=integ[i3][j3];
//			}
//		}
//		newpop[i3-1].thick/=popsize;
//	}

}


//淘汰高浓度的个体，用随机个体代替，但保留浓度最高的个体
void optSect::addrandpop(double r)//r为淘汰率
{
	int t;
	t=int(r*popsize+0.5);//计算被淘汰个体的个数
	for(int i=1;i<=t;i++)
	{
		for(int j=0;j<chromsize;j++)//给PV节点附初值
		{
		   newpop[i].chrom[j]=randf(vlow[j],vhigh[j]);
		}
		
//		for(int j1=fn;j1<fn+in;j1++)
//		{
//			newpop[i].chrom[j1]=optSectndi(ilow[j1-fn],ihigh[j1-fn]);
//		}

		minfunction(&newpop[i]);/*计算初始适应度，
	                        这里&(oldpop[i])是取地址传给形参*/	
	}
}

//快速排序法，个体浓度从大到小排列
void optSect::qksortbig(individual *pop,int left,int right)
{
	int l,r;
	double pivot;
	struct individual temp1;
	l=left;
	r=right;
	pivot=pop[(left+right)/2].thick;
	
	while(l<r)
	{
		while(pop[l].thick>pivot) ++l;//从大到小排序特征
		while(pop[r].thick<pivot) --r;//从大到小排序特征

		if(l>=r) break;

		temp1=pop[l];
		pop[l]=pop[r];
		pop[r]=temp1;
		if(l!=pivot) --r;
if (r != pivot) ++l;
	}
	if (l == r) l++;
	if (left < r) qksortbig(pop, left, l - 1);
	if (l < right) qksortbig(pop, r + 1, right);
}

//用遗传算法进行优化计算
int optSect::optCalc(char* mainline_name)
{
	//o.open("output_lld.txt");
	int i, j, chrom;
	char cut_out[256];

	srand((unsigned)time_t(NULL));//一定放在主函数里,如果需要每次产生
								//的随机数都不同，打开此句

	initGA();
	initmalloc();
	//1.形成初始化种群
	initpop();
	pselect(0.15); //按照排名顺序分配选择概率,第一个元素的概率为0.2
	qksortlittle(oldpop, 0, popsize - 1);
	statistics(oldpop);

	//2.遗传
	make_ga();

	//优化结束后输出计算结果
	//1.得到断点
	for (i = 0; i < 5; i++)
		cut[i] = -1;

	cut_out[0] = 0;
	j = 0;
	for (i = 0; i < chromsize; i++)
	{
		chrom = bestfit.chrom[i];
		if (chrom == 1)//分段点
		{
			cut[j++] = path_mainline[i].ind;
			strcat(cut_out, dsbob2->nd_tb->GetDataByPh(path_mainline[i].ind)->id);
			strcat(cut_out, ",");
		}
	}
	lld->delstrright(cut_out, 1);
	//2.统计每一段的情况，负荷个数，负荷大小，权重，不平衡度
	for (i = 0; i < 6; i++)
	{
		dsbob2->fdsect[i].fdid = "";
		dsbob2->fdsect[i].fdname = "";
		dsbob2->fdsect[i].version = "";
		dsbob2->fdsect[i].calcDate = "";
		dsbob2->fdsect[i].sect_id = 0;
		dsbob2->fdsect[i].cut = "";
		dsbob2->fdsect[i].sumLD = 0;
		dsbob2->fdsect[i].sumVip = 0;

		dsbob2->fdsect[i].idList.clear();

		dsbob2->fdsect[i].sumRatedS = 0;
		dsbob2->fdsect[i].sumW = 0;
		dsbob2->fdsect[i].sumLen = 0;
		dsbob2->fdsect[i].sumUN = 0;
		//
		dsbob2->fdsect[i].cntsect = 0;
		dsbob2->fdsect[i].wW = 0;  //负荷大小
		dsbob2->fdsect[i].wLD = 0; //负荷个数
		dsbob2->fdsect[i].wLen = 0;//分段长度
		dsbob2->fdsect[i].wUN = 0; //联络开关约束
		dsbob2->fdsect[i].wVIP = 0;//重要用户约束
		dsbob2->fdsect[i].wSecW = 0;//分段负荷大小约束

		dsbob2->fdsect[i].ruoW = 0;  //负荷大小
		dsbob2->fdsect[i].ruoLD = 0; //负荷个数
		dsbob2->fdsect[i].ruoLen = 0;//分段长度
		dsbob2->fdsect[i].punishUN = 0; //联络开关约束
		dsbob2->fdsect[i].punishVIP = 0;//重要用户约束
		dsbob2->fdsect[i].punishSecW = 0;//分段负荷大小约束
	}
	sect_id = 0;
	OnTP(cut, j, 1);
	for (i = 0; i < sect_id; i++)
	{
		dsbob2->fdsect[i].cut = cut_out;
		dsbob2->fdsect[i].sect_id = i;
		dsbob2->fdsect[i].wW = w_w;  //负荷大小
		dsbob2->fdsect[i].wLD = w_cntLd; //负荷个数
		dsbob2->fdsect[i].wLen = w_len;//分段长度
		dsbob2->fdsect[i].wUN = p_un; //联络开关约束
		dsbob2->fdsect[i].wVIP = p_vip;//重要用户约束
		dsbob2->fdsect[i].wSecW = p_avgLD;//分段负荷大小约束
	}

	FILE *fp;
	fp = fopen(mainline_name, "a"); //w++ 

	char line[256];
	line[0] = 0;
	//规范化计算结果
	fprintf(fp, "\n");
	sprintf(line, "<cut::mainnd name=""\"断点\">\n");
	fprintf(fp, line);
	sprintf(line, "@cut_id	x1	y1\n");
	fprintf(fp, line);
	sprintf(line, "/@	节点编号	x1	y1\n");
	fprintf(fp, line);

	for (int k = 0;k < j;k++)
	{
		for (i = 0;i < mainNDList.size();i++)
		{
			if (mainNDList.at(i).ind == cut[k])
			{
				sprintf(line,"# %s\t%d\t%d", mainNDList.at(i).id, mainNDList.at(i).x1, mainNDList.at(i).y1 - deltaH);
				fprintf(fp, line);
				fprintf(fp, "\n");
			}
		}
	}
	sprintf(line, "</cut::mainnd>");
	fprintf(fp, line);
	fclose(fp);

	//char temp[128];
	//optresult[0] = 0;

	//nd = dsbob2->nd_tb->GetTable();
	//ofstream outfile("ga.txt", ios::out);

	//outfile << "最优解出现代：" << bestfit.generation << endl;
	//outfile << "适应度：" << (bestfit.fitness) << endl;
	//for (i = 0; i < chromsize; i++)
	//{
	//	chrom = bestfit.chrom[i];
	//	if (chrom == 1)//分段点
	//	{
	//		outfile << (nd+path_mainline[i].ind)->id << "  ";			
	//	}
	//}
	//outfile.close();
	
	//o.close();
	//优化结果通过参数返回

	//3.结束
	freeGA();

	return 1;
}
//得到不同的随机数
void optSect::getRandom(int** cuts)
{
	//int lenPic, retNum;
	//lenPic = 3;
	//retNum = 100;
	//int i, j,n,m;

	//lld->OnGetRandom(chromsize, nSect, cuts, popsize);

	////解释成电气量
	//for (i = 0;i < chromsize;i++)
	//{
	//	for (n = 0;n < m - 1;n++)
	//	{
	//		if (cuts[n] == i)
	//		{
	//			oldpop[j].chrom[i] = 1;
	//		}
	//		else
	//			oldpop[j].chrom[i] = 0;
	//	}
	//}
	//if (isValidChrome(&oldpop[j]))
	//{

	//}


	//int iFind = 0;//默认是否是有效的
	//int m = 0;
	//int n = 0;
	//int chrom = 0;
	////随机产生两个断点
	//while (true)
	//{
	//	if (m > nSect)
	//		break;
	//	chrom = randi(0, chromsize);

	//	iFind = 0;
	//	for (n = 0;n < m;n++)
	//	{
	//		if (cuts[n] == chrom)
	//		{
	//			iFind = 1;
	//			break;
	//		}
	//	}
	//	
	//	if (iFind == 0)
	//		cuts[m++] = chrom;
	//}
}
