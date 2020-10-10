// NM1.cpp: implementation of the CNM1 class.
//
//////////////////////////////////////////////////////////////////////

#include "../DNASPLIB/NM1.h"
#include "math.h"

//#include <vld.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNM1::CNM1()
{
}

CNM1::CNM1(DSBOB2* m_pDb, CLLD* m_pLLD) : PWRFLOW(m_pDb,m_pLLD)
{
	this->dsbob2 = m_pDb;
	lld = m_pLLD;

	pathFlow[0] = 0;//默认不需要带残余潮流输出，lld 2020-4-29

	i_island = 0;
	//island_tb_fault = new DTable<ISLAND>(1000);//最多有1000个岛
	eq_tb_isolate = new DTable<EQ>(50);//最多有10个周边隔离开关
}

CNM1::~CNM1()
{
//	delete island_tb_fault;
}

//////////////////////////////////////////////////////////////////////////
int CNM1::OnGetDevByID(DTable<EQ>* eq_tb, char * id)
{
	for(int ii=0; ii<lv_eq_tb_isolate; ii++)
	{
		if (strcmp(eq_tb->GetDataByPh(ii)->id,id)==0 )
			return ii;
	}

	return -1;
}

//通过节点号获得设备
int CNM1::OnGetDevByNDs(DSBOB2* dsbob2,int start,int end)
{
	//两端都有节点的设备
	int i_id,k,l;
	int i[20],j[20],t[20],i_cnt,j_cnt;
	ChildPtr1	*equip_p, *equip_p0;

	//没有节点
	if ( (start==-1) && (end==-1) )
		return -1;

	ND* nd_tt = dsbob2->nd_tb->GetTable();

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
		equip_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_tt+i_id)->r_eq);
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
			if (dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->iznd != -1) 
				i_id = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->i_id;

			equip_p = equip_p->Next;
		}
		::FreeChildPtr(equip_p0);
	
		return i_id;
	}
//////////////////////////////////////////////////////////////////////////

	//准备处理双端元件
	for(k=0; k<20; k++)
		i[k]=j[k]=t[k]=-1;
	
	k=0;
	equip_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_tt+start)->r_eq);
	equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
	while(equip_p)
	{
		i_id = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->i_id;
		i[k] = i_id;
		t[k++] = i_id;

		equip_p = equip_p->Next;
	}
	::FreeChildPtr(equip_p0);
	i_cnt = k;

	k=0;
	equip_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_tt+end)->r_eq);
	equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
	while(equip_p)
	{
		i_id = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->i_id;
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

//从电源点到给定的设备之间的通道，设备是否有自切断能力
//带保护的开关和熔断器
//DEL int CNM1::OnSO2ND(int soid,int ndid)
//DEL {
//DEL 	int i,j,ret=-1,iFind=-1;
//DEL 	int			ind, knd, kknd,ld_nd,r_zn,knd_,un_nd,un_znd,eq_id;
//DEL 	ChildPtr1	*eq_p, *eq_p0;
//DEL 
//DEL 	int lv_nd = dsbob2->nd_tb->GetCount();
//DEL 	if(lv_nd == 0)
//DEL 	{
//DEL 		return 0;
//DEL 	}
//DEL 	ND* nd_t = new ND[lv_nd];
//DEL 	dsbob2->nd_tb->CopyTable(nd_t);//得到一个新的节点表，以免和原来的冲突
//DEL 
//DEL 	//将所有的临时接点设为已处理
//DEL 	for(i = 0; i < lv_nd; i++)
//DEL 	{
//DEL 		(nd_t+i)->q |= ND_UNPROC;
//DEL 	}
//DEL 
//DEL 	ind = soid;//母线的起始节点编号
//DEL 	//之前已经将所有的节点都置为未处理
//DEL 	if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
//DEL 	{
//DEL 		(nd_t+ind)->q &= (~ND_UNPROC);
//DEL 
//DEL 		knd = ind;//当前节点赋给一个临时的变量
//DEL 		(nd_t+knd)->fch = knd; //当前节点号赋给前向链 
//DEL 		(nd_t+ind)->bch = knd;//从电源点开始开始进行,将电源点作为上一级记录下来
//DEL      	while (1)//还有扩展开关没有处理
//DEL 		{
//DEL 			eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
//DEL 			eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
//DEL 			while(eq_p)
//DEL 			{
//DEL 				eq_id = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;//负荷对应的节点号
//DEL 
//DEL 				if(knd == dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind)
//DEL 				{
//DEL 					kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;
//DEL 				}
//DEL 				else
//DEL 				{
//DEL 					kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
//DEL 				}
//DEL 
//DEL 				if (kknd != -1)
//DEL 				{
//DEL 					//process this opposite nd
//DEL 					if((nd_t+kknd)->q & ND_UNPROC)
//DEL 					{
//DEL 						//将扩展的前向链 设成 找到的新节点的前向链
//DEL 						(nd_t+kknd)->fch = (nd_t+knd)->fch;
//DEL 						//新节点 设成 扩展节点的前向链
//DEL 						(nd_t+knd)->fch = kknd;
//DEL 						//将这个节点的上一级记录下来，准备回溯
//DEL 						(nd_t+kknd)->bch = knd;
//DEL 						
//DEL 						//将找到的新节点设成已处理
//DEL 						(nd_t+kknd)->q &= (~ND_UNPROC);
//DEL 					}
//DEL 				}
//DEL 
//DEL 				eq_p = eq_p->Next;
//DEL 			}
//DEL 			::FreeChildPtr(eq_p0);
//DEL 			//move to next nd
//DEL 			knd = (nd_t+knd)->fch;
//DEL 
//DEL 			if (knd == ndid)
//DEL 			{
//DEL 				iFind = knd;
//DEL 				ret = 0;
//DEL 				break;
//DEL 			}
//DEL 			
//DEL 			if(knd == ind)
//DEL 			{
//DEL 				break;
//DEL 			}
//DEL 		}
//DEL 	}
//DEL 
//DEL 	//找到了节点
//DEL 	if (iFind != -1)
//DEL 	{
//DEL 		knd = iFind;
//DEL 		do
//DEL 		{
//DEL 			ind = knd;//本次开关的节点号
//DEL 			knd = (nd_t+knd)->bch;//得到开关的上级节点号
//DEL 
//DEL 			eq_id = OnGetDevByNDs(dsbob2,ind,knd);
//DEL 			if ( 
//DEL 				(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 3)//熔断器
//DEL 				||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 5) //带保护的开关
//DEL 				)
//DEL 			{
//DEL 				ret= 2;//有速断开关，看门狗
//DEL 				break;
//DEL 			}
//DEL 			else
//DEL 			if ( 
//DEL 				(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 0)
//DEL 				||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 1) 
//DEL 				||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 2) 
//DEL 				||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 4) 
//DEL 				)
//DEL 			{
//DEL 				ret= 1;//有普通开关
//DEL 				break;
//DEL 			}
//DEL 
//DEL 		}	while(knd != ind);//一直到电源点为止
//DEL 	
//DEL 	}
//DEL 
//DEL 	delete []nd_t;
//DEL 	nd_t = 0;
//DEL 
//DEL 	return ret;
//DEL }

int CNM1::OninitialIsland(int ifd)
{
	//将所有的岛都置为-1
	ChildPtr1	*eq_p, *eq_p0;
	eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((dsbob2->fd_tb->GetTable()+fdph)->r_eq);
	eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
	while(eq_p)
	{
		dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->island = -1;
		eq_p = eq_p->Next;
	}
	::FreeChildPtr(eq_p0);

	return 1;
}

double CNM1::OnGetUnCBAbund(int fdph,int unph,char* oppfdid,double& p_abund,double& q_abund,double& v_abund)
{
	int r_un;
	ChildPtr1	*un_p,*un_p0;

//	int opp_nd,ibs;
	char  left[MAX_ID_DLL];
//	double p_abund,q_abund,v_abund;//对侧的电源的有功和无功
	p_abund=q_abund=v_abund=0;

	//可以根据馈线联络开关关系表直接得到
	r_un = dsbob2->fd_tb->GetDataByPh(fdph)->r_un;
	if (r_un != -1)//区段内有下游开关
	{
		un_p = dsbob2->r_fd_un_tb->FindChildPtr1(r_un);
		un_p0 = un_p; 
		while(un_p)
		{
			//区段内的所有边界开关都需要打开
			if (unph == dsbob2->un_tb->GetDataByPh(un_p->Ptr)->idph)
			{
				strcpy(left , dsbob2->un_tb->GetDataByPh(un_p->Ptr)->left);
				if (strcmp(fdid,left)==0)
				{
					p_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_P;
					q_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_Q;
					v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_V;
					strcpy(oppfdid,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->right);
				}
				else
				{
					p_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_P;
					q_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_Q;
					v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_V;
					strcpy(oppfdid,left);
				}

				break;
			}

			un_p = un_p->Next;
		}
		::FreeChildPtr(un_p0);
	}

	return sqrt(p_abund*p_abund+q_abund*q_abund);
}

int CNM1::OnExeNM1(char * path)
{
	strcpy(pathFlow, path);

	int i;
	//准备工作
	OnSetAbund();//填充联络开关的裕度

	lld->g_cnt_nm1_fd = 0;
	//以大馈线为单位
	int lv_fd = dsbob2->fd_tb->GetCount();
	FD* fd = dsbob2->fd_tb->GetTable();
	for(i = 0; i < lv_fd; i++)
	{
		//全线的N-1
		OnWholeFeederNM1(i);

		//馈线的每一个设备N-1
		OnDevNM1(i);

		//写设备的N-1计算结果
		OnWriteNM1(i);
	}

	delete eq_tb_isolate;

	return 1;
}

//全馈线的N-1
int CNM1::OnWholeFeederNM1(int ifd)
{
	int		soid;
	double dReal,dImp;//馈线的主供电源
	double soReal, soImp;//馈线上的其它电源
	double ldReal, ldImp;//馈线上的其它电源
	double curW;//本条馈线的视在功率
	double tranW;//联络的馈线裕度
	char   unionSO[MAX_UION_ID];//联络的电源点id集合，中间用，分开	
	char   fdid[MAX_ID_DLL];//联络的电源点id集合，中间用，分开	

	double Faultload;	//故障段失电负荷(MW)
	double Trload;		//非故障段需转移负荷(MW)
	double NowTrload;	//非故障段实际转移负荷(MW)
	double PowerLostRatio;//非故障段失电负荷所占总负荷的比例%
//	char Result;			//分断N-1计算结果 
	char LineResult;		//全线N-1计算结果
//	int iCountSect;		//线段的分段数量

	char unionList[4000];

	ChildPtr1	*so_p,*so_p0;
	SO* so = dsbob2->so_tb->GetTable();
	ChildPtr1	*ld_p, *ld_p0;
	LD* ld = dsbob2->ld_tb->GetTable();

	strcpy(fdid,dsbob2->fd_tb->GetDataByPh(ifd)->id);
	int r_so = dsbob2->fd_tb->GetDataByPh(ifd)->r_so;
	if (r_so != -1)//区段内有下游开关
	{
		dReal = dImp = 0;
		soReal = soImp = 0;
		so_p = dsbob2->r_fd_so_tb->FindChildPtr1(r_so);
		so_p0 = so_p; 
		while(so_p)
		{
			soid = so_p->Ptr;//主供电源
			if  (dsbob2->so_tb->GetDataByPh(soid)->slackavr == 4)
			{
				dReal += dsbob2->so_tb->GetDataByPh(soid)->w;
				dImp  += dsbob2->so_tb->GetDataByPh(soid)->r;
			}
			else
			if (dsbob2->so_tb->GetDataByPh(soid)->slackavr == 0)
			{
				soReal += 1000;//平衡节点可以提供无限的电力
				soImp += 1000;
			}
			else
			{
				soReal += dsbob2->so_tb->GetDataByPh(soid)->w;
				soImp += dsbob2->so_tb->GetDataByPh(soid)->r;
			}

			so_p = so_p->Next;
		}
		::FreeChildPtr(so_p0);
	}
	else
	{
		dReal=0;
		dImp=0;
		soReal = 0;
		soImp = 0;
	}

	//应该是用负荷来判断
	ldReal = ldImp = 0;
	int r_ld = dsbob2->fd_tb->GetDataByPh(ifd)->r_ld;
	if (r_ld != -1)//区段内有下游开关
	{
		ld_p = dsbob2->r_fd_ld_tb->FindChildPtr1(r_ld);
		ld_p0 = ld_p;
		while (ld_p)
		{
			soid = ld_p->Ptr;//主供电源
			ldReal += dsbob2->ld_tb->GetDataByPh(soid)->w;
			ldImp += dsbob2->ld_tb->GetDataByPh(soid)->r;

			ld_p = ld_p->Next;
		}
		::FreeChildPtr(ld_p0);
	}

	curW = sqrt(ldReal*ldReal +ldImp*ldImp);//当前电源点的视在功率,返回的是有名值

	unionList[0] = 0;
	if (curW == 0)
	{
		PowerLostRatio=0;//非故障段失电负荷所占总负荷的比例%
		LineResult = '0';
		tranW = 0;
		strcpy(unionList , "-");
	}
	else
	{
		//先判断备用电源的情况
		if ((sqrt(soReal*soReal + soImp * soImp)) > curW)//如果对侧的联络裕度大于需要转供的裕度，整条馈线的n-1可以通过
		{
			PowerLostRatio = 0;//非故障段失电负荷所占总负荷的比例%
			LineResult = '1';
			tranW = curW;
		}
		else
		{
			//查找与本条线连接的其它馈线,以及相关的裕度
			tranW = getUNCB_abun(fdid, unionSO);
			strcpy(unionList, unionSO);
			if ((strlen(unionList) == 0) || (strcmp(unionList, ",") == 0))
				strcpy(unionList, "-");
			else
				lld->delstrright(unionList, 1);

			if ((tranW+ (sqrt(soReal*soReal + soImp * soImp))) > curW)//如果对侧的联络裕度大于需要转供的裕度，整条馈线的n-1可以通过
			{
				PowerLostRatio = 0;//非故障段失电负荷所占总负荷的比例%
				LineResult = '1';
				tranW = curW;
			}
			else
			{
				PowerLostRatio = 100;//非故障段失电负荷所占总负荷的比例%
				LineResult = '0';
				tranW = 0;
			}
		}
	}

	Faultload=0;	//故障段失电负荷(MW)
	Trload=curW;		//非故障段需转移负荷(MW)
	NowTrload=tranW;	//非故障段实际转移负荷(MW)
	
	//全线N-1计算结果
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].id=lld->g_cnt_nm1_fd;
	strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].fdid,fdid);
	strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].secid,"整条线n-1");
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].Faultload = Faultload;
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].Trload=Trload;
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].NowTrload=NowTrload;
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].PowerLostRatio=PowerLostRatio;
	if (strlen(unionList)>0)
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].unionList , unionList);
	else
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].unionList, "其它电源点");

	dsbob2->nm1_result[lld->g_cnt_nm1_fd].secResult='-';
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].LineResult=LineResult;
	strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].transSec , "-");
	strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].lossSec , "-");
	lld->g_cnt_nm1_fd++;

	return 1;	
}

int CNM1::OnWriteNM1(int ifd)
{
	double Faultload;	//故障段失电负荷(MW)
	double Trload;		//非故障段需转移负荷(MW)
	double NowTrload;	//非故障段实际转移负荷(MW)
	double PowerLostRatio;//非故障段失电负荷所占总负荷的比例%
	char Result;		//分断N-1计算结果 
	char LineResult;	//全线N-1计算结果
//	int iCountSect;		//线段的分段数量
	double w,r;

	int i,eqph=-1;
	fdid[0]=0;
//	ChildPtr1	*so_p,*so_p0;
	ChildPtr1	*eq_p, *eq_p0;
//	ChildPtr1	*sec_p, *sec_p0;
//	int ind,iznd;
//	char nd[MAX_ID_DLL],znd[MAX_ID_DLL];

	int sond=-1;// = dsbob2->so_tb->GetDataByPh(soid)->ind;
	FD* fd = dsbob2->fd_tb->GetTable();
	fdph = ifd;//dsbob2->OnGetPhByfdid(fdid);
	strcpy(fdid,dsbob2->fd_tb->GetDataByPh(ifd)->id);

	LD* ld = dsbob2->ld_tb->GetTable();

	//获得所有的设备
	eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((fd+fdph)->r_eq);
	eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
	while(eq_p)
	{
		//故障段失电负荷(MW)
		if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nFaultload > 0)
		{
			Faultload=0;
			for(i=0;i<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nFaultload;i++)
			{
				w = (ld+dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->Faultload[i])->w;
				r = (ld+dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->Faultload[i])->r;
				Faultload += sqrt(w*w+r*r);
			}
		}
		else
			Faultload=0;

		//非故障段需转移负荷(MW)
		if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNeedTrload > 0)
		{
			Trload=0;
			for(i=0;i<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNeedTrload;i++)
			{
				w = (ld+dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NeedTrload[i])->w;
				r = (ld+dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NeedTrload[i])->r;
				Trload += sqrt(w*w+r*r);
			}
		}
		else
			Trload=0;

		//非故障段实际转移负荷(MW)
		if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNowTrload > 0)
		{
			NowTrload=0;
			for(i=0;i<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNowTrload;i++)
			{
				w = (ld+dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NowTrload[i])->w;
				r = (ld+dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NowTrload[i])->r;
				NowTrload += sqrt(w*w+r*r);
			}
		}
		else
			NowTrload=0;

		strcpy(tranfdlist,dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->tranfdlist);
		if ((strlen(tranfdlist)==0) || (strcmp(tranfdlist,",")==0) )
			strcpy(tranfdlist , "-");
		else
			lld->delstrright(tranfdlist,1);

		strcpy(closeunlist,dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->closeunlist);
		if ((strlen(closeunlist)==0) || (strcmp(closeunlist,",")==0) )
			strcpy(closeunlist , "-");
		else
			lld->delstrright(closeunlist,1);

		strcpy(opencblist,dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->opencblist);
		if ((strlen(opencblist)==0) || (strcmp(opencblist,",")==0) )
			strcpy(opencblist , "-");
		else
			lld->delstrright(opencblist,1);

		if (fabs(Trload - NowTrload) < 0.01)
		{
			PowerLostRatio = 0;
			Result = '1';
		}
		else
		{
			if (NowTrload > 0)
				PowerLostRatio=100 * ((Trload-NowTrload)/Trload);//非故障段失电负荷所占总负荷的比例%
			else
				PowerLostRatio=100;

			LineResult = '0';//只要有一个不满足n-1，全线都不满足
			Result = '0';
		}
		
		//分段N-1计算结果
		dsbob2->nm1_result[lld->g_cnt_nm1_fd].id=lld->g_cnt_nm1_fd;
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].fdid,fdid);
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].secid,dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id);
		dsbob2->nm1_result[lld->g_cnt_nm1_fd].Faultload = Faultload;
		dsbob2->nm1_result[lld->g_cnt_nm1_fd].Trload=Trload;
		dsbob2->nm1_result[lld->g_cnt_nm1_fd].NowTrload=NowTrload;
		dsbob2->nm1_result[lld->g_cnt_nm1_fd].PowerLostRatio=PowerLostRatio;
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].unionList , tranfdlist);
		dsbob2->nm1_result[lld->g_cnt_nm1_fd].secResult=Result;
		dsbob2->nm1_result[lld->g_cnt_nm1_fd].LineResult='-';
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].transSec , closeunlist);
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].lossSec , opencblist);
		//strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].zoneLoss , zoneLoss);
		lld->g_cnt_nm1_fd++;

		eq_p = eq_p->Next;
	}
	::FreeChildPtr(eq_p0);

	return 1;
}

//针对一条馈线来做N-1计算
int CNM1::OnDevNM1(int ifd)
{
//	double Faultload;	//故障段失电负荷(MW)
//	double Trload;		//非故障段需转移负荷(MW)
//	double NowTrload;	//非故障段实际转移负荷(MW)
//	double PowerLostRatio;//非故障段失电负荷所占总负荷的比例%
//	char Result;		//分断N-1计算结果 
//	char LineResult;	//全线N-1计算结果
//	int iCountSect;		//线段的分段数量

	int i,j,eqph=-1,ret,r_zn;
	fdid[0]=0;
	ChildPtr1	*so_p,*so_p0;
	ChildPtr1	*eq_p, *eq_p0;
	ChildPtr1	*sec_p, *sec_p0;
	int ind,iznd,oppsite;
//	 nd[MAX_ID_DLL],znd[MAX_ID_DLL];

	int sond=-1;//出线开关靠近母线的那个节点作为电源点
	int firstND = -1;//因为要从出线开关开始搜索，需要在电源点上面一个点起始
	FD* fd = dsbob2->fd_tb->GetTable();
	fdph = ifd;//dsbob2->OnGetPhByfdid(fdid);
	strcpy(fdid,dsbob2->fd_tb->GetDataByPh(ifd)->id);

	int iFind = -1;//判断是否为一个区段内设备
	ZN* zn = dsbob2->zn_tb->GetTable();

	//真实的电源点
	int r_so = dsbob2->fd_tb->GetDataByPh(ifd)->r_so;
	if (r_so != -1)
	{
		so_p = dsbob2->r_fd_so_tb->FindChildPtr1(r_so);
		so_p0 = so_p; 
		while(so_p)
		{
			//从主电源点开始进行搜索
			if (dsbob2->so_tb->GetDataByPh(so_p->Ptr)->slackavr == 4)
			{
				sond= dsbob2->so_tb->GetDataByPh(so_p->Ptr)->ind;
				break;
			}
			so_p = so_p->Next;
		}
  		::FreeChildPtr(so_p0);	
	}
	
	//馈线内无电源点
	if (sond == -1)
	{
		return 1;
	}

	//获得所有的设备
	eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((fd+fdph)->r_eq);
	eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
	while(eq_p)
	{
				//int aalld=eq_p->Ptr;
				// sss[128];
				//strcpy(sss, dsbob2->eq_tb->GetDataByPh(aalld)->id);
				//int iind = dsbob2->eq_tb->GetDataByPh(aalld)->ind;
				//int iznd = dsbob2->eq_tb->GetDataByPh(aalld)->iznd;
				//			if (aalld==12)
		//			{
		//				int debug=0;
		//			}
		
		//如果出线开关发生了故障，那么一直到主干线联络开关（直接联络）故障
		//它自己和直接联络开关是不能作为隔离开关的，也不存在上有回溯的可能

		//联络，出线开关没有故障，不需要分析
		if ( 
			(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 4)
			//||(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type & 16)
			)
		{
			eq_p = eq_p->Next;
			continue;
		}

		iFind = -1;//判断是否是一个区段内的设备，**目前只有馈线段区段
		if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
		{
			//区段内的设备与区段周边开关是绑在一起的，一个故障，全部故障
			r_zn = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->r_zn;
			if ( ((r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1(r_zn)) != -1)
				&& (dsbob2->sec_tb->GetDataByPh(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph)->r0 != 0)//0阻抗支路
				)
			{
				sec_p = dsbob2->r_zn_sec_tb->FindChildPtr1(dsbob2->zn_tb->GetDataByPh(r_zn)->r_sec);
				sec_p0 = sec_p; 
				while(sec_p)
				{
					//设备是否已经处理过了	
					//int dddd = sec_p->Ptr;
					if ( dsbob2->eq_tb->GetDataByPh(dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->lC_zn)->ld_cnt_loss != 0)
					{
						iFind = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->lC_zn;						
						break;
					}
					sec_p = sec_p->Next;
				}
  				::FreeChildPtr(sec_p0);	
			}
			
			//该区段里面已有馈线段处理过，不需要再进行处理，直接将结果赋给该设备
			if (iFind != -1)
			{
				for (i=0; i<max_switch_ld; i++)
				{
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpSwitchload[i] = dsbob2->eq_tb->GetDataByPh(iFind)->UpSwitchload[i];
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpNonOutload[i] = dsbob2->eq_tb->GetDataByPh(iFind)->UpNonOutload[i];
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpPlanSwitchload[i] = dsbob2->eq_tb->GetDataByPh(iFind)->UpPlanSwitchload[i];
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpPlanNonOutload[i] = dsbob2->eq_tb->GetDataByPh(iFind)->UpPlanNonOutload[i];
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->Faultload[i] = dsbob2->eq_tb->GetDataByPh(iFind)->Faultload[i];
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NeedTrload[i] = dsbob2->eq_tb->GetDataByPh(iFind)->NeedTrload[i];
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NowTrload[i] = dsbob2->eq_tb->GetDataByPh(iFind)->NowTrload[i];
				}
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpSwitchload= dsbob2->eq_tb->GetDataByPh(iFind)->nUpSwitchload;
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpNonOutload= dsbob2->eq_tb->GetDataByPh(iFind)->nUpNonOutload;
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpPlanSwitchload= dsbob2->eq_tb->GetDataByPh(iFind)->nUpPlanSwitchload;
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpPlanNonOutload= dsbob2->eq_tb->GetDataByPh(iFind)->nUpPlanNonOutload;
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nFaultload= dsbob2->eq_tb->GetDataByPh(iFind)->nFaultload;
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNeedTrload= dsbob2->eq_tb->GetDataByPh(iFind)->nNeedTrload;
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNowTrload= dsbob2->eq_tb->GetDataByPh(iFind)->nNowTrload;
				strcpy(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->tranfdlist , dsbob2->eq_tb->GetDataByPh(iFind)->tranfdlist);
				strcpy(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->closeunlist , dsbob2->eq_tb->GetDataByPh(iFind)->closeunlist);
				strcpy(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->opencblist , dsbob2->eq_tb->GetDataByPh(iFind)->opencblist);

				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ld_cnt_loss = dsbob2->eq_tb->GetDataByPh(iFind)->ld_cnt_loss;
			}
		}

		//除了馈线段外，其它的所有设备都要一个一个的分析
		if (iFind == -1)
		{
			//对每一个设备进行故障处理
			//初始化设备，把每一个设备所属的岛赋为-1，后面的拓扑将会填写残余岛的编号
			OninitialIsland(ifd);

			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpSwitchload=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpPlanSwitchload=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpNonOutload=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpPlanNonOutload=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nFaultload=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNeedTrload=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNowTrload=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->opencblist[0]=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->tranfdlist[0]=0;
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->closeunlist[0]=0;
			for (i=0; i<max_switch_ld; i++)
			{
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpSwitchload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpNonOutload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpPlanSwitchload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpPlanNonOutload[i] = -1;//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->Faultload[i] = -1;	//故障段失电负荷列表
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NeedTrload[i] = -1;//故障段下游需要转供的负荷列表
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NowTrload[i] = -1;//故障段下游可以转供的负荷列表
			}
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->tranfdlist[0] = 0;//设备发生故障后的转供馈线列表
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->closeunlist[0] = 0;//设备发生故障后的 闭合 联络开关列表
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->opencblist[0] = 0; //设备发生故障后的 打开 分段开关列表

			tranfdlist[0] = 0;//设备发生故障后的转供馈线列表
			closeunlist[0] = 0;//设备发生故障后的 闭合 联络开关列表
			opencblist[0] = 0; //设备发生故障后的 打开 分段开关列表
			eq_tb_isolate->Empty();
			lv_eq_tb_isolate=0;
			i_island = 0;

//			//自己本身就是负荷
//			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20)//负荷
//			{
//				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->Faultload[nFaultload++] = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;
//				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nFaultload++;
//			}
//			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->island = lv_eq_tb_isolate;//故障岛

			//故障设备本身就应该是属于故障岛，岛号为0，lld add 2020-5-11
			eqph = eq_p->Ptr;
			ind =  dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
			iznd =  dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;
			dsbob2->eq_tb->GetDataByPh(eqph)->island = i_island;

			//1.寻找这个设备的周边隔离开关，将开关放到临时的开关表里备用		
			//出线开关、联络开关要特殊处理
			//出线开关故障后，自己要作为隔离开关(一定有隔离刀闸，不一定画上去)，如果没有出线开关，电源点的第一个
			//馈线段要模拟成出线开关,它自己是隔离自己开关，只有这一个是
			if (
				(dsbob2->eq_tb->GetDataByPh(eqph)->type & 16)
				)
			{
				EQ* cbt = new EQ();
				strcpy(cbt->id, dsbob2->eq_tb->GetDataByPh(eqph)->id);
				dsbob2->eq_tb->FindPh(*cbt);

				//修改cbt
				cbt->i_id = lv_eq_tb_isolate++;
				cbt->ind = ind;//外侧开关，从要扩展的设备看上去
				cbt->iznd = iznd;//内侧开关
				eq_tb_isolate->Append(*cbt);
				delete cbt;
			}
			else
			{
				//	//从该设备的两个节点开始搜索，把停电设备的周边隔离开关选出来，存到eq_tb_isolate里面
				//不考虑母线和三通开关等多节点设备，假设最多两个节点，从这两个节点开始向外扩展搜索
				//把设备的周边隔离开关选出来，存到eq_tb_isolate里面
				if (ind != -1)
					OnAnalyseIsolateCB(eqph, ind, iznd);
				if (iznd != -1)
					OnAnalyseIsolateCB(eqph, iznd, ind);
			}
			i_island++;//故障的负荷已经处理，为0，后面节点上游1，和下游2~的问题

			//2.走一遍全拓扑，通过eq_tb_isolate将岛分离出来
			//第0个岛为故障岛
			//此后的步骤需要将每个元件的岛号写入，便于进行潮流计算，lld 2020-4-29
			ret = OnGetIsland(eqph,sond);

			//3.统计每个岛的情况，包括含电源点的岛，不含电源点的岛，含联络开关的岛
			ret = OnAnalyseIsland(eqph,fdph);

			//计算每个岛的潮流，lld add 2020-4-27
			//计算残余网络的潮流，每个设备写一个潮流计算结果：设备编号_pf_date.pf
			//先把岛内的设备全部转换成相关的元件，并将元件赋予相应的岛号
			if (pathFlow[0] != 0)
			{
				char  fileName[128];
				int ibs, zbs;
				//将每个设备的岛号清零
				OninitialIsland(ifd);
				for (i = 0;i < i_island-2;i++)
				{
					if ((islandlist[i].unCount == 0)
						|| (islandlist[i].ldCount == 0)
						|| (islandlist[i].secCount == 0))
						continue;

					for (j = 0;j < islandlist[i].soCount;j++)
					{
						ibs = dsbob2->so_tb->GetDataByPh(islandlist[i].island_solist[j].soptr)->ibs;
						dsbob2->bs_tb->GetDataByPh(ibs)->iisland = i;
					}
					for (j = 0;j < islandlist[i].unCount;j++)
					{
						ibs = dsbob2->nd_tb->GetDataByPh(islandlist[i].island_unlist[j].nd)->ibs;
						dsbob2->bs_tb->GetDataByPh(ibs)->iisland = i;

						dsbob2->bs_tb->GetDataByPh(ibs)->q |= BS_UNION;//联络开关准备做残余网络的电源点

						//将联络开关模拟成电源点
					}
					for (j = 0;j < islandlist[i].ldCount;j++)
					{
						ibs = dsbob2->ld_tb->GetDataByPh(islandlist[i].island_ldlist[j].ld_ptr)->ibs;
						dsbob2->bs_tb->GetDataByPh(ibs)->iisland = i;
					}
					for (j = 0;j < islandlist[i].secCount;j++)
					{
						ibs = dsbob2->sec_tb->GetDataByPh(islandlist[i].island_seclist[j].sec_ptr)->ibs;
						dsbob2->bs_tb->GetDataByPh(ibs)->iisland = i;
						zbs = dsbob2->sec_tb->GetDataByPh(islandlist[i].island_seclist[j].sec_ptr)->zbs;
						dsbob2->bs_tb->GetDataByPh(zbs)->iisland = i;
					}

					ExecuteIslandPwrflow(i,1);
					//写设备文件
					strcpy(fileName, pathFlow);
					strcat(fileName, dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id);
					strcat(fileName, ".eFile");
					dsbob2->WriteOutfile_pf(fileName,-1);
				}
			}

			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
			{//馈线已处理
				dsbob2->sec_tb->GetDataByPh(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph)->lC_zn = eq_p->Ptr;
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ld_cnt_loss = 1;
			}
		}
		eq_p = eq_p->Next;
	}
	::FreeChildPtr(eq_p0);

	return 1;	
}

//隔离后的岛，从电源点开始搜索，包括死岛和活岛
//速断点要找出来，速断点前面的速断，后面的有切换时间
int CNM1::OnGetIsland(int eqph,int sond)
{
	int lv_nd = dsbob2->nd_tb->GetCount();
	if(lv_nd == 0)
		return 0;

	int i,ret=-1,iFind=-1;
	int			ind, knd, kknd,i_id_eq,ldph;
	ChildPtr1	*eq_p, *eq_p0;
	char eqid[MAX_ID_DLL];
//	char ldid[MAX_ID_DLL];

	int UpStreamload[200];//上游负荷的物理号
	int nUpStreamload=0;

	int			extndcount=0;//扩展节点数
	int			extnd[100];//扩展节点

	ND* nd_t = new ND[lv_nd];
	dsbob2->nd_tb->CopyTable(nd_t);//得到一个新的节点表，以免和原来的冲突

	//将所有的临时接点设为已处理
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

	ind = sond;//母线的起始节点编号
	//电源点做起始节点搜索到的设备，它一定可以承担，因为负荷变小了
	extnd[extndcount++] = ind;
	
	//之前已经将所有的节点都置为未处理
	if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd_t+ind)->q &= (~ND_UNPROC);

		while( extndcount >= 1 )//这个循环完成一个区段的搜索
		{
			ind = extnd[--extndcount];//每次都取最后的那个节点进行扩展
			knd = ind;//当前节点赋给一个临时的变量
			(nd_t+knd)->fch = ind; //当前节点号赋给前向链 
			(nd_t+ind)->bch = knd;//从电源点开始开始进行,将电源点作为上一级记录下来
			(nd_t+knd)->q &= (~ND_UNPROC);
			while (1)//还有扩展开关没有处理
			{
				//knd = ind;//当前节点赋给一个临时的变量
				eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
				eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
				while(eq_p)
				{
					if(knd == dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind)
					{
						kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;
					}
					else
					{
						kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
					}

					i_id_eq = eq_p->Ptr;
					//联络开关
					if (dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 4)
					{
						dsbob2->eq_tb->GetDataByPh(i_id_eq)->island = i_island;
					}

					//负荷可以切换
					if ((i_island == 1) && (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20))
					{
						dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->island = i_island;
						UpStreamload[nUpStreamload++] = dsbob2->eq_tb->GetDataByPh(i_id_eq)->idph;
					}

					//负荷
					if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20)
						dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->island = i_island;

					if ((kknd==-1) || !((nd_t+kknd)->q & ND_UNPROC))
					{
						eq_p = eq_p->Next;
						continue;					
					}

					//如果是隔离设备（开关）
					strcpy(eqid,dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id);
					if ( (iFind == -1) && ( (ret = OnGetDevByID(eq_tb_isolate,eqid)) != -1 ) )
					{
						//第一次进，一定是故障区段的首端开关
						iFind = kknd;
						//准备开新的岛
						for (i = 0; i < lv_eq_tb_isolate; i++)
						{
							if ((eq_tb_isolate->GetDataByPh(i)->i_id == ret)
								&& !(eq_tb_isolate->GetDataByPh(i)->type & 16)//不能是出线开关
								)
							{
//								if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==2)
//									dsbob2->eq_tb->GetDataByPh(eqph)->q |= Q_EQ_UP_DISCONNECTOR;
								continue;
							}

							(nd_t+eq_tb_isolate->GetDataByPh(i)->ind)->q &= (~ND_UNPROC);
							//eq_tb_isolate->GetDataByPh(i)->island = i_island;
							extnd[extndcount++] = eq_tb_isolate->GetDataByPh(i)->iznd;

							strcat(opencblist,eq_tb_isolate->GetDataByPh(i)->id);			
							strcat(opencblist,"|");							
						}

						eq_p = eq_p->Next;
						continue;					
					}
					if (kknd != -1)
					{
						//process this opposite nd
						if((nd_t+kknd)->q & ND_UNPROC)
						{
							//将扩展的前向链 设成 找到的新节点的前向链
							(nd_t+kknd)->fch = (nd_t+knd)->fch;
							//新节点 设成 扩展节点的前向链
							(nd_t+knd)->fch = kknd;
							//将这个节点的上一级记录下来，准备回溯
							(nd_t+kknd)->bch = knd;
							
							//将找到的新节点设成已处理
							(nd_t+kknd)->q &= (~ND_UNPROC);
						}
					}
					dsbob2->eq_tb->GetDataByPh(i_id_eq)->island = i_island;	

					eq_p = eq_p->Next;
				}
				::FreeChildPtr(eq_p0);
				//move to next nd
				knd = (nd_t+knd)->fch;

				if(knd == ind)
				{
					i_island++;	
					break;
				}
			}
		}
	}

	//找到了节点
	int iRelay=-1;//上游速断开关
	int iSwitch=-1;//上游可以切换开关
	
	if (iFind != -1)
	{
		knd = iFind;
		do
		{
			ind = knd;//本次开关的节点号
			knd = (nd_t+knd)->bch;//得到开关的上级节点号

			i_id_eq = OnGetDevByNDs(dsbob2,ind,knd);
			//如果上游有孤岛
			if (dsbob2->eq_tb->GetDataByPh(i_id_eq)->island == 0)
			{
				break;
			}

			if ( !(dsbob2->eq_tb->GetDataByPh(i_id_eq)->q & Q_BREAKERFEEDER)
				&& ((dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 3)//熔断器
				||(dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 5)) //带保护的开关
				)
			{
				//速断开关靠近故障侧的节点
				if((iRelay==-1) && (ind != iFind))//第一个开关本身自己就是带保护的开关，不能把自己设成
					iRelay = ind;
			}
			if ( !(dsbob2->eq_tb->GetDataByPh(i_id_eq)->q & Q_BREAKERFEEDER)
				&& 
				((dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 0)//只要不是刀闸就行
				||(dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 1)//只要不是刀闸就行
				||(dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 3)//只要不是刀闸就行
				||(dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 5))//只要不是刀闸就行
				)
			{
				//速断开关靠近故障侧的节点
				if(iSwitch==-1)
					iSwitch = ind;
			}

		}	while(knd != ind);//一直到电源点为止	
	}

	//存在速断开关
	if (iRelay != -1)
	{
		OnGetUpIsland(sond,iRelay,eqph,0);
//
//		//用速断开关的靠近电源点的节点进行搜索，得到不停电的负荷
//		//将所有的临时接点设为已处理
//		for(i = 0; i < lv_nd; i++)
//		{
//			(nd_t+i)->q |= ND_UNPROC;
//		}
//
//		ind = sond;//母线的起始节点编号
//		//之前已经将所有的节点都置为未处理
//		if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
//		{
//			(nd_t+ind)->q &= (~ND_UNPROC);
//
//			knd = ind;//当前节点赋给一个临时的变量
//			(nd_t+knd)->fch = ind; //当前节点号赋给前向链 
////			(nd_t+ind)->bch = knd;//从电源点开始开始进行,将电源点作为上一级记录下来
//			(nd_t+knd)->q &= (~ND_UNPROC);
//			while (1)//还有扩展开关没有处理
//			{
//				eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
//				eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
//				while(eq_p)
//				{
//					if(knd == dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind)
//					{
//						kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
//					}
//
//					if ((kknd == iRelay) || (!((nd_t+kknd)->q & ND_UNPROC)))
//					{
//						eq_p = eq_p->Next;
//						continue;					
//					}
//
//					if ( dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20 )//负荷可以切换
//					{
//						dsbob2->eq_tb->GetDataByPh(eqph)->UpNonOutload[dsbob2->eq_tb->GetDataByPh(eqph)->nUpNonOutload++] = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;
//
//						eq_p = eq_p->Next;
//						continue;					
//					}
//
//					//process this opposite nd
//					if( (kknd != -1) && ((nd_t+kknd)->q & ND_UNPROC) )
//					{
//						//将扩展的前向链 设成 找到的新节点的前向链
//						(nd_t+kknd)->fch = (nd_t+knd)->fch;
//						//新节点 设成 扩展节点的前向链
//						(nd_t+knd)->fch = kknd;
//						//将这个节点的上一级记录下来，准备回溯
////						(nd_t+kknd)->bch = knd;
//						
//						//将找到的新节点设成已处理
//						(nd_t+kknd)->q &= (~ND_UNPROC);
//					}
//					//只能是第1个岛的情况
//					eq_p = eq_p->Next;
//				}
//				::FreeChildPtr(eq_p0);
//
//				//move to next nd
//				knd = (nd_t+knd)->fch;
//				if(knd == ind)
//				{
//					break;
//				}
//			}
//		}
	}

	if (iSwitch != -1)
	{
		OnGetUpIsland(sond,iSwitch,eqph,1);
	}

	int j = 0;
	//上游负荷
	for (i=0; i<nUpStreamload;i++)
	{
		ldph = UpStreamload[i];
		iFind=-1;
		for (j=0; j<dsbob2->eq_tb->GetDataByPh(eqph)->nUpNonOutload;j++)
		{
			if (ldph == dsbob2->eq_tb->GetDataByPh(eqph)->UpNonOutload[j])
			{
				iFind = 1;
				break;
			}
		}

		if (iFind == -1)
			dsbob2->eq_tb->GetDataByPh(eqph)->UpSwitchload[dsbob2->eq_tb->GetDataByPh(eqph)->nUpSwitchload++] = ldph;

		iFind=-1;
		for (j=0; j<dsbob2->eq_tb->GetDataByPh(eqph)->nUpPlanNonOutload;j++)
		{
			if (ldph == dsbob2->eq_tb->GetDataByPh(eqph)->UpPlanNonOutload[j])
			{
				iFind = 1;
				break;
			}
		}

		if (iFind == -1)
			dsbob2->eq_tb->GetDataByPh(eqph)->UpPlanSwitchload[dsbob2->eq_tb->GetDataByPh(eqph)->nUpPlanSwitchload++] = ldph;
	
	}	
	
	delete []nd_t;
	nd_t = 0;

	return 1;	
}

//搜索每一个设备的隔离开关，只能是开关，起隔离作用的所有开关
//出线开关故障，只能把开关作为隔离开关处理，不能向下扩展
int CNM1::OnAnalyseIsolateCB(int eqid,int start,int oppsite)
{
	int i,ret=-1,iFind=-1;
	int			ind,knd, kknd,eq_id;
	ChildPtr1	*eq_p, *eq_p0;

//	int ldph;
//	 ldid[MAX_ID_DLL];
	int lv_nd = dsbob2->nd_tb->GetCount();
	if(lv_nd == 0)
	{
		return 0;
	}
	ND* nd_t = new ND[lv_nd];
	dsbob2->nd_tb->CopyTable(nd_t);//得到一个新的节点表，以免和原来的冲突

	//将所有的临时接点设为已处理
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}
	if (oppsite!=-1)
		(nd_t+oppsite)->q &= (~ND_UNPROC);

	ind = start;//母线的起始节点编号
	//之前已经将所有的节点都置为未处理
	if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd_t+ind)->q &= (~ND_UNPROC);

		knd = ind;//当前节点赋给一个临时的变量
		(nd_t+knd)->fch = knd; //当前节点号赋给前向链 
		(nd_t+ind)->bch = knd;//从电源点开始开始进行,将电源点作为上一级记录下来
     	while (1)//还有扩展开关没有处理
		{
			eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
			eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
			while(eq_p)
			{
				eq_id = eq_p->Ptr;
				if(knd == dsbob2->eq_tb->GetDataByPh(eq_id)->ind)
					kknd = dsbob2->eq_tb->GetDataByPh(eq_id)->iznd;
				else
					kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;

				if ( (kknd != -1) && ((nd_t+kknd)->q & ND_UNPROC) )
				{
					if ( 
						(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 3)//熔断器
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 5) //带保护的开关
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 0) 
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 1) 
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 2) 
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 4) 
						)
					{
						//记录下来
						EQ* cbt = new EQ();
						strcpy(cbt->id,dsbob2->eq_tb->GetDataByPh(eq_id)->id);
						dsbob2->eq_tb->FindPh(*cbt);
					
						//修改cbt
						cbt->i_id = lv_eq_tb_isolate++;
						cbt->ind = knd;//外侧开关，从要扩展的设备看上去
						cbt->iznd = kknd;//内侧开关
						eq_tb_isolate->Append(*cbt);
						delete cbt;
					}
					else
					{
						//将扩展的前向链 设成 找到的新节点的前向链
						(nd_t+kknd)->fch = (nd_t+knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd_t+knd)->fch = kknd;
						//将这个节点的上一级记录下来，准备回溯
						(nd_t+kknd)->bch = knd;
						
						//将找到的新节点设成已处理
						(nd_t+kknd)->q &= (~ND_UNPROC);
						dsbob2->eq_tb->GetDataByPh(eq_id)->island = i_island;	
					}
				}

				if (dsbob2->eq_tb->GetDataByPh(eq_id)->type == 20)//负荷
				{
					dsbob2->eq_tb->GetDataByPh(eqid)->Faultload[dsbob2->eq_tb->GetDataByPh(eqid)->nFaultload++] = dsbob2->eq_tb->GetDataByPh(eq_id)->idph;
					dsbob2->eq_tb->GetDataByPh(eq_id)->island = i_island;	
				}

				eq_p = eq_p->Next;
			}			
			::FreeChildPtr(eq_p0);

			//move to next nd
			knd = (nd_t+knd)->fch;
			if(knd == ind)
			{
				break;
			}
		}
	}
	delete[] nd_t;
	nd_t = 0;

	return 1;
}

//给定电源点和一个节点，搜索全部的设备
int CNM1::OnGetUpIsland(int sond,int end_nd,int eqph,int type)
{
	int i,ret=-1,iFind=-1;
	int			ind, knd, kknd;
	ChildPtr1	*eq_p, *eq_p0;
//	 eqid[MAX_ID_DLL];
//	 ldid[MAX_ID_DLL];

	int lv_nd=dsbob2->nd_tb->GetCount();
	ND* nd_t = new ND[lv_nd];
	dsbob2->nd_tb->CopyTable(nd_t);//得到一个新的节点表，以免和原来的冲突	//用速断开关的靠近电源点的节点进行搜索，得到不停电的负荷
	//将所有的临时接点设为已处理
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

	ind = sond;//母线的起始节点编号
	//之前已经将所有的节点都置为未处理
	if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd_t+ind)->q &= (~ND_UNPROC);

		knd = ind;//当前节点赋给一个临时的变量
		(nd_t+knd)->fch = ind; //当前节点号赋给前向链 
//			(nd_t+ind)->bch = knd;//从电源点开始开始进行,将电源点作为上一级记录下来
		(nd_t+knd)->q &= (~ND_UNPROC);
		while (1)//还有扩展开关没有处理
		{
			eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
			eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
			while(eq_p)
			{
				if(knd == dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind)
				{
					kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
				}

				if ( dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20 )//负荷可以切换
				{
					if (type==0)
						dsbob2->eq_tb->GetDataByPh(eqph)->UpNonOutload[dsbob2->eq_tb->GetDataByPh(eqph)->nUpNonOutload++] = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;
					if (type==1)
						dsbob2->eq_tb->GetDataByPh(eqph)->UpPlanNonOutload[dsbob2->eq_tb->GetDataByPh(eqph)->nUpPlanNonOutload++] = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;

					eq_p = eq_p->Next;
					continue;					
				}

				if ((kknd == -1) || (kknd == end_nd) || (!((nd_t + kknd)->q & ND_UNPROC)))
				{
					eq_p = eq_p->Next;
					continue;
				}

				//process this opposite nd
				if( (kknd != -1) && ((nd_t+kknd)->q & ND_UNPROC) )
				{
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd_t+kknd)->fch = (nd_t+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd_t+knd)->fch = kknd;
					//将这个节点的上一级记录下来，准备回溯
//						(nd_t+kknd)->bch = knd;
					
					//将找到的新节点设成已处理
					(nd_t+kknd)->q &= (~ND_UNPROC);
				}
				//只能是第1个岛的情况
				eq_p = eq_p->Next;
			}
			::FreeChildPtr(eq_p0);

			//move to next nd
			knd = (nd_t+knd)->fch;
			if(knd == ind)
			{
				break;
			}
		}
	}

	delete[] nd_t;
	return 1;
}

//int CNM1::OnIslandFlow(int eqph, int fdph)
//{
//	//岛已经划分出来了，负荷点和电源点也已经有了
//
//
//	return 1;
//}

//分析活岛和死岛，获得可以转供的负荷
int CNM1::OnAnalyseIsland(int eqph,int fdph)
{
	int ii=0,ph,knd,ind,iznd,pcb_temp,i,j;
	ChildPtr1	*eq_p, *eq_p0;
	double w,r,pq_abund,p_abund,q_abund,v_abund;
	char fdid_abund[MAX_ID_DLL];
	int bFind;
	int ldCount,cpCount,unCount,soCount, secCount;//一个岛内的数量

	int lv_cp=0;
	if (lld->mx_cp>0)
	{
		int lv_cp =  dsbob2->cp_tb->GetCount(); 
	}
	int lv_so =  dsbob2->so_tb->GetCount(); 
	SO* so = dsbob2->so_tb->GetTable(); 
	ND* nd = dsbob2->nd_tb->GetTable(); 
	for(ii=2;ii<i_island;ii++)
	{//0是故障岛，死岛；1是上游岛，从2开始是下游岛
		g_island = ii-2;
		ldCount=cpCount=unCount=soCount=secCount=0;

		eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((dsbob2->fd_tb->GetTable()+fdph)->r_eq);
		eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
		while(eq_p)
		{
			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->island != ii)
			{
				eq_p = eq_p->Next;
				continue;	
			}

			int aalld = eq_p->Ptr;
			//if ( (aalld==21)|| (aalld == 40) || (aalld == 42) || (aalld == 46) || (aalld == 52))
			////if (strcmp(dsbob2->eq_tb->GetDataByPh(aalld)->id, "6bdb2928aee648dab330f683d7f29e67") == 0)
			//{
			//	int deb = 0;
			//}
			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 10)//线段
			{
				ph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;
				islandlist[g_island].island_seclist[secCount].sec_ptr = ph;
				secCount++;
			}

			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20)//负荷
			{
				ph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;

				//应该转供的负荷
				dsbob2->eq_tb->GetDataByPh(eqph)->NeedTrload[dsbob2->eq_tb->GetDataByPh(eqph)->nNeedTrload++] = ph;

				islandlist[g_island].island_ldlist[ldCount].ld_ptr = ph;
				w = dsbob2->ld_tb->GetDataByPh(ph)->w;
				r = dsbob2->ld_tb->GetDataByPh(ph)->r;
				islandlist[g_island].island_ldlist[ldCount].p_ld = w;
				islandlist[g_island].island_ldlist[ldCount].q_ld = r;
				islandlist[g_island].island_ldlist[ldCount].pq_ld = sqrt(w * w + r * r);

				//处理双电源问题
				knd = dsbob2->ld_tb->GetDataByPh(ph)->ind;//负荷对应的节点号
				if (((dsbob2->nd_tb->GetDataByPh(knd)->q) & ND_SO))
					//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//多电源
				{
					islandlist[g_island].island_ldlist[ldCount].soSum = 2;
				}//多电源在计算停电损失的时候要考虑到
				else
					islandlist[g_island].island_ldlist[ldCount].soSum = 1;

				islandlist[g_island].island_ldlist[ldCount].nd = knd;
				ldCount++;
			}

			ind = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
			iznd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;
		
			if ( ((ind!=-1) && ((nd + ind)->q & ND_SO)) || ((iznd != -1) && ((nd + iznd)->q & ND_SO)))
			{
				//对so进行全循环，因为so的数量很少,可能是分布式电源
				for (j = 0; j < lv_so; j++)
				{
					if ( ((so + j)->ind == ind) || ((so + j)->ind == iznd))
					{
						islandlist[g_island].island_solist[soCount].soptr = j;
						islandlist[g_island].island_solist[soCount].nd = (so + j)->ind;
						islandlist[g_island].island_solist[soCount].p = (so + j)->w;
						islandlist[g_island].island_solist[soCount].q = (so + j)->r;
						strcpy(islandlist[g_island].island_solist[soCount].secid, (so + j)->fdid);
						islandlist[g_island].island_solist[soCount].pq = sqrt((so + j)->w*(so + j)->w + (so + j)->r*(so + j)->r);
						soCount++;
						break;
					}
				}
			}

			if ( (lld->mx_cp>0)
				&& (
				(((ind != -1) && ((nd + ind)->q & ND_CP)) 
					|| ((iznd != -1) && ((nd + iznd)->q & ND_CP)))
					)
				)
//				if ( (((nd+ind)->q & ND_CP) || ((nd + iznd)->q & ND_CP) ) && (lld->mx_cp>0) )
			{
				//对cp进行全循环，因为cp的数量很少
				for (j=0; j<lv_cp; j++)
				{
					if (( (dsbob2->cp_tb->GetTable()+j)->ind == ind)
						|| ((dsbob2->cp_tb->GetTable() + j)->ind == iznd) )
					{
						islandlist[g_island].island_cplist[cpCount].cp_ptr = j;
						islandlist[g_island].island_cplist[cpCount].nd = dsbob2->cp_tb->GetTable()->ind;
						islandlist[g_island].island_cplist[cpCount].r = dsbob2->cp_tb->GetTable()->r;
//							islandlist[g_island].island_cplist[cpCount].island = ii;
						cpCount++;
						break;
					}
				}
			}			

			if	(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 4)//联络开关
			{ 
				ph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;
				ind = dsbob2->cb_tb->GetDataByPh(ph)->ind;//负荷对应的节点号
				iznd = dsbob2->cb_tb->GetDataByPh(ph)->iznd;//负荷对应的节点号
				if (ind >= 0)
					knd = ind;
				else
					knd = iznd;

				bFind = 0;
				pq_abund = OnGetUnCBAbund(fdph,ph,fdid_abund,p_abund,q_abund,v_abund);
				//如果联络开关的母线和以前的联络开关母线一样
				for (i=0;i<unCount;i++)
				{
					//比较联络开关裕度，选最大的裕度
					pcb_temp = islandlist[g_island].island_unlist[i].nd;
					if ( (nd+pcb_temp)->ibs == (nd+knd)->ibs )
					{
						//替换
						if (pq_abund > islandlist[g_island].island_unlist[i].pq_abund)
						{
							islandlist[g_island].island_unlist[i].nd = knd;
							strcpy(islandlist[g_island].island_unlist[i].secid , fdid_abund);

							islandlist[g_island].island_unlist[i].p_abund=p_abund;
							islandlist[g_island].island_unlist[i].q_abund=q_abund;
							islandlist[g_island].island_unlist[i].pq_abund=pq_abund;
							islandlist[g_island].island_unlist[i].v_abund=v_abund;
							islandlist[g_island].island_unlist[i].cbptr = ph;
						}
						bFind = 1;
					}
				}

				if (!bFind)
				{
					islandlist[g_island].island_unlist[unCount].nd = knd;
					strcpy(islandlist[g_island].island_unlist[unCount].secid , fdid_abund);

					islandlist[g_island].island_unlist[unCount].p_abund=p_abund;
					islandlist[g_island].island_unlist[unCount].q_abund=q_abund;
					islandlist[g_island].island_unlist[unCount].pq_abund=pq_abund;
					islandlist[g_island].island_unlist[unCount].v_abund=v_abund;
					islandlist[g_island].island_unlist[unCount].cbptr = ph;

					unCount++;						
				}
			}
			eq_p = eq_p->Next;
		}
		::FreeChildPtr(eq_p0);

		islandlist[g_island].soCount = soCount;
		islandlist[g_island].ldCount = ldCount;
		islandlist[g_island].unCount = unCount;
		islandlist[g_island].cpCount = cpCount;
		islandlist[g_island].secCount = secCount;
		islandlist[g_island].validate = 0;
		islandlist[g_island].canTrans = 1;//先假设能转
	}

	g_island=i_island-2;

	//光给出结论还不行，要把能转供和不能转供的岛输出出来
	if (AnanyseIsland() == 0)//初步分析都不能通过，说明是孤岛
		return 0;

	if (AnanyseResult(eqph) == 0)//输出残余网络的分析结果
		return 0;

	return 1;
}

//分析残余网络中的岛
int CNM1::AnanyseIsland()
{
	int i;
	//逐岛进行检查
	i = 0;
	while (1)
	{
		if (i>=g_island)
			break;

		if (!isNeedDivide(i))//判断是否需要切分岛
		{
			i++;//不需要切分，包括两种情况：（1）没电源点；（2）单个已经可以转供
			continue;
		}

		//切分新的岛，新的岛加在队列的最后
		//切分前，联络开关的裕度已经排好了序
		//只有切分的两个岛完全没有孤立负荷才能成功
		if (!divideIsland(i))//切分不成功，没有合适的断开开关可以转供
		{
			islandlist[i].canTrans = 0;//岛内只能转供部分负荷
			islandlist[i].validate = 1;
			i++;
		}
	}	

	return 1;
}

//输出残余网络的分析结果，残余网络分析完后，将现有的网络分成了一个个的岛
//单个岛进行分析时，对于不能转供的岛，可以分析出部分能转供的负荷
int CNM1::AnanyseResult(int eqph)
{
	int i,j;
	
	int downstream_ld_cnt_t;
	double downstream_ld_pq_t;
	downstream_ld_cnt=0;//区段下游的负荷个数
	downstream_ld_pq=0;//区段下游的负荷视在功率
	downstream_multi_ld_cnt=0;//区段下游多电源的个个数
	downstream_multi_ld_pq=0;//区段下游多电源的视在功率
	downstream_trans_ld_cnt=0;//区段下游可以转的负荷个数
	downstream_trans_ld_pq=0;//区段下游可以转的负荷视在功率
	unfd_idList[0]='\0';//下游可以转供的联络开关
	un_idList[0]=0;

	transSec[0]=0;
	lossSec[0]=0;
//	 l_transSec[FEMA_ID_LEN];
//	 l_lossSec[FEMA_ID_LEN];
	SEC* sec = dsbob2->sec_tb->GetTable();

	int downstream_ld_cnt_t_;//不能全部转供情况下
	double downstream_ld_pq_t_;//
	int lossSec_t[MAX_SEC];
	int transSec_t[MAX_SEC];
	int cntTransSec, cntLossSec;

	//逐岛进行统计
	for (i=0; i<g_island; i++)
	{
		//负荷（总）
		downstream_ld_cnt_t=downstream_ld_pq_t=0;
		
		downstream_ld_cnt_t=islandlist[i].ldCount;
		downstream_ld_cnt += downstream_ld_cnt_t;
		for(j=0; j<downstream_ld_cnt_t; j++)
		{
			downstream_ld_pq_t += islandlist[i].island_ldlist[j].pq_ld;
			if (islandlist[i].island_ldlist[j].soSum>1)//多电源
			{
				downstream_multi_ld_cnt++;
				downstream_multi_ld_pq += islandlist[i].island_ldlist[j].pq_ld;
			}
		}
		downstream_ld_pq += downstream_ld_pq_t;

		//分两种情况，一种是可以全部转供
		if (islandlist[i].canTrans)//岛能转供,提供可以转供的负荷（含多电源）
		{
			downstream_trans_ld_cnt += downstream_ld_cnt_t;
			downstream_trans_ld_pq += downstream_ld_pq_t;

			//得到可以转供的联络开关
			strcat(unfd_idList,islandlist[i].island_unlist[0].secid);
			strcat(unfd_idList,",");
			strcat(un_idList,dsbob2->cb_tb->GetDataByPh(islandlist[i].island_unlist[0].cbptr)->id);			
			strcat(un_idList,"|");

			for(j=0; j<downstream_ld_cnt_t; j++)
			{
				downstream_ld_pq_t += islandlist[i].island_ldlist[j].pq_ld;

				dsbob2->eq_tb->GetDataByPh(eqph)->NowTrload[dsbob2->eq_tb->GetDataByPh(eqph)->nNowTrload++] = islandlist[i].island_ldlist[j].ld_ptr;

				if (islandlist[i].island_ldlist[j].soSum>1)//多电源
				{
					downstream_multi_ld_cnt++;
					downstream_multi_ld_pq += islandlist[i].island_ldlist[j].pq_ld;
				}
			}

//			//将联络开关输出
//			
//			if (islandlist[i].secCount > 0)
//			{
//				//输出岛内的线段
//				l_transSec[0]=0;
//				for(j=0; j<islandlist[i].secCount; j++)
//				{
//					strcat(l_transSec,lld->IntToString(islandlist[i].island_seclist[j].sec_ptr));
//					strcat(l_transSec,"|");
//				}
//				//滤掉最后的那个
//				lld->delstrright(l_transSec,1,l_transSec);
//				strcat(transSec,l_transSec);
//				strcat(transSec,",");
//			}
		}
		else
		{
			//岛内部分可以转供，将可以转供的线段和不可以转供的线段划分出来
			//用最大裕度的联络开关进行拓扑分析	
			
			//必须要初始化
			downstream_ld_cnt_t_=downstream_ld_pq_t_=cntTransSec=cntLossSec=0;
			if (getPartTrans(eqph,i,downstream_ld_cnt_t_,downstream_ld_pq_t_,transSec_t,lossSec_t,cntTransSec,cntLossSec))
			{
				downstream_trans_ld_cnt += downstream_ld_cnt_t_;
				downstream_trans_ld_pq += downstream_ld_pq_t_;
				
				if (downstream_ld_cnt_t_ > 0)
				{
					//得到可以转供的联络开关
					strcat(unfd_idList,islandlist[i].island_unlist[0].secid);
					strcat(unfd_idList,",");

					strcat(un_idList,dsbob2->cb_tb->GetDataByPh(islandlist[i].island_unlist[0].cbptr)->id);			
					strcat(un_idList,"|");
				}

//				l_transSec[0]=0;
//				if (cntTransSec>0)
//				{
//					for(j=0; j<cntTransSec; j++)
//					{
//						strcat(l_transSec,lld->IntToString(transSec_t[j]));
//						strcat(l_transSec,"|");
//					}
//					//滤掉最后的那个
//					lld->delstrright(l_transSec,1,l_transSec);
//					strcat(transSec,l_transSec);
//					strcat(transSec,",");
//				}
//				//输出岛内的线段
//				if (cntLossSec > 0)
//				{
//					l_lossSec[0]=0;
//					for(j=0; j<cntLossSec; j++)
//					{
//						strcat(l_lossSec,lld->IntToString(lossSec_t[j]));
//						strcat(l_lossSec,"|");
//					}
//					//滤掉最后的那个
//					lld->delstrright(l_lossSec,1,l_lossSec);
//					strcat(lossSec,l_lossSec);
//					strcat(lossSec,",");
//				}

			}
			else
			{//全部不能转供
				for(j=0; j<downstream_ld_cnt_t; j++)
				{
					downstream_ld_pq_t += islandlist[i].island_ldlist[j].pq_ld;

//					//一定损失的负荷加到负荷列表里面
//					ra_feeder[ra_cnt].ld_id = islandlist[i].island_ldlist[j].ld_ptr;//负荷的物理号
//					ra_feeder[ra_cnt].up_cb = up_cb;//上游开关的性质，对故障切换有用
//					ra_feeder[ra_cnt].out = 1;//负荷损失
//					ra_feeder[ra_cnt].swit = 0;//负荷不能切换
//					ra_feeder[ra_cnt].tran = 0;//负荷能转供
//					ra_feeder[ra_cnt].sec_id = sec_ana;//负荷对应的故障馈线物理号（此时物理逻辑统一）
//					ldlist[ra_feeder[ra_cnt].ld_id] = 1;//负荷已处理
//					ra_cnt++;					
				}


//				//输出岛内的线段
//				if (islandlist[i].secCount > 0)
//				{
//					l_lossSec[0]=0;
//					for(j=0; j<islandlist[i].secCount; j++)
//					{
//						strcat(l_lossSec,lld->IntToString(islandlist[i].island_seclist[j].sec_ptr));
//						strcat(l_lossSec,"|");
//					}
//					//滤掉最后的那个
//					lld->delstrright(l_lossSec,1,l_lossSec);
//					strcat(lossSec,l_lossSec);
//					strcat(lossSec,",");
//				}
			}
		}
	}

	//获取转供的联络线、联络开关、切断开关
	strcpy(dsbob2->eq_tb->GetDataByPh(eqph)->tranfdlist , unfd_idList);
	strcpy(dsbob2->eq_tb->GetDataByPh(eqph)->closeunlist , un_idList);
	strcpy(dsbob2->eq_tb->GetDataByPh(eqph)->opencblist , opencblist);

	return 1;
}


//搜索给定的岛，得到部分转供的线段，可以转供的负荷个数和大小
int CNM1::getPartTrans(int eqph,int island, int& ld_cnt, double& ld_pq,int* transSec,int* lossSec,int& cntTransSec, int& cntLossSec)
{
	int ndCount,ldCount;//一个岛内的数量
	double pq_abund,unAbund;//联络有功、无功、电压裕度

	int knd,lv_nd,lv_cb,lv_cp,lv_so;
	int			i,j;
	int			ind, kknd,ld_nd,r_zn;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;

	ChildPtr1	*sec_p;//开关可能属于2条馈线，不能用开关的fdid来区别，只能用馈线段

	//没有负荷或者联络、电源
	if ( (islandlist[island].ldCount == 0) 
	  || ( (islandlist[island].unCount == 0) && (islandlist[island].soCount == 0))
	  ) 
	{
		return 0;
	}

	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = dsbob2->nd_tb->GetCount();
	
	CB* cb = dsbob2->cb_tb->GetTable();
	lv_cb = dsbob2->cb_tb->GetCount();

	if(lld->mx_cp >0)
	{	
		CP* cp = dsbob2->cp_tb->GetTable();
		lv_cp = dsbob2->cp_tb->GetCount();
	}

	SO* so = dsbob2->so_tb->GetTable();
	lv_so = dsbob2->so_tb->GetCount();

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	//此处似乎可以优化，如果没有那么多的空间，可以只做相关的节点
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->q |= ND_UNPROC;
	}

//////////////////////////////////////////////////////////////////////////
//关于分布式电源中对网络的贡献问题
//1.如果分布式电源为带逆变器的光伏、风机、储能，则分布式电源等同于联络开关
//2.如果分布式电源为同步电机，这等同于多电源，可以无缝切换
//lld add 2017-3-26
//////////////////////////////////////////////////////////////////////////

	unAbund=0;
	knd = -1;
	//优先考虑联络开关
	if (islandlist[island].unCount > 0)
	{
		knd = islandlist[island].island_unlist[0].nd;//通过节点号找电源点
		unAbund=islandlist[island].island_unlist[0].pq_abund;
	}
	else	//如果没有联络开关，考虑最大的分布式电源
	{
		double ttt=0;
		for(i = 0; i < islandlist[island].soCount; i++)
		{
			unAbund = islandlist[island].island_solist[i].pq;
			if (unAbund > ttt)
			{
				ttt = unAbund;
				knd = islandlist[island].island_solist[i].nd;//通过节点号找电源点
			}
		}
	}
	if (knd == -1)//没有联络开关，也没有分布式电源
		return 0;

	double pq_abund_sum=0;//负荷的累加值
	int unTranFlag=0;//不能转供的标志位
	cntTransSec=cntLossSec=0;

	ndCount = ldCount = 0;
	ind = knd;//母线的起始节点编号

	//之前已经将所有的节点都置为未处理
	if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd+ind)->q &= (~ND_UNPROC);

		//通过节点的q可以判断出是不是负荷节点，q |= 8

		//拿当前节点向下扩展，找到所有和它相连的节点
		knd = ind;//当前节点赋给一个临时的变量
		(nd+knd)->fch = knd; //当前节点号赋给前向链 
     	while (1)//还有扩展开关没有处理
		{
			//通过节点的q可以判断出是不是负荷节点，q |= 8
			if ((nd+knd)->q & ND_LD) 
			{
				//得到负荷id，负荷很重要，后面所有的都要以这个刚性的需求来进行处理
				ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
				ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
				while(ld_p)
				{
					if (!unTranFlag)
					{
						pq_abund = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
						pq_abund += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
						pq_abund = sqrt(pq_abund);
						
						//处理双电源问题
						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//多电源
						{
						}//多电源在计算停电损失的时候要考虑到
						else
						{
							pq_abund_sum += pq_abund;
						}

						if (pq_abund_sum > unAbund)//不能转供的了
						{
							ld_cnt = ldCount;
							ld_pq = pq_abund_sum - pq_abund;
							unTranFlag = 1;//开始不能转供了

							//一定损失的负荷加到负荷列表里面
							dsbob2->eq_tb->GetDataByPh(eqph)->NowTrload[dsbob2->eq_tb->GetDataByPh(eqph)->nNowTrload++] = ld_p->Ptr;

							ldCount++;
						}
					}
					else
					{
//							//一定损失的负荷加到负荷列表里面
//							ra_feeder[ra_cnt].ld_id = ld_p->Ptr;//负荷的物理号
//							ra_feeder[ra_cnt].up_cb = up_cb;//上游开关的性质，对故障切换有用
//							ra_feeder[ra_cnt].out = 1;//负荷损失
//							ra_feeder[ra_cnt].swit = 0;//负荷不能切换
//							ra_feeder[ra_cnt].tran = 0;//负荷能转供
//							ra_feeder[ra_cnt].sec_id = sec_ana;//负荷对应的故障馈线物理号（此时物理逻辑统一）
//							ldlist[ra_feeder[ra_cnt].ld_id] = 1;//负荷已处理
//							ra_cnt++;
					}

					ld_p = ld_p->Next;
				}
				::FreeChildPtr(ld_p0);
			}

			//直接绑定在网络中的分布式电源,ialgp=0
			if ((nd+knd)->q & ND_SO) 
			{
				//对so进行全循环，因为so的数量很少
				for (j=0; j<lv_so; j++)
				{
					if ( ((so+j)->ind == knd) && ((so+j)->ialgp == 0) && (knd !=ind) )
					{
						unAbund += sqrt((so+j)->w*(so+j)->w + (so+j)->r*(so+j)->r);//无功相当于增加电源点的视在功率
						break;
					}
				}
			}

			if ( (lld->mx_cp>0) && ((nd+knd)->q & ND_CP) )
			{
				//对cp进行全循环，因为cp的数量很少
				for (j=0; j<lv_cp; j++)
				{
					if ((dsbob2->cp_tb->GetTable()+j)->ind == knd)
					{
						unAbund += (dsbob2->cp_tb->GetTable()+j)->r;//无功相当于增加电源点的视在功率
						break;
					}
				}
			}

			//read cbs connected to this nd
			//从节点-开关关系表找到所有和这个节点相连的开关
			cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
			cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
			while(cb_p)
			{
				//如果是联络开关，停下来，找到和它相连的电源点
				if ( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				  || (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN)  
				  || (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB) 
				  || (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_ISOLATE) 
				  ) 
				{
					//将这个开关设为已处理			
					cb_p = cb_p->Next;
					continue;
				}

				//find the opposite nd through this cb
				//kknd表示开关的对侧节点，这里的节点都是整型的编号
				if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}
				//process this opposite nd
				//如果对侧节点 未处理==开关不参与拓扑推进
				if((nd+kknd)->q & ND_UNPROC)
				{
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd+kknd)->fch = (nd+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd+knd)->fch = kknd;
					//将找到的新节点设成已处理
					(nd+kknd)->q &= (~ND_UNPROC);
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this nd通过线段向下扩展
			sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//检查该线段是不是新的区段，如果是，将其加入到影响区段里面
				//根据线段区段之间的关联关系找到相对对应的区段
            	r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		        
				if (r_zn == -1)//该线段不属于任何区段
				{
					sec_p = sec_p->Next;
					continue;
				}

				//find the opposite nd through this sec
				if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
				{
					kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
				}

				//process this opposite nd
				if((nd+kknd)->q & ND_UNPROC)
				{
					//循环链表的使用，每扩展一个新的节点，做2件事：
					//1.将上一个节点的前向链设为自己的前向链
					//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
					
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd+kknd)->fch = (nd+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd+knd)->fch = kknd;
					//将找到的新节点设成已处理
					(nd+kknd)->q &= (~ND_UNPROC);

					if (unTranFlag)//不能转供了
					{
						lossSec[cntLossSec++]=sec_p->Ptr;
					}
					else
						transSec[cntTransSec++]=sec_p->Ptr;

				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd
			knd = (nd+knd)->fch;
			//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
			//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
			//拿另外的节点再开始搜索，一直到所有的节点都处理
			if(knd == ind)
			{
				break;
			}
		}
	}
	
	return 1;
}

//对单个岛进行分析，查看是否该岛可以进行切分
int CNM1::isNeedDivide(int island)
{
	int j;
	int soCount=0;//岛内分布式电源的个数
	double soAbund=0;//岛内分布式电源的总的额度（视在，为概率值，可能只能提供一部分）
	int unCount=0;//岛内联络开关的个数）
	double unAbund=0;//岛内联络开关的总的裕度（视在）
	int ldCount=0;//岛内负荷个数
	double ldTotal=0;//岛内负荷的总数量（视在）

	//只要校验过了，就不需要再做了
	//validate必须是完全确认后的情况，也就是辐射网或者孤岛，或者有联络但不能切分（找不到切分开关）
	//只要还有联络点可以切分，就不能认为校验过

	//不能转供或者已经处理过了,只有单电源点裕度不够才能是不能转供，多电源点不能确定不能转供
	if (islandlist[island].validate == 1)
		return 0;

	soCount = islandlist[island].soCount;
	unCount = islandlist[island].unCount;
	ldCount = islandlist[island].ldCount;
	
	//岛内分布式电源
	for (j=0; j<soCount; j++)
	{
		soAbund += islandlist[island].island_solist[j].pq;
	}

	//岛内联络开关，联络开关也相当于电源点
	for (j=0; j<unCount; j++)
	{
		unAbund += islandlist[island].island_unlist[j].pq_abund;
	}

	//岛内负荷
	for (j=0; j<ldCount; j++)
	{
		ldTotal += islandlist[island].island_ldlist[j].pq_ld;
	}

	//1.负荷为0；2.有负荷但是没有联络开关;3.残余网络内没有电源点
	//if ( (ldTotal == 0) || ((ldTotal>0) && (unCount==0))  || ((ldTotal>0) && (soCount==0)) )
	if ( (ldTotal == 0) || ((ldTotal>0) && (unCount==0)) )
	{
		islandlist[island].canTrans = 0;//整个岛都不能转供，只针对死岛
		islandlist[island].validate = 1;
		return 0;//不需要切分
	}

	//岛内的分布式电源就可以转供
	if (soAbund * 0.95 > ldTotal)
	{
		//无缝转供，不需要进行切分
		islandlist[island].canTrans = 1;
		islandlist[island].validate = 1;
		return 0;//不需要切分
	}

	//只有一个联络开关，通过这个联络开关就可以转供所有的岛负荷
	if ( (unAbund * 0.95 > ldTotal) && (unCount == 1) )
	{
		//无缝转供，不需要进行切分
		islandlist[island].canTrans = 1;
		islandlist[island].validate = 1;
		return 0;//不需要切分
	}

	//islandlist[island].canTrans = 1;	//初步认为岛可以转供
	trim_unlist(island);//对岛内所有的联络开关按照裕度由小到大进行排序

	//用岛内最大裕度的电源点是否可以完成转
	//n-1不能通过的最大可能是裕度不够，基本不会出现低电压，潮流也可以不用校验
	if (islandTreeFlow(island))//潮流校验不能通过，裕度或联络点电压不够
	{//由裕度最大的联络开关转供
		islandlist[island].canTrans = 1;//整个岛能转供
		islandlist[island].validate = 1;
		return 0;//不需要切分
	}

	//只有一个联络开关的情况
	if (islandlist[island].unCount == 1)
	{
		islandlist[island].canTrans = 0;//整个岛都不能转供，只针对死岛
		islandlist[island].validate = 1;
		return 0;//不需要切分
	}
	
	return 1;
}

//分析单个岛的潮流,该岛已经过环网拆除，有且仅有一个电源点
//如果不能进行转供，提供可以转供的全部负荷和电源点，以及转供路径
int CNM1::islandTreeFlow(int island)
{
	int i/*,j*/;
//	int knd,kknd,globalbs;
	double v_abund;
//	int unCount,mx_un;//联络开关的个数,最大裕度的联络开关
	double pq_so,pq_ld;
	//在做之前，先判断电源点是否平衡负荷

	v_abund = islandlist[island].island_unlist[0].v_abund;
	if (v_abund<0.93)//联络点的电压已经不能支撑
		return 0;

	pq_so = islandlist[island].island_unlist[0].pq_abund;
	pq_ld = 0;
	for (i=0; i<islandlist[island].ldCount; i++)
	{
		pq_ld += islandlist[island].island_ldlist[i].pq_ld;
	}

	//联络开关的裕度比需要转供的负荷要小,考虑5%的线损
	if ((pq_so * 0.95) <= pq_ld)
		return 0;

//////////////////////////////////////////////////////////////////////////
//只要裕度够，一般不会出现电压的问题
/*
	trim_ndlist(island);//对岛内所有的节点由小到大进行排序，二分法找节点

	//对给定的岛进行拓扑分析，合并开关，指定计算母线号
	islandTPAnalyse(island,islandlist[island].island_unlist[0].nd);

	//准备潮流计算，需要准备电源点、负荷点、电容器点、支路的数据
	//1.初始化潮流网络
	InitWorkingArray(island);
	
	//2.得到准备计算的网络数据
	SetNetworkData(island);

	//3.前推回代潮流计算
	OrderBusNumberA();
	ExecuteSweep();

	//4.计算结果判定
	for(j = 0; j < lv_wb; j++)
	{
		if(g_wb[j].tio >= 0)  
		{
			if ( g_wb[j].v < 0.93 )//出现了低电压
				return 0;
		}					
	}
*/


	return 1;
}

//给定联络开关的个数，求取联络开关的两两组合数
int CNM1::getUNcomb(int unCount,int* before,int* next)
{
	int ret = 0;

	int a[2];
	int r=2;
	int m = unCount;
	int cur;//指示定位数组中哪个成员正在移进
	
    unsigned int count=0;
 
	//初始化定位数组，0 起始的位置 ，开始的选择必是位置 0，1，2
    a[0]=0;
    a[1]=1;
//	before[0] = 0;
//	next[0] = 1;

    cur=r-1;//当前是最后一个成员要移进
 
	do{
		if (a[cur]-cur<=m-r )
		{  
			
			before[count] = a[0];
			next[count] = a[1];
			count++;
			/*
			for (int j=0;j<r;j++)
			cout<<setw(4)<<a[j];
			cout<<endl;
			*/
			a[cur]++;
			
			continue;
		}
		else
		{
			if (cur==0)
			{
//				cout<<count<<endl;
				ret = count;
				break;
			}
			
			a[--cur]++;
			for(int i=1;i<r-cur;i++)
			{
				a[cur+i]=a[cur]+i;
			}
			
			if(a[cur]-cur<m-r)
				cur=r-1;                
		}
	}while (1);

	return ret;//返回组合的个数
}

//切分岛
int CNM1::divideIsland(int island)
{
	int i,ret,unCount;
	NM1_Minpath minpath;
	int unCB_1[MAX_UION_COMPOSITE];
	int unCB_2[MAX_UION_COMPOSITE];
	int unCB1,unCB2,cnt_comb,ret_CB1,ret_CB2;
	int bk_cb,g_bk_cb;//准备打开的开关
	double retPQ;//打开开关后最大的转移负荷
	double maxPQ=0;//能转供的最大负荷

	//将岛内所有的联络开关都列出来，两两组合排序，加起来最大的裕度的排在前面
	//分析联络开关的组合，得到可以切分的联络开关
	//（1）都能转供掉负荷；（2）实在不行，转供最多的负荷
	unCount = islandlist[island].unCount;//岛内的联络开关数量
	//得到联络开关的组合数量并排序
	cnt_comb = getUNcomb(unCount,unCB_1,unCB_2);

	ret_CB1=ret_CB2=-1;
	//选开关组合
	for (i=0; i<cnt_comb; i++)
	{
		//取出两个开关的组合
		unCB1 = unCB_1[i];
		unCB2 = unCB_2[i];

		//分析两个联络开关的最小路，将所有的电源点和负荷（电容器）折到最小路上
		if (getMiminalPath(island, unCB1,unCB2,minpath))
		{
			retPQ = 0;
			bk_cb = -1;
			ret = setBreakCB(island, minpath,bk_cb,retPQ);//设定解环的开关，产生新岛
			if (ret < 0)//要么是没有平衡开关
				continue;//不能设定解环开关
			else if (ret >0)//完全可以进行切分
			{
				ret_CB1=unCB1;
				ret_CB2=unCB2;
				g_bk_cb = bk_cb;
				break;//不需要找下一个组合了
			}
			else
			{//只有部分合格，也就是只能转供部分负荷，取最大的负荷
				if (retPQ > maxPQ)
				{
					ret_CB1=unCB1;
					ret_CB2=unCB2;
					maxPQ = retPQ;
					g_bk_cb = bk_cb;
				}
			}
		}
		else
		{
			//从残余网络中找不到两个联络的联络开关，说明网络有问题
			ret_CB1=ret_CB2=-1;
			break;
			//continue;//得不到最小路
		}
	}

	//用开关组合进行拆分,三个开关，两个联络和一个分段
	if ( (ret_CB1 == -1) || (g_bk_cb == -1) )
	{//没有办法进行切分，也就是两个联络之间没有有效的通路，即使有也不能转供负荷，只能用最大来转供一部分
		return 0;
	}

	dsbob2->cb_tb->GetDataByPh(minpath.cbptr[g_bk_cb])->q |= CB_N_ISOLATE;
	//记录分段开关id
	strcat(open_idList,dsbob2->cb_tb->GetDataByPh(minpath.cbptr[g_bk_cb])->id);
	strcat(open_idList,"|");

	//用联络开关作为起点，对两个岛进行搜索，一个给其中一个岛号，另外再增加一个岛号
	islandlist[g_island].validate = 0;
	setNewIsland(island,ret_CB2,g_island);
	islandlist[island].validate = 0;
	//新的岛,新的岛要从老的岛分裂出来
	setNewIsland(island,ret_CB1);

	//对这两个新岛进行潮流校验
	if (!islandTreeFlow(island))//潮流校验不能通过，单个最大的电源点不能转供所有的负荷
	{
		//还需要判断是否有多个联络开关
		if (islandlist[island].unCount < 2 )
		{
			islandlist[island].canTrans = 0;//不能转供
			islandlist[island].validate = 1;//校验过了
		}
		else
			islandlist[island].canTrans = 1;//不能转供，但可以再切分
	}
	else
	{
		islandlist[island].canTrans = 1;//能转供
		islandlist[island].validate = 1;//校验过了
	}

	if (!islandTreeFlow(g_island))//潮流校验不能通过，单个最大的电源点不能转供所有的负荷
	{
		//还需要判断是否有多个联络开关
		if (islandlist[g_island].unCount < 2 )
		{
			islandlist[g_island].canTrans = 0;//不能转供
			islandlist[g_island].validate = 1;//校验过了
		}
		else
			islandlist[g_island].canTrans = 1;//不能转供，但可以再切分
	}
	else
	{
		islandlist[g_island].canTrans = 1;//能转供
		islandlist[g_island].validate = 1;//校验过了

//		//记录合位的联络开关开关id,只有校验过了才可以进行转
//		strcat(un_idList,"|");
//		strcat(un_idList,dsbob2->cb_tb->GetDataByPh(islandlist[island].island_unlist[unCB1].cbptr);
	}

	g_island++;

	return 1;
}




//给定两个联络开关unCB1和unCB2，找到其最短路径
//永远都用岛内的最大联络裕度开关进行岛的切分
int CNM1::getMiminalPath(int island, int unCB1,int unCB2,NM1_Minpath& minpath)
{
	int cbCount,comCount;//最小路上开关的数量，线段的数量

	int knd,lv_nd;
	int			i;
	int			ind, kknd,r_zn;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p0,*sec_p;
	
	//如果联络开关没有找到，说明没有回路
	int iFind = 0;//找到需要的联络开关
	int first  = islandlist[island].island_unlist[unCB1].cbptr;
	int second = islandlist[island].island_unlist[unCB2].cbptr;

	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = islandlist[island].ndCount;
	//将岛内的节点设为未处理
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[island].island_ndlist[i].nd_ptr;
		(nd+knd)->q |= ND_UNPROC;
	}

	//从一个联络开关处开始搜索
	ind = islandlist[island].island_unlist[unCB1].nd;
	//之前已经将所有的节点都置为未处理
	if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd+ind)->q &= (~ND_UNPROC);

		//拿当前节点向下扩展，找到所有和它相连的节点
		knd = ind;//当前节点赋给一个临时的变量
		(nd+knd)->fch = knd; //当前节点号赋给前向链 
   		while (1)//还有扩展开关没有处理
		{
			//read cbs connected to this nd
			//从节点-开关关系表找到所有和这个节点相连的开关
			cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
			cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
			while(cb_p)
			{
				//如果是联络开关或断开的分段开关，停下来，找到和它相连的电源点
				if( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				  ||(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_ISOLATE) )		
				{
					if (second == cb_p->Ptr)//必须找到另外一个联络开关
						iFind++;

					cb_p = cb_p->Next;
					continue;
				}

				//find the opposite nd through this cb
				//kknd表示开关的对侧节点，这里的节点都是整型的编号
				if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}
				//process this opposite nd
				//如果对侧节点 未处理==开关不参与拓扑推进
				if((nd+kknd)->q & ND_UNPROC)
				{
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd+kknd)->fch = (nd+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd+knd)->fch = kknd;

					//将这个节点的上一级记录下来，准备回溯
					(nd+kknd)->bch = knd;

					//将找到的新节点设成已处理
					(nd+kknd)->q &= (~ND_UNPROC);					
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this nd通过线段向下扩展
			sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//检查该线段是不是新的区段，如果是，将其加入到影响区段里面
				//根据线段区段之间的关联关系找到相对对应的区段
            	r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		        
				if (r_zn == -1)//该线段不属于任何区段
				{
					sec_p = sec_p->Next;
					continue;
				}

				//find the opposite nd through this sec
				if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
				{
					kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
				}

				//process this opposite nd
				if((nd+kknd)->q & ND_UNPROC)
				{
					//循环链表的使用，每扩展一个新的节点，做2件事：
					//1.将上一个节点的前向链设为自己的前向链
					//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
					
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd+kknd)->fch = (nd+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd+knd)->fch = kknd;
					//将这个节点的上一级记录下来，准备回溯
					(nd+kknd)->bch = knd;
					//将找到的新节点设成已处理
					(nd+kknd)->q &= (~ND_UNPROC);
				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd
			knd = (nd+knd)->fch;
			//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
			//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
			//拿另外的节点再开始搜索，一直到所有的节点都处理
			if(knd == ind)
			{
				break;
			}
		}
	}

	if (iFind != 1)//从一个联络开关开始没有找到另一个联络开关，或者找到次数大于1个（迂回供电）
		return 0;

	//在回溯的时候，要判断分段开关所管的支路的电压差
	knd = islandlist[island].island_unlist[unCB2].nd;//给定的联络开关的节点号开始回溯
	kknd = islandlist[island].island_unlist[unCB1].nd;//回溯的终点

	//初始化
	cbCount=comCount=0;
	//lld->sortInt(minpath.composite_nd,comCount);//不能排序，否则乱了
	for (i=0; i<MAX_ND; i++)
	{
		minpath.composite_ld[i] = 0;
		minpath.composite_so[i] = 0;
		minpath.composite_ld_vip[i] = 0;
	}

	//对起始点进行处理
	minpath.composite_so[comCount]=islandlist[island].island_unlist[unCB1].pq_abund;
	minpath.composite_nd[comCount++]=knd;
//	minpath.composite_fch[comCount]=-1;
//	minpath.composite_bch[comCount++]=knd;

	//每次取两个节点进行处理，不存在多找到开关和线段的问题
	do
	{
		//每扩展一个节点，将这个节点的前面的节点作为它的前向链
		//这个节点的后向链暂时不知道，只有等到下一个扩展节点才能给他赋值
		ind = knd;//本次开关的节点号
		knd = (nd+ind)->bch;//得到开关的上级节点号

//		if(knd==-1)
//		{
//			return 0;//lld edit for debug
//		}

		minpath.composite_nd[comCount++]=knd;
//		minpath.composite_bch[comCount-1]=knd;
//		minpath.composite_fch[comCount++]=ind;
	
		//需要维护组合节点的前后链表

		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
		cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
		while(cb_p)
		{
			//find the opposite nd through this cb
			//kknd表示开关的对侧节点，这里的节点都是整型的编号
			if ((((knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind) && (ind == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd))
			  ||((knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd) && (ind == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)))
			  && (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q !=1))
			{
				//离联络开关最近的那个开关可以视为主开关
				//如果开关没有分支，那么可以将其设为直线开关，2个连续的直线开关选一个就可以了。非主开关
				
				//每找到一个开关，将其上游线段上的电流加进来
				minpath.cbptr[cbCount++] = cb_p->Ptr;
			}			

			cb_p = cb_p->Next;
		}
		::FreeChildPtr(cb_p0);


	}	while(knd != kknd);//一直到电源点为止
//	minpath.composite_bch[comCount-1]=-1;//终点的后向链给-1
	minpath.composite_so[comCount-1]=islandlist[island].island_unlist[unCB2].pq_abund;

	minpath.island = island;
//	minpath.loopR = loopR;
	minpath.cbCount = cbCount;
//	minpath.secCount = secCount;
	minpath.comCount = comCount;

	//对剩下的联络开关进行节点的折算
	int knd_t = islandlist[island].island_unlist[unCB2].nd;//给定的联络开关的节点号开始回溯
	int kknd_t = islandlist[island].island_unlist[unCB1].nd;//回溯的终点
	int cnt = islandlist[island].unCount;
	double pq_abund;
	for (i=0; i<cnt; i++)
	{
		knd = islandlist[island].island_unlist[i].nd;
		if ((knd == knd_t)||(knd == kknd_t))
			continue;

		pq_abund = islandlist[island].island_unlist[i].pq_abund;//给定的联络开关的节点号开始回溯
		kknd = setCompositeND(knd, minpath.composite_nd,comCount);
		if (kknd != -1)
			minpath.composite_so[kknd] += pq_abund;
	}

	cnt = islandlist[island].ldCount;
	for (i=0; i<cnt; i++)
	{
		knd = islandlist[island].island_ldlist[i].nd;
		pq_abund = islandlist[island].island_ldlist[i].pq_ld;//给定的联络开关的节点号开始回溯
		kknd = setCompositeND(knd, minpath.composite_nd,comCount);
		if (kknd != -1)
		{
			minpath.composite_ld[kknd] += -pq_abund;
			minpath.composite_ld_vip[kknd] += islandlist[island].island_ldlist[i].vip;
		}
	}

	return 1;
}

//将联络开关的裕度和负荷都折算到最短路径上
int CNM1::setCompositeND(int src_nd, int* com_nd, int cnt)//设定解环的开关
{
	int ret,knd;
	knd = src_nd;//给定的联络开关的节点号开始回溯
	if (knd==-1)
	{
		//lld add 2016-11-19 for nm1 error
		return -1;
	}

	ND* nd = dsbob2->nd_tb->GetTable();
	//每次取两个节点进行处理，不存在多找到开关和线段的问题
	while(1)
	{
		knd = (nd+knd)->bch;//得到开关的上级节点号

	if (knd==-1)
	{
		//lld add 2016-11-19 for nm1 error
		return -1;
	}
		ret = lld->IsExistInArray(com_nd,knd,cnt);
		if (ret!=-1)
		{
			return ret;
		}
	}	

	return -1;
}

//根据最小路找解环开关，返回开关的序号 和转供的最大负荷
int CNM1::setBreakCB(int island, NM1_Minpath minpath,int& bk_cb,double& retPQ)//设定解环的开关
{
	double maxLD,deli;
	int		i,k,cb_nd,cb_knd,nd,knd;
//	int		cb_ptr;
	double	bso,bld,bvip=0,fso,fld,fvip=0;

	SEC* sec = dsbob2->sec_tb->GetTable();
	CB* cb = dsbob2->cb_tb->GetTable();

	//计算优化环流
//	optloop = g_wbase * 1000 * fabs(minpath.delV / minpath.loopR);
//在进行线段处理的时候就要处理开关
	
	deli=10000;
	//bk_cb = -1;
	//流过开关上的环流最接近最优环流即可
	for(k = 0; k < minpath.cbCount; k++)
	{
		//只有一个开关,直接断开即可
//		if (minpath.cbCount == 1)
//		{
//			bk_cb=0;
//			break;
//		}	

		//计算开关两侧的负荷电源累加，包括两个联络开关
		cb_nd = dsbob2->cb_tb->GetDataByPh(minpath.cbptr[k])->ind;//左节点
		cb_knd = dsbob2->cb_tb->GetDataByPh(minpath.cbptr[k])->iznd;//右节点
		//用开关两侧的节点在最小路里面找，属于哪个，最小路里面是有顺序的
		nd = lld->IsExistInArray(minpath.composite_nd,cb_nd,minpath.comCount);
		knd = lld->IsExistInArray(minpath.composite_nd,cb_knd,minpath.comCount);
		
		bso=fso=bld=fld=0;

		//保证nd<knd
		if (nd > knd)
		{//交换
			i = nd;
			nd = knd;
			knd = i;
		}
		while (1)
		{
			fso += minpath.composite_so[nd];
			fvip += minpath.composite_ld_vip[nd];
			fld += minpath.composite_ld[nd--];
			if (nd < 0)
				break;
		}

		while (1)
		{
			bso += minpath.composite_so[knd];
			bvip += minpath.composite_ld_vip[knd];
			bld += minpath.composite_ld[knd++];
			if (knd == minpath.comCount)
				break;
		}

		maxLD=0;
		//开关两侧的功率都不能平衡，开关无效
		if (((fso+fld)<0) && ((bso+bld)<0))
		{
		}
		else if (((fso+fld)>=0) && ((bso+bld)>=0))
		{
			if (fabs((fso+fld) - (bso+bld)) < deli)
			{
				//最优解开点
				deli = fabs((fso+fld) - (bso+bld)); 
				bk_cb = k;
			}
		}
		else
		{
			if (deli == 10000) //只有在没有找到最佳开关的情况下才考虑次优解
			{
				//转供最大的负荷可能是最佳的开关
				//负荷最大，联络开关数最少
				if ((fso+fld) > 0)
				{
					if (fld > maxLD)
					{
						maxLD = fld;
						bk_cb = k;//基本可以接受，有一侧是平衡的
					}
				}
				else
				{
					if (-bld > maxLD)
					{
						maxLD = -bld;
						bk_cb = k;//基本可以接受，有一侧是平衡的
					}
				}
			}
		}

		//两边基本相等，至少有一侧是平衡的
	}
	
	//没有找到解环开关，那么这个岛实际上就是不能进行拆分和转供的
	if (bk_cb == -1)
		return -1;

	if (deli==10000)//只有在没有合适的两侧转供都没有的情况下才能单边都会值
	{
		retPQ=maxLD;
		return 0;
	}

	return 1;
}

//给定一个现有的岛和两个联络开关，解环，形成两个新岛
//已经解成辐射网了，不能再有联络开关存在
int CNM1::setNewIsland(int old_island, int un_cb,int new_island)//设定解环的开关
{
	int island,ndCount,ldCount,cbCount,secCount,cpCount,unCount;//一个岛内的数量
	double p_abund,q_abund,v_abund,pq_abund;//联络有功、无功、电压裕度
	char nd_abund[MAX_SEC_ID_LEN];//联络开关的对侧节点
	char fdid_abund[MAX_SEC_ID_LEN];//联络开关对侧的馈线编号
	bool bFind;

	int knd,lv_nd,lv_cp,lv_cb;
	int			i,j;
	int			ind, kknd,ld_nd,r_zn,pcb_temp;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p,*sec_p0;
	ChildPtr1	*ld_p, *ld_p0;
	char scb_id[MAX_SEC_ID_LEN];//联络开关对侧的馈线id序列
	char fdid[MAX_SEC_ID_LEN];
	fdid[0]=0;//对馈线初始化

	CP* cp = dsbob2->cp_tb->GetTable();
	lv_cp = dsbob2->cp_tb->GetCount();

	CB* cb = dsbob2->cb_tb->GetTable();
	lv_cb = islandlist[old_island].cbCount;

	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = islandlist[old_island].ndCount;
	//将岛内的节点设为未处理
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[old_island].island_ndlist[i].nd_ptr;
		(nd+knd)->q |= ND_UNPROC;
	}
	//将岛内的开关设为未处理
	for (i=0; i<lv_cb; i++)
	{
		knd = islandlist[old_island].island_cblist[i].cb_ptr;
		(cb+knd)->q |= CB_N_RESTORE;
	}
	
	knd = islandlist[old_island].island_unlist[un_cb].nd;//取原始的联络开关
	pcb_temp = islandlist[old_island].island_unlist[un_cb].cbptr;

//传入开关自己处理完毕，准备从这个开关开始进行自己路径的追踪
//////////////////////////////////////////////////////////////////////////
	ndCount = cbCount = cpCount = secCount = unCount = ldCount = 0;
	ind = knd;//母线的起始节点编号

	if (new_island)
	{
		island = new_island;
	}
	else
		island = old_island;

	//将原始的联络开关作为新的岛的电源点进行向下搜索
	//新的岛用联络开关来替代电源点，到最后每个到都只有一个联络开关就可以做到辐射状
	islandlist[island].island_unlist[unCount].nd = islandlist[old_island].island_unlist[un_cb].nd;
	strcpy(islandlist[island].island_unlist[unCount].secid , islandlist[old_island].island_unlist[un_cb].secid);
	islandlist[island].island_unlist[unCount].pq_abund = islandlist[old_island].island_unlist[un_cb].pq_abund;
	islandlist[island].island_unlist[unCount].v_abund = islandlist[old_island].island_unlist[un_cb].v_abund;
	islandlist[island].island_unlist[unCount].cbptr = islandlist[old_island].island_unlist[un_cb].cbptr;
	//islandlist[island].island_unlist[unCount].main_nd = islandlist[old_island].island_unlist[un_cb].main_nd;

	unCount++;

	//之前已经将所有的节点都置为未处理
	if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd+ind)->q &= (~ND_UNPROC);
		islandlist[island].island_ndlist[ndCount++].nd_ptr = ind;

		//通过节点的q可以判断出是不是负荷节点，q |= 8

		//拿当前节点向下扩展，找到所有和它相连的节点
		knd = ind;//当前节点赋给一个临时的变量
		(nd+knd)->fch = knd; //当前节点号赋给前向链 
		while (1)//还有扩展开关没有处理
		{
			//通过节点的q可以判断出是不是负荷节点，q |= 8
			if ((nd+knd)->q & ND_LD) 
			{
				//得到负荷id
				ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
				ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
				while(ld_p)
				{
					islandlist[island].island_ldlist[ldCount].ld_ptr = ld_p->Ptr;
					islandlist[island].island_ldlist[ldCount].p_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
					islandlist[island].island_ldlist[ldCount].q_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
					pq_abund = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
					pq_abund += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
					pq_abund = sqrt(pq_abund);
					islandlist[island].island_ldlist[ldCount].pq_ld = pq_abund;	
					
					//处理双电源问题
					ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
					if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
						//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//多电源
					{
						islandlist[island].island_ldlist[ldCount].soSum = 2;
					}//多电源在计算停电损失的时候要考虑到
					else
						islandlist[island].island_ldlist[ldCount].soSum = 1;

					islandlist[island].island_ldlist[ldCount].nd = knd;
	//						strcpy(islandlist[island].island_ldlist[ldCount].ld_id , dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->id);
					
					ldCount++;

					ld_p = ld_p->Next;
				}
				::FreeChildPtr(ld_p0);
			}

			if ((nd+knd)->q & ND_CP) 
			{
				//对cp进行全循环，因为cp的数量很少
				for (j=0; j<lv_cp; j++)
				{
					if (cp->ind == knd)
					{
						islandlist[island].island_cplist[cpCount].cp_ptr = j;
						islandlist[island].island_cplist[cpCount].nd = knd;
						islandlist[island].island_cplist[cpCount].r = cp->r;
	//							islandlist[island].island_cplist[cpCount].island = ii;
						cpCount++;
						break;
					}
				}
			}

			//read cbs connected to this nd
			//从节点-开关关系表找到所有和这个节点相连的开关
			cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
			cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
			while(cb_p)
			{
				if (pcb_temp == cb_p->Ptr)//不能是联络开关自己
				{
					cb_p = cb_p->Next;
					continue;
				}

				//如果是联络开关，停下来，找到和它相连的电源点
				//如果 是纯辐射网，联络开关不存在
				if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				{
					//将这个开关设为已处理
					dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);

					//仅仅只是一个开位的开关是不能判断的，必须没有联络的线路才是
					if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
					{
						//kkndcb,此为联络节点，找到一个联络开关
						//sum_unioncb++;
						if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
						{
							strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->znd);
						}
						else
						{
							strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->nd);
						}

						strcpy(scb_id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id);
						//判断联络开关的裕度
						p_abund=q_abund=v_abund=0;
 						getUNCB_abun(fdid,scb_id,nd_abund, \
									fdid_abund,p_abund,q_abund,v_abund);
						pq_abund = sqrt(p_abund*p_abund + q_abund*q_abund);
						
						if ( (p_abund==0) && (q_abund==0) && (v_abund==0))
						{
							pq_abund = 5;
							v_abund = 1.05;
						}
						else
							pq_abund = sqrt(p_abund*p_abund + q_abund*q_abund);
						
						//虽然有联络开关，但没有联络线路，联络开关是悬空的
						if (fdid_abund != "")
						{
							bFind = false;
							//如果联络开关的母线和以前的联络开关母线一样
							for (i=0;i<unCount;i++)
							{
								//比较联络开关裕度，选最大的裕度
								pcb_temp = islandlist[island].island_unlist[i].nd;
								if ( (nd+pcb_temp)->ibs == (nd+knd)->ibs )
								{
									//替换
									if (pq_abund > islandlist[island].island_unlist[i].pq_abund)
									{
										islandlist[island].island_unlist[i].nd = knd;
										strcpy(islandlist[island].island_unlist[i].secid , fdid_abund);

										islandlist[island].island_unlist[i].p_abund=p_abund;
										islandlist[island].island_unlist[i].q_abund=q_abund;
										islandlist[island].island_unlist[i].pq_abund=pq_abund;
										islandlist[island].island_unlist[i].v_abund=v_abund;
										islandlist[island].island_unlist[i].cbptr = cb_p->Ptr;
									}
									bFind = true;
								}
							}

							if (!bFind)
							{
								islandlist[island].island_unlist[unCount].nd = knd;
								strcpy(islandlist[island].island_unlist[unCount].secid , fdid_abund);

								islandlist[island].island_unlist[unCount].p_abund=p_abund;
								islandlist[island].island_unlist[unCount].q_abund=q_abund;
								islandlist[island].island_unlist[unCount].pq_abund=pq_abund;
								islandlist[island].island_unlist[unCount].v_abund=v_abund;
								islandlist[island].island_unlist[unCount].cbptr = cb_p->Ptr;

								unCount++;						
							}
						}
					}						
					cb_p = cb_p->Next;
					continue;
				}

				//只要是搜索到的开关都保留下来,已经处理过的不保留
				if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_RESTORE) //this so nd has not processed
				{
					//printf("%s,%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb);
					//只处理主干线上的开关，CB_OPANABL
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb)
					{
						islandlist[island].island_cblist[cbCount].cb_ptr = cb_p->Ptr;

						//如果是分位开关（常开的分为开关），停下来
						if( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN)
						  ||(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_ISOLATE ) )
						{
							cb_p = cb_p->Next;
							continue;
						}
						else
							islandlist[island].island_cblist[cbCount].normalOpen = 0;
						cbCount++;
					}

					//只处理主干线上的开关，CB_OPANABL
					//将这个开关设为已处理
					dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);
					//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);

				}

				//find the opposite nd through this cb
				//kknd表示开关的对侧节点，这里的节点都是整型的编号
				if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}
				//process this opposite nd
				//如果对侧节点 未处理==开关不参与拓扑推进
				if((nd+kknd)->q & ND_UNPROC)
				{
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd+kknd)->fch = (nd+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd+knd)->fch = kknd;
					//将找到的新节点设成已处理
					(nd+kknd)->q &= (~ND_UNPROC);
					islandlist[island].island_ndlist[ndCount++].nd_ptr = kknd;
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this nd通过线段向下扩展
			sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//检查该线段是不是新的区段，如果是，将其加入到影响区段里面
				//根据线段区段之间的关联关系找到相对对应的区段
				r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
				
				if (r_zn == -1)//该线段不属于任何区段
				{
					sec_p = sec_p->Next;
					continue;
				}

				//find the opposite nd through this sec
				if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
				{
					kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
				}

				//process this opposite nd
				if((nd+kknd)->q & ND_UNPROC)
				{
					if (strlen(fdid)==0)
						strcpy(fdid,dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->fdid);

					//循环链表的使用，每扩展一个新的节点，做2件事：
					//1.将上一个节点的前向链设为自己的前向链
					//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
					
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd+kknd)->fch = (nd+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd+knd)->fch = kknd;
					//将找到的新节点设成已处理
					(nd+kknd)->q &= (~ND_UNPROC);
					islandlist[island].island_ndlist[ndCount++].nd_ptr = kknd;
				
					islandlist[island].island_seclist[secCount].sec_ptr = sec_p->Ptr;
					islandlist[island].island_seclist[secCount].nd = knd;
					islandlist[island].island_seclist[secCount].znd = kknd;
	//						strcpy(islandlist[island].island_seclist[secCount].sec_id , dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->id);
//					islandlist[island].island_seclist[secCount].r = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r0;
//					islandlist[island].island_seclist[secCount].x = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->x0;
	//						islandlist[island].island_seclist[secCount].island = ii;
					secCount++;
				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd
			knd = (nd+knd)->fch;
			//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
			//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
			//拿另外的节点再开始搜索，一直到所有的节点都处理
			if(knd == ind)
			{
				islandlist[island].cbCount = cbCount;
				islandlist[island].ldCount = ldCount;
				islandlist[island].unCount = unCount;
				islandlist[island].ndCount = ndCount;
				islandlist[island].cpCount = cpCount;
				islandlist[island].secCount = secCount;
				break;
			}
		}
	}

	return 1;
}

//分析低电压，这个需要计算潮流
int CNM1::voltageAnanyse()
{
	int i,j,ret;
	NM1_Minpath minpath;
	int unCB1,unCB2;
	int loop[2];
	unfd_idList[0]='\0';

	//逐岛进行校验
	i = 0;
	while (1)
	{
		if (i>=g_island)
			break;

		if (islandlist[i].validate)//只处理没有校验过的岛
		{
			i++;
			continue;
		}

		//如果整个岛都不能转供（没有电源点的死岛），直接分析下一个岛
		if (!islandlist[i].canTrans)
		{
			i++;
			continue;
		}

		//岛内只有一个电源点
		if (islandlist[i].unCount == 1)
		{
			if (!islandTreeFlow(i))//潮流校验不能通过，裕度或联络点电压不够
			{
				islandlist[i].canTrans = 0;//整个岛不能转供
				islandlist[i].validate = 0;

				i++;
				continue;
			}
			else
			{//由裕度最大的联络开关转供
				islandlist[i].validate = 1;
				strcat(unfd_idList,islandlist[i].island_unlist[0].secid);
			}
		}
		else//岛内有多个电源点
		{
			//选一个最大的裕度电源出来
			//取最大的联络作为校验项，联络开关已经排序
			//检验用最大裕度的联络开关是否可以转供所有的负荷
			
			//最大裕度是否可以转供岛内的负荷
			if (!islandTreeFlow(i))//潮流校验不能通过，单个最大的电源点不能转供所有的负荷
			{
				//先做两个电源点，如果两个电源点还不能支撑，需要采取别的办法来处理
					//需要做岛的分裂，拆开一个，再加一个，一个一个的加
					//将一个环变成两个树
//////////////////////////////////////////////////////////////////////////
//通过增加支路进行解环
				//i是原始岛编号，有可能这个岛分裂成几个岛
				//通过这个循环用支路交换法将所有的环网解开
				j = 1;//第0个是裕度最大的联络点
				while (1)
				{//逐步加入联络开关
					if (j > islandlist[i].unCount)
					{//所有的电源点都加上了，不能计算校验成功
						islandlist[i].canTrans = 0;
						islandlist[i].validate = 1;
					
						i++;
						break;
					}
					//合上一个联络开关必然形成一个环网
					//分析再加入一个联络开关后形成环网的两个联络开关
					//对整个岛做拓扑，发现存在环网的联络开关，每次加一个开关
					ret = getLoopunCB(i,unCB1,unCB2);
					
					if (ret == 1)//只有有且只有一个环网的时候才可以解环
					{
						if (!getMiminalPath(i, unCB1,unCB2,minpath))
						{
							j++;
							continue;
						}

						loop[0] = unCB1;
						loop[1] = unCB2;

						//不需要计算环网的潮流，直接用原有的辐射网的判据就可以了
						//将环解开，判断是否有低电压
						//计算解环后的电压情况
						double retPQ=0;
//						ret = setBreakCB(i, minpath,loop,retPQ);//设定解环的开关，产生新岛
						//顺着联络开关unCB1,unCB2进行搜索

						if (ret > 0)

						j++;
					}
				}
//环已解开，在原来的基础上增加了一个新的岛
//////////////////////////////////////////////////////////////////////////

			}
			else
			{//该岛用一个电源点就可以转供，不需要找其它的电源，转到下一个岛
				islandlist[i].validate = 1;
				//strcat(unfd_idList,islandlist[i].island_unlist[0].secid);
				i++;
			}
		}
	}

	return 1;
}



//计算的时候需要先进行拓扑分析，得到母线编号
int CNM1::islandTPAnalyse(int island,int so_nd)
{
	int i,j;
	int knd,kknd,globalbs;

	ND* nd = dsbob2->nd_tb->GetTable();
	int lv_nd = islandlist[island].ndCount;
	CB* cb = dsbob2->cb_tb->GetTable();
	int lv_cb = islandlist[island].cbCount;//联络开关不能算岛内的开关

	ChildPtr1	*cb_p, *cb_p0;

//////////////////////////////////////////////////////////////////////////
//将节点转化为计算母线	
//将岛内的节点设为未处理
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[island].island_ndlist[i].nd_ptr;
		(nd+knd)->q |= ND_UNPROC;
	}

	//用电源点进行拓扑分析，直到所有的岛全部做完
	//电源点的节点号
	knd = so_nd;
	globalbs = 1;//从电源点开始进行搜索

	//对岛内的每个节点进行循环
	for(i = 0; i < lv_nd; i++)
	{
		//把节点取出来
		j = islandlist[island].island_ndlist[i].nd_ptr;
		if((nd+j)->q & ND_UNPROC)  //this nd has not processed
		{
			//get bs number for this nd
			//ibs = global->nextbs;//最后的那个计算母线号，每添加一个都往上涨1
	
			//create circular chain for this nd
			knd = j;
			(nd+knd)->fch = knd;//设定前向链
			(nd+knd)->q &= (~ND_UNPROC);//将该节点设成已处理
			(nd+knd)->primebs = globalbs;//把这个节点所属于的计算母线号填入到节点里面

			//下面要做一个节点广度扩展的死循环，一直到找不到可以扩展的节点为止
			while( 1 )
			{
				//通过节点-开关关联表将和这个节点相关联的开关取出来放到系列中
				//特别是遇到开闭所母线时会有很多的开关要加到开关系列中
				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//准备对这个开关进行扩展，先把最初的那个开关暂时存起来
				while(cb_p)//cb_p是与这个节点关联的所有的开关的指针集合
				{
					//只有开关是闭合的状态才向下扩展
					if (!((cb+cb_p->Ptr)->q & CB_NMLOPEN))  //cb is closed
					{
						//find the opposte nd through this cb
						//找到对侧节点并将这个节点存放到kknd
						if(knd == (cb+cb_p->Ptr)->ind)
						{
							kknd = (cb+cb_p->Ptr)->iznd;
						}
						else
						{
							kknd = (cb+cb_p->Ptr)->ind;
						}

						//process this opposite nd
						//如果对侧节点没有处理，将对侧节点设为已处理，置前向链和后向链
						//将节点合并成一个计算母线，即将同一个计算母线号写入对侧的节点计算母线号
						if((nd+kknd)->q & ND_UNPROC)
						{
							(nd+kknd)->fch = (nd+knd)->fch;
							(nd+knd)->fch = kknd;
							(nd+kknd)->q &= (~ND_UNPROC);
							(nd+kknd)->primebs = globalbs;//找到对侧节点后，还是给节点赋一个相同的母线号

						}
					}
					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);//释放指针

				//get next nd in the present nd group
				knd = (nd+knd)->fch;//下一个节点就是前驱节点，前面的那个
				if(knd == j) break;
			}
			globalbs++;//临时母线编号得到
		}
	}
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[island].island_ndlist[i].nd_ptr;
		islandlist[island].island_ndlist[i].ibs = (nd+knd)->ibs;
		islandlist[island].island_ndlist[i].primebs = (nd+knd)->primebs;
	}

	return 1;
}



//分析两个电源点的岛的潮流,双端供电环网
//仅仅多了一个电源点，拓扑不变
int CNM1::islandLoopFlow(int island,int* loop)
{
//	int j;
//	int knd,kknd,globalbs;
//	double p_abund,q_abund,v_abund;

	islandTPAnalyse(island,loop[0]);

	//准备潮流计算，需要准备电源点、负荷点、电容器点、支路的数据
	//1.初始化潮流网络
	//
	// lld 2019-9-25
	//InitWorkingArray(island);
	
	//2.得到准备计算的网络数据
	SetNetworkData(island,loop);

	//3.牛顿法潮流计算
	OrderBusNumberB();
	g_method=1;
	ExecuteNewton();

//	//4.计算结果判定
//	for(j = 0; j < lv_wb; j++)
//	{
//		if(g_wb[j].tio >= 0)  
//		{
//			if ( g_wb[island].v < 0.93 )//出现了低电压
//				return 0;
//		}					
//	}

	return 1;
}

int CNM1::getbs(int island,int nd)
{
//二分法
	int high,middle,low;

	int ndCount = islandlist[island].ndCount;

	high = ndCount - 1;
	low = 0;
    while(low <= high)
	{
        middle = (low + high) / 2;
		
		if (nd == islandlist[island].island_ndlist[middle].nd_ptr)
		{  //找到,返回下标索引值
			return islandlist[island].island_ndlist[middle].primebs;
		}
		else if(nd < islandlist[island].island_ndlist[middle].nd_ptr)
		{  //查找值在低半区
            high = middle - 1;
        }
		else
		{  //查找值在高半区
            low = middle + 1;
        }
    }

	return -1;
}

//struct In 
//{ 
//double data; 
//int other; 
//}s[100] 

//按照data的值从小到大将结构体排序,关于结构体内的排序关键数据data的类型可以很多种，参考上面的例子写 
int struct_cmp( const void *a ,const void *b) 
{ 
	return (*(NM1_UNLIST *)a).pq_abund < (*(NM1_UNLIST *)b).pq_abund ? 1 : -1; 
} 

int CNM1::sort_unlist(NM1_UNLIST* unlist,int cnt)
{
	qsort(unlist,cnt,sizeof(unlist[0]),struct_cmp); 

	return 1;
}

//将每个岛内的联络开关数计算出来
int CNM1::trim_unlist(int island)
{
	sort_unlist(islandlist[island].island_unlist,islandlist[island].unCount);
	return 1;
}

//排序，针对残余网络内的全部节点，方便以后进行二分法搜索
int CNM1::trim_ndlist(int island)
{
	int i,k;
	int nd_t;
	int nd[MAX_ND];
	int ndCount;

	ndCount = islandlist[island].ndCount;
	for (i=0; i<ndCount; i++)
	{
		nd_t = islandlist[island].island_ndlist[i].nd_ptr;
		nd[i] = nd_t;
	}
	lld->sortInt(nd,ndCount);

	for (k=0; k<ndCount; k++)
	{
		islandlist[island].island_ndlist[k].nd_ptr = nd[k];
	}

	return 1;
}

int CNM1::SetNetworkData(int island,int* loop)
{
	int i,nd,cnt,loopun;

//////////////////////////////////////////////////////////////////////////
	//电源点
	if (loop!=NULL)
	{
		for (i=0; i<2; i++)
		{
			loopun = loop[i];
			nd = islandlist[island].island_unlist[loopun].nd;
			g_wg[i].logic = islandlist[island].island_unlist[loopun].cbptr;
			g_wg[i].ibs = getbs(island,nd);
			g_wg[i].slackavr = 0; //表明是松弛节点
    
			//配电网的电源根节点就是平衡节点，默认是10.5
			g_wg[i].v = islandlist[island].island_unlist[i].v_abund;
			
			//电源点的首端有功和无功，主要用于负荷点的处理
			g_wg[i].w = islandlist[island].island_unlist[i].p_abund / g_wbase;	//有功
			g_wg[i].r = islandlist[island].island_unlist[i].q_abund / g_wbase;	//无功
		}
	}
	else
	{
		nd = islandlist[island].island_unlist[0].nd;
		g_wg[0].logic = islandlist[island].island_unlist[0].cbptr;
		g_wg[0].ibs = getbs(island,nd);
		g_wg[0].slackavr = 0; //表明是松弛节点
    
		//配电网的电源根节点就是平衡节点，默认是10.5
		g_wg[0].v = islandlist[island].island_unlist[0].v_abund;
		
		//电源点的首端有功和无功，主要用于负荷点的处理
		g_wg[0].w = islandlist[island].island_unlist[0].p_abund / g_wbase;	//有功
		g_wg[0].r = islandlist[island].island_unlist[0].q_abund / g_wbase;	//无功
	}
				
//////////////////////////////////////////////////////////////////////////
	//负荷点,把每个负荷点的数据都读出来				
	cnt = islandlist[island].ldCount;
    for(i = 0; i < cnt; i++)
    {
		//set load working array 
		g_wld[lv_wld].logic = islandlist[island].island_ldlist[i].ld_ptr;
		g_wld[lv_wld].ibs = getbs(island,islandlist[island].island_ldlist[i].nd);
		g_wld[lv_wld].w = islandlist[island].island_ldlist[i].p_ld / g_wbase ;
		g_wld[lv_wld].r = islandlist[island].island_ldlist[i].q_ld / g_wbase ;
		
		lv_wld++;
	}
//////////////////////////////////////////////////////////////////////////
	//电容器
	cnt = islandlist[island].cpCount;
    for(i = 0; i < cnt; i++)
    {
		//set shunt working array 
		g_wsh[lv_wsh].logic = islandlist[island].island_cplist[i].cp_ptr;
		g_wsh[lv_wsh].ibs = getbs(island,islandlist[island].island_cplist[i].nd);;
		g_wsh[lv_wsh].r = islandlist[island].island_cplist[i].r;
		g_wsh[lv_wsh].rnom = islandlist[island].island_cplist[i].r;
		lv_wsh++;
    }

//////////////////////////////////////////////////////////////////////////
	//支路
	cnt = islandlist[island].secCount;
    for(i = 0; i < cnt; i++)
    {
		g_wbr[lv_wbr].type = _SEC;
		g_wbr[lv_wbr].logic = islandlist[island].island_seclist[i].sec_ptr;
		g_wbr[lv_wbr].ibs = getbs(island,islandlist[island].island_seclist[i].nd);
		g_wbr[lv_wbr].zbs = getbs(island,islandlist[island].island_seclist[i].znd);
	
//		g_wbr[lv_wbr].r = islandlist[island].island_seclist[i].r;
//		g_wbr[lv_wbr].x = islandlist[island].island_seclist[i].x;

		lv_wbr++;
    }
	
	return 1;
}




//判断给定一个联络开关开始搜索，是否可以搜到联络开关
int CNM1::exist_unCB(int island,int unCB1)
{
	int ret=0;//在岛内再也找到到合上联络开关后会形成环网的情况

//	int cbCount,secCount;//最小路上开关的数量，线段的数量
	double loopR=-1;

	int knd,lv_nd,un_nd;
	int			i;
	int			ind, kknd,r_zn;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p0,*sec_p;
	
	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = islandlist[island].ndCount;
	//将岛内的节点设为未处理
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[island].island_ndlist[i].nd_ptr;
		(nd+knd)->q |= ND_UNPROC;
	}

	//从一个联络开关处开始搜索
	ind = un_nd = islandlist[island].island_unlist[unCB1].nd;
	//之前已经将所有的节点都置为未处理
	if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd+ind)->q &= (~ND_UNPROC);

		//拿当前节点向下扩展，找到所有和它相连的节点
		knd = ind;//当前节点赋给一个临时的变量
		(nd+knd)->fch = knd; //当前节点号赋给前向链 
   		while (1)//还有扩展开关没有处理
		{
			//read cbs connected to this nd
			//从节点-开关关系表找到所有和这个节点相连的开关
			cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
			cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
			while(cb_p)
			{
				if ( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd == un_nd)  
					|| (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind == un_nd) ) 
				{
					cb_p = cb_p->Next;
					continue;//不能是自己
				}

				//如果是联络开关或断开的分段开关，停下来，找到和它相连的电源点
				if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				{
					//通过节点号得到联络开关的位置
					for (i=0; i<islandlist[island].unCount; i++)
					{
						if (knd==islandlist[island].island_unlist[i].nd)
						{
							ret = i;
							break;
						}
					}
					break;
				}

				//如果是联络开关或断开的分段开关，停下来
				if( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				  ||(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN) )		
				{
					//将这个开关设为已处理
					dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);

					cb_p = cb_p->Next;
					continue;
				}

				
				//find the opposite nd through this cb
				//kknd表示开关的对侧节点，这里的节点都是整型的编号
				if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}
				//process this opposite nd
				//如果对侧节点 未处理==开关不参与拓扑推进
				if((nd+kknd)->q & ND_UNPROC)
				{
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd+kknd)->fch = (nd+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd+knd)->fch = kknd;

					//将这个节点的上一级记录下来，准备回溯
					(nd+kknd)->bch = knd;

					//将找到的新节点设成已处理
					(nd+kknd)->q &= (~ND_UNPROC);
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			if (ret != -1)
				return ret;

			//read secs connected to this nd通过线段向下扩展
			sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//检查该线段是不是新的区段，如果是，将其加入到影响区段里面
				//根据线段区段之间的关联关系找到相对对应的区段
            	r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		        
				if (r_zn == -1)//该线段不属于任何区段
				{
					sec_p = sec_p->Next;
					continue;
				}

				//find the opposite nd through this sec
				if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
				{
					kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
				}

				//process this opposite nd
				if((nd+kknd)->q & ND_UNPROC)
				{
					//循环链表的使用，每扩展一个新的节点，做2件事：
					//1.将上一个节点的前向链设为自己的前向链
					//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
					
					//将扩展的前向链 设成 找到的新节点的前向链
					(nd+kknd)->fch = (nd+knd)->fch;
					//新节点 设成 扩展节点的前向链
					(nd+knd)->fch = kknd;
					//将这个节点的上一级记录下来，准备回溯
					(nd+kknd)->bch = knd;
					//将找到的新节点设成已处理
					(nd+kknd)->q &= (~ND_UNPROC);
				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd
			knd = (nd+knd)->fch;
			//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
			//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
			//拿另外的节点再开始搜索，一直到所有的节点都处理
			if(knd == ind)
			{
				break;
			}
		}
	}

	return ret;
}

//用每个联络开关出发开始搜索，有且只有一个环存在
int CNM1::getLoopunCB(int island,int& unCB1,int& unCB2)
{
	int ret=-1;
	for (int i=0; i<islandlist[island].unCount; i++)
	{
		if ((ret = exist_unCB(island,i)) != -1)
		{
			unCB1 = i;
			unCB2 = ret;
			break;
		}
	}

	return ret;
}


//直接用eq来做，简化处理
int CNM1::getResidueStruct(int noextbs,int* pextbs,int* pupextbs)
{//int degbug=1;
	int ndCount,ldCount,cbCount,secCount,cpCount,unCount,soCount;//一个岛内的数量
	double p_abund,q_abund,v_abund,pq_abund;//联络有功、无功、电压裕度
	char nd_abund[MAX_SEC_ID_LEN];//联络开关的对侧节点
	char fdid_abund[MAX_SEC_ID_LEN];//联络开关对侧的馈线编号
	bool bFind;

	int knd,lv_nd,lv_cb,lv_cp,lv_so;
	char fdid[MAX_SEC_ID_LEN];//区段所属的馈线ID，不含有前缀
	int			i,ii,j;
	int			ind, kknd,ld_nd,cb_id,r_zn,pcb_temp;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;
	char scb_id[MAX_SEC_ID_LEN];//联络开关对侧的馈线id序列

	ChildPtr1	*sec_p;//开关可能属于2条馈线，不能用开关的fdid来区别，只能用馈线段
	
	//没有可以转供的开关
	if (noextbs == 0)
	{
		return 0;
	}

	knd = pupextbs[0];//通过节点号找电源点
	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = dsbob2->nd_tb->GetCount();
	
//	sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//	sec_p0 = sec_p;//进行指针的临时保存，指针是整型的数字
//	if (!sec_p)
//	{
//		::FreeChildPtr(sec_p0);
//		return 0;
//	}
//	strcpy(fdid,dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->fdid);
//	if (strlen(fdid)==0)
//	{
//		::FreeChildPtr(sec_p0);
//		return 0;
//	}
//	::FreeChildPtr(sec_p0);
	
	CB* cb = dsbob2->cb_tb->GetTable();
	lv_cb = dsbob2->cb_tb->GetCount();

	if (lld->mx_cp>0)
	{
		CP* cp = dsbob2->cp_tb->GetTable();
		lv_cp = dsbob2->cp_tb->GetCount();
	}

	SO* so = dsbob2->so_tb->GetTable();
	lv_so = dsbob2->so_tb->GetCount();

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	//此处似乎可以优化，如果没有那么多的空间，可以只做相关的节点
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->q |= ND_UNPROC;
	}

	//将开关设为已处理
	for(i = 0; i < lv_cb; i++)
	{
		(cb+i)->q |= CB_N_RESTORE;
	}

    //依次处理区段的边界开关
	//每一个开关都是独立的供电，但其分支之间可能有联络和转供
	g_island = 0;
	for(ii = 0; ii < noextbs; ii++)
	{
		knd = pupextbs[ii];//取开关的上游节点进行判断

		cb_id = -1;
//////////////////////////////////////////////////////////////////////////
//将传入的开关自己找到，进行处理，如果是联络和分位，直接跳掉
		//如果边界开关本来就处于分位，直接跳过
		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
		cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
		while(cb_p)
		{
			pcb_temp = cb_p->Ptr;
			if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
			{
				kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
			}
			else
			{
				kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
			}

			if (kknd == pextbs[ii])//边界开关,把它自己找到
			{
				cb_id = cb_p->Ptr;
				break;
			}
			cb_p = cb_p->Next;
		}
		::FreeChildPtr(cb_p0);

		if (cb_id != -1)//一般不会出现找不到自己的情况
		{
			dsbob2->cb_tb->GetDataByPh(cb_id)->q &= (~ND_UNPROC);
			//这个开关要加到断开的开关里面，否则会出现联通问题
			//firstCB[ii] = cb_id;//将出来的开关设为必须断开的开关
			//这个开关属于必须要断开的开关，在搜索的时候可以不需要加到分段里面
			//只有加入到结构里面的分段开关才起作用

			//区段一出来就是联络开关，这个在开闭所里面比较普遍
			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & Q_UNIONCB)  
			{
				continue;
			}
			
			//出来的开关自己是分位开关，停下来
			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_OPEN)
			{
				continue;
			}
		}

//传入开关自己处理完毕，准备从这个开关开始进行自己路径的追踪
//////////////////////////////////////////////////////////////////////////
		ndCount = cbCount = cpCount = secCount = unCount = ldCount = soCount = 0;
		ind = pextbs[ii];//母线的起始节点编号

//将上一个节点设为已处理
		(nd+pupextbs[ii])->q &= (~ND_UNPROC);
//		islandlist[g_island].island_ndlist[ndCount++].nd_ptr = pupextbs[ii];

		//之前已经将所有的节点都置为未处理
		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
		{
			(nd+ind)->q &= (~ND_UNPROC);
			islandlist[g_island].island_ndlist[ndCount++].nd_ptr = ind;

			//通过节点的q可以判断出是不是负荷节点，q |= 8

			//拿当前节点向下扩展，找到所有和它相连的节点
			knd = ind;//当前节点赋给一个临时的变量
			(nd+knd)->fch = knd; //当前节点号赋给前向链 
     		while (1)//还有扩展开关没有处理
			{
				//通过节点的q可以判断出是不是负荷节点，q |= 8
				if ((nd+knd)->q & ND_LD) 
				{
					//得到负荷id，负荷很重要，后面所有的都要以这个刚性的需求来进行处理
					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
					ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
					while(ld_p)
					{
						islandlist[g_island].island_ldlist[ldCount].ld_ptr = ld_p->Ptr;
						islandlist[g_island].island_ldlist[ldCount].p_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
						islandlist[g_island].island_ldlist[ldCount].q_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
						pq_abund = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
						pq_abund += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
						pq_abund = sqrt(pq_abund);
						islandlist[g_island].island_ldlist[ldCount].pq_ld = pq_abund;	
						
						//处理双电源问题
						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//多电源
						{
							islandlist[g_island].island_ldlist[ldCount].soSum = 2;
						}//多电源在计算停电损失的时候要考虑到
						else
							islandlist[g_island].island_ldlist[ldCount].soSum = 1;
	
						islandlist[g_island].island_ldlist[ldCount].nd = knd;
//						strcpy(islandlist[g_island].island_ldlist[ldCount].ld_id , dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->id);
						
						ldCount++;

						ld_p = ld_p->Next;
					}
					::FreeChildPtr(ld_p0);
				}

				if ((nd+knd)->q & ND_SO) 
				{
					//对so进行全循环，因为so的数量很少,可能是分布式电源
					for (j=0; j<lv_so; j++)
					{
						if ( (so+j)->ind == knd)
						{
							islandlist[g_island].island_solist[soCount].soptr = j;
							islandlist[g_island].island_solist[soCount].nd = knd;
							islandlist[g_island].island_solist[soCount].p = (so+j)->w;
							islandlist[g_island].island_solist[soCount].q = (so+j)->r;
							strcpy(islandlist[g_island].island_solist[soCount].secid , (so+j)->fdid);
							islandlist[g_island].island_solist[soCount].pq = sqrt((so+j)->w*(so+j)->w+(so+j)->r*(so+j)->r);
							soCount++;
							break;
						}
					}
				}

				if ( ((nd+knd)->q & ND_CP) && (lld->mx_cp>0) )
				{
					//对cp进行全循环，因为cp的数量很少
					for (j=0; j<lv_cp; j++)
					{
						if ( (dsbob2->cp_tb->GetTable()+j)->ind == knd)
						{
							islandlist[g_island].island_cplist[cpCount].cp_ptr = j;
							islandlist[g_island].island_cplist[cpCount].nd = knd;
							islandlist[g_island].island_cplist[cpCount].r = dsbob2->cp_tb->GetTable()->r;
//							islandlist[g_island].island_cplist[cpCount].island = ii;
							cpCount++;
							break;
						}
					}
				}

				//read cbs connected to this nd
				//从节点-开关关系表找到所有和这个节点相连的开关
				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while(cb_p)
				{
					//如果是联络开关，停下来，找到和它相连的电源点
					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
					{
						//将这个开关设为已处理
						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);

						//仅仅只是一个开位的开关是不能判断的，必须没有联络的线路才是
						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
						{
							//kkndcb,此为联络节点，找到一个联络开关
							//sum_unioncb++;
							if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
							{
								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->znd);
							}
							else
							{
								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->nd);
							}

							strcpy(scb_id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id);
							//判断联络开关的裕度
							p_abund=q_abund=v_abund=0;
 							getUNCB_abun(fdid,scb_id,nd_abund, \
										fdid_abund,p_abund,q_abund,v_abund);
							if ( (p_abund==0) && (q_abund==0) && (v_abund==0) )
							{
								pq_abund = 5;//默认最大的联络裕度
								v_abund = 1.05;
							}
							else
								pq_abund = sqrt(p_abund*p_abund + q_abund*q_abund);
							
							//虽然有联络开关，但没有联络线路，联络开关是悬空的
							if (fdid_abund != "")
							{
								bFind = false;
								//如果联络开关的母线和以前的联络开关母线一样
								for (i=0;i<unCount;i++)
								{
									//比较联络开关裕度，选最大的裕度
									pcb_temp = islandlist[g_island].island_unlist[i].nd;
									if ( (nd+pcb_temp)->ibs == (nd+knd)->ibs )
									{
										//替换
										if (pq_abund > islandlist[g_island].island_unlist[i].pq_abund)
										{
											islandlist[g_island].island_unlist[i].nd = knd;
											strcpy(islandlist[g_island].island_unlist[i].secid , fdid_abund);

											islandlist[g_island].island_unlist[i].p_abund=p_abund;
											islandlist[g_island].island_unlist[i].q_abund=q_abund;
											islandlist[g_island].island_unlist[i].pq_abund=pq_abund;
											islandlist[g_island].island_unlist[i].v_abund=v_abund;
											islandlist[g_island].island_unlist[i].cbptr = cb_p->Ptr;
										}
										bFind = true;
									}
								}

								if (!bFind)
								{
									islandlist[g_island].island_unlist[unCount].nd = knd;
									strcpy(islandlist[g_island].island_unlist[unCount].secid , fdid_abund);

									islandlist[g_island].island_unlist[unCount].p_abund=p_abund;
									islandlist[g_island].island_unlist[unCount].q_abund=q_abund;
									islandlist[g_island].island_unlist[unCount].pq_abund=pq_abund;
									islandlist[g_island].island_unlist[unCount].v_abund=v_abund;
									islandlist[g_island].island_unlist[unCount].cbptr = cb_p->Ptr;

									unCount++;						
								}
							}
						}						
						cb_p = cb_p->Next;
						continue;
					}

					//只要是搜索到的开关都保留下来,已经处理过的不保留
					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_RESTORE) //this so nd has not processed
					{
						//printf("%s,%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb);
						//如果是分位开关（常开的分为开关），也要当做联络开关处理，但是其裕度为无限大

						//将这个开关设为已处理
						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);

						//仅仅只是一个开位的开关是不能判断的，必须没有联络的线路才是
						//!!!!!!!内部联络开关
						if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)
						{
							//kkndcb,此为联络节点，找到一个联络开关
							//sum_unioncb++;
							if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
							{
								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->znd);
							}
							else
							{
								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->nd);
							}

							strcpy(scb_id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id);
							//判断联络开关的裕度
							p_abund = q_abund = pq_abund = 20;
							
							//虽然有联络开关，但没有联络线路，联络开关是悬空的
							islandlist[g_island].island_unlist[unCount].nd = knd;
							strcpy(islandlist[g_island].island_unlist[unCount].secid , fdid);

							islandlist[g_island].island_unlist[unCount].p_abund=p_abund;
							islandlist[g_island].island_unlist[unCount].q_abund=q_abund;
							islandlist[g_island].island_unlist[unCount].pq_abund=pq_abund;
							islandlist[g_island].island_unlist[unCount].v_abund=1.05;
							islandlist[g_island].island_unlist[unCount].cbptr = cb_p->Ptr;

							unCount++;						

							cb_p = cb_p->Next;
							continue;
						}
						else
						{
							//如果是以前在分析的时候设定为解环开关，这里要将其恢复
							if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_ISOLATE)
								dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_ISOLATE);							

							islandlist[g_island].island_cblist[cbCount].cb_ptr = cb_p->Ptr;
							cbCount++;
						}

						//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);

					}

					//find the opposite nd through this cb
					//kknd表示开关的对侧节点，这里的节点都是整型的编号
					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}
					//process this opposite nd
					//如果对侧节点 未处理==开关不参与拓扑推进
					if((nd+kknd)->q & ND_UNPROC)
					{
						//将扩展的前向链 设成 找到的新节点的前向链
						(nd+kknd)->fch = (nd+knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd+knd)->fch = kknd;
						//将找到的新节点设成已处理
						(nd+kknd)->q &= (~ND_UNPROC);
						islandlist[g_island].island_ndlist[ndCount++].nd_ptr = kknd;
					}

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//read secs connected to this nd通过线段向下扩展
				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
				sec_p0 = sec_p;
				while(sec_p)
				{
				    //检查该线段是不是新的区段，如果是，将其加入到影响区段里面
				    //根据线段区段之间的关联关系找到相对对应的区段
            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		            
					if (r_zn == -1)//该线段不属于任何区段
					{
						sec_p = sec_p->Next;
						continue;
					}
 
					//find the opposite nd through this sec
					if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
					{
						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
					}
					else
					{
						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
					}

					//process this opposite nd
					if((nd+kknd)->q & ND_UNPROC)
					{
						//循环链表的使用，每扩展一个新的节点，做2件事：
						//1.将上一个节点的前向链设为自己的前向链
						//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
						
						//将扩展的前向链 设成 找到的新节点的前向链
						(nd+kknd)->fch = (nd+knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd+knd)->fch = kknd;
						//将找到的新节点设成已处理
						(nd+kknd)->q &= (~ND_UNPROC);
						islandlist[g_island].island_ndlist[ndCount++].nd_ptr = kknd;
					
						islandlist[g_island].island_seclist[secCount].sec_ptr = sec_p->Ptr;
						islandlist[g_island].island_seclist[secCount].nd = knd;
						islandlist[g_island].island_seclist[secCount].znd = kknd;
//						strcpy(islandlist[g_island].island_seclist[secCount].sec_id , dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->id);
						islandlist[g_island].island_seclist[secCount].r = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r0;
						islandlist[g_island].island_seclist[secCount].x = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->x0;
//						islandlist[g_island].island_seclist[secCount].island = ii;
						secCount++;
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);

				//move to next nd
				knd = (nd+knd)->fch;
				//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
				//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
				//拿另外的节点再开始搜索，一直到所有的节点都处理
				if(knd == ind)
				{
					islandlist[g_island].soCount = soCount;
					islandlist[g_island].cbCount = cbCount;
					islandlist[g_island].ldCount = ldCount;
					islandlist[g_island].unCount = unCount;
					islandlist[g_island].ndCount = ndCount;
					islandlist[g_island].cpCount = cpCount;
					islandlist[g_island].secCount = secCount;
					islandlist[g_island].validate = 0;
					islandlist[g_island].canTrans = 1;//先假设能转

					g_island++;
					break;
				}
			}
		}
	}
	
	return 1;
}




//////////////////////////////////////////////////////////////////////////



//给定多个电源点进行拓扑搜索，找到相关的岛
//unSum联络开关总数量,pun联络开关编号，pun_联络开关对侧编号,unRebund联络开关的裕度
//cbSum搜索路径中全部的断开的分段开关数,pcb分段开关的编号,开关包括以前的区段进入开关
//需要输入负荷的母线号序列
//出现环网和负荷不够的情况，不能转供
//0成功转带 -1系统崩溃，-2环网, -3裕度不够, -4有负荷孤岛
int CNM1::OnSearchTP(int unSum,int* pun, int* pun_, double* unRebund, \
					int cbSum,int* pcb,	int ldSum, double& tranSumld
					)
{
	int i,j,ret,iFind;
	bool bPassT=true;
	int			ind, knd, kknd,ld_nd,r_zn,knd_,un_nd,un_znd;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;
	double	unRebund0,sumP_T;
	double	w_next,r_next,deltaMultiP,deltaMultiQ;//联络开关的下游有功和无功
	int	cntLD;//负荷的总个数，需要最后判断是否能转带
	
	ret = 1;
	CB* cb = dsbob2->cb_tb->GetTable();

	int lv_nd = dsbob2->nd_tb->GetCount();
	if(lv_nd == 0)
	{
		return 0;
	}
	ND* nd_t = new ND[lv_nd];
	dsbob2->nd_tb->CopyTable(nd_t);//得到一个新的节点表，以免和原来的冲突

	//将所有的临时接点设为已处理
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

	//从联络开关开始搜索，看是否有环网，如果没有环网，看这个联络开关的裕度是否够
	//所有的联络都搜索完后，看负荷的个数是否达到需要转供的个数
	cntLD = 0;//通过联络开关搜索到的负荷个数
	tranSumld = 0;
	for(i = 0; i < unSum; i++)
	{
		unRebund0 = *(unRebund+i);//本联络开关的裕度
		un_nd  = knd = *(pun+i);//取联络开关的下游节点进行搜索
		un_znd = knd_ = *(pun_+i);//联络开关的对侧
		
		//单个边界开关开始追踪计数器
		w_next=r_next=deltaMultiP=deltaMultiQ=0;

		ind = knd;//母线的起始节点编号
//将上一个节点设为已处理
		(nd_t + knd_)->q &= (~ND_UNPROC);

		//之前已经将所有的节点都置为未处理
		if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
		{
			(nd_t+ind)->q &= (~ND_UNPROC);
			//通过节点的q可以判断出是不是负荷节点，q |= 8

			//拿当前节点向下扩展，找到所有和它相连的节点
			knd = ind;//当前节点赋给一个临时的变量
			(nd_t+knd)->fch = knd; //当前节点号赋给前向链 
     		while (1)//还有扩展开关没有处理
			{
				//通过节点的q可以判断出是不是负荷节点，q |= 8
				if ((nd_t+knd)->q & ND_LD) 
				{
					//得到负荷id
					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd_t+knd)->r_ld);
					ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
					while(ld_p)
					{
						//直接修改负荷的ID号
						w_next += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
						r_next += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;

						//处理双电源问题
						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//多电源
						{
							deltaMultiP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);
							deltaMultiQ += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r);
						}//多电源在计算停电损失的时候要考虑到

						cntLD++;
						ld_p = ld_p->Next;
					}
					::FreeChildPtr(ld_p0);
				}

				//read cbs connected to this nd
				//从节点-开关关系表找到所有和这个节点相连的开关
				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd_t+knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while(cb_p)
				{
					//如果是联络开关，出现环网
					//需要考虑发现它自己
					if ( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_NMLOPEN)  
						&& !( 
						  ((dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind == un_nd) 
						  && (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd == un_znd))
						     ||((dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd == un_nd) 
							 && (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind == un_znd))
						   )
					   )
					{
						//出现了环网，直接退出
						delete []nd_t;
						nd_t = 0;
						lv_nd = 0;
						::FreeChildPtr(cb_p0);

						return -2;
					}

					//如果是分位开关，停下来
					iFind = 0;
					for (j=0; j<cbSum; j++)
					{
						if (strcmp(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id , (cb+*(pcb+j))->id) == 0)
						{
							iFind = 1;
							break;
						}
					}

					if (iFind == 1)
					{
						cb_p = cb_p->Next;
						continue;
					}

					//find the opposite nd through this cb
					//kknd表示开关的对侧节点，这里的节点都是整型的编号
					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}
					//process this opposite nd
					//如果对侧节点 未处理==开关不参与拓扑推进
					if((nd_t+kknd)->q & ND_UNPROC)
					{
						//将扩展的前向链 设成 找到的新节点的前向链
						(nd_t+kknd)->fch = (nd_t+knd)->fch;
						//新节点 设成 扩展节点的前向链
						(nd_t+knd)->fch = kknd;
						//将找到的新节点设成已处理
						(nd_t+kknd)->q &= (~ND_UNPROC);
					}

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//read secs connected to this nd通过线段向下扩展
				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd_t+knd)->r_sec);
				sec_p0 = sec_p;
				while(sec_p)
				{
				    //检查该线段是不是新的区段，如果是，将其加入到影响区段里面
				    //根据线段区段之间的关联关系找到相对对应的区段
            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		            
					if (r_zn == -1)//该线段不属于任何区段
					{
						sec_p = sec_p->Next;
						continue;
					}
					
					//find the opposite nd through this sec
					if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
					{
						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
					}
					else
					{
						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
					}

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
						//将找到的新节点设成已处理
						(nd_t+kknd)->q &= (~ND_UNPROC);
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);

				//move to next nd
				knd = (nd_t+knd)->fch;
				//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
				//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
				//拿另外的节点再开始搜索，一直到所有的节点都处理
				if(knd == ind)
				{
					sumP_T = sqrt(w_next*w_next+r_next*r_next);
					if (unRebund0 < sumP_T)
					{
						bPassT = false;
					}
					else
					{
						//可以转供负荷累加
						tranSumld += sumP_T;
					}

					break;
				}
			}
		}
	}

	delete []nd_t;
	nd_t = 0;
	lv_nd = 0;
	if (!bPassT)
		ret = -3;

	if (cntLD < ldSum)
		ret = -4;

	return ret;
}

//
////从区段的首端开始向后（主要靠计算母线的排序来保证）搜索，已经将进入区段的那个节点排除了
////noextbs（边界开关数量，不含进入区段的那个节点）
////extbs边界开关的区段外计算母线，upextbs区段内对侧的计算母线
//int CNM1::OnSearchUnion(int noextbs,int* pextbs,int* pupextbs, \
//						   double& deltap,double& deltaq,int& deltaC, \
//						   double& sumP,double& sumQ,int& sumC, \
//						   double& lossP,double& lossQ,int& lossC, \
//						   double& deltaMultiP,double& deltaMultiQ,int& deltaMultiC, \
//						   * unfd_idList)
//{
//	//没有可以转供的开关
//	if (noextbs == 0)
//	{
//		return 0;
//	}
//
//	int knd = pupextbs[0];//通过节点号找电源点
//	ChildPtr1	*sec_p;//开关可能属于2条馈线，不能用开关的fdid来区别，只能用馈线段
//	ND* nd = dsbob2->nd_tb->GetTable();
//	int lv_nd = dsbob2->nd_tb->GetCount();
//	 fdid[80];//区段所属的馈线ID，不含有前缀
//	sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//	if (!sec_p)
//		return 0;
//	strcpy(fdid,dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->fdid);
//	if (strlen(fdid)==0)
//		return 0;
//
//
//	int			i,ii,j,k,ret,cnt_unioncb/*当个边界开关搜索时找到的联络开关数*/,sum_unioncb/*总的联络开关数*/;
//	int			ind, kknd,ld_nd,cb_id,r_zn,pcb_temp,cnt_ld/*负荷的总个数*/;
//	ChildPtr1	*cb_p, *cb_p0;
//	ChildPtr1   *sec_p0;
//	ChildPtr1	*ld_p, *ld_p0;
//	double w_next,r_next,c_next;//单个边界开关的下游的总有功、无功、用户数
//	 unfd_id[80];//联络开关对侧的馈线id序列
//	unfd_idList[0]='\0';//将对侧的联络馈线清空，准备输出
////全局
//	int* unCB = new int[20];//联络开关的首端节点，最多20个联络开关
//	int* unCB_ = new int[20];//联络开关的对侧节点（连到另一条线），最多20个联络开关
//	double* unRebund = new double[20];//联络开关的裕度
//	int* openCB = new int[50];//最多50个断开的分段开关，这个填开关的索引号（指针）
//
//	int cntMainCB;//残余的开关数量
//	int* mainCB = new int[200];//最多100个开关 
//	int* firstCB = new int[20];//首端开关 
////单个搜索路径,单个路径只有一个首端开关
//	int* l_unCB = new int[20];//联络开关的首端节点，最多20个联络开关
//	int* l_unCB_ = new int[20];//联络开关的对侧节点（连到另一条线），最多20个联络开关
//	double* l_unRebund = new double[20];//联络开关的裕度
//	int* l_openCB = new int[50];//最多50个断开的分段开关，这个填开关的索引号（指针）
//
//	int l_cntMainCB;//残余的开关数量
//	int* l_mainCB = new int[200];//最多100个开关 
//
//	bool bTranPass=true;//可以全部转供，只有所有的路径都通过，才能直接pass
//	double tranSumLoad=0;//可以部分转供的总负荷
//	double tranSumld;//开关序列中得到的转供负荷
//	
//	CStringList lCB;
//	//** lCB;
//	CString sProb;
//	int cntProb;//得到开关序列
//
//	 scb_id[80];
//	
//	double unionAbund,unionAund_,mxAbund,secondAbund,sumAbund;//联络开关的裕度，全部路径
//	
//	CB* cb = dsbob2->cb_tb->GetTable();
//	int lv_cb = dsbob2->cb_tb->GetCount();
//
//	//mark all nds as unprocessed
//	//将所有的节点设为未处理，准备拓扑分析用
//	//此处似乎可以优化，如果没有那么多的空间，可以只做相关的节点
//	for(i = 0; i < lv_nd; i++)
//	{
//		(nd+i)->q |= ND_UNPROC;
//	}
//
//	//将开关设为已处理
//	for(i = 0; i < lv_cb; i++)
//	{
//		(cb+i)->q |= ND_UNPROC;
//	}
//
//    //依次处理区段的边界开关
//	//每一个开关都是独立的供电，但其分支之间可能有联络和转供
//	//（1）先一个开关一个开关的处理
//	//      1.在处理开关路径时，没有联络，设置例外1
//	//		2.有一个联络开关，但裕度不够，设置例外2
//	//		3.出现有多个联络开关，需要调用多联络处理，如果转供不了，设置例外3
//	//（2）每个开关均处理完毕，发现存在例外，全部调用多联络模块进行重构
//	
//	sum_unioncb = 0;//全部联络开关计数器
//	cntMainCB = 0;
//	cnt_ld = 0;
//	sumAbund = mxAbund = secondAbund = 0;//联络开关总裕度置初值
//	for(ii = 0; ii < noextbs; ii++)
//	{
//		bTranPass=true;//每个联络开关默认是可以通过转供的
//
//		knd = pupextbs[ii];//取开关的上游节点进行判断
//
//		cb_id = -1;
////////////////////////////////////////////////////////////////////////////
////将传入的开关自己找到，进行处理，如果是联络和分位，直接跳掉
//		//如果边界开关本来就处于分位，直接跳过
//		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//		cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
//		while(cb_p)
//		{
//			pcb_temp = cb_p->Ptr;
//			if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//			{
//				kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//			}
//			else
//			{
//				kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//			}
//
//			if (kknd == pextbs[ii])//边界开关,把它自己找到
//			{
//				cb_id = cb_p->Ptr;
//				break;
//			}
//			cb_p = cb_p->Next;
//		}
//		::FreeChildPtr(cb_p0);
//
//		if (cb_id != -1)//一般不会出现找不到自己的情况
//		{
//			dsbob2->cb_tb->GetDataByPh(cb_id)->q &= (~ND_UNPROC);
//			//这个开关要加到断开的开关里面，否则会出现联通问题
//			firstCB[ii] = cb_id;//将出来的开关设为必须断开的开关
//			
//			//区段一出来就是联络开关，这个在开闭所里面比较普遍
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_NMLOPEN)  
//			{
//				continue;
//			}
//			
//			//出来的开关自己是分位开关，停下来
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_OPEN)
//			{
//				continue;
//			}
//		}
//
////传入开关自己处理完毕，准备从这个开关开始进行自己路径的追踪
////////////////////////////////////////////////////////////////////////////
//
//		//单个边界开关开始追踪计数器
//		w_next=r_next=c_next=0;
//		cnt_unioncb = 0;//本转供分支上的联络开关数量
//		unionAund_ = 0;//本转供分支上的联络裕度
//		l_cntMainCB = 0;
//
//		ind = pextbs[ii];//母线的起始节点编号
////将上一个节点设为已处理
//		(nd+pupextbs[ii])->q &= (~ND_UNPROC);
//
//		//之前已经将所有的节点都置为未处理
//		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
//		{
//			(nd+ind)->q &= (~ND_UNPROC);
//			//通过节点的q可以判断出是不是负荷节点，q |= 8
//
//			//拿当前节点向下扩展，找到所有和它相连的节点
//			knd = ind;//当前节点赋给一个临时的变量
//			(nd+knd)->fch = knd; //当前节点号赋给前向链 
//     		while (1)//还有扩展开关没有处理
//			{
//				//通过节点的q可以判断出是不是负荷节点，q |= 8
//				if ((nd+knd)->q & ND_LD) 
//				{
//					//得到负荷id
//					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
//					ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
//					while(ld_p)
//					{
//						//直接修改负荷的ID号
//						w_next += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
//						r_next += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
//						c_next++;
//						cnt_ld++;//全局负荷个数
//
//						//处理双电源问题
//						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
//						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
//							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//多电源
//						{
//							deltaMultiP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);
//							deltaMultiC++;
//						}//多电源在计算停电损失的时候要考虑到
//
//						ld_p = ld_p->Next;
//					}
//					::FreeChildPtr(ld_p0);
//				}
//
//				//read cbs connected to this nd
//				//从节点-开关关系表找到所有和这个节点相连的开关
//				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
//				while(cb_p)
//				{
//					//如果是联络开关，停下来，找到和它相连的电源点
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_NMLOPEN)  
//					{
//						//将这个开关设为已处理
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);
//
//						//仅仅只是一个开位的开关是不能判断的，必须没有联络的线路才是
//						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
//						{
//							//kkndcb,此为联络节点
//							//sum_unioncb++;
//
//							strcpy(scb_id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id);
//							//判断联络开关的裕度？
// 							unionAbund = getUNCB_abun(fdid,unfd_id,scb_id);
//							//虽然有联络开关，但没有联络线路，联络开关是悬空的
//							if (unfd_id != "")
//							{
//								strcat(unfd_idList,unfd_id);
//								strcat(unfd_idList,",");//联络的馈线id
//							}
//
//							l_unCB[cnt_unioncb] = knd;
//							l_unCB_[cnt_unioncb] = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;//外侧
//							l_unRebund[cnt_unioncb] = unionAbund;//外侧
//							unionAund_ += unionAbund;
//							cnt_unioncb++;
//						
//							unCB[sum_unioncb] = knd;
//							unCB_[sum_unioncb] = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;//外侧
//							unRebund[sum_unioncb] = unionAbund;//外侧
//							sumAbund += unionAbund;
//							sum_unioncb++;						
//
//						}						
//						cb_p = cb_p->Next;
//						continue;
//					}
//
//					//只要是搜索到的开关都保留下来,已经处理过的不保留
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & ND_UNPROC) //this so nd has not processed
//					{
//						//printf("%s,%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb);
//						//只处理主干线上的开关，CB_OPANABL
//						if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb)
//						{
//							l_mainCB[l_cntMainCB++] = cb_p->Ptr;
//							mainCB[cntMainCB++] = cb_p->Ptr;
//						}
//
//						//只处理主干线上的开关，CB_OPANABL
//						//将这个开关设为已处理
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);
//						//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);
//
//					}
//
//					//如果是分位开关，停下来
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN)
//					{
//						cb_p = cb_p->Next;
//						continue;
//					}
//
//					//find the opposite nd through this cb
//					//kknd表示开关的对侧节点，这里的节点都是整型的编号
//					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//					}
//					//process this opposite nd
//					//如果对侧节点 未处理==开关不参与拓扑推进
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//将扩展的前向链 设成 找到的新节点的前向链
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//新节点 设成 扩展节点的前向链
//						(nd+knd)->fch = kknd;
//						//将找到的新节点设成已处理
//						(nd+kknd)->q &= (~ND_UNPROC);
//					}
//
//					cb_p = cb_p->Next;
//				}
//				::FreeChildPtr(cb_p0);
//
//				//read secs connected to this nd通过线段向下扩展
//				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//				sec_p0 = sec_p;
//				while(sec_p)
//				{
//				    //检查该线段是不是新的区段，如果是，将其加入到影响区段里面
//				    //根据线段区段之间的关联关系找到相对对应的区段
//            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
//            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
//		            
//					if (r_zn == -1)//该线段不属于任何区段
//					{
//						sec_p = sec_p->Next;
//						continue;
//					}
// 
//					//find the opposite nd through this sec
//					if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
//					{
//						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
//					}
//
//					//process this opposite nd
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//循环链表的使用，每扩展一个新的节点，做2件事：
//						//1.将上一个节点的前向链设为自己的前向链
//						//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
//						
//						//将扩展的前向链 设成 找到的新节点的前向链
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//新节点 设成 扩展节点的前向链
//						(nd+knd)->fch = kknd;
//						//将找到的新节点设成已处理
//						(nd+kknd)->q &= (~ND_UNPROC);
//					}
//					sec_p = sec_p->Next;
//				}
//				::FreeChildPtr(sec_p0);
//
//				//move to next nd
//				knd = (nd+knd)->fch;
//				//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
//				//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
//				//拿另外的节点再开始搜索，一直到所有的节点都处理
//				if(knd == ind)
//				{
//					sumP += w_next;//不含多电源转供,下游全部的负荷
//					sumQ += r_next;
//					sumC += c_next;
//
//					if (cnt_unioncb == 0)//没有联络开关
//					{
//						bTranPass = FALSE;//单个路径不能处理所有的负荷
//						break;
//					}
//
//					//需要在这里处理所有的转供
//					//有负荷但没有联络，有一个联络但没有裕度
//					if ( ((cnt_unioncb == 1) 
//						&& (unionAund_ < sqrt(w_next * w_next + r_next * r_next)))
//						|| ((cnt_unioncb==0) && (c_next>0)) )
//					{
//						bTranPass = false;//单个路径不能处理所有的负荷
//						break;
//					}
//
//					if (cnt_unioncb >= 2)//超过2个联络开关，需要调用复杂的重构函数
//					{
//						if (l_cntMainCB > 5)//超过5个可以转供的开关
//						{
//							if ((fabs(unionAund_*0.7)) < sqrt(w_next * w_next + r_next * r_next))
//							{
//								bTranPass = false;//单个路径不能处理所有的负荷
//								break;
//							}
//						}
//						else
//						{
//							//5个联络开关的两两序列，合一个，分一个
//							cntProb = lld->getRandomCB(l_cntMainCB,lCB);//得到开关序列
//							for (i=0;i<cntProb-1;i++)
//							{
//								sProb = lld->getStringByIndex(&lCB,i);//得到一个开关序列
//								k=0;
//								for (j=0;j<sProb.GetLength();j++)
//								{
//									if (sProb[j] == '0')//断开的开关
//									{
//										openCB[k++]=l_mainCB[j];
//									}
//								}
//	
//								if (k>3)//开关操作次数大于3个，没有实际价值
//									continue;
//
//								openCB[k++] = firstCB[ii];//在最后加上需要分开的入口开关
//								//不能切块分区供电, 							
//								if ((ret = OnSearchTP(cnt_unioncb,l_unCB, l_unCB_, l_unRebund, k,openCB,c_next,tranSumld)) < 0)
//									bTranPass = false;//单个路径不能处理所有的负荷
//								else
//								{
//									//bTranPass = TRUE;//单个路径不能处理所有的负荷
//									break;//只要有一种能通过就可以了
//								}
//							}
//						}
//						
//					}
//
//					break;
//				}
//			}
//		}
//	}
//	
//	//所有的下游区段都搜索完毕后，如果不能每个路径上都能转供，那么使用多个电源点重构
//	//如果单个路径的校验的电源点个数和总的电源点个数一样，没必要进行全局的校验
//	if ((sum_unioncb > 0) && (!bTranPass))//存在联络点,此处为校验联络电源点
//	{
//		//判断转供裕度
//		//计算后如果 不能做到一个电源全部转带 需要进行拆解，穷举每个开关进行拓扑分析
//		//如果有10个开关，那么就有2^10个可能，直接穷举出来就可以了
//		//一般只在主干线上有分段开关，开闭所的出线不算主干线
//
//		//给定几个开关的状态，判断一个网络的联通性，给定这个开关的起点和联络点（电源点）
//		//给定负荷的母线情况，判断哪些负荷可以转供，哪些不能
//
//		//mainCB[cntMainCB++] = cb_p;所有在路径上的开关都找到，包括首端和联络
//		//firstCB首端开关
//		//unCB[sum_unioncb] = knd;
//
//		if (cntMainCB > 5)//超过10个可以转供的开关
//		{
//			if (fabs(sumAbund*0.7) < sqrt(sumP * sumP + sumQ * sumQ))
//			{
//				bTranPass = FALSE;//单个路径不能处理所有的负荷
//			}
//		}
//		else
//		{		
//			cntProb = lld->getRandomCB(cntMainCB,lCB);//得到开关序列
//			for (i=0;i<cntProb-1;i++)
//			{
//				sProb = lld->getStringByIndex(&lCB,i);//得到一个开关序列
//				k=0;
//				for (j=0;j<sProb.GetLength();j++)
//				{
//					if (sProb[j] == '0')//断开的开关
//					{
//						openCB[k++]=mainCB[j];
//					}
//				}
//
//				if (k > 3)
//					continue;
//
//				//加上首端的开关
//				for(ii = 0; ii < noextbs; ii++)
//				{
//					openCB[k++]=firstCB[ii];
//				}
//				
//				if ((ret = OnSearchTP(sum_unioncb,unCB, unCB_, unRebund, k,openCB,cnt_ld,tranSumld)) == 1)
//				{
//					//转供成功
//					//判断转供裕度
//					deltap = (sumP-deltaMultiP);//双电源暂时不算转供
//					deltaq = (sumQ-deltaMultiQ);
//					deltaC = (sumC-deltaMultiC);
//					break;
//				}
//			}
//		}
//	}
//	
//	if (bTranPass)//通过每个支路的校验
//	{
//		deltap = (sumP-deltaMultiP);//双电源暂时不算转供
//		deltaq = (sumQ-deltaMultiQ);
//		deltaC = (sumC-deltaMultiC);
//	}
//		
//	delete []unCB;
//	delete []unCB_;
//	delete []unRebund;
//	delete []openCB;
//	delete []firstCB;
//
//	delete []l_unCB;
//	delete []l_unCB_;
//	delete []l_unRebund;
//	delete []l_openCB;
//
//	return sum_unioncb;
//}

//根据馈线（大馈线，可能含有多个平衡节点，小型分布式电源等电源点，多个联络开关）
int CNM1::OnSetAbund()
{
	int r_un,r_so;
	ChildPtr1	*un_p,*un_p0;
	ChildPtr1	*so_p,*so_p0;
	char fdid[MAX_ID_DLL],left[MAX_ID_DLL];

	ND* ndfind = new ND();
	int i,j,ind;
	int lv_so = dsbob2->so_tb->GetCount();
	SO* so = dsbob2->so_tb->GetTable();
	ND* nd = dsbob2->nd_tb->GetTable();
	int lv_fd = dsbob2->fd_tb->GetCount();
	FD* fd = dsbob2->fd_tb->GetTable();
	double abunR,dW,dR;
	double abunW;
	double p_abund;
	double q_abund;
	double v_abund;

	for(j = 0; j < lv_fd; j++)
	{
		strcpy(fdid,(fd+j)->id);

		r_so = dsbob2->fd_tb->GetDataByPh(j)->r_so;
		if (r_so != -1)//馈线上有电源点
		{
			abunR = dW = dR = 0;
			so_p = dsbob2->r_fd_so_tb->FindChildPtr1(r_so);
			so_p0 = so_p;
			while (so_p)
			{
				i = so_p->Ptr;
				if (((so + i)->slackavr == 4) || ((so + i)->slackavr == 0))
				{
					dW += ((so + i)->w) / 100;	//有功
					dR += ((so + i)->r) / 100;	//无功
						
					//馈线上的最大允许电流，与线径有关
					if ((so + i)->maxcurrent > abunR)
						abunR = (so + i)->maxcurrent;
				}
				so_p = so_p->Next;
			}
			::FreeChildPtr(so_p0);

			if (abunR > 0)
			{
				//将最大电流折合成有功和无功
				abunW = 100 * (((1.732*abunR / 1000) / 10) - sqrt(dW*dW + dR * dR));//视在功率
			}
			else
				abunW = 5;//如果对侧的联络开关裕度为0，给一个基本值，大概是5

			p_abund = abunW * 0.98;//将视在功率的折成有功裕度
			q_abund = abunW * 0.2;//无功裕度
		
			r_un = dsbob2->fd_tb->GetDataByPh(j)->r_un;
			if (r_un != -1)//区段内有下游开关
			{
				un_p = dsbob2->r_fd_un_tb->FindChildPtr1(r_un);
				un_p0 = un_p; 
				while(un_p)
				{
					strcpy(left , dsbob2->un_tb->GetDataByPh(un_p->Ptr)->left);
					if (strcmp(fdid,left)==0)
					{
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_P=p_abund;
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_Q=q_abund;

						strcpy(ndfind->id, dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_nd);
						ind = dsbob2->nd_tb->FindPh(*ndfind);
						if (ind == -1)
							v_abund = 1;
						else	
						    v_abund = dsbob2->bs_tb->GetDataByPh((nd+ind)->ibs)->v;
						if (v_abund == 0)
							v_abund = 1;
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_V = v_abund;
					}					
					else
					{
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_P=p_abund;
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_Q=q_abund;

						strcpy(ndfind->id, dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_nd);
						ind = dsbob2->nd_tb->FindPh(*ndfind);
						if (ind == -1)
							v_abund = 1;
						else
							v_abund = dsbob2->bs_tb->GetDataByPh((nd + ind)->ibs)->v;
						if (v_abund == 0)
							v_abund = 1;
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_V = v_abund;
					}	
					un_p = un_p->Next;
				}
				::FreeChildPtr(un_p0);
			}
		}
		else
		{//馈线上没有电源点，但是有联络开关
			abunW = 5;//如果对侧的联络开关裕度为0，给一个基本值，大概是5
			p_abund =  abunW * 0.98;//将视在功率的折成有功裕度
			q_abund =  abunW * 0.2;//无功裕度

			r_un = dsbob2->fd_tb->GetDataByPh(j)->r_un;
			if (r_un != -1)//区段内有下游开关
			{
				un_p = dsbob2->r_fd_un_tb->FindChildPtr1(r_un);
				un_p0 = un_p; 
				while(un_p)
				{
					strcpy(left , dsbob2->un_tb->GetDataByPh(un_p->Ptr)->left);
					if (strcmp(fdid,left)==0)
					{
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_P=p_abund;
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_Q=q_abund;

						strcpy(ndfind->id, dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_nd);
						ind = dsbob2->nd_tb->FindPh(*ndfind);
						if (ind == -1)
							v_abund = 1;
						else
							v_abund = dsbob2->bs_tb->GetDataByPh((nd + ind)->ibs)->v;
						if (v_abund == 0)
							v_abund = 10;
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_V = v_abund;
					}					
					else
					{
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_P=p_abund;
						dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_Q=q_abund;

						strcpy(ndfind->id, dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_nd);
						ind = dsbob2->nd_tb->FindPh(*ndfind);
//						ind = dsbob2->nd_tb->FindPh(ND((dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_nd)));
						if (ind == -1)
							dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_V = 1;
						else
						{
							v_abund = dsbob2->bs_tb->GetDataByPh((nd+ind)->ibs)->v;
							if (v_abund == 0)
								v_abund = 10;
							dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_V = v_abund;
						}
					}	
					un_p = un_p->Next;
				}
				::FreeChildPtr(un_p0);
			}
		}
	}

	delete ndfind;

	return 1;
}

//做一个通用的表的对分法函数

//每个联络开关所关联的馈线负荷裕度,
//so_id馈线的id号，cb_id联络开关id，返回这个联络开关对侧的裕度
int CNM1::getUNCB_abun(char* fdid,char* cb_id,char* nd_id,char* fdid_abund,double& p_abund,double& q_abund,double& v_abund)
{
	int r_un;
	ChildPtr1	*un_p,*un_p0;

	// opp_nd[MAX_ID_DLL];//联络开关对侧节点的id
//	int opp_nd;
	char left[MAX_ID_DLL];
//	double dW,dR,abunW,abunR;//对侧的电源的有功和无功

	//可以根据馈线联络开关关系表直接得到
	int ph = dsbob2->OnGetPhByfdid(fdid);
	r_un = dsbob2->fd_tb->GetDataByPh(ph)->r_un;
	if (r_un != -1)//区段内有下游开关
	{
		un_p = dsbob2->r_fd_un_tb->FindChildPtr1(r_un);
		un_p0 = un_p; 
		while(un_p)
		{
			//区段内的所有边界开关都需要打开
			if (strcmp(cb_id,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->id)==0)
			{
				strcpy(left , dsbob2->un_tb->GetDataByPh(un_p->Ptr)->left);
				if (strcmp(fdid,left)==0)
				{
					p_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_P;
					q_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_Q;
					v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_V;
				}
				else
				{
					p_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_P;
					q_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_Q;
					v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_V;
				}

				break;
			}

			un_p = un_p->Next;
		}
		::FreeChildPtr(un_p0);
	}

	return 1;
}
	
//	int ret = 0;
//	int i;
//	
//	 fdtemp[100];	//开关所属馈线的id
//	// soID[100],so_OPP_ID[100],sL[100],sR[100];
//	CString soID,so_OPP_ID,sL,sR;
//	double dW,dR,abunW,abunR;//对侧的电源的有功和无功
//	 opp_nd[80];//联络开关对侧节点的id
//
////	//馈线对侧的电源id号
////	if (fd_OPP_id[0] != '\0')
////	{
////		fd_OPP_id[0] ='\0';
////	}
//
//	soID=fdid;
//	//get so table
//	int lv_so = dsbob2->so_tb->GetCount();
//	SO* so = dsbob2->so_tb->GetTable();
//
//	CB* cb = dsbob2->cb_tb->GetTable();
//	int lv_cb = dsbob2->cb_tb->GetCount();
//
//	dW=dR=abunW=abunR=0;
//	//搜索开关列表，得到编号，对每一个开关进行搜索，可以改进此处的搜索方法
//	//对分法
//
//	int high = lv_cb - 1;
//	int low = 0;
//	int middle;
//
//    while(low <= high)
//	{
//        middle = (low + high) / 2;
//		
//		if ( strcmp(cb_id,(cb+middle)->id)==0 )
//		{  //找到,返回下标索引值
//			if ( (lld->isContainor((cb+middle)->fdid,'|') < 0)
//				|| ( !((cb+middle)->q & CB_NMLOPEN) ) )
//				continue;
//			strcpy(fdtemp,(cb+middle)->fdid);
//			lld->getLR_Bykey(fdtemp,"|",sL,sR);
//
//			if ( strcmp((cb+middle)->nd,nd_id) == 0 )
//				strcpy(opp_nd , (cb+middle)->znd);
//			else
//				strcpy(opp_nd , nd_id);
//
//			//得到对侧的电源id号,得到是对侧的电源点所属的馈线ID，不含前缀
//			if (sL == soID)
//			{
//			  so_OPP_ID = sR;	
//			}
//			else
//			{
//			  so_OPP_ID = sL;
//			}
//
//			//通过对侧的id号得到电源的实际有功和无功
//			for(i = 0; i < lv_so; i++)
//			{
//				if (strcmp((so+i)->fdid,so_OPP_ID) == 0)
//				{
//					abunR = (so+i)->MAXCURRENT;
//					//将最大电流折合成有功和无功
//
//					dW = ((so+i)->w) / 100;	//有功
//					dR = ((so+i)->r) / 100;	//无功
//					abunW = 100*(((1.732*abunR/1000)/10)  - sqrt(dW*dW+dR*dR));//视在功率
//					
//					//如果对侧的联络开关裕度为0，给一个基本值，大概是5
//					p_abund = abunW * 0.98;//将视在功率的折成有功裕度
//					q_abund =	abunW * 0.2;//无功裕度
//
//					break;
//				}
//			}
//			strcpy(fdid_abund,so_OPP_ID);//联络开关对侧的馈线id
//
//			ret = 1;
//			//无论是否找到电源点，都必须要退出
//			break;
//		}
//		else if(strcmp(cb_id,(cb+middle)->id)<0)
//		{  //查找值在低半区
//            high = middle - 1;
//        }
//		else
//		{  //查找值在高半区
//            low = middle + 1;
//        }
//    }
//
//	if ( (p_abund==0) && (q_abund==0) )
//	{
//		v_abund = 0;
//		return ret;
//	}
//
//	//节点的电压
//	ND* nd = dsbob2->nd_tb->GetTable();
//	BS* bs = dsbob2->bs_tb->GetTable();
//	int lv_nd = dsbob2->nd_tb->GetCount();
//
//	high = lv_nd - 1;
//	low = 0;
//
//    while(low <= high)
//	{
//        middle = (low + high) / 2;
//		
//		if ( strcmp(opp_nd,(nd+middle)->id)==0 )
//		{  //找到,返回下标索引值
//			int ibs = (nd+middle)->ibs;
//			v_abund = (bs+ibs)->v;
//			break;
//		}
//		else if(strcmp(opp_nd,(nd+middle)->id)<0)
//		{  //查找值在低半区
//            high = middle - 1;
//        }
//		else
//		{  //查找值在高半区
//            low = middle + 1;
//        }
//    }

//so_id馈线的id号，cb_id联络开关id，返回这个联络开关对侧的裕度
//如果不指定联络开关，则计算这条线的整个开关返回
double CNM1::getUNCB_abun(char* fdid,char* so_OPP_id,char* cb_id)
{
	int r_un;
	ChildPtr1	*un_p,*un_p0;

	// opp_nd[MAX_ID_DLL];//联络开关对侧节点的id
//	int ibs;
	char left[MAX_ID_DLL];
	double p_abund,q_abund,v_abund;//对侧的电源的有功和无功
	p_abund=q_abund=v_abund=0;
	if (so_OPP_id[0] != '\0')
	{
		so_OPP_id[0] ='\0';
	}

	//可以根据馈线联络开关关系表直接得到
	int ph = dsbob2->OnGetPhByfdid(fdid);
	r_un = dsbob2->fd_tb->GetDataByPh(ph)->r_un;
	if (r_un != -1)//区段内有下游开关
	{
		un_p = dsbob2->r_fd_un_tb->FindChildPtr1(r_un);
		un_p0 = un_p; 
		while(un_p)
		{
			//区段内的所有边界开关都需要打开
			if (cb_id==0)
			{
				strcpy(left , dsbob2->un_tb->GetDataByPh(un_p->Ptr)->left);
				if (strcmp(fdid,left)==0)
				{
					p_abund += dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_P;
					q_abund += dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_Q;
					v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_V;
				}
				else
				{
					p_abund += dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_P;
					q_abund += dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_Q;
					v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_V;
				}

				strcat(so_OPP_id,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->id);//准备输出
				strcat(so_OPP_id,",");
			}
			else
			if (strcmp(cb_id,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->id)==0)
			{
				strcpy(left , dsbob2->un_tb->GetDataByPh(un_p->Ptr)->left);
				if (strcmp(fdid,left)==0)
				{
					p_abund += dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_P;
					q_abund += dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_Q;
					v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_V;
				}
				else
				{
					p_abund += dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_P;
					q_abund += dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_Q;
					v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_V;
				}
				strcat(so_OPP_id,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->id);//准备输出

				break;
			}

			un_p = un_p->Next;
		}
		::FreeChildPtr(un_p0);
	}

	return sqrt(p_abund*p_abund+q_abund*q_abund);




//	int m,i;
//	 fdtemp[100];	//开关所属馈线的id
//	CString soID,so_OPP_ID,sL,sR;
//	double dW,dR,abunW,abunR,temp_pq;//对侧的电源的有功和无功
//
//	if (so_OPP_id[0] != '\0')
//	{
//		so_OPP_id[0] ='\0';
////		for (i=0; i<strlen(so_OPP_id); i++)
////			so_OPP_id[i]='';
//	}
//	soID=fdid;
//	//get so table
//	int lv_so = dsbob2->so_tb->GetCount();
//	SO* so = dsbob2->so_tb->GetTable();
//
//	CB* cb = dsbob2->cb_tb->GetTable();
//	int lv_cb = dsbob2->cb_tb->GetCount();
//	//read cb table into buffer
////	CB* cb_t = new CB[lv_cb];
////	dsbob2->cb_tb->CopyTable(cb_t);
//
//	dW=dR=abunW=abunR=0;
//	//搜索开关列表，得到编号
//	for ( m = 0; m < lv_cb; m++ )
//	{
//		if ((cb+m)->q & 1)//联络开关同时开关要在岛内
//		{
//				//判断是不是悬空的开关，如果是悬空的，可能就不是联络线
//			if (cb_id == NULL)//搜索给定馈线的所有的联络开关
//			{
//				if (lld->isContainor((cb+m)->fdid,'|') < 0)
//					continue;
//
//				strcpy(fdtemp,(cb+m)->fdid);
//				lld->getLR_Bykey(fdtemp,"|",sL,sR);
//
//				//得到对侧的电源id号
//				if ((sL == soID) || (sR == soID))
//				{
//					if (sL == soID)
//					{
//					  so_OPP_ID = sR;	
//					}
//					else
//					{
//					  so_OPP_ID = sL;
//					}
//
//					//通过对侧的id号得到电源的实际有功和无功
//					for(i = 0; i < lv_so; i++)
//					{
//						if (strcmp((so+i)->fdid,so_OPP_ID) == 0)
//						{
//							if (so_OPP_ID != "")
//							{
//								strcat(so_OPP_id,so_OPP_ID);//准备输出
//								strcat(so_OPP_id,",");
//							}
//
//							abunR = (so+i)->MAXCURRENT;
//							abunR = (1.732*abunR/1000)/10;
//
//							dW = ((so+i)->w) / 100;	//有功
//							dR = ((so+i)->r) / 100;	//无功
//							temp_pq = sqrt(dW*dW+dR*dR);
//							if (temp_pq < 0.9 * abunR)
//								abunW += 100*(abunR - temp_pq);
//
//						}
//					}
//				}
//			}
//			else
//			{
//				if ( strcmp((cb+m)->id,cb_id) == 0)
//				{
//					if (lld->isContainor((cb+m)->fdid,'|') < 0)
//						continue;
//
//					strcpy(fdtemp,(cb+m)->fdid);
//					lld->getLR_Bykey(fdtemp,"|",sL,sR);
//
//					//得到对侧的电源id号,得到是对侧的电源点所属的馈线ID，不含前缀
//					if (sL == soID)
//					{
//					  so_OPP_ID = sR;	
//					}
//					else
//					{
//					  so_OPP_ID = sL;
//					}
//
//					//通过对侧的id号得到电源的实际有功和无功
//					for(i = 0; i < lv_so; i++)
//					{
//						if (strcmp((so+i)->fdid,so_OPP_ID) == 0)
//						{
//							abunR = (so+i)->MAXCURRENT;
//
//							dW += ((so+i)->w) / 100;	//有功
//							dR += ((so+i)->r) / 100;	//无功
//
//							abunW += 100*(((1.732*abunR/1000)/10)  - sqrt(dW*dW+dR*dR));						
//
//							break;
//						}
//					}
//
//					strcpy(so_OPP_id,so_OPP_ID);//准备输出
//					break;
//				}
//			}
//		}
//	}

//	delete[] cb_t;
//	cb_t = 0;

//	return 0.9 * fabs(abunW);
}

//针对每一个区段计算如果该区段发生停电，下游的用户是否可以转供
//在计算是有这样的假设：
//1.在某个区段发生故障后，损失的用户数是本区段内的用户，下游不能转供的用户，上游用户无损失
//2.双（多）电源用户永远都不停电
//3.所有的线段都是区段的一部分，区段是计算的最小单元
//4.如果发生n-1后，可以转供的只有和本条线路连接的联络开关，不考虑其它线路向外转供负荷的问题

//可以以馈线段为单位来做，如果这个馈线段出现故障，将会影响到下游的哪些负荷
//哪些负荷会转供，哪些损失掉了，如果损失了，就可以认为不满足n-1
//可以转供的负荷大小，损失负荷大小

//对于下游的负荷和电源，需要考虑的仅仅是下游的开关进行组合
//在此假设条件下，可以动的开关是很少的，穷举已经可以做到了

//逐条线计算n-1
int CNM1::PutNM1Data()
{
//	int		i,j,iNO;
//	double dReal,dImp;
//	double curW;//本条馈线的视在功率
//	double tranW;//联络的馈线裕度
////	int		ibs, zbs;	//线段的首末母线（节点优化编号前）
////	int		iout,zout;	//线段首末节点是否在网络内
//	   unionSO[MAX_UION_ID];//联络的电源点id集合，中间用，分开	
//
////文件名要加上用户名和版本号，这样才可以区别开
//	CString sFileName,sSQL,fdid,unionList,transSec,lossSec,zoneLoss;
//	 secid[MAX_SEC_ID_LEN];
////	sFileName = "nm1.efile";
////
////	ofstream outfile(sFileName,ios::out);
////
////	outfile<<"<!System= Version=1.0 Code=UTF-8 Data=!>"<< endl;
////	outfile<<"<so_detailResult::Result name=N-1扫描详细结果表>"<< endl;
////	outfile<<"@	  ID       nd       ndCB     Faultload          Trload					NowTrload				 PowerLostRatio						unionSO      Result			LineResult"<< endl;
////	outfile<<"/@  结果编号 线路id 分段id 故障段失电负荷(MW) 非故障段需转移负荷(MW)	非故障段实际转移负荷(MW) 非故障段失电负荷所占总负荷的比例%  转供电源     分断N-1计算结果 全线N-1计算结果"<< endl;
//    
//	double Faultload;	//故障段失电负荷(MW)
//	double Trload;		//非故障段需转移负荷(MW)
//	double NowTrload;	//非故障段实际转移负荷(MW)
//	double PowerLostRatio;//非故障段失电负荷所占总负荷的比例%
//	 Result;			//分断N-1计算结果 
//	 LineResult;		//全线N-1计算结果
//	int iCountSect;		//线段的分段数量
//
////
//	ZN* zn;
//	int r_zn,r_sec;
//	ChildPtr1	*sec_p,*sec_p0 ;
//
//
//	//先做一个全景的扫描，得到每条馈线段的故障后果
//	//*****此为核心代码，后面主要是对这段代码进行结果整理
//	PutRiskData();
//
//	//对每一条线段进行计算
//	//**********每次只做一个岛，每次只写入部分线段的数据，最后合并起来就是全网
//	//先进行本条馈线（岛）的馈线段n-1计算，计算结果已经写到sec的几个字段里面
//	//get sec table
//	int lv_sec = dsbob2->sec_tb->GetCount();
//	SEC* sec = dsbob2->sec_tb->GetTable();
//
//	BS* bs = dsbob2->bs_tb->GetTable();
//
//	iNO = 1;
//	int lv_so = dsbob2->so_tb->GetCount();
//	SO* so = dsbob2->so_tb->GetTable();
//	for(i = 0; i < lv_so; i++)
//	{
////////////////////////////////////////////////////////////////////////////
////1.第一步，进行全部线路的n-1，假设电源出口处出现故障，本条线的所有负荷是否可以转带
////		dReal = (so+i)->w;//含有一部分线损在里面，用负荷容量进行累加可以更准确
////		dImp  = (so+i)->r;
//		
////if (i==98)
////{
////	int debu=0;
////}
//
//		dReal = (dsbob2->island_tb->GetTable()+(so+i)->iisland)->wload;
//		dImp  = (dsbob2->island_tb->GetTable()+(so+i)->iisland)->rload;
//
//		curW = sqrt(dReal*dReal + dImp*dImp);//当前电源点的视在功率,返回的是有名值
//
//		if (curW == 0)
//		{
//			PowerLostRatio=0;//非故障段失电负荷所占总负荷的比例%
//			LineResult = '0';
//			tranW = 0;
//			unionList = "-";
//		}
//		else
//		{
//			//查找与本条线连接的其它馈线,以及相关的裕度
//			tranW = getUNCB_abun((so+i)->fdid,unionSO);
//			unionList = unionSO;
//			if ((unionList=="") || (unionList==",") )
//				unionList = "-";
//			else
//				unionList=unionList.Left(unionList.GetLength()-1);
//
//			if ((tranW) > curW)//如果对侧的联络裕度大于需要转供的裕度，整条馈线的n-1可以通过
//			{
//				PowerLostRatio=0;//非故障段失电负荷所占总负荷的比例%
//				LineResult = '1';
//				tranW = curW;
//			}
//			else
//			{
//				PowerLostRatio=100;//非故障段失电负荷所占总负荷的比例%
//				LineResult = '0';
//				tranW = 0;
//			}
//		}
//
//		fdid = (so+i)->fdid;
//		//strcpy(secid , "-");
//		Faultload=0;	//故障段失电负荷(MW)
//		Trload=curW;		//非故障段需转移负荷(MW)
//		NowTrload=tranW;	//非故障段实际转移负荷(MW)
//		
//		//;		//全线N-1计算结果
//		dsbob2->nm1_result[lld->g_cnt_nm1_fd].id=lld->g_cnt_nm1_fd;
//		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].fdid,fdid);
//		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].secid,"整条线n-1");
//		dsbob2->nm1_result[lld->g_cnt_nm1_fd].Faultload = Faultload;
//		dsbob2->nm1_result[lld->g_cnt_nm1_fd].Trload=Trload;
//		dsbob2->nm1_result[lld->g_cnt_nm1_fd].NowTrload=NowTrload;
//		dsbob2->nm1_result[lld->g_cnt_nm1_fd].PowerLostRatio=PowerLostRatio;
//		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].unionList , unionList);
//		dsbob2->nm1_result[lld->g_cnt_nm1_fd].secResult='-';
//		dsbob2->nm1_result[lld->g_cnt_nm1_fd].LineResult=LineResult;
//		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].transSec , "-");
//		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].lossSec , "-");
//		lld->g_cnt_nm1_fd++;
//
////		sSQL.Format("# %d\t%s\t整条线n-1\t%0.4f\t%0.4f\t%0.4f\t%0.2f\t%s\t-\t%d", \
////			          iNO++,fdid,Faultload,Trload,NowTrload,PowerLostRatio, unionList,LineResult);
////
////		outfile.write(sSQL,sSQL.GetLength());
////		outfile<<endl;
//
//		//通过馈线id来区分
////////////////////////////////////////////////////////////////////////////
////2.做每个馈线段。如果每个馈线段都能通过n-1，则这条线可以通过，否则这条线不能通过
//		//每条馈线都有所属区段的标示r_zn
//		LineResult = 1;
//		iCountSect = 0;
//        for(j = 0; j < lv_sec; j++)
//		{
////			ibs = (sec+j)->ibs;
////			zbs = (sec+j)->zbs;
//
////			iout = (sec+j)->topflags & SEC_IOPEN || (bs+ibs)->iisland < 0;
////			zout = (sec+j)->topflags & SEC_ZOPEN || (bs+zbs)->iisland < 0;
////			
////			if(iout || zout)//线段不在岛内
////			{
////				continue;			
////			}
//
////判断这个馈线段是否在这个电源点里面
//			//if (fdid == (sec+j)->fdid)
//			if (strcmp(fdid , (sec+j)->fdid) == 0)
//			{//mxP = mxP>mxQ ? mxP:mxQ
//				strcpy(secid,(sec+j)->id);
////			
////				//得到馈线所属区段内所有馈线段
////				if ((r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1((sec+j)->r_zn)) == -1)
////					continue;
////
////				zn = dsbob2->zn_tb->GetDataByPh(r_zn); 
////
////				//先在区段表里面找到这个区段，得到r_ld,再用r_ld进行搜索
////				r_sec = zn->r_sec;
////				sec_p =  dsbob2->r_zn_sec_tb->FindChildPtr1(r_sec);
////				
////				//得到线段所属的区段负荷
////				zoneLoss="";
////				sec_p0 = sec_p; 
////				while(sec_p)
////				{
////					zoneLoss += dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->id;
////					zoneLoss += "|";
////
////					sec_p = sec_p->Next;
////				}
////				::FreeChildPtr(sec_p0);
////
////				if (zoneLoss=="")
////					zoneLoss = "-";
////				else
////					zoneLoss=zoneLoss.Left(zoneLoss.GetLength()-1);
//
//
////				lld->delstrright((sec+j)->id,7,secid);	
//				if ((sec+j)->lP_zn  > 0.001)
//					Faultload	= (sec+j)->lP_zn;	//故障段失电负荷(MW)
//				else
//					Faultload	= 0;	//故障段失电负荷(MW)
//
//				if (fabs((sec+j)->lP_znnext) > 0.001)
//					Trload		= fabs((sec+j)->lP_znnext);		//非故障段需转移负荷(MW)
//				else
//					Trload		= 0;
//
//				if (fabs((sec+j)->transP) > 0.001)
//					NowTrload	= fabs((sec+j)->transP);	//非故障段实际转移负荷(MW)
//				else
//					NowTrload	= 0;
//
//				unionList   = (sec+j)->tranfdlist;
//				if ((unionList=="") || (unionList==",") )
//					unionList = "-";
//				else
//					unionList=unionList.Left(unionList.GetLength()-1);
//
//				transSec = (sec+j)->closeunlist;
//				if ( (transSec=="") || (transSec=="|") || (transSec=="-"))
//					transSec = "-";
//				else
//					transSec=transSec.Left(transSec.GetLength()-1);
//
//				lossSec  = (sec+j)->opencblist;
//				if ((lossSec=="") || (lossSec=="|")  || (lossSec=="-"))
//					lossSec = "-";
//				else
//					lossSec=lossSec.Left(lossSec.GetLength()-1);
//				
//				if (fabs(Trload - NowTrload) < 0.01)
//				{
//					PowerLostRatio = 0;
//					Result = '1';
//				}
//				else
//				{
//					if (NowTrload > 0)
//						PowerLostRatio=100 * ((Trload-NowTrload)/Trload);//非故障段失电负荷所占总负荷的比例%
//					else
//						PowerLostRatio=100;
//
//					LineResult = '0';//只要有一个不满足n-1，全线都不满足
//					Result = '0';
//				}
//				
//				//分段N-1计算结果
//				dsbob2->nm1_result[lld->g_cnt_nm1_fd].id=lld->g_cnt_nm1_fd;
//				strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].fdid,fdid);
//				strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].secid,secid);
//				dsbob2->nm1_result[lld->g_cnt_nm1_fd].Faultload = Faultload;
//				dsbob2->nm1_result[lld->g_cnt_nm1_fd].Trload=Trload;
//				dsbob2->nm1_result[lld->g_cnt_nm1_fd].NowTrload=NowTrload;
//				dsbob2->nm1_result[lld->g_cnt_nm1_fd].PowerLostRatio=PowerLostRatio;
//				strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].unionList , unionList);
//				dsbob2->nm1_result[lld->g_cnt_nm1_fd].secResult=Result;
//				dsbob2->nm1_result[lld->g_cnt_nm1_fd].LineResult='-';
//				strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].transSec , transSec);
//				strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].lossSec , lossSec);
//				//strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].zoneLoss , zoneLoss);
//				lld->g_cnt_nm1_fd++;
//
////			sSQL.Format("# %d\t%s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.2f\t%s\t%d\t-", \
////			          iNO++,fdid,secid,Faultload,Trload,NowTrload,PowerLostRatio, unionList,Result);
////
////				
////		//sSQL.Format("# %d\t%s\t%s\t%f\t%f\t%f\t%f\t%s",iNO++,fdid,(sec+i)->descr,Faultload,Trload,NowTrload,Result);
////
////				outfile.write(sSQL,sSQL.GetLength());
////				outfile<<endl;
////				iCountSect++;
//
//			}
//		}
//
//		//所有的馈线段结束后可以得到这条馈线是否满足n-1的条件//分断N-1计算结果 
////		if (iCountSect > 0)
////		{
//// 			sSQL.Format("# %d\t%s\t分段n-1总结果\t-\t-\t-\t-\t-\t-\t%d",iNO++,(so+i)->fdid,LineResult);
////
////			outfile.write(sSQL,sSQL.GetLength());
////			outfile<<endl;
////		}
//	}
////	outfile<<"</so_detailResult::Result>"<< endl;
////	outfile<<" "<< endl;
////	
////	outfile.close();

 	return 1;
}

//本程序有一个局限性，就是只做了线段的N-1，没有做设备的N-1
//实际上，每一个设备的N-1也需要考虑，比如线路上的开关的N-1，变压器的N-1等等

//如果要用到FEMA计算可靠性，还需要得出馈线段失电后，下游损失的负荷个数，下游可以转供的个数，上游需要恢复的个数
//可以用一个结构来存储该信息
int CNM1::PutRiskData()
{
	int		i,j,ret;
	
	int		lv_sec;//线段的指针，线段条数
	int		ibs, zbs;	//线段的首末母线（节点优化编号前）
	int		iout,zout;	//线段首末节点是否在网络内
	
	int		iznC;		//区段内用户数
	double	ld_pq_temp;	//区段中最大的负荷（含下游负荷）
		
	//CString intnode[MAX_ZNNO],extnode[MAX_ZNNO];//存区段边界节点
	int extbs[MAX_ZNNO],upextbs[MAX_ZNNO];//存区段下游扩展母线及其上游母线
	ChildPtr1	*cb_p,*cb_p0, *ld_p,*ld_p0 ;
	int ld_nd, r_zn,r_ld,r_cb;//负荷的计算母线号,节点优化后的
	CB*  cb1;

	int noextbs;

	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();
	
	//get sec table
	lv_sec = dsbob2->sec_tb->GetCount();
	SEC* sec = dsbob2->sec_tb->GetTable();
	
	//区段表重置
	int lv_zn = dsbob2->zn_tb->GetCount();
	ZN* zn = dsbob2->zn_tb->GetTable();
	for (i = 0; i < lv_zn; i++)
	{
		(zn+i)->lossC = -1; //区段自己和下游加起来一起损失的用户
		(zn+i)->transC = -1;//下游可以转供的用户
	}

	//get sec table
	int lv_ld = dsbob2->ld_tb->GetCount();
	LD* ld = dsbob2->ld_tb->GetTable();

	//对每一条线段进行计算
	//**********每次只做一个岛，每次只写入部分线段的数据，最后合并起来就是全网

//1.先判断线段所属区段，查看区段的ialg4q（区段内用户数），w（区段内有功）
	//区段下游用户数ialg4p（区段下游用户数），区段下游有功（wz）
//2.如果是新区段，计算该区段的上述4个值，填到区段里面
//3.这里有一个重要是前提，每个区段的第一个开关为进入区段的开关。在做区段的时候已经做了这个

//4.在做之前，准备ra_cnt和ra_feeder[MAX_RA_CNT];
//	ra_cnt = 0;
//    for(i = 0; i < MAX_RA_CNT; i++)
//    {
//		ra_feeder[i].ld_id=-1;
//		ra_feeder[i].up_cb=-1;
//		ra_feeder[i].out=-1;
//		ra_feeder[i].tran=-1;
//		ra_feeder[i].swit=-1;
//	}
//printf("max ra cnt:"+i);

    for(i = 0; i < lv_sec; i++)
    {
//		sec_ana = i;//正在分析的馈线段，全局变量，在其他的函数里面可能会用到

        ibs = (sec+i)->ibs;
        zbs = (sec+i)->zbs;

        iout = (sec+i)->topflags & SEC_IOPEN || (bs+ibs)->iisland < 0;
        zout = (sec+i)->topflags & SEC_ZOPEN || (bs+zbs)->iisland < 0;
		
		 //SEG_1012260900 SEG_1012260976 SEG_1012261668 SEG_1002673994	
//	if (strcmp((sec+i)->fdid,"CIRCUIT_1003150390")==0)
//	{
//		int ddddd=0;
//	}
//+	id	0x0c736e10 "SEG_1018146472"

//	if (strcmp((sec+i)->id,"SEG_1020908087")==0)
//	{
//		int ddddd=0;
//	}

	
		if(iout && zout)//线段同时不在岛内
        {
			//搜索这个线段的下游的用户和负荷
            (sec+i)->lossP   = 0;//(sec+i)->lP_znnext;
            (sec+i)->transP  = 0;
            (sec+i)->lossC   = 0;//(sec+i)->lC_znnext;
            (sec+i)->transC  = 0;

			(sec+i)->lC_zn = 0;//区段内用户
			(sec+i)->lP_zn = 0;//区段内有功
			(sec+i)->lC_znnext = 0;//区段下游用户
			(sec+i)->lP_znnext = 0;//区段下游有功
			strcpy((sec+i)->tranfdlist,"");//转供路径

			//区段的联络开关需要由联络线路的列表来实现
			strcpy((sec+i)->closeunlist,"-");//转供路径
			strcpy((sec+i)->opencblist,"-");//转供路径

			continue;			
        }
		
		up_cb = -1;//默认没有上游开关
		for(j = 0; j < lv_ld; j++)
		{
			ldlist[j] = -1;		//所有的负荷都未处理
		}

//区段和线段的关系
		//通过区段ID得到和区段关联的负荷的指针
		if ( ((r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1((sec+i)->r_zn)) == -1)
			|| ((sec+i)->r0 == 0)//0阻抗支路
			)
		{//线段不属于任何区段

            (sec+i)->lossP   = 0;//(sec+i)->lP_znnext;
            (sec+i)->transP  = 0;
            (sec+i)->lossC   = 0;//(sec+i)->lC_znnext;
            (sec+i)->transC  = 0;

			(sec+i)->lC_zn = 0;//区段内用户
			(sec+i)->lP_zn = 0;//区段内有功
			(sec+i)->lC_znnext = 0;//区段下游用户
			(sec+i)->lP_znnext = 0;//区段下游有功
			strcpy((sec+i)->tranfdlist,"");//转供路径

			//区段的联络开关需要由联络线路的列表来实现
			strcpy((sec+i)->closeunlist,"-");//转供路径
			strcpy((sec+i)->opencblist,"-");//转供路径

			continue;
		}

		zn = dsbob2->zn_tb->GetDataByPh(r_zn); 

		int lossC =  zn->lossC;    //区段自己和下游加起来一起损失的用户,上游不损失
		int transC = zn->transC;    //下游可以转供的用户
		iznC = zn->inner_C;//区段内的用户数量
		if ((lossC == -1) && (transC == -1))//区段没有计算过
		{
//////////////////////////////////////////////////////////////////////////
//区段上游开关的情况，判断开关是否是熔断器和带保护的断路器，如果是，无切换时间

			r_cb = zn->r_cb;
			if (r_cb != -1)//区段内有下游开关
			{
				cb_p = dsbob2->r_zn_cb_tb->FindChildPtr1(r_cb);
				if (cb_p)//区段内存在第一个开关，即进入的开关
				{
					cb1 = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr);
					if (cb1->cbtype==3)
						up_cb = 3;

					if (cb1->cbtype==5)
						up_cb = 5;
				}
			}

//////////////////////////////////////////////////////////////////////////
//计算本区段内的情况
			zn_ld_cnt=zn_ld_pq=zn_multi_ld_cnt=zn_multi_ld_pq=0;
			open_idList[0]=0;//区段下游没有分段开关打开

			if (iznC > 0)//区段内有负荷点
			{
				//先在区段表里面找到这个区段，得到r_ld,再用r_ld进行搜索
				r_ld = zn->r_ld;
				ld_p =  dsbob2->r_zn_ld_tb->FindChildPtr1(r_ld);
				
				//得到线段所属的区段负荷
				ld_p0 = ld_p; 
				while(ld_p)
				{
					ld_pq_temp =sqrt((double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w)*(double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w)+(double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r)*(double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r)); 
					//视在功率
					zn_ld_cnt++;
					zn_ld_pq += ld_pq_temp;
						
					ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
					if ( (dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & (ND_SO) )//多电源
					{
						zn_multi_ld_cnt++;
						zn_multi_ld_pq += ld_pq_temp;//区段内多电源的视在功率
					}//多电源在计算停电损失的时候要考虑到
	
					//一定损失的负荷加到负荷列表里面
//					ra_feeder[ra_cnt].ld_id = ld_p->Ptr;//负荷的物理号
//					ra_feeder[ra_cnt].up_cb = up_cb;//上游开关的性质，对故障切换有用
//					ra_feeder[ra_cnt].out = 1;//负荷损失
//					ra_feeder[ra_cnt].swit = 0;//负荷不能切换
//					ra_feeder[ra_cnt].tran = 0;//负荷不能转供
//					ra_feeder[ra_cnt].sec_id = i;//负荷对应的故障馈线物理号（此时物理逻辑统一）
//					ldlist[ld_p->Ptr] = 1;//负荷已处理
//					ra_cnt++;

					ld_p = ld_p->Next;
				}
				::FreeChildPtr(ld_p0);
			}
			
//////////////////////////////////////////////////////////////////////////
//计算区段下游的转供情况
			//区段的类型
			noextbs = zn->type; 

			if (noextbs > MAX_ZNNO)//应该是图画错了，一般不会出现这种情况
			{
				//搜索这个线段的下游的用户和负荷
				(sec+i)->lossP   = 0;//(sec+i)->lP_znnext;
				(sec+i)->transP  = 0;
				(sec+i)->lossC   = 0;//(sec+i)->lC_znnext;
				(sec+i)->transC  = 0;

				(sec+i)->lC_zn = 0;//区段内用户
				(sec+i)->lP_zn = 0;//区段内有功
				(sec+i)->lC_znnext = 0;//区段下游用户
				(sec+i)->lP_znnext = 0;//区段下游有功
				strcpy((sec+i)->tranfdlist,"");//转供路径

				//区段的联络开关需要由联络线路的列表来实现
				strcpy((sec+i)->closeunlist,"-");//转供路径
				strcpy((sec+i)->opencblist,"-");//转供路径

				char slog[256];
				sprintf(slog,"%s 线路的区段边界开关数为%d，请检查网络",(sec+i)->fdid,noextbs);
			lld->WriteLog(lld->logfile,slog);

				continue;
			}

///得到区段-开关的相互关系，获得区段的内外节点
			r_cb = zn->r_cb;
			if (r_cb != -1)//区段内有下游开关
			{
				cb_p = dsbob2->r_zn_cb_tb->FindChildPtr1(r_cb);
				
				//得到区段内的全部开关,包括首端开关
				int ii=0;
				cb_p0 = cb_p; 
				while(cb_p)
				{
					//区段内的所有边界开关都需要打开
					cb1 = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr);
					//如果边界开关是联络的话，也是不需要加入分析的
					if (cb1->q & CB_NMLOPEN)  
					{
						//仅仅只是一个开位的开关是不能判断的，必须没有联络的线路才是
						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
						{
							cb_p = cb_p->Next;
							continue;				
						}
					}					
					
					strcat(open_idList,cb1->id);
					strcat(open_idList,"|");
					if (cb_p->Ptr == zn->cbptr)//进入区段的开关需要跳过
					{
						//将进入开关的指针写到打开的隔离开关序列里面

						cb_p = cb_p->Next;
						continue;				
					}
					extbs[ii] = cb1->iznd;//边界开关的外侧节点的母线
					upextbs[ii] = cb1->ind;//边界开关内侧节点所连的馈线段对侧节点的计算母线
			
					ii++;
					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//lld add 2014-1-15
				//noextbs下游边界开关数量,extbs边界外节点,upextbs边界内节点,区段外进入搜索的开关，可以是多个开关
				//ret == 0;nm1不通过 
				ret = residueAnalyze(ii,extbs,upextbs);
				//残余网络分析结束后，将岛内的结果反应出来
				if (ret)//下游可以转供，可能是全部也有可能只有部分能转供
				{
					zn->inner_C = zn_ld_cnt;//区段内的负荷个数
					zn->inner_P = zn_ld_pq;//区段内的负荷视在功率

					zn->transC = downstream_trans_ld_cnt;//下游可以转供的用户
					zn->lossC = zn_ld_cnt+downstream_ld_cnt-zn_multi_ld_cnt-downstream_multi_ld_cnt-downstream_trans_ld_cnt; //区段自己和下游加起来一起损失的用户
					zn->transP = downstream_trans_ld_pq;
					zn->lossP = zn_ld_pq+downstream_ld_pq-zn_multi_ld_pq-downstream_multi_ld_pq-downstream_trans_ld_pq;

					zn->next_C = downstream_ld_cnt;//区段下游用户
					zn->next_P = downstream_ld_pq;//区段下游的负荷视在功率
				
					//有可能是多个联络开关合，多个分段开关分
					strcpy(zn->unfd_idList,unfd_idList);//联络线路还是要传出去，只是有一些变化

					//根据联络开关所带的馈线求联络开关
					strcpy(zn->closeunlist,un_idList);//联络线路还是要传出去，只是有一些变化
					
//					strcpy(zn->transSec,transSec);//联络线路还是要传出去，只是有一些变化
//					strcpy(zn->lossSec,lossSec);//联络线路还是要传出去，只是有一些变化
				}
				else
				{//下游全部损失
					zn->inner_C = zn_ld_cnt;//区段内的负荷个数
					zn->inner_P = zn_ld_pq;//区段内的负荷视在功率

					zn->transC = 0;//下游可以转供的用户
					zn->lossC = zn_ld_cnt-zn_multi_ld_cnt; //区段自己和下游加起来一起损失的用户
					zn->transP = 0;
					zn->lossP = zn_ld_pq-zn_multi_ld_pq;

					zn->next_C = 0;//区段下游用户
					zn->next_P = 0;//区段下游的负荷视在功率
					strcpy(zn->unfd_idList,"");//联络线路还是要传出去，只是有一些变化
					zn->closeunlist[0] = 0;//下游没有分段开关需要打开
					//zn->opencblist[0] = 0;//下游没有分段开关需要打开

//					strcpy(zn->transSec,"-");//联络线路还是要传出去，只是有一些变化
//					strcpy(zn->lossSec,"-");//联络线路还是要传出去，只是有一些变化
				}
				strcpy(zn->opencblist,open_idList);//下游需要打开的分段开关列表
			}
			else//区段内没有下游开关，下游全部损失
			{
				zn->inner_C = zn_ld_cnt;//区段内的负荷个数
				zn->inner_P = zn_ld_pq;//区段内的负荷视在功率

				zn->transC = 0;//下游可以转供的用户
				zn->lossC = zn_ld_cnt-zn_multi_ld_cnt; //区段自己和下游加起来一起损失的用户
				zn->transP = 0;
				zn->lossP = zn_ld_pq-zn_multi_ld_pq;

				zn->next_C = 0;//区段下游用户
				zn->next_P = 0;//区段下游的负荷视在功率
				strcpy(zn->unfd_idList,"");//联络线路还是要传出去，只是有一些变化
				zn->closeunlist[0] = 0;//下游没有分段开关需要打开
			}						   
		}		

		//将区段内的相关信息赋值给所属的馈线段
        (sec+i)->lossP   = zn->lossP;		//区段及其下游总负荷
        (sec+i)->transP  = zn->transP;      //区段下游可以转供的负荷，上游不需要转供
        (sec+i)->lossC   = zn->lossC;	//区段自己和下游加起来一起损失的用户
        (sec+i)->transC  = zn->transC;	//下游可以转供的用户
		(sec+i)->lC_zn = zn->inner_C;//区段内用户
		(sec+i)->lP_zn = zn->inner_P;//区段内有功
		(sec+i)->lC_znnext = zn->next_C;//区段下游用户
		(sec+i)->lP_znnext = zn->next_P;//区段下游有功
		strcpy((sec+i)->tranfdlist,zn->unfd_idList);//转供路径

		//区段的联络开关需要由联络线路的列表来实现
		strcpy((sec+i)->closeunlist,zn->closeunlist);//转供路径
		strcpy((sec+i)->opencblist,zn->opencblist);//转供路径

//		strcpy((sec+i)->transSec, zn->transSec);//联络线路还是要传出去，只是有一些变化
//		strcpy((sec+i)->lossSec, zn->lossSec);//联络线路还是要传出去，只是有一些变化
		
		//负荷在上游的也要找出来
		//这条馈线段所属的馈线		
	    for(j = 0; j < lv_ld; j++)
		{
			if ( (ldlist[j] == -1)
				 && (strcmp((ld+j)->fdid,(sec+i)->fdid)==0)//负荷属于这条馈线
				 )
			{
//				ra_feeder[ra_cnt].ld_id = j;//负荷的物理号
//				ra_feeder[ra_cnt].up_cb = up_cb;//上游开关的性质，对故障切换有用
//				ra_feeder[ra_cnt].out = 0;//负荷损失
//				ra_feeder[ra_cnt].swit = 1;//负荷能切换
//				ra_feeder[ra_cnt].tran = 0;//负荷不能转供
//				ra_feeder[ra_cnt].sec_id = i;//负荷对应的故障馈线物理号（此时物理逻辑统一）
//				ra_cnt++;
			}
		}

	}

	return 1;
}




//针对每一个馈线段得到相关的风险指标
//在计算是有这样的假设：
//1.在某个线段发生故障后，损失的用户数是本区段内的用户，下游不能转供的用户，上游用户无损失
//2.双（多）电源用户永远都不停电
//3.所有的线段都是区段的一部分，区段是计算的最小单元
//int CNM1::PutRiskData()
//{
//	int		i/*,k*/;
//	
//	int		lv_sec;//线段的指针，线段条数
//	//	CString secID;		//线段ID
//	int		ibs, zbs;	//线段的首末母线（节点优化编号前）
//	int		iout,zout;	//线段首末节点是否在网络内
//	
//	double	lossP,transP;	//线段故障后损失负荷（包括自己），转供负荷
//	int		lossC,transC;	//线段故障后损失用户，转供用户
//	int inner_C,next_C;
//	double inner_P,next_P;
//	 unfd_idList[400];
//	double  sumP,sumQ,deltap,deltaq;//下游总负荷，下游可转供负荷
//	int     sumC,deltaC;	//下游总用户，下游可转供用户
//	double  deltaMultiP,deltaMultiQ;	//下游双电源负荷
//	int     deltaMultiC;	//下游双电源用户
//	
//	double	znP;	//区段内总负荷
//	
//	int		iznC;		//区段内用户数
//	double	mxP,mxQ,mxTemp;	//区段中最大的负荷（含下游负荷）
//	
//	int		multiC;	//区段内双电源的个数
//	double	multiP;	//区段中双电源负荷
//	
//	CString intnode[MAX_ZNNO],extnode[MAX_ZNNO];//存区段边界节点
//	int /*peri_nd[MAX_ZNNO]*//*区段周边外侧节点*/extbs[MAX_ZNNO],upextbs[MAX_ZNNO];//存区段下游扩展母线及其上游母线
//	ChildPtr1	*sec_p, *sec_p0, *cb_p,*cb_p0, *ld_p,*ld_p0 ;
//	int ld_nd, r_zn,r_ld,r_cb;//负荷的计算母线号,节点优化后的
//	SEC* sec1;
//	CB*  cb1;
//	int cnt_u;
//	double Faultload;	//故障段失电负荷(MW)
//	double Trload;		//非故障段需转移负荷(MW)
//	double NowTrload;	//非故障段实际转移负荷(MW)
//	
//	//lld comment 2011-11-25
//	int noextbs;
//	//int *pextbs, *pupextbs, noextbs;//分别指向上、下游母线数组的指针，及用于扩展的下游节点的个数
//	
//	//lld comment 2011-11-25
//	//pextbs,pupextbs指针初始化
//	//pextbs = NULL;
//    //pupextbs = NULL;
//	
//	CStringList znList;
//	//get bs table
//	BS* bs = dsbob2->bs_tb->GetTable();
//	
//	//get sec table
//	lv_sec = dsbob2->sec_tb->GetCount();
//	SEC* sec = dsbob2->sec_tb->GetTable();
//	
//	//区段表重置
//	int lv_zn = dsbob2->zn_tb->GetCount();
//	ZN* zn = dsbob2->zn_tb->GetTable();
//	for (i = 0; i < lv_zn; i++)
//	{
//		(zn+i)->lossC = -1; //区段自己和下游加起来一起损失的用户
//		(zn+i)->transC = -1;//下游可以转供的用户
//	}
//	//对每一条线段进行计算
//	//**********每次只做一个岛，每次只写入部分线段的数据，最后合并起来就是全网
//1.先判断线段所属区段，查看区段的ialg4q（区段内用户数），w（区段内有功）
//	//区段下游用户数ialg4p（区段下游用户数），区段下游有功（wz）
//2.如果是新区段，计算该区段的上述4个值，填到区段里面
//3.这里有一个重要是前提，每个区段的第一个开关为进入区段的开关。在做区段的时候已经做了这个
//    for(i = 0; i < lv_sec; i++)
//    {
//        ibs = (sec+i)->ibs;
//        zbs = (sec+i)->zbs;
//        iout = (sec+i)->topflags & SEC_IOPEN || (bs+ibs)->iisland < 0;
//        zout = (sec+i)->topflags & SEC_ZOPEN || (bs+zbs)->iisland < 0;
//		
//        if(iout || zout)//线段不在岛内
//        {
//			//搜索这个线段的下游的用户和负荷
//            (sec+i)->lossP   = 0;//(sec+i)->lP_znnext;
//            (sec+i)->transP  = 0;
//            (sec+i)->lossC   = 0;//(sec+i)->lC_znnext;
//            (sec+i)->transC  = 0;
//			continue;			
//        }
//		
//		lossP=transP=0;
//		lossC=transC=0;	
//区段和线段的关系
//		//通过区段ID得到和区段关联的负荷的指针
//		if ((r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1((sec+i)->r_zn)) == -1)
//			continue;
//		zn = dsbob2->zn_tb->GetDataByPh(r_zn); 
//		lossC =  zn->lossC;    //区段自己和下游加起来一起损失的用户,上游不损失
//		transC = zn->transC;    //下游可以转供的用户
//		iznC = zn->inner_C;
//		if ((lossC == -1) && (transC == -1))//区段没有计算过
//		{
//////////////////////////////////////////////////////////////////////////
//计算本区段内的情况
//1.通过区段线段关系表得到全部的线段，特别是区段内的首端馈线的负荷和所带用户情况
//			sec_p = dsbob2->r_zn_sec_tb->FindChildPtr1(zn->id);
//			sec_p = dsbob2->r_zn_sec_tb->FindChildPtr1(zn->r_sec);
//			
//			
//			//得到线段所属的区段负荷,所有的这些统计都要考虑损耗，否则不准确
//			mxTemp = 0;
//			sec_p0 = sec_p; 
//			while(sec_p)
//			{
//				sec1 = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr);
//				
//				//线段的首末节点功率值，这个值是动态的，和每个拓扑有关
//				mxP = fabs(sec1->w);
//				mxQ = fabs(sec1->wz);
//				mxP = mxP>mxQ ? mxP:mxQ;//得到较大的线段有功		
//				
//				mxTemp = mxTemp>mxP ? mxTemp:mxP;
//				
//				sec_p = sec_p->Next;
//			}
//			::FreeChildPtr(sec_p0);
//			mxP = mxTemp;//整个区段的最大有功，因为有功的流动是上游最大
//			
//			znP = 0;
//			multiP = 0;
//			multiC = 0;
//			if (iznC > 0)//区段内有负荷点
//			{
//				//先在区段表里面找到这个区段，得到r_ld,再用r_ld进行搜索
//				r_ld = zn->r_ld;
//				ld_p =  dsbob2->r_zn_ld_tb->FindChildPtr1(r_ld);
//				
//				//得到线段所属的区段负荷
//				ld_p0 = ld_p; 
//				while(ld_p)
//				{
//					znP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);//注入有功累加
//					
//					ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
//					if ( (dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & (ND_SO) )//多电源
//					{
//						multiP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);
//						multiC++;//多电源的负荷数量累加
//					}//多电源在计算停电损失的时候要考虑到
//					
//					ld_p = ld_p->Next;
//				}
//				::FreeChildPtr(ld_p0);
//			}
//			
//////////////////////////////////////////////////////////////////////////
//计算区段下游的转供情况
//			//区段的类型
//			noextbs = zn->type; 
//			if (noextbs > MAX_ZNNO)//应该是图画错了，一般不会出现这种情况
//			{
//				continue;
//			}
///得到区段-开关的相互关系，获得区段的内外节点
//			cnt_u = 0;
//			//边界开关存取结束		
//			deltap=deltaq=deltaC=0;
//			sumP=sumQ=sumC=0;
//			deltaMultiP=deltaMultiQ=deltaMultiC=0;
//			unfd_idList[0]='\0';
//			double lossP_zn,lossQ_zn;
//			int lossC_zn;
//			lossP_zn=lossQ_zn=lossC_zn=0;
//			
//			r_cb = zn->r_cb;
//			if (r_cb != -1)//区段内有下游开关
//			{
//				cb_p = dsbob2->r_zn_cb_tb->FindChildPtr1(r_cb);
//	//			cb_p = dsbob2->r_zn_cb_tb->FindChildPtr1(zn->id);
//				
//				//得到区段内的全部开关,包括首端开关
//				int ii=0;
//				cb_p0 = cb_p; 
//				while(cb_p)
//				{
//					cb1 = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr);
//					if (cb_p->Ptr == zn->cbptr)//进入区段的开关需要跳过
//					{
//						cb_p = cb_p->Next;
//						continue;				
//					}
//					extbs[ii] = cb1->iznd;//边界开关的外侧节点的母线
//					upextbs[ii] = cb1->ind;//边界开关内侧节点所连的馈线段对侧节点的计算母线
//			
//					ii++;
//					cb_p = cb_p->Next;
//				}
//				::FreeChildPtr(cb_p0);
//				noextbs--;
//				//pextbs = extbs;
//				//pupextbs = upextbs;
//				//区段内负荷，区段内用户，区段下游负荷，区段下游用户，区段下游是否有联络
//				//这里面只需传下游的开关就可以了upextbs存储区段下游的内侧母线
//				//extbs--边界开关所连接的下一个线段的首节点（计算母线）
//				//upextbs--边界开关所连接的区段内线段的末节点（计算母线）
//				//计算下游可以转供的部分负荷和用户
//				
//				//deltaC下游可以转供的用户；deltaMultiC下游双电源用户；sumC下游总的用户数
//				//cnt_u = OnSearchUnion(noextbs,extbs,upextbs,deltap,deltaq,deltaC, \
//				//	sumP,sumQ,sumC,deltaMultiP,deltaMultiQ,deltaMultiC,znList,unfd_idList);//区段下游联络开关序列
//				//OnSearchUnion(1,deltap,deltaq,deltaC,extbs,noextbs,upextbs);//区段下游联络开关序列
//	///lld for test new nm1 program
//				
//				//noextbs,extbs,upextbs,区段外进入搜索的开关，可以是多个开关
//				//deltap,deltaq,deltaC，下游可以转供的负荷
//				//sumP,sumQ,sumC,下游总负荷
//				//lossP_zn,lossQ_zn,lossC_zn,下游损失负荷
//				//deltaMultiP,deltaMultiQ,deltaMultiC,下游多电源
//				//unfd_idList，转供路径
//				cnt_u = OnSearchUnion(noextbs,extbs,upextbs,deltap,deltaq,deltaC, \
//					sumP,sumQ,sumC,lossP_zn,lossQ_zn,lossC_zn,deltaMultiP,deltaMultiQ,deltaMultiC,unfd_idList);
//			}
//						   
//////////////////////////////////////////////////////////////////////////
//下游搜索完毕，准备输出结果
//			if (cnt_u == 0)//没有联络开关
//			{
//				lossP  = -(fabs(mxP)-fabs(multiP)-fabs(deltaMultiP));//区段内的最大负荷-区段内的双电源负荷-区段下游的双电源负荷
//				transP = -fabs(multiP)-fabs(deltaMultiP);
//				lossC  = iznC +sumC-multiC-deltaMultiC;//区段内负荷个数+下游负荷个数-区段内双电源的个数-下游双电源的个数
//				transC = multiC+deltaMultiC;
//			}		
//			else 
//			{
//				//只要有联络开关都可以转供 ，lld ncomment
//				lossP = -(fabs(mxP)-fabs(multiP)-fabs(deltap)-fabs(deltaMultiP));//区段内的最大负荷 - 下游可以转供的负荷
//				transP = -fabs(multiP)-fabs(deltap)-fabs(deltaMultiP);
//				lossC = iznC + sumC-deltaC-multiC-deltaMultiC;
//				transC = deltaC+multiC+deltaMultiC;
//			}
//			
//			//将计算结果写到区段里面
//			if (lossC == 0)
//				lossP = 0;
//			
//			if (transC == 0)
//				transP = 0;
//			
//			zn->lossC = lossC; //区段自己和下游加起来一起损失的用户
//			zn->transC = transC;//下游可以转供的用户
//			zn->lossP = lossP;
//			zn->transP = transP;
//			
//			zn->inner_C = iznC;//区段内用户
//			zn->inner_P = znP-multiP;//区段内有功
//			zn->next_C = sumC;//区段下游用户
//			if (sumC == 0)//区段下游没有用户，但是可能因为损耗存在，下游可能会有看似有功的东西出现
//			{
//				zn->next_P = 0;
//			}
//			else
//			{
//				zn->next_P = fabs(mxP)-fabs(multiP)-(znP/0.962);//区段下游有功,考虑3.8%的线损 
//			}
//			strcpy(zn->unfd_idList,unfd_idList);//联络线路还是要传出去，只是有一些变化
//		}		
//		//将区段内的相关信息赋值给所属的馈线段
//        (sec+i)->lossP   = zn->lossP;		//区段及其下游总负荷
//        (sec+i)->transP  = zn->transP;      //区段下游可以转供的负荷，上游不需要转供
//        (sec+i)->lossC   = zn->lossC;	//区段自己和下游加起来一起损失的用户
//        (sec+i)->transC  = zn->transC;	//下游可以转供的用户
//		(sec+i)->lC_zn = zn->inner_C;//区段内用户
//		(sec+i)->lP_zn = zn->inner_P;//区段内有功
//		(sec+i)->lC_znnext = zn->next_C;//区段下游用户
//		(sec+i)->lP_znnext = zn->next_P;//区段下游有功
//		strcpy((sec+i)->tranfdlist,zn->unfd_idList);//转供路径
//	}
// 	return 1;
//}

//通过区段得到下游残余网络的联络点及其联络点信息
//下游残余网络的负荷值也要得到
//也需要得到残余网络的所有负荷个数及容量；分段开关的数量
//int CNM1::getUNList(int noextbs,int* pextbs,int* pupextbs, \
//		int* sectCB,int nSectCB, int* load,int nLoad, NM1_UNLIST* unlist)
//{
//	int sum_unioncb/*总的联络开关数*/;
//	sum_unioncb=0;
//	double p_abund,q_abund,v_abund;//联络有功、无功、电压裕度
//	 nd_abund[80];//联络开关的对侧节点
//	 fdid_abund[80];//联络开关对侧的馈线编号
//	BOOL bFind;
//	//没有可以转供的开关
//	if (noextbs == 0)
//	{
//		return 0;
//	}
//	int knd = pupextbs[0];//通过节点号找电源点
//////////////////////////////////////////////////////////////////////////
//通过节点得到这个区段所属的馈线
//	 fdid[80];//区段所属的馈线ID，不含有前缀
//	ChildPtr1	*sec_p;//开关可能属于2条馈线，不能用开关的fdid来区别，只能用馈线段
//	ND* nd = dsbob2->nd_tb->GetTable();
//	int lv_nd = dsbob2->nd_tb->GetCount();
//	sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//	if (!sec_p)
//		return 0;
//	strcpy(fdid,dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->fdid);
//	if (strlen(fdid)==0)
//		return 0;
//////////////////////////////////////////////////////////////////////////
//	int			i,ii;
//	int			ind, kknd,ld_nd,cb_id,r_zn;
//	ChildPtr1	*cb_p, *cb_p0;
//	ChildPtr1   *sec_p0;
//	ChildPtr1	*ld_p, *ld_p0;
//	 scb_id[80];
//	
//	CB* cb = dsbob2->cb_tb->GetTable();
//	int lv_cb = dsbob2->cb_tb->GetCount();
//	//mark all nds as unprocessed
//	//将所有的节点设为未处理，准备拓扑分析用
//	//此处似乎可以优化，如果没有那么多的空间，可以只做相关的节点
//	for(i = 0; i < lv_nd; i++)
//	{
//		(nd+i)->q |= ND_UNPROC;
//	}
//	//将开关设为已处理
//	for(i = 0; i < lv_cb; i++)
//	{
//		(cb+i)->q |= ND_UNPROC;
//	}
//    //依次处理区段的边界开关
//	//每一个开关都是独立的供电，但其分支之间可能有联络和转供
//	//（1）先一个开关一个开关的处理
//	//      1.在处理开关路径时，没有联络，设置例外1
//	//		2.有一个联络开关，但裕度不够，设置例外2
//	//		3.出现有多个联络开关，需要调用多联络处理，如果转供不了，设置例外3
//	//（2）每个开关均处理完毕，发现存在例外，全部调用多联络模块进行重构
//	
//	for(ii = 0; ii < noextbs; ii++)
//	{
//		knd = pupextbs[ii];//取开关的上游节点进行判断
//		cb_id = -1;
//////////////////////////////////////////////////////////////////////////
//将传入的开关自己找到，进行处理，如果是联络和分位，直接跳掉
//		//如果边界开关本来就处于分位，直接跳过
//		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//		cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
//		while(cb_p)
//		{
//			//pcb_temp = cb_p->Ptr;
//			if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//			{
//				kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//			}
//			else
//			{
//				kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//			}
//			if (kknd == pextbs[ii])//边界开关,把它自己找到
//			{
//				cb_id = cb_p->Ptr;
//				break;
//			}
//			cb_p = cb_p->Next;
//		}
//		::FreeChildPtr(cb_p0);
//		if (cb_id != -1)//一般不会出现找不到自己的情况
//		{
//			dsbob2->cb_tb->GetDataByPh(cb_id)->q &= (~ND_UNPROC);
//			//这个开关要加到断开的开关里面，否则会出现联通问题
//			
//			//区段一出来就是联络开关，这个在开闭所里面比较普遍
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_NMLOPEN)  
//			{
//				continue;
//			}
//			
//			//出来的开关自己是分位开关，停下来
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_OPEN)
//			{
//				continue;
//			}
//		}
//传入开关自己处理完毕，准备从这个开关开始进行自己路径的追踪
//////////////////////////////////////////////////////////////////////////
//		ind = pextbs[ii];//母线的起始节点编号
//将上一个节点设为已处理
//		(nd+pupextbs[ii])->q &= (~ND_UNPROC);
//		//之前已经将所有的节点都置为未处理
//		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
//		{
//			(nd+ind)->q &= (~ND_UNPROC);
//			//通过节点的q可以判断出是不是负荷节点，q |= 8
//			//拿当前节点向下扩展，找到所有和它相连的节点
//			knd = ind;//当前节点赋给一个临时的变量
//			(nd+knd)->fch = knd; //当前节点号赋给前向链 
//     		while (1)//还有扩展开关没有处理
//			{
//				//通过节点的q可以判断出是不是负荷节点，q |= 8
//				if ((nd+knd)->q & ND_LD) 
//				{
//					//得到负荷id
//					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
//					ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
//					while(ld_p)
//					{
//						//处理双电源问题,多电源默认为不停电，做n-1必须满足多电源点
//						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
//						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
//							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//多电源
//						{
//							continue;	
//							//deltaMultiP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);
//							//deltaMultiC++;
//						}//多电源在计算停电损失的时候要考虑到
//						//直接修改负荷的ID号
//						//sumP += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
//						//sumQ += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
//						ld_p = ld_p->Next;
//					}
//					::FreeChildPtr(ld_p0);
//				}
//				//read cbs connected to this nd
//				//从节点-开关关系表找到所有和这个节点相连的开关
//				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
//				while(cb_p)
//				{
//					//如果是联络开关，停下来，找到和它相连的电源点
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_NMLOPEN)  
//					{
//						//将这个开关设为已处理
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);
//						//仅仅只是一个开位的开关是不能判断的，必须没有联络的线路才是
//						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
//						{
//							//kkndcb,此为联络节点，找到一个联络开关
//							//sum_unioncb++;
//							if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//							{
//								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->znd);
//							}
//							else
//							{
//								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->nd);
//							}
//							strcpy(scb_id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id);
//							//判断联络开关的裕度？
// 							getUNCB_abun(fdid,scb_id,nd_abund, \
//										fdid_abund,p_abund,q_abund,v_abund);
//							
//							//虽然有联络开关，但没有联络线路，联络开关是悬空的
//							if (fdid_abund != "")
//							{
//								bFind = FALSE;
//								//如果联络开关的母线和以前的联络开关母线一样
//								for (i=0;i<sum_unioncb;i++)
//								{
//									//比较联络开关裕度，选最大的裕度
//									if ( (nd+unlist[i].nd)->ibs == (nd+knd)->ibs )
//									{
//										//替换
//										if (p_abund > unlist[i].p_abund)
//										{
//											unlist[i].nd =knd;
//											strcpy(unlist[i].secid , fdid_abund);
//											unlist[i].p_abund=p_abund;
//											unlist[i].q_abund=q_abund;
//											unlist[i].v_abund=v_abund;
//										}
//										bFind = TRUE;
//									}
//								}
//								if (!bFind)
//								{
//									unlist[sum_unioncb].nd =knd;
//									strcpy(unlist[sum_unioncb].secid , fdid_abund);
//									unlist[sum_unioncb].p_abund=p_abund;
//									unlist[sum_unioncb].q_abund=q_abund;
//									unlist[sum_unioncb].v_abund=v_abund;
//									sum_unioncb++;						
//								}
//							}
//						}						
//						cb_p = cb_p->Next;
//						continue;
//					}
//					//只要是搜索到的开关都保留下来,已经处理过的不保留
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & ND_UNPROC) //this so nd has not processed
//					{
//						//只处理主干线上的开关，CB_OPANABL
//						//将这个开关设为已处理
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);
//						//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);
//					}
//					//如果是分位开关，停下来
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN)
//					{
//						cb_p = cb_p->Next;
//						continue;
//					}
//					//find the opposite nd through this cb
//					//kknd表示开关的对侧节点，这里的节点都是整型的编号
//					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//					}
//					//process this opposite nd
//					//如果对侧节点 未处理==开关不参与拓扑推进
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//将扩展的前向链 设成 找到的新节点的前向链
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//新节点 设成 扩展节点的前向链
//						(nd+knd)->fch = kknd;
//						//将找到的新节点设成已处理
//						(nd+kknd)->q &= (~ND_UNPROC);
//					}
//					cb_p = cb_p->Next;
//				}
//				::FreeChildPtr(cb_p0);
//				//read secs connected to this nd通过线段向下扩展
//				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//				sec_p0 = sec_p;
//				while(sec_p)
//				{
//				    //检查该线段是不是新的区段，如果是，将其加入到影响区段里面
//				    //根据线段区段之间的关联关系找到相对对应的区段
//            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
//            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
//		            
//					if (r_zn == -1)//该线段不属于任何区段
//					{
//						sec_p = sec_p->Next;
//						continue;
//					}
// 
//					//find the opposite nd through this sec
//					if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
//					{
//						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
//					}
//					//process this opposite nd
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//循环链表的使用，每扩展一个新的节点，做2件事：
//						//1.将上一个节点的前向链设为自己的前向链
//						//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
//						
//						//将扩展的前向链 设成 找到的新节点的前向链
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//新节点 设成 扩展节点的前向链
//						(nd+knd)->fch = kknd;
//						//将找到的新节点设成已处理
//						(nd+kknd)->q &= (~ND_UNPROC);
//					}
//					sec_p = sec_p->Next;
//				}
//				::FreeChildPtr(sec_p0);
//				//move to next nd
//				knd = (nd+knd)->fch;
//				//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
//				//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
//				//拿另外的节点再开始搜索，一直到所有的节点都处理
//				if(knd == ind)
//				{
//					break;
//				}
//			}
//		}
//	}
//	
//	return sum_unioncb;
//}

//分析哪些能转供，哪些不能转供，能转供的负荷数
//非故障段需转移负荷(MW)	非故障段实际转移负荷(MW)  转供电源
int CNM1::residueAnalyze(int noextbs,int* pextbs,int* pupextbs)
{
	//仅仅只是进行拓扑岛的划分，后面需要进一步具体分析
	if (!getResidueStruct(noextbs,pextbs,pupextbs))//下游没有联络开关
		return 0;

	//光给出结论还不行，要把能转供和不能转供的岛输出出来
	if (AnanyseIsland() == 0)//初步分析都不能通过，说明是孤岛
		return 0;

	if (AnanyseResult(1) == 0)//输出残余网络的分析结果
		return 0;

	return 1;
}

//搜索残余网络，得到每个岛的联络开关信息、分段开关信息、负荷信息、电气岛
//通过分析区段的边界开关，得到边界开关连接的每一个岛，这个岛有可能是死岛
//也有可能是活岛，即使是活岛，也有可能是多个联络或联络不能正常转供
//int CNM1::getResidueStruct(int noextbs,int* pextbs,int* pupextbs)
//{//int degbug=1;
//	int ndCount,ldCount,cbCount,secCount,cpCount,unCount,soCount;//一个岛内的数量
//	double p_abund,q_abund,v_abund,pq_abund;//联络有功、无功、电压裕度
//	 nd_abund[MAX_SEC_ID_LEN];//联络开关的对侧节点
//	 fdid_abund[MAX_SEC_ID_LEN];//联络开关对侧的馈线编号
//	BOOL bFind;
//
//	int knd,lv_nd,lv_cb,lv_cp,lv_so;
//	 fdid[MAX_SEC_ID_LEN];//区段所属的馈线ID，不含有前缀
//	int			i,ii,j;
//	int			ind, kknd,ld_nd,cb_id,r_zn,pcb_temp;
//	ChildPtr1	*cb_p, *cb_p0;
//	ChildPtr1   *sec_p0;
//	ChildPtr1	*ld_p, *ld_p0;
//	 scb_id[MAX_SEC_ID_LEN];//联络开关对侧的馈线id序列
//
//	ChildPtr1	*sec_p;//开关可能属于2条馈线，不能用开关的fdid来区别，只能用馈线段
//	
//	//没有可以转供的开关
//	if (noextbs == 0)
//	{
//		return 0;
//	}
//
//	knd = pupextbs[0];//通过节点号找电源点
//	ND* nd = dsbob2->nd_tb->GetTable();
//	lv_nd = dsbob2->nd_tb->GetCount();
//	
////	sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
////	sec_p0 = sec_p;//进行指针的临时保存，指针是整型的数字
////	if (!sec_p)
////	{
////		::FreeChildPtr(sec_p0);
////		return 0;
////	}
////	strcpy(fdid,dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->fdid);
////	if (strlen(fdid)==0)
////	{
////		::FreeChildPtr(sec_p0);
////		return 0;
////	}
////	::FreeChildPtr(sec_p0);
//	
//	CB* cb = dsbob2->cb_tb->GetTable();
//	lv_cb = dsbob2->cb_tb->GetCount();
//
//	if (lld->mx_cp>0)
//	{
//		CP* cp = dsbob2->cp_tb->GetTable();
//		lv_cp = dsbob2->cp_tb->GetCount();
//	}
//
//	SO* so = dsbob2->so_tb->GetTable();
//	lv_so = dsbob2->so_tb->GetCount();
//
//	//mark all nds as unprocessed
//	//将所有的节点设为未处理，准备拓扑分析用
//	//此处似乎可以优化，如果没有那么多的空间，可以只做相关的节点
//	for(i = 0; i < lv_nd; i++)
//	{
//		(nd+i)->q |= ND_UNPROC;
//	}
//
//	//将开关设为已处理
//	for(i = 0; i < lv_cb; i++)
//	{
//		(cb+i)->q |= CB_N_RESTORE;
//	}
//
//    //依次处理区段的边界开关
//	//每一个开关都是独立的供电，但其分支之间可能有联络和转供
//	g_island = 0;
//	for(ii = 0; ii < noextbs; ii++)
//	{
//		knd = pupextbs[ii];//取开关的上游节点进行判断
//
//		cb_id = -1;
////////////////////////////////////////////////////////////////////////////
////将传入的开关自己找到，进行处理，如果是联络和分位，直接跳掉
//		//如果边界开关本来就处于分位，直接跳过
//		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//		cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
//		while(cb_p)
//		{
//			pcb_temp = cb_p->Ptr;
//			if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//			{
//				kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//			}
//			else
//			{
//				kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//			}
//
//			if (kknd == pextbs[ii])//边界开关,把它自己找到
//			{
//				cb_id = cb_p->Ptr;
//				break;
//			}
//			cb_p = cb_p->Next;
//		}
//		::FreeChildPtr(cb_p0);
//
//		if (cb_id != -1)//一般不会出现找不到自己的情况
//		{
//			dsbob2->cb_tb->GetDataByPh(cb_id)->q &= (~ND_UNPROC);
//			//这个开关要加到断开的开关里面，否则会出现联通问题
//			//firstCB[ii] = cb_id;//将出来的开关设为必须断开的开关
//			//这个开关属于必须要断开的开关，在搜索的时候可以不需要加到分段里面
//			//只有加入到结构里面的分段开关才起作用
//
//			//区段一出来就是联络开关，这个在开闭所里面比较普遍
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & Q_UNIONCB)  
//			{
//				continue;
//			}
//			
//			//出来的开关自己是分位开关，停下来
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_OPEN)
//			{
//				continue;
//			}
//		}
//
////传入开关自己处理完毕，准备从这个开关开始进行自己路径的追踪
////////////////////////////////////////////////////////////////////////////
//		ndCount = cbCount = cpCount = secCount = unCount = ldCount = soCount = 0;
//		ind = pextbs[ii];//母线的起始节点编号
//
////将上一个节点设为已处理
//		(nd+pupextbs[ii])->q &= (~ND_UNPROC);
////		islandlist[g_island].island_ndlist[ndCount++].nd_ptr = pupextbs[ii];
//
//		//之前已经将所有的节点都置为未处理
//		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
//		{
//			(nd+ind)->q &= (~ND_UNPROC);
//			islandlist[g_island].island_ndlist[ndCount++].nd_ptr = ind;
//
//			//通过节点的q可以判断出是不是负荷节点，q |= 8
//
//			//拿当前节点向下扩展，找到所有和它相连的节点
//			knd = ind;//当前节点赋给一个临时的变量
//			(nd+knd)->fch = knd; //当前节点号赋给前向链 
//     		while (1)//还有扩展开关没有处理
//			{
//				//通过节点的q可以判断出是不是负荷节点，q |= 8
//				if ((nd+knd)->q & ND_LD) 
//				{
//					//得到负荷id，负荷很重要，后面所有的都要以这个刚性的需求来进行处理
//					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
//					ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
//					while(ld_p)
//					{
//						islandlist[g_island].island_ldlist[ldCount].ld_ptr = ld_p->Ptr;
//						islandlist[g_island].island_ldlist[ldCount].p_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
//						islandlist[g_island].island_ldlist[ldCount].q_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
//						pq_abund = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
//						pq_abund += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
//						pq_abund = sqrt(pq_abund);
//						islandlist[g_island].island_ldlist[ldCount].pq_ld = pq_abund;	
//						
//						//处理双电源问题
//						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//负荷对应的节点号
//						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
//							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//多电源
//						{
//							islandlist[g_island].island_ldlist[ldCount].soSum = 2;
//						}//多电源在计算停电损失的时候要考虑到
//						else
//							islandlist[g_island].island_ldlist[ldCount].soSum = 1;
//	
//						islandlist[g_island].island_ldlist[ldCount].nd = knd;
////						strcpy(islandlist[g_island].island_ldlist[ldCount].ld_id , dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->id);
//						
//						ldCount++;
//
//						ld_p = ld_p->Next;
//					}
//					::FreeChildPtr(ld_p0);
//				}
//
//				if ((nd+knd)->q & ND_SO) 
//				{
//					//对so进行全循环，因为so的数量很少,可能是分布式电源
//					for (j=0; j<lv_so; j++)
//					{
//						if ( (so+j)->ind == knd)
//						{
//							islandlist[g_island].island_solist[soCount].soptr = j;
//							islandlist[g_island].island_solist[soCount].nd = knd;
//							islandlist[g_island].island_solist[soCount].p = (so+j)->w;
//							islandlist[g_island].island_solist[soCount].q = (so+j)->r;
//							strcpy(islandlist[g_island].island_solist[soCount].secid , (so+j)->fdid);
//							islandlist[g_island].island_solist[soCount].pq = sqrt((so+j)->w*(so+j)->w+(so+j)->r*(so+j)->r);
//							soCount++;
//							break;
//						}
//					}
//				}
//
//				if ( ((nd+knd)->q & ND_CP) && (lld->mx_cp>0) )
//				{
//					//对cp进行全循环，因为cp的数量很少
//					for (j=0; j<lv_cp; j++)
//					{
//						if ( (dsbob2->cp_tb->GetTable()+j)->ind == knd)
//						{
//							islandlist[g_island].island_cplist[cpCount].cp_ptr = j;
//							islandlist[g_island].island_cplist[cpCount].nd = knd;
//							islandlist[g_island].island_cplist[cpCount].r = dsbob2->cp_tb->GetTable()->r;
////							islandlist[g_island].island_cplist[cpCount].island = ii;
//							cpCount++;
//							break;
//						}
//					}
//				}
//
//				//read cbs connected to this nd
//				//从节点-开关关系表找到所有和这个节点相连的开关
//				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
//				while(cb_p)
//				{
//					//如果是联络开关，停下来，找到和它相连的电源点
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
//					{
//						//将这个开关设为已处理
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);
//
//						//仅仅只是一个开位的开关是不能判断的，必须没有联络的线路才是
//						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
//						{
//							//kkndcb,此为联络节点，找到一个联络开关
//							//sum_unioncb++;
//							if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//							{
//								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->znd);
//							}
//							else
//							{
//								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->nd);
//							}
//
//							strcpy(scb_id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id);
//							//判断联络开关的裕度
//							p_abund=q_abund=v_abund=0;
// 							getUNCB_abun(fdid,scb_id,nd_abund, \
//										fdid_abund,p_abund,q_abund,v_abund);
//							if ( (p_abund==0) && (q_abund==0) && (v_abund==0) )
//							{
//								pq_abund = 5;//默认最大的联络裕度
//								v_abund = 1.05;
//							}
//							else
//								pq_abund = sqrt(p_abund*p_abund + q_abund*q_abund);
//							
//							//虽然有联络开关，但没有联络线路，联络开关是悬空的
//							if (fdid_abund != "")
//							{
//								bFind = FALSE;
//								//如果联络开关的母线和以前的联络开关母线一样
//								for (i=0;i<unCount;i++)
//								{
//									//比较联络开关裕度，选最大的裕度
//									pcb_temp = islandlist[g_island].island_unlist[i].nd;
//									if ( (nd+pcb_temp)->ibs == (nd+knd)->ibs )
//									{
//										//替换
//										if (pq_abund > islandlist[g_island].island_unlist[i].pq_abund)
//										{
//											islandlist[g_island].island_unlist[i].nd = knd;
//											strcpy(islandlist[g_island].island_unlist[i].secid , fdid_abund);
//
//											islandlist[g_island].island_unlist[i].p_abund=p_abund;
//											islandlist[g_island].island_unlist[i].q_abund=q_abund;
//											islandlist[g_island].island_unlist[i].pq_abund=pq_abund;
//											islandlist[g_island].island_unlist[i].v_abund=v_abund;
//											islandlist[g_island].island_unlist[i].cbptr = cb_p->Ptr;
//										}
//										bFind = TRUE;
//									}
//								}
//
//								if (!bFind)
//								{
//									islandlist[g_island].island_unlist[unCount].nd = knd;
//									strcpy(islandlist[g_island].island_unlist[unCount].secid , fdid_abund);
//
//									islandlist[g_island].island_unlist[unCount].p_abund=p_abund;
//									islandlist[g_island].island_unlist[unCount].q_abund=q_abund;
//									islandlist[g_island].island_unlist[unCount].pq_abund=pq_abund;
//									islandlist[g_island].island_unlist[unCount].v_abund=v_abund;
//									islandlist[g_island].island_unlist[unCount].cbptr = cb_p->Ptr;
//
//									unCount++;						
//								}
//							}
//						}						
//						cb_p = cb_p->Next;
//						continue;
//					}
//
//					//只要是搜索到的开关都保留下来,已经处理过的不保留
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_RESTORE) //this so nd has not processed
//					{
//						//printf("%s,%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb);
//						//如果是分位开关（常开的分为开关），也要当做联络开关处理，但是其裕度为无限大
//
//						//将这个开关设为已处理
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);
//
//						//仅仅只是一个开位的开关是不能判断的，必须没有联络的线路才是
//						//!!!!!!!内部联络开关
//						if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)
//						{
//							//kkndcb,此为联络节点，找到一个联络开关
//							//sum_unioncb++;
//							if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//							{
//								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->znd);
//							}
//							else
//							{
//								strcpy(nd_abund , dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->nd);
//							}
//
//							strcpy(scb_id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id);
//							//判断联络开关的裕度
//							p_abund = q_abund = pq_abund = 20;
//							
//							//虽然有联络开关，但没有联络线路，联络开关是悬空的
//							islandlist[g_island].island_unlist[unCount].nd = knd;
//							strcpy(islandlist[g_island].island_unlist[unCount].secid , fdid);
//
//							islandlist[g_island].island_unlist[unCount].p_abund=p_abund;
//							islandlist[g_island].island_unlist[unCount].q_abund=q_abund;
//							islandlist[g_island].island_unlist[unCount].pq_abund=pq_abund;
//							islandlist[g_island].island_unlist[unCount].v_abund=1.05;
//							islandlist[g_island].island_unlist[unCount].cbptr = cb_p->Ptr;
//
//							unCount++;						
//
//							cb_p = cb_p->Next;
//							continue;
//						}
//						else
//						{
//							//如果是以前在分析的时候设定为解环开关，这里要将其恢复
//							if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_ISOLATE)
//								dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_ISOLATE);							
//
//							islandlist[g_island].island_cblist[cbCount].cb_ptr = cb_p->Ptr;
//							cbCount++;
//						}
//
//						//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);
//
//					}
//
//					//find the opposite nd through this cb
//					//kknd表示开关的对侧节点，这里的节点都是整型的编号
//					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//					}
//					//process this opposite nd
//					//如果对侧节点 未处理==开关不参与拓扑推进
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//将扩展的前向链 设成 找到的新节点的前向链
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//新节点 设成 扩展节点的前向链
//						(nd+knd)->fch = kknd;
//						//将找到的新节点设成已处理
//						(nd+kknd)->q &= (~ND_UNPROC);
//						islandlist[g_island].island_ndlist[ndCount++].nd_ptr = kknd;
//					}
//
//					cb_p = cb_p->Next;
//				}
//				::FreeChildPtr(cb_p0);
//
//				//read secs connected to this nd通过线段向下扩展
//				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//				sec_p0 = sec_p;
//				while(sec_p)
//				{
//				    //检查该线段是不是新的区段，如果是，将其加入到影响区段里面
//				    //根据线段区段之间的关联关系找到相对对应的区段
//            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
//            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
//		            
//					if (r_zn == -1)//该线段不属于任何区段
//					{
//						sec_p = sec_p->Next;
//						continue;
//					}
// 
//					//find the opposite nd through this sec
//					if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
//					{
//						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
//					}
//
//					//process this opposite nd
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//循环链表的使用，每扩展一个新的节点，做2件事：
//						//1.将上一个节点的前向链设为自己的前向链
//						//2.修改上一个节点的前向链，把自己设为上一个节点的前向链
//						
//						//将扩展的前向链 设成 找到的新节点的前向链
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//新节点 设成 扩展节点的前向链
//						(nd+knd)->fch = kknd;
//						//将找到的新节点设成已处理
//						(nd+kknd)->q &= (~ND_UNPROC);
//						islandlist[g_island].island_ndlist[ndCount++].nd_ptr = kknd;
//					
//						islandlist[g_island].island_seclist[secCount].sec_ptr = sec_p->Ptr;
//						islandlist[g_island].island_seclist[secCount].nd = knd;
//						islandlist[g_island].island_seclist[secCount].znd = kknd;
////						strcpy(islandlist[g_island].island_seclist[secCount].sec_id , dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->id);
//						islandlist[g_island].island_seclist[secCount].r = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r0;
//						islandlist[g_island].island_seclist[secCount].x = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->x0;
////						islandlist[g_island].island_seclist[secCount].island = ii;
//						secCount++;
//					}
//					sec_p = sec_p->Next;
//				}
//				::FreeChildPtr(sec_p0);
//
//				//move to next nd
//				knd = (nd+knd)->fch;
//				//循环到头，连起来了，表示全部走了一遍，表示处理掉一条转供路径
//				//这个转供路径也有可能是有多个联络开关的，每个联络开关带一部分负荷
//				//拿另外的节点再开始搜索，一直到所有的节点都处理
//				if(knd == ind)
//				{
//					islandlist[g_island].soCount = soCount;
//					islandlist[g_island].cbCount = cbCount;
//					islandlist[g_island].ldCount = ldCount;
//					islandlist[g_island].unCount = unCount;
//					islandlist[g_island].ndCount = ndCount;
//					islandlist[g_island].cpCount = cpCount;
//					islandlist[g_island].secCount = secCount;
//					islandlist[g_island].validate = 0;
//					islandlist[g_island].canTrans = 1;//先假设能转
//
//					g_island++;
//					break;
//				}
//			}
//		}
//	}
//	
//	return 1;
//}







