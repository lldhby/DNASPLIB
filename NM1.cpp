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

	pathFlow[0] = 0;//Ĭ�ϲ���Ҫ�����ೱ�������lld 2020-4-29

	i_island = 0;
	//island_tb_fault = new DTable<ISLAND>(1000);//�����1000����
	eq_tb_isolate = new DTable<EQ>(50);//�����10���ܱ߸��뿪��
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

//ͨ���ڵ�Ż���豸
int CNM1::OnGetDevByNDs(DSBOB2* dsbob2,int start,int end)
{
	//���˶��нڵ���豸
	int i_id,k,l;
	int i[20],j[20],t[20],i_cnt,j_cnt;
	ChildPtr1	*equip_p, *equip_p0;

	//û�нڵ�
	if ( (start==-1) && (end==-1) )
		return -1;

	ND* nd_tt = dsbob2->nd_tb->GetTable();

//////////////////////////////////////////////////////////////////////////
	//���⴦���ɵȵ���Ԫ������ĩ�ڵ����Ϊ-1
	i_id = -1;
	if ((start!=-1) && (end==-1)) 
	{
		i_id = start;
	}
		
	if ((start==-1) && (end!=-1))
	{
		i_id = end;
	}

	//�϶��ǵ���Ԫ��
	if (i_id != -1)
	{
		equip_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_tt+i_id)->r_eq);
		equip_p0 = equip_p;//����ָ�����ʱ���棬ָ�������͵�����
		while(equip_p)
		{
			//�п�������豸
//			if ( (equip_tb->GetDataByPh(equip_p->Ptr)->ind != -1) 
//				&& (equip_tb->GetDataByPh(equip_p->Ptr)->iznd != -1) )
//			{
//			}
//			else
//				i_id = equip_tb->GetDataByPh(equip_p->Ptr)->i_id;

			//�ڽ����豸д���ʱ���Ѿ��涨�˵����豸��ĩ��Ϊ-1
			if (dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->iznd != -1) 
				i_id = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->i_id;

			equip_p = equip_p->Next;
		}
		::FreeChildPtr(equip_p0);
	
		return i_id;
	}
//////////////////////////////////////////////////////////////////////////

	//׼������˫��Ԫ��
	for(k=0; k<20; k++)
		i[k]=j[k]=t[k]=-1;
	
	k=0;
	equip_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_tt+start)->r_eq);
	equip_p0 = equip_p;//����ָ�����ʱ���棬ָ�������͵�����
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
	equip_p0 = equip_p;//����ָ�����ʱ���棬ָ�������͵�����
	while(equip_p)
	{
		i_id = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->i_id;
		j[k++] = i_id;

		equip_p = equip_p->Next;
	}
	::FreeChildPtr(equip_p0);
	j_cnt = k;

//���ӵ��豸������ͬһ���豸���������豸����̫��
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

//�ӵ�Դ�㵽�������豸֮���ͨ�����豸�Ƿ������ж�����
//�������Ŀ��غ��۶���
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
//DEL 	dsbob2->nd_tb->CopyTable(nd_t);//�õ�һ���µĽڵ�������ԭ���ĳ�ͻ
//DEL 
//DEL 	//�����е���ʱ�ӵ���Ϊ�Ѵ���
//DEL 	for(i = 0; i < lv_nd; i++)
//DEL 	{
//DEL 		(nd_t+i)->q |= ND_UNPROC;
//DEL 	}
//DEL 
//DEL 	ind = soid;//ĸ�ߵ���ʼ�ڵ���
//DEL 	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
//DEL 	if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
//DEL 	{
//DEL 		(nd_t+ind)->q &= (~ND_UNPROC);
//DEL 
//DEL 		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
//DEL 		(nd_t+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
//DEL 		(nd_t+ind)->bch = knd;//�ӵ�Դ�㿪ʼ��ʼ����,����Դ����Ϊ��һ����¼����
//DEL      	while (1)//������չ����û�д���
//DEL 		{
//DEL 			eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
//DEL 			eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
//DEL 			while(eq_p)
//DEL 			{
//DEL 				eq_id = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
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
//DEL 						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
//DEL 						(nd_t+kknd)->fch = (nd_t+knd)->fch;
//DEL 						//�½ڵ� ��� ��չ�ڵ��ǰ����
//DEL 						(nd_t+knd)->fch = kknd;
//DEL 						//������ڵ����һ����¼������׼������
//DEL 						(nd_t+kknd)->bch = knd;
//DEL 						
//DEL 						//���ҵ����½ڵ�����Ѵ���
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
//DEL 	//�ҵ��˽ڵ�
//DEL 	if (iFind != -1)
//DEL 	{
//DEL 		knd = iFind;
//DEL 		do
//DEL 		{
//DEL 			ind = knd;//���ο��صĽڵ��
//DEL 			knd = (nd_t+knd)->bch;//�õ����ص��ϼ��ڵ��
//DEL 
//DEL 			eq_id = OnGetDevByNDs(dsbob2,ind,knd);
//DEL 			if ( 
//DEL 				(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 3)//�۶���
//DEL 				||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 5) //�������Ŀ���
//DEL 				)
//DEL 			{
//DEL 				ret= 2;//���ٶϿ��أ����Ź�
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
//DEL 				ret= 1;//����ͨ����
//DEL 				break;
//DEL 			}
//DEL 
//DEL 		}	while(knd != ind);//һֱ����Դ��Ϊֹ
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
	//�����еĵ�����Ϊ-1
	ChildPtr1	*eq_p, *eq_p0;
	eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((dsbob2->fd_tb->GetTable()+fdph)->r_eq);
	eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
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
//	double p_abund,q_abund,v_abund;//�Բ�ĵ�Դ���й����޹�
	p_abund=q_abund=v_abund=0;

	//���Ը����������翪�ع�ϵ��ֱ�ӵõ�
	r_un = dsbob2->fd_tb->GetDataByPh(fdph)->r_un;
	if (r_un != -1)//�����������ο���
	{
		un_p = dsbob2->r_fd_un_tb->FindChildPtr1(r_un);
		un_p0 = un_p; 
		while(un_p)
		{
			//�����ڵ����б߽翪�ض���Ҫ��
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
	//׼������
	OnSetAbund();//������翪�ص�ԣ��

	lld->g_cnt_nm1_fd = 0;
	//�Դ�����Ϊ��λ
	int lv_fd = dsbob2->fd_tb->GetCount();
	FD* fd = dsbob2->fd_tb->GetTable();
	for(i = 0; i < lv_fd; i++)
	{
		//ȫ�ߵ�N-1
		OnWholeFeederNM1(i);

		//���ߵ�ÿһ���豸N-1
		OnDevNM1(i);

		//д�豸��N-1������
		OnWriteNM1(i);
	}

	delete eq_tb_isolate;

	return 1;
}

//ȫ���ߵ�N-1
int CNM1::OnWholeFeederNM1(int ifd)
{
	int		soid;
	double dReal,dImp;//���ߵ�������Դ
	double soReal, soImp;//�����ϵ�������Դ
	double ldReal, ldImp;//�����ϵ�������Դ
	double curW;//�������ߵ����ڹ���
	double tranW;//���������ԣ��
	char   unionSO[MAX_UION_ID];//����ĵ�Դ��id���ϣ��м��ã��ֿ�	
	char   fdid[MAX_ID_DLL];//����ĵ�Դ��id���ϣ��м��ã��ֿ�	

	double Faultload;	//���϶�ʧ�縺��(MW)
	double Trload;		//�ǹ��϶���ת�Ƹ���(MW)
	double NowTrload;	//�ǹ��϶�ʵ��ת�Ƹ���(MW)
	double PowerLostRatio;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
//	char Result;			//�ֶ�N-1������ 
	char LineResult;		//ȫ��N-1������
//	int iCountSect;		//�߶εķֶ�����

	char unionList[4000];

	ChildPtr1	*so_p,*so_p0;
	SO* so = dsbob2->so_tb->GetTable();
	ChildPtr1	*ld_p, *ld_p0;
	LD* ld = dsbob2->ld_tb->GetTable();

	strcpy(fdid,dsbob2->fd_tb->GetDataByPh(ifd)->id);
	int r_so = dsbob2->fd_tb->GetDataByPh(ifd)->r_so;
	if (r_so != -1)//�����������ο���
	{
		dReal = dImp = 0;
		soReal = soImp = 0;
		so_p = dsbob2->r_fd_so_tb->FindChildPtr1(r_so);
		so_p0 = so_p; 
		while(so_p)
		{
			soid = so_p->Ptr;//������Դ
			if  (dsbob2->so_tb->GetDataByPh(soid)->slackavr == 4)
			{
				dReal += dsbob2->so_tb->GetDataByPh(soid)->w;
				dImp  += dsbob2->so_tb->GetDataByPh(soid)->r;
			}
			else
			if (dsbob2->so_tb->GetDataByPh(soid)->slackavr == 0)
			{
				soReal += 1000;//ƽ��ڵ�����ṩ���޵ĵ���
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

	//Ӧ�����ø������ж�
	ldReal = ldImp = 0;
	int r_ld = dsbob2->fd_tb->GetDataByPh(ifd)->r_ld;
	if (r_ld != -1)//�����������ο���
	{
		ld_p = dsbob2->r_fd_ld_tb->FindChildPtr1(r_ld);
		ld_p0 = ld_p;
		while (ld_p)
		{
			soid = ld_p->Ptr;//������Դ
			ldReal += dsbob2->ld_tb->GetDataByPh(soid)->w;
			ldImp += dsbob2->ld_tb->GetDataByPh(soid)->r;

			ld_p = ld_p->Next;
		}
		::FreeChildPtr(ld_p0);
	}

	curW = sqrt(ldReal*ldReal +ldImp*ldImp);//��ǰ��Դ������ڹ���,���ص�������ֵ

	unionList[0] = 0;
	if (curW == 0)
	{
		PowerLostRatio=0;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
		LineResult = '0';
		tranW = 0;
		strcpy(unionList , "-");
	}
	else
	{
		//���жϱ��õ�Դ�����
		if ((sqrt(soReal*soReal + soImp * soImp)) > curW)//����Բ������ԣ�ȴ�����Ҫת����ԣ�ȣ��������ߵ�n-1����ͨ��
		{
			PowerLostRatio = 0;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
			LineResult = '1';
			tranW = curW;
		}
		else
		{
			//�����뱾�������ӵ���������,�Լ���ص�ԣ��
			tranW = getUNCB_abun(fdid, unionSO);
			strcpy(unionList, unionSO);
			if ((strlen(unionList) == 0) || (strcmp(unionList, ",") == 0))
				strcpy(unionList, "-");
			else
				lld->delstrright(unionList, 1);

			if ((tranW+ (sqrt(soReal*soReal + soImp * soImp))) > curW)//����Բ������ԣ�ȴ�����Ҫת����ԣ�ȣ��������ߵ�n-1����ͨ��
			{
				PowerLostRatio = 0;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
				LineResult = '1';
				tranW = curW;
			}
			else
			{
				PowerLostRatio = 100;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
				LineResult = '0';
				tranW = 0;
			}
		}
	}

	Faultload=0;	//���϶�ʧ�縺��(MW)
	Trload=curW;		//�ǹ��϶���ת�Ƹ���(MW)
	NowTrload=tranW;	//�ǹ��϶�ʵ��ת�Ƹ���(MW)
	
	//ȫ��N-1������
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].id=lld->g_cnt_nm1_fd;
	strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].fdid,fdid);
	strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].secid,"������n-1");
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].Faultload = Faultload;
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].Trload=Trload;
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].NowTrload=NowTrload;
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].PowerLostRatio=PowerLostRatio;
	if (strlen(unionList)>0)
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].unionList , unionList);
	else
		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].unionList, "������Դ��");

	dsbob2->nm1_result[lld->g_cnt_nm1_fd].secResult='-';
	dsbob2->nm1_result[lld->g_cnt_nm1_fd].LineResult=LineResult;
	strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].transSec , "-");
	strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].lossSec , "-");
	lld->g_cnt_nm1_fd++;

	return 1;	
}

int CNM1::OnWriteNM1(int ifd)
{
	double Faultload;	//���϶�ʧ�縺��(MW)
	double Trload;		//�ǹ��϶���ת�Ƹ���(MW)
	double NowTrload;	//�ǹ��϶�ʵ��ת�Ƹ���(MW)
	double PowerLostRatio;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
	char Result;		//�ֶ�N-1������ 
	char LineResult;	//ȫ��N-1������
//	int iCountSect;		//�߶εķֶ�����
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

	//������е��豸
	eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((fd+fdph)->r_eq);
	eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
	while(eq_p)
	{
		//���϶�ʧ�縺��(MW)
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

		//�ǹ��϶���ת�Ƹ���(MW)
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

		//�ǹ��϶�ʵ��ת�Ƹ���(MW)
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
				PowerLostRatio=100 * ((Trload-NowTrload)/Trload);//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
			else
				PowerLostRatio=100;

			LineResult = '0';//ֻҪ��һ��������n-1��ȫ�߶�������
			Result = '0';
		}
		
		//�ֶ�N-1������
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

//���һ����������N-1����
int CNM1::OnDevNM1(int ifd)
{
//	double Faultload;	//���϶�ʧ�縺��(MW)
//	double Trload;		//�ǹ��϶���ת�Ƹ���(MW)
//	double NowTrload;	//�ǹ��϶�ʵ��ת�Ƹ���(MW)
//	double PowerLostRatio;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
//	char Result;		//�ֶ�N-1������ 
//	char LineResult;	//ȫ��N-1������
//	int iCountSect;		//�߶εķֶ�����

	int i,j,eqph=-1,ret,r_zn;
	fdid[0]=0;
	ChildPtr1	*so_p,*so_p0;
	ChildPtr1	*eq_p, *eq_p0;
	ChildPtr1	*sec_p, *sec_p0;
	int ind,iznd,oppsite;
//	 nd[MAX_ID_DLL],znd[MAX_ID_DLL];

	int sond=-1;//���߿��ؿ���ĸ�ߵ��Ǹ��ڵ���Ϊ��Դ��
	int firstND = -1;//��ΪҪ�ӳ��߿��ؿ�ʼ��������Ҫ�ڵ�Դ������һ������ʼ
	FD* fd = dsbob2->fd_tb->GetTable();
	fdph = ifd;//dsbob2->OnGetPhByfdid(fdid);
	strcpy(fdid,dsbob2->fd_tb->GetDataByPh(ifd)->id);

	int iFind = -1;//�ж��Ƿ�Ϊһ���������豸
	ZN* zn = dsbob2->zn_tb->GetTable();

	//��ʵ�ĵ�Դ��
	int r_so = dsbob2->fd_tb->GetDataByPh(ifd)->r_so;
	if (r_so != -1)
	{
		so_p = dsbob2->r_fd_so_tb->FindChildPtr1(r_so);
		so_p0 = so_p; 
		while(so_p)
		{
			//������Դ�㿪ʼ��������
			if (dsbob2->so_tb->GetDataByPh(so_p->Ptr)->slackavr == 4)
			{
				sond= dsbob2->so_tb->GetDataByPh(so_p->Ptr)->ind;
				break;
			}
			so_p = so_p->Next;
		}
  		::FreeChildPtr(so_p0);	
	}
	
	//�������޵�Դ��
	if (sond == -1)
	{
		return 1;
	}

	//������е��豸
	eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((fd+fdph)->r_eq);
	eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
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
		
		//������߿��ط����˹��ϣ���ôһֱ�����������翪�أ�ֱ�����磩����
		//���Լ���ֱ�����翪���ǲ�����Ϊ���뿪�صģ�Ҳ���������л��ݵĿ���

		//���磬���߿���û�й��ϣ�����Ҫ����
		if ( 
			(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 4)
			//||(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type & 16)
			)
		{
			eq_p = eq_p->Next;
			continue;
		}

		iFind = -1;//�ж��Ƿ���һ�������ڵ��豸��**Ŀǰֻ�����߶�����
		if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
		{
			//�����ڵ��豸�������ܱ߿����ǰ���һ��ģ�һ�����ϣ�ȫ������
			r_zn = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->r_zn;
			if ( ((r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1(r_zn)) != -1)
				&& (dsbob2->sec_tb->GetDataByPh(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph)->r0 != 0)//0�迹֧·
				)
			{
				sec_p = dsbob2->r_zn_sec_tb->FindChildPtr1(dsbob2->zn_tb->GetDataByPh(r_zn)->r_sec);
				sec_p0 = sec_p; 
				while(sec_p)
				{
					//�豸�Ƿ��Ѿ��������	
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
			
			//�����������������߶δ����������Ҫ�ٽ��д���ֱ�ӽ�����������豸
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

		//�������߶��⣬�����������豸��Ҫһ��һ���ķ���
		if (iFind == -1)
		{
			//��ÿһ���豸���й��ϴ���
			//��ʼ���豸����ÿһ���豸�����ĵ���Ϊ-1����������˽�����д���ൺ�ı��
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
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpSwitchload[i] = -1;//���϶����θ����б����л����������л�ʱ������⣩
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpNonOutload[i] = -1;//���϶����θ����б����л����������л�ʱ������⣩
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpPlanSwitchload[i] = -1;//���϶����θ����б����л����������л�ʱ������⣩
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpPlanNonOutload[i] = -1;//���϶����θ����б����л����������л�ʱ������⣩
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->Faultload[i] = -1;	//���϶�ʧ�縺���б�
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NeedTrload[i] = -1;//���϶�������Ҫת���ĸ����б�
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NowTrload[i] = -1;//���϶����ο���ת���ĸ����б�
			}
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->tranfdlist[0] = 0;//�豸�������Ϻ��ת�������б�
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->closeunlist[0] = 0;//�豸�������Ϻ�� �պ� ���翪���б�
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->opencblist[0] = 0; //�豸�������Ϻ�� �� �ֶο����б�

			tranfdlist[0] = 0;//�豸�������Ϻ��ת�������б�
			closeunlist[0] = 0;//�豸�������Ϻ�� �պ� ���翪���б�
			opencblist[0] = 0; //�豸�������Ϻ�� �� �ֶο����б�
			eq_tb_isolate->Empty();
			lv_eq_tb_isolate=0;
			i_island = 0;

//			//�Լ�������Ǹ���
//			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20)//����
//			{
//				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->Faultload[nFaultload++] = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;
//				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nFaultload++;
//			}
//			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->island = lv_eq_tb_isolate;//���ϵ�

			//�����豸�����Ӧ�������ڹ��ϵ�������Ϊ0��lld add 2020-5-11
			eqph = eq_p->Ptr;
			ind =  dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
			iznd =  dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;
			dsbob2->eq_tb->GetDataByPh(eqph)->island = i_island;

			//1.Ѱ������豸���ܱ߸��뿪�أ������طŵ���ʱ�Ŀ��ر��ﱸ��		
			//���߿��ء����翪��Ҫ���⴦��
			//���߿��ع��Ϻ��Լ�Ҫ��Ϊ���뿪��(һ���и��뵶բ����һ������ȥ)�����û�г��߿��أ���Դ��ĵ�һ��
			//���߶�Ҫģ��ɳ��߿���,���Լ��Ǹ����Լ����أ�ֻ����һ����
			if (
				(dsbob2->eq_tb->GetDataByPh(eqph)->type & 16)
				)
			{
				EQ* cbt = new EQ();
				strcpy(cbt->id, dsbob2->eq_tb->GetDataByPh(eqph)->id);
				dsbob2->eq_tb->FindPh(*cbt);

				//�޸�cbt
				cbt->i_id = lv_eq_tb_isolate++;
				cbt->ind = ind;//��࿪�أ���Ҫ��չ���豸����ȥ
				cbt->iznd = iznd;//�ڲ࿪��
				eq_tb_isolate->Append(*cbt);
				delete cbt;
			}
			else
			{
				//	//�Ӹ��豸�������ڵ㿪ʼ��������ͣ���豸���ܱ߸��뿪��ѡ�������浽eq_tb_isolate����
				//������ĸ�ߺ���ͨ���صȶ�ڵ��豸��������������ڵ㣬���������ڵ㿪ʼ������չ����
				//���豸���ܱ߸��뿪��ѡ�������浽eq_tb_isolate����
				if (ind != -1)
					OnAnalyseIsolateCB(eqph, ind, iznd);
				if (iznd != -1)
					OnAnalyseIsolateCB(eqph, iznd, ind);
			}
			i_island++;//���ϵĸ����Ѿ�����Ϊ0������ڵ�����1��������2~������

			//2.��һ��ȫ���ˣ�ͨ��eq_tb_isolate�����������
			//��0����Ϊ���ϵ�
			//�˺�Ĳ�����Ҫ��ÿ��Ԫ���ĵ���д�룬���ڽ��г������㣬lld 2020-4-29
			ret = OnGetIsland(eqph,sond);

			//3.ͳ��ÿ�������������������Դ��ĵ���������Դ��ĵ��������翪�صĵ�
			ret = OnAnalyseIsland(eqph,fdph);

			//����ÿ�����ĳ�����lld add 2020-4-27
			//�����������ĳ�����ÿ���豸дһ���������������豸���_pf_date.pf
			//�Ȱѵ��ڵ��豸ȫ��ת������ص�Ԫ��������Ԫ��������Ӧ�ĵ���
			if (pathFlow[0] != 0)
			{
				char  fileName[128];
				int ibs, zbs;
				//��ÿ���豸�ĵ�������
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

						dsbob2->bs_tb->GetDataByPh(ibs)->q |= BS_UNION;//���翪��׼������������ĵ�Դ��

						//�����翪��ģ��ɵ�Դ��
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
					//д�豸�ļ�
					strcpy(fileName, pathFlow);
					strcat(fileName, dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id);
					strcat(fileName, ".eFile");
					dsbob2->WriteOutfile_pf(fileName,-1);
				}
			}

			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
			{//�����Ѵ���
				dsbob2->sec_tb->GetDataByPh(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph)->lC_zn = eq_p->Ptr;
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ld_cnt_loss = 1;
			}
		}
		eq_p = eq_p->Next;
	}
	::FreeChildPtr(eq_p0);

	return 1;	
}

//�����ĵ����ӵ�Դ�㿪ʼ���������������ͻ
//�ٶϵ�Ҫ�ҳ������ٶϵ�ǰ����ٶϣ���������л�ʱ��
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

	int UpStreamload[200];//���θ��ɵ������
	int nUpStreamload=0;

	int			extndcount=0;//��չ�ڵ���
	int			extnd[100];//��չ�ڵ�

	ND* nd_t = new ND[lv_nd];
	dsbob2->nd_tb->CopyTable(nd_t);//�õ�һ���µĽڵ�������ԭ���ĳ�ͻ

	//�����е���ʱ�ӵ���Ϊ�Ѵ���
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

	ind = sond;//ĸ�ߵ���ʼ�ڵ���
	//��Դ������ʼ�ڵ����������豸����һ�����Գе�����Ϊ���ɱ�С��
	extnd[extndcount++] = ind;
	
	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd_t+ind)->q &= (~ND_UNPROC);

		while( extndcount >= 1 )//���ѭ�����һ�����ε�����
		{
			ind = extnd[--extndcount];//ÿ�ζ�ȡ�����Ǹ��ڵ������չ
			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
			(nd_t+knd)->fch = ind; //��ǰ�ڵ�Ÿ���ǰ���� 
			(nd_t+ind)->bch = knd;//�ӵ�Դ�㿪ʼ��ʼ����,����Դ����Ϊ��һ����¼����
			(nd_t+knd)->q &= (~ND_UNPROC);
			while (1)//������չ����û�д���
			{
				//knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
				eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
				eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
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
					//���翪��
					if (dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 4)
					{
						dsbob2->eq_tb->GetDataByPh(i_id_eq)->island = i_island;
					}

					//���ɿ����л�
					if ((i_island == 1) && (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20))
					{
						dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->island = i_island;
						UpStreamload[nUpStreamload++] = dsbob2->eq_tb->GetDataByPh(i_id_eq)->idph;
					}

					//����
					if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20)
						dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->island = i_island;

					if ((kknd==-1) || !((nd_t+kknd)->q & ND_UNPROC))
					{
						eq_p = eq_p->Next;
						continue;					
					}

					//����Ǹ����豸�����أ�
					strcpy(eqid,dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id);
					if ( (iFind == -1) && ( (ret = OnGetDevByID(eq_tb_isolate,eqid)) != -1 ) )
					{
						//��һ�ν���һ���ǹ������ε��׶˿���
						iFind = kknd;
						//׼�����µĵ�
						for (i = 0; i < lv_eq_tb_isolate; i++)
						{
							if ((eq_tb_isolate->GetDataByPh(i)->i_id == ret)
								&& !(eq_tb_isolate->GetDataByPh(i)->type & 16)//�����ǳ��߿���
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
							//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
							(nd_t+kknd)->fch = (nd_t+knd)->fch;
							//�½ڵ� ��� ��չ�ڵ��ǰ����
							(nd_t+knd)->fch = kknd;
							//������ڵ����һ����¼������׼������
							(nd_t+kknd)->bch = knd;
							
							//���ҵ����½ڵ�����Ѵ���
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

	//�ҵ��˽ڵ�
	int iRelay=-1;//�����ٶϿ���
	int iSwitch=-1;//���ο����л�����
	
	if (iFind != -1)
	{
		knd = iFind;
		do
		{
			ind = knd;//���ο��صĽڵ��
			knd = (nd_t+knd)->bch;//�õ����ص��ϼ��ڵ��

			i_id_eq = OnGetDevByNDs(dsbob2,ind,knd);
			//��������йµ�
			if (dsbob2->eq_tb->GetDataByPh(i_id_eq)->island == 0)
			{
				break;
			}

			if ( !(dsbob2->eq_tb->GetDataByPh(i_id_eq)->q & Q_BREAKERFEEDER)
				&& ((dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 3)//�۶���
				||(dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 5)) //�������Ŀ���
				)
			{
				//�ٶϿ��ؿ������ϲ�Ľڵ�
				if((iRelay==-1) && (ind != iFind))//��һ�����ر����Լ����Ǵ������Ŀ��أ����ܰ��Լ����
					iRelay = ind;
			}
			if ( !(dsbob2->eq_tb->GetDataByPh(i_id_eq)->q & Q_BREAKERFEEDER)
				&& 
				((dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 0)//ֻҪ���ǵ�բ����
				||(dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 1)//ֻҪ���ǵ�բ����
				||(dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 3)//ֻҪ���ǵ�բ����
				||(dsbob2->eq_tb->GetDataByPh(i_id_eq)->type == 5))//ֻҪ���ǵ�բ����
				)
			{
				//�ٶϿ��ؿ������ϲ�Ľڵ�
				if(iSwitch==-1)
					iSwitch = ind;
			}

		}	while(knd != ind);//һֱ����Դ��Ϊֹ	
	}

	//�����ٶϿ���
	if (iRelay != -1)
	{
		OnGetUpIsland(sond,iRelay,eqph,0);
//
//		//���ٶϿ��صĿ�����Դ��Ľڵ�����������õ���ͣ��ĸ���
//		//�����е���ʱ�ӵ���Ϊ�Ѵ���
//		for(i = 0; i < lv_nd; i++)
//		{
//			(nd_t+i)->q |= ND_UNPROC;
//		}
//
//		ind = sond;//ĸ�ߵ���ʼ�ڵ���
//		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
//		if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
//		{
//			(nd_t+ind)->q &= (~ND_UNPROC);
//
//			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
//			(nd_t+knd)->fch = ind; //��ǰ�ڵ�Ÿ���ǰ���� 
////			(nd_t+ind)->bch = knd;//�ӵ�Դ�㿪ʼ��ʼ����,����Դ����Ϊ��һ����¼����
//			(nd_t+knd)->q &= (~ND_UNPROC);
//			while (1)//������չ����û�д���
//			{
//				eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
//				eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
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
//					if ( dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20 )//���ɿ����л�
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
//						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
//						(nd_t+kknd)->fch = (nd_t+knd)->fch;
//						//�½ڵ� ��� ��չ�ڵ��ǰ����
//						(nd_t+knd)->fch = kknd;
//						//������ڵ����һ����¼������׼������
////						(nd_t+kknd)->bch = knd;
//						
//						//���ҵ����½ڵ�����Ѵ���
//						(nd_t+kknd)->q &= (~ND_UNPROC);
//					}
//					//ֻ���ǵ�1���������
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
	//���θ���
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

//����ÿһ���豸�ĸ��뿪�أ�ֻ���ǿ��أ���������õ����п���
//���߿��ع��ϣ�ֻ�ܰѿ�����Ϊ���뿪�ش�������������չ
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
	dsbob2->nd_tb->CopyTable(nd_t);//�õ�һ���µĽڵ�������ԭ���ĳ�ͻ

	//�����е���ʱ�ӵ���Ϊ�Ѵ���
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}
	if (oppsite!=-1)
		(nd_t+oppsite)->q &= (~ND_UNPROC);

	ind = start;//ĸ�ߵ���ʼ�ڵ���
	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd_t+ind)->q &= (~ND_UNPROC);

		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
		(nd_t+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
		(nd_t+ind)->bch = knd;//�ӵ�Դ�㿪ʼ��ʼ����,����Դ����Ϊ��һ����¼����
     	while (1)//������չ����û�д���
		{
			eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
			eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
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
						(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 3)//�۶���
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 5) //�������Ŀ���
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 0) 
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 1) 
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 2) 
						||(dsbob2->eq_tb->GetDataByPh(eq_id)->type == 4) 
						)
					{
						//��¼����
						EQ* cbt = new EQ();
						strcpy(cbt->id,dsbob2->eq_tb->GetDataByPh(eq_id)->id);
						dsbob2->eq_tb->FindPh(*cbt);
					
						//�޸�cbt
						cbt->i_id = lv_eq_tb_isolate++;
						cbt->ind = knd;//��࿪�أ���Ҫ��չ���豸����ȥ
						cbt->iznd = kknd;//�ڲ࿪��
						eq_tb_isolate->Append(*cbt);
						delete cbt;
					}
					else
					{
						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
						(nd_t+kknd)->fch = (nd_t+knd)->fch;
						//�½ڵ� ��� ��չ�ڵ��ǰ����
						(nd_t+knd)->fch = kknd;
						//������ڵ����һ����¼������׼������
						(nd_t+kknd)->bch = knd;
						
						//���ҵ����½ڵ�����Ѵ���
						(nd_t+kknd)->q &= (~ND_UNPROC);
						dsbob2->eq_tb->GetDataByPh(eq_id)->island = i_island;	
					}
				}

				if (dsbob2->eq_tb->GetDataByPh(eq_id)->type == 20)//����
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

//������Դ���һ���ڵ㣬����ȫ�����豸
int CNM1::OnGetUpIsland(int sond,int end_nd,int eqph,int type)
{
	int i,ret=-1,iFind=-1;
	int			ind, knd, kknd;
	ChildPtr1	*eq_p, *eq_p0;
//	 eqid[MAX_ID_DLL];
//	 ldid[MAX_ID_DLL];

	int lv_nd=dsbob2->nd_tb->GetCount();
	ND* nd_t = new ND[lv_nd];
	dsbob2->nd_tb->CopyTable(nd_t);//�õ�һ���µĽڵ�������ԭ���ĳ�ͻ	//���ٶϿ��صĿ�����Դ��Ľڵ�����������õ���ͣ��ĸ���
	//�����е���ʱ�ӵ���Ϊ�Ѵ���
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

	ind = sond;//ĸ�ߵ���ʼ�ڵ���
	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd_t+ind)->q &= (~ND_UNPROC);

		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
		(nd_t+knd)->fch = ind; //��ǰ�ڵ�Ÿ���ǰ���� 
//			(nd_t+ind)->bch = knd;//�ӵ�Դ�㿪ʼ��ʼ����,����Դ����Ϊ��һ����¼����
		(nd_t+knd)->q &= (~ND_UNPROC);
		while (1)//������չ����û�д���
		{
			eq_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_t+knd)->r_eq);
			eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
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

				if ( dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20 )//���ɿ����л�
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
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd_t+kknd)->fch = (nd_t+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd_t+knd)->fch = kknd;
					//������ڵ����һ����¼������׼������
//						(nd_t+kknd)->bch = knd;
					
					//���ҵ����½ڵ�����Ѵ���
					(nd_t+kknd)->q &= (~ND_UNPROC);
				}
				//ֻ���ǵ�1���������
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
//	//���Ѿ����ֳ����ˣ����ɵ�͵�Դ��Ҳ�Ѿ�����
//
//
//	return 1;
//}

//���������������ÿ���ת���ĸ���
int CNM1::OnAnalyseIsland(int eqph,int fdph)
{
	int ii=0,ph,knd,ind,iznd,pcb_temp,i,j;
	ChildPtr1	*eq_p, *eq_p0;
	double w,r,pq_abund,p_abund,q_abund,v_abund;
	char fdid_abund[MAX_ID_DLL];
	int bFind;
	int ldCount,cpCount,unCount,soCount, secCount;//һ�����ڵ�����

	int lv_cp=0;
	if (lld->mx_cp>0)
	{
		int lv_cp =  dsbob2->cp_tb->GetCount(); 
	}
	int lv_so =  dsbob2->so_tb->GetCount(); 
	SO* so = dsbob2->so_tb->GetTable(); 
	ND* nd = dsbob2->nd_tb->GetTable(); 
	for(ii=2;ii<i_island;ii++)
	{//0�ǹ��ϵ���������1�����ε�����2��ʼ�����ε�
		g_island = ii-2;
		ldCount=cpCount=unCount=soCount=secCount=0;

		eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((dsbob2->fd_tb->GetTable()+fdph)->r_eq);
		eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
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
			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 10)//�߶�
			{
				ph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;
				islandlist[g_island].island_seclist[secCount].sec_ptr = ph;
				secCount++;
			}

			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 20)//����
			{
				ph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;

				//Ӧ��ת���ĸ���
				dsbob2->eq_tb->GetDataByPh(eqph)->NeedTrload[dsbob2->eq_tb->GetDataByPh(eqph)->nNeedTrload++] = ph;

				islandlist[g_island].island_ldlist[ldCount].ld_ptr = ph;
				w = dsbob2->ld_tb->GetDataByPh(ph)->w;
				r = dsbob2->ld_tb->GetDataByPh(ph)->r;
				islandlist[g_island].island_ldlist[ldCount].p_ld = w;
				islandlist[g_island].island_ldlist[ldCount].q_ld = r;
				islandlist[g_island].island_ldlist[ldCount].pq_ld = sqrt(w * w + r * r);

				//����˫��Դ����
				knd = dsbob2->ld_tb->GetDataByPh(ph)->ind;//���ɶ�Ӧ�Ľڵ��
				if (((dsbob2->nd_tb->GetDataByPh(knd)->q) & ND_SO))
					//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//���Դ
				{
					islandlist[g_island].island_ldlist[ldCount].soSum = 2;
				}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
				else
					islandlist[g_island].island_ldlist[ldCount].soSum = 1;

				islandlist[g_island].island_ldlist[ldCount].nd = knd;
				ldCount++;
			}

			ind = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
			iznd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;
		
			if ( ((ind!=-1) && ((nd + ind)->q & ND_SO)) || ((iznd != -1) && ((nd + iznd)->q & ND_SO)))
			{
				//��so����ȫѭ������Ϊso����������,�����Ƿֲ�ʽ��Դ
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
				//��cp����ȫѭ������Ϊcp����������
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

			if	(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 4)//���翪��
			{ 
				ph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idph;
				ind = dsbob2->cb_tb->GetDataByPh(ph)->ind;//���ɶ�Ӧ�Ľڵ��
				iznd = dsbob2->cb_tb->GetDataByPh(ph)->iznd;//���ɶ�Ӧ�Ľڵ��
				if (ind >= 0)
					knd = ind;
				else
					knd = iznd;

				bFind = 0;
				pq_abund = OnGetUnCBAbund(fdph,ph,fdid_abund,p_abund,q_abund,v_abund);
				//������翪�ص�ĸ�ߺ���ǰ�����翪��ĸ��һ��
				for (i=0;i<unCount;i++)
				{
					//�Ƚ����翪��ԣ�ȣ�ѡ����ԣ��
					pcb_temp = islandlist[g_island].island_unlist[i].nd;
					if ( (nd+pcb_temp)->ibs == (nd+knd)->ibs )
					{
						//�滻
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
		islandlist[g_island].canTrans = 1;//�ȼ�����ת
	}

	g_island=i_island-2;

	//��������ۻ����У�Ҫ����ת���Ͳ���ת���ĵ��������
	if (AnanyseIsland() == 0)//��������������ͨ����˵���ǹµ�
		return 0;

	if (AnanyseResult(eqph) == 0)//�����������ķ������
		return 0;

	return 1;
}

//�������������еĵ�
int CNM1::AnanyseIsland()
{
	int i;
	//�𵺽��м��
	i = 0;
	while (1)
	{
		if (i>=g_island)
			break;

		if (!isNeedDivide(i))//�ж��Ƿ���Ҫ�зֵ�
		{
			i++;//����Ҫ�з֣����������������1��û��Դ�㣻��2�������Ѿ�����ת��
			continue;
		}

		//�з��µĵ����µĵ����ڶ��е����
		//�з�ǰ�����翪�ص�ԣ���Ѿ��ź�����
		//ֻ���зֵ���������ȫû�й������ɲ��ܳɹ�
		if (!divideIsland(i))//�зֲ��ɹ���û�к��ʵĶϿ����ؿ���ת��
		{
			islandlist[i].canTrans = 0;//����ֻ��ת�����ָ���
			islandlist[i].validate = 1;
			i++;
		}
	}	

	return 1;
}

//�����������ķ���������������������󣬽����е�����ֳ���һ�����ĵ�
//���������з���ʱ�����ڲ���ת���ĵ������Է�����������ת���ĸ���
int CNM1::AnanyseResult(int eqph)
{
	int i,j;
	
	int downstream_ld_cnt_t;
	double downstream_ld_pq_t;
	downstream_ld_cnt=0;//�������εĸ��ɸ���
	downstream_ld_pq=0;//�������εĸ������ڹ���
	downstream_multi_ld_cnt=0;//�������ζ��Դ�ĸ�����
	downstream_multi_ld_pq=0;//�������ζ��Դ�����ڹ���
	downstream_trans_ld_cnt=0;//�������ο���ת�ĸ��ɸ���
	downstream_trans_ld_pq=0;//�������ο���ת�ĸ������ڹ���
	unfd_idList[0]='\0';//���ο���ת�������翪��
	un_idList[0]=0;

	transSec[0]=0;
	lossSec[0]=0;
//	 l_transSec[FEMA_ID_LEN];
//	 l_lossSec[FEMA_ID_LEN];
	SEC* sec = dsbob2->sec_tb->GetTable();

	int downstream_ld_cnt_t_;//����ȫ��ת�������
	double downstream_ld_pq_t_;//
	int lossSec_t[MAX_SEC];
	int transSec_t[MAX_SEC];
	int cntTransSec, cntLossSec;

	//�𵺽���ͳ��
	for (i=0; i<g_island; i++)
	{
		//���ɣ��ܣ�
		downstream_ld_cnt_t=downstream_ld_pq_t=0;
		
		downstream_ld_cnt_t=islandlist[i].ldCount;
		downstream_ld_cnt += downstream_ld_cnt_t;
		for(j=0; j<downstream_ld_cnt_t; j++)
		{
			downstream_ld_pq_t += islandlist[i].island_ldlist[j].pq_ld;
			if (islandlist[i].island_ldlist[j].soSum>1)//���Դ
			{
				downstream_multi_ld_cnt++;
				downstream_multi_ld_pq += islandlist[i].island_ldlist[j].pq_ld;
			}
		}
		downstream_ld_pq += downstream_ld_pq_t;

		//�����������һ���ǿ���ȫ��ת��
		if (islandlist[i].canTrans)//����ת��,�ṩ����ת���ĸ��ɣ������Դ��
		{
			downstream_trans_ld_cnt += downstream_ld_cnt_t;
			downstream_trans_ld_pq += downstream_ld_pq_t;

			//�õ�����ת�������翪��
			strcat(unfd_idList,islandlist[i].island_unlist[0].secid);
			strcat(unfd_idList,",");
			strcat(un_idList,dsbob2->cb_tb->GetDataByPh(islandlist[i].island_unlist[0].cbptr)->id);			
			strcat(un_idList,"|");

			for(j=0; j<downstream_ld_cnt_t; j++)
			{
				downstream_ld_pq_t += islandlist[i].island_ldlist[j].pq_ld;

				dsbob2->eq_tb->GetDataByPh(eqph)->NowTrload[dsbob2->eq_tb->GetDataByPh(eqph)->nNowTrload++] = islandlist[i].island_ldlist[j].ld_ptr;

				if (islandlist[i].island_ldlist[j].soSum>1)//���Դ
				{
					downstream_multi_ld_cnt++;
					downstream_multi_ld_pq += islandlist[i].island_ldlist[j].pq_ld;
				}
			}

//			//�����翪�����
//			
//			if (islandlist[i].secCount > 0)
//			{
//				//������ڵ��߶�
//				l_transSec[0]=0;
//				for(j=0; j<islandlist[i].secCount; j++)
//				{
//					strcat(l_transSec,lld->IntToString(islandlist[i].island_seclist[j].sec_ptr));
//					strcat(l_transSec,"|");
//				}
//				//�˵������Ǹ�
//				lld->delstrright(l_transSec,1,l_transSec);
//				strcat(transSec,l_transSec);
//				strcat(transSec,",");
//			}
		}
		else
		{
			//���ڲ��ֿ���ת����������ת�����߶κͲ�����ת�����߶λ��ֳ���
			//�����ԣ�ȵ����翪�ؽ������˷���	
			
			//����Ҫ��ʼ��
			downstream_ld_cnt_t_=downstream_ld_pq_t_=cntTransSec=cntLossSec=0;
			if (getPartTrans(eqph,i,downstream_ld_cnt_t_,downstream_ld_pq_t_,transSec_t,lossSec_t,cntTransSec,cntLossSec))
			{
				downstream_trans_ld_cnt += downstream_ld_cnt_t_;
				downstream_trans_ld_pq += downstream_ld_pq_t_;
				
				if (downstream_ld_cnt_t_ > 0)
				{
					//�õ�����ת�������翪��
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
//					//�˵������Ǹ�
//					lld->delstrright(l_transSec,1,l_transSec);
//					strcat(transSec,l_transSec);
//					strcat(transSec,",");
//				}
//				//������ڵ��߶�
//				if (cntLossSec > 0)
//				{
//					l_lossSec[0]=0;
//					for(j=0; j<cntLossSec; j++)
//					{
//						strcat(l_lossSec,lld->IntToString(lossSec_t[j]));
//						strcat(l_lossSec,"|");
//					}
//					//�˵������Ǹ�
//					lld->delstrright(l_lossSec,1,l_lossSec);
//					strcat(lossSec,l_lossSec);
//					strcat(lossSec,",");
//				}

			}
			else
			{//ȫ������ת��
				for(j=0; j<downstream_ld_cnt_t; j++)
				{
					downstream_ld_pq_t += islandlist[i].island_ldlist[j].pq_ld;

//					//һ����ʧ�ĸ��ɼӵ������б�����
//					ra_feeder[ra_cnt].ld_id = islandlist[i].island_ldlist[j].ld_ptr;//���ɵ������
//					ra_feeder[ra_cnt].up_cb = up_cb;//���ο��ص����ʣ��Թ����л�����
//					ra_feeder[ra_cnt].out = 1;//������ʧ
//					ra_feeder[ra_cnt].swit = 0;//���ɲ����л�
//					ra_feeder[ra_cnt].tran = 0;//������ת��
//					ra_feeder[ra_cnt].sec_id = sec_ana;//���ɶ�Ӧ�Ĺ�����������ţ���ʱ�����߼�ͳһ��
//					ldlist[ra_feeder[ra_cnt].ld_id] = 1;//�����Ѵ���
//					ra_cnt++;					
				}


//				//������ڵ��߶�
//				if (islandlist[i].secCount > 0)
//				{
//					l_lossSec[0]=0;
//					for(j=0; j<islandlist[i].secCount; j++)
//					{
//						strcat(l_lossSec,lld->IntToString(islandlist[i].island_seclist[j].sec_ptr));
//						strcat(l_lossSec,"|");
//					}
//					//�˵������Ǹ�
//					lld->delstrright(l_lossSec,1,l_lossSec);
//					strcat(lossSec,l_lossSec);
//					strcat(lossSec,",");
//				}
			}
		}
	}

	//��ȡת���������ߡ����翪�ء��жϿ���
	strcpy(dsbob2->eq_tb->GetDataByPh(eqph)->tranfdlist , unfd_idList);
	strcpy(dsbob2->eq_tb->GetDataByPh(eqph)->closeunlist , un_idList);
	strcpy(dsbob2->eq_tb->GetDataByPh(eqph)->opencblist , opencblist);

	return 1;
}


//���������ĵ����õ�����ת�����߶Σ�����ת���ĸ��ɸ����ʹ�С
int CNM1::getPartTrans(int eqph,int island, int& ld_cnt, double& ld_pq,int* transSec,int* lossSec,int& cntTransSec, int& cntLossSec)
{
	int ndCount,ldCount;//һ�����ڵ�����
	double pq_abund,unAbund;//�����й����޹�����ѹԣ��

	int knd,lv_nd,lv_cb,lv_cp,lv_so;
	int			i,j;
	int			ind, kknd,ld_nd,r_zn;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;

	ChildPtr1	*sec_p;//���ؿ�������2�����ߣ������ÿ��ص�fdid������ֻ�������߶�

	//û�и��ɻ������硢��Դ
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
	//�����еĽڵ���Ϊδ����׼�����˷�����
	//�˴��ƺ������Ż������û����ô��Ŀռ䣬����ֻ����صĽڵ�
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->q |= ND_UNPROC;
	}

//////////////////////////////////////////////////////////////////////////
//���ڷֲ�ʽ��Դ�ж�����Ĺ�������
//1.����ֲ�ʽ��ԴΪ��������Ĺ������������ܣ���ֲ�ʽ��Դ��ͬ�����翪��
//2.����ֲ�ʽ��ԴΪͬ����������ͬ�ڶ��Դ�������޷��л�
//lld add 2017-3-26
//////////////////////////////////////////////////////////////////////////

	unAbund=0;
	knd = -1;
	//���ȿ������翪��
	if (islandlist[island].unCount > 0)
	{
		knd = islandlist[island].island_unlist[0].nd;//ͨ���ڵ���ҵ�Դ��
		unAbund=islandlist[island].island_unlist[0].pq_abund;
	}
	else	//���û�����翪�أ��������ķֲ�ʽ��Դ
	{
		double ttt=0;
		for(i = 0; i < islandlist[island].soCount; i++)
		{
			unAbund = islandlist[island].island_solist[i].pq;
			if (unAbund > ttt)
			{
				ttt = unAbund;
				knd = islandlist[island].island_solist[i].nd;//ͨ���ڵ���ҵ�Դ��
			}
		}
	}
	if (knd == -1)//û�����翪�أ�Ҳû�зֲ�ʽ��Դ
		return 0;

	double pq_abund_sum=0;//���ɵ��ۼ�ֵ
	int unTranFlag=0;//����ת���ı�־λ
	cntTransSec=cntLossSec=0;

	ndCount = ldCount = 0;
	ind = knd;//ĸ�ߵ���ʼ�ڵ���

	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd+ind)->q &= (~ND_UNPROC);

		//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8

		//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
		(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
     	while (1)//������չ����û�д���
		{
			//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
			if ((nd+knd)->q & ND_LD) 
			{
				//�õ�����id�����ɺ���Ҫ���������еĶ�Ҫ��������Ե����������д���
				ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
				ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
				while(ld_p)
				{
					if (!unTranFlag)
					{
						pq_abund = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
						pq_abund += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
						pq_abund = sqrt(pq_abund);
						
						//����˫��Դ����
						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//���Դ
						{
						}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
						else
						{
							pq_abund_sum += pq_abund;
						}

						if (pq_abund_sum > unAbund)//����ת������
						{
							ld_cnt = ldCount;
							ld_pq = pq_abund_sum - pq_abund;
							unTranFlag = 1;//��ʼ����ת����

							//һ����ʧ�ĸ��ɼӵ������б�����
							dsbob2->eq_tb->GetDataByPh(eqph)->NowTrload[dsbob2->eq_tb->GetDataByPh(eqph)->nNowTrload++] = ld_p->Ptr;

							ldCount++;
						}
					}
					else
					{
//							//һ����ʧ�ĸ��ɼӵ������б�����
//							ra_feeder[ra_cnt].ld_id = ld_p->Ptr;//���ɵ������
//							ra_feeder[ra_cnt].up_cb = up_cb;//���ο��ص����ʣ��Թ����л�����
//							ra_feeder[ra_cnt].out = 1;//������ʧ
//							ra_feeder[ra_cnt].swit = 0;//���ɲ����л�
//							ra_feeder[ra_cnt].tran = 0;//������ת��
//							ra_feeder[ra_cnt].sec_id = sec_ana;//���ɶ�Ӧ�Ĺ�����������ţ���ʱ�����߼�ͳһ��
//							ldlist[ra_feeder[ra_cnt].ld_id] = 1;//�����Ѵ���
//							ra_cnt++;
					}

					ld_p = ld_p->Next;
				}
				::FreeChildPtr(ld_p0);
			}

			//ֱ�Ӱ��������еķֲ�ʽ��Դ,ialgp=0
			if ((nd+knd)->q & ND_SO) 
			{
				//��so����ȫѭ������Ϊso����������
				for (j=0; j<lv_so; j++)
				{
					if ( ((so+j)->ind == knd) && ((so+j)->ialgp == 0) && (knd !=ind) )
					{
						unAbund += sqrt((so+j)->w*(so+j)->w + (so+j)->r*(so+j)->r);//�޹��൱�����ӵ�Դ������ڹ���
						break;
					}
				}
			}

			if ( (lld->mx_cp>0) && ((nd+knd)->q & ND_CP) )
			{
				//��cp����ȫѭ������Ϊcp����������
				for (j=0; j<lv_cp; j++)
				{
					if ((dsbob2->cp_tb->GetTable()+j)->ind == knd)
					{
						unAbund += (dsbob2->cp_tb->GetTable()+j)->r;//�޹��൱�����ӵ�Դ������ڹ���
						break;
					}
				}
			}

			//read cbs connected to this nd
			//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
			cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
			cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
			while(cb_p)
			{
				//��������翪�أ�ͣ�������ҵ����������ĵ�Դ��
				if ( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				  || (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN)  
				  || (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB) 
				  || (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_ISOLATE) 
				  ) 
				{
					//�����������Ϊ�Ѵ���			
					cb_p = cb_p->Next;
					continue;
				}

				//find the opposite nd through this cb
				//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
				if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}
				//process this opposite nd
				//����Բ�ڵ� δ����==���ز����������ƽ�
				if((nd+kknd)->q & ND_UNPROC)
				{
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd+kknd)->fch = (nd+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd+knd)->fch = kknd;
					//���ҵ����½ڵ�����Ѵ���
					(nd+kknd)->q &= (~ND_UNPROC);
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this ndͨ���߶�������չ
			sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
				//�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
            	r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		        
				if (r_zn == -1)//���߶β������κ�����
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
					//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
					//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
					//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
					
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd+kknd)->fch = (nd+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd+knd)->fch = kknd;
					//���ҵ����½ڵ�����Ѵ���
					(nd+kknd)->q &= (~ND_UNPROC);

					if (unTranFlag)//����ת����
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
			//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
			//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
			//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
			if(knd == ind)
			{
				break;
			}
		}
	}
	
	return 1;
}

//�Ե��������з������鿴�Ƿ�õ����Խ����з�
int CNM1::isNeedDivide(int island)
{
	int j;
	int soCount=0;//���ڷֲ�ʽ��Դ�ĸ���
	double soAbund=0;//���ڷֲ�ʽ��Դ���ܵĶ�ȣ����ڣ�Ϊ����ֵ������ֻ���ṩһ���֣�
	int unCount=0;//�������翪�صĸ�����
	double unAbund=0;//�������翪�ص��ܵ�ԣ�ȣ����ڣ�
	int ldCount=0;//���ڸ��ɸ���
	double ldTotal=0;//���ڸ��ɵ������������ڣ�

	//ֻҪУ����ˣ��Ͳ���Ҫ������
	//validate��������ȫȷ�Ϻ�������Ҳ���Ƿ��������߹µ������������絫�����з֣��Ҳ����зֿ��أ�
	//ֻҪ�������������з֣��Ͳ�����ΪУ���

	//����ת�������Ѿ��������,ֻ�е���Դ��ԣ�Ȳ��������ǲ���ת�������Դ�㲻��ȷ������ת��
	if (islandlist[island].validate == 1)
		return 0;

	soCount = islandlist[island].soCount;
	unCount = islandlist[island].unCount;
	ldCount = islandlist[island].ldCount;
	
	//���ڷֲ�ʽ��Դ
	for (j=0; j<soCount; j++)
	{
		soAbund += islandlist[island].island_solist[j].pq;
	}

	//�������翪�أ����翪��Ҳ�൱�ڵ�Դ��
	for (j=0; j<unCount; j++)
	{
		unAbund += islandlist[island].island_unlist[j].pq_abund;
	}

	//���ڸ���
	for (j=0; j<ldCount; j++)
	{
		ldTotal += islandlist[island].island_ldlist[j].pq_ld;
	}

	//1.����Ϊ0��2.�и��ɵ���û�����翪��;3.����������û�е�Դ��
	//if ( (ldTotal == 0) || ((ldTotal>0) && (unCount==0))  || ((ldTotal>0) && (soCount==0)) )
	if ( (ldTotal == 0) || ((ldTotal>0) && (unCount==0)) )
	{
		islandlist[island].canTrans = 0;//������������ת����ֻ�������
		islandlist[island].validate = 1;
		return 0;//����Ҫ�з�
	}

	//���ڵķֲ�ʽ��Դ�Ϳ���ת��
	if (soAbund * 0.95 > ldTotal)
	{
		//�޷�ת��������Ҫ�����з�
		islandlist[island].canTrans = 1;
		islandlist[island].validate = 1;
		return 0;//����Ҫ�з�
	}

	//ֻ��һ�����翪�أ�ͨ��������翪�ؾͿ���ת�����еĵ�����
	if ( (unAbund * 0.95 > ldTotal) && (unCount == 1) )
	{
		//�޷�ת��������Ҫ�����з�
		islandlist[island].canTrans = 1;
		islandlist[island].validate = 1;
		return 0;//����Ҫ�з�
	}

	//islandlist[island].canTrans = 1;	//������Ϊ������ת��
	trim_unlist(island);//�Ե������е����翪�ذ���ԣ����С�����������

	//�õ������ԣ�ȵĵ�Դ���Ƿ�������ת
	//n-1����ͨ������������ԣ�Ȳ���������������ֵ͵�ѹ������Ҳ���Բ���У��
	if (islandTreeFlow(island))//����У�鲻��ͨ����ԣ�Ȼ�������ѹ����
	{//��ԣ���������翪��ת��
		islandlist[island].canTrans = 1;//��������ת��
		islandlist[island].validate = 1;
		return 0;//����Ҫ�з�
	}

	//ֻ��һ�����翪�ص����
	if (islandlist[island].unCount == 1)
	{
		islandlist[island].canTrans = 0;//������������ת����ֻ�������
		islandlist[island].validate = 1;
		return 0;//����Ҫ�з�
	}
	
	return 1;
}

//�����������ĳ���,�õ��Ѿ���������������ҽ���һ����Դ��
//������ܽ���ת�����ṩ����ת����ȫ�����ɺ͵�Դ�㣬�Լ�ת��·��
int CNM1::islandTreeFlow(int island)
{
	int i/*,j*/;
//	int knd,kknd,globalbs;
	double v_abund;
//	int unCount,mx_un;//���翪�صĸ���,���ԣ�ȵ����翪��
	double pq_so,pq_ld;
	//����֮ǰ�����жϵ�Դ���Ƿ�ƽ�⸺��

	v_abund = islandlist[island].island_unlist[0].v_abund;
	if (v_abund<0.93)//�����ĵ�ѹ�Ѿ�����֧��
		return 0;

	pq_so = islandlist[island].island_unlist[0].pq_abund;
	pq_ld = 0;
	for (i=0; i<islandlist[island].ldCount; i++)
	{
		pq_ld += islandlist[island].island_ldlist[i].pq_ld;
	}

	//���翪�ص�ԣ�ȱ���Ҫת���ĸ���ҪС,����5%������
	if ((pq_so * 0.95) <= pq_ld)
		return 0;

//////////////////////////////////////////////////////////////////////////
//ֻҪԣ�ȹ���һ�㲻����ֵ�ѹ������
/*
	trim_ndlist(island);//�Ե������еĽڵ���С����������򣬶��ַ��ҽڵ�

	//�Ը����ĵ��������˷������ϲ����أ�ָ������ĸ�ߺ�
	islandTPAnalyse(island,islandlist[island].island_unlist[0].nd);

	//׼���������㣬��Ҫ׼����Դ�㡢���ɵ㡢�������㡢֧·������
	//1.��ʼ����������
	InitWorkingArray(island);
	
	//2.�õ�׼���������������
	SetNetworkData(island);

	//3.ǰ�ƻش���������
	OrderBusNumberA();
	ExecuteSweep();

	//4.�������ж�
	for(j = 0; j < lv_wb; j++)
	{
		if(g_wb[j].tio >= 0)  
		{
			if ( g_wb[j].v < 0.93 )//�����˵͵�ѹ
				return 0;
		}					
	}
*/


	return 1;
}

//�������翪�صĸ�������ȡ���翪�ص����������
int CNM1::getUNcomb(int unCount,int* before,int* next)
{
	int ret = 0;

	int a[2];
	int r=2;
	int m = unCount;
	int cur;//ָʾ��λ�������ĸ���Ա�����ƽ�
	
    unsigned int count=0;
 
	//��ʼ����λ���飬0 ��ʼ��λ�� ����ʼ��ѡ�����λ�� 0��1��2
    a[0]=0;
    a[1]=1;
//	before[0] = 0;
//	next[0] = 1;

    cur=r-1;//��ǰ�����һ����ԱҪ�ƽ�
 
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

	return ret;//������ϵĸ���
}

//�зֵ�
int CNM1::divideIsland(int island)
{
	int i,ret,unCount;
	NM1_Minpath minpath;
	int unCB_1[MAX_UION_COMPOSITE];
	int unCB_2[MAX_UION_COMPOSITE];
	int unCB1,unCB2,cnt_comb,ret_CB1,ret_CB2;
	int bk_cb,g_bk_cb;//׼���򿪵Ŀ���
	double retPQ;//�򿪿��غ�����ת�Ƹ���
	double maxPQ=0;//��ת������󸺺�

	//���������е����翪�ض��г���������������򣬼���������ԣ�ȵ�����ǰ��
	//�������翪�ص���ϣ��õ������зֵ����翪��
	//��1������ת�������ɣ���2��ʵ�ڲ��У�ת�����ĸ���
	unCount = islandlist[island].unCount;//���ڵ����翪������
	//�õ����翪�ص��������������
	cnt_comb = getUNcomb(unCount,unCB_1,unCB_2);

	ret_CB1=ret_CB2=-1;
	//ѡ�������
	for (i=0; i<cnt_comb; i++)
	{
		//ȡ���������ص����
		unCB1 = unCB_1[i];
		unCB2 = unCB_2[i];

		//�����������翪�ص���С·�������еĵ�Դ��͸��ɣ����������۵���С·��
		if (getMiminalPath(island, unCB1,unCB2,minpath))
		{
			retPQ = 0;
			bk_cb = -1;
			ret = setBreakCB(island, minpath,bk_cb,retPQ);//�趨�⻷�Ŀ��أ������µ�
			if (ret < 0)//Ҫô��û��ƽ�⿪��
				continue;//�����趨�⻷����
			else if (ret >0)//��ȫ���Խ����з�
			{
				ret_CB1=unCB1;
				ret_CB2=unCB2;
				g_bk_cb = bk_cb;
				break;//����Ҫ����һ�������
			}
			else
			{//ֻ�в��ֺϸ�Ҳ����ֻ��ת�����ָ��ɣ�ȡ���ĸ���
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
			//�Ӳ����������Ҳ���������������翪�أ�˵������������
			ret_CB1=ret_CB2=-1;
			break;
			//continue;//�ò�����С·
		}
	}

	//�ÿ�����Ͻ��в��,�������أ����������һ���ֶ�
	if ( (ret_CB1 == -1) || (g_bk_cb == -1) )
	{//û�а취�����з֣�Ҳ������������֮��û����Ч��ͨ·����ʹ��Ҳ����ת�����ɣ�ֻ���������ת��һ����
		return 0;
	}

	dsbob2->cb_tb->GetDataByPh(minpath.cbptr[g_bk_cb])->q |= CB_N_ISOLATE;
	//��¼�ֶο���id
	strcat(open_idList,dsbob2->cb_tb->GetDataByPh(minpath.cbptr[g_bk_cb])->id);
	strcat(open_idList,"|");

	//�����翪����Ϊ��㣬������������������һ��������һ�����ţ�����������һ������
	islandlist[g_island].validate = 0;
	setNewIsland(island,ret_CB2,g_island);
	islandlist[island].validate = 0;
	//�µĵ�,�µĵ�Ҫ���ϵĵ����ѳ���
	setNewIsland(island,ret_CB1);

	//���������µ����г���У��
	if (!islandTreeFlow(island))//����У�鲻��ͨ�����������ĵ�Դ�㲻��ת�����еĸ���
	{
		//����Ҫ�ж��Ƿ��ж�����翪��
		if (islandlist[island].unCount < 2 )
		{
			islandlist[island].canTrans = 0;//����ת��
			islandlist[island].validate = 1;//У�����
		}
		else
			islandlist[island].canTrans = 1;//����ת�������������з�
	}
	else
	{
		islandlist[island].canTrans = 1;//��ת��
		islandlist[island].validate = 1;//У�����
	}

	if (!islandTreeFlow(g_island))//����У�鲻��ͨ�����������ĵ�Դ�㲻��ת�����еĸ���
	{
		//����Ҫ�ж��Ƿ��ж�����翪��
		if (islandlist[g_island].unCount < 2 )
		{
			islandlist[g_island].canTrans = 0;//����ת��
			islandlist[g_island].validate = 1;//У�����
		}
		else
			islandlist[g_island].canTrans = 1;//����ת�������������з�
	}
	else
	{
		islandlist[g_island].canTrans = 1;//��ת��
		islandlist[g_island].validate = 1;//У�����

//		//��¼��λ�����翪�ؿ���id,ֻ��У����˲ſ��Խ���ת
//		strcat(un_idList,"|");
//		strcat(un_idList,dsbob2->cb_tb->GetDataByPh(islandlist[island].island_unlist[unCB1].cbptr);
	}

	g_island++;

	return 1;
}




//�����������翪��unCB1��unCB2���ҵ������·��
//��Զ���õ��ڵ��������ԣ�ȿ��ؽ��е����з�
int CNM1::getMiminalPath(int island, int unCB1,int unCB2,NM1_Minpath& minpath)
{
	int cbCount,comCount;//��С·�Ͽ��ص��������߶ε�����

	int knd,lv_nd;
	int			i;
	int			ind, kknd,r_zn;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p0,*sec_p;
	
	//������翪��û���ҵ���˵��û�л�·
	int iFind = 0;//�ҵ���Ҫ�����翪��
	int first  = islandlist[island].island_unlist[unCB1].cbptr;
	int second = islandlist[island].island_unlist[unCB2].cbptr;

	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = islandlist[island].ndCount;
	//�����ڵĽڵ���Ϊδ����
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[island].island_ndlist[i].nd_ptr;
		(nd+knd)->q |= ND_UNPROC;
	}

	//��һ�����翪�ش���ʼ����
	ind = islandlist[island].island_unlist[unCB1].nd;
	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd+ind)->q &= (~ND_UNPROC);

		//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
		(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
   		while (1)//������չ����û�д���
		{
			//read cbs connected to this nd
			//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
			cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
			cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
			while(cb_p)
			{
				//��������翪�ػ�Ͽ��ķֶο��أ�ͣ�������ҵ����������ĵ�Դ��
				if( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				  ||(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_ISOLATE) )		
				{
					if (second == cb_p->Ptr)//�����ҵ�����һ�����翪��
						iFind++;

					cb_p = cb_p->Next;
					continue;
				}

				//find the opposite nd through this cb
				//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
				if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}
				//process this opposite nd
				//����Բ�ڵ� δ����==���ز����������ƽ�
				if((nd+kknd)->q & ND_UNPROC)
				{
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd+kknd)->fch = (nd+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd+knd)->fch = kknd;

					//������ڵ����һ����¼������׼������
					(nd+kknd)->bch = knd;

					//���ҵ����½ڵ�����Ѵ���
					(nd+kknd)->q &= (~ND_UNPROC);					
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this ndͨ���߶�������չ
			sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
				//�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
            	r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		        
				if (r_zn == -1)//���߶β������κ�����
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
					//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
					//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
					//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
					
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd+kknd)->fch = (nd+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd+knd)->fch = kknd;
					//������ڵ����һ����¼������׼������
					(nd+kknd)->bch = knd;
					//���ҵ����½ڵ�����Ѵ���
					(nd+kknd)->q &= (~ND_UNPROC);
				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd
			knd = (nd+knd)->fch;
			//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
			//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
			//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
			if(knd == ind)
			{
				break;
			}
		}
	}

	if (iFind != 1)//��һ�����翪�ؿ�ʼû���ҵ���һ�����翪�أ������ҵ���������1�����ػع��磩
		return 0;

	//�ڻ��ݵ�ʱ��Ҫ�жϷֶο������ܵ�֧·�ĵ�ѹ��
	knd = islandlist[island].island_unlist[unCB2].nd;//���������翪�صĽڵ�ſ�ʼ����
	kknd = islandlist[island].island_unlist[unCB1].nd;//���ݵ��յ�

	//��ʼ��
	cbCount=comCount=0;
	//lld->sortInt(minpath.composite_nd,comCount);//�������򣬷�������
	for (i=0; i<MAX_ND; i++)
	{
		minpath.composite_ld[i] = 0;
		minpath.composite_so[i] = 0;
		minpath.composite_ld_vip[i] = 0;
	}

	//����ʼ����д���
	minpath.composite_so[comCount]=islandlist[island].island_unlist[unCB1].pq_abund;
	minpath.composite_nd[comCount++]=knd;
//	minpath.composite_fch[comCount]=-1;
//	minpath.composite_bch[comCount++]=knd;

	//ÿ��ȡ�����ڵ���д��������ڶ��ҵ����غ��߶ε�����
	do
	{
		//ÿ��չһ���ڵ㣬������ڵ��ǰ��Ľڵ���Ϊ����ǰ����
		//����ڵ�ĺ�������ʱ��֪����ֻ�еȵ���һ����չ�ڵ���ܸ�����ֵ
		ind = knd;//���ο��صĽڵ��
		knd = (nd+ind)->bch;//�õ����ص��ϼ��ڵ��

//		if(knd==-1)
//		{
//			return 0;//lld edit for debug
//		}

		minpath.composite_nd[comCount++]=knd;
//		minpath.composite_bch[comCount-1]=knd;
//		minpath.composite_fch[comCount++]=ind;
	
		//��Ҫά����Ͻڵ��ǰ������

		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
		cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
		while(cb_p)
		{
			//find the opposite nd through this cb
			//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
			if ((((knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind) && (ind == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd))
			  ||((knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd) && (ind == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)))
			  && (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q !=1))
			{
				//�����翪��������Ǹ����ؿ�����Ϊ������
				//�������û�з�֧����ô���Խ�����Ϊֱ�߿��أ�2��������ֱ�߿���ѡһ���Ϳ����ˡ���������
				
				//ÿ�ҵ�һ�����أ����������߶��ϵĵ����ӽ���
				minpath.cbptr[cbCount++] = cb_p->Ptr;
			}			

			cb_p = cb_p->Next;
		}
		::FreeChildPtr(cb_p0);


	}	while(knd != kknd);//һֱ����Դ��Ϊֹ
//	minpath.composite_bch[comCount-1]=-1;//�յ�ĺ�������-1
	minpath.composite_so[comCount-1]=islandlist[island].island_unlist[unCB2].pq_abund;

	minpath.island = island;
//	minpath.loopR = loopR;
	minpath.cbCount = cbCount;
//	minpath.secCount = secCount;
	minpath.comCount = comCount;

	//��ʣ�µ����翪�ؽ��нڵ������
	int knd_t = islandlist[island].island_unlist[unCB2].nd;//���������翪�صĽڵ�ſ�ʼ����
	int kknd_t = islandlist[island].island_unlist[unCB1].nd;//���ݵ��յ�
	int cnt = islandlist[island].unCount;
	double pq_abund;
	for (i=0; i<cnt; i++)
	{
		knd = islandlist[island].island_unlist[i].nd;
		if ((knd == knd_t)||(knd == kknd_t))
			continue;

		pq_abund = islandlist[island].island_unlist[i].pq_abund;//���������翪�صĽڵ�ſ�ʼ����
		kknd = setCompositeND(knd, minpath.composite_nd,comCount);
		if (kknd != -1)
			minpath.composite_so[kknd] += pq_abund;
	}

	cnt = islandlist[island].ldCount;
	for (i=0; i<cnt; i++)
	{
		knd = islandlist[island].island_ldlist[i].nd;
		pq_abund = islandlist[island].island_ldlist[i].pq_ld;//���������翪�صĽڵ�ſ�ʼ����
		kknd = setCompositeND(knd, minpath.composite_nd,comCount);
		if (kknd != -1)
		{
			minpath.composite_ld[kknd] += -pq_abund;
			minpath.composite_ld_vip[kknd] += islandlist[island].island_ldlist[i].vip;
		}
	}

	return 1;
}

//�����翪�ص�ԣ�Ⱥ͸��ɶ����㵽���·����
int CNM1::setCompositeND(int src_nd, int* com_nd, int cnt)//�趨�⻷�Ŀ���
{
	int ret,knd;
	knd = src_nd;//���������翪�صĽڵ�ſ�ʼ����
	if (knd==-1)
	{
		//lld add 2016-11-19 for nm1 error
		return -1;
	}

	ND* nd = dsbob2->nd_tb->GetTable();
	//ÿ��ȡ�����ڵ���д��������ڶ��ҵ����غ��߶ε�����
	while(1)
	{
		knd = (nd+knd)->bch;//�õ����ص��ϼ��ڵ��

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

//������С·�ҽ⻷���أ����ؿ��ص���� ��ת������󸺺�
int CNM1::setBreakCB(int island, NM1_Minpath minpath,int& bk_cb,double& retPQ)//�趨�⻷�Ŀ���
{
	double maxLD,deli;
	int		i,k,cb_nd,cb_knd,nd,knd;
//	int		cb_ptr;
	double	bso,bld,bvip=0,fso,fld,fvip=0;

	SEC* sec = dsbob2->sec_tb->GetTable();
	CB* cb = dsbob2->cb_tb->GetTable();

	//�����Ż�����
//	optloop = g_wbase * 1000 * fabs(minpath.delV / minpath.loopR);
//�ڽ����߶δ����ʱ���Ҫ������
	
	deli=10000;
	//bk_cb = -1;
	//���������ϵĻ�����ӽ����Ż�������
	for(k = 0; k < minpath.cbCount; k++)
	{
		//ֻ��һ������,ֱ�ӶϿ�����
//		if (minpath.cbCount == 1)
//		{
//			bk_cb=0;
//			break;
//		}	

		//���㿪������ĸ��ɵ�Դ�ۼӣ������������翪��
		cb_nd = dsbob2->cb_tb->GetDataByPh(minpath.cbptr[k])->ind;//��ڵ�
		cb_knd = dsbob2->cb_tb->GetDataByPh(minpath.cbptr[k])->iznd;//�ҽڵ�
		//�ÿ�������Ľڵ�����С·�����ң������ĸ�����С·��������˳���
		nd = lld->IsExistInArray(minpath.composite_nd,cb_nd,minpath.comCount);
		knd = lld->IsExistInArray(minpath.composite_nd,cb_knd,minpath.comCount);
		
		bso=fso=bld=fld=0;

		//��֤nd<knd
		if (nd > knd)
		{//����
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
		//��������Ĺ��ʶ�����ƽ�⣬������Ч
		if (((fso+fld)<0) && ((bso+bld)<0))
		{
		}
		else if (((fso+fld)>=0) && ((bso+bld)>=0))
		{
			if (fabs((fso+fld) - (bso+bld)) < deli)
			{
				//���Ž⿪��
				deli = fabs((fso+fld) - (bso+bld)); 
				bk_cb = k;
			}
		}
		else
		{
			if (deli == 10000) //ֻ����û���ҵ���ѿ��ص�����²ſ��Ǵ��Ž�
			{
				//ת�����ĸ��ɿ�������ѵĿ���
				//����������翪��������
				if ((fso+fld) > 0)
				{
					if (fld > maxLD)
					{
						maxLD = fld;
						bk_cb = k;//�������Խ��ܣ���һ����ƽ���
					}
				}
				else
				{
					if (-bld > maxLD)
					{
						maxLD = -bld;
						bk_cb = k;//�������Խ��ܣ���һ����ƽ���
					}
				}
			}
		}

		//���߻�����ȣ�������һ����ƽ���
	}
	
	//û���ҵ��⻷���أ���ô�����ʵ���Ͼ��ǲ��ܽ��в�ֺ�ת����
	if (bk_cb == -1)
		return -1;

	if (deli==10000)//ֻ����û�к��ʵ�����ת����û�е�����²��ܵ��߶���ֵ
	{
		retPQ=maxLD;
		return 0;
	}

	return 1;
}

//����һ�����еĵ����������翪�أ��⻷���γ������µ�
//�Ѿ���ɷ������ˣ������������翪�ش���
int CNM1::setNewIsland(int old_island, int un_cb,int new_island)//�趨�⻷�Ŀ���
{
	int island,ndCount,ldCount,cbCount,secCount,cpCount,unCount;//һ�����ڵ�����
	double p_abund,q_abund,v_abund,pq_abund;//�����й����޹�����ѹԣ��
	char nd_abund[MAX_SEC_ID_LEN];//���翪�صĶԲ�ڵ�
	char fdid_abund[MAX_SEC_ID_LEN];//���翪�ضԲ�����߱��
	bool bFind;

	int knd,lv_nd,lv_cp,lv_cb;
	int			i,j;
	int			ind, kknd,ld_nd,r_zn,pcb_temp;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p,*sec_p0;
	ChildPtr1	*ld_p, *ld_p0;
	char scb_id[MAX_SEC_ID_LEN];//���翪�ضԲ������id����
	char fdid[MAX_SEC_ID_LEN];
	fdid[0]=0;//�����߳�ʼ��

	CP* cp = dsbob2->cp_tb->GetTable();
	lv_cp = dsbob2->cp_tb->GetCount();

	CB* cb = dsbob2->cb_tb->GetTable();
	lv_cb = islandlist[old_island].cbCount;

	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = islandlist[old_island].ndCount;
	//�����ڵĽڵ���Ϊδ����
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[old_island].island_ndlist[i].nd_ptr;
		(nd+knd)->q |= ND_UNPROC;
	}
	//�����ڵĿ�����Ϊδ����
	for (i=0; i<lv_cb; i++)
	{
		knd = islandlist[old_island].island_cblist[i].cb_ptr;
		(cb+knd)->q |= CB_N_RESTORE;
	}
	
	knd = islandlist[old_island].island_unlist[un_cb].nd;//ȡԭʼ�����翪��
	pcb_temp = islandlist[old_island].island_unlist[un_cb].cbptr;

//���뿪���Լ�������ϣ�׼����������ؿ�ʼ�����Լ�·����׷��
//////////////////////////////////////////////////////////////////////////
	ndCount = cbCount = cpCount = secCount = unCount = ldCount = 0;
	ind = knd;//ĸ�ߵ���ʼ�ڵ���

	if (new_island)
	{
		island = new_island;
	}
	else
		island = old_island;

	//��ԭʼ�����翪����Ϊ�µĵ��ĵ�Դ�������������
	//�µĵ������翪���������Դ�㣬�����ÿ������ֻ��һ�����翪�ؾͿ�����������״
	islandlist[island].island_unlist[unCount].nd = islandlist[old_island].island_unlist[un_cb].nd;
	strcpy(islandlist[island].island_unlist[unCount].secid , islandlist[old_island].island_unlist[un_cb].secid);
	islandlist[island].island_unlist[unCount].pq_abund = islandlist[old_island].island_unlist[un_cb].pq_abund;
	islandlist[island].island_unlist[unCount].v_abund = islandlist[old_island].island_unlist[un_cb].v_abund;
	islandlist[island].island_unlist[unCount].cbptr = islandlist[old_island].island_unlist[un_cb].cbptr;
	//islandlist[island].island_unlist[unCount].main_nd = islandlist[old_island].island_unlist[un_cb].main_nd;

	unCount++;

	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd+ind)->q &= (~ND_UNPROC);
		islandlist[island].island_ndlist[ndCount++].nd_ptr = ind;

		//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8

		//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
		(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
		while (1)//������չ����û�д���
		{
			//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
			if ((nd+knd)->q & ND_LD) 
			{
				//�õ�����id
				ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
				ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
				while(ld_p)
				{
					islandlist[island].island_ldlist[ldCount].ld_ptr = ld_p->Ptr;
					islandlist[island].island_ldlist[ldCount].p_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
					islandlist[island].island_ldlist[ldCount].q_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
					pq_abund = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
					pq_abund += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
					pq_abund = sqrt(pq_abund);
					islandlist[island].island_ldlist[ldCount].pq_ld = pq_abund;	
					
					//����˫��Դ����
					ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
					if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
						//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//���Դ
					{
						islandlist[island].island_ldlist[ldCount].soSum = 2;
					}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
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
				//��cp����ȫѭ������Ϊcp����������
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
			//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
			cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
			cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
			while(cb_p)
			{
				if (pcb_temp == cb_p->Ptr)//���������翪���Լ�
				{
					cb_p = cb_p->Next;
					continue;
				}

				//��������翪�أ�ͣ�������ҵ����������ĵ�Դ��
				//��� �Ǵ������������翪�ز�����
				if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				{
					//�����������Ϊ�Ѵ���
					dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);

					//����ֻ��һ����λ�Ŀ����ǲ����жϵģ�����û���������·����
					if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
					{
						//kkndcb,��Ϊ����ڵ㣬�ҵ�һ�����翪��
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
						//�ж����翪�ص�ԣ��
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
						
						//��Ȼ�����翪�أ���û��������·�����翪�������յ�
						if (fdid_abund != "")
						{
							bFind = false;
							//������翪�ص�ĸ�ߺ���ǰ�����翪��ĸ��һ��
							for (i=0;i<unCount;i++)
							{
								//�Ƚ����翪��ԣ�ȣ�ѡ����ԣ��
								pcb_temp = islandlist[island].island_unlist[i].nd;
								if ( (nd+pcb_temp)->ibs == (nd+knd)->ibs )
								{
									//�滻
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

				//ֻҪ���������Ŀ��ض���������,�Ѿ�������Ĳ�����
				if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_RESTORE) //this so nd has not processed
				{
					//printf("%s,%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb);
					//ֻ�����������ϵĿ��أ�CB_OPANABL
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb)
					{
						islandlist[island].island_cblist[cbCount].cb_ptr = cb_p->Ptr;

						//����Ƿ�λ���أ������ķ�Ϊ���أ���ͣ����
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

					//ֻ�����������ϵĿ��أ�CB_OPANABL
					//�����������Ϊ�Ѵ���
					dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);
					//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);

				}

				//find the opposite nd through this cb
				//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
				if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}
				//process this opposite nd
				//����Բ�ڵ� δ����==���ز����������ƽ�
				if((nd+kknd)->q & ND_UNPROC)
				{
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd+kknd)->fch = (nd+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd+knd)->fch = kknd;
					//���ҵ����½ڵ�����Ѵ���
					(nd+kknd)->q &= (~ND_UNPROC);
					islandlist[island].island_ndlist[ndCount++].nd_ptr = kknd;
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this ndͨ���߶�������չ
			sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
				//�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
				r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
				
				if (r_zn == -1)//���߶β������κ�����
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

					//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
					//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
					//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
					
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd+kknd)->fch = (nd+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd+knd)->fch = kknd;
					//���ҵ����½ڵ�����Ѵ���
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
			//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
			//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
			//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
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

//�����͵�ѹ�������Ҫ���㳱��
int CNM1::voltageAnanyse()
{
	int i,j,ret;
	NM1_Minpath minpath;
	int unCB1,unCB2;
	int loop[2];
	unfd_idList[0]='\0';

	//�𵺽���У��
	i = 0;
	while (1)
	{
		if (i>=g_island)
			break;

		if (islandlist[i].validate)//ֻ����û��У����ĵ�
		{
			i++;
			continue;
		}

		//���������������ת����û�е�Դ�����������ֱ�ӷ�����һ����
		if (!islandlist[i].canTrans)
		{
			i++;
			continue;
		}

		//����ֻ��һ����Դ��
		if (islandlist[i].unCount == 1)
		{
			if (!islandTreeFlow(i))//����У�鲻��ͨ����ԣ�Ȼ�������ѹ����
			{
				islandlist[i].canTrans = 0;//����������ת��
				islandlist[i].validate = 0;

				i++;
				continue;
			}
			else
			{//��ԣ���������翪��ת��
				islandlist[i].validate = 1;
				strcat(unfd_idList,islandlist[i].island_unlist[0].secid);
			}
		}
		else//�����ж����Դ��
		{
			//ѡһ������ԣ�ȵ�Դ����
			//ȡ����������ΪУ������翪���Ѿ�����
			//���������ԣ�ȵ����翪���Ƿ����ת�����еĸ���
			
			//���ԣ���Ƿ����ת�����ڵĸ���
			if (!islandTreeFlow(i))//����У�鲻��ͨ�����������ĵ�Դ�㲻��ת�����еĸ���
			{
				//����������Դ�㣬���������Դ�㻹����֧�ţ���Ҫ��ȡ��İ취������
					//��Ҫ�����ķ��ѣ���һ�����ټ�һ����һ��һ���ļ�
					//��һ�������������
//////////////////////////////////////////////////////////////////////////
//ͨ������֧·���н⻷
				//i��ԭʼ����ţ��п�����������ѳɼ�����
				//ͨ�����ѭ����֧·�����������еĻ����⿪
				j = 1;//��0����ԣ�����������
				while (1)
				{//�𲽼������翪��
					if (j > islandlist[i].unCount)
					{//���еĵ�Դ�㶼�����ˣ����ܼ���У��ɹ�
						islandlist[i].canTrans = 0;
						islandlist[i].validate = 1;
					
						i++;
						break;
					}
					//����һ�����翪�ر�Ȼ�γ�һ������
					//�����ټ���һ�����翪�غ��γɻ������������翪��
					//�������������ˣ����ִ��ڻ��������翪�أ�ÿ�μ�һ������
					ret = getLoopunCB(i,unCB1,unCB2);
					
					if (ret == 1)//ֻ������ֻ��һ��������ʱ��ſ��Խ⻷
					{
						if (!getMiminalPath(i, unCB1,unCB2,minpath))
						{
							j++;
							continue;
						}

						loop[0] = unCB1;
						loop[1] = unCB2;

						//����Ҫ���㻷���ĳ�����ֱ����ԭ�еķ��������оݾͿ�����
						//�����⿪���ж��Ƿ��е͵�ѹ
						//����⻷��ĵ�ѹ���
						double retPQ=0;
//						ret = setBreakCB(i, minpath,loop,retPQ);//�趨�⻷�Ŀ��أ������µ�
						//˳�����翪��unCB1,unCB2��������

						if (ret > 0)

						j++;
					}
				}
//���ѽ⿪����ԭ���Ļ�����������һ���µĵ�
//////////////////////////////////////////////////////////////////////////

			}
			else
			{//�õ���һ����Դ��Ϳ���ת��������Ҫ�������ĵ�Դ��ת����һ����
				islandlist[i].validate = 1;
				//strcat(unfd_idList,islandlist[i].island_unlist[0].secid);
				i++;
			}
		}
	}

	return 1;
}



//�����ʱ����Ҫ�Ƚ������˷������õ�ĸ�߱��
int CNM1::islandTPAnalyse(int island,int so_nd)
{
	int i,j;
	int knd,kknd,globalbs;

	ND* nd = dsbob2->nd_tb->GetTable();
	int lv_nd = islandlist[island].ndCount;
	CB* cb = dsbob2->cb_tb->GetTable();
	int lv_cb = islandlist[island].cbCount;//���翪�ز����㵺�ڵĿ���

	ChildPtr1	*cb_p, *cb_p0;

//////////////////////////////////////////////////////////////////////////
//���ڵ�ת��Ϊ����ĸ��	
//�����ڵĽڵ���Ϊδ����
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[island].island_ndlist[i].nd_ptr;
		(nd+knd)->q |= ND_UNPROC;
	}

	//�õ�Դ��������˷�����ֱ�����еĵ�ȫ������
	//��Դ��Ľڵ��
	knd = so_nd;
	globalbs = 1;//�ӵ�Դ�㿪ʼ��������

	//�Ե��ڵ�ÿ���ڵ����ѭ��
	for(i = 0; i < lv_nd; i++)
	{
		//�ѽڵ�ȡ����
		j = islandlist[island].island_ndlist[i].nd_ptr;
		if((nd+j)->q & ND_UNPROC)  //this nd has not processed
		{
			//get bs number for this nd
			//ibs = global->nextbs;//�����Ǹ�����ĸ�ߺţ�ÿ���һ����������1
	
			//create circular chain for this nd
			knd = j;
			(nd+knd)->fch = knd;//�趨ǰ����
			(nd+knd)->q &= (~ND_UNPROC);//���ýڵ�����Ѵ���
			(nd+knd)->primebs = globalbs;//������ڵ������ڵļ���ĸ�ߺ����뵽�ڵ�����

			//����Ҫ��һ���ڵ�����չ����ѭ����һֱ���Ҳ���������չ�Ľڵ�Ϊֹ
			while( 1 )
			{
				//ͨ���ڵ�-���ع�����������ڵ�������Ŀ���ȡ�����ŵ�ϵ����
				//�ر�������������ĸ��ʱ���кܶ�Ŀ���Ҫ�ӵ�����ϵ����
				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//׼����������ؽ�����չ���Ȱ�������Ǹ�������ʱ������
				while(cb_p)//cb_p��������ڵ���������еĿ��ص�ָ�뼯��
				{
					//ֻ�п����Ǳպϵ�״̬��������չ
					if (!((cb+cb_p->Ptr)->q & CB_NMLOPEN))  //cb is closed
					{
						//find the opposte nd through this cb
						//�ҵ��Բ�ڵ㲢������ڵ��ŵ�kknd
						if(knd == (cb+cb_p->Ptr)->ind)
						{
							kknd = (cb+cb_p->Ptr)->iznd;
						}
						else
						{
							kknd = (cb+cb_p->Ptr)->ind;
						}

						//process this opposite nd
						//����Բ�ڵ�û�д������Բ�ڵ���Ϊ�Ѵ�����ǰ�����ͺ�����
						//���ڵ�ϲ���һ������ĸ�ߣ�����ͬһ������ĸ�ߺ�д��Բ�Ľڵ����ĸ�ߺ�
						if((nd+kknd)->q & ND_UNPROC)
						{
							(nd+kknd)->fch = (nd+knd)->fch;
							(nd+knd)->fch = kknd;
							(nd+kknd)->q &= (~ND_UNPROC);
							(nd+kknd)->primebs = globalbs;//�ҵ��Բ�ڵ�󣬻��Ǹ��ڵ㸳һ����ͬ��ĸ�ߺ�

						}
					}
					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);//�ͷ�ָ��

				//get next nd in the present nd group
				knd = (nd+knd)->fch;//��һ���ڵ����ǰ���ڵ㣬ǰ����Ǹ�
				if(knd == j) break;
			}
			globalbs++;//��ʱĸ�߱�ŵõ�
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



//����������Դ��ĵ��ĳ���,˫�˹��绷��
//��������һ����Դ�㣬���˲���
int CNM1::islandLoopFlow(int island,int* loop)
{
//	int j;
//	int knd,kknd,globalbs;
//	double p_abund,q_abund,v_abund;

	islandTPAnalyse(island,loop[0]);

	//׼���������㣬��Ҫ׼����Դ�㡢���ɵ㡢�������㡢֧·������
	//1.��ʼ����������
	//
	// lld 2019-9-25
	//InitWorkingArray(island);
	
	//2.�õ�׼���������������
	SetNetworkData(island,loop);

	//3.ţ�ٷ���������
	OrderBusNumberB();
	g_method=1;
	ExecuteNewton();

//	//4.�������ж�
//	for(j = 0; j < lv_wb; j++)
//	{
//		if(g_wb[j].tio >= 0)  
//		{
//			if ( g_wb[island].v < 0.93 )//�����˵͵�ѹ
//				return 0;
//		}					
//	}

	return 1;
}

int CNM1::getbs(int island,int nd)
{
//���ַ�
	int high,middle,low;

	int ndCount = islandlist[island].ndCount;

	high = ndCount - 1;
	low = 0;
    while(low <= high)
	{
        middle = (low + high) / 2;
		
		if (nd == islandlist[island].island_ndlist[middle].nd_ptr)
		{  //�ҵ�,�����±�����ֵ
			return islandlist[island].island_ndlist[middle].primebs;
		}
		else if(nd < islandlist[island].island_ndlist[middle].nd_ptr)
		{  //����ֵ�ڵͰ���
            high = middle - 1;
        }
		else
		{  //����ֵ�ڸ߰���
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

//����data��ֵ��С���󽫽ṹ������,���ڽṹ���ڵ�����ؼ�����data�����Ϳ��Ժܶ��֣��ο����������д 
int struct_cmp( const void *a ,const void *b) 
{ 
	return (*(NM1_UNLIST *)a).pq_abund < (*(NM1_UNLIST *)b).pq_abund ? 1 : -1; 
} 

int CNM1::sort_unlist(NM1_UNLIST* unlist,int cnt)
{
	qsort(unlist,cnt,sizeof(unlist[0]),struct_cmp); 

	return 1;
}

//��ÿ�����ڵ����翪�����������
int CNM1::trim_unlist(int island)
{
	sort_unlist(islandlist[island].island_unlist,islandlist[island].unCount);
	return 1;
}

//������Բ��������ڵ�ȫ���ڵ㣬�����Ժ���ж��ַ�����
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
	//��Դ��
	if (loop!=NULL)
	{
		for (i=0; i<2; i++)
		{
			loopun = loop[i];
			nd = islandlist[island].island_unlist[loopun].nd;
			g_wg[i].logic = islandlist[island].island_unlist[loopun].cbptr;
			g_wg[i].ibs = getbs(island,nd);
			g_wg[i].slackavr = 0; //�������ɳڽڵ�
    
			//������ĵ�Դ���ڵ����ƽ��ڵ㣬Ĭ����10.5
			g_wg[i].v = islandlist[island].island_unlist[i].v_abund;
			
			//��Դ����׶��й����޹�����Ҫ���ڸ��ɵ�Ĵ���
			g_wg[i].w = islandlist[island].island_unlist[i].p_abund / g_wbase;	//�й�
			g_wg[i].r = islandlist[island].island_unlist[i].q_abund / g_wbase;	//�޹�
		}
	}
	else
	{
		nd = islandlist[island].island_unlist[0].nd;
		g_wg[0].logic = islandlist[island].island_unlist[0].cbptr;
		g_wg[0].ibs = getbs(island,nd);
		g_wg[0].slackavr = 0; //�������ɳڽڵ�
    
		//������ĵ�Դ���ڵ����ƽ��ڵ㣬Ĭ����10.5
		g_wg[0].v = islandlist[island].island_unlist[0].v_abund;
		
		//��Դ����׶��й����޹�����Ҫ���ڸ��ɵ�Ĵ���
		g_wg[0].w = islandlist[island].island_unlist[0].p_abund / g_wbase;	//�й�
		g_wg[0].r = islandlist[island].island_unlist[0].q_abund / g_wbase;	//�޹�
	}
				
//////////////////////////////////////////////////////////////////////////
	//���ɵ�,��ÿ�����ɵ�����ݶ�������				
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
	//������
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
	//֧·
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




//�жϸ���һ�����翪�ؿ�ʼ�������Ƿ�����ѵ����翪��
int CNM1::exist_unCB(int island,int unCB1)
{
	int ret=0;//�ڵ�����Ҳ�ҵ����������翪�غ���γɻ��������

//	int cbCount,secCount;//��С·�Ͽ��ص��������߶ε�����
	double loopR=-1;

	int knd,lv_nd,un_nd;
	int			i;
	int			ind, kknd,r_zn;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p0,*sec_p;
	
	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = islandlist[island].ndCount;
	//�����ڵĽڵ���Ϊδ����
	for (i=0; i<lv_nd; i++)
	{
		knd = islandlist[island].island_ndlist[i].nd_ptr;
		(nd+knd)->q |= ND_UNPROC;
	}

	//��һ�����翪�ش���ʼ����
	ind = un_nd = islandlist[island].island_unlist[unCB1].nd;
	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
	{
		(nd+ind)->q &= (~ND_UNPROC);

		//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
		(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
   		while (1)//������չ����û�д���
		{
			//read cbs connected to this nd
			//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
			cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
			cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
			while(cb_p)
			{
				if ( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd == un_nd)  
					|| (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind == un_nd) ) 
				{
					cb_p = cb_p->Next;
					continue;//�������Լ�
				}

				//��������翪�ػ�Ͽ��ķֶο��أ�ͣ�������ҵ����������ĵ�Դ��
				if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				{
					//ͨ���ڵ�ŵõ����翪�ص�λ��
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

				//��������翪�ػ�Ͽ��ķֶο��أ�ͣ����
				if( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
				  ||(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN) )		
				{
					//�����������Ϊ�Ѵ���
					dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);

					cb_p = cb_p->Next;
					continue;
				}

				
				//find the opposite nd through this cb
				//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
				if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}
				//process this opposite nd
				//����Բ�ڵ� δ����==���ز����������ƽ�
				if((nd+kknd)->q & ND_UNPROC)
				{
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd+kknd)->fch = (nd+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd+knd)->fch = kknd;

					//������ڵ����һ����¼������׼������
					(nd+kknd)->bch = knd;

					//���ҵ����½ڵ�����Ѵ���
					(nd+kknd)->q &= (~ND_UNPROC);
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			if (ret != -1)
				return ret;

			//read secs connected to this ndͨ���߶�������չ
			sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				//�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
				//�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
            	r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		        
				if (r_zn == -1)//���߶β������κ�����
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
					//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
					//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
					//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
					
					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd+kknd)->fch = (nd+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd+knd)->fch = kknd;
					//������ڵ����һ����¼������׼������
					(nd+kknd)->bch = knd;
					//���ҵ����½ڵ�����Ѵ���
					(nd+kknd)->q &= (~ND_UNPROC);
				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd
			knd = (nd+knd)->fch;
			//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
			//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
			//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
			if(knd == ind)
			{
				break;
			}
		}
	}

	return ret;
}

//��ÿ�����翪�س�����ʼ����������ֻ��һ��������
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


//ֱ����eq�������򻯴���
int CNM1::getResidueStruct(int noextbs,int* pextbs,int* pupextbs)
{//int degbug=1;
	int ndCount,ldCount,cbCount,secCount,cpCount,unCount,soCount;//һ�����ڵ�����
	double p_abund,q_abund,v_abund,pq_abund;//�����й����޹�����ѹԣ��
	char nd_abund[MAX_SEC_ID_LEN];//���翪�صĶԲ�ڵ�
	char fdid_abund[MAX_SEC_ID_LEN];//���翪�ضԲ�����߱��
	bool bFind;

	int knd,lv_nd,lv_cb,lv_cp,lv_so;
	char fdid[MAX_SEC_ID_LEN];//��������������ID��������ǰ׺
	int			i,ii,j;
	int			ind, kknd,ld_nd,cb_id,r_zn,pcb_temp;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1   *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;
	char scb_id[MAX_SEC_ID_LEN];//���翪�ضԲ������id����

	ChildPtr1	*sec_p;//���ؿ�������2�����ߣ������ÿ��ص�fdid������ֻ�������߶�
	
	//û�п���ת���Ŀ���
	if (noextbs == 0)
	{
		return 0;
	}

	knd = pupextbs[0];//ͨ���ڵ���ҵ�Դ��
	ND* nd = dsbob2->nd_tb->GetTable();
	lv_nd = dsbob2->nd_tb->GetCount();
	
//	sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//	sec_p0 = sec_p;//����ָ�����ʱ���棬ָ�������͵�����
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
	//�����еĽڵ���Ϊδ����׼�����˷�����
	//�˴��ƺ������Ż������û����ô��Ŀռ䣬����ֻ����صĽڵ�
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->q |= ND_UNPROC;
	}

	//��������Ϊ�Ѵ���
	for(i = 0; i < lv_cb; i++)
	{
		(cb+i)->q |= CB_N_RESTORE;
	}

    //���δ������εı߽翪��
	//ÿһ�����ض��Ƕ����Ĺ��磬�����֧֮������������ת��
	g_island = 0;
	for(ii = 0; ii < noextbs; ii++)
	{
		knd = pupextbs[ii];//ȡ���ص����νڵ�����ж�

		cb_id = -1;
//////////////////////////////////////////////////////////////////////////
//������Ŀ����Լ��ҵ������д������������ͷ�λ��ֱ������
		//����߽翪�ر����ʹ��ڷ�λ��ֱ������
		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
		cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
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

			if (kknd == pextbs[ii])//�߽翪��,�����Լ��ҵ�
			{
				cb_id = cb_p->Ptr;
				break;
			}
			cb_p = cb_p->Next;
		}
		::FreeChildPtr(cb_p0);

		if (cb_id != -1)//һ�㲻������Ҳ����Լ������
		{
			dsbob2->cb_tb->GetDataByPh(cb_id)->q &= (~ND_UNPROC);
			//�������Ҫ�ӵ��Ͽ��Ŀ������棬����������ͨ����
			//firstCB[ii] = cb_id;//�������Ŀ�����Ϊ����Ͽ��Ŀ���
			//����������ڱ���Ҫ�Ͽ��Ŀ��أ���������ʱ����Բ���Ҫ�ӵ��ֶ�����
			//ֻ�м��뵽�ṹ����ķֶο��ز�������

			//����һ�����������翪�أ�����ڿ���������Ƚ��ձ�
			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & Q_UNIONCB)  
			{
				continue;
			}
			
			//�����Ŀ����Լ��Ƿ�λ���أ�ͣ����
			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_OPEN)
			{
				continue;
			}
		}

//���뿪���Լ�������ϣ�׼����������ؿ�ʼ�����Լ�·����׷��
//////////////////////////////////////////////////////////////////////////
		ndCount = cbCount = cpCount = secCount = unCount = ldCount = soCount = 0;
		ind = pextbs[ii];//ĸ�ߵ���ʼ�ڵ���

//����һ���ڵ���Ϊ�Ѵ���
		(nd+pupextbs[ii])->q &= (~ND_UNPROC);
//		islandlist[g_island].island_ndlist[ndCount++].nd_ptr = pupextbs[ii];

		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
		{
			(nd+ind)->q &= (~ND_UNPROC);
			islandlist[g_island].island_ndlist[ndCount++].nd_ptr = ind;

			//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8

			//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
			(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
     		while (1)//������չ����û�д���
			{
				//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
				if ((nd+knd)->q & ND_LD) 
				{
					//�õ�����id�����ɺ���Ҫ���������еĶ�Ҫ��������Ե����������д���
					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
					ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
					while(ld_p)
					{
						islandlist[g_island].island_ldlist[ldCount].ld_ptr = ld_p->Ptr;
						islandlist[g_island].island_ldlist[ldCount].p_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
						islandlist[g_island].island_ldlist[ldCount].q_ld = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
						pq_abund = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
						pq_abund += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r * dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
						pq_abund = sqrt(pq_abund);
						islandlist[g_island].island_ldlist[ldCount].pq_ld = pq_abund;	
						
						//����˫��Դ����
						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//���Դ
						{
							islandlist[g_island].island_ldlist[ldCount].soSum = 2;
						}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
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
					//��so����ȫѭ������Ϊso����������,�����Ƿֲ�ʽ��Դ
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
					//��cp����ȫѭ������Ϊcp����������
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
				//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
				while(cb_p)
				{
					//��������翪�أ�ͣ�������ҵ����������ĵ�Դ��
					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
					{
						//�����������Ϊ�Ѵ���
						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);

						//����ֻ��һ����λ�Ŀ����ǲ����жϵģ�����û���������·����
						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
						{
							//kkndcb,��Ϊ����ڵ㣬�ҵ�һ�����翪��
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
							//�ж����翪�ص�ԣ��
							p_abund=q_abund=v_abund=0;
 							getUNCB_abun(fdid,scb_id,nd_abund, \
										fdid_abund,p_abund,q_abund,v_abund);
							if ( (p_abund==0) && (q_abund==0) && (v_abund==0) )
							{
								pq_abund = 5;//Ĭ����������ԣ��
								v_abund = 1.05;
							}
							else
								pq_abund = sqrt(p_abund*p_abund + q_abund*q_abund);
							
							//��Ȼ�����翪�أ���û��������·�����翪�������յ�
							if (fdid_abund != "")
							{
								bFind = false;
								//������翪�ص�ĸ�ߺ���ǰ�����翪��ĸ��һ��
								for (i=0;i<unCount;i++)
								{
									//�Ƚ����翪��ԣ�ȣ�ѡ����ԣ��
									pcb_temp = islandlist[g_island].island_unlist[i].nd;
									if ( (nd+pcb_temp)->ibs == (nd+knd)->ibs )
									{
										//�滻
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

					//ֻҪ���������Ŀ��ض���������,�Ѿ�������Ĳ�����
					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_RESTORE) //this so nd has not processed
					{
						//printf("%s,%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb);
						//����Ƿ�λ���أ������ķ�Ϊ���أ���ҲҪ�������翪�ش���������ԣ��Ϊ���޴�

						//�����������Ϊ�Ѵ���
						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);

						//����ֻ��һ����λ�Ŀ����ǲ����жϵģ�����û���������·����
						//!!!!!!!�ڲ����翪��
						if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)
						{
							//kkndcb,��Ϊ����ڵ㣬�ҵ�һ�����翪��
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
							//�ж����翪�ص�ԣ��
							p_abund = q_abund = pq_abund = 20;
							
							//��Ȼ�����翪�أ���û��������·�����翪�������յ�
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
							//�������ǰ�ڷ�����ʱ���趨Ϊ�⻷���أ�����Ҫ����ָ�
							if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_ISOLATE)
								dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_ISOLATE);							

							islandlist[g_island].island_cblist[cbCount].cb_ptr = cb_p->Ptr;
							cbCount++;
						}

						//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);

					}

					//find the opposite nd through this cb
					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}
					//process this opposite nd
					//����Բ�ڵ� δ����==���ز����������ƽ�
					if((nd+kknd)->q & ND_UNPROC)
					{
						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
						(nd+kknd)->fch = (nd+knd)->fch;
						//�½ڵ� ��� ��չ�ڵ��ǰ����
						(nd+knd)->fch = kknd;
						//���ҵ����½ڵ�����Ѵ���
						(nd+kknd)->q &= (~ND_UNPROC);
						islandlist[g_island].island_ndlist[ndCount++].nd_ptr = kknd;
					}

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//read secs connected to this ndͨ���߶�������չ
				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
				sec_p0 = sec_p;
				while(sec_p)
				{
				    //�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
				    //�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		            
					if (r_zn == -1)//���߶β������κ�����
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
						//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
						//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
						//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
						
						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
						(nd+kknd)->fch = (nd+knd)->fch;
						//�½ڵ� ��� ��չ�ڵ��ǰ����
						(nd+knd)->fch = kknd;
						//���ҵ����½ڵ�����Ѵ���
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
				//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
				//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
				//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
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
					islandlist[g_island].canTrans = 1;//�ȼ�����ת

					g_island++;
					break;
				}
			}
		}
	}
	
	return 1;
}




//////////////////////////////////////////////////////////////////////////



//���������Դ����������������ҵ���صĵ�
//unSum���翪��������,pun���翪�ر�ţ�pun_���翪�ضԲ���,unRebund���翪�ص�ԣ��
//cbSum����·����ȫ���ĶϿ��ķֶο�����,pcb�ֶο��صı��,���ذ�����ǰ�����ν��뿪��
//��Ҫ���븺�ɵ�ĸ�ߺ�����
//���ֻ����͸��ɲ��������������ת��
//0�ɹ�ת�� -1ϵͳ������-2����, -3ԣ�Ȳ���, -4�и��ɹµ�
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
	double	w_next,r_next,deltaMultiP,deltaMultiQ;//���翪�ص������й����޹�
	int	cntLD;//���ɵ��ܸ�������Ҫ����ж��Ƿ���ת��
	
	ret = 1;
	CB* cb = dsbob2->cb_tb->GetTable();

	int lv_nd = dsbob2->nd_tb->GetCount();
	if(lv_nd == 0)
	{
		return 0;
	}
	ND* nd_t = new ND[lv_nd];
	dsbob2->nd_tb->CopyTable(nd_t);//�õ�һ���µĽڵ�������ԭ���ĳ�ͻ

	//�����е���ʱ�ӵ���Ϊ�Ѵ���
	for(i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

	//�����翪�ؿ�ʼ���������Ƿ��л��������û�л�������������翪�ص�ԣ���Ƿ�
	//���е����綼������󣬿����ɵĸ����Ƿ�ﵽ��Ҫת���ĸ���
	cntLD = 0;//ͨ�����翪���������ĸ��ɸ���
	tranSumld = 0;
	for(i = 0; i < unSum; i++)
	{
		unRebund0 = *(unRebund+i);//�����翪�ص�ԣ��
		un_nd  = knd = *(pun+i);//ȡ���翪�ص����νڵ��������
		un_znd = knd_ = *(pun_+i);//���翪�صĶԲ�
		
		//�����߽翪�ؿ�ʼ׷�ټ�����
		w_next=r_next=deltaMultiP=deltaMultiQ=0;

		ind = knd;//ĸ�ߵ���ʼ�ڵ���
//����һ���ڵ���Ϊ�Ѵ���
		(nd_t + knd_)->q &= (~ND_UNPROC);

		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
		if((nd_t+ind)->q & ND_UNPROC) //this so nd has not processed
		{
			(nd_t+ind)->q &= (~ND_UNPROC);
			//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8

			//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
			(nd_t+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
     		while (1)//������չ����û�д���
			{
				//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
				if ((nd_t+knd)->q & ND_LD) 
				{
					//�õ�����id
					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd_t+knd)->r_ld);
					ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
					while(ld_p)
					{
						//ֱ���޸ĸ��ɵ�ID��
						w_next += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
						r_next += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;

						//����˫��Դ����
						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//���Դ
						{
							deltaMultiP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);
							deltaMultiQ += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r);
						}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�

						cntLD++;
						ld_p = ld_p->Next;
					}
					::FreeChildPtr(ld_p0);
				}

				//read cbs connected to this nd
				//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd_t+knd)->r_cb);
				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
				while(cb_p)
				{
					//��������翪�أ����ֻ���
					//��Ҫ���Ƿ������Լ�
					if ( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_NMLOPEN)  
						&& !( 
						  ((dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind == un_nd) 
						  && (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd == un_znd))
						     ||((dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd == un_nd) 
							 && (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind == un_znd))
						   )
					   )
					{
						//�����˻�����ֱ���˳�
						delete []nd_t;
						nd_t = 0;
						lv_nd = 0;
						::FreeChildPtr(cb_p0);

						return -2;
					}

					//����Ƿ�λ���أ�ͣ����
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
					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}
					//process this opposite nd
					//����Բ�ڵ� δ����==���ز����������ƽ�
					if((nd_t+kknd)->q & ND_UNPROC)
					{
						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
						(nd_t+kknd)->fch = (nd_t+knd)->fch;
						//�½ڵ� ��� ��չ�ڵ��ǰ����
						(nd_t+knd)->fch = kknd;
						//���ҵ����½ڵ�����Ѵ���
						(nd_t+kknd)->q &= (~ND_UNPROC);
					}

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//read secs connected to this ndͨ���߶�������չ
				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd_t+knd)->r_sec);
				sec_p0 = sec_p;
				while(sec_p)
				{
				    //�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
				    //�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
		            
					if (r_zn == -1)//���߶β������κ�����
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
						//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
						//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
						//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
						
						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
						(nd_t+kknd)->fch = (nd_t+knd)->fch;
						//�½ڵ� ��� ��չ�ڵ��ǰ����
						(nd_t+knd)->fch = kknd;
						//���ҵ����½ڵ�����Ѵ���
						(nd_t+kknd)->q &= (~ND_UNPROC);
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);

				//move to next nd
				knd = (nd_t+knd)->fch;
				//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
				//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
				//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
				if(knd == ind)
				{
					sumP_T = sqrt(w_next*w_next+r_next*r_next);
					if (unRebund0 < sumP_T)
					{
						bPassT = false;
					}
					else
					{
						//����ת�������ۼ�
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
////�����ε��׶˿�ʼ�����Ҫ������ĸ�ߵ���������֤���������Ѿ����������ε��Ǹ��ڵ��ų���
////noextbs���߽翪�������������������ε��Ǹ��ڵ㣩
////extbs�߽翪�ص����������ĸ�ߣ�upextbs�����ڶԲ�ļ���ĸ��
//int CNM1::OnSearchUnion(int noextbs,int* pextbs,int* pupextbs, \
//						   double& deltap,double& deltaq,int& deltaC, \
//						   double& sumP,double& sumQ,int& sumC, \
//						   double& lossP,double& lossQ,int& lossC, \
//						   double& deltaMultiP,double& deltaMultiQ,int& deltaMultiC, \
//						   * unfd_idList)
//{
//	//û�п���ת���Ŀ���
//	if (noextbs == 0)
//	{
//		return 0;
//	}
//
//	int knd = pupextbs[0];//ͨ���ڵ���ҵ�Դ��
//	ChildPtr1	*sec_p;//���ؿ�������2�����ߣ������ÿ��ص�fdid������ֻ�������߶�
//	ND* nd = dsbob2->nd_tb->GetTable();
//	int lv_nd = dsbob2->nd_tb->GetCount();
//	 fdid[80];//��������������ID��������ǰ׺
//	sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//	if (!sec_p)
//		return 0;
//	strcpy(fdid,dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->fdid);
//	if (strlen(fdid)==0)
//		return 0;
//
//
//	int			i,ii,j,k,ret,cnt_unioncb/*�����߽翪������ʱ�ҵ������翪����*/,sum_unioncb/*�ܵ����翪����*/;
//	int			ind, kknd,ld_nd,cb_id,r_zn,pcb_temp,cnt_ld/*���ɵ��ܸ���*/;
//	ChildPtr1	*cb_p, *cb_p0;
//	ChildPtr1   *sec_p0;
//	ChildPtr1	*ld_p, *ld_p0;
//	double w_next,r_next,c_next;//�����߽翪�ص����ε����й����޹����û���
//	 unfd_id[80];//���翪�ضԲ������id����
//	unfd_idList[0]='\0';//���Բ������������գ�׼�����
////ȫ��
//	int* unCB = new int[20];//���翪�ص��׶˽ڵ㣬���20�����翪��
//	int* unCB_ = new int[20];//���翪�صĶԲ�ڵ㣨������һ���ߣ������20�����翪��
//	double* unRebund = new double[20];//���翪�ص�ԣ��
//	int* openCB = new int[50];//���50���Ͽ��ķֶο��أ������ص������ţ�ָ�룩
//
//	int cntMainCB;//����Ŀ�������
//	int* mainCB = new int[200];//���100������ 
//	int* firstCB = new int[20];//�׶˿��� 
////��������·��,����·��ֻ��һ���׶˿���
//	int* l_unCB = new int[20];//���翪�ص��׶˽ڵ㣬���20�����翪��
//	int* l_unCB_ = new int[20];//���翪�صĶԲ�ڵ㣨������һ���ߣ������20�����翪��
//	double* l_unRebund = new double[20];//���翪�ص�ԣ��
//	int* l_openCB = new int[50];//���50���Ͽ��ķֶο��أ������ص������ţ�ָ�룩
//
//	int l_cntMainCB;//����Ŀ�������
//	int* l_mainCB = new int[200];//���100������ 
//
//	bool bTranPass=true;//����ȫ��ת����ֻ�����е�·����ͨ��������ֱ��pass
//	double tranSumLoad=0;//���Բ���ת�����ܸ���
//	double tranSumld;//���������еõ���ת������
//	
//	CStringList lCB;
//	//** lCB;
//	CString sProb;
//	int cntProb;//�õ���������
//
//	 scb_id[80];
//	
//	double unionAbund,unionAund_,mxAbund,secondAbund,sumAbund;//���翪�ص�ԣ�ȣ�ȫ��·��
//	
//	CB* cb = dsbob2->cb_tb->GetTable();
//	int lv_cb = dsbob2->cb_tb->GetCount();
//
//	//mark all nds as unprocessed
//	//�����еĽڵ���Ϊδ����׼�����˷�����
//	//�˴��ƺ������Ż������û����ô��Ŀռ䣬����ֻ����صĽڵ�
//	for(i = 0; i < lv_nd; i++)
//	{
//		(nd+i)->q |= ND_UNPROC;
//	}
//
//	//��������Ϊ�Ѵ���
//	for(i = 0; i < lv_cb; i++)
//	{
//		(cb+i)->q |= ND_UNPROC;
//	}
//
//    //���δ������εı߽翪��
//	//ÿһ�����ض��Ƕ����Ĺ��磬�����֧֮������������ת��
//	//��1����һ������һ�����صĴ���
//	//      1.�ڴ�����·��ʱ��û�����磬��������1
//	//		2.��һ�����翪�أ���ԣ�Ȳ�������������2
//	//		3.�����ж�����翪�أ���Ҫ���ö����紦�����ת�����ˣ���������3
//	//��2��ÿ�����ؾ�������ϣ����ִ������⣬ȫ�����ö�����ģ������ع�
//	
//	sum_unioncb = 0;//ȫ�����翪�ؼ�����
//	cntMainCB = 0;
//	cnt_ld = 0;
//	sumAbund = mxAbund = secondAbund = 0;//���翪����ԣ���ó�ֵ
//	for(ii = 0; ii < noextbs; ii++)
//	{
//		bTranPass=true;//ÿ�����翪��Ĭ���ǿ���ͨ��ת����
//
//		knd = pupextbs[ii];//ȡ���ص����νڵ�����ж�
//
//		cb_id = -1;
////////////////////////////////////////////////////////////////////////////
////������Ŀ����Լ��ҵ������д������������ͷ�λ��ֱ������
//		//����߽翪�ر����ʹ��ڷ�λ��ֱ������
//		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//		cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
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
//			if (kknd == pextbs[ii])//�߽翪��,�����Լ��ҵ�
//			{
//				cb_id = cb_p->Ptr;
//				break;
//			}
//			cb_p = cb_p->Next;
//		}
//		::FreeChildPtr(cb_p0);
//
//		if (cb_id != -1)//һ�㲻������Ҳ����Լ������
//		{
//			dsbob2->cb_tb->GetDataByPh(cb_id)->q &= (~ND_UNPROC);
//			//�������Ҫ�ӵ��Ͽ��Ŀ������棬����������ͨ����
//			firstCB[ii] = cb_id;//�������Ŀ�����Ϊ����Ͽ��Ŀ���
//			
//			//����һ�����������翪�أ�����ڿ���������Ƚ��ձ�
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_NMLOPEN)  
//			{
//				continue;
//			}
//			
//			//�����Ŀ����Լ��Ƿ�λ���أ�ͣ����
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_OPEN)
//			{
//				continue;
//			}
//		}
//
////���뿪���Լ�������ϣ�׼����������ؿ�ʼ�����Լ�·����׷��
////////////////////////////////////////////////////////////////////////////
//
//		//�����߽翪�ؿ�ʼ׷�ټ�����
//		w_next=r_next=c_next=0;
//		cnt_unioncb = 0;//��ת����֧�ϵ����翪������
//		unionAund_ = 0;//��ת����֧�ϵ�����ԣ��
//		l_cntMainCB = 0;
//
//		ind = pextbs[ii];//ĸ�ߵ���ʼ�ڵ���
////����һ���ڵ���Ϊ�Ѵ���
//		(nd+pupextbs[ii])->q &= (~ND_UNPROC);
//
//		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
//		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
//		{
//			(nd+ind)->q &= (~ND_UNPROC);
//			//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
//
//			//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
//			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
//			(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
//     		while (1)//������չ����û�д���
//			{
//				//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
//				if ((nd+knd)->q & ND_LD) 
//				{
//					//�õ�����id
//					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
//					ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
//					while(ld_p)
//					{
//						//ֱ���޸ĸ��ɵ�ID��
//						w_next += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
//						r_next += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
//						c_next++;
//						cnt_ld++;//ȫ�ָ��ɸ���
//
//						//����˫��Դ����
//						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
//						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
//							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//���Դ
//						{
//							deltaMultiP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);
//							deltaMultiC++;
//						}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
//
//						ld_p = ld_p->Next;
//					}
//					::FreeChildPtr(ld_p0);
//				}
//
//				//read cbs connected to this nd
//				//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
//				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
//				while(cb_p)
//				{
//					//��������翪�أ�ͣ�������ҵ����������ĵ�Դ��
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_NMLOPEN)  
//					{
//						//�����������Ϊ�Ѵ���
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);
//
//						//����ֻ��һ����λ�Ŀ����ǲ����жϵģ�����û���������·����
//						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
//						{
//							//kkndcb,��Ϊ����ڵ�
//							//sum_unioncb++;
//
//							strcpy(scb_id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id);
//							//�ж����翪�ص�ԣ�ȣ�
// 							unionAbund = getUNCB_abun(fdid,unfd_id,scb_id);
//							//��Ȼ�����翪�أ���û��������·�����翪�������յ�
//							if (unfd_id != "")
//							{
//								strcat(unfd_idList,unfd_id);
//								strcat(unfd_idList,",");//���������id
//							}
//
//							l_unCB[cnt_unioncb] = knd;
//							l_unCB_[cnt_unioncb] = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;//���
//							l_unRebund[cnt_unioncb] = unionAbund;//���
//							unionAund_ += unionAbund;
//							cnt_unioncb++;
//						
//							unCB[sum_unioncb] = knd;
//							unCB_[sum_unioncb] = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;//���
//							unRebund[sum_unioncb] = unionAbund;//���
//							sumAbund += unionAbund;
//							sum_unioncb++;						
//
//						}						
//						cb_p = cb_p->Next;
//						continue;
//					}
//
//					//ֻҪ���������Ŀ��ض���������,�Ѿ�������Ĳ�����
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & ND_UNPROC) //this so nd has not processed
//					{
//						//printf("%s,%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb);
//						//ֻ�����������ϵĿ��أ�CB_OPANABL
//						if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb)
//						{
//							l_mainCB[l_cntMainCB++] = cb_p->Ptr;
//							mainCB[cntMainCB++] = cb_p->Ptr;
//						}
//
//						//ֻ�����������ϵĿ��أ�CB_OPANABL
//						//�����������Ϊ�Ѵ���
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);
//						//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);
//
//					}
//
//					//����Ƿ�λ���أ�ͣ����
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN)
//					{
//						cb_p = cb_p->Next;
//						continue;
//					}
//
//					//find the opposite nd through this cb
//					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
//					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//					}
//					//process this opposite nd
//					//����Բ�ڵ� δ����==���ز����������ƽ�
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//�½ڵ� ��� ��չ�ڵ��ǰ����
//						(nd+knd)->fch = kknd;
//						//���ҵ����½ڵ�����Ѵ���
//						(nd+kknd)->q &= (~ND_UNPROC);
//					}
//
//					cb_p = cb_p->Next;
//				}
//				::FreeChildPtr(cb_p0);
//
//				//read secs connected to this ndͨ���߶�������չ
//				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//				sec_p0 = sec_p;
//				while(sec_p)
//				{
//				    //�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
//				    //�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
//            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
//            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
//		            
//					if (r_zn == -1)//���߶β������κ�����
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
//						//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
//						//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
//						//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
//						
//						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//�½ڵ� ��� ��չ�ڵ��ǰ����
//						(nd+knd)->fch = kknd;
//						//���ҵ����½ڵ�����Ѵ���
//						(nd+kknd)->q &= (~ND_UNPROC);
//					}
//					sec_p = sec_p->Next;
//				}
//				::FreeChildPtr(sec_p0);
//
//				//move to next nd
//				knd = (nd+knd)->fch;
//				//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
//				//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
//				//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
//				if(knd == ind)
//				{
//					sumP += w_next;//�������Դת��,����ȫ���ĸ���
//					sumQ += r_next;
//					sumC += c_next;
//
//					if (cnt_unioncb == 0)//û�����翪��
//					{
//						bTranPass = FALSE;//����·�����ܴ������еĸ���
//						break;
//					}
//
//					//��Ҫ�����ﴦ�����е�ת��
//					//�и��ɵ�û�����磬��һ�����絫û��ԣ��
//					if ( ((cnt_unioncb == 1) 
//						&& (unionAund_ < sqrt(w_next * w_next + r_next * r_next)))
//						|| ((cnt_unioncb==0) && (c_next>0)) )
//					{
//						bTranPass = false;//����·�����ܴ������еĸ���
//						break;
//					}
//
//					if (cnt_unioncb >= 2)//����2�����翪�أ���Ҫ���ø��ӵ��ع�����
//					{
//						if (l_cntMainCB > 5)//����5������ת���Ŀ���
//						{
//							if ((fabs(unionAund_*0.7)) < sqrt(w_next * w_next + r_next * r_next))
//							{
//								bTranPass = false;//����·�����ܴ������еĸ���
//								break;
//							}
//						}
//						else
//						{
//							//5�����翪�ص��������У���һ������һ��
//							cntProb = lld->getRandomCB(l_cntMainCB,lCB);//�õ���������
//							for (i=0;i<cntProb-1;i++)
//							{
//								sProb = lld->getStringByIndex(&lCB,i);//�õ�һ����������
//								k=0;
//								for (j=0;j<sProb.GetLength();j++)
//								{
//									if (sProb[j] == '0')//�Ͽ��Ŀ���
//									{
//										openCB[k++]=l_mainCB[j];
//									}
//								}
//	
//								if (k>3)//���ز�����������3����û��ʵ�ʼ�ֵ
//									continue;
//
//								openCB[k++] = firstCB[ii];//����������Ҫ�ֿ�����ڿ���
//								//�����п��������, 							
//								if ((ret = OnSearchTP(cnt_unioncb,l_unCB, l_unCB_, l_unRebund, k,openCB,c_next,tranSumld)) < 0)
//									bTranPass = false;//����·�����ܴ������еĸ���
//								else
//								{
//									//bTranPass = TRUE;//����·�����ܴ������еĸ���
//									break;//ֻҪ��һ����ͨ���Ϳ�����
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
//	//���е��������ζ�������Ϻ��������ÿ��·���϶���ת������ôʹ�ö����Դ���ع�
//	//�������·����У��ĵ�Դ��������ܵĵ�Դ�����һ����û��Ҫ����ȫ�ֵ�У��
//	if ((sum_unioncb > 0) && (!bTranPass))//���������,�˴�ΪУ�������Դ��
//	{
//		//�ж�ת��ԣ��
//		//�������� ��������һ����Դȫ��ת�� ��Ҫ���в�⣬���ÿ�����ؽ������˷���
//		//�����10�����أ���ô����2^10�����ܣ�ֱ����ٳ����Ϳ�����
//		//һ��ֻ�����������зֶο��أ��������ĳ��߲���������
//
//		//�����������ص�״̬���ж�һ���������ͨ�ԣ�����������ص���������㣨��Դ�㣩
//		//�������ɵ�ĸ��������ж���Щ���ɿ���ת������Щ����
//
//		//mainCB[cntMainCB++] = cb_p;������·���ϵĿ��ض��ҵ��������׶˺�����
//		//firstCB�׶˿���
//		//unCB[sum_unioncb] = knd;
//
//		if (cntMainCB > 5)//����10������ת���Ŀ���
//		{
//			if (fabs(sumAbund*0.7) < sqrt(sumP * sumP + sumQ * sumQ))
//			{
//				bTranPass = FALSE;//����·�����ܴ������еĸ���
//			}
//		}
//		else
//		{		
//			cntProb = lld->getRandomCB(cntMainCB,lCB);//�õ���������
//			for (i=0;i<cntProb-1;i++)
//			{
//				sProb = lld->getStringByIndex(&lCB,i);//�õ�һ����������
//				k=0;
//				for (j=0;j<sProb.GetLength();j++)
//				{
//					if (sProb[j] == '0')//�Ͽ��Ŀ���
//					{
//						openCB[k++]=mainCB[j];
//					}
//				}
//
//				if (k > 3)
//					continue;
//
//				//�����׶˵Ŀ���
//				for(ii = 0; ii < noextbs; ii++)
//				{
//					openCB[k++]=firstCB[ii];
//				}
//				
//				if ((ret = OnSearchTP(sum_unioncb,unCB, unCB_, unRebund, k,openCB,cnt_ld,tranSumld)) == 1)
//				{
//					//ת���ɹ�
//					//�ж�ת��ԣ��
//					deltap = (sumP-deltaMultiP);//˫��Դ��ʱ����ת��
//					deltaq = (sumQ-deltaMultiQ);
//					deltaC = (sumC-deltaMultiC);
//					break;
//				}
//			}
//		}
//	}
//	
//	if (bTranPass)//ͨ��ÿ��֧·��У��
//	{
//		deltap = (sumP-deltaMultiP);//˫��Դ��ʱ����ת��
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

//�������ߣ������ߣ����ܺ��ж��ƽ��ڵ㣬С�ͷֲ�ʽ��Դ�ȵ�Դ�㣬������翪�أ�
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
		if (r_so != -1)//�������е�Դ��
		{
			abunR = dW = dR = 0;
			so_p = dsbob2->r_fd_so_tb->FindChildPtr1(r_so);
			so_p0 = so_p;
			while (so_p)
			{
				i = so_p->Ptr;
				if (((so + i)->slackavr == 4) || ((so + i)->slackavr == 0))
				{
					dW += ((so + i)->w) / 100;	//�й�
					dR += ((so + i)->r) / 100;	//�޹�
						
					//�����ϵ����������������߾��й�
					if ((so + i)->maxcurrent > abunR)
						abunR = (so + i)->maxcurrent;
				}
				so_p = so_p->Next;
			}
			::FreeChildPtr(so_p0);

			if (abunR > 0)
			{
				//���������ۺϳ��й����޹�
				abunW = 100 * (((1.732*abunR / 1000) / 10) - sqrt(dW*dW + dR * dR));//���ڹ���
			}
			else
				abunW = 5;//����Բ�����翪��ԣ��Ϊ0����һ������ֵ�������5

			p_abund = abunW * 0.98;//�����ڹ��ʵ��۳��й�ԣ��
			q_abund = abunW * 0.2;//�޹�ԣ��
		
			r_un = dsbob2->fd_tb->GetDataByPh(j)->r_un;
			if (r_un != -1)//�����������ο���
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
		{//������û�е�Դ�㣬���������翪��
			abunW = 5;//����Բ�����翪��ԣ��Ϊ0����һ������ֵ�������5
			p_abund =  abunW * 0.98;//�����ڹ��ʵ��۳��й�ԣ��
			q_abund =  abunW * 0.2;//�޹�ԣ��

			r_un = dsbob2->fd_tb->GetDataByPh(j)->r_un;
			if (r_un != -1)//�����������ο���
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

//��һ��ͨ�õı�ĶԷַ�����

//ÿ�����翪�������������߸���ԣ��,
//so_id���ߵ�id�ţ�cb_id���翪��id������������翪�ضԲ��ԣ��
int CNM1::getUNCB_abun(char* fdid,char* cb_id,char* nd_id,char* fdid_abund,double& p_abund,double& q_abund,double& v_abund)
{
	int r_un;
	ChildPtr1	*un_p,*un_p0;

	// opp_nd[MAX_ID_DLL];//���翪�ضԲ�ڵ��id
//	int opp_nd;
	char left[MAX_ID_DLL];
//	double dW,dR,abunW,abunR;//�Բ�ĵ�Դ���й����޹�

	//���Ը����������翪�ع�ϵ��ֱ�ӵõ�
	int ph = dsbob2->OnGetPhByfdid(fdid);
	r_un = dsbob2->fd_tb->GetDataByPh(ph)->r_un;
	if (r_un != -1)//�����������ο���
	{
		un_p = dsbob2->r_fd_un_tb->FindChildPtr1(r_un);
		un_p0 = un_p; 
		while(un_p)
		{
			//�����ڵ����б߽翪�ض���Ҫ��
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
//	 fdtemp[100];	//�����������ߵ�id
//	// soID[100],so_OPP_ID[100],sL[100],sR[100];
//	CString soID,so_OPP_ID,sL,sR;
//	double dW,dR,abunW,abunR;//�Բ�ĵ�Դ���й����޹�
//	 opp_nd[80];//���翪�ضԲ�ڵ��id
//
////	//���߶Բ�ĵ�Դid��
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
//	//���������б��õ���ţ���ÿһ�����ؽ������������ԸĽ��˴�����������
//	//�Էַ�
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
//		{  //�ҵ�,�����±�����ֵ
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
//			//�õ��Բ�ĵ�Դid��,�õ��ǶԲ�ĵ�Դ������������ID������ǰ׺
//			if (sL == soID)
//			{
//			  so_OPP_ID = sR;	
//			}
//			else
//			{
//			  so_OPP_ID = sL;
//			}
//
//			//ͨ���Բ��id�ŵõ���Դ��ʵ���й����޹�
//			for(i = 0; i < lv_so; i++)
//			{
//				if (strcmp((so+i)->fdid,so_OPP_ID) == 0)
//				{
//					abunR = (so+i)->MAXCURRENT;
//					//���������ۺϳ��й����޹�
//
//					dW = ((so+i)->w) / 100;	//�й�
//					dR = ((so+i)->r) / 100;	//�޹�
//					abunW = 100*(((1.732*abunR/1000)/10)  - sqrt(dW*dW+dR*dR));//���ڹ���
//					
//					//����Բ�����翪��ԣ��Ϊ0����һ������ֵ�������5
//					p_abund = abunW * 0.98;//�����ڹ��ʵ��۳��й�ԣ��
//					q_abund =	abunW * 0.2;//�޹�ԣ��
//
//					break;
//				}
//			}
//			strcpy(fdid_abund,so_OPP_ID);//���翪�ضԲ������id
//
//			ret = 1;
//			//�����Ƿ��ҵ���Դ�㣬������Ҫ�˳�
//			break;
//		}
//		else if(strcmp(cb_id,(cb+middle)->id)<0)
//		{  //����ֵ�ڵͰ���
//            high = middle - 1;
//        }
//		else
//		{  //����ֵ�ڸ߰���
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
//	//�ڵ�ĵ�ѹ
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
//		{  //�ҵ�,�����±�����ֵ
//			int ibs = (nd+middle)->ibs;
//			v_abund = (bs+ibs)->v;
//			break;
//		}
//		else if(strcmp(opp_nd,(nd+middle)->id)<0)
//		{  //����ֵ�ڵͰ���
//            high = middle - 1;
//        }
//		else
//		{  //����ֵ�ڸ߰���
//            low = middle + 1;
//        }
//    }

//so_id���ߵ�id�ţ�cb_id���翪��id������������翪�ضԲ��ԣ��
//�����ָ�����翪�أ�����������ߵ��������ط���
double CNM1::getUNCB_abun(char* fdid,char* so_OPP_id,char* cb_id)
{
	int r_un;
	ChildPtr1	*un_p,*un_p0;

	// opp_nd[MAX_ID_DLL];//���翪�ضԲ�ڵ��id
//	int ibs;
	char left[MAX_ID_DLL];
	double p_abund,q_abund,v_abund;//�Բ�ĵ�Դ���й����޹�
	p_abund=q_abund=v_abund=0;
	if (so_OPP_id[0] != '\0')
	{
		so_OPP_id[0] ='\0';
	}

	//���Ը����������翪�ع�ϵ��ֱ�ӵõ�
	int ph = dsbob2->OnGetPhByfdid(fdid);
	r_un = dsbob2->fd_tb->GetDataByPh(ph)->r_un;
	if (r_un != -1)//�����������ο���
	{
		un_p = dsbob2->r_fd_un_tb->FindChildPtr1(r_un);
		un_p0 = un_p; 
		while(un_p)
		{
			//�����ڵ����б߽翪�ض���Ҫ��
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

				strcat(so_OPP_id,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->id);//׼�����
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
				strcat(so_OPP_id,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->id);//׼�����

				break;
			}

			un_p = un_p->Next;
		}
		::FreeChildPtr(un_p0);
	}

	return sqrt(p_abund*p_abund+q_abund*q_abund);




//	int m,i;
//	 fdtemp[100];	//�����������ߵ�id
//	CString soID,so_OPP_ID,sL,sR;
//	double dW,dR,abunW,abunR,temp_pq;//�Բ�ĵ�Դ���й����޹�
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
//	//���������б��õ����
//	for ( m = 0; m < lv_cb; m++ )
//	{
//		if ((cb+m)->q & 1)//���翪��ͬʱ����Ҫ�ڵ���
//		{
//				//�ж��ǲ������յĿ��أ���������յģ����ܾͲ���������
//			if (cb_id == NULL)//�����������ߵ����е����翪��
//			{
//				if (lld->isContainor((cb+m)->fdid,'|') < 0)
//					continue;
//
//				strcpy(fdtemp,(cb+m)->fdid);
//				lld->getLR_Bykey(fdtemp,"|",sL,sR);
//
//				//�õ��Բ�ĵ�Դid��
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
//					//ͨ���Բ��id�ŵõ���Դ��ʵ���й����޹�
//					for(i = 0; i < lv_so; i++)
//					{
//						if (strcmp((so+i)->fdid,so_OPP_ID) == 0)
//						{
//							if (so_OPP_ID != "")
//							{
//								strcat(so_OPP_id,so_OPP_ID);//׼�����
//								strcat(so_OPP_id,",");
//							}
//
//							abunR = (so+i)->MAXCURRENT;
//							abunR = (1.732*abunR/1000)/10;
//
//							dW = ((so+i)->w) / 100;	//�й�
//							dR = ((so+i)->r) / 100;	//�޹�
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
//					//�õ��Բ�ĵ�Դid��,�õ��ǶԲ�ĵ�Դ������������ID������ǰ׺
//					if (sL == soID)
//					{
//					  so_OPP_ID = sR;	
//					}
//					else
//					{
//					  so_OPP_ID = sL;
//					}
//
//					//ͨ���Բ��id�ŵõ���Դ��ʵ���й����޹�
//					for(i = 0; i < lv_so; i++)
//					{
//						if (strcmp((so+i)->fdid,so_OPP_ID) == 0)
//						{
//							abunR = (so+i)->MAXCURRENT;
//
//							dW += ((so+i)->w) / 100;	//�й�
//							dR += ((so+i)->r) / 100;	//�޹�
//
//							abunW += 100*(((1.732*abunR/1000)/10)  - sqrt(dW*dW+dR*dR));						
//
//							break;
//						}
//					}
//
//					strcpy(so_OPP_id,so_OPP_ID);//׼�����
//					break;
//				}
//			}
//		}
//	}

//	delete[] cb_t;
//	cb_t = 0;

//	return 0.9 * fabs(abunW);
}

//���ÿһ�����μ�����������η���ͣ�磬���ε��û��Ƿ����ת��
//�ڼ������������ļ��裺
//1.��ĳ�����η������Ϻ���ʧ���û����Ǳ������ڵ��û������β���ת�����û��������û�����ʧ
//2.˫���ࣩ��Դ�û���Զ����ͣ��
//3.���е��߶ζ������ε�һ���֣������Ǽ������С��Ԫ
//4.�������n-1�󣬿���ת����ֻ�кͱ�����·���ӵ����翪�أ�������������·����ת�����ɵ�����

//���������߶�Ϊ��λ���������������߶γ��ֹ��ϣ�����Ӱ�쵽���ε���Щ����
//��Щ���ɻ�ת������Щ��ʧ���ˣ������ʧ�ˣ��Ϳ�����Ϊ������n-1
//����ת���ĸ��ɴ�С����ʧ���ɴ�С

//�������εĸ��ɺ͵�Դ����Ҫ���ǵĽ��������εĿ��ؽ������
//�ڴ˼��������£����Զ��Ŀ����Ǻ��ٵģ�����Ѿ�����������

//�����߼���n-1
int CNM1::PutNM1Data()
{
//	int		i,j,iNO;
//	double dReal,dImp;
//	double curW;//�������ߵ����ڹ���
//	double tranW;//���������ԣ��
////	int		ibs, zbs;	//�߶ε���ĩĸ�ߣ��ڵ��Ż����ǰ��
////	int		iout,zout;	//�߶���ĩ�ڵ��Ƿ���������
//	   unionSO[MAX_UION_ID];//����ĵ�Դ��id���ϣ��м��ã��ֿ�	
//
////�ļ���Ҫ�����û����Ͱ汾�ţ������ſ�������
//	CString sFileName,sSQL,fdid,unionList,transSec,lossSec,zoneLoss;
//	 secid[MAX_SEC_ID_LEN];
////	sFileName = "nm1.efile";
////
////	ofstream outfile(sFileName,ios::out);
////
////	outfile<<"<!System= Version=1.0 Code=UTF-8 Data=!>"<< endl;
////	outfile<<"<so_detailResult::Result name=N-1ɨ����ϸ�����>"<< endl;
////	outfile<<"@	  ID       nd       ndCB     Faultload          Trload					NowTrload				 PowerLostRatio						unionSO      Result			LineResult"<< endl;
////	outfile<<"/@  ������ ��·id �ֶ�id ���϶�ʧ�縺��(MW) �ǹ��϶���ת�Ƹ���(MW)	�ǹ��϶�ʵ��ת�Ƹ���(MW) �ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%  ת����Դ     �ֶ�N-1������ ȫ��N-1������"<< endl;
//    
//	double Faultload;	//���϶�ʧ�縺��(MW)
//	double Trload;		//�ǹ��϶���ת�Ƹ���(MW)
//	double NowTrload;	//�ǹ��϶�ʵ��ת�Ƹ���(MW)
//	double PowerLostRatio;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
//	 Result;			//�ֶ�N-1������ 
//	 LineResult;		//ȫ��N-1������
//	int iCountSect;		//�߶εķֶ�����
//
////
//	ZN* zn;
//	int r_zn,r_sec;
//	ChildPtr1	*sec_p,*sec_p0 ;
//
//
//	//����һ��ȫ����ɨ�裬�õ�ÿ�����߶εĹ��Ϻ��
//	//*****��Ϊ���Ĵ��룬������Ҫ�Ƕ���δ�����н������
//	PutRiskData();
//
//	//��ÿһ���߶ν��м���
//	//**********ÿ��ֻ��һ������ÿ��ֻд�벿���߶ε����ݣ����ϲ���������ȫ��
//	//�Ƚ��б������ߣ����������߶�n-1���㣬�������Ѿ�д��sec�ļ����ֶ�����
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
////1.��һ��������ȫ����·��n-1�������Դ���ڴ����ֹ��ϣ������ߵ����и����Ƿ����ת��
////		dReal = (so+i)->w;//����һ�������������棬�ø������������ۼӿ��Ը�׼ȷ
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
//		curW = sqrt(dReal*dReal + dImp*dImp);//��ǰ��Դ������ڹ���,���ص�������ֵ
//
//		if (curW == 0)
//		{
//			PowerLostRatio=0;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
//			LineResult = '0';
//			tranW = 0;
//			unionList = "-";
//		}
//		else
//		{
//			//�����뱾�������ӵ���������,�Լ���ص�ԣ��
//			tranW = getUNCB_abun((so+i)->fdid,unionSO);
//			unionList = unionSO;
//			if ((unionList=="") || (unionList==",") )
//				unionList = "-";
//			else
//				unionList=unionList.Left(unionList.GetLength()-1);
//
//			if ((tranW) > curW)//����Բ������ԣ�ȴ�����Ҫת����ԣ�ȣ��������ߵ�n-1����ͨ��
//			{
//				PowerLostRatio=0;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
//				LineResult = '1';
//				tranW = curW;
//			}
//			else
//			{
//				PowerLostRatio=100;//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
//				LineResult = '0';
//				tranW = 0;
//			}
//		}
//
//		fdid = (so+i)->fdid;
//		//strcpy(secid , "-");
//		Faultload=0;	//���϶�ʧ�縺��(MW)
//		Trload=curW;		//�ǹ��϶���ת�Ƹ���(MW)
//		NowTrload=tranW;	//�ǹ��϶�ʵ��ת�Ƹ���(MW)
//		
//		//;		//ȫ��N-1������
//		dsbob2->nm1_result[lld->g_cnt_nm1_fd].id=lld->g_cnt_nm1_fd;
//		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].fdid,fdid);
//		strcpy(dsbob2->nm1_result[lld->g_cnt_nm1_fd].secid,"������n-1");
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
////		sSQL.Format("# %d\t%s\t������n-1\t%0.4f\t%0.4f\t%0.4f\t%0.2f\t%s\t-\t%d", \
////			          iNO++,fdid,Faultload,Trload,NowTrload,PowerLostRatio, unionList,LineResult);
////
////		outfile.write(sSQL,sSQL.GetLength());
////		outfile<<endl;
//
//		//ͨ������id������
////////////////////////////////////////////////////////////////////////////
////2.��ÿ�����߶Ρ����ÿ�����߶ζ���ͨ��n-1���������߿���ͨ�������������߲���ͨ��
//		//ÿ�����߶����������εı�ʾr_zn
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
////			if(iout || zout)//�߶β��ڵ���
////			{
////				continue;			
////			}
//
////�ж�������߶��Ƿ��������Դ������
//			//if (fdid == (sec+j)->fdid)
//			if (strcmp(fdid , (sec+j)->fdid) == 0)
//			{//mxP = mxP>mxQ ? mxP:mxQ
//				strcpy(secid,(sec+j)->id);
////			
////				//�õ����������������������߶�
////				if ((r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1((sec+j)->r_zn)) == -1)
////					continue;
////
////				zn = dsbob2->zn_tb->GetDataByPh(r_zn); 
////
////				//�������α������ҵ�������Σ��õ�r_ld,����r_ld��������
////				r_sec = zn->r_sec;
////				sec_p =  dsbob2->r_zn_sec_tb->FindChildPtr1(r_sec);
////				
////				//�õ��߶����������θ���
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
//					Faultload	= (sec+j)->lP_zn;	//���϶�ʧ�縺��(MW)
//				else
//					Faultload	= 0;	//���϶�ʧ�縺��(MW)
//
//				if (fabs((sec+j)->lP_znnext) > 0.001)
//					Trload		= fabs((sec+j)->lP_znnext);		//�ǹ��϶���ת�Ƹ���(MW)
//				else
//					Trload		= 0;
//
//				if (fabs((sec+j)->transP) > 0.001)
//					NowTrload	= fabs((sec+j)->transP);	//�ǹ��϶�ʵ��ת�Ƹ���(MW)
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
//						PowerLostRatio=100 * ((Trload-NowTrload)/Trload);//�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%
//					else
//						PowerLostRatio=100;
//
//					LineResult = '0';//ֻҪ��һ��������n-1��ȫ�߶�������
//					Result = '0';
//				}
//				
//				//�ֶ�N-1������
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
//		//���е����߶ν�������Եõ����������Ƿ�����n-1������//�ֶ�N-1������ 
////		if (iCountSect > 0)
////		{
//// 			sSQL.Format("# %d\t%s\t�ֶ�n-1�ܽ��\t-\t-\t-\t-\t-\t-\t%d",iNO++,(so+i)->fdid,LineResult);
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

//��������һ�������ԣ�����ֻ�����߶ε�N-1��û�����豸��N-1
//ʵ���ϣ�ÿһ���豸��N-1Ҳ��Ҫ���ǣ�������·�ϵĿ��ص�N-1����ѹ����N-1�ȵ�

//���Ҫ�õ�FEMA����ɿ��ԣ�����Ҫ�ó����߶�ʧ���������ʧ�ĸ��ɸ��������ο���ת���ĸ�����������Ҫ�ָ��ĸ���
//������һ���ṹ���洢����Ϣ
int CNM1::PutRiskData()
{
	int		i,j,ret;
	
	int		lv_sec;//�߶ε�ָ�룬�߶�����
	int		ibs, zbs;	//�߶ε���ĩĸ�ߣ��ڵ��Ż����ǰ��
	int		iout,zout;	//�߶���ĩ�ڵ��Ƿ���������
	
	int		iznC;		//�������û���
	double	ld_pq_temp;	//���������ĸ��ɣ������θ��ɣ�
		
	//CString intnode[MAX_ZNNO],extnode[MAX_ZNNO];//�����α߽�ڵ�
	int extbs[MAX_ZNNO],upextbs[MAX_ZNNO];//������������չĸ�߼�������ĸ��
	ChildPtr1	*cb_p,*cb_p0, *ld_p,*ld_p0 ;
	int ld_nd, r_zn,r_ld,r_cb;//���ɵļ���ĸ�ߺ�,�ڵ��Ż����
	CB*  cb1;

	int noextbs;

	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();
	
	//get sec table
	lv_sec = dsbob2->sec_tb->GetCount();
	SEC* sec = dsbob2->sec_tb->GetTable();
	
	//���α�����
	int lv_zn = dsbob2->zn_tb->GetCount();
	ZN* zn = dsbob2->zn_tb->GetTable();
	for (i = 0; i < lv_zn; i++)
	{
		(zn+i)->lossC = -1; //�����Լ������μ�����һ����ʧ���û�
		(zn+i)->transC = -1;//���ο���ת�����û�
	}

	//get sec table
	int lv_ld = dsbob2->ld_tb->GetCount();
	LD* ld = dsbob2->ld_tb->GetTable();

	//��ÿһ���߶ν��м���
	//**********ÿ��ֻ��һ������ÿ��ֻд�벿���߶ε����ݣ����ϲ���������ȫ��

//1.���ж��߶��������Σ��鿴���ε�ialg4q���������û�������w���������й���
	//���������û���ialg4p�����������û����������������й���wz��
//2.����������Σ���������ε�����4��ֵ�����������
//3.������һ����Ҫ��ǰ�ᣬÿ�����εĵ�һ������Ϊ�������εĿ��ء��������ε�ʱ���Ѿ��������

//4.����֮ǰ��׼��ra_cnt��ra_feeder[MAX_RA_CNT];
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
//		sec_ana = i;//���ڷ��������߶Σ�ȫ�ֱ������������ĺ���������ܻ��õ�

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

	
		if(iout && zout)//�߶�ͬʱ���ڵ���
        {
			//��������߶ε����ε��û��͸���
            (sec+i)->lossP   = 0;//(sec+i)->lP_znnext;
            (sec+i)->transP  = 0;
            (sec+i)->lossC   = 0;//(sec+i)->lC_znnext;
            (sec+i)->transC  = 0;

			(sec+i)->lC_zn = 0;//�������û�
			(sec+i)->lP_zn = 0;//�������й�
			(sec+i)->lC_znnext = 0;//���������û�
			(sec+i)->lP_znnext = 0;//���������й�
			strcpy((sec+i)->tranfdlist,"");//ת��·��

			//���ε����翪����Ҫ��������·���б���ʵ��
			strcpy((sec+i)->closeunlist,"-");//ת��·��
			strcpy((sec+i)->opencblist,"-");//ת��·��

			continue;			
        }
		
		up_cb = -1;//Ĭ��û�����ο���
		for(j = 0; j < lv_ld; j++)
		{
			ldlist[j] = -1;		//���еĸ��ɶ�δ����
		}

//���κ��߶εĹ�ϵ
		//ͨ������ID�õ������ι����ĸ��ɵ�ָ��
		if ( ((r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1((sec+i)->r_zn)) == -1)
			|| ((sec+i)->r0 == 0)//0�迹֧·
			)
		{//�߶β������κ�����

            (sec+i)->lossP   = 0;//(sec+i)->lP_znnext;
            (sec+i)->transP  = 0;
            (sec+i)->lossC   = 0;//(sec+i)->lC_znnext;
            (sec+i)->transC  = 0;

			(sec+i)->lC_zn = 0;//�������û�
			(sec+i)->lP_zn = 0;//�������й�
			(sec+i)->lC_znnext = 0;//���������û�
			(sec+i)->lP_znnext = 0;//���������й�
			strcpy((sec+i)->tranfdlist,"");//ת��·��

			//���ε����翪����Ҫ��������·���б���ʵ��
			strcpy((sec+i)->closeunlist,"-");//ת��·��
			strcpy((sec+i)->opencblist,"-");//ת��·��

			continue;
		}

		zn = dsbob2->zn_tb->GetDataByPh(r_zn); 

		int lossC =  zn->lossC;    //�����Լ������μ�����һ����ʧ���û�,���β���ʧ
		int transC = zn->transC;    //���ο���ת�����û�
		iznC = zn->inner_C;//�����ڵ��û�����
		if ((lossC == -1) && (transC == -1))//����û�м����
		{
//////////////////////////////////////////////////////////////////////////
//�������ο��ص�������жϿ����Ƿ����۶����ʹ������Ķ�·��������ǣ����л�ʱ��

			r_cb = zn->r_cb;
			if (r_cb != -1)//�����������ο���
			{
				cb_p = dsbob2->r_zn_cb_tb->FindChildPtr1(r_cb);
				if (cb_p)//�����ڴ��ڵ�һ�����أ�������Ŀ���
				{
					cb1 = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr);
					if (cb1->cbtype==3)
						up_cb = 3;

					if (cb1->cbtype==5)
						up_cb = 5;
				}
			}

//////////////////////////////////////////////////////////////////////////
//���㱾�����ڵ����
			zn_ld_cnt=zn_ld_pq=zn_multi_ld_cnt=zn_multi_ld_pq=0;
			open_idList[0]=0;//��������û�зֶο��ش�

			if (iznC > 0)//�������и��ɵ�
			{
				//�������α������ҵ�������Σ��õ�r_ld,����r_ld��������
				r_ld = zn->r_ld;
				ld_p =  dsbob2->r_zn_ld_tb->FindChildPtr1(r_ld);
				
				//�õ��߶����������θ���
				ld_p0 = ld_p; 
				while(ld_p)
				{
					ld_pq_temp =sqrt((double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w)*(double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w)+(double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r)*(double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r)); 
					//���ڹ���
					zn_ld_cnt++;
					zn_ld_pq += ld_pq_temp;
						
					ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
					if ( (dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & (ND_SO) )//���Դ
					{
						zn_multi_ld_cnt++;
						zn_multi_ld_pq += ld_pq_temp;//�����ڶ��Դ�����ڹ���
					}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
	
					//һ����ʧ�ĸ��ɼӵ������б�����
//					ra_feeder[ra_cnt].ld_id = ld_p->Ptr;//���ɵ������
//					ra_feeder[ra_cnt].up_cb = up_cb;//���ο��ص����ʣ��Թ����л�����
//					ra_feeder[ra_cnt].out = 1;//������ʧ
//					ra_feeder[ra_cnt].swit = 0;//���ɲ����л�
//					ra_feeder[ra_cnt].tran = 0;//���ɲ���ת��
//					ra_feeder[ra_cnt].sec_id = i;//���ɶ�Ӧ�Ĺ�����������ţ���ʱ�����߼�ͳһ��
//					ldlist[ld_p->Ptr] = 1;//�����Ѵ���
//					ra_cnt++;

					ld_p = ld_p->Next;
				}
				::FreeChildPtr(ld_p0);
			}
			
//////////////////////////////////////////////////////////////////////////
//�����������ε�ת�����
			//���ε�����
			noextbs = zn->type; 

			if (noextbs > MAX_ZNNO)//Ӧ����ͼ�����ˣ�һ�㲻������������
			{
				//��������߶ε����ε��û��͸���
				(sec+i)->lossP   = 0;//(sec+i)->lP_znnext;
				(sec+i)->transP  = 0;
				(sec+i)->lossC   = 0;//(sec+i)->lC_znnext;
				(sec+i)->transC  = 0;

				(sec+i)->lC_zn = 0;//�������û�
				(sec+i)->lP_zn = 0;//�������й�
				(sec+i)->lC_znnext = 0;//���������û�
				(sec+i)->lP_znnext = 0;//���������й�
				strcpy((sec+i)->tranfdlist,"");//ת��·��

				//���ε����翪����Ҫ��������·���б���ʵ��
				strcpy((sec+i)->closeunlist,"-");//ת��·��
				strcpy((sec+i)->opencblist,"-");//ת��·��

				char slog[256];
				sprintf(slog,"%s ��·�����α߽翪����Ϊ%d����������",(sec+i)->fdid,noextbs);
			lld->WriteLog(lld->logfile,slog);

				continue;
			}

///�õ�����-���ص��໥��ϵ��������ε�����ڵ�
			r_cb = zn->r_cb;
			if (r_cb != -1)//�����������ο���
			{
				cb_p = dsbob2->r_zn_cb_tb->FindChildPtr1(r_cb);
				
				//�õ������ڵ�ȫ������,�����׶˿���
				int ii=0;
				cb_p0 = cb_p; 
				while(cb_p)
				{
					//�����ڵ����б߽翪�ض���Ҫ��
					cb1 = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr);
					//����߽翪��������Ļ���Ҳ�ǲ���Ҫ���������
					if (cb1->q & CB_NMLOPEN)  
					{
						//����ֻ��һ����λ�Ŀ����ǲ����жϵģ�����û���������·����
						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
						{
							cb_p = cb_p->Next;
							continue;				
						}
					}					
					
					strcat(open_idList,cb1->id);
					strcat(open_idList,"|");
					if (cb_p->Ptr == zn->cbptr)//�������εĿ�����Ҫ����
					{
						//�����뿪�ص�ָ��д���򿪵ĸ��뿪����������

						cb_p = cb_p->Next;
						continue;				
					}
					extbs[ii] = cb1->iznd;//�߽翪�ص����ڵ��ĸ��
					upextbs[ii] = cb1->ind;//�߽翪���ڲ�ڵ����������߶ζԲ�ڵ�ļ���ĸ��
			
					ii++;
					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//lld add 2014-1-15
				//noextbs���α߽翪������,extbs�߽���ڵ�,upextbs�߽��ڽڵ�,��������������Ŀ��أ������Ƕ������
				//ret == 0;nm1��ͨ�� 
				ret = residueAnalyze(ii,extbs,upextbs);
				//����������������󣬽����ڵĽ����Ӧ����
				if (ret)//���ο���ת����������ȫ��Ҳ�п���ֻ�в�����ת��
				{
					zn->inner_C = zn_ld_cnt;//�����ڵĸ��ɸ���
					zn->inner_P = zn_ld_pq;//�����ڵĸ������ڹ���

					zn->transC = downstream_trans_ld_cnt;//���ο���ת�����û�
					zn->lossC = zn_ld_cnt+downstream_ld_cnt-zn_multi_ld_cnt-downstream_multi_ld_cnt-downstream_trans_ld_cnt; //�����Լ������μ�����һ����ʧ���û�
					zn->transP = downstream_trans_ld_pq;
					zn->lossP = zn_ld_pq+downstream_ld_pq-zn_multi_ld_pq-downstream_multi_ld_pq-downstream_trans_ld_pq;

					zn->next_C = downstream_ld_cnt;//���������û�
					zn->next_P = downstream_ld_pq;//�������εĸ������ڹ���
				
					//�п����Ƕ�����翪�غϣ�����ֶο��ط�
					strcpy(zn->unfd_idList,unfd_idList);//������·����Ҫ����ȥ��ֻ����һЩ�仯

					//�������翪�����������������翪��
					strcpy(zn->closeunlist,un_idList);//������·����Ҫ����ȥ��ֻ����һЩ�仯
					
//					strcpy(zn->transSec,transSec);//������·����Ҫ����ȥ��ֻ����һЩ�仯
//					strcpy(zn->lossSec,lossSec);//������·����Ҫ����ȥ��ֻ����һЩ�仯
				}
				else
				{//����ȫ����ʧ
					zn->inner_C = zn_ld_cnt;//�����ڵĸ��ɸ���
					zn->inner_P = zn_ld_pq;//�����ڵĸ������ڹ���

					zn->transC = 0;//���ο���ת�����û�
					zn->lossC = zn_ld_cnt-zn_multi_ld_cnt; //�����Լ������μ�����һ����ʧ���û�
					zn->transP = 0;
					zn->lossP = zn_ld_pq-zn_multi_ld_pq;

					zn->next_C = 0;//���������û�
					zn->next_P = 0;//�������εĸ������ڹ���
					strcpy(zn->unfd_idList,"");//������·����Ҫ����ȥ��ֻ����һЩ�仯
					zn->closeunlist[0] = 0;//����û�зֶο�����Ҫ��
					//zn->opencblist[0] = 0;//����û�зֶο�����Ҫ��

//					strcpy(zn->transSec,"-");//������·����Ҫ����ȥ��ֻ����һЩ�仯
//					strcpy(zn->lossSec,"-");//������·����Ҫ����ȥ��ֻ����һЩ�仯
				}
				strcpy(zn->opencblist,open_idList);//������Ҫ�򿪵ķֶο����б�
			}
			else//������û�����ο��أ�����ȫ����ʧ
			{
				zn->inner_C = zn_ld_cnt;//�����ڵĸ��ɸ���
				zn->inner_P = zn_ld_pq;//�����ڵĸ������ڹ���

				zn->transC = 0;//���ο���ת�����û�
				zn->lossC = zn_ld_cnt-zn_multi_ld_cnt; //�����Լ������μ�����һ����ʧ���û�
				zn->transP = 0;
				zn->lossP = zn_ld_pq-zn_multi_ld_pq;

				zn->next_C = 0;//���������û�
				zn->next_P = 0;//�������εĸ������ڹ���
				strcpy(zn->unfd_idList,"");//������·����Ҫ����ȥ��ֻ����һЩ�仯
				zn->closeunlist[0] = 0;//����û�зֶο�����Ҫ��
			}						   
		}		

		//�������ڵ������Ϣ��ֵ�����������߶�
        (sec+i)->lossP   = zn->lossP;		//���μ��������ܸ���
        (sec+i)->transP  = zn->transP;      //�������ο���ת���ĸ��ɣ����β���Ҫת��
        (sec+i)->lossC   = zn->lossC;	//�����Լ������μ�����һ����ʧ���û�
        (sec+i)->transC  = zn->transC;	//���ο���ת�����û�
		(sec+i)->lC_zn = zn->inner_C;//�������û�
		(sec+i)->lP_zn = zn->inner_P;//�������й�
		(sec+i)->lC_znnext = zn->next_C;//���������û�
		(sec+i)->lP_znnext = zn->next_P;//���������й�
		strcpy((sec+i)->tranfdlist,zn->unfd_idList);//ת��·��

		//���ε����翪����Ҫ��������·���б���ʵ��
		strcpy((sec+i)->closeunlist,zn->closeunlist);//ת��·��
		strcpy((sec+i)->opencblist,zn->opencblist);//ת��·��

//		strcpy((sec+i)->transSec, zn->transSec);//������·����Ҫ����ȥ��ֻ����һЩ�仯
//		strcpy((sec+i)->lossSec, zn->lossSec);//������·����Ҫ����ȥ��ֻ����һЩ�仯
		
		//���������ε�ҲҪ�ҳ���
		//�������߶�����������		
	    for(j = 0; j < lv_ld; j++)
		{
			if ( (ldlist[j] == -1)
				 && (strcmp((ld+j)->fdid,(sec+i)->fdid)==0)//����������������
				 )
			{
//				ra_feeder[ra_cnt].ld_id = j;//���ɵ������
//				ra_feeder[ra_cnt].up_cb = up_cb;//���ο��ص����ʣ��Թ����л�����
//				ra_feeder[ra_cnt].out = 0;//������ʧ
//				ra_feeder[ra_cnt].swit = 1;//�������л�
//				ra_feeder[ra_cnt].tran = 0;//���ɲ���ת��
//				ra_feeder[ra_cnt].sec_id = i;//���ɶ�Ӧ�Ĺ�����������ţ���ʱ�����߼�ͳһ��
//				ra_cnt++;
			}
		}

	}

	return 1;
}




//���ÿһ�����߶εõ���صķ���ָ��
//�ڼ������������ļ��裺
//1.��ĳ���߶η������Ϻ���ʧ���û����Ǳ������ڵ��û������β���ת�����û��������û�����ʧ
//2.˫���ࣩ��Դ�û���Զ����ͣ��
//3.���е��߶ζ������ε�һ���֣������Ǽ������С��Ԫ
//int CNM1::PutRiskData()
//{
//	int		i/*,k*/;
//	
//	int		lv_sec;//�߶ε�ָ�룬�߶�����
//	//	CString secID;		//�߶�ID
//	int		ibs, zbs;	//�߶ε���ĩĸ�ߣ��ڵ��Ż����ǰ��
//	int		iout,zout;	//�߶���ĩ�ڵ��Ƿ���������
//	
//	double	lossP,transP;	//�߶ι��Ϻ���ʧ���ɣ������Լ�����ת������
//	int		lossC,transC;	//�߶ι��Ϻ���ʧ�û���ת���û�
//	int inner_C,next_C;
//	double inner_P,next_P;
//	 unfd_idList[400];
//	double  sumP,sumQ,deltap,deltaq;//�����ܸ��ɣ����ο�ת������
//	int     sumC,deltaC;	//�������û������ο�ת���û�
//	double  deltaMultiP,deltaMultiQ;	//����˫��Դ����
//	int     deltaMultiC;	//����˫��Դ�û�
//	
//	double	znP;	//�������ܸ���
//	
//	int		iznC;		//�������û���
//	double	mxP,mxQ,mxTemp;	//���������ĸ��ɣ������θ��ɣ�
//	
//	int		multiC;	//������˫��Դ�ĸ���
//	double	multiP;	//������˫��Դ����
//	
//	CString intnode[MAX_ZNNO],extnode[MAX_ZNNO];//�����α߽�ڵ�
//	int /*peri_nd[MAX_ZNNO]*//*�����ܱ����ڵ�*/extbs[MAX_ZNNO],upextbs[MAX_ZNNO];//������������չĸ�߼�������ĸ��
//	ChildPtr1	*sec_p, *sec_p0, *cb_p,*cb_p0, *ld_p,*ld_p0 ;
//	int ld_nd, r_zn,r_ld,r_cb;//���ɵļ���ĸ�ߺ�,�ڵ��Ż����
//	SEC* sec1;
//	CB*  cb1;
//	int cnt_u;
//	double Faultload;	//���϶�ʧ�縺��(MW)
//	double Trload;		//�ǹ��϶���ת�Ƹ���(MW)
//	double NowTrload;	//�ǹ��϶�ʵ��ת�Ƹ���(MW)
//	
//	//lld comment 2011-11-25
//	int noextbs;
//	//int *pextbs, *pupextbs, noextbs;//�ֱ�ָ���ϡ�����ĸ�������ָ�룬��������չ�����νڵ�ĸ���
//	
//	//lld comment 2011-11-25
//	//pextbs,pupextbsָ���ʼ��
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
//	//���α�����
//	int lv_zn = dsbob2->zn_tb->GetCount();
//	ZN* zn = dsbob2->zn_tb->GetTable();
//	for (i = 0; i < lv_zn; i++)
//	{
//		(zn+i)->lossC = -1; //�����Լ������μ�����һ����ʧ���û�
//		(zn+i)->transC = -1;//���ο���ת�����û�
//	}
//	//��ÿһ���߶ν��м���
//	//**********ÿ��ֻ��һ������ÿ��ֻд�벿���߶ε����ݣ����ϲ���������ȫ��
//1.���ж��߶��������Σ��鿴���ε�ialg4q���������û�������w���������й���
//	//���������û���ialg4p�����������û����������������й���wz��
//2.����������Σ���������ε�����4��ֵ�����������
//3.������һ����Ҫ��ǰ�ᣬÿ�����εĵ�һ������Ϊ�������εĿ��ء��������ε�ʱ���Ѿ��������
//    for(i = 0; i < lv_sec; i++)
//    {
//        ibs = (sec+i)->ibs;
//        zbs = (sec+i)->zbs;
//        iout = (sec+i)->topflags & SEC_IOPEN || (bs+ibs)->iisland < 0;
//        zout = (sec+i)->topflags & SEC_ZOPEN || (bs+zbs)->iisland < 0;
//		
//        if(iout || zout)//�߶β��ڵ���
//        {
//			//��������߶ε����ε��û��͸���
//            (sec+i)->lossP   = 0;//(sec+i)->lP_znnext;
//            (sec+i)->transP  = 0;
//            (sec+i)->lossC   = 0;//(sec+i)->lC_znnext;
//            (sec+i)->transC  = 0;
//			continue;			
//        }
//		
//		lossP=transP=0;
//		lossC=transC=0;	
//���κ��߶εĹ�ϵ
//		//ͨ������ID�õ������ι����ĸ��ɵ�ָ��
//		if ((r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1((sec+i)->r_zn)) == -1)
//			continue;
//		zn = dsbob2->zn_tb->GetDataByPh(r_zn); 
//		lossC =  zn->lossC;    //�����Լ������μ�����һ����ʧ���û�,���β���ʧ
//		transC = zn->transC;    //���ο���ת�����û�
//		iznC = zn->inner_C;
//		if ((lossC == -1) && (transC == -1))//����û�м����
//		{
//////////////////////////////////////////////////////////////////////////
//���㱾�����ڵ����
//1.ͨ�������߶ι�ϵ��õ�ȫ�����߶Σ��ر��������ڵ��׶����ߵĸ��ɺ������û����
//			sec_p = dsbob2->r_zn_sec_tb->FindChildPtr1(zn->id);
//			sec_p = dsbob2->r_zn_sec_tb->FindChildPtr1(zn->r_sec);
//			
//			
//			//�õ��߶����������θ���,���е���Щͳ�ƶ�Ҫ������ģ�����׼ȷ
//			mxTemp = 0;
//			sec_p0 = sec_p; 
//			while(sec_p)
//			{
//				sec1 = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr);
//				
//				//�߶ε���ĩ�ڵ㹦��ֵ�����ֵ�Ƕ�̬�ģ���ÿ�������й�
//				mxP = fabs(sec1->w);
//				mxQ = fabs(sec1->wz);
//				mxP = mxP>mxQ ? mxP:mxQ;//�õ��ϴ���߶��й�		
//				
//				mxTemp = mxTemp>mxP ? mxTemp:mxP;
//				
//				sec_p = sec_p->Next;
//			}
//			::FreeChildPtr(sec_p0);
//			mxP = mxTemp;//�������ε�����й�����Ϊ�й����������������
//			
//			znP = 0;
//			multiP = 0;
//			multiC = 0;
//			if (iznC > 0)//�������и��ɵ�
//			{
//				//�������α������ҵ�������Σ��õ�r_ld,����r_ld��������
//				r_ld = zn->r_ld;
//				ld_p =  dsbob2->r_zn_ld_tb->FindChildPtr1(r_ld);
//				
//				//�õ��߶����������θ���
//				ld_p0 = ld_p; 
//				while(ld_p)
//				{
//					znP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);//ע���й��ۼ�
//					
//					ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
//					if ( (dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & (ND_SO) )//���Դ
//					{
//						multiP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);
//						multiC++;//���Դ�ĸ��������ۼ�
//					}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
//					
//					ld_p = ld_p->Next;
//				}
//				::FreeChildPtr(ld_p0);
//			}
//			
//////////////////////////////////////////////////////////////////////////
//�����������ε�ת�����
//			//���ε�����
//			noextbs = zn->type; 
//			if (noextbs > MAX_ZNNO)//Ӧ����ͼ�����ˣ�һ�㲻������������
//			{
//				continue;
//			}
///�õ�����-���ص��໥��ϵ��������ε�����ڵ�
//			cnt_u = 0;
//			//�߽翪�ش�ȡ����		
//			deltap=deltaq=deltaC=0;
//			sumP=sumQ=sumC=0;
//			deltaMultiP=deltaMultiQ=deltaMultiC=0;
//			unfd_idList[0]='\0';
//			double lossP_zn,lossQ_zn;
//			int lossC_zn;
//			lossP_zn=lossQ_zn=lossC_zn=0;
//			
//			r_cb = zn->r_cb;
//			if (r_cb != -1)//�����������ο���
//			{
//				cb_p = dsbob2->r_zn_cb_tb->FindChildPtr1(r_cb);
//	//			cb_p = dsbob2->r_zn_cb_tb->FindChildPtr1(zn->id);
//				
//				//�õ������ڵ�ȫ������,�����׶˿���
//				int ii=0;
//				cb_p0 = cb_p; 
//				while(cb_p)
//				{
//					cb1 = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr);
//					if (cb_p->Ptr == zn->cbptr)//�������εĿ�����Ҫ����
//					{
//						cb_p = cb_p->Next;
//						continue;				
//					}
//					extbs[ii] = cb1->iznd;//�߽翪�ص����ڵ��ĸ��
//					upextbs[ii] = cb1->ind;//�߽翪���ڲ�ڵ����������߶ζԲ�ڵ�ļ���ĸ��
//			
//					ii++;
//					cb_p = cb_p->Next;
//				}
//				::FreeChildPtr(cb_p0);
//				noextbs--;
//				//pextbs = extbs;
//				//pupextbs = upextbs;
//				//�����ڸ��ɣ��������û����������θ��ɣ����������û������������Ƿ�������
//				//������ֻ�贫���εĿ��ؾͿ�����upextbs�洢�������ε��ڲ�ĸ��
//				//extbs--�߽翪�������ӵ���һ���߶ε��׽ڵ㣨����ĸ�ߣ�
//				//upextbs--�߽翪�������ӵ��������߶ε�ĩ�ڵ㣨����ĸ�ߣ�
//				//�������ο���ת���Ĳ��ָ��ɺ��û�
//				
//				//deltaC���ο���ת�����û���deltaMultiC����˫��Դ�û���sumC�����ܵ��û���
//				//cnt_u = OnSearchUnion(noextbs,extbs,upextbs,deltap,deltaq,deltaC, \
//				//	sumP,sumQ,sumC,deltaMultiP,deltaMultiQ,deltaMultiC,znList,unfd_idList);//�����������翪������
//				//OnSearchUnion(1,deltap,deltaq,deltaC,extbs,noextbs,upextbs);//�����������翪������
//	///lld for test new nm1 program
//				
//				//noextbs,extbs,upextbs,��������������Ŀ��أ������Ƕ������
//				//deltap,deltaq,deltaC�����ο���ת���ĸ���
//				//sumP,sumQ,sumC,�����ܸ���
//				//lossP_zn,lossQ_zn,lossC_zn,������ʧ����
//				//deltaMultiP,deltaMultiQ,deltaMultiC,���ζ��Դ
//				//unfd_idList��ת��·��
//				cnt_u = OnSearchUnion(noextbs,extbs,upextbs,deltap,deltaq,deltaC, \
//					sumP,sumQ,sumC,lossP_zn,lossQ_zn,lossC_zn,deltaMultiP,deltaMultiQ,deltaMultiC,unfd_idList);
//			}
//						   
//////////////////////////////////////////////////////////////////////////
//����������ϣ�׼��������
//			if (cnt_u == 0)//û�����翪��
//			{
//				lossP  = -(fabs(mxP)-fabs(multiP)-fabs(deltaMultiP));//�����ڵ���󸺺�-�����ڵ�˫��Դ����-�������ε�˫��Դ����
//				transP = -fabs(multiP)-fabs(deltaMultiP);
//				lossC  = iznC +sumC-multiC-deltaMultiC;//�����ڸ��ɸ���+���θ��ɸ���-������˫��Դ�ĸ���-����˫��Դ�ĸ���
//				transC = multiC+deltaMultiC;
//			}		
//			else 
//			{
//				//ֻҪ�����翪�ض�����ת�� ��lld ncomment
//				lossP = -(fabs(mxP)-fabs(multiP)-fabs(deltap)-fabs(deltaMultiP));//�����ڵ���󸺺� - ���ο���ת���ĸ���
//				transP = -fabs(multiP)-fabs(deltap)-fabs(deltaMultiP);
//				lossC = iznC + sumC-deltaC-multiC-deltaMultiC;
//				transC = deltaC+multiC+deltaMultiC;
//			}
//			
//			//��������д����������
//			if (lossC == 0)
//				lossP = 0;
//			
//			if (transC == 0)
//				transP = 0;
//			
//			zn->lossC = lossC; //�����Լ������μ�����һ����ʧ���û�
//			zn->transC = transC;//���ο���ת�����û�
//			zn->lossP = lossP;
//			zn->transP = transP;
//			
//			zn->inner_C = iznC;//�������û�
//			zn->inner_P = znP-multiP;//�������й�
//			zn->next_C = sumC;//���������û�
//			if (sumC == 0)//��������û���û������ǿ�����Ϊ��Ĵ��ڣ����ο��ܻ��п����й��Ķ�������
//			{
//				zn->next_P = 0;
//			}
//			else
//			{
//				zn->next_P = fabs(mxP)-fabs(multiP)-(znP/0.962);//���������й�,����3.8%������ 
//			}
//			strcpy(zn->unfd_idList,unfd_idList);//������·����Ҫ����ȥ��ֻ����һЩ�仯
//		}		
//		//�������ڵ������Ϣ��ֵ�����������߶�
//        (sec+i)->lossP   = zn->lossP;		//���μ��������ܸ���
//        (sec+i)->transP  = zn->transP;      //�������ο���ת���ĸ��ɣ����β���Ҫת��
//        (sec+i)->lossC   = zn->lossC;	//�����Լ������μ�����һ����ʧ���û�
//        (sec+i)->transC  = zn->transC;	//���ο���ת�����û�
//		(sec+i)->lC_zn = zn->inner_C;//�������û�
//		(sec+i)->lP_zn = zn->inner_P;//�������й�
//		(sec+i)->lC_znnext = zn->next_C;//���������û�
//		(sec+i)->lP_znnext = zn->next_P;//���������й�
//		strcpy((sec+i)->tranfdlist,zn->unfd_idList);//ת��·��
//	}
// 	return 1;
//}

//ͨ�����εõ����β������������㼰���������Ϣ
//���β�������ĸ���ֵҲҪ�õ�
//Ҳ��Ҫ�õ�������������и��ɸ������������ֶο��ص�����
//int CNM1::getUNList(int noextbs,int* pextbs,int* pupextbs, \
//		int* sectCB,int nSectCB, int* load,int nLoad, NM1_UNLIST* unlist)
//{
//	int sum_unioncb/*�ܵ����翪����*/;
//	sum_unioncb=0;
//	double p_abund,q_abund,v_abund;//�����й����޹�����ѹԣ��
//	 nd_abund[80];//���翪�صĶԲ�ڵ�
//	 fdid_abund[80];//���翪�ضԲ�����߱��
//	BOOL bFind;
//	//û�п���ת���Ŀ���
//	if (noextbs == 0)
//	{
//		return 0;
//	}
//	int knd = pupextbs[0];//ͨ���ڵ���ҵ�Դ��
//////////////////////////////////////////////////////////////////////////
//ͨ���ڵ�õ������������������
//	 fdid[80];//��������������ID��������ǰ׺
//	ChildPtr1	*sec_p;//���ؿ�������2�����ߣ������ÿ��ص�fdid������ֻ�������߶�
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
//	//�����еĽڵ���Ϊδ����׼�����˷�����
//	//�˴��ƺ������Ż������û����ô��Ŀռ䣬����ֻ����صĽڵ�
//	for(i = 0; i < lv_nd; i++)
//	{
//		(nd+i)->q |= ND_UNPROC;
//	}
//	//��������Ϊ�Ѵ���
//	for(i = 0; i < lv_cb; i++)
//	{
//		(cb+i)->q |= ND_UNPROC;
//	}
//    //���δ������εı߽翪��
//	//ÿһ�����ض��Ƕ����Ĺ��磬�����֧֮������������ת��
//	//��1����һ������һ�����صĴ���
//	//      1.�ڴ�����·��ʱ��û�����磬��������1
//	//		2.��һ�����翪�أ���ԣ�Ȳ�������������2
//	//		3.�����ж�����翪�أ���Ҫ���ö����紦�����ת�����ˣ���������3
//	//��2��ÿ�����ؾ�������ϣ����ִ������⣬ȫ�����ö�����ģ������ع�
//	
//	for(ii = 0; ii < noextbs; ii++)
//	{
//		knd = pupextbs[ii];//ȡ���ص����νڵ�����ж�
//		cb_id = -1;
//////////////////////////////////////////////////////////////////////////
//������Ŀ����Լ��ҵ������д������������ͷ�λ��ֱ������
//		//����߽翪�ر����ʹ��ڷ�λ��ֱ������
//		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//		cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
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
//			if (kknd == pextbs[ii])//�߽翪��,�����Լ��ҵ�
//			{
//				cb_id = cb_p->Ptr;
//				break;
//			}
//			cb_p = cb_p->Next;
//		}
//		::FreeChildPtr(cb_p0);
//		if (cb_id != -1)//һ�㲻������Ҳ����Լ������
//		{
//			dsbob2->cb_tb->GetDataByPh(cb_id)->q &= (~ND_UNPROC);
//			//�������Ҫ�ӵ��Ͽ��Ŀ������棬����������ͨ����
//			
//			//����һ�����������翪�أ�����ڿ���������Ƚ��ձ�
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_NMLOPEN)  
//			{
//				continue;
//			}
//			
//			//�����Ŀ����Լ��Ƿ�λ���أ�ͣ����
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_OPEN)
//			{
//				continue;
//			}
//		}
//���뿪���Լ�������ϣ�׼����������ؿ�ʼ�����Լ�·����׷��
//////////////////////////////////////////////////////////////////////////
//		ind = pextbs[ii];//ĸ�ߵ���ʼ�ڵ���
//����һ���ڵ���Ϊ�Ѵ���
//		(nd+pupextbs[ii])->q &= (~ND_UNPROC);
//		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
//		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
//		{
//			(nd+ind)->q &= (~ND_UNPROC);
//			//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
//			//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
//			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
//			(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
//     		while (1)//������չ����û�д���
//			{
//				//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
//				if ((nd+knd)->q & ND_LD) 
//				{
//					//�õ�����id
//					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
//					ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
//					while(ld_p)
//					{
//						//����˫��Դ����,���ԴĬ��Ϊ��ͣ�磬��n-1����������Դ��
//						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
//						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
//							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//���Դ
//						{
//							continue;	
//							//deltaMultiP += (double)(dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w);
//							//deltaMultiC++;
//						}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
//						//ֱ���޸ĸ��ɵ�ID��
//						//sumP += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
//						//sumQ += dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->r;
//						ld_p = ld_p->Next;
//					}
//					::FreeChildPtr(ld_p0);
//				}
//				//read cbs connected to this nd
//				//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
//				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
//				while(cb_p)
//				{
//					//��������翪�أ�ͣ�������ҵ����������ĵ�Դ��
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_NMLOPEN)  
//					{
//						//�����������Ϊ�Ѵ���
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);
//						//����ֻ��һ����λ�Ŀ����ǲ����жϵģ�����û���������·����
//						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
//						{
//							//kkndcb,��Ϊ����ڵ㣬�ҵ�һ�����翪��
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
//							//�ж����翪�ص�ԣ�ȣ�
// 							getUNCB_abun(fdid,scb_id,nd_abund, \
//										fdid_abund,p_abund,q_abund,v_abund);
//							
//							//��Ȼ�����翪�أ���û��������·�����翪�������յ�
//							if (fdid_abund != "")
//							{
//								bFind = FALSE;
//								//������翪�ص�ĸ�ߺ���ǰ�����翪��ĸ��һ��
//								for (i=0;i<sum_unioncb;i++)
//								{
//									//�Ƚ����翪��ԣ�ȣ�ѡ����ԣ��
//									if ( (nd+unlist[i].nd)->ibs == (nd+knd)->ibs )
//									{
//										//�滻
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
//					//ֻҪ���������Ŀ��ض���������,�Ѿ�������Ĳ�����
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & ND_UNPROC) //this so nd has not processed
//					{
//						//ֻ�����������ϵĿ��أ�CB_OPANABL
//						//�����������Ϊ�Ѵ���
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~ND_UNPROC);
//						//printf("%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q);
//					}
//					//����Ƿ�λ���أ�ͣ����
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_OPEN)
//					{
//						cb_p = cb_p->Next;
//						continue;
//					}
//					//find the opposite nd through this cb
//					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
//					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//					}
//					//process this opposite nd
//					//����Բ�ڵ� δ����==���ز����������ƽ�
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//�½ڵ� ��� ��չ�ڵ��ǰ����
//						(nd+knd)->fch = kknd;
//						//���ҵ����½ڵ�����Ѵ���
//						(nd+kknd)->q &= (~ND_UNPROC);
//					}
//					cb_p = cb_p->Next;
//				}
//				::FreeChildPtr(cb_p0);
//				//read secs connected to this ndͨ���߶�������չ
//				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//				sec_p0 = sec_p;
//				while(sec_p)
//				{
//				    //�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
//				    //�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
//            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
//            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
//		            
//					if (r_zn == -1)//���߶β������κ�����
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
//						//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
//						//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
//						//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
//						
//						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//�½ڵ� ��� ��չ�ڵ��ǰ����
//						(nd+knd)->fch = kknd;
//						//���ҵ����½ڵ�����Ѵ���
//						(nd+kknd)->q &= (~ND_UNPROC);
//					}
//					sec_p = sec_p->Next;
//				}
//				::FreeChildPtr(sec_p0);
//				//move to next nd
//				knd = (nd+knd)->fch;
//				//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
//				//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
//				//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
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

//������Щ��ת������Щ����ת������ת���ĸ�����
//�ǹ��϶���ת�Ƹ���(MW)	�ǹ��϶�ʵ��ת�Ƹ���(MW)  ת����Դ
int CNM1::residueAnalyze(int noextbs,int* pextbs,int* pupextbs)
{
	//����ֻ�ǽ������˵��Ļ��֣�������Ҫ��һ���������
	if (!getResidueStruct(noextbs,pextbs,pupextbs))//����û�����翪��
		return 0;

	//��������ۻ����У�Ҫ����ת���Ͳ���ת���ĵ��������
	if (AnanyseIsland() == 0)//��������������ͨ����˵���ǹµ�
		return 0;

	if (AnanyseResult(1) == 0)//�����������ķ������
		return 0;

	return 1;
}

//�����������磬�õ�ÿ���������翪����Ϣ���ֶο�����Ϣ��������Ϣ��������
//ͨ���������εı߽翪�أ��õ��߽翪�����ӵ�ÿһ������������п���������
//Ҳ�п����ǻ����ʹ�ǻ��Ҳ�п����Ƕ����������粻������ת��
//int CNM1::getResidueStruct(int noextbs,int* pextbs,int* pupextbs)
//{//int degbug=1;
//	int ndCount,ldCount,cbCount,secCount,cpCount,unCount,soCount;//һ�����ڵ�����
//	double p_abund,q_abund,v_abund,pq_abund;//�����й����޹�����ѹԣ��
//	 nd_abund[MAX_SEC_ID_LEN];//���翪�صĶԲ�ڵ�
//	 fdid_abund[MAX_SEC_ID_LEN];//���翪�ضԲ�����߱��
//	BOOL bFind;
//
//	int knd,lv_nd,lv_cb,lv_cp,lv_so;
//	 fdid[MAX_SEC_ID_LEN];//��������������ID��������ǰ׺
//	int			i,ii,j;
//	int			ind, kknd,ld_nd,cb_id,r_zn,pcb_temp;
//	ChildPtr1	*cb_p, *cb_p0;
//	ChildPtr1   *sec_p0;
//	ChildPtr1	*ld_p, *ld_p0;
//	 scb_id[MAX_SEC_ID_LEN];//���翪�ضԲ������id����
//
//	ChildPtr1	*sec_p;//���ؿ�������2�����ߣ������ÿ��ص�fdid������ֻ�������߶�
//	
//	//û�п���ת���Ŀ���
//	if (noextbs == 0)
//	{
//		return 0;
//	}
//
//	knd = pupextbs[0];//ͨ���ڵ���ҵ�Դ��
//	ND* nd = dsbob2->nd_tb->GetTable();
//	lv_nd = dsbob2->nd_tb->GetCount();
//	
////	sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
////	sec_p0 = sec_p;//����ָ�����ʱ���棬ָ�������͵�����
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
//	//�����еĽڵ���Ϊδ����׼�����˷�����
//	//�˴��ƺ������Ż������û����ô��Ŀռ䣬����ֻ����صĽڵ�
//	for(i = 0; i < lv_nd; i++)
//	{
//		(nd+i)->q |= ND_UNPROC;
//	}
//
//	//��������Ϊ�Ѵ���
//	for(i = 0; i < lv_cb; i++)
//	{
//		(cb+i)->q |= CB_N_RESTORE;
//	}
//
//    //���δ������εı߽翪��
//	//ÿһ�����ض��Ƕ����Ĺ��磬�����֧֮������������ת��
//	g_island = 0;
//	for(ii = 0; ii < noextbs; ii++)
//	{
//		knd = pupextbs[ii];//ȡ���ص����νڵ�����ж�
//
//		cb_id = -1;
////////////////////////////////////////////////////////////////////////////
////������Ŀ����Լ��ҵ������д������������ͷ�λ��ֱ������
//		//����߽翪�ر����ʹ��ڷ�λ��ֱ������
//		cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//		cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
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
//			if (kknd == pextbs[ii])//�߽翪��,�����Լ��ҵ�
//			{
//				cb_id = cb_p->Ptr;
//				break;
//			}
//			cb_p = cb_p->Next;
//		}
//		::FreeChildPtr(cb_p0);
//
//		if (cb_id != -1)//һ�㲻������Ҳ����Լ������
//		{
//			dsbob2->cb_tb->GetDataByPh(cb_id)->q &= (~ND_UNPROC);
//			//�������Ҫ�ӵ��Ͽ��Ŀ������棬����������ͨ����
//			//firstCB[ii] = cb_id;//�������Ŀ�����Ϊ����Ͽ��Ŀ���
//			//����������ڱ���Ҫ�Ͽ��Ŀ��أ���������ʱ����Բ���Ҫ�ӵ��ֶ�����
//			//ֻ�м��뵽�ṹ����ķֶο��ز�������
//
//			//����һ�����������翪�أ�����ڿ���������Ƚ��ձ�
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & Q_UNIONCB)  
//			{
//				continue;
//			}
//			
//			//�����Ŀ����Լ��Ƿ�λ���أ�ͣ����
//			if (dsbob2->cb_tb->GetDataByPh(cb_id)->q & CB_OPEN)
//			{
//				continue;
//			}
//		}
//
////���뿪���Լ�������ϣ�׼����������ؿ�ʼ�����Լ�·����׷��
////////////////////////////////////////////////////////////////////////////
//		ndCount = cbCount = cpCount = secCount = unCount = ldCount = soCount = 0;
//		ind = pextbs[ii];//ĸ�ߵ���ʼ�ڵ���
//
////����һ���ڵ���Ϊ�Ѵ���
//		(nd+pupextbs[ii])->q &= (~ND_UNPROC);
////		islandlist[g_island].island_ndlist[ndCount++].nd_ptr = pupextbs[ii];
//
//		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
//		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
//		{
//			(nd+ind)->q &= (~ND_UNPROC);
//			islandlist[g_island].island_ndlist[ndCount++].nd_ptr = ind;
//
//			//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
//
//			//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
//			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
//			(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
//     		while (1)//������չ����û�д���
//			{
//				//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
//				if ((nd+knd)->q & ND_LD) 
//				{
//					//�õ�����id�����ɺ���Ҫ���������еĶ�Ҫ��������Ե����������д���
//					ld_p = dsbob2->r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
//					ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
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
//						//����˫��Դ����
//						ld_nd = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//���ɶ�Ӧ�Ľڵ��
//						if ( ((dsbob2->nd_tb->GetDataByPh(ld_nd)->q) & ND_SO) )
//							//&& ((dsbob->nd_tb->GetDataByPos(ld_nd)->q) & ND_LD) )//���Դ
//						{
//							islandlist[g_island].island_ldlist[ldCount].soSum = 2;
//						}//���Դ�ڼ���ͣ����ʧ��ʱ��Ҫ���ǵ�
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
//					//��so����ȫѭ������Ϊso����������,�����Ƿֲ�ʽ��Դ
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
//					//��cp����ȫѭ������Ϊcp����������
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
//				//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
//				cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
//				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
//				while(cb_p)
//				{
//					//��������翪�أ�ͣ�������ҵ����������ĵ�Դ��
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)  
//					{
//						//�����������Ϊ�Ѵ���
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);
//
//						//����ֻ��һ����λ�Ŀ����ǲ����жϵģ�����û���������·����
//						if (lld->isContainor(dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
//						{
//							//kkndcb,��Ϊ����ڵ㣬�ҵ�һ�����翪��
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
//							//�ж����翪�ص�ԣ��
//							p_abund=q_abund=v_abund=0;
// 							getUNCB_abun(fdid,scb_id,nd_abund, \
//										fdid_abund,p_abund,q_abund,v_abund);
//							if ( (p_abund==0) && (q_abund==0) && (v_abund==0) )
//							{
//								pq_abund = 5;//Ĭ����������ԣ��
//								v_abund = 1.05;
//							}
//							else
//								pq_abund = sqrt(p_abund*p_abund + q_abund*q_abund);
//							
//							//��Ȼ�����翪�أ���û��������·�����翪�������յ�
//							if (fdid_abund != "")
//							{
//								bFind = FALSE;
//								//������翪�ص�ĸ�ߺ���ǰ�����翪��ĸ��һ��
//								for (i=0;i<unCount;i++)
//								{
//									//�Ƚ����翪��ԣ�ȣ�ѡ����ԣ��
//									pcb_temp = islandlist[g_island].island_unlist[i].nd;
//									if ( (nd+pcb_temp)->ibs == (nd+knd)->ibs )
//									{
//										//�滻
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
//					//ֻҪ���������Ŀ��ض���������,�Ѿ�������Ĳ�����
//					if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_N_RESTORE) //this so nd has not processed
//					{
//						//printf("%s,%d\n",dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->id,dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->maincb);
//						//����Ƿ�λ���أ������ķ�Ϊ���أ���ҲҪ�������翪�ش���������ԣ��Ϊ���޴�
//
//						//�����������Ϊ�Ѵ���
//						dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q &= (~CB_N_RESTORE);
//
//						//����ֻ��һ����λ�Ŀ����ǲ����жϵģ�����û���������·����
//						//!!!!!!!�ڲ����翪��
//						if (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)
//						{
//							//kkndcb,��Ϊ����ڵ㣬�ҵ�һ�����翪��
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
//							//�ж����翪�ص�ԣ��
//							p_abund = q_abund = pq_abund = 20;
//							
//							//��Ȼ�����翪�أ���û��������·�����翪�������յ�
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
//							//�������ǰ�ڷ�����ʱ���趨Ϊ�⻷���أ�����Ҫ����ָ�
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
//					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
//					if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
//					}
//					else
//					{
//						kknd = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
//					}
//					//process this opposite nd
//					//����Բ�ڵ� δ����==���ز����������ƽ�
//					if((nd+kknd)->q & ND_UNPROC)
//					{
//						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//�½ڵ� ��� ��չ�ڵ��ǰ����
//						(nd+knd)->fch = kknd;
//						//���ҵ����½ڵ�����Ѵ���
//						(nd+kknd)->q &= (~ND_UNPROC);
//						islandlist[g_island].island_ndlist[ndCount++].nd_ptr = kknd;
//					}
//
//					cb_p = cb_p->Next;
//				}
//				::FreeChildPtr(cb_p0);
//
//				//read secs connected to this ndͨ���߶�������չ
//				sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
//				sec_p0 = sec_p;
//				while(sec_p)
//				{
//				    //�����߶��ǲ����µ����Σ�����ǣ�������뵽Ӱ����������
//				    //�����߶�����֮��Ĺ�����ϵ�ҵ���Զ�Ӧ������
//            		r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1( \
//            		    dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->r_zn);
//		            
//					if (r_zn == -1)//���߶β������κ�����
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
//						//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
//						//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ����
//						//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����
//						
//						//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
//						(nd+kknd)->fch = (nd+knd)->fch;
//						//�½ڵ� ��� ��չ�ڵ��ǰ����
//						(nd+knd)->fch = kknd;
//						//���ҵ����½ڵ�����Ѵ���
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
//				//ѭ����ͷ���������ˣ���ʾȫ������һ�飬��ʾ�����һ��ת��·��
//				//���ת��·��Ҳ�п������ж�����翪�صģ�ÿ�����翪�ش�һ���ָ���
//				//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
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
//					islandlist[g_island].canTrans = 1;//�ȼ�����ת
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







