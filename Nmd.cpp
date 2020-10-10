////////////////////////////////////////////////////////////////////////////////
//
//  NMD.CPP
//   
////////////////////////////////////////////////////////////////////////////////
#if _WIN32
#elif __linux__
#else
#endif

#include "../DNASPDLLLIB/Nmd.h"
#include "../DNASPLIB/tmpodrtb.h"

#include "string.h"
 using namespace std;
#include <fstream>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
//constructor
NMD::NMD(DSBOB2* m_pDb,CLLD* m_pLLD)
{
//	ASSERT(m_pDb != NULL);
	dsbob = m_pDb;
	lld = m_pLLD;

	digital_tb = dsbob->digital_tb;
	analog_tb = dsbob->analog_tb;
	co_tb = dsbob->co_tb;
	st_tb = dsbob->st_tb;
	xfmr_tb = dsbob->xfmr_tb;
	xf_tb = dsbob->xf_tb;
	cb_tb = dsbob->cb_tb;
	so_tb = dsbob->so_tb;
	fd_tb = dsbob->fd_tb;
	un_tb = dsbob->un_tb;
	ld_tb = dsbob->ld_tb;
	cp_tb = dsbob->cp_tb;
	bus_tb = dsbob->bus_tb;

	sec_tb = dsbob->sec_tb;
	nd_tb = dsbob->nd_tb;
	global_tb = dsbob->global_tb;
	tapty_tb = dsbob->tapty_tb;
	r_co_st_tb = dsbob->r_co_st_tb;
	r_st_xfmr_tb = dsbob->r_st_xfmr_tb;
	r_xfmr_xf_tb = dsbob->r_xfmr_xf_tb;
	r_st_cb_tb = dsbob->r_st_cb_tb;
	r_st_so_tb = dsbob->r_st_so_tb;
	r_st_ld_tb = dsbob->r_st_ld_tb;
	r_st_cp_tb = dsbob->r_st_cp_tb;
	r_st_bus_tb = dsbob->r_st_bus_tb;
	r_zn_ld_tb = dsbob->r_zn_ld_tb;
	r_zn_sec_tb = dsbob->r_zn_sec_tb;
	r_zn_cp_tb = dsbob->r_zn_cp_tb;
	r_zn_xf_tb = dsbob->r_zn_xf_tb;
	r_zn_cb_tb = dsbob->r_zn_cb_tb;
	r_nd_cb_tb = dsbob->r_nd_cb_tb;
	r_nd_ld_tb = dsbob->r_nd_ld_tb;
	r_nd_xf_tb = dsbob->r_nd_xf_tb;
	r_nd_sec_tb = dsbob->r_nd_sec_tb;
	r_nd_cp_tb = dsbob->r_nd_cp_tb;
	r_fd_so_tb = dsbob->r_fd_so_tb;
	eq_tb = dsbob->eq_tb;
	r_nd_eq_tb = dsbob->r_nd_eq_tb;
	r_fd_eq_tb = dsbob->r_fd_eq_tb;

	lv_digital = 0;
	lv_analog = 0;
	lv_co = 0;
	lv_st = 0;
	lv_xfmr = 0;
	lv_xf = 0;
	lv_cb = 0;
	lv_so = 0;
	lv_fd = 0;
	lv_un = 0;
	lv_ld = 0;
	lv_cp = 0;
	lv_bus = 0;
	lv_zn = 0;
	lv_sec = 0;
	lv_nd = 0;
	lv_bs = 0;
	lv_island = 0;
	lv_global = 0;
	lv_tapty = 0;
	lv_lnty = 0;
	lv_r_co_st = 0;
	lv_r_st_xfmr = 0;
	lv_r_xfmr_xf = 0;
	lv_r_st_cb = 0;
	lv_r_st_so = 0;
	lv_r_st_ld = 0;
	lv_r_st_cp = 0;
	lv_r_st_bus = 0;
	lv_r_zn_ld = 0;
	lv_r_zn_sec = 0;
	lv_r_nd_cb = 0;
	lv_r_zn_cb = 0;
	lv_r_zn_cp = 0;
	lv_r_nd_xf = 0;
	lv_r_nd_sec = 0;
	lv_r_nd_cp = 0;

	lv_r_fd_un = 0;
	lv_r_fd_so = 0;

	digital = 0;
	analog = 0;
	co = 0;
	st = 0;
	xfmr = 0;
	xf = 0;
	cb = 0;
	so = 0;
	fd = 0;
	un = 0;
	ld = 0;
	cp = 0;
	bus = 0;
	zn = 0;
	sec = 0;
	nd = 0;
	bs = 0;
	island = 0;
	global = 0;
	tapty = 0;
	r_co_st = 0;
	r_st_xfmr = 0;
	r_xfmr_xf = 0;
	r_st_cb = 0;
	r_st_so = 0;
	r_st_ld = 0;
	r_st_cp = 0;
	r_st_bus = 0;
	r_zn_ld = 0;
	r_zn_sec = 0;
	r_zn_cb = 0;
	r_zn_cp = 0;
	r_zn_xf = 0;

	r_nd_cb = 0;
	r_nd_xf = 0;
	r_nd_sec = 0;
	r_nd_cp = 0;

	r_fd_so = 0;
	r_fd_nd = 0;
}


////////////////////////////////////////////////////////////////////////////////
//  destructor
NMD::~NMD()
{
	delete[] digital;
	delete[] analog;
	delete[] co;
	delete[] st;
	delete[] xfmr;
	delete[] xf;
	delete[] cb;
	delete[] so;

	delete[] fd;
	delete[] un;

	delete[] ld;
	delete[] cp;
	delete[] bus;
	delete[] zn;
	delete[] sec;
	delete[] nd;
	delete[] bs;
	delete[] island;
	delete[] global;
	delete[] tapty;
	delete[] r_co_st;
	delete[] r_st_xfmr;
	delete[] r_xfmr_xf;
	delete[] r_st_cb;
	delete[] r_st_so;
	delete[] r_st_ld;
	delete[] r_st_cp;
	delete[] r_st_bus;
	delete[] r_zn_ld;
	delete[] r_zn_sec;
	delete[] r_zn_cp;
	delete[] r_zn_xf;

	delete[] r_zn_cb;


	delete[] r_nd_cb;
	delete[] r_nd_xf;
	delete[] r_nd_sec;
	delete[] r_nd_cp;

	delete[] r_fd_so;
	delete[] r_fd_nd;
}

////////////////////////////////////////////////////////////////////////////////
//  DoValidate()
int NMD::DoValidate()
{
	char str[64];

	if(ValidateStart() == NMD_FAIL)
	{
		sprintf(str,"校验初始化有误!");
		lld->WriteLog(lld->logfile,str);
		return -1;
	}

//对每一个设备进行校验，主要是查看设备与上一级的关联关系，在关联关系表里面填上值
//看是否有关联，如果没有关联，说明有问题
	if(CB_Validate() == NMD_FAIL)
	{
       	sprintf(str,"开关表校验有误!");
		lld->WriteLog(lld->logfile,str);
		return -2;
	}

	if(BUS_Validate() == NMD_FAIL)
	{
		sprintf(str,"母线表校验有误!");
		lld->WriteLog(lld->logfile,str);
		return -3;
	}

	if(CP_Validate() == NMD_FAIL)
	{
		sprintf(str,"电容表校验有误!");
		lld->WriteLog(lld->logfile,str);
		return -4;
	}

	if(LD_Validate() == NMD_FAIL)
	{
		sprintf(str,"负荷表校验有误!");
		lld->WriteLog(lld->logfile,str);
		return -5;
	}

	if(SO_Validate() == NMD_FAIL)
	{
		sprintf(str,"电源表校验有误!");
		lld->WriteLog(lld->logfile,str);
		return -6;
	}

	if(SEC_Validate() == NMD_FAIL)
	{
		sprintf(str,"线段表校验有误!");
		lld->WriteLog(lld->logfile,str);
		return -7;
	}

	//先做变压器，得到绕组表，绕组和线段是分开的
	//变压器校验、生成绕组表、节点-绕组关系表、绕组-变压器关系表
	//if(XFMR_Validate() == NMD_FAIL)
	//{
 //       sprintf(str,"变压器表校验有误!");
	//	lld->WriteLog(lld->logfile,str);
	//	return -8;
	//}
	if (XF_Validate() == NMD_FAIL)
	{
		sprintf(str, "绕组表校验有误!");
		lld->WriteLog(lld->logfile, str);
		return -8;
	}

	
	//只有主网才用这个来做
	if ((lld->MATH_VOLTAGELEVEL != 10)
		&& (XF_Validate() == NMD_FAIL))
	{
        sprintf(str,"绕组表校验有误!");
		lld->WriteLog(lld->logfile,str);
		return -9;
	}

	TrimTable();


 //   int count,i;
	//ofstream outfile("lld_nd.txt",ios::out);
 // 	//nd_tb->Sequence();
 //	outfile<<"   "<< endl;
 //	outfile<<"nd:   "<< endl;
 // 	nd = nd_tb->GetTable();
 // 	count = nd_tb->GetCount();
 // 		outfile<<"id       "<<"r_cb    "<<"r_sec       "<<"r_eq       "<<endl;
 // 	for(int i = 0; i <count; i++)
 // 	{
 //		outfile<<i<<"  "<<(nd+i)->id<<"   "<<(nd+i)->r_cb<<"   "<<(nd+i)->r_sec<< "   "<<(nd+i)->r_eq<< endl;
 // 	}

 //	outfile<<"   "<< endl;
 //	outfile<<"cb:   "<< endl;
 // 	cb = cb_tb->GetTable();
 // 	count = cb_tb->GetCount();
	//outfile<<"id       "<<"nd    "<<"znd       "<<"ind    iznd       "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<i<<"  "<<(cb+i)->id<<"   "<<(cb+i)->nd<<"   "<<(cb+i)->znd<<"   "<<(cb+i)->ind<<"   "<<(cb+i)->iznd<< endl;
 // 	}

	//outfile << "   " << endl;
	//outfile << "r_nd_cb:   " << endl;
	//r_nd_cb = r_nd_cb_tb->GetTable();
	//count = r_nd_cb_tb->GetCount();
	//outfile << "id       " << "nd_id    " << "cb_id       " << "nd_id_ptr    cb_id_ptr       " << endl;
	//for (i = 0; i < count; i++)
	//{
	//	outfile << i << "  " << (r_nd_cb + i)->id << "   " << (r_nd_cb + i)->nd_id 
	//		<< "   " << (r_nd_cb + i)->cb_id << "   " << (r_nd_cb + i)->nd_id_ptr
	//		<< "   " << (r_nd_cb + i)->cb_id_ptr << endl;
	//}

 //	outfile<<"   "<< endl;
 //	outfile<<"sec:   "<< endl;
 // 	sec = sec_tb->GetTable();
 // 	count = sec_tb->GetCount();
	//outfile<<"id       "<<"nd    "<<"znd       "<<"ind    iznd       "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<i<<"  "<<(sec+i)->id<<"   "<<(sec+i)->nd<<"   "<<(sec+i)->znd<<"   "<<(sec+i)->ind<<"   "<<(sec+i)->iznd<< endl;
 // 	}

 //	outfile<<"   "<< endl;
 //	outfile<<"r_nd_cb:   "<< endl;
 // 	r_nd_cb = r_nd_cb_tb->GetTable();
 // 	count = r_nd_cb_tb->GetCount();
	//outfile<<"id       "<<"nd_id    "<<"cb_id       "<<"nd_id_ptr    cb_id_ptr    ftpr    bptr    "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(r_nd_cb+i)->id<<"   "<<(r_nd_cb+i)->nd_id<<"   "<<(r_nd_cb+i)->cb_id<<"   "<<(r_nd_cb+i)->nd_id_ptr<<"   "<<(r_nd_cb+i)->cb_id_ptr<<"   "<<(r_nd_cb+i)->fptr<<"   "<<(r_nd_cb+i)->bptr<<"   "<< endl;
 // 	}

 //	outfile<<"   "<< endl;
 //	outfile<<"r_nd_sec:   "<< endl;
 // 	r_nd_sec = r_nd_sec_tb->GetTable();
 // 	count = r_nd_sec_tb->GetCount();
	//outfile<<"id       "<<"nd_id    "<<"sec_id       "<<"nd_id_ptr    sec_id_ptr    ftpr    bptr    "<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<(r_nd_sec+i)->id<<"   "<<(r_nd_sec+i)->nd_id<<"   "<<(r_nd_sec+i)->sec_id<<"   "<<(r_nd_sec+i)->nd_id_ptr<<"   "<<(r_nd_sec+i)->sec_id_ptr<<"   "<<(r_nd_sec+i)->fptr<<"   "<<(r_nd_sec+i)->bptr<<"   "<< endl;
 // 	}


	//outfile.close();

	//sprintf(str,"校验区段");
	//lld->WriteLog(lld->logfile,str);


	if (lld->MATH_VOLTAGELEVEL == 10)
	{
		if(ZN_Validate() == NMD_FAIL)
		{
			sprintf(str,"区段表校验有误!");
			lld->WriteLog(lld->logfile,str);
			return -10;
		}

	//做馈线关联关系表
		if (FD_Validate() == NMD_FAIL)
		{
			sprintf(str, "馈线关联表校验有误!");
			lld->WriteLog(lld->logfile, str);
			return -11;
		}
	}
//
////如果做可靠性，需要设备之间的关联关系
//	if(RA_Validate() == NMD_FAIL)
//	{
//		sprintf(str,"可靠性关联表校验有误!");
//		lld->addLogLine(str);
//		return NMD_FAIL;
//	}

	if(ValidateEnd() == NMD_FAIL)
	{
		sprintf(str,"校验结束有误!");
		lld->WriteLog(lld->logfile,str);
		return -12;
	}

	return NMD_SUCCEED;
}

int	NMD::TrimTable()
{
	//lv_bs = lv_nd = 2*lld->mx_cb+2*lld->mx_sec+lld->mx_so+1000;//实体表在前面已经紧缩过了
	if (lv_r_nd_cb > 0)
	{
		//!!!!此时绝对不可以排序，因为有默认的-1在里面
		lld->mx_r_nd_cb = r_nd_cb_tb_t->Sort(lv_r_nd_cb,0);
		//if (r_nd_cb_tb != NULL)//先删掉以前的
		//	delete r_nd_cb_tb;
		r_nd_cb_tb = new DTable<R_ND_CB>(lld->mx_r_nd_cb);
		if(lld->mx_r_nd_cb != 0)
		{
			R_ND_CB* temp = new R_ND_CB[lld->mx_r_nd_cb];//先开一段临时内存表，将所有的记录都开出来
			r_nd_cb_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			r_nd_cb_tb->Insert(temp,lld->mx_r_nd_cb);
			dsbob->r_nd_cb_tb = r_nd_cb_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete r_nd_cb_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		r_nd_cb_tb_t=0;		
	}

	if (lv_r_nd_sec > 0)
	{
		lld->mx_r_nd_sec = r_nd_sec_tb_t->Sort(lv_r_nd_sec,0);
		//if (r_nd_sec_tb != NULL)//先删掉以前的
		//	delete r_nd_sec_tb;

		r_nd_sec_tb = new DTable<R_ND_SEC>(lld->mx_r_nd_sec);
		if(lld->mx_r_nd_sec != 0)
		{
			R_ND_SEC* temp = new R_ND_SEC[lld->mx_r_nd_sec];//先开一段临时内存表，将所有的记录都开出来
			r_nd_sec_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			r_nd_sec_tb->Insert(temp,lld->mx_r_nd_sec);
			dsbob->r_nd_sec_tb = r_nd_sec_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete r_nd_sec_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		r_nd_sec_tb_t=0;		
	}

	if (lv_xf > 0)//由xfmr表生成
	{
		lld->mx_xf = xf_tb_t->Sort(lv_xf, 0);
		//if (xf_tb != NULL)//先删掉以前的
		//	delete xf_tb;

		xf_tb = new DTable<XF>(lld->mx_xf);
		if (lld->mx_xf != 0)
		{
			XF* temp = new XF[lld->mx_xf];//先开一段临时内存表，将所有的记录都开出来
			xf_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			xf_tb->Insert(temp, lld->mx_xf);
			dsbob->xf_tb = xf_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete xf_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		xf_tb_t = 0;
	}

	if (lv_r_xfmr_xf > 0)
	{
		lld->mx_r_xfmr_xf = r_xfmr_xf_tb_t->Sort(lv_r_xfmr_xf, 0);
		//if (r_xfmr_xf_tb != NULL)//先删掉以前的
		//	delete r_xfmr_xf_tb;

		r_xfmr_xf_tb = new DTable<R_XFMR_XF>(lld->mx_r_xfmr_xf);
		if (lld->mx_r_xfmr_xf != 0)
		{
			R_XFMR_XF* temp = new R_XFMR_XF[lld->mx_r_xfmr_xf];//先开一段临时内存表，将所有的记录都开出来
			r_xfmr_xf_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			r_xfmr_xf_tb->Insert(temp, lld->mx_r_xfmr_xf);
			dsbob->r_xfmr_xf_tb = r_xfmr_xf_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete r_xfmr_xf_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		r_xfmr_xf_tb_t = 0;
	}

	if (lv_r_nd_xf > 0)
	{
		lld->mx_r_nd_xf = r_nd_xf_tb_t->Sort(lv_r_nd_xf, 0);
		//if (r_nd_xf_tb != NULL)//先删掉以前的
		//	delete r_nd_xf_tb;

		r_nd_xf_tb = new DTable<R_ND_XF>(lld->mx_r_nd_xf);
		if (lld->mx_r_nd_xf != 0)
		{
			R_ND_XF* temp = new R_ND_XF[lld->mx_r_nd_xf];//先开一段临时内存表，将所有的记录都开出来
			r_nd_xf_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			r_nd_xf_tb->Insert(temp, lld->mx_r_nd_xf);
			dsbob->r_nd_xf_tb = r_nd_xf_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete r_nd_xf_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		r_nd_xf_tb_t = 0;
	}
	
	if (lv_r_nd_cp > 0)
	{
		lld->mx_r_nd_cp = r_nd_cp_tb_t->Sort(lv_r_nd_cp, 0);
		//if (r_nd_cp_tb != NULL)//先删掉以前的
		//	delete r_nd_cp_tb;

		r_nd_cp_tb = new DTable<R_ND_CP>(lld->mx_r_nd_cp);
		if (lld->mx_r_nd_cp != 0)
		{
			R_ND_CP* temp = new R_ND_CP[lld->mx_r_nd_cp];//先开一段临时内存表，将所有的记录都开出来
			r_nd_cp_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			r_nd_cp_tb->Insert(temp, lld->mx_r_nd_cp);
			dsbob->r_nd_cp_tb = r_nd_cp_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete r_nd_cp_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		r_nd_cp_tb_t = 0;
	}

	if (lv_r_nd_ld > 0)
	{
		lld->mx_r_nd_ld = r_nd_ld_tb_t->Sort(lv_r_nd_ld,0);
		//if (r_nd_ld_tb != NULL)//先删掉以前的
		//	delete r_nd_ld_tb;

		r_nd_ld_tb = new DTable<R_ND_LD>(lld->mx_r_nd_ld);
		if(lld->mx_r_nd_ld != 0)
		{
			R_ND_LD* temp = new R_ND_LD[lld->mx_r_nd_ld];//先开一段临时内存表，将所有的记录都开出来
			r_nd_ld_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

			//update table将临时表里面的记录回写到正式表里面
			r_nd_ld_tb->Insert(temp,lld->mx_r_nd_ld);
			dsbob->r_nd_ld_tb = r_nd_ld_tb;

			delete[] temp;//将临时表内存释放
			temp = 0;//保险起见，防止释放后指针乱指
		}
		delete r_nd_ld_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		r_nd_ld_tb_t=0;		
	}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  Validate_Start()
int	NMD::ValidateStart()
{
//	int		i;

	//initialize global data
	global = new GLOBAL[1];
	global_tb->Empty();//先将全局表清空，然后准备写入全局系统的数据

	//只要不带t，都是给dsbob开的，因为直接传过来的是引用
	//删除指针的时候，不能看实际内容大小，要看开内存时候的大小
	if (lld->MATH_VOLTAGELEVEL == 10)
		lv_eq = lv_r_fd_eq = 1000+lld->mx_sec+lld->mx_cb+lld->mx_ld + lld->mx_xf;
	lv_nd = lld->mx_nd;
	if (lld->MATH_VOLTAGELEVEL == 10)
	{
		lv_r_zn_xf = lv_r_zn_ld = lv_r_nd_ld = lv_r_fd_ld= 10*lld->mx_ld;
		if (lld->mx_xf > 0)
		{
			lv_r_nd_xf = 2 * lld->mx_xf;
		}
	}
	else
	{
		lv_r_nd_ld = 10 * lld->mx_ld;
		lv_r_nd_xf = 10 * lld->mx_xf;
	}
	lv_r_nd_cb = 2 * lld->mx_cb;
	lv_r_nd_sec = 2 * lld->mx_sec;
	lv_r_nd_cp = 2 * lld->mx_cp;
	lv_r_nd_eq = 5*(lld->mx_sec+lld->mx_cb+lld->mx_ld);
	lv_zn = lv_r_zn_cb = 5000 + lld->mx_cb;
	lv_r_zn_sec =lld->mx_sec;
	
	lv_r_fd_un = 2*lld->mx_un;//每个联络开关对应两条馈线
	
	lv_r_fd_so = 2 * lld->mx_so;//一般情况下是1:1
	lv_r_fd_zn =  500*lld->mx_fd;
	

	if (lld->mx_xfmr>0)
	{
		lv_xf = 100+lld->mx_xfmr;//及其少量的三绕组变压器
		lv_r_nd_xf = 10 * lv_xf;
		lv_r_xfmr_xf = 500+2*lv_xf;
		if (lv_xf > 0)
		{
			xf_tb_t = new DTable<XF>(lv_xf);
		}
		if (lv_r_xfmr_xf > 0)
			r_xfmr_xf_tb_t = new DTable<R_XFMR_XF>(lv_r_xfmr_xf);
	}

	if (lv_r_nd_cb > 0)
		r_nd_cb_tb_t      = new DTable<R_ND_CB>(lv_r_nd_cb);
	if (lv_r_nd_sec > 0)
		r_nd_sec_tb_t     = new DTable<R_ND_SEC>(lv_r_nd_sec);
	if (lv_r_nd_xf > 0)
		r_nd_xf_tb_t = new DTable<R_ND_XF>(lv_r_nd_xf);
	if (lv_r_nd_ld > 0)
		r_nd_ld_tb_t      = new DTable<R_ND_LD>(lv_r_nd_ld);
	if (lv_r_nd_cp > 0)
		r_nd_cp_tb_t = new DTable<R_ND_CP>(lv_r_nd_cp);

	if (lld->MATH_VOLTAGELEVEL == 10)
	{
		if (lv_eq > 0)
			eq_tb_t = new DTable<EQ>(lv_eq);

		if (lv_r_nd_eq > 0)
			r_nd_eq_tb_t = new DTable<R_ND_EQ>(lv_r_nd_eq);

		if (lv_zn > 0)
			zn_tb_t = new DTable<ZN>(lv_zn);
		if (lv_r_zn_cb > 0)
			r_zn_cb_tb_t = new DTable<R_ZN_CB>(lv_r_zn_cb);
		if (lv_r_zn_sec > 0)
			r_zn_sec_tb_t = new DTable<R_ZN_SEC>(lv_r_zn_sec);
		if (lv_r_zn_ld > 0)
			r_zn_ld_tb_t = new DTable<R_ZN_LD>(lv_r_zn_ld);
		if (lv_r_zn_cp > 0)
			r_zn_cp_tb_t = new DTable<R_ZN_CP>(lv_r_zn_cp);
		if (lv_r_zn_xf > 0)
			r_zn_xf_tb_t = new DTable<R_ZN_XF>(lv_r_zn_xf);

		if (lv_r_fd_so > 0)
			r_fd_so_tb_t = new DTable<R_FD_SO>(lv_r_fd_so);
		if (lv_r_fd_un > 0)
			r_fd_un_tb_t = new DTable<R_FD_UN>(lv_r_fd_un);
		if (lv_eq > 0)
			r_fd_eq_tb_t = new DTable<R_FD_EQ>(lv_r_fd_eq);
		if (lv_r_fd_zn > 0)
			r_fd_zn_tb_t = new DTable<R_FD_ZN>(lv_r_fd_zn);
		if (lv_r_fd_ld > 0)
			r_fd_ld_tb_t = new DTable<R_FD_LD>(lv_r_fd_ld);
	}

//	eq_tb->Empty();
//
//	nd_tb->Empty();
//	r_nd_cb_tb->Empty();
//	r_nd_sec_tb->Empty();
//	r_nd_ld_tb->Empty();
//	r_nd_eq_tb->Empty();
//
//	//remove all r_zn_sec records
//	zn_tb->Empty();
//	r_zn_ld_tb->Empty();
//	r_zn_cb_tb->Empty();
//	r_zn_sec_tb->Empty();
//
//	r_fd_un_tb->Empty();
//	r_fd_so_tb->Empty();
//	r_fd_eq_tb->Empty();
//	r_fd_ld_tb->Empty();
//	r_fd_zn_tb->Empty();

	//reset pointers in tables，处理表与表之间的关系，主要是针对网络表里面相互关系的字段的
	//将每个表里面的相互关系的字段全部填成-1，实际上是对每个表进行初始化
	//先将从数据库读出来的内存表取出来，将字段置成-1，然后再回写回去，达到修改内存表的目的
	ResetPtr();

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  ValidateEnd()
int	NMD::ValidateEnd()
{
	//	char str[200];

		//紧缩表，将临时表全部删除
	if (lld->MATH_VOLTAGELEVEL == 10)
	{
		if (lv_eq > 0)
		{
			lld->mx_eq = eq_tb_t->Sort(lv_eq, 0);
			eq_tb = new DTable<EQ>(lld->mx_eq);
			if (lld->mx_eq != 0)
			{
				EQ* temp = new EQ[lld->mx_eq];//先开一段临时内存表，将所有的记录都开出来
				eq_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				eq_tb->Insert(temp, lld->mx_eq);
				dsbob->eq_tb = eq_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete eq_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			eq_tb_t = 0;
		}

		if (lv_r_nd_eq > 0)
		{
			lld->mx_r_nd_eq = r_nd_eq_tb_t->Sort(lv_r_nd_eq, 0);
			r_nd_eq_tb = new DTable<R_ND_EQ>(lld->mx_r_nd_eq);
			if (lld->mx_r_nd_eq != 0)
			{
				R_ND_EQ* temp = new R_ND_EQ[lld->mx_r_nd_eq];//先开一段临时内存表，将所有的记录都开出来
				r_nd_eq_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_nd_eq_tb->Insert(temp, lld->mx_r_nd_eq);
				dsbob->r_nd_eq_tb = r_nd_eq_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_nd_eq_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_nd_eq_tb_t = 0;
		}

		//	if (lv_bs > 0)
		//	{
		//		lld->mx_bs = bs_tb_t->Sort(lv_bs);
		//		bs_tb = new DTable<BS>(lld->mx_bs);
		//		if(lld->mx_bs != 0)
		//		{
		//			BS* temp = new BS[lld->mx_bs];//先开一段临时内存表，将所有的记录都开出来
		//			bs_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满
		//
		//			//update table将临时表里面的记录回写到正式表里面
		//			bs_tb->Insert(temp,lld->mx_bs);
		//
		//			delete[] temp;//将临时表内存释放
		//			temp = 0;//保险起见，防止释放后指针乱指
		//		}
		//		delete bs_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
		//		bs_tb_t=0;		
		//	}

		if (lv_zn > 0)
		{
			lld->mx_zn = zn_tb_t->Sort(lv_zn, 0);
			zn_tb = new DTable<ZN>(lld->mx_zn);
			if (lld->mx_zn != 0)
			{
				ZN* temp = new ZN[lld->mx_zn];//先开一段临时内存表，将所有的记录都开出来
				zn_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				zn_tb->Insert(temp, lld->mx_zn);
				dsbob->zn_tb = zn_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete zn_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			zn_tb_t = 0;
		}

		if (lv_r_zn_cb > 0)
		{
			lld->mx_r_zn_cb = r_zn_cb_tb_t->Sort(lv_r_zn_cb, 0);
			r_zn_cb_tb = new DTable<R_ZN_CB>(lld->mx_r_zn_cb);
			if (lld->mx_r_zn_cb != 0)
			{
				R_ZN_CB* temp = new R_ZN_CB[lld->mx_r_zn_cb];//先开一段临时内存表，将所有的记录都开出来
				r_zn_cb_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_zn_cb_tb->Insert(temp, lld->mx_r_zn_cb);
				dsbob->r_zn_cb_tb = r_zn_cb_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_zn_cb_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_zn_cb_tb_t = 0;
		}

		if (lv_r_zn_sec > 0)
		{
			lld->mx_r_zn_sec = r_zn_sec_tb_t->Sort(lv_r_zn_sec, 0);
			r_zn_sec_tb = new DTable<R_ZN_SEC>(lld->mx_r_zn_sec);
			if (lld->mx_r_zn_sec != 0)
			{
				R_ZN_SEC* temp = new R_ZN_SEC[lld->mx_r_zn_sec];//先开一段临时内存表，将所有的记录都开出来
				r_zn_sec_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_zn_sec_tb->Insert(temp, lld->mx_r_zn_sec);
				dsbob->r_zn_sec_tb = r_zn_sec_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_zn_sec_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_zn_sec_tb_t = 0;
		}

		if (lv_r_zn_cp > 0)
		{
			lld->mx_r_zn_cp = r_zn_cp_tb_t->Sort(lv_r_zn_cp, 0);
			r_zn_cp_tb = new DTable<R_ZN_CP>(lld->mx_r_zn_cp);
			if (lld->mx_r_zn_cp != 0)
			{
				R_ZN_CP* temp = new R_ZN_CP[lld->mx_r_zn_cp];//先开一段临时内存表，将所有的记录都开出来
				r_zn_cp_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_zn_cp_tb->Insert(temp, lld->mx_r_zn_cp);
				dsbob->r_zn_cp_tb = r_zn_cp_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_zn_cp_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_zn_cp_tb_t = 0;
		}

		if (lv_r_zn_ld > 0)
		{
			lld->mx_r_zn_ld = r_zn_ld_tb_t->Sort(lv_r_zn_ld, 0);
			r_zn_ld_tb = new DTable<R_ZN_LD>(lld->mx_r_zn_ld);
			if (lld->mx_r_zn_ld != 0)
			{
				R_ZN_LD* temp = new R_ZN_LD[lld->mx_r_zn_ld];//先开一段临时内存表，将所有的记录都开出来
				r_zn_ld_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_zn_ld_tb->Insert(temp, lld->mx_r_zn_ld);
				dsbob->r_zn_ld_tb = r_zn_ld_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_zn_ld_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_zn_ld_tb_t = 0;
		}

		if (lv_r_fd_so > 0)
		{
			lld->mx_r_fd_so = r_fd_so_tb_t->Sort(lv_r_fd_so, 0);
			r_fd_so_tb = new DTable<R_FD_SO>(lld->mx_r_fd_so);
			if (lld->mx_r_fd_so != 0)
			{
				R_FD_SO* temp = new R_FD_SO[lld->mx_r_fd_so];//先开一段临时内存表，将所有的记录都开出来
				r_fd_so_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_fd_so_tb->Insert(temp, lld->mx_r_fd_so);
				dsbob->r_fd_so_tb = r_fd_so_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_fd_so_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_fd_so_tb_t = 0;
		}

		//sprintf(str,"lld1.....");
		//lld->WriteLog(lld->logfile,str);

		if ((lv_r_fd_un > 0) && (lld->mx_un > 0))
		{
			lld->mx_r_fd_un = r_fd_un_tb_t->Sort(lv_r_fd_un, 0);
			r_fd_un_tb = new DTable<R_FD_UN>(lld->mx_r_fd_un);
			if (lld->mx_r_fd_un != 0)
			{
				R_FD_UN* temp = new R_FD_UN[lld->mx_r_fd_un];//先开一段临时内存表，将所有的记录都开出来
				r_fd_un_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_fd_un_tb->Insert(temp, lld->mx_r_fd_un);
				dsbob->r_fd_un_tb = r_fd_un_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_fd_un_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_fd_un_tb_t = 0;
		}

		if (lv_r_fd_eq > 0)
		{
			lld->mx_r_fd_eq = r_fd_eq_tb_t->Sort(lv_r_fd_eq, 0);
			r_fd_eq_tb = new DTable<R_FD_EQ>(lld->mx_r_fd_eq);
			if (lld->mx_r_fd_eq != 0)
			{
				R_FD_EQ* temp = new R_FD_EQ[lld->mx_r_fd_eq];//先开一段临时内存表，将所有的记录都开出来
				r_fd_eq_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_fd_eq_tb->Insert(temp, lld->mx_r_fd_eq);
				dsbob->r_fd_eq_tb = r_fd_eq_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_fd_eq_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_fd_eq_tb_t = 0;
		}

		if (lv_r_fd_ld > 0)
		{
			lld->mx_r_fd_ld = r_fd_ld_tb_t->Sort(lv_r_fd_ld, 0);
			r_fd_ld_tb = new DTable<R_FD_LD>(lld->mx_r_fd_ld);
			if (lld->mx_r_fd_ld != 0)
			{
				R_FD_LD* temp = new R_FD_LD[lld->mx_r_fd_ld];//先开一段临时内存表，将所有的记录都开出来
				r_fd_ld_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_fd_ld_tb->Insert(temp, lld->mx_r_fd_ld);
				dsbob->r_fd_ld_tb = r_fd_ld_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
		}

		if (lv_r_fd_zn > 0)
		{
			lld->mx_r_fd_zn = r_fd_zn_tb_t->Sort(lv_r_fd_zn, 0);
			r_fd_zn_tb = new DTable<R_FD_ZN>(lld->mx_r_fd_zn);
			if (lld->mx_r_fd_zn != 0)
			{
				R_FD_ZN* temp = new R_FD_ZN[lld->mx_r_fd_zn];//先开一段临时内存表，将所有的记录都开出来
				r_fd_zn_tb_t->CopyTable(temp);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面,刚好装满

				//update table将临时表里面的记录回写到正式表里面
				r_fd_zn_tb->Insert(temp, lld->mx_r_fd_zn);
				dsbob->r_fd_zn_tb = r_fd_zn_tb;

				delete[] temp;//将临时表内存释放
				temp = 0;//保险起见，防止释放后指针乱指
			}
			delete r_fd_zn_tb_t;//把临时开的区段内存全部删掉，将内存腾出来
			r_fd_zn_tb_t = 0;
		}
		//需要在进行搜索之前整理物理编号，否则节点表(节点在随机插入的时候按物理排序，非逻辑的id排序)排序后变乱了
		RebuildPtr();

		//	CompressTable();
		//	if (nmd_type == 1)
		//		DIGITAL_ANALOG_PtrBuild();

			//set vl of nds
			//填写每一个母线的节点电压值,这个是为潮流计算做准备的
			//从目前的根节点母线开始追踪，如果是开关或者是线段，将根母线的电压赋给扩展到的节点
			//如果搜索到新的母线，那么要将新的母线的节点电压填到节点里面	

			//lld for temp*************2011-11-11
			//这个函数主要是设定每一个节点的电压值，因为单纯计算10kV的配电网，这个过程基本可以损略
		SetVL(1);
	}

	return NMD_SUCCEED;
}

//区段要做的事：
//1.自动将区段做出来，做区段-线段r_zn_sec，区段-负荷r_zn_ld，区段-开关r_zn_cb的关系
//2.做设备表，设备-节点关系r_nd_eq
//3.做，馈线-电源r_fd_so，馈线-联络r_fd_un，馈线-区段r_fd_zn，馈线-负荷r_fd_ld，馈线-设备r_fd_eq的关系
int	NMD::ZN_Validate()
{
	char		str[500];
	int			i,jj;
	int			sum_lds=0;//总的负荷个数，防止出错
	int			ind, knd, kknd, kkndcb,ext_cbnd,cnt_ld;
	int			fdph;
	int			sond;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;
	ChildPtr1	*cp_p, *cp_p0;
	ChildPtr1	*xf_p, *xf_p0;
	//******需要将扩展开关和区段边界开关区分开******//
	int extndcount/*扩展节点数量*/,extnd[100]/*扩展开关的节点*/, \
		cbndcnt/*区段边界开关数*/, cb_zn_znd[100]/*扩展开关的内侧节点*/,cb_zn_intnd[100], \
		cb_zn_nd[100]/*边界开关的外侧节点*/,zncnt/*区段编号*/;//一个区段可能会有100个边界节点

	int extcbcnt;//区段内的开关总数
	int extcbptr[100];//区段内扩展的边界开关的指针数组，通过这个数组来得到边界开关
	int inner_nd,out_nd;//区段的首开关的内外节点，以区段为核心，“外”指区段外，进区段的开关外节点
	int firstcb;//电源点出现开关

	char zn_id[16];
	double so_sumLoad,zn_sumLoad_p;//区段内的总负荷数量,有功一般按0.15折算无功
	
	double so_ltime;//馈线的定位隔离时间，lld add 2016-1-27

	char cb_id[80],sec_id[80],ld_id[80], cp_id[80], xf_id[80],fdid[80],eq_nd[80],eq_znd[80],so_nd[64];
	int unionCBcnt;//联络开关数量
	int unionCB[100];//存储联络开关的节点号
	int zn_p/*区段的物理位置*/, icb/*开关的逻辑位置*/;

	int	eq_cnt,l;
	int lv_sec = sec_tb->GetCount();
	int lv_cb = cb_tb->GetCount();
	int lv_ld = ld_tb->GetCount();

	//lld add 2018-4-25
	EQ*	eq = new EQ;
	eq_cnt=0;

	//get record count in nd table
	lv_nd = nd_tb->GetCount();
	if(lv_nd == 0)
	{
		printf("there is no record in nd table\n");
		sprintf(str,"节点表中无记录!");
		lld->WriteLog(lld->logfile,str);
		return NMD_SUCCEED;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->q |= ND_UNPROC;
		//(nd+i)->bch = -1;//将节点的后向链置为-1
	}

	//get record count in so table
	lv_so = so_tb->GetCount();
	if(lv_so == 0)
	{
		//printf("there is no record in lv_so table\n");
		sprintf(str,"电源表中无记录!");
			lld->WriteLog(lld->logfile,str);
	
		delete[] nd;
		nd = 0;
		lv_nd = 0;

		delete[] eq;
		eq = 0;
		eq_cnt = 0;

		return NMD_SUCCEED;
	}

	//read all so nds into buffer
	//sond = new int[lv_so];//整型的指针，相当于一个整数的数组，记录一个地址
	//islandso = new int[lv_so];//整型的指针，相当于一个整数的数组，记录一个地址
	//for(i = 0; i < lv_so; i++)
	//{
	//	//将母线表so_tb的节点编号ind，按顺序写到sond指针数组里面

	//	//只有平衡节点才能做
	//	if (so_tb->GetDataByLog(i)->slackavr == 0)
	//	{
	//		*(sond + islandso) = so_tb->GetDataByLog(i)->ind;
	//		islandso++;
	//	}
	//}

	//r_fd_nd = r_fd_nd_tb->GetTable();//节点关系表

	//zn_tb_t = new DTable<ZN>(lld->mx_zn);

	ZN* znt = new ZN();
	zncnt = 1;//本次计算累计的区段个数，z999999个，用这个来表示每一个区段
	int znsum = 0;//本次计算包含的所有区段数
	so_ltime = 0;


	for(i = 0; i < lv_so; i++)
	{
		//if (i == 60)
		//{
		//	int debug = 0;
		//	continue;
		//}
		if (so_tb->GetDataByLog(i)->slackavr != 4)//只有主节点才能做
			continue;

		strcpy(so_nd, so_tb->GetDataByLog(i)->nd);
		ND* ndt = new ND(so_nd);
		so_tb->GetDataByLog(i)->ind = sond = nd_tb->FindPh(*ndt);
		delete ndt;
	
		//so_ltime = so_tb->GetDataByLog(i)->fault_ltime;
		strcpy(fdid,so_tb->GetDataByLog(i)->fdid);//电源所属的馈线
		fdph = dsbob->OnGetPhByfdid(fdid);//fd_tb->FindPh(FD(fdid));//在馈线表中的位置
		so_ltime = fd_tb->GetDataByPh(fdph)->fault_ltime;
		//需要特殊处理电源区段，也就是电源点到出现开关的那个区段
		firstcb = 0;
		ext_cbnd = -1;//电源点区段没有进入开关
		cb_zn_intnd[0] = -1;

		so_sumLoad = 0;
		unionCBcnt = 0;
		extndcount = 0;
		ind = sond;//母线的起始节点编号
		fd_tb->GetDataByPh(fdph)->cnt_nd++;//馈线上节点的个数增加
		fd_tb->GetDataByPh(fdph)->cnt_so++;//馈线上电源的个数增加

		(nd+ind)->bch = ind;//从电源点开始开始进行,将电源点作为上一级记录下来
		
		//之前已经将所有的节点都置为未处理
		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
		{
			(nd+ind)->q &= (~ND_UNPROC);
			//通过节点的q可以判断出是不是负荷节点，q |= 8

			//拿当前节点向下扩展，找到所有和它相连的节点
			cb_zn_znd[extndcount] = ind;
			extnd[extndcount++] = ind;
			
			while( extndcount >= 1 )//这个循环完成一个区段的搜索
			{
				ind = extnd[--extndcount];//每次都取最后的那个节点进行扩展
				ext_cbnd = cb_zn_intnd[extndcount];//区段进入的节点的上一个开关的节点号，用来判断区段的进入开关

				knd = ind;//当前节点赋给一个临时的变量
				(nd+knd)->fch = knd; //当前节点号赋给前向链 
				(nd+knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

				//从开始扩展的节点开始先形成一个区段ID和区段描述
				sprintf(zn_id,"z%06d",zncnt++);
				zn_p = znt->id = zn_tb_t->GetNext();

				cnt_ld = 0;//区段内的负荷个数
				zn_sumLoad_p = 0.0;//区段内的总负荷大小_有功
				//zn_sumLoad_q = 0.0;//区段内的总负荷大小_无功
				cbndcnt = 0;//区段内的节点数
				
				inner_nd = out_nd = -1;
				extcbcnt = 0;//区段内的开关数

				while (1)//还有扩展开关没有处理
				{
					if ((nd+knd)->q & ND_SO)//节点上挂有分布式电源点 
					{
						fd_tb->GetDataByPh(fdph)->cnt_so++;//馈线上电源的个数增加
					}

					if (strcmp((nd + knd)->id, "129_30500000_1765366") == 0)
					{
						int lld = 0;
					}


					//将负荷的id号改写
					//通过节点的q可以判断出是不是负荷节点，q |= 8
					if ((nd+knd)->q & ND_LD) 
					{
						//得到负荷id
						ld_p = r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
						ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
						while(ld_p)
						{
							strcpy(ld_id, ld_tb->GetDataByPh(ld_p->Ptr)->id);

							if (ND_BR_Insert(_R_ZN_LD,zn_p, zn_id, ld_p->Ptr, ld_id,znt->fdid,znt->ver) == NMD_FAIL )
							{
			//					sprintf(str,"负荷(%s)的区段和负荷关系建立失败!",ld_id);
			//lld->WriteLog(lld->logfile,str);
								ld_p = ld_p->Next;
								return NMD_FAIL;
							}
							//构造馈线-负荷关系表
							if(ND_BR_Insert(_R_FD_LD,fdph, fdid, ld_p->Ptr, ld_id,znt->fdid,znt->ver)==NMD_FAIL)
							{
			//					sprintf(str,"负荷(%s)的馈线和负荷关系建立失败!",ld_id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							
							//设备表的增加
							eq->Reset();
							eq->i_id = eq_cnt;
							eq->idph = ld_p->Ptr;
							strcpy(eq->id , ld_tb->GetDataByPh(ld_p->Ptr)->id);
							strcpy(eq->fdid , ld_tb->GetDataByPh(ld_p->Ptr)->fdid);
							eq->ind = ld_tb->GetDataByPh(ld_p->Ptr)->ind;
							eq->iznd = -1;
							eq->prob_fault = ld_tb->GetDataByPh(ld_p->Ptr)->prob_fault;
							
							//设备的修复时间
							eq->repairTime = ld_tb->GetDataByPh(ld_p->Ptr)->repairTime;

							eq->type = 20;
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;//馈线上设备的个数增加
							fd_tb->GetDataByPh(fdph)->cnt_ld++;//馈线上负荷的个数增加
							if( Insert_r_char_int(2,eq_cnt,fdph,fdid) == NMD_FAIL )
							{
			//					//printf("fail to insert r_fd_eq (%d)\n", i);
			//					sprintf(str,"设备(%s)的设备和馈线关系建立失败!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							if( Insert_r_char_int(0,eq_cnt++,eq->ind,ld_tb->GetDataByPh(ld_p->Ptr)->nd) == NMD_FAIL )
							{
			//					sprintf(str,"负荷(%s)的设备关系建立失败!",(eq->id));
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}

							cnt_ld++;//区段内的负荷个数加上1
							sum_lds++;//总的负荷个数也增加
							//区段内的总有功和总无功
							zn_sumLoad_p += ld_tb->GetDataByPh(ld_p->Ptr)->wm;
							//zn_sumLoad_q += ld_tb->GetDataByPh(ld_p->Ptr)->rm;
							so_sumLoad += ld_tb->GetDataByPh(ld_p->Ptr)->wm;
							ld_p = ld_p->Next;
						}
						::FreeChildPtr(ld_p0);
					}

					//read cbs connected to this nd
					//从节点-开关关系表找到所有和这个节点相连的开关
					cb_p = r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
					cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
					while(cb_p)
					{
						//find the opposite nd through this cb
						//kknd表示开关的对侧节点，这里的节点都是整型的编号
						if(knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
						{
							kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
							strcpy(eq_nd,cb_tb->GetDataByPh(cb_p->Ptr)->nd);
							strcpy(eq_znd,cb_tb->GetDataByPh(cb_p->Ptr)->znd);
						}
						else
						{
							kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
							strcpy(eq_znd,cb_tb->GetDataByPh(cb_p->Ptr)->nd);
							strcpy(eq_nd,cb_tb->GetDataByPh(cb_p->Ptr)->znd);
						}

						int ph = cb_p->Ptr;
						//如果开关是分的状态，都不要将节点加入到扩展节点，也不用加对侧节点进行回溯
						if  (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//联络开关的q为1，常开
							//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0) 
						{
							//(nd + kkndcb)->q &= (~ND_UNPROC);

							//联络开关的节点保存，准备回溯获得合并开关
							unionCB[unionCBcnt++] = knd;

							//增加开关，
							eq->Reset();
							eq->i_id = eq_cnt;
							eq->idph = cb_p->Ptr;
							strcpy(eq->id , cb_tb->GetDataByPh(cb_p->Ptr)->id);
							strcpy(eq->fdid , cb_tb->GetDataByPh(cb_p->Ptr)->fdid);
							eq->ind = knd;
							eq->iznd = kkndcb;//开关的节点按电源点的顺序排列
							eq->type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;
							fd_tb->GetDataByPh(fdph)->cnt_un++;//馈线上联络开关的个数增加
							if( Insert_r_char_int(2,eq_cnt,fdph,fdid) == NMD_FAIL )
							{
			//					sprintf(str,"开关(%s)的馈线和联络开关关系建立失败!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							if( Insert_r_char_int(0,eq_cnt,eq->ind,eq_nd) == NMD_FAIL ||
								Insert_r_char_int(0,eq_cnt++,eq->iznd,eq_znd) == NMD_FAIL )
							{
			//					sprintf(str,"开关(%s)的节点和设备关系建立失败!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								cb_p = cb_p->Next;
								//continue;
								return NMD_FAIL;
							}							
						}
						
						//process this opposite nd
						//如果对侧节点 已处理==开关不参与拓扑推进
						if ( (kkndcb!=-1) && ((nd+kkndcb)->q & ND_UNPROC) )
						{
							fd_tb->GetDataByPh(fdph)->cnt_nd++;//馈线上联络开关的个数增加

							//下面两行是准备做区段用的
							cb_zn_znd[cbndcnt] = knd;//准备扩展的节点开关内侧节点
							cb_zn_nd[cbndcnt++] = kkndcb;//存开关外侧的节点，准备写入区段表

							//下面三行是准备做区段用的
							cb_tb->GetDataByPh(cb_p->Ptr)->ind = knd; //开关真正的首节点
							cb_tb->GetDataByPh(cb_p->Ptr)->iznd = kkndcb;// 真正的末节点
							extcbptr[extcbcnt++] = cb_p->Ptr;
							
							//如果开关是分的状态，都不要将节点加入到扩展节点，也不用加对侧节点进行回溯
							if (cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)//分位非联络开关
							{
								int debug = 0;
							}
							else//闭合开关，准备向下扩展
							{		
								//只要是新找到的节点都要做关系
//								if (ND_BR_Insert(_R_FD_ND,ph, fdid, kkndcb, (nd+kkndcb)->id,fdid,"")==NMD_FAIL)
//									return NMD_FAIL;
//								fd_tb->GetDataByPh(ph)->cnt_nd++;//馈线上联络开关的个数增加
								
								//将开关的对侧节点存起来准备下一个区段扩展用
								cb_zn_intnd[extndcount] = knd;//准备扩展的节点开关内侧节点
								extnd[extndcount++] = kkndcb;//准备扩展的节点
							
								//将这个节点的上一级记录下来，准备回溯
								(nd+kkndcb)->bch = knd;

								//将找到的新节点设成已处理
								(nd+kkndcb)->q &= (~ND_UNPROC);
							
							//增加开关，
							eq->Reset();
							eq->i_id = eq_cnt;
							eq->idph = cb_p->Ptr;
							strcpy(eq->id , cb_tb->GetDataByPh(cb_p->Ptr)->id);
							strcpy(eq->fdid , cb_tb->GetDataByPh(cb_p->Ptr)->fdid);
							eq->ind = knd;
							eq->iznd = kkndcb;//开关的节点按电源点的顺序排列
							eq->prob_fault = cb_tb->GetDataByPh(cb_p->Ptr)->prob_fault;
							eq->repairTime = cb_tb->GetDataByPh(cb_p->Ptr)->repairTime;
							eq->type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;
							if ((sond == eq->ind) || (sond == eq->iznd))//出线开关
							{
								eq->type |= 16;
							}
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;//馈线上联络开关的个数增加
							if( Insert_r_char_int(2,eq_cnt,fdph,fdid) == NMD_FAIL )
							{
								//printf("fail to insert r_fd_eq (%d)\n", i);
			//					sprintf(str,"设备(%s)的馈线和设备关系建立失败!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							if( Insert_r_char_int(0,eq_cnt,eq->ind,eq_nd) == NMD_FAIL ||
								Insert_r_char_int(0,eq_cnt++,eq->iznd,eq_znd) == NMD_FAIL )
							{
								//printf("fail to insert r_nd_cb for cb (%d)\n", i);
			//					sprintf(str,"设备(%s)的节点和设备关系建立失败!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
								}
							}
						}
				//将区段进入的那个开关保存下来，为n-1校验使用，一个区段只能出现一次
						if(kkndcb == ext_cbnd)//区段进入的那个节点,区段的首节点
						{
							cb_zn_znd[cbndcnt] = knd;//准备扩展的节点开关内侧节点
							cb_zn_nd[cbndcnt++] = kkndcb;//存开关外侧的节点，准备写入区段表
						
							cb_tb->GetDataByPh(cb_p->Ptr)->ind = kkndcb; //开关真正的首节点
							cb_tb->GetDataByPh(cb_p->Ptr)->iznd = knd;// 真正的末节点

							extcbptr[extcbcnt++] = cb_p->Ptr;
							znt->cbptr = cb_p->Ptr;

							//将开关的对侧节点存起来准备下一个区段扩展用
						}

						cb_p = cb_p->Next;
					}
					::FreeChildPtr(cb_p0);

					//read secs connected to this nd
					sec_p = r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
					sec_p0 = sec_p;
					while(sec_p)
					{
						//find the opposite nd through this sec
						if(knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
						{
							kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
							strcpy(eq_nd,sec_tb->GetDataByPh(sec_p->Ptr)->nd);
							strcpy(eq_znd,sec_tb->GetDataByPh(sec_p->Ptr)->znd);
						}
						else
						{
							kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;
							strcpy(eq_znd,sec_tb->GetDataByPh(sec_p->Ptr)->nd);
							strcpy(eq_nd,sec_tb->GetDataByPh(sec_p->Ptr)->znd);
						}

//printf("%s\n",sec_tb->GetDataByPh(sec_p->Ptr)->id);
						//process this opposite nd
						if((nd+kknd)->q & ND_UNPROC)
						{
							strcpy(sec_id, sec_tb->GetDataByPh(sec_p->Ptr)->id);
							if (ND_BR_Insert(_R_ZN_SEC,zn_p, zn_id, sec_p->Ptr, sec_id,znt->fdid,znt->ver)==NMD_FAIL)
							{
			//					sprintf(str,"设备(%s)的区段和线段关系建立失败!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
//							if (ND_BR_Insert(_R_FD_ND,ph, fdid, kknd, (nd+kknd)->id,fdid,"")==NMD_FAIL)
//								return NMD_FAIL;
//							fd_tb->GetDataByPh(ph)->cnt_nd++;//馈线上联络开关的个数增加

							(nd+kknd)->fch = (nd+knd)->fch;
							//新节点 设成 扩展节点的前向链
							(nd+knd)->fch = kknd;

							//将这个节点的上一级记录下来，准备回溯
							(nd+kknd)->bch = knd;

							//将找到的新节点设成已处理
							(nd+kknd)->q &= (~ND_UNPROC);
							
							eq->Reset();
							eq->i_id = eq_cnt;
							eq->idph = sec_p->Ptr;
							strcpy(eq->id , sec_tb->GetDataByPh(sec_p->Ptr)->id);
							strcpy(eq->fdid , sec_tb->GetDataByPh(sec_p->Ptr)->fdid);
							eq->ind = knd;
							eq->iznd = kknd;
							eq->prob_fault = sec_tb->GetDataByPh(sec_p->Ptr)->prob_fault;
							eq->repairTime = sec_tb->GetDataByPh(sec_p->Ptr)->repairTime;

							//需要检修率，每个线段的检修率需要总的检修率*线段长度
							l = dsbob->OnGetPhByfdid(eq->fdid);//fd_tb->FindPh(FD(eq->fdid));
							eq->prob_check = sec_tb->GetDataByPh(sec_p->Ptr)->len * fd_tb->GetDataByPh(l)->plan_prob;

							eq->r_zn = sec_tb->GetDataByPh(sec_p->Ptr)->r_zn;
							eq->type = 10;
							sec_tb->GetDataByPh(sec_p->Ptr)->lC_zn =eq_cnt;//馈线段对应的设备i_id号
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;//馈线上联络开关的个数增加
							if( Insert_r_char_int(2,eq_cnt,fdph,fdid) == NMD_FAIL )
							{
								//printf("fail to insert r_fd_eq (%d)\n", i);
			//					sprintf(str,"设备(%s)的馈线和设备关系建立失败!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							if( Insert_r_char_int(0,eq_cnt,eq->ind,eq_nd) == NMD_FAIL ||
								Insert_r_char_int(0,eq_cnt++,eq->iznd,eq_znd) == NMD_FAIL )
							{
								//printf("fail to insert r_nd_sec for sec (%d)\n", i);
			//					sprintf(str,"设备(%s)的节点和设备关系建立失败!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
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
							strcpy(eq_nd, xf_tb->GetDataByPh(xf_p->Ptr)->nd);
							strcpy(eq_znd, xf_tb->GetDataByPh(xf_p->Ptr)->znd);
						}
						else
						{
							kknd = xf_tb->GetDataByPh(xf_p->Ptr)->ind;
							strcpy(eq_znd, xf_tb->GetDataByPh(xf_p->Ptr)->nd);
							strcpy(eq_nd, xf_tb->GetDataByPh(xf_p->Ptr)->znd);
						}

						//printf("%s\n",xf_tb->GetDataByPh(xf_p->Ptr)->id);
												//process this opposite nd
						if ((nd + kknd)->q & ND_UNPROC)
						{
							strcpy(xf_id, xf_tb->GetDataByPh(xf_p->Ptr)->id);
							if (ND_BR_Insert(_R_ZN_XF, zn_p, zn_id, xf_p->Ptr, xf_id, znt->fdid, znt->ver) == NMD_FAIL)
							{
								//sprintf(str, "设备(%s)的区段和线段关系建立失败!", eq->id);
								//lld->WriteLog(lld->logfile, str);
								return NMD_FAIL;
							}
							//							if (ND_BR_Insert(_R_FD_ND,ph, fdid, kknd, (nd+kknd)->id,fdid,"")==NMD_FAIL)
							//								return NMD_FAIL;
							//							fd_tb->GetDataByPh(ph)->cnt_nd++;//馈线上联络开关的个数增加

							(nd + kknd)->fch = (nd + knd)->fch;
							//新节点 设成 扩展节点的前向链
							(nd + knd)->fch = kknd;

							//将这个节点的上一级记录下来，准备回溯
							(nd + kknd)->bch = knd;

							//将找到的新节点设成已处理
							(nd + kknd)->q &= (~ND_UNPROC);

							eq->Reset();
							eq->i_id = eq_cnt;
							eq->idph = xf_p->Ptr;
							strcpy(eq->id, xf_tb->GetDataByPh(xf_p->Ptr)->id);
							strcpy(eq->fdid, xf_tb->GetDataByPh(xf_p->Ptr)->fdid);
							eq->ind = knd;
							eq->iznd = kknd;
							//eq->prob_fault = xf_tb->GetDataByPh(xf_p->Ptr)->prob_fault;
							//eq->repairTime = xf_tb->GetDataByPh(xf_p->Ptr)->repairTime;

							//需要检修率，每个线段的检修率需要总的检修率*线段长度
							l = dsbob->OnGetPhByfdid(eq->fdid);//fd_tb->FindPh(FD(eq->fdid));
							//eq->prob_check = xf_tb->GetDataByPh(xf_p->Ptr)->len * fd_tb->GetDataByPh(l)->plan_prob;

							eq->r_zn = xf_tb->GetDataByPh(xf_p->Ptr)->r_zn;
							eq->type = 10;
							//xf_tb->GetDataByPh(xf_p->Ptr)->lC_zn = eq_cnt;//馈线段对应的设备i_id号
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;//馈线上联络开关的个数增加
							if (Insert_r_char_int(2, eq_cnt, fdph, fdid) == NMD_FAIL)
							{
								//printf("fail to insert r_fd_eq (%d)\n", i);
								//sprintf(str, "设备(%s)电容器和设备关系建立失败!", eq->id);
								//lld->WriteLog(lld->logfile, str);
								return NMD_FAIL;
							}
							if (Insert_r_char_int(0, eq_cnt, eq->ind, eq_nd) == NMD_FAIL ||
								Insert_r_char_int(0, eq_cnt++, eq->iznd, eq_znd) == NMD_FAIL)
							{
								//printf("fail to insert r_nd_xf for xf (%d)\n", i);
								//sprintf(str, "设备(%s)的节点和设备关系建立失败!", eq->id);
								//lld->WriteLog(lld->logfile, str);
								return NMD_FAIL;
							}
						}
						xf_p = xf_p->Next;
					}
					::FreeChildPtr(xf_p0);

					//move to next nd，搜索完一层
					knd = (nd+knd)->fch;
					//循环到头，连起来了，表示全部走了一遍
					//拿另外的节点再开始搜索，一直到所有的节点都处理
					if(knd == ind)
					{
						//把区段的端点取出来写到区段表里面
						strcpy(znt->descr, zn_id);
                        strcpy(znt->fdid,so_tb->GetDataByPh(i)->fdid);//区段所属馈线
                        strcpy(znt->ver,so_tb->GetDataByPh(i)->ver);//区段所属馈线

						znt->type = cbndcnt;//至少会有一个
						znt->inner_C = cnt_ld;//区段内的负荷个数
						znt->inner_P = zn_sumLoad_p;//区段内的总有功
//						strcpy(znt->alg4q , lld->DoubleToString(zn_sumLoad_q));//区段内的负荷个数

						//zn_cb
						for (int ii=0; ii<cbndcnt;ii++)
						{
							icb = extcbptr[ii];
							strcpy(cb_id,cb_tb->GetDataByPh(icb)->id);												////ZN_CB_Insert(znt, znt->id, icb, cb_id);
							//ZN_CB_Insert(zn_p, znt->id, icb, cb_id);
							if (ND_BR_Insert(_R_ZN_CB,zn_p, znt->descr, icb, cb_id,znt->fdid,znt->ver) == NMD_FAIL)
							{
			//					sprintf(str,"馈线(%d)的区段和开关关系建立失败!",znt->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
						}
						
						znt->r_cb = zn_tb_t->GetDataByPh(zn_p)->r_cb;
						znt->r_sec = zn_tb_t->GetDataByPh(zn_p)->r_sec;
						znt->r_ld = zn_tb_t->GetDataByPh(zn_p)->r_ld;
						zn_tb_t->Append(*znt);

						fd_tb->GetDataByPh(fdph)->cnt_zn++;//馈线上联络开关的个数增加
						if( Insert_r_char_int(1,znt->id,fdph,fdid) == NMD_FAIL )
						{
							//printf("fail to insert r_fd_zn (%d)\n", i);
			//				sprintf(str,"馈线(%d)的馈线和区段关系建立失败!",znt->id);
			//lld->WriteLog(lld->logfile,str);
							return NMD_FAIL;

						}
						znt->Reset();
						znsum++;//插入的区段数累加
						break;
					}	
				}
			}
		}
		//写入整个电源点的变压器经过折扣口的总容量（全局负载率和单个变压器的负载率）
		//so_tb->GetDataByPh(i)->sr = so_sumLoad;


//////////////////////////////////////////////////////////////////////////
//做一次回溯，将开关的属性填上，是主干开关还是分支开关
//主干开关是 从电源点一直到联络开关的最短路径，因为是辐射网，只能有一条供电路径
//将路径上的节点记录下来，如果第二个联络开关也找到这个节点，就可以退出了
//这个节点可以标记为T节点，然后再来一次搜索就可以确定开关区段
		//先找到联络开关
		int lll = 0;
		int T_nd[100];//存T节点的数组
		int cnt_T=0;//T节点的个数
		int route_nd_all[500];//搜索路径上的节点总数
		int cnt_route=0;
		//int kknd;
		
		for (jj=0;jj<unionCBcnt;jj++)
		{
			knd = unionCB[jj];//联络开关的节点号
			kknd = knd;
			do
			{
				//将没有写入数组的节点写入到数组route_nd_all中，如果发现已经有了，一定是T节点
				if (!lld->addNode(route_nd_all,knd,cnt_route))
				{//T节点,从联络开关开始搜索时，只要遇到T节点就可以停止了
					//如果是联络开关节点自己，也不应该加入到T节点里面
					if (knd != kknd)
					{
						T_nd[cnt_T++] = knd;
						break;
					}
				}

				ind = knd;//本次开关的节点号
				knd = (nd+knd)->bch;//得到开关的上级节点号

				cb_p = r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while(cb_p)
				{
					//find the opposite nd through this cb
					//kknd表示开关的对侧节点，这里的节点都是整型的编号
					if ((((knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind) && (ind == cb_tb->GetDataByPh(cb_p->Ptr)->iznd))
					  ||((knd == cb_tb->GetDataByPh(cb_p->Ptr)->iznd) && (ind == cb_tb->GetDataByPh(cb_p->Ptr)->ind)))
					  && (cb_tb->GetDataByPh(cb_p->Ptr)->q !=1))
					{
						//将开关设定为主开关，主干开关包括联络开关和电源点开关
						//////////////////////////////////////////////////////////////////////////
						//如果开关没有分支，那么可以将其设为直线开关，2个连续的直线开关选一个就可以了。非主开关
						cb_tb->GetDataByPh(cb_p->Ptr)->maincb = 1;
						lll++;
					}			

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

			}	while(knd != sond);//一直到电源点为止

//		if (lll > 10)
//		{
//			printf("%s\n",so_tb->GetDataByPh(i)->id);	
//		}

		}

		//对主干网架进行二次扫描，得到开关区段
		//从联络点开始一直到T节点，只有一个主开关，相当于将直线上的开关合并成一个
		lld->sortInt(T_nd,cnt_T);//对所有的T节点进行排序
		for (jj=0;jj<unionCBcnt;jj++)
		{
			lll=0;
			knd = unionCB[jj];//联络开关的节点号
			do
			{
				if (lld->findNode(T_nd,knd,cnt_T) >= 0)
				{//T节点
					lll=0;
				}

				ind = knd;//本次开关的节点号
				knd = (nd+knd)->bch;//得到开关的上级节点号

				cb_p = r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while(cb_p)
				{
					//find the opposite nd through this cb
					//kknd表示开关的对侧节点，这里的节点都是整型的编号
					if ((((knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind) && (ind == cb_tb->GetDataByPh(cb_p->Ptr)->iznd))
					  ||((knd == cb_tb->GetDataByPh(cb_p->Ptr)->iznd) && (ind == cb_tb->GetDataByPh(cb_p->Ptr)->ind)))
					  && (cb_tb->GetDataByPh(cb_p->Ptr)->q !=1))
					{
						if (lll > 0)//在T节点前重复发现，lll==0已经保留了
						{
							//离联络开关最近的那个开关可以视为主开关
							//如果开关没有分支，那么可以将其设为直线开关，2个连续的直线开关选一个就可以了。非主开关
							cb_tb->GetDataByPh(cb_p->Ptr)->maincb = 0;
						}
						lll++;
					}			

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

			}	while(knd != sond);//一直到电源点为止
		}
	
	}
 	nd_tb->UpdateTable(nd);
	delete[] nd;
	nd = 0;
	lv_nd = 0;

	delete znt;
	delete eq;

	if (sum_lds == 0)
		return 0;

//////////////////////////////////////////////////////////////////////////
//输出部分校验结果

//孤立线段，不再区段内的线段
	int isolate=0;
	int r_zn;
	sec = sec_tb->GetTable();
	for(i = 0; i < sec_tb->GetCount(); i++)
	{
		//找区段
        r_zn = r_zn_sec_tb_t->FindParentPtr1((sec+i)->r_zn);
		
		if (r_zn == -1)//该线段不属于任何区段
		{
			sprintf(str,"warning:nmd校验时线段 %s 无法加入网络!\n",(sec+i)->id);
			//str.Format("warning:nmd校验时线段 %s 无法加入网络!",sec_tb->GetDataByLog(i)->id);
			lld->WriteLog(lld->logfile,str);
			isolate++;
		}
	}
//	char str[100];
	if (isolate > 0)
	{
		strcpy(str, "-------------------------");
		lld->WriteLog(lld->logfile,str);
		printf("warning:出现线段无法加入网络\n");
		sprintf(str,"warning: nmd校验时有 %d 个线段无法加入网络!",isolate);
		//str.Format("warning: nmd校验时有 %d 个线段无法加入网络!",isolate);
		lld->WriteLog(lld->logfile,str);
		//return NMD_FAIL
	}

//负荷	
	isolate=0;
	ld = ld_tb->GetTable();
	lv_ld = ld_tb->GetCount();

	for( i = 0; i < lv_ld; i++)
	{
        r_zn = r_zn_ld_tb_t->FindParentPtr1((ld+i)->r_zn);
		if (r_zn == -1)//该线段不属于任何区段
		{
			sprintf(str,"\nerror: nmd校验时负荷 %s 在孤岛内!",(ld+i)->id);
			//str.Format("error: nmd校验时负荷 %s 在孤岛内!",ld_tb->GetDataByLog(i)->id);
			lld->WriteLog(lld->logfile,str);
			//lld->add_globalexception(str);
			//strcat(globalexception,str);
			isolate++;
		}
		
	}

	if (isolate > 0)
	{
		strcpy(str, "-------------------------");
		lld->WriteLog(lld->logfile, str);
		printf("出现负荷在孤岛内的情况\n");
		sprintf(str,"error: nmd校验时有 %d 个负荷在孤岛内!",isolate);
		//str.Format("error: nmd校验时有 %d 个负荷在孤岛内!",isolate);
		lld->WriteLog(lld->logfile,str);
		//return NMD_FAIL;
	}

 //   int count;
	//ofstream outfile("lld_eq.txt",ios::out);
 // 	//nd_tb->Sequence();
 //	outfile<<"   "<< endl;
 //	outfile<<"eq:   "<< endl;
 // 	eq = eq_tb_t->GetTable();
 // 	count = eq_tb_t->GetCount();
	//outfile<<"i_id       "<<"type    "<<"id       "<<"ind       iznd"<<endl;
 // 	for(i = 0; i <count; i++)
 // 	{
 //		outfile<<i<<"   "<<(eq+i)->type<<"   "<<(eq+i)->id<<"   "<<(eq+i)->ind<< "   "<<(eq+i)->iznd<< endl;
 // 	}

	//outfile << "   " << endl;
	//outfile << "r_fd_eq:   " << endl;
	//r_fd_eq = r_fd_eq_tb_t->GetTable();
	//count = r_fd_eq_tb_t->GetCount();
	//outfile << "i_id       " << "eq_id    " << "fd_id       " << "eq_id_ptr       fd_id_ptr" << endl;
	//for (i = 0; i <count; i++)
	//{
	//	outfile << i << "   " << (r_fd_eq + i)->eq_id << "   " << (r_fd_eq + i)->fd_id << "   " << (r_fd_eq + i)->eq_id_ptr << "   " << (r_fd_eq + i)->fd_id_ptr << endl;
	//}

	//

	//outfile.close();

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  CB_Validate()
int NMD::CB_Validate()
{
	char str[500];
	if (lld->mx_cb==0)
		return 1;

	int		i;

	//read cb table into buffer
	lv_cb = cb_tb->GetCount();
	if(lv_cb == 0)
	{
		//printf("there is no record in cb table\n");
		//sprintf(str,"开关表中无记录!");
		//	lld->WriteLog(lld->logfile,str);
		return NMD_SUCCEED;
	}
	
	cb = new CB[lv_cb];
	cb_tb->CopyTable(cb);

	ND* ndt = new ND();
	//check cb one by one
	for(i = 0; i < lv_cb; i++)
	{
		strcpy(ndt->id , (cb + i)->nd);
		(cb+i)->ind = nd_tb->FindPh(*ndt);
		strcpy(ndt->id , (cb + i)->znd);
		(cb+i)->iznd = nd_tb->FindPh(*ndt);

		//if (strstr((cb + i)->id, "PD_30500000_374446")!=NULL)
		//{
		//	int debug = 0;
		//}

		//将开关的首末节点插入到节点表中,形成节点表（临时表）
//		if( ND_Insert((cb+i)->nd,&((cb+i)->ind),-1,(cb+i)->fdid,(cb+i)->ver) == NMD_SUCCEED &&
//			ND_Insert((cb+i)->znd,&((cb+i)->iznd),-1,(cb+i)->fdid,(cb+i)->ver) == NMD_SUCCEED )
		{
			//insert r_nd_cb table
			//首末节点分别插入,节点-支路关系表，确定一个节点对应几个支路
			if( ND_BR_Insert(_CB,(cb+i)->ind,(cb+i)->nd,i,(cb+i)->id,(cb+i)->fdid,(cb+i)->ver) == NMD_FAIL ||
				ND_BR_Insert(_CB,(cb+i)->iznd,(cb+i)->znd,i,(cb+i)->id,(cb+i)->fdid,(cb+i)->ver) == NMD_FAIL )
			{
				sprintf(str,"开关(%s)的节点和开关关系建立失败!可能该开关的节点连其他开关。",(cb + i)->id);
			lld->WriteLog(lld->logfile,str);
				return NMD_FAIL;
			}
		}
	}

	//update cb table
	cb_tb->UpdateTable(cb);

	delete ndt;
	delete[] cb;
	cb = 0;
	lv_cb = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  BUS_Validate()
int	NMD::BUS_Validate()
{
	char str[500];
	int		i, vl;

	if (lld->mx_bus==0)
	{
		//printf("There is not bus in network\n");
		//sprintf(str,"网络中无母线!");
		//	lld->WriteLog(lld->logfile,str);

		return 1;
	}
	//read bus table into buffer
	lv_bus = bus_tb->GetCount();
	if(lv_bus == 0)
	{
		return NMD_SUCCEED;
	}

	bus = new BUS[lv_bus];
	bus_tb->CopyTable(bus);

	ND* ndt = new ND();
	//check bus one by one
	for(i = 0; i < lv_bus; i++)
	{
//		if(ND_Insert((bus+i)->nd,&(bus+i)->ind,-1,(bus+i)->fdid,(bus+i)->ver) != NMD_SUCCEED)
//		{
//			printf("fail to insert nd for the bus (%d)\n", i);
//			sprintf(str,"母线(%s)插入节点失败!",(bus + i)->id);
//			lld->addLogLine(str);
//			return NMD_FAIL;
//		}
		strcpy(ndt->id,(bus + i)->nd);
		(bus+i)->ind = nd_tb->FindPh(*ndt);

		//check vl of this bus
		vl = (bus+i)->vl;
		//if(vl != 10 && vl != 35 && vl != 110 && vl != 220)
		if( (vl <= 8 || vl >= 12) && (vl <= 30 || vl >= 40) &&
			(vl <= 90 || vl >= 130) && (vl <= 190 || vl >= 240) 
			&& (vl <= 430 || vl >= 530) )
		{
//			printf("the vl of bus (%d) is illegal\n", i);
			//sprintf(str,"母线(%s)电压取值错误!",(bus + i)->id);
			//lld->WriteLog(lld->logfile,str);
			return NMD_FAIL;
		}
	}

	//update bus table
	bus_tb->UpdateTable(bus);
	delete ndt;
	delete[] bus;
	bus = 0;
	lv_bus = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  LD_Validate()
int NMD::LD_Validate()
{
	int		i;
	int		ph;
	char	str[500];

	//read ld table into buffer
	if (lld->mx_ld==0)
		return 1;
	
	lv_ld = ld_tb->GetCount();
	ld = new LD[lv_ld];
	ld_tb->CopyTable(ld);

	//check ld one by one
	for(i = 0; i < lv_ld; i++)
	{
		//if (strstr((ld+i)->id, "PD_30200003_156953"))
		//{
		//	int degug = 0;
		//}

		if((ld+i)->type & Q_LOAD)  //一般负荷
		{
			ND* nd1 = new ND();
			strcpy(nd1->id , (ld+i)->nd);
			ph = nd_tb->FindPh(*nd1);
			if ((ld+i)->topflags == LD_SO)
				nd1->q |= ND_LD | ND_SO;
			else
				nd1->q |= ND_LD;
			nd_tb->UpdateDataByPh(nd1,ph);
			delete nd1;
			(ld+i)->ind = ph;

			//if(ND_Insert((ld+i)->nd,&(ld+i)->ind,nd_q,(ld+i)->fdid,(ld+i)->ver) == NMD_SUCCEED)
			{//插入节点负荷关系表
				if( ND_BR_Insert(_LD,(ld+i)->ind,(ld+i)->nd,i,(ld+i)->id,(ld+i)->fdid,(ld+i)->ver) == NMD_FAIL)
				{
					//printf("fail to insert r_nd_ld for ld (%d)\n", i);
					
			//		sprintf(str,"负荷(%s)的节点和负荷关系建立失败。",(ld + i)->id);
			//lld->WriteLog(lld->logfile,str);
					return NMD_FAIL;
				}
			}
		}
	}

	//update ld table
	ld_tb->UpdateTable(ld);

	delete[] ld;
	ld = 0;
	lv_ld = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  SEC_Validate()
int NMD::SEC_Validate()
{
	if (lld->mx_sec==0)
		return 1;
	char str[500];
	int		i;

	//read sec table into buffer
	lv_sec = sec_tb->GetCount();
	if(lv_sec == 0)
	{
		return NMD_SUCCEED;
	}
	
	sec = new SEC[lv_sec];
	sec_tb->CopyTable(sec);

	ND* ndt = new ND();
	//check sec one by one
	for(i = 0; i < lv_sec; i++)
	{
		strcpy(ndt->id, (sec + i)->nd);
			(sec+i)->ind = nd_tb->FindPh(*ndt);
			strcpy(ndt->id, (sec + i)->znd);
			(sec+i)->iznd = nd_tb->FindPh(*ndt);

			//if (strstr((sec + i)->id, "PD_36000000_4943911")!=NULL)
			//{
			//	int debug = 0;
			//}



//		if( ND_Insert((sec+i)->nd,&((sec+i)->ind),-1,(sec+i)->fdid,(sec+i)->ver) == NMD_SUCCEED &&
//			ND_Insert((sec+i)->znd,&((sec+i)->iznd),-1,(sec+i)->fdid,(sec+i)->ver) == NMD_SUCCEED )
		{
			if( ND_BR_Insert(_SEC,(sec+i)->ind,(sec+i)->nd,i,(sec+i)->id,(sec+i)->fdid,(sec+i)->ver) == NMD_FAIL ||
				ND_BR_Insert(_SEC,(sec+i)->iznd,(sec+i)->znd,i,(sec+i)->id,(sec+i)->fdid,(sec+i)->ver) == NMD_FAIL )
			{
				//printf("fail to insert r_nd_sec for sec (%d)\n", i);
			//	sprintf(str,"线段(%s)的节点和线段关系建立失败!",(sec+ i)->id);
			//lld->WriteLog(lld->logfile,str);
				return NMD_FAIL;
			}
		}
	}

	//update sec table
	sec_tb->UpdateTable(sec);

	delete ndt;
	delete[] sec;
	sec = 0;
	lv_sec = 0;

	return NMD_SUCCEED;
}

int NMD::Insert_r_char_int(int type,int eq_ptr,int nd_ptr,char* nd_id)
{
	int		phpos, fptr;
	switch( type )
	{
	case 0:  //节点-设备关系
		{
		if ( (nd_ptr == -1) || (strlen(nd_id)==0) || (strcmp(nd_id,"-")==0) )
			return 1;

		R_ND_EQ* r_equip_nd = new R_ND_EQ;
		*r_equip_nd = R_ND_EQ(nd_id,eq_ptr);
		r_equip_nd->equip_id_ptr = eq_ptr;
		r_equip_nd->nd_id_ptr = nd_ptr;
		phpos = (nd+nd_ptr)->r_eq;//得到节点表中和开关的关联信息
		if(phpos == -1)  //first connected cb of this nd节点没有连接开关
		{
			(nd+nd_ptr)->r_eq = r_nd_eq_tb_t->GetNext();
		}
		else  //build cb chain for this nd，如果节点以前已经连接了开关
		{
			//将节点开关关系表中的位置向后挪动，准备新的节点关系指针插入
			fptr = r_nd_eq_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_nd_eq_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_eq_tb_t->GetDataByPh(phpos)->fptr = r_nd_eq_tb_t->GetNext();
			r_equip_nd->bptr = phpos;
		}
		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_equip_nd->id = r_nd_eq_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_nd_eq_tb_t->Append(*r_equip_nd);
		if(phpos == -1)//判断是否越界
		{
			return NMD_FAIL;
		}
		delete r_equip_nd;
		r_equip_nd = 0;

		break;
		}
	case 1:  //馈线-区段关系
		{
		r_fd_zn = new R_FD_ZN;
		*r_fd_zn = R_FD_ZN(nd_id, eq_ptr);//构造区段和开关的关系
		r_fd_zn->fd_id_ptr = nd_ptr;//区段的指针，也就是区段的物理位置
		r_fd_zn->zn_id_ptr = eq_ptr;//zn_tb_t->GetPhByLog(eq_ptr);//开关的指针

		//strcpy(r_fd_zn->fdid , fdid);//节点所属馈线
		//strcpy(r_fd_zn->ver , ver);//节点所属馈线
		
		zn_tb_t->GetDataByPh(eq_ptr)->r_fd = r_fd_zn_tb_t->GetNext();//准备插入到关系表中的物理位置
		phpos = fd_tb->GetDataByPh(nd_ptr)->r_zn;//得到区段表中和开关的关联信息
		if(phpos == -1)  //first connected sec of this zn区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			fd_tb->GetDataByPh(nd_ptr)->r_zn = r_fd_zn_tb_t->GetNext();
		}
		else  //build sec chain for this zn，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_fd_zn_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_zn_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_zn_tb_t->GetDataByPh(phpos)->fptr = r_fd_zn_tb_t->GetNext();
			r_fd_zn->bptr = phpos;
		}
//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_fd_zn->id = r_fd_zn_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_fd_zn_tb_t->Append(*r_fd_zn);
		if(phpos == -1)//判断是否越界
		{
			delete r_fd_zn;
			r_fd_zn = 0;
			return NMD_FAIL;
		}
		delete r_fd_zn;
		r_fd_zn = 0;
		
		break;
		}
	case 2:  //馈线-设备关系
		{
		r_fd_eq = new R_FD_EQ;
		*r_fd_eq = R_FD_EQ(nd_id, eq_ptr);//构造区段和开关的关系
		r_fd_eq->fd_id_ptr = nd_ptr;//区段的指针，也就是区段的物理位置
		r_fd_eq->eq_id_ptr = eq_ptr;//dsbob->eq_tb->GetPhByLog(eq_ptr);//开关的指针

		//strcpy(r_fd_eq->fdid , fdid);//节点所属馈线
		//strcpy(r_fd_eq->ver , ver);//节点所属馈线
		
		eq_tb_t->GetDataByPh(eq_ptr)->r_fd = r_fd_eq_tb_t->GetNext();//准备插入到关系表中的物理位置
		phpos = fd_tb->GetDataByPh(nd_ptr)->r_eq;//得到区段表中和开关的关联信息
		if(phpos == -1)  //first connected sec of this eq区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			fd_tb->GetDataByPh(nd_ptr)->r_eq = r_fd_eq_tb_t->GetNext();
		}
		else  //build sec chain for this eq，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_fd_eq_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_eq_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_eq_tb_t->GetDataByPh(phpos)->fptr = r_fd_eq_tb_t->GetNext();
			r_fd_eq->bptr = phpos;
		}
//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_fd_eq->id = r_fd_eq_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_fd_eq_tb_t->Append(*r_fd_eq);
		if(phpos == -1)//判断是否越界
		{
			delete r_fd_eq;
			r_fd_eq = 0;
			return NMD_FAIL;
		}
		delete r_fd_eq;
		r_fd_eq = 0;
		
		break;
		}
	}

	return 1;
}

//通过一个给定的节点找到馈线ID
int NMD::OnGetFDByND(char* fdid,int ind)
{
	int ret = -1;

	int			knd, kkndcb;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;

	fdid[0] = 0;
	int bFind = 0;

	lv_nd = nd_tb->GetCount();
	ND* nd_t = new ND[lv_nd];
	nd_tb->CopyTable(nd_t);

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	for(int i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

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
			if ((nd_t+knd)->q & ND_LD) 
			{
				//得到负荷id
				ld_p = r_nd_ld_tb->FindChildPtr1((nd_t+knd)->r_ld);
				ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
				while(ld_p)
				{
					strcpy(fdid , ld_tb->GetDataByPh(ld_p->Ptr)->fdid);
					bFind = 1;
					break;

					ld_p = ld_p->Next;
				}
				::FreeChildPtr(ld_p0);

				if (bFind)
					break;
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

				//如果是联络开关，停下来,只要是联络开关，不管是否悬空，都需要进行处理
				if  ((cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//联络开关的q为1，常开
					//|| (cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB))  
					//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
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
				}

				cb_p = cb_p->Next;
			}
			::FreeChildPtr(cb_p0);

			//read secs connected to this nd
			sec_p = r_nd_sec_tb->FindChildPtr1((nd_t+knd)->r_sec);
			sec_p0 = sec_p;
			while(sec_p)
			{
				strcpy(fdid,sec_tb->GetDataByPh(sec_p->Ptr)->fdid);
				bFind = 1;
				break;

				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			if (bFind)
				break;

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

	if (bFind)
		ret = 1;

	delete[] nd_t;
	nd_t = 0;

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
//  FD_Validate()
//通过馈线找它周边的连接馈线,馈线和联络开关的关系表，馈线fd-父，联络fds-子
int NMD::FD_Validate()
{
	//	char left[32],right[32];
	char str[500];//,sTemp1[MAX_UNIN_ID_SUM];
	int		i;
	int		l, r, cb_l, cb_r;
	char	un_id[MAX_ZN_ID_LEN];
	char	fdid[MAX_SEC_ID_LEN], fd_left[MAX_SEC_ID_LEN], fd_right[MAX_SEC_ID_LEN];

	if (lld->mx_un <= 0)//网络中没有联络开关
	{
		lld->mx_r_fd_un = 0;
		return 1;
	}
	//read un table into buffer
	lv_un = un_tb->GetCount();
	if (lv_un == 0)
	{
		sprintf(str, "联络开关表中无记录!");
		lld->WriteLog(lld->logfile, str);
		return NMD_SUCCEED;
	}

	//	ChildPtr1	*sec_p, *sec_p0;
	//	ChildPtr1	*cb_p, *cb_p0;

	CB* cbt = new CB();

	un = new UN[lv_un];
	un_tb->CopyTable(un);

	nd = nd_tb->GetTable();
	//check uns one by one
	for (i = 0; i < lv_un; i++)
	{
		//从un里把开关的联络馈线写出来
		strcpy(un_id, (un + i)->id);

		//1.建立馈线和联络开关的关系表，馈线是父，联络开关是子
		l = dsbob->OnGetPhByfdid((un + i)->left);//fd_tb->FindPh(FD((un+i)->left));
		if (l == -1)
			continue;
		fd_tb->GetDataByPh(l)->cnt_un++;//馈线上联络开关的个数增加
		r = dsbob->OnGetPhByfdid((un + i)->right);//fd_tb->FindPh(FD((un+i)->right));
		if (r == -1)
		{
			//默认的值
			if (ND_BR_Insert(_FDS, l, (un + i)->left, i, (un + i)->id, "", "") == NMD_FAIL)
			{
				//sprintf(str, "馈线(%s)的馈线和联络开关关系建立失败!", (sec + i)->id);
				//lld->WriteLog(lld->logfile, str);
				return NMD_FAIL;
			}
			strcpy(cbt->id, un_id);
			l = cb_tb->FindPh(*cbt);
			(un + i)->idph = l;//开关的物理号
			cb_l = cb_tb->GetDataByPh(l)->ind;	//开关的左节点
			cb_r = cb_tb->GetDataByPh(l)->iznd;	//开关的右节点
			if (cb_l != -1)
			{
				strcpy((un + i)->l_nd, (nd + cb_l)->id);
				(un + i)->r_nd[0] = 0;
			}
			else if (cb_r != -1)
			{
				strcpy((un + i)->l_nd, (nd + cb_r)->id);
				(un + i)->r_nd[0] = 0;
			}
			continue;
		}

		fd_tb->GetDataByPh(r)->cnt_un++;
		if (ND_BR_Insert(_FDS, l, (un + i)->left, i, (un + i)->id, "", "") == NMD_FAIL ||
			ND_BR_Insert(_FDS, r, (un + i)->right, i, (un + i)->id, "", "") == NMD_FAIL)
		{
			//printf("fail to insert r_fd_un (%d)\n", i);
			//sprintf(str, "馈线(%s)的馈线和联络开关关系建立失败!", (sec + i)->id);
			//lld->WriteLog(lld->logfile, str);
			return NMD_FAIL;
		}

		//给联络开关填上左右节点，因为不知道是哪个开关连着的
		//cb_l->fd_left;
		strcpy(cbt->id, un_id);
		l = cb_tb->FindPh(*cbt);
		(un + i)->idph = l;//开关的物理号
		cb_l = cb_tb->GetDataByPh(l)->ind;	//开关的左节点
		cb_r = cb_tb->GetDataByPh(l)->iznd;	//开关的右节点
		strcpy(fd_left, (un + i)->left);//左边的馈线
		strcpy(fd_right, (un + i)->right);//右边的馈线

		//通过开关左边的节点找到了馈线
		if ((cb_l != -1) && (OnGetFDByND(fdid, cb_l)))
		{
			if (strcmp(fdid, fd_left) == 0)
			{
				strcpy((un + i)->l_nd, (nd + cb_l)->id);
				if (cb_r != -1)
					strcpy((un + i)->r_nd, (nd + cb_r)->id);
				else
					(un + i)->r_nd[0] = 0;
			}
			else
			{
				strcpy((un + i)->r_nd, (nd + cb_l)->id);
				if (cb_r != -1)
					strcpy((un + i)->l_nd, (nd + cb_r)->id);
				else
					(un + i)->l_nd[0] = 0;
			}
		}
		else//通过右节点来找
			if ((cb_r != -1) && (OnGetFDByND(fdid, cb_r)))
			{
				if (strcmp(fdid, fd_left) == 0)
				{
					strcpy((un + i)->l_nd, (nd + cb_r)->id);
					if (cb_l != -1)
						strcpy((un + i)->r_nd, (nd + cb_l)->id);
					else
						(un + i)->r_nd[0] = 0;
				}
				else
				{
					strcpy((un + i)->r_nd, (nd + cb_r)->id);
					if (cb_l != -1)
						strcpy((un + i)->l_nd, (nd + cb_l)->id);
					else
						(un + i)->l_nd[0] = 0;
				}
			}
	}
	un_tb->UpdateTable(un);
	delete[] un;
	un = 0;
	lv_un = 0;

	delete cbt;

	//输出结果
		//ofstream outfile_debug("debug_un.txt",ios::out);
		//lv_un = un_tb->GetCount();
		//un = un_tb->GetTable();
		//for(i = 0; i < lv_un; i++)
		//{
		//	outfile_debug<<"id: "<<(un+i)->id<<" left:"<<(un+i)->left<<" right:"<<(un+i)->right<<" nd_left:"<<(un+i)->l_nd<<"nd_right:"<<(un+i)->r_nd<<endl;
		//}

		//lv_fd = fd_tb->GetCount();
		//fd = fd_tb->GetTable();
		//for (i = 0; i < lv_fd; i++)
		//{
		//	outfile_debug << "id: " << (fd + i)->id << " uncnt:" << (fd + i)->cnt_un << " ldcnt:" << (fd + i)->cnt_ld << " socnt:" << (fd + i)->cnt_so << "zncnt:" << (fd + i)->cnt_zn << endl;
		//}

		//outfile_debug.close();
	
		//ofstream outfile_debug1("debug_nd.txt",ios::out);
		//lv_nd = nd_tb->GetCount();
		//nd = nd_tb->GetTable();
		//for(i = 0; i < lv_nd; i++)
		//{
		//	outfile_debug1<<"id: "<<(nd+i)->id<<endl;
		//}
		//outfile_debug1.close();

	return NMD_SUCCEED;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  R_ST_CB_RebuildPtr()
int NMD::R_ST_CB_RebuildPtr()
{
	int		i;

	//read table into buffer
	if (r_st_cb_tb == NULL)
		return 1;

	lv_r_st_cb = r_st_cb_tb->GetCount();
	if (lv_r_st_cb == 0)
	{
		return NMD_SUCCEED;
	}

	r_st_cb = new R_ST_CB[lv_r_st_cb];
	r_st_cb_tb->CopyTable(r_st_cb);

	//rebuild pointer
	for (i = 0; i < lv_r_st_cb; i++)
	{
		if ((r_st_cb + i)->st_id_ptr != -1)
		{
			(r_st_cb + i)->st_id_ptr = st_tb->GetLogByPh((r_st_cb + i)->st_id_ptr);
		}
		if ((r_st_cb + i)->cb_id_ptr != -1)
		{
			(r_st_cb + i)->cb_id_ptr = cb_tb->GetLogByPh((r_st_cb + i)->cb_id_ptr);
		}
		if ((r_st_cb + i)->fptr != -1)
		{
			(r_st_cb + i)->fptr = r_st_cb_tb->GetLogByPh((r_st_cb + i)->fptr);
		}
		if ((r_st_cb + i)->bptr != -1)
		{
			(r_st_cb + i)->bptr = r_st_cb_tb->GetLogByPh((r_st_cb + i)->bptr);
		}
	}

	//update table
	r_st_cb_tb->UpdateTable(r_st_cb);

	delete[] r_st_cb;
	r_st_cb = 0;
	lv_r_st_cb = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ST_SO_RebuildPtr()
int NMD::R_ST_SO_RebuildPtr()
{
	int		i;

	if (r_st_so_tb == NULL)
		return 1;

	//read table into buffer
	lv_r_st_so = r_st_so_tb->GetCount();
	if (lv_r_st_so == 0)
	{
		return NMD_SUCCEED;
	}

	r_st_so = new R_ST_SO[lv_r_st_so];
	r_st_so_tb->CopyTable(r_st_so);

	//rebuild pointer
	for (i = 0; i < lv_r_st_so; i++)
	{
		if ((r_st_so + i)->st_id_ptr != -1)
		{
			(r_st_so + i)->st_id_ptr = st_tb->GetLogByPh((r_st_so + i)->st_id_ptr);
		}
		if ((r_st_so + i)->so_id_ptr != -1)
		{
			(r_st_so + i)->so_id_ptr = so_tb->GetLogByPh((r_st_so + i)->so_id_ptr);
		}
		if ((r_st_so + i)->fptr != -1)
		{
			(r_st_so + i)->fptr = r_st_so_tb->GetLogByPh((r_st_so + i)->fptr);
		}
		if ((r_st_so + i)->bptr != -1)
		{
			(r_st_so + i)->bptr = r_st_so_tb->GetLogByPh((r_st_so + i)->bptr);
		}
	}

	//update table
	r_st_so_tb->UpdateTable(r_st_so);

	delete[] r_st_so;
	r_st_so = 0;
	lv_r_st_so = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ST_LD_RebuildPtr()
int NMD::R_ST_LD_RebuildPtr()
{
	int		i;
	if (r_st_ld_tb == NULL)
		return 1;

	//read table into buffer
	lv_r_st_ld = r_st_ld_tb->GetCount();
	if (lv_r_st_ld == 0)
	{
		return NMD_SUCCEED;
	}

	r_st_ld = new R_ST_LD[lv_r_st_ld];
	r_st_ld_tb->CopyTable(r_st_ld);

	//rebuild pointer
	for (i = 0; i < lv_r_st_ld; i++)
	{
		if ((r_st_ld + i)->st_id_ptr != -1)
		{
			(r_st_ld + i)->st_id_ptr = st_tb->GetLogByPh((r_st_ld + i)->st_id_ptr);
		}
		if ((r_st_ld + i)->ld_id_ptr != -1)
		{
			(r_st_ld + i)->ld_id_ptr = ld_tb->GetLogByPh((r_st_ld + i)->ld_id_ptr);
		}
		if ((r_st_ld + i)->fptr != -1)
		{
			(r_st_ld + i)->fptr = r_st_ld_tb->GetLogByPh((r_st_ld + i)->fptr);
		}
		if ((r_st_ld + i)->bptr != -1)
		{
			(r_st_ld + i)->bptr = r_st_ld_tb->GetLogByPh((r_st_ld + i)->bptr);
		}
	}

	//update table
	r_st_ld_tb->UpdateTable(r_st_ld);

	delete[] r_st_ld;
	r_st_ld = 0;
	lv_r_st_ld = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ST_CP_RebuildPtr()
int NMD::R_ST_CP_RebuildPtr()
{
	int		i;
	if (r_st_cp_tb == NULL)
		return 1;

	//read table into buffer
	lv_r_st_cp = r_st_cp_tb->GetCount();
	if (lv_r_st_cp == 0)
	{
		return NMD_SUCCEED;
	}

	r_st_cp = new R_ST_CP[lv_r_st_cp];
	r_st_cp_tb->CopyTable(r_st_cp);

	//rebuild pointer
	for (i = 0; i < lv_r_st_cp; i++)
	{
		if ((r_st_cp + i)->st_id_ptr != -1)
		{
			(r_st_cp + i)->st_id_ptr = st_tb->GetLogByPh((r_st_cp + i)->st_id_ptr);
		}
		if ((r_st_cp + i)->cp_id_ptr != -1)
		{
			(r_st_cp + i)->cp_id_ptr = cp_tb->GetLogByPh((r_st_cp + i)->cp_id_ptr);
		}
		if ((r_st_cp + i)->fptr != -1)
		{
			(r_st_cp + i)->fptr = r_st_cp_tb->GetLogByPh((r_st_cp + i)->fptr);
		}
		if ((r_st_cp + i)->bptr != -1)
		{
			(r_st_cp + i)->bptr = r_st_cp_tb->GetLogByPh((r_st_cp + i)->bptr);
		}
	}

	//update table
	r_st_cp_tb->UpdateTable(r_st_cp);

	delete[] r_st_cp;
	r_st_cp = 0;
	lv_r_st_cp = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ST_BUS_RebuildPtr()
int NMD::R_ST_BUS_RebuildPtr()
{
	int		i;
	if (r_st_bus_tb == NULL)
		return 1;

	//read table into buffer
	lv_r_st_bus = r_st_bus_tb->GetCount();
	if (lv_r_st_bus == 0)
	{
		return NMD_SUCCEED;
	}

	r_st_bus = new R_ST_BUS[lv_r_st_bus];
	r_st_bus_tb->CopyTable(r_st_bus);

	//rebuild pointer
	for (i = 0; i < lv_r_st_bus; i++)
	{
		if ((r_st_bus + i)->st_id_ptr != -1)
		{
			(r_st_bus + i)->st_id_ptr = st_tb->GetLogByPh((r_st_bus + i)->st_id_ptr);
		}
		if ((r_st_bus + i)->bus_id_ptr != -1)
		{
			(r_st_bus + i)->bus_id_ptr = bus_tb->GetLogByPh((r_st_bus + i)->bus_id_ptr);
		}
		if ((r_st_bus + i)->fptr != -1)
		{
			(r_st_bus + i)->fptr = r_st_bus_tb->GetLogByPh((r_st_bus + i)->fptr);
		}
		if ((r_st_bus + i)->bptr != -1)
		{
			(r_st_bus + i)->bptr = r_st_bus_tb->GetLogByPh((r_st_bus + i)->bptr);
		}
	}

	//update table
	r_st_bus_tb->UpdateTable(r_st_bus);

	delete[] r_st_bus;
	r_st_bus = 0;
	lv_r_st_bus = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ND_XF_RebuildPtr()
int NMD::R_ND_XF_RebuildPtr()
{
	int		i;

	//read table into buffer
	lv_r_nd_xf = r_nd_xf_tb->GetCount();
	if (lv_r_nd_xf == 0)
	{
		return NMD_SUCCEED;
	}

	r_nd_xf = new R_ND_XF[lv_r_nd_xf];
	r_nd_xf_tb->CopyTable(r_nd_xf);

	//rebuild pointer
	for (i = 0; i < lv_r_nd_xf; i++)
	{
		if ((r_nd_xf + i)->nd_id_ptr != -1)
		{
			(r_nd_xf + i)->nd_id_ptr = nd_tb->GetLogByPh((r_nd_xf + i)->nd_id_ptr);
		}
		if ((r_nd_xf + i)->xf_id_ptr != -1)
		{
			(r_nd_xf + i)->xf_id_ptr = xf_tb->GetLogByPh((r_nd_xf + i)->xf_id_ptr);
		}
		if ((r_nd_xf + i)->fptr != -1)
		{
			(r_nd_xf + i)->fptr = r_nd_xf_tb->GetLogByPh((r_nd_xf + i)->fptr);
		}
		if ((r_nd_xf + i)->bptr != -1)
		{
			(r_nd_xf + i)->bptr = r_nd_xf_tb->GetLogByPh((r_nd_xf + i)->bptr);
		}
	}

	//update table
	r_nd_xf_tb->UpdateTable(r_nd_xf);

	delete[] r_nd_xf;
	r_nd_xf = 0;
	lv_r_nd_xf = 0;

	return NMD_SUCCEED;
}


int NMD::R_ST_XFMR_Build()
{
	if ((lld->mx_st == 0) || (lld->mx_xfmr == 0) )
	{
		return NMD_SUCCEED;
	}

	int		i, j;
	int		first, prevph;
	char	st_id[MAX_ST_ID_LEN], st_abb[MAX_ST_ABB_LEN];

	//get record count in st table
	lv_st = st_tb->GetCount();
	if (lv_st == 0)
	{
		return NMD_SUCCEED;
	}

	//get record count in xfmr table
	lv_xfmr = xfmr_tb->GetCount();
	if (lv_xfmr == 0)
	{
		return NMD_SUCCEED;
	}

	//read st table into buffer
	st = new ST[lv_st];
	st_tb->CopyTable(st);

	//read xfmr table into buffer
	xfmr = new XFMR[lv_xfmr];
	xfmr_tb->CopyTable(xfmr);

	//build relation between st and xfmr by Ids
	r_st_xfmr = new R_ST_XFMR;
	for (i = 0; i < lv_st; i++)
	{
		first = 1;
		strcpy(st_id, (st + i)->id);
		strcpy(st_abb, (st + i)->abb);
		for (j = 0; j < lv_xfmr; j++)
		{
			if (strcmp(st_id, (xfmr + j)->st)==0)
			//if (strncmp(st_abb, (xfmr + j)->id, ST_ABB_LEN) == 0)  //Attention: using abb
			{
				*r_st_xfmr = R_ST_XFMR(st_id, (xfmr + j)->id);
				r_st_xfmr->st_id_ptr = st_tb->GetPhByLog(i);
				r_st_xfmr->xfmr_id_ptr = xfmr_tb->GetPhByLog(j);

				(xfmr + j)->r_st = r_st_xfmr_tb->GetNext();

				if (first)
				{
					(st + i)->r_xfmr = r_st_xfmr_tb->GetNext();
					first = 0;
				}
				else
				{
					r_st_xfmr->bptr = prevph;
					r_st_xfmr_tb->GetDataByPh(prevph)->fptr = r_st_xfmr_tb->GetNext();
				}

				prevph = r_st_xfmr_tb->Insert(*r_st_xfmr);
				if (prevph == -1)
				{
					return NMD_FAIL;
				}
			}
		}
	}
	delete r_st_xfmr;
	r_st_xfmr = 0;

	//update st and xfmr table
	st_tb->UpdateTable(st);
	xfmr_tb->UpdateTable(xfmr);

	delete[] st;
	delete[] xfmr;
	st = 0;
	xfmr = 0;
	lv_st = 0;
	lv_xfmr = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  XFMR_Validate()对变压器表进行校验，将结果写入到变压器表和绕组表
int	NMD::XFMR_Validate()
{
	if (lld->mx_xfmr == 0)
		return 1;

	char str[500];
	int		i, prevph, ist,ph;
	int		hnd, mnd, lnd, cnd;
	char	xf_id[MAX_XF_ID_LEN], nd_id[MAX_ND_ID_LEN];
	ND* nd1 = new ND();
	TAPTY* tapty1 = new TAPTY();
	//first build relation between st and xfmr
	if (R_ST_XFMR_Build() == NMD_FAIL)
	{
		//printf("the r_st_xfmr table has not built up\n");
		return NMD_FAIL;
	}

	//read xfmr table into buffer
	lv_xfmr = xfmr_tb->GetCount();
	if (lv_xfmr == 0)
	{
		//printf("there is no record in xfmr table\n");
	//		CString str;
	//		str.Format("变压器表中无记录!");
	//		lld.addLogLine(str);
		return NMD_SUCCEED;
	}

	//remove all xf records
	//	xf_tb->Empty();

	xfmr = new XFMR[lv_xfmr];
	xfmr_tb->CopyTable(xfmr);

	st = new ST;
	xf = new XF;
	r_xfmr_xf = new R_XFMR_XF;

	//check xfmr one by one
	for (i = 0; i < lv_xfmr; i++)
	{
		//if ((xfmr + i)->r_st == -1)
		//{
		//	printf("there is no record in r_st_xfmr table about xfmr(%d)\n", i);
		//	global->dberrnum++;
		//	continue;
		//}
		//else
		{
			//check st of this xfmr
			//ist = (xfmr + i)->r_st;
			//ist = r_st_xfmr_tb->GetDataByPh(ist)->st_id_ptr;
			//st_tb->CopyDataByPh(st, ist);
			//if (strcmp(st->id, (xfmr + i)->st) != 0)
			//{
			//	printf("the st of xfmr (%d) is not correct\n", i);
			//	global->dberrnum++;
			//	continue;
			//}

			//check two-winding transformer
			if ((xfmr + i)->type == 2)
			{
								//check nds of xfmr
				strcpy(nd1->id, (xfmr + i)->hnd);
				hnd = nd_tb->FindPh(*nd1);
				strcpy(nd1->id, (xfmr + i)->lnd);
				lnd = nd_tb->FindPh(*nd1);
				//if(hnd == -1)
								//{
								//	//sprintf(str,"错误：************变压器(%s)高压侧节点(%s)无连接元件!",(xfmr + i)->id,(xfmr+i)->hnd);
								//	//lld.addLogLine(str);
								//	//return NMD_FAIL;
								//}
				//				lnd = nd_tb->FindPh(ND((xfmr+i)->lnd));
				//				//如果低压侧无接点，可以单独插入一个节点，高压侧不能没有节点
				//				if(lnd == -1)
				//				{
				////lld add 2010.1.13 for null nd in xfmr
				//					nd = new ND((xfmr+i)->lnd);
				//					int phpos = nd_tb->Insert(*nd);
				//					if( phpos == -1 )
				//					{
				//						//printf("fail to insert nd table\n");
				//						//sprintf(str,"节点表中插入变压器末端悬空记录%s失败!",(xfmr+i)->lnd);
				//						//lld.addLogLine(str);
				//						//return NMD_FAIL;
				//					}
				//					delete nd;
				//					nd = 0;
				////lld add end 2010.1.13 for null nd in xfmr
									//sprintf(str,"变压器(%s)低压侧节点(%s)无连接元件!",(xfmr + i)->id,(xfmr+i)->lnd);
									//lld.addLogLine(str);
									//return NMD_FAIL;
								//}

								//check tap type
				//if (((xfmr + i)->htapty[0] != '\0') && ((xfmr + i)->htapty[0] != '0'))//填了抽头类型，说明有抽头
				//{
				//	strcpy(tapty1->id, (xfmr + i)->htapty);
				//	ph = tapty_tb->FindPh(*tapty1);

				//	(xfmr + i)->htap = ph;
				//	if ((xfmr + i)->htap == -1)
				//	{
				//		sprintf(str, "变压器(%s)高压侧抽头类型(%s)有误!", (xfmr + i)->id, (xfmr + i)->htapty);
				//		return NMD_FAIL;
				//	}
				//}
				//if (((xfmr + i)->ltapty[0] != '\0') && ((xfmr + i)->ltapty[0] != '0'))//填了抽头类型，说明有抽头
				//{
				//	strcpy(tapty1->id, (xfmr + i)->ltapty);
				//	ph = tapty_tb->FindPh(*tapty1);

				//	(xfmr + i)->ltap = ph;
				//	if ((xfmr + i)->ltap == -1)
				//	{
				//		sprintf(str, "变压器(%s)低压侧抽头类型(%s)有误!", (xfmr + i)->id, (xfmr + i)->ltapty);
				//		//lld.addLogLine(str);
				//		return NMD_FAIL;
				//	}
				//}

				//2绕组变压器只需要用一个绕组来等值即可
				//insert xf and r_xfmr_xf for two-winding transformer
				//(xfmr + i)->r_xf = r_xfmr_xf_tb->GetNext();
				strcpy(xf_id, (xfmr + i)->id);
				strcat(xf_id, "t");
				*xf = XF(xf_id);
				strcpy(xf->descr, (xfmr + i)->descr);
				strcpy(xf->nd, (xfmr + i)->hnd);
				strcpy(xf->znd, (xfmr + i)->lnd);
				xf->ind = hnd;
				xf->iznd = lnd;
				xf->r1 = (xfmr + i)->rh;//高压侧的r
				xf->x1 = (xfmr + i)->xh;//高压侧的x
				xf->kvnom = (xfmr + i)->kvnomh;//高压侧电压等级（额定电压）
				xf->zkvnom = (xfmr + i)->kvnoml;

				if (((xfmr + i)->htapty[0] != '\0') && ((xfmr + i)->htapty[0] != '0'))//填了抽头类型，说明有抽头
				{
					strcpy(tapty1->id, (xfmr + i)->htapty);
					ph = tapty_tb->FindPh(*tapty1);
					xf->itapty = ph;//高压侧抽头类型号
					xf->tap = (xfmr + i)->htap;//高压侧抽头量测号
				}
				if (((xfmr + i)->ltapty[0] != '\0') && ((xfmr + i)->ltapty[0] != '0'))//填了抽头类型，说明有抽头
				{
					strcpy(tapty1->id, (xfmr + i)->ltapty);
					ph = tapty_tb->FindPh(*tapty1);
					xf->iztapty = ph;//低压侧抽头类型号
					xf->ztap = (xfmr + i)->ltap;//低压侧抽头量测号
				}
				//建立节点和绕组之间的关系
				strcpy(xf->fdid, (xfmr + i)->fdid);
				ph = xf_tb_t->GetNext();//获得绕组的下一个物理号，即将插入的那个号
				if (ND_BR_Insert(_XF, xf->ind, xf->nd, ph, xf->id, (xfmr + i)->fdid, (xf + i)->ver) == NMD_FAIL ||
					ND_BR_Insert(_XF, xf->iznd, xf->znd, ph, xf->id, (xfmr + i)->fdid, (xf + i)->ver) == NMD_FAIL)
				{
					//printf("fail to insert r_nd_xf for xf (%d)\n", i);
					//sprintf(str, "线段(%s)的节点和线段关系建立失败!", (xf + i)->id);
					//lld.addLogLine(str);
					continue;
				}

				//建立绕组和变压器之间的关系//子属于父的关系
				(xf + i)->r_xfmr = r_xfmr_xf_tb_t->GetNext();//准备插入到关系表中的物理位置
				if (ND_BR_Insert(_XFMR, i, (xfmr+i)->id, ph, xf_id, "", "") == NMD_FAIL)
				{
					//printf("fail to insert r_xfmr_xf table\n");
					//sprintf(str, "变压器和绕组关系插入失败!");
					//lld.addLogLine(str);
					continue;
				}
				if ((ph = xf_tb_t->Insert(*xf)) == -1)
				{
					printf("fail to insert xf table\n");
					sprintf(str, "绕组(%s)中插入失败!", (xf + i)->id);
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
			}
			else if ((xfmr + i)->type == 3) //check three-winding transformer
			{
				//check nds of xfmr
				strcpy(nd1->id, (xfmr + i)->hnd);
				ph = nd_tb->FindPh(*nd1);
				hnd =ph;
				if (hnd == -1)
				{
					sprintf(str, "错误：************变压器(%s)高压侧节点(%s)无连接元件!", (xfmr + i)->id, (xfmr + i)->hnd);
					//lld.addLogLine(str);
					//return NMD_FAIL;
				}
				strcpy(nd1->id, (xfmr + i)->mnd);
				ph = nd_tb->FindPh(*nd1);
				mnd = ph;
				if (mnd == -1)
				{
					//lld add 2010.1.13 for null nd in xfmr
					nd = new ND((xfmr + i)->mnd);
					int phpos = nd_tb->Insert(*nd);
					if (phpos == -1)
					{
						printf("fail to insert nd table\n");
						sprintf(str, "节点表中插入变压器末端悬空记录%s失败!", (xfmr + i)->mnd);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
					delete nd;
					nd = 0;
					//lld add end 2010.1.13 for null nd in xfmr
					sprintf(str, "变压器(%s)中压侧节点(%s)无连接元件!", (xfmr + i)->id, (xfmr + i)->mnd);
					//lld.addLogLine(str);
					//return NMD_FAIL;
				}
				strcpy(nd1->id, (xfmr + i)->lnd);
				ph = nd_tb->FindPh(*nd1);
				lnd = ph;
				if (lnd == -1)
				{
					//lld add 2010.1.13 for null nd in xfmr
					nd = new ND((xfmr + i)->lnd);
					int phpos = nd_tb->Insert(*nd);
					if (phpos == -1)
					{
						printf("fail to insert nd table\n");
						sprintf(str, "节点表中插入变压器末端悬空记录%s失败!", (xfmr + i)->lnd);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
					delete nd;
					nd = 0;
					//lld add end 2010.1.13 for null nd in xfmr
					sprintf(str, "变压器(%s)低压侧节点(%s)无连接元件!", (xfmr + i)->id, (xfmr + i)->lnd);
					//lld.addLogLine(str);
					//return NMD_FAIL;
				}

				//insert the center nd of this three-winding xfmr
				strcpy(nd_id, (xfmr + i)->id);
				strcat(nd_id, "c");//上绕组要在变压器ID的后面加c,2绕组的加t
				nd = new ND(nd_id);
				cnd = nd_tb->Insert(*nd);//补一个中间节点，节点名称就是变压器的ID后面加c
				if (cnd == -1)
				{
					return NMD_FAIL;
				}
				delete nd;
				nd = 0;

				//check tap type
				if (((xfmr + i)->htapty[0] != '\0') && ((xfmr + i)->htapty[0] != '0'))//填了抽头类型，说明有抽头
	//				if((xfmr+i)->htapty[0] != '\0')
				{
					strcpy(tapty1->id, (xfmr + i)->htapty);
					ph = tapty_tb->FindPh(*tapty1);

					(xfmr + i)->htap = ph;
					if ((xfmr + i)->htap == -1)
					{
						sprintf(str, "变压器(%s)高压侧抽头类型(%s)有误!", (xfmr + i)->id, (xfmr + i)->htapty);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
				}
				if (((xfmr + i)->mtapty[0] != '\0') && ((xfmr + i)->mtapty[0] != '0'))//填了抽头类型，说明有抽头
	//				if((xfmr+i)->mtapty[0] != '\0')
				{
					strcpy(tapty1->id, (xfmr + i)->mtapty);
					ph = tapty_tb->FindPh(*tapty1);

					(xfmr + i)->mtap = ph;
					if ((xfmr + i)->mtap == -1)
					{
						sprintf(str, "变压器(%s)中压侧抽头类型(%s)有误!", (xfmr + i)->id, (xfmr + i)->mtapty);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
				}
				//				if((xfmr+i)->ltapty[0] != '\0')
				if (((xfmr + i)->ltapty[0] != '\0') && ((xfmr + i)->ltapty[0] != '0'))//填了抽头类型，说明有抽头
				{
					strcpy(tapty1->id, (xfmr + i)->ltapty);
					ph = tapty_tb->FindPh(*tapty1);

					(xfmr + i)->ltap = ph;
					if ((xfmr + i)->ltap == -1)
					{
						sprintf(str, "变压器(%s)低压侧抽头类型(%s)有误!", (xfmr + i)->id, (xfmr + i)->ltapty);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
				}

				//insert primary side xf for three-winding xfmr
				(xfmr + i)->r_xf = r_xfmr_xf_tb->GetNext();
				strcpy(xf_id, (xfmr + i)->id);
				strcat(xf_id, "h");//变压器ID后面加h代表该变压器高压侧绕组的ID
				*xf = XF(xf_id);
				strcpy(xf->descr, (xfmr + i)->descr);
				strcat(xf->descr, "高压侧");
				strcpy(xf->nd, (xfmr + i)->hnd);//变压器高压侧的节点编号在变压器校验的时候已经加上整形的ID号了
				strcpy(xf->znd, nd_id);//中间节点的ID
				xf->ind = hnd;
				xf->iznd = cnd;
				xf->r1 = (xfmr + i)->rh;
				xf->x1 = (xfmr + i)->xh;
				xf->kvnom = (xfmr + i)->kvnomh;
				xf->zkvnom = 1.0;
				xf->itapty = (xfmr + i)->htap;
				xf->r_xfmr = r_xfmr_xf_tb->GetNext();

				*r_xfmr_xf = R_XFMR_XF((xfmr + i)->id, xf_id);
				r_xfmr_xf->xfmr_id_ptr = xfmr_tb->GetPhByLog(i);
				r_xfmr_xf->xf_id_ptr = xf_tb->GetNext();

				if (xf_tb->Insert(*xf) == -1)
				{
					printf("fail to insert xf table\n");
					sprintf(str, "绕组(%s)插入失败!", (xf + i)->id);
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
				prevph = r_xfmr_xf_tb->Insert(*r_xfmr_xf);
				if (prevph == -1)
				{
					printf("fail to insert r_xfmr_xf table\n");
					sprintf(str, "变压器和绕组关系插入失败!");
					//lld.addLogLine(str);
					return NMD_FAIL;
				}

				//insert secondary side xf for three-winding xfmr
				strcpy(xf_id, (xfmr + i)->id);
				strcat(xf_id, "m");
				*xf = XF(xf_id);
				strcpy(xf->descr, (xfmr + i)->descr);
				strcat(xf->descr, "中压侧");
				strcpy(xf->nd, (xfmr + i)->mnd);
				strcpy(xf->znd, nd_id);
				xf->ind = mnd;
				xf->iznd = cnd;
				xf->r1 = (xfmr + i)->rm;
				xf->x1 = (xfmr + i)->xm;
				xf->kvnom = (xfmr + i)->kvnomm;
				xf->zkvnom = 1.0;
				xf->itapty = (xfmr + i)->mtap;
				xf->r_xfmr = r_xfmr_xf_tb->GetNext();

				*r_xfmr_xf = R_XFMR_XF((xfmr + i)->id, xf_id);
				r_xfmr_xf->xfmr_id_ptr = xfmr_tb->GetPhByLog(i);
				r_xfmr_xf->xf_id_ptr = xf_tb->GetNext();
				r_xfmr_xf_tb->GetDataByPh(prevph)->fptr = r_xfmr_xf_tb->GetNext();
				r_xfmr_xf->bptr = prevph;

				if (xf_tb->Insert(*xf) == -1)
				{
					printf("fail to insert xf table\n");
					sprintf(str, "绕组(%s)插入失败!", (xf + i)->id);
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
				prevph = r_xfmr_xf_tb->Insert(*r_xfmr_xf);
				if (prevph == -1)
				{
					printf("fail to insert r_xfmr_xf table\n");
					sprintf(str, "变压器和绕组关系插入失败!");
					//lld.addLogLine(str);
					return NMD_FAIL;
				}

				//insert third side xf for three-winding xfmr
				strcpy(xf_id, (xfmr + i)->id);
				strcat(xf_id, "l");
				*xf = XF(xf_id);
				strcpy(xf->descr, (xfmr + i)->descr);
				strcat(xf->descr, "低压侧");
				strcpy(xf->nd, (xfmr + i)->lnd);
				strcpy(xf->znd, nd_id);
				xf->ind = lnd;
				xf->iznd = cnd;
				xf->r1 = (xfmr + i)->rl;
				xf->x1 = (xfmr + i)->xl;
				xf->kvnom = (xfmr + i)->kvnoml;
				xf->zkvnom = 1.0;
				xf->itapty = (xfmr + i)->ltap;
				xf->r_xfmr = r_xfmr_xf_tb->GetNext();

				*r_xfmr_xf = R_XFMR_XF((xfmr + i)->id, xf_id);
				r_xfmr_xf->xfmr_id_ptr = xfmr_tb->GetPhByLog(i);
				r_xfmr_xf->xf_id_ptr = xf_tb->GetNext();
				r_xfmr_xf_tb->GetDataByPh(prevph)->fptr = r_xfmr_xf_tb->GetNext();
				r_xfmr_xf->bptr = prevph;

				if (xf_tb->Insert(*xf) == -1)
				{
					printf("fail to insert xf table\n");
					sprintf(str, "绕组(%s)插入记录失败!", (xf + i)->id);
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
				prevph = r_xfmr_xf_tb->Insert(*r_xfmr_xf);
				if (prevph == -1)
				{
					printf("fail to insert r_xfmr_xf table\n");
					sprintf(str, "变压器和绕组关系插入记录失败!");
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
			}
			else
			{
				printf("the type of xfmr (%d) is illegal\n", i);
				sprintf(str, "变压器(%s)类型非法!", (xfmr + i)->id);
				//lld.addLogLine(str);
				//				global->dberrnum++;
				continue;
			}

			//			//mark the station having a xfmr
			//			st->qcomp = st->qcomp | ST_XFMR;
			//			st_tb->UpdateDataByPh(st, ist);
		}
	}

	//update xfmr table
	xfmr_tb->UpdateTable(xfmr);

	delete[] xfmr;
	delete st;
	delete xf;
	delete r_xfmr_xf;
	xfmr = 0;
	st = 0;
	xf = 0;
	r_xfmr_xf = 0;
	lv_xfmr = 0;
	delete nd1;
	delete tapty1;

	//printf("xfmr validation passed\n");
	//	CString str;
	//	str.Format("变压器表校验通过!");
	//	lld.addLogLine(str);
	return NMD_SUCCEED;
}


////////////////////////////////////////////////////////////////////////////////
//  XF_Validate()
int NMD::XF_Validate()
{
	if (lld->mx_xf == 0)
		return 1;

	char	str[500];
	char xfmr_id[128], tapty_id[128];
	int ph;
	int	i;
	//ND* nd1 = new ND();
	//TAPTY* tapty1 = new TAPTY();
	//XFMR* xfmr_t = new XFMR(); //临时，准备做搜索用

	//read xf table into buffer
	lv_xf = xf_tb->GetCount();
	if (lv_xf == 0)
	{
		//printf("there is no record in xf table\n");
//		CString str;
//		str.Format("绕组表中无记录!");
//		lld.addLogLine(str);
		return NMD_SUCCEED;
	}

	xf = new XF[lv_xf];
	xf_tb->CopyTable(xf);

	ND* ndt = new ND();

	//check xf one by one
	for (i = 0; i < lv_xf; i++)
	{
		//直接做相关的节点分析
		strcpy(ndt->id, (xf + i)->nd);
		(xf + i)->ind = nd_tb->FindPh(*ndt);
		strcpy(ndt->id, (xf + i)->znd);
		(xf + i)->iznd = nd_tb->FindPh(*ndt);

		if (ND_BR_Insert(_XF, (xf + i)->ind, (xf + i)->nd, i, (xf + i)->id, (xf + i)->fdid, (xf + i)->ver) == NMD_FAIL ||
			ND_BR_Insert(_XF, (xf + i)->iznd, (xf + i)->znd, i, (xf + i)->id, (xf + i)->fdid, (xf + i)->ver) == NMD_FAIL)
		{
			return NMD_FAIL;
		}




//lld edit 2020-08-03
		////1.找到对应的变压器
		//strcpy(xfmr_id, (xf + i)->fdid);//得到绕组所属的变压器id
		//strcpy(xfmr_t->id, xfmr_id);
		//ph = xfmr_tb->FindPh(*xfmr_t);
		//strcpy(tapty_id , (xf + i)->tapty);

		//if (ph != -1)//只有找到变压器的绕组才有意义
		//{
		//	//2.处理绕组的抽头
		//	ph = xfmr_tb->FindPh(*xfmr_t);
		//	strcpy(nd1->id, (xf + i)->nd);
		//	(xf + i)->ind = nd_tb->FindPh(*nd1);
		//	strcpy(nd1->id, (xf + i)->znd);
		//	(xf + i)->iznd = nd_tb->FindPh(*nd1);

		//	//填写变压器的部分参数
		//	if (strcmp((xf + i)->ver, "高") == 0)//高压侧绕组
		//	{
		//		strcpy(xfmr_t->hnd, (xf + i)->nd);

		//		strcpy(tapty1->id, tapty_id);
		//		(xf + i)->itapty = xfmr_t->htap = tapty_tb->FindPh(*tapty1);
		//		xfmr_t->kvnomh = (xf + i)->basevoltage;

		//		xfmr_t->rh = (xf + i)->r;
		//		xfmr_t->xh = (xf + i)->x;
		//		xfmr_t->r0h = (xf + i)->mvanom;
		//		xfmr_t->x0h = (xf + i)->r_;
		//	}
		//	else if (strcmp((xf + i)->ver, "低") == 0)//低压侧绕组
		//	{
		//		strcpy(xfmr_t->lnd, (xf + i)->nd);
		//		strcpy(tapty1->id, tapty_id);
		//		(xf + i)->iztapty = xfmr_t->ltap = tapty_tb->FindPh(*tapty1);
		//		xfmr_t->kvnoml = (xf + i)->basevoltage;

		//		xfmr_t->rl = (xf + i)->r;
		//		xfmr_t->xl = (xf + i)->x;
		//		xfmr_t->r0l = (xf + i)->mvanom;
		//		xfmr_t->x0l = (xf + i)->r_;
		//	}
		//	else if (strcmp((xf + i)->ver, "中") == 0)//低压侧绕组
		//	{
		//		strcpy(xfmr_t->mnd, (xf + i)->nd);
		//		strcpy(tapty1->id, tapty_id);
		//		(xf + i)->iztapty = xfmr_t->mtap = tapty_tb->FindPh(*tapty1);
		//		xfmr_t->kvnomm = (xf + i)->basevoltage;

		//		xfmr_t->rm = (xf + i)->r;
		//		xfmr_t->xm = (xf + i)->x;
		//		xfmr_t->r0m = (xf + i)->mvanom;
		//		xfmr_t->x0m = (xf + i)->r_;
		//	}
		//	xfmr_tb->UpdateDataByPh(xfmr_t, ph);

		//	//建立绕组和变压器之间的关系
		////子属于父的关系
		//	(xf + i)->r_xfmr = r_xfmr_xf_tb_t->GetNext();//准备插入到关系表中的物理位置
		//	if (ND_BR_Insert(_XFMR, ph, xfmr_id, i, (xf + i)->id, (xf + i)->fdid, (xf + i)->ver) == NMD_FAIL)
		//	{
		//		//printf("fail to insert r_xfmr_xf table\n");
		//		//sprintf(str, "变压器和绕组关系插入失败!");
		//		//lld.addLogLine(str);
		//		continue;
		//	}

		//	//建立节点和绕组之间的关系
		//	if (ND_BR_Insert(_XF, (xf + i)->ind, (xf + i)->nd, i, (xf + i)->id, (xf + i)->fdid, (xf + i)->ver) == NMD_FAIL ||
		//		ND_BR_Insert(_XF, (xf + i)->iznd, (xf + i)->znd, i, (xf + i)->id, (xf + i)->fdid, (xf + i)->ver) == NMD_FAIL)
		//	{
		//		//printf("fail to insert r_nd_xf for xf (%d)\n", i);
		//		//sprintf(str, "线段(%s)的节点和线段关系建立失败!", (xf + i)->id);
		//		//lld.addLogLine(str);
		//		continue;
		//	}
		//}
	}

	//update xf table
	xf_tb->UpdateTable(xf);

	delete[] xf;
	xf = 0;
	lv_xf = 0;
	delete ndt;//lld add 2020-8-3

	//delete xfmr_t;

	//delete nd1;
	//delete tapty1;
	//	printf("xf validation passed\n");
//	CString str;
//	str.Format("绕组表校验通过!");
//	lld.addLogLine(str);
	return NMD_SUCCEED;
}

int NMD::SO_Validate()
{
	if (lld->mx_so==0)
		return 1;
	char str[500],fdid[80];
	int	 i,ph;

	//read so table into buffer
	lv_so = so_tb->GetCount();
	
	so = new SO[lv_so];
	so_tb->CopyTable(so);

	//check so one by one
	for(i = 0; i < lv_so; i++)
	{
		ND* nd1 = new ND();
		strcpy(nd1->id , (so+i)->nd);
		ph = nd_tb->FindPh(*nd1);
		if (ph == -1)
		{
			sprintf(str, "电源%s节点号不正确。",(so+i)->id);
			lld->WriteLog(lld->logfile, str);
			continue;
		}
		nd1->q |= ND_SO;
		nd_tb->UpdateDataByPh(nd1,ph);
		delete nd1;	

		(so+i)->ind = ph;

//		if(ND_Insert((so+i)->nd,&(so+i)->ind,ND_SO,(so+i)->fdid,(so+i)->ver) != NMD_SUCCEED)
//		{
//			sprintf(str,"电源(%s)的电源插入失败!",(so+ i)->id);
//			lld->addLogLine(str);
//			continue;
//		}
				
		if (lld->MATH_VOLTAGELEVEL == 10)
		{
			strcpy(fdid, (so + i)->fdid);//电源所属的馈线
			//增加馈线-电源关系表
			ph = dsbob->OnGetPhByfdid(fdid);//fd_tb->FindPh(FD(fdid));//在馈线表中的位置

			if (ND_BR_Insert(_SO, ph, fdid, i, (so + i)->id, "", "") == NMD_FAIL)
			{
				//sprintf(str, "电源(%s)的电源和馈线关系建立失败!", (so + i)->id);
				//lld->WriteLog(lld->logfile, str);
				return NMD_FAIL;
			}
		}
	}

	//update so table
	so_tb->UpdateTable(so);

	delete[] so;
	so = 0;
	lv_so = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  CP_Validate()，不需要做
int NMD::CP_Validate()
{
	int		i,ph/* , ist*/;
	char    str[500];

	if ( (lld->mx_cp==0)
		||( (strlen(cp_tb->GetDataByLog(0)->nd)==0) && (strlen(cp_tb->GetDataByLog(0)->znd) == 0))
		)
	{
		//sprintf(str,"网络中无cp!");
		//lld->WriteLog(lld->logfile,str);

		return 1;
	}

	//read cp table into buffer
	//cp_tb = dsbob->cp_tb->GetTable();
	lv_cp = cp_tb->GetCount();
	if(lv_cp == 0)
	{
		return NMD_SUCCEED;
	}
	
	cp = new CP[lv_cp];
	cp_tb->CopyTable(cp);
	//check cp one by one
	for(i = 0; i < lv_cp; i++)
	{
		ND* nd1 = new ND();
		strcpy(nd1->id , (cp+i)->nd);
		ph = nd_tb->FindPh(*nd1);
		nd1->q |= ND_CP;
		nd_tb->UpdateDataByPh(nd1,ph);
		(cp + i)->ind = ph;

		if (strlen((cp + i)->znd) > 0)
		{
			strcpy(nd1->id, (cp + i)->znd);
			ph = nd_tb->FindPh(*nd1);
			nd1->q |= ND_CP;
			nd_tb->UpdateDataByPh(nd1, ph);
			(cp + i)->iznd = ph;

			if (ND_BR_Insert(_CP, (cp + i)->ind, (cp + i)->nd, i, (cp + i)->id, (cp + i)->fdid, (cp + i)->ver) == NMD_FAIL ||
				ND_BR_Insert(_CP, (cp + i)->iznd, (cp + i)->znd, i, (cp + i)->id, (cp + i)->fdid, (cp + i)->ver) == NMD_FAIL)
			{
				//printf("fail to insert r_nd_cp for cp (%d)\n", i);
				//sprintf(str, "电容器(%s)的节点和电容器关系建立失败!", (cp + i)->id);
				//lld->WriteLog(lld->logfile, str);
				return NMD_FAIL;
			}
		}
		delete nd1;

	}

	//update cp table
	cp_tb->UpdateTable(cp);

	delete[] cp;
	cp = 0;
	lv_cp = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  ND_Insert(),插入到临时表， lld edit 2018-4-25
int NMD::ND_Insert(const char* nd_id, int* nd_p, int mask, const char* fdid, const char* ver)
{
	if ( (strlen(nd_id)==0) || (strcmp(nd_id,"-")==0) )
		return NMD_SUCCEED;

	char	str[500];
	int		phpos/*, len*/;
	ND* ndt = new ND();
	//看节点表里面是不是已经有了这个节点
	strcpy(ndt->id, nd_id);
	phpos = nd_tb->FindPh(*ndt);
	if( phpos != -1 )  //an old nd
	{
		nd = new ND;
		nd_tb->CopyDataByPh(nd, phpos);
		strcpy(nd->fdid,fdid);
		strcpy(nd->ver,ver);

		if( mask != -1 )//是不是需要修改节点的q值，如果需要的话，改变q值
		{
			nd->q = nd->q | mask;
		}

		nd_tb->UpdateDataByPh(nd, phpos);

		delete nd;
		nd = 0;

		if( mask != -1 )//是不是需要修改节点的q值，如果需要的话，改变q值
		{
			nd = new ND;
			nd_tb->CopyDataByPh(nd, phpos);
			nd->q = nd->q | mask;
			nd_tb->UpdateDataByPh(nd, phpos);
			delete nd;
			nd = 0;
		}
		*nd_p = phpos;//把物理位置返回回去，以便下一个节点插入
	}
	else  // a new nd
	{
		//insert the new nd
		nd = new ND(nd_id);
		if( mask != -1 )
		{
			nd->q =  nd->q | mask;
		}
//		strcpy(nd->fdid,fdid);
//		strcpy(nd->ver,ver);
//		len = strlen(nd_id);//节点是接地刀闸
//		if(nd_id[len-2] == 'G' && nd_id[len-1] == 'n')
//		{
//			nd->q = nd->q | ND_GRND;
//		}
		phpos = nd_tb->Insert(*nd);
		if( phpos == -1 )
		{
			sprintf(str,"节点表中插入记录失败!");
			lld->WriteLog(lld->logfile,str);
			return NMD_FAIL;
		}
		*nd_p = phpos;
		delete nd;
		delete ndt;
		nd = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  ND_BR_Insert()
//根据节点的ID号和设备的ID号，在关系表中插入2条记录
//节点号，设备号，节点指针（在节点表中的位置，角标，物理号），设备指针，前向链（本表），后向链（本表）
//假设设备有两个节点，先做第一个节点
//
//
//在做第二个节点
//
//
//  This function inserts r_nd_br record. 
int NMD::ND_BR_Insert(int type, int nd_p, const char* nd_id, int ibr, const char* br_id, const char* fdid, const char* ver)
{///////////////////////////////////////////////////////////////////////////////
	if ( (nd_p == -1) || (strlen(nd_id)==0) || (strcmp(nd_id,"-")==0)|| (strcmp(nd_id, "-1") == 0))
		return NMD_SUCCEED;

	int		phpos, fptr;

	switch( type )
	{
	case _CB:  //cb
		r_nd_cb = new R_ND_CB;
		*r_nd_cb = R_ND_CB(nd_id, br_id);//构造节点和支路的关系
		r_nd_cb->nd_id_ptr = nd_p;//节点的指针,在节点表中的位置，角标
		r_nd_cb->cb_id_ptr = cb_tb->GetPhByLog(ibr);//开关的指针，在开关表中的位置
		phpos = nd_tb->GetDataByPh(nd_p)->r_cb;//得到节点表中和开关的关联信息

		//strcpy(r_nd_cb_t->fdid , fdid);//节点所属馈线
		//strcpy(r_nd_cb_t->ver , ver);//节点所属馈线

		if(phpos == -1)  //first connected cb of this nd节点没有连接开关
		{
			//节点是父，开关是子。
			//节点连的第一个开关指针（子），通过这个指针可以得到所有的这个节点连的开关
			nd_tb->GetDataByPh(nd_p)->r_cb = r_nd_cb_tb_t->GetNext();
		}
		else  //build cb chain for this nd，如果节点以前已经连接了开关
		{
			//将节点开关关系表中的位置向后挪动，准备新的节点关系指针插入
			fptr = r_nd_cb_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_nd_cb_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_cb_tb_t->GetDataByPh(phpos)->fptr = r_nd_cb_tb_t->GetNext();
			r_nd_cb->bptr = phpos;
		}
		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_nd_cb->id = r_nd_cb_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_nd_cb_tb_t->Append(*r_nd_cb);
		if(phpos == -1)//判断是否越界
		{
			return NMD_FAIL;
		}
		delete r_nd_cb;
		r_nd_cb = 0;
		break;
	case _XF:  //xf
		r_nd_xf = new R_ND_XF;
		*r_nd_xf = R_ND_XF(nd_id, br_id);
		r_nd_xf->nd_id_ptr = nd_p;//连接本线段的节点在节点表中的物理位置，注意：关系在建设中
		r_nd_xf->xf_id_ptr = ibr;//绕组表比较特殊，先做关系表，再插入数据 lld 2019-9-17 xf_tb_t->GetPhByLog(ibr);//线段表的物理位置

		strcpy(r_nd_xf->fdid, fdid);//节点所属馈线
		strcpy(r_nd_xf->ver, ver);//节点所属馈线

		phpos = nd_tb->GetDataByPh(nd_p)->r_xf;
		if (phpos == -1)  //first connected xf of this nd，节点表中还没有这条线的联络记录，只能有一个，就是第一次位置
		{
			nd_tb->GetDataByPh(nd_p)->r_xf = r_nd_xf_tb_t->GetNext();//准备插入到关系表中的物理位置
		}
		else  //build xf chain for this nd，做链表，将这个节点所连的所有线段的全部物理位置管理起来
		{
			//后向物理位置，找到最后的那个物理位置
			fptr = r_nd_xf_tb_t->GetDataByPh(phpos)->fptr;
			while (fptr != -1)
			{
				phpos = fptr;
				fptr = r_nd_xf_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_xf_tb_t->GetDataByPh(phpos)->fptr = r_nd_xf_tb_t->GetNext();//将本次的指针给向最后的那个物理位置
			r_nd_xf->bptr = phpos;//前一个给本次，作为本次的上一个数据
		}

		r_nd_xf->id = r_nd_xf_tb_t->GetNext();//用物理位置做id号，保证每次都插入到最后
		phpos = r_nd_xf_tb_t->Append(*r_nd_xf);
		if (phpos == -1)
		{
			return NMD_FAIL;
		}
		delete r_nd_xf;
		r_nd_xf = 0;
		break;

	case _XFMR:  //xf
		r_xfmr_xf = new R_XFMR_XF;
		*r_xfmr_xf = R_XFMR_XF(nd_id, br_id);
		r_xfmr_xf->xfmr_id_ptr = nd_p;//变压器的物理号
		r_xfmr_xf->xf_id_ptr = xf_tb_t->GetPhByLog(ibr);
		//strcpy(r_xfmr_xf->fdid , fdid);//节点所属馈线
		//strcpy(r_xfmr_xf->ver , ver);//节点所属馈线
		phpos = xfmr_tb->GetDataByPh(nd_p)->r_xf;

		if(phpos == -1)  //first connected xf of this nd
		{
			xfmr_tb->GetDataByPh(nd_p)->r_xf = r_xfmr_xf_tb_t->GetNext();
		}
		else  //build xf chain for this nd
		{
			fptr = r_xfmr_xf_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_xfmr_xf_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_xfmr_xf_tb_t->GetDataByPh(phpos)->fptr = r_xfmr_xf_tb_t->GetNext();
			r_xfmr_xf->bptr = phpos;
		}
		r_xfmr_xf->id = r_xfmr_xf_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_xfmr_xf_tb_t->Append(*r_xfmr_xf);
		if(phpos == -1)
		{
			return NMD_FAIL;
		}
		delete r_xfmr_xf;
		r_xfmr_xf = 0;
		break;

	case _SEC:  //sec
		r_nd_sec = new R_ND_SEC;
		*r_nd_sec = R_ND_SEC(nd_id, br_id);
		r_nd_sec->nd_id_ptr = nd_p;//连接本线段的节点在节点表中的物理位置，注意：关系在建设中
		r_nd_sec->sec_id_ptr = sec_tb->GetPhByLog(ibr);//线段表的物理位置

		strcpy(r_nd_sec->fdid , fdid);//节点所属馈线
		strcpy(r_nd_sec->ver , ver);//节点所属馈线

		phpos = nd_tb->GetDataByPh(nd_p)->r_sec;
		if(phpos == -1)  //first connected sec of this nd，节点表中还没有这条线的联络记录，只能有一个，就是第一次位置
		{
			nd_tb->GetDataByPh(nd_p)->r_sec = r_nd_sec_tb_t->GetNext();//准备插入到关系表中的物理位置
		}
		else  //build sec chain for this nd，做链表，将这个节点所连的所有线段的全部物理位置管理起来
		{
			//后向物理位置，找到最后的那个物理位置
			fptr = r_nd_sec_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_nd_sec_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_sec_tb_t->GetDataByPh(phpos)->fptr = r_nd_sec_tb_t->GetNext();//将本次的指针给向最后的那个物理位置
			r_nd_sec->bptr = phpos;//前一个给本次，作为本次的上一个数据
		}
		r_nd_sec->id = r_nd_sec_tb_t->GetNext();//用物理位置做id号，保证每次都插入到最后
		phpos = r_nd_sec_tb_t->Append(*r_nd_sec);
		if(phpos == -1)
		{
			return NMD_FAIL;
		}
		delete r_nd_sec;
		r_nd_sec = 0;
		break;

	case _CP:  //sec
		r_nd_cp = new R_ND_CP;
		*r_nd_cp = R_ND_CP(nd_id, br_id);
		r_nd_cp->nd_id_ptr = nd_p;//连接本线段的节点在节点表中的物理位置，注意：关系在建设中
		r_nd_cp->cp_id_ptr = cp_tb->GetPhByLog(ibr);//线段表的物理位置

		strcpy(r_nd_cp->fdid, fdid);//节点所属馈线
		strcpy(r_nd_cp->ver, ver);//节点所属馈线

		phpos = nd_tb->GetDataByPh(nd_p)->r_cp;
		if (phpos == -1)  //first connected cp of this nd，节点表中还没有这条线的联络记录，只能有一个，就是第一次位置
		{
			nd_tb->GetDataByPh(nd_p)->r_cp = r_nd_cp_tb_t->GetNext();//准备插入到关系表中的物理位置
		}
		else  //build cp chain for this nd，做链表，将这个节点所连的所有线段的全部物理位置管理起来
		{
			//后向物理位置，找到最后的那个物理位置
			fptr = r_nd_cp_tb_t->GetDataByPh(phpos)->fptr;
			while (fptr != -1)
			{
				phpos = fptr;
				fptr = r_nd_cp_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_cp_tb_t->GetDataByPh(phpos)->fptr = r_nd_cp_tb_t->GetNext();//将本次的指针给向最后的那个物理位置
			r_nd_cp->bptr = phpos;//前一个给本次，作为本次的上一个数据
		}
		r_nd_cp->id = r_nd_cp_tb_t->GetNext();//用物理位置做id号，保证每次都插入到最后
		phpos = r_nd_cp_tb_t->Append(*r_nd_cp);
		if (phpos == -1)
		{
			return NMD_FAIL;
		}
		delete r_nd_cp;
		r_nd_cp = 0;
		break;

	case _LD:  //ld
		r_nd_ld = new R_ND_LD;
		*r_nd_ld = R_ND_LD(nd_id, br_id);//构造节点和支路的关系
		r_nd_ld->nd_id_ptr = nd_p;//节点的指针
		r_nd_ld->ld_id_ptr = ld_tb->GetPhByLog(ibr);//开关的指针

		strcpy(r_nd_ld->fdid , fdid);//节点所属馈线
		strcpy(r_nd_ld->ver , ver);//节点所属馈线

		phpos = nd_tb->GetDataByPh(nd_p)->r_ld;//得到节点表中和开关的关联信息
		if(phpos == -1)  //first connected ld of this nd节点没有连接开关
		{
			//节点是父，开关是子。
			//节点连的第一个开关指针（子），通过这个指针可以得到所有的这个节点连的开关
			nd_tb->GetDataByPh(nd_p)->r_ld = r_nd_ld_tb_t->GetNext();
		}
		else  //build ld chain for this nd，如果节点以前已经连接了开关
		{
			//将节点开关关系表中的位置向后挪动，准备新的节点关系指针插入
			fptr = r_nd_ld_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_nd_ld_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_ld_tb_t->GetDataByPh(phpos)->fptr = r_nd_ld_tb_t->GetNext();
			r_nd_ld->bptr = phpos;
		}
		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_nd_ld->id = r_nd_ld_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_nd_ld_tb_t->Append(*r_nd_ld);
		if(phpos == -1)//判断是否越界
		{
			return NMD_FAIL;
		}
		delete r_nd_ld;
		r_nd_ld = 0;
		break;

	case _SO:  //so
		r_fd_so = new R_FD_SO;
		*r_fd_so = R_FD_SO(nd_id, br_id);//构造节点和支路的关系
		r_fd_so->fd_id_ptr = nd_p;//节点的指针
		r_fd_so->so_id_ptr = so_tb->GetPhByLog(ibr);//开关的指针

//		strcpy(r_fd_so->fdid , fdid);//节点所属馈线
		strcpy(r_fd_so->ver , ver);//节点所属馈线

		phpos = fd_tb->GetDataByPh(nd_p)->r_so;//得到节点表中和开关的关联信息
		if(phpos == -1)  //first connected so of this fd节点没有连接开关
		{
			//节点是父，开关是子。
			//节点连的第一个开关指针（子），通过这个指针可以得到所有的这个节点连的开关
			fd_tb->GetDataByPh(nd_p)->r_so = r_fd_so_tb_t->GetNext();
		}
		else  //buiso so chain for this fd，如果节点以前已经连接了开关
		{
			//将节点开关关系表中的位置向后挪动，准备新的节点关系指针插入
			fptr = r_fd_so_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_so_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_so_tb_t->GetDataByPh(phpos)->fptr = r_fd_so_tb_t->GetNext();
			r_fd_so->bptr = phpos;
		}
		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_fd_so->id = r_fd_so_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_fd_so_tb_t->Append(*r_fd_so);
		if(phpos == -1)//判断是否越界
		{
			return NMD_FAIL;
		}
		delete r_fd_so;
		r_fd_so = 0;
		break;

	case _R_ZN_CB:  //zn 区段是父，开关是子
		r_zn_cb = new R_ZN_CB;
		*r_zn_cb = R_ZN_CB(nd_id, br_id);//构造区段和开关的关系
		r_zn_cb->zn_id_ptr = nd_p;//区段的指针，也就是区段的物理位置
		r_zn_cb->cb_id_ptr = cb_tb->GetPhByLog(ibr);//开关的指针

		strcpy(r_zn_cb->fdid , fdid);//节点所属馈线
		strcpy(r_zn_cb->ver , ver);//节点所属馈线
		
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_cb;//得到区段表中和开关的关联信息
		if(phpos == -1)  //first connected cb of this zn区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			zn_tb_t->GetDataByPh(nd_p)->r_cb = r_zn_cb_tb_t->GetNext();
		}
		else  //build cb chain for this zn，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_zn_cb_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_zn_cb_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_cb_tb_t->GetDataByPh(phpos)->fptr = r_zn_cb_tb_t->GetNext();
			r_zn_cb->bptr = phpos;
		}
//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_zn_cb->id = r_zn_cb_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_zn_cb_tb_t->Append(*r_zn_cb);
		if(phpos == -1)//判断是否越界
		{
			delete r_zn_cb;
			r_zn_cb = 0;
			return NMD_FAIL;
		}
		delete r_zn_cb;
		r_zn_cb = 0;
		
		break;

	case _R_ZN_SEC:  //zn 区段是父，开关是子
		r_zn_sec = new R_ZN_SEC;
		*r_zn_sec = R_ZN_SEC(nd_id, br_id);//构造区段和开关的关系
		r_zn_sec->zn_id_ptr = nd_p;//区段的指针，也就是区段的物理位置
		r_zn_sec->sec_id_ptr = sec_tb->GetPhByLog(ibr);//开关的指针

		strcpy(r_zn_sec->fdid , fdid);//节点所属馈线
		strcpy(r_zn_sec->ver , ver);//节点所属馈线
		
		sec_tb->GetDataByPh(ibr)->r_zn = r_zn_sec_tb_t->GetNext();//准备插入到关系表中的物理位置
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_sec;//得到区段表中和开关的关联信息
		if(phpos == -1)  //first connected sec of this zn区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			zn_tb_t->GetDataByPh(nd_p)->r_sec = r_zn_sec_tb_t->GetNext();
		}
		else  //build sec chain for this zn，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_zn_sec_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_zn_sec_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_sec_tb_t->GetDataByPh(phpos)->fptr = r_zn_sec_tb_t->GetNext();
			r_zn_sec->bptr = phpos;
		}
//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_zn_sec->id = r_zn_sec_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_zn_sec_tb_t->Append(*r_zn_sec);
		if(phpos == -1)//判断是否越界
		{
			delete r_zn_sec;
			r_zn_sec = 0;
			return NMD_FAIL;
		}
		delete r_zn_sec;
		r_zn_sec = 0;
		
		break;

	case _R_ZN_CP:  //zn 区段是父，开关是子
		r_zn_cp = new R_ZN_CP;
		*r_zn_cp = R_ZN_CP(nd_id, br_id);//构造区段和开关的关系
		r_zn_cp->zn_id_ptr = nd_p;//区段的指针，也就是区段的物理位置
		r_zn_cp->cp_id_ptr = cp_tb->GetPhByLog(ibr);//开关的指针

		strcpy(r_zn_cp->fdid, fdid);//节点所属馈线
		strcpy(r_zn_cp->ver, ver);//节点所属馈线

		cp_tb->GetDataByPh(ibr)->r_zn = r_zn_cp_tb_t->GetNext();//准备插入到关系表中的物理位置
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_cp;//得到区段表中和开关的关联信息
		if (phpos == -1)  //first connected cp of this zn区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			zn_tb_t->GetDataByPh(nd_p)->r_cp = r_zn_cp_tb_t->GetNext();
		}
		else  //build cp chain for this zn，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_zn_cp_tb_t->GetDataByPh(phpos)->fptr;
			while (fptr != -1)
			{
				phpos = fptr;
				fptr = r_zn_cp_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_cp_tb_t->GetDataByPh(phpos)->fptr = r_zn_cp_tb_t->GetNext();
			r_zn_cp->bptr = phpos;
		}
		//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_zn_cp->id = r_zn_cp_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_zn_cp_tb_t->Append(*r_zn_cp);
		if (phpos == -1)//判断是否越界
		{
			delete r_zn_cp;
			r_zn_cp = 0;
			return NMD_FAIL;
		}
		delete r_zn_cp;
		r_zn_cp = 0;

		break;

	case _R_ZN_XF:  //zn 区段是父，开关是子
		r_zn_xf = new R_ZN_XF;
		*r_zn_xf = R_ZN_XF(nd_id, br_id);//构造区段和开关的关系
		r_zn_xf->zn_id_ptr = nd_p;//区段的指针，也就是区段的物理位置
		r_zn_xf->xf_id_ptr = xf_tb->GetPhByLog(ibr);//开关的指针

		strcpy(r_zn_xf->fdid, fdid);//节点所属馈线
		strcpy(r_zn_xf->ver, ver);//节点所属馈线

		xf_tb->GetDataByPh(ibr)->r_zn = r_zn_xf_tb_t->GetNext();//准备插入到关系表中的物理位置
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_xf;//得到区段表中和开关的关联信息
		if (phpos == -1)  //first connected xf of this zn区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			zn_tb_t->GetDataByPh(nd_p)->r_xf = r_zn_xf_tb_t->GetNext();
		}
		else  //build xf chain for this zn，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_zn_xf_tb_t->GetDataByPh(phpos)->fptr;
			while (fptr != -1)
			{
				phpos = fptr;
				fptr = r_zn_xf_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_xf_tb_t->GetDataByPh(phpos)->fptr = r_zn_xf_tb_t->GetNext();
			r_zn_xf->bptr = phpos;
		}
		//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_zn_xf->id = r_zn_xf_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_zn_xf_tb_t->Append(*r_zn_xf);
		if (phpos == -1)//判断是否越界
		{
			delete r_zn_xf;
			r_zn_xf = 0;
			return NMD_FAIL;
		}
		delete r_zn_xf;
		r_zn_xf = 0;

		break;


	case _R_ZN_LD:  //zn 区段是父，开关是子
		r_zn_ld = new R_ZN_LD;
		*r_zn_ld = R_ZN_LD(nd_id, br_id);//构造区段和开关的关系
		r_zn_ld->zn_id_ptr = nd_p;//区段的指针，也就是区段的物理位置
		r_zn_ld->ld_id_ptr = ld_tb->GetPhByLog(ibr);//开关的指针

		strcpy(r_zn_ld->fdid , fdid);//节点所属馈线
		strcpy(r_zn_ld->ver , ver);//节点所属馈线
		
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_ld;//得到区段表中和开关的关联信息
		if(phpos == -1)  //first connected ld of this zn区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			zn_tb_t->GetDataByPh(nd_p)->r_ld = r_zn_ld_tb_t->GetNext();
		}
		else  //build ld chain for this zn，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_zn_ld_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_zn_ld_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_ld_tb_t->GetDataByPh(phpos)->fptr = r_zn_ld_tb_t->GetNext();
			r_zn_ld->bptr = phpos;
		}
//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		ld_tb->GetDataByPh(ibr)->r_zn = r_zn_ld->id = r_zn_ld_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_zn_ld_tb_t->Append(*r_zn_ld);
		if(phpos == -1)//判断是否越界
		{
			delete r_zn_ld;
			r_zn_ld = 0;
			return NMD_FAIL;
		}
		delete r_zn_ld;
		r_zn_ld = 0;
		
		break;

	case _FDS:  //fd 区段是父，un是子
		r_fd_un = new R_FD_UN;
		*r_fd_un = R_FD_UN(nd_id, br_id);//构造区段和开关的关系
		r_fd_un->fd_id_ptr = nd_p;//区段的指针，也就是区段的物理位置
		r_fd_un->un_id_ptr = un_tb->GetPhByLog(ibr);//开关的指针

		//strcpy(r_fd_un->fdid , fdid);//节点所属馈线
		strcpy(r_fd_un->ver , ver);//节点所属馈线
		
		un_tb->GetDataByPh(ibr)->r_fd = r_fd_un_tb_t->GetNext();//准备插入到关系表中的物理位置
		phpos = fd_tb->GetDataByPh(nd_p)->r_un;//得到区段表中和开关的关联信息
		if(phpos == -1)  //first connected sec of this zn区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			fd_tb->GetDataByPh(nd_p)->r_un = r_fd_un_tb_t->GetNext();
		}
		else  //build sec chain for this zn，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_fd_un_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_un_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_un_tb_t->GetDataByPh(phpos)->fptr = r_fd_un_tb_t->GetNext();
			r_fd_un->bptr = phpos;
		}
//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_fd_un->id = r_fd_un_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_fd_un_tb_t->Append(*r_fd_un);
		if(phpos == -1)//判断是否越界
		{
			delete r_fd_un;
			r_fd_un = 0;
			return NMD_FAIL;
		}
		delete r_fd_un;
		r_fd_un = 0;
		
		break;

	case _R_FD_LD:  //fd 区段是父，un是子
		r_fd_ld = new R_FD_LD;
		*r_fd_ld = R_FD_LD(nd_id, br_id);//构造区段和开关的关系
		r_fd_ld->fd_id_ptr = nd_p;//区段的指针，也就是区段的物理位置
		r_fd_ld->ld_id_ptr = ld_tb->GetPhByLog(ibr);//开关的指针

		//strcpy(r_fd_ld->fdid , fdid);//节点所属馈线
		strcpy(r_fd_ld->ver , ver);//节点所属馈线
		
		ld_tb->GetDataByPh(ibr)->r_fd = r_fd_ld_tb_t->GetNext();//准备插入到关系表中的物理位置
		phpos = fd_tb->GetDataByPh(nd_p)->r_ld;//得到区段表中和开关的关联信息
		if(phpos == -1)  //first connected sec of this zn区段没有连接开关
		{
			//区段是父，开关是子。
			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
			fd_tb->GetDataByPh(nd_p)->r_ld = r_fd_ld_tb_t->GetNext();
		}
		else  //build sec chain for this zn，如果节点以前已经连接了开关
		{
			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
			fptr = r_fd_ld_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_ld_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_ld_tb_t->GetDataByPh(phpos)->fptr = r_fd_ld_tb_t->GetNext();
			r_fd_ld->bptr = phpos;
		}
//		//插入后返回指针链表的物理位置，便于下一个指针的插入
		r_fd_ld->id = r_fd_ld_tb_t->GetNext();//用逻辑位置做id号
		phpos = r_fd_ld_tb_t->Append(*r_fd_ld);
		if(phpos == -1)//判断是否越界
		{
			delete r_fd_ld;
			r_fd_ld = 0;
			return NMD_FAIL;
		}
		delete r_fd_ld;
		r_fd_ld = 0;
		
		break;

//	case _R_FD_ND:  //fd 区段是父，un是子
//		r_fd_nd = new R_FD_ND;
//		*r_fd_nd = R_FD_ND(nd_id, br_id);//构造区段和开关的关系
//		r_fd_nd->fd_id_ptr = nd_p;//区段的指针，也就是区段的物理位置
//		r_fd_nd->nd_id_ptr = nd_tb_t->GetPhByLog(ibr);//开关的指针
//
//		//strcpy(r_fd_nd->fdid , fdid);//节点所属馈线
//		strcpy(r_fd_nd->ver , ver);//节点所属馈线
//		
//		//nd_tb_t->GetDataByPh(ibr)->r_fd = dsbob->r_fd_nd_tb_t->GetNext();//准备插入到关系表中的物理位置
//		phpos = fd_tb_t->GetDataByPh(nd_p)->r_nd;//得到区段表中和开关的关联信息
//		if(phpos == -1)  //first connected sec of this zn区段没有连接开关
//		{
//			//区段是父，开关是子。
//			//区段的第一个开关指针（子），通过这个指针可以得到所有的这个区段连的开关
//			fd_tb_t->GetDataByPh(nd_p)->r_nd = dsbob->r_fd_nd_tb_t->GetNext();
//		}
//		else  //buind sec chain for this zn，如果节点以前已经连接了开关
//		{
//			//将区段-开关关系表中的位置向后挪动，准备新的区段关系指针插入
//			fptr = dsbob->r_fd_nd_tb_t->GetDataByPh(phpos)->fptr;
//			while( fptr != -1 )  
//			{
//				phpos = fptr;
//				fptr = dsbob->r_fd_nd_tb_t->GetDataByPh(phpos)->fptr;
//			}
//			dsbob->r_fd_nd_tb_t->GetDataByPh(phpos)->fptr = dsbob->r_fd_nd_tb_t->GetNext();
//			r_fd_nd->bptr = phpos;
//		}
////		//插入后返回指针链表的物理位置，便于下一个指针的插入
//		r_fd_nd->id = dsbob->r_fd_nd_tb_t->GetNext();//用逻辑位置做id号
//		phpos = dsbob->r_fd_nd_tb_t->Insert(*r_fd_nd);
//		if(phpos == -1)//判断是否越界
//		{
//			delete r_fd_nd;
//			r_fd_nd = 0;
//			return NMD_FAIL;
//		}
//		delete r_fd_nd;
//		r_fd_nd = 0;
//		
//		break;
//	
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  ResetPtr()
//  This function resets all pointer variables in physical component tables.
//  As we build the relation tables by Ids of physical component, so before
//  doing validation, we should remove all records in relation tables and 
//  set the pointer variables in physical component tables to -1.
int NMD::ResetPtr()
{///////////////////////////////////////////////////////////////////////////////
	if (lld->mx_co >0)
		CO_ResetPtr();

	if (lld->mx_st > 0)
		ST_ResetPtr();
	if (lld->mx_xfmr > 0)
		XFMR_ResetPtr();

	if (lld->mx_cb > 0)
		CB_ResetPtr();
//	SO_ResetPtr();
	if (lld->mx_ld > 0)
		LD_ResetPtr();
//	CP_ResetPtr();
	if (lld->mx_bus > 0)
		BUS_ResetPtr();
	//ZN_ResetPtr();
	if (lld->mx_sec > 0)
		SEC_ResetPtr();

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  CO_ResetPtr()
int NMD::CO_ResetPtr()
{
	int		i;

	if (lld->mx_co == 0)
		return 1;

	lv_co = co_tb->GetCount();
	if(lv_co != 0)
	{
		//read table into buffer
		co = new CO[lv_co];//先开一段临时内存表，将所有的记录都开出来
		co_tb->CopyTable(co);//将现在的co_tb的所有记录都拷贝到这个临时内存表里面

		//reset pointers将临时表里面的所有记录的字段r_st（公司与电站的关系）写成-1
		for(i = 0; i < lv_co; i++)
		{
			(co+i)->r_st = -1;
		}

		//update table将临时表里面的记录回写到正式表里面
		co_tb->UpdateTable(co);

		delete[] co;//将临时表内存释放
		co = 0;//保险起见，防止释放后指针乱指
		lv_co = 0;//保险，以后需要用这个的时候会调用方法GetCount()得到的
	}

	return NMD_SUCCEED;
}

//  ST_ResetPtr()
int NMD::ST_ResetPtr()
{
	int		i;
	
	if (lld->mx_st == 0)
		return 1;

	lv_st = st_tb->GetCount();
	if(lv_st != 0)
	{
		//read table into buffer
		st = new ST[lv_st];
		st_tb->CopyTable(st);

		//reset pointers
		for(i = 0; i < lv_st; i++)
		{
			(st+i)->r_co = -1;
			(st+i)->r_xfmr = -1;
			(st+i)->r_so = -1;
			(st+i)->r_cb = -1;
			(st+i)->r_ld = -1;
			(st+i)->r_cp = -1;
			(st+i)->r_bus = -1;
			(st+i)->r_zn = -1;
		}

		//update table
		st_tb->UpdateTable(st);

		delete[] st;
		st = 0;
		lv_st = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  XFMR_ResetPtr()
int NMD::XFMR_ResetPtr()
{
	if (lld->mx_xfmr == 0)
		return 1;

	int		i;

	lv_xfmr = xfmr_tb->GetCount();
	if(lv_xfmr != 0)
	{
		//read table into buffer
		xfmr = new XFMR[lv_xfmr];
		xfmr_tb->CopyTable(xfmr);

		//reset pointers
		for(i = 0; i < lv_xfmr; i++)
		{
			(xfmr+i)->ialgph = -1;
			(xfmr+i)->ialgqh = -1;
			(xfmr+i)->ialgih = -1;
			(xfmr+i)->ialgth = -1;
			(xfmr+i)->ialgpm = -1;
			(xfmr+i)->ialgqm = -1;
			(xfmr+i)->ialgim = -1;
			(xfmr+i)->ialgtm = -1;
			(xfmr+i)->ialgpl = -1;
			(xfmr+i)->ialgql = -1;
			(xfmr+i)->ialgil = -1;
			(xfmr+i)->ialgtl = -1;
			//抽头位置为计算时传入，此时不宜初始化，2019-9-6
			//(xfmr+i)->htap = -1;
			//(xfmr+i)->mtap = -1;
			//(xfmr+i)->ltap = -1;
			(xfmr+i)->r_st = -1;
			(xfmr+i)->r_xf = -1;
		}

		//update table
		xfmr_tb->UpdateTable(xfmr);

		delete[] xfmr;
		xfmr = 0;
		lv_xfmr = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  CB_ResetPtr()
int NMD::CB_ResetPtr()
{
	if (lld->mx_cb==0)
		return 0;

	int		i;

	lv_cb = cb_tb->GetCount();
	if(lv_cb != 0)
	{
		//read table into buffer
		cb = new CB[lv_cb];
		cb_tb->CopyTable(cb);

		//reset pointers
		for(i = 0; i < lv_cb; i++)
		{
			(cb+i)->ind = -1;
			(cb+i)->iznd = -1;
			(cb+i)->idigital = -1;
			(cb+i)->r_st = -1;
		}

		//update table
		cb_tb->UpdateTable(cb);

		delete[] cb;
		cb = 0;
		lv_cb = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  SO_ResetPtr()
int NMD::SO_ResetPtr()
{
	if (lld->mx_so == 0)
		return 1;

	int		i;

	lv_so = so_tb->GetCount();
	if(lv_so != 0)
	{
		//read table into buffer
		so = new SO[lv_so];
		so_tb->CopyTable(so);

		//reset pointers
		for(i = 0; i < lv_so; i++)
		{
			(so+i)->ind = -1;
			(so+i)->ialgp = -1;
			(so+i)->ialgq = -1;
			(so+i)->r_st = -1;
		}

		//update table
		so_tb->UpdateTable(so);

		delete[] so;
		so = 0;
		lv_so = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  LD_ResetPtr()
int NMD::LD_ResetPtr()
{
	if (lld->mx_ld == 0)
		return 1;

	int		i;

	lv_ld = ld_tb->GetCount();
	if(lv_ld != 0)
	{
		//read table into buffer
		ld = new LD[lv_ld];
		ld_tb->CopyTable(ld);

		//reset pointers
		for(i = 0; i < lv_ld; i++)
		{
			(ld+i)->ind = -1;
			(ld+i)->ialgp = -1;
			(ld+i)->ialgq = -1;
			(ld+i)->r_st = -1;
			(ld+i)->r_zn = -1;
		}

		//update table
		ld_tb->UpdateTable(ld);

		delete[] ld;
		ld = 0;
		lv_ld = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  BUS_ResetPtr()
int NMD::BUS_ResetPtr()
{
	int		i;

	if (lld->mx_bus==0)
		return 0;

	lv_bus = bus_tb->GetCount();
	if(lv_bus != 0)
	{
		//read table into buffer
		bus = new BUS[lv_bus];
		bus_tb->CopyTable(bus);

		//reset pointers
		for(i = 0; i < lv_bus; i++)
		{
			(bus+i)->ind = -1;
			(bus+i)->ialgv = -1;
			(bus+i)->r_st = -1;
		}

		//update table
		bus_tb->UpdateTable(bus);

		delete[] bus;
		bus = 0;
		lv_bus = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  ZN_ResetPtr()
int NMD::ZN_ResetPtr()
{
	if (lld->mx_zn == 0)
		return 1;

	int		i;

	lv_zn = zn_tb->GetCount();
	if(lv_zn != 0)
	{
		//read table into buffer
		zn = new ZN[lv_zn];
		zn_tb->CopyTable(zn);

		//reset pointers
		for(i = 0; i < lv_zn; i++)
		{
//			(zn+i)->ind = -1;
//			(zn+i)->iznd = -1;
//			(zn+i)->ialgip = -1;
//			(zn+i)->ialgiq = -1;
//			(zn+i)->ialgii = -1;
//			(zn+i)->ialgzp = -1;
//			(zn+i)->ialgzq = -1;
//			(zn+i)->ialgzi = -1;
			(zn+i)->r_st = -1;
			(zn+i)->r_ld = -1;
			(zn+i)->r_sec = -1;
			(zn+i)->r_cb = -1;
		}

		//update table
		zn_tb->UpdateTable(zn);

		delete[] zn;
		zn = 0;
		lv_zn = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  SEC_ResetPtr()
int NMD::SEC_ResetPtr()
{
	if (lld->mx_sec == 0)
		return 1;

	int		i;

	lv_sec = sec_tb->GetCount();
	if(lv_sec != 0)
	{
		//read table into buffer
		sec = new SEC[lv_sec];
		sec_tb->CopyTable(sec);

		//reset pointers
		for(i = 0; i < lv_sec; i++)
		{
			(sec+i)->ind = -1;
			(sec+i)->iznd = -1;
			(sec+i)->ialgip = -1;
			(sec+i)->ialgiq = -1;
			(sec+i)->ialgii = -1;
			(sec+i)->ialgzp = -1;
			(sec+i)->ialgzq = -1;
			(sec+i)->ialgzi = -1;
			(sec+i)->r_zn = -1;
		}

		//update table
		sec_tb->UpdateTable(sec);

		delete[] sec;
		sec = 0;
		lv_sec = 0;
	}

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  RebuildPtr()
//  This function rebuilds all position pointers between tables. It first  
//  converts the physical position pointers into logic ones, and then  
//  stores the data in each table in sequence order.
//1.经过这一步后，每个表里面的关系值，如r_ld等由原来存储的物理位置变成内逻辑位置
//2.然后将每一个设备的物理位置和逻辑位置统一设成一个统一的由1到总数的顺序排列
int NMD::RebuildPtr()
{//////////////////////////////////////////////////////////////////////////////

	//rebuild position pointers in each table
	CO_RebuildPtr();
    ST_RebuildPtr();
	XFMR_RebuildPtr();
	XF_RebuildPtr();
	CB_RebuildPtr();
	SO_RebuildPtr();
	LD_RebuildPtr();
	if(lld->mx_cp>0)
		CP_RebuildPtr();
	if (lld->mx_bus > 0)
		BUS_RebuildPtr();
	SEC_RebuildPtr();
	ND_RebuildPtr();
	R_CO_ST_RebuildPtr();
	R_ST_XFMR_RebuildPtr();
	R_XFMR_XF_RebuildPtr();
	R_ST_CB_RebuildPtr();
	R_ST_SO_RebuildPtr();
	R_ST_LD_RebuildPtr();
	R_ST_CP_RebuildPtr();
	R_ST_BUS_RebuildPtr();
	//R_ST_ZN_RebuildPtr();

//lld->WriteLog(lld->logfile,"lld---1");
	if (lld->MATH_VOLTAGELEVEL == 10)
	{
		ZN_RebuildPtr();
		R_ZN_LD_RebuildPtr();
		//lld->WriteLog(lld->logfile,"lld---2");
		R_ZN_SEC_RebuildPtr();
		//lld->WriteLog(lld->logfile,"lld---3");
		R_ZN_CB_RebuildPtr();
		//lld->WriteLog(lld->logfile,"lld---4");
		R_ZN_CB_RebuildPtr();
		//lld->WriteLog(lld->logfile,"lld---7");
		R_ZN_CB_RebuildPtr();
		//lld->WriteLog(lld->logfile,"lld---8");
	}
	R_ND_CB_RebuildPtr();
	R_ND_LD_RebuildPtr();
//	R_ND_XF_RebuildPtr();
//	R_ND_ZN_RebuildPtr();
	R_ND_SEC_RebuildPtr();
	R_ND_CP_RebuildPtr();

	//seqence data in each table
//	if (nmd_type == 1)
//	{
//		digital_tb->Sequence();
//		analog_tb->Sequence();
//	}
	if (lld->mx_co > 0)
	{
		co_tb->Sequence();
		dsbob->co_tb = co_tb;
	}
	if (lld->mx_st > 0)
	{
		st_tb->Sequence();
		dsbob->st_tb = st_tb;
	}
	if (lld->mx_xfmr > 0)
	{
		xfmr_tb->Sequence();
		dsbob->xfmr_tb = xfmr_tb;
	}
	if (lld->mx_xf > 0)
	{
		xf_tb->Sequence();
		dsbob->xf_tb = xf_tb;
	}
//	zn_tb->Sequence();
	cb_tb->Sequence();
	dsbob->cb_tb = cb_tb;
	so_tb->Sequence();
	dsbob->so_tb = so_tb;
	ld_tb->Sequence();
	dsbob->ld_tb = ld_tb;
	if (lld->mx_cp > 0)
	{
		cp_tb->Sequence();
		dsbob->cp_tb = cp_tb;
	}
	if (lld->mx_bus > 0)
	{
		bus_tb->Sequence();
		dsbob->bus_tb = bus_tb;
	}
	sec_tb->Sequence();
	dsbob->sec_tb = sec_tb;
	nd_tb->Sequence();
	dsbob->nd_tb = nd_tb;
	//	bs_tb->Sequence();
//	island_tb->Sequence();
//  global_tb->Sequence();
//  tapty_tb->Sequence();
//  r_co_st_tb->Sequence();
//	r_st_xfmr_tb->Sequence();
//	r_xfmr_xf_tb->Sequence();
//	r_st_cb_tb->Sequence();
//	r_st_so_tb->Sequence();
//	r_st_ld_tb->Sequence();
//	r_st_cp_tb->Sequence();
//	r_st_bus_tb->Sequence();
//	r_st_zn_tb->Sequence();
//	r_zn_ld_tb->Sequence();
//	r_zn_sec_tb->Sequence();
//	r_zn_cb_tb->Sequence();
//	r_nd_cb_tb->Sequence();
//	r_nd_ld_tb->Sequence();
//	r_nd_xf_tb->Sequence();
//	r_nd_zn_tb->Sequence();
//	r_nd_sec_tb->Sequence();
//lld->WriteLog(lld->logfile,"lld---3");

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  CO_RebuildPtr()
int NMD::CO_RebuildPtr()
{
	if (lld->mx_co == 0)
		return 1;

// 	int		i;

	//read table into buffer
	lv_co = co_tb->GetCount();
	if(lv_co == 0)
	{
		return NMD_SUCCEED;
	}

/*
	co = new CO[lv_co];
	co_tb->CopyTable(co);
	
	//rebuild pointer
	for(i = 0; i < lv_co; i++)
	{
		if((co+i)->r_st != -1)
		{
			(co+i)->r_st = r_co_st_tb->GetLogByPh((co+i)->r_st);
		}
	}

	//update table
	co_tb->UpdateTable(co);

	delete[] co;
	co = 0;
	lv_co = 0;
*/

	return NMD_SUCCEED;
}

int NMD::ST_RebuildPtr()
{
	if (lld->mx_st == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_st = st_tb->GetCount();
	if(lv_st == 0)
	{
		return NMD_SUCCEED;
	}

	st = new ST[lv_st];
	st_tb->CopyTable(st);
	
	//rebuild pointer
	for(i = 0; i < lv_st; i++)
	{
		if((st+i)->r_co != -1)
		{
			(st+i)->r_co = r_co_st_tb->GetLogByPh((st+i)->r_co);
		}
		if((st+i)->r_xfmr != -1)
		{
			(st+i)->r_xfmr = r_st_xfmr_tb->GetLogByPh((st+i)->r_xfmr);
		}
		if((st+i)->r_so != -1)
		{
			(st+i)->r_so = r_st_so_tb->GetLogByPh((st+i)->r_so);
		}
		if((st+i)->r_cb != -1)
		{
			(st+i)->r_cb = r_st_cb_tb->GetLogByPh((st+i)->r_cb);
		}
		if((st+i)->r_ld != -1)
		{
			(st+i)->r_ld = r_st_ld_tb->GetLogByPh((st+i)->r_ld);
		}
		if((st+i)->r_cp != -1)
		{
			(st+i)->r_cp = r_st_cp_tb->GetLogByPh((st+i)->r_cp);
		}
		if((st+i)->r_bus != -1)
		{
			(st+i)->r_bus = r_st_bus_tb->GetLogByPh((st+i)->r_bus);
		}
		//if((st+i)->r_zn != -1)
		//{
		//	(st+i)->r_zn = r_st_zn_tb->GetLogByPh((st+i)->r_zn);
		//}
	}

	//update table
	st_tb->UpdateTable(st);

	delete[] st;
	st = 0;
	lv_st = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  XFMR_RebuildPtr()
int NMD::XFMR_RebuildPtr()
{
	if (lld->mx_xfmr == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_xfmr = xfmr_tb->GetCount();
	if(lv_xfmr == 0)
	{
		return NMD_SUCCEED;
	}

	xfmr = new XFMR[lv_xfmr];
	xfmr_tb->CopyTable(xfmr);
	
	//rebuild pointer
	for(i = 0; i < lv_xfmr; i++)
	{
//		if((xfmr+i)->r_st != -1)
//		{
//			(xfmr+i)->r_st = r_st_xfmr_tb->GetLogByPh((xfmr+i)->r_st);
//		}
		if((xfmr+i)->r_xf != -1)
		{
			(xfmr+i)->r_xf = r_xfmr_xf_tb->GetLogByPh((xfmr+i)->r_xf);
		}
	}

	//update table
	xfmr_tb->UpdateTable(xfmr);

	delete[] xfmr;
	xfmr = 0;
	lv_xfmr = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  XF_RebuildPtr()
int NMD::XF_RebuildPtr()
{
	if (lld->mx_xf == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_xf = xf_tb->GetCount();
	if(lv_xf == 0)
	{
		return NMD_SUCCEED;
	}

	xf = new XF[lv_xf];
	xf_tb->CopyTable(xf);
	
	//rebuild pointer
	for(i = 0; i < lv_xf; i++)
	{
		if((xf+i)->r_xfmr != -1)
		{
			(xf+i)->r_xfmr = r_xfmr_xf_tb->GetLogByPh((xf+i)->r_xfmr);
		}
		if((xf+i)->ind != -1)
		{
			(xf+i)->ind = nd_tb->GetLogByPh((xf+i)->ind);
		}
		if((xf+i)->iznd != -1)
		{
			(xf+i)->iznd = nd_tb->GetLogByPh((xf+i)->iznd);
		}
		if((xf+i)->itapty != -1)
		{
			(xf+i)->itapty = tapty_tb->GetLogByPh((xf+i)->itapty);
		}
		if((xf+i)->iztapty != -1)
		{
			(xf+i)->iztapty = tapty_tb->GetLogByPh((xf+i)->iztapty);
		}
	}

	//update table
	xf_tb->UpdateTable(xf);

	delete[] xf;
	xf = 0;
	lv_xf = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  CB_RebuildPtr()
int NMD::CB_RebuildPtr()
{
	if (lld->mx_cb==0)
		return 0;

	int		i;

	//read table into buffer
	lv_cb = cb_tb->GetCount();
	if(lv_cb == 0)
	{
		return NMD_SUCCEED;
	}

	cb = new CB[lv_cb];
	cb_tb->CopyTable(cb);
	
	//rebuild pointer
	for(i = 0; i < lv_cb; i++)
	{
//		if((cb+i)->r_st != -1)
//		{
//			(cb+i)->r_st = r_st_cb_tb->GetLogByPh((cb+i)->r_st);
//		}
		if((cb+i)->ind != -1)
		{
			(cb+i)->ind = nd_tb->GetLogByPh((cb+i)->ind);
		}
		if((cb+i)->iznd != -1)
		{
			(cb+i)->iznd = nd_tb->GetLogByPh((cb+i)->iznd);
		}
	}

	//update table
	cb_tb->UpdateTable(cb);

	delete[] cb;
	cb = 0;
	lv_cb = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  SO_RebuildPtr()
int NMD::SO_RebuildPtr()
{
	if (lld->mx_so == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_so = so_tb->GetCount();
	if(lv_so == 0)
	{
		return NMD_SUCCEED;
	}

	so = new SO[lv_so];
	so_tb->CopyTable(so);
	
	//rebuild pointer
	for(i = 0; i < lv_so; i++)
	{
		if((so+i)->ind != -1)
		{
			(so+i)->ind = nd_tb->GetLogByPh((so+i)->ind);
		}
	}

	//update table
	so_tb->UpdateTable(so);

	delete[] so;
	so = 0;
	lv_so = 0;

	return NMD_SUCCEED;
}
////////////////////////////////////////////////////////////////////////////////
//  LD_RebuildPtr()
int NMD::LD_RebuildPtr()
{
	if (lld->mx_ld == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_ld = ld_tb->GetCount();
	if(lv_ld == 0)
	{
		return NMD_SUCCEED;
	}

	ld = new LD[lv_ld];
	ld_tb->CopyTable(ld);
	
	//rebuild pointer
	for(i = 0; i < lv_ld; i++)
	{
//		if((ld+i)->r_st != -1)
//		{
//			(ld+i)->r_st = r_st_ld_tb->GetLogByPh((ld+i)->r_st);
//		}
		if((ld+i)->r_zn != -1)
		{
			(ld+i)->r_zn = r_zn_ld_tb->GetLogByPh((ld+i)->r_zn);
		}
		if((ld+i)->ind != -1)
		{
			(ld+i)->ind = nd_tb->GetLogByPh((ld+i)->ind);
		}

	}

	//update table
	ld_tb->UpdateTable(ld);

	delete[] ld;
	ld = 0;
	lv_ld = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  BUS_RebuildPtr()
int NMD::BUS_RebuildPtr()
{
	if (lld->mx_bus == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_bus = bus_tb->GetCount();
	if(lv_bus == 0)
	{
		return NMD_SUCCEED;
	}

	bus = new BUS[lv_bus];
	bus_tb->CopyTable(bus);
	
	//rebuild pointer
	for(i = 0; i < lv_bus; i++)
	{
//		if((bus+i)->r_st != -1)
//		{
//			(bus+i)->r_st = r_st_bus_tb->GetLogByPh((bus+i)->r_st);
//		}
		if((bus+i)->ind != -1)
		{
			(bus+i)->ind = nd_tb->GetLogByPh((bus+i)->ind);
		}
	}

	//update table
	bus_tb->UpdateTable(bus);

	delete[] bus;
	bus = 0;
	lv_bus = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  CP_RebuildPtr()
int NMD::CP_RebuildPtr()
{
	if ((lld->mx_cp == 0)
		|| ((strlen(cp_tb->GetDataByLog(0)->nd) == 0) && (strlen(cp_tb->GetDataByLog(0)->znd) == 0)))
		return 1;

	int		i;

	//read table into buffer
	lv_cp = cp_tb->GetCount();
	if(lv_cp == 0)
	{
		return NMD_SUCCEED;
	}

	cp = new CP[lv_cp];
	cp_tb->CopyTable(cp);
	
	//rebuild pointer
	for(i = 0; i < lv_cp; i++)
	{
		if((cp+i)->ind != -1)
		{
			(cp+i)->ind = nd_tb->GetLogByPh((cp+i)->ind);
		}
		if ((cp + i)->iznd != -1)
		{
			(cp + i)->iznd = nd_tb->GetLogByPh((cp + i)->iznd);
		}
	}

	//update table
	cp_tb->UpdateTable(cp);

	delete[] cp;
	cp = 0;
	lv_cp = 0;

	return NMD_SUCCEED;
}
////////////////////////////////////////////////////////////////////////////////
//  ZN_RebuildPtr()
int NMD::ZN_RebuildPtr()
{
	if (lld->mx_zn == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_zn = zn_tb->GetCount();
	if(lv_zn == 0)
	{
		return NMD_SUCCEED;
	}

	zn = new ZN[lv_zn];
	zn_tb->CopyTable(zn);
	
	//rebuild pointer
	for(i = 0; i < lv_zn; i++)
	{
//		if((zn+i)->r_st != -1)
//		{
//			(zn+i)->r_st = r_st_zn_tb->GetLogByPh((zn+i)->r_st);
//		}
		if((zn+i)->r_ld != -1)
		{
			(zn+i)->r_ld = r_zn_ld_tb->GetLogByPh((zn+i)->r_ld);
		}
		if((zn+i)->r_sec != -1)
		{
			(zn+i)->r_sec = r_zn_sec_tb->GetLogByPh((zn+i)->r_sec);
		}
	}

	//update table
	zn_tb->UpdateTable(zn);

	delete[] zn;
	zn = 0;
	lv_zn = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  SEC_RebuildPtr()
int NMD::SEC_RebuildPtr()
{
	if (lld->mx_sec == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_sec = sec_tb->GetCount();
	if(lv_sec == 0)
	{
		return NMD_SUCCEED;
	}

	sec = new SEC[lv_sec];
	sec_tb->CopyTable(sec);
	
	//rebuild pointer
	for(i = 0; i < lv_sec; i++)
	{
		if((sec+i)->r_zn != -1)
		{
			(sec+i)->r_zn = r_zn_sec_tb->GetLogByPh((sec+i)->r_zn);
		}
		if((sec+i)->ind != -1)
		{
			(sec+i)->ind = nd_tb->GetLogByPh((sec+i)->ind);
		}
		if((sec+i)->iznd != -1)
		{
			(sec+i)->iznd = nd_tb->GetLogByPh((sec+i)->iznd);
		}
	}

	//update table
	sec_tb->UpdateTable(sec);

	delete[] sec;
	sec = 0;
	lv_sec = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  ND_RebuildPtr()
int NMD::ND_RebuildPtr()
{
	if (lld->mx_nd == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_nd = nd_tb->GetCount();
	if(lv_nd == 0)
	{
		return NMD_SUCCEED;
	}

	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);
	
	//rebuild pointer
	for(i = 0; i < lv_nd; i++)
	{
		if((nd+i)->r_cb != -1)
		{
			(nd+i)->r_cb = r_nd_cb_tb->GetLogByPh((nd+i)->r_cb);
		}
		if((nd+i)->r_ld != -1)
		{
			(nd+i)->r_ld = r_nd_ld_tb->GetLogByPh((nd+i)->r_ld);
		}
		if((nd+i)->r_xf != -1)
		{
			(nd+i)->r_xf = r_nd_xf_tb->GetLogByPh((nd+i)->r_xf);
		}
//		if((nd+i)->r_zn != -1)
//		{
//			(nd+i)->r_zn = r_nd_zn_tb->GetLogByPh((nd+i)->r_zn);
//		}
		if((nd+i)->r_sec != -1)
		{
			(nd+i)->r_sec = r_nd_sec_tb->GetLogByPh((nd+i)->r_sec);
		}
	}

	//update table
	nd_tb->UpdateTable(nd);

	delete[] nd;
	nd = 0;
	lv_nd = 0;

	return NMD_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  R_XFMR_XF_RebuildPtr()
int NMD::R_XFMR_XF_RebuildPtr()
{
	if (lld->mx_r_xfmr_xf == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_r_xfmr_xf = r_xfmr_xf_tb->GetCount();
	if(lv_r_xfmr_xf == 0)
	{
		return NMD_SUCCEED;
	}

	r_xfmr_xf = new R_XFMR_XF[lv_r_xfmr_xf];
	r_xfmr_xf_tb->CopyTable(r_xfmr_xf);
	
	//rebuild pointer
	for(i = 0; i < lv_r_xfmr_xf; i++)
	{
		if((r_xfmr_xf+i)->xfmr_id_ptr != -1)
		{
			(r_xfmr_xf+i)->xfmr_id_ptr = xfmr_tb->GetLogByPh((r_xfmr_xf+i)->xfmr_id_ptr);
		}
		if((r_xfmr_xf+i)->xf_id_ptr != -1)
		{
			(r_xfmr_xf+i)->xf_id_ptr = xf_tb->GetLogByPh((r_xfmr_xf+i)->xf_id_ptr);
		}
		if((r_xfmr_xf+i)->fptr != -1)
		{
			(r_xfmr_xf+i)->fptr = r_xfmr_xf_tb->GetLogByPh((r_xfmr_xf+i)->fptr);
		}
		if((r_xfmr_xf+i)->bptr != -1)
		{
			(r_xfmr_xf+i)->bptr = r_xfmr_xf_tb->GetLogByPh((r_xfmr_xf+i)->bptr);
		}
	}

	//update table
	r_xfmr_xf_tb->UpdateTable(r_xfmr_xf);

	delete[] r_xfmr_xf;
	r_xfmr_xf = 0;
	lv_r_xfmr_xf = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ZN_LD_RebuildPtr()
int NMD::R_ZN_LD_RebuildPtr()
{
	int		i;

	//read table into buffer
	if (lld->mx_r_zn_ld > 0 )
		lv_r_zn_ld = r_zn_ld_tb->GetCount();
	else
		return NMD_SUCCEED;
	if(lv_r_zn_ld == 0)
	{
		return NMD_SUCCEED;
	}

	r_zn_ld = new R_ZN_LD[lv_r_zn_ld];
	r_zn_ld_tb->CopyTable(r_zn_ld);
	
	//rebuild pointer
	for(i = 0; i < lv_r_zn_ld; i++)
	{
		if((r_zn_ld+i)->zn_id_ptr != -1)
		{
			(r_zn_ld+i)->zn_id_ptr = zn_tb->GetLogByPh((r_zn_ld+i)->zn_id_ptr);
		}
		if((r_zn_ld+i)->ld_id_ptr != -1)
		{
			(r_zn_ld+i)->ld_id_ptr = ld_tb->GetLogByPh((r_zn_ld+i)->ld_id_ptr);
		}
		if((r_zn_ld+i)->fptr != -1)
		{
			(r_zn_ld+i)->fptr = r_zn_ld_tb->GetLogByPh((r_zn_ld+i)->fptr);
		}
		if((r_zn_ld+i)->bptr != -1)
		{
			(r_zn_ld+i)->bptr = r_zn_ld_tb->GetLogByPh((r_zn_ld+i)->bptr);
		}
	}

	//update table
	r_zn_ld_tb->UpdateTable(r_zn_ld);

	delete[] r_zn_ld;
	r_zn_ld = 0;
	lv_r_zn_ld = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ZN_SEC_RebuildPtr()
int NMD::R_ZN_SEC_RebuildPtr()
{
	if (lld->mx_r_zn_sec == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_r_zn_sec = r_zn_sec_tb->GetCount();
	if(lv_r_zn_sec == 0)
	{
		return NMD_SUCCEED;
	}

	r_zn_sec = new R_ZN_SEC[lv_r_zn_sec];
	r_zn_sec_tb->CopyTable(r_zn_sec);
	
	//rebuild pointer
	for(i = 0; i < lv_r_zn_sec; i++)
	{
		if((r_zn_sec+i)->zn_id_ptr != -1)
		{
			(r_zn_sec+i)->zn_id_ptr = zn_tb->GetLogByPh((r_zn_sec+i)->zn_id_ptr);
		}
		if((r_zn_sec+i)->sec_id_ptr != -1)
		{
			(r_zn_sec+i)->sec_id_ptr = sec_tb->GetLogByPh((r_zn_sec+i)->sec_id_ptr);
		}
		if((r_zn_sec+i)->fptr != -1)
		{
			(r_zn_sec+i)->fptr = r_zn_sec_tb->GetLogByPh((r_zn_sec+i)->fptr);
		}
		if((r_zn_sec+i)->bptr != -1)
		{
			(r_zn_sec+i)->bptr = r_zn_sec_tb->GetLogByPh((r_zn_sec+i)->bptr);
		}
	}

	//update table
	r_zn_sec_tb->UpdateTable(r_zn_sec);

	delete[] r_zn_sec;
	r_zn_sec = 0;
	lv_r_zn_sec = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ZN_cp_RebuildPtr()
int NMD::R_ZN_CP_RebuildPtr()
{
	if (lld->mx_r_zn_cp == 0)
		return 1;

	int		i;

	//read table into buffer
	int lv_r_zn_cp = r_zn_cp_tb->GetCount();
	if (lv_r_zn_cp == 0)
	{
		return NMD_SUCCEED;
	}

	r_zn_cp = new R_ZN_CP[lv_r_zn_cp];
	r_zn_cp_tb->CopyTable(r_zn_cp);

	//rebuild pointer
	for (i = 0; i < lv_r_zn_cp; i++)
	{
		if ((r_zn_cp + i)->zn_id_ptr != -1)
		{
			(r_zn_cp + i)->zn_id_ptr = zn_tb->GetLogByPh((r_zn_cp + i)->zn_id_ptr);
		}
		if ((r_zn_cp + i)->cp_id_ptr != -1)
		{
			(r_zn_cp + i)->cp_id_ptr = cp_tb->GetLogByPh((r_zn_cp + i)->cp_id_ptr);
		}
		if ((r_zn_cp + i)->fptr != -1)
		{
			(r_zn_cp + i)->fptr = r_zn_cp_tb->GetLogByPh((r_zn_cp + i)->fptr);
		}
		if ((r_zn_cp + i)->bptr != -1)
		{
			(r_zn_cp + i)->bptr = r_zn_cp_tb->GetLogByPh((r_zn_cp + i)->bptr);
		}
	}

	//update table
	r_zn_cp_tb->UpdateTable(r_zn_cp);

	delete[] r_zn_cp;
	r_zn_cp = 0;
	lv_r_zn_cp = 0;

	return NMD_SUCCEED;
}


////////////////////////////////////////////////////////////////////////////////
//  R_ZN_CB_RebuildPtr()
int NMD::R_ZN_CB_RebuildPtr()
{
	if (lld->mx_r_zn_cb == 0)
		return 1;
	
	int		i;

	//read table into buffer
	lv_r_zn_cb = r_zn_cb_tb->GetCount();
	if(lv_r_zn_cb == 0)
	{
		return NMD_SUCCEED;
	}

	r_zn_cb = new R_ZN_CB[lv_r_zn_cb];
	r_zn_cb_tb->CopyTable(r_zn_cb);
	
	//rebuild pointer
	for(i = 0; i < lv_r_zn_cb; i++)
	{
		if((r_zn_cb+i)->zn_id_ptr != -1)
		{
			(r_zn_cb+i)->zn_id_ptr = zn_tb->GetLogByPh((r_zn_cb+i)->zn_id_ptr);
		}
		if((r_zn_cb+i)->cb_id_ptr != -1)
		{
			(r_zn_cb+i)->cb_id_ptr = cb_tb->GetLogByPh((r_zn_cb+i)->cb_id_ptr);
		}
		if((r_zn_cb+i)->fptr != -1)
		{
			(r_zn_cb+i)->fptr = r_zn_cb_tb->GetLogByPh((r_zn_cb+i)->fptr);
		}
		if((r_zn_cb+i)->bptr != -1)
		{
			(r_zn_cb+i)->bptr = r_zn_cb_tb->GetLogByPh((r_zn_cb+i)->bptr);
		}
	}

	//update table
	r_zn_cb_tb->UpdateTable(r_zn_cb);

	delete[] r_zn_cb;
	r_zn_cb = 0;
	lv_r_zn_cb = 0;

	return NMD_SUCCEED;
}


////////////////////////////////////////////////////////////////////////////////
//  R_ND_CB_RebuildPtr()
int NMD::R_ND_LD_RebuildPtr()
{
	int		i;
//	char log_[100];
	
	//read table into buffer
	if (lld->mx_r_zn_ld == 0)
		return NMD_SUCCEED;
	lv_r_nd_ld = r_nd_ld_tb->GetCount();
	if(lv_r_nd_ld == 0)
	{
		return NMD_SUCCEED;
	}
//sprintf(log_,"cnt %d",lv_r_nd_ld);
//lld->WriteLog(lld->logfile,log_);
	
	r_nd_ld = new R_ND_LD[lv_r_nd_ld];
	r_nd_ld_tb->CopyTable(r_nd_ld);
//sprintf(log_,"copytable %d",lv_r_nd_ld);
//lld->WriteLog(lld->logfile,log_);
	
	//rebuild pointer
	for(i = 0; i < lv_r_nd_ld; i++)
	{
		if((r_nd_ld+i)->nd_id_ptr != -1)
		{
			(r_nd_ld+i)->nd_id_ptr = nd_tb->GetLogByPh((r_nd_ld+i)->nd_id_ptr);
//sprintf(log_,"nd_ %d",lv_r_nd_ld);
//lld->WriteLog(lld->logfile,log_);
		}
		if((r_nd_ld+i)->ld_id_ptr != -1)
		{
			(r_nd_ld+i)->ld_id_ptr = ld_tb->GetLogByPh((r_nd_ld+i)->ld_id_ptr);
//sprintf(log_,"ld_ %d",lv_r_nd_ld);
//lld->WriteLog(lld->logfile,log_);
		}
		if((r_nd_ld+i)->fptr != -1)
		{
			(r_nd_ld+i)->fptr = r_nd_ld_tb->GetLogByPh((r_nd_ld+i)->fptr);
		}
		if((r_nd_ld+i)->bptr != -1)
		{
			(r_nd_ld+i)->bptr = r_nd_ld_tb->GetLogByPh((r_nd_ld+i)->bptr);
		}
	}
	
	//update table
	r_nd_ld_tb->UpdateTable(r_nd_ld);
//sprintf(log_,"up_ %d",lv_r_nd_ld);
//lld->WriteLog(lld->logfile,log_);
	
	delete[] r_nd_ld;
	r_nd_ld = 0;
	lv_r_nd_ld = 0;
//sprintf(log_,"dele_ %d",lv_r_nd_ld);
//lld->WriteLog(lld->logfile,log_);
	
	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ND_CB_RebuildPtr()
int NMD::R_ND_CB_RebuildPtr()
{
	int		i;
	if (lld->mx_r_nd_cb == 0)
		return 1;

	if (r_nd_cb_tb == NULL)
		return 1;
	//read table into buffer
	lv_r_nd_cb = r_nd_cb_tb->GetCount();
	if(lv_r_nd_cb == 0)
	{
		return NMD_SUCCEED;
	}

	r_nd_cb = new R_ND_CB[lv_r_nd_cb];
	r_nd_cb_tb->CopyTable(r_nd_cb);
	
	//rebuild pointer
	for(i = 0; i < lv_r_nd_cb; i++)
	{
		if((r_nd_cb+i)->nd_id_ptr != -1)
		{
			(r_nd_cb+i)->nd_id_ptr = nd_tb->GetLogByPh((r_nd_cb+i)->nd_id_ptr);
		}
		if((r_nd_cb+i)->cb_id_ptr != -1)
		{
			(r_nd_cb+i)->cb_id_ptr = cb_tb->GetLogByPh((r_nd_cb+i)->cb_id_ptr);
		}
		if((r_nd_cb+i)->fptr != -1)
		{
			(r_nd_cb+i)->fptr = r_nd_cb_tb->GetLogByPh((r_nd_cb+i)->fptr);
		}
		if((r_nd_cb+i)->bptr != -1)
		{
			(r_nd_cb+i)->bptr = r_nd_cb_tb->GetLogByPh((r_nd_cb+i)->bptr);
		}
	}

	//update table
	r_nd_cb_tb->UpdateTable(r_nd_cb);

	delete[] r_nd_cb;
	r_nd_cb = 0;
	lv_r_nd_cb = 0;

	return NMD_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  R_ND_SEC_RebuildPtr()
int NMD::R_ND_SEC_RebuildPtr()
{
	if (lld->mx_r_nd_sec == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_r_nd_sec = r_nd_sec_tb->GetCount();
	if(lv_r_nd_sec == 0)
	{
		return NMD_SUCCEED;
	}

	r_nd_sec = new R_ND_SEC[lv_r_nd_sec];
	r_nd_sec_tb->CopyTable(r_nd_sec);
	
	//rebuild pointer
	for(i = 0; i < lv_r_nd_sec; i++)
	{
		if((r_nd_sec+i)->nd_id_ptr != -1)
		{
			(r_nd_sec+i)->nd_id_ptr = nd_tb->GetLogByPh((r_nd_sec+i)->nd_id_ptr);
		}
		if((r_nd_sec+i)->sec_id_ptr != -1)
		{
			(r_nd_sec+i)->sec_id_ptr = sec_tb->GetLogByPh((r_nd_sec+i)->sec_id_ptr);
		}
		if((r_nd_sec+i)->fptr != -1)
		{
			(r_nd_sec+i)->fptr = r_nd_sec_tb->GetLogByPh((r_nd_sec+i)->fptr);
		}
		if((r_nd_sec+i)->bptr != -1)
		{
			(r_nd_sec+i)->bptr = r_nd_sec_tb->GetLogByPh((r_nd_sec+i)->bptr);
		}
	}

	//update table
	r_nd_sec_tb->UpdateTable(r_nd_sec);

	delete[] r_nd_sec;
	r_nd_sec = 0;
	lv_r_nd_sec = 0;

	return NMD_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  R_ND_cp_RebuildPtr()
int NMD::R_ND_CP_RebuildPtr()
{
	if ((lld->mx_cp == 0)
		|| ((strlen(cp_tb->GetDataByLog(0)->nd) == 0) && (strlen(cp_tb->GetDataByLog(0)->znd) == 0))
		)
		return 1;

	if (lld->mx_r_nd_cp == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_r_nd_cp = r_nd_cp_tb->GetCount();
	if (lv_r_nd_cp == 0)
	{
		return NMD_SUCCEED;
	}

	r_nd_cp = new R_ND_CP[lv_r_nd_cp];
	r_nd_cp_tb->CopyTable(r_nd_cp);

	//rebuild pointer
	for (i = 0; i < lv_r_nd_cp; i++)
	{
		if ((r_nd_cp + i)->nd_id_ptr != -1)
		{
			(r_nd_cp + i)->nd_id_ptr = nd_tb->GetLogByPh((r_nd_cp + i)->nd_id_ptr);
		}
		if ((r_nd_cp + i)->cp_id_ptr != -1)
		{
			(r_nd_cp + i)->cp_id_ptr = cp_tb->GetLogByPh((r_nd_cp + i)->cp_id_ptr);
		}
		if ((r_nd_cp + i)->fptr != -1)
		{
			(r_nd_cp + i)->fptr = r_nd_cp_tb->GetLogByPh((r_nd_cp + i)->fptr);
		}
		if ((r_nd_cp + i)->bptr != -1)
		{
			(r_nd_cp + i)->bptr = r_nd_cp_tb->GetLogByPh((r_nd_cp + i)->bptr);
		}
	}

	//update table
	r_nd_cp_tb->UpdateTable(r_nd_cp);

	delete[] r_nd_cp;
	r_nd_cp = 0;
	lv_r_nd_cp = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  DIGITAL_ANALOG_PtrBuild()
int NMD::DIGITAL_ANALOG_PtrBuild()
{
	int			i/*, j*/;
// 	ChildPtr1	*ptr, *temp;

	DIGITAL* digitalt = new DIGITAL();

	//build pinters to digital table for each cb
	lv_cb = cb_tb->GetCount();
	if(lv_cb != 0)
	{
		for(i = 0; i < lv_cb; i++)
		{
			cb = cb_tb->GetDataByLog(i);
			//把遥信表里面的和这个开关关联的测点的逻辑号填到开关表里面，这样可以方便搜索
			strcpy(digitalt->id, cb->digital);
			cb->idigital = digital_tb->FindLog(*digitalt);
		}
		cb = 0;
		lv_cb = 0;
	}
	delete digitalt;
/*
	//build pointers to analog table for each zn
	lv_zn = zn_tb->GetCount();
	if(lv_zn != 0)
	{
		for(i = 0; i < lv_zn; i++)
		{
			zn = zn_tb->GetDataByLog(i);
			zn->ialgip = analog_tb->FindLog(ANALOG(zn->algip));
			zn->ialgiq = analog_tb->FindLog(ANALOG(zn->algiq));
			zn->ialgii = analog_tb->FindLog(ANALOG(zn->algii));
			zn->ialgzp = analog_tb->FindLog(ANALOG(zn->algzp));
			zn->ialgzq = analog_tb->FindLog(ANALOG(zn->algzq));
			zn->ialgzi = analog_tb->FindLog(ANALOG(zn->algzi));
			zn->ialg3p = analog_tb->FindLog(ANALOG(zn->alg3p));
			zn->ialg3q = analog_tb->FindLog(ANALOG(zn->alg3q));
			zn->ialg3i = analog_tb->FindLog(ANALOG(zn->alg3i));
			zn->ialg4p = analog_tb->FindLog(ANALOG(zn->alg4p));
			zn->ialg4q = analog_tb->FindLog(ANALOG(zn->alg4q));
			zn->ialg4i = analog_tb->FindLog(ANALOG(zn->alg4i));
			zn->ialg5p = analog_tb->FindLog(ANALOG(zn->alg5p));
			zn->ialg5q = analog_tb->FindLog(ANALOG(zn->alg5q));
			zn->ialg5i = analog_tb->FindLog(ANALOG(zn->alg5i));
		}
		zn = 0;
		lv_zn = 0;
	}
*/

	//build pointers to analog tabl for each sec
	//lv_sec = sec_tb->GetCount();
	//if(lv_sec != 0)
	//{
	//	for(i = 0; i < lv_sec;i++)
	//	{
	//		sec = sec_tb->GetDataByLog(i);
	//		sec->ialgip = analog_tb->FindLog(ANALOG(sec->algip));
	//		sec->ialgiq = analog_tb->FindLog(ANALOG(sec->algiq));
	//		sec->ialgii = analog_tb->FindLog(ANALOG(sec->algii));
	//		sec->ialgzp = analog_tb->FindLog(ANALOG(sec->algzp));
	//		sec->ialgzq = analog_tb->FindLog(ANALOG(sec->algzq));
	//		sec->ialgzi = analog_tb->FindLog(ANALOG(sec->algzi));
	//	}
	//	sec = 0;
	//	lv_sec = 0;
	//}

	//if (lld->mx_bus>0)
	//{
	//	//build pointers to analog table for each bus
	//	lv_bus = bus_tb->GetCount();
	//	if(lv_bus != 0)
	//	{
	//		for(i = 0; i < lv_bus; i++)
	//		{
	//			bus = bus_tb->GetDataByLog(i);
	//			bus->ialgv = analog_tb->FindLog(ANALOG(bus->algv));
	//		}
	//		bus = 0;
	//		lv_bus = 0;
	//	}
	//}

	////build pointers to analog table for each ld
	////主要是这个表
	//lv_ld = ld_tb->GetCount();
	//if(lv_ld != 0)
	//{
	//	for(i = 0; i < lv_ld; i++)
	//	{
	//		ld = ld_tb->GetDataByLog(i);
	//		ld->ialgp = analog_tb->FindLog(ANALOG(ld->algp));
	//		ld->ialgq = analog_tb->FindLog(ANALOG(ld->algq));
	//		ld->ialgi = analog_tb->FindLog(ANALOG(ld->algi));
	//	}
	//	ld = 0;
	//	lv_ld = 0;
	//}

	////build pointers to analog table for each so
	////电源表的量测有用
	//lv_so = so_tb->GetCount();
	//if(lv_so != 0)
	//{
	//	for(i = 0; i < lv_so; i++)
	//	{
	//		so = so_tb->GetDataByLog(i);
	//		so->ialgp = analog_tb->FindLog(ANALOG(so->algp));
	//		so->ialgq = analog_tb->FindLog(ANALOG(so->algq));
	//		so->ialgi = analog_tb->FindLog(ANALOG(so->algi));
	//	}
	//	so = 0;
	//	lv_so = 0;
	//}

	////build pointers to analog table for each cp
	//lv_cp = cp_tb->GetCount();
	//if(lv_cp != 0)
	//{
	//	for(i = 0; i < lv_cp; i++)
	//	{
	//		cp = cp_tb->GetDataByLog(i);
	//		cp->ialgq = analog_tb->FindLog(ANALOG(cp->algq));
	//	}
	//	cp = 0;
	//	lv_cp = 0;
	//}

	//build pointers to analog table for each xfmr and xf
/*
	lv_xfmr = xfmr_tb->GetCount();
	if(lv_xfmr != 0)
	{
		for(i = 0; i < lv_xfmr; i++)
		{
			xfmr = xfmr_tb->GetDataByLog(i);
			if(xfmr->type == 2)  //two-winding xfmr
			{
				xfmr->ialgph = analog_tb->FindLog(ANALOG(xfmr->algph));
				xfmr->ialgqh = analog_tb->FindLog(ANALOG(xfmr->algqh));
				xfmr->ialgih = analog_tb->FindLog(ANALOG(xfmr->algih));
				xfmr->ialgth = analog_tb->FindLog(ANALOG(xfmr->algth));
				xfmr->ialgpl = analog_tb->FindLog(ANALOG(xfmr->algpl));
				xfmr->ialgql = analog_tb->FindLog(ANALOG(xfmr->algql));
				xfmr->ialgil = analog_tb->FindLog(ANALOG(xfmr->algil));
				xfmr->ialgtl = analog_tb->FindLog(ANALOG(xfmr->algtl));
			}
			else if(xfmr->type == 3) //three-winding xfmr
			{
				xfmr->ialgph = analog_tb->FindLog(ANALOG(xfmr->algph));
				xfmr->ialgqh = analog_tb->FindLog(ANALOG(xfmr->algqh));
				xfmr->ialgih = analog_tb->FindLog(ANALOG(xfmr->algih));
				xfmr->ialgth = analog_tb->FindLog(ANALOG(xfmr->algth));
				xfmr->ialgpm = analog_tb->FindLog(ANALOG(xfmr->algpm));
				xfmr->ialgqm = analog_tb->FindLog(ANALOG(xfmr->algqm));
				xfmr->ialgim = analog_tb->FindLog(ANALOG(xfmr->algim));
				xfmr->ialgtm = analog_tb->FindLog(ANALOG(xfmr->algtm));
				xfmr->ialgpl = analog_tb->FindLog(ANALOG(xfmr->algpl));
				xfmr->ialgql = analog_tb->FindLog(ANALOG(xfmr->algql));
				xfmr->ialgil = analog_tb->FindLog(ANALOG(xfmr->algil));
				xfmr->ialgtl = analog_tb->FindLog(ANALOG(xfmr->algtl));
			}
			else
			{
				continue;
			}
		}

		for(i = 0; i < lv_xfmr; i++)
		{
			j = 0;  //child number
			xfmr = xfmr_tb->GetDataByLog(i);
			ptr = r_xfmr_xf_tb->FindChildPtr1(xfmr->r_xf);
			temp = ptr;
			while( temp )
			{
				j++;
				temp = temp->Next;
			}
			if(j == 1)
			{
				temp = ptr;
				xf = xf_tb->GetDataByPh(temp->Ptr);
				xf->ialgip = xfmr->ialgph;
				xf->ialgiq = xfmr->ialgqh;
				xf->ialgii = xfmr->ialgih;
				xf->ialgit = xfmr->ialgth;
				xf->ialgzp = xfmr->ialgpl;
				xf->ialgzq = xfmr->ialgql;
				xf->ialgzi = xfmr->ialgil;
				xf->ialgzt = xfmr->ialgtl;
			}
			else if(j == 3)
			{
				temp = ptr;
				xf = xf_tb->GetDataByPh(temp->Ptr);
				xf->ialgip = xfmr->ialgph;
				xf->ialgiq = xfmr->ialgqh;
				xf->ialgii = xfmr->ialgih;
				xf->ialgit = xfmr->ialgth;
				xf->ialgzp = -1;
				xf->ialgzq = -1;
				xf->ialgzi = -1;
				xf->ialgzt = -1;

				temp = temp->Next;
				xf = xf_tb->GetDataByPh(temp->Ptr);
				xf->ialgip = xfmr->ialgpm;
				xf->ialgiq = xfmr->ialgqm;
				xf->ialgii = xfmr->ialgim;
				xf->ialgit = xfmr->ialgtm;
				xf->ialgzp = -1;
				xf->ialgzq = -1;
				xf->ialgzi = -1;
				xf->ialgzt = -1;

				temp = temp->Next;
				xf = xf_tb->GetDataByPh(temp->Ptr);
				xf->ialgip = xfmr->ialgpl;
				xf->ialgiq = xfmr->ialgql;
				xf->ialgii = xfmr->ialgil;
				xf->ialgit = xfmr->ialgtl;
				xf->ialgzp = -1;
				xf->ialgzq = -1;
				xf->ialgzi = -1;
				xf->ialgzt = -1;
			}
            ::FreeChildPtr(ptr);
		}
		
		xfmr = 0;
		xf = 0;
		lv_xfmr = 0;
	}*/


	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  SetVL()
//  This function sets vl filed of nd table. It searches through all nds
//  from bus nd by non-xf branches (i.e. cb, zn, sec), and sets vl of all
//  these nds equal to that of bus nd.
int NMD::SetVL()
{///////////////////////////////////////////////////////////////////////////////
	char str[500];
	int			i, vl;
	int			ind, knd, kknd;
	int			*busnd;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;

	//get record count in nd table
	lv_nd = nd_tb->GetCount();
	if(lv_nd == 0)
	{
		printf("there is no record in nd table\n");
		sprintf(str,"节点表中无记录!");
		lld->WriteLog(lld->logfile,str);
		return NMD_SUCCEED;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->q |= ND_UNPROC;
	}

	if (lld->mx_bus == 0)
	//get record count in bus table
//	lv_bus = bus_tb->GetCount();
//	if(lv_bus == 0)
	{
		printf("there is no record in bus table\n");
		sprintf(str,"母线表中无记录!");
		lld->WriteLog(lld->logfile,str);

//在整个系统中都没有母线的情况是存在的，将节点的电压都设成10kV
		for(i = 0; i < lv_nd; i++)
		{
			(nd+i)->vl = 10;
		}

		nd_tb->UpdateTable(nd);
		
		delete[] nd;
		nd = 0;
		lv_nd = 0;

		return NMD_SUCCEED;
	}

	//read all bus nds into buffer
	//and set vl of all bus nds
	//设定母线的电压值，一般情况下位1
	busnd = new int[lv_bus];//整型的指针，相当于一个整数的数组，记录一个地址
	for(i = 0; i < lv_bus; i++)
	{
		//将母线表bus_tb的节点编号ind，按顺序写到busnd指针数组里面
		*(busnd+i) = bus_tb->GetDataByLog(i)->ind;
		//bus_tb中的ind就是nd_tb中的序号，这2个在进行编号的时候就是一致的 
		(nd+*(busnd+i))->vl = bus_tb->GetDataByLog(i)->vl;
	}

	//search through nds from bus nd
	//深度搜索
	for(i = 0; i < lv_bus; i++)
	{
		ind = *(busnd+i);//母线的起始节点编号
		//之前已经将所有的节点都置为未处理
		if((nd+ind)->q & ND_UNPROC) //this bus nd has not processed
		{
			vl = (nd+ind)->vl;

			//create circular chain for this bus nd
			knd = ind;//当前节点赋给一个临时的变量
			(nd+knd)->fch = knd; //当前节点号赋给前向链 
			(nd+knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

			//拿当前节点向下扩展，找到所有和它相连的节点
			while( 1 )
			{
				//read cbs connected to this nd
				//从节点-开关关系表找到所有和这个节点相连的开关
				cb_p = r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
				while(cb_p)
				{
					//find the opposite nd through this cb
					//kknd表示开关的对侧节点，这里的节点都是整型的编号
					if(knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kknd = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kknd = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}

					//process this opposite nd
					//如果对侧节点 未处理
					if((nd+kknd)->q & ND_UNPROC)
					{
						//set vl of this nd
						if((nd+kknd)->vl == 0)  //non-bus nd
						{
							(nd+kknd)->vl = vl;
						}
						else  //bus nd
						{
							if((nd+kknd)->vl != vl)
							{
								printf("the bus vl of nd (%d) may be wrong\n", kknd);
								sprintf(str,"节点(%s)的母线电压等级可能错误!",(nd+kknd)->id);
								lld->WriteLog(lld->logfile,str);
							}
						}

						//循环链表的使用，每扩展一个新的节点，做2件事：
						//1.将上一个节点的前向链设为自己的前向链，其实是第一个节点永远是所有节点的前向链
						//2.修改上一个节点的前向链，把自己设为上一个节点的前向链

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

				//read secs connected to this nd
				sec_p = r_nd_sec_tb->FindChildPtr1((nd+knd)->r_sec);
				sec_p0 = sec_p;
				while(sec_p)
				{
					//find the opposite nd through this sec
					if(knd == sec_tb->GetDataByPh(sec_p->Ptr)->ind)
					{
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
					}
					else
					{
						kknd = sec_tb->GetDataByPh(sec_p->Ptr)->ind;
					}

					//process this opposite nd
					if((nd+kknd)->q & ND_UNPROC)
					{
						//set vl of this nd
						if((nd+kknd)->vl == 0)  //non-bus nd
						{
							(nd+kknd)->vl = vl;
						}
						else  //bus nd
						{
							if((nd+kknd)->vl != vl)
							{
								printf("the bus vl of nd (%d) may be wrong\n", kknd);
							}
						}

						//put this nd into circular chain
						(nd+kknd)->fch = (nd+knd)->fch;
						(nd+knd)->fch = kknd;
						(nd+kknd)->q &= (~ND_UNPROC);
					}
					sec_p = sec_p->Next;
				}
				::FreeChildPtr(sec_p0);

				//move to next nd
				knd = (nd+knd)->fch;

				//循环到头，连起来了，表示全部走了一遍
				//拿另外的节点再开始搜索，一直到所有的节点都处理
				if(knd == ind)
				{
					break;
				}
			}
		}
	}

	//update table
	nd_tb->UpdateTable(nd);

	delete[] busnd;
	delete[] nd;
	nd = 0;
	lv_nd = 0;

	return NMD_SUCCEED;
}

int NMD::SetVL(int lld)
{///////////////////////////////////////////////////////////////////////////////
	char	str[500];
	int			i;
	
	//get record count in nd table
	lv_nd = nd_tb->GetCount();
	if(lv_nd == 0)
	{
		printf("there is no record in nd table\n");
		sprintf(str,"节点表中无记录!");
		return NMD_SUCCEED;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->vl = 10;
	}

	//update table
	nd_tb->UpdateTable(nd);

	delete[] nd;
	nd = 0;
	lv_nd = 0;

	return NMD_SUCCEED;
}

int NMD::R_CO_ST_RebuildPtr()
{
	if (lld->mx_r_co_st == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_r_co_st = r_co_st_tb->GetCount();
	if(lv_r_co_st == 0)
	{
		return NMD_SUCCEED;
	}

	r_co_st = new R_CO_ST[lv_r_co_st];
	r_co_st_tb->CopyTable(r_co_st);
	
	//rebuild pointer
	for(i = 0; i < lv_r_co_st; i++)
	{
		if((r_co_st+i)->co_id_ptr != -1)
		{
			(r_co_st+i)->co_id_ptr = co_tb->GetLogByPh((r_co_st+i)->co_id_ptr);
		}
		if((r_co_st+i)->st_id_ptr != -1)
		{
			(r_co_st+i)->st_id_ptr = st_tb->GetLogByPh((r_co_st+i)->st_id_ptr);
		}
		if((r_co_st+i)->fptr != -1)
		{
			(r_co_st+i)->fptr = r_co_st_tb->GetLogByPh((r_co_st+i)->fptr);
		}
		if((r_co_st+i)->bptr != -1)
		{
			(r_co_st+i)->bptr = r_co_st_tb->GetLogByPh((r_co_st+i)->bptr);
		}
	}

	//update table
	r_co_st_tb->UpdateTable(r_co_st);

	delete[] r_co_st;
	r_co_st = 0;
	lv_r_co_st = 0;

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  R_ST_XFMR_RebuildPtr()
int NMD::R_ST_XFMR_RebuildPtr()
{
	if (lld->mx_r_st_xfmr == 0)
		return 1;

	int		i;

	//read table into buffer
	lv_r_st_xfmr = r_st_xfmr_tb->GetCount();
	if(lv_r_st_xfmr == 0)
	{
		return NMD_SUCCEED;
	}

	r_st_xfmr = new R_ST_XFMR[lv_r_st_xfmr];
	r_st_xfmr_tb->CopyTable(r_st_xfmr);
	
	//rebuild pointer
	for(i = 0; i < lv_r_st_xfmr; i++)
	{
		if((r_st_xfmr+i)->st_id_ptr != -1)
		{
			(r_st_xfmr+i)->st_id_ptr = st_tb->GetLogByPh((r_st_xfmr+i)->st_id_ptr);
		}
		if((r_st_xfmr+i)->xfmr_id_ptr != -1)
		{
			(r_st_xfmr+i)->xfmr_id_ptr = xfmr_tb->GetLogByPh((r_st_xfmr+i)->xfmr_id_ptr);
		}
		if((r_st_xfmr+i)->fptr != -1)
		{
			(r_st_xfmr+i)->fptr = r_st_xfmr_tb->GetLogByPh((r_st_xfmr+i)->fptr);
		}
		if((r_st_xfmr+i)->bptr != -1)
		{
			(r_st_xfmr+i)->bptr = r_st_xfmr_tb->GetLogByPh((r_st_xfmr+i)->bptr);
		}
	}

	//update table
	r_st_xfmr_tb->UpdateTable(r_st_xfmr);

	delete[] r_st_xfmr;
	r_st_xfmr = 0;
	lv_r_st_xfmr = 0;

	return NMD_SUCCEED;
}

//追踪谐波
int NMD::traceHW(char* hwSub,vector<int>& secs)
{///////////////////////////////////////////////////////////////////////////////
	char trace_vl[500];
	char str[500];
	int			i, vl;
	int			ind, knd, kknd;
	int			*busnd;
	ChildPtr1	*sec_p, *sec_p0;

	secs.clear();
	//get record count in nd table
	lv_nd = nd_tb->GetCount();
	if (lv_nd == 0)
	{
		printf("there is no record in nd table\n");
		sprintf(str, "节点表中无记录!");
		lld->WriteLog(lld->logfile, str);
		return NMD_SUCCEED;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	lld->trim_return(hwSub);
	//mark all nds as unprocessed
	//将所有的节点设为未处理，准备拓扑分析用
	for (i = 0; i < lv_nd; i++)
	{
		(nd + i)->q |= ND_UNPROC;

		//找到追踪节点
		if (strcmp(hwSub, (nd + i)->id) == 0)
		{
			ind = i;
			strcpy(trace_vl,(nd + i)->ver);
		}
	}

	//search through nds from bus nd
	//深度搜索
	//之前已经将所有的节点都置为未处理
	if ((nd + ind)->q & ND_UNPROC) //this bus nd has not processed
	{
		//create circular chain for this bus nd
		knd = ind;//当前节点赋给一个临时的变量
		(nd + knd)->fch = knd; //当前节点号赋给前向链 
		(nd + knd)->q &= (~ND_UNPROC);//将当前节点设为已处理

		//拿当前节点向下扩展，找到所有和它相连的节点
		while (1)
		{
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

				//process this opposite nd
				if ((nd + kknd)->q & ND_UNPROC)
				{
					if (strcmp((nd + kknd)->ver,trace_vl) == 0)  //non-bus nd
					{
						//输出结果
						secs.push_back(sec_p->Ptr);
					}
					else  //bus nd
					{
						sec_p = sec_p->Next;
						continue;
					}

					//put this nd into circular chain
					(nd + kknd)->fch = (nd + knd)->fch;
					(nd + knd)->fch = kknd;
					(nd + kknd)->q &= (~ND_UNPROC);
				}
				sec_p = sec_p->Next;
			}
			::FreeChildPtr(sec_p0);

			//move to next nd
			knd = (nd + knd)->fch;

			//循环到头，连起来了，表示全部走了一遍
			//拿另外的节点再开始搜索，一直到所有的节点都处理
			if (knd == ind)
			{
				break;
			}
		}
	}

	delete[] nd;
	nd = 0;
	lv_nd = 0;

	return NMD_SUCCEED;
}
















