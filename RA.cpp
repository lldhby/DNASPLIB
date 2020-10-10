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

//为了用线程来计算可靠性，只能将结果放到外面
_LD_RESULT* ld_result;

//用馈线进行排序
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
	char id[MAX_SEC_ID_LEN];//负荷id
	char fdid[MAX_SEC_ID_LEN],t_fdid[MAX_SEC_ID_LEN];//所属馈线
	
//	double perTimeFault_ld,perTimeSum_ld;
//	double perFault_line,perSum_line,perFaultTime_line,perSumTime_line,perTimeFault_sys,perTimeSum_sys;
//	double perTimeFault_line,perTimeSum_line,perFaultTime_sys,perSumTime_sys,perFault_sys,perSum_sys;
	double ENS_Fault_line,ENS_Fault_sys,ENS_Sum_sys,ENS_Sum_line;

	double perFault_ld=0;	//负荷点年平均故障率，次/年
	double perFault_line=0;	//馈线年平均故障率，次/年
	double perFault_sys=0;	//系统年平均故障率，次/年
	double perSum_ld=0;		//含预安排的 负荷点故障率累加
	double perSum_line=0;	//含预安排的 馈线故障率累加
	double perSum_sys=0;	//含预安排的 系统故障率累加

	//u 时户，可以累加
	double perFaultTime_ld=0;	//负荷点故障年平均停运时间，小时/年，停运时间
	double perFaultTime_line=0;//年平均停运时间，小时/年
	double perFaultTime_sys=0;//年平均停运时间，小时/年
	double perSumTime_ld=0;//
	double perSumTime_line=0;//
	double perSumTime_sys=0;//

	//r，u/lambda ，不能累加
	double perTimeFault_ld=0;//年平均停运持续时间，小时/次
	double perTimeFault_line=0;//年平均停运持续时间，小时/次
	double perTimeFault_sys=0;//年平均停运持续时间，小时/次
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

	//最小路计算程序
	//doCalc();  

	//启用FEMA计算程序
	doFEMA(type);

	ofstream outfile_ra(Efile, ios::out);

	outfile_ra << "<!System= Version=1.0 Code=UTF-8 Data=!>" << endl;
	outfile_ra << "<ra_detailResult::Result name=可靠性计算详细结果表>" << endl;
	outfile_ra << "@\tid\ttype\tASAI_Fault\tSAIFI_Fault\tSAIDI_Fault\tENS_Fault\tASAI_Sum\tSAIFI_Sum\tSAIDI_Sum\tENS_Sum" << endl;
	outfile_ra << "/@\tid\t结果类型\t可靠性(故障%)\t停电频率(故障,次/年)\t停电时间(故障,小时/年)\t缺供电量(故障,kWh/年)\t可靠性(含预安排%)\t停电频率(含预安排,次/年)\t停电时间(含预安排,小时/年)\t缺供电量(含预安排,kWh/年)" << endl;

	//最后进行可靠性统计和文件形成
	//先对数组进行排序
	sort_RA(ld_result, lv_ld);

	int fdcnt = 0;
	l = perFault_line = perSum_line = perFaultTime_line = perSumTime_line = ENS_Sum_line = ENS_Fault_line = ENS_Fault_sys = ENS_Sum_sys = 0;
	t_fdid[0] = 0;
	strcpy(t_fdid, (ld_result + 0)->fdid);//最开始的那个已经给出来了
	if (strlen(t_fdid)>0)
		fdcnt++;
	ENS_Fault_sys = ENS_Sum_sys = 0;//总的损失电量清零

	//只需要计算相关的馈线

	for (i = 0; i < lv_ld; i++)
	{
		//if (strcmp("11000000_373240", (ld_result + i)->id) == 0)
		//{
		//	int lld = 0;
		//}
		strcpy(fdid, (ld_result + i)->fdid);
		//单个用户的可靠性指标
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

		//需要先写这个馈线的可靠性指标，所以要把单个用户的可靠性指标记录下来
		//每个负荷点的可靠性指标计算
		dsbob2->ra_result[l].type = 2;
		strcpy(dsbob2->ra_result[l].fdid, (ld_result + i)->id); //负荷的id 

		dsbob2->ra_result[l].SAIFI_Fault = perFault_ld;//平均停运率
		dsbob2->ra_result[l].SAIDI_Fault = perTimeFault_ld * perFault_ld;//年平均停运时间
		dsbob2->ra_result[l].ASAI_Fault = 100 * (1 - (perFaultTime_ld / 8760));//可用率
		dsbob2->ra_result[l].ENS_Fault = (1000 * (ld_result + i)->s) * perFaultTime_ld;//缺供电量

		ENS_Fault_line += (1000 * (ld_result + i)->s) * perFaultTime_ld;

		dsbob2->ra_result[l].SAIFI_Sum = perSum_ld;//平均停运率
		dsbob2->ra_result[l].SAIDI_Sum = perTimeSum_ld * perSum_ld;//平均停运持续时间
		dsbob2->ra_result[l].ASAI_Sum = 100 * (1 - (perSumTime_ld / 8760));//可用率
		//if ()
		dsbob2->ra_result[l].ENS_Sum = (1000 * (ld_result + i)->s) * perSumTime_ld;//缺供电量

		ENS_Sum_line += (1000 * (ld_result + i)->s) * perSumTime_ld;

		//时户数 和 故障率 可以累加
		perFault_line += perFault_ld;
		perSum_line += perSum_ld;

		perFaultTime_line += perFaultTime_ld;
		perSumTime_line += perSumTime_ld;

		if (perFaultTime_ld > 0)
		{
			//写负荷指标
			sprintf(sSQL, "# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f", \
				dsbob2->ra_result[l].fdid, "负荷", dsbob2->ra_result[l].ASAI_Fault, dsbob2->ra_result[l].SAIFI_Fault, \
				dsbob2->ra_result[l].SAIDI_Fault, dsbob2->ra_result[l].ENS_Fault, dsbob2->ra_result[l].ASAI_Sum, \
				dsbob2->ra_result[l].SAIFI_Sum, dsbob2->ra_result[l].SAIDI_Sum, dsbob2->ra_result[l].ENS_Sum);

			outfile_ra.write(sSQL, strlen(sSQL));
			outfile_ra << endl;
		}
		l++;

		//判断下一个是不是还是这条馈线
		if (
			(i < lv_ld - 1)
			&& (strcmp(t_fdid, (ld_result + i + 1)->fdid) != 0)
			)//新的馈线
		//if (strcmp(t_fdid, (ld_result + i + 1)->fdid) != 0)
		{
			//直接写可靠性指标	
			//	outfile<<"/@  id 结果类型  可用率(故障%) 停电频率(故障,次/年) 停电时间(故障,小时/年)	缺供电量(故障,kWh/年) 可用率(含预安排%) 停电频率(含预安排,次/年) 停电时间(含预安排,小时/年)	缺供电量(含预安排,kWh/年)"<< endl;
						//馈线的可靠性指标计算

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

			//需要先写这个馈线的可靠性指标,最后的那个负荷不能再写了
			if (perFaultTime_line > 0)
			{
				sprintf(sSQL, "# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f", \
					fdid, "馈线", 100 * (1 - (perFaultTime_line / l / 8760)), \
					(perFault_line / l), (perFault_line / l)*perTimeFault_line, ENS_Fault_line, \
					100 * (1 - (perSumTime_line / l / 8760)), (perSum_line / l), (perSum_line / l)*perTimeSum_line, ENS_Sum_line);
				outfile_ra.write(sSQL, strlen(sSQL));
				outfile_ra << endl;
			}
			l = perFault_line = perSum_line = perFaultTime_line = perSumTime_line = ENS_Sum_line = ENS_Fault_line = 0;

			//if (i < lv_ld - 1)
			//换了一条馈线
			strcpy(t_fdid, (ld_result + i + 1)->fdid);
			fdcnt++;
		}
		//最后的那个负荷,并且有一条以上的馈线
		if ((i == lv_ld - 1) && (strlen(t_fdid)>0))
		{
			strcpy(fdid, t_fdid);
			//补写最后的那条馈线
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

			//需要先写这个馈线的可靠性指标,最后的那个负荷不能再写了
			sprintf(sSQL, "# %s\t%s\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f\t%0.4f", \
				fdid, "馈线", 100 * (1 - (perFaultTime_line / l / 8760)), \
				(perFault_line / l), (perFault_line / l)*perTimeFault_line, ENS_Fault_line, \
				100 * (1 - (perSumTime_line / l / 8760)), (perSum_line / l), (perSum_line / l)*perTimeSum_line, ENS_Sum_line);
			outfile_ra.write(sSQL, strlen(sSQL));
			outfile_ra << endl;
		}		
	}

	//最后写系统的指标
		//全部系统的可靠性指标
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
			"-", "系统", 100 * (1 - (perFaultTime_sys / lv_ld / 8760)), \
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

//需要对每个设备都要进行扫描
int CRA::doFEMA(int type)
{
	//以馈线为单位
	//调试数据
//	outfile << "eqid    故障率  计划率 故障时户 计划时户 " << "    " << endl;
	int lv_fd = dsbob2->fd_tb->GetCount();
	FD* fd = dsbob2->fd_tb->GetTable();
	for(int i = 0; i < lv_fd; i++)
	{
		//馈线的每一个设备N-1
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
	//strcpy(ret, "不考虑故障");
	return id;
}

int CRA::WriteFEMA(int fdph)  
{
	//往结果里面写东西
	double perPlanTime;//计划时户数，年计划平均停运时间，小时/年
	double perFaultTime;//故障时户数
	double prob_fault;//只需要把故障率单独拿出来，检修率每条线上的设备都是一样的
	double repair_fault;//故障修复时间
	//以下6个参数是针对整条馈线
	float fault_ltime;	//故障定位时间
	float fault_stime;	//故障切换时间
	float fault_ttime;	//故障修复时间

	float plan_ltime;	//检修切换时间
	float plan_stime;	//检修切换时间
	float plan_ttime;	//检修修复时间

	double prob_plan;	//检修率，次/百公里年
	float  repair_plan;	//检修持续时间，小时

//////////////////////////////////////////////////////////////////////////
//FEMA
//	int UpSwitchload[max_switch_ld];//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
//	int UpNonOutload[max_switch_ld];//故障段上游负荷列表（可切换，仅仅是切换时间的问题）
//	int Faultload[max_switch_ld];	//故障段失电负荷列表
//	int NeedTrload[max_switch_ld];//故障段下游需要转供的负荷列表
//	int NowTrload[max_switch_ld];//故障段下游可以转供的负荷列表
//	
//	int iUpStreamloadSwitchType;//上游负荷切换类型，0无损切，1需要等隔离后切
//////////////////////////////////////////////////////////////////////////

	int i,j,eqph=-1,ldph;
	 fdid[MAX_ID_DLL];
	fdid[0]=0;
	ChildPtr1	*eq_p, *eq_p0;

//	ChildPtr1	*so_p,*so_p0;
//	int r_so = dsbob2->fd_tb->GetDataByPh(fdph)->r_so;
//	if (r_so != -1)//区段内有下游开关
//	{
//		so_p = dsbob2->r_fd_so_tb->FindChildPtr1(r_so);
//		so_p0 = so_p; 
//		while(so_p)
//		{
//			if (dsbob2->so_tb->GetDataByPh(so_p->Ptr)->slackavr == 0)
//			{
//				//soph= dsbob2->so_tb->GetDataByPh(so_p->Ptr)->ind;
//
//				fault_stime = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->fault_stime;	//故障切换时间,小时
//				fault_ttime = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->fault_ttime;	//故障转供时间
//
//				plan_stime = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_stime;	//检修切换时间
//				plan_ttime = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_ttime;	//检修转供时间
//			//		plan_prob = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_prob;	//检修率，次/百公里年
//				repair_plan = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_rtime;	//检修持续时间，小时
//				
//				break;
//			}
//			so_p = so_p->Next;
//		}
//  		::FreeChildPtr(so_p0);	
//	}

	FD* fd = dsbob2->fd_tb->GetTable();
	LD* ld = dsbob2->ld_tb->GetTable();

	fault_ltime = (fd+fdph)->fault_ltime;	//故障定位时间,小时
	fault_stime = (fd+fdph)->fault_stime;	//故障切换时间,小时
	fault_ttime = (fd+fdph)->fault_ttime;	//故障转供时间

	plan_ltime = (fd+fdph)->plan_ltime;	//检修切换时间
	plan_stime = (fd+fdph)->plan_stime;	//检修切换时间
	plan_ttime = (fd+fdph)->plan_ttime;	//检修转供时间
	//		plan_prob = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_prob;	//检修率，次/百公里年
	repair_plan = (fd+fdph)->plan_rtime;	//检修持续时间，小时

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

	//获得所有的设备
	eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1((fd+fdph)->r_eq);
	eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
	while(eq_p)
	{
		int debug = eq_p->Ptr;
		//联络开关不参与计算
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

		prob_fault = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->prob_fault;//只需要把故障率单独拿出来，检修率每条线上的设备都是一样的
		repair_fault = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->repairTime;//故障修复时间
		prob_plan = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->prob_check;//计划检修率，馈线段计划检修时间是固定的

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
			perFaultTime = (fault_ltime + fault_stime) * prob_fault;//故障切换时间

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
				perPlanTime = (plan_ltime+plan_stime) * prob_plan;//故障切换时间

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
			perFaultTime = repair_fault * prob_fault;//故障切换时间
			if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
				perPlanTime  = repair_plan * prob_plan;//预安排切换时间
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
					perFaultTime = (fault_ltime+fault_ttime) * prob_fault;//故障切换时间
					if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
						perPlanTime  = (plan_stime+plan_ttime) * prob_plan;//预安排切换时间
					else
						perPlanTime  = 0;
					iFind=1;
					break;
				}
			}
			if (iFind == 0)
			{
				perFaultTime = repair_fault * prob_fault;//故障切换时间
				if (dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type==10)
					perPlanTime  = repair_plan * prob_plan;//预安排切换时间
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

//在这个线程里面，要把所有的东西都做了
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
	//将每个设备取出来，进行可靠性计算
	//lambda，可以累加

	double perPlanTime;//年计划平均停运时间，小时/年
	double perFaultTime;
	double perFault;//故障率
	double perPlan;//故障率
	
	//以下6个参数是针对整条馈线
	double prob_fault=0;//只需要把故障率单独拿出来，检修率每条线上的设备都是一样的
	double repair_fault=0;//故障修复时间

	float fault_stime=0;	//故障切换时间
	float fault_ttime=0;	//故障修复时间

	double prob_plan=0;//只需要把故障率单独拿出来，检修率每条线上的设备都是一样的
	double repair_plan=0;

	float plan_ltime=0;
	float plan_stime=0;	//检修切换时间
	float plan_ttime=0;	//检修修复时间

//lld edit for new probability math 2016-10-30
//	double plan_oh_prob;	//架空线检修率，次/百公里年
//	float plan_oh_rtime;	//架空线检修持续时间，小时
//	double plan_ohhis_prob;	//架空线绝缘检修率，次/百公里年
//	float plan_ohhis_rtime;	//架空线绝缘检修持续时间，小时
//	double plan_cable_prob;	//电缆检修率，次/百公里年
//	float plan_cable_rtime;	//电缆检修持续时间，小时
//lld edit end for new probability math 2016-10-30

//	double plan_prob;	//检修率，次/百公里年
//	float plan_rtime;	//检修持续时间，小时

	float fault_ltime=0;
	double perFault_ld;//故障率
	double perSum_ld;//计划检修率

	double perFaultTime_ld;//故障率*故障时间
	double perSumTime_ld;//检修率*检修时间	

	int fault_oper_type;//故障时：0-失电，1-转供，2-切换，3-无损失（前端为熔断器或保护开关）
	int plan_oper_type; //检修时：0-失电，1-转供，2-切换，3-无损失（前端为熔断器或保护开关）
//////////////////////////////////////////////////////////////////////////

	 fdid[MAX_SEC_ID_LEN];//馈线id，得到电源点的所属馈线
	fdid[0]=0;

//	DSBOB2* dsbob2 = (DSBOB2 *)pPM;  
//	CRA* pRA = new CRA();

	//处理线程的全局变量，主要是电源的起始点	
//	ReleaseSemaphore(lld->g_hThreadParameter, 1, NULL);//信号量++  
//    EnterCriticalSection(&lld->g_csThreadCode);  
//	if (lld->g_begin_so == -1)
//		return 0;
//
//	firstso = lld->g_begin_so;
//	endso	= lld->g_end_so;
	lld->g_ld_ra = 0;
	k =  lld->g_ld_ra;
    //printf("开始计算的电源点为%d  截止电源点%d\n", firstso, endso);  
//    LeaveCriticalSection(&lld->g_csThreadCode);  

	int lv_ld = dsbob2->ld_tb->GetCount();
	int lv_eq = dsbob2->eq_tb->GetCount();
	//read all nds into buffer
	int lv_nd = dsbob2->nd_tb->GetCount();
	ND* nd_t = new ND[lv_nd];
	//nd_t是新开的内存表，可以修改里面的东西，马上有效，dsbob里面的没有改动
	dsbob2->nd_tb->CopyTable(nd_t);
//
////输出临时计算结构
//ofstream outfile_debug("debug_information.txt",ios::out);


	//CRA* pRa = new CRA();
//开始进行可靠性计算
	FD* fd = dsbob2->fd_tb->GetTable();
	SO* so = dsbob2->so_tb->GetTable();
	int lv_so = dsbob2->so_tb->GetCount();
	for(ii = 0; ii < lv_so; ii++)
	{
//		printf("so编号为%s  全局值为%d\n", (so+ii)->id, ii);
		if (39 == ii)
		{
			int debug = 0;
			//continue;
		}

//		printf("so编号为%s  全局值为%d\n", (so+ii)->id, ii);
			   
		if ( dsbob2->so_tb->GetDataByPh(ii)->fds == -1)//潮流不收敛
		{
//			sprintf(sSQL,"# %s\t%s\t0 0 0 0 0 0 0 0", \
//			          (so+ii)->fdid,"馈线（潮流不收敛）");
//			lld->addLogLine(sSQL);
			continue;
		}

		strcpy(fdid,dsbob2->so_tb->GetDataByPh(ii)->fdid);
		ph = dsbob2->OnGetPhByfdid(fdid);
		cntload = dsbob2->fd_tb->GetDataByPh(ph)->cnt_ld;
		if (cntload==0) 
			continue;		

		fault_ltime = (fd+ph)->fault_ltime;	//故障定位时间,小时
		fault_stime = (fd+ph)->fault_stime;	//故障切换时间,小时
		fault_ttime = (fd+ph)->fault_ttime;	//故障转供时间

		plan_ltime = (fd+ph)->plan_ltime;	//检修切换时间
		plan_stime = (fd+ph)->plan_stime;	//检修切换时间
		plan_ttime = (fd+ph)->plan_ttime;	//检修转供时间
		//		plan_prob = dsbob2->so_tb->GetDataByPh(so_p->Ptr)->plan_prob;	//检修率，次/百公里年
		repair_plan = (fd+ph)->plan_rtime;	//检修持续时间，小时




		//获得馈线上的设备个数
		cntUN = dsbob2->fd_tb->GetDataByPh(ph)->cnt_un;
		cntzn = dsbob2->fd_tb->GetDataByPh(ph)->cnt_zn;
		cnteq = dsbob2->fd_tb->GetDataByPh(ph)->cnt_eq;
		cntnd = dsbob2->fd_tb->GetDataByPh(ph)->cnt_nd;
		
		//每次只做一条馈线的计算
		if (cntUN==0) cntUN =1;//防止空指针
		DTable<MINPATH>*	ldpath_tb = new DTable<MINPATH>(cntload);//负荷最小路
		DTable<MINPATH>*	bakpath_tb = new DTable<MINPATH>(cntUN*cntload);//负荷备用最小路
		DTable<MINPATH>*	unpath_tb = new DTable<MINPATH>(cntUN);;//联络开关最小路
	
		int innd = dsbob2->so_tb->GetDataByPh(ii)->ind;
		ld_p = dsbob2->r_fd_ld_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_ld);
		ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
		cntload=0;
		while(ld_p)
		{
			MINPATH* minpath = new  MINPATH();
			//minpath->Reset();
			minpath->id = cntload;
			minpath->type = 0;
			strcpy(minpath->eq_id , dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->id);
			minpath->level = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->level;//一般负荷
			knd = minpath->ind = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//得到负荷的节点

			minpath->s = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->w;
			//从这个节点向上回溯
			j=0;
			while (1)
			{
				//设备的上一个节点
				kknd = (dsbob2->nd_tb->GetTable()+knd)->bch;
				if (kknd == knd)
					break;//死循环
				
				//把设备的编号也记录下来
				minpath->equippath[j] = getEquipID(dsbob2,knd,kknd);
				
				//把每个ind记录下来就是最小路的轨迹
				minpath->minpath[j++]=knd;

				knd = kknd;//设备的上级节点号
				if (knd==innd)//回溯到输入节点，这里是电源节点
				{
					minpath->minpath[j++]=knd;
					break;
				}
			}
			minpath->pathlen=j;//最小路的长度

			ldpath_tb->Insert(*minpath);
			ld_p = ld_p->Next;
			delete minpath;
			cntload++;
		}
		::FreeChildPtr(ld_p0);
	//把每一个设备 到电源点 的最小路记录下来
		eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_eq);
		eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
		//cntload=0;
		while(eq_p)
		{
			knd = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;
			//从这个节点向上回溯
			j=0;
			while (1)
			{
				kknd = (dsbob2->nd_tb->GetTable()+knd)->bch;
				if (knd==kknd)//最顶端的设备
				{
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath[j++]=knd;
					break;
				}
				
				//把设备的编号也记录下来
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->idpath[j] = getEquipID(dsbob2,knd,kknd);
				
				//把每个ind记录下来就是最小路的轨迹
				dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath[j++]=knd;
				
				knd = kknd;//得到开关的上级节点号
				if (knd==innd)//回溯到输入节点
				{
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath[j++]=knd;
					break;
				}
			}
			dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->pathlen=j;//设备的最小路
			eq_p = eq_p->Next;
		}
		::FreeChildPtr(eq_p0);

		//联络开关到电源点的路径

//		if (strcmp(fdid,"CIRCUIT_1018323230")==0)
//		{
//			int debug=0;
//		}
		cntUN=0;
		un_p = dsbob2->r_fd_un_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_un);
		un_p0 = un_p;//进行指针的临时保存，指针是整型的数字
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
			minpath->ind = knd;//联络开关的起点

			//从这个节点向上回溯
			j=0;
			while (1)
			{
				kknd = (dsbob2->nd_tb->GetTable()+knd)->bch;
				if (knd==kknd)//孤立联络开关
				{
					minpath->minpath[j++]=knd;
					//dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath[j++]=knd;
					break;
				}
				
				//把设备的编号也记录下来
				minpath->equippath[j] = getEquipID(dsbob2,knd,kknd);

				//把每个ind记录下来就是最小路的轨迹
				minpath->minpath[j++]=knd;
				
				knd = kknd;//得到开关的上级节点号
				if (knd==innd)//回溯到输入节点
				{
					minpath->minpath[j++]=knd;
					break;
				}
			}
			minpath->pathlen=j;//联络开关到电源点的最小路
			unpath_tb->Insert(*minpath);
			delete minpath;
			cntUN++;		
			un_p = un_p->Next;
		}
		::FreeChildPtr(un_p0);

		//3.以联络开关为电源点，找到全部的负荷
		for(l = 0; l < cntUN; l++)
		{			
			innd = ind = unpath_tb->GetDataByPh(l)->ind;

			//只对本条馈线上的节点进行处理

			//edit by lld 2018-4-24

//			nd_p = dsbob2->r_fd_nd_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_nd);
//			nd_p0 = nd_p;//进行指针的临时保存，指针是整型的数字
//			//cntload=0;
//			while(nd_p)
//			{
//				(nd_t+nd_p->Ptr)->q |= ND_UNPROC;
//				nd_p = nd_p->Next;
//			}
//			::FreeChildPtr(nd_p0);

			//将所有的节点设为未处理，准备拓扑分析用
			for(i = 0; i < lv_nd; i++)
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
					//read cbs connected to this nd
					//从节点-开关关系表找到所有和这个节点相连的开关
					cb_p = dsbob2->r_nd_cb_tb->FindChildPtr1((nd_t+knd)->r_cb);
					cb_p0 = cb_p;//进行指针的临时保存，指针是整型的数字
					while(cb_p)
					{
						//find the opposite nd_t through this cb
						//kknd表示开关的对侧节点，这里的节点都是整型的编号
						if(knd == dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind)
						{
							kkndcb = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->iznd;
						}
						else
						{
							kkndcb = dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->ind;
						}

						//如果是联络开关，停下来,只要是联络开关，不管是否悬空，都需要进行处理
						if ( (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_NMLOPEN)//联络开关的q为1，常开
							|| (dsbob2->cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB)  
							//&& (lld->isContainor(cb_tb->GetDataByPh(cb_p->Ptr)->fdid,'|') > 0)
							) 
						{
							//如果是从联络开关进行搜索，其它联络等同于普通悬空开关
							cb_p = cb_p->Next;
							continue;
						}

		//printf("%s %d\n",cb_tb->GetDataByPh(cb_p->Ptr)->id,cb_tb->GetDataByPh(cb_p->Ptr)->q);
//						//只有闭合的开关才需要向下扩展
//						if ((cb_tb->GetDataByPh(cb_p->Ptr)->q & CB_FDLCB))
//						{
//
//							//断开开关的后向链不需要记录下来
//							cb_p = cb_p->Next;
//							continue;
//						}

						//process this opposite nd
						//如果对侧节点 未处理==开关不参与拓扑推进
						if((nd_t+kkndcb)->q & ND_UNPROC)
						{
							//避免重复处理开关
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

			//4.从负荷点向上回溯，找到全部的备用最小路
			cntload_bak=0;
			ld_p = dsbob2->r_fd_ld_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_ld);
			ld_p0 = ld_p;//进行指针的临时保存，指针是整型的数字
//			cntload=0;
			while(ld_p)
			{
				MINPATH* minpath = new  MINPATH();
				//minpath->Reset();
				minpath->id = cntload_bak;
				minpath->type = 1;
				knd = minpath->ind = dsbob2->ld_tb->GetDataByPh(ld_p->Ptr)->ind;//得到负荷的节点

				//从这个节点向上回溯,得到这个节点的备用最小路（节点-联络开关）之一
				j=0;
				while (1)
				{
					minpath->bakpath[j++] = knd;

					kknd = (nd_t+knd)->bch;//得到开关的上级节点号
					if (knd == kknd)//孤立负荷
					{
						minpath->bakpath[j++] = knd;
						break;
					}

					knd = kknd;
					if (knd==innd)//回溯到输入节点，联络开关
					{
						minpath->bakpath[j++] = knd;
						break;
					}
				}
				minpath->pathlen=j;
				
				minpath->bakAbund = unpath_tb->GetDataByPh(l)->s;//联络开关的裕度
				minpath->bakCBnd = unpath_tb->GetDataByPh(l)->ind;//联络开关的节点号（本次电源侧）
				
				//处理备用最小路的联络开关路径问题
				minpath->unso_len = unpath_tb->GetDataByPh(l)->pathlen;
				for (m=0; m<unpath_tb->GetDataByPh(l)->pathlen;m++)
				{
					minpath->unso[m]=unpath_tb->GetDataByPh(l)->minpath[m];//联络开关到电源点的最小路径
					minpath->unso_reverse[m]=unpath_tb->GetDataByPh(l)->minpath[unpath_tb->GetDataByPh(l)->pathlen-m-1];
				}
				//minpath->cntbakpath++;//备用最小路的条数
				bakpath_tb->Insert(*minpath);
				delete minpath;
				cntload_bak++;
				ld_p = ld_p->Next;
			}
			::FreeChildPtr(ld_p0);
		}

		if (cntUN==0)
		{
			//没有联络开关，无备用最小路
			cntload_bak = 0;
			bakpath_tb->Empty();	
			unpath_tb->Empty();
		}
	
//		fault_stime = (so+ii)->fault_stime;	//故障切换时间,小时
//		fault_ttime = (so+ii)->fault_ttime;	//故障转供时间
//
//		plan_stime = (so+ii)->plan_stime;	//检修切换时间
//		plan_ttime = (so+ii)->plan_ttime;	//检修转供时间
////		plan_prob = (so+ii)->plan_prob;	//检修率，次/百公里年
//		plan_rtime = (so+ii)->plan_rtime;	//检修持续时间，小时

//	double prob_fault;//只需要把故障率单独拿出来，检修率每条线上的设备都是一样的
//	double repair_fault;//故障修复时间
//
//	float fault_ltime;
//	float fault_stime;	//故障切换时间
//	float fault_ttime;	//故障修复时间
//
//	double prob_plan;//只需要把故障率单独拿出来，检修率每条线上的设备都是一样的
//	double repair_plan;
//
//	float plan_ltime;
//	float plan_stime;	//检修切换时间
//	float plan_ttime;	//检修修复时间


		for(l=0; l<cntload; l++)
		{
			//初始化		
			perFault_ld = perSum_ld = 0;
			perFaultTime_ld = perSumTime_ld = 0;
			
			//每做一个负荷后，需要把存在区段中的已处理信息清除
			zn_p = dsbob2->r_fd_zn_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_zn);
			zn_p0 = zn_p;//进行指针的临时保存，指针是整型的数字
			while(zn_p)
			{
				dsbob2->zn_tb->GetDataByPh(zn_p->Ptr)->fault_oper_type = -1;
				dsbob2->zn_tb->GetDataByPh(zn_p->Ptr)->plan_oper_type = -1;

				zn_p = zn_p->Next;
			}
			::FreeChildPtr(zn_p0);

			ind = ldpath_tb->GetDataByPh(l)->ind;//得到负荷的节点
			
			//outfile_debug<<dsbob2->ld_tb->GetDataByPh(l)->id<<endl;

			vip_cnt = 0;
			//对馈线上的每一个设备进行处理
			eq_p = dsbob2->r_fd_eq_tb->FindChildPtr1(dsbob2->fd_tb->GetDataByPh(ph)->r_eq);
			eq_p0 = eq_p;//进行指针的临时保存，指针是整型的数字
			while(eq_p)
			{
				fault_oper_type = plan_oper_type = -1;//该设备一旦故障或检修后，引起的后果
				equiptype = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->type;//10 为线路

				int debug = eq_p->Ptr;
				//联络开关不参与计算

				//同一个区段内的设备对负荷点的影响是一样的，仅仅只是故障率有区别
				//设备故障后，后果是什么样的，将故障率抽出来，后果是：停电，不停电(转供，切换)

				//这个设备所属区段,只有馈线段才有可能涉及到区段计算，其它设备都是单体设备，不能简化
				r_zn = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->r_zn;
				if (r_zn != -1)
				{
					//找到这个区
					r_zn = dsbob2->r_zn_sec_tb->FindParentPtr1(r_zn);
					zn = dsbob2->zn_tb->GetDataByPh(r_zn);
					
					//看这个区段是否已经处理过，处理过的话，共享这个设备的所有属性
					if ( (zn->fault_oper_type != -1) || (zn->plan_oper_type != -1) )
					{
						//直接取该设备
						fault_oper_type = zn->fault_oper_type;
						plan_oper_type = zn->plan_oper_type;//该设备一旦故障或检修后，引起的后果
						goto loop0;
					}
				}
				
				knd = -1;
				ind_Equipment = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind;//设备的节点是有顺序的，从电源点开始向下
				iznd_Equipment = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd;

				if ((lld->ele_comp(ldpath_tb->GetDataByPh(l)->minpath,ind_Equipment,-2)>=0)
					&&
					(lld->ele_comp(ldpath_tb->GetDataByPh(l)->minpath,iznd_Equipment,-2)>=0))
					knd = iznd_Equipment;
				else
				//只有一端挂在主干线上，返回单端主干线上的节点
				if (lld->ele_comp(ldpath_tb->GetDataByPh(l)->minpath,ind_Equipment,-2)>=0)
					knd = ind_Equipment;
				else
				if (lld->ele_comp(ldpath_tb->GetDataByPh(l)->minpath,iznd_Equipment,-2)>=0) 
					knd = iznd_Equipment;

				if (knd != -1)//在最小路上,该设备的故障可能引起负荷停运
				{
//				t_beg = lld->getCurMi();		
					//需要判断设备是否在交叉点的后面，如果在后面，就不能转供
					if (equipCanTrans(dsbob2,ldpath_tb->GetTable(),l,cntload,bakpath_tb->GetTable(),cntload_bak,knd) > 0)
					{//能转供
						fault_oper_type = plan_oper_type = 1;
					}
					else
					{
						fault_oper_type = plan_oper_type = 0;
					}

					goto loop0; 
				}

				//t_b = lld->getCurMi();

				//2.没有直接连最小路，查看其与最小路之间的连接情况
				ele=knd=pos=-1;//引用变量的初始化
				//设备的最小路和负荷的最小路的交叉点，这个交叉点有可能不在联络开关与电源点最小路径上
				knd = lld->getCrossPoint(dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath, \
					dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->pathlen,ldpath_tb->GetDataByPh(l)->minpath,ldpath_tb->GetDataByPh(l)->pathlen,ele);

				//t_getCrossPoint += lld->getCurMi() - t_b;
				
				//2.从设备到交叉点是否有开关或熔断器
				if (knd > 0)
				{
//				t_b = lld->getCurMi();
					//从自己到交叉点上有没有开关或熔断器，需要一直找到交叉点为止
					ret = getFirstCB(dsbob2,dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->minpath,knd);
//				t_getFirstCB += lld->getCurMi() - t_b;
				
//				ret = 0;
					if (ret == 0)//前面没有开关，相当于在最小路上
					{
//				t_beg = lld->getCurMi();
						//需要判断设备是否在交叉点的后面，如果在后面，就不能转供
						if (equipCanTrans(dsbob2,ldpath_tb->GetTable(),l,cntload,bakpath_tb->GetTable(),cntload_bak,ele) > 0)
						{//能转供
							fault_oper_type = plan_oper_type = 1;
						}
						else
						{
							fault_oper_type = plan_oper_type = 0;
						}

//				t_nopath_eq += lld->getCurMi() - t_beg;
						goto loop0;
					}

					//前面有开关，需要根据开关的性质来判断是切换还是无损切换
					if ( (ret & EQUIPMENT_LOADER) || (ret & EQUIPMENT_BREAKER)
						|| (ret & EQUIPMENT_DISCONN) )
					{//切换，用切换时间来替代检修时间
						fault_oper_type = 2;
					}

					if ( (ret & EQUIPMENT_RELAY)
						||(ret & EQUIPMENT_FUSE) )
					{//主动断开，没有切换时间
						fault_oper_type = 3;
					}

					//检修只考虑线路段
					if (equiptype == 10) 
					{
						if ( (ret & EQUIPMENT_DISCONN)
							)
						{//只有隔离开关才不能切掉正常负荷
							plan_oper_type = 2;
						}

						//计划检修的情况下，熔断器也是可以进行正常负荷切换
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
				perPlanTime=0;//年平均停运时间，小时/年
				perFaultTime = 0;
				perFault = 0;//故障率
				perPlan = 0;//计划停运率

				//故障情况下，需要一设备一议
				prob_fault = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->prob_fault;
				repair_fault = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->repairTime;

				prob_plan = dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->prob_check;
				//plan_rtime = equip_tb->GetDataByPh(j)->checkTime;
				//先累加故障情况
				switch(fault_oper_type)
				{
				case 0://不能转
						perFault = prob_fault;//故障率
						perFaultTime = prob_fault * repair_fault;//故障后的时户数
						break;

				case 1://可以转
						perFault = prob_fault;//故障		
						perFaultTime = prob_fault * (fault_ltime+fault_ttime);//转供时间每条馈线都一样
						break;

				case 2://有损切
						perFault = prob_fault;
						perFaultTime = prob_fault * (fault_ltime+fault_stime);
						break;
				}

				if (equiptype == 10)
				{
					switch(plan_oper_type)
					{
					case 0://不能转
							perPlan = prob_plan;//预安排停运率
							perPlanTime = prob_plan * repair_plan;
						break;

					case 1://可以转
							perPlan = prob_plan;//预安排停运率		
							perPlanTime = prob_plan * (plan_stime+plan_ttime);//转供时间每条馈线都一样
						break;

					case 2://有损切
							perPlan = prob_plan;
							perPlanTime = prob_plan * (plan_stime+plan_ltime);
						break;
					}

					zn->fault_oper_type = fault_oper_type; 
					zn->plan_oper_type = plan_oper_type;//该设备一旦故障或检修后，引起的后果
				}

				//outfile<<equip_tb->GetDataByPh(j)->ind<<" "<<equip_tb->GetDataByPh(j)->iznd<<"   故障率"<<prob_fault<<"  perFaultTime时间"<<perFaultTime<<"       计划率"<<plan_prob<<" perPlanTime时间"<<perPlanTime<<endl;
			//outfile_debug<<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->id<<" "<<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->ind<<" "<<dsbob2->eq_tb->GetDataByPh(eq_p->Ptr)->iznd<<"   故障率"<<prob_fault<<"  perFaultTime时间"<<perFaultTime<<"       计划率"<<prob_plan<<" perPlanTime时间"<<perPlanTime<<endl;
				//故障率累加
				perFault_ld += perFault;
				perSum_ld += perFault + perPlan;

				perFaultTime_ld += perFaultTime;//故障时户数
				perSumTime_ld += perFaultTime + perPlanTime;//总的时户数
				
				eq_p = eq_p->Next;
			}
			::FreeChildPtr(eq_p0);

//			EnterCriticalSection(&lld->g_csThreadCode);  
			//准备写到结果数组里面
			strcpy((ld_result+lld->g_ld_ra)->id,ldpath_tb->GetDataByPh(l)->eq_id);
			strcpy((ld_result+lld->g_ld_ra)->fdid,fdid);
			(ld_result+lld->g_ld_ra)->perFault = perFault_ld;
			(ld_result+lld->g_ld_ra)->perSum = perSum_ld;
			(ld_result+lld->g_ld_ra)->perFaultTime = perFaultTime_ld;
			(ld_result+lld->g_ld_ra)->perSumTime = perSumTime_ld;
			(ld_result+lld->g_ld_ra)->s = ldpath_tb->GetDataByPh(l)->s;
			lld->g_ld_ra++;
				//outfile_debug<<"负荷："<<ldpath_tb->GetDataByPh(l)->eq_id<<fdid<<"   故障率"<<perFault_ld<<"  perFaultTime时间"<<perFaultTime_ld<<"       含计划率"<<perSum_ld<<" perPlanTime时间"<<perSumTime_ld<<endl;
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

//给定首末节点号，返回设备的指针
int CRA::getEquipID(DSBOB2* dsbob2,int start, int end)
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
	
	//EQUIP* e_find=new EQUIP();
	
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

//馈线的ID，UN的ID，得到对方的裕度
int CRA::getUNCB_abun(DSBOB2* dsbob2,char* fdid,char* cb_id,double& p_abund,double& q_abund,double& v_abund)
{
//	int ph;
//	ChildPtr1	*un_p, *un_p0;
//	 cbid[64],left[64],right[64];
//
//	double dW,dR,abunW,abunR;//对侧的电源的有功和无功
//	FD* fd = dsbob2->fd_tb->GetTable();
//	int lv_fd = dsbob2->fd_tb->GetCount();
//
//	UN* un = dsbob2->un_tb->GetTable();
//	int lv_un = dsbob2->un_tb->GetCount();
//
//	dW=dR=abunW=abunR=0;
//	ph = dsbob2->fd_tb->FindPh(FD(fdid));//在馈线表中的位置
//
//	//得到负荷id
//	un_p = dsbob2->r_fd_un_tb->FindChildPtr1((fd+ph)->r_un);
//	un_p0 = un_p;//进行指针的临时保存，指针是整型的数字
//	while(un_p)
//	{
//		strcpy(cbid , dsbob2->un_tb->GetDataByPh(un_p->Ptr)->id);//联络开关的ID
//		if (strcmp(cbid,cb_id)==0)
//		{
//			//还需要判断左右侧
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

//负荷点ind，指定负荷最小路，备用最小路，全部备用最小路个数，设备id
int CRA::equipCanTrans(DSBOB2* dsbob2,MINPATH* ldpath,int ldid,int cntload,MINPATH* bakpath,int cntbakpath,int eq_ind)
{
	int i,l;
	int ind;
	int level,main_cross,cross_nd,ele,pos;//负荷等级
	double union_abund,ld_self,ld_sum,ld_vip_sum;

//	long time_begin=lld->getCurMi();

	//扫描每一条备用最小路
	for (i=0; i<cntbakpath; i++)
	{
		ind = (bakpath+i)->ind;//所属负荷的节点号
		if (ind != (ldpath+ldid)->ind)
			continue;//其它负荷点的最小路

		union_abund = (bakpath+i)->bakAbund;//联络裕度
		if ((ldpath+ldid)->s > union_abund)
			goto loop0;

		//可以不需要负荷的最小路存在
		ld_self = ld_sum = (ldpath+ldid)->s;
		ld_vip_sum = 0;
		level = ldpath->level;
		
		//找到和主干线的交叉点
		pos = lld->getCrossPoint((bakpath+i)->unso,(bakpath+i)->unso_len,(ldpath+ldid)->minpath,(ldpath+ldid)->pathlen ,cross_nd);

		//判断时，都需要折到主干线上去
		
//先判断设备是否在转供的路径交叉点后
		if (lld->ele_comp((bakpath+i)->minpath,eq_ind,cross_nd) == 1)
			continue;//直接不能转供
//如果设备自己和交叉点之间不能隔离，不能转供
		//if (isDisconnect(mainND_reverse,main_iznd,cross_nd) == -1)						
		if (isDisconnect(dsbob2,(bakpath+i)->unso_reverse,eq_ind,cross_nd) == -1)						
			continue;

		//time_begin = lld->getCurMi();

//1.设备的后面第一个开关在交叉点前面，转供的可能性已经存在
//2.判断后面的负荷是否可以进行有效的转供

		//判断是否有其它负荷叠加到转供路径上
		
		for(l=0; l<cntload; l++)
		{
			if ( ldid != (ind = (ldpath+l)->ind) )//得到其它的负荷节点
			{
				//两个最小路之间的交叉点
				//getCrossPoint(path_load[l].minpath,mainND,ele,cross,pos);
				main_cross = lld->getCrossPoint((ldpath+l)->minpath,(ldpath+l)->pathlen, (bakpath+i)->unso,(bakpath+i)->unso_len,ele);
				//1.先判断负荷的重要性，
				//如果比计算的负荷点重要，而且在设备的后面，且可以隔离，要优先加到上面
				if ((ldpath+l)->level < level)
				{
					//重要用户折到最小路上的点在设备的靠近电源侧，无影响
					if (lld->ele_comp((bakpath+i)->unso,eq_ind,ele) == 1)
						continue;

					//重要用户在设备的后面，且有隔离开关隔开，绑到转供负荷上
					if (isDisconnect(dsbob2,(bakpath+i)->unso_reverse,eq_ind,ele) != -1)						
						ld_vip_sum += (ldpath+l)->s;

					continue;
				}

				//2.如果同等重要或者不如重要,在交叉点后面的都要转供
				if (lld->ele_vector((bakpath+i)->unso,ele,(bakpath+i)->unso_len) <= main_cross)
				{
					if (isDisconnect(dsbob2,(bakpath+i)->unso_reverse,eq_ind,ele) != -1)						
						ld_sum += (ldpath+l)->s;
				}
			}
		}

		//t_tran += lld->getCurMi()-time_begin;

		//优先保证重要用户
		if (ld_sum < (union_abund - ld_vip_sum) )//转供
			return 1;
loop0:
		union_abund = 0;
	}

//	t_equipCanTrans += lld->getCurMi()-time_begin;
	return -1;
}

//得到设备最小路上的到交叉点的开关，-1，没有开关；0，隔离开关；1，熔断器或带保护的断路器
int CRA::isDisconnect(DSBOB2* dsbob2,int* minPath_src,int start, int end)//通过负荷的节点号，得到负荷的最小路
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
			type = getCBType(dsbob2,ind,iznd);
		//t_tran += lld->getCurMi()-time_begin;

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

//给定首末节点号，返回开关的类型，找不到或不是开关返回-1
int CRA::getCBType(DSBOB2* dsbob2,int start, int end)
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
	
	ND* nd_tt = dsbob2->nd_tb->GetTable();
	ChildPtr1	*equip_p, *equip_p0;
	//EQUIP* e_find=new EQUIP();
	
	k=0;
	equip_p = dsbob2->r_nd_eq_tb->FindChildPtr1((nd_tt+start)->r_eq);
	equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
	while(equip_p)
	{
		//0,1,2,3,5 为开关
		itype = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->type;
		if ( (itype == 0)//负荷
			|| (itype == 1) 
			|| (itype == 2) 
			|| (itype == 3) 
			|| (itype == 5) 
			)//线段
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
	equip_p0 = equip_p;//进行指针的临时保存，指针是整型的数字
	while(equip_p)
	{
		itype = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->type;
		if ( (itype == 0)//负荷
			|| (itype == 1) 
			|| (itype == 2) 
			|| (itype == 3) 
			|| (itype == 5) 
			)//线段
		{
			i_id = dsbob2->eq_tb->GetDataByPh(equip_p->Ptr)->i_id;
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

//得到设备最小路上的到交叉点的开关，-1，没有开关；0，隔离开关；1，熔断器或带保护的断路器
int CRA::getFirstCB(DSBOB2* dsbob2,int* minPath,int p)//通过负荷的节点号，得到负荷的最小路
{
	int i;
	int ind,iznd,type;
	int iFind=0;
	if (p > 0)//找最小路上的
	{
		//如果是设备自己，需要先把自己排除
		for(i=0; i<p; i++)
		{
			ind = minPath[i];
			iznd = minPath[i+1];

			//type = getEquip(ind,iznd)->type;
			
			type = getCBType(dsbob2,ind,iznd);
			if (type == 3)//熔断器
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