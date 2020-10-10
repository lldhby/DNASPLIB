// optCP.cpp: implementation of the CReliabilty class.
//
//////////////////////////////////////////////////////////////////////
#include "math.h"

#ifdef _WIN32
#include "windows.h"
#else __linux__
#include "../DNASPLIB/lib.h"
#endif

#include "../DNASPLIB/optCP.h"
#include "../DNASPLIB/LLD.h"

#include <fstream>
using namespace std;

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif


typedef int(*dllCall)(DSBOB2*, CLLD*);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

optCP::optCP()
{
}

optCP::optCP(DSBOB2* m_pDb, CLLD* m_pLLD) : PWRFLOW(m_pDb, m_pLLD)
{
	dsbob2		= m_pDb;//将dsbob2传进来后就可以使用其全部的结果，包括潮流计算的结果
	lld = m_pLLD;
	int cntSEC = dsbob2->sec_tb->GetCount();
	sec_tb_t = new  DTable<SEC>(cntSEC);
	for (int i = 0;i < cntSEC;i++)
	{
		SEC* sect = new SEC;

		strcpy(sect->id, dsbob2->sec_tb->GetDataByLog(i)->id);
		strcpy(sect->nd, dsbob2->sec_tb->GetDataByLog(i)->nd);
		strcpy(sect->znd, dsbob2->sec_tb->GetDataByLog(i)->znd);

		sec_tb_t->Append(*sect);
		delete sect;
	}

	//if (lld->mx_cb > 0)
	//	cb_tb		= dsbob2->cb_tb;

	//so_tb		= dsbob2->so_tb;
	//ld_tb		= dsbob2->ld_tb;
	//sec_tb		= dsbob2->sec_tb;
	//cp_tb		= dsbob2->cp_tb;
	//nd_tb		= dsbob2->nd_tb;
	//r_nd_cb_tb	= dsbob2->r_nd_cb_tb;
	//r_nd_cp_tb	= dsbob2->r_nd_cp_tb;
	//r_nd_xf_tb	= dsbob2->r_nd_xf_tb;
	//r_nd_sec_tb = dsbob2->r_nd_sec_tb;
	//r_nd_ld_tb  = dsbob2->r_nd_ld_tb;
}

optCP::~optCP()
{
	delete sec_tb_t;
}

//电压是否合格
int optCP::isValidate()
{
	int lv_sec = dsbob2->sec_tb->GetCount();
	for (int ii = 0; ii < lv_sec; ii++)
	{
		if ((dsbob2->sec_tb->GetDataByPh(ii)->v_b > 1.07)
			|| (dsbob2->sec_tb->GetDataByPh(ii)->v_b < 0.9))
		{
			return 0;
		}
		if ((dsbob2->sec_tb->GetDataByPh(ii)->v_e > 1.07)
			|| (dsbob2->sec_tb->GetDataByPh(ii)->v_e < 0.9))
		{
			return 0;
		}
	}

	return 1;
}

int optCP::optInitFlow(char* Efile_name)
{
	int ret = 0;

#ifdef _WIN32
	HINSTANCE hDLL = ::LoadLibrary("C:\\DNASP\\DNASPAPP\\DLL\\x64\\Debug\\DLLso.dll");
	if (hDLL != NULL)
	{
		dllCall InitMath = (dllCall)GetProcAddress(hDLL, "InitMath");
		if (InitMath != NULL)
		{
			ret = InitMath(dsbob2, lld);
			FreeLibrary(hDLL);
		}
	}
#else __linux__
	ret = InitMath(dsbob2, lld);
#endif



	//NMD* nmd = new NMD(dsbob2, lld);

	////根据已经有的信息形成区段表，lld add 2011-06-06
	////**因为所有的东西都还在内存，所以不存在因为数据库保存的时候出现索引问题导致的数据混乱
	//if ((ret = nmd->DoValidate()) < 0)//拓扑检验，拓扑检验全部都在内存里面进行
	//{
	//	delete nmd;
	//	return -2;
	//}

	//if (!dsbob2->ReadDigitalData())
	//{
	//	//    printf("\n开关状态有误，请检查cb表和 digital表\n");
	//	delete nmd;
	//	return -3;
	//}

	////1.进行拓扑分析
	////dsbob2->bs_tb->Empty();//先将母线表清空
	//TP* tp = new TP(dsbob2, lld);
	//if (tp->tp_allnew() == TP_FAIL)
	//{
	//	delete tp;
	//	delete nmd;
	//	return -4;
	//}

	if ((ret = dsbob2->ReadAnalogData()) > 0)
	{
		//delete tp;
		//delete nmd;
		return -5;
	}

	PWRFLOW* pwrflow = new PWRFLOW(dsbob2, lld);
	pwrflow->g_method = 1;
	if (pwrflow->ExecutePwrflow() < 0)
	{
		delete pwrflow;
		//delete tp;
		//delete nmd;

		return -1;
	}
	else
	{
		char temp1[64], temp2[64];

		//获得每个节点的电压值
		strcpy(temp1, "xx");
		strcpy(temp2, "inipf");
		lld->ReplaceStr(Efile_name, temp1, temp2);
		//在写之前要把以前的删掉
		lld->DelFile(Efile_name);
		dsbob2->WriteOutfile_pf(Efile_name, 1);//输出潮流计算结果	

		delete pwrflow;
		//delete tp;
		//delete nmd;
	}

	return 0;
}

#if _WIN32
int optCP::optInitFlow(NDT* nd,SECT* sec,char* Efile_name)
{
	int ret = 0;

	HINSTANCE hDLL = ::LoadLibrary("C:\\DNASP\\DNASPAPP\\DLL\\x64\\Debug\\DLLso.dll");
	if (hDLL != NULL)
	{
		dllCall InitMath = (dllCall)GetProcAddress(hDLL, "InitMath");
		if (InitMath != NULL)
		{
			ret = InitMath(dsbob2, lld);
			FreeLibrary(hDLL);
		}
	}
	//NMD* nmd = new NMD(dsbob2, lld);

	////根据已经有的信息形成区段表，lld add 2011-06-06
	////**因为所有的东西都还在内存，所以不存在因为数据库保存的时候出现索引问题导致的数据混乱
	//if ((ret = nmd->DoValidate()) < 0)//拓扑检验，拓扑检验全部都在内存里面进行
	//{
	//	delete nmd;
	//	return -2;
	//}

	//if (!dsbob2->ReadDigitalData())
	//{
	//	//    printf("\n开关状态有误，请检查cb表和 digital表\n");
	//	delete nmd;
	//	return -3;
	//}

	////1.进行拓扑分析
	////dsbob2->bs_tb->Empty();//先将母线表清空
	//TP* tp = new TP(dsbob2, lld);
	//if (tp->tp_allnew() == TP_FAIL)
	//{
	//	delete tp;
	//	delete nmd;
	//	return -4;
	//}

	if ((ret = dsbob2->ReadAnalogData()) > 0)
	{
		//delete tp;
		//delete nmd;
		return -5;
	}

	PWRFLOW* pwrflow = new PWRFLOW(dsbob2, lld);
	//pwrflow->g_method = 1;
	if (pwrflow->ExecutePwrflow() < 0)
	{
		delete pwrflow;
		//delete tp;
		//delete nmd;

		return -1;
	}
	else
	{
		int ii,jj,ttt;
		char ndid[MAX_ID_DLL];
		ND* nd_ = new ND();

		//通过节点传出相关的计算值 主节点，节点归算有功，注入有功，无功，电压
		ND* nd1 = dsbob2->nd_tb->GetTable();
		int cntND1 = dsbob2->nd_tb->GetCount();
		for (jj = 0; jj < cntND1; jj++)
		{
			strcpy(ndid, (nd1 + jj)->id);
			if ((strlen(ndid) == 0) || (strcmp(ndid, "-") == 0))
				continue;

			ii = -1;
			for (int kk = 0; kk < cntND1; kk++)
			{
				//从传入的节点表找对应的节点，这个和自动生成的节点不一样
				if (strcmp(ndid, (nd + kk)->id) == 0)
				{
					ii = kk;
					break;
				}
			}
			if (ii == -1)
				continue;

			(nd + ii)->q = 1;
			(nd + ii)->cntLD = 0;
			(nd + ii)->w = 0;
			(nd + ii)->ldList[0] = 0;
			//通过id号找节点
			int q = 0;
			strcpy(nd_->id, (nd + ii)->id);
			ttt = dsbob2->nd_tb->FindPh(*nd_);
			if (ttt != -1)
			{
				q = dsbob2->nd_tb->GetDataByPh(ttt)->q;
				if (q == 1024)
				{
					(nd + ii)->q |= 2;//是主节点
				}
				(nd + ii)->w = dsbob2->nd_tb->GetDataByPh(ttt)->w;
				(nd + ii)->len = dsbob2->nd_tb->GetDataByPh(ttt)->len;
				(nd + ii)->cntLD = dsbob2->nd_tb->GetDataByPh(ttt)->primebs;

				strcpy((nd + ii)->ldList, dsbob2->nd_tb->GetDataByPh(ttt)->ldList);
				strcpy((nd + ii)->UpCBID, dsbob2->nd_tb->GetDataByPh(ttt)->UpCBID);
				strcpy((nd + ii)->UnionCBID, dsbob2->nd_tb->GetDataByPh(ttt)->UnionCBID);
				(nd + ii)->v = dsbob2->nd_tb->GetDataByPh(ttt)->v;
				(nd + ii)->adeg = dsbob2->nd_tb->GetDataByPh(ttt)->adeg;
			}
		}
		delete nd_;

		//输出优化结果
		char secid[64];
		SEC* sec_ = new SEC();
		int cntSEC = dsbob2->sec_tb->GetCount();
		for (ii = 0; ii < cntSEC; ii++)
		{
			strcpy(sec_->id, (sec + ii)->id);
			ttt = dsbob2->sec_tb->FindPh(*sec_);
			if (ttt != -1)
			{
				(sec + ii)->v_b = 10 * dsbob2->sec_tb->GetDataByPh(ttt)->v_b;
				(sec + ii)->adeg_b = dsbob2->sec_tb->GetDataByPh(ttt)->adeg_b;
				(sec + ii)->v_e = 10 * dsbob2->sec_tb->GetDataByPh(ttt)->v_e;
				(sec + ii)->adeg_e = dsbob2->sec_tb->GetDataByPh(ttt)->adeg_e;

				(sec + ii)->w = dsbob2->sec_tb->GetDataByPh(ttt)->w;
				(sec + ii)->r = dsbob2->sec_tb->GetDataByPh(ttt)->r;
				(sec + ii)->i = dsbob2->sec_tb->GetDataByPh(ttt)->i / 10;
				(sec + ii)->wz = dsbob2->sec_tb->GetDataByPh(ttt)->wz;
				(sec + ii)->rz = dsbob2->sec_tb->GetDataByPh(ttt)->rz;
				(sec + ii)->iz = dsbob2->sec_tb->GetDataByPh(ttt)->iz / 10;
				strcpy((sec + ii)->sc, dsbob2->sec_tb->GetDataByPh(ttt)->opencblist);
			}
		}
		delete sec_;

		dsbob2->WriteOutfile_pf(Efile_name, 1);//输出潮流计算结果	

		delete pwrflow;
		//delete tp;
		//delete nmd;
	}

	return 0;
}
#endif

//优化串联电容器
int optCP::optSerialCP()
{
	int i,j,k,l,ret,ind,iznd,pos;
	char nd_[64],sec_[64];
	bool success = false;
	//补偿容量越小越好
	//电容器从30kVar开始到500，每次增加5kVar
	int cntND = dsbob2->nd_tb->GetCount();
	int cntSEC = dsbob2->sec_tb->GetCount();

	//首先选择需要进行优化的节点和线段，一个线段上只能有一个节点
	//1.节点上有变压器的不要
	//2.节点上有电源的不要
	std::vector<int> optnds;//需要优化的节点
	std::vector<string> optsecs;//需要优化的节点

	possible_nds.clear();
	possible_r.clear();

	optnds.clear();
	optsecs.clear();

	for (k = 0; k < cntSEC; k++)
	{
		ind = dsbob2->sec_tb->GetDataByLog(k)->ind;
		iznd = dsbob2->sec_tb->GetDataByLog(k)->iznd;

		if ((ind != -1) && (!lld->isFind(optnds, ind, pos)))
		{
			if (stay.size() > 0)//指定了节点
			{
				if (!lld->isFind(stay, dsbob2->sec_tb->GetDataByLog(k)->nd, pos))
					continue;
			}
			optnds.push_back(ind);
			optsecs.push_back(dsbob2->sec_tb->GetDataByLog(k)->id);
			continue;
		}

		if ((iznd != -1) && (!lld->isFind(optnds, iznd, pos)))
		{
			if (stay.size() > 0)//指定了节点
			{
				if (!lld->isFind(stay, dsbob2->sec_tb->GetDataByLog(k)->znd, pos))
					continue;
			}
			optnds.push_back(iznd);
			optsecs.push_back(dsbob2->sec_tb->GetDataByLog(k)->id);
		}
	}

	for (i = 1000; i < 10000; i = i + 500)
	{
		success = false;
		for (j = 0; j < optnds.size(); j++)
		{
			//线段的节点需要恢复到原来的
			for (k = 0; k < cntSEC; k++)
			{
				strcpy(dsbob2->sec_tb->GetDataByLog(k)->nd, sec_tb_t->GetDataByLog(k)->nd);
				strcpy(dsbob2->sec_tb->GetDataByLog(k)->znd, sec_tb_t->GetDataByLog(k)->znd);
			}

			//节点
			strcpy(nd_, dsbob2->nd_tb->GetDataByLog(optnds[j])->id);
			if (strcmp(nd_, "nd_serialCP") == 0)
				continue;

			//节点连接的线段，如果线段处理过，则不需要处理
			strcpy(sec_, optsecs[j].c_str());
			if ((ret = SerialCP(nd_, sec_, i)) == 0)
			{
				//可用节点
				possible_nds.push_back(nd_);
				possible_r.push_back(i);
				success = true;
			}
		}
		if (success)
			break;
	}
	if (success)
		return 1;
	else
		return 0;
}

int optCP::SerialCP(char* nd,char* secid,double r)
{
	int ret = 0;
	int ph = -1;
	int cnt_t = 0;
	int success = -1;
	double minV, maxV;
	minV= maxV = 0;
	//潮流计算
	SEC* sec1 = new SEC();
	strcpy(sec1->id, secid);
	ph = dsbob2->sec_tb->FindPh(*sec1);
	dsbob2->cp_tb->GetDataByPh(0)->r= r;
	if (strcmp(sec1->nd, nd) == 0)
	{
		strcpy(dsbob2->cp_tb->GetDataByPh(0)->nd, nd);
		strcpy(dsbob2->cp_tb->GetDataByPh(0)->znd, "nd_serialCP");
		strcpy(sec1->nd, "nd_serialCP");
	}
	else if (strcmp(sec1->znd, nd) == 0)
	{
		strcpy(dsbob2->cp_tb->GetDataByPh(0)->znd, nd);
		strcpy(dsbob2->cp_tb->GetDataByPh(0)->nd, "nd_serialCP");
		strcpy(sec1->znd, "nd_serialCP");
	}
	dsbob2->sec_tb->UpdateDataByPh(sec1, ph);
	delete sec1;

	//NMD* nmd = new NMD(dsbob2, lld);
	//if ((ret = nmd->DoValidate()) > 0)//拓扑检验，拓扑检验全部都在内存里面进行
	//{
	//	//拓扑校验不需要开关的开合数据，只需要联络开关的信息
	//	//此处的校验是静态拓扑，为设备间的物理连接关系
	//}
	//else
	//{
	//	delete nmd;
	//	return -2;
	//}
	//
	//if (!dsbob2->ReadDigitalData())
	//{
	//	//    printf("\n开关状态有误，请检查cb表和 digital表\n");
	//	delete nmd;
	//	return -3;
	//}

	////1.进行拓扑分析
	////dsbob2->bs_tb->Empty();//先将母线表清空
	//TP* tp = new TP(dsbob2, lld);
	//if (tp->tp_allnew() == TP_FAIL)
	//{
	//	delete tp;
	//	delete nmd;
	//	return -4;
	//}
#ifdef _WIN32
	HINSTANCE hDLL = ::LoadLibrary("C:\\DNASP\\DNASPAPP\\DLL\\x64\\Debug\\DLLso.dll");
	if (hDLL != NULL)
	{
		dllCall InitMath = (dllCall)GetProcAddress(hDLL, "InitMath");
		if (InitMath != NULL)
		{
			ret = InitMath(dsbob2, lld);
			FreeLibrary(hDLL);
		}
	}
#else __linux__
	ret = InitMath(dsbob2, lld);
#endif

	if ((ret = dsbob2->ReadAnalogData()) > 0)
	{
		//printf("\n遥测量有误，请检查so表和analog表\n");
		//AfxMessageBox("遥测量有误，请检查so表和analog表");
		//delete tp;
		//delete nmd;
//		dsbob2->deletemem();
		return -5;
	}

	PWRFLOW* pwrflow = new PWRFLOW(dsbob2, lld);
	pwrflow->g_method = 1;//因为有了分布式电源，前推回推方法计算潮流改为牛顿法
	//使用潮流计算的方法，在获取电源点的时候决定，如果多机，牛顿法，单机，前推回代
	if (pwrflow->ExecutePwrflow() < 0)
	{
		//printf("\a error in power flow calculation\n");
		minV = pwrflow->minVoltage;
		maxV = pwrflow->maxVoltage;

		delete pwrflow;
		//delete tp;
		//delete nmd;
		return -6;
	}

	//判断节点电压是否符合要求，不能大于10.7，不能小于9.0
	 minV = pwrflow->minVoltage;
	 maxV = pwrflow->maxVoltage;
	if ((pwrflow->maxVoltage > 1.07) || (pwrflow->minVoltage < 0.93))
	{
		delete pwrflow;
		//delete tp;
		//delete nmd;
		return -1;
	}

	delete pwrflow;	
	//delete tp;
	//delete nmd;

	return 0;
}

//并联电容器补偿计算
int optCP::ParallelCP()
{
	int ret = 0;
	int ph = -1;
	int cnt_t = 0;
	int success = -1;
	double minV=0;
	double maxV=0;
	if (OnCalFlow(minV,maxV) < 0)
		return -1;//潮流不收敛

	//判断节点电压是否符合要求，不能大于10.7，不能小于9.0
	success = isValidate();

	//	int success = -1;
	if (success == 0)
		return 0;
	else
		return -1;//电压不合格
}


//直接在节点上增加无功出力
int optCP::OnCalFlow(double& minV,double& maxV)
{
	int i;
	int ph = -1;
	int cnt_t = 0;
	int success = -3;
	minV = maxV = 0;
	PWRFLOW* pwrflow = new PWRFLOW(dsbob2, lld);
	//pwrflow->g_method = 1;//因为有了分布式电源，前推回推方法计算潮流改为牛顿法
	if (cpbss.size() > 0)
	{
		pwrflow->cpbss.clear();
		pwrflow->cps.clear();
		for (i = 0;i < cpbss.size();i++)
		{
			pwrflow->cpbss.push_back(cpbss.at(i));
			pwrflow->cps.push_back(cps.at(i));
		}
	}

	if (pwrflow->ExecutePwrflow() < 0)
	{
		minV = pwrflow->minVoltage;
		maxV = pwrflow->maxVoltage;
		delete pwrflow;
		return -1;
	}

	minV = pwrflow->minVoltage;
	maxV = pwrflow->maxVoltage;
	if ((pwrflow->maxVoltage > 1.07)|| (pwrflow->minVoltage < 0.93))
	{
		delete pwrflow;
		return -2;
	}

	delete pwrflow;

	return 0;
}

int optCP::optResult()
{
	ofstream outfile("C:\\DNASP\\DNASP64\\trunk\\Bin\\x86\\optResult.txt", ios::out);
	outfile << "电容器优化结果" << endl;
	//outfile << "<busv::pf name=" << "\"母线电压\"" << ">" << endl;
	outfile << "节点编号" << "	电容器容量（kVar）" << endl;
	if (possible_nds.size() > 0)
	{
		for (int ii = 0;ii < possible_nds.size();ii++)
			outfile << possible_nds.at(ii) << "  " << possible_r.at(ii) << endl;
	}

	outfile.close();


	return 1;
}

double optCP::random()   //产生一个0～1之间的随机数
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
double optCP::randf(double low, double high)  
{
	double x=random();
	double i=x*(high-low)+low;
	return(i);
}

//产生一个[low,high]之间的随机整数
int optCP::randi(int low,int high)
{
	int i=high-low+1;
    int k= rand()%i+low;
	return(k);
}

void optCP::initGA()//相当于构造函数
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

	for(int i=0;i<200;i++)//染色体的上下限值
	{
		psel[i]=0;
		vlow[i]=0;
		vhigh[i]=0;
		ilow[i]=0;
		ihigh[i]=0;
	}
}

//
//为全局变量分配空间
void optCP::initmalloc()
{
	int i, j;

	//染色体的长度需要计算
	int lv_nd = dsbob2->nd_tb->GetCount();
	chromsize = 6;//串联电容器只能是3个点
	popsize = 200;

	//分配给当前代和新一代种群内存空间
    if((oldpop=new struct individual[popsize])==NULL)
		return;
	if((newpop=new struct individual[popsize])==NULL)
		return;

	//分配给染色体内存空间，最后两个是时户数和用户数等可靠性指标，可以累加计算可靠性
	for(j=0;j<popsize;j++)
	{
		if((oldpop[j].chrom=new int[chromsize])==NULL)
			return;
	    if((newpop[j].chrom=new int[chromsize])==NULL)
			return;
	}

	if((bestfit.chrom=new int[chromsize])==NULL)
		return;
	if((bestthick.chrom=new int[chromsize])==NULL)
		return;

	//给变量的上下限分配内存空间
 //   if((vlow=new double[fn])==NULL)
	//		nomemory("vlow");
	//if((vhigh=new double[fn])==NULL)
	//		nomemory("vhigh");
 //   if((ilow=new int[in])==NULL)
	//		nomemory("ilow");
	//if((ihigh=new int[in])==NULL)
	//		nomemory("ihigh");

	//if ((psel = new double[popsize]) == NULL)
	//		nomemory("psel");

}

void optCP::freeGA()//析构函数
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

int optCP::getValidgen(individual *pop)
{
	int i, j, k;
	j = 0;
	i = -1;
	while (true)
	{
		if (j++ > 100)
			break;

		i = randi(0, chromsize);
		if ((i == 0) || (i == 1) || (i == chromsize - 2) || (i == chromsize - 1) )
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
int optCP::editChrome(individual *pop)
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
	k = pic;
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
int optCP::isValidChrome(individual *pop,int type)
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

				valid++;
			}
		}

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
int optCP::isNew(individual *pop,int size )
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
void optCP::initpop()
{	
	int i,j,k,m,n,iFind,valid,l;
	int chrom;

	//染色体初始化，每个个体的染色体都不一样
	int* pic = new int[3];
	int ii = 0;
	int jj = 0;
	int lenPic, retNum, ret;
	lenPic = 3;
	retNum = popsize;
	int lv_nd = dsbob2->nd_tb->GetCount();
	for (i = 0;i < 3;i++)
	{
		ilow[i] = 0;
		ihigh[i] = lv_nd-1;
	}
	for (i = 3;i < 6;i++)
	{
		ilow[i] = 20;
		ihigh[i] = 2000;
	}

	int** intlist = NULL;
	intlist = (int**)calloc(retNum, sizeof(int*));
	for (ii = 0; ii < retNum; ++ii)
	{
		intlist[ii] = (int*)calloc(lenPic, sizeof(int));
	}
	for (ii = 0; ii < retNum; ++ii)
		for (jj = 0; jj < lenPic; ++jj)
			intlist[ii][jj] = -1;

	ret = lld->OnGetRandom(lv_nd-1, lenPic, intlist, retNum);

	for (j = 0; j < popsize;j++)
	{
		if (stay.size()>0)
		{ 
			for (i = 0;i < 3;i++)
			{
				if (i<stay.size())
					oldpop[j].chrom[i] = atoi(stay.at(i).c_str());
				else
					oldpop[j].chrom[i] = -1;
			}
			for (i = 3;i < 6;i++)
				oldpop[j].chrom[i] = randi(20, 2000);
		}
		else
		{
			for (i = 0;i < 3;i++)
				oldpop[j].chrom[i] = *(*(intlist + j) + i);
			for (i = 3;i < 6;i++)
				oldpop[j].chrom[i] = randi(20, 2000);
		}
		
		minfunction(&oldpop[j]);//计算个体的适应度，放到个体的染色体中
	}

	delete[] pic;
	for (ii = 0; ii < retNum; ++ii)
	{
		if (NULL != intlist[ii])
			free(intlist[ii]);
	}
	free(intlist);
	intlist = 0;

	o << "          原始值 " << endl;//输出适应度，最小值特征：－
	o << " ========================== " << endl;//输出适应度，最小值特征：－
	for (j = 0; j<popsize;j++)//输出随机种群
	{
		o << j + 1 << " ";
		for (i = 0;i<6;i++)
		{
			o << oldpop[j].chrom[i] << " ";//输出随机变量
		}
		o << oldpop[j].fitness << endl;//输出适应度，最小值特征：－
	}
}

//形成目标函数,每条线100个计算结果
void optCP::minfunction(individual * critter)
{
	int i, j;
	int ret = 0;
	int ret1 = 0;
	int success = 0;
	int chrom;
	char nd_[64];
	double fitness = 0;
	double minV = 10000;
	double maxV = 0;

	int lv_nd = dsbob2->nd_tb->GetCount();
	////把染色体拆开
	cps.clear();
	cpbss.clear();
	for (i = 0;i < 3;i++)
	{
		chrom = critter->chrom[i];
		if (chrom == -1)
		{//不需要使用的染色体

		}
		else
		{
			strcpy(nd_, dsbob2->nd_tb->GetDataByLog(chrom)->id);
			ND* nd1 = new ND();
			strcpy(nd1->id, nd_);
			dsbob2->nd_tb->FindPh(*nd1);
			if (nd1->ibs == -1)
			{
				cps.clear();
				cpbss.clear();
				fitness = 0;
				ret1 = -3;
				break;
			}
			cpbss.push_back(nd1->ibs);
			delete nd1;
			cps.push_back(critter->chrom[i + 3]);
			fitness += critter->chrom[i + 3];
		}
	}

	if (ret1 ==  -3)
	{ 
		cps.clear();
		cpbss.clear();
		fitness += 2000;//无解
	}
	else
	{
		ret = OnCalFlow(minV,maxV);
		if (ret == 0)
		{//判断节点电压是否符合要求，不能大于10.7，不能小于9.0
			//success = isValidate();
			//if (success == 1)
			//{
				fitness += 0;
			//}
			//else
			//{
			//	fitness += 2000;
			//}
		}
		else
		if (ret == -2)
			fitness += 4000;
		else
			fitness += 5000;//潮流不收敛
	}
	critter->fitness = fitness;
	critter->thick = ret;
}

//计算种群统计数据
void optCP::statistics(individual *pop)
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
		if ( ((bestfit.fitness-pop[k].fitness)>9.99e-8)//求最小值特征
			&& (pop[k].thick==0) )
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
void optCP::pselect(double q)
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
void optCP::qksortlittle(individual *pop,int left,int right)
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
		//while((pop[l].fitness+k*pop[l].thick)<pivot) ++l;//从小到大排序特征
		//while((pop[r].fitness+k*pop[r].thick)>pivot) --r;//从小到大排序特征

		while ((pop[l].fitness) < pivot) ++l;//从小到大排序特征
		while ((pop[r].fitness) > pivot) --r;//从小到大排序特征

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
void optCP::safebest()
{
	int s=1;//s为判据
	for(int i=0;i<popsize;i++)
	{  //如果新一代中有上一代的最优解，则最优解不用再保留
		if ( (newpop[i].fitness==bestfit.fitness)
			&& (newpop[i].thick == 0) )//解要可行
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

void optCP::make_ga()//遗传算法操作
{
	gen=1;
	//最佳个体保持20代或遗传代数大于最大代数时，结束迭代计算
	while((gen<=maxgen))
	{
		//产生新一代
		generation();
		
		//THICK(0);//计算这一代的个体浓度
//		qksortbig(newpop,0,popsize-1);//按个体浓度从大到小排序
		qksortlittle(newpop,0,popsize-1);
		safebest();//保留最优解
		//qksortlittle(newpop,0,popsize-1);
		//计算新一代种群的适应度统计数据
		statistics(newpop);
		//输出新一代统计数据
		genresult();

		//o2<<gen<<"  "<<bestfit.fitness<<endl;
		temp=oldpop;
		oldpop=newpop;
		newpop=temp;
		gen++;
	}
}

//输出种群统计结果
void optCP::genresult()
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
void optCP::generation()
{
	int i,mate1,mate2,j=0,k,l;
	
  /* 选择, 交叉, 变异 */
	while(j<=popsize-2)
	{
		l = 0;
		while (true)
		{
			if (l++ > 2000)
				break;
			/* 挑选交叉配对 */
			mate1 = select();
			while (mate1 == -1)
				mate1 = select();

			mate2 = select();
			while ((mate1 == mate2)||(mate2 == -1))
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
			//if (isValidChrome(&newpop[j], 0) == 0)
			//	continue;
			//if (isValidChrome(&newpop[j + 1], 0) == 0)
			//	continue;
			break;
		}

		//if (l==101)
		//{
		//	for (i = 0;i < chromsize;i++)
		//	{
		//		newpop[j].chrom[i] = oldpop[mate1].chrom[i];
		//	}
		//	for (i = 0;i < chromsize;i++)
		//	{
		//		newpop[j + 1].chrom[i] = oldpop[mate2].chrom[i];
		//	}
		//}

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
int optCP::select()
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
int optCP::flip(double prob)
{
	double p;
	p=random();//每次调用都产生不同的随机数
	if(p<=prob)
		return(1);
	else
		return(0);
}

//由两个父体交叉产生两个子体
void optCP::crossover(int *parent1,int *parent2,
			      int *child1,int *child2)
{
	double jcross;
	if(flip(pcross))
	{
		ncross++;
		for(int i=0;i<chromsize;i++)
		{
			jcross=randf(-0.25,1.25);
			child1[i]=parent1[i]*jcross+parent2[i]*(1-jcross);
			if(child1[i]<ilow[i]) child1[i]=ilow[i];
			if(child1[i]>ihigh[i]) child1[i]=ihigh[i];
			jcross=randf(-0.25,1.25);
			child2[i]=parent2[i]*jcross+parent1[i]*(1-jcross);
			if(child2[i]<ilow[i]) child2[i]=ilow[i];
			if(child2[i]>ihigh[i]) child2[i]=ihigh[i];
		}

//		for(int i1=fn;i1<fn+in;i1++)
//		{
//			jcross=optCPndf(-0.25,1.25);
//			child1[i1]=parent1[i1]*jcross+parent2[i1]*(1-jcross);
//			child1[i1]=int(child1[i1]+0.5);
//			if(child1[i1]<ilow[i1-fn]) child1[i1]=ilow[i1-fn];
//			if(child1[i1]>ihigh[i1-fn]) child1[i1]=ihigh[i1-fn];
//			jcross=optCPndf(-0.25,1.25);
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
void optCP::mutation(int *child)
//void optCP::mutation(individual *child)
{
	if(flip(pmutation))
	{
		int k,k1,j,old;
		j=chromsize-1;
		k=randi(0,j);
		//k1 = getValidgen(child);
		if(k<chromsize)
		{
			child[k] = randi(ilow[k],ihigh[k]);
		}
//		else
//		{
//			child[k]=optCPndi(ilow[k-fn],ihigh[k-fn]);
//		}
		nmutation++;
	}
}


//计算新抗体浓度
void optCP::THICK(double T)
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
//	int optCPte=50;//电压增量的权系数
//	for(int i1=1;i1<popsize;i1++)//计算抗体之间的距离和结合力
//	{
//		for(int j1=i1+1;j1<=popsize;j1++)
//		{
//			double DV=0.0;
//			for(int k1=0;k1<N1;k1++)
//			{
//				DV+=pow(optCPte*(newpop[i1-1].chrom[k1]-newpop[j1-1].chrom[k1]),2);
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
void optCP::addrandpop(double r)//r为淘汰率
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
//			newpop[i].chrom[j1]=optCPndi(ilow[j1-fn],ihigh[j1-fn]);
//		}

		minfunction(&newpop[i]);/*计算初始适应度，
	                        这里&(oldpop[i])是取地址传给形参*/	
	}
}

//快速排序法，个体浓度从大到小排列
void optCP::qksortbig(individual *pop,int left,int right)
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
		if(r!=pivot) ++l;
	}
	if(l==r) l++;
	if(left<r) qksortbig(pop,left,l-1);
	if(l<right) qksortbig(pop,r+1,right);
}

//用遗传算法进行优化计算,只能解决并联电容器的问题，串联直接循环即可
int optCP::optCalc(int type)
{
	//并联两个及以上的电容器才可以启动遗传算法
	o.open("output_ga.txt");
	//int i, chrom;
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

	o.close();

	//优化结果通过参数返回
	int i, ret;
	int chrom;
	char nd_[64];
	////把染色体拆开
	cps.clear();
	cpbss.clear();
	possible_nds.clear();
	possible_r.clear();
	for (i = 0;i < 3;i++)
	{
		chrom = bestfit.chrom[i];
		strcpy(nd_, dsbob2->nd_tb->GetDataByPh(chrom)->id);
		cpbss.push_back(dsbob2->OnGetBsByND(nd_));
		cps.push_back(bestfit.chrom[i + 3]);

		possible_nds.push_back(nd_);
		possible_r.push_back(bestfit.chrom[i + 3]);

	}
	double minV = 0;
	double maxV = 0;
	ret = OnCalFlow(minV,maxV);
	//ret = isValidate();

	//3.结束
	freeGA();

	return 1;
}
//得到不同的随机数
void optCP::getRandom(int** cuts)
{
	//int lenPic, retNum;
	//lenPic = 3;
	//retNum = 100;
	//int i, j,n,m;

//	lld->OnGetRandom(chromsize, 3, cuts, popsize);

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
