// RA.cpp: implementation of the CRA class.
//
//////////////////////////////////////////////////////////////////////

#include "../DNASPLIB/RA.h"
#include "../DNASPLIB/LLD.h"

#include "math.h"
#if _WIN32
#include <process.h>  //windows
#elif __linux__
#include <pthread.h>  //linux
#endif
#include <fstream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//    int count,i;
//ofstream outfile("ra.txt", ios::out);

CRA::CRA()
{

}

CRA::CRA(DSBOB2* m_pDb, CLLD* m_pLLD) : CNM1(m_pDb, m_pLLD)
{
	dsbob2 = m_pDb;
	lld = m_pLLD;
	lld->g_ld_ra = 0;
//	if (lld->mx_cb > 0)
//		cb_tb		= dsbob2->cb_tb;
//
//	so_tb		= dsbob2->so_tb;
//	ld_tb		= dsbob2->ld_tb;
//	sec_tb		= dsbob2->sec_tb;
//	nd_tb		= dsbob2->nd_tb;
//	r_nd_cb_tb	= dsbob2->r_nd_cb_tb;
//	r_nd_xf_tb	= dsbob2->r_nd_xf_tb;
//	r_nd_sec_tb = dsbob2->r_nd_sec_tb;
//	r_nd_ld_tb  = dsbob2->r_nd_ld_tb;
}

CRA::~CRA()
{

}

//Ϊ�����߳�������ɿ��ԣ�ֻ�ܽ�����ŵ�����
_LD_RESULT* ld_result;

//�����߽�������
int cmp_RA( const void *a ,const void *b) 
{ 
	struct _LD_RESULT *c = (_LD_RESULT*)a;
	struct _LD_RESULT *d = (_LD_RESULT*)b;

	if (strcmp(c->fdid , d->fdid)!=0)
		return strcmp(c->fdid,d->fdid);
	else
		return strcmp(c->id,d->id);

	//return strcmp((*(_LD_RESULT *)a).fdid , (*(_LD_RESULT *)b).fdid); 
} 

int sort_RA(_LD_RESULT* ld_result,int cnt)
{
	qsort(ld_result,cnt,sizeof(ld_result[0]),cmp_RA); 

	return 1;
}

int CRA::OnMath(char* Efile,int type)
{
	int i,l;
	char sSQL[500]; 
	char id[MAX_SEC_ID_LEN];//����id
	char fdid[MAX_SEC_ID_LEN],t_fdid[MAX_SEC_ID_LEN];//��������
	
//	double perTimeFault_ld,perTimeSum_ld;
//	double perFault_line,perSum_line,perFaultTime_line,perSumTime_line,perTimeFault_sys,perTimeSum_sys;
//	double perTimeFault_line,perTimeSum_line,perFaultTime_sys,perSumTime_sys,perFault_sys,perSum_sys;
	double ENS_Fault_line,ENS_Fault_sys,ENS_Sum_sys,ENS_Sum_line;

	double perFault_ld=0;	//���ɵ���ƽ�������ʣ���/��
	double perFault_line=0;	//������ƽ�������ʣ���/��
	double perFault_sys=0;	//ϵͳ��ƽ�������ʣ���/��
	double perSum_ld=0;		//��Ԥ���ŵ� ���ɵ�������ۼ�
	double perSum_line=0;	//��Ԥ���ŵ� ���߹������ۼ�
	double perSum_sys=0;	//��Ԥ���ŵ� ϵͳ�������ۼ�

	//u ʱ���������ۼ�
	double perFaultTime_ld=0;	//���ɵ������ƽ��ͣ��ʱ�䣬Сʱ/�꣬ͣ��ʱ��
	double perFaultTime_line=0;//��ƽ��ͣ��ʱ�䣬Сʱ/��
	double perFaultTime_sys=0;//��ƽ��ͣ��ʱ�䣬Сʱ/��
	double perSumTime_ld=0;//
	double perSumTime_line=0;//
	double perSumTime_sys=0;//

	//r��u/lambda �������ۼ�
	double perTimeFault_ld=0;//��ƽ��ͣ�˳���ʱ�䣬Сʱ/��
	double perTimeFault_line=0;//��ƽ��ͣ�˳���ʱ�䣬Сʱ/��
	double perTimeFault_sys=0;//��ƽ��ͣ�˳���ʱ�䣬Сʱ/��
	double perTimeSum_ld=0;//
	double perTimeSum_line=0;//
	double perTimeSum_sys=0;//	
	
	int lv_ld = dsbob2->ld_tb->GetCount();
	LD* ld = dsbob2->ld_tb->GetTable();
	ld_result = new _LD_RESULT[lv_ld+100];
	for (i=0; i<lv_ld; i++)
	{
		strcpy((ld_result+i)->fdid,(ld+i)->fdid);
		strcpy((ld_result+i)->id,(ld+i)->id);
		(ld_result+i)->perFault=0;
		(ld_result+i)->perPlan=0;
		(ld_result+i)->perFaultTime=0;
		(ld_result+i)->perPlanTime=0;
		(ld_result+i)->perSum=0;
		(ld_result+i)->perSumTime=0;
		(ld_result+i)->s=sqrt( (ld+i)->w*(ld+i)->w +(ld+i)->r*(ld+i)->r );
	}

	//��С·�������
	//doCalc();  

	//����FEMA�������
	doFEMA(type);

	ofstream outfile_ra(Efile, ios::out);

	outfile_ra << "<!System= Version=1.0 Code=UTF-8 Data=!>" << endl;
	outfile_ra << "<ra_detailResult::Result name=�ɿ��Լ�����ϸ�����>" << endl;
	outfile_ra << "@\tid\ttype\tASAI_Fault\tSAIFI_Fault\tSAIDI_Fault\tENS_Fault\tASAI_Sum\tSAIFI_Sum\tSAIDI_Sum\tENS_Sum" << endl;
	outfile_ra << "/@\tid\t�������\t�ɿ���(����%)\tͣ��Ƶ��(����,��/��)\tͣ��ʱ��(����,Сʱ/��)\tȱ������(����,kWh/��)\t�ɿ���(��Ԥ����%)\tͣ��Ƶ��(��Ԥ����,��/��)\tͣ��ʱ��(��Ԥ����,Сʱ/��)\tȱ������(��Ԥ����,kWh/��)" << endl;

	//�����пɿ���ͳ�ƺ��ļ��γ�
	//�ȶ������������
	sort_RA(ld_result, lv_ld);

	int fdcnt = 0;
	l = perFault_line = perSum_line = perFaultTime_line = perSumTime_line = ENS_Sum_line = ENS_Fault_line = ENS_Fault_sys = ENS_Sum_sys = 0;
	t_fdid[0] = 0;
	strcpy(t_fdid, (ld_result + 0)->fdid);//�ʼ���Ǹ��Ѿ���������
	if (strlen(t_fdid)>0)
		fdcnt++;
	ENS_Fault_sys = ENS_Sum_sys = 0;//�ܵ���ʧ��������

	//ֻ��Ҫ������ص�����

	for (i = 0; i < lv_ld; i++)
	{
		//if (strcmp("11000000_373240", (ld_result + i)->id) == 0)
		//{
		//	int lld = 0;
		//}
		strcpy(fdid, (ld_result + i)->fdid);
		//�����û��Ŀɿ���ָ��
		strcpy(id, (ld_result + i)->id);
		perFault_ld = (ld_result + i)->perFault;
		perFaultTime_ld = (ld_result + i)->perFaultTime;
		perSum_ld = (ld_result + i)->perSum;
		perSumTime_ld = (ld_result + i)->perSumTime;

		if (perFault_ld==0)
			perTimeFault_ld = 0;
		else
			perTimeFault_ld = perFaultTime_ld / perFault_ld;

		if (perSum_ld == 0)
			perTimeSum_ld = 0;
		else
			perTimeSum_ld = perSumTime_ld / perSum_ld;

		//��Ҫ��д������ߵĿɿ���ָ�꣬����Ҫ�ѵ����û��Ŀɿ���ָ���¼����
		//ÿ�����ɵ�Ŀɿ���ָ�����
		dsbob2->ra_result[l].type = 2;
		strcpy(dsbob2->ra_result[l].fdid, (ld_result + i)->id); //���ɵ�id 

		dsbob2->ra_result[l].SAIFI_Fault = perFault_ld;//ƽ��ͣ����
		dsbob2->ra_result[l].SAIDI_Fault = perTimeFault_ld * perFault_ld;//��ƽ��ͣ��ʱ��
		dsbob2->ra_result[l].ASAI_Fault = 100 * (1 - (perFaultTime_ld / 8760));//������
		dsbob2->ra_result[l].ENS_Fault = (1000 * (ld_result + i)->s) * perFaultTime_ld;//ȱ������

		ENS_Fault_line += (1000 * (ld_result + i)->s) * perFaultTime_ld;

		dsbob2->ra_result[l].SAIFI_Sum = perSum_ld;//ƽ��ͣ����
		dsbob2->ra_result[l].SAIDI_Sum = perTimeSum_ld * perSum_ld;//ƽ��ͣ�˳���ʱ��
		dsbob2->ra_result[l].ASAI_Sum = 100 * (1 - (perSumTime_ld / 8760));//������
		//if ()
		dsbob2->ra_result[l].ENS_Sum = (1000 * (ld_result + i)->s) * perSumTime_ld;//ȱ������

		ENS_Sum_line += (1000 * (ld_result + i)->s) * perSumTime_ld;

		//ʱ���� �� ������ �����ۼ�
		perFault_line += perFault_ld;
		perSum_line += perSum_ld;

		perFaultTime_line += perFaultTime_ld;
		perSumTime_line += perSumTime_ld;

		if (perFaultTime_ld > 0)
		{
			//д����ָ��
			sprintf(sSQL, "# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f", \
				dsbob2->ra_result[l].fdid, "����", dsbob2->ra_result[l].ASAI_Fault, dsbob2->ra_result[l].SAIFI_Fault, \
				dsbob2->ra_result[l].SAIDI_Fault, dsbob2->ra_result[l].ENS_Fault, dsbob2->ra_result[l].ASAI_Sum, \
				dsbob2->ra_result[l].SAIFI_Sum, dsbob2->ra_result[l].SAIDI_Sum, dsbob2->ra_result[l].ENS_Sum);

			outfile_ra.write(sSQL, strlen(sSQL));
			outfile_ra << endl;
		}
		l++;

		//�ж���һ���ǲ��ǻ�����������
		if (
			(i < lv_ld - 1)
			&& (strcmp(t_fdid, (ld_result + i + 1)->fdid) != 0)
			)//�µ�����
		//if (strcmp(t_fdid, (ld_result + i + 1)->fdid) != 0)
		{
			//ֱ��д�ɿ���ָ��	
			//	outfile<<"/@  id �������  ������(����%) ͣ��Ƶ��(����,��/��) ͣ��ʱ��(����,Сʱ/��)	ȱ������(����,kWh/��) ������(��Ԥ����%) ͣ��Ƶ��(��Ԥ����,��/��) ͣ��ʱ��(��Ԥ����,Сʱ/��)	ȱ������(��Ԥ����,kWh/��)"<< endl;
						//���ߵĿɿ���ָ�����

			if (perFault_line > 0)
				perTimeFault_line = perFaultTime_line / perFault_line;
			else
				perTimeFault_line = 0;
			if (perSum_line > 0)
				perTimeSum_line = perSumTime_line / perSum_line;
			else
				perTimeSum_line = 0;

			perFaultTime_sys += perFaultTime_line;
			perSumTime_sys += perSumTime_line;
			perFault_sys += perFault_line;
			perSum_sys += perSum_line;

			ENS_Fault_sys += ENS_Fault_line;
			ENS_Sum_sys += ENS_Sum_line;

			//��Ҫ��д������ߵĿɿ���ָ��,�����Ǹ����ɲ�����д��
			if (perFaultTime_line > 0)
			{
				sprintf(sSQL, "# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f", \
					fdid, "����", 100 * (1 - (perFaultTime_line / l / 8760)), \
					(perFault_line / l), (perFault_line / l)*perTimeFault_line, ENS_Fault_line, \
					100 * (1 - (perSumTime_line / l / 8760)), (perSum_line / l), (perSum_line / l)*perTimeSum_line, ENS_Sum_line);
				outfile_ra.write(sSQL, strlen(sSQL));
				outfile_ra << endl;
			}
			l = perFault_line = perSum_line = perFaultTime_line = perSumTime_line = ENS_Sum_line = ENS_Fault_line = 0;

			//if (i < lv_ld - 1)
			//����һ������
			strcpy(t_fdid, (ld_result + i + 1)->fdid);
			fdcnt++;
		}
		//�����Ǹ�����,������һ�����ϵ�����
		if ((i == lv_ld - 1) && (strlen(t_fdid)>0))
		{
			strcpy(fdid, t_fdid);
			//��д������������
			if (perFault_line > 0)
				perTimeFault_line = perFaultTime_line / perFault_line;
			else
				perTimeFault_line = 0;
			if (perSum_line > 0)
				perTimeSum_line = perSumTime_line / perSum_line;
			else
				perTimeSum_line = 0;

			perFaultTime_sys += perFaultTime_line;
			perSumTime_sys += perSumTime_line;
			perFault_sys += perFault_line;
			perSum_sys += perSum_line;

			ENS_Fault_sys += ENS_Fault_line;
			ENS_Sum_sys += ENS_Sum_line;

			//��Ҫ��д������ߵĿɿ���ָ��,�����Ǹ����ɲ�����д��
			sprintf(sSQL, "# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f", \
				fdid, "����", 100 * (1 - (perFaultTime_line / l / 8760)), \
				(perFault_line / l), (perFault_line / l)*perTimeFault_line, ENS_Fault_line, \
				100 * (1 - (perSumTime_line / l / 8760)), (perSum_line / l), (perSum_line / l)*perTimeSum_line, ENS_Sum_line);
			outfile_ra.write(sSQL, strlen(sSQL));
			outfile_ra << endl;
		}		
	}

	//���дϵͳ��ָ��
		//ȫ��ϵͳ�Ŀɿ���ָ��
	if (lv_ld > 0)
	{
		double sys_pq = dsbob2->co_tb->GetDataByLog(0)->wload * dsbob2->co_tb->GetDataByLog(0)->wload;
		sys_pq += dsbob2->co_tb->GetDataByLog(0)->rload * dsbob2->co_tb->GetDataByLog(0)->rload;
		sys_pq = sqrt(sys_pq);

		//if (sys_pq == 0)

		if (perFault_sys == 0)
			perTimeFault_sys = 0;
		else
			perTimeFault_sys = perFaultTime_sys / perFault_sys;

		if (perSum_sys == 0)
			perTimeSum_sys = 0;
		else
			perTimeSum_sys = perSumTime_sys / perSum_sys;

		sprintf(sSQL, "# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f", \
			"-", "ϵͳ", 100 * (1 - (perFaultTime_sys / lv_ld / 8760)), \
			(perFault_sys / lv_ld), (perFault_sys / lv_ld)*perTimeFault_sys, ENS_Fault_sys, \
			100 * (1 - (perSumTime_sys / lv_ld / 8760)), (perSum_sys / lv_ld), (perSum_sys / lv_ld)*perTimeSum_sys, ENS_Sum_sys);
	}
	else
	{
		sprintf(sSQL, "# -\tsys\t0\t0\t0\t0\t0\t0\t0\t0");
	}

	outfile_ra.write(sSQL, strlen(sSQL));
	outfile_ra << endl;

	outfile_ra << "</ra_detailResult::Result>" << endl;
	outfile_ra << " " << endl;
	outfile_ra.close();

	delete[] ld_result;

    return 1;  
}

//��Ҫ��ÿ���豸��Ҫ����ɨ��
int CRA::doFEMA(int type)
{
	//������Ϊ��λ
	//��������
//	outfile << "eqid    ������  �ƻ��� ����ʱ�� �ƻ�ʱ�� " << "    " << endl;
	int lv_fd = dsbob2->fd_tb->GetCount();
	FD* fd = dsbob2->fd_tb->GetTable();
	for(int i = 0; i < lv_fd; i++)
	{
		//���ߵ�ÿһ���豸N-1
		if (type)
			OnDevNM1(i);
		WriteFEMA(i);	
	}
//	outfile.close();

	return 1;
}

char * CRA::OnGetName(char * id)
{
	char  ret[20];
	for (int i = 0;i < 31;i++)
	{
		if (strcmp(id, devtest[i].id) == 0)
			return devtest[i].name;
	}
	//strcpy(ret, "�����ǹ���");
	return id;
}

int CRA::WriteFEMA(int fdph)  
{
	//���������д����
	double perPlanTime;//�ƻ�ʱ��������ƻ�ƽ��ͣ��ʱ�䣬Сʱ/��
	double perFaultTime;//����ʱ����
	double prob_fault;//ֻ��Ҫ�ѹ����ʵ����ó�����������ÿ�����ϵ��豸����һ����
	double repair_fault;//�����޸�ʱ��
	//����6�������������������
	float fault_ltime;	//���϶�λʱ��
	float fault_stime;	//�����л�ʱ��
	float fault_ttime;	//�����޸�ʱ��

	float plan_ltime;	//�����л�ʱ��
	float plan_stime;	//�����л�ʱ��
	float plan_ttime;	//�����޸�ʱ��

	double prob_plan;	//�����ʣ���/�ٹ�����
	float  repair_plan;	//���޳���ʱ�䣬Сʱ

//////////////////////////////////////////////////////////////////////////
//FEMA
//	int UpSwitchload[max_switch_ld];//���϶����θ����б����л����������л�ʱ������⣩
//	int UpNonOutload[max_switch_ld];//���϶����θ����б����л����������л�ʱ������⣩
//	int Faultload[max_switch_ld];	//���϶�ʧ�縺���б�
//	int NeedTrload[max_switch_ld];//���϶�������Ҫת���ĸ����б�
//	int NowTrload[max_switch_ld];//���϶����ο���ת���ĸ����б�
//	
//	int iUpStreamloadSwitchType;//���θ����л����ͣ�0�����У�1��Ҫ�ȸ������
//////////////////////////////////////////////////////////////////////////

	int i,j,eqph=-1,ldph;
	 fdid[MAX_ID_DLL];
	fdid[0]=0;
	ChildPtr1	*eq_p, *eq_p0;

//	ChildPtr1	*so_p,*so_p0;
//	int r_so = dsbob2->fd_tb->GetDataByPh(fdph)->r_so;
//	if (r_so != -1)//�����������ο���
//	{
//		so_p = dsbob2->r_fd_so_tb->FindChildPtr1(r_so);
//		so_p0 = so_p; 
//		while(so_p)
//		{
//			if (dsbob2->so_tb->GetDataByPh(so_p->Ptr)->slackavr == 0)
//			{
//				//soph= dsbob2->so_tb->GetDataByPh(so_p->Ptr)->ind;
//
//				fault_stime = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->fault_stime;	//�����л�ʱ��,Сʱ
//				fault_ttime = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->fault_ttime;	//����ת��ʱ��
//
//				plan_stime = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_stime;	//�����л�ʱ��
//				plan_ttime = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_ttime;	//����ת��ʱ��
//			//		plan_prob = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_prob;	//�����ʣ���/�ٹ�����
//				repair_plan = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_rtime;	//���޳���ʱ�䣬Сʱ
//				
//				break;
//			}
//			so_p = so_p->Next;
//		}
//  		::FreeChildPtr(so_p0);	
//	}

	FD* fd = dsbob2->fd_tb->GetTable();
	LD* ld = dsbob2->ld_tb->GetTable();

	fault_ltime = (fd+fdph)->fault_ltime;	//���϶�λʱ��,Сʱ
	fault_stime = (fd+fdph)->fault_stime;	//�����л�ʱ��,Сʱ
	fault_ttime = (fd+fdph)->fault_ttime;	//����ת��ʱ��

	plan_ltime = (fd+fdph)->plan_ltime;	//�����л�ʱ��
	plan_stime = (fd+fdph)->plan_stime;	//�����л�ʱ��
	plan_ttime = (fd+fdph)->plan_ttime;	//����ת��ʱ��
	//		plan_prob = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_prob;	//�����ʣ���/�ٹ�����
	repair_plan = (fd+fdph)->plan_rtime;	//���޳���ʱ�䣬Сʱ

	//strcpy(devtest[0].id, "9ed8cae3ff4945d9a1213d1f3a0168ab");
	//strcpy(devtest[0].name, "CB4");
	//
	//strcpy(devtest[1].id, "171dd40e1a2f4971818894bf623c8c93@2@2");
	//strcpy(devtest[1].name, "0-1");

	//strcpy(devtest[2].id, "4d354ef9d8274813823fea2a754eb5cc@1");
	//strcpy(devtest[2].name, "1-2");

	//strcpy(devtest[3].id, "c965ec3ebd6f48a784de6e42c34ce8b3");
	//strcpy(devtest[3].name, "CB1");

	//strcpy(devtest[4].id, "4e8286229acb4f069dfbffcbdd082087");
	//strcpy(devtest[4].name, "CB3");

	//strcpy(devtest[5].id, "2e70cfc922524fb7b23a023d0e7d4081@2");
	//strcpy(devtest[5].name, "2-3");

	//strcpy(devtest[6].id, "58244b12c548473d88f1b558f47f0506");
	//strcpy(devtest[6].name, "S1");

	//strcpy(devtest[7].id, "a00c47696f3849889b64d1c20086a716@1@1");
	//strcpy(devtest[7].name, "3-4");

	//strcpy(devtest[8].id, "4fdb2c6b679d4f6681b095cf86903a13");
	//strcpy(devtest[8].name, "LS1");

	//strcpy(devtest[9].id, "4fdb2c6b679d4f6681b095cf86903a13");
	//strcpy(devtest[9].name, "4-5");

	//strcpy(devtest[10].id, "a00c47696f3849889b64d1c20086a716@1@2@2");
	//strcpy(devtest[10].name, "5-6");

	//strcpy(devtest[11].id, "8a52f1105cc24070a10d7be5e851974c");
	//strcpy(devtest[11].name, "S2");

	//strcpy(devtest[12].id, "198c2dba46624fab8fa047e4f4e3787d");
	//strcpy(devtest[12].name, "CB2");

	//strcpy(devtest[13].id, "eb9618e1a7f24f5d83cbd6493288b5c3@1@2");
	//strcpy(devtest[13].name, "2-a");

	//strcpy(devtest[14].id, "2e5536c61cf94a56b2acf2399e5d74b5@1");
	//strcpy(devtest[14].name, "4-b");

	//strcpy(devtest[15].id, "602ca7771938432683e2fc8857c43b49@1@2");
	//strcpy(devtest[15].name, "5-8");

	//strcpy(devtest[16].id, "b8116ce332a14e349dd812e0ae111fa3@2@1");
	//strcpy(devtest[16].name, "8-c");

	//strcpy(devtest[17].id, "602ca7771938432683e2fc8857c43b49@2@1");
	//strcpy(devtest[17].name, "8-d");

	//strcpy(devtest[18].id, "62b8c6de8d3e451bbd71612f3a744df2@1@2");
	//strcpy(devtest[18].name, "6-e");

	//strcpy(devtest[19].id, "5524b9ad93d34fb3b000db03581cd27c");
	//strcpy(devtest[19].name, "S3");

	//strcpy(devtest[20].id, "302b403761a94edbb48cd0b6ea17aad0");
	//strcpy(devtest[20].name, "F5");

	//strcpy(devtest[21].id, "1a4ab5e78a0b4ed4b1cca2ddd7dfe3f6");
	//strcpy(devtest[21].name, "Fa");

	//strcpy(devtest[22].id, "c5458fa2f0f3426181af366bd51ac085");
	//strcpy(devtest[22].name, "Fb");

	//strcpy(devtest[23].id, "46ab10d9f5974ff2ad1b34236d4da7f6");
	//strcpy(devtest[23].name, "Fc");

	//strcpy(devtest[24].id, "3b0dd61d197b4e6284f6b57cb06077fe");
	//strcpy(devtest[24].name, "Fd");

	//strcpy(devtest[25].id, "ed5cbf961fda4dc8a76f67503bd35e59");
	//strcpy(devtest[25].name, "Fe");

	//strcpy(devtest[26].id, "6bdb2928aee648dab330f683d7f29e67");
	//strcpy(devtest[26].name, "Ta");

	//strcpy(devtest[27].id, "a4c196af7bb1418da2a79fadb7613d08");
	//strcpy(devtest[27].name, "Tb");

	//strcpy(devtest[28].id, "9ef5d0fc8e934776bba8bf850a4e87d7");
	//strcpy(devtest[28].name, "Tc");
	//		
	//strcpy(devtest[29].id, "6b02bf4f1f7642c094fb88d17cfbce95");
	//strcpy(devtest[29].name, "Td");

	//strcpy(devtest[30].id, "f23b6748ad4a4861927ab081289c38d5");
	//strcpy(devtest[30].name, "Te");




	
//////////////////////////////////////////////////////////////////////////
//lld for test
//struct outtxt
//{
//	 id[64];
//	double fault;
//	double faulttime;
//	double plan;
//	double plantime;
//
//	double perFault;
//	double perFaultTime;
//	double perSum;
//	double perSumTime;
//};
//
//outtxt lldout[5];
//for (i=0; i<5;i++)
//{
//	strcpy(lldout[i].id,(ld+i)->id);
//	lldout[i].fault=0;
//	lldout[i].faulttime=0;
//	lldout[i].plan=0;
//	lldout[i].plantime=0;
//}

	int nUpSwitchload;
	int nUpNonOutload;
	int nUpPlanSwitchload;
	int nUpPlanNonOutload;
	int nFaultload;
	int nNeedTrload;
	int nNowTrload;

	//������е��豸
	eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((fd+fdph)->r_eq);
	eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
	while(eq_p)
	{
		int debug = eq_p->Ptr;
		//���翪�ز��������
		if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type == 4)
		{
			eq_p = eq_p->Next;
			continue;
		}
		////s2
		//if (strcmp("8a52f1105cc24070a10d7be5e851974c", dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id) == 0)
		//{
		//	int ddddd = 0;
		//}

		prob_fault = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->prob_fault;//ֻ��Ҫ�ѹ����ʵ����ó�����������ÿ�����ϵ��豸����һ����
		repair_fault = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->repairTime;//�����޸�ʱ��
		prob_plan = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->prob_check;//�ƻ������ʣ����߶μƻ�����ʱ���ǹ̶���

		nUpNonOutload = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpNonOutload;
		nUpSwitchload = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpSwitchload;
		nUpPlanNonOutload = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpPlanNonOutload;
		nUpPlanSwitchload = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nUpPlanSwitchload;
		nFaultload = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nFaultload;
		nNeedTrload = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNeedTrload;
		nNowTrload = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->nNowTrload;
		int* UpNonOutload = new int[nUpNonOutload];
		int* UpSwitchload = new int[nUpSwitchload];
		int* UpPlanNonOutload = new int[nUpPlanNonOutload];
		int* UpPlanSwitchload = new int[nUpPlanSwitchload];
		int* Faultload = new int[nFaultload];
		int* NeedTrload = new int[nNeedTrload];
		int* NowTrload = new int[nNowTrload];

		perFaultTime=0;
		perPlanTime=0;
		

		//for (i=0; i<5;i++)
		//{
		//	lldout[i].fault=prob_fault;
		//	lldout[i].plan=prob_plan;

		//}
//		
//		outfile<<OnGetName(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id)<<":"<<endl;
		for (i=0; i<nUpNonOutload;i++)
		{
			perFaultTime = 0;

			ldph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpNonOutload[i];
			(ld_result+ldph)->perFault += 0;			
			(ld_result+ldph)->perSum += 0;
			(ld_result+ldph)->perFaultTime += perFaultTime;
			(ld_result+ldph)->perSumTime += perFaultTime;

			//lldout[ldph].faulttime=0;

		//outfile<<(ld_result+ldph)->id<<"   "<<prob_fault<<"   "<<perFaultTime<<"   "<<prob_plan<<"   "<<perPlanTime<< endl;
		}
		for (i=0; i<nUpSwitchload;i++)
		{
			perFaultTime = (fault_ltime + fault_stime) * prob_fault;//�����л�ʱ��

			ldph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpSwitchload[i];
			(ld_result+ldph)->perFault += prob_fault;			
			(ld_result+ldph)->perSum += prob_fault;
			(ld_result+ldph)->perFaultTime += perFaultTime;
			(ld_result+ldph)->perSumTime += perFaultTime;

			//lldout[ldph].faulttime=perFaultTime;
		//outfile<<(ld_result+ldph)->id<<"   "<<prob_fault<<"   "<<perFaultTime<<"   "<<prob_plan<<"   "<<perPlanTime<< endl;
		}

		if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
		{
			for (i=0; i<nUpPlanNonOutload;i++)
			{
				perPlanTime = 0;

				ldph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpPlanNonOutload[i];
				(ld_result+ldph)->perPlan += 0;			
				(ld_result+ldph)->perSum += 0;
				(ld_result+ldph)->perPlanTime += perPlanTime;
				(ld_result+ldph)->perSumTime += perPlanTime;
	
				//lldout[ldph].plantime=0;
			//outfile<<(ld_result+ldph)->id<<"   "<<prob_fault<<"   "<<perFaultTime<<"   "<<prob_plan<<"   "<<perPlanTime<< endl;
			}
			for (i=0; i<nUpPlanSwitchload;i++)
			{
				perPlanTime = (plan_ltime+plan_stime) * prob_plan;//�����л�ʱ��

				ldph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->UpPlanSwitchload[i];
				(ld_result+ldph)->perPlan += prob_plan;			
				(ld_result+ldph)->perSum += prob_plan;
				(ld_result+ldph)->perPlanTime += perPlanTime;
				(ld_result+ldph)->perSumTime += perPlanTime;

				//lldout[ldph].plantime= perPlanTime;
			//outfile<<(ld_result+ldph)->id<<"   "<<prob_fault<<"   "<<perFaultTime<<"   "<<prob_plan<<"   "<<perPlanTime<< endl;
			}
		}
		
		for (i=0; i<nFaultload;i++)
		{
			perFaultTime = repair_fault * prob_fault;//�����л�ʱ��
			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
				perPlanTime  = repair_plan * prob_plan;//Ԥ�����л�ʱ��
			else
				perPlanTime  = 0;

			ldph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->Faultload[i];
			(ld_result+ldph)->perFault += prob_fault;			
			(ld_result+ldph)->perSum += prob_fault + prob_plan;
			(ld_result+ldph)->perFaultTime += perFaultTime;
			(ld_result+ldph)->perSumTime += perFaultTime + perPlanTime;

			//lldout[ldph].faulttime= perFaultTime;
			//lldout[ldph].plantime = perPlanTime;

			//outfile<<(ld_result+ldph)->id<<"   "<<prob_fault<<"   "<<perFaultTime<<"   "<<prob_plan<<"   "<<perPlanTime<< endl;
		}
		for (i=0; i<nNeedTrload;i++)
		{
			ldph = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NeedTrload[i];
			int iFind = 0;
			for(j=0; j<nNowTrload;j++)
			{
				if (ldph == dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->NowTrload[j])
				{
					perFaultTime = (fault_ltime+fault_ttime) * prob_fault;//�����л�ʱ��
					if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
						perPlanTime  = (plan_stime+plan_ttime) * prob_plan;//Ԥ�����л�ʱ��
					else
						perPlanTime  = 0;
					iFind=1;
					break;
				}
			}
			if (iFind == 0)
			{
				perFaultTime = repair_fault * prob_fault;//�����л�ʱ��
				if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
					perPlanTime  = repair_plan * prob_plan;//Ԥ�����л�ʱ��
				else
					perPlanTime  = 0;
			}
			(ld_result+ldph)->perFault += prob_fault;			
			(ld_result+ldph)->perSum += prob_fault + prob_plan;
			(ld_result+ldph)->perFaultTime += perFaultTime;
			(ld_result+ldph)->perSumTime += perFaultTime + perPlanTime;
			//outfile<<(ld_result+ldph)->id<<"   "<<prob_fault<<"   "<<perFaultTime<<"   "<<prob_plan<<"   "<<perPlanTime<< endl;
			//lldout[ldph].faulttime= perFaultTime;
			//lldout[ldph].plantime= perPlanTime;
		}
		
//int j=0;
//outtxt lldout_[5];
//for (i=0; i<5;i++)
//	if (strcmp(lldout[i].id,"6bdb2928aee648dab330f683d7f29e67")==0)
//	{
//		strcpy(lldout_[j].id,lldout[i].id);
//		lldout_[j].fault=lldout[i].fault;
//		lldout_[j].faulttime=lldout[i].faulttime;
//		lldout_[j].plan=lldout[i].plan;
//		lldout_[j++].plantime=lldout[i].plantime;
//	}
//for (i=0; i<5;i++)
//	if (strcmp(lldout[i].id,"a4c196af7bb1418da2a79fadb7613d08")==0)
//	{
//		strcpy(lldout_[j].id,lldout[i].id);
//		lldout_[j].fault=lldout[i].fault;
//		lldout_[j].faulttime=lldout[i].faulttime;
//		lldout_[j].plan=lldout[i].plan;
//		lldout_[j++].plantime=lldout[i].plantime;
//		break;
//	}
//for (i=0; i<5;i++)
//	if (strcmp(lldout[i].id,"9ef5d0fc8e934776bba8bf850a4e87d7")==0)
//	{
//		strcpy(lldout_[j].id,lldout[i].id);
//		lldout_[j].fault=lldout[i].fault;
//		lldout_[j].faulttime=lldout[i].faulttime;
//		lldout_[j].plan=lldout[i].plan;
//		lldout_[j++].plantime=lldout[i].plantime;
//		break;
//	}
//for (i=0; i<5;i++)
//	if (strcmp(lldout[i].id,"6b02bf4f1f7642c094fb88d17cfbce95")==0)
//	{
//		strcpy(lldout_[j].id,lldout[i].id);
//		lldout_[j].fault=lldout[i].fault;
//		lldout_[j].faulttime=lldout[i].faulttime;
//		lldout_[j].plan=lldout[i].plan;
//		lldout_[j++].plantime=lldout[i].plantime;
//		break;
//	}
//for (i=0; i<5;i++)
//	if (strcmp(lldout[i].id,"f23b6748ad4a4861927ab081289c38d5")==0)
//	{
//		strcpy(lldout_[j].id,lldout[i].id);
//		lldout_[j].fault=lldout[i].fault;
//		lldout_[j].faulttime=lldout[i].faulttime;
//		lldout_[j].plan=lldout[i].plan;
//		lldout_[j++].plantime=lldout[i].plantime;
//		break;
//	}
//	
//		outfile<<"a:"<<lldout_[0].fault<<" "<<lldout_[0].faulttime;
//		outfile<<"   b:"<<lldout_[1].fault<<" "<<lldout_[1].faulttime;
//		outfile<<"   c:"<<lldout_[2].fault<<" "<<lldout_[2].faulttime;
//		outfile<<"   d:"<<lldout_[3].fault<<" "<<lldout_[3].faulttime;
//		outfile<<"   e:"<<lldout_[4].fault<<" "<<lldout_[4].faulttime<<endl;
//
//		outfile<<"a:"<<lldout_[0].plan<<" "<<lldout_[0].plantime;
//		outfile<<"   b:"<<lldout_[1].plan<<" "<<lldout_[1].plantime;
//		outfile<<"   c:"<<lldout_[2].plan<<" "<<lldout_[2].plantime;
//		outfile<<"   d:"<<lldout_[3].plan<<" "<<lldout_[3].plantime;
//		outfile<<"   e:"<<lldout_[4].plan<<" "<<lldout_[4].plantime<<endl;

		delete[] UpSwitchload;
		delete[] UpNonOutload;
		delete[] UpPlanSwitchload;
		delete[] UpPlanNonOutload;
		delete[] Faultload;
		delete[] NeedTrload;
		delete[] NowTrload;		

		eq_p = eq_p->Next;
	}
  	::FreeChildPtr(eq_p0);	

	return 1;
}

//������߳����棬Ҫ�����еĶ���������
//unsigned int __stdcall doMath(void *pPM)  
int CRA::doCalc()  
{
//	 sSQL[128];

	ND* ndfind = new ND();
	int i,j,k,l,m,ii,ph;
	int ind, knd, kknd, kkndcb,cntload,cntUN,cntzn,cnteq,cntnd,cntload_bak,vip_cnt;
	int equiptype,ret,ele,pos;
	int ind_Equipment,iznd_Equipment;

	int r_zn;
	ZN* zn=NULL;
	ChildPtr1	*eq_p, *eq_p0;
	ChildPtr1	*cb_p, *cb_p0;
	ChildPtr1	*sec_p, *sec_p0;
	ChildPtr1	*ld_p, *ld_p0;
	ChildPtr1	*un_p, *un_p0;
	ChildPtr1	*zn_p, *zn_p0;
//	ChildPtr1	*nd_p, *nd_p0;
//////////////////////////////////////////////////////////////////////////
	//��ÿ���豸ȡ���������пɿ��Լ���
	//lambda�������ۼ�

	double perPlanTime;//��ƻ�ƽ��ͣ��ʱ�䣬Сʱ/��
	double perFaultTime;
	double perFault;//������
	double perPlan;//������
	
	//����6�������������������
	double prob_fault=0;//ֻ��Ҫ�ѹ����ʵ����ó�����������ÿ�����ϵ��豸����һ����
	double repair_fault=0;//�����޸�ʱ��

	float fault_stime=0;	//�����л�ʱ��
	float fault_ttime=0;	//�����޸�ʱ��

	double prob_plan=0;//ֻ��Ҫ�ѹ����ʵ����ó�����������ÿ�����ϵ��豸����һ����
	double repair_plan=0;

	float plan_ltime=0;
	float plan_stime=0;	//�����л�ʱ��
	float plan_ttime=0;	//�����޸�ʱ��

//lld edit for new probability math 2016-10-30
//	double plan_oh_prob;	//�ܿ��߼����ʣ���/�ٹ�����
//	float plan_oh_rtime;	//�ܿ��߼��޳���ʱ�䣬Сʱ
//	double plan_ohhis_prob;	//�ܿ��߾�Ե�����ʣ���/�ٹ�����
//	float plan_ohhis_rtime;	//�ܿ��߾�Ե���޳���ʱ�䣬Сʱ
//	double plan_cable_prob;	//���¼����ʣ���/�ٹ�����
//	float plan_cable_rtime;	//���¼��޳���ʱ�䣬Сʱ
//lld edit end for new probability math 2016-10-30

//	double plan_prob;	//�����ʣ���/�ٹ�����
//	float plan_rtime;	//���޳���ʱ�䣬Сʱ

	float fault_ltime=0;
	double perFault_ld;//������
	double perSum_ld;//�ƻ�������

	double perFaultTime_ld;//������*����ʱ��
	double perSumTime_ld;//������*����ʱ��	

	int fault_oper_type;//����ʱ��0-ʧ�磬1-ת����2-�л���3-����ʧ��ǰ��Ϊ�۶����򱣻����أ�
	int plan_oper_type; //����ʱ��0-ʧ�磬1-ת����2-�л���3-����ʧ��ǰ��Ϊ�۶����򱣻����أ�
//////////////////////////////////////////////////////////////////////////

	 fdid[MAX_SEC_ID_LEN];//����id���õ���Դ�����������
	fdid[0]=0;

//	DSBOB2* dsbob2 = (DSBOB2 *)pPM;  
//	CRA* pRA = new CRA();

	//�����̵߳�ȫ�ֱ�������Ҫ�ǵ�Դ����ʼ��	
//	ReleaseSemaphore(lld->g_hThreadParameter, 1, NULL);//�ź���++  
//    EnterCriticalSection(&lld->g_csThreadCode);  
//	if (lld->g_begin_so == -1)
//		return 0;
//
//	firstso = lld->g_begin_so;
//	endso	= lld->g_end_so;
	lld->g_ld_ra = 0;
	k =  lld->g_ld_ra;
    //printf("��ʼ����ĵ�Դ��Ϊ%d  ��ֹ��Դ��%d\n", firstso, endso);  
//    LeaveCriticalSection(&lld->g_csThreadCode);  

	int lv_ld = dsbob2->ld_tb->GetCount();
	int lv_eq = dsbob2->eq_tb->GetCount();
	//read all nds into buffer
	int lv_nd = dsbob2->nd_tb->GetCount();
	ND* nd_t = new ND[lv_nd];
	//nd_t���¿����ڴ�������޸�����Ķ�����������Ч��dsbob�����û�иĶ�
	dsbob2->nd_tb->CopyTable(nd_t);
//
////�����ʱ����ṹ
//ofstream outfile_debug("debug_information.txt",ios::out);


	//CRA* pRa = new CRA();
//��ʼ���пɿ��Լ���
	FD* fd = dsbob2->fd_tb->GetTable();
	SO* so = dsbob2->so_tb->GetTable();
	int lv_so = dsbob2->so_tb->GetCount();
	for(ii = 0; ii < lv_so; ii++)
	{
//		printf("so���Ϊ%s  ȫ��ֵΪ%d\n", (so+ii)->id, ii);
		if (39 == ii)
		{
			int debug = 0;
			//continue;
		}

//		printf("so���Ϊ%s  ȫ��ֵΪ%d\n", (so+ii)->id, ii);
			   
		if ( dsbob2->so_tb->GetDataByPh(ii)->fds == -1)//����������
		{
//			sprintf(sSQL,"# %s\t%s\t0 0 0 0 0 0 0 0", \
//			          (so+ii)->fdid,"���ߣ�������������");
//			lld->addLogLine(sSQL);
			continue;
		}

		strcpy(fdid,dsbob2->so_tb->GetDataByPh(ii)->fdid);
		ph = dsbob2->OnGetPhByfdid(fdid);
		cntload = dsbob2->fd_tb->GetDataByPh(ph)->cnt_ld;
		if (cntload==0) 
			continue;		

		fault_ltime = (fd+ph)->fault_ltime;	//���϶�λʱ��,Сʱ
		fault_stime = (fd+ph)->fault_stime;	//�����л�ʱ��,Сʱ
		fault_ttime = (fd+ph)->fault_ttime;	//����ת��ʱ��

		plan_ltime = (fd+ph)->plan_ltime;	//�����л�ʱ��
		plan_stime = (fd+ph)->plan_stime;	//�����л�ʱ��
		plan_ttime = (fd+ph)->plan_ttime;	//����ת��ʱ��
		//		plan_prob = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_prob;	//�����ʣ���/�ٹ�����
		repair_plan = (fd+ph)->plan_rtime;	//���޳���ʱ�䣬Сʱ




		//��������ϵ��豸����
		cntUN = dsbob2->fd_tb->GetDataByPh(ph)->cnt_un;
		cntzn = dsbob2->fd_tb->GetDataByPh(ph)->cnt_zn;
		cnteq = dsbob2->fd_tb->GetDataByPh(ph)->cnt_eq;
		cntnd = dsbob2->fd_tb->GetDataByPh(ph)->cnt_nd;
		
		//ÿ��ֻ��һ�����ߵļ���
		if (cntUN==0) cntUN =1;//��ֹ��ָ��
		DTable<MINPATH>*	ldpath_tb = new DTable<MINPATH>(cntload);//������С·
		DTable<MINPATH>*	bakpath_tb = new DTable<MINPATH>(cntUN*cntload);//���ɱ�����С·
		DTable<MINPATH>*	unpath_tb = new DTable<MINPATH>(cntUN);;//���翪����С·
	
		int innd = dsbob2->so_tb->GetDataByPh(ii)->ind;
		ld_p = dsbob2->r_fd_ld_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_ld);
		ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
		cntload=0;
		while(ld_p)
		{
			MINPATH* minpath = new  MINPATH();
			//minpath->Reset();
			minpath->id = cntload;
			minpath->type = 0;
			strcpy(minpath->eq_id , dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->id);
			minpath->level = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->level;//һ�㸺��
			knd = minpath->ind = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//�õ����ɵĽڵ�

			minpath->s = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
			//������ڵ����ϻ���
			j=0;
			while (1)
			{
				//�豸����һ���ڵ�
				kknd = (dsbob2->nd_tb->GetTable()+knd)->bch;
				if (kknd == knd)
					break;//��ѭ��
				
				//���豸�ı��Ҳ��¼����
				minpath->equippath[j] = getEquipID(dsbob2,knd,kknd);
				
				//��ÿ��ind��¼����������С·�Ĺ켣
				minpath->minpath[j++]=knd;

				knd = kknd;//�豸���ϼ��ڵ��
				if (knd==innd)//���ݵ�����ڵ㣬�����ǵ�Դ�ڵ�
				{
					minpath->minpath[j++]=knd;
					break;
				}
			}
			minpath->pathlen=j;//��С·�ĳ���

			ldpath_tb->Insert(*minpath);
			ld_p = ld_p->Next;
			delete minpath;
			cntload++;
		}
		::FreeChildPtr(ld_p0);
	//��ÿһ���豸 ����Դ�� ����С·��¼����
		eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_eq);
		eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
		//cntload=0;
		while(eq_p)
		{
			knd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
			//������ڵ����ϻ���
			j=0;
			while (1)
			{
				kknd = (dsbob2->nd_tb->GetTable()+knd)->bch;
				if (knd==kknd)//��˵��豸
				{
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath[j++]=knd;
					break;
				}
				
				//���豸�ı��Ҳ��¼����
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idpath[j] = getEquipID(dsbob2,knd,kknd);
				
				//��ÿ��ind��¼����������С·�Ĺ켣
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath[j++]=knd;
				
				knd = kknd;//�õ����ص��ϼ��ڵ��
				if (knd==innd)//���ݵ�����ڵ�
				{
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath[j++]=knd;
					break;
				}
			}
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->pathlen=j;//�豸����С·
			eq_p = eq_p->Next;
		}
		::FreeChildPtr(eq_p0);

		//���翪�ص���Դ���·��

//		if (strcmp(fdid,"CIRCUIT_1018323230")==0)
//		{
//			int debug=0;
//		}
		cntUN=0;
		un_p = dsbob2->r_fd_un_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_un);
		un_p0 = un_p;//����ָ�����ʱ���棬ָ�������͵�����
		while(un_p)
		{
			MINPATH* minpath = new  MINPATH();
			//minpath->Reset();
			minpath->id = cntUN;
			minpath->type = 2;
			if ( strcmp(fdid,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->left)==0 )
			{
				minpath->s = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_P;
				strcpy(ndfind->id, dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_nd);
				knd = dsbob2->nd_tb->FindPh(*ndfind);
			}
			else
			{
				minpath->s = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_P;
				strcpy(ndfind->id, dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_nd);
				knd = dsbob2->nd_tb->FindPh(*ndfind);

				//knd = dsbob2->nd_tb->FindPh(ND(dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_nd));
			}
			minpath->ind = knd;//���翪�ص����

			//������ڵ����ϻ���
			j=0;
			while (1)
			{
				kknd = (dsbob2->nd_tb->GetTable()+knd)->bch;
				if (knd==kknd)//�������翪��
				{
					minpath->minpath[j++]=knd;
					//dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath[j++]=knd;
					break;
				}
				
				//���豸�ı��Ҳ��¼����
				minpath->equippath[j] = getEquipID(dsbob2,knd,kknd);

				//��ÿ��ind��¼����������С·�Ĺ켣
				minpath->minpath[j++]=knd;
				
				knd = kknd;//�õ����ص��ϼ��ڵ��
				if (knd==innd)//���ݵ�����ڵ�
				{
					minpath->minpath[j++]=knd;
					break;
				}
			}
			minpath->pathlen=j;//���翪�ص���Դ�����С·
			unpath_tb->Insert(*minpath);
			delete minpath;
			cntUN++;		
			un_p = un_p->Next;
		}
		::FreeChildPtr(un_p0);

		//3.�����翪��Ϊ��Դ�㣬�ҵ�ȫ���ĸ���
		for(l = 0; l < cntUN; l++)
		{			
			innd = ind = unpath_tb->GetDataByPh(l)->ind;

			//ֻ�Ա��������ϵĽڵ���д���

			//edit by lld 2018-4-24

//			nd_p = dsbob2->r_fd_nd_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_nd);
//			nd_p0 = nd_p;//����ָ�����ʱ���棬ָ�������͵�����
//			//cntload=0;
//			while(nd_p)
//			{
//				(nd_t+nd_p->Ptr)->q |= ND_UNPROC;
//				nd_p = nd_p->Next;
//			}
//			::FreeChildPtr(nd_p0);

			//�����еĽڵ���Ϊδ����׼�����˷�����
			for(i = 0; i < lv_nd; i++)
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
					//read cbs connected to this nd
					//�ӽڵ�-���ع�ϵ���ҵ����к�����ڵ������Ŀ���
					cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd_t+knd)->r_cb);
					cb_p0 = cb_p;//����ָ�����ʱ���棬ָ�������͵�����
					while(cb_p)
					{
						//find the opposite nd_t through this cb
						//kknd��ʾ���صĶԲ�ڵ㣬����Ľڵ㶼�����͵ı��
						if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
						{
							kkndcb = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
						}
						else
						{
							kkndcb = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
						}

						//��������翪�أ�ͣ����,ֻҪ�����翪�أ������Ƿ����գ�����Ҫ���д���
						if ( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_NMLOPEN)//���翪�ص�qΪ1������
							|| (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)  
							//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
							) 
						{
							//����Ǵ����翪�ؽ������������������ͬ����ͨ���տ���
							cb_p = cb_p->Next;
							continue;
						}

		//printf("%s %d\n",cb_tb->GetDataByPh(cb_p->Ptr)->id,cb_tb->GetDataByPh(cb_p->Ptr)->q);
//						//ֻ�бպϵĿ��ز���Ҫ������չ
//						if ((cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB))
//						{
//
//							//�Ͽ����صĺ���������Ҫ��¼����
//							cb_p = cb_p->Next;
//							continue;
//						}

						//process this opposite nd
						//����Բ�ڵ� δ����==���ز����������ƽ�
						if((nd_t+kkndcb)->q & ND_UNPROC)
						{
							//�����ظ�������
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
					sec_p = dsbob2->r_nd_sec_tb->FindChildPtr1((nd_t+knd)->r_sec);
					sec_p0 = sec_p;
					while(sec_p)
					{
						//find the opposite nd_t through this sec
						if(knd == dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind)
						{
							kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->iznd;
						}
						else
						{
							kknd = dsbob2->sec_tb->GetDataByPh(sec_p->Ptr)->ind;
						}

		//printf("%s\n",sec_tb->GetDataByPh(sec_p->Ptr)->id);
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

							//������ڵ����һ����¼������׼������
							(nd_t+kknd)->bch = knd;

							//���ҵ����½ڵ�����Ѵ���
							(nd_t+kknd)->q &= (~ND_UNPROC);
						}
						sec_p = sec_p->Next;
					}
					::FreeChildPtr(sec_p0);

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

			//4.�Ӹ��ɵ����ϻ��ݣ��ҵ�ȫ���ı�����С·
			cntload_bak=0;
			ld_p = dsbob2->r_fd_ld_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_ld);
			ld_p0 = ld_p;//����ָ�����ʱ���棬ָ�������͵�����
//			cntload=0;
			while(ld_p)
			{
				MINPATH* minpath = new  MINPATH();
				//minpath->Reset();
				minpath->id = cntload_bak;
				minpath->type = 1;
				knd = minpath->ind = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//�õ����ɵĽڵ�

				//������ڵ����ϻ���,�õ�����ڵ�ı�����С·���ڵ�-���翪�أ�֮һ
				j=0;
				while (1)
				{
					minpath->bakpath[j++] = knd;

					kknd = (nd_t+knd)->bch;//�õ����ص��ϼ��ڵ��
					if (knd == kknd)//��������
					{
						minpath->bakpath[j++] = knd;
						break;
					}

					knd = kknd;
					if (knd==innd)//���ݵ�����ڵ㣬���翪��
					{
						minpath->bakpath[j++] = knd;
						break;
					}
				}
				minpath->pathlen=j;
				
				minpath->bakAbund = unpath_tb->GetDataByPh(l)->s;//���翪�ص�ԣ��
				minpath->bakCBnd = unpath_tb->GetDataByPh(l)->ind;//���翪�صĽڵ�ţ����ε�Դ�ࣩ
				
				//��������С·�����翪��·������
				minpath->unso_len = unpath_tb->GetDataByPh(l)->pathlen;
				for (m=0; m<unpath_tb->GetDataByPh(l)->pathlen;m++)
				{
					minpath->unso[m]=unpath_tb->GetDataByPh(l)->minpath[m];//���翪�ص���Դ�����С·��
					minpath->unso_reverse[m]=unpath_tb->GetDataByPh(l)->minpath[unpath_tb->GetDataByPh(l)->pathlen-m-1];
				}
				//minpath->cntbakpath++;//������С·������
				bakpath_tb->Insert(*minpath);
				delete minpath;
				cntload_bak++;
				ld_p = ld_p->Next;
			}
			::FreeChildPtr(ld_p0);
		}

		if (cntUN==0)
		{
			//û�����翪�أ��ޱ�����С·
			cntload_bak = 0;
			bakpath_tb->Empty();	
			unpath_tb->Empty();
		}
	
//		fault_stime = (so+ii)->fault_stime;	//�����л�ʱ��,Сʱ
//		fault_ttime = (so+ii)->fault_ttime;	//����ת��ʱ��
//
//		plan_stime = (so+ii)->plan_stime;	//�����л�ʱ��
//		plan_ttime = (so+ii)->plan_ttime;	//����ת��ʱ��
////		plan_prob = (so+ii)->plan_prob;	//�����ʣ���/�ٹ�����
//		plan_rtime = (so+ii)->plan_rtime;	//���޳���ʱ�䣬Сʱ

//	double prob_fault;//ֻ��Ҫ�ѹ����ʵ����ó�����������ÿ�����ϵ��豸����һ����
//	double repair_fault;//�����޸�ʱ��
//
//	float fault_ltime;
//	float fault_stime;	//�����л�ʱ��
//	float fault_ttime;	//�����޸�ʱ��
//
//	double prob_plan;//ֻ��Ҫ�ѹ����ʵ����ó�����������ÿ�����ϵ��豸����һ����
//	double repair_plan;
//
//	float plan_ltime;
//	float plan_stime;	//�����л�ʱ��
//	float plan_ttime;	//�����޸�ʱ��


		for(l=0; l<cntload; l++)
		{
			//��ʼ��		
			perFault_ld = perSum_ld = 0;
			perFaultTime_ld = perSumTime_ld = 0;
			
			//ÿ��һ�����ɺ���Ҫ�Ѵ��������е��Ѵ�����Ϣ���
			zn_p = dsbob2->r_fd_zn_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_zn);
			zn_p0 = zn_p;//����ָ�����ʱ���棬ָ�������͵�����
			while(zn_p)
			{
				dsbob2->zn_tb->GetDataByPh(zn_p->Ptr)->fault_oper_type = -1;
				dsbob2->zn_tb->GetDataByPh(zn_p->Ptr)->plan_oper_type = -1;

				zn_p = zn_p->Next;
			}
			::FreeChildPtr(zn_p0);

			ind = ldpath_tb->GetDataByPh(l)->ind;//�õ����ɵĽڵ�
			
			//outfile_debug<<dsbob2->ld_tb->GetDataByPh(l)->id<<endl;

			vip_cnt = 0;
			//�������ϵ�ÿһ���豸���д���
			eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_eq);
			eq_p0 = eq_p;//����ָ�����ʱ���棬ָ�������͵�����
			while(eq_p)
			{
				fault_oper_type = plan_oper_type = -1;//���豸һ�����ϻ���޺�����ĺ��
				equiptype = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type;//10 Ϊ��·

				int debug = eq_p->Ptr;
				//���翪�ز��������

				//ͬһ�������ڵ��豸�Ը��ɵ��Ӱ����һ���ģ�����ֻ�ǹ�����������
				//�豸���Ϻ󣬺����ʲô���ģ��������ʳ����������ǣ�ͣ�磬��ͣ��(ת�����л�)

				//����豸��������,ֻ�����߶β��п����漰�����μ��㣬�����豸���ǵ����豸�����ܼ�
				r_zn = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->r_zn;
				if (r_zn != -1)
				{
					//�ҵ������
					r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1(r_zn);
					zn = dsbob2->zn_tb->GetDataByPh(r_zn);
					
					//����������Ƿ��Ѿ��������������Ļ�����������豸����������
					if ( (zn->fault_oper_type != -1) || (zn->plan_oper_type != -1) )
					{
						//ֱ��ȡ���豸
						fault_oper_type = zn->fault_oper_type;
						plan_oper_type = zn->plan_oper_type;//���豸һ�����ϻ���޺�����ĺ��
						goto loop0;
					}
				}
				
				knd = -1;
				ind_Equipment = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;//�豸�Ľڵ�����˳��ģ��ӵ�Դ�㿪ʼ����
				iznd_Equipment = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;

				if ((lld->ele_comp(ldpath_tb->GetDataByPh(l)->minpath,ind_Equipment,-2)>=0)
					&&
					(lld->ele_comp(ldpath_tb->GetDataByPh(l)->minpath,iznd_Equipment,-2)>=0))
					knd = iznd_Equipment;
				else
				//ֻ��һ�˹����������ϣ����ص����������ϵĽڵ�
				if (lld->ele_comp(ldpath_tb->GetDataByPh(l)->minpath,ind_Equipment,-2)>=0)
					knd = ind_Equipment;
				else
				if (lld->ele_comp(ldpath_tb->GetDataByPh(l)->minpath,iznd_Equipment,-2)>=0) 
					knd = iznd_Equipment;

				if (knd != -1)//����С·��,���豸�Ĺ��Ͽ������𸺺�ͣ��
				{
//				t_beg = lld->getCurMi();		
					//��Ҫ�ж��豸�Ƿ��ڽ����ĺ��棬����ں��棬�Ͳ���ת��
					if (equipCanTrans(dsbob2,ldpath_tb->GetTable(),l,cntload,bakpath_tb->GetTable(),cntload_bak,knd) > 0)
					{//��ת��
						fault_oper_type = plan_oper_type = 1;
					}
					else
					{
						fault_oper_type = plan_oper_type = 0;
					}

					goto loop0; 
				}

				//t_b = lld->getCurMi();

				//2.û��ֱ������С·���鿴������С·֮����������
				ele=knd=pos=-1;//���ñ����ĳ�ʼ��
				//�豸����С·�͸��ɵ���С·�Ľ���㣬���������п��ܲ������翪�����Դ����С·����
				knd = lld->getCrossPoint(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath, \
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->pathlen,ldpath_tb->GetDataByPh(l)->minpath,ldpath_tb->GetDataByPh(l)->pathlen,ele);

				//t_getCrossPoint += lld->getCurMi() - t_b;
				
				//2.���豸��������Ƿ��п��ػ��۶���
				if (knd > 0)
				{
//				t_b = lld->getCurMi();
					//���Լ������������û�п��ػ��۶�������Ҫһֱ�ҵ������Ϊֹ
					ret = getFirstCB(dsbob2,dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath,knd);
//				t_getFirstCB += lld->getCurMi() - t_b;
				
//				ret = 0;
					if (ret == 0)//ǰ��û�п��أ��൱������С·��
					{
//				t_beg = lld->getCurMi();
						//��Ҫ�ж��豸�Ƿ��ڽ����ĺ��棬����ں��棬�Ͳ���ת��
						if (equipCanTrans(dsbob2,ldpath_tb->GetTable(),l,cntload,bakpath_tb->GetTable(),cntload_bak,ele) > 0)
						{//��ת��
							fault_oper_type = plan_oper_type = 1;
						}
						else
						{
							fault_oper_type = plan_oper_type = 0;
						}

//				t_nopath_eq += lld->getCurMi() - t_beg;
						goto loop0;
					}

					//ǰ���п��أ���Ҫ���ݿ��ص��������ж����л����������л�
					if ( (ret & EQUIPMENT_LOADER) || (ret & EQUIPMENT_BREAKER)
						|| (ret & EQUIPMENT_DISCONN) )
					{//�л������л�ʱ�����������ʱ��
						fault_oper_type = 2;
					}

					if ( (ret & EQUIPMENT_RELAY)
						||(ret & EQUIPMENT_FUSE) )
					{//�����Ͽ���û���л�ʱ��
						fault_oper_type = 3;
					}

					//����ֻ������·��
					if (equiptype == 10) 
					{
						if ( (ret & EQUIPMENT_DISCONN)
							)
						{//ֻ�и��뿪�زŲ����е���������
							plan_oper_type = 2;
						}

						//�ƻ����޵�����£��۶���Ҳ�ǿ��Խ������������л�
						if ( (ret & EQUIPMENT_RELAY)
							||(ret & EQUIPMENT_BREAKER) 
							||(ret & EQUIPMENT_FUSE) 
							||(ret & EQUIPMENT_LOADER) )
						{
							plan_oper_type = 3;
						}
					}
				}
loop0:
				perPlanTime=0;//��ƽ��ͣ��ʱ�䣬Сʱ/��
				perFaultTime = 0;
				perFault = 0;//������
				perPlan = 0;//�ƻ�ͣ����

				//��������£���Ҫһ�豸һ��
				prob_fault = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->prob_fault;
				repair_fault = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->repairTime;

				prob_plan = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->prob_check;
				//plan_rtime = equip_tb->GetDataByPh(j)->checkTime;
				//���ۼӹ������
				switch(fault_oper_type)
				{
				case 0://����ת
						perFault = prob_fault;//������
						perFaultTime = prob_fault * repair_fault;//���Ϻ��ʱ����
						break;

				case 1://����ת
						perFault = prob_fault;//����		
						perFaultTime = prob_fault * (fault_ltime+fault_ttime);//ת��ʱ��ÿ�����߶�һ��
						break;

				case 2://������
						perFault = prob_fault;
						perFaultTime = prob_fault * (fault_ltime+fault_stime);
						break;
				}

				if (equiptype == 10)
				{
					switch(plan_oper_type)
					{
					case 0://����ת
							perPlan = prob_plan;//Ԥ����ͣ����
							perPlanTime = prob_plan * repair_plan;
						break;

					case 1://����ת
							perPlan = prob_plan;//Ԥ����ͣ����		
							perPlanTime = prob_plan * (plan_stime+plan_ttime);//ת��ʱ��ÿ�����߶�һ��
						break;

					case 2://������
							perPlan = prob_plan;
							perPlanTime = prob_plan * (plan_stime+plan_ltime);
						break;
					}

					zn->fault_oper_type = fault_oper_type; 
					zn->plan_oper_type = plan_oper_type;//���豸һ�����ϻ���޺�����ĺ��
				}

				//outfile<<equip_tb->GetDataByPh(j)->ind<<" "<<equip_tb->GetDataByPh(j)->iznd<<"   ������"<<prob_fault<<"  perFaultTimeʱ��"<<perFaultTime<<"       �ƻ���"<<plan_prob<<" perPlanTimeʱ��"<<perPlanTime<<endl;
			//outfile_debug<<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id<<" "<<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind<<" "<<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd<<"   ������"<<prob_fault<<"  perFaultTimeʱ��"<<perFaultTime<<"       �ƻ���"<<prob_plan<<" perPlanTimeʱ��"<<perPlanTime<<endl;
				//�������ۼ�
				perFault_ld += perFault;
				perSum_ld += perFault + perPlan;

				perFaultTime_ld += perFaultTime;//����ʱ����
				perSumTime_ld += perFaultTime + perPlanTime;//�ܵ�ʱ����
				
				eq_p = eq_p->Next;
			}
			::FreeChildPtr(eq_p0);

//			EnterCriticalSection(&lld->g_csThreadCode);  
			//׼��д�������������
			strcpy((ld_result+lld->g_ld_ra)->id,ldpath_tb->GetDataByPh(l)->eq_id);
			strcpy((ld_result+lld->g_ld_ra)->fdid,fdid);
			(ld_result+lld->g_ld_ra)->perFault = perFault_ld;
			(ld_result+lld->g_ld_ra)->perSum = perSum_ld;
			(ld_result+lld->g_ld_ra)->perFaultTime = perFaultTime_ld;
			(ld_result+lld->g_ld_ra)->perSumTime = perSumTime_ld;
			(ld_result+lld->g_ld_ra)->s = ldpath_tb->GetDataByPh(l)->s;
			lld->g_ld_ra++;
				//outfile_debug<<"���ɣ�"<<ldpath_tb->GetDataByPh(l)->eq_id<<fdid<<"   ������"<<perFault_ld<<"  perFaultTimeʱ��"<<perFaultTime_ld<<"       ���ƻ���"<<perSum_ld<<" perPlanTimeʱ��"<<perSumTime_ld<<endl;
//		    LeaveCriticalSection(&lld->g_csThreadCode);  
		}
		
//		ldpath=0;
//		unpath=0;
//		bakpath=0;

		delete	ldpath_tb;
		delete	bakpath_tb;
		delete	unpath_tb;	

		//outfile_debug.close();
	}
	delete ndfind;
	//delete pRA;
	return 0;  
}  

//������ĩ�ڵ�ţ������豸��ָ��
int CRA::getEquipID(DSBOB2* dsbob2,int start, int end)
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
	
	//EQUIP* e_find=new EQUIP();
	
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

//���ߵ�ID��UN��ID���õ��Է���ԣ��
int CRA::getUNCB_abun(DSBOB2* dsbob2,char* fdid,char* cb_id,double& p_abund,double& q_abund,double& v_abund)
{
//	int ph;
//	ChildPtr1	*un_p, *un_p0;
//	 cbid[64],left[64],right[64];
//
//	double dW,dR,abunW,abunR;//�Բ�ĵ�Դ���й����޹�
//	FD* fd = dsbob2->fd_tb->GetTable();
//	int lv_fd = dsbob2->fd_tb->GetCount();
//
//	UN* un = dsbob2->un_tb->GetTable();
//	int lv_un = dsbob2->un_tb->GetCount();
//
//	dW=dR=abunW=abunR=0;
//	ph = dsbob2->fd_tb->FindPh(FD(fdid));//�����߱��е�λ��
//
//	//�õ�����id
//	un_p = dsbob2->r_fd_un_tb->FindChildPtr1((fd+ph)->r_un);
//	un_p0 = un_p;//����ָ�����ʱ���棬ָ�������͵�����
//	while(un_p)
//	{
//		strcpy(cbid , dsbob2->un_tb->GetDataByPh(un_p->Ptr)->id);//���翪�ص�ID
//		if (strcmp(cbid,cb_id)==0)
//		{
//			//����Ҫ�ж����Ҳ�
//			strcpy(left,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->left);
//			strcpy(right,dsbob2->un_tb->GetDataByPh(un_p->Ptr)->right);
//			
//			if (strcmp(left,fdid) == 0)
//			{
//				p_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_P;
//				q_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_Q;
//				v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->r_abund_V;
//			}
//
//			if (strcmp(right,fdid) == 0)
//			{
//				p_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_P;
//				q_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_Q;
//				v_abund = dsbob2->un_tb->GetDataByPh(un_p->Ptr)->l_abund_V;
//			}
//			
//			break;
//		}
//		un_p = un_p->Next;
//	}
//	::FreeChildPtr(un_p0);

	return 1;
}

//���ɵ�ind��ָ��������С·��������С·��ȫ��������С·�������豸id
int CRA::equipCanTrans(DSBOB2* dsbob2,MINPATH* ldpath,int ldid,int cntload,MINPATH* bakpath,int cntbakpath,int eq_ind)
{
	int i,l;
	int ind;
	int level,main_cross,cross_nd,ele,pos;//���ɵȼ�
	double union_abund,ld_self,ld_sum,ld_vip_sum;

//	long time_begin=lld->getCurMi();

	//ɨ��ÿһ��������С·
	for (i=0; i<cntbakpath; i++)
	{
		ind = (bakpath+i)->ind;//�������ɵĽڵ��
		if (ind != (ldpath+ldid)->ind)
			continue;//�������ɵ����С·

		union_abund = (bakpath+i)->bakAbund;//����ԣ��
		if ((ldpath+ldid)->s > union_abund)
			goto loop0;

		//���Բ���Ҫ���ɵ���С·����
		ld_self = ld_sum = (ldpath+ldid)->s;
		ld_vip_sum = 0;
		level = ldpath->level;
		
		//�ҵ��������ߵĽ����
		pos = lld->getCrossPoint((bakpath+i)->unso,(bakpath+i)->unso_len,(ldpath+ldid)->minpath,(ldpath+ldid)->pathlen ,cross_nd);

		//�ж�ʱ������Ҫ�۵���������ȥ
		
//���ж��豸�Ƿ���ת����·��������
		if (lld->ele_comp((bakpath+i)->minpath,eq_ind,cross_nd) == 1)
			continue;//ֱ�Ӳ���ת��
//����豸�Լ��ͽ����֮�䲻�ܸ��룬����ת��
		//if (isDisconnect(mainND_reverse,main_iznd,cross_nd) == -1)						
		if (isDisconnect(dsbob2,(bakpath+i)->unso_reverse,eq_ind,cross_nd) == -1)						
			continue;

		//time_begin = lld->getCurMi();

//1.�豸�ĺ����һ�������ڽ����ǰ�棬ת���Ŀ������Ѿ�����
//2.�жϺ���ĸ����Ƿ���Խ�����Ч��ת��

		//�ж��Ƿ����������ɵ��ӵ�ת��·����
		
		for(l=0; l<cntload; l++)
		{
			if ( ldid != (ind = (ldpath+l)->ind) )//�õ������ĸ��ɽڵ�
			{
				//������С·֮��Ľ����
				//getCrossPoint(path_load[l].minpath,mainND,ele,cross,pos);
				main_cross = lld->getCrossPoint((ldpath+l)->minpath,(ldpath+l)->pathlen, (bakpath+i)->unso,(bakpath+i)->unso_len,ele);
				//1.���жϸ��ɵ���Ҫ�ԣ�
				//����ȼ���ĸ��ɵ���Ҫ���������豸�ĺ��棬�ҿ��Ը��룬Ҫ���ȼӵ�����
				if ((ldpath+l)->level < level)
				{
					//��Ҫ�û��۵���С·�ϵĵ����豸�Ŀ�����Դ�࣬��Ӱ��
					if (lld->ele_comp((bakpath+i)->unso,eq_ind,ele) == 1)
						continue;

					//��Ҫ�û����豸�ĺ��棬���и��뿪�ظ�������ת��������
					if (isDisconnect(dsbob2,(bakpath+i)->unso_reverse,eq_ind,ele) != -1)						
						ld_vip_sum += (ldpath+l)->s;

					continue;
				}

				//2.���ͬ����Ҫ���߲�����Ҫ,�ڽ�������Ķ�Ҫת��
				if (lld->ele_vector((bakpath+i)->unso,ele,(bakpath+i)->unso_len) <= main_cross)
				{
					if (isDisconnect(dsbob2,(bakpath+i)->unso_reverse,eq_ind,ele) != -1)						
						ld_sum += (ldpath+l)->s;
				}
			}
		}

		//t_tran += lld->getCurMi()-time_begin;

		//���ȱ�֤��Ҫ�û�
		if (ld_sum < (union_abund - ld_vip_sum) )//ת��
			return 1;
loop0:
		union_abund = 0;
	}

//	t_equipCanTrans += lld->getCurMi()-time_begin;
	return -1;
}

//�õ��豸��С·�ϵĵ������Ŀ��أ�-1��û�п��أ�0�����뿪�أ�1���۶�����������Ķ�·��
int CRA::isDisconnect(DSBOB2* dsbob2,int* minPath_src,int start, int end)//ͨ�����ɵĽڵ�ţ��õ����ɵ���С·
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
		//ע�������з��򣬿���start�ĵ�һ������
		ind = minPath_src[i];
		iznd = minPath_src[i+1];

		if (ind == start)//��ʼ
		{
			iFind = 1;
		}
		
		if (iFind)
		{

		//time_begin = lld->getCurMi();
		
			//type = getEquip(ind,iznd)->type;
			type = getCBType(dsbob2,ind,iznd);
		//t_tran += lld->getCurMi()-time_begin;

			if ( (type == 0)//��·��
			  || ( type == 5)//��������
			  || ( type == 1)//loadbreakswitch
			  || ( type == 2)//disconnector
			  || ( type == 3)//fuse
			   )
			{
				return type;
			}
		}
	
		if (iznd == end)//����
		{
			iFind = 0;
			break;
		}
	}
		
	return -1;
}

//������ĩ�ڵ�ţ����ؿ��ص����ͣ��Ҳ������ǿ��ط���-1
int CRA::getCBType(DSBOB2* dsbob2,int start, int end)
{
	//����
	if ( (start==-1) || (end==-1))
		return -1;

	//�豸�����ڸ���
//	EQUIP* t_equip;

	int i_id,k,l,itype;
	int i[20],j[20],t[20],i_cnt,j_cnt;
	for(k=0; k<20; k++)
		i[k]=j[k]=t[k]=-1;
	
	ND* nd_tt = dsbob2->nd_tb->GetTable();
	ChildPtr1	*equip_p, *equip_p0;
	//EQUIP* e_find=new EQUIP();
	
	k=0;
	equip_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_tt+start)->r_eq);
	equip_p0 = equip_p;//����ָ�����ʱ���棬ָ�������͵�����
	while(equip_p)
	{
		//0,1,2,3,5 Ϊ����
		itype = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->type;
		if ( (itype == 0)//����
			|| (itype == 1) 
			|| (itype == 2) 
			|| (itype == 3) 
			|| (itype == 5) 
			)//�߶�
		{
			i_id = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->i_id;
			i[k] = i_id;
			t[k++] = itype;
		}

		equip_p = equip_p->Next;
	}
	::FreeChildPtr(equip_p0);
	i_cnt = k;

	k=0;
	equip_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_tt+end)->r_eq);
	equip_p0 = equip_p;//����ָ�����ʱ���棬ָ�������͵�����
	while(equip_p)
	{
		itype = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->type;
		if ( (itype == 0)//����
			|| (itype == 1) 
			|| (itype == 2) 
			|| (itype == 3) 
			|| (itype == 5) 
			)//�߶�
		{
			i_id = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->i_id;
			j[k++] = i_id;
		}

		equip_p = equip_p->Next;
	}
	::FreeChildPtr(equip_p0);
	j_cnt = k;

//���ӵ��豸������ͬһ���豸
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

//�õ��豸��С·�ϵĵ������Ŀ��أ�-1��û�п��أ�0�����뿪�أ�1���۶�����������Ķ�·��
int CRA::getFirstCB(DSBOB2* dsbob2,int* minPath,int p)//ͨ�����ɵĽڵ�ţ��õ����ɵ���С·
{
	int i;
	int ind,iznd,type;
	int iFind=0;
	if (p > 0)//����С·�ϵ�
	{
		//������豸�Լ�����Ҫ�Ȱ��Լ��ų�
		for(i=0; i<p; i++)
		{
			ind = minPath[i];
			iznd = minPath[i+1];

			//type = getEquip(ind,iznd)->type;
			
			type = getCBType(dsbob2,ind,iznd);
			if (type == 3)//�۶���
				iFind |= EQUIPMENT_FUSE;
			else if ( type == 0)//breaker
				iFind |= EQUIPMENT_BREAKER;
			else if (type == 1)//loadbreakswitch
				iFind |= EQUIPMENT_LOADER;
			else if (type == 2)//disconnector
				iFind |= EQUIPMENT_DISCONN;
			else if (type == 5)//breaker_relay
				iFind |= EQUIPMENT_RELAY;
		}
	}
	
	return iFind;
}