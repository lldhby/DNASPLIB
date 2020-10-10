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
		sprintf(str,"У���ʼ������!");
		lld->WriteLog(lld->logfile,str);
		return -1;
	}

//��ÿһ���豸����У�飬��Ҫ�ǲ鿴�豸����һ���Ĺ�����ϵ���ڹ�����ϵ����������ֵ
//���Ƿ��й��������û�й�����˵��������
	if(CB_Validate() == NMD_FAIL)
	{
       	sprintf(str,"���ر�У������!");
		lld->WriteLog(lld->logfile,str);
		return -2;
	}

	if(BUS_Validate() == NMD_FAIL)
	{
		sprintf(str,"ĸ�߱�У������!");
		lld->WriteLog(lld->logfile,str);
		return -3;
	}

	if(CP_Validate() == NMD_FAIL)
	{
		sprintf(str,"���ݱ�У������!");
		lld->WriteLog(lld->logfile,str);
		return -4;
	}

	if(LD_Validate() == NMD_FAIL)
	{
		sprintf(str,"���ɱ�У������!");
		lld->WriteLog(lld->logfile,str);
		return -5;
	}

	if(SO_Validate() == NMD_FAIL)
	{
		sprintf(str,"��Դ��У������!");
		lld->WriteLog(lld->logfile,str);
		return -6;
	}

	if(SEC_Validate() == NMD_FAIL)
	{
		sprintf(str,"�߶α�У������!");
		lld->WriteLog(lld->logfile,str);
		return -7;
	}

	//������ѹ�����õ������������߶��Ƿֿ���
	//��ѹ��У�顢����������ڵ�-�����ϵ������-��ѹ����ϵ��
	//if(XFMR_Validate() == NMD_FAIL)
	//{
 //       sprintf(str,"��ѹ����У������!");
	//	lld->WriteLog(lld->logfile,str);
	//	return -8;
	//}
	if (XF_Validate() == NMD_FAIL)
	{
		sprintf(str, "�����У������!");
		lld->WriteLog(lld->logfile, str);
		return -8;
	}

	
	//ֻ�����������������
	if ((lld->MATH_VOLTAGELEVEL != 10)
		&& (XF_Validate() == NMD_FAIL))
	{
        sprintf(str,"�����У������!");
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

	//sprintf(str,"У������");
	//lld->WriteLog(lld->logfile,str);


	if (lld->MATH_VOLTAGELEVEL == 10)
	{
		if(ZN_Validate() == NMD_FAIL)
		{
			sprintf(str,"���α�У������!");
			lld->WriteLog(lld->logfile,str);
			return -10;
		}

	//�����߹�����ϵ��
		if (FD_Validate() == NMD_FAIL)
		{
			sprintf(str, "���߹�����У������!");
			lld->WriteLog(lld->logfile, str);
			return -11;
		}
	}
//
////������ɿ��ԣ���Ҫ�豸֮��Ĺ�����ϵ
//	if(RA_Validate() == NMD_FAIL)
//	{
//		sprintf(str,"�ɿ��Թ�����У������!");
//		lld->addLogLine(str);
//		return NMD_FAIL;
//	}

	if(ValidateEnd() == NMD_FAIL)
	{
		sprintf(str,"У���������!");
		lld->WriteLog(lld->logfile,str);
		return -12;
	}

	return NMD_SUCCEED;
}

int	NMD::TrimTable()
{
	//lv_bs = lv_nd = 2*lld->mx_cb+2*lld->mx_sec+lld->mx_so+1000;//ʵ�����ǰ���Ѿ���������
	if (lv_r_nd_cb > 0)
	{
		//!!!!��ʱ���Բ�����������Ϊ��Ĭ�ϵ�-1������
		lld->mx_r_nd_cb = r_nd_cb_tb_t->Sort(lv_r_nd_cb,0);
		//if (r_nd_cb_tb != NULL)//��ɾ����ǰ��
		//	delete r_nd_cb_tb;
		r_nd_cb_tb = new DTable<R_ND_CB>(lld->mx_r_nd_cb);
		if(lld->mx_r_nd_cb != 0)
		{
			R_ND_CB* temp = new R_ND_CB[lld->mx_r_nd_cb];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			r_nd_cb_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			r_nd_cb_tb->Insert(temp,lld->mx_r_nd_cb);
			dsbob->r_nd_cb_tb = r_nd_cb_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete r_nd_cb_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		r_nd_cb_tb_t=0;		
	}

	if (lv_r_nd_sec > 0)
	{
		lld->mx_r_nd_sec = r_nd_sec_tb_t->Sort(lv_r_nd_sec,0);
		//if (r_nd_sec_tb != NULL)//��ɾ����ǰ��
		//	delete r_nd_sec_tb;

		r_nd_sec_tb = new DTable<R_ND_SEC>(lld->mx_r_nd_sec);
		if(lld->mx_r_nd_sec != 0)
		{
			R_ND_SEC* temp = new R_ND_SEC[lld->mx_r_nd_sec];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			r_nd_sec_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			r_nd_sec_tb->Insert(temp,lld->mx_r_nd_sec);
			dsbob->r_nd_sec_tb = r_nd_sec_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete r_nd_sec_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		r_nd_sec_tb_t=0;		
	}

	if (lv_xf > 0)//��xfmr������
	{
		lld->mx_xf = xf_tb_t->Sort(lv_xf, 0);
		//if (xf_tb != NULL)//��ɾ����ǰ��
		//	delete xf_tb;

		xf_tb = new DTable<XF>(lld->mx_xf);
		if (lld->mx_xf != 0)
		{
			XF* temp = new XF[lld->mx_xf];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			xf_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			xf_tb->Insert(temp, lld->mx_xf);
			dsbob->xf_tb = xf_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete xf_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		xf_tb_t = 0;
	}

	if (lv_r_xfmr_xf > 0)
	{
		lld->mx_r_xfmr_xf = r_xfmr_xf_tb_t->Sort(lv_r_xfmr_xf, 0);
		//if (r_xfmr_xf_tb != NULL)//��ɾ����ǰ��
		//	delete r_xfmr_xf_tb;

		r_xfmr_xf_tb = new DTable<R_XFMR_XF>(lld->mx_r_xfmr_xf);
		if (lld->mx_r_xfmr_xf != 0)
		{
			R_XFMR_XF* temp = new R_XFMR_XF[lld->mx_r_xfmr_xf];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			r_xfmr_xf_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			r_xfmr_xf_tb->Insert(temp, lld->mx_r_xfmr_xf);
			dsbob->r_xfmr_xf_tb = r_xfmr_xf_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete r_xfmr_xf_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		r_xfmr_xf_tb_t = 0;
	}

	if (lv_r_nd_xf > 0)
	{
		lld->mx_r_nd_xf = r_nd_xf_tb_t->Sort(lv_r_nd_xf, 0);
		//if (r_nd_xf_tb != NULL)//��ɾ����ǰ��
		//	delete r_nd_xf_tb;

		r_nd_xf_tb = new DTable<R_ND_XF>(lld->mx_r_nd_xf);
		if (lld->mx_r_nd_xf != 0)
		{
			R_ND_XF* temp = new R_ND_XF[lld->mx_r_nd_xf];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			r_nd_xf_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			r_nd_xf_tb->Insert(temp, lld->mx_r_nd_xf);
			dsbob->r_nd_xf_tb = r_nd_xf_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete r_nd_xf_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		r_nd_xf_tb_t = 0;
	}
	
	if (lv_r_nd_cp > 0)
	{
		lld->mx_r_nd_cp = r_nd_cp_tb_t->Sort(lv_r_nd_cp, 0);
		//if (r_nd_cp_tb != NULL)//��ɾ����ǰ��
		//	delete r_nd_cp_tb;

		r_nd_cp_tb = new DTable<R_ND_CP>(lld->mx_r_nd_cp);
		if (lld->mx_r_nd_cp != 0)
		{
			R_ND_CP* temp = new R_ND_CP[lld->mx_r_nd_cp];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			r_nd_cp_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			r_nd_cp_tb->Insert(temp, lld->mx_r_nd_cp);
			dsbob->r_nd_cp_tb = r_nd_cp_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete r_nd_cp_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		r_nd_cp_tb_t = 0;
	}

	if (lv_r_nd_ld > 0)
	{
		lld->mx_r_nd_ld = r_nd_ld_tb_t->Sort(lv_r_nd_ld,0);
		//if (r_nd_ld_tb != NULL)//��ɾ����ǰ��
		//	delete r_nd_ld_tb;

		r_nd_ld_tb = new DTable<R_ND_LD>(lld->mx_r_nd_ld);
		if(lld->mx_r_nd_ld != 0)
		{
			R_ND_LD* temp = new R_ND_LD[lld->mx_r_nd_ld];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			r_nd_ld_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

			//update table����ʱ������ļ�¼��д����ʽ������
			r_nd_ld_tb->Insert(temp,lld->mx_r_nd_ld);
			dsbob->r_nd_ld_tb = r_nd_ld_tb;

			delete[] temp;//����ʱ���ڴ��ͷ�
			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete r_nd_ld_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
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
	global_tb->Empty();//�Ƚ�ȫ�ֱ���գ�Ȼ��׼��д��ȫ��ϵͳ������

	//ֻҪ����t�����Ǹ�dsbob���ģ���Ϊֱ�Ӵ�������������
	//ɾ��ָ���ʱ�򣬲��ܿ�ʵ�����ݴ�С��Ҫ�����ڴ�ʱ��Ĵ�С
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
	
	lv_r_fd_un = 2*lld->mx_un;//ÿ�����翪�ض�Ӧ��������
	
	lv_r_fd_so = 2 * lld->mx_so;//һ���������1:1
	lv_r_fd_zn =  500*lld->mx_fd;
	

	if (lld->mx_xfmr>0)
	{
		lv_xf = 100+lld->mx_xfmr;//�����������������ѹ��
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

	//reset pointers in tables����������֮��Ĺ�ϵ����Ҫ���������������໥��ϵ���ֶε�
	//��ÿ����������໥��ϵ���ֶ�ȫ�����-1��ʵ�����Ƕ�ÿ������г�ʼ��
	//�Ƚ������ݿ���������ڴ��ȡ���������ֶ��ó�-1��Ȼ���ٻ�д��ȥ���ﵽ�޸��ڴ���Ŀ��
	ResetPtr();

	return NMD_SUCCEED;
}

////////////////////////////////////////////////////////////////////////////////
//  ValidateEnd()
int	NMD::ValidateEnd()
{
	//	char str[200];

		//����������ʱ��ȫ��ɾ��
	if (lld->MATH_VOLTAGELEVEL == 10)
	{
		if (lv_eq > 0)
		{
			lld->mx_eq = eq_tb_t->Sort(lv_eq, 0);
			eq_tb = new DTable<EQ>(lld->mx_eq);
			if (lld->mx_eq != 0)
			{
				EQ* temp = new EQ[lld->mx_eq];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				eq_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				eq_tb->Insert(temp, lld->mx_eq);
				dsbob->eq_tb = eq_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete eq_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			eq_tb_t = 0;
		}

		if (lv_r_nd_eq > 0)
		{
			lld->mx_r_nd_eq = r_nd_eq_tb_t->Sort(lv_r_nd_eq, 0);
			r_nd_eq_tb = new DTable<R_ND_EQ>(lld->mx_r_nd_eq);
			if (lld->mx_r_nd_eq != 0)
			{
				R_ND_EQ* temp = new R_ND_EQ[lld->mx_r_nd_eq];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_nd_eq_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_nd_eq_tb->Insert(temp, lld->mx_r_nd_eq);
				dsbob->r_nd_eq_tb = r_nd_eq_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_nd_eq_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			r_nd_eq_tb_t = 0;
		}

		//	if (lv_bs > 0)
		//	{
		//		lld->mx_bs = bs_tb_t->Sort(lv_bs);
		//		bs_tb = new DTable<BS>(lld->mx_bs);
		//		if(lld->mx_bs != 0)
		//		{
		//			BS* temp = new BS[lld->mx_bs];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
		//			bs_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��
		//
		//			//update table����ʱ������ļ�¼��д����ʽ������
		//			bs_tb->Insert(temp,lld->mx_bs);
		//
		//			delete[] temp;//����ʱ���ڴ��ͷ�
		//			temp = 0;//�����������ֹ�ͷź�ָ����ָ
		//		}
		//		delete bs_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		//		bs_tb_t=0;		
		//	}

		if (lv_zn > 0)
		{
			lld->mx_zn = zn_tb_t->Sort(lv_zn, 0);
			zn_tb = new DTable<ZN>(lld->mx_zn);
			if (lld->mx_zn != 0)
			{
				ZN* temp = new ZN[lld->mx_zn];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				zn_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				zn_tb->Insert(temp, lld->mx_zn);
				dsbob->zn_tb = zn_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete zn_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			zn_tb_t = 0;
		}

		if (lv_r_zn_cb > 0)
		{
			lld->mx_r_zn_cb = r_zn_cb_tb_t->Sort(lv_r_zn_cb, 0);
			r_zn_cb_tb = new DTable<R_ZN_CB>(lld->mx_r_zn_cb);
			if (lld->mx_r_zn_cb != 0)
			{
				R_ZN_CB* temp = new R_ZN_CB[lld->mx_r_zn_cb];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_zn_cb_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_zn_cb_tb->Insert(temp, lld->mx_r_zn_cb);
				dsbob->r_zn_cb_tb = r_zn_cb_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_zn_cb_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			r_zn_cb_tb_t = 0;
		}

		if (lv_r_zn_sec > 0)
		{
			lld->mx_r_zn_sec = r_zn_sec_tb_t->Sort(lv_r_zn_sec, 0);
			r_zn_sec_tb = new DTable<R_ZN_SEC>(lld->mx_r_zn_sec);
			if (lld->mx_r_zn_sec != 0)
			{
				R_ZN_SEC* temp = new R_ZN_SEC[lld->mx_r_zn_sec];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_zn_sec_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_zn_sec_tb->Insert(temp, lld->mx_r_zn_sec);
				dsbob->r_zn_sec_tb = r_zn_sec_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_zn_sec_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			r_zn_sec_tb_t = 0;
		}

		if (lv_r_zn_cp > 0)
		{
			lld->mx_r_zn_cp = r_zn_cp_tb_t->Sort(lv_r_zn_cp, 0);
			r_zn_cp_tb = new DTable<R_ZN_CP>(lld->mx_r_zn_cp);
			if (lld->mx_r_zn_cp != 0)
			{
				R_ZN_CP* temp = new R_ZN_CP[lld->mx_r_zn_cp];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_zn_cp_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_zn_cp_tb->Insert(temp, lld->mx_r_zn_cp);
				dsbob->r_zn_cp_tb = r_zn_cp_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_zn_cp_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			r_zn_cp_tb_t = 0;
		}

		if (lv_r_zn_ld > 0)
		{
			lld->mx_r_zn_ld = r_zn_ld_tb_t->Sort(lv_r_zn_ld, 0);
			r_zn_ld_tb = new DTable<R_ZN_LD>(lld->mx_r_zn_ld);
			if (lld->mx_r_zn_ld != 0)
			{
				R_ZN_LD* temp = new R_ZN_LD[lld->mx_r_zn_ld];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_zn_ld_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_zn_ld_tb->Insert(temp, lld->mx_r_zn_ld);
				dsbob->r_zn_ld_tb = r_zn_ld_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_zn_ld_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			r_zn_ld_tb_t = 0;
		}

		if (lv_r_fd_so > 0)
		{
			lld->mx_r_fd_so = r_fd_so_tb_t->Sort(lv_r_fd_so, 0);
			r_fd_so_tb = new DTable<R_FD_SO>(lld->mx_r_fd_so);
			if (lld->mx_r_fd_so != 0)
			{
				R_FD_SO* temp = new R_FD_SO[lld->mx_r_fd_so];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_fd_so_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_fd_so_tb->Insert(temp, lld->mx_r_fd_so);
				dsbob->r_fd_so_tb = r_fd_so_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_fd_so_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
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
				R_FD_UN* temp = new R_FD_UN[lld->mx_r_fd_un];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_fd_un_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_fd_un_tb->Insert(temp, lld->mx_r_fd_un);
				dsbob->r_fd_un_tb = r_fd_un_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_fd_un_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			r_fd_un_tb_t = 0;
		}

		if (lv_r_fd_eq > 0)
		{
			lld->mx_r_fd_eq = r_fd_eq_tb_t->Sort(lv_r_fd_eq, 0);
			r_fd_eq_tb = new DTable<R_FD_EQ>(lld->mx_r_fd_eq);
			if (lld->mx_r_fd_eq != 0)
			{
				R_FD_EQ* temp = new R_FD_EQ[lld->mx_r_fd_eq];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_fd_eq_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_fd_eq_tb->Insert(temp, lld->mx_r_fd_eq);
				dsbob->r_fd_eq_tb = r_fd_eq_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_fd_eq_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			r_fd_eq_tb_t = 0;
		}

		if (lv_r_fd_ld > 0)
		{
			lld->mx_r_fd_ld = r_fd_ld_tb_t->Sort(lv_r_fd_ld, 0);
			r_fd_ld_tb = new DTable<R_FD_LD>(lld->mx_r_fd_ld);
			if (lld->mx_r_fd_ld != 0)
			{
				R_FD_LD* temp = new R_FD_LD[lld->mx_r_fd_ld];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_fd_ld_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_fd_ld_tb->Insert(temp, lld->mx_r_fd_ld);
				dsbob->r_fd_ld_tb = r_fd_ld_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
		}

		if (lv_r_fd_zn > 0)
		{
			lld->mx_r_fd_zn = r_fd_zn_tb_t->Sort(lv_r_fd_zn, 0);
			r_fd_zn_tb = new DTable<R_FD_ZN>(lld->mx_r_fd_zn);
			if (lld->mx_r_fd_zn != 0)
			{
				R_FD_ZN* temp = new R_FD_ZN[lld->mx_r_fd_zn];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
				r_fd_zn_tb_t->CopyTable(temp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

				//update table����ʱ������ļ�¼��д����ʽ������
				r_fd_zn_tb->Insert(temp, lld->mx_r_fd_zn);
				dsbob->r_fd_zn_tb = r_fd_zn_tb;

				delete[] temp;//����ʱ���ڴ��ͷ�
				temp = 0;//�����������ֹ�ͷź�ָ����ָ
			}
			delete r_fd_zn_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
			r_fd_zn_tb_t = 0;
		}
		//��Ҫ�ڽ�������֮ǰ���������ţ�����ڵ��(�ڵ�����������ʱ���������򣬷��߼���id����)����������
		RebuildPtr();

		//	CompressTable();
		//	if (nmd_type == 1)
		//		DIGITAL_ANALOG_PtrBuild();

			//set vl of nds
			//��дÿһ��ĸ�ߵĽڵ��ѹֵ,�����Ϊ����������׼����
			//��Ŀǰ�ĸ��ڵ�ĸ�߿�ʼ׷�٣�����ǿ��ػ������߶Σ�����ĸ�ߵĵ�ѹ������չ���Ľڵ�
			//����������µ�ĸ�ߣ���ôҪ���µ�ĸ�ߵĽڵ��ѹ��ڵ�����	

			//lld for temp*************2011-11-11
			//���������Ҫ���趨ÿһ���ڵ�ĵ�ѹֵ����Ϊ��������10kV���������������̻�����������
		SetVL(1);
	}

	return NMD_SUCCEED;
}

//����Ҫ�����£�
//1.�Զ���������������������-�߶�r_zn_sec������-����r_zn_ld������-����r_zn_cb�Ĺ�ϵ
//2.���豸���豸-�ڵ��ϵr_nd_eq
//3.��������-��Դr_fd_so������-����r_fd_un������-����r_fd_zn������-����r_fd_ld������-�豸r_fd_eq�Ĺ�ϵ
int	NMD::ZN_Validate()
{
	char		str[500];
	int			i,jj;
	int			sum_lds=0;//�ܵĸ��ɸ�������ֹ����
	int			ind, knd, kknd, kkndcb,ext_cbnd,cnt_ld;
	int			fdph;
	int			sond;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;
	ChildPtr1	*cp_p, *cp_p0;
	ChildPtr1	*xf_p, *xf_p0;
	//******��Ҫ����չ���غ����α߽翪�����ֿ�******//
	int extndcount/*��չ�ڵ�����*/,extnd[100]/*��չ���صĽڵ�*/, \
		cbndcnt/*���α߽翪����*/, cb_zn_znd[100]/*��չ���ص��ڲ�ڵ�*/,cb_zn_intnd[100], \
		cb_zn_nd[100]/*�߽翪�ص����ڵ�*/,zncnt/*���α��*/;//һ�����ο��ܻ���100���߽�ڵ�

	int extcbcnt;//�����ڵĿ�������
	int extcbptr[100];//��������չ�ı߽翪�ص�ָ�����飬ͨ������������õ��߽翪��
	int inner_nd,out_nd;//���ε��׿��ص�����ڵ㣬������Ϊ���ģ����⡱ָ�����⣬�����εĿ�����ڵ�
	int firstcb;//��Դ����ֿ���

	char zn_id[16];
	double so_sumLoad,zn_sumLoad_p;//�����ڵ��ܸ�������,�й�һ�㰴0.15�����޹�
	
	double so_ltime;//���ߵĶ�λ����ʱ�䣬lld add 2016-1-27

	char cb_id[80],sec_id[80],ld_id[80], cp_id[80], xf_id[80],fdid[80],eq_nd[80],eq_znd[80],so_nd[64];
	int unionCBcnt;//���翪������
	int unionCB[100];//�洢���翪�صĽڵ��
	int zn_p/*���ε�����λ��*/, icb/*���ص��߼�λ��*/;

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
		sprintf(str,"�ڵ�����޼�¼!");
		lld->WriteLog(lld->logfile,str);
		return NMD_SUCCEED;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	//mark all nds as unprocessed
	//�����еĽڵ���Ϊδ����׼�����˷�����
	for(i = 0; i < lv_nd; i++)
	{
		(nd+i)->q |= ND_UNPROC;
		//(nd+i)->bch = -1;//���ڵ�ĺ�������Ϊ-1
	}

	//get record count in so table
	lv_so = so_tb->GetCount();
	if(lv_so == 0)
	{
		//printf("there is no record in lv_so table\n");
		sprintf(str,"��Դ�����޼�¼!");
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
	//sond = new int[lv_so];//���͵�ָ�룬�൱��һ�����������飬��¼һ����ַ
	//islandso = new int[lv_so];//���͵�ָ�룬�൱��һ�����������飬��¼һ����ַ
	//for(i = 0; i < lv_so; i++)
	//{
	//	//��ĸ�߱�so_tb�Ľڵ���ind����˳��д��sondָ����������

	//	//ֻ��ƽ��ڵ������
	//	if (so_tb->GetDataByLog(i)->slackavr == 0)
	//	{
	//		*(sond + islandso) = so_tb->GetDataByLog(i)->ind;
	//		islandso++;
	//	}
	//}

	//r_fd_nd = r_fd_nd_tb->GetTable();//�ڵ��ϵ��

	//zn_tb_t = new DTable<ZN>(lld->mx_zn);

	ZN* znt = new ZN();
	zncnt = 1;//���μ����ۼƵ����θ�����z999999�������������ʾÿһ������
	int znsum = 0;//���μ������������������
	so_ltime = 0;


	for(i = 0; i < lv_so; i++)
	{
		//if (i == 60)
		//{
		//	int debug = 0;
		//	continue;
		//}
		if (so_tb->GetDataByLog(i)->slackavr != 4)//ֻ�����ڵ������
			continue;

		strcpy(so_nd, so_tb->GetDataByLog(i)->nd);
		ND* ndt = new ND(so_nd);
		so_tb->GetDataByLog(i)->ind = sond = nd_tb->FindPh(*ndt);
		delete ndt;
	
		//so_ltime = so_tb->GetDataByLog(i)->fault_ltime;
		strcpy(fdid,so_tb->GetDataByLog(i)->fdid);//��Դ����������
		fdph = dsbob->OnGetPhByfdid(fdid);//fd_tb->FindPh(FD(fdid));//�����߱��е�λ��
		so_ltime = fd_tb->GetDataByPh(fdph)->fault_ltime;
		//��Ҫ���⴦���Դ���Σ�Ҳ���ǵ�Դ�㵽���ֿ��ص��Ǹ�����
		firstcb = 0;
		ext_cbnd = -1;//��Դ������û�н��뿪��
		cb_zn_intnd[0] = -1;

		so_sumLoad = 0;
		unionCBcnt = 0;
		extndcount = 0;
		ind = sond;//ĸ�ߵ���ʼ�ڵ���
		fd_tb->GetDataByPh(fdph)->cnt_nd++;//�����Ͻڵ�ĸ�������
		fd_tb->GetDataByPh(fdph)->cnt_so++;//�����ϵ�Դ�ĸ�������

		(nd+ind)->bch = ind;//�ӵ�Դ�㿪ʼ��ʼ����,����Դ����Ϊ��һ����¼����
		
		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
		if((nd+ind)->q & ND_UNPROC) //this so nd has not processed
		{
			(nd+ind)->q &= (~ND_UNPROC);
			//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8

			//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
			cb_zn_znd[extndcount] = ind;
			extnd[extndcount++] = ind;
			
			while( extndcount >= 1 )//���ѭ�����һ�����ε�����
			{
				ind = extnd[--extndcount];//ÿ�ζ�ȡ�����Ǹ��ڵ������չ
				ext_cbnd = cb_zn_intnd[extndcount];//���ν���Ľڵ����һ�����صĽڵ�ţ������ж����εĽ��뿪��

				knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
				(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
				(nd+knd)->q &= (~ND_UNPROC);//����ǰ�ڵ���Ϊ�Ѵ���

				//�ӿ�ʼ��չ�Ľڵ㿪ʼ���γ�һ������ID����������
				sprintf(zn_id,"z%06d",zncnt++);
				zn_p = znt->id = zn_tb_t->GetNext();

				cnt_ld = 0;//�����ڵĸ��ɸ���
				zn_sumLoad_p = 0.0;//�����ڵ��ܸ��ɴ�С_�й�
				//zn_sumLoad_q = 0.0;//�����ڵ��ܸ��ɴ�С_�޹�
				cbndcnt = 0;//�����ڵĽڵ���
				
				inner_nd = out_nd = -1;
				extcbcnt = 0;//�����ڵĿ�����

				while (1)//������չ����û�д���
				{
					if ((nd+knd)->q & ND_SO)//�ڵ��Ϲ��зֲ�ʽ��Դ�� 
					{
						fd_tb->GetDataByPh(fdph)->cnt_so++;//�����ϵ�Դ�ĸ�������
					}

					if (strcmp((nd + knd)->id, "129_30500000_1765366") == 0)
					{
						int lld = 0;
					}


					//�����ɵ�id�Ÿ�д
					//ͨ���ڵ��q�����жϳ��ǲ��Ǹ��ɽڵ㣬q |= 8
					if ((nd+knd)->q & ND_LD) 
					{
						//�õ�����id
						ld_p = r_nd_ld_tb->FindChildPtr1((nd+knd)->r_ld);
						ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
						while(ld_p)
						{
							strcpy(ld_id, ld_tb->GetDataByPh(ld_p->Ptr)->id);

							if (ND_BR_Insert(_R_ZN_LD,zn_p, zn_id, ld_p->Ptr, ld_id,znt->fdid,znt->ver) == NMD_FAIL )
							{
			//					sprintf(str,"����(%s)�����κ͸��ɹ�ϵ����ʧ��!",ld_id);
			//lld->WriteLog(lld->logfile,str);
								ld_p = ld_p->Next;
								return NMD_FAIL;
							}
							//��������-���ɹ�ϵ��
							if(ND_BR_Insert(_R_FD_LD,fdph, fdid, ld_p->Ptr, ld_id,znt->fdid,znt->ver)==NMD_FAIL)
							{
			//					sprintf(str,"����(%s)�����ߺ͸��ɹ�ϵ����ʧ��!",ld_id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							
							//�豸�������
							eq->Reset();
							eq->i_id = eq_cnt;
							eq->idph = ld_p->Ptr;
							strcpy(eq->id , ld_tb->GetDataByPh(ld_p->Ptr)->id);
							strcpy(eq->fdid , ld_tb->GetDataByPh(ld_p->Ptr)->fdid);
							eq->ind = ld_tb->GetDataByPh(ld_p->Ptr)->ind;
							eq->iznd = -1;
							eq->prob_fault = ld_tb->GetDataByPh(ld_p->Ptr)->prob_fault;
							
							//�豸���޸�ʱ��
							eq->repairTime = ld_tb->GetDataByPh(ld_p->Ptr)->repairTime;

							eq->type = 20;
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;//�������豸�ĸ�������
							fd_tb->GetDataByPh(fdph)->cnt_ld++;//�����ϸ��ɵĸ�������
							if( Insert_r_char_int(2,eq_cnt,fdph,fdid) == NMD_FAIL )
							{
			//					//printf("fail to insert r_fd_eq (%d)\n", i);
			//					sprintf(str,"�豸(%s)���豸�����߹�ϵ����ʧ��!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							if( Insert_r_char_int(0,eq_cnt++,eq->ind,ld_tb->GetDataByPh(ld_p->Ptr)->nd) == NMD_FAIL )
							{
			//					sprintf(str,"����(%s)���豸��ϵ����ʧ��!",(eq->id));
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}

							cnt_ld++;//�����ڵĸ��ɸ�������1
							sum_lds++;//�ܵĸ��ɸ���Ҳ����
							//�����ڵ����й������޹�
							zn_sumLoad_p += ld_tb->GetDataByPh(ld_p->Ptr)->wm;
							//zn_sumLoad_q += ld_tb->GetDataByPh(ld_p->Ptr)->rm;
							so_sumLoad += ld_tb->GetDataByPh(ld_p->Ptr)->wm;
							ld_p = ld_p->Next;
						}
						::FreeChildPtr(ld_p0);
					}

					//read cbs connected to this nd
					//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
					cb_p = r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
					cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
					while(cb_p)
					{
						//find the opposite nd through this cb
						//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
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
						//��������Ƿֵ�״̬������Ҫ���ڵ���뵽��չ�ڵ㣬Ҳ���üӶԲ�ڵ���л���
						if  (cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//���翪�ص�qΪ1������
							//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0) 
						{
							//(nd + kkndcb)->q &= (~ND_UNPROC);

							//���翪�صĽڵ㱣�棬׼�����ݻ�úϲ�����
							unionCB[unionCBcnt++] = knd;

							//���ӿ��أ�
							eq->Reset();
							eq->i_id = eq_cnt;
							eq->idph = cb_p->Ptr;
							strcpy(eq->id , cb_tb->GetDataByPh(cb_p->Ptr)->id);
							strcpy(eq->fdid , cb_tb->GetDataByPh(cb_p->Ptr)->fdid);
							eq->ind = knd;
							eq->iznd = kkndcb;//���صĽڵ㰴��Դ���˳������
							eq->type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;
							fd_tb->GetDataByPh(fdph)->cnt_un++;//���������翪�صĸ�������
							if( Insert_r_char_int(2,eq_cnt,fdph,fdid) == NMD_FAIL )
							{
			//					sprintf(str,"����(%s)�����ߺ����翪�ع�ϵ����ʧ��!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							if( Insert_r_char_int(0,eq_cnt,eq->ind,eq_nd) == NMD_FAIL ||
								Insert_r_char_int(0,eq_cnt++,eq->iznd,eq_znd) == NMD_FAIL )
							{
			//					sprintf(str,"����(%s)�Ľڵ���豸��ϵ����ʧ��!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								cb_p = cb_p->Next;
								//continue;
								return NMD_FAIL;
							}							
						}
						
						//process this opposite nd
						//����Բ�ڵ� �Ѵ���==���ز����������ƽ�
						if ( (kkndcb!=-1) && ((nd+kkndcb)->q & ND_UNPROC) )
						{
							fd_tb->GetDataByPh(fdph)->cnt_nd++;//���������翪�صĸ�������

							//����������׼���������õ�
							cb_zn_znd[cbndcnt] = knd;//׼����չ�Ľڵ㿪���ڲ�ڵ�
							cb_zn_nd[cbndcnt++] = kkndcb;//�濪�����Ľڵ㣬׼��д�����α�

							//����������׼���������õ�
							cb_tb->GetDataByPh(cb_p->Ptr)->ind = knd; //�����������׽ڵ�
							cb_tb->GetDataByPh(cb_p->Ptr)->iznd = kkndcb;// ������ĩ�ڵ�
							extcbptr[extcbcnt++] = cb_p->Ptr;
							
							//��������Ƿֵ�״̬������Ҫ���ڵ���뵽��չ�ڵ㣬Ҳ���üӶԲ�ڵ���л���
							if (cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)//��λ�����翪��
							{
								int debug = 0;
							}
							else//�պϿ��أ�׼��������չ
							{		
								//ֻҪ�����ҵ��Ľڵ㶼Ҫ����ϵ
//								if (ND_BR_Insert(_R_FD_ND,ph, fdid, kkndcb, (nd+kkndcb)->id,fdid,"")==NMD_FAIL)
//									return NMD_FAIL;
//								fd_tb->GetDataByPh(ph)->cnt_nd++;//���������翪�صĸ�������
								
								//�����صĶԲ�ڵ������׼����һ��������չ��
								cb_zn_intnd[extndcount] = knd;//׼����չ�Ľڵ㿪���ڲ�ڵ�
								extnd[extndcount++] = kkndcb;//׼����չ�Ľڵ�
							
								//������ڵ����һ����¼������׼������
								(nd+kkndcb)->bch = knd;

								//���ҵ����½ڵ�����Ѵ���
								(nd+kkndcb)->q &= (~ND_UNPROC);
							
							//���ӿ��أ�
							eq->Reset();
							eq->i_id = eq_cnt;
							eq->idph = cb_p->Ptr;
							strcpy(eq->id , cb_tb->GetDataByPh(cb_p->Ptr)->id);
							strcpy(eq->fdid , cb_tb->GetDataByPh(cb_p->Ptr)->fdid);
							eq->ind = knd;
							eq->iznd = kkndcb;//���صĽڵ㰴��Դ���˳������
							eq->prob_fault = cb_tb->GetDataByPh(cb_p->Ptr)->prob_fault;
							eq->repairTime = cb_tb->GetDataByPh(cb_p->Ptr)->repairTime;
							eq->type = cb_tb->GetDataByPh(cb_p->Ptr)->cbtype;
							if ((sond == eq->ind) || (sond == eq->iznd))//���߿���
							{
								eq->type |= 16;
							}
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;//���������翪�صĸ�������
							if( Insert_r_char_int(2,eq_cnt,fdph,fdid) == NMD_FAIL )
							{
								//printf("fail to insert r_fd_eq (%d)\n", i);
			//					sprintf(str,"�豸(%s)�����ߺ��豸��ϵ����ʧ��!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							if( Insert_r_char_int(0,eq_cnt,eq->ind,eq_nd) == NMD_FAIL ||
								Insert_r_char_int(0,eq_cnt++,eq->iznd,eq_znd) == NMD_FAIL )
							{
								//printf("fail to insert r_nd_cb for cb (%d)\n", i);
			//					sprintf(str,"�豸(%s)�Ľڵ���豸��ϵ����ʧ��!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
								}
							}
						}
				//�����ν�����Ǹ����ر���������Ϊn-1У��ʹ�ã�һ������ֻ�ܳ���һ��
						if(kkndcb == ext_cbnd)//���ν�����Ǹ��ڵ�,���ε��׽ڵ�
						{
							cb_zn_znd[cbndcnt] = knd;//׼����չ�Ľڵ㿪���ڲ�ڵ�
							cb_zn_nd[cbndcnt++] = kkndcb;//�濪�����Ľڵ㣬׼��д�����α�
						
							cb_tb->GetDataByPh(cb_p->Ptr)->ind = kkndcb; //�����������׽ڵ�
							cb_tb->GetDataByPh(cb_p->Ptr)->iznd = knd;// ������ĩ�ڵ�

							extcbptr[extcbcnt++] = cb_p->Ptr;
							znt->cbptr = cb_p->Ptr;

							//�����صĶԲ�ڵ������׼����һ��������չ��
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
			//					sprintf(str,"�豸(%s)�����κ��߶ι�ϵ����ʧ��!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
//							if (ND_BR_Insert(_R_FD_ND,ph, fdid, kknd, (nd+kknd)->id,fdid,"")==NMD_FAIL)
//								return NMD_FAIL;
//							fd_tb->GetDataByPh(ph)->cnt_nd++;//���������翪�صĸ�������

							(nd+kknd)->fch = (nd+knd)->fch;
							//�½ڵ� ��� ��չ�ڵ��ǰ����
							(nd+knd)->fch = kknd;

							//������ڵ����һ����¼������׼������
							(nd+kknd)->bch = knd;

							//���ҵ����½ڵ�����Ѵ���
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

							//��Ҫ�����ʣ�ÿ���߶εļ�������Ҫ�ܵļ�����*�߶γ���
							l = dsbob->OnGetPhByfdid(eq->fdid);//fd_tb->FindPh(FD(eq->fdid));
							eq->prob_check = sec_tb->GetDataByPh(sec_p->Ptr)->len * fd_tb->GetDataByPh(l)->plan_prob;

							eq->r_zn = sec_tb->GetDataByPh(sec_p->Ptr)->r_zn;
							eq->type = 10;
							sec_tb->GetDataByPh(sec_p->Ptr)->lC_zn =eq_cnt;//���߶ζ�Ӧ���豸i_id��
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;//���������翪�صĸ�������
							if( Insert_r_char_int(2,eq_cnt,fdph,fdid) == NMD_FAIL )
							{
								//printf("fail to insert r_fd_eq (%d)\n", i);
			//					sprintf(str,"�豸(%s)�����ߺ��豸��ϵ����ʧ��!",eq->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
							if( Insert_r_char_int(0,eq_cnt,eq->ind,eq_nd) == NMD_FAIL ||
								Insert_r_char_int(0,eq_cnt++,eq->iznd,eq_znd) == NMD_FAIL )
							{
								//printf("fail to insert r_nd_sec for sec (%d)\n", i);
			//					sprintf(str,"�豸(%s)�Ľڵ���豸��ϵ����ʧ��!",eq->id);
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
								//sprintf(str, "�豸(%s)�����κ��߶ι�ϵ����ʧ��!", eq->id);
								//lld->WriteLog(lld->logfile, str);
								return NMD_FAIL;
							}
							//							if (ND_BR_Insert(_R_FD_ND,ph, fdid, kknd, (nd+kknd)->id,fdid,"")==NMD_FAIL)
							//								return NMD_FAIL;
							//							fd_tb->GetDataByPh(ph)->cnt_nd++;//���������翪�صĸ�������

							(nd + kknd)->fch = (nd + knd)->fch;
							//�½ڵ� ��� ��չ�ڵ��ǰ����
							(nd + knd)->fch = kknd;

							//������ڵ����һ����¼������׼������
							(nd + kknd)->bch = knd;

							//���ҵ����½ڵ�����Ѵ���
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

							//��Ҫ�����ʣ�ÿ���߶εļ�������Ҫ�ܵļ�����*�߶γ���
							l = dsbob->OnGetPhByfdid(eq->fdid);//fd_tb->FindPh(FD(eq->fdid));
							//eq->prob_check = xf_tb->GetDataByPh(xf_p->Ptr)->len * fd_tb->GetDataByPh(l)->plan_prob;

							eq->r_zn = xf_tb->GetDataByPh(xf_p->Ptr)->r_zn;
							eq->type = 10;
							//xf_tb->GetDataByPh(xf_p->Ptr)->lC_zn = eq_cnt;//���߶ζ�Ӧ���豸i_id��
							eq_tb_t->Append(*eq);
							fd_tb->GetDataByPh(fdph)->cnt_eq++;//���������翪�صĸ�������
							if (Insert_r_char_int(2, eq_cnt, fdph, fdid) == NMD_FAIL)
							{
								//printf("fail to insert r_fd_eq (%d)\n", i);
								//sprintf(str, "�豸(%s)���������豸��ϵ����ʧ��!", eq->id);
								//lld->WriteLog(lld->logfile, str);
								return NMD_FAIL;
							}
							if (Insert_r_char_int(0, eq_cnt, eq->ind, eq_nd) == NMD_FAIL ||
								Insert_r_char_int(0, eq_cnt++, eq->iznd, eq_znd) == NMD_FAIL)
							{
								//printf("fail to insert r_nd_xf for xf (%d)\n", i);
								//sprintf(str, "�豸(%s)�Ľڵ���豸��ϵ����ʧ��!", eq->id);
								//lld->WriteLog(lld->logfile, str);
								return NMD_FAIL;
							}
						}
						xf_p = xf_p->Next;
					}
					::FreeChildPtr(xf_p0);

					//move to next nd��������һ��
					knd = (nd+knd)->fch;
					//ѭ����ͷ���������ˣ���ʾȫ������һ��
					//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
					if(knd == ind)
					{
						//�����εĶ˵�ȡ����д�����α�����
						strcpy(znt->descr, zn_id);
                        strcpy(znt->fdid,so_tb->GetDataByPh(i)->fdid);//������������
                        strcpy(znt->ver,so_tb->GetDataByPh(i)->ver);//������������

						znt->type = cbndcnt;//���ٻ���һ��
						znt->inner_C = cnt_ld;//�����ڵĸ��ɸ���
						znt->inner_P = zn_sumLoad_p;//�����ڵ����й�
//						strcpy(znt->alg4q , lld->DoubleToString(zn_sumLoad_q));//�����ڵĸ��ɸ���

						//zn_cb
						for (int ii=0; ii<cbndcnt;ii++)
						{
							icb = extcbptr[ii];
							strcpy(cb_id,cb_tb->GetDataByPh(icb)->id);												////ZN_CB_Insert(znt, znt->id, icb, cb_id);
							//ZN_CB_Insert(zn_p, znt->id, icb, cb_id);
							if (ND_BR_Insert(_R_ZN_CB,zn_p, znt->descr, icb, cb_id,znt->fdid,znt->ver) == NMD_FAIL)
							{
			//					sprintf(str,"����(%d)�����κͿ��ع�ϵ����ʧ��!",znt->id);
			//lld->WriteLog(lld->logfile,str);
								return NMD_FAIL;
							}
						}
						
						znt->r_cb = zn_tb_t->GetDataByPh(zn_p)->r_cb;
						znt->r_sec = zn_tb_t->GetDataByPh(zn_p)->r_sec;
						znt->r_ld = zn_tb_t->GetDataByPh(zn_p)->r_ld;
						zn_tb_t->Append(*znt);

						fd_tb->GetDataByPh(fdph)->cnt_zn++;//���������翪�صĸ�������
						if( Insert_r_char_int(1,znt->id,fdph,fdid) == NMD_FAIL )
						{
							//printf("fail to insert r_fd_zn (%d)\n", i);
			//				sprintf(str,"����(%d)�����ߺ����ι�ϵ����ʧ��!",znt->id);
			//lld->WriteLog(lld->logfile,str);
							return NMD_FAIL;

						}
						znt->Reset();
						znsum++;//������������ۼ�
						break;
					}	
				}
			}
		}
		//д��������Դ��ı�ѹ�������ۿۿڵ���������ȫ�ָ����ʺ͵�����ѹ���ĸ����ʣ�
		//so_tb->GetDataByPh(i)->sr = so_sumLoad;


//////////////////////////////////////////////////////////////////////////
//��һ�λ��ݣ������ص��������ϣ������ɿ��ػ��Ƿ�֧����
//���ɿ����� �ӵ�Դ��һֱ�����翪�ص����·������Ϊ�Ƿ�������ֻ����һ������·��
//��·���ϵĽڵ��¼����������ڶ������翪��Ҳ�ҵ�����ڵ㣬�Ϳ����˳���
//����ڵ���Ա��ΪT�ڵ㣬Ȼ������һ�������Ϳ���ȷ����������
		//���ҵ����翪��
		int lll = 0;
		int T_nd[100];//��T�ڵ������
		int cnt_T=0;//T�ڵ�ĸ���
		int route_nd_all[500];//����·���ϵĽڵ�����
		int cnt_route=0;
		//int kknd;
		
		for (jj=0;jj<unionCBcnt;jj++)
		{
			knd = unionCB[jj];//���翪�صĽڵ��
			kknd = knd;
			do
			{
				//��û��д������Ľڵ�д�뵽����route_nd_all�У���������Ѿ����ˣ�һ����T�ڵ�
				if (!lld->addNode(route_nd_all,knd,cnt_route))
				{//T�ڵ�,�����翪�ؿ�ʼ����ʱ��ֻҪ����T�ڵ�Ϳ���ֹͣ��
					//��������翪�ؽڵ��Լ���Ҳ��Ӧ�ü��뵽T�ڵ�����
					if (knd != kknd)
					{
						T_nd[cnt_T++] = knd;
						break;
					}
				}

				ind = knd;//���ο��صĽڵ��
				knd = (nd+knd)->bch;//�õ����ص��ϼ��ڵ��

				cb_p = r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
				while(cb_p)
				{
					//find the opposite nd through this cb
					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
					if ((((knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind) && (ind == cb_tb->GetDataByPh(cb_p->Ptr)->iznd))
					  ||((knd == cb_tb->GetDataByPh(cb_p->Ptr)->iznd) && (ind == cb_tb->GetDataByPh(cb_p->Ptr)->ind)))
					  && (cb_tb->GetDataByPh(cb_p->Ptr)->q !=1))
					{
						//�������趨Ϊ�����أ����ɿ��ذ������翪�غ͵�Դ�㿪��
						//////////////////////////////////////////////////////////////////////////
						//�������û�з�֧����ô���Խ�����Ϊֱ�߿��أ�2��������ֱ�߿���ѡһ���Ϳ����ˡ���������
						cb_tb->GetDataByPh(cb_p->Ptr)->maincb = 1;
						lll++;
					}			

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

			}	while(knd != sond);//һֱ����Դ��Ϊֹ

//		if (lll > 10)
//		{
//			printf("%s\n",so_tb->GetDataByPh(i)->id);	
//		}

		}

		//���������ܽ��ж���ɨ�裬�õ���������
		//������㿪ʼһֱ��T�ڵ㣬ֻ��һ�������أ��൱�ڽ�ֱ���ϵĿ��غϲ���һ��
		lld->sortInt(T_nd,cnt_T);//�����е�T�ڵ��������
		for (jj=0;jj<unionCBcnt;jj++)
		{
			lll=0;
			knd = unionCB[jj];//���翪�صĽڵ��
			do
			{
				if (lld->findNode(T_nd,knd,cnt_T) >= 0)
				{//T�ڵ�
					lll=0;
				}

				ind = knd;//���ο��صĽڵ��
				knd = (nd+knd)->bch;//�õ����ص��ϼ��ڵ��

				cb_p = r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
				while(cb_p)
				{
					//find the opposite nd through this cb
					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
					if ((((knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind) && (ind == cb_tb->GetDataByPh(cb_p->Ptr)->iznd))
					  ||((knd == cb_tb->GetDataByPh(cb_p->Ptr)->iznd) && (ind == cb_tb->GetDataByPh(cb_p->Ptr)->ind)))
					  && (cb_tb->GetDataByPh(cb_p->Ptr)->q !=1))
					{
						if (lll > 0)//��T�ڵ�ǰ�ظ����֣�lll==0�Ѿ�������
						{
							//�����翪��������Ǹ����ؿ�����Ϊ������
							//�������û�з�֧����ô���Խ�����Ϊֱ�߿��أ�2��������ֱ�߿���ѡһ���Ϳ����ˡ���������
							cb_tb->GetDataByPh(cb_p->Ptr)->maincb = 0;
						}
						lll++;
					}			

					cb_p = cb_p->Next;
				}
				::FreeChildPtr(cb_p0);

			}	while(knd != sond);//һֱ����Դ��Ϊֹ
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
//�������У����

//�����߶Σ����������ڵ��߶�
	int isolate=0;
	int r_zn;
	sec = sec_tb->GetTable();
	for(i = 0; i < sec_tb->GetCount(); i++)
	{
		//������
        r_zn = r_zn_sec_tb_t->FindParentPtr1((sec+i)->r_zn);
		
		if (r_zn == -1)//���߶β������κ�����
		{
			sprintf(str,"warning:nmdУ��ʱ�߶� %s �޷���������!\n",(sec+i)->id);
			//str.Format("warning:nmdУ��ʱ�߶� %s �޷���������!",sec_tb->GetDataByLog(i)->id);
			lld->WriteLog(lld->logfile,str);
			isolate++;
		}
	}
//	char str[100];
	if (isolate > 0)
	{
		strcpy(str, "-------------------------");
		lld->WriteLog(lld->logfile,str);
		printf("warning:�����߶��޷���������\n");
		sprintf(str,"warning: nmdУ��ʱ�� %d ���߶��޷���������!",isolate);
		//str.Format("warning: nmdУ��ʱ�� %d ���߶��޷���������!",isolate);
		lld->WriteLog(lld->logfile,str);
		//return NMD_FAIL
	}

//����	
	isolate=0;
	ld = ld_tb->GetTable();
	lv_ld = ld_tb->GetCount();

	for( i = 0; i < lv_ld; i++)
	{
        r_zn = r_zn_ld_tb_t->FindParentPtr1((ld+i)->r_zn);
		if (r_zn == -1)//���߶β������κ�����
		{
			sprintf(str,"\nerror: nmdУ��ʱ���� %s �ڹµ���!",(ld+i)->id);
			//str.Format("error: nmdУ��ʱ���� %s �ڹµ���!",ld_tb->GetDataByLog(i)->id);
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
		printf("���ָ����ڹµ��ڵ����\n");
		sprintf(str,"error: nmdУ��ʱ�� %d �������ڹµ���!",isolate);
		//str.Format("error: nmdУ��ʱ�� %d �������ڹµ���!",isolate);
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
		//sprintf(str,"���ر����޼�¼!");
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

		//�����ص���ĩ�ڵ���뵽�ڵ����,�γɽڵ����ʱ��
//		if( ND_Insert((cb+i)->nd,&((cb+i)->ind),-1,(cb+i)->fdid,(cb+i)->ver) == NMD_SUCCEED &&
//			ND_Insert((cb+i)->znd,&((cb+i)->iznd),-1,(cb+i)->fdid,(cb+i)->ver) == NMD_SUCCEED )
		{
			//insert r_nd_cb table
			//��ĩ�ڵ�ֱ����,�ڵ�-֧·��ϵ��ȷ��һ���ڵ��Ӧ����֧·
			if( ND_BR_Insert(_CB,(cb+i)->ind,(cb+i)->nd,i,(cb+i)->id,(cb+i)->fdid,(cb+i)->ver) == NMD_FAIL ||
				ND_BR_Insert(_CB,(cb+i)->iznd,(cb+i)->znd,i,(cb+i)->id,(cb+i)->fdid,(cb+i)->ver) == NMD_FAIL )
			{
				sprintf(str,"����(%s)�Ľڵ�Ϳ��ع�ϵ����ʧ��!���ܸÿ��صĽڵ����������ء�",(cb + i)->id);
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
		//sprintf(str,"��������ĸ��!");
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
//			sprintf(str,"ĸ��(%s)����ڵ�ʧ��!",(bus + i)->id);
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
			//sprintf(str,"ĸ��(%s)��ѹȡֵ����!",(bus + i)->id);
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

		if((ld+i)->type & Q_LOAD)  //һ�㸺��
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
			{//����ڵ㸺�ɹ�ϵ��
				if( ND_BR_Insert(_LD,(ld+i)->ind,(ld+i)->nd,i,(ld+i)->id,(ld+i)->fdid,(ld+i)->ver) == NMD_FAIL)
				{
					//printf("fail to insert r_nd_ld for ld (%d)\n", i);
					
			//		sprintf(str,"����(%s)�Ľڵ�͸��ɹ�ϵ����ʧ�ܡ�",(ld + i)->id);
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
			//	sprintf(str,"�߶�(%s)�Ľڵ���߶ι�ϵ����ʧ��!",(sec+ i)->id);
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
	case 0:  //�ڵ�-�豸��ϵ
		{
		if ( (nd_ptr == -1) || (strlen(nd_id)==0) || (strcmp(nd_id,"-")==0) )
			return 1;

		R_ND_EQ* r_equip_nd = new R_ND_EQ;
		*r_equip_nd = R_ND_EQ(nd_id,eq_ptr);
		r_equip_nd->equip_id_ptr = eq_ptr;
		r_equip_nd->nd_id_ptr = nd_ptr;
		phpos = (nd+nd_ptr)->r_eq;//�õ��ڵ���кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected cb of this nd�ڵ�û�����ӿ���
		{
			(nd+nd_ptr)->r_eq = r_nd_eq_tb_t->GetNext();
		}
		else  //build cb chain for this nd������ڵ���ǰ�Ѿ������˿���
		{
			//���ڵ㿪�ع�ϵ���е�λ�����Ų����׼���µĽڵ��ϵָ�����
			fptr = r_nd_eq_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_nd_eq_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_eq_tb_t->GetDataByPh(phpos)->fptr = r_nd_eq_tb_t->GetNext();
			r_equip_nd->bptr = phpos;
		}
		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_equip_nd->id = r_nd_eq_tb_t->GetNext();//���߼�λ����id��
		phpos = r_nd_eq_tb_t->Append(*r_equip_nd);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			return NMD_FAIL;
		}
		delete r_equip_nd;
		r_equip_nd = 0;

		break;
		}
	case 1:  //����-���ι�ϵ
		{
		r_fd_zn = new R_FD_ZN;
		*r_fd_zn = R_FD_ZN(nd_id, eq_ptr);//�������κͿ��صĹ�ϵ
		r_fd_zn->fd_id_ptr = nd_ptr;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_fd_zn->zn_id_ptr = eq_ptr;//zn_tb_t->GetPhByLog(eq_ptr);//���ص�ָ��

		//strcpy(r_fd_zn->fdid , fdid);//�ڵ���������
		//strcpy(r_fd_zn->ver , ver);//�ڵ���������
		
		zn_tb_t->GetDataByPh(eq_ptr)->r_fd = r_fd_zn_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		phpos = fd_tb->GetDataByPh(nd_ptr)->r_zn;//�õ����α��кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected sec of this zn����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			fd_tb->GetDataByPh(nd_ptr)->r_zn = r_fd_zn_tb_t->GetNext();
		}
		else  //build sec chain for this zn������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_fd_zn_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_zn_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_zn_tb_t->GetDataByPh(phpos)->fptr = r_fd_zn_tb_t->GetNext();
			r_fd_zn->bptr = phpos;
		}
//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_fd_zn->id = r_fd_zn_tb_t->GetNext();//���߼�λ����id��
		phpos = r_fd_zn_tb_t->Append(*r_fd_zn);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			delete r_fd_zn;
			r_fd_zn = 0;
			return NMD_FAIL;
		}
		delete r_fd_zn;
		r_fd_zn = 0;
		
		break;
		}
	case 2:  //����-�豸��ϵ
		{
		r_fd_eq = new R_FD_EQ;
		*r_fd_eq = R_FD_EQ(nd_id, eq_ptr);//�������κͿ��صĹ�ϵ
		r_fd_eq->fd_id_ptr = nd_ptr;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_fd_eq->eq_id_ptr = eq_ptr;//dsbob->eq_tb->GetPhByLog(eq_ptr);//���ص�ָ��

		//strcpy(r_fd_eq->fdid , fdid);//�ڵ���������
		//strcpy(r_fd_eq->ver , ver);//�ڵ���������
		
		eq_tb_t->GetDataByPh(eq_ptr)->r_fd = r_fd_eq_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		phpos = fd_tb->GetDataByPh(nd_ptr)->r_eq;//�õ����α��кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected sec of this eq����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			fd_tb->GetDataByPh(nd_ptr)->r_eq = r_fd_eq_tb_t->GetNext();
		}
		else  //build sec chain for this eq������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_fd_eq_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_eq_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_eq_tb_t->GetDataByPh(phpos)->fptr = r_fd_eq_tb_t->GetNext();
			r_fd_eq->bptr = phpos;
		}
//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_fd_eq->id = r_fd_eq_tb_t->GetNext();//���߼�λ����id��
		phpos = r_fd_eq_tb_t->Append(*r_fd_eq);
		if(phpos == -1)//�ж��Ƿ�Խ��
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

//ͨ��һ�������Ľڵ��ҵ�����ID
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
	//�����еĽڵ���Ϊδ����׼�����˷�����
	for(int i = 0; i < lv_nd; i++)
	{
		(nd_t+i)->q |= ND_UNPROC;
	}

	//�ڵ��bchΪ�ϼ��ڵ�
	(nd_t+ind)->bch = ind;//��Դ�㿪ʼ��ʼ����,��Դ����Ϊ��һ����¼����
	
	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if((nd_t+ind)->q & ND_UNPROC) //this so nd_t has not processed
	{
		//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
		(nd_t+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
		(nd_t+knd)->q &= (~ND_UNPROC);//����ǰ�ڵ���Ϊ�Ѵ���

		while (1)//������չ����û�д���
		{
			if ((nd_t+knd)->q & ND_LD) 
			{
				//�õ�����id
				ld_p = r_nd_ld_tb->FindChildPtr1((nd_t+knd)->r_ld);
				ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
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
			//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
			cb_p = r_nd_cb_tb->FindChildPtr1((nd_t+knd)->r_cb);
			cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
			while(cb_p)
			{
				//find the opposite nd_t through this cb
				//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
				if(knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
				{
					kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
				}
				else
				{
					kkndcb = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
				}

				//��������翪�أ�ͣ����,ֻҪ�����翪�أ������Ƿ����գ�����Ҫ���д���
				if  ((cb_tb->GetDataByPh(cb_p->Ptr)->q & Q_UNIONCB)//���翪�ص�qΪ1������
					//|| (cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB))  
					//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
					) 
				{
					//����Ǵ����翪�ؽ������������������ͬ����ͨ���տ���
					cb_p = cb_p->Next;
					continue;
				}

				//process this opposite nd
				//����Բ�ڵ� δ����==���ز����������ƽ�
				if((nd_t+kkndcb)->q & ND_UNPROC)
				{
					//������ڵ����һ����¼������׼������
					(nd_t+kkndcb)->bch = knd;

					//����չ��ǰ���� ��� �ҵ����½ڵ��ǰ����
					(nd_t+kkndcb)->fch = (nd_t+knd)->fch;
					//�½ڵ� ��� ��չ�ڵ��ǰ����
					(nd_t+knd)->fch = kkndcb;
					//���ҵ����½ڵ�����Ѵ���
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

			//move to next nd��������һ��
			knd = (nd_t+knd)->fch;
			//ѭ����ͷ���������ˣ���ʾȫ������һ��
			//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
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
//ͨ�����������ܱߵ���������,���ߺ����翪�صĹ�ϵ������fd-��������fds-��
int NMD::FD_Validate()
{
	//	char left[32],right[32];
	char str[500];//,sTemp1[MAX_UNIN_ID_SUM];
	int		i;
	int		l, r, cb_l, cb_r;
	char	un_id[MAX_ZN_ID_LEN];
	char	fdid[MAX_SEC_ID_LEN], fd_left[MAX_SEC_ID_LEN], fd_right[MAX_SEC_ID_LEN];

	if (lld->mx_un <= 0)//������û�����翪��
	{
		lld->mx_r_fd_un = 0;
		return 1;
	}
	//read un table into buffer
	lv_un = un_tb->GetCount();
	if (lv_un == 0)
	{
		sprintf(str, "���翪�ر����޼�¼!");
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
		//��un��ѿ��ص���������д����
		strcpy(un_id, (un + i)->id);

		//1.�������ߺ����翪�صĹ�ϵ�������Ǹ������翪������
		l = dsbob->OnGetPhByfdid((un + i)->left);//fd_tb->FindPh(FD((un+i)->left));
		if (l == -1)
			continue;
		fd_tb->GetDataByPh(l)->cnt_un++;//���������翪�صĸ�������
		r = dsbob->OnGetPhByfdid((un + i)->right);//fd_tb->FindPh(FD((un+i)->right));
		if (r == -1)
		{
			//Ĭ�ϵ�ֵ
			if (ND_BR_Insert(_FDS, l, (un + i)->left, i, (un + i)->id, "", "") == NMD_FAIL)
			{
				//sprintf(str, "����(%s)�����ߺ����翪�ع�ϵ����ʧ��!", (sec + i)->id);
				//lld->WriteLog(lld->logfile, str);
				return NMD_FAIL;
			}
			strcpy(cbt->id, un_id);
			l = cb_tb->FindPh(*cbt);
			(un + i)->idph = l;//���ص������
			cb_l = cb_tb->GetDataByPh(l)->ind;	//���ص���ڵ�
			cb_r = cb_tb->GetDataByPh(l)->iznd;	//���ص��ҽڵ�
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
			//sprintf(str, "����(%s)�����ߺ����翪�ع�ϵ����ʧ��!", (sec + i)->id);
			//lld->WriteLog(lld->logfile, str);
			return NMD_FAIL;
		}

		//�����翪���������ҽڵ㣬��Ϊ��֪�����ĸ��������ŵ�
		//cb_l->fd_left;
		strcpy(cbt->id, un_id);
		l = cb_tb->FindPh(*cbt);
		(un + i)->idph = l;//���ص������
		cb_l = cb_tb->GetDataByPh(l)->ind;	//���ص���ڵ�
		cb_r = cb_tb->GetDataByPh(l)->iznd;	//���ص��ҽڵ�
		strcpy(fd_left, (un + i)->left);//��ߵ�����
		strcpy(fd_right, (un + i)->right);//�ұߵ�����

		//ͨ��������ߵĽڵ��ҵ�������
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
		else//ͨ���ҽڵ�����
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

	//������
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
//  XFMR_Validate()�Ա�ѹ�������У�飬�����д�뵽��ѹ����������
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
	//		str.Format("��ѹ�������޼�¼!");
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
								//	//sprintf(str,"����************��ѹ��(%s)��ѹ��ڵ�(%s)������Ԫ��!",(xfmr + i)->id,(xfmr+i)->hnd);
								//	//lld.addLogLine(str);
								//	//return NMD_FAIL;
								//}
				//				lnd = nd_tb->FindPh(ND((xfmr+i)->lnd));
				//				//�����ѹ���޽ӵ㣬���Ե�������һ���ڵ㣬��ѹ�಻��û�нڵ�
				//				if(lnd == -1)
				//				{
				////lld add 2010.1.13 for null nd in xfmr
				//					nd = new ND((xfmr+i)->lnd);
				//					int phpos = nd_tb->Insert(*nd);
				//					if( phpos == -1 )
				//					{
				//						//printf("fail to insert nd table\n");
				//						//sprintf(str,"�ڵ���в����ѹ��ĩ�����ռ�¼%sʧ��!",(xfmr+i)->lnd);
				//						//lld.addLogLine(str);
				//						//return NMD_FAIL;
				//					}
				//					delete nd;
				//					nd = 0;
				////lld add end 2010.1.13 for null nd in xfmr
									//sprintf(str,"��ѹ��(%s)��ѹ��ڵ�(%s)������Ԫ��!",(xfmr + i)->id,(xfmr+i)->lnd);
									//lld.addLogLine(str);
									//return NMD_FAIL;
								//}

								//check tap type
				//if (((xfmr + i)->htapty[0] != '\0') && ((xfmr + i)->htapty[0] != '0'))//���˳�ͷ���ͣ�˵���г�ͷ
				//{
				//	strcpy(tapty1->id, (xfmr + i)->htapty);
				//	ph = tapty_tb->FindPh(*tapty1);

				//	(xfmr + i)->htap = ph;
				//	if ((xfmr + i)->htap == -1)
				//	{
				//		sprintf(str, "��ѹ��(%s)��ѹ���ͷ����(%s)����!", (xfmr + i)->id, (xfmr + i)->htapty);
				//		return NMD_FAIL;
				//	}
				//}
				//if (((xfmr + i)->ltapty[0] != '\0') && ((xfmr + i)->ltapty[0] != '0'))//���˳�ͷ���ͣ�˵���г�ͷ
				//{
				//	strcpy(tapty1->id, (xfmr + i)->ltapty);
				//	ph = tapty_tb->FindPh(*tapty1);

				//	(xfmr + i)->ltap = ph;
				//	if ((xfmr + i)->ltap == -1)
				//	{
				//		sprintf(str, "��ѹ��(%s)��ѹ���ͷ����(%s)����!", (xfmr + i)->id, (xfmr + i)->ltapty);
				//		//lld.addLogLine(str);
				//		return NMD_FAIL;
				//	}
				//}

				//2�����ѹ��ֻ��Ҫ��һ����������ֵ����
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
				xf->r1 = (xfmr + i)->rh;//��ѹ���r
				xf->x1 = (xfmr + i)->xh;//��ѹ���x
				xf->kvnom = (xfmr + i)->kvnomh;//��ѹ���ѹ�ȼ������ѹ��
				xf->zkvnom = (xfmr + i)->kvnoml;

				if (((xfmr + i)->htapty[0] != '\0') && ((xfmr + i)->htapty[0] != '0'))//���˳�ͷ���ͣ�˵���г�ͷ
				{
					strcpy(tapty1->id, (xfmr + i)->htapty);
					ph = tapty_tb->FindPh(*tapty1);
					xf->itapty = ph;//��ѹ���ͷ���ͺ�
					xf->tap = (xfmr + i)->htap;//��ѹ���ͷ�����
				}
				if (((xfmr + i)->ltapty[0] != '\0') && ((xfmr + i)->ltapty[0] != '0'))//���˳�ͷ���ͣ�˵���г�ͷ
				{
					strcpy(tapty1->id, (xfmr + i)->ltapty);
					ph = tapty_tb->FindPh(*tapty1);
					xf->iztapty = ph;//��ѹ���ͷ���ͺ�
					xf->ztap = (xfmr + i)->ltap;//��ѹ���ͷ�����
				}
				//�����ڵ������֮��Ĺ�ϵ
				strcpy(xf->fdid, (xfmr + i)->fdid);
				ph = xf_tb_t->GetNext();//����������һ������ţ�����������Ǹ���
				if (ND_BR_Insert(_XF, xf->ind, xf->nd, ph, xf->id, (xfmr + i)->fdid, (xf + i)->ver) == NMD_FAIL ||
					ND_BR_Insert(_XF, xf->iznd, xf->znd, ph, xf->id, (xfmr + i)->fdid, (xf + i)->ver) == NMD_FAIL)
				{
					//printf("fail to insert r_nd_xf for xf (%d)\n", i);
					//sprintf(str, "�߶�(%s)�Ľڵ���߶ι�ϵ����ʧ��!", (xf + i)->id);
					//lld.addLogLine(str);
					continue;
				}

				//��������ͱ�ѹ��֮��Ĺ�ϵ//�����ڸ��Ĺ�ϵ
				(xf + i)->r_xfmr = r_xfmr_xf_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
				if (ND_BR_Insert(_XFMR, i, (xfmr+i)->id, ph, xf_id, "", "") == NMD_FAIL)
				{
					//printf("fail to insert r_xfmr_xf table\n");
					//sprintf(str, "��ѹ���������ϵ����ʧ��!");
					//lld.addLogLine(str);
					continue;
				}
				if ((ph = xf_tb_t->Insert(*xf)) == -1)
				{
					printf("fail to insert xf table\n");
					sprintf(str, "����(%s)�в���ʧ��!", (xf + i)->id);
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
					sprintf(str, "����************��ѹ��(%s)��ѹ��ڵ�(%s)������Ԫ��!", (xfmr + i)->id, (xfmr + i)->hnd);
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
						sprintf(str, "�ڵ���в����ѹ��ĩ�����ռ�¼%sʧ��!", (xfmr + i)->mnd);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
					delete nd;
					nd = 0;
					//lld add end 2010.1.13 for null nd in xfmr
					sprintf(str, "��ѹ��(%s)��ѹ��ڵ�(%s)������Ԫ��!", (xfmr + i)->id, (xfmr + i)->mnd);
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
						sprintf(str, "�ڵ���в����ѹ��ĩ�����ռ�¼%sʧ��!", (xfmr + i)->lnd);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
					delete nd;
					nd = 0;
					//lld add end 2010.1.13 for null nd in xfmr
					sprintf(str, "��ѹ��(%s)��ѹ��ڵ�(%s)������Ԫ��!", (xfmr + i)->id, (xfmr + i)->lnd);
					//lld.addLogLine(str);
					//return NMD_FAIL;
				}

				//insert the center nd of this three-winding xfmr
				strcpy(nd_id, (xfmr + i)->id);
				strcat(nd_id, "c");//������Ҫ�ڱ�ѹ��ID�ĺ����c,2����ļ�t
				nd = new ND(nd_id);
				cnd = nd_tb->Insert(*nd);//��һ���м�ڵ㣬�ڵ����ƾ��Ǳ�ѹ����ID�����c
				if (cnd == -1)
				{
					return NMD_FAIL;
				}
				delete nd;
				nd = 0;

				//check tap type
				if (((xfmr + i)->htapty[0] != '\0') && ((xfmr + i)->htapty[0] != '0'))//���˳�ͷ���ͣ�˵���г�ͷ
	//				if((xfmr+i)->htapty[0] != '\0')
				{
					strcpy(tapty1->id, (xfmr + i)->htapty);
					ph = tapty_tb->FindPh(*tapty1);

					(xfmr + i)->htap = ph;
					if ((xfmr + i)->htap == -1)
					{
						sprintf(str, "��ѹ��(%s)��ѹ���ͷ����(%s)����!", (xfmr + i)->id, (xfmr + i)->htapty);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
				}
				if (((xfmr + i)->mtapty[0] != '\0') && ((xfmr + i)->mtapty[0] != '0'))//���˳�ͷ���ͣ�˵���г�ͷ
	//				if((xfmr+i)->mtapty[0] != '\0')
				{
					strcpy(tapty1->id, (xfmr + i)->mtapty);
					ph = tapty_tb->FindPh(*tapty1);

					(xfmr + i)->mtap = ph;
					if ((xfmr + i)->mtap == -1)
					{
						sprintf(str, "��ѹ��(%s)��ѹ���ͷ����(%s)����!", (xfmr + i)->id, (xfmr + i)->mtapty);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
				}
				//				if((xfmr+i)->ltapty[0] != '\0')
				if (((xfmr + i)->ltapty[0] != '\0') && ((xfmr + i)->ltapty[0] != '0'))//���˳�ͷ���ͣ�˵���г�ͷ
				{
					strcpy(tapty1->id, (xfmr + i)->ltapty);
					ph = tapty_tb->FindPh(*tapty1);

					(xfmr + i)->ltap = ph;
					if ((xfmr + i)->ltap == -1)
					{
						sprintf(str, "��ѹ��(%s)��ѹ���ͷ����(%s)����!", (xfmr + i)->id, (xfmr + i)->ltapty);
						//lld.addLogLine(str);
						return NMD_FAIL;
					}
				}

				//insert primary side xf for three-winding xfmr
				(xfmr + i)->r_xf = r_xfmr_xf_tb->GetNext();
				strcpy(xf_id, (xfmr + i)->id);
				strcat(xf_id, "h");//��ѹ��ID�����h����ñ�ѹ����ѹ�������ID
				*xf = XF(xf_id);
				strcpy(xf->descr, (xfmr + i)->descr);
				strcat(xf->descr, "��ѹ��");
				strcpy(xf->nd, (xfmr + i)->hnd);//��ѹ����ѹ��Ľڵ����ڱ�ѹ��У���ʱ���Ѿ��������ε�ID����
				strcpy(xf->znd, nd_id);//�м�ڵ��ID
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
					sprintf(str, "����(%s)����ʧ��!", (xf + i)->id);
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
				prevph = r_xfmr_xf_tb->Insert(*r_xfmr_xf);
				if (prevph == -1)
				{
					printf("fail to insert r_xfmr_xf table\n");
					sprintf(str, "��ѹ���������ϵ����ʧ��!");
					//lld.addLogLine(str);
					return NMD_FAIL;
				}

				//insert secondary side xf for three-winding xfmr
				strcpy(xf_id, (xfmr + i)->id);
				strcat(xf_id, "m");
				*xf = XF(xf_id);
				strcpy(xf->descr, (xfmr + i)->descr);
				strcat(xf->descr, "��ѹ��");
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
					sprintf(str, "����(%s)����ʧ��!", (xf + i)->id);
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
				prevph = r_xfmr_xf_tb->Insert(*r_xfmr_xf);
				if (prevph == -1)
				{
					printf("fail to insert r_xfmr_xf table\n");
					sprintf(str, "��ѹ���������ϵ����ʧ��!");
					//lld.addLogLine(str);
					return NMD_FAIL;
				}

				//insert third side xf for three-winding xfmr
				strcpy(xf_id, (xfmr + i)->id);
				strcat(xf_id, "l");
				*xf = XF(xf_id);
				strcpy(xf->descr, (xfmr + i)->descr);
				strcat(xf->descr, "��ѹ��");
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
					sprintf(str, "����(%s)�����¼ʧ��!", (xf + i)->id);
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
				prevph = r_xfmr_xf_tb->Insert(*r_xfmr_xf);
				if (prevph == -1)
				{
					printf("fail to insert r_xfmr_xf table\n");
					sprintf(str, "��ѹ���������ϵ�����¼ʧ��!");
					//lld.addLogLine(str);
					return NMD_FAIL;
				}
			}
			else
			{
				printf("the type of xfmr (%d) is illegal\n", i);
				sprintf(str, "��ѹ��(%s)���ͷǷ�!", (xfmr + i)->id);
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
	//	str.Format("��ѹ����У��ͨ��!");
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
	//XFMR* xfmr_t = new XFMR(); //��ʱ��׼����������

	//read xf table into buffer
	lv_xf = xf_tb->GetCount();
	if (lv_xf == 0)
	{
		//printf("there is no record in xf table\n");
//		CString str;
//		str.Format("��������޼�¼!");
//		lld.addLogLine(str);
		return NMD_SUCCEED;
	}

	xf = new XF[lv_xf];
	xf_tb->CopyTable(xf);

	ND* ndt = new ND();

	//check xf one by one
	for (i = 0; i < lv_xf; i++)
	{
		//ֱ������صĽڵ����
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
		////1.�ҵ���Ӧ�ı�ѹ��
		//strcpy(xfmr_id, (xf + i)->fdid);//�õ����������ı�ѹ��id
		//strcpy(xfmr_t->id, xfmr_id);
		//ph = xfmr_tb->FindPh(*xfmr_t);
		//strcpy(tapty_id , (xf + i)->tapty);

		//if (ph != -1)//ֻ���ҵ���ѹ���������������
		//{
		//	//2.��������ĳ�ͷ
		//	ph = xfmr_tb->FindPh(*xfmr_t);
		//	strcpy(nd1->id, (xf + i)->nd);
		//	(xf + i)->ind = nd_tb->FindPh(*nd1);
		//	strcpy(nd1->id, (xf + i)->znd);
		//	(xf + i)->iznd = nd_tb->FindPh(*nd1);

		//	//��д��ѹ���Ĳ��ֲ���
		//	if (strcmp((xf + i)->ver, "��") == 0)//��ѹ������
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
		//	else if (strcmp((xf + i)->ver, "��") == 0)//��ѹ������
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
		//	else if (strcmp((xf + i)->ver, "��") == 0)//��ѹ������
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

		//	//��������ͱ�ѹ��֮��Ĺ�ϵ
		////�����ڸ��Ĺ�ϵ
		//	(xf + i)->r_xfmr = r_xfmr_xf_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		//	if (ND_BR_Insert(_XFMR, ph, xfmr_id, i, (xf + i)->id, (xf + i)->fdid, (xf + i)->ver) == NMD_FAIL)
		//	{
		//		//printf("fail to insert r_xfmr_xf table\n");
		//		//sprintf(str, "��ѹ���������ϵ����ʧ��!");
		//		//lld.addLogLine(str);
		//		continue;
		//	}

		//	//�����ڵ������֮��Ĺ�ϵ
		//	if (ND_BR_Insert(_XF, (xf + i)->ind, (xf + i)->nd, i, (xf + i)->id, (xf + i)->fdid, (xf + i)->ver) == NMD_FAIL ||
		//		ND_BR_Insert(_XF, (xf + i)->iznd, (xf + i)->znd, i, (xf + i)->id, (xf + i)->fdid, (xf + i)->ver) == NMD_FAIL)
		//	{
		//		//printf("fail to insert r_nd_xf for xf (%d)\n", i);
		//		//sprintf(str, "�߶�(%s)�Ľڵ���߶ι�ϵ����ʧ��!", (xf + i)->id);
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
//	str.Format("�����У��ͨ��!");
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
			sprintf(str, "��Դ%s�ڵ�Ų���ȷ��",(so+i)->id);
			lld->WriteLog(lld->logfile, str);
			continue;
		}
		nd1->q |= ND_SO;
		nd_tb->UpdateDataByPh(nd1,ph);
		delete nd1;	

		(so+i)->ind = ph;

//		if(ND_Insert((so+i)->nd,&(so+i)->ind,ND_SO,(so+i)->fdid,(so+i)->ver) != NMD_SUCCEED)
//		{
//			sprintf(str,"��Դ(%s)�ĵ�Դ����ʧ��!",(so+ i)->id);
//			lld->addLogLine(str);
//			continue;
//		}
				
		if (lld->MATH_VOLTAGELEVEL == 10)
		{
			strcpy(fdid, (so + i)->fdid);//��Դ����������
			//��������-��Դ��ϵ��
			ph = dsbob->OnGetPhByfdid(fdid);//fd_tb->FindPh(FD(fdid));//�����߱��е�λ��

			if (ND_BR_Insert(_SO, ph, fdid, i, (so + i)->id, "", "") == NMD_FAIL)
			{
				//sprintf(str, "��Դ(%s)�ĵ�Դ�����߹�ϵ����ʧ��!", (so + i)->id);
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
//  CP_Validate()������Ҫ��
int NMD::CP_Validate()
{
	int		i,ph/* , ist*/;
	char    str[500];

	if ( (lld->mx_cp==0)
		||( (strlen(cp_tb->GetDataByLog(0)->nd)==0) && (strlen(cp_tb->GetDataByLog(0)->znd) == 0))
		)
	{
		//sprintf(str,"��������cp!");
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
				//sprintf(str, "������(%s)�Ľڵ�͵�������ϵ����ʧ��!", (cp + i)->id);
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
//  ND_Insert(),���뵽��ʱ�� lld edit 2018-4-25
int NMD::ND_Insert(const char* nd_id, int* nd_p, int mask, const char* fdid, const char* ver)
{
	if ( (strlen(nd_id)==0) || (strcmp(nd_id,"-")==0) )
		return NMD_SUCCEED;

	char	str[500];
	int		phpos/*, len*/;
	ND* ndt = new ND();
	//���ڵ�������ǲ����Ѿ���������ڵ�
	strcpy(ndt->id, nd_id);
	phpos = nd_tb->FindPh(*ndt);
	if( phpos != -1 )  //an old nd
	{
		nd = new ND;
		nd_tb->CopyDataByPh(nd, phpos);
		strcpy(nd->fdid,fdid);
		strcpy(nd->ver,ver);

		if( mask != -1 )//�ǲ�����Ҫ�޸Ľڵ��qֵ�������Ҫ�Ļ����ı�qֵ
		{
			nd->q = nd->q | mask;
		}

		nd_tb->UpdateDataByPh(nd, phpos);

		delete nd;
		nd = 0;

		if( mask != -1 )//�ǲ�����Ҫ�޸Ľڵ��qֵ�������Ҫ�Ļ����ı�qֵ
		{
			nd = new ND;
			nd_tb->CopyDataByPh(nd, phpos);
			nd->q = nd->q | mask;
			nd_tb->UpdateDataByPh(nd, phpos);
			delete nd;
			nd = 0;
		}
		*nd_p = phpos;//������λ�÷��ػ�ȥ���Ա���һ���ڵ����
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
//		len = strlen(nd_id);//�ڵ��ǽӵص�բ
//		if(nd_id[len-2] == 'G' && nd_id[len-1] == 'n')
//		{
//			nd->q = nd->q | ND_GRND;
//		}
		phpos = nd_tb->Insert(*nd);
		if( phpos == -1 )
		{
			sprintf(str,"�ڵ���в����¼ʧ��!");
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
//���ݽڵ��ID�ź��豸��ID�ţ��ڹ�ϵ���в���2����¼
//�ڵ�ţ��豸�ţ��ڵ�ָ�루�ڽڵ���е�λ�ã��Ǳ꣬����ţ����豸ָ�룬ǰ������������������������
//�����豸�������ڵ㣬������һ���ڵ�
//
//
//�����ڶ����ڵ�
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
		*r_nd_cb = R_ND_CB(nd_id, br_id);//����ڵ��֧·�Ĺ�ϵ
		r_nd_cb->nd_id_ptr = nd_p;//�ڵ��ָ��,�ڽڵ���е�λ�ã��Ǳ�
		r_nd_cb->cb_id_ptr = cb_tb->GetPhByLog(ibr);//���ص�ָ�룬�ڿ��ر��е�λ��
		phpos = nd_tb->GetDataByPh(nd_p)->r_cb;//�õ��ڵ���кͿ��صĹ�����Ϣ

		//strcpy(r_nd_cb_t->fdid , fdid);//�ڵ���������
		//strcpy(r_nd_cb_t->ver , ver);//�ڵ���������

		if(phpos == -1)  //first connected cb of this nd�ڵ�û�����ӿ���
		{
			//�ڵ��Ǹ����������ӡ�
			//�ڵ����ĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����ڵ����Ŀ���
			nd_tb->GetDataByPh(nd_p)->r_cb = r_nd_cb_tb_t->GetNext();
		}
		else  //build cb chain for this nd������ڵ���ǰ�Ѿ������˿���
		{
			//���ڵ㿪�ع�ϵ���е�λ�����Ų����׼���µĽڵ��ϵָ�����
			fptr = r_nd_cb_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_nd_cb_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_cb_tb_t->GetDataByPh(phpos)->fptr = r_nd_cb_tb_t->GetNext();
			r_nd_cb->bptr = phpos;
		}
		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_nd_cb->id = r_nd_cb_tb_t->GetNext();//���߼�λ����id��
		phpos = r_nd_cb_tb_t->Append(*r_nd_cb);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			return NMD_FAIL;
		}
		delete r_nd_cb;
		r_nd_cb = 0;
		break;
	case _XF:  //xf
		r_nd_xf = new R_ND_XF;
		*r_nd_xf = R_ND_XF(nd_id, br_id);
		r_nd_xf->nd_id_ptr = nd_p;//���ӱ��߶εĽڵ��ڽڵ���е�����λ�ã�ע�⣺��ϵ�ڽ�����
		r_nd_xf->xf_id_ptr = ibr;//�����Ƚ����⣬������ϵ���ٲ������� lld 2019-9-17 xf_tb_t->GetPhByLog(ibr);//�߶α������λ��

		strcpy(r_nd_xf->fdid, fdid);//�ڵ���������
		strcpy(r_nd_xf->ver, ver);//�ڵ���������

		phpos = nd_tb->GetDataByPh(nd_p)->r_xf;
		if (phpos == -1)  //first connected xf of this nd���ڵ���л�û�������ߵ������¼��ֻ����һ�������ǵ�һ��λ��
		{
			nd_tb->GetDataByPh(nd_p)->r_xf = r_nd_xf_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		}
		else  //build xf chain for this nd��������������ڵ������������߶ε�ȫ������λ�ù�������
		{
			//��������λ�ã��ҵ������Ǹ�����λ��
			fptr = r_nd_xf_tb_t->GetDataByPh(phpos)->fptr;
			while (fptr != -1)
			{
				phpos = fptr;
				fptr = r_nd_xf_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_xf_tb_t->GetDataByPh(phpos)->fptr = r_nd_xf_tb_t->GetNext();//�����ε�ָ����������Ǹ�����λ��
			r_nd_xf->bptr = phpos;//ǰһ�������Σ���Ϊ���ε���һ������
		}

		r_nd_xf->id = r_nd_xf_tb_t->GetNext();//������λ����id�ţ���֤ÿ�ζ����뵽���
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
		r_xfmr_xf->xfmr_id_ptr = nd_p;//��ѹ���������
		r_xfmr_xf->xf_id_ptr = xf_tb_t->GetPhByLog(ibr);
		//strcpy(r_xfmr_xf->fdid , fdid);//�ڵ���������
		//strcpy(r_xfmr_xf->ver , ver);//�ڵ���������
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
		r_xfmr_xf->id = r_xfmr_xf_tb_t->GetNext();//���߼�λ����id��
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
		r_nd_sec->nd_id_ptr = nd_p;//���ӱ��߶εĽڵ��ڽڵ���е�����λ�ã�ע�⣺��ϵ�ڽ�����
		r_nd_sec->sec_id_ptr = sec_tb->GetPhByLog(ibr);//�߶α������λ��

		strcpy(r_nd_sec->fdid , fdid);//�ڵ���������
		strcpy(r_nd_sec->ver , ver);//�ڵ���������

		phpos = nd_tb->GetDataByPh(nd_p)->r_sec;
		if(phpos == -1)  //first connected sec of this nd���ڵ���л�û�������ߵ������¼��ֻ����һ�������ǵ�һ��λ��
		{
			nd_tb->GetDataByPh(nd_p)->r_sec = r_nd_sec_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		}
		else  //build sec chain for this nd��������������ڵ������������߶ε�ȫ������λ�ù�������
		{
			//��������λ�ã��ҵ������Ǹ�����λ��
			fptr = r_nd_sec_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_nd_sec_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_sec_tb_t->GetDataByPh(phpos)->fptr = r_nd_sec_tb_t->GetNext();//�����ε�ָ����������Ǹ�����λ��
			r_nd_sec->bptr = phpos;//ǰһ�������Σ���Ϊ���ε���һ������
		}
		r_nd_sec->id = r_nd_sec_tb_t->GetNext();//������λ����id�ţ���֤ÿ�ζ����뵽���
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
		r_nd_cp->nd_id_ptr = nd_p;//���ӱ��߶εĽڵ��ڽڵ���е�����λ�ã�ע�⣺��ϵ�ڽ�����
		r_nd_cp->cp_id_ptr = cp_tb->GetPhByLog(ibr);//�߶α������λ��

		strcpy(r_nd_cp->fdid, fdid);//�ڵ���������
		strcpy(r_nd_cp->ver, ver);//�ڵ���������

		phpos = nd_tb->GetDataByPh(nd_p)->r_cp;
		if (phpos == -1)  //first connected cp of this nd���ڵ���л�û�������ߵ������¼��ֻ����һ�������ǵ�һ��λ��
		{
			nd_tb->GetDataByPh(nd_p)->r_cp = r_nd_cp_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		}
		else  //build cp chain for this nd��������������ڵ������������߶ε�ȫ������λ�ù�������
		{
			//��������λ�ã��ҵ������Ǹ�����λ��
			fptr = r_nd_cp_tb_t->GetDataByPh(phpos)->fptr;
			while (fptr != -1)
			{
				phpos = fptr;
				fptr = r_nd_cp_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_cp_tb_t->GetDataByPh(phpos)->fptr = r_nd_cp_tb_t->GetNext();//�����ε�ָ����������Ǹ�����λ��
			r_nd_cp->bptr = phpos;//ǰһ�������Σ���Ϊ���ε���һ������
		}
		r_nd_cp->id = r_nd_cp_tb_t->GetNext();//������λ����id�ţ���֤ÿ�ζ����뵽���
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
		*r_nd_ld = R_ND_LD(nd_id, br_id);//����ڵ��֧·�Ĺ�ϵ
		r_nd_ld->nd_id_ptr = nd_p;//�ڵ��ָ��
		r_nd_ld->ld_id_ptr = ld_tb->GetPhByLog(ibr);//���ص�ָ��

		strcpy(r_nd_ld->fdid , fdid);//�ڵ���������
		strcpy(r_nd_ld->ver , ver);//�ڵ���������

		phpos = nd_tb->GetDataByPh(nd_p)->r_ld;//�õ��ڵ���кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected ld of this nd�ڵ�û�����ӿ���
		{
			//�ڵ��Ǹ����������ӡ�
			//�ڵ����ĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����ڵ����Ŀ���
			nd_tb->GetDataByPh(nd_p)->r_ld = r_nd_ld_tb_t->GetNext();
		}
		else  //build ld chain for this nd������ڵ���ǰ�Ѿ������˿���
		{
			//���ڵ㿪�ع�ϵ���е�λ�����Ų����׼���µĽڵ��ϵָ�����
			fptr = r_nd_ld_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_nd_ld_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_nd_ld_tb_t->GetDataByPh(phpos)->fptr = r_nd_ld_tb_t->GetNext();
			r_nd_ld->bptr = phpos;
		}
		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_nd_ld->id = r_nd_ld_tb_t->GetNext();//���߼�λ����id��
		phpos = r_nd_ld_tb_t->Append(*r_nd_ld);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			return NMD_FAIL;
		}
		delete r_nd_ld;
		r_nd_ld = 0;
		break;

	case _SO:  //so
		r_fd_so = new R_FD_SO;
		*r_fd_so = R_FD_SO(nd_id, br_id);//����ڵ��֧·�Ĺ�ϵ
		r_fd_so->fd_id_ptr = nd_p;//�ڵ��ָ��
		r_fd_so->so_id_ptr = so_tb->GetPhByLog(ibr);//���ص�ָ��

//		strcpy(r_fd_so->fdid , fdid);//�ڵ���������
		strcpy(r_fd_so->ver , ver);//�ڵ���������

		phpos = fd_tb->GetDataByPh(nd_p)->r_so;//�õ��ڵ���кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected so of this fd�ڵ�û�����ӿ���
		{
			//�ڵ��Ǹ����������ӡ�
			//�ڵ����ĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����ڵ����Ŀ���
			fd_tb->GetDataByPh(nd_p)->r_so = r_fd_so_tb_t->GetNext();
		}
		else  //buiso so chain for this fd������ڵ���ǰ�Ѿ������˿���
		{
			//���ڵ㿪�ع�ϵ���е�λ�����Ų����׼���µĽڵ��ϵָ�����
			fptr = r_fd_so_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_so_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_so_tb_t->GetDataByPh(phpos)->fptr = r_fd_so_tb_t->GetNext();
			r_fd_so->bptr = phpos;
		}
		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_fd_so->id = r_fd_so_tb_t->GetNext();//���߼�λ����id��
		phpos = r_fd_so_tb_t->Append(*r_fd_so);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			return NMD_FAIL;
		}
		delete r_fd_so;
		r_fd_so = 0;
		break;

	case _R_ZN_CB:  //zn �����Ǹ�����������
		r_zn_cb = new R_ZN_CB;
		*r_zn_cb = R_ZN_CB(nd_id, br_id);//�������κͿ��صĹ�ϵ
		r_zn_cb->zn_id_ptr = nd_p;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_zn_cb->cb_id_ptr = cb_tb->GetPhByLog(ibr);//���ص�ָ��

		strcpy(r_zn_cb->fdid , fdid);//�ڵ���������
		strcpy(r_zn_cb->ver , ver);//�ڵ���������
		
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_cb;//�õ����α��кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected cb of this zn����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			zn_tb_t->GetDataByPh(nd_p)->r_cb = r_zn_cb_tb_t->GetNext();
		}
		else  //build cb chain for this zn������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_zn_cb_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_zn_cb_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_cb_tb_t->GetDataByPh(phpos)->fptr = r_zn_cb_tb_t->GetNext();
			r_zn_cb->bptr = phpos;
		}
//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_zn_cb->id = r_zn_cb_tb_t->GetNext();//���߼�λ����id��
		phpos = r_zn_cb_tb_t->Append(*r_zn_cb);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			delete r_zn_cb;
			r_zn_cb = 0;
			return NMD_FAIL;
		}
		delete r_zn_cb;
		r_zn_cb = 0;
		
		break;

	case _R_ZN_SEC:  //zn �����Ǹ�����������
		r_zn_sec = new R_ZN_SEC;
		*r_zn_sec = R_ZN_SEC(nd_id, br_id);//�������κͿ��صĹ�ϵ
		r_zn_sec->zn_id_ptr = nd_p;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_zn_sec->sec_id_ptr = sec_tb->GetPhByLog(ibr);//���ص�ָ��

		strcpy(r_zn_sec->fdid , fdid);//�ڵ���������
		strcpy(r_zn_sec->ver , ver);//�ڵ���������
		
		sec_tb->GetDataByPh(ibr)->r_zn = r_zn_sec_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_sec;//�õ����α��кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected sec of this zn����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			zn_tb_t->GetDataByPh(nd_p)->r_sec = r_zn_sec_tb_t->GetNext();
		}
		else  //build sec chain for this zn������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_zn_sec_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_zn_sec_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_sec_tb_t->GetDataByPh(phpos)->fptr = r_zn_sec_tb_t->GetNext();
			r_zn_sec->bptr = phpos;
		}
//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_zn_sec->id = r_zn_sec_tb_t->GetNext();//���߼�λ����id��
		phpos = r_zn_sec_tb_t->Append(*r_zn_sec);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			delete r_zn_sec;
			r_zn_sec = 0;
			return NMD_FAIL;
		}
		delete r_zn_sec;
		r_zn_sec = 0;
		
		break;

	case _R_ZN_CP:  //zn �����Ǹ�����������
		r_zn_cp = new R_ZN_CP;
		*r_zn_cp = R_ZN_CP(nd_id, br_id);//�������κͿ��صĹ�ϵ
		r_zn_cp->zn_id_ptr = nd_p;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_zn_cp->cp_id_ptr = cp_tb->GetPhByLog(ibr);//���ص�ָ��

		strcpy(r_zn_cp->fdid, fdid);//�ڵ���������
		strcpy(r_zn_cp->ver, ver);//�ڵ���������

		cp_tb->GetDataByPh(ibr)->r_zn = r_zn_cp_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_cp;//�õ����α��кͿ��صĹ�����Ϣ
		if (phpos == -1)  //first connected cp of this zn����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			zn_tb_t->GetDataByPh(nd_p)->r_cp = r_zn_cp_tb_t->GetNext();
		}
		else  //build cp chain for this zn������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_zn_cp_tb_t->GetDataByPh(phpos)->fptr;
			while (fptr != -1)
			{
				phpos = fptr;
				fptr = r_zn_cp_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_cp_tb_t->GetDataByPh(phpos)->fptr = r_zn_cp_tb_t->GetNext();
			r_zn_cp->bptr = phpos;
		}
		//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_zn_cp->id = r_zn_cp_tb_t->GetNext();//���߼�λ����id��
		phpos = r_zn_cp_tb_t->Append(*r_zn_cp);
		if (phpos == -1)//�ж��Ƿ�Խ��
		{
			delete r_zn_cp;
			r_zn_cp = 0;
			return NMD_FAIL;
		}
		delete r_zn_cp;
		r_zn_cp = 0;

		break;

	case _R_ZN_XF:  //zn �����Ǹ�����������
		r_zn_xf = new R_ZN_XF;
		*r_zn_xf = R_ZN_XF(nd_id, br_id);//�������κͿ��صĹ�ϵ
		r_zn_xf->zn_id_ptr = nd_p;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_zn_xf->xf_id_ptr = xf_tb->GetPhByLog(ibr);//���ص�ָ��

		strcpy(r_zn_xf->fdid, fdid);//�ڵ���������
		strcpy(r_zn_xf->ver, ver);//�ڵ���������

		xf_tb->GetDataByPh(ibr)->r_zn = r_zn_xf_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_xf;//�õ����α��кͿ��صĹ�����Ϣ
		if (phpos == -1)  //first connected xf of this zn����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			zn_tb_t->GetDataByPh(nd_p)->r_xf = r_zn_xf_tb_t->GetNext();
		}
		else  //build xf chain for this zn������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_zn_xf_tb_t->GetDataByPh(phpos)->fptr;
			while (fptr != -1)
			{
				phpos = fptr;
				fptr = r_zn_xf_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_xf_tb_t->GetDataByPh(phpos)->fptr = r_zn_xf_tb_t->GetNext();
			r_zn_xf->bptr = phpos;
		}
		//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_zn_xf->id = r_zn_xf_tb_t->GetNext();//���߼�λ����id��
		phpos = r_zn_xf_tb_t->Append(*r_zn_xf);
		if (phpos == -1)//�ж��Ƿ�Խ��
		{
			delete r_zn_xf;
			r_zn_xf = 0;
			return NMD_FAIL;
		}
		delete r_zn_xf;
		r_zn_xf = 0;

		break;


	case _R_ZN_LD:  //zn �����Ǹ�����������
		r_zn_ld = new R_ZN_LD;
		*r_zn_ld = R_ZN_LD(nd_id, br_id);//�������κͿ��صĹ�ϵ
		r_zn_ld->zn_id_ptr = nd_p;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_zn_ld->ld_id_ptr = ld_tb->GetPhByLog(ibr);//���ص�ָ��

		strcpy(r_zn_ld->fdid , fdid);//�ڵ���������
		strcpy(r_zn_ld->ver , ver);//�ڵ���������
		
		phpos = zn_tb_t->GetDataByPh(nd_p)->r_ld;//�õ����α��кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected ld of this zn����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			zn_tb_t->GetDataByPh(nd_p)->r_ld = r_zn_ld_tb_t->GetNext();
		}
		else  //build ld chain for this zn������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_zn_ld_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_zn_ld_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_zn_ld_tb_t->GetDataByPh(phpos)->fptr = r_zn_ld_tb_t->GetNext();
			r_zn_ld->bptr = phpos;
		}
//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		ld_tb->GetDataByPh(ibr)->r_zn = r_zn_ld->id = r_zn_ld_tb_t->GetNext();//���߼�λ����id��
		phpos = r_zn_ld_tb_t->Append(*r_zn_ld);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			delete r_zn_ld;
			r_zn_ld = 0;
			return NMD_FAIL;
		}
		delete r_zn_ld;
		r_zn_ld = 0;
		
		break;

	case _FDS:  //fd �����Ǹ���un����
		r_fd_un = new R_FD_UN;
		*r_fd_un = R_FD_UN(nd_id, br_id);//�������κͿ��صĹ�ϵ
		r_fd_un->fd_id_ptr = nd_p;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_fd_un->un_id_ptr = un_tb->GetPhByLog(ibr);//���ص�ָ��

		//strcpy(r_fd_un->fdid , fdid);//�ڵ���������
		strcpy(r_fd_un->ver , ver);//�ڵ���������
		
		un_tb->GetDataByPh(ibr)->r_fd = r_fd_un_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		phpos = fd_tb->GetDataByPh(nd_p)->r_un;//�õ����α��кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected sec of this zn����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			fd_tb->GetDataByPh(nd_p)->r_un = r_fd_un_tb_t->GetNext();
		}
		else  //build sec chain for this zn������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_fd_un_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_un_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_un_tb_t->GetDataByPh(phpos)->fptr = r_fd_un_tb_t->GetNext();
			r_fd_un->bptr = phpos;
		}
//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_fd_un->id = r_fd_un_tb_t->GetNext();//���߼�λ����id��
		phpos = r_fd_un_tb_t->Append(*r_fd_un);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			delete r_fd_un;
			r_fd_un = 0;
			return NMD_FAIL;
		}
		delete r_fd_un;
		r_fd_un = 0;
		
		break;

	case _R_FD_LD:  //fd �����Ǹ���un����
		r_fd_ld = new R_FD_LD;
		*r_fd_ld = R_FD_LD(nd_id, br_id);//�������κͿ��صĹ�ϵ
		r_fd_ld->fd_id_ptr = nd_p;//���ε�ָ�룬Ҳ�������ε�����λ��
		r_fd_ld->ld_id_ptr = ld_tb->GetPhByLog(ibr);//���ص�ָ��

		//strcpy(r_fd_ld->fdid , fdid);//�ڵ���������
		strcpy(r_fd_ld->ver , ver);//�ڵ���������
		
		ld_tb->GetDataByPh(ibr)->r_fd = r_fd_ld_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
		phpos = fd_tb->GetDataByPh(nd_p)->r_ld;//�õ����α��кͿ��صĹ�����Ϣ
		if(phpos == -1)  //first connected sec of this zn����û�����ӿ���
		{
			//�����Ǹ����������ӡ�
			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
			fd_tb->GetDataByPh(nd_p)->r_ld = r_fd_ld_tb_t->GetNext();
		}
		else  //build sec chain for this zn������ڵ���ǰ�Ѿ������˿���
		{
			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
			fptr = r_fd_ld_tb_t->GetDataByPh(phpos)->fptr;
			while( fptr != -1 )  
			{
				phpos = fptr;
				fptr = r_fd_ld_tb_t->GetDataByPh(phpos)->fptr;
			}
			r_fd_ld_tb_t->GetDataByPh(phpos)->fptr = r_fd_ld_tb_t->GetNext();
			r_fd_ld->bptr = phpos;
		}
//		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
		r_fd_ld->id = r_fd_ld_tb_t->GetNext();//���߼�λ����id��
		phpos = r_fd_ld_tb_t->Append(*r_fd_ld);
		if(phpos == -1)//�ж��Ƿ�Խ��
		{
			delete r_fd_ld;
			r_fd_ld = 0;
			return NMD_FAIL;
		}
		delete r_fd_ld;
		r_fd_ld = 0;
		
		break;

//	case _R_FD_ND:  //fd �����Ǹ���un����
//		r_fd_nd = new R_FD_ND;
//		*r_fd_nd = R_FD_ND(nd_id, br_id);//�������κͿ��صĹ�ϵ
//		r_fd_nd->fd_id_ptr = nd_p;//���ε�ָ�룬Ҳ�������ε�����λ��
//		r_fd_nd->nd_id_ptr = nd_tb_t->GetPhByLog(ibr);//���ص�ָ��
//
//		//strcpy(r_fd_nd->fdid , fdid);//�ڵ���������
//		strcpy(r_fd_nd->ver , ver);//�ڵ���������
//		
//		//nd_tb_t->GetDataByPh(ibr)->r_fd = dsbob->r_fd_nd_tb_t->GetNext();//׼�����뵽��ϵ���е�����λ��
//		phpos = fd_tb_t->GetDataByPh(nd_p)->r_nd;//�õ����α��кͿ��صĹ�����Ϣ
//		if(phpos == -1)  //first connected sec of this zn����û�����ӿ���
//		{
//			//�����Ǹ����������ӡ�
//			//���εĵ�һ������ָ�루�ӣ���ͨ�����ָ����Եõ����е�����������Ŀ���
//			fd_tb_t->GetDataByPh(nd_p)->r_nd = dsbob->r_fd_nd_tb_t->GetNext();
//		}
//		else  //buind sec chain for this zn������ڵ���ǰ�Ѿ������˿���
//		{
//			//������-���ع�ϵ���е�λ�����Ų����׼���µ����ι�ϵָ�����
//			fptr = dsbob->r_fd_nd_tb_t->GetDataByPh(phpos)->fptr;
//			while( fptr != -1 )  
//			{
//				phpos = fptr;
//				fptr = dsbob->r_fd_nd_tb_t->GetDataByPh(phpos)->fptr;
//			}
//			dsbob->r_fd_nd_tb_t->GetDataByPh(phpos)->fptr = dsbob->r_fd_nd_tb_t->GetNext();
//			r_fd_nd->bptr = phpos;
//		}
////		//����󷵻�ָ�����������λ�ã�������һ��ָ��Ĳ���
//		r_fd_nd->id = dsbob->r_fd_nd_tb_t->GetNext();//���߼�λ����id��
//		phpos = dsbob->r_fd_nd_tb_t->Insert(*r_fd_nd);
//		if(phpos == -1)//�ж��Ƿ�Խ��
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
		co = new CO[lv_co];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
		co_tb->CopyTable(co);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

		//reset pointers����ʱ����������м�¼���ֶ�r_st����˾���վ�Ĺ�ϵ��д��-1
		for(i = 0; i < lv_co; i++)
		{
			(co+i)->r_st = -1;
		}

		//update table����ʱ������ļ�¼��д����ʽ������
		co_tb->UpdateTable(co);

		delete[] co;//����ʱ���ڴ��ͷ�
		co = 0;//�����������ֹ�ͷź�ָ����ָ
		lv_co = 0;//���գ��Ժ���Ҫ�������ʱ�����÷���GetCount()�õ���
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
			//��ͷλ��Ϊ����ʱ���룬��ʱ���˳�ʼ����2019-9-6
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
//1.������һ����ÿ��������Ĺ�ϵֵ����r_ld����ԭ���洢������λ�ñ�����߼�λ��
//2.Ȼ��ÿһ���豸������λ�ú��߼�λ��ͳһ���һ��ͳһ����1��������˳������
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
			//��ң�ű�����ĺ�������ع����Ĳ����߼�������ر����棬�������Է�������
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
	////��Ҫ�������
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
	////��Դ�����������
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
		sprintf(str,"�ڵ�����޼�¼!");
		lld->WriteLog(lld->logfile,str);
		return NMD_SUCCEED;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	//mark all nds as unprocessed
	//�����еĽڵ���Ϊδ����׼�����˷�����
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
		sprintf(str,"ĸ�߱����޼�¼!");
		lld->WriteLog(lld->logfile,str);

//������ϵͳ�ж�û��ĸ�ߵ�����Ǵ��ڵģ����ڵ�ĵ�ѹ�����10kV
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
	//�趨ĸ�ߵĵ�ѹֵ��һ�������λ1
	busnd = new int[lv_bus];//���͵�ָ�룬�൱��һ�����������飬��¼һ����ַ
	for(i = 0; i < lv_bus; i++)
	{
		//��ĸ�߱�bus_tb�Ľڵ���ind����˳��д��busndָ����������
		*(busnd+i) = bus_tb->GetDataByLog(i)->ind;
		//bus_tb�е�ind����nd_tb�е���ţ���2���ڽ��б�ŵ�ʱ�����һ�µ� 
		(nd+*(busnd+i))->vl = bus_tb->GetDataByLog(i)->vl;
	}

	//search through nds from bus nd
	//�������
	for(i = 0; i < lv_bus; i++)
	{
		ind = *(busnd+i);//ĸ�ߵ���ʼ�ڵ���
		//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
		if((nd+ind)->q & ND_UNPROC) //this bus nd has not processed
		{
			vl = (nd+ind)->vl;

			//create circular chain for this bus nd
			knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
			(nd+knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
			(nd+knd)->q &= (~ND_UNPROC);//����ǰ�ڵ���Ϊ�Ѵ���

			//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
			while( 1 )
			{
				//read cbs connected to this nd
				//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
				cb_p = r_nd_cb_tb->FindChildPtr1((nd+knd)->r_cb);
				cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
				while(cb_p)
				{
					//find the opposite nd through this cb
					//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
					if(knd == cb_tb->GetDataByPh(cb_p->Ptr)->ind)
					{
						kknd = cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
					}
					else
					{
						kknd = cb_tb->GetDataByPh(cb_p->Ptr)->ind;
					}

					//process this opposite nd
					//����Բ�ڵ� δ����
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
								sprintf(str,"�ڵ�(%s)��ĸ�ߵ�ѹ�ȼ����ܴ���!",(nd+kknd)->id);
								lld->WriteLog(lld->logfile,str);
							}
						}

						//ѭ�������ʹ�ã�ÿ��չһ���µĽڵ㣬��2���£�
						//1.����һ���ڵ��ǰ������Ϊ�Լ���ǰ��������ʵ�ǵ�һ���ڵ���Զ�����нڵ��ǰ����
						//2.�޸���һ���ڵ��ǰ���������Լ���Ϊ��һ���ڵ��ǰ����

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

				//ѭ����ͷ���������ˣ���ʾȫ������һ��
				//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
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
		sprintf(str,"�ڵ�����޼�¼!");
		return NMD_SUCCEED;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	//mark all nds as unprocessed
	//�����еĽڵ���Ϊδ����׼�����˷�����
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

//׷��г��
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
		sprintf(str, "�ڵ�����޼�¼!");
		lld->WriteLog(lld->logfile, str);
		return NMD_SUCCEED;
	}

	//read all nds into buffer
	nd = new ND[lv_nd];
	nd_tb->CopyTable(nd);

	lld->trim_return(hwSub);
	//mark all nds as unprocessed
	//�����еĽڵ���Ϊδ����׼�����˷�����
	for (i = 0; i < lv_nd; i++)
	{
		(nd + i)->q |= ND_UNPROC;

		//�ҵ�׷�ٽڵ�
		if (strcmp(hwSub, (nd + i)->id) == 0)
		{
			ind = i;
			strcpy(trace_vl,(nd + i)->ver);
		}
	}

	//search through nds from bus nd
	//�������
	//֮ǰ�Ѿ������еĽڵ㶼��Ϊδ����
	if ((nd + ind)->q & ND_UNPROC) //this bus nd has not processed
	{
		//create circular chain for this bus nd
		knd = ind;//��ǰ�ڵ㸳��һ����ʱ�ı���
		(nd + knd)->fch = knd; //��ǰ�ڵ�Ÿ���ǰ���� 
		(nd + knd)->q &= (~ND_UNPROC);//����ǰ�ڵ���Ϊ�Ѵ���

		//�õ�ǰ�ڵ�������չ���ҵ����к��������Ľڵ�
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
						//������
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

			//ѭ����ͷ���������ˣ���ʾȫ������һ��
			//������Ľڵ��ٿ�ʼ������һֱ�����еĽڵ㶼����
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
















