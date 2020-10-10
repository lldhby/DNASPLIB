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
	//在这个类里形成计算母线和拓扑岛表

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

	lv_island = lld->mx_nd;//极端情况，一个节点一个岛
	if (lv_island)
		island_tb_t         = new DTable<ISLAND>(lv_island);
	
	//get record count in tables
	//把与拓扑有关的表的所有的记录数得到
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
	//对每一个节点进行遍历，将开关节点进行合并，对每个节点都填上计算母线值，判断母线是否是活的
	//1.在全局表里面有一个计算母线的编号累加器，将下一个计算母线的编号取到
	//2.首先处理开关，找到这个节点所有的开关，如果开关是闭合的，将本节点和对侧节点都填上计算母线号
	//搜索时已找不到开关为结束点
	//3.找到和这个节点相连接的绕组和线段数量，是否连有负荷节点，电源节点，电容节点
	//如果这些都没有的话，说明这个母线是死母线，将这个节点和它的下游节点都设成死母线
	if(tp_nd_sort() == TP_FAIL)
	{
		return TP_FAIL;
	}

	//sort bs into island
	//对已经设定的计算母线进行处理
	//1.从当前的岛表得到新的岛的序号，从电源点开始进行处理，查找电源点开始的相邻母线，
	//如果已经处理，跳过，没有处理，则向后扩展，直到没有母线可以选择
	//2.对母线表进行选择，已经扩展到的母线，全部标成q是活的，加上岛的编号
	//3.对计算母线表进行循环，如果q不是活的，都是死岛，将其岛号赋值为-1
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
		//!!!!此时绝对不可以排序，因为有默认的-1在里面
		lld->mx_bs = bs_tb_t->Sort(lv_bs,0);
		bs_tb = new DTable<BS>(lld->mx_bs);
		if(lld->mx_bs != 0)
		{
			BS* temp = new BS[lld->mx_bs];//先开一段临时内存表，将所有的记录都开出来
			bs_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			bs_tb->Insert(temp,lld->mx_bs);
			dsbob2->bs_tb = bs_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete bs_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		bs_tb_t=0;		
	}

	if (lv_island > 0)
	{
		//!!!!此时绝对不可以排序，因为有默认的-1在里面
		lv_island = lld->mx_island = island_tb_t->Sort(lv_island,0);
		island_tb = new DTable<ISLAND>(lld->mx_island);
		if(lld->mx_island != 0)
		{
			ISLAND* temp = new ISLAND[lld->mx_island];//先开一段临时内存表，将所有的记录都开出来
			island_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			island_tb->Insert(temp,lld->mx_island);
			dsbob2->island_tb = island_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete island_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		island_tb_t=0;		
	}

	//添加岛和电源点、馈线之间的关系
	tp_trim_cord();

	return TP_SUCCEED;
}

//梳理关系
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
	//最大母线数默认是2000个
	//在bs_tb里灌入数据，前面2000个都是按顺序排
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
//  tp_nd_sort()	深度搜索，将开关的节点进行合并
//特别注意：在开关合并后，计算母线已经没有了开关的信息
//需要在计算母线里面加上该母线含有多少个联络开关，联络开关的编号
//每个编号用|隔开，这样在搜索联络开关的时候也可以知道会有几个联络
//一般情况下一个计算母线只有一个联络开关
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

	bool hasUnioncb = false;//母线上挂有联络开关
//	CString cbid = "";
	bool isMultiSo = false;//负荷是多电源，1-单电源；2-双电源；3-三电源。。。

	bs = bs_tb_t->GetTable();

	//initialize nd list and mark all nds as unprocessed
	//将所有的节点的母线编号都置成0；都设为未处理
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
	i = 0;//第一个母线。初始母线
	(bs+i)->fbs = i;  //point to itself
	(bs+i)->bbs = i;
	(bs+i)->iisland = -1;
	(bs+i)->q |= BS_DEAD;//母线的标志位，初始的为死岛，并且是开的
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
	//一个节点一个节点的处理，先取一个节点，然后找这个节点的扩展节点，开关要跳过
	//把这个节点的所有计算母线都取出来
	for (i = 0; i < lv_nd; i++)
	{
		if ((nd + i)->q & ND_UNPROC)  //this nd has not processed
		{
			//get bs number for this nd
			ibs = global->nextbs;//最后的那个计算母线号，每添加一个都往上涨1

			if (ibs == -1 || ibs > lv_bs)
			{
				printf("out of memory\n");
				return TP_FAIL;
			}

			//create circular chain for this nd
			knd = i;//knd==0开始进行节点扩展
			(nd + knd)->fch = knd;//设定前向链
			(nd + knd)->q &= (~ND_UNPROC);//将该节点设成已处理
			(nd + knd)->ibs = ibs;//把这个节点所属于的计算母线号填入到节点里面

			//initialize the nd group
			qndgrp = 0; //q of the nd group
			g_nbr = 0;	//branch number from the nd group

			//下面要做一个节点广度扩展的死循环，一直到找不到可以扩展的节点为止
			while (1)
			{
				//节点群的最初设定为0，每循环一次将这个群里面添加一个标志位
				qndgrp |= (nd + knd)->q;

				//xf，通过节点-绕组的关系表将和这个节点有关联的绕组数量取出来
				g_nbr += r_nd_xf_tb->FindChildNum((nd + knd)->r_xf);

				//sec，通过节点-线段的关系表将和这个节点有关联的线段数量取出来
				g_nbr += r_nd_sec_tb->FindChildNum((nd + knd)->r_sec);

				//处理0阻抗支路 add by lld 2015-4-29
				sec_p = r_nd_sec_tb->FindChildPtr1((nd + knd)->r_sec);
				sec_p0 = sec_p;//准备对这个支路进行扩展，先把最初的那个支路暂时存起来
				while (sec_p!=NULL)//cb_p是与这个节点关联的所有的支路的指针集合
				{
					if (((sec + sec_p->Ptr)->r1 == 0) && ((sec + sec_p->Ptr)->x1 == 0))
					{
						//find the opposte nd through this cb
						//找到对侧节点并将这个节点存放到kknd
						if (knd == (sec + sec_p->Ptr)->ind)
						{
							kknd = (sec + sec_p->Ptr)->iznd;
						}
						else
						{
							kknd = (sec + sec_p->Ptr)->ind;
						}

						//process this opposite nd
						//如果对侧节点没有处理，将对侧节点设为已处理，置前向链和后向链
						//将节点合并成一个计算母线，即将同一个计算母线号写入对侧的节点计算母线号
						if ((nd + kknd)->q & ND_UNPROC)
						{
							(nd + kknd)->fch = (nd + knd)->fch;
							(nd + knd)->fch = kknd;
							(nd + kknd)->q &= (~ND_UNPROC);
							(nd + kknd)->ibs = ibs;//找到对侧节点后，还是给节点赋一个相同的母线号

						}
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);//释放指针

				//go through cbs connected to this nd
				//对开关进行处理，主要是开关的节点需要合并，开关的2个节点合并成一个节点
				//对开关节点需要将全部的开关都处理成一个节点

				//通过节点-开关关联表将和这个节点相关联的开关取出来放到系列中
				//特别是遇到开闭所母线时会有很多的开关要加到开关系列中
				cb_p = r_nd_cb_tb->FindChildPtr1((nd + knd)->r_cb);
				cb_p0 = cb_p;//准备对这个开关进行扩展，先把最初的那个开关暂时存起来
				while (cb_p != NULL)//cb_p是与这个节点关联的所有的开关的指针集合
				{
					//if ( ((cb + cb_p->Ptr)->ind == 200) || ((cb + cb_p->Ptr)->iznd == 200) )
					//{
					//int debug = 0;
					//}

					//只有开关是闭合的状态才向下扩展
					if (!((cb+cb_p->Ptr)->q & CB_OPEN))
					{
						if (!((cb+cb_p->Ptr)->q & Q_UNIONCB))  //cb is closed
						{
							if ( (strcmp((cb+cb_p->Ptr)->nd,"N") != 0) && (strcmp((cb+cb_p->Ptr)->znd,"N") != 0) )
							{
								if (!((cb+cb_p->Ptr)->q & CB_FDLCB))  //cb is closed
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

									if (kknd == -1)//悬空开关
									{
										cb_p = cb_p->Next;
										continue;
									}

									//process this opposite nd
									//如果对侧节点没有处理，将对侧节点设为已处理，置前向链和后向链
									//将节点合并成一个计算母线，即将同一个计算母线号写入对侧的节点计算母线号
									if((nd+kknd)->q & ND_UNPROC)
									{
										(nd+kknd)->fch = (nd+knd)->fch;
										(nd+knd)->fch = kknd;
										(nd+kknd)->q &= (~ND_UNPROC);
										(nd+kknd)->ibs = ibs;//找到对侧节点后，还是给节点赋一个相同的母线号

									}
								}
							}
						}
					}
					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);//释放指针

				//get next nd in the present nd group
				knd = (nd+knd)->fch;//下一个节点就是前驱节点，前面的那个
				if(knd == i) break;
			}

			//check if the bs is open
			//判断母线是否连接有电源点、负荷点、电容点
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

			//insert bs into circular chain  用循环链表达到非递归深度搜索的目的
			//获得母线的前向和后向链
			(bs+ibs)->fbs = (bs+lastbs)->fbs;
			(bs+ibs)->bbs = lastbs;
			(bs+(bs+lastbs)->fbs)->bbs = ibs;
			(bs+lastbs)->fbs = ibs;
			(bs+ibs)->pnd = i;//通过计算母线pnd可以找到节点号

			//move to next bs
			lastbs = global->nextbs++;
		}
	}

	//当所有的计算母线都已经生成后，就可以得到真实的计算母线数
	lld->mx_bs = global->nextbs;
	//以后就用这个真实的母线数进行处理

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
	
////写一次节点表看看
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
	//将岛清空
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
	//从电源点开始进行母线的推进，只有一个活岛是存在的，其它的都是死岛
	for(i = 0; i < lv_so; i++)
	{
		//get this so bs
		ind = (so+i)->ind;	//节点号
		ibs = (nd+ind)->ibs;//计算母线号

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
		//得到准备开辟的表的序号，从已有的表个数可以得到
		desiredisland = island_tb_t->GetCount();  

		//delete start bs from unprocessed chain
		*(fch_bs+*(bch_bs+startbs)) = *(fch_bs+startbs);
		*(bch_bs+*(fch_bs+startbs)) = *(bch_bs+startbs);

        //initialize the current process chain, note that although
        //the unprocessed list is chained forward and backward,
        //the current process is chained forward only

		*(fch_bs+startbs) = startbs;
		*(bch_bs+startbs) = -1;//将后向链置为-1表示这个母线已经处理
		g_bs = startbs;

		//find the neighbours of bs
		do
		{
			//得到这个节点的所有相邻节点，将其计算母线号放到数组g_z_br[j]里面
			//相邻节点主要靠线段和变压器绕组向前推进
			if(tp_bs_neighbor() == TP_FAIL)
			{
				delete[] fch_bs;
				delete[] bch_bs;
				return TP_FAIL;
			}

			//loop on neighors of bs
			//深度搜索算法
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
			//island->reffd = dsbob2->fd_tb->FindPh(*fd1);//so的物理号给这个岛
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

	//得到总的母线数量和支路数量

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

	//通过一个深度循环，将所给定的母线的所有相邻的计算母线找出来
	//所有的母线无论是否处理都放到g_z_br数组里面
	//一个计算母线周边不能有太多的母线，20个应该是足够了
	knd0 = (bs+g_bs)->pnd;//反操作，通过母线得到节点编号
	knd = knd0;
	do
	{
		//sec，节点上连接有线段，将所有线段的指针一起捆绑最后返回首指针，便于找到连接线段
		if((nd+knd)->r_sec != -1)
		{
			br_p = r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
			br_p0 = br_p;//得到所有的连接线段
			while(br_p != NULL)
			{
				br = br_p->Ptr;
				sec = sec_tb->GetDataByLog(br);
				if(knd == sec->ind)//得到线段的对侧节点
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
			br_p0 = br_p;//得到所有的连接线段
			while (br_p)
			{
				br = br_p->Ptr;
				cp = cp_tb->GetDataByLog(br);
				if (knd == cp->ind)//得到线段的对侧节点
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

		//lld add 2014-11-20,将电源点的所属岛输出，便于以后进行计算
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
		//负荷在岛外的处理
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
				xf->topflags |= XF_IOPEN;//首端开路
			}
			else
			{
				xf->topflags &= (~XF_IOPEN);
			}
			if ((bs + zbs)->q & BS_OPEN || (xf->q & XF_REMOVE))
			{
				xf->topflags |= XF_ZOPEN;//末端开路
			}
			else
			{
				xf->topflags &= (~XF_ZOPEN);
			}
			if ((bs + ibs)->iisland < 0 && (bs + zbs)->iisland < 0)
			{
				xf->topflags |= XF_DEAD;//有一个母线不在岛内
			}
			else
			{
				xf->topflags &= (~XF_DEAD);
			}
			xf->ibs = ibs;
			xf->zbs = zbs;

			//set status of xfmr

			//10kV暂时不用考虑变压器的状态，lld edit 2020-8-4

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
			//		if (strcmp(xf->ver, "高") == 0)
			//		{
			//			xfmr->topflags |= XFMR_HOPEN;
			//		}
			//		if (strcmp(xf->ver, "中") == 0)
			//		{
			//			xfmr->topflags |= XFMR_MOPEN;
			//		}
			//		if (strcmp(xf->ver, "低") == 0)
			//		{
			//			xfmr->topflags |= XFMR_LOPEN;
			//		}
			//	}
			//	else
			//	{
			//		if (strcmp(xf->ver, "高") == 0)
			//		{
			//			xfmr->topflags &= (~XFMR_HOPEN);
			//		}
			//		if (strcmp(xf->ver, "中") == 0)
			//		{
			//			xfmr->topflags &= (~XFMR_MOPEN);
			//		}
			//		if (strcmp(xf->ver, "低") == 0)
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
	int			sum_lds = 0;//总的负荷个数，防止出错
	int			ind, knd, kknd, kkndcb, ext_cbnd, cnt_ld;
	int			sond;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*xf_p, *xf_p0;
	ChildPtr1	*ld_p, *ld_p0;
	//******需要将扩展开关和区段边界开关区分开******//
	int extndcount/*扩展节点数量*/, extnd[100]/*扩展开关的节点*/, \
		cbndcnt/*区段边界开关数*/;//一个区段可能会有100个边界节点

	int extcbcnt;//区段内的开关总数
	int extcbptr[100];//区段内扩展的边界开关的指针数组，通过这个数组来得到边界开关
	int inner_nd, out_nd;//区段的首开关的内外节点，以区段为核心，“外”指区段外，进区段的开关外节点
	int firstcb;//电源点出现开关

	double so_sumLoad, zn_sumLoad_p;//区段内的总负荷数量,有功一般按0.15折算无功

	char cb_id[80], sec_id[80], ld_id[80], so_nd[64];
	int unionCBcnt;//联络开关数量
	int unionCB[50];//存储联络开关的节点号

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
	//将所有的节点设为未处理，准备拓扑分析用
	for (i = 0; i < lv_nd; i++)
	{
		(nd + i)->q |= ND_UNPROC;
		//(nd+i)->bch = -1;//将节点的后向链置为-1
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
	//序号 节点号 节点名称 父亲
	int index = 0;
	char eq_id[128];
	char eq_name[128];
	char parent_id[128];

	for (i = 0; i < lv_so; i++)
	{
		if (so_tb->GetDataByLog(i)->slackavr != 0)//只有主节点才能做
			continue;
		
		strcpy(eq_id, so_tb->GetDataByLog(i)->id);
		strcat(eq_id, ",");
		strcat(eq_id, so_tb->GetDataByLog(i)->nd);
		strcpy(eq_name, so_tb->GetDataByLog(i)->descr);
		strcpy(parent_id, "-1");

		cnt_ld = 0;//区段内的负荷个数
		unionCBcnt = 0;
		extndcount = 0;
		ind = so_tb->GetDataByLog(i)->ind;//母线的起始节点编号

		(nd + ind)->bch = ind;//从电源点开始开始进行,将电源点作为上一级记录下来

		//之前已经将所有的节点都置为未处理
		if ((nd + ind)->q & ND_UNPROC) //this so nd has not processed
		{
			(nd + ind)->q &= (~ND_UNPROC);
			knd = ind;//当前节点赋给一个临时的变量
			(nd + knd)->fch = knd; //当前节点号赋给前向链 
			(nd + knd)->q &= (~ND_UNPROC);//将当前节点设为已处理
				
			//父
			outfile<< index++<<" "<< eq_id <<" "<< eq_name<<" "<< parent_id<<endl;
			strcpy(parent_id, eq_id);
			while (1)//还有扩展开关没有处理
			{
				//将负荷的id号改写
				//通过节点的q可以判断出是不是负荷节点，q |= 8
				if ((nd + knd)->q & ND_LD)
				{
					//得到负荷id
					ld_p = r_nd_ld_tb->FindChildPtr1((nd + knd)->r_ld);
					ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
					while (ld_p)
					{
						strcpy(eq_id, ld_tb->GetDataByPh(ld_p->Ptr)->id);
						strcat(eq_id, ",");
						strcat(eq_id, ld_tb->GetDataByLog(ld_p->Ptr)->nd);
						strcpy(eq_name, ld_tb->GetDataByLog(ld_p->Ptr)->descr);
						outfile << index++ << " " << eq_id << "（负荷load） " << eq_name << " " << parent_id << endl;

						cnt_ld++;//区段内的负荷个数加上1

						ld_p = ld_p->Next;
					}
					::FreeChildPtr(ld_p0);
				}

				//read cbs connected to this nd
				//从节点-开关关系表找到所有和这个节点相连的开关
				cb_p = r_nd_cb_tb->FindChildPtr1((nd + knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while (cb_p)
				{
					//find the opposite nd through this cb
					//kknd表示开关的对侧节点，这里的节点都是整型的编号
					if (knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}

					int ph = cb_p->Ptr;
					//如果开关是分的状态，都不要将节点加入到扩展节点，也不用加对侧节点进行回溯
					if (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//联络开关的q为1，常开
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
					//如果对侧节点 已处理==开关不参与拓扑推进
					if ((kkndcb != -1) && ((nd + kkndcb)->q & ND_UNPROC))
					{
						//如果开关是分的状态，都不要将节点加入到扩展节点，也不用加对侧节点进行回溯
						if (cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)//分位非联络开关
						{
							strcpy(eq_id, cb_tb->GetDataByPh(cb_p->Ptr)->id);
							strcat(eq_id, ",");
							strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->nd);
							strcat(eq_id, ",");
							strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->znd);
							strcpy(eq_name, cb_tb->GetDataByLog(cb_p->Ptr)->descr);
							outfile << index++ << " " << eq_id << "un " << eq_name << " " << parent_id << endl;
						}
						else//闭合开关，准备向下扩展
						{
							//将开关的对侧节点存起来准备下一个区段扩展用
							extnd[extndcount++] = kkndcb;//准备扩展的节点

							strcpy(eq_id, cb_tb->GetDataByPh(cb_p->Ptr)->id);
							strcat(eq_id, ",");
							strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->nd);
							strcat(eq_id, ",");
							strcat(eq_id, cb_tb->GetDataByLog(cb_p->Ptr)->znd);
							strcpy(eq_name, cb_tb->GetDataByLog(cb_p->Ptr)->descr);
							outfile << index++ << " " << eq_id << "cb " << eq_name << " " << parent_id << endl;
							
							(nd + kkndcb)->fch = (nd + knd)->fch;
							//新节点 设成 扩展节点的前向链
							(nd + knd)->fch = kkndcb;
							
							//将这个节点的上一级记录下来，准备回溯
							(nd + kkndcb)->bch = knd;

							//将找到的新节点设成已处理
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
						//新节点 设成 扩展节点的前向链
						(nd + knd)->fch = kknd;

						//将这个节点的上一级记录下来，准备回溯
						(nd + kknd)->bch = knd;

						strcpy(eq_id, sec_tb->GetDataByPh(sec_p->Ptr)->id);
						strcat(eq_id, ",");
						strcat(eq_id, sec_tb->GetDataByLog(sec_p->Ptr)->nd);
						strcat(eq_id, ",");
						strcat(eq_id, sec_tb->GetDataByLog(sec_p->Ptr)->znd);
						strcpy(eq_name, sec_tb->GetDataByLog(sec_p->Ptr)->descr);
						outfile << index++ << " " << eq_id << "sec " << eq_name << " " << parent_id << endl;

						//将找到的新节点设成已处理
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
						//新节点 设成 扩展节点的前向链
						(nd + knd)->fch = kknd;

						//将这个节点的上一级记录下来，准备回溯
						(nd + kknd)->bch = knd;

						strcpy(eq_id, xf_tb->GetDataByPh(xf_p->Ptr)->id);
						strcat(eq_id, ",");
						strcat(eq_id, xf_tb->GetDataByLog(xf_p->Ptr)->nd);
						strcat(eq_id, ",");
						strcat(eq_id, xf_tb->GetDataByLog(xf_p->Ptr)->znd);
						strcpy(eq_name, xf_tb->GetDataByLog(xf_p->Ptr)->descr);
						outfile << index++ << " " << eq_id << "xf " << eq_name << " " << parent_id << endl;
						//将找到的新节点设成已处理
						(nd + kknd)->q &= (~ND_UNPROC);
					}
					xf_p = xf_p->Next;
				}
				::FreeChildPtr(xf_p0);

				//move to next nd，搜索完一层
				knd = (nd + knd)->fch;
				kknd = (nd + knd)->bch;
				//循环到头，连起来了，表示全部走了一遍
				//拿另外的节点再开始搜索，一直到所有的节点都处理
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



















