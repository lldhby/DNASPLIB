////////////////////////////////////////////////////////////////////////////////
//
//  TP.CPP
//  Methods implementation of TP class
//   
////////////////////////////////////////////////////////////////////////////////
#if _WIN32
#elif __linux__
#else
#endif

#include "../DNASPLIB/LLD.h"
#include "../DNASPDLLLIB/Tp.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// constructor
TP::TP(DSBOB2* m_pDb, CLLD* m_pLLD)
{
	//����������γɼ���ĸ�ߺ����˵���

	dsbob2		= m_pDb;
	lld = m_pLLD;
	//	digital_tb	= dsbob2->digital_tb;
//	analog_tb	= dsbob2->analog_tb;
	co_tb		= dsbob2->co_tb;
//	st_tb		= dsbob2->st_tb;
	if (lld->mx_xfmr > 0)
		xfmr_tb		= dsbob2->xfmr_tb;
	if (lld->mx_cb > 0)
		cb_tb		= dsbob2->cb_tb;
	so_tb		= dsbob2->so_tb;
	ld_tb		= dsbob2->ld_tb;
	if (lld->mx_cp > 0)
		cp_tb		= dsbob2->cp_tb;

	if (lld->mx_bus > 0)
		bus_tb		= dsbob2->bus_tb;
	//if (lld->mx_zn > 0)
	//	zn_tb		= dsbob2->zn_tb;
	sec_tb		= dsbob2->sec_tb;
	if (lld->mx_xf > 0)
		xf_tb = dsbob2->xf_tb;
	nd_tb		= dsbob2->nd_tb;

	bs_tb		= dsbob2->bs_tb;	
	island_tb	= dsbob2->island_tb;
	global_tb	= dsbob2->global_tb;

//	tapty_tb	= dsbob2->tapty_tb;
//	r_co_st_tb	= dsbob2->r_co_st_tb;
//	r_st_xfmr_tb= dsbob2->r_st_xfmr_tb;
	if (lld->mx_r_xfmr_xf > 0)
		r_xfmr_xf_tb= dsbob2->r_xfmr_xf_tb;
//	r_st_cb_tb	= dsbob2->r_st_cb_tb;
//	r_st_so_tb	= dsbob2->r_st_so_tb;
//	r_st_ld_tb	= dsbob2->r_st_ld_tb;
//	r_st_cp_tb	= dsbob2->r_st_cp_tb;
//	r_st_bus_tb = dsbob2->r_st_bus_tb;
//	r_st_zn_tb	= dsbob2->r_st_zn_tb;
	//r_zn_ld_tb	= dsbob2->r_zn_ld_tb;
	//r_zn_sec_tb = dsbob2->r_zn_sec_tb;
	//r_zn_cb_tb = dsbob2->r_zn_cb_tb;

	if (lld->mx_cb > 0)
		r_nd_cb_tb	= dsbob2->r_nd_cb_tb;
	if (lld->mx_xf > 0)
		r_nd_xf_tb	= dsbob2->r_nd_xf_tb;
//	r_nd_zn_tb	= dsbob2->r_nd_zn_tb;
	r_nd_sec_tb = dsbob2->r_nd_sec_tb;
	if (lld->mx_cp > 0)
		r_nd_cp_tb  = dsbob2->r_nd_cp_tb;
	r_nd_ld_tb	= dsbob2->r_nd_ld_tb;
}

////////////////////////////////////////////////////////////////////////////////
//  destructor
TP::~TP()
{
}

////////////////////////////////////////////////////////////////////////////////
//  tp_allnew()
int	TP::tp_allnew()
{
	lv_bs = lld->mx_bs = lld->mx_nd = nd_tb->GetCount();
	if (lv_bs)
		bs_tb_t         = new DTable<BS>(lv_bs);

	lv_island = lld->mx_nd;//���������һ���ڵ�һ����
	if (lv_island)
		island_tb_t         = new DTable<ISLAND>(lv_island);
	
	//get record count in tables
	//���������йصı�����еļ�¼���õ�
	if(tp_table_count() == TP_FAIL)
	{
		return TP_FAIL;
	}

	//get records in tables
	if(tp_table_get() == TP_FAIL)
	{
		return TP_FAIL;
	}

	//sort nds into bs
	//��ÿһ���ڵ���б����������ؽڵ���кϲ�����ÿ���ڵ㶼���ϼ���ĸ��ֵ���ж�ĸ���Ƿ��ǻ��
	//1.��ȫ�ֱ�������һ������ĸ�ߵı���ۼ���������һ������ĸ�ߵı��ȡ��
	//2.���ȴ����أ��ҵ�����ڵ����еĿ��أ���������Ǳպϵģ������ڵ�ͶԲ�ڵ㶼���ϼ���ĸ�ߺ�
	//����ʱ���Ҳ�������Ϊ������
	//3.�ҵ�������ڵ������ӵ�������߶��������Ƿ����и��ɽڵ㣬��Դ�ڵ㣬���ݽڵ�
	//�����Щ��û�еĻ���˵�����ĸ������ĸ�ߣ�������ڵ���������νڵ㶼�����ĸ��
	if(tp_nd_sort() == TP_FAIL)
	{
		return TP_FAIL;
	}

	//sort bs into island
	//���Ѿ��趨�ļ���ĸ�߽��д���
	//1.�ӵ�ǰ�ĵ���õ��µĵ�����ţ��ӵ�Դ�㿪ʼ���д������ҵ�Դ�㿪ʼ������ĸ�ߣ�
	//����Ѿ�����������û�д����������չ��ֱ��û��ĸ�߿���ѡ��
	//2.��ĸ�߱����ѡ���Ѿ���չ����ĸ�ߣ�ȫ�����q�ǻ�ģ����ϵ��ı��
	//3.�Լ���ĸ�߱����ѭ�������q���ǻ�ģ��������������䵺�Ÿ�ֵΪ-1
    if(tp_bs_sort() == TP_FAIL)
    {
		return TP_FAIL;
	}

	//determine bs and status of each component
	if(tp_comp_status() == TP_FAIL)
	{
		return TP_FAIL;
	}

	if (lv_bs > 0)
	{
		//!!!!��ʱ���Բ�����������Ϊ��Ĭ�ϵ�-1������
		lld->mx_bs = bs_tb_t->Sort(lv_bs,0);
		bs_tb = new DTable<BS>(lld->mx_bs);
		if(lld->mx_bs != 0)
		{
			BS* temp = new BS[lld->mx_bs];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			bs_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			bs_tb->Insert(temp,lld->mx_bs);
			dsbob2->bs_tb = bs_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete bs_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		bs_tb_t=0;		
	}

	if (lv_island > 0)
	{
		//!!!!��ʱ���Բ�����������Ϊ��Ĭ�ϵ�-1������
		lv_island = lld->mx_island = island_tb_t->Sort(lv_island,0);
		island_tb = new DTable<ISLAND>(lld->mx_island);
		if(lld->mx_island != 0)
		{
			ISLAND* temp = new ISLAND[lld->mx_island];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			island_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			island_tb->Insert(temp,lld->mx_island);
			dsbob2->island_tb = island_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete island_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		island_tb_t=0;		
	}

	//��ӵ��͵�Դ�㡢����֮��Ĺ�ϵ
	tp_trim_cord();

	return TP_SUCCEED;
}

//�����ϵ
int TP::tp_trim_cord()
{
	int i = 0,ph=-1,islandph=-1;
	FD* fd1 = new FD();//dsbob2->fd_tb->GetTable();
	for (i = 0; i < lv_so; i++)
	{
		strcpy(fd1->id, dsbob2->so_tb->GetDataByLog(i)->fdid);
		ph = dsbob2->fd_tb->FindPh(*fd1);
		islandph = dsbob2->so_tb->GetDataByLog(i)->iisland;
		if ((ph != -1) && (islandph!=-1) )
		{
			dsbob2->island_tb->GetDataByPh(islandph)->reffd = ph;
		}
	}
	delete fd1;

	//ofstream outfile("bs_island.txt", ios::out);
	//outfile << "   " << endl;
	//outfile << "island:   " << endl;

	//ISLAND* island = dsbob2->island_tb->GetTable();
	//int count = dsbob2->island_tb->GetCount();
	//outfile << "index       " << "id    " << "reffd" << endl;
	//for (int i = 0; i < count; i++)
	//{
	//	outfile << i << "  " << (island + i)->id << "   " << (island + i)->reffd << endl;
	//}
	//outfile.close();


	return 1;
}
////////////////////////////////////////////////////////////////////////////////
//  tp_table_count()
int TP::tp_table_count()
{
//	lv_st = st_tb->GetCount();
//	lv_xfmr = xfmr_tb->GetCount();

	if (lld->mx_xf > 0)
		lv_xf = xf_tb->GetCount();
	if (lld->mx_cb > 0)
		lv_cb = cb_tb->GetCount();
	if (lld->mx_so > 0)
		lv_so = so_tb->GetCount();
	if (lld->mx_ld > 0)
		lv_ld = ld_tb->GetCount();
	if (lld->mx_cp > 0)
		lv_cp = cp_tb->GetCount();
	if (lld->mx_bus > 0)
		lv_bus = bus_tb->GetCount();
	
	//lv_zn = zn_tb->GetCount();
	lv_sec = sec_tb->GetCount();
	lv_nd = nd_tb->GetCount();
	//lv_bs = bs_tb->GetCount();

	return TP_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  tp_table_get()
int TP::tp_table_get()
{
	int		i;

	//get nd table
	nd = nd_tb->GetTable();

	//get bs table
	//���ĸ����Ĭ����2000��
	//��bs_tb��������ݣ�ǰ��2000�����ǰ�˳����
//	if(lv_bs != lld->mx_bs)
//	{
		bs = new BS[lv_bs];
		for(i = 0; i < lv_bs; i++)
		{
			(bs+i)->id = i;
		}
		bs_tb_t->Empty();
		lv_bs = bs_tb_t->Insert(bs, lv_bs);
		delete[] bs;
//		if(lv_bs != lld->mx_bs)
//		{
//			printf("fail to insert bs table\n");
//			return TP_FAIL;
//		}
//	}
//	bs = bs_tb->GetTable();

	//get cb table
	if (lld->mx_cb > 0)
		cb = cb_tb->GetTable();
	
	if (lld->mx_xf > 0)
		xf = xf_tb->GetTable();

	//get global table
	global = global_tb->GetTable();

	//get so table
	so = so_tb->GetTable();

	//get nd table
	nd= nd_tb->GetTable();

  	sec = sec_tb->GetTable();

	return TP_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  tp_nd_sort()	��������������صĽڵ���кϲ�
//�ر�ע�⣺�ڿ��غϲ��󣬼���ĸ���Ѿ�û���˿��ص���Ϣ
//��Ҫ�ڼ���ĸ��������ϸ�ĸ�ߺ��ж��ٸ����翪�أ����翪�صı��
//ÿ�������|�������������������翪�ص�ʱ��Ҳ����֪�����м�������
//һ�������һ������ĸ��ֻ��һ�����翪��
int TP::tp_nd_sort()
{
	int			i;
	int			knd;
	int			kknd;
	int			ibs;
	int			lastbs;
    int			anyshunt;
    int			loadgen;
    int			notopenbs;
	int			qndgrp;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;

	bool hasUnioncb = false;//ĸ���Ϲ������翪��
//	CString cbid = "";
	bool isMultiSo = false;//�����Ƕ��Դ��1-����Դ��2-˫��Դ��3-����Դ������

	bs = bs_tb_t->GetTable();

	//initialize nd list and mark all nds as unprocessed
	//�����еĽڵ��ĸ�߱�Ŷ��ó�0������Ϊδ����
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->ibs = 0;
		//(nd + i)->fch = -1;//lld add 2019-4-13
		(nd+i)->primebs = 0;  //we don't form primebs in this version
		(nd+i)->q |= ND_UNPROC;
	}

	//initialize bs list
	for(i = 0; i < lv_bs; i++)
	{
		(bs+i)->iisland = -2;
		(bs+i)->q = 0;
		(bs+i)->fbs = i + 1;
		(bs+i)->pnd = -1;
	}

	//initialize the nullbs chain
	i = 0;//��һ��ĸ�ߡ���ʼĸ��
	(bs+i)->fbs = i;  //point to itself
	(bs+i)->bbs = i;
	(bs+i)->iisland = -1;
	(bs+i)->q |= BS_DEAD;//ĸ�ߵı�־λ����ʼ��Ϊ�����������ǿ���
	(bs + i)->q |= BS_OPEN;

	//last bs point to -1
	i = lld->mx_bs - 1;
	(bs + i)->fbs = -1;

	//initialize global pointers to bs list
	global->nullbs = 0;
	global->nextbs = 1;
	//
	//	//initialize last inserted bs
	lastbs = global->nullbs;

	//sort nds into bs
	//һ���ڵ�һ���ڵ�Ĵ�����ȡһ���ڵ㣬Ȼ��������ڵ����չ�ڵ㣬����Ҫ����
	//������ڵ�����м���ĸ�߶�ȡ����
	for (i = 0; i < lv_nd; i++)
	{
		if ((nd + i)->q & ND_UNPROC)  //this nd has not processed
		{
			//get bs number for this nd
			ibs = global->nextbs;//�����Ǹ�����ĸ�ߺţ�ÿ���һ����������1

			if (ibs == -1 || ibs > lv_bs)
			{
				printf("out of memory\n");
				return TP_FAIL;
			}

			//create circular chain for this nd
			knd = i;//knd==0��ʼ���нڵ���չ
			(nd + knd)->fch = knd;//�趨ǰ����
			(nd + knd)->q &= (~ND_UNPROC);//���ýڵ�����Ѵ���
			(nd + knd)->ibs = ibs;//������ڵ������ڵļ���ĸ�ߺ����뵽�ڵ�����

			//initialize the nd group
			qndgrp = 0; //q of the nd group
			g_nbr = 0;	//branch number from the nd group

			//����Ҫ��һ���ڵ�����չ����ѭ����һֱ���Ҳ���������չ�Ľڵ�Ϊֹ
			while (1)
			{
				//�ڵ�Ⱥ������趨Ϊ0��ÿѭ��һ�ν����Ⱥ�������һ����־λ
				qndgrp |= (nd + knd)->q;

				//xf��ͨ���ڵ�-����Ĺ�ϵ��������ڵ��й�������������ȡ����
				g_nbr += r_nd_xf_tb->FindChildNum((nd + knd)->r_xf);

				//sec��ͨ���ڵ�-�߶εĹ�ϵ��������ڵ��й������߶�����ȡ����
				g_nbr += r_nd_sec_tb->FindChildNum((nd + knd)->r_sec);

				//����0�迹֧· add by lld 2015-4-29
				sec_p = r_nd_sec_tb->FindChildPtr1((nd + knd)->r_sec);
				sec_p0 = sec_p;//׼�������֧·������չ���Ȱ�������Ǹ�֧·��ʱ������
				while (sec_p!=NULL)//cb_p��������ڵ���������е�֧·��ָ�뼯��
				{
					if (((sec + sec_p->Ptr)->r1 == 0) && ((sec + sec_p->Ptr)->x1 == 0))
					{
						//find the opposte nd through this cb
						//�ҵ��Բ�ڵ㲢������ڵ��ŵ�kknd
						if (knd == (sec + sec_p->Ptr)->ind)
						{
							kknd = (sec + sec_p->Ptr)->iznd;
						}
						else
						{
							kknd = (sec + sec_p->Ptr)->ind;
						}

						//process this opposite nd
						//����Բ�ڵ�û�д������Բ�ڵ���Ϊ�Ѵ�����ǰ�����ͺ�����
						//���ڵ�ϲ���һ������ĸ�ߣ�����ͬһ������ĸ�ߺ�д��Բ�Ľڵ����ĸ�ߺ�
						if ((nd + kknd)->q & ND_UNPROC)
						{
							(nd + kknd)->fch = (nd + knd)->fch;
							(nd + knd)->fch = kknd;
							(nd + kknd)->q &= (~ND_UNPROC);
							(nd + kknd)->ibs = ibs;//�ҵ��Բ�ڵ�󣬻��Ǹ��ڵ㸳һ����ͬ��ĸ�ߺ�

						}
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);//�ͷ�ָ��

				//go through cbs connected to this nd
				//�Կ��ؽ��д�����Ҫ�ǿ��صĽڵ���Ҫ�ϲ������ص�2���ڵ�ϲ���һ���ڵ�
				//�Կ��ؽڵ���Ҫ��ȫ���Ŀ��ض������һ���ڵ�

				//ͨ���ڵ�-���ع�����������ڵ�������Ŀ���ȡ�����ŵ�ϵ����
				//�ر�������������ĸ��ʱ���кܶ�Ŀ���Ҫ�ӵ�����ϵ����
				cb_p = r_nd_cb_tb->FindChildPtr1((nd + knd)->r_cb);
				cb_p0 = cb_p;//׼����������ؽ�����չ���Ȱ�������Ǹ�������ʱ������
				while (cb_p != NULL)//cb_p��������ڵ���������еĿ��ص�ָ�뼯��
				{
					//if ( ((cb + cb_p->Ptr)->ind == 200) || ((cb + cb_p->Ptr)->iznd == 200) )
					//{
					//int debug = 0;
					//}

					//ֻ�п����Ǳպϵ�״̬��������չ
					if (!((cb+cb_p->Ptr)->q & CB_OPEN))
					{
						if (!((cb+cb_p->Ptr)->q & Q_UNIONCB))  //cb is closed
						{
							if ( (strcmp((cb+cb_p->Ptr)->nd,"N") != 0) && (strcmp((cb+cb_p->Ptr)->znd,"N") != 0) )
							{
								if (!((cb+cb_p->Ptr)->q & CB_FDLCB))  //cb is closed
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

									if (kknd == -1)//���տ���
									{
										cb_p = cb_p->Next;
										continue;
									}

									//process this opposite nd
									//����Բ�ڵ�û�д������Բ�ڵ���Ϊ�Ѵ�����ǰ�����ͺ�����
									//���ڵ�ϲ���һ������ĸ�ߣ�����ͬһ������ĸ�ߺ�д��Բ�Ľڵ����ĸ�ߺ�
									if((nd+kknd)->q & ND_UNPROC)
									{
										(nd+kknd)->fch = (nd+knd)->fch;
										(nd+knd)->fch = kknd;
										(nd+kknd)->q &= (~ND_UNPROC);
										(nd+kknd)->ibs = ibs;//�ҵ��Բ�ڵ�󣬻��Ǹ��ڵ㸳һ����ͬ��ĸ�ߺ�

									}
								}
							}
						}
					}
					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);//�ͷ�ָ��

				//get next nd in the present nd group
				knd = (nd+knd)->fch;//��һ���ڵ����ǰ���ڵ㣬ǰ����Ǹ�
				if(knd == i) break;
			}

			//check if the bs is open
			//�ж�ĸ���Ƿ������е�Դ�㡢���ɵ㡢���ݵ�
			anyshunt = (qndgrp & ND_SO) || (qndgrp & ND_LD) || (qndgrp & ND_CP);
			loadgen = (qndgrp & ND_SO) || (qndgrp & ND_LD);
			notopenbs = (g_nbr > 1) || (g_nbr == 1 && anyshunt) || loadgen;

			//mark the bs if open
			if(notopenbs)
			{
				(bs+ibs)->q &= (~BS_OPEN);

				//loop through the nd group
				knd = i;
				do
				{
					(nd+knd)->q &= (~ND_OPEN);
					knd = (nd+knd)->fch;
				}	while(knd != i);
			}
			else
			{
				(bs+ibs)->q |= BS_OPEN;

				//loop through the nd group
				knd = i;
				do
				{
					(nd+knd)->q |= ND_OPEN;
					knd = (nd+knd)->fch;
				}	while(knd != i);
			}

			//insert bs into circular chain  ��ѭ������ﵽ�ǵݹ����������Ŀ��
			//���ĸ�ߵ�ǰ��ͺ�����
			(bs+ibs)->fbs = (bs+lastbs)->fbs;
			(bs+ibs)->bbs = lastbs;
			(bs+(bs+lastbs)->fbs)->bbs = ibs;
			(bs+lastbs)->fbs = ibs;
			(bs+ibs)->pnd = i;//ͨ������ĸ��pnd�����ҵ��ڵ��

			//move to next bs
			lastbs = global->nextbs++;
		}
	}

	//�����еļ���ĸ�߶��Ѿ����ɺ󣬾Ϳ��Եõ���ʵ�ļ���ĸ����
	lld->mx_bs = global->nextbs;
	//�Ժ���������ʵ��ĸ�������д���

	//////////////////////////////////////////////////////////////////////////


	//ofstream outfile("lld_temp.txt",ios::out);
 // 	//nd_tb->Sequence();
 //	outfile<<"   "<< endl;
 //	outfile<<"nd:   "<< endl;
 //
 //	outfile<<"   "<< endl;
 //	outfile<<"cb:   "<< endl;
 // 	cb = cb_tb->GetTable();
 // 	int count = cb_tb->GetCount();
 // 		outfile<<"id       "<<"nd    "<<"znd       "<<"ind    "<<"iznd   "<<"q     "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(cb+i)->id<<"   "<<(cb+i)->nd<<"   "<<(cb+i)->znd<<"   "<<(cb+i)->ind<<"   "<<(cb+i)->iznd<<"   "<<(cb+i)->q<< endl;
 // 	}
 //	
 //	outfile<<"   "<< endl;
 //	outfile<<"sec:   "<< endl;
 // 	sec = sec_tb->GetTable();
 // 	count = sec_tb->GetCount();
 // 		outfile<<"id       "<<"nd    "<<"znd       "<<"ind    "<<"iznd       "<<"r_zn       "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(sec+i)->id<<"   "<<(sec+i)->nd<<"   "<<(sec+i)->znd<<"   "<<(sec+i)->ind<<"   "<<(sec+i)->iznd<<"   "<<(sec+i)->r_zn<<"   "<< endl;
 // 	}
 //	
 //	//outfile<<"   "<< endl;
 //	//outfile<<"zn:   "<< endl;
 // //	zn = zn_tb->GetTable();
 // //	count = zn_tb->GetCount();
 // //		outfile<<"id       "<<"type    "<<"r_sec       "<<"r_cb    "<<"cbptr       "<<endl;
 // //	for(i = 0; i <count; i++)
 // //	{
 //	//	outfile<<(zn+i)->id<<"   "<<(zn+i)->type<<"   "<<(zn+i)->r_sec<<"   "<<(zn+i)->r_cb<<"   "<<(zn+i)->cbptr<<"   "<< endl;
 // //	}

	//outfile.close();
	
////дһ�νڵ����
//	ofstream outfile("bs_temp11.txt",ios::out);
//  	//nd_tb->Sequence();
// 	outfile<<"   "<< endl;
// 	outfile<<"nd:   "<< endl;
// 
// 	nd = nd_tb->GetTable();
//  	int count = nd_tb->GetCount();
//  		outfile<<"id       "<<"ibs    "<<"q       "<<"fch     "<<"bch     "<<"r_sec     "<<"r_xf     "<<endl;
//  	for(i = 0; i <count; i++)
//  	{
// 		outfile<<(nd+i)->id<<"   "<<(nd+i)->ibs<<"   "<<(nd+i)->q<<"   "<<(nd+i)->fch<<"  "<<(nd+i)->bch<<"   "<<(nd+i)->r_sec<<"   "<<(nd+i)->r_xf<< endl;
//  	}
// 
//// 	outfile<<"   "<< endl;
//// 	outfile<<"bs:   "<< endl;
//// 	bs = bs_tb->GetTable();
////  	count = lld->mx_bs;
////  		outfile<<"bsid       "<<"pnd    "<<"q       "<<"fbs     "<<"bbs     "<<endl;
////  	for(i = 0; i <count; i++)
////  	{
//// 		outfile<<(bs+i)->id<<"   "<<(bs+i)->pnd<<"   "<<(bs+i)->q<<"   "<<(bs+i)->fbs<<"  "<<(bs+i)->bbs<<"  "<< endl;
////  	}
// 
//// 	outfile<<"   "<< endl;
//// 	outfile<<"bus:   "<< endl;
//// 	bus = bus_tb->GetTable();
////  	count = bus_tb->GetCount();
////  		outfile<<"busid       "<<"nd    "<<"ind       "<<"ibs     "<<"q     "<<endl;
////  	for(i = 0; i <count; i++)
////  	{
//// 		outfile<<(bus+i)->id<<"   "<<(bus+i)->nd<<"   "<<(bus+i)->ind<<"   "<<(bus+i)->ibs<<"  "<<(bus+i)->topflags<<"  "<< endl;
////  	}
//
//	outfile<<"   "<< endl;
// 	outfile<<"cb:   "<< endl;
//  	cb = cb_tb->GetTable();
//  	count = cb_tb->GetCount();
//  		outfile<<"id       "<<"nd    "<<"znd       "<<"ind    "<<"iznd   "<<"q     "<<endl;
//  	for(i = 0; i <count; i++)
//  	{
// 		outfile<<(cb+i)->id<<"   "<<(cb+i)->nd<<"   "<<(cb+i)->znd<<"   "<<(cb+i)->ind<<"   "<<(cb+i)->iznd<<"   "<<(cb+i)->q<<"  "<< endl;
//  	}
// 	
// 	outfile<<"   "<< endl;
// 	outfile<<"sec:   "<< endl;
//  	sec = sec_tb->GetTable();
//  	count = sec_tb->GetCount();
//  		outfile<<"id       "<<"nd    "<<"znd       "<<"ind    "<<"iznd       " << "ibs    " << "ibs       " <<endl;
//  	for(i = 0; i <count; i++)
//  	{
// 		outfile<<(sec+i)->id<<"   "<<(sec+i)->nd<<"   "<<(sec+i)->znd<<"   "<<(sec+i)->ind<<"   "<<(sec+i)->iznd<<"   " << (sec + i)->ibs << "   " << (sec + i)->zbs <<endl;
//  	}
// 	
//	if (lld->mx_xf > 0)
//	{
//		outfile << "   " << endl;
//		outfile << "xf:   " << endl;
//		xf = xf_tb->GetTable();
//		count = xf_tb->GetCount();
//		outfile << "id       " << "nd    " << "znd       " << "ind    " << "iznd       " << "ibs    " << "ibs       " << endl;
//		for (i = 0; i < count; i++)
//		{
//			outfile << (xf + i)->id << "   " << (xf + i)->nd << "   " << (xf + i)->znd << "   " << (xf + i)->ind << "   " << (xf + i)->iznd << "   " << (xf + i)->ibs << "   " << (xf + i)->zbs << endl;
//		}
//	}
//	
//	outfile.close();
//


  


	return TP_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  tp_bs_sort()
//  sorts buses into islands by selecting a valid bus as a starting point, 
//  finding all its neighbors, selecting each neighbor in turn and finding 
//  all its neighbors until no bus in the island is unprocessed. flags are 
//  maintained for each island to determine if an island has generation 
//  (source) and a voltage reference. if an insland doesn't have, then it 
//  is determined to be non-viable and all buses in the island will be 
//  marked as dead and be assigned to the -1 island.
int TP::tp_bs_sort()
{///////////////////////////////////////////////////////////////////////////////

	int		i, j;
	int		ind;
	int		ibs;
	int		startbs;
	int		desiredisland;
	int		neighbor;
	int		kf;
	int		kb;
	int		knd0;
	int		knd;
	int		*fch_bs;
	int		*bch_bs;

	//initialize island table
	//�������
	//island_tb_t->Empty();

	//initialize the unprocessed bs chained
	fch_bs = new int[lld->mx_bs];
	bch_bs = new int[lld->mx_bs];
	for(i = 0; i < lld->mx_bs; i++)
	{
		*(fch_bs+i) = (bs+i)->fbs;
		*(bch_bs+i) = (bs+i)->bbs;
	}

	(bs+global->nullbs)->iisland = -1;

	//start an island from an unprocessed so bs
	//�ӵ�Դ�㿪ʼ����ĸ�ߵ��ƽ���ֻ��һ����Ǵ��ڵģ������Ķ�������
	for(i = 0; i < lv_so; i++)
	{
		//get this so bs
		ind = (so+i)->ind;	//�ڵ��
		ibs = (nd+ind)->ibs;//����ĸ�ߺ�

		//check if this so bs has been processed
		if(*(bch_bs+ibs) == -1)
		{
			continue;
		}

		//check if this so bs is open
		if((bs+ibs)->q & BS_OPEN)
		{
			continue;
		}

		//select this so bs as start bs 
		startbs = ibs;
	
		//get island number to be created
		//�õ�׼�����ٵı����ţ������еı�������Եõ�
		desiredisland = island_tb_t->GetCount();  

		//delete start bs from unprocessed chain
		*(fch_bs+*(bch_bs+startbs)) = *(fch_bs+startbs);
		*(bch_bs+*(fch_bs+startbs)) = *(bch_bs+startbs);

        //initialize the current process chain, note that although
        //the unprocessed list is chained forward and backward,
        //the current process is chained forward only

		*(fch_bs+startbs) = startbs;
		*(bch_bs+startbs) = -1;//����������Ϊ-1��ʾ���ĸ���Ѿ�����
		g_bs = startbs;

		//find the neighbours of bs
		do
		{
			//�õ�����ڵ���������ڽڵ㣬�������ĸ�ߺŷŵ�����g_z_br[j]����
			//���ڽڵ���Ҫ���߶κͱ�ѹ��������ǰ�ƽ�
			if(tp_bs_neighbor() == TP_FAIL)
			{
				delete[] fch_bs;
				delete[] bch_bs;
				return TP_FAIL;
			}

			//loop on neighors of bs
			//��������㷨
			for(j = 0; j < g_nbr; j++)
			{
				neighbor = g_z_br[j];

				//if neighbor is unprocessed, remove it from the 
                //unprocessed chain and add it to current process  
				if(*(bch_bs+neighbor) >= 0)
				{
                    //the neighbor bs is not processed yet,
                    //remove from unprocessd chain
                    kf = *(fch_bs+neighbor);
                    kb = *(bch_bs+neighbor);
                    *(bch_bs+kf) = kb;
                    *(fch_bs+kb) = kf;

                    //add to the current process chain
                    kf = *(fch_bs+g_bs);
                    *(fch_bs+g_bs) = neighbor;
                    *(fch_bs+neighbor) = kf;
                    *(bch_bs+neighbor) = -1;  //mark as processed
            
				}
			}

			//get next bs in current process
			g_bs = *(fch_bs+g_bs);

		}   while(g_bs != startbs);

		//an viable island sort is now complete
		//FD* fd1 = new FD();
		if(island_tb_t->GetCount() < island_tb_t->GetSize())
		{
			//create a valid island
			island = new ISLAND(desiredisland);

			//strcpy(fd1->id, (so + i)->fdid);
			//island->reffd = dsbob2->fd_tb->FindPh(*fd1);//so������Ÿ������
			island_tb_t->Append(*island);

            //assign all buses in island to this island, 
            //check whether any assignments are new.
            g_bs = startbs;
            do
            {
				//set status of this bs
				(bs+g_bs)->q &= (~BS_DEAD);
				(bs+g_bs)->iisland = desiredisland;

				//set status of nds of this bs
				knd0 = (bs+g_bs)->pnd;
				knd = knd0;
				do
				{
					(nd+knd)->q &= (~ND_DEAD);
					knd = (nd+knd)->fch;
				}   while(knd != knd0);

				g_bs = *(fch_bs+g_bs);
				
			}   while(g_bs != startbs);

			delete island;
		}
		else
		{
			//no more room for islands
			delete[] fch_bs;
			delete[] bch_bs;

			//delete fd1;
			return TP_FAIL;
		}
		//delete fd1;
	}

	//set all unprocessed bs as dead bs
	g_bs = *(fch_bs+global->nullbs);
	while(g_bs != global->nullbs)
	{
		//set status of this bs
		(bs+g_bs)->q |= BS_DEAD;
		(bs+g_bs)->iisland = -1;

		//set status of nds of this bs
		knd0 = (bs+g_bs)->pnd;
		knd = knd0;
		do
		{
			(nd+knd)->q |= ND_DEAD;
			knd = (nd+knd)->fch;
		}   while(knd != knd0);

		g_bs = *(fch_bs+g_bs);
	}

	delete[] fch_bs;
	delete[] bch_bs;

	//�õ��ܵ�ĸ��������֧·����

	return TP_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  tp_bs_neighbor()
int TP::tp_bs_neighbor()
{
	int			br;
	int			knd;
	int			knd0;
	int			znd;
	ChildPtr1	*br_p, *br_p0;

	g_nbr = 0;  //number of branch in island

	//ͨ��һ�����ѭ��������������ĸ�ߵ��������ڵļ���ĸ���ҳ���
	//���е�ĸ�������Ƿ����ŵ�g_z_br��������
	//һ������ĸ���ܱ߲�����̫���ĸ�ߣ�20��Ӧ�����㹻��
	knd0 = (bs+g_bs)->pnd;//��������ͨ��ĸ�ߵõ��ڵ���
	knd = knd0;
	do
	{
		//sec���ڵ����������߶Σ��������߶ε�ָ��һ��������󷵻���ָ�룬�����ҵ������߶�
		if((nd+knd)->r_sec != -1)
		{
			br_p = r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			br_p0 = br_p;//�õ����е������߶�
			while(br_p != NULL)
			{
				br = br_p->Ptr;
				sec = sec_tb->GetDataByLog(br);
				if(knd == sec->ind)//�õ��߶εĶԲ�ڵ�
				{
					znd = sec->iznd;
				}
				else
				{
					znd = sec->ind;
				}
				g_z_br[g_nbr++] = (nd+znd)->ibs;
				br_p = br_p->Next;
			}
			::FreeChildPtr(br_p0);
		}

		if ((nd + knd)->r_cp != -1)
		{
			br_p = r_nd_cp_tb->FindChildPtr1((nd + knd)->r_cp);
			br_p0 = br_p;//�õ����е������߶�
			while (br_p)
			{
				br = br_p->Ptr;
				cp = cp_tb->GetDataByLog(br);
				if (knd == cp->ind)//�õ��߶εĶԲ�ڵ�
				{
					znd = cp->iznd;
				}
				else
				{
					znd = cp->ind;
				}
				g_z_br[g_nbr++] = (nd + znd)->ibs;
				br_p = br_p->Next;
			}
			::FreeChildPtr(br_p0);
		}

		//xf
		if((nd+knd)->r_xf != -1)
		{
			br_p = r_nd_xf_tb->FindChildPtr1((nd+knd)->r_xf);
			br_p0 = br_p;
			while(br_p != NULL)
			{
				br = br_p->Ptr;
				xf = xf_tb->GetDataByLog(br);
				if(knd == xf->ind)
				{
					znd = xf->iznd;
				}
				else
				{
					znd = xf->ind;
				}
				g_z_br[g_nbr++] = (nd+znd)->ibs;
				br_p = br_p->Next;
			}
			
			::FreeChildPtr(br_p0);
		}

		//move to next nd of this bs
		knd = (nd+knd)->fch;
				
	}	while(knd != knd0);

	if(g_nbr > g_mxnbr)
	{
		return TP_FAIL;
	}

	return TP_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  tp_comp_status()
//  set status of each component by topology processing results
int TP::tp_comp_status()
{//////////////////////////////////////////////////////////////////////////////

	int		i;
	int		ind;
	int		znd;
//	int     ind3;
//	int     ind4;
//	int     ind5;
	int		ibs;
	int		zbs;
//	int		ibs3;
//	int     ibs4;
//	int     ibs5;
//	int     len;
 	int		kxfmr;

	//bus
	for(i = 0; i < lv_bus; i++)
	{
		bus = bus_tb->GetDataByPh(i);
		ind = bus->ind;
		if (ind < 0)
			continue;

		ibs = (nd+ind)->ibs;
		if((bs+ibs)->iisland < 0)
		{
			bus->topflags |= BUS_DEAD;
		}
		else
		{
			bus->topflags &= (~BUS_DEAD);
		}
		bus->ibs = ibs;
	}

	//so
	for(i = 0; i < lv_so; i++)
	{
		so = so_tb->GetDataByPh(i);
		ind = so->ind;
		ibs = (nd+ind)->ibs;

		//lld add 2014-11-20,����Դ�������������������Ժ���м���
		so->iisland = (bs+ibs)->iisland;
		//lld add end 2014-11-20

		//if((bs+ibs)->q & BS_OPEN || (so->q & SO_REMOVE))
		if((bs+ibs)->q & BS_OPEN )
		{
			so->q |= SO_OPEN;
		}
		else
		{
			so->q &= (~SO_OPEN);
		}
		//if(((bs+ibs)->iisland < 0) || (so->q & SO_REMOVE))
		if(((bs+ibs)->iisland < 0))
		{
			so->topflags |= SO_DEAD;
		}
		else
		{
			so->topflags &= (~SO_DEAD);
		}
		so->ibs = ibs;
	}

	//ld
	for(i = 0; i < lv_ld; i++)
	{
		ld = ld_tb->GetDataByPh(i);
		ind = ld->ind;
//////////////////////////////////////////////////////////////////////////
		//�����ڵ���Ĵ���
		if (ind == -1)
		{
			ld->q |= LD_OPEN;
			ld->topflags |= LD_DEAD;
			ld->ibs = -1;

			continue;
		}
		//lld add 2011-11-16
//////////////////////////////////////////////////////////////////////////
		ibs = (nd+ind)->ibs;
		//if((bs+ibs)->q & BS_OPEN || (ld->q & LD_REMOVE))
		if((bs+ibs)->q & BS_OPEN )
		{
			ld->q |= LD_OPEN;
		}
		else
		{
			ld->q &= (~LD_OPEN);
		}
		//if(((bs+ibs)->iisland < 0) || (ld->q & LD_REMOVE))
		if(((bs+ibs)->iisland < 0))
		{
			ld->topflags |= LD_DEAD;
		}
		else
		{
			ld->topflags &= (~LD_DEAD);
		}
		ld->ibs = ibs;
	}

	//cp
	for(i = 0; i < lv_cp; i++)
	{
		cp = cp_tb->GetDataByPh(i);

		ind = cp->ind;
		znd = cp->iznd;
		if ((ind == -1) && (znd == -1))
		{
			cp->topflags |= CP_IOPEN;
			cp->topflags |= CP_ZOPEN;
			continue;
		}

		ibs = (nd + ind)->ibs;
		zbs = (nd + znd)->ibs;
		//if((bs+ibs)->q & BS_OPEN || (cp->q & cp_REMOVE))
		if ((bs + ibs)->q & BS_OPEN)
		{
			cp->topflags |= CP_IOPEN;
		}
		else
		{
			cp->topflags &= (~CP_IOPEN);
		}
		//if((bs+zbs)->q & BS_OPEN || (cp->q & cp_REMOVE))
		if ((bs + zbs)->q & BS_OPEN)
		{
			cp->topflags |= CP_ZOPEN;
		}
		else
		{
			cp->topflags &= (~CP_ZOPEN);
		}
		if ((bs + ibs)->iisland < 0 && (bs + zbs)->iisland < 0)
		{
			cp->topflags |= CP_DEAD;
		}
		else
		{
			cp->topflags &= (~CP_DEAD);
		}
		cp->ibs = ibs;
		cp->zbs = zbs;






		//ind = cp->ind;
		//ibs = (nd+ind)->ibs;
		////if((bs+ibs)->q & BS_OPEN || (cp->q & LD_REMOVE))
		//if((bs+ibs)->q & BS_OPEN)
		//{
		//	cp->q |= CP_OPEN;
		//}
		//else
		//{
		//	cp->q &= (~CP_OPEN);
		//}
		////if(((bs+ibs)->iisland < 0) || (cp->q & CP_REMOVE))
		//if(((bs+ibs)->iisland < 0))
		//{
		//	cp->topflags |= CP_DEAD;
		//}
		//else
		//{
		//	cp->topflags &= (~CP_DEAD);
		//}
		//cp->ibs = ibs;
	}

	//xf
	if (lld->mx_xf > 0)
	{
		lv_xf = xf_tb->GetCount();
		for (i = 0; i < lv_xf; i++)
		{
			xf = xf_tb->GetDataByPh(i);
			ind = xf->ind;
			znd = xf->iznd;
			ibs = (nd + ind)->ibs;
			zbs = (nd + znd)->ibs;
			if ((bs + ibs)->q & BS_OPEN || (xf->q & XF_REMOVE))
			{
				xf->topflags |= XF_IOPEN;//�׶˿�·
			}
			else
			{
				xf->topflags &= (~XF_IOPEN);
			}
			if ((bs + zbs)->q & BS_OPEN || (xf->q & XF_REMOVE))
			{
				xf->topflags |= XF_ZOPEN;//ĩ�˿�·
			}
			else
			{
				xf->topflags &= (~XF_ZOPEN);
			}
			if ((bs + ibs)->iisland < 0 && (bs + zbs)->iisland < 0)
			{
				xf->topflags |= XF_DEAD;//��һ��ĸ�߲��ڵ���
			}
			else
			{
				xf->topflags &= (~XF_DEAD);
			}
			xf->ibs = ibs;
			xf->zbs = zbs;

			//set status of xfmr

			//10kV��ʱ���ÿ��Ǳ�ѹ����״̬��lld edit 2020-8-4

			//kxfmr = r_xfmr_xf_tb->FindParentPtr1(xf->r_xfmr);
			//if (kxfmr == -1)
			//{
			//	return TP_FAIL;
			//}
			//xfmr = xfmr_tb->GetDataByPh(kxfmr);
			//if (xfmr->type == 2)
			//{
			//	if (xf->topflags & XF_IOPEN)
			//	{
			//		xfmr->topflags |= XFMR_HOPEN;
			//	}
			//	else
			//	{
			//		xfmr->topflags &= (~XFMR_HOPEN);
			//	}
			//	if (xf->topflags & XF_ZOPEN)
			//	{
			//		xfmr->topflags |= XFMR_LOPEN;
			//	}
			//	else
			//	{
			//		xfmr->topflags &= (~XFMR_LOPEN);
			//	}
			//	if (xf->topflags & XF_DEAD)
			//	{
			//		xfmr->topflags |= XFMR_DEAD;
			//	}
			//	else
			//	{
			//		xfmr->topflags &= (~XFMR_DEAD);
			//	}
			//}
			//else if (xfmr->type == 3)
			//{
			//	//len = strlen(xf->id);

			//	if (xf->topflags & XF_IOPEN)
			//	{
			//		if (strcmp(xf->ver, "��") == 0)
			//		{
			//			xfmr->topflags |= XFMR_HOPEN;
			//		}
			//		if (strcmp(xf->ver, "��") == 0)
			//		{
			//			xfmr->topflags |= XFMR_MOPEN;
			//		}
			//		if (strcmp(xf->ver, "��") == 0)
			//		{
			//			xfmr->topflags |= XFMR_LOPEN;
			//		}
			//	}
			//	else
			//	{
			//		if (strcmp(xf->ver, "��") == 0)
			//		{
			//			xfmr->topflags &= (~XFMR_HOPEN);
			//		}
			//		if (strcmp(xf->ver, "��") == 0)
			//		{
			//			xfmr->topflags &= (~XFMR_MOPEN);
			//		}
			//		if (strcmp(xf->ver, "��") == 0)
			//		{
			//			xfmr->topflags &= (~XFMR_LOPEN);
			//		}
			//	}
			//	if (xf->topflags & XF_DEAD)
			//	{
			//		xfmr->topflags |= XFMR_DEAD;
			//	}
			//	else
			//	{
			//		xfmr->topflags &= (~XFMR_DEAD);
			//	}
			//}
		}
	}

	//sec
	for(i = 0; i < lv_sec; i++)
	{
		sec = sec_tb->GetDataByPh(i);
		ind = sec->ind;
		znd = sec->iznd;
		if ((ind == -1) && (znd == -1))
		{
			sec->topflags |= SEC_IOPEN;
			sec->topflags |= SEC_ZOPEN;
			continue;
		}

		ibs = (nd+ind)->ibs;
		zbs = (nd+znd)->ibs;
		//if((bs+ibs)->q & BS_OPEN || (sec->q & SEC_REMOVE))
		if((bs+ibs)->q & BS_OPEN)
		{
			sec->topflags |= SEC_IOPEN;
		}
		else
		{
			sec->topflags &= (~SEC_IOPEN);
		}
		//if((bs+zbs)->q & BS_OPEN || (sec->q & SEC_REMOVE))
		if((bs+zbs)->q & BS_OPEN )
		{
			sec->topflags |= SEC_ZOPEN;
		}
		else
		{
			sec->topflags &= (~SEC_ZOPEN);
		}
		if((bs+ibs)->iisland < 0 && (bs+zbs)->iisland < 0)
		{
			sec->topflags |= SEC_DEAD;
		}
		else
		{
			sec->topflags &= (~SEC_DEAD);
		}
		sec->ibs = ibs;
		sec->zbs = zbs;
	}



	//ofstream outfile("bs_temp11.txt",ios::out);
 // 	//nd_tb->Sequence();
 //	outfile<<"   "<< endl;
 //	outfile<<"island:   "<< endl;
 //
 //	ISLAND* island  = dsbob2->island_tb->GetTable();
 // 	int count = dsbob2->island_tb->GetCount();
 // 		outfile<<"id       "<<"ibs    "<<"q       "<<"fch     "<<"bch     "<<"r_sec     "<<"r_xf     "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<i<<"  "<<(island +i)->id<<"   "<<(island +i)->reffd<< endl;
 // 	}
	//outfile.close();
 //
 //	outfile<<"   "<< endl;
 //	outfile<<"bs:   "<< endl;
 //	bs = bs_tb_t->GetTable();
 // 	count = lld->mx_bs;
 // 		outfile<<"bsid       "<<"pnd    "<<"q       "<<"fbs     "<<"bbs     "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(bs+i)->id<<"   "<<(bs+i)->pnd<<"   "<<(bs+i)->q<<"   "<<(bs+i)->fbs<<"  "<<(bs+i)->bbs<<"  "<< endl;
 // 	}

	//outfile<<"   "<< endl;
 //	outfile<<"so:   "<< endl;
 // 	so = so_tb->GetTable();
 // 	count = so_tb->GetCount();
 // 		outfile<<"id       "<<"nd    "<<"ind    "<<"bs   "<<"q     "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(so+i)->id<<"   "<<(so+i)->nd<<"   "<<(so+i)->ind<<"   "<<(so+i)->ibs<<"   "<<(so+i)->q<<"  "<< endl;
 // 	}

	//if (lld->mx_bus>0)
	//{
 //		outfile<<"   "<< endl;
 //		outfile<<"bus:   "<< endl;
 //		bus = bus_tb->GetTable();
 // 		count = bus_tb->GetCount();
 // 			outfile<<"busid       "<<"nd    "<<"ind       "<<"ibs    "<<endl;
 // 		for(i = 0; i <count; i++)
 // 		{
 //			outfile<<(bus+i)->id<<"   "<<(bus+i)->nd<<"   "<<(bus+i)->ind<<"   "<<(bus+i)->ibs<<"  "<< endl;
 // 		}
	//}
	//outfile<<"   "<< endl;
 //	outfile<<"cb:   "<< endl;
 // 	cb = cb_tb->GetTable();
 // 	count = cb_tb->GetCount();
 // 		outfile<<"id       "<<"nd    "<<"znd       "<<"ind    "<<"iznd   "<<"q     "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(cb+i)->id<<"   "<<(cb+i)->nd<<"   "<<(cb+i)->znd<<"   "<<(cb+i)->ind<<"   "<<(cb+i)->iznd<<"   "<<(cb+i)->q<<"  "<< endl;
 // 	}
 //	
 //	outfile<<"   "<< endl;
 //	outfile<<"sec:   "<< endl;
 // 	sec = sec_tb->GetTable();
 // 	count = sec_tb->GetCount();
 // 		outfile<<"id       "<<"nd    "<<"znd       "<<"ind    "<<"iznd       "<<"ibs   "<<"zbs   "<<  endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(sec+i)->id<<"   "<<(sec+i)->nd<<"   "<<(sec+i)->znd<<"   "<<(sec+i)->ind<<"   "<<(sec+i)->iznd<<"   "<<(sec+i)->ibs <<"   "<<(sec+i)->zbs <<"   "<<endl;
 // 	}

	//if (lld->mx_xf > 0)
	//{
	//	outfile << "   " << endl;
	//	outfile << "xf:   " << endl;
	//	xf = xf_tb->GetTable();
	//	count = xf_tb->GetCount();
	//	outfile << "id       " << "nd    " << "znd       " << "ind    " << "iznd       " << "ibs   " << "zbs   " << endl;
	//	for (i = 0; i < count; i++)
	//	{
	//		outfile << (xf + i)->id << "   " << (xf + i)->nd << "   " << (xf + i)->znd << "   " << (xf + i)->ind << "   " << (xf + i)->iznd << "   " << (xf + i)->ibs << "   " << (xf + i)->zbs << "   " << endl;
	//	}
	//}
	//
	//outfile<<"   "<< endl;
 //	outfile<<"ld:   "<< endl;
 //	ld = ld_tb->GetTable();
 // 	count = ld_tb->GetCount();
 // 		outfile<<"ldid       "<<"nd    "<<"ind       "<<"ibs    "<<"iisland    "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(ld+i)->id<<"   "<<(ld+i)->nd<<"   "<<(ld+i)->ind<<"   "<<(ld+i)->ibs<<"  "<< (ld+i)->topflags <<"  "<<endl;
 // 	}	
 //	
 //	outfile.close();	




	return TP_SUCCEED;
}

void TP::getEqID(char* eqid, int knd,int kknd)
{
	eqid[0] = 0;
	int i, count;
	if (lld->mx_xf > 0)
	{
		xf = xf_tb->GetTable();
		count = xf_tb->GetCount();
		for (i = 0; i < count; i++)
		{
			if ((((xf + i)->ind == knd) && ((xf + i)->iznd == kknd))
				|| ((((xf + i)->ind == kknd) && ((xf + i)->iznd == knd))))
			{
				strcpy(eqid, (xf + i)->id);
				strcat(eqid, ",");
				strcat(eqid, (xf + i)->nd);
				strcat(eqid, ",");
				strcat(eqid, (xf + i)->znd);
				strcat(eqid, "xf");
				return;
			}
		}
	}
	sec = sec_tb->GetTable();
	count = sec_tb->GetCount();
	for (i = 0; i < count; i++)
	{
		if ((((sec + i)->ind == knd) && ((sec + i)->iznd == kknd))
			|| ((((sec + i)->ind == kknd) && ((sec + i)->iznd == knd))))
		{
			strcpy(eqid, (sec + i)->id);
			strcat(eqid, ",");
			strcat(eqid, (sec + i)->nd);
			strcat(eqid, ",");
			strcat(eqid, (sec + i)->znd);
			strcat(eqid, "sec");
			return;
		}
	}
	cb = cb_tb->GetTable();
	count = cb_tb->GetCount();
	for (i = 0; i < count; i++)
	{
		if ((((cb + i)->ind == knd) && ((cb + i)->iznd == kknd))
			|| ((((cb + i)->ind == kknd) && ((cb + i)->iznd == knd))))
		{
			strcpy(eqid, (cb + i)->id);
			strcat(eqid, ",");
			strcat(eqid, (cb + i)->nd);
			strcat(eqid, ",");
			strcat(eqid, (cb + i)->znd);
			strcat(eqid, "cb");
			return;
		}
	}
}

int TP::tp_result()
{
	int			i;
	int			sum_lds = 0;//�ܵĸ��ɸ�������ֹ����
	int			ind, knd, kknd, kkndcb, ext_cbnd, cnt_ld;
	int			sond;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*xf_p, *xf_p0;
	ChildPtr1	*ld_p, *ld_p0;
	//******��Ҫ����չ���غ����α߽翪�����ֿ�******//
	int extndcount/*��չ�ڵ�����*/, extnd[100]/*��չ���صĽڵ�*/, \
		cbndcnt/*���α߽翪����*/;//һ�����ο��ܻ���100���߽�ڵ�

	int extcbcnt;//�����ڵĿ�������
	int extcbptr[100];//��������չ�ı߽翪�ص�ָ�����飬ͨ������������õ��߽翪��
	int inner_nd, out_nd;//���ε��׿��ص�����ڵ㣬������Ϊ���ģ����⡱ָ�����⣬�����εĿ�����ڵ�
	int firstcb;//��Դ����ֿ���

	double so_sumLoad, zn_sumLoad_p;//�����ڵ��ܸ�������,�й�һ�㰴0.15�����޹�

	char cb_id[80], sec_id[80], ld_id[80], so_nd[64];
	int unionCBcnt;//���翪������
	int unionCB[50];//�洢���翪�صĽڵ��

	int lv_sec = sec_tb->GetCount();
	int lv_cb = cb_tb->GetCount();
	int lv_ld = ld_tb->GetCount();

	//get record count in nd table
	lv_nd = nd_tb->GetCount();
	if (lv_nd == 0)
	{
		return 1;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	//mark all nds as unprocessed
	//�����еĽڵ���Ϊδ����׼�����˷�����
	for (i = 0; i < lv_nd; i++)
	{
		(nd + i)->q |= ND_UNPROC;
		//(nd+i)->bch = -1;//���ڵ�ĺ�������Ϊ-1
	}

	//get record count in so table
	lv_so = so_tb->GetCount();
	if (lv_so == 0)
	{
		delete[] nd;
		nd = 0;
		lv_nd = 0;

		return 1;
	}

	ofstream outfile("tp_parent_children.txt",ios::out);
	//��� �ڵ�� �ڵ����� ����
	int index = 0;
	char eq_id[128];
	char eq_name[128];
	char parent_id[128];

	for (i = 0; i < lv_so; i++)
	{
		if (so_tb->GetDataByLog(i)->slackavr != 0)//ֻ�����ڵ������
			continue;
		
		strcpy(eq_id, so_tb->GetDataByLog(i)->id);
		strcat(eq_id, ",");
		strcat(eq_id, so_tb->GetDataByLog(i)->nd);
		strcpy(eq_name, so_tb->GetDataByLog(i)->descr);
		strcpy(parent_id, "-1");

		cnt_ld = 0;//�����ڵĸ��ɸ���
		unionCBcnt = 0;
		extndcount = 0;
		ind = so_tb->GetDataByLog(i)->ind;//ĸ�ߵ���ʼ�ڵ���

		(nd + ind)->bch = ind;//�ӵ�Դ�㿪ʼ��ʼ����,����Դ����Ϊ��һ����¼����

		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
		if ((nd + ind)->q & ND_UNPROC) //this so nd has not processed
		{
			(nd + ind)->q &= (~ND_UNPROC);
			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
			(nd + knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
			(nd + knd)->q &= (~ND_UNPROC);//����ǰ�ڵ���Ϊ�Ѵ���
				
			//��
			outfile<< index++<<" "<< eq_id <<" "<< eq_name<<" "<< parent_id<<endl;
			strcpy(parent_id, eq_id);
			while (1)//������չ����û�д���
			{
				//�����ɵ�id�Ÿ�д
				//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
				if ((nd + knd)->q & ND_LD)
				{
					//�õ�����id
					ld_p = r_nd_ld_tb->FindChildPtr1((nd + knd)->r_ld);
					ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
					while (ld_p)
					{
						strcpy(eq_id, ld_tb->GetDataByPh(ld_p->Ptr)->id);
						strcat(eq_id, ",");
						strcat(eq_id, ld_tb->GetDataByLog(ld_p->Ptr)->nd);
						strcpy(eq_name, ld_tb->GetDataByLog(ld_p->Ptr)->descr);
						outfile << index++ << " " << eq_id << "������load�� " << eq_name << " " << parent_id << endl;

						cnt_ld++;//�����ڵĸ��ɸ�������1

						ld_p = ld_p->Next;
					}
					::FreeChildPtr(ld_p0);
				}

				//read cbs connected to this nd
				//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
				cb_p = r_nd_cb_tb->FindChildPtr1((nd + knd)->r_cb);
				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
				while (cb_p)
				{
					//find the opposite nd through this cb
					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
					if (knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}

					int ph = cb_p->Ptr;
					//��������Ƿֵ�״̬������Ҫ���ڵ���뵽��չ�ڵ㣬Ҳ���üӶԲ�ڵ���л���
					if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//���翪�ص�qΪ1������
						//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0) 
					{
						strcpy(eq_id, cb_tb->GetDataByPh(cb_p->Ptr)->id);
						strcat(eq_id, ",");
						strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->nd);
						strcat(eq_id, ",");
						strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->znd);
						strcpy(eq_name, cb_tb->GetDataByLog(cb_p->Ptr)->descr);
						outfile << index++ << " " << eq_id << "(un) " << eq_name << " " << parent_id << endl;
					}

					//process this opposite nd
					//����Բ�ڵ� �Ѵ���==���ز����������ƽ�
					if ((kkndcb != -1) && ((nd + kkndcb)->q & ND_UNPROC))
					{
						//��������Ƿֵ�״̬������Ҫ���ڵ���뵽��չ�ڵ㣬Ҳ���üӶԲ�ڵ���л���
						if (cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)//��λ�����翪��
						{
							strcpy(eq_id, cb_tb->GetDataByPh(cb_p->Ptr)->id);
							strcat(eq_id, ",");
							strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->nd);
							strcat(eq_id, ",");
							strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->znd);
							strcpy(eq_name, cb_tb->GetDataByLog(cb_p->Ptr)->descr);
							outfile << index++ << " " << eq_id << "un " << eq_name << " " << parent_id << endl;
						}
						else//�պϿ��أ�׼��������չ
						{
							//�����صĶԲ�ڵ������׼����һ��������չ��
							extnd[extndcount++] = kkndcb;//׼����չ�Ľڵ�

							strcpy(eq_id, cb_tb->GetDataByPh(cb_p->Ptr)->id);
							strcat(eq_id, ",");
							strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->nd);
							strcat(eq_id, ",");
							strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->znd);
							strcpy(eq_name, cb_tb->GetDataByLog(cb_p->Ptr)->descr);
							outfile << index++ << " " << eq_id << "cb " << eq_name << " " << parent_id << endl;
							
							(nd + kkndcb)->fch = (nd + knd)->fch;
							//�½ڵ� ��� ��չ�ڵ��ǰ����
							(nd + knd)->fch = kkndcb;
							
							//������ڵ����һ����¼������׼������
							(nd + kkndcb)->bch = knd;

							//���ҵ����½ڵ�����Ѵ���
							(nd + kkndcb)->q &= (~ND_UNPROC);
						}
					}
					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

				//read secs connected to this nd
				sec_p = r_nd_sec_tb->FindChildPtr1((nd + knd)->r_sec);
				sec_p0 = sec_p;
				while (sec_p)
				{
					//find the opposite nd through this sec
					if (knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
					{
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
					}
					else
					{
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;
					}

					//printf("%s\n",sec_tb->GetDataByPh(sec_p->Ptr)->id);
											//process this opposite nd
					if ((nd + kknd)->q & ND_UNPROC)
					{

						(nd + kknd)->fch = (nd + knd)->fch;
						//�½ڵ� ��� ��չ�ڵ��ǰ����
						(nd + knd)->fch = kknd;

						//������ڵ����һ����¼������׼������
						(nd + kknd)->bch = knd;

						strcpy(eq_id, sec_tb->GetDataByPh(sec_p->Ptr)->id);
						strcat(eq_id, ",");
						strcat(eq_id, sec_tb->GetDataByLog(sec_p->Ptr)->nd);
						strcat(eq_id, ",");
						strcat(eq_id, sec_tb->GetDataByLog(sec_p->Ptr)->znd);
						strcpy(eq_name, sec_tb->GetDataByLog(sec_p->Ptr)->descr);
						outfile << index++ << " " << eq_id << "sec " << eq_name << " " << parent_id << endl;

						//���ҵ����½ڵ�����Ѵ���
						(nd + kknd)->q &= (~ND_UNPROC);

					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);

				//read xfs connected to this nd
				xf_p = r_nd_xf_tb->FindChildPtr1((nd + knd)->r_xf);
				xf_p0 = xf_p;
				while (xf_p)
				{
					//find the opposite nd through this xf
					if (knd == xf_tb->GetDataByPh(xf_p->Ptr)->ind)
					{
						kknd = xf_tb->GetDataByPh(xf_p->Ptr)->iznd;
					}
					else
					{
						kknd = xf_tb->GetDataByPh(xf_p->Ptr)->ind;
					}

					//printf("%s\n",xf_tb->GetDataByPh(xf_p->Ptr)->id);
											//process this opposite nd
					if ((nd + kknd)->q & ND_UNPROC)
					{

						(nd + kknd)->fch = (nd + knd)->fch;
						//�½ڵ� ��� ��չ�ڵ��ǰ����
						(nd + knd)->fch = kknd;

						//������ڵ����һ����¼������׼������
						(nd + kknd)->bch = knd;

						strcpy(eq_id, xf_tb->GetDataByPh(xf_p->Ptr)->id);
						strcat(eq_id, ",");
						strcat(eq_id, xf_tb->GetDataByLog(xf_p->Ptr)->nd);
						strcat(eq_id, ",");
						strcat(eq_id, xf_tb->GetDataByLog(xf_p->Ptr)->znd);
						strcpy(eq_name, xf_tb->GetDataByLog(xf_p->Ptr)->descr);
						outfile << index++ << " " << eq_id << "xf " << eq_name << " " << parent_id << endl;
						//���ҵ����½ڵ�����Ѵ���
						(nd + kknd)->q &= (~ND_UNPROC);
					}
					xf_p = xf_p->Next;
				}
				::FreeChildPtr(xf_p0);

				//move to next nd��������һ��
				knd = (nd + knd)->fch;
				kknd = (nd + knd)->bch;
				//ѭ����ͷ���������ˣ���ʾȫ������һ��
				//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
				if (knd == ind)
				{
					break;
				}
				getEqID(parent_id, knd, kknd);
			}
		}
	}
	outfile.close();	
}



















