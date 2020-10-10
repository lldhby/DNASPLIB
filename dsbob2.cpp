// DSBOB2.cpp: implementation of the DSBOB2 class.
//
//////////////////////////////////////////////////////////////////////
#if _WIN32
//#include "..\DPCAS4.1\WINDLL\calcdll\lib.h"
#include <process.h>  
#include <windows.h>  
#include <fcntl.h>  
#include <io.h>  
#elif __linux__
#include <cstdio.h>  
#include <pthread.h>  //linux
#else
#endif

#include "../DNASPLIB/LLD.h"
#include "../DNASPLIB/dsbob2.h"

#include <fstream>
#include "math.h"
#include "string.h"

//#include <vld.h>	//����Ƿ��ڴ�й¶

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const int MAX_NW_ND=100000; 


DSBOB2::DSBOB2()
{
	//����ı���Ҫ����ָ����ȫ��Ψһ
	co_tb           = new DTable<CO>(lld->mx_co);
	global_tb       = new DTable<GLOBAL>(lld->mx_global);
}

DSBOB2::DSBOB2(CLLD* m_lld)
{
	lld = m_lld;
	//����ı���Ҫ����ָ����ȫ��Ψһ
	co_tb = new DTable<CO>(lld->mx_co);
	global_tb = new DTable<GLOBAL>(lld->mx_global);

}

DSBOB2::~DSBOB2()
{
	delete co_tb;
	delete global_tb;
}

float DSBOB2::getVLBymrID(char * mrID, int type)
{
	int ph, i;
	float ret_vl;
	char 	vl[32];
	ret_vl = -1;

	//if the parameter type equals 1, then it is basevoltage,such as 110kV,220kV.etc
	if (type == 1)
	{
		for (i = 0; i < 10; i++)
		{
			if (strcmp(mrID, basevl[i].mRID) == 0)
			{
				return basevl[i].nomkV / 1000;//����ѹ�ȼ�����
			}
		}
		return -1;
	}


	//���ݵ�ѹ�ȼ���ID�ţ���vl�������ҵ���صĻ�׼��ѹֵ
	//�˴��Ļ�׼��ѹ���ڱ���ֵ�ļ���

	VL* vl_t = new VL();
	strcpy(vl_t->mRID, mrID);//�����߶�

	ph = vl_tb->FindPh(*vl_t);
	if (ph != -1)
	{
		strcpy(vl, vl_t->BaseVoltage);
		for (i = 0; i < 10; i++)
		{
			if (strcmp(vl, basevl[i].mRID) == 0)
			{
				ret_vl = basevl[i].nomkV / 1000;//����ѹ�ȼ�����
				break;
			}
		}
	}

	delete vl_t;

	return ret_vl;
}

int DSBOB2::getDevType(char * line,char * devType,int& n_line,int &flag_b)
{
	char  flagField_b[32], flagField_e[32],devB[32], devE[32];
	//int flag_b;
	//int n_cb, n_sec, n_secp, n_bus, n_xf, n_xfmr, n_ld, n_line, n_st, n_so, n_nd, n_equip;

	//����
	strcpy(devB, "<");
	strcat(devB, devType);
	strcpy(flagField_b, devB);

	strcpy(devE, "</");
	strcat(devE, devType);
	strcpy(flagField_e, devE);

	if (strstr(line, flagField_b) != NULL)
	{
		n_line = 0;
		flag_b = 1;

		return 1;
	}
	else
	if (strstr(line, flagField_e) != NULL)
	{
		n_line = flag_b = 0;

		return 2;
	}

	return 0;
}

int DSBOB2::ReadDataFromFile(char* FileName,char* optParam)
{
	int ii, ph;
	int cnt_t, cnt_un = 0, cnt_fd = 0, cnt_mainso = 0;
	int cnt_so = 0, cnt_cb = 0, cnt_sec = 0, cnt_ld = 0, cnt_nd = 0;
	char unfd[2 * MAX_ID_DLL];//���翪�ؿ϶���������������
	char temp_[2 * MAX_ID_DLL];//���翪�ؿ϶���������������
	temp_[0] = 0;

	//�˴���Ҫ��������ֵ
	lld->mx_so = 400;
	lld->mx_cb = 50000;
	lld->mx_sec = 50000;
	lld->mx_ld = 10000;
	lld->mx_nd = 50000;

	fd_tb_t = new DTable<FD>(lld->mx_so + lld->mx_cb);
	DTable<UN>* un_tb_t = new DTable<UN>(lld->mx_cb);
	so_tb = new DTable<SO>(lld->mx_so);
	cb_tb = new DTable<CB>(lld->mx_cb);
	ld_tb = new DTable<LD>(lld->mx_ld);
	sec_tb = new DTable<SEC>(lld->mx_sec);
	nd_tb = new DTable<ND>(lld->mx_nd);
	//xfmr_tb = new DTable<XFMR>(lld->mx_xfmr);
	//cp_tb = new DTable<CP>(lld->mx_cp);
	//bus_tb = new DTable<BUS>(lld->mx_bus);
	//tapty_tb = new DTable<TAPTY>(lld->mx_tapty);
	lld->mx_cp = lld->mx_bus = 0;

	//ǰ����׼������Ĳ�������������ʵ�����ݣ��м���==����
	char line[512];
	char type[16];
	char   *p;
	FILE *fp;
	fp = fopen(FileName, "r");
	if (fp == NULL)
		return -1;

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		p = line;

		//��ȡ��������
		if (strstr(line, "optparam:") != NULL)
			strcpy(optParam, line);

		if (strstr(line, "type:") != NULL)
		{
			sscanf(p, "%*s%d", &lld->g_mathType);
		}

		//��ȡ��Դ����
		if (strstr(line, "so:") != NULL)
		{
			SO* sot = new SO;
			//			�豸����	�豸ID��	�豸����	�ڵ��	������ѹ	�ڵ�����	
					//  ��λ����ʱ��(h)	�л�ʱ��	ת��ʱ��	
					//	Ԥ���Ÿ���ʱ��	Ԥ�����л�ʱ��	Ԥ����ת��ʱ��	
					//	Ԥ������	Ԥ����ͣ��ʱ��	
					//	������	�޸�ʱ��	
					//	�׶��������(A)	��������������(A)	��·����(MVA)	��·rx��	
					//	��������	��ѹ�ȼ�
//		so:	0 - 0	10.5	4	1	0.3	0.5	0.1	0.1	0.1	6	7	0.5	8	0	450	300	0.3	 testSG 10

			sscanf(p, "%*s%s%*s%s%f%d%f%f%f%f%f%f%f%f%f%f%f%f%f%f%s%s",
				sot->id, sot->nd,
				&sot->vtarget, &sot->slackavr,
				&sot->fault_ltime, &sot->fault_stime, &sot->fault_ttime,
				&sot->plan_ltime, &sot->plan_stime, &sot->plan_ttime,
				&sot->plan_prob, &sot->plan_rtime, &sot->prob_fault, &sot->repairTime,
				&sot->I_value, &sot->maxcurrent, &sot->ss, &sot->sr,
				sot->fdid, sot->vl);
			if (sot->slackavr == 4)//���ڵ㣬һ���ǵ����ĳ��߽ڵ�
			{
				FD* fdt = new FD;

				strcpy(fdt->id, sot->fdid);
				fdt->maxcurrent = sot->maxcurrent;//������
				strcpy(fdt->vl, sot->vl);//��ѹ�ȼ�

				//����ʵ����׶˵�������λ��kA�����õ�ʱ����Ҫ����ת��Ϊ�й����޹�
				fdt->I_value = sot->I_value;

				if (lld->isGroupParam == 1)
				{
					fdt->fault_ltime = atof(lld->fault_ltime) / 60;//Сʱ��ԭʼ��дΪ����
					fdt->fault_stime = atof(lld->fault_stime) / 60;//������λ����ʱ��
					fdt->fault_ttime = atof(lld->fault_ttime) / 60;//������λ����ʱ��
					fdt->plan_prob = atof(lld->plan_prob) / 100000;//��/��/100ǧ��
					fdt->plan_rtime = atof(lld->plan_rtime);//ԭʼ��дΪСʱ���Ѻ���λ����ʱ��
					fdt->plan_ltime = atof(lld->plan_ltime) / 60;//����ʱ��
					fdt->plan_stime = atof(lld->plan_stime) / 60;//��������ʱ��
					fdt->plan_ttime = atof(lld->plan_ttime) / 60;//��������ʱ��
				}
				else
				{
					fdt->fault_ltime = sot->fault_ltime / 60;//���϶�λʱ�䣬��СʱΪ��λ
					fdt->fault_stime = sot->fault_stime / 60;//�л�ʱ�䣨���λָ�ʱ�䣩��������λʱ�䣬Сʱ
					fdt->fault_ttime = sot->fault_ttime / 60;//ת��ʱ�䣨���λָ�ʱ�䣩��������λʱ�䣬Сʱ

					//�л�ʱ��
					fdt->plan_ltime = sot->plan_ltime / 60;//Ԥ���Ÿ���ʱ�䣬��СʱΪ��λ��Ԥ����û�ж�λʱ�䣩
					fdt->plan_stime = sot->plan_stime / 60;
					fdt->plan_ttime = sot->plan_ttime / 60;

					fdt->plan_prob = sot->plan_prob / 100000;//Ԥ������,(��/�ٹ�����)
					fdt->plan_rtime = sot->plan_rtime;//ƽ��Ԥ����ͣ�˳���ʱ��(h)
				}

				ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
				if (ph == -1)
				{
					fdt->i_id = cnt_fd;
					fd_tb_t->Append(*fdt);
					cnt_fd++;
				}
				else
				{
					fdt->i_id = ph;
					fd_tb_t->UpdateDataByPh(fdt, ph);
				}

				delete fdt;
				cnt_mainso++;
			}
			else if (sot->slackavr == 1)	//PQ�㶨
			{
			}
			else if (sot->slackavr == -1)//V�㶨
			{
				sot->v = sot->vtarget;
			}
			else if (sot->slackavr == -2)//I�㶨
			{
			}

			so_tb->Append(*sot);
			cnt_so++;
			delete sot;
		}

		//��ȡ��������
		if (strstr(line, "cb:") != NULL)
		{
			CB* cbt = new CB;
			//�豸����	�豸ID��	�豸����	�׽ڵ�	ĩ�ڵ�	��������	q	��
			//ʩ����ͣ���ʣ��� / �ٹ����̨)�꣩	ƽ�������޸�ʱ�䣨h��	
			//��������	��ѹ�ȼ�
//		cb:	Fa - 21	31	1	1024	0.2	2	testSG	10
			sscanf(p, "%*s%s%*s%s%s%*d%d%f%f%s%*s", cbt->id, cbt->nd, cbt->znd,
				&cbt->q, &cbt->prob_fault, &cbt->repairTime, cbt->fdid);

			if (Q_BREAKERRELAY & cbt->q)
				cbt->cbtype = 5;
			else if (Q_BREAKER & cbt->q)
				cbt->cbtype = 0;
			else if (Q_LOADBREAKSWITCH & cbt->q)
				cbt->cbtype = 1;
			else if (Q_DISCONNECTOR & cbt->q)
				cbt->cbtype = 2;
			else if (Q_FUSE & cbt->q)
				cbt->cbtype = 3;

			if (lld->isGroupParam == 1)
			{
				if (cbt->cbtype == 0)
				{
					cbt->prob_fault = atof(lld->fault_breaker_prob) / 100;//  ��/̨/��
					cbt->repairTime = atof(lld->fault_breaker_rtime);
				}
				else
					if (cbt->cbtype == 1)
					{
						cbt->prob_fault = atof(lld->fault_loadbreakswitch_prob) / 100;
						cbt->repairTime = atof(lld->fault_loadbreakswitch_rtime);
					}
					else
						if (cbt->cbtype == 2)
						{
							cbt->prob_fault = atof(lld->fault_disconnector_prob) / 100;
							cbt->repairTime = atof(lld->fault_disconnector_rtime);
						}
						else
							if (cbt->cbtype == 3)
							{
								cbt->prob_fault = atof(lld->fault_fuse_prob) / 100;
								cbt->repairTime = atof(lld->fault_fuse_rtime);
							}
							else
								if (cbt->cbtype == 5)
								{
									cbt->prob_fault = atof(lld->fault_breaker_prob) / 100;
									cbt->repairTime = atof(lld->fault_breaker_rtime);
								}
			}
			else
			{
				cbt->prob_fault = cbt->prob_fault / 100;
			}

			//lld edit end for tangshan 2016-11-9
			if (cbt->q & Q_BREAKERFEEDER)
			{
				cbt->cbtype = 5;//���߿��ر�Ȼ������
			}

			//���翪��û�й����ʣ������翪�ز��������ɿ���
			if (cbt->q & Q_UNIONCB)
			{
				cbt->cbtype = 4;

				cbt->prob_fault = 0;
				cbt->repairTime = 0;

				UN* unt = new UN;
				strcpy(unt->id, cbt->id);
				//��ȡ���翪�ص���������ID
				strcpy(unfd, cbt->fdid);
				if (lld->isContainor(unfd, '|') != -1)
				{
					lld->getleftstr(unfd, '|', temp_);

					//���߱����Ƿ�����������ߣ����û�У������
					if (strlen(temp_) > 0)
					{
						FD* fdt = new FD;
						strcpy(fdt->id, temp_);
						ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
						if (ph == -1)
						{
							fdt->i_id = cnt_fd;

							fd_tb_t->Append(*fdt);
							cnt_fd++;
						}
						else
						{
							//fd_tb_t->UpdateDataByPh(fdt,ph);
						}
						delete fdt;

						strcpy(unt->left, temp_);
						lld->getrightstr(unfd, '|', temp_);
						strcpy(unt->right, temp_);

						fdt = new FD;
						strcpy(fdt->id, temp_);
						ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
						if (ph == -1)
						{
							fdt->i_id = cnt_fd;
							fd_tb_t->Append(*fdt);
							cnt_fd++;
						}
						else
						{
							//fd_tb_t->UpdateDataByPh(fdt,ph);
						}
						delete fdt;
					}
				}
				else
				{
					strcpy(unt->left, unfd);
					strcpy(unt->right, unfd);//�Բ���Ĭ�ϵ�����
				}

				un_tb_t->Append(*unt);
				cnt_un++;
				delete unt;
			}

			cb_tb->Append(*cbt);
			cnt_cb++;
			delete cbt;
		}

		//if (cntBUS > 0)
		//{
		//	bus_tb = new DTable<BUS>(lld->mx_bus);
		//	cnt_t = 0;
		//	for (ii = 0; ii<cntBUS; ii++)
		//	{
		//		BUS* bust = new BUS;
		//		strcpy(bust->id, (bus + ii)->id);
		//		strcpy(bust->nd, (bus + ii)->nd);
		//		bust->vl = (bus + ii)->kv;

		//		bus_tb->Append(*bust);
		//		cnt_t++;
		//		delete bust;
		//	}
		//	lld->mx_bus = bus_tb->Sort(cnt_t);
		//}
		//else
		//	lld->mx_bus = 0;


		//if (cntCP > 0)
		//{
		//	cp_tb = new DTable<CP>(lld->mx_cp);
		//	bus_tb = new DTable<BUS>(lld->mx_bus);

		//	cnt_t = 0;
		//	for (ii = 0; ii<cntCP; ii++)
		//	{
		//		CP* cpt = new CP;
		//		strcpy(cpt->id, (cp + ii)->id);
		//		strcpy(cpt->nd, (cp + ii)->nd);
		//		strcpy(cpt->fdid, (cp + ii)->fdid);
		//		strcpy(cpt->vl, (cp + ii)->vl);

		//		cpt->r = (cp + ii)->r;//����ĵ�λ��MVar

		//		cp_tb->Append(*cpt);
		//		cnt_t++;
		//		delete cpt;
		//	}
		//	lld->mx_cp = cp_tb->Sort(cnt_t);
		//}
		//else
		//	lld->mx_cp = 0;

		//��ȡ��������
		double ld_w, ld_r;
		if (strstr(line, "ld:") != NULL)
		{
			LD* ldt = new LD;
			//�豸����	�豸ID��	�豸����	�׽ڵ�	ʵ������	�����	
			//q����Ҫ�û���˫��Դ)	��ʩ����ͣ���ʣ���/�ٹ����̨)�꣩	ƽ�������޸�ʱ�䣨h��
			//��������	��ѹ�ȼ�

			sscanf(p, "%*s%s%*s%s%f%f%f%d%f%f%s%s", ldt->id, ldt->nd,
				&ldt->w, &ldt->r, &ldt->wm, &ldt->type, &ldt->prob_fault, &ldt->repairTime,
				ldt->fdid, ldt->vl);

			if (ldt->type & LD_SO)
				ldt->topflags |= LD_SO;//���Դ����
			else
				ldt->topflags |= LD_LEVEL2;//Ĭ�϶�������

			ldt->q = ldt->wm;
			ldt->wm = 0;
			ld_w = ldt->w;
			ld_r = ldt->r;

			//�ö���������
			if (ld_w == -1)
			{
				ld_w = ld_r * sqrt(1 - 0.15*0.15);
				ld_r *= 0.15;
				ldt->wm = ((double)(ldt->q) / 100) * ld_w;//��ѹ�����й����⣬��λkW
				ldt->rm = ((double)(ldt->q) / 100) * ld_r;//��ѹ�����޹�����
			}
			else
			{
				ld_r = ld_w * 0.15;
				ldt->wm = ld_w;//��ѹ�����й����⣬��λkW
				ldt->rm = ld_r;//��ѹ�����޹�����
			}

			ldt->q = 0;//���õı�־λ���㣬���������ܻ��д���

			if (lld->isGroupParam == 1)
			{
				ldt->prob_fault = atof(lld->fault_transformer_prob) / 100;
				ldt->repairTime = atof(lld->fault_transformer_rtime);
			}
			else
			{
				ldt->prob_fault = ldt->prob_fault / 100;
			}

			ld_tb->Append(*ldt);
			cnt_ld++;
			delete ldt;
		}

		//��ȡ���߶�����
		if (strstr(line, "sec:") != NULL)
		{
			SEC* sect = new SEC;

			//�豸����	�豸ID��	�豸����	�׽ڵ�	ĩ�ڵ�	
			//����	�翹	��·����	q	��ʩ����ͣ���ʣ��� / �ٹ����̨)�꣩	
			//ƽ�������޸�ʱ�䣨h��	��������	��ѹ�ȼ�
			sscanf(p, "%*s%s%*s%s%s%f%f%f%d%f%f%s%s", sect->id, sect->nd, sect->znd,
				&sect->r1, &sect->x1, &sect->len, &sect->q, &sect->prob_fault, &sect->repairTime,
				sect->fdid, sect->vl);

			//if (Q_CABLE & sect->q)
			//	sect->q = 0;
			//else if (Q_INSULATION & sect->q)
			//	sect->q = 1;
			//else if (Q_ACLINESEGMENT & sect->q)
			//	sect->q = 2;

			if (sect->len == 0)//����Ϊ0���߶Σ����迹֧·
			{
				sect->r1 = 0;//����
				sect->x1 = 0;
				sect->b1 = 0;
			}
			else
			{
				sect->r1 = (sect->len / 1000) * sect->r1;//ŷķ/1ǧ��
				sect->x1 = (sect->len / 1000) * sect->x1;
				//sect->b1 = (sect->len / 1000) * (sec+ii)->b1;
			}

			if (lld->isGroupParam == 1)
			{
				if (sect->q & Q_CABLE)//����
				{
					sect->prob_fault = sect->len * atof(lld->fault_cable_prob) / 100000;//ת���� ��/��/100ǧ��
					sect->repairTime = atof(lld->fault_cable_rtime);
				}
				else if (sect->q & Q_ACLINESEGMENT)//�ܿվ�Ե��
				{
					sect->prob_fault = sect->len * atof(lld->fault_ohline_ins_prob) / 100000;//ת���ɴ�/��
					sect->repairTime = atof(lld->fault_ohline_ins_rtime);
				}
				else//�ɿ��Դ��в���,�ĵ���ǰ�����ݣ�����
				{
					sect->prob_fault = sect->len * atof(lld->fault_ohline_prob) / 100000;//ת���ɴ�/��
					sect->repairTime = atof(lld->fault_ohline_rtime);
				}
			}
			else
			{
				sect->prob_fault = sect->len * sect->prob_fault / 100000;//ת���ɴ�/��
				sect->repairTime = sect->repairTime;
			}

			//��������˳���Ϊ0���������Ҫ��һ��Ĭ��ֵ
			if ((sect->len == 0) || (sect->len == 1))
			{
				sect->prob_fault = 0;//ת���ɴ�/��/��
			}

			if (sect->repairTime == 0)//û�������ʱ��
			{
				if (strcmp(sect->algii, "0") == 0)//����
					sect->repairTime = 5.5;
				else
					sect->repairTime = 2.5;
			}
			sec_tb->Append(*sect);
			cnt_sec++;
			delete sect;
		}
		//�������еı�ѹ�������������
		//if (cntXFMR > 0)
		//{
		//	xfmr_tb = new DTable<XFMR>(cntXFMR);
		//	cp_tb = new DTable<CP>(lld->mx_cp);
		//	bus_tb = new DTable<BUS>(lld->mx_bus);

		//	cnt_t = 0;
		//	for (ii = 0; ii<cntXFMR; ii++)
		//	{
		//		XFMR* xfmrt = new XFMR;
		//		//�������迹֧·�����������һ�˽��и��ɵ㣬����ֱ�Ӵ����
		//		strcpy(xfmrt->id, (xfmr + ii)->id);
		//		strcpy(xfmrt->descr, (xfmr + ii)->descr);
		//		xfmrt->type = (xfmr + ii)->type;
		//		xfmrt->rh = (xfmr + ii)->rh;
		//		xfmrt->xh = (xfmr + ii)->xh;
		//		xfmrt->xm = (xfmr + ii)->xm;
		//		xfmrt->rm = (xfmr + ii)->rm;
		//		xfmrt->rl = (xfmr + ii)->rl;
		//		xfmrt->xl = (xfmr + ii)->xl;

		//		xfmrt->kvnomh = (xfmr + ii)->kvnomh;
		//		xfmrt->kvnomm = (xfmr + ii)->kvnomm;
		//		xfmrt->kvnoml = (xfmr + ii)->kvnoml;
		//		xfmrt->mvah = (xfmr + ii)->mvah;
		//		xfmrt->mvam = (xfmr + ii)->mvam;
		//		xfmrt->mval = (xfmr + ii)->mval;

		//		strcpy(xfmrt->hnd, (xfmr + ii)->hnd);
		//		strcpy(xfmrt->mnd, (xfmr + ii)->mnd);
		//		strcpy(xfmrt->lnd, (xfmr + ii)->lnd);

		//		//��ͷ����
		//		strcpy(xfmrt->htapty, (xfmr + ii)->htapty);
		//		strcpy(xfmrt->mtapty, (xfmr + ii)->mtapty);
		//		strcpy(xfmrt->ltapty, (xfmr + ii)->ltapty);

		//		//��ͷλ��
		//		xfmrt->htap = (xfmr + ii)->htap;
		//		xfmrt->mtap = (xfmr + ii)->mtap;
		//		xfmrt->ltap = (xfmr + ii)->ltap;

		//		xfmrt->q = (xfmr + ii)->q;
		//		//			xfmrt->topflags = (xfmr+ii)->topflags;
		//		//			xfmrt->r_xf = (xfmr+ii)->r_xf;

		//		strcpy(xfmrt->fdid, (xfmr + ii)->fdid);

		//		xfmrt->prob_fault = (xfmr + ii)->prob_fault / 100;
		//		xfmrt->repairTime = (xfmr + ii)->repairTime;

		//		xfmr_tb->Append(*xfmrt);
		//		cnt_t++;
		//		delete xfmrt;
		//	}
		//	lld->mx_xfmr = xfmr_tb->Sort(cnt_t);//���ݼ����������
		//}
		//else
		//	lld->mx_xfmr = 0;

		//if (cntTAPTY > 0)
		//{
		//	xfmr_tb = new DTable<XFMR>(cntXFMR);
		//	cp_tb = new DTable<CP>(lld->mx_cp);
		//	bus_tb = new DTable<BUS>(lld->mx_bus);
		//	tapty_tb = new DTable<TAPTY>(cntTAPTY);

		//	cnt_t = 0;
		//	for (ii = 0; ii<cntTAPTY; ii++)
		//	{
		//		TAPTY* taptyt = new TAPTY;
		//		strcpy(taptyt->id, (tapty + ii)->id);
		//		taptyt->mn = (tapty + ii)->mn;
		//		taptyt->mx = (tapty + ii)->mx;
		//		taptyt->nom = (tapty + ii)->nom;
		//		taptyt->step = (tapty + ii)->step;

		//		tapty_tb->Append(*taptyt);
		//		cnt_t++;
		//		delete taptyt;
		//	}
		//	tapty_tb->Sort(cnt_t);//���ݼ����������
		//}

		//������ڵ�
		if (strstr(line, "nd:") != NULL)
		{
			ND* ndt = new ND;

			//�豸����	�豸ID��	��־λ	��ѹ�ȼ�
			sscanf(p, "%*s%s%d%d", ndt->id, &ndt->q, &ndt->vl);
			ndt->q = 0;//���õı�־λ���㣬���������ܻ��д���

			nd_tb->Append(*ndt);
			cnt_nd++;
			delete ndt;
		}
	}

	//�����ռ�
	lld->mx_fd = fd_tb_t->Sort(cnt_fd);
	fd_tb = new DTable<FD>(lld->mx_fd);
	if (lld->mx_fd != 0)
	{
		FD* temp_ptr = new FD[lld->mx_fd];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
		fd_tb_t->CopyTable(temp_ptr);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

										//update table����ʱ������ļ�¼��д����ʽ������
		fd_tb->Insert(temp_ptr, lld->mx_fd);

		delete[] temp_ptr;//����ʱ���ڴ��ͷ�
		temp_ptr = 0;//�����������ֹ�ͷź�ָ����ָ
	}
	delete fd_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	fd_tb_t = 0;

	if (cnt_un > 0)
	{
		lld->mx_un = un_tb_t->Sort(cnt_un);
		un_tb = new DTable<UN>(lld->mx_un);
		if (lld->mx_un != 0)
		{
			UN* temp_ptr = new UN[lld->mx_un];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			un_tb_t->CopyTable(temp_ptr);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

											//update table����ʱ������ļ�¼��д����ʽ������
			un_tb->Insert(temp_ptr, lld->mx_un);

			delete[] temp_ptr;//����ʱ���ڴ��ͷ�
			temp_ptr = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete un_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		un_tb_t = 0;
	}

	lld->mx_so = so_tb->Sort(cnt_so);
	lld->mx_cb = cb_tb->Sort(cnt_cb);
	lld->mx_sec = sec_tb->Sort(cnt_sec);
	if (cnt_ld > 0)
		lld->mx_ld = ld_tb->Sort(cnt_ld);
	else
		lld->mx_ld = 0;

	lld->mx_nd = nd_tb->Sort(cnt_nd);

	fclose(fp);

	return 1;
}

#if _WIN32

//int DSBOB2::ReadData(
//	SOT* so, int cntSO,
//	CBT* cb, int cntCB,
//	BUST* bus, int cntBUS,
//	SECT* sec, int cntSEC,
//	XFMRT* xfmr, int cntXFMR,
//	TAPTYT* tapty, int cntTAPTY,
//	LDT* ld, int cntLD,
//	CPT* cp, int cntCP,
//	NDT* nd, int cntND
//)
//{
//	int ii, ph;
//	int cnt_t, cnt_un = 0, cnt_fd = 0, cnt_mainso = 0;
//	char unfd[2 * MAX_ID_DLL];//���翪�ؿ϶���������������
//	char  temp_[2 * MAX_ID_DLL];//���翪�ؿ϶���������������
//	temp_[0] = '\0';
//
//	fd_tb_t = new DTable<FD>(cntSO + cntCB);
//	DTable<UN>* un_tb_t = new DTable<UN>(cntCB);
//
//	//��Դ�����߲���һ��һ�Ĺ�ϵ
//	so_tb = new DTable<SO>(cntSO);
//	if (cntSO > 0)
//	{
//		cnt_t = 0;
//		for (ii = 0; ii < cntSO; ii++)
//		{
//			SO* sot = new SO;
//			strcpy(sot->id, (so + ii)->id);
//			//strcpy(sot->descr,m_Set.m_coldata[1].value);
//			strcpy(sot->nd, (so + ii)->nd);
//			strcpy(sot->fdid, (so + ii)->fdid);
//			strcpy(sot->vl, "10");//��ѹ�ȼ�
////			strcpy(sot->vl, (so + ii)->vl);//��ѹ�ȼ�
//			sot->slackavr = (so + ii)->slackavr;
//			sot->ss = (so + ii)->ss;
//
//			if ((so + ii)->slackavr == 4)//���ڵ㣬һ���ǵ����ĳ��߽ڵ�
//			{
//				FD* fdt = new FD;
//
//				strcpy(fdt->id, (so + ii)->fdid);
//				sot->maxcurrent = (so + ii)->MAXCURRENT;
//				fdt->maxcurrent = (so + ii)->MAXCURRENT;//������
//
//				//(so+ii)->vl[0] = 0;
//				strcpy(fdt->vl, "10");//��ѹ�ȼ�
//				//strcpy(fdt->vl, (so + ii)->vl);//��ѹ�ȼ�
//
//				//����ʵ����׶˵�������λ��kA�����õ�ʱ����Ҫ����ת��Ϊ�й����޹�
//				sot->I_value = (so + ii)->I_value;
//				fdt->I_value = (so + ii)->I_value;
//
//				if (lld->isGroupParam == 1)
//				{
//					fdt->fault_ltime = atof(lld->fault_ltime) / 60;//Сʱ��ԭʼ��дΪ����
//					fdt->fault_stime = atof(lld->fault_stime) / 60;//������λ����ʱ��
//					fdt->fault_ttime = atof(lld->fault_ttime) / 60;//������λ����ʱ��
//					fdt->plan_prob = atof(lld->plan_prob) / 100000;//��/��/100ǧ��
//					fdt->plan_rtime = atof(lld->plan_rtime);//ԭʼ��дΪСʱ���Ѻ���λ����ʱ��
//					fdt->plan_ltime = atof(lld->plan_ltime) / 60;//����ʱ��
//					fdt->plan_stime = atof(lld->plan_stime) / 60;//��������ʱ��
//					fdt->plan_ttime = atof(lld->plan_ttime) / 60;//��������ʱ��
//				}
//				else
//				{
//					fdt->fault_ltime = (so + ii)->fault_ltime / 60;//���϶�λʱ�䣬��СʱΪ��λ
//					fdt->fault_stime = (so + ii)->fault_stime / 60;//�л�ʱ�䣨���λָ�ʱ�䣩��������λʱ�䣬Сʱ
//					fdt->fault_ttime = (so + ii)->fault_ttime / 60;//ת��ʱ�䣨���λָ�ʱ�䣩��������λʱ�䣬Сʱ
//
//					//�л�ʱ��
//					fdt->plan_ltime = (so + ii)->plan_ltime / 60;//Ԥ���Ÿ���ʱ�䣬��СʱΪ��λ��Ԥ����û�ж�λʱ�䣩
//					fdt->plan_stime = (so + ii)->plan_stime / 60;
//					fdt->plan_ttime = (so + ii)->plan_ttime / 60;
//
//					fdt->plan_prob = (so + ii)->plan_prob / 100000;//Ԥ������,(��/�ٹ�����)
//					fdt->plan_rtime = (so + ii)->plan_rtime;//ƽ��Ԥ����ͣ�˳���ʱ��(h)
//				}
//
//				ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
//				if (ph == -1)
//				{
//					fdt->i_id = cnt_fd;
//					fd_tb_t->Append(*fdt);
//					cnt_fd++;
//				}
//				else
//				{
//					fdt->i_id = ph;
//					fd_tb_t->UpdateDataByPh(fdt, ph);
//				}
//
//				delete fdt;
//				cnt_mainso++;
//			}
//			else
//				if ((so + ii)->slackavr == 1)	//PQ�㶨
//				{
//					sot->w = (so + ii)->w;
//					sot->r = (so + ii)->r;
//				}
//				else
//					if ((so + ii)->slackavr == -1)//V�㶨
//					{
//						sot->w = (so + ii)->w;
//						sot->v = (so + ii)->vtarget;
//					}
//					else
//						if ((so + ii)->slackavr == -2)//I�㶨
//						{
//							sot->w = (so + ii)->w;
//							sot->i = (so + ii)->i;
//						}
//
//			sot->ss = (so + ii)->ss;
//			sot->sr = (so + ii)->sr;
//
//			sot->vtarget = (so + ii)->vtarget;
//
//			sot->prob_fault = (so + ii)->prob_fault / 100;
//			sot->repairTime = (so + ii)->repairTime;
//
//			so_tb->Append(*sot);
//			cnt_t++;
//			delete sot;
//		}
//		lld->mx_so = so_tb->Sort(cnt_t);
//	}
//	else
//		lld->mx_so = 0;
//
//	//cb
//	if (cntCB > 0)
//	{
//		cb_tb = new DTable<CB>(cntCB);
//		cnt_t = 0;
//		for (ii = 0; ii < cntCB; ii++)
//		{
//			CB* cbt = new CB;
//			strcpy(cbt->id, (cb + ii)->id);
//			strcpy(cbt->nd, (cb + ii)->nd);
//			strcpy(cbt->znd, (cb + ii)->znd);
//			strcpy(cbt->fdid, (cb + ii)->fdid);
//			cbt->q = (cb + ii)->q;
//
//			if (Q_BREAKER & cbt->q)
//			{
//				if (Q_BREAKERRELAY & cbt->q)
//					cbt->cbtype = 5;
//				else
//					cbt->cbtype = 0;
//			}
//			else if (Q_LOADBREAKSWITCH & cbt->q)
//			{
//				if (Q_BREAKERRELAY & cbt->q)
//					cbt->cbtype = 5;
//				else
//					cbt->cbtype = 1;
//			}
//			else if (Q_DISCONNECTOR & cbt->q)
//				cbt->cbtype = 2;
//			else if (Q_FUSE & cbt->q)
//				cbt->cbtype = 3;
//
//			if (lld->isGroupParam == 1)
//			{
//				if (cbt->cbtype == 0)
//				{
//					cbt->prob_fault = atof(lld->fault_breaker_prob) / 100;//  ��/̨/��
//					cbt->repairTime = atof(lld->fault_breaker_rtime);
//				}
//				else
//					if (cbt->cbtype == 1)
//					{
//						cbt->prob_fault = atof(lld->fault_loadbreakswitch_prob) / 100;
//						cbt->repairTime = atof(lld->fault_loadbreakswitch_rtime);
//					}
//					else
//						if (cbt->cbtype == 2)
//						{
//							cbt->prob_fault = atof(lld->fault_disconnector_prob) / 100;
//							cbt->repairTime = atof(lld->fault_disconnector_rtime);
//						}
//						else
//							if (cbt->cbtype == 3)
//							{
//								cbt->prob_fault = atof(lld->fault_fuse_prob) / 100;
//								cbt->repairTime = atof(lld->fault_fuse_rtime);
//							}
//							else
//								if (cbt->cbtype == 5)
//								{
//									cbt->prob_fault = atof(lld->fault_breaker_prob) / 100;
//									cbt->repairTime = atof(lld->fault_breaker_rtime);
//								}
//			}
//			else
//			{
//				cbt->prob_fault = (cb + ii)->prob_fault / 100;
//				cbt->repairTime = (cb + ii)->repairTime;
//			}
//
//			//lld edit end for tangshan 2016-11-9
//			if (cbt->q & Q_BREAKERFEEDER)
//			{
//				cbt->cbtype = 5;//���߿��ر�Ȼ������
//			}
//
//			//���翪��û�й����ʣ������翪�ز��������ɿ���
//			if (cbt->q & Q_UNIONCB)
//			{
//				cbt->cbtype = 4;
//
//				cbt->prob_fault = 0;
//				cbt->repairTime = 0;
//
//				UN* unt = new UN;
//				strcpy(unt->id, cbt->id);
//				//��ȡ���翪�ص���������ID
//				strcpy(unfd, cbt->fdid);
//				if (lld->isContainor(unfd, '|') != -1)
//				{
//					lld->getleftstr(unfd, '|', temp_);
//
//					//���߱����Ƿ�����������ߣ����û�У������
//					if (strlen(temp_) > 0)
//					{
//						FD* fdt = new FD;
//						strcpy(fdt->id, temp_);
//						ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
//						if (ph == -1)
//						{
//							fdt->i_id = cnt_fd;
//
//							fd_tb_t->Append(*fdt);
//							cnt_fd++;
//						}
//						else
//						{
//							//fd_tb_t->UpdateDataByPh(fdt,ph);
//						}
//						delete fdt;
//
//						strcpy(unt->left, temp_);
//						lld->getrightstr(unfd, '|', temp_);
//						strcpy(unt->right, temp_);
//
//						fdt = new FD;
//						strcpy(fdt->id, temp_);
//						ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
//						if (ph == -1)
//						{
//							fdt->i_id = cnt_fd;
//							fd_tb_t->Append(*fdt);
//							cnt_fd++;
//						}
//						else
//						{
//							//fd_tb_t->UpdateDataByPh(fdt,ph);
//						}
//						delete fdt;
//					}
//				}
//				else
//				{
//					strcpy(unt->left, unfd);
//					strcpy(unt->right, unfd);//�Բ���Ĭ�ϵ�����
//				}
//
//				un_tb_t->Append(*unt);
//				cnt_un++;
//				delete unt;
//			}
//
//			cb_tb->Append(*cbt);
//			cnt_t++;
//			delete cbt;
//		}
//		//printf("fdid = %s\n",cb_tb_a->GetTable()->fdid);
//		//cb_tb_a->Sequence();
//		lld->mx_cb = cb_tb->Sort(cnt_t);
//	}
//	else
//		lld->mx_cb = 0;
//
//	if (cntBUS > 0)
//	{
//		bus_tb = new DTable<BUS>(lld->mx_bus);
//		cnt_t = 0;
//		for (ii = 0; ii < cntBUS; ii++)
//		{
//			BUS* bust = new BUS;
//			strcpy(bust->id, (bus + ii)->id);
//			strcpy(bust->nd, (bus + ii)->nd);
//			bust->vl = (bus + ii)->kv;
//
//			bus_tb->Append(*bust);
//			cnt_t++;
//			delete bust;
//		}
//		lld->mx_bus = bus_tb->Sort(cnt_t);
//	}
//	else
//		lld->mx_bus = 0;
//
//
//	if (cntCP > 0)
//	{
//		cp_tb = new DTable<CP>(cntCP);
//		cnt_t = 0;
//		for (ii = 0; ii < cntCP; ii++)
//		{
//			CP* cpt = new CP;
//			strcpy(cpt->id, (cp + ii)->id);
//			strcpy(cpt->nd, (cp + ii)->nd);
//			strcpy(cpt->znd, (cp + ii)->znd);
//			strcpy(cpt->fdid, (cp + ii)->fdid);
//			strcpy(cpt->vl, (cp + ii)->vl);
//
//			cpt->r = (cp + ii)->r;//����ĵ�λ��MVar
//
//			cp_tb->Append(*cpt);
//			cnt_t++;
//			delete cpt;
//		}
//		lld->mx_cp = cp_tb->Sort(cnt_t);
//	}
//	else
//		lld->mx_cp = 0;
//
//	if (cntLD > 0)
//	{
//		double ld_w, ld_r,ld_f;
//
//		ld_tb = new DTable<LD>(cntLD);
//		cnt_t = 0;
//		for (ii = 0; ii < cntLD; ii++)
//		{
//			LD* ldt = new LD;
//			strcpy(ldt->id, (ld + ii)->id);
//			strcpy(ldt->nd, (ld + ii)->nd);
//			strcpy(ldt->vl, "10");
//			//strcpy(ldt->vl, (ld + ii)->vl);
//			strcpy(ldt->fdid, (ld + ii)->fdid);
//			ldt->type = (ld + ii)->type;
//
//			if (ldt->type & LD_SO)
//				ldt->topflags |= LD_SO;//���Դ����
//			else
//				ldt->topflags |= LD_LEVEL2;//Ĭ�϶�������
//
//			//ldt->q = (ld+ii)->q;//��ѹ���ĸ����ʣ�������100%
//
//			ld_w = (ld + ii)->w;
//			ld_r = (ld + ii)->r;
//			ld_f = (ld + ii)->wm;
//
//			//�ö���������
//			if (ld_w == -1)
//			{
//				ld_w = ld_r * sqrt(1 - (1-ld_f)*(1 - ld_f));
//				ld_r *= (1 - ld_f);
//				ldt->wm = ((double)((ld + ii)->q) / 100) * ld_w;//��ѹ�����й����⣬��λkW
//				ldt->rm = ((double)((ld + ii)->q) / 100) * ld_r;//��ѹ�����޹�����
//			}
//			else
//			{
//				ld_r = ld_w * (1 - ld_f);
//				ldt->wm = ld_w;//��ѹ�����й����⣬��λkW
//				ldt->rm = ld_r;//��ѹ�����޹�����
//			}
//
//			ldt->q = 0;//���õı�־λ���㣬���������ܻ��д���
//
//			if (lld->isGroupParam == 1)
//			{
//				ldt->prob_fault = atof(lld->fault_transformer_prob) / 100;
//				ldt->repairTime = atof(lld->fault_transformer_rtime);
//			}
//			else
//			{
//				ldt->prob_fault = (ld + ii)->prob_fault / 100;
//				ldt->repairTime = (ld + ii)->repairTime;
//			}
//
//			ld_tb->Append(*ldt);
//			cnt_t++;
//			delete ldt;
//		}
//		lld->mx_ld = ld_tb->Sort(cnt_t);;//���ݼ����������
//	}
//	else
//		lld->mx_ld = 0;
//
////	lld->mx_xf = 0;//���ݼ����������
////	lld->mx_sec = lld->mx_xf + cntSEC;//���ݼ����������
//
//	if (cntSEC > 0)
//	{
//		sec_tb = new DTable<SEC>(cntSEC);
//		cnt_t = 0;
//		for (ii = 0; ii < cntSEC; ii++)
//		{
//			SEC* sect = new SEC;
//			//�������迹֧·�����������һ�˽��и��ɵ㣬����ֱ�Ӵ����
//			strcpy(sect->id, (sec + ii)->id);
//			//strcpy(sect->descr,m_Set.m_coldata[1].value);
//			strcpy(sect->nd, (sec + ii)->nd);
//			strcpy(sect->znd, (sec + ii)->znd);
//
//			sect->q = (sec + ii)->q;
//			//if (Q_CABLE & sect->q)
//			//	sect->q = 0;
//			//else if (Q_INSULATION & sect->q)
//			//	sect->q = 1;
//			//else if (Q_ACLINESEGMENT & sect->q)
//			//	sect->q = 2;
//
//			sect->len = (sec + ii)->len;
//			if ( ((sec + ii)->len == 0) || ((sec + ii)->len == -1) )//����Ϊ0���߶Σ����迹֧·
//			{
//				sect->len = 0;
//				sect->r1 = 0;
//				sect->x1 = 0;
//				sect->b1 = 0;
//			}
//			else
//			{
//				//����
//				sect->r1 = (sect->len / 1000) * (sec + ii)->r0;//ŷķ/1ǧ��
//				sect->x1 = (sect->len / 1000) * (sec + ii)->x0;
//				//sect->b1 = (sect->len / 1000) * (sec+ii)->b0;
//			}
//
//			if (lld->isGroupParam == 1)
//			{
//				if (Q_CABLE & sect->q)//����
//				{
//					sect->prob_fault = sect->len * atof(lld->fault_cable_prob) / 100000;//ת���� ��/��/100ǧ��
//					sect->repairTime = atof(lld->fault_cable_rtime);
//				}
//				else if (Q_INSULATION & sect->q)//�ܿվ�Ե��
//				{
//					sect->prob_fault = sect->len * atof(lld->fault_ohline_ins_prob) / 100000;//ת���ɴ�/��
//					sect->repairTime = atof(lld->fault_ohline_ins_rtime);
//				}
//				else//�ɿ��Դ��в���,�ĵ���ǰ�����ݣ��ܿ���
//				{
//					sect->prob_fault = sect->len * atof(lld->fault_ohline_prob) / 100000;//ת���ɴ�/��
//					sect->repairTime = atof(lld->fault_ohline_rtime);
//				}
//			}
//			else
//			{
//				sect->prob_fault = sect->len * (sec + ii)->prob_fault / 100000;//ת���ɴ�/��
//				sect->repairTime = (sec + ii)->repairTime;
//			}
//
//			//��������˳���Ϊ0���������Ҫ��һ��Ĭ��ֵ
//			if ((sect->len == 0) || (sect->len == 1))
//			{
//				sect->prob_fault = 0;//ת���ɴ�/��/��
//			}
//
//			if (sect->repairTime == 0)//û�������ʱ��
//			{
//				if (strcmp(sect->algii, "0") == 0)//����
//					sect->repairTime = 5.5;
//				else
//					sect->repairTime = 2.5;
//			}
//
//			strcpy(sect->vl,(sec+ii)->vl);
//			strcpy(sect->fdid,(sec+ii)->fdid);
//
//			sec_tb->Append(*sect);
//			cnt_t++;
//			delete sect;
//		}
//
//		if (cntSEC > 0)
//			lld->mx_sec = sec_tb->Sort(cnt_t);//���ݼ����������
//	}
//	else
//		lld->mx_sec = 0;
//
//	//�������еı�ѹ�������������
//	if (cntXFMR > 0)
//	{
//		xfmr_tb = new DTable<XFMR>(cntXFMR);
//		cnt_t = 0;
//		for (ii = 0; ii < cntXFMR; ii++)
//		{
//			XFMR* xfmrt = new XFMR;
//			//�������迹֧·�����������һ�˽��и��ɵ㣬����ֱ�Ӵ����
//			strcpy(xfmrt->id, (xfmr + ii)->id);
//			strcpy(xfmrt->descr, (xfmr + ii)->descr);
//			xfmrt->type = (xfmr + ii)->type;
//			xfmrt->rh = (xfmr + ii)->rh;
//			xfmrt->xh = (xfmr + ii)->xh;
//			xfmrt->xm = (xfmr + ii)->xm;
//			xfmrt->rm = (xfmr + ii)->rm;
//			xfmrt->rl = (xfmr + ii)->rl;
//			xfmrt->xl = (xfmr + ii)->xl;
//
//			xfmrt->kvnomh = (xfmr + ii)->kvnomh;
//			xfmrt->kvnomm = (xfmr + ii)->kvnomm;
//			xfmrt->kvnoml = (xfmr + ii)->kvnoml;
//			xfmrt->mvah = (xfmr + ii)->mvah;
//			xfmrt->mvam = (xfmr + ii)->mvam;
//			xfmrt->mval = (xfmr + ii)->mval;
//
//			strcpy(xfmrt->hnd, (xfmr + ii)->hnd);
//			strcpy(xfmrt->mnd, (xfmr + ii)->mnd);
//			strcpy(xfmrt->lnd, (xfmr + ii)->lnd);
//
//			//��ͷ����
//			strcpy(xfmrt->htapty, (xfmr + ii)->htapty);
//			strcpy(xfmrt->mtapty, (xfmr + ii)->mtapty);
//			strcpy(xfmrt->ltapty, (xfmr + ii)->ltapty);
//
//			//��ͷλ��
//			xfmrt->htap = (xfmr + ii)->htap;
//			xfmrt->mtap = (xfmr + ii)->mtap;
//			xfmrt->ltap = (xfmr + ii)->ltap;
//
//			xfmrt->q = (xfmr + ii)->q;
//			//			xfmrt->topflags = (xfmr+ii)->topflags;
//			//			xfmrt->r_xf = (xfmr+ii)->r_xf;
//
//			strcpy(xfmrt->fdid, (xfmr + ii)->fdid);
//
//			xfmrt->prob_fault = (xfmr + ii)->prob_fault / 100;
//			xfmrt->repairTime = (xfmr + ii)->repairTime;
//
//			xfmr_tb->Append(*xfmrt);
//			cnt_t++;
//			delete xfmrt;
//		}
//		lld->mx_xfmr = xfmr_tb->Sort(cnt_t);//���ݼ����������
//	}
//	else
//		lld->mx_xfmr = 0;
//
//	if (cntTAPTY > 0)
//	{
//		tapty_tb = new DTable<TAPTY>(cntTAPTY);
//		cnt_t = 0;
//		for (ii = 0; ii < cntTAPTY; ii++)
//		{
//			TAPTY* taptyt = new TAPTY;
//			strcpy(taptyt->id, (tapty + ii)->id);
//			taptyt->mn = (tapty + ii)->mn;
//			taptyt->mx = (tapty + ii)->mx;
//			taptyt->nom = (tapty + ii)->nom;
//			taptyt->step = (tapty + ii)->step;
//
//			tapty_tb->Append(*taptyt);
//			cnt_t++;
//			delete taptyt;
//		}
//		lld->mx_tapty = tapty_tb->Sort(cnt_t);//���ݼ����������
//	}
//
//	//������ڵ�
//	if (cntND > 0)
//	{
//		nd_tb = new DTable<ND>(cntND);//�Ż�ʱ���ܼӽڵ�
//		cnt_t = 0;
//		for (ii = 0; ii < cntND; ii++)
//		{
//			ND* ndt = new ND;
//			strcpy(ndt->id, (nd + ii)->id);
//			ndt->q = 0;//���õı�־λ���㣬���������ܻ��д���
//
//			nd_tb->Append(*ndt);
//			cnt_t++;
//			delete ndt;
//		}
//		lld->mx_nd = nd_tb->Sort(cnt_t);//���ݼ����������
//	}
//	else
//		lld->mx_nd = 0;
//
//	//�����ռ�
//	lld->mx_fd = fd_tb_t->Sort(cnt_fd);
//	fd_tb = new DTable<FD>(lld->mx_fd);
//	if (lld->mx_fd != 0)
//	{
//		FD* temp_ptr = new FD[lld->mx_fd];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
//		fd_tb_t->CopyTable(temp_ptr);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��
//
//		//update table����ʱ������ļ�¼��д����ʽ������
//		fd_tb->Insert(temp_ptr, lld->mx_fd);
//
//		delete[] temp_ptr;//����ʱ���ڴ��ͷ�
//		temp_ptr = 0;//�����������ֹ�ͷź�ָ����ָ
//	}
//	delete fd_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
//	fd_tb_t = 0;
//
//	if (cnt_un > 0)
//	{
//		lld->mx_un = un_tb_t->Sort(cnt_un);
//		un_tb = new DTable<UN>(lld->mx_un);
//		if (lld->mx_un != 0)
//		{
//			UN* temp_ptr = new UN[lld->mx_un];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
//			un_tb_t->CopyTable(temp_ptr);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��
//
//			//update table����ʱ������ļ�¼��д����ʽ������
//			un_tb->Insert(temp_ptr, lld->mx_un);
//
//			delete[] temp_ptr;//����ʱ���ڴ��ͷ�
//			temp_ptr = 0;//�����������ֹ�ͷź�ָ����ָ
//		}
//		delete un_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
//		un_tb_t = 0;
//	}
//
//	//	int count,i;
//	//	ofstream outfile("data.txt",ios::out);
//	//  	//nd_tb->Sequence();
//	//// 	outfile<<"   "<< endl;
//	//	outfile << "nd:   " << endl;
//	//	count = cntND;
//	//	ND* nd1 = nd_tb->GetTable();
//	//	outfile << "index    id       " << "q    " << endl;
//	//	for (i = 0; i <count; i++)
//	//	{
//	//		outfile << i + 1 << "   " << (nd1 + i)->id << "   " << (nd1 + i)->q << endl;
//	//	}
//	//	
//	//	outfile<<"so:   "<< endl;
//	//  	count = cntSO;
//	//	SO* so1= so_tb->GetTable();
//	//	outfile<<"index    id       "<<"nd    "<<endl;
//	//  	for(i = 0; i <count; i++)
//	//  	{
//	// 		outfile<<i+1<<"   "<<(so1+i)->id<<"   "<<(so1+i)->nd<< endl;
//	//  	}
//	//
//	// 	outfile<<"   "<< endl;
//	// 	outfile<<"cb:   "<< endl;
//	//  	count = cntCB;
//	//	CB* cb1= cb_tb->GetTable();
//	//	outfile<<"index    id       "<<"nd    "<<"znd    "<<endl;
//	//  	for(i = 0; i <count; i++)
//	//  	{
//	// 		outfile<<i+1<<"   "<<(cb1+i)->id<<"   "<<(cb1+i)->nd<<"   "<<(cb1+i)->znd<<"   "<< endl;
//	//  	}
//	//
//	// 	outfile<<"   "<< endl;
//	// 	outfile<<"sec:   "<< endl;
//	//  	count = cntSEC;
//	//	SEC* sec1= sec_tb->GetTable();
//	//	outfile<<"index    id       "<<"nd    "<<"znd    "<<"r0    "<<"x0    "<<endl;
//	//  	for(i = 0; i <count; i++)
//	//  	{
//	// 		outfile<<i+1<<"   "<<(sec1+i)->id<<"   "<<(sec1+i)->nd<<"   "<<(sec1+i)->znd<<"   "<<(sec1+i)->r0<<"   "<<(sec1+i)->x0<< endl;
//	//  	}
//	//	
//	// 	outfile<<"   "<< endl;
//	// 	outfile<<"ld:   "<< endl;
//	//  	count = cntLD;
//	//	LD* ld1= ld_tb->GetTable();
//	//	outfile<<"index   id       "<<"nd    "<<"p    "<<"q    "<<endl;
//	//  	for(i = 0; i <count; i++)
//	//  	{
//	// 		outfile<<i+1<<"   "<<(ld1+i)->id<<"   "<<(ld1+i)->nd<<"   "<<(ld1+i)->wm<<"   "<<(ld1+i)->rm<< endl;
//	//  	}
//	//	
//	// 	outfile<<"   "<< endl;
//	// 	outfile<<"fd:   "<< endl;
//	//  	count = fd_tb->GetCount();
//	//	FD* fd = fd_tb->GetTable();
//	//	outfile<<"index    id       "<<endl;
//	//  	for(i = 0; i <count; i++)
//	//  	{
//	// 		outfile<<(fd+i)->i_id<<"   "<<(fd+i)->id<<endl;
//	//  	}
//	//
//	// //	outfile<<"   "<< endl;
//	// //	outfile<<"un:   "<< endl;
//	// // 	count = un_tb->GetCount();
//	//	//UN* un = un_tb->GetTable();
//	//	//outfile<<"index    id       "<<endl;
//	// // 	for(i = 0; i <count; i++)
//	// // 	{
//	// //		outfile<<i+1<<"   "<<(un+i)->id<<endl;
//	// // 	}
//	//
//	//	outfile.close();
//
//
//
//	return cnt_mainso;
//}
#endif

//���˷���ר�ã�ֻ����д��·���ɣ����溬����ĩ�ڵ㣬�ڵ������
int DSBOB2::ReadDataFromTPfile(char * sFileName,char * traceSub)
{
	//int cnt_sec = 0,cnt_nd=0;
	//sec_tb = new DTable<SEC>(lld->mx_sec);
	//nd_tb = new DTable<ND>(lld->mx_nd);
	//char  line[512];
	//char templine[512];

	// char   *p;
	//FILE *fp;
	//w_t fff[128];
	//w_t ttt[128];
	//fp = fopen(sFileName, "r");
	//if (fp == NULL)
	//	return -1;
	//else
	//{
	//	//_setmode(_fileno(fp), _O_U8TEXT);
	//}

	//fgets(line, sizeof(line), fp);
	//strcpy(traceSub, line);

	//vector<string> nds;
	//while (fgets(line, sizeof(line), fp) != NULL)
	//{
	//	SEC* sect = new SEC;
	//	int pos;
	//	p = line;

	//	sscanf(p, "%s%s%s%s", sect->id, sect->nd, sect->znd, sect->vl);
	//	
	//	//�Զ����ڵ��
	//	if (!lld->isFind(nds, sect->nd))
	//	{
	//		nds.push_back(sect->nd);
	//		ND* ndt = new ND;
	//		strcpy(ndt->id, sect->nd);
	//		strcpy(ndt->ver, sect->vl);
	//		nd_tb->Append(*ndt);
	//		cnt_nd++;
	//		delete ndt;
	//	}
	//	if (!lld->isFind(nds, sect->znd))
	//	{
	//		nds.push_back(sect->znd);
	//		ND* ndt = new ND;
	//		strcpy(ndt->id, sect->nd);
	//		strcpy(ndt->ver, sect->vl);
	//		nd_tb->Append(*ndt);
	//		cnt_nd++;
	//		delete ndt;
	//	}
	//	sec_tb->Append(*sect);
	//	cnt_sec++;
	//	delete sect;
	//}

	//if (cnt_sec > 0)
	//	lld->mx_sec = sec_tb->Sort(cnt_sec);
	//if (cnt_nd > 0)
	//	lld->mx_nd = nd_tb->Sort(cnt_nd);

	//fclose(fp);

	return 1;
}

//��CIM�ļ��������������棬���ڶ�·������������Ҫ����ʱ����һ���ϼ���Դ��֧·
int DSBOB2::ReadDataFromEfile(char * sFileName)
{
	int ii, ph;
	int cnt_t, cnt_un = 0, cnt_fd = 0, cnt_mainso = 0;
	int cnt_so = 0, cnt_cb = 0, cnt_sec = 0, cnt_ld = 0, cnt_nd = 0, cnt_xf = 0;
	char  unfd[2 * MAX_ID_DLL];//���翪�ؿ϶���������������
	char  temp_[2 * MAX_ID_DLL];//���翪�ؿ϶���������������
 	temp_[0] = 0;

	fd_tb_t = new DTable<FD>(lld->mx_so + lld->mx_cb);
	DTable<UN>* un_tb_t = new DTable<UN>(lld->mx_cb);
	so_tb = new DTable<SO>(lld->mx_so);
	cb_tb = new DTable<CB>(lld->mx_cb);
	ld_tb = new DTable<LD>(lld->mx_ld);
	sec_tb = new DTable<SEC>(lld->mx_sec);
	nd_tb = new DTable<ND>(lld->mx_nd);
	//xf_tb = new DTable<XF>(lld->mx_xf);
	//xfmr_tb = new DTable<XFMR>(lld->mx_xfmr);
	//cp_tb = new DTable<CP>(lld->mx_cp);
	//bus_tb = new DTable<BUS>(lld->mx_bus);
	//tapty_tb = new DTable<TAPTY>(lld->mx_tapty);
	lld->mx_xf = lld->mx_cp = lld->mx_bus = 0;

	double ss, r_gxfmr, imp_g;
	double ld_w, ld_r;

	char  temp1[128], temp2[128];
	char  flagType[32];
	int ret,type,flag_b;
	int n_cb, n_sec, n_secp, n_bus, n_xf, n_xfmr, n_ld, n_line, n_st, n_so, n_nd, n_equip;
	char  line[512];
	char  templine[512];
//	 type[16];
	char    *p;
	FILE *fp;
	fp = fopen(sFileName, "r");
	if (fp == NULL)
		return -1;

	n_line = flag_b = 0;
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		strcpy(flagType, "inputData::parameter");
		if ((ret = getDevType(line, flagType, n_line, flag_b) > 0))
		{
			type = 0;
			continue;
		}
		strcpy(flagType, "inputData::so");
		if ((ret = getDevType(line, flagType, n_line, flag_b) > 0))
		{
			type = 1;
			continue;
		}
		strcpy(flagType, "inputData::sec");
		if ((ret = getDevType(line, flagType, n_line, flag_b) > 0))
		{
			type = 2;
			continue;
		}
		strcpy(flagType, "inputData::cb");
		if ((ret = getDevType(line, flagType, n_line, flag_b) > 0))
		{
			type = 3;
			continue;
		}
		strcpy(flagType, "inputData::ld");
		if ((ret = getDevType(line, flagType, n_line, flag_b) > 0))
		{
			type = 4;
			continue;
		}
		strcpy(flagType, "inputData::nd");
		if ((ret = getDevType(line, flagType, n_line, flag_b) > 0))
		{
			type = 5;
			continue;
		}
		strcpy(flagType, "inputData::xf");
		if ((ret = getDevType(line, flagType, n_line, flag_b) > 0))
		{
			type = 6;
			continue;
		}

		if (flag_b == 0)
			continue;

		if (line[0] == '#')
		{
			p = line;
			switch (type)
			{
			case 0: //
			{
				sscanf(p, "%*s%s%s", outFileName,
					 temp2);
			}
			break;
			case 1: //
			{
				SO* sot = new SO;

				sscanf(p, "%*s%s%s%s%*s%f%d%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%s",
					sot->id, sot->fdid, sot->nd, &sot->vtarget, &sot->slackavr, 
					&sot->maxcurrent, &sot->w, &sot->r, &sot->v, &sot->I_value,
					&sot->fault_ltime, &sot->fault_stime, &sot ->fault_ttime,
					&sot ->plan_prob, &sot ->plan_rtime,
					&sot ->plan_ltime, &sot ->plan_stime, &sot ->plan_ttime,
					&sot ->prob_fault, &sot ->repairTime,					
					sot->vl);
				if (sot->slackavr == 4)//���ڵ㣬һ���ǵ����ĳ��߽ڵ�
				{
					FD* fdt = new FD;

					strcpy(fdt->id, sot->fdid);
					fdt->maxcurrent = sot->maxcurrent;//������
					strcpy(fdt->vl, sot->vl);//��ѹ�ȼ�

   				    //����ʵ����׶˵�������λ��A�����õ�ʱ����Ҫ����ת��Ϊ�й����޹�
					fdt->I_value = sot->I_value;

					if (lld->isGroupParam == 1)
					{
						fdt->fault_ltime = atof(lld->fault_ltime) / 60;//Сʱ��ԭʼ��дΪ����
						fdt->fault_stime = atof(lld->fault_stime) / 60;//������λ����ʱ��
						fdt->fault_ttime = atof(lld->fault_ttime) / 60;//������λ����ʱ��
						fdt->plan_prob = atof(lld->plan_prob) / 100000;//��/��/100ǧ��
						fdt->plan_rtime = atof(lld->plan_rtime);//ԭʼ��дΪСʱ���Ѻ���λ����ʱ��
						fdt->plan_ltime = atof(lld->plan_ltime) / 60;//����ʱ��
						fdt->plan_stime = atof(lld->plan_stime) / 60;//��������ʱ��
						fdt->plan_ttime = atof(lld->plan_ttime) / 60;//��������ʱ��
					}
					else
					{
						fdt->fault_ltime = sot->fault_ltime / 60;//���϶�λʱ�䣬��СʱΪ��λ
						fdt->fault_stime = sot->fault_stime / 60;//�л�ʱ�䣨���λָ�ʱ�䣩��������λʱ�䣬Сʱ
						fdt->fault_ttime = sot->fault_ttime / 60;//ת��ʱ�䣨���λָ�ʱ�䣩��������λʱ�䣬Сʱ

																 //�л�ʱ��
						fdt->plan_ltime = sot->plan_ltime / 60;//Ԥ���Ÿ���ʱ�䣬��СʱΪ��λ��Ԥ����û�ж�λʱ�䣩
						fdt->plan_stime = sot->plan_stime / 60;
						fdt->plan_ttime = sot->plan_ttime / 60;

						fdt->plan_prob = sot->plan_prob / 100000;//Ԥ������,(��/�ٹ�����)
						fdt->plan_rtime = sot->plan_rtime;//ƽ��Ԥ����ͣ�˳���ʱ��(h)
					}

					ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
					if (ph == -1)
					{
						fdt->i_id = cnt_fd;
						fd_tb_t->Append(*fdt);
						cnt_fd++;
					}
					else
					{
						fdt->i_id = ph;
						fd_tb_t->UpdateDataByPh(fdt, ph);
					}

					delete fdt;
					cnt_mainso++;
				}
				else if (sot->slackavr == 1)	//PQ�㶨
				{
				}
				else if (sot->slackavr == -1)//V�㶨
				{
					sot->v = sot->vtarget;
				}
				else if (sot->slackavr == -2)//I�㶨
				{
				}

				if (strstr(outFileName, "sc"))
				{
					lld->g_mathType = lld->CALCTYPE_SC;

					std::vector<std::string> src;
					src = lld->splitWithStl(temp2, "|");
					strcpy(temp1, src[0].c_str());
					sot->cap_I = atof(temp1);//���ݵ���
					strcpy(temp1, src[1].c_str());
					sot->ss = atof(temp1);//��·����������ʹ�ö�·�����ȽϺã�1.732*��·����*10kV
					strcpy(temp1, src[2].c_str());
					sot->sw = atof(temp1);//�ӵر�翹�����ӵ���
					strcpy(temp1, src[3].c_str());
					
					if (strcmp(temp1, "-") == 0)//��ָ���ӵص���Ĵ�С�����õ����ķ�ʽ���㣬��10��ʼ
						sot->sr = 100000;
					else
						sot->sr = atof(temp1);//�ӵص���

					strcpy(temp1, src[4].c_str());
					if (strcmp(temp1, "-") == 0)//��ָ���ӵص���ɵ���Ĵ�С
						sot->pLoss = 100000;
					else
					{
						sot->pLoss = atof(temp1);//�ӵص���ɵ���
					}
				}
				else if (strstr(outFileName, "pf"))
				{
					lld->g_mathType = lld->CALCTYPE_FLOW;
				}
				else if (strstr(outFileName, "nm1"))
				{
					lld->g_mathType = lld->CALCTYPE_NM1;
				}
				else if (strstr(outFileName, "ra"))
				{
					lld->g_mathType = lld->CALCTYPE_RA;
					//��Ҫ�������ļ��ж�����ֵ
					lld->isGroupParam = 0;
					//��ȫ�ֵļ������
					if (strlen(temp2) > 0)
					{
						char  l[200];
						char  r[200];
						char  key[2];
						strcpy(key, "=");
						std::vector<std::string> src;
						src = lld->splitWithStl(temp2, "|");

						strcpy(l, "fault_ohline_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_ohline_prob, r);
						strcpy(l, "fault_ohline_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_ohline_rtime, r);
						strcpy(l, "fault_ohline_ins_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_ohline_ins_prob, r);
						strcpy(l, "fault_ohline_ins_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_ohline_ins_rtime, r);
						strcpy(l, "fault_cable_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_cable_prob, r);
						strcpy(l, "fault_cable_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_cable_rtime, r);
						strcpy(l, "fault_breaker_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_breaker_prob, r);
						strcpy(l, "fault_breaker_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_breaker_rtime, r);
						strcpy(l, "fault_loadbreakswitch_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_loadbreakswitch_prob, r);
						strcpy(l, "fault_loadbreakswitch_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_loadbreakswitch_rtime, r);
						strcpy(l, "fault_disconnector_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_disconnector_prob, r);
						strcpy(l, "fault_disconnector_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_disconnector_rtime, r);
						strcpy(l, "fault_fuse_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_fuse_prob, r);
						strcpy(l, "fault_fuse_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_fuse_rtime, r);
						strcpy(l, "fault_transformer_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_transformer_prob, r);
						strcpy(l, "fault_transformer_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_transformer_rtime, r);
						strcpy(l, "fault_ltime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_ltime, r);
						strcpy(l, "fault_stime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_stime, r);
						strcpy(l, "fault_ttime");lld->getRightByleft(src, l, r, key);strcpy(lld->fault_ttime, r);
						strcpy(l, "plan_ltime");lld->getRightByleft(src, l, r, key);strcpy(lld->plan_ltime, r);
						strcpy(l, "plan_stime");lld->getRightByleft(src, l, r, key);strcpy(lld->plan_stime, r);
						strcpy(l, "plan_ttime");lld->getRightByleft(src, l, r, key);strcpy(lld->plan_ttime, r);
						strcpy(l, "plan_ohline_prob");lld->getRightByleft(src, l, r, key);strcpy(lld->plan_prob, r);
						strcpy(l, "plan_ohline_rtime");lld->getRightByleft(src, l, r, key);strcpy(lld->plan_rtime, r);

						lld->isGroupParam = 1;//����ʹ��ȫ�ֲ���
					}
				}

				so_tb->Append(*sot);
				cnt_so++;
				delete sot;
			}
			break;
			case 2: //
			{
				SEC* sect = new SEC;

				//�豸����	�豸ID��	�豸����	�׽ڵ�	ĩ�ڵ�	
				//����	�翹	��·����	q	��ʩ����ͣ���ʣ��� / �ٹ����̨)�꣩	
				//ƽ�������޸�ʱ�䣨h��	��������	��ѹ�ȼ�
				sscanf(p, "%*s%s%s%s%s%f%f%f%f%f%d%f%f%s", sect->id, sect->fdid, sect->nd, sect->znd,
					&sect->r1, &sect->x1, &sect->r0, &sect->x0, &sect->len, 
					&sect->q, &sect->prob_fault, &sect->repairTime, sect->vl);

				//if (Q_CABLE & sect->q)
				//	sect->q = 0;
				//else if (Q_INSULATION & sect->q)
				//	sect->q = 1;
				//else if (Q_ACLINESEGMENT & sect->q)
				//	sect->q = 2;

				if ((sect->len == 0) || (sect->len == -1))//����Ϊ0���߶Σ����迹֧·
				{
					sect->r1 = 0;
					sect->x1 = 0;
					sect->b1 = 0;
					sect->r0 = 0;
					sect->x0 = 0;
					sect->b0 = 0;
				}
				else
				{
					sect->r1 = (sect->len / 1000) * sect->r1;//ŷķ/1ǧ��
					sect->x1 = (sect->len / 1000) * sect->x1;
					sect->r0 = (sect->len / 1000) * sect->r0;//ŷķ/1ǧ��
					sect->x0 = (sect->len / 1000) * sect->x0;
					//sect->b0 = (sect->len / 1000) * (sec+ii)->b0;
				}

				if (lld->isGroupParam == 1)
				{
					if (sect->q & Q_CABLE)//����
					{
						sect->prob_fault = sect->len * atof(lld->fault_cable_prob) / 100000;//ת���� ��/��/100ǧ��
						sect->repairTime = atof(lld->fault_cable_rtime);
					}
					else if (sect->q & Q_ACLINESEGMENT)//�ܿվ�Ե��
					{
						sect->prob_fault = sect->len * atof(lld->fault_ohline_ins_prob) / 100000;//ת���ɴ�/��
						sect->repairTime = atof(lld->fault_ohline_ins_rtime);
					}
					else//�ɿ��Դ��в���,�ĵ���ǰ�����ݣ�����
					{
						sect->prob_fault = sect->len * atof(lld->fault_ohline_prob) / 100000;//ת���ɴ�/��
						sect->repairTime = atof(lld->fault_ohline_rtime);
					}
				}
				else
				{
					sect->prob_fault = sect->len * sect->prob_fault / 100000;//ת���ɴ�/��
					sect->repairTime = sect->repairTime;
				}

				//��������˳���Ϊ0���������Ҫ��һ��Ĭ��ֵ
				if ((sect->len == 0) || (sect->len == -1) || (sect->len == 1))
				{
					sect->prob_fault = 0;//ת���ɴ�/��/��
				}

				if (sect->repairTime == 0)//û�������ʱ��
				{
					if (sect->q & Q_CABLE)//����
						sect->repairTime = 5.5;
					else
						sect->repairTime = 2.5;
				}
				sec_tb->Append(*sect);
				cnt_sec++;
				delete sect;
			}
			break;
			case 3: //
			{
				CB* cbt = new CB;
				//�豸����	�豸ID��	�豸����	�׽ڵ�	ĩ�ڵ�	��������	q	��
				//ʩ����ͣ���ʣ��� / �ٹ����̨)�꣩	ƽ�������޸�ʱ�䣨h��	
				//��������	��ѹ�ȼ�
				//		cb:	Fa - 21	31	1	1024	0.2	2	testSG	10
				sscanf(p, "%*s%s%s%s%s%d%f%f%s", cbt->id, cbt->fdid, cbt->nd, cbt->znd,
					&cbt->q, &cbt->prob_fault, &cbt->repairTime, cbt->vl);

				if (Q_BREAKERRELAY & cbt->q)
					cbt->cbtype = 5;
				else if (Q_BREAKER & cbt->q)
					cbt->cbtype = 0;
				else if (Q_LOADBREAKSWITCH & cbt->q)
					cbt->cbtype = 1;
				else if (Q_DISCONNECTOR & cbt->q)
					cbt->cbtype = 2;
				else if (Q_FUSE & cbt->q)
					cbt->cbtype = 3;

				if (lld->isGroupParam == 1)
				{
					if (cbt->cbtype == 0)
					{
						cbt->prob_fault = atof(lld->fault_breaker_prob) / 100;//  ��/̨/��
						cbt->repairTime = atof(lld->fault_breaker_rtime);
					}
					else
						if (cbt->cbtype == 1)
						{
							cbt->prob_fault = atof(lld->fault_loadbreakswitch_prob) / 100;
							cbt->repairTime = atof(lld->fault_loadbreakswitch_rtime);
						}
						else
							if (cbt->cbtype == 2)
							{
								cbt->prob_fault = atof(lld->fault_disconnector_prob) / 100;
								cbt->repairTime = atof(lld->fault_disconnector_rtime);
							}
							else
								if (cbt->cbtype == 3)
								{
									cbt->prob_fault = atof(lld->fault_fuse_prob) / 100;
									cbt->repairTime = atof(lld->fault_fuse_rtime);
								}
								else
									if (cbt->cbtype == 5)
									{
										cbt->prob_fault = atof(lld->fault_breaker_prob) / 100;
										cbt->repairTime = atof(lld->fault_breaker_rtime);
									}
				}
				else
				{
					cbt->prob_fault = cbt->prob_fault / 100;
				}

				//lld edit end for tangshan 2016-11-9
				if (cbt->q & Q_BREAKERFEEDER)
				{
					cbt->cbtype = 5;//���߿��ر�Ȼ������
				}

				//���翪��û�й����ʣ������翪�ز��������ɿ���
				if (cbt->q & Q_UNIONCB)
				{
					cbt->cbtype = 4;

					cbt->prob_fault = 0;
					cbt->repairTime = 0;

					UN* unt = new UN;
					strcpy(unt->id, cbt->id);
					//��ȡ���翪�ص���������ID
					strcpy(unfd, cbt->fdid);
					if (lld->isContainor(unfd, '|') != -1)
					{
						lld->getleftstr(unfd, '|', temp_);

						//���߱����Ƿ�����������ߣ����û�У������
						if (strlen(temp_) > 0)
						{
							FD* fdt = new FD;
							strcpy(fdt->id, temp_);
							ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
							if (ph == -1)
							{
								fdt->i_id = cnt_fd;

								fd_tb_t->Append(*fdt);
								cnt_fd++;
							}
							else
							{
								//fd_tb_t->UpdateDataByPh(fdt,ph);
							}
							delete fdt;

							strcpy(unt->left, temp_);
							lld->getrightstr(unfd, '|', temp_);
							strcpy(unt->right, temp_);

							fdt = new FD;
							strcpy(fdt->id, temp_);
							ph = OnGetPhByfdid(fdt->id, 1);//fd_tb_t->FindPh(*fdt);
							if (ph == -1)
							{
								fdt->i_id = cnt_fd;
								fd_tb_t->Append(*fdt);
								cnt_fd++;
							}
							else
							{
								//fd_tb_t->UpdateDataByPh(fdt,ph);
							}
							delete fdt;
						}
					}
					else
					{
						strcpy(unt->left, unfd);
						strcpy(unt->right, unfd);//�Բ���Ĭ�ϵ�����
					}

					un_tb_t->Append(*unt);
					cnt_un++;
					delete unt;
				}

				cb_tb->Append(*cbt);
				cnt_cb++;
				delete cbt;

			}
			break;
			case 4: //
			{
				LD* ldt = new LD;
				//�豸����	�豸ID��	�豸����	�׽ڵ�	ʵ������	�����	
				//q����Ҫ�û���˫��Դ)	��ʩ����ͣ���ʣ���/�ٹ����̨)�꣩	ƽ�������޸�ʱ�䣨h��
				//��������	��ѹ�ȼ�

				sscanf(p, "%*s%s%s%s%f%f%f%f%f%d%f%f%s", ldt->id, ldt->fdid, ldt->nd,
					&ldt->w, &ldt->r, &ldt->wm, &ldt->rm, &ldt->basevoltage, &ldt->type, &ldt->prob_fault, &ldt->repairTime, ldt->vl);

				//ldt->q = ldt->wm;
				//ldt->wm = 0;
				ld_w = ldt->w;//�й�����
				ld_r = ldt->r;//�޹�����

				//�ö���������
				if (ld_w == -1)
				{
					if (ldt->rm == -1)//û���ѹ������,topflags��ʱ����������0-100��Ĭ��50
						ld_r = ((double)(ldt->basevoltage) / 100)*400;//Ĭ��400
					else
						ld_r = ((double)(ldt->basevoltage) / 100)*ldt->rm;

					ld_w = ld_r * sqrt(1 - 0.15*0.15);
					ld_r *= 0.15;
					ldt->wm = ld_w;//���ϸ����ʣ���λkW
					ldt->rm = ld_r;//��ѹ�����޹�����
				}
				else
				{
					if (ld_r==-1)
						ld_r = ld_w * 0.15;
					ldt->wm = ld_w;//��ѹ�����й����⣬��λkW
					ldt->rm = ld_r;//��ѹ�����޹�����
				}

				ldt->q = ldt->type;//���õı�־λ���㣬���������ܻ��д���
				if (ldt->type & LD_SO)
					ldt->topflags == LD_SO| ldt->type;//���Դ���ɣ���Ҫ�û���Ϊ���Դ
				else
					ldt->topflags == ldt->type;//Ĭ���������ɣ�����������Ϊ��Ҫ�û�

				if (lld->isGroupParam == 1)
				{
					ldt->prob_fault = atof(lld->fault_transformer_prob) / 100;
					ldt->repairTime = atof(lld->fault_transformer_rtime);
				}
				else
				{
					ldt->prob_fault = ldt->prob_fault / 100;
				}

				ld_tb->Append(*ldt);
				cnt_ld++;
				delete ldt;
			}
			break;
			case 5: //
			{
				ND* ndt = new ND;

				//�豸����	�豸ID��	��־λ	��ѹ�ȼ�
				sscanf(p, "%*s%s%d%d", ndt->id, &ndt->q, &ndt->vl);

				nd_tb->Append(*ndt);
				cnt_nd++;
				delete ndt;
			}
			break;
			case 6: //
			{
				XF* xft = new XF();

				//�豸����	�豸ID��	�豸����	�׽ڵ�	ĩ�ڵ�	
				//����	�翹	��·����	q	��ʩ����ͣ���ʣ��� / �ٹ����̨)�꣩	
				//ƽ�������޸�ʱ�䣨h��	��������	��ѹ�ȼ�
				sscanf(p, "%*s%s%s%s%s%f%f%f%f%d%s", xft->id, xft->fdid, xft->nd, xft->znd,
					&xft->r1, &xft->x1, &xft->r0, &xft->x0,
					&xft->q, xft->vl);

				xf_tb->Append(*xft);
				cnt_xf++;
				delete xft;
			}
			break;


			}
			continue;
		}
	}

	//�����ռ�
	lld->mx_fd = fd_tb_t->Sort(cnt_fd);
	fd_tb = new DTable<FD>(lld->mx_fd);
	if (lld->mx_fd != 0)
	{
		FD* temp_ptr = new FD[lld->mx_fd];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
		fd_tb_t->CopyTable(temp_ptr);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

									 //update table����ʱ������ļ�¼��д����ʽ������
		fd_tb->Insert(temp_ptr, lld->mx_fd);

		delete[] temp_ptr;//����ʱ���ڴ��ͷ�
		temp_ptr = 0;//�����������ֹ�ͷź�ָ����ָ
	}
	delete fd_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	fd_tb_t = 0;

	if (cnt_un>0)
	{
		lld->mx_un = un_tb_t->Sort(cnt_un);
		un_tb = new DTable<UN>(lld->mx_un);
		if (lld->mx_un != 0)
		{
			UN* temp_ptr = new UN[lld->mx_un];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			un_tb_t->CopyTable(temp_ptr);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������,�պ�װ��

										 //update table����ʱ������ļ�¼��д����ʽ������
			un_tb->Insert(temp_ptr, lld->mx_un);

			delete[] temp_ptr;//����ʱ���ڴ��ͷ�
			temp_ptr = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete un_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		un_tb_t = 0;
	}

	lld->mx_so = so_tb->Sort(cnt_so);
	lld->mx_cb = cb_tb->Sort(cnt_cb);
	lld->mx_sec = sec_tb->Sort(cnt_sec);
	//lld->mx_xf = xf_tb->Sort(cnt_xf);
	if (cnt_ld > 0)
		lld->mx_ld = ld_tb->Sort(cnt_ld);
	else
		lld->mx_ld = 0;

	lld->mx_nd = nd_tb->Sort(cnt_nd);

	fclose(fp);

	return cnt_mainso;
}

int DSBOB2::ReadCIME(char * filename)
{
	ND* pnd = new ND;
	 char flagField_b[32], flagField_e[32];
	int flag_b;
	lld->mx_sec = lld->mx_cb = lld->mx_bus = lld->mx_xf = lld->mx_xfmr = lld->mx_ld = lld->mx_st = lld->mx_so = lld->mx_nd = lld->mx_cp = 0;
	int n_sec, n_cb, n_secp, n_bus, n_xf, n_xfmr, n_ld, n_line, n_st, n_so, n_nd, n_equip, n_vl, n_base_v, n_cp, n_tap;
	int type;//�豸���ͣ�0-breaker��1-disconnector;2-BusbarSection;3-ACLineSegment;
			//4-ACLineDot;5-PowerTransformer;6-TransformerWinding;7-Load;8-Substation
			//9-BaseVoltage;10-SynchronousMachine;11-VoltageLevel;12-GroundDisconnector
			//13-ShuntCompensator;14-TapChangerType;
	char line[512];
	  char  *p=NULL;
	//	   d[]= "\t\n\32";
	//	int n_field;

	int i;

	//������Ҫ�����������豸����
		//��Ҫ��������CIM�ļ��е��ֶ����
#define MAX_LEN 20

//<BaseVoltage

	int index_breaker[MAX_LEN];
	int index_disconnector[MAX_LEN];
	int index_bus[MAX_LEN];
	int index_ac[MAX_LEN];
	int index_acdot[MAX_LEN];
	//int index_dc[5];
	int index_dc[MAX_LEN];
	int index_dcdot[MAX_LEN];
	int index_ld[MAX_LEN];
	int index_xfmr[MAX_LEN];
	int index_xf[MAX_LEN];
	int index_st[MAX_LEN];
	int index_base_v[MAX_LEN];//��׼��ѹ�ȼ�
	int index_sy[MAX_LEN];//ͬ����
	int index_vl[MAX_LEN];//��׼��ѹ�ȼ�
	int index_gd[MAX_LEN];//�ӵص�բ
	int index_cp[MAX_LEN];//���ݵ翹
	int index_tap[MAX_LEN];//���ݵ翹

	//�ֶζ�Ӧ����������
	int type_breaker[MAX_LEN];
	int type_disconnector[MAX_LEN];
	int type_bus[MAX_LEN];
	int type_ac[MAX_LEN];
	int type_acdot[MAX_LEN];
	int type_dc[MAX_LEN];
	int type_dcdot[MAX_LEN];
	int type_ld[MAX_LEN];
	int type_xfmr[MAX_LEN];
	int type_xf[MAX_LEN];
	int type_st[MAX_LEN];
	int type_base_v[MAX_LEN];
	int type_sy[MAX_LEN];//ͬ����
	int type_vl[MAX_LEN];
	int type_gd[MAX_LEN];//�ӵص�բ
	int type_cp[MAX_LEN];//���ݵ翹
	int type_tap[MAX_LEN];//��ѹ������ĵ�λ

	for (i = 0;i < MAX_LEN;i++)
	{
		index_breaker[i] = -1;
		index_disconnector[i] = -1;
		index_bus[i] = -1;
		index_ac[i] = -1;
		index_acdot[i] = -1;
		index_dc[i] = -1;
		index_dcdot[i] = -1;
		index_ld[i] = -1;
		index_xfmr[i] = -1;
		index_xf[i] = -1;
		index_st[i] = -1;
		index_base_v[i] = -1;
		index_sy[i] = -1;
		index_vl[i] = -1;
		index_gd[i] = -1;
		index_cp[i] = -1;
		index_tap[i] = -1;

		//�ֶζ�Ӧ����������
		type_breaker[i] = -1;
		type_disconnector[i] = -1;
		type_bus[i] = -1;
		type_ac[i] = -1;
		type_acdot[i] = -1;
		type_dc[i] = -1;
		type_dcdot[i] = -1;
		type_ld[i] = -1;
		type_xfmr[i] = -1;
		type_xf[i] = -1;
		type_st[i] = -1;
		type_base_v[i] = -1;
		type_sy[i] = -1;
		type_vl[i] = -1;
		type_gd[i] = -1;
		type_cp[i] = -1;
		type_tap[i] = -1;
	}

	//������Ҫ��ͨ���
	char filter_breaker[64];
	char filter_disconnector[64];
	char filter_bus[64];
	char filter_ac[64];
	char filter_acdot[64];
	char filter_ld[64];
	char filter_xfmr[64];
	char filter_xf[64];
	char filter_st[64];
	char filter_base_v[64];
	char filter_sy[64];
	char filter_vl[64];
	char filter_gd[64];
	char  filter_cp[64];
	char filter_tap[64];

	//�豸���Ͷ������


	char id[64];
	char descr[128];
	char nd[64];
	char znd[64];
	char ref[64];//�����豸,��վ���ѹ��
	char iflag[8];//0/1;true/false
	char 	w[64];
	char 	r[64];

	char 	vl[64];//������׼��ѹ�ȼ������ַ���ʾ������Ҫ����ʶ������õ���صĻ�׼��ѹ
	char 	vTarget[64];//��ѹ����Ŀ��ֵ

	char 	r1[64];//����
	char 	x1[64];
	char 	b1[64];
	char 	r0[64];//����
	char 	x0[64];
	char 	b0[64];

	//	lld.TimeHere("start. ");
	// outdir[256];//�����ļ���Ŀ¼
	// datafile[32];//�����ļ���
	//lld->getFileDir_Name(filename, outdir, datafile);
	char  configfile_name[256];//׼��������ļ���
	lld->getExecutePath(configfile_name);
	//strcpy(configfile_name, outdir);
	strcat(configfile_name, "\\config.cime");

	//�ȶ������ļ�
	FILE *fp;
	fp = fopen(configfile_name, "r");
	if (fp == NULL)
	{
		//		nd_tb           = new DTable<ND>(1);
		//		bs_tb           = new DTable<BS>(1);
		//		r_nd_cb_tb      = new DTable<R_ND_CB>(1);
		//		r_nd_sec_tb     = new DTable<R_ND_SEC>(1);
		//		r_nd_xf_tb      = new DTable<R_ND_XF>(1);
		//		r_xfmr_xf_tb      = new DTable<R_XFMR_XF>(1);

		return -1;
	}

	while (fgets(line, sizeof(line), fp) != NULL)
	{

		//����
		strcpy(flagField_b, "<Breaker");
		strcpy(flagField_e, "</Breaker");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 0;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//��բ
		strcpy(flagField_b, "<Disconnector");
		strcpy(flagField_e, "</Disconnector");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 1;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//ĸ��
		strcpy(flagField_b, "<BusbarSection");
		strcpy(flagField_e, "</BusbarSection");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 2;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//��·
		strcpy(flagField_b, "<ACLineSegment");
		strcpy(flagField_e, "</ACLineSegment");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 3;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//��·��
		strcpy(flagField_b, "<ACLineDot");
		strcpy(flagField_e, "</ACLineDot");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 4;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//����
		strcpy(flagField_b, "<PowerTransformer");
		strcpy(flagField_e, "</PowerTransformer");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 5;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//����
		strcpy(flagField_b, "<TransformerWinding");
		strcpy(flagField_e, "</TransformerWinding");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 6;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//����
		strcpy(flagField_b, "<Load");
		strcpy(flagField_e, "</Load");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 7;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//��վ
		strcpy(flagField_b, "<Substation");
		strcpy(flagField_e, "</Substation");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 8;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//��׼��ѹ�ȼ�
		strcpy(flagField_b, "<BaseVoltage");
		strcpy(flagField_e, "</BaseVoltage");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 9;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//�ֲ�ʽ��Դ
		strcpy(flagField_b, "<SynchronousMachine");
		strcpy(flagField_e, "</SynchronousMachine");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 10;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//��ѹ�ȼ�
		strcpy(flagField_b, "<VoltageLevel");
		strcpy(flagField_e, "</VoltageLevel");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 11;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//�ص�
		strcpy(flagField_b, "<GroundDisconnector");
		strcpy(flagField_e, "</GroundDisconnector");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 12;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//���ݵ翹
		strcpy(flagField_b, "<ShuntCompensator");
		strcpy(flagField_e, "</ShuntCompensator");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 13;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}

		//��ѹ���ֽ�ͷ
		strcpy(flagField_b, "<TapChangerType");
		strcpy(flagField_e, "</TapChangerType");

		if (strstr(line, flagField_b) != NULL)
		{
			n_line = 0;
			flag_b = 1;
			type = 14;
			continue;//ȡ��һ��
		}

		if (strstr(line, flagField_e) != NULL)
		{
			n_line = flag_b = 0;
			continue;//ȡ��һ��
		}
		//////////////////////////////////////////////////////////////////////////


				//��Ҫ���������в��ܶ����Լ�������
		if (n_line > 2)
		{
			p = line;
			switch (type)
			{
			case 0: //breaker
			{
				//sscanf(p,"%d%d%d%d%d%d%d",&index_breaker[0],&index_breaker[1],&index_breaker[2],&index_breaker[3],&index_breaker[4],&index_breaker[5],&index_breaker[6]);
				//��ʱֻҪ5��,*��ʾҪ����
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d", &index_breaker[0], &index_breaker[1], &index_breaker[2], &index_breaker[3], &index_breaker[4], &index_breaker[5]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d", &type_breaker[0], &type_breaker[1], &type_breaker[2], &type_breaker[3], &type_breaker[4], &type_breaker[5]);
			}

			break;

			case 1: //disconnector
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d", &index_disconnector[0], &index_disconnector[1], &index_disconnector[2], &index_disconnector[3], &index_disconnector[4], &index_disconnector[5]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d", &type_disconnector[0], &type_disconnector[1], &type_disconnector[2], &type_disconnector[3], &type_disconnector[4], &type_disconnector[5]);
			}

			break;
			case 2: //ĸ��
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d", &index_bus[0], &index_bus[1], &index_bus[2], &index_bus[3], &index_bus[4], &index_bus[5]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d", &type_bus[0], &type_bus[1], &type_bus[2], &type_bus[3], &type_bus[4], &type_bus[5]);
			}

			break;
			case 3: //��·
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d%d%d%d", &index_ac[0], &index_ac[1], &index_ac[2], &index_ac[3], &index_ac[4], &index_ac[5], &index_ac[6], &index_ac[7], &index_ac[8], &index_ac[9], &index_ac[10], &index_ac[11], &index_ac[12]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d%d%d%d", &type_ac[0], &type_ac[1], &type_ac[2], &type_ac[3], &type_ac[4], &type_ac[5], &type_ac[6], &type_ac[7], &type_ac[8], &type_ac[9], &type_ac[10], &type_ac[11], &type_ac[12]);
			}

			break;
			case 4: //�߶ζ˵㣬һ����·�����2���߶ζ˵�
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d%d", &index_acdot[0], &index_acdot[1], &index_acdot[2], &index_acdot[3], &index_acdot[4], &index_acdot[5], &index_acdot[6], &index_acdot[7], &index_acdot[8], &index_acdot[9]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d%d", &type_acdot[0], &type_acdot[1], &type_acdot[2], &type_acdot[3], &type_acdot[4], &type_acdot[5], &type_acdot[6], &type_acdot[7], &type_acdot[8], &type_acdot[9]);
			}
			break;

			case 5: //����
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d", &index_xfmr[0], &index_xfmr[1], &index_xfmr[2], &index_xfmr[3], &index_xfmr[4], &index_xfmr[5]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d", &type_xfmr[0], &type_xfmr[1], &type_xfmr[2], &type_xfmr[3], &type_xfmr[4], &type_xfmr[5]);
			}
			break;

			case 6: //����
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", &index_xf[0], &index_xf[1], &index_xf[2], &index_xf[3], &index_xf[4], &index_xf[5], &index_xf[6], &index_xf[7], &index_xf[8], &index_xf[9], &index_xf[10], &index_xf[11], &index_xf[12], &index_xf[13], &index_xf[14], &index_xf[15]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", &type_xf[0], &type_xf[1], &type_xf[2], &type_xf[3], &type_xf[4], &type_xf[5], &type_xf[6], &type_xf[7], &type_xf[8], &type_xf[9], &type_xf[10], &type_xf[11], &type_xf[12], &type_xf[13], &type_xf[14], &type_xf[15]);
			}
			break;

			case 7: //����
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d", &index_ld[0], &index_ld[1], &index_ld[2], &index_ld[3], &index_ld[4], &index_ld[5], &index_ld[6], &index_ld[7]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d", &type_ld[0], &type_ld[1], &type_ld[2], &type_ld[3], &type_ld[4], &type_ld[5], &type_ld[6], &type_ld[7]);
			}
			break;

			case 8: //��վ
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d", &index_st[0], &index_st[1], &index_st[2], &index_st[3], &index_st[4], &index_st[5]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d", &type_st[0], &type_st[1], &type_st[2], &type_st[3], &type_st[4], &type_st[5]);
			}
			break;

			case 9: //��׼��ѹ�ȼ���ȷ���ļ��ֵ�ѹ�ȼ�������110kV,220kV,500kV��
			{
				if (n_line == 3)
					sscanf(p, "%d%d%d", &index_base_v[0], &index_base_v[1], &index_base_v[2]);
				if (n_line == 4)
					sscanf(p, "%d%d%d", &type_base_v[0], &type_base_v[1], &type_base_v[2]);
			}
			break;

			case 10: //�ֲ�ʽ��Դ
			{
				//sscanf(p,filter_sy,id,descr,type,nd,&v,&w,&r);

				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", &index_sy[0], &index_sy[1], &index_sy[2], &index_sy[3], &index_sy[4], &index_sy[5], &index_sy[6], &index_sy[7], &index_sy[8], &index_sy[9], &index_sy[10], &index_sy[11], &index_sy[12]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", &type_sy[0], &type_sy[1], &type_sy[2], &type_sy[3], &type_sy[4], &type_sy[5], &type_sy[6], &type_sy[7], &type_sy[8], &type_sy[9], &type_sy[10], &type_sy[11], &type_sy[12]);
			}
			break;

			case 11: //��ѹ�ȼ���ÿ���豸�����������
			{
				//sscanf(p,filter_vl,id,descr,type,nd,&v,&w,&r);

				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d", &index_vl[0], &index_vl[1], &index_vl[2], &index_vl[3], &index_vl[4], &index_vl[5], &index_vl[6]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d", &type_vl[0], &type_vl[1], &type_vl[2], &type_vl[3], &type_vl[4], &type_vl[5], &type_vl[6]);
			}
			break;

			case 12: //�ص�
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d", &index_gd[0], &index_gd[1], &index_gd[2], &index_gd[3], &index_gd[4], &index_gd[5]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d", &type_gd[0], &type_gd[1], &type_gd[2], &type_gd[3], &type_gd[4], &type_gd[5]);
			}
			break;

			case 13: //���ݵ翹
			{
				if (n_line == 3)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d", &index_cp[0], &index_cp[1], &index_cp[2], &index_cp[3], &index_cp[4], &index_cp[5], &index_cp[6], &index_cp[7], &index_cp[8]);
				if (n_line == 4)
					sscanf(p, "%d%*d%d%d%d%d%d%d%d%d", &type_cp[0], &type_cp[1], &type_cp[2], &type_cp[3], &type_cp[4], &type_cp[5], &type_cp[6], &type_cp[7], &type_cp[8]);
			}
			break;

			case 14: //��ѹ���ֽ�ͷ
			{
				if (n_line == 3)
					sscanf(p, "%d%d%d%d%d%d%d%d", &index_tap[0], &index_tap[1], &index_tap[2], &index_tap[3], &index_tap[4], &index_tap[5], &index_tap[6], &index_tap[7]);
				if (n_line == 4)
					sscanf(p, "%d%d%d%d%d%d%d%d", &type_tap[0], &type_tap[1], &type_tap[2], &type_tap[3], &type_tap[4], &type_tap[5], &type_tap[6], &type_tap[7]);
			}
			break;

			}
		}

		if (flag_b == 1)
			n_line++;

	}
	fclose(fp);


	//����Ӧ��CIME�ļ�
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		//		nd_tb           = new DTable<ND>(1);
		//		bs_tb           = new DTable<BS>(1);
		//		r_nd_cb_tb      = new DTable<R_ND_CB>(1);
		//		r_nd_sec_tb     = new DTable<R_ND_SEC>(1);
		//		r_nd_xf_tb      = new DTable<R_ND_XF>(1);
		//		r_xfmr_xf_tb      = new DTable<R_XFMR_XF>(1);
		return -1;
	}

	//��ȡ��ǰ����ʽ�ȶ����
	lld->getFilter(index_breaker, type_breaker, filter_breaker);//���õ�5�������������ֶ����������+1��
	lld->getFilter(index_disconnector, type_disconnector, filter_disconnector);
	lld->getFilter(index_bus, type_bus, filter_bus);
	lld->getFilter(index_ac, type_ac, filter_ac);
	lld->getFilter(index_acdot, type_acdot, filter_acdot);
	lld->getFilter(index_xfmr, type_xfmr, filter_xfmr);
	lld->getFilter(index_xf, type_xf, filter_xf);
	lld->getFilter(index_ld, type_ld, filter_ld);
	lld->getFilter(index_st, type_st, filter_st);
	lld->getFilter(index_base_v, type_base_v, filter_base_v);
	lld->getFilter(index_sy, type_sy, filter_sy);
	lld->getFilter(index_vl, type_vl, filter_vl);
	lld->getFilter(index_gd, type_sy, filter_gd);
	lld->getFilter(index_cp, type_vl, filter_cp);
	lld->getFilter(index_tap, type_vl, filter_tap);

	n_cb = n_sec = n_secp = n_bus = n_xf = n_xfmr = n_ld = n_st = n_so = n_nd = n_base_v = n_vl = n_cp = n_tap = 0;
	flag_b = 0;
	n_line = 0;
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		//ֻ�д�<�ĲŽ����жϣ������ʱ
		if (strstr(line, "<") != NULL)
		{
			//����
			strcpy(flagField_b, "<Breaker");
			strcpy(flagField_e, "</Breaker");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 0;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//��բ
			strcpy(flagField_b, "<Disconnector");
			strcpy(flagField_e, "</Disconnector");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 1;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//ĸ��
			strcpy(flagField_b, "<BusbarSection");
			strcpy(flagField_e, "</BusbarSection");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 2;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//��·
			strcpy(flagField_b, "<ACLineSegment");
			strcpy(flagField_e, "</ACLineSegment");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 3;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//��·��
			strcpy(flagField_b, "<ACLineDot");
			strcpy(flagField_e, "</ACLineDot");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 4;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//����
			strcpy(flagField_b, "<PowerTransformer");
			strcpy(flagField_e, "</PowerTransformer");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 5;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//����
			strcpy(flagField_b, "<TransformerWinding");
			strcpy(flagField_e, "</TransformerWinding");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 6;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//����
			strcpy(flagField_b, "<Load");
			strcpy(flagField_e, "</Load");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 7;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//��վ
			strcpy(flagField_b, "<Substation");
			strcpy(flagField_e, "</Substation");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 8;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//��׼��ѹ
			strcpy(flagField_b, "<BaseVoltage");
			strcpy(flagField_e, "</BaseVoltage");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 9;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//�ֲ�ʽ��Դ
			strcpy(flagField_b, "<SynchronousMachine");
			strcpy(flagField_e, "</SynchronousMachine");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 10;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//��ѹ�ȼ�
			strcpy(flagField_b, "<VoltageLevel");
			strcpy(flagField_e, "</VoltageLevel");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 11;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//�ص�
			strcpy(flagField_b, "<GroundDisconnector");
			strcpy(flagField_e, "</GroundDisconnector");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 12;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//���ݵ翹
			strcpy(flagField_b, "<ShuntCompensator");
			strcpy(flagField_e, "</ShuntCompensator");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 13;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}

			//��ѹ���ֽ�ͷ
			strcpy(flagField_b, "<TapChangerType");
			strcpy(flagField_e, "</TapChangerType");

			if (strstr(line, flagField_b) != NULL)
			{
				n_line = 0;
				flag_b = 1;
				type = 14;
				continue;//ȡ��һ��
			}

			if (strstr(line, flagField_e) != NULL)
			{
				n_line = flag_b = 0;
				continue;//ȡ��һ��
			}
		}

		if (n_line > 1)
		{
			//			p=strtok(line,d);//��һ�ε���,��Ҫ����Ĵ�������ַs     
			//			n_field = 0;
			switch (type)
			{
			case 0: //breaker
			{
				sscanf(p, filter_breaker, id, descr, nd, znd, ref, &iflag);

				//				if (strcmp(id,"114841792511214984")==0)
				//				{
				//					int deg=-1;
				//				}


								//����ڵ㶼û�У�����Ҫ���뵽������
				if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
					|| (strlen(znd) == 0) || (strcmp(znd, "0") == 0) || (strcmp(znd, "-1") == 0) || (strcmp(znd, "NULL") == 0)
					)
					break;

				CB* cb = new CB;
				p = line;

				//				for(i=0; i<12; i++)
				//				{
				//					bFind = 0;
				//					for(j=0; j<5; j++)
				//					{
				//						if (i == index_breaker[j])
				//						{
				//							if (j==4)
				//								strcat(filter,"%d");
				//							else
				//								strcat(filter,"%s");
				//							bFind = 1;
				//							break;
				//						}
				//					}
				//					if (!bFind)
				//					{
				//						strcat(filter,"%*s");
				//					}
				//				}
				//					sscanf(p,"%*s%*s%s%*s%s%*s%s%s%*s%*s%*s%d",cb->id,cb->descr,cb->nd,cb->znd,&(cb->q));
				strcpy(cb->id, id);
				strcpy(cb->descr, descr);
				strcpy(cb->nd, nd);
				strcpy(cb->znd, znd);

				//pnd = new ND;
				strcpy(pnd->id, nd);
				nd_tb_t->Append(*pnd);
				//delete pnd;
				n_nd++;

				//pnd = new ND;
				strcpy(pnd->id, znd);
				nd_tb_t->Append(*pnd);
				//delete pnd;
				n_nd++;

				strcpy(cb->fdid, ref);//������վ
				if ((strcmp(iflag, "0") == 0) || (strcmp(iflag, "false") == 0))
				{
					cb->q |= CB_OPEN;
				}
				if ((strcmp(iflag, "1") == 0) || (strcmp(iflag, "true") == 0))
				{
					cb->q |= CB_LIVE;
				}

				cb->q |= Q_BREAKER;

				cb_tb_t->Append(*cb);
				n_cb++;
				delete cb;
			}

			break;

			case 1: //disconnector
			{
				sscanf(p, filter_disconnector, id, descr, nd, znd, ref, &iflag);

				//				if (strcmp(id,"114841792511214984")==0)
				//				{
				//					int deg=-1;
				//				}
								//����ڵ㶼û�У�����Ҫ���뵽������
				if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
					|| (strlen(znd) == 0) || (strcmp(znd, "0") == 0) || (strcmp(znd, "-1") == 0) || (strcmp(znd, "NULL") == 0)
					)
					break;

				CB* cb = new CB;
				//				ND* pnd = new ND;

				strcpy(cb->id, id);
				strcpy(cb->descr, descr);
				strcpy(cb->nd, nd);
				strcpy(cb->znd, znd);

				strcpy(pnd->id, nd);
				nd_tb_t->Append(*pnd);
				n_nd++;
				strcpy(pnd->id, znd);
				nd_tb_t->Append(*pnd);
				n_nd++;

				strcpy(cb->fdid, ref);//������վ
				if ((strcmp(iflag, "0") == 0) || (strcmp(iflag, "false") == 0))
				{
					cb->q |= CB_OPEN;
				}
				if ((strcmp(iflag, "1") == 0) || (strcmp(iflag, "true") == 0))
				{
					cb->q |= CB_LIVE;
				}

				cb->q |= Q_DISCONNECTOR;

				cb_tb_t->Append(*cb);
				n_cb++;
				delete cb;
			}

			break;
			case 2: //ĸ��
			{
				sscanf(p, filter_bus, id, descr, nd, ref, vl, &w);

				//����ڵ㶼û�У�����Ҫ���뵽������
				if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
					//					|| (strlen(znd)==0) || (strcmp(znd,"0")==0) || (strcmp(znd,"-1")==0)  || (strcmp(znd,"NULL")==0) 
					)
					break;

				//if (strcmpvl)


				BUS* bus = new BUS;

				strcpy(bus->id, id);
				strcpy(bus->descr, descr);
				strcpy(bus->nd, nd);
				strcpy(bus->algv, vl);//ĸ�ߵĵ�ѹ�ȼ�

				strcpy(pnd->id, nd);
				nd_tb_t->Append(*pnd);
				n_nd++;
				//				strcpy(pnd->id,znd);
				//				nd_tb_t->Append(*pnd);
				//				n_nd++;

				strcpy(bus->fdid, ref);//������վ
				bus->v = atof(w);//ĸ�ߵ������ѹ

				bus_tb_t->Append(*bus);
				n_bus++;
				delete bus;
			}

			break;
			case 3: //��·
			{
				SEC* sec = new SEC;

				sscanf(p, filter_ac, id, descr, nd, znd, &w, vl, &r1, &x1, &b1, &r0, &x0, &b0);
				strcpy(sec->id, id);
				strcpy(sec->descr, descr);
				strcpy(sec->st_b, nd);
				strcpy(sec->st_e, znd);
				strcpy(sec->vl, vl);

				sec->transP = atof(w);//����������
				sec->r1 = atof(r1);
				sec->x1 = atof(x1);
				sec->b1 = atof(b1);
				sec->r0 = atof(r0);
				sec->x0 = atof(x0);
				sec->b0 = atof(b0);

				sec_tb_t->Append(*sec);
				n_sec++;
				delete sec;
			}

			break;
			case 4: //�߶ζ˵㣬һ����·�����2���߶ζ˵�
			{
				sscanf(p, filter_acdot, id, descr, ref, znd, nd, vl, &w, &r);

				//����ڵ㶼û�У�����Ҫ���뵽������
				if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
					//					|| (strlen(znd)==0) || (strcmp(znd,"0")==0) || (strcmp(znd,"-1")==0)  || (strcmp(znd,"NULL")==0) 
					)
					break;

				SECP* secp = new SECP;

				strcpy(secp->id, id);
				strcpy(secp->descr, descr);
				strcpy(secp->fdid, ref);//�߶���������·
				strcpy(secp->st, znd);//�߶���������·
				strcpy(secp->nd, nd);
				strcpy(secp->baseVL, vl);
				secp->w = atof(w);
				secp->r = atof(r);

				secp_tb_t->Append(*secp);
				n_secp++;
				delete secp;
			}
			break;

			case 5: //����
			{
				XFMR* xfmr = new XFMR;

				sscanf(p, filter_xfmr, id, descr, nd, ref, &w, &r);
				strcpy(xfmr->id, id);
				strcpy(xfmr->descr, descr);
				//strcpy(xfmr->ver,nd);//��ѹ�����ͣ����Ʊ䣬���Ʊ�
				if (strcmp(nd, "���Ʊ�") == 0)
				{
					xfmr->type = 3;
				}
				else
				{
					xfmr->type = 2;
				}

				strcpy(xfmr->st, ref);
				strcpy(xfmr->htapty, w);
				strcpy(xfmr->algph, r);

				xfmr_tb_t->Append(*xfmr);
				n_xfmr++;
				delete xfmr;
			}
			break;

			case 6: //����
			{
				sscanf(p, filter_xf, id, descr, znd, ref, nd, vl, vTarget, &b1, &b0, &r1, &x1, &r0, &x0, &w, &r);

				//				if (strcmp(id,"117375067049952152")==0)
				//				{
				//					int debyg = 1;
				//				}


								//����ڵ㶼û�У�����Ҫ���뵽������
				if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
					//					|| (strlen(znd)==0) || (strcmp(znd,"0")==0) || (strcmp(znd,"-1")==0)  || (strcmp(znd,"NULL")==0) 
					)
					break;

				XF* xf = new XF;

				strcpy(xf->id, id);
				strcpy(xf->descr, descr);
				strcpy(xf->fdid, ref);//�߶���������
				strcpy(xf->nd, nd);

				strcpy(xf->ver, znd);//�������ͣ��Ǹ�ѹ�໹�ǵ�ѹ��
				strcpy(xf->vl, vl);//��ѹ�ȼ�
				strcpy(xf->tapty, vTarget);//����ĳ�ͷ
				xf->w = atof(w);//�й�����
				xf->rz = atof(r);//�޹�����
				xf->kvnom = atof(b1);//��·���
				xf->zkvnom = atof(b0);//��·��ѹ�ٷֱ�
				xf->r1 = atof(r1);//����
				xf->x1 = atof(x1);
				xf->mvanom = atof(r0);//����
				xf->r_ = atof(x0);

				xf_tb_t->Append(*xf);
				n_xf++;
				delete xf;
			}
			break;

			case 7: //����
			{
				sscanf(p, filter_ld, id, descr, ref, nd, vl, &w, &r);

				//����ڵ㶼û�У�����Ҫ���뵽������
				if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
					//					|| (strlen(znd)==0) || (strcmp(znd,"0")==0) || (strcmp(znd,"-1")==0)  || (strcmp(znd,"NULL")==0) 
					)
					break;

				LD* ld = new LD;

				strcpy(ld->id, id);
				strcpy(ld->descr, descr);
				strcpy(ld->nd, nd);
				strcpy(ld->vl, vl);
				strcpy(ld->fdid, ref);
				ld->w = atof(w);
				ld->r = atof(r);

				strcpy(pnd->id, nd);
				ld->type |= Q_LOAD;

				nd_tb_t->Append(*pnd);
				n_nd++;
				//				strcpy(pnd->id,znd);
				//				nd_tb_t->Append(*pnd);
				//				n_nd++;

				ld_tb_t->Append(*ld);
				n_ld++;
				delete ld;
			}
			break;

			case 8: //��վ
			{
				ST* st = new ST;

				sscanf(p, filter_st, id, descr, nd, ref, &w, &r);
				strcpy(st->id, id);
				strcpy(st->descr, descr);
				strcpy(st->abb, nd);//��վ����
				strcpy(st->fdid, ref);//��վ��������

				if (strcmp(w, "NULL") == 0)
				{
				}
				else
				{
					st->wso = atof(w);
					st->rso = atof(r);
				}

				st_tb_t->Append(*st);
				n_st++;
				delete st;
			}
			break;

			case 9: //��׼��ѹ�ȼ�
			{
				sscanf(p, filter_base_v, id, descr, &w);
				strcpy(basevl[n_base_v].mRID, id);//��ѹ�ȼ���ID��ÿһ���豸����������
				strcpy(basevl[n_base_v].name, descr);
				basevl[n_base_v].nomkV = atof(w);//��ѹ�ȼ�ֵ

				n_base_v++;
			}
			break;

			case 10: //�ֲ�ʽ��Դ
			{
				SO* so = new SO;

				//sscanf(p,"%d%*d%d%d%d%d%d%d%d%d%d%d%d%d%d",&index_sy[0],&index_sy[1],&index_sy[2],&index_sy[3],&index_sy[4],&index_sy[5],&index_sy[6],&index_sy[7],&index_sy[8],&index_sy[9],&index_sy[10],&index_sy[11],&index_sy[12],&index_sy[13],&index_sy[14]);

				sscanf(p, filter_sy, id, descr, ref, nd, &znd, &vl, &vTarget, &r1, &x1, &r0, &x0, &w, &r);
				strcpy(so->id, id);
				strcpy(so->descr, descr);
				//�ڵ����ͣ�1ΪPQ�� 0Ϊƽ��ڵ㣬2ΪPV�ڵ㣬3ΪPI�ڵ�
				if (strcmp(ref, "0") == 0)//ƽ��ڵ�
				{
					so->slackavr = 0;
					so->vtarget = atof(vTarget);//�������ĵ�ѹֵ��ƽ��ڵ��PV�ڵ�
				}
				else
					if (strcmp(ref, "1") == 0)//PQ�ڵ�
						so->slackavr = 1;
					else
						if (strcmp(ref, "2") == 0)//PV�ڵ�
							so->slackavr = -1;
						else
							if (strcmp(ref, "3") == 0)//PI�ڵ�
								so->slackavr = 2;
				strcpy(so->nd, nd);
				strcpy(so->fdid, znd);//������վ��ʶ
				strcpy(so->vl, vl);//������ѹ�ȼ�
				so->sw = atof(r1);//����
				so->sr = atof(x1);//
				so->pLoss = atof(r0);//����
				so->qLoss = atof(x0);//

				so->w = atof(w);//��Դ�й�������Ϊ��ֵ
				so->r = atof(r);//��Դ�޹�����

				so_tb_t->Append(*so);
				n_so++;
				delete so;
			}
			break;

			case 11: //��ѹ�ȼ�
			{
				VL* vl = new VL;
				sscanf(p, filter_vl, id, descr, &w, &r, &vTarget, &nd);
				strcpy(vl->mRID, id);//��ѹ�ȼ���ID��ÿһ���豸����������
				strcpy(vl->name, descr);
				vl->highkV = atof(w);//��ѹ�ȼ�ֵ
				vl->lowkV = atof(r);//��ѹ�ȼ�ֵ
				strcpy(vl->BaseVoltage, nd);//��ѹ�ȼ���ID��ÿһ���豸����������
				strcpy(vl->Substation, vTarget);

				vl_tb_t->Append(*vl);
				delete vl;
				n_vl++;
			}
			break;


			case 12: //gd
			{
				sscanf(p, filter_gd, id, descr, nd, znd, ref, &iflag);

				//				if (strcmp(id,"114841792511214984")==0)
				//				{
				//					int deg=-1;
				//				}
								//����ڵ㶼û�У�����Ҫ���뵽������
				if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
					|| (strlen(znd) == 0) || (strcmp(znd, "0") == 0) || (strcmp(znd, "-1") == 0) || (strcmp(znd, "NULL") == 0)
					)
					break;

				CB* cb = new CB;
				//				ND* pnd = new ND;

				strcpy(cb->id, id);
				strcpy(cb->descr, descr);
				strcpy(cb->nd, nd);
				strcpy(cb->znd, znd);

				strcpy(pnd->id, nd);
				nd_tb_t->Append(*pnd);
				n_nd++;
				strcpy(pnd->id, znd);
				nd_tb_t->Append(*pnd);
				n_nd++;

				strcpy(cb->fdid, ref);//������վ
				if ((strcmp(iflag, "0") == 0) || (strcmp(iflag, "false") == 0))
					cb->q = 0;
				if ((strcmp(iflag, "1") == 0) || (strcmp(iflag, "true") == 0))
					cb->q = 1;

				cb_tb_t->Append(*cb);
				n_cb++;
				delete cb;
			}

			break;

			case 13: //���ݵ翹
			{
				sscanf(p, filter_cp, id, descr, &w, nd, vl, ref, &r);

				//����ڵ㶼û�У�����Ҫ���뵽������
				if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
					//					|| (strlen(znd)==0) || (strcmp(znd,"0")==0) || (strcmp(znd,"-1")==0)  || (strcmp(znd,"NULL")==0) 
					)
					break;

				CP* cp = new CP;

				strcpy(cp->id, id);
				strcpy(cp->descr, descr);
				strcpy(cp->nd, nd);
				strcpy(cp->algq, vl);
				strcpy(cp->fdid, ref);
				cp->mrnom = atof(w);
				cp->r = atof(r);

				strcpy(pnd->id, nd);
				nd_tb_t->Append(*pnd);
				n_nd++;
				//				strcpy(pnd->id,znd);
				//				nd_tb_t->Append(*pnd);
				//				n_nd++;

				cp_tb_t->Append(*cp);
				n_cp++;
				delete cp;
			}
			break;

			//////////////////////////////////////////////////////////////////////////
			//1.��ͷ���ֽӿ��أ��������ڸ�ѹ�࣬����ͨ���ֽӿ��صĵ���С����ѹ���ȸ�
			//2.���һ������һ���������С�ĵ�����ѹ���ѹ=��ѹ���ѹ/��ȣ���ѹ��Ϊϵͳ��ѹ�����ɵ������������
			//3.���һ�����������ٵ�һ����
			//4.�м䵵�Ӹ�ѹ��Ȧ�Ķֵ
			//5.�������ѹ���ı��ѹ��ֽ�ͷ�ɸı��С���ѹ����ѹ��ֽ�ͷֻ�ı���ѹ��ֻ�ı��ѹ��Ҫ�ߡ���һ���


			case 14: //��ѹ���ֽ�ͷ
			{//��ʶ �ֽ�ͷ������ ���λ �е��ѹ ���λ ��С��λ ���� ��λ����
				sscanf(p, filter_tap, id, descr, &r0, &vl, &w, &r, &x0, &ref);

				TAPTY* tap = new TAPTY;

				strcpy(tap->id, id);
				tap->mn = atof(w);//��С��λ
				tap->mx = atof(r);//���λ
				tap->nom = atof(ref);//��ǰ��λ
				tap->step = atof(x0);//��λ����

				tapty_tb_t->Append(*tap);
				n_tap++;
				delete tap;
			}
			break;
			}
		}

		if (flag_b == 1)
			n_line++;
	}
	fclose(fp);

	//��ĸ��������ѡ����ѹ�ȼ���ĸ��+�糧ĸ������Ϊ��Դ��
	//���ֻ����220kV��110kV��220kV��ĸ�߾��ǵ�Դ��
	//500+220ĸ����Ϊ��Դ��������
		//����ĵ�ѹ�ȼ������滻	


	//
	////������,�鿴����Ĳ�����
	//	int count;
	//	ofstream outfile("data_src.txt",ios::out);
	// 
	// 	outfile<<"   "<< endl;
	// 	outfile<<"xfmr:   "<< endl;
	// 
	// 	XFMR* xfmr = xfmr_tb_t->GetTable();
	//  	count = xfmr_tb_t->GetCount();
	//  	outfile<<"id       "<<"descr    "<<"st       "<<"NoLoadLoss       "<<"ExcitingCurrent       "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(xfmr+i)->id<<"   "<<(xfmr+i)->descr<<"   "<<(xfmr+i)->st<<"   "<<(xfmr+i)->htapty<<"   "<<(xfmr+i)->algph<< endl;
	//  	}
	// 
	// 	outfile<<"   "<< endl;
	// 	outfile<<"xf:   "<< endl;
	// 	XF* xf = xf_tb_t->GetTable();
	//  	count = xf_tb_t->GetCount();
	//	outfile<<"id       "<<"descr    "<<"type       "<<"fdid       "<<"nd     "<<"w     "<<"r     "<<"loadLoss     "<<"leakageImpedence     "<<"r+     "<<"x+     "<<"r0     "<<"x0     "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(xf+i)->id<<"   "<<(xf+i)->descr<<"   "<<(xf+i)->ver<<"   "<<(xf+i)->fdid<<"   "<<(xf+i)->nd<<"  "<<(xf+i)->w<<"   "<<(xf+i)->rz<<"  "<<(xf+i)->kvnom<<"  "<<(xf+i)->zkvnom<<"  "<<(xf+i)->r<<"  "<<(xf+i)->x<<"  "<<(xf+i)->mvanom<<"  "<<(xf+i)->r_<< endl;
	//  	}
	// 
	// 	outfile<<"   "<< endl;
	// 	outfile<<"bus:   "<< endl;
	// 	BUS* bus = bus_tb_t->GetTable();
	//  	count = bus_tb_t->GetCount();
	//	outfile<<"busid       "<<"descr       "<<"nd    "<<"v       "<<"st       "<<"vl       "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(bus+i)->id<<"   "<<(bus+i)->descr<<"   "<<(bus+i)->nd<<"   "<<(bus+i)->v<<"  "<< (bus+i)->fdid<<"  "<< (bus+i)->algv<<endl;
	//  	}
	//
	//	outfile<<"   "<< endl;
	// 	outfile<<"cb:   "<< endl;
	//  	CB* cb = cb_tb_t->GetTable();
	//  	count = cb_tb_t->GetCount();
	//	outfile<<"id       "<<"descr       "<<"nd    "<<"znd       "<<"q     "<<"st     "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(cb+i)->id<<"   "<<(cb+i)->descr<<"   "<<(cb+i)->nd<<"   "<<(cb+i)->znd<<"   "<<(cb+i)->q<<"  "<<(cb+i)->fdid<<"  "<< endl;
	//  	}
	// 	
	// 	outfile<<"   "<< endl;
	// 	outfile<<"sec:   "<< endl;
	//  	SEC* sec = sec_tb_t->GetTable();
	//  	count = sec_tb_t->GetCount();
	//	outfile<<"id       "<<"descr       "<<"nd    "<<"znd       "<<  endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(sec+i)->id<<"   "<<(sec+i)->descr<<"   "<<(sec+i)->nd<<"   "<<(sec+i)->znd<<endl;
	//  	}
	//
	// 	outfile<<"   "<< endl;
	// 	outfile<<"secp:   "<< endl;
	//  	SECP* secp = secp_tb_t->GetTable();
	//  	count = secp_tb_t->GetCount();
	//	outfile<<"id       "<<"descr       "<<"fdid    "<<"nd    "<<"w    "<<"r       "<<"st       "<<  endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(secp+i)->id<<"   "<<(secp+i)->descr<<"   "<<(secp+i)->fdid<<"   "<<(secp+i)->nd<<"   "<<(secp+i)->w<<"   "<<(secp+i)->r<<"  "<<(secp+i)->st<<endl;
	//  	}
	//
	//	outfile<<"   "<< endl;
	// 	outfile<<"ld:   "<< endl;
	// 	LD* ld = ld_tb_t->GetTable();
	//  	count = ld_tb_t->GetCount();
	//	outfile<<"ldid       "<<"descr       "<<"nd    "<<"w    "<<"r    "<<"st    "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(ld+i)->id<<"   "<<(ld+i)->descr<<"   "<<(ld+i)->nd<<"   "<<(ld+i)->w<<"  "<< (ld+i)->r <<"  "<<(ld+i)->fdid <<endl;
	//  	}
	// 	outfile<<"   "<< endl;
	// 	
	//	outfile<<"   "<< endl;
	// 	outfile<<"cp:   "<< endl;
	// 	CP* cp = cp_tb_t->GetTable();
	//  	count = cp_tb_t->GetCount();
	//	outfile<<"cpid       "<<"descr       "<<"nd    "<<"mrnom    "<<"r    "<<"st    "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(cp+i)->id<<"   "<<(cp+i)->descr<<"   "<<(cp+i)->nd<<"   "<<(cp+i)->mrnom<<"  "<< (cp+i)->r <<"  "<<(cp+i)->fdid <<endl;
	//  	}
	// 	outfile<<"   "<< endl;
	//
	//	outfile<<"   "<< endl;
	// 	outfile<<"st:   "<< endl;
	// 	ST* st = st_tb_t->GetTable();
	//  	count = st_tb_t->GetCount();
	//	outfile<<"stid       "<<"descr       "<<"type    "<<"subarea       "<<"w    "<<"r    "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(st+i)->id<<"   "<<(st+i)->descr<<"   "<<(st+i)->abb<<"   "<<(st+i)->fdid<<"   "<<(st+i)->wso<<"  "<< (st+i)->rso<<endl;
	//  	}
	// 	outfile<<"   "<< endl;
	//
	//	outfile<<"   "<< endl;
	// 	outfile<<"so:   "<< endl;
	// 	SO* so = so_tb_t->GetTable();
	//  	count = so_tb_t->GetCount();
	//	outfile<<"id       "<<"descr       "<<"nd    "<<"r+    "<<"x+    "<<"r0    "<<"x0    "<<"type    "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(so+i)->id<<"   "<<(so+i)->descr<<"   "<<(so+i)->nd<<"   "<<(so+i)->sw<<"   "<<(so+i)->sr<<"   "<<(so+i)->pLoss<<"   "<<(so+i)->qLoss<<"   "<<(so+i)->slackavr<<endl;
	//  	}
	// 	outfile<<"   "<< endl;
	//
	//	outfile<<"   "<< endl;
	// 	outfile<<"nd:   "<< endl;
	// 	ND* ppnd = nd_tb_t->GetTable();
	//  	count = nd_tb_t->GetCount();
	//	outfile<<"id       "<<endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(ppnd+i)->id<<endl;
	//  	}
	// 	outfile<<"   "<< endl;
	//
	//	outfile.close;	
	//
	//	return 0;

	//////////////////////////////////////////////////////////////////////////
	//��д���ڴ�����ݽ��������γɼ���Ҫ�õ���ʽ���ݽṹ
		//��id�ŵ�˳������
	if (n_bus > 0)
	{
		lld->mx_bus = n_equip = bus_tb_t->Sort(n_bus);
		bus_tb = new DTable<BUS>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			BUS* bus = new BUS[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			bus_tb_t->CopyTable(bus);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			bus_tb->Insert(bus, n_equip);

			delete[] bus;//����ʱ���ڴ��ͷ�
			bus = 0;//�����������ֹ�ͷź�ָ����ָ
		}
	}
	delete bus_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	bus_tb_t = 0;

	if (n_cb > 0)
	{
		lld->mx_cb = n_equip = cb_tb_t->Sort(n_cb);
		cb_tb = new DTable<CB>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			CB* cb = new CB[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			cb_tb_t->CopyTable(cb);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			cb_tb->Insert(cb, n_equip);

			delete[] cb;//����ʱ���ڴ��ͷ�
			cb = 0;//�����������ֹ�ͷź�ָ����ָ
		}
	}
	delete cb_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	cb_tb_t = 0;

	if (n_ld > 0)
	{
		lld->mx_ld = n_equip = ld_tb_t->Sort(n_ld);
		ld_tb = new DTable<LD>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			LD* ld = new LD[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			ld_tb_t->CopyTable(ld);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			ld_tb->Insert(ld, n_equip);

			delete[] ld;//����ʱ���ڴ��ͷ�
			ld = 0;//�����������ֹ�ͷź�ָ����ָ
		}
	}
	delete ld_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	ld_tb_t = 0;

	if (n_cp > 0)
	{
		lld->mx_cp = n_equip = cp_tb_t->Sort(n_cp);
		cp_tb = new DTable<CP>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			CP* cp = new CP[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			cp_tb_t->CopyTable(cp);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			cp_tb->Insert(cp, n_equip);

			delete[] cp;//����ʱ���ڴ��ͷ�
			cp = 0;//�����������ֹ�ͷź�ָ����ָ
		}
	}
	delete cp_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	cp_tb_t = 0;

	if (n_st > 0)
	{
		lld->mx_st = n_equip = st_tb_t->Sort(n_st);
		st_tb = new DTable<ST>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			ST* st = new ST[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			st_tb_t->CopyTable(st);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			st_tb->Insert(st, n_equip);

			delete[] st;//����ʱ���ڴ��ͷ�
			st = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete st_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		st_tb_t = 0;
	}
	else
	{
		st_tb = new DTable<ST>(1);//����һ����¼���Է�ֹ����ɾ��ʱ����
	}

	//��ѹ�ȼ�����Ҫ����������ĺܶ�������Ҫ�õ��������
	if (n_st > 0)
	{
		lld->mx_vl = n_equip = vl_tb_t->Sort(n_vl);
		vl_tb = new DTable<VL>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			VL* vl = new VL[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			vl_tb_t->CopyTable(vl);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			vl_tb->Insert(vl, n_equip);

			delete[] vl;//����ʱ���ڴ��ͷ�
			vl = 0;//�����������ֹ�ͷź�ָ����ָ
		}
	}
	delete vl_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	vl_tb_t = 0;


	//��Ҫ�ڵ�Դ������д�ϵ�ѹ�ȼ��ı�ǩ�������ж���������ߵ�ѹֵ
	if (n_st > 0)
	{
		lld->mx_so = n_equip = so_tb_t->Sort(n_so);
		so_tb = new DTable<SO>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			SO* so = new SO[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			so_tb_t->CopyTable(so);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			so_tb->Insert(so, n_equip);

			delete[] so;//����ʱ���ڴ��ͷ�
			so = 0;//�����������ֹ�ͷź�ָ����ָ
		}
	}
	//lld->mx_so = 0;
	delete so_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	so_tb_t = 0;

	//////////////////////////////////////////////////////////////////////////
	//�������ӹ�ϵ

	//���ڵ��д�ã����ź���
	//���������Ĺ�ϵ��Ҫ������Ҫ���ڵ�
	char  xfmr_id[128], xfmr_id_nd[128];
	int ph, n_equip1;
	char  vl_t[64];
	double vl_d;

	//���ڵ���ڴ�
//	n_cb=n_sec=n_secp=n_bus=n_xf=n_xfmr=n_ld=0;

	if ((n_xfmr > 0) && (n_xf > 0))
	{
		lld->mx_xfmr = n_equip = xfmr_tb_t->Sort(n_xfmr);
		lld->mx_r_xfmr_xf = lld->mx_xf = n_equip1 = xf_tb_t->Sort(n_xf);

		int lv_xf = xf_tb_t->GetCount();
		XF* xf = new XF[lv_xf];
		xf_tb_t->CopyTable(xf);

		XFMR* xfmr_t = new XFMR(); //��ʱ��׼����������

		//check xf one by one
		for (i = 0; i < lv_xf; i++)
		{
			//�����ѹ��������ڵ�ΪNULL��˵����ѹ��δͶ��
			if (strcmp((xf + i)->nd, "NULL") == 0)
			{
				strcpy((xf + i)->nd, (xf + i)->id);
			}

			//1.����ѹ�ȼ�����
			strcpy(vl_t, (xf + i)->vl);
			vl_d = getVLBymrID(vl_t);
			(xf + i)->basevoltage = vl_d;

			//3.������������ѹ���Ĺ�����ϵ
			strcpy(xfmr_id, (xf + i)->fdid);//�õ����������ı�ѹ��id
			strcpy(xfmr_t->id, xfmr_id);
			ph = xfmr_tb_t->FindPh(*xfmr_t);

			//һ����ѹ������������������
			//�������ѹ���ı����Ϊ��������ĵ㣬��ʱ��
			if (ph != -1)
			{
				//�˴���Ҫ�ж����������,��������˺ϲ��Ľ׶�ȡ��������������
			//	if ( (strstr(xfmr_type,"��")!=NULL) || (strstr(xfmr_type,"3")!=NULL) )
				{
					if (strlen((xf + i)->znd) == 0)//ĩ�ڵ�
					{
						sprintf(xfmr_id_nd, "t_%s", xfmr_id);
						strcpy((xf + i)->znd, xfmr_id_nd);
					}
				}
			}
			else
			{
				continue;
			}

			strcpy(nd, (xf + i)->nd);
			strcpy(znd, (xf + i)->znd);
			if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
				|| (strlen(znd) == 0) || (strcmp(znd, "0") == 0) || (strcmp(znd, "-1") == 0) || (strcmp(znd, "NULL") == 0)
				)
				continue;

			strcpy(pnd->id, (xf + i)->nd);
			nd_tb_t->Append(*pnd);
			n_nd++;
			strcpy(pnd->id, (xf + i)->znd);
			nd_tb_t->Append(*pnd);
			n_nd++;
		}
		xf_tb_t->UpdateTable(xf);

		delete[] xf;
		xf = 0;
		lv_xf = 0;
		delete xfmr_t;

		xf_tb = new DTable<XF>(n_equip1);
		if (n_equip1 != 0)
		{
			//read table into buffer
			XF* xf = new XF[n_equip1];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			xf_tb_t->CopyTable(xf);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			xf_tb->Insert(xf, n_equip1);

			delete[] xf;//����ʱ���ڴ��ͷ�
			xf = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete xf_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		xf_tb_t = 0;

		xfmr_tb = new DTable<XFMR>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			XFMR* xfmr = new XFMR[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			xfmr_tb_t->CopyTable(xfmr);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			xfmr_tb->Insert(xfmr, n_equip);

			delete[] xfmr;//����ʱ���ڴ��ͷ�
			xfmr = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete xfmr_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		xfmr_tb_t = 0;

		lld->mx_tapty = n_equip = tapty_tb_t->Sort(n_tap);
		tapty_tb = new DTable<TAPTY>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			TAPTY* tapty = new TAPTY[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			tapty_tb_t->CopyTable(tapty);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			tapty_tb->Insert(tapty, n_equip);

			delete[] tapty;//����ʱ���ڴ��ͷ�
			tapty = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete tapty_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		tapty_tb_t = 0;
	}

	//��·�Ľڵ�ҲҪ���⴦��
	if ((n_sec > 0) && (n_secp > 0))
	{
		lld->mx_sec = n_equip = sec_tb_t->Sort(n_sec);
		lld->mx_secp = n_equip1 = secp_tb_t->Sort(n_secp);
		int lv_secp = secp_tb_t->GetCount();
		if (lv_secp == 0)
		{
		}
		SECP* secp = secp_tb_t->GetTable();
		//secp = secp_tb_t->GetTable();
		char secp_nd[64], secp_st[64];
		SEC* sec_t = new SEC();

		//check secp one by one
		for (i = 0; i < lv_secp; i++)
		{
			//				if (strcmp((secp+i)->id,"116812117096530167")==0)
			//				{
			//					int deg=0;
			//				}
					//�����˵��߶Σ�Ҳ������·�����յ������û��ĩ�ڵ㣬�����и���ֵ
			if (strcmp((secp + i)->nd, "NULL") == 0)
			{
				strcpy((secp + i)->nd, (secp + i)->id);
			}

			strcpy(secp_nd, (secp + i)->nd);//�ڵ��
			strcpy(sec_t->id, (secp + i)->fdid);//�����߶�
			strcpy(secp_st, (secp + i)->st);//������վ

			ph = sec_tb_t->FindPh(*sec_t);

			if (ph != -1)
			{
				if (strlen(sec_t->nd) == 0)//�׽ڵ�,���ȱ�֤д��
				{
					strcpy(sec_t->nd, secp_nd);
					strcpy(sec_t->sep_b, (secp + i)->descr); //��Ӧ���߶�ͷ���д��
					sec_t->w = (secp + i)->w;
					sec_t->r = (secp + i)->r;

					strcpy(nd, secp_nd);
					if ((strlen(nd) == 0) || (strcmp(nd, "0") == 0) || (strcmp(nd, "-1") == 0) || (strcmp(nd, "NULL") == 0)
						//				|| (strlen(znd)==0) || (strcmp(znd,"0")==0) || (strcmp(znd,"-1")==0)  || (strcmp(znd,"NULL")==0) 
						)
						continue;
					strcpy(pnd->id, secp_nd);
					nd_tb_t->Append(*pnd);
					n_nd++;
					//flag = 1;//����д��
				}
				else if (strlen(sec_t->znd) == 0)//ĩ�ڵ�
				{
					strcpy(sec_t->znd, secp_nd);
					strcpy(sec_t->sep_e, (secp + i)->descr);
					sec_t->wz = (secp + i)->w;
					sec_t->rz = (secp + i)->r;

					strcpy(znd, secp_nd);
					if ((strlen(znd) == 0) || (strcmp(znd, "0") == 0) || (strcmp(znd, "-1") == 0) || (strcmp(znd, "NULL") == 0)
						)
						continue;

					strcpy(pnd->id, secp_nd);
					nd_tb_t->Append(*pnd);
					n_nd++;
				}

				//����ѹ�ȼ�����
				strcpy(vl_t, sec_t->vl);
				vl_d = getVLBymrID(vl_t, 1);
				sec_t->basevoltage = vl_d;

				sec_tb_t->UpdateDataByPh(sec_t, ph);
			}
		}
		delete sec_t;

		delete secp_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		secp_tb_t = 0;

		sec_tb = new DTable<SEC>(n_equip);
		if (n_equip != 0)
		{
			//read table into buffer
			SEC* sec = new SEC[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
			sec_tb_t->CopyTable(sec);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

			//update table����ʱ������ļ�¼��д����ʽ������
			sec_tb->Insert(sec, n_equip);

			delete[] sec;//����ʱ���ڴ��ͷ�
			sec = 0;//�����������ֹ�ͷź�ָ����ָ
		}
		delete sec_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
		sec_tb_t = 0;
	}

	lld->mx_nd = n_equip = nd_tb_t->Sort(n_nd);
	nd_tb = new DTable<ND>(n_equip);
	if (n_equip != 0)
	{
		//read table into buffer
		ND* nd = new ND[n_equip];//�ȿ�һ����ʱ�ڴ�������еļ�¼��������
		nd_tb_t->CopyTable(nd);//�����ڵ�co_tb�����м�¼�������������ʱ�ڴ������

		//update table����ʱ������ļ�¼��д����ʽ������
		nd_tb->Insert(nd, n_equip);

		delete[] nd;//����ʱ���ڴ��ͷ�
		nd = 0;//�����������ֹ�ͷź�ָ����ָ
	}
	delete nd_tb_t;//����ʱ���������ڴ�ȫ��ɾ�������ڴ��ڳ���
	nd_tb_t = 0;

	//////////////////////////////////////////////////////////////////////////
	//���ѻ�׼��ѹ�ȼ����㵽��ص��豸�ϣ�ĸ�ߡ���·�����ɡ�����
	int lv_bus = lld->mx_bus;
	if (lv_bus > 0)
	{
		BUS* bus_t = new BUS[lv_bus];
		bus_tb->CopyTable(bus_t);

		for (i = 0; i < lv_bus; i++)
		{
			strcpy(vl_t, (bus_t + i)->algv);
			vl_d = getVLBymrID(vl_t);
			(bus_t + i)->basevoltage = vl_d;
			(bus_t + i)->vl = vl_d;//ĸ�ߵĻ�׼��ѹֵ�����õ������ʱ��Ҫ��
		}
		bus_tb->UpdateTable(bus_t);
		delete[] bus_t;
		bus_t = 0;
	}

	int lv_ld = lld->mx_ld;
	if (lv_ld > 0)
	{
		LD* ld_t = new LD[lv_ld];
		ld_tb->CopyTable(ld_t);

		for (i = 0; i < lv_ld; i++)
		{
			strcpy(vl_t, (ld_t + i)->vl);
			vl_d = getVLBymrID(vl_t);
			(ld_t + i)->basevoltage = vl_d;
		}
		ld_tb->UpdateTable(ld_t);
		delete[] ld_t;
		ld_t = 0;
	}

	int lv_cp = lld->mx_cp;
	if (lv_cp > 0)
	{
		CP* cp_t = new CP[lv_cp];
		cp_tb->CopyTable(cp_t);

		for (i = 0; i < lv_cp; i++)
		{
			strcpy(vl_t, (cp_t + i)->vl);
			vl_d = getVLBymrID(vl_t);
			(cp_t + i)->basevoltage = vl_d;
		}
		cp_tb->UpdateTable(cp_t);
		delete[] cp_t;
		cp_t = 0;
	}

	int lv_so = lld->mx_so;
	if (lv_so > 0)
	{
		SO* so_t = new SO[lv_so];
		so_tb->CopyTable(so_t);

		for (i = 0; i < lv_so; i++)
		{
			strcpy(vl_t, (so_t + i)->vl);
			vl_d = getVLBymrID(vl_t);
			(so_t + i)->basevoltage = vl_d;
		}
		so_tb->UpdateTable(so_t);
		delete[] so_t;
		so_t = 0;
	}

	delete pnd;//����������ʱָ��


//������,��ʽ��ԭʼ��������
	ofstream outfile("preCacl_tp.txt", ios::out);

	//nd_tb->Sequence();
	outfile << "   " << endl;
	outfile << "xfmr:   " << endl;

	XFMR* xfmr = xfmr_tb->GetTable();
	int count = xfmr_tb->GetCount();
	outfile << "id       " << "descr    " << "type       " << "st       " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (xfmr + i)->id << "   " << (xfmr + i)->descr << "   " << (xfmr + i)->ver << "   " << (xfmr + i)->st << endl;
	}

	outfile << "   " << endl;
	TAPTY* tapty = tapty_tb->GetTable();
	count = tapty_tb->GetCount();
	outfile << "id       " << "mn    " << "mx       " << "mon       " << "step       " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (tapty + i)->id << "   " << (tapty + i)->mn << "   " << (tapty + i)->mx << "   " << (tapty + i)->step << endl;
	}

	outfile << "   " << endl;
	outfile << "xf:   " << endl;
	XF* xf = xf_tb->GetTable();
	count = xf_tb->GetCount();
	outfile << "id       " << "descr    " << "type       " << "xfmr       " << "nd     " << "znd     " << "tapy     " << "basevoltage     " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (xf + i)->id << "   " << (xf + i)->descr << "   " << (xf + i)->ver << "   " << (xf + i)->fdid << "   " << (xf + i)->nd << "  " << (xf + i)->znd << "  " << (xf + i)->tapty << "   " << (xf + i)->basevoltage << endl;
	}

	outfile << "   " << endl;
	outfile << "bus:   " << endl;
	BUS* bus = bus_tb->GetTable();
	count = bus_tb->GetCount();
	outfile << "busid       " << "descr       " << "nd    " << "v       " << "st       " << "basevoltage       " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (bus + i)->id << "   " << (bus + i)->descr << "   " << (bus + i)->nd << "   " << (bus + i)->v << "  " << (bus + i)->fdid << "  " << (bus + i)->basevoltage << endl;
	}

	outfile << "   " << endl;
	outfile << "cb:   " << endl;
	CB* cb = cb_tb->GetTable();
	count = cb_tb->GetCount();
	outfile << "id       " << "descr       " << "nd    " << "znd       " << "q     " << "st     " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (cb + i)->id << "   " << (cb + i)->descr << "   " << (cb + i)->nd << "   " << (cb + i)->znd << "   " << (cb + i)->q << "  " << (cb + i)->fdid << "  " << endl;
	}

	outfile << "   " << endl;
	outfile << "sec:   " << endl;
	SEC* sec = sec_tb->GetTable();
	count = sec_tb->GetCount();
	outfile << "id       " << "descr       " << "nd    " << "znd       " << "r1    " << "x1      " << "r0    " << "x0      " << "basevoltage       " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (sec + i)->id << "   " << (sec + i)->descr << "   " << (sec + i)->nd << "   " << (sec + i)->znd << "   " << (sec + i)->r1 << "   " << (sec + i)->x1 << "   " << (sec + i)->r0 << "   " << (sec + i)->x0 << "   " << (sec + i)->basevoltage << endl;
	}

	// 	outfile<<"   "<< endl;
	// 	outfile<<"secp:   "<< endl;
	//  	secp = secp_tb_t->GetTable();
	//  	count = secp_tb_t->GetCount();
	//	outfile<<"id       "<<"descr       "<<"fdid    "<<"nd    "<<"w    "<<"r       "<<"st       "<<  endl;
	//  	for(i = 0; i <count; i++)
	//  	{
	// 		outfile<<(secp+i)->id<<"   "<<(secp+i)->descr<<"   "<<(secp+i)->fdid<<"   "<<(secp+i)->nd<<"   "<<(secp+i)->w<<"   "<<(secp+i)->r<<"  "<<(secp+i)->st<<endl;
	//  	}

	outfile << "   " << endl;
	outfile << "ld:   " << endl;
	LD* ld = ld_tb->GetTable();
	count = ld_tb->GetCount();
	outfile << "ldid       " << "descr       " << "nd    " << "w    " << "r    " << "st    " << "basevoltage    " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (ld + i)->id << "   " << (ld + i)->descr << "   " << (ld + i)->nd << "   " << (ld + i)->w << "  " << (ld + i)->r << "  " << (ld + i)->fdid << "  " << (ld + i)->basevoltage << endl;
	}
	outfile << "   " << endl;

	outfile << "   " << endl;
	outfile << "st:   " << endl;
	ST* st = st_tb->GetTable();
	count = st_tb->GetCount();
	outfile << "stid       " << "descr       " << "type    " << "subarea       " << "w    " << "r    " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (st + i)->id << "   " << (st + i)->descr << "   " << (st + i)->abb << "   " << (st + i)->fdid << "   " << (st + i)->wso << "  " << (st + i)->rso << endl;
	}
	outfile << "   " << endl;

	outfile << "   " << endl;
	outfile << "so:   " << endl;
	SO* so = so_tb->GetTable();
	count = so_tb->GetCount();
	outfile << "id       " << "descr       " << "vTarget    " << "w    " << "r    " << "nd    " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (so + i)->id << "   " << (so + i)->descr << "   " << (so + i)->vtarget << "   " << (so + i)->w << "   " << (so + i)->r << "   " << (so + i)->nd << endl;
	}
	outfile << "   " << endl;
	outfile << "   " << endl;

	outfile << "vl:   " << endl;
	VL* vl1 = vl_tb->GetTable();
	count = vl_tb->GetCount();
	outfile << "mrID       " << "name       " << "Substation    " << "highkV    " << "lowkV    " << "basevoltage    " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (vl1 + i)->mRID << "   " << (vl1 + i)->name << "   " << (vl1 + i)->Substation << "   " << (vl1 + i)->highkV << "   " << (vl1 + i)->lowkV << "   " << (vl1 + i)->BaseVoltage << endl;
	}
	outfile << "   " << endl;
	outfile << "   " << endl;

	outfile << "nd:   " << endl;

	ND* ppnd = nd_tb->GetTable();
	count = nd_tb->GetCount();
	outfile << "id       " << endl;
	for (i = 0; i < count; i++)
	{
		outfile << (ppnd + i)->id << endl;
	}
	outfile << "   " << endl;

	outfile.close();
	//	printf(lld.OutputAllTime());


	//////////////////////////////////////////////////////////////////////////
	//��ʵ������ɵ���ʱ�ڴ��	

	digital_tb = new DTable<DIGITAL>(lld->mx_cb);//�ڵ����ڴ濪����
	lld->mx_analog = lld->mx_cb * 5;//ƽ��ÿ��������5��ң����
	analog_tb = new DTable<ANALOG>(lld->mx_analog);//�ڵ����ڴ濪����

	lld->mx_island = lld->mx_so;
	if (lld->mx_island > 0)
		island_tb = new DTable<ISLAND>(lld->mx_island);

	lld->mx_bs = lld->mx_nd;
	if (lld->mx_bs > 0)
		bs_tb = new DTable<BS>(lld->mx_bs);

	lld->mx_r_nd_cb = 2 * lld->mx_cb;
	lld->mx_r_nd_sec = 2 * lld->mx_sec;
	lld->mx_r_nd_xf = 2 * lld->mx_xf;

	lld->mx_r_st_bus = 2 * lld->mx_bus;
	lld->mx_r_st_cb = 2 * lld->mx_cb;
	lld->mx_r_st_cp = 2 * lld->mx_cp;
	lld->mx_r_st_ld = 2 * lld->mx_ld;
	lld->mx_r_st_so = 2 * lld->mx_so;
	lld->mx_r_st_xfmr = 2 * lld->mx_xfmr;

	if (lld->mx_r_nd_cb > 0)
		r_nd_cb_tb = new DTable<R_ND_CB>(lld->mx_r_nd_cb);
	if (lld->mx_r_nd_sec > 0)
		r_nd_sec_tb = new DTable<R_ND_SEC>(lld->mx_r_nd_sec);
	if (lld->mx_r_nd_xf > 0)
		r_nd_xf_tb = new DTable<R_ND_XF>(lld->mx_r_nd_xf);
	if (lld->mx_r_xfmr_xf > 0)
		r_xfmr_xf_tb = new DTable<R_XFMR_XF>(lld->mx_r_xfmr_xf);

	if (lld->mx_r_st_bus > 0)
		r_st_bus_tb = new DTable<R_ST_BUS>(lld->mx_r_st_bus);
	if (lld->mx_r_st_cb > 0)
		r_st_cb_tb = new DTable<R_ST_CB>(lld->mx_r_st_cb);
	if (lld->mx_r_st_cp > 0)
		r_st_cp_tb = new DTable<R_ST_CP>(lld->mx_r_st_cp);
	if (lld->mx_r_st_ld > 0)
		r_st_ld_tb = new DTable<R_ST_LD>(lld->mx_r_st_ld);
	if (lld->mx_r_st_so > 0)
		r_st_so_tb = new DTable<R_ST_SO>(lld->mx_r_st_so);
	if (lld->mx_r_st_xfmr > 0)
		r_st_xfmr_tb = new DTable<R_ST_XFMR>(lld->mx_r_st_xfmr);

	if (lld->mx_st > 0)
		r_co_st_tb = new DTable<R_CO_ST>(lld->mx_st);


	return n_cb + n_sec + n_secp + n_bus + n_xf + n_xfmr + n_ld;//�豸Ԫ����������
}

void DSBOB2::inputmem(int type)
{
	if (type == 0)
	{
		//////////////////////////////////////////////////////////////////////////
		//��CIME����������������ݣ�������Ҫ�е�ѹ�ȼ��ĸ��������ֵ��ʱ��Ҫע��
		//ʵ�����CIME���������������
		lld->mx_bus = 1500;
		bus_tb_t = new DTable<BUS>(lld->mx_bus);

		lld->mx_cb = 20000;
		cb_tb_t = new DTable<CB>(lld->mx_cb);

		lld->mx_sec = 800;
		sec_tb_t = new DTable<SEC>(lld->mx_sec);

		lld->mx_secp = 1600;
		secp_tb_t = new DTable<SECP>(lld->mx_secp);

		lld->mx_xfmr = 700;
		xfmr_tb_t = new DTable<XFMR>(lld->mx_xfmr);

		lld->mx_xf = 1500;
		xf_tb_t = new DTable<XF>(lld->mx_xf);

		lld->mx_tapty = 1500;
		tapty_tb_t = new DTable<TAPTY>(lld->mx_tapty);

		lld->mx_ld = 2000;
		ld_tb_t = new DTable<LD>(lld->mx_ld);

		lld->mx_cp = 1000;
		cp_tb_t = new DTable<CP>(lld->mx_cp);

		lld->mx_st = 400;
		st_tb_t = new DTable<ST>(lld->mx_st);

		lld->mx_vl = 1000;
		vl_tb_t = new DTable<VL>(lld->mx_vl);

		lld->mx_so = 100;
		so_tb_t = new DTable<SO>(lld->mx_so);

		lld->mx_nd = 35000;
		nd_tb_t = new DTable<ND>(lld->mx_nd);//�ڵ����ڴ濪����
	}
	else if (type == 1)
	{
		//һ��һ��
		lld->mx_cb = 1000;
		cb_tb_t = new DTable<CB>(lld->mx_cb);

		lld->mx_sec = 1500;
		sec_tb_t = new DTable<SEC>(lld->mx_sec);

		lld->mx_ld = 500;
		ld_tb_t = new DTable<LD>(lld->mx_ld);

		lld->mx_so = 20;
		so_tb_t = new DTable<SO>(lld->mx_so);

		lld->mx_nd = 5000;
		nd_tb_t = new DTable<ND>(lld->mx_nd);//�ڵ����ڴ濪����
	}
}

void DSBOB2::deletemem()
{
	//�Զ�����
	if (lld->mx_analog > 0)
		delete analog_tb;
	if (lld->mx_digital > 0)
		delete digital_tb;

	//ʵ���
	if (lld->mx_st > 0)
		delete st_tb;
	if (lld->mx_so > 0)
		delete so_tb;
	if (lld->mx_bus > 0)
		delete bus_tb;
	if (lld->mx_cb > 0)
		delete cb_tb;
	if (lld->mx_sec > 0)
		delete sec_tb;
	if (lld->mx_ld > 0)
		delete ld_tb;
	if (lld->mx_xfmr > 0)
		delete xfmr_tb;
	if (lld->mx_xf > 0)
		delete xf_tb;
	if (lld->mx_cp > 0)
		delete cp_tb;

//��ʱ��
	if (lld->mx_r_xfmr_xf > 0)
		delete r_xfmr_xf_tb;

	if (lld->mx_bs > 0)
		delete bs_tb;
	if (lld->mx_island > 0)
		delete island_tb;

	if (lld->mx_nd > 0)
		delete nd_tb;
	if (lld->mx_r_nd_ld > 0)
		delete r_nd_ld_tb;
	if (lld->mx_r_nd_cb > 0)
		delete r_nd_cb_tb;
	if (lld->mx_r_nd_sec > 0)
		delete r_nd_sec_tb;
	if (lld->mx_r_nd_cp > 0)
		delete r_nd_cp_tb;
	if (lld->mx_r_nd_xf > 0)
		delete r_nd_xf_tb;

	if (lld->mx_zn > 0)
		delete zn_tb;
	if (lld->mx_r_zn_ld > 0)
		delete r_zn_ld_tb;
	if (lld->mx_r_zn_sec > 0)
		delete r_zn_sec_tb;
	if (lld->mx_r_zn_cp > 0)
		delete r_zn_cp_tb;
	if (lld->mx_r_zn_cb > 0)
		delete r_zn_cb_tb;

	if (lld->mx_fd > 0)
		delete fd_tb;
	if (lld->mx_r_fd_un > 0)
		delete r_fd_un_tb;
	if (lld->mx_r_fd_zn > 0)
		delete r_fd_zn_tb;
	if (lld->mx_r_fd_eq > 0)
		delete r_fd_eq_tb;
	if (lld->mx_r_fd_so > 0)
		delete r_fd_so_tb;
	if (lld->mx_r_fd_ld > 0)
		delete r_fd_ld_tb;

	if (lld->mx_eq > 0)
		delete eq_tb;
	if (lld->mx_r_nd_eq > 0)
		delete r_nd_eq_tb;
	if (lld->mx_un > 0)
		delete un_tb;
}

int DSBOB2::OnGetBsByND(char * nd)
{
	int ret = -1;
	ND* nd1 = new ND();
	strcpy(nd1->id, nd);
	nd_tb->FindPh(*nd1);
	ret = nd1->ibs;
	delete nd1;

	return ret;
}

int DSBOB2::OnGetPhByfdid(char * fdid,int type)
{
	//int ph = -1;
	//FD* fd1 = new FD();
	//strcpy(fd1->id, fdid);
	if (type==1)
	{
		//
		//ph = fd_tb_t->FindPh(*fd1);//so������Ÿ������
		//delete fd1;
		//return fd1->i_id;

 		int count = fd_tb_t->GetCount();
		FD* fd = fd_tb_t->GetTable();
		for(int ii=0;ii<count;ii++)
		{
			if ( strcmp((fd+ii)->id,fdid)==0 )
			{
				return (fd + ii)->i_id;
			}
		}
	}
	else
	{
		//ph = fd_tb->FindPh(*fd1);//so������Ÿ������
		//delete fd1;
		//return fd1->i_id;


  		int count = fd_tb->GetCount();
		FD* fd = fd_tb->GetTable();
		for(int ii=0;ii<count;ii++)
		{
			if ( strcmp((fd+ii)->id,fdid)==0 )
			{
				return (fd + ii)->i_id;
			}
		}
	}

	//delete fd1;
	return -1;
}

//��һ��ÿ�����ض���Ҫ�в�㣬���û�в�㣬����Ϊ�Ǹ������Ǻ�λ
int DSBOB2::ReadDigitalData()
{
	if (lld->mx_cb == 0)
		return 1;

	int			i;
	int			count;
	char  str[500];
	bool ret = true;

	CB* cb = cb_tb->GetTable();
	count = cb_tb->GetCount();
	for(i = 0; i < count; i++)
	{
		if ( ( (cb+i)->q & 1 ) || (cb+i)->q & 4096 )//���翪�ػ����տ���
		{
			(cb+i)->q |= CB_OPEN;
		}
		else if ( (cb+i)->q == 0 )//���ڷ�λ�Ŀ���,�������һ���ǲ����ڵ�
		{
			(cb+i)->q |= CB_OPEN;

			//str.Format("���󣺿���id=%s��descr=%s ״̬����\n",(cb + i)->id,(cb + i)->descr);
			sprintf(str,"���󣺿���id=%s��descr=%s ״̬����\n",(cb + i)->id,(cb + i)->descr);	
			lld->WriteLog(lld->logfile,str);
			//lld->addLogLine(str);
			//printf(str);
		}
		else
		{
			(cb+i)->q &= ~CB_OPEN;
		}
	}

	return ret;
}

//�׶˱����й����޹�����������ܽ��и��ɵķ��䣿

//��ȡң�����ݣ������Ҫ�����ɷ��䣬��ѹ��û��һ���и��ɵ����⣬
//���׶˵ĸ��ɰ���ѹ��������ȫ��������ȥ

//��Դ�����û�����⣬��������Ļ����ñ�ѹ�������������д���
//��������⣬���������Ϊ׼��������������������з���

//�˴������߼����⣬��Ҫ�޸ģ��Ƿ�������Ҫ���ÿһ�����ߣ�������һ����������
int DSBOB2::ReadAnalogData()
{
	int ret;//0-�ɹ���1-��Դң��Խ�磬2-����ң��Խ��
    int i;
	char  sLinelog[500];
	//ANALOG* analog = analog_tb->GetTable();
	double P_value,Q_value,I_value,U_value;
	
	//��Դ�������
	SO* so = so_tb->GetTable();
	for(i = 0; i < so_tb->GetCount(); i++)
	{
		//ֻ���Ƿ��е���ֵ�����ֵ��Ĭ��û�еģ��в�����д
		I_value = (so+i)->I_value;//ʵ��ĳ��ڵ���
		U_value = (so+i)->vtarget;//������ѹ,�п��ܲ���10kV

		if (I_value > 0)//�׶�������ֵ���õ����ȽϺã��о�����������
		{
			(so+i)->isAnalog_P = 1;
			(so+i)->isAnalog_Q = 1;
			P_value = 1.732 * 0.98 * I_value * U_value/1000;
			Q_value = 1.732 * 0.2 * I_value * U_value/1000;
	
			if (P_value*(10/U_value) >= 20)
			{
				//sLinelog.Format("%s�������й�ֵ����20MVA�����顣%f",(so+i)->fdid,P_value);
				sprintf(sLinelog,"%s�������й�ֵ����20MVA�����顣%f",(so+i)->fdid,P_value);
				lld->WriteLog(lld->logfile,sLinelog);

				P_value = 20;
				ret = 1;
			}

			if (fabs(Q_value*(10/U_value)) >= 5)
			{
				sprintf(sLinelog,"%s�������޹�ֵ����5MVar����%f",(so+i)->fdid,Q_value);
				lld->WriteLog(lld->logfile,sLinelog);

				Q_value = 3;
				ret = 1;
			}

			(so+i)->w = P_value;
			(so+i)->r = Q_value;
		}
		else//�׶�û�е���ֵ��˵��û������
		{
			(so+i)->isAnalog_P = 0;
			(so+i)->isAnalog_Q = 0;
		}
	
	}

	return 0;
}

int DSBOB2::WriteOutfile_nm1(char * sFileName)
{
char 	 sSQL[256];
////	int j;
//	CString transSec,lossSec,transSec_t,lossSec_t;
//	CStringList strlist;
	ofstream outfile(sFileName,ios::out);

	outfile<<"<!System= Version=1.0 Code=UTF-8 Data=!>"<< endl;
	outfile<<"<nm1_detailResult::Result name=���簲ȫ����ɨ����ϸ�����>"<< endl;
	outfile<<"@\tfdid\tdevid\tFaultload\tTrload\tNowTrload\tPowerLostRatio\tunionSO\tResult\tLineResult\tcloseunlist\topencblist"<< endl;
	outfile<<"/@\t��·id\t�豸id\t���϶�ʧ�縺��(kW)\t�ǹ��϶���ת�Ƹ���(kW)\t�ǹ��϶�ʵ��ת�Ƹ���(kW)\t�ǹ��϶�ʧ�縺����ռ�ܸ��ɵı���%\tת����Դ\t�ֶμ�����\tȫ�߼�����\t�պ����翪��\t�򿪷ֶο���"<< endl;

//	SEC* sec = sec_tb->GetTable();
	for (int i=0;i<lld->g_cnt_nm1_fd;i++)
	{	
		if (strcmp(nm1_result[i].unionList,"-")==0)
		{
			sprintf(sSQL,"# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.2f\t%s\t%c\t%c\t%s\t%s", \
			          nm1_result[i].fdid,nm1_result[i].secid,1000*(nm1_result[i].Faultload),1000*(nm1_result[i].Trload), \
					  1000*(nm1_result[i].NowTrload),nm1_result[i].PowerLostRatio, nm1_result[i].unionList, \
					  nm1_result[i].secResult,nm1_result[i].LineResult,nm1_result[i].transSec,nm1_result[i].lossSec);
		}
		else
			sprintf(sSQL,"# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.2f\t%s\t%c\t%c\t%s\t%s", \
			          nm1_result[i].fdid,nm1_result[i].secid,1000*(nm1_result[i].Faultload),1000*(nm1_result[i].Trload), \
					  1000*(nm1_result[i].NowTrload),nm1_result[i].PowerLostRatio, nm1_result[i].unionList, \
					  nm1_result[i].secResult,nm1_result[i].LineResult,nm1_result[i].transSec,nm1_result[i].lossSec);

		outfile.write(sSQL,strlen(sSQL));
		outfile<<endl;
	}
	
	outfile<<"</nm1_detailResult::Result>"<< endl;
	outfile<<" "<< endl;
	
	outfile.close();


	return 1;
}

//д�������ļ���������ΪE��ʽ��10kV
int DSBOB2::WriteOutfile_pf(char * sFileName,int iisland)
{
	char 	id[80];
	char  sSQL[256];
	int i, count;
	int ibs,zbs,iout,zout;
	float v_b,adeg_b,v_e,adeg_e;
//******�Ժ�Ҫ������ļ�����������û������Ա���м���
	ofstream outfile(sFileName,ios::out);
	outfile<<"<!System= Version=1.0 Code=UTF-8 Data=!>"<< endl;
	BS* bs;
	//bs_tb->Sequence();
	bs = bs_tb->GetTable();

	// 	//3 - write bus table
	if (lld->mx_bus > 0)
	{
		count = bus_tb->GetCount();
		if (count > 0)
		{
			outfile<<"<busv::pf name="<<"\"ĸ�ߵ�ѹ\""<<">"<< endl;
			outfile<<"@	bus_id\tv\tadeg"<< endl;
			outfile<<"/@\tĸ�߱��\t��ѹ(kV)\t���"<< endl;

			BUS* bus= bus_tb->GetTable();
			//bus_tb_a->Sequence();
			for (i = 0; i < count; i++)
			{
				ibs = (bus+i)->ibs;
				iout = ( bus+i)->topflags & BUS_OPEN;
				if(iout)//ĸ�߲��ڵ���
					continue;			
				if ( ((bus+i)->adeg == 0) && ((bus+i)->v == 1) )
					continue;

				//1000Ϊ������еĽ��������Ϊ���ָ����
				if ((iisland != 1000)
					&& ((iisland > -1)
						&& ((bs + ibs)->iisland != iisland)))
					continue;


				//if ((iisland > -1)
				//	&& ((bs + ibs)->iisland != iisland))
				//	continue;
				//lld->substr((bus+i)->id,4,id);
				//lld->delstrleft((bus+i)->id,6,id);	
				sprintf(sSQL,"# %s\t%f\t%f",(bus+i)->id,10*(bus+i)->v,(bus+i)->adeg);

				outfile.write(sSQL,strlen(sSQL));
				outfile<<endl;
			}
			outfile<<"</busv::pf>"<< endl;
			outfile<<" "<< endl;
		}
	}

 	count = sec_tb->GetCount();
	if (count > 0)
	{
		outfile<<"<secflow::pf name="<<"\"�߶γ���\""<<">"<< endl;
		outfile<<"@	sec_id\tPi\tQi\tPj\tQj\tQci\tQcj\tsBeg\tsEnd\tv_b\tadeg_b\tv_e\tadeg_e"<< endl;
		outfile<<"/@	�߶α��\t�׽ڵ��й�(kW)\t�׽ڵ��޹�(kVar)\tĩ�ڵ��й�(kW)\tĩ�ڵ��޹�(kVar)\t���������й�(kW)\t���������޹�(kVar)\t�׽ڵ�\tĩ�ڵ�\t�׽ڵ��ѹ(kV)\t�׽ڵ����\tĩ�ڵ��ѹ(kV)\tĩ�ڵ����"<< endl;

 		SEC* sec = sec_tb->GetTable();
		//sec_tb_a->Sequence();
 		for (i = 0; i < count; i++)
		{
//if (strcmp((sec+i)->id,"SEG_1002666983")==0)
//{
//	int de=0;
// }
			//lld->delstrleft((sec+i)->id,6,id);	
			ibs = (sec+i)->ibs;
			zbs = (sec+i)->zbs;

			//1000Ϊ������еĽ��������Ϊ���ָ����
			if ((iisland != 1000)
				&& ((iisland > -1)
					&& ((bs + ibs)->iisland != iisland)))
				continue;

			iout = (sec+i)->topflags & SEC_IOPEN;
			zout = (sec+i)->topflags & SEC_ZOPEN;
			//TRACE((sec+i)->id+'\n');
			if(iout && zout)//�߶β��ڵ���
			{
				v_b=adeg_b=v_e=adeg_e=0;
				continue;			
			}
			sprintf(sSQL,"# %s\t%f\t%f\t%f\t%f\t0.0\t0.0\t%s\t%s\t%f\t%f\t%f\t%f",(sec+i)->id,1000*((sec+i)->w), \
				1000*((sec+i)->r),1000*((sec+i)->wz),1000*((sec+i)->rz),(sec+i)->nd,(sec+i)->znd,10*(sec+i)->v_b,(sec+i)->adeg_b,10*(sec+i)->v_e,(sec+i)->adeg_e);
			outfile.write(sSQL,strlen(sSQL));
			outfile<<endl;
		}
		outfile<<"</secflow::pf>"<< endl;
		outfile<<" "<< endl;
	}
	if (lld->mx_xf > 0)
	{
		count = xf_tb->GetCount();
		if (count > 0)
		{
			outfile << "<xfflow::pf name=" << "\"��ѹ�����鳱��\"" << ">" << endl;
			outfile << "@	xf_id\tPi\tQi\tPj\tQj\tQci\tQcj\tsBeg\tsEnd\tv_b\tadeg_b\tv_e\tadeg_e" << endl;
			outfile << "/@	������\t�׽ڵ��й�(kW)\t�׽ڵ��޹�(kVar)\tĩ�ڵ��й�(kW)\tĩ�ڵ��޹�(kVar)\t���������й�(kW)\t���������޹�(kVar)\t�׽ڵ�\tĩ�ڵ�\t�׽ڵ��ѹ(kV)\t�׽ڵ����\tĩ�ڵ��ѹ(kV)\tĩ�ڵ����" << endl;

			XF* xf = xf_tb->GetTable();
			//xf_tb_a->Sequence();
			for (i = 0; i < count; i++)
			{
				//if (strcmp((xf+i)->id,"SEG_1002666983")==0)
				//{
				//	int de=0;
				// }
							//lld->delstrleft((xf+i)->id,6,id);	
				ibs = (xf + i)->ibs;
				zbs = (xf + i)->zbs;
				//1000Ϊ������еĽ��������Ϊ���ָ����
				if ((iisland != 1000)
					&& ((iisland > -1)
						&& ((bs + ibs)->iisland != iisland)))
					continue;

				iout = (xf + i)->topflags & XF_IOPEN;
				zout = (xf + i)->topflags & XF_ZOPEN;
				//TRACE((xf+i)->id+'\n');
				if (iout && zout)//�߶β��ڵ���
				{
					v_b = adeg_b = v_e = adeg_e = 0;
					continue;
				}
				sprintf(sSQL, "# %s\t%f\t%f\t%f\t%f\t0.0\t0.0\t%s\t%s\t%f\t%f\t%f\t%f", (xf + i)->id, 1000 * ((xf + i)->w), \
					1000 * ((xf + i)->r_), 1000 * ((xf + i)->wz), 1000 * ((xf + i)->rz), (xf + i)->nd, (xf + i)->znd, 10 * (xf + i)->v_b, (xf + i)->adeg_b, 10 * (xf + i)->v_e, (xf + i)->adeg_e);
				outfile.write(sSQL, strlen(sSQL));
				outfile << endl;
			}
			outfile << "</xfflow::pf>" << endl;
			outfile << " " << endl;
		}
	}

	//�ǲ��������������
	if (iisland != -1)
	{
		count = so_tb->GetCount();
		if (count > 0)
		{
			outfile << "<srcflow::pf name=" << "\"��Դ����\"" << ">" << endl;
			outfile << "@	node_id\tPg\tQg" << endl;
			outfile << "/@	�ڵ���\tע���й�(kW)\tע���޹�(kVar)" << endl;

			SO* so = so_tb->GetTable();
			// 		so_tb->Sequence();
			for (i = 0; i < count; i++)
			{
				ibs = (so + i)->ibs;
		
				//1000Ϊ������еĽ��������Ϊ���ָ����
				if( (iisland!=1000)
				&& ((iisland > -1)
					&& ((bs + ibs)->iisland != iisland)))
					continue;

				if (fabs((so + i)->w) < 0.000000001)
				{
					sprintf(sSQL, "# %s\t%f\t%f", (so + i)->id, 1000 * ((so + i)->w), 1000 * ((so + i)->r));
				}
				else
					sprintf(sSQL, "# %s\t%f\t%f", (so + i)->id, 1000 * ((so + i)->w), 1000 * ((so + i)->r));

				outfile.write(sSQL, strlen(sSQL));
				outfile << endl;
			}
			outfile << "</srcflow::pf>" << endl;
			outfile << " " << endl;
		}
	}

 	count = ld_tb->GetCount();
	if (count > 0)
	{
		outfile<<"<loadflow::pf name="<<"\"���ɳ���\""<<">"<< endl;
		outfile<<"@	load_id\tPload\tQload"<< endl;
		outfile<<"/@	���ɱ��\t�����й�(kW)\t�����޹�(kVar)"<< endl;

		LD* ld = ld_tb->GetTable();
 		//ld_tb_a->Sequence();
		for (i = 0; i < count; i++)
		{
			ibs = (ld + i)->ibs;
			//1000Ϊ������еĽ��������Ϊ���ָ����
			if ((iisland != 1000)
				&& ((iisland > -1)
					&& ((bs + ibs)->iisland != iisland)))
				continue;

			iout = (ld+i)->topflags & LD_OPEN;
			if(iout)//���ɲ��ڵ���
				continue;

			//if (fabs((ld+i)->w) < 0.000000001)
			//	continue;

//			ibs = (ld+i)->ibs;
//	        iout = ( ld+i)->topflags & LD_OPEN || (bs+ibs)->iisland < 0;
//	        if(iout)//ĸ�߲��ڵ���
//				continue;			

//			lld->substr((ld+i)->id,9,id);
			//lld->delstrleft((ld+i)->id,6,id);	
			//if ((ld + i)->w <= 0)
			//{
			//	int ddd = 0;
			//}
			//sprintf(sSQL,"# %s\t%f\t%f",(ld+i)->id,1000*((ld+i)->wm),1000*((ld+i)->rm));
			sprintf(sSQL,"# %s\t%f\t%f",(ld+i)->id,1000*((ld+i)->w),1000*((ld+i)->r));
//printf("\n  %s\n",sSQL);

			outfile.write(sSQL,strlen(sSQL));
			outfile<<endl;
		}
		outfile<<"</loadflow::pf>"<< endl;
		outfile<<" "<< endl;
	}
//printf("\n  555 \n");
	
	if (lld->mx_cp>0)
	{
		count = cp_tb->GetCount();
		if (count > 0)
		{
			outfile<<"<cpflow::pf name="<<"\"�޹���������\""<<">"<< endl;
			outfile<<"@	cp_id\tQv"<< endl;
			outfile<<"/@	�޹���������\t�����޹�(kVar)"<< endl;

			CP* cp = cp_tb->GetTable();
 			//cp_tb_a->Sequence();
			for (i = 0; i < count; i++)
			{
				ibs = (cp + i)->ibs;
				if ((iisland > -1)
					&& ((bs + ibs)->iisland != iisland))
					continue;

				if (fabs((cp+i)->r) < 0.000000001)
					continue;

				sprintf(sSQL,"# %s\t%f",(cp+i)->id,1000*((cp+i)->r));
	//printf("\n  %s\n",sSQL);

				outfile.write(sSQL,strlen(sSQL));
				outfile<<endl;
			}
			outfile<<"</cpflow::pf>"<< endl;
			outfile<<" "<< endl;
		}
	}
	
	//�����������
	//��ÿһ��������ѭ����������һ������Ӧһ�����ߣ��ӵ���������ҵ���Դ�㣬�ӵ�Դ������ҵ�����id
	if (iisland != 0)
	{

		//	CString idh;
		float ploss, qloss, rateloss/*������*/;
		//	 name[80];
		count = so_tb->GetCount();
		if (count > 0)
		{
			outfile << "<fdindex::ll name=" << "\"����\"" << ">" << endl;
			outfile << "@	fdid\tploss\tqloss\trateloss" << endl;
			outfile << "/@\t����id\t�й����(kW)\t�޹����(kVar)\t����(%)" << endl;

			SO* so = so_tb->GetTable();
			//���������һ�����ߣ�ֻ��Ҫ���е�һ������
			for (i = 0; i < count; i++)
			{
				strcpy(id, (so + i)->fdid);

				if ((so + i)->w == 0)
				{
					ploss = 0;
					qloss = 0;
					rateloss = 0;
				}
				else
				{
					ploss = 1000 * ((so + i)->pLoss);
					qloss = 1000 * ((so + i)->qLoss);
					rateloss = 100 * ((so + i)->pLoss / (so + i)->w);
				}
				sprintf(sSQL, "# %s\t%f\t%f\t%f", id, ploss, qloss, rateloss);
				outfile.write(sSQL, strlen(sSQL));
				outfile << endl;
			}
			outfile << "</fdindex::ll>" << endl;
			outfile << " " << endl;
		}

		//ϵͳ����
		if (lld->mx_co > 0)
		{
			//count = co_tb->GetCount();
			//if (count > 0)
			{
				outfile << "<sysindex::ll name=" << "\"����\"" << ">" << endl;
				outfile << "@\tconame\tploss\tqloss\trateloss" << endl;
				outfile << "/@\t��˾����\t�й����(kW)\t�޹����(kVar)\t����(%)" << endl;

				CO* co;
				co = co_tb->GetTable();
				for (i = 0; i < 1; i++)
				{
					//ͨ����Դ��Ľڵ�ŵõ����ߵ�id�ź�descr
					//idh = (co+i)->id;
					ploss = 1000 * ((co + i)->wloss);
					qloss = 1000 * ((co + i)->rloss);
					rateloss = 100 * ((co + i)->wloss / (co + i)->wso);
					sprintf(sSQL, "# %s\t%f\t%f\t%f", (co + i)->id, ploss, qloss, rateloss);
					outfile.write(sSQL, strlen(sSQL));
					outfile << endl;
				}
				outfile << "</sysindex::ll>" << endl;
			}
		}
	}
    outfile.close();

	return 1;
}

//д�������ļ����޹��Ż����򣬳�����ΪE��ʽ��10kV
int DSBOB2::WriteOutfile_ro(char * sFileName)
{
//******�Ժ�Ҫ������ļ�����������û������Ա���м���
	char  logLine[128];
	int n=lld->g_cnt_ro_fd;
	if (n == 0)
	{
		sprintf(logLine,"�Ż������������ڣ�����������޹��Ż���");

		ofstream outfile(sFileName,ios::out);
		outfile<<logLine<< endl;
		outfile.close();		

		lld->WriteLog(lld->logfile,logLine);
		return 0;
	}
	
	ofstream outfile(sFileName,ios::out);
	outfile<<"<!System= Version=1.0 Code=UTF-8 Data=!>"<< endl;

   	outfile<<"<roQ::ro name="<<"\"�޹��Ż�����\""<<">"<< endl;
	outfile<<"@	 capid fdid Qv b_cosfi a_cosfi b_v a_v"<< endl;
	outfile<<"/@ ������id ����id  ��������(kVar)	����ǰ�������� ������������ ����ǰ��ѹ(kA) �������ѹ(kA)"<< endl;
	for (int j=0;j<n;j++)
	{
		outfile << "#\t" <<  ro_result[j].ndid << "   " 
			<< ro_result[j].fdid <<"  " << 1000*(ro_result[j].Qv) << "   " 
			<< ro_result[j].b_cosfi << "  " <<ro_result[j].a_cosfi <<"  " << ro_result[j].b_v << "   "
			<< ro_result[j].a_v <<  endl;
	}
	outfile<<"</roQ::ro>"<< endl;
	outfile<<" "<< endl;	
	
    outfile.close();

	return 1;
}

//int DSBOB2::WriteOutfile_optsect(* sFileName,optSect* pOptSect)
int DSBOB2::WriteOutfile_optsect(char * sFileName,int sect_id)
{
	char 	id[80];
	char  sSQL[256];
	int i, count;
	int ibs, zbs, iout, zout;
	float v_b, adeg_b, v_e, adeg_e;
	//******�Ժ�Ҫ������ļ�����������û������Ա���м���
	ofstream outfile(sFileName, ios::out);
	outfile << "<!System= Version=1.0 Code=UTF-8 Data=!>" << endl;

	count = sec_tb->GetCount();
	if (count > 0)
	{
		outfile << "<optsect::pf name=" << "\"�߶γ���\"" << ">" << endl;
		outfile << "@	sec_id\tPi\tQi\tPj\tQj\tQci\tQcj\tsBeg\tsEnd\tv_b\tadeg_b\tv_e\tadeg_e" << endl;
		outfile << "/@	�߶α��\t�׽ڵ��й�(kW)\t�׽ڵ��޹�(kVar)\tĩ�ڵ��й�(kW)\tĩ�ڵ��޹�(kVar)\t���������й�(kW)\t���������޹�(kVar)\t�׽ڵ�\tĩ�ڵ�\t�׽ڵ��ѹ(kV)\t�׽ڵ����\tĩ�ڵ��ѹ(kV)\tĩ�ڵ����" << endl;

		SEC* sec = sec_tb->GetTable();
		//sec_tb_a->Sequence();
		for (i = 0; i < count; i++)
		{
			//if (strcmp((sec+i)->id,"SEG_1002666983")==0)
			//{
			//	int de=0;
			// }
						//lld->delstrleft((sec+i)->id,6,id);	
			ibs = (sec + i)->ibs;
			zbs = (sec + i)->zbs;

			iout = (sec + i)->topflags & SEC_IOPEN;
			zout = (sec + i)->topflags & SEC_ZOPEN;
			//TRACE((sec+i)->id+'\n');
			if (iout && zout)//�߶β��ڵ���
			{
				v_b = adeg_b = v_e = adeg_e = 0;
				continue;
			}
			sprintf(sSQL, "# %s\t%f\t%f\t%f\t%f\t0.0\t0.0\t%s\t%s\t%f\t%f\t%f\t%f", (sec + i)->id, 1000 * ((sec + i)->w), \
				1000 * ((sec + i)->r), 1000 * ((sec + i)->wz), 1000 * ((sec + i)->rz), (sec + i)->nd, (sec + i)->znd, 10 * (sec + i)->v_b, (sec + i)->adeg_b, 10 * (sec + i)->v_e, (sec + i)->adeg_e);
			outfile.write(sSQL, strlen(sSQL));
			outfile << endl;
		}
		outfile << "</optsect::pf>" << endl;
		outfile << " " << endl;
	}

char 	 ndid[64];
	count = nd_tb->GetCount();
	if (count > 0)
	{
		outfile << "<optsect::nd name=" << "\"�ڵ㸺��\"" << ">" << endl;
		outfile << "@	node_id\tPg\tQg" << endl;
		outfile << "/@	�ڵ���\t�й�(kW)\t����\t�б�\t��֧����\t���翪��" << endl;

		ND* nd = nd_tb->GetTable();
		for (i = 0; i < count; i++)
		{
			strcpy(ndid, (nd + i)->id);
			if ((strlen(ndid) == 0) || (strcmp(ndid, "-") == 0))
				continue;

			if (nd->q & ND_MAIN)
			{
				if (strlen((nd + i)->ldList) == 0)
					strcpy((nd + i)->ldList, "-");
				if (strlen((nd + i)->UpCBID) == 0)
					strcpy((nd + i)->UpCBID, "-");
				if (strlen((nd + i)->UnionCBID) == 0)
					strcpy((nd + i)->UnionCBID, "-");

				sprintf(sSQL, "# %s\t%f\t%d\t%s\t%s\t%s",
					ndid, (nd + i)->w, (nd + i)->primebs,
					(nd + i)->ldList, (nd + i)->UpCBID, (nd + i)->UnionCBID);

				outfile.write(sSQL, strlen(sSQL));
				outfile << endl;
			}
		}
		outfile << "</optsect::nd>" << endl;
		outfile << " " << endl;
	}

	//�ϵ���Ϣ��ÿ���ֶε���ϸ��Ϣ
	outfile << "<optsect::optSect name=" << "\"�ֶ���Ϣ\"" << ">" << endl;
	outfile << "@	node_id\tPg\tQg" << endl;
	outfile << "/@	�ֶα��\t���ɸ���\t��Ҫ�û�����\t�����б�" 
		<<"\tװ������\tʵ������\t�ܳ���\t���ɸ���Ȩ��\t���ɴ�СȨ��"
		<<"\t����Ȩ��\t���翪�ط�����\t�ֶθ�������������\t��Ҫ�û�������\t�ϵ��б�\t����ȫ�����߶�ID�б�"		
		<< endl;
	//count = pOptSect->sect_id;
	// temp[2000];
	char  ldList[2000];
	char  secList[2000];
	char  cut_[256];
	char  qota[2];
	strcpy(qota, ",");
	for (i = 0; i < sect_id; i++)
	{
		strcpy(cut_ ,fdsect[i].cut.c_str());
		lld->OnVector2String(fdsect[i].idList,qota, ldList);
		if (strlen(ldList) == 0)
			strcpy(ldList, "-");
		lld->OnVector2String(fdsect[i].secList, qota, secList);
		if (strlen(secList) == 0)
			strcpy(secList, "-");

		if (fdsect[i].sumRatedS < 0)
			fdsect[i].sumRatedS = -fdsect[i].sumRatedS;
		if (fdsect[i].sumW < 0)
			fdsect[i].sumW = -fdsect[i].sumW;		

		sprintf(sSQL, "# %d\t%d\t%d\t%s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%s\t%s",
			fdsect[i].sect_id, fdsect[i].sumLD,
			fdsect[i].sumVip, ldList,
			fdsect[i].sumRatedS, 1000*fdsect[i].sumW,
			fdsect[i].sumLen, fdsect[i].wLD,
			fdsect[i].wW, fdsect[i].wLen,
			fdsect[i].wUN, fdsect[i].wSecW,
			fdsect[i].wVIP, cut_,secList);
		outfile.write(sSQL, strlen(sSQL));
		outfile << endl;
	}
	outfile << "</optsect::optSect>" << endl;	

	outfile.close();

	return 1;
}
