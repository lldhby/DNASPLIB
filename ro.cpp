// RO.cpp: implementation of the CRO class.
//
//////////////////////////////////////////////////////////////////////
#include "../DNASPLIB/ro.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double CRO::getCosfi(double w,double r)//求功率因素
{
	double a,b;
	a=fabs(w);
	b=fabs(r);
	
	return a/sqrt(a*a+b*b);
}

CRO::CRO(DSBOB2* m_pDb, CLLD* m_pLLD) : PWRFLOW(m_pDb, m_pLLD)
{
	this->dsbob2 = m_pDb;
	lld = m_pLLD;
	g_rdi	    = new G_Rdi[mx_rdi];
	lv_rdi		= 0;

//	g_jd_t		= new G_JD_T[mx_jd];
//	lv_jd_t		= 0;
//	g_ju_t		= new G_JU_T[mx_ju];
//	lv_ju_t		= 0;
//	g_jl_t		= new G_JL_T[mx_jl];
//	lv_jl_t		= 0;

	alpha = 0;/*折旧维修率*/
	garma = 0;/*投资回收率*/
	belta = 0;/*电价*/
	Kc = 0;/*单位容量补偿设备投资*/
	tiao = 0;/*全网最大负荷损耗小时数*/
}

CRO::~CRO()
{
//	delete[] g_jd_t;
//	delete[] g_ju_t;
//	delete[] g_jl_t;

	delete[] g_rdi;
}

//将无功优化的一次、二次矩进行重置
void CRO::InitWorkingArray_1(int type)
{
	int i,j;

	for(i = 0; i < mx_rdi; i++)
	{
		g_rdi[i].ibs = 0;
		g_rdi[i].ibsp = 0;
		g_rdi[i].ibr = 0;
		g_rdi[i].dr = 0;
		g_rdi[i].v = 0;
		g_rdi[i].r = 0;
		g_rdi[i].linearRdi = 0;
		g_rdi[i].quanRdi = 0;
		g_rdi[i].quanQ = 0;

		if (type==0)
		{
		}
		else
		{
			g_rdi[i].b_cosfi = 0;
			g_rdi[i].b_v = -1;//补偿前电压，输出时用这个来进行判断
		}
		g_rdi[i].a_cosfi = 0;
		g_rdi[i].a_v = 0;
		g_rdi[i].Qv = 0;//补偿容量

		//下游元素初始化
		for(j = 0; j < MXELE; j++)
		{
			g_rdi[i].downstream[j] = -1;
		}
		g_rdi[i].n_downstream = 0;
	}
	lv_rdi = 0;
//		//对角
//		for(i = 0; i < mx_jd; i++)
//		{
//			g_jd_t[i].H = 0.0;
//			g_jd_t[i].N = 0.0;
//			g_jd_t[i].J = 0.0;
//			g_jd_t[i].L = 0.0;
//		}
//	
//		//上三角
//		for(i = 0; i < mx_ju; i++)
//		{
//			g_ju_t[i].j = 0;	//在矩阵中的列的位置
//			g_ju_t[i].d = 0;	//
//			g_ju_t[i].k = 0;	//
//			g_ju_t[i].H = 0.0;
//			g_ju_t[i].N = 0.0;
//			g_ju_t[i].J = 0.0;
//			g_ju_t[i].L = 0.0;
//		}
//
//		//下三角,雅可比矩阵不是对称阵
//		for(i = 0; i < mx_jl; i++)
//		{
//			g_jl_t[i].j = -1;
//			g_jl_t[i].d = 0;
//			g_jl_t[i].k = 0;
//			g_jl_t[i].H = 0.0;
//			g_jl_t[i].N = 0.0;
//			g_jl_t[i].J = 0.0;
//			g_jl_t[i].L = 0.0;
//		}
//	}
}

//求取中枢点，中枢点是一个序列
int CRO::getPivot(int* pivot)
{
	int i,j,k,t,t1;
	int n_path,n_step;
	int path[MAX_MINPATH_LEN][MAX_LOW_VOLTAGE_LEN];//共有100个节点低电压，最小路MAX_MINPATH_LEN个节点
	int path_t[MAX_MINPATH_LEN],vol_t[MAX_MINPATH_LEN];

    for(i = 0; i < MAX_MINPATH_LEN; i++)
    {
		pivot[i] = -1;
	    for(j = 0; j < MAX_LOW_VOLTAGE_LEN; j++)
			path[i][j] = -1;			
	}

	//将原来写入的中枢点清掉
    for(i = 0; i < lv_wb; i++)
    {
		//找到本节点的上级和所有下级节点，其中上级节点是已知的，下级节点需要循环
   		g_rdi[i].pivot = -1;
    }

	//寻找不合格电压
	n_path=0;
    for(i = 1; i < lv_wb; i++)
    {
		//只需要考虑负荷点的低电压
		if (g_rdi[i].n_downstream > 0)
			continue;

		//找到本节点的上级和所有下级节点，其中上级节点是已知的，下级节点需要循环
		if (g_rdi[i].v < 0.93)
		{
			for(j = 0; j < MAX_MINPATH_LEN; j++)
			{
				path_t[j] = -1; 
				vol_t[j] = -1;
			}

			n_step = 0;
			//查找到电源点的最小路
			k = g_rdi[i].ibs;//本节点
			while(1)
			{
				if (k==0) 
					break;

				path_t[n_step++] = k;//将节点号写入到数组

				if (k==g_rdi[k].ibsp)
					break;
				else
					k = g_rdi[k].ibsp;
			}

			lld->converse_vector(path_t,n_step+1,vol_t,-1);
			for(j = 0; j < MAX_MINPATH_LEN; j++)
			{
				path[j][n_path] = vol_t[j]; 
			}
	
			n_path++;//找下一条
		}
    }

	if ( n_path==0 )//电压全合格
		return -1;

	if ( n_path==1 )//只有一个不合格
	{
		for(i = 0; i < n_step; i++)
		{
			pivot[i] = path[i][0];//前面所有的都属于中枢点
		}
		
		return 1;
	}

	//通过最小路找交点
	//从电源点开始搜索，一旦发现有一个点不在任意一个的最小路上，说明已经到了分叉点

	//转置矩阵
    for(i = 0; i < MAX_MINPATH_LEN; i++)//最小路里面的一个元素，从根开始
    {
		t = path[i][0];
	    for(j = 0; j < MAX_LOW_VOLTAGE_LEN; j++)//循环所有的最小路
		{
			t1 = path[i][j];
			if (t1<0)//有最小路一开始就是空
				break;

			if (t != t1)//出现不一致的情况，说明是顶端的点
			{
				for(k = 0; k < i; k++)
				{
					pivot[k] = path[k][0];//前一个即可
				}
				return i;
			}
		}
    }

	return -1;//没有中枢点
}

//每次做潮流计算的时候都需要形成新的雅可比矩阵，所以不需要保留原有的内容
// H N
// J L
//1.对角线上的N,J互换
//2.雅可比矩阵的导纳阵j，k互换，其中的N，J互换
//网损微增率=P/Q / (1-Q/Q)
//每一行的H 和N进行累加，得到P/Q，J 和L累加得到Q/Q 
int CRO::getTmattrice()//得到雅可比矩阵的转置矩阵
{
	int i,ibs,zbs,k,k1,k2;
	double H,N,J,L,dw,dr;

	//雅可比矩阵jd[],ju[],jl[]，ju[]和jl[]需要重新生成
	for (i=0; i<mx_jd; i++)
	{
//		g_jd_t[i].H = g_jd[i].H;
//		g_jd_t[i].N = g_jd[i].N;
//		g_jd_t[i].J = g_jd[i].J;
//		g_jd_t[i].L = g_jd[i].L;

		//N,J互换
		N = g_jd[i].N;
		g_jd[i].N = g_jd[i].J;
		g_jd[i].J = N;
		
	}

	for (i=0; i<mx_wb; i++)
	{
		//b项清零，便于解矩阵方程
		g_wb[i].dw = 0;
		g_wb[i].dr = 0;
	}

//	for (i=0; i<mx_ju; i++)
//	{
//		g_ju_t[i].j = g_ju[i].j;	//在矩阵中的列的位置
//		g_ju_t[i].d = g_ju[i].d;	//
//		g_ju_t[i].k = g_ju[i].k;	//
//		g_ju_t[i].H = g_ju[i].H;
//		g_ju_t[i].N = g_ju[i].N;
//		g_ju_t[i].J = g_ju[i].J;
//		g_ju_t[i].L = g_ju[i].L;
//	}
//
//	for (i=0; i<mx_jl; i++)
//	{
//		g_jl_t[i].j = g_jl[i].j;	//在矩阵中的列的位置
//		g_jl_t[i].d = g_jl[i].d;	//
//		g_jl_t[i].k = g_jl[i].k;	//
//		g_jl_t[i].H = g_jl[i].H;
//		g_jl_t[i].N = g_jl[i].N;
//		g_jl_t[i].J = g_jl[i].J;
//		g_jl_t[i].L = g_jl[i].L;
//	}

	//只需要将H N J L换一下就可以了
	//在这里要直接计算支路扰动的行的H，N的累加值

	dw = dr = 0;
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;//在矩阵中的行号
        zbs = g_wbr[i].zbs;

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		//本节点和其它节点邻接在上三角中的位置（列）
		//此处的i是支路编号，这个对应了本支路的对上三角的扰动位置
        k1 = g_ju[i].k; //节点所连接的支路编号，也就是在上三角中的角标
        H = g_ju[k1].H;
        N = g_ju[k1].N;
        J = g_ju[k1].J;
        L = g_ju[k1].L;
		//装置之前的H,N进行累加的b项
		g_wb[ibs].dw += H;
		g_wb[ibs].dr += N;

        k2 = g_jl[i].k;//在下三角中的行号
		g_wb[zbs].dw += g_jl[k2].H;
		g_wb[zbs].dr += g_jl[k2].N;
        
		g_ju[k1].H = g_jl[k2].H;
        g_ju[k1].J = g_jl[k2].N;//N,J互换
        g_ju[k1].N = g_jl[k2].J;
        g_ju[k1].L = g_jl[k2].L;

        g_jl[k2].H = H;
        g_jl[k2].J = N;
        g_jl[k2].N = J;
        g_jl[k2].L = L;
	}

	return 1;
}

//DEL int CRO::reTmattrice()//得到雅可比矩阵的转置矩阵
//DEL {
//DEL //	int i;
//DEL //
//DEL ////退出前要恢复原样
//DEL //	for (i=0; i<mx_ju; i++)
//DEL //	{
//DEL //		g_ju[i].j = g_ju_t[i].j;	//在矩阵中的列的位置
//DEL //		g_ju[i].d = g_ju_t[i].d;	//
//DEL //		g_ju[i].k = g_ju_t[i].k;	//
//DEL //		g_ju[i].H = g_ju_t[i].H;
//DEL //		g_ju[i].N = g_ju_t[i].N;
//DEL //		g_ju[i].J = g_ju_t[i].J;
//DEL //		g_ju[i].L = g_ju_t[i].L;
//DEL //	}
//DEL //
//DEL //	for (i=0; i<mx_jl; i++)
//DEL //	{
//DEL //		g_jl[i].j = g_jl_t[i].j;	//在矩阵中的列的位置
//DEL //		g_jl[i].d = g_jl_t[i].d;	//
//DEL //		g_jl[i].k = g_jl_t[i].k;	//
//DEL //		g_jl[i].H = g_jl_t[i].H;
//DEL //		g_jl[i].N = g_jl_t[i].N;
//DEL //		g_jl[i].J = g_jl_t[i].J;
//DEL //		g_jl[i].L = g_jl_t[i].L;
//DEL //	}
//DEL 
//DEL 	return 1;
//DEL }


int CRO::getArrayQ()//得到无功网损微增率，并排序
{
	int i;

    GaussElimination();
    SolveEquation();
	
	//不需要进行迭代，直接出结果，隔一个取一个出来即可
    for(i = 0; i < lv_wb; i++)
    {
        g_Q_inc[i]= g_wb[i].dv;//每个节点的电压幅值
    }

	//排序
	lld->sortDouble_Desc(g_Q_inc,lv_wb);

	return 1;
}

double CRO::getEconomy()//计算经济当量
{
	//（折旧维修率+投资回收率）* 单位容量补偿设备投资（元/Kvar）/ 
	// 单位电能损耗价格（元/kW.h）* 全网最大负荷损耗小时数
	
	//单位容量补偿设备投资:100元/kVar;电费：0.5元/kW.h；全网最大负荷损耗小时数：2000
	

	return 1;
}

//此无功优化只适应于10kV辐射状配电网
//无功优化只能是在电压合格的情况下进行无功优化，否则就是舍本求末
int CRO::OnExeRO()
{
	int	ret,i, lv_island;
	// clock_t start, finish;

	char so_id[80];
	char out_error[200];
	int soid;

	//get island number
	lv_island = dsbob2->island_tb->GetCount();

	lld->g_cnt_ro_fd = 0;
	//execute calculations 对每一个岛进行计算
	for(g_island = 0; g_island < lv_island; g_island++)
	{
//		if ( 
//			(g_island == 7)
////			|| (g_island == 31)
////			|| (g_island == 37)
////			|| (g_island == 216)
////			|| (g_island == 356)
////			|| (g_island == 521)
////			|| (g_island == 569)
////			|| (g_island == 617)
//			)
//		{
//			int deb = 0;
//		}
//需要在此判断是否这个岛含有分布式电源

		//只对本岛内的数据进行初始化
		//initialize working arrays		1.对母线进行初始化
		InitWorkingArray();
		InitWorkingArray_1();//每做一个岛都需要进行重置无功臂

		//read network data		2.读入网络数据
		if(GetNetworkData() == PWRFLOW_FAIL)
		{
			continue;
			//return PWRFLOW_FAIL;
		}

		//////////////////////////////////////////////////////////////////////////
		//g_loopnum=1;//=======
		if (g_loopnum > 0)
		{
			//出现了环网，需要进行雅可比矩阵来做
			//1.先用牛顿法计算潮流
			if(OrderBusNumberB() == PWRFLOW_FAIL)
			{
				soid=dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->id);
				sprintf(out_error,"OrderBusNumberB error in soid %s\n",so_id);
				lld->WriteLog(lld->logfile,out_error);
				lld->add_globalexception(out_error);
				continue;
			}

			g_method = 1;//用牛顿法进行初始化//=========
			//execute Newton-Raphson method
			if(ExecuteNewton() == PWRFLOW_FAIL)
			{
				soid=dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->id);
				sprintf(out_error,"ExecuteNewton error in soid %s\n",so_id);
				lld->WriteLog(lld->logfile,out_error);
				lld->add_globalexception(out_error);
				continue;
			}
			
//			soid=dsbob2->island_tb->GetDataByLog(g_island)->refso;
//			strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->id);
//			
//			sprintf(str,"loop error in soid %s\n",so_id);
//			lld->addLogLine(str);
//			continue;
		}
		else
		{
			//电气岛确定后，无功臂也确定了
			if(OrderBusNumberA() == PWRFLOW_FAIL)
			{
				//add by lld 2011-11-02
				soid=dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->id);
				
				sprintf(out_error,"OrderBusNumberA error in soid %s\n",so_id);
				lld->WriteLog(lld->logfile,out_error);
				continue;
			}

			//OnSearchXFMR("102_200219338",str);

	//lld->TimeHere("end order data start sweep.");
			//execute backward and forward sweep	4.执行前推回代
			if(ExecuteSweep() == -1)
			{
				soid=dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->fdid);
				
				sprintf(out_error,"OrderBusNumberA error in fdid %s\n",so_id);
				lld->WriteLog(lld->logfile,out_error);

				//整条线都不能做无功优化
				dsbob2->ro_result[lld->g_cnt_ro_fd].id=lld->g_cnt_ro_fd;
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].ndid,"潮流不收敛");
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].fdid,so_id);

				dsbob2->ro_result[lld->g_cnt_ro_fd].Qv = -1;
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].b_cosfi , "-1");
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].a_cosfi , "-1");
				dsbob2->ro_result[lld->g_cnt_ro_fd].b_v = 0;
				dsbob2->ro_result[lld->g_cnt_ro_fd].a_v = 0;
				lld->g_cnt_ro_fd++;

				continue;
			}
		}
//lld->TimeHere("end prep data start order.");
		//order bus number by BFS	3.通过广度搜索进行母线编号优化
//lld->TimeHere("end sweep.");
		//write result into database，具体针对一个计算岛

//增加无功优化模块，可能需要反复计算潮流
		//计算无功二次精确矩，对每一个母线进行计算
		//二次精确矩决定补偿地点，一次精确矩决定补偿容量

		//1.分析本条线路是否需要进行无功补偿，如果需要的话，才进行下一步的分析，否则直接跳过
			//启动无功优化程序，对本馈线进行无功优化，需要反复计算潮流
			//对当前的电源点（单个的电气岛）进行优化补偿，得出补偿结论。
	//0-分析电压，如果电压点太低，其前面的节点无法支撑，直接退出
		if ( (ret = isVoltageViolate(1)) < 0 )//完全无需补偿
		{
//			g_wloss_b = g_wloss; 
//			g_rloss_b = g_rloss;

			//将以前的电压记录下来进行对比
			for(i = 0; i < lv_wb; i++)
			{
				if (g_wb[i].v > 0)
				{
					g_rdi[i].b_v = g_wb[i].v;
				}
			}		

			isNeedRPO();

		}
	}
	return PWRFLOW_SUCCEED;
}

//判断补偿后潮流二次计算得到的结果，如果还有越限
//0 补偿后没有越限；-1 补偿后电压越高限；-2 电压越低限；-3 高低限都越
int CRO::isVoltageViolate(int oragin)
{
	int i,ibs;
	int ret=0;
	int l_flag,h_flag;

	l_flag = h_flag = 0;
	//计算补偿点的电压等值，便于前后对比,补偿前的状态
	//所有不合格节点全部显示出来+7%， -10%
	for(i = 0; i < lv_wb; i++)
	{
		ibs = g_wb[i].tio;
		if ( ibs >= 0 ) //补偿后的情况  
		{
			//需要判断电压是否越限
			//g_wb[i].b_v = g_wb[i].v;//补偿前的电压
			if ( g_wb[i].v < 0.93 )
			{
				if (oragin)
					g_rdi[i].b_v  = g_wb[i].v;
				l_flag = -1;
			}

			if ( g_wb[i].v > 1.07 )
			{
				if (oragin)
					g_rdi[i].b_v  = g_wb[i].v;
				h_flag = -1;
			}
		}
	}

	//一般情况下判断
	
	//全部都是越低限，没有越高限的情况出现
	if ((l_flag<0) && (h_flag==0))
		ret = -1;

	//只越高限,没有越低限的情况
	if ((l_flag==0) && (h_flag<0))
		ret = -2;
	
	//高低线都不满足，既越低限，又越高限
	if ((l_flag<0) && (h_flag<0))
		ret = -3;

	return ret;
}

//是否需要进行无功补偿
//0-无需补偿；-1 电压太低，补偿无效；-2 补偿到位； -3 补偿不到位
int CRO::isNeedRPO()
{	
	//1.第一阶段，对末端的功率因素要补偿到位，0.95
	if (firstRPO() == 1)
		return 1;

	if (secondRPO() == 1)
		return 1;
	else
		putRPO(3);//即使无解，也要输出二阶段的优化结果

//	g_wloss_a = g_wloss;
//	g_rloss_a = g_rloss; 

	return 1;
}

//第一阶段补偿
//先进行第一阶段的优化，如果配变的功率因素低于0.9，需要将其补到0.95的水平
//只有低压侧补偿到位，末端无功平衡了才能进行杆上的补偿
//0 第一阶段没有成功，需要第二节点，1 成功 
int CRO::firstRPO()
{
	int i,ret,cos_flag,v_flag;//是否存在需要变压器补偿和低电压情况
	double w,r,cosfi,Qv,v_low;

	ret=cos_flag=v_flag=0;
	int ibs;
	LD* ld = dsbob2->ld_tb->GetTable();
    for(i = 0; i < lv_wld; i++)
    {
        w = g_wld[i].w;
        r = g_wld[i].r;

		ibs = g_wld[i].ibs;//负荷点的优化后编号
		
		v_low = g_wb[ibs].v;
		cosfi = w / sqrt(w*w + r*r);
		if ( (cosfi < 0.9) )
		{
			//进行第一阶段的补偿
			//直接在变压器侧发无功，不能补偿太多，只能到0.95
			Qv = w * (sqrt(1/(cosfi * cosfi) -1) - sqrt(1/(0.95 * 0.95) -1));
			g_wb[ibs].nr += Qv;//补偿容量参与下一阶段的计算	

			//******需要将这个补偿点和补偿容量记录下来
			g_rdi[ibs].Qv = Qv;//需要补偿的容量						
			g_rdi[ibs].b_cosfi =cosfi;//补偿前的功率因素						
			//g_rdi[ibs].b_v = v_low;//补偿前的电压						

			cos_flag = 1;//出现了第一阶段补偿问题
		}
    }

	//已经进行了第一阶段的补偿
	if (cos_flag > 0) 
	{
		g_ld_distr=0;//无功优化计算时不能进行负荷分配
		//1.先计算一遍潮流
		if (ExecuteSweep() == -1)//潮流不收敛
		{
			return -4;
		}
		g_ld_distr=1;//负荷分配开关打开
	
		//补偿完后计算潮流，看看有没有低电压出现
		ret = isVoltageViolate();
		if ( ret<0 )//存在电压越限的问题
		{
			ret = 0;
		}
		else//电压不越限，考虑直接输出第一阶段的补偿结果
 		{
			for(i = 0; i < lv_wld; i++)
			{
				ibs = g_wld[i].ibs;//负荷点的优化后编号

				if (g_rdi[ibs].Qv >0)//有补偿的点
				{
					g_rdi[ibs].a_cosfi = 0.95;//补偿后的功率因素						
					g_rdi[ibs].a_v = g_wb[ibs].v;//补偿后的电压						
				}
			}

			//变压器低压侧的无功补偿已经到位，同时没有电压越限的问题
			putRPO(1);//输出一阶段的优化结果
			ret = 1;
		}
	}
	
	return ret;
}

int CRO::secondRPO()
{
	int i,ret = 0;
	int site;
	double comp_Q,Q1,Q2,Q3;
//
	comp_Q=Q1=Q2=Q3=0.0;
	site=rdi_bs1=rdi_bs2=rdi_bs3=-1;

	if (g_loopnum > 0)
	{		//计算网损微增率

//		double lamanda;

		getTmattrice();//转置雅可比矩阵

		//得到网损微增率，并排序
		getArrayQ();

		//得到补偿点，最多3个

		//无功经济当量，求补偿容量
//		lamanda = getEconomy();	

	}
	else
	{
		getRdi();
		i = 0;
		while (1)
		{
			if (i==3)//最多补偿三个点，补偿完一个点后，再补第二个
				break;

			//2.准备第二阶段
			//计算每个节点的一次矩rdi_1和二次矩rdi_2

			//计算每个节点的无功损耗（矩），流入的-流出的
			getQuadQ();

			//根据电压中枢点和二次矩得到一个补偿点位置
			site = getSite();
			if (site == -1)//找不到新的点
			{
				break;
			}

			//计算补偿容量
			comp_Q = get_Q(site);
			
			if (Sweep(site,comp_Q) <0)//潮流不收敛
			{
				i++;
				continue;
			}

			if (i==0)
			{
				Q1=comp_Q;
				rdi_bs1=site;
			}
			else if(i==1)
			{
				Q2=comp_Q;
				rdi_bs2=site;
			}
			else if(i==2)
			{
				Q3=comp_Q;
				rdi_bs3=site;
			}		

			ret = isVoltageViolate();
			if (ret == 0 )			//补偿成功
			{
				break;
			}

			i++;
		}
		
		//回退,并准备输出，全部的3个点都选完后进行
		if (rdi_bs1!=-1)
		{
			g_rdi[rdi_bs1].Qv = Q1;//补偿容量
			g_wb[rdi_bs1].nr -= Q1;//节点的无功出力恢复成原状
		}
		if (rdi_bs2!=-1)
		{
			g_rdi[rdi_bs2].Qv = Q2;//补偿容量
			g_wb[rdi_bs2].nr -= Q2;//节点的无功出力恢复成原状
		}
		if (rdi_bs3!=-1)
		{
			g_rdi[rdi_bs3].Qv = Q3;//补偿容量
			g_wb[rdi_bs3].nr -= Q3;//节点的无功出力恢复成原状
		}

	//输出补偿结果
		//节点电压有问题的全部都要输出
		for(i = 0; i < lv_wb; i++)
		{
			if (g_rdi[i].b_v > 0)
			{
				g_rdi[i].a_v = g_wb[i].v;
			}
		}		

		putRPO(2);//输出二阶段的优化结果


return 1;



//		int t_rdi_bs;
//		double t_rdi,rdi1/*二次无功精确矩最大的补偿点1*/,rdi2,rdi3;
//		double Qv,rdi1_v/*补偿前电压*/,rdi2_v,rdi3_v,Qv1,Qv2,Qv3,Qv1_b,Qv2_b,Qv3_b;
//
//		//取出二次无功矩最大的三个节点
//		rdi1=rdi2=rdi3=0;
//		rdi_bs1=rdi_bs2=rdi_bs3=0;
//		for(i = 1; i < lv_wb; i++)
//		{	
//			//如果是末端，一般不主张进行补偿
//
//
//			//选出3个最大的无功负荷矩
//			//比1还大
//			if (g_rdi[i].quanQ > rdi1)
//			{
//				//出来比最大的还大
//				//1.把最大的给临时
//				t_rdi = g_rdi[i].quanQ;//最大的无功二次精确矩
//				t_rdi_bs = i;//最大的无功二次精确矩 出现的 母线
//
//				//2.按顺序挪动
//				rdi3 = rdi2;//最大的无功二次精确矩
//				rdi_bs3 = rdi_bs2;//最大的无功二次精确矩 出现的 母线
//				
//				rdi2 = rdi1;//最大的无功二次精确矩
//				rdi_bs2 = rdi_bs1;//最大的无功二次精确矩 出现的 母线
//				
//				//3.将最大的给1
//				rdi1 = t_rdi;//最大的无功二次精确矩
//				rdi_bs1 = t_rdi_bs;//最大的无功二次精确矩 出现的 母线
//			}
//
//			//2-1之间
//			if ( (g_rdi[i].quanQ > rdi2) && (g_rdi[i].quanQ < rdi1) )
//			{
//				//1.把2-1之间的给临时
//				t_rdi = g_rdi[i].quanQ;//最大的无功二次精确矩
//				t_rdi_bs = i;//最大的无功二次精确矩 出现的 母线
//
//				//2.原来是1保留，3去掉，换成2
//				rdi3 = rdi2;//最大的无功二次精确矩
//				rdi_bs3 = rdi_bs2;//最大的无功二次精确矩 出现的 母线
//
//				//3.将2-1之间的给2
//				rdi2 = t_rdi;//最大的无功二次精确矩
//				rdi_bs2 = t_rdi_bs;//最大的无功二次精确矩 出现的 母线
//			}
//
//			//3-2之间
//			if ( (g_rdi[i].quanQ > rdi3) && (g_rdi[i].quanQ < rdi2) )
//			{
//				rdi3 = g_rdi[i].quanQ;//第三大的无功二次精确矩
//				rdi_bs3 = i;
//			}
//		}
//
//		//准备对无功负荷矩进行补偿，用一次无功负荷矩来处理补偿容量
//		int k=0;
//		Qv1 = Qv2 = Qv3 = Qv1_b = Qv2_b = Qv3_b = 0;
//		while (1)
//		{
//			if ( (rdi1>0) && (rdi_bs1 > -1) )
//			{
//				Qv1_b = Qv1;//将前一个保留下来，准备越高限时回退
//				ret = setCP(rdi_bs1,Qv1,k);
//
//				if (ret == 1)//完全合格
//				{
//					g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//					break;
//				}
//
//				if ( (ret == 2) || (ret == 3) )//出现越高限的情况
//				{
//					g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//					//回退做一遍潮流
//					ret = Sweep(rdi_bs1,Qv1_b);
//					g_wb[rdi_bs1].nr -= Qv1_b;//节点的无功出力恢复成原状
//					break;
//				}
//			}
//
//			if ( (rdi2>0) && (rdi_bs2 > -1) )
//			{
//				Qv2_b = Qv2;//将前一个保留下来，准备越高限时回退
//				ret = setCP(rdi_bs2,Qv2,k);
//
//				if (ret == 1)//完全合格
//				{
//					g_wb[rdi_bs2].nr -= Qv2;//节点的无功出力恢复成原状
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//					
//					break;
//				}
//
//				if ( (ret == 2) || (ret == 3) )//出现越高限的情况
//				{
//					g_wb[rdi_bs2].nr -= Qv2;//节点的无功出力恢复成原状
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//
//					//回退做一遍潮流
//					ret = Sweep(rdi_bs2,Qv1_b);
//					g_wb[rdi_bs2].nr -= Qv1_b;//节点的无功出力恢复成原状
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//					
//					break;
//				}
//			}
//
//			if ( (rdi3>0) && (rdi_bs3 > -1) )
//			{
//				Qv3_b = Qv3;//将前一个保留下来，准备越高限时回退
//				ret = setCP(rdi_bs3,Qv3,k);
//
//				if (ret == 1)//完全合格
//				{
//					g_wb[rdi_bs3].nr -= Qv3;//节点的无功出力恢复成原状
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//					if ( (rdi2>0) && (rdi_bs2 > -1) )
//						g_wb[rdi_bs2].nr -= Qv2;//节点的无功出力恢复成原状
//					
//					break;
//				}
//
//				if ( (ret == 2) || (ret == 3) )//出现越高限的情况
//				{
//					g_wb[rdi_bs3].nr -= Qv3;//节点的无功出力恢复成原状
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//					if ( (rdi2>0) && (rdi_bs2 > -1) )
//						g_wb[rdi_bs2].nr -= Qv2;//节点的无功出力恢复成原状
//
//					//回退做一遍潮流
//					ret = Sweep(rdi_bs3,Qv3_b);
//					g_wb[rdi_bs3].nr -= Qv3_b;//节点的无功出力恢复成原状
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//					if ( (rdi2>0) && (rdi_bs2 > -1) )
//						g_wb[rdi_bs2].nr -= Qv2;//节点的无功出力恢复成原状
//					
//					break;
//				}
//			}
//
//			if ( (rdi1>0) && (rdi_bs1 > -1) )
//				g_wb[rdi_bs1].nr -= Qv1;//节点的无功出力恢复成原状
//		
//			if ( (rdi2>0) && (rdi_bs2 > -1) )
//				g_wb[rdi_bs2].nr -= Qv2;//节点的无功出力恢复成原状
//
//			if ( (rdi3>0) && (rdi_bs3 > -1) )
//				g_wb[rdi_bs3].nr -= Qv3;//节点的无功出力恢复成原状
//
//			if (k++ >100)
//				return 0;
//		}
//
	}


//输出补偿结果
	//节点电压有问题的全部都要输出
	for(i = 0; i < lv_wb; i++)
	{
		if (g_rdi[i].b_v > 0)
		{
			g_rdi[i].a_v = g_wb[i].v;
		}
	}		

	putRPO(2);//输出二阶段的优化结果

	return 1;
}

//求取补偿节点，排序最大，在中枢点上
int CRO::getSite()
{
	int i,j,k;
	int pivot[MAX_MINPATH_LEN];
	double* quan_t = new double[lv_wb-1];//二次矩
	double* quan = new double[lv_wb-1];//二次矩
	int* ibs = new int[lv_wb-1];		//节点号
	double quan_max;
	int max_ibs=-1;
	int ret_ibs=-1;

	int site;
	site = getPivot(pivot);
	
	//排序二次矩
	for(i=1; i<lv_wb; i++)//将等于0的根节点去掉
    {
		quan_t[i-1] = quan[i-1] = g_rdi[i].quanQ;
		ibs[i-1] =  g_rdi[i].ibs;
	}

	//对quan_t排序
//	lld->sortDouble(quan_t,lv_wb-1);

	lld->sortDouble_Desc(quan_t,lv_wb-1);

	for(i=0; i<lv_wb; i++)
    {
		quan_max = 	quan_t[i];//从最大的开始取
		if (quan_max <=0)//二次无功矩一定要大于0
			continue;
		
		for(j=0; j<lv_wb; j++)
		{		
			if (quan[j] == quan_max)//找到了最大的点
			{
				max_ibs = ibs[j];
				//是否在中枢点内
				for(k=0; k<lv_wb; k++)
				{		
					if (pivot[k] == max_ibs)//二次矩最大，而且又是中枢点
					{
						//如果新找到的补偿点为已有的任何点，都不行
						if (
							  (max_ibs == rdi_bs1) || 
							  (max_ibs == rdi_bs2) ||
							  (max_ibs == rdi_bs3)  )
							continue;
						else
						{
							ret_ibs = max_ibs;
							goto end0;
						}
					}
				}
			}
		}
	}

end0:


	delete []ibs;
	delete []quan;
	delete []quan_t;
	
	return ret_ibs;
}

int CRO::setCP(int rdi_bs2,double& Q,int num)
{
	int ret;
	double Qv;

	if (num==0)
		Qv = getND_DelQ(rdi_bs2);
	else
		Qv = -Q;

	if (num>0)
		Qv = 1.05 * Qv;

	if (Qv < 0)
	{
		Qv = -Qv;

		if (Sweep(rdi_bs2,Qv) <0)
			return 0;

		ret = isVoltageViolate();
		if (ret == 0 )			//补偿两个点就可以了
		{
			Q = Qv;
			return 1;
		}

		if ((ret == -2 ) ||	(ret == -3 ))		//只越高限
		{
			Q = Qv;
			return 2;
		}
	}

	Q = Qv;
	return 0;
}

//潮流校验
int CRO::Sweep(int rdi_bs2, double Qv )
{
	//g_rdi[rdi_bs2].b_v = g_wb[rdi_bs2].v;//补偿前的电压
	//Qv = -Qv;

	g_rdi[rdi_bs2].Qv = Qv;
	g_wb[rdi_bs2].nr += Qv;
	
	//计算出补偿容量后需要再计算一下潮流，看看有没有越限，
	//特别是下方式下有没有无功倒送的问题
	//g_wb[rdi_bs2].b_v = g_wb[rdi_bs2].v;//补偿前电压

	g_ld_distr=0;//无功优化计算时不能进行负荷分配
	//1.先计算一遍潮流
	if (ExecuteSweep() == -1)//潮流不收敛
	{
//		Q = Qv;
		return -4;
	}
	g_ld_distr=1;//负荷分配开关打开

	return 1;
}

void CRO::resetRdi()
{
	int i;

	for(i = 0; i < mx_rdi; i++)
	{
		g_rdi[i].dr = 0;
		g_rdi[i].v = 0;
		g_rdi[i].quanQ = 0;

		g_rdi[i].a_cosfi = 0;
		g_rdi[i].a_v = 0;
		g_rdi[i].Qv = 0;//补偿容量
	}
}


int CRO::getRdi()
{
	int		i, j,k,m;
	int		ibs, zbs;
	double	rdi_2,r;//从搜索节点开始向上搜索到根节点的全部r，x累加

	int wb[mx_rdi];//跟踪节点数，初步设定为1000个
    for(i = 0; i < mx_rdi; i++)
    {
        wb[i] = 0;
    }

	//对每个节点都要做，类似于短路电流计算
	//从末端一直追踪到首端，一层一层的追
	for(i=1; i<lv_wb; i++)//将等于0的根节点去掉
    {
        j = g_wb[i].jb;//上级支路
		m = g_wb[i].jc;//上级节点

		if (wb[i] == 1)//已经处理过 或者上级是电源点
			continue;

        ibs = g_wbr[j].ibs;//首节点
        zbs = g_wbr[j].zbs;//末节点
		r = g_wbr[j].r;

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		if (wb[zbs] == 0)//没处理
		{
			g_rdi[zbs].ibs = zbs;
			g_rdi[zbs].ibr = j;
			g_rdi[zbs].ibsp = ibs;
			g_rdi[zbs].r = r;//节点上游支路的阻抗
			
			//上游的元素增加一个
			g_rdi[ibs].downstream[g_rdi[ibs].n_downstream++] = zbs;

			//无功臂可以求出			
			//m=0;
			//做路径
			k = zbs;
			rdi_2=0;//之前的所有最短节点的电阻
			while(1)
			{
				if (k == 0) //root (source) bus，找到电源点，搜索停止
				{
					break;
				}

				//追到以前曾经的节点就可以了，辐射网节点的路径唯一
				if (wb[k] == 1)
				{
					rdi_2 += g_rdi[k].quanRdi;

					break;
				}

				rdi_2 += r;
				
				if (k==g_wb[k].jc)//避免死循环
					break;

				k = g_wb[k].jc;//节点的父节点，优化后
			}
			g_rdi[zbs].quanRdi = rdi_2;
				
			wb[zbs] = 1;
		}
	}

	return 1;
}

int  CRO::getQuadQ()
{
	int i,j,k;
//	int ibs,ibr,ibsp;
	double v,dr;
	double quadQi,quadQj,t_q,t_v;

	resetRdi();

	//利用buildRds得到的结果g_rds[]进行处理
	//需要处理每一个母线点的二次矩，0节点例外
    for(i = 1; i < lv_wb; i++)
    {
		//找到本节点的上级和所有下级节点，其中上级节点是已知的，下级节点需要循环
		v = g_rdi[i].v = g_wb[i].v;
		dr = g_rdi[i].dr = g_wb[i].dr-g_wb[i].rloss;//流入该节点的无功值

		//流入
		quadQi = (dr*dr) / (v*v);
		
		//流出
		quadQj = 0;
		for(j=0; j<g_rdi[i].n_downstream; j++)
		{
			k=g_rdi[i].downstream[j];
			t_q = g_rdi[k].dr = g_wb[k].dr-g_wb[k].rloss;
			t_v = g_rdi[k].v = g_wb[k].v;
			quadQj += (t_q*t_q)/(t_v*t_v);
		}


		//如果是线路的末端，这不需要进行杆上补偿，直接进行低压侧补偿
		if (quadQj == 0)
			continue;

		g_rdi[i].quanQ = g_rdi[i].quanRdi * (quadQi - quadQj);
    }

	return 1;
}

//得到节点的容量
double CRO::get_Q(int g_wb_id)
{
	int	 k;
//	int ibsp;
	double r,v,dr;
	//double	ret=0;
	double	origin,rdi;
	double	H2,H1;
	double beta,tau,Kc,alpha,gamma;

	beta = lld->beta_a;
	tau = lld->tau_a;
	Kc = lld->Kc_a;
	alpha = lld->alpha_a;
	gamma = lld->gamma_a;
	
	//找到本节点的上级和所有下级节点，其中上级节点是已知的，下级节点需要循环
	origin = g_rdi[g_wb_id].dr;//原来的无功流入,也就是原来的无功需求
	
	H1=H2=0;
	k = g_wb_id;//本节点的上级节点
	while(1)
	{
		if (k==0) 
			break;

		dr = g_rdi[k].dr;//节点流入的无功
		r  = g_rdi[k].r;
		v  = g_rdi[k].v;
		
		H1 += (2 * dr * r) / (v * v);
		H2 += r  / (v * v);

		if (k==g_rdi[k].ibsp)
			break;
		else
			k = g_rdi[k].ibsp;
	}

	//需要补偿的容量
	rdi = (beta * tau * H1) / ( (2 * beta * tau * H2) + (alpha + gamma) * Kc );

	if (rdi > origin)
		return origin;
	else
		return rdi;
}

//做每个节点的无功臂
int CRO::buildRdi()
{
	int		i, j,k;
	int		ibs, zbs;
	double	rdi_1,rdi_2,v,r;//从搜索节点开始向上搜索到根节点的全部r，x累加

	int wb[mx_rdi];//跟踪节点数，初步设定为1000个
    for(i = 0; i < mx_rdi; i++)
    {
        wb[i] = 0;
    }

	//对rdi重置
	InitWorkingArray_1(0);

	//对每个节点都要做，类似于短路电流计算
	//从末端一直追踪到首端，一层一层的追
	for(i=1; i<lv_wb; i++)//将等于0的根节点去掉
    {
        j = g_wb[i].jb;//上级支路
		//m = g_wb[i].jc;//上级节点

		if (wb[i] == 1)//已经处理过 或者上级是电源点
			continue;

        ibs = g_wbr[j].ibs;//首节点
        zbs = g_wbr[j].zbs;//末节点

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		if (wb[zbs] == 0)//没处理
		{
			g_rdi[zbs].ibs = zbs;
			g_rdi[zbs].ibr = j;
			g_rdi[zbs].ibsp = ibs;
			g_rdi[zbs].v = g_wb[zbs].v;
			g_rdi[zbs].dr = g_wb[zbs].dr-g_wb[zbs].rloss;//流入该节点的无功值
			g_rdi[zbs].r = g_wbr[j].r;
			
			//补偿前的电压
//			g_rdi[zbs].b_v = g_wb[zbs].v;
//			if ( fabs(g_wb[zbs].nw)>0 || fabs(g_wb[zbs].nr)>0 )//补偿前的功率因素
//				g_rdi[zbs].b_cosfi = getCosfi(fabs(g_wb[zbs].nw),fabs(g_wb[zbs].nr));
			
			//上游的元素增加一个
			g_rdi[ibs].downstream[g_rdi[ibs].n_downstream++] = zbs;

			//无功臂可以求出			
			//m=0;
			//做路径
			k = zbs;
			rdi_1=rdi_2=0;//之前的所有最短节点的电阻
			while(1)
			{
				if (k == 0) //root (source) bus，找到电源点，搜索停止
				{
					break;
				}

				//追到以前曾经的节点就可以了，辐射网节点的路径唯一
				if (wb[k] == 1)
				{
					rdi_1 += g_rdi[k].linearRdi;
					rdi_2 += g_rdi[k].quanRdi;

					break;
				}

				v = g_wb[k].v;
				r = g_wbr[g_wb[k].jb].r;//流入该节点的无功值

				rdi_1 += r/(v*v);
				rdi_2 += r;
				
				if (k==g_wb[k].jc)//避免死循环
					break;

				k = g_wb[k].jc;//节点的父节点，优化后
			}
			g_rdi[zbs].linearRdi = rdi_1;
			g_rdi[zbs].quanRdi = rdi_2;
				
			wb[zbs] = 1;
		}
	}

	return 1;
}


//计算每个节点的无功二次精确矩
int  CRO::PutND_QuadQ()
{
	int i,j;
	int ibs,ibr,ibsp;
	double v,dr;
	double quadQi,quadQj,t_q,t_v;

	//利用buildRds得到的结果g_rds[]进行处理
	//需要处理每一个母线点的二次矩，0节点例外
    for(i = 1; i < lv_wb; i++)
    {
		//找到本节点的上级和所有下级节点，其中上级节点是已知的，下级节点需要循环
   		ibs = g_rdi[i].ibs;
		ibr = g_rdi[i].ibr;
		ibsp = g_rdi[i].ibsp;
		v = g_rdi[i].v;
		dr = g_rdi[i].dr;//流入该节点的无功值
		//r = g_rdi[i].r;

		//流入
		quadQi = (dr*dr) / (v*v);
		
		//流出
		quadQj = 0;
		for(j=0; j<g_rdi[i].n_downstream; j++)
		{
			t_q = g_rdi[g_rdi[i].downstream[j]].dr;
			t_v = g_rdi[g_rdi[i].downstream[j]].v;
			quadQj += (t_q*t_q)/(t_v*t_v);
		}

		//如果是线路的末端，这不需要进行杆上补偿，直接进行低压侧补偿
		if (quadQj == 0)
			continue;

		g_rdi[i].quanQ = g_rdi[i].quanRdi * (quadQi - quadQj);
    }

/*
    int    i, j, k,m;
    int    ibs, zbs;
//    double g, b, vi, ai, vj, aj, w, r, x, y;
//    double vi, ri, vj, rj, quadQi, quadQj/*, x, y;
    double t_quadQj;//临时用来计算无功，便于累加;
//	double deltap,deltaq;//计算功率差值

	int wb[mx_rdi];
//将所有的节点设为未处理
    for(i = 0; i < mx_rdi; i++)
    {
        wb[i] = 0;
    }

	//无功二次精确矩只存在于电源点和负荷点之外，也就是0节点
	//无功二次精确矩也应该包括末端节点，否则不完整
	for(i = lv_wb - 1; i > 0; i--)
    {
        j = g_wb[i].jb;//上级支路
		m = g_wb[i].jc;//上级节点

		if ((wb[m] == 1) || (m == 0))//已经处理过 或者上级是电源点
			continue;

        ibs = g_wbr[j].ibs;//首节点
        zbs = g_wbr[j].zbs;//末节点

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		//首末节点的电压和无功
        vi = g_wb[ibs].v;
        ri  = g_wb[ibs].dr-g_wb[ibs].dloss;//流入该节点的无功值
        vj = g_wb[zbs].v;
        rj  = g_wb[zbs].dr-g_wb[zbs].dloss;//dr为节点的前端流入功率，loss为本线段的损耗

		quadQi = (ri*ri) / (vi*vi);
		quadQj = (rj*rj) / (vj*vj);

		//找到和上级节点相关联的其它支路
		//特点：节点共用一个上级节点
		for(k = lv_wb - 1; k > 0; k--)
		{
			if ((m == g_wb[k].jc) && (k != i)) //共用上级节点
			{
				vj = g_wb[k].v;
				rj = g_wb[k].dr-g_wb[k].dloss;//流入该节点的无功值

				t_quadQj = (rj*rj) / (vj*vj);	
				quadQj += t_quadQj;
			}
		}
		g_wb[m].rdi = quadQi-quadQj;
        wb[m] = 1;
    }

	//只有没有找到的才是末端
	for(i = lv_wb - 1; i > 0; i--)
    {
		if ((wb[i] == 1) || (i == 0))//已经处理过 或者上级是电源点
			continue;
		else
		{
			vj = g_wb[i].v;
			rj  = g_wb[i].dr-g_wb[i].dloss;//dr为节点的前端流入功率，loss为本线段的损耗
			quadQj = (rj*rj) / (vj*vj);
			g_wb[i].rdi = quadQj;

			wb[i] = 1;
		}
	}
*/
	return 1;
}

//计算给定节点的一次无功矩，用这个来确定这个节点的补偿容量
double CRO::getND_DelQ(int g_wb_id)
{
	int	 k;
//	int ibsp;
	double dr;
	double	ret=0;
	double	Q1,last,rdi;
	double	a,H2,H1;
	a = 0;
	//计算一次臂,从前向后比较好计算

	//计算节点
	k = g_wb_id;
	H2 = g_rdi[k].linearRdi;//末节点
	k = g_rdi[k].ibsp;
	H1 = g_rdi[k].linearRdi;//末节点的上一个节点
	Q1 = g_rdi[k].dr;	

	//
	rdi = 0;
	last = Q1;
	k = g_rdi[k].ibsp;
	while(1)
	{
		if (k==0) 
			break;

		dr = g_rdi[k].dr;//节点流入的无功
		rdi += g_rdi[k].linearRdi * (dr-last);
		last = dr;

		if (k==g_rdi[k].ibsp)
			break;
		else
			k = g_rdi[k].ibsp;
	}
	
	rdi = (rdi+H1*Q1)/(H1-H2);//补偿完以后的无功总需求
	
	if (fabs(rdi) < a)//补偿的比需要的少
	{
		a = fabs(rdi)-a;	
	}
	else//该节点一共需要的无功总数+下游无功网损的总数，尽量避免无功倒送
	{
		a = -a;
	}

	return a;//返回需要补偿的无功总量，大于0

/*
	rdi = 0;
	//需要反过来得到从0开始的路径
	for(i=n_Y; i>0; i--)
	{
		ibs = Y[i-1];//下游节点

		v = g_rdi[i].v;
		dr = g_rdi[i].dr;//流入该节点的无功值
		r = g_rdi[i].r;//上级的电阻

		rdi = r/(v*v);
		rdi *= (dr - g_rdi[ibs].dr);
	}


//节点前的所有路径上的一次矩进行累加
	//计算除了首节点（电源节点） 的常量部分
	while(1)
	{
		i = g_rdi[g_wb_id].ibs;//从选定的点开始向上搜索，优化后

		if(i == 0)  //root (source) bus，找到电源点，搜索停止
		{
			g_rdi[lv_rdi-1].dr = g_wb[i].nr;//本节点流入的支路无功
			g_rdi[lv_rdi-1].v = g_wb[i].v;//本节点电压
			break;
		}

		j = g_wb[i].jb;//节点的父支路，优化后

		g_rdi[lv_rdi-1].r = g_wbr[j].r;//父支路
		g_rdi[lv_rdi-1].ibr = j;//父支路
		g_rdi[lv_rdi-1].dr = g_wb[i].dr-g_wb[i].dloss;//本节点流入的支路无功
		g_rdi[lv_rdi-1].v = g_wb[i].v;//本节点电压

		ibs = g_wbr[j].ibs;//向上一级进行搜索后的馈线段
        zbs = g_wbr[j].zbs;
	}
	
	for (i=0; i<=lv_rdi-2; i++)
	{
		//对于i>1的情况，每次都需要把节点前所有路径的R / U^2 进行累加
		//需要计算每一个节点的一次矩和无功损耗
		for (j=i; j<=lv_rdi-2; j++)
		{// R / U^2 
			Y[i] += g_rdi[j].r/ (g_rdi[j].v * g_rdi[j].v);
		}
	}


	lv_rdi = 0;
	g_rdi[lv_rdi++].ibs = g_wb_id;//节点优化后的编号，用这个编号去搜索根节点 
	//计算这个节点流进和流出的电流，正常情况下应该是0
	while(1)
	{
		i = g_rdi[lv_rdi-1].ibs;//从选定的点开始向上搜索，优化后

		if(i == 0)  //root (source) bus，找到电源点，搜索停止
		{
			g_rdi[lv_rdi-1].dr = g_wb[i].nr;//本节点流入的支路无功
			g_rdi[lv_rdi-1].v = g_wb[i].v;//本节点电压
			break;
		}

		j = g_wb[i].jb;//节点的父支路，优化后

		g_rdi[lv_rdi-1].r = g_wbr[j].r;//父支路
		g_rdi[lv_rdi-1].ibr = j;//父支路
		g_rdi[lv_rdi-1].dr = g_wb[i].dr-g_wb[i].dloss;//本节点流入的支路无功
		g_rdi[lv_rdi-1].v = g_wb[i].v;//本节点电压

		ibs = g_wbr[j].ibs;//向上一级进行搜索后的馈线段
        zbs = g_wbr[j].zbs;
		if(ibs == i)
		{
			//节点的父节点
			g_rdi[lv_rdi].ibs = zbs;//找到故障点的上一级，因为已经分层优化编号，所以一定是最短的路径
			g_rdi[lv_rdi-1].ibsp = zbs;//找到故障点的上一级，因为已经分层优化编号，所以一定是最短的路径
		}
		else
		{
			g_rdi[lv_rdi].ibs = ibs;
			g_rdi[lv_rdi-1].ibsp = ibs;
		}

		lv_rdi++;
	}

	if (lv_rdi<=2)//电源点本身
		return 0;

//搜索完成后，需要做回溯进行一次精确矩的计算
	//计算除了首节点（电源节点） 的常量部分
	for (i=0; i<=lv_rdi-2; i++)
	{
		//对于i>1的情况，每次都需要把节点前所有路径的R / U^2 进行累加
		//需要计算每一个节点的一次矩和无功损耗
		for (j=i; j<=lv_rdi-2; j++)
		{// R / U^2 
			Y[i] += g_rdi[j].r/ (g_rdi[j].v * g_rdi[j].v);
		}
	}

	a = 0;
	//Y[0]、 Y[1]之前的直接进行累加就可以了
	for (i=2; i<=lv_rdi-2; i++)
	{// (R / U^) * (Qi-Qj)
		a += -(g_rdi[i].dr - g_rdi[i-1].dr) * Y[i];
	}
	
	//最后的Y[0] - Y[1]要特殊处理，需要进行解方程
	if ( Y[0] - Y[1] > 0)
		ret = (a + (Y[1] * g_rdi[1].dr)) / (Y[0] - Y[1]);

	if (ret > 0.006)
		ret = 0.006;

	return ret;
*/
}

//分析电压 0 电压完全合格，不需补偿，-1 不可能补偿到位 -2 可以补偿到位 -3 电压越高限
//DEL int CRO::analyseVoltage()
//DEL {
//DEL 	int i,j,m,ibs;
//DEL 	int ret=0;
//DEL 	int l_flag,h_flag;
//DEL 	double dw,dr,r,x,delta;
//DEL 
//DEL 	l_flag = h_flag = 0;
//DEL 	//计算补偿点的电压等值，便于前后对比,补偿前的状态
//DEL 	//所有不合格节点全部显示出来+7%， -10%
//DEL 	for(i = 0; i < lv_wb; i++)
//DEL 	{
//DEL 		ibs = g_wb[i].tio;
//DEL 		if ( ibs >= 0 ) //补偿后的情况  
//DEL 		{
//DEL 			//需要判断电压是否越限
//DEL 			//g_wb[i].b_v = g_wb[i].v;//补偿前的电压
//DEL 			if ( g_wb[i].v < 0.93 )
//DEL 			{
//DEL 				l_flag = -1;
//DEL 				//和上级节点的电压进行比较
//DEL 
//DEL 				j = g_wb[i].jb;//上级支路
//DEL 				m = g_wb[i].jc;//上级节点
//DEL 				
//DEL 				dw = g_wb[m].dw-g_wb[i].wloss;//流入该节点的无功值
//DEL 				dr = g_wb[m].dr-g_wb[i].rloss;//流入该节点的无功值
//DEL 				r = g_wbr[j].r;//节点上级的电抗
//DEL 				x = g_wbr[j].x;//节点上级的电抗
//DEL 
//DEL 				//本段的损耗，将无功全部就地补偿后的情况
//DEL 				delta = 0.93+(dw*r)/0.93;//上一级电压
//DEL 
//DEL 				//delta = (0.93*(0.93-g_wb[i].v))/x;
//DEL 
//DEL 				//出现了无功倒送，补偿电压不够
//DEL 				if ((delta - dr) > 0)
//DEL 				{
//DEL 				}
//DEL 			}
//DEL 
//DEL 			if ( g_wb[i].v > 1.07 )
//DEL 				h_flag = -3;
//DEL 	    }
//DEL 	}
//DEL 
//DEL 	//一般情况下判断
//DEL 	if ((l_flag<0) && (h_flag<0))
//DEL 		ret = -3;
//DEL 
//DEL 	if ((l_flag<0) && (h_flag==0))
//DEL 		ret = -1;
//DEL 
//DEL 	if ((l_flag==0) && (h_flag<0))
//DEL 		ret = -2;
//DEL 	
//DEL 	return ret;
//DEL }



//输出无功结果,0--不需要进行补偿，lev==1 一阶段；2--二阶段;3--不合格电压的改善
int CRO::putRPO(int lev)
{
	int i;
	int ibs,pnd;
	char fdid[80];//负荷所属馈线ID
//	char ndid[80];//负荷所属馈线ID

	int lv_ld = dsbob2->ld_tb->GetCount();
	LD* ld = dsbob2->ld_tb->GetTable();
	ND* nd = dsbob2->nd_tb->GetTable();
	BS* bs = dsbob2->bs_tb->GetTable();//将母线上的补偿点和补偿容量记录下来


	//slack so
	SO* so = dsbob2->so_tb->GetTable();
//	 so_id[80];
//	lld->delstr(so->id,7,11,so_id);
	i = g_wg[g_slackgen].logic;
	strcpy(fdid,(so+i)->fdid);
	
	if (lev == 1)//只输出一阶段的优化结果
	{
		for(i = 0; i < lv_wb; i++)
		{
			//任何有电压问题的节点都需要输出
			if ( (g_rdi[i].b_v > 0) || ((g_rdi[i].Qv > 0) && (g_wb[i].tio>=0)) ) //第一阶段只有负荷点
			{
				dsbob2->ro_result[lld->g_cnt_ro_fd].id=lld->g_cnt_ro_fd;
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].ndid,(ld+g_wb[i].tio)->id);
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].fdid,fdid);

				dsbob2->ro_result[lld->g_cnt_ro_fd].Qv = g_wbase * g_rdi[i].Qv;
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].b_cosfi , lld->DoubleToString(g_rdi[i].b_cosfi));
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].a_cosfi , "0.95");
				dsbob2->ro_result[lld->g_cnt_ro_fd].b_v = 10 * g_rdi[i].b_v;
				dsbob2->ro_result[lld->g_cnt_ro_fd].a_v = 10 * g_rdi[i].a_v;
				lld->g_cnt_ro_fd++;
			}
		}
	}

	if (lev == 2)//输出二阶段的优化结果
	{
		for(i = 0; i < lv_wb; i++)
		{

			//只要是无功精确矩的最大三个点都列出来
			if ( 
				(g_rdi[i].b_v < 0.93) //前电压有问题
				||(g_rdi[i].b_v > 10.07) //前电压有问题
				||(g_rdi[i].a_v < 0.93) //后电压有问题
				||(g_rdi[i].a_v > 10.07) //后电压有问题
				|| ((i == rdi_bs1) && (g_rdi[i].Qv > 0)) 
				|| ((i == rdi_bs2) && (g_rdi[i].Qv > 0))
				|| ((i == rdi_bs3) && (g_rdi[i].Qv > 0)) 
				)
			{
				//找到相关的节点，不是母线
				ibs = g_wb[i].tio;
				pnd = (bs+ibs)->pnd;
				if (pnd<0)
				{
					continue;
				}
				else
				{
					dsbob2->ro_result[lld->g_cnt_ro_fd].id=lld->g_cnt_ro_fd;
					strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].ndid,(nd+pnd)->id);
					strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].fdid,fdid);

					dsbob2->ro_result[lld->g_cnt_ro_fd].Qv = g_wbase * g_rdi[i].Qv;
					
					//补偿前后的功率因素
					if (g_rdi[i].b_cosfi>0)
					{
						strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].b_cosfi , lld->DoubleToString(g_rdi[i].b_cosfi));
						strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].a_cosfi , lld->DoubleToString(g_rdi[i].a_cosfi));
					}
					else
					{
						strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].b_cosfi , "-");
						strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].a_cosfi , "-");
					}
					dsbob2->ro_result[lld->g_cnt_ro_fd].b_v = 10 * g_rdi[i].b_v;
					dsbob2->ro_result[lld->g_cnt_ro_fd].a_v = 10 * g_rdi[i].a_v;

					lld->g_cnt_ro_fd++;
				}
			}
		}
	}

	if (lev == 3)//输出二阶段的优化结果
	{
		for(i = 0; i < lv_wb; i++)
		{
			ibs = g_wb[i].tio;
			pnd = (bs+ibs)->pnd;
			if (
				((g_rdi[i].b_v > 0) && (pnd>=0)) 
				||(g_rdi[i].Qv > 0)
				) //第一阶段只有负荷点
			{
				dsbob2->ro_result[lld->g_cnt_ro_fd].id=lld->g_cnt_ro_fd;
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].ndid,(nd+pnd)->id);
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].fdid,fdid);

				if (g_rdi[i].Qv >0)
					dsbob2->ro_result[lld->g_cnt_ro_fd].Qv = g_rdi[i].Qv;
				else
					dsbob2->ro_result[lld->g_cnt_ro_fd].Qv = 0;

				//补偿前后的功率因素
				if (g_rdi[i].b_cosfi>0)
				{
					strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].b_cosfi , lld->DoubleToString(g_rdi[i].b_cosfi));
					strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].a_cosfi , lld->DoubleToString(g_rdi[i].a_cosfi));
				}
				else
				{
					strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].b_cosfi , "-");
					strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].a_cosfi , "-");
				}
				
				dsbob2->ro_result[lld->g_cnt_ro_fd].b_v = 10 * g_rdi[i].b_v;
				dsbob2->ro_result[lld->g_cnt_ro_fd].a_v = 10 * g_rdi[i].a_v;

				lld->g_cnt_ro_fd++;
			}
		}
	}

	return 1;
}

//计算二次无功精确矩的“臂”，“矩”为该节点的无功损耗
double CRO::getNDRDI(int g_wb_id/*节点优化后的编号*/)
{
	int		i, j;
	int		ibs, zbs;
	double	total_r;//,total_x;//从搜索节点开始向上搜索到根节点的全部r，x累加

	//i = fault->ibs;//故障点开始进行搜索，得到故障点到电源的最短路径
	//i= 5;//优化前的计算母线编号，ito优化前的编号
//	g_rdi[lv_rdi].ibs = g_wb[i].toi;//节点优化后的编号，用这个编号去搜索根节点 
//	lv_rdi++;

	lv_rdi=total_r=0;
	g_rdi[lv_rdi++].ibs = g_wb_id;//节点优化后的编号，用这个编号去搜索根节点 
	//计算这个节点流进和流出的电流，正常情况下应该是0
	while(1)
	{
		i = g_rdi[lv_rdi-1].ibs;//从故障点开始向上搜索，优化后

		if(i == 0)  //root (source) bus，找到电源点，搜索停止
		{
			break;
		}

		j = g_wb[i].jb;//节点的父节点，优化后

		ibs = g_wbr[j].ibs;//向上一级进行搜索后的馈线段
        zbs = g_wbr[j].zbs;
		total_r += g_wbr[j].r;
		//无功精确矩只需要电阻，不需要电抗，而且只需要最小路上的电阻，旁路并联可以忽略
		//total_x += g_wbr[j].x;

		if(ibs == i)
		{
			g_rdi[lv_rdi].ibs = zbs;//找到故障点的上一级，因为已经分层优化编号，所以一定是最短的路径
		}
		else
		{
			g_rdi[lv_rdi].ibs = ibs;
		}
//
//		g_rdi[lv_rdi-1].ibr = j;//父节点
//		
//		g_wbr[j].fp = 1;  //标注找到的馈线段
//
		lv_rdi++;
	}

	return total_r;
}



