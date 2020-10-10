//*****************************************************************************
//
//  PWRFLOW.CPP
//  Methods implementation of PWRFLOW class
//
//*****************************************************************************
#if _WIN32
#elif __linux__
#else
#endif

#include "../DNASPLIB/pwrflow.h"
#include "../DNASPLIB/tmpodrtb.h"

#include "math.h"
#include "fstream"
using namespace std;

#define ZN_ID_LEN		7  //区段的前缀z999999

///////////////////////////////////////////////////////////////////////////////
//  constructor
PWRFLOW::PWRFLOW()
{
}

PWRFLOW::PWRFLOW(DSBOB2* m_pDb, CLLD* m_pLLD)
{
	dsbob2		= m_pDb;
	lld=m_pLLD;
	g_method	= 0;
	g_mxiter0	= 30;
	g_mxiter	= 25;
	g_niter0	= 0;
	g_niter		= 0;
	g_epsmx0	= 0.01;
	g_epsmx		= 0.00001;
	g_eps0		= 0.0;
	g_island	= 1;
	g_bsmx		= 0;
	g_eps		= 0.0;
	g_pvbsnum	= 0;
	g_loopnum	= 0;
	g_slackbs	= 0;
	g_slackgen	= 0;
	g_wg		= new G_WG[mx_wg];
	lv_wg		= 0;
	if ((lld->mx_bs+MXELE) > mx_wb)
		g_wb		= new G_WB[lld->mx_bs+MXELE];//需要加上母线最大的连接数，否则节点数少的时候会出错
	else
		g_wb		= new G_WB[mx_wb];//前推回代上用到母线编号作为角标
	lv_wb		= 0;
	g_wld		= new G_WLD[mx_wld];
	lv_wld		= 0;
	g_wbr		= new G_WBR[mx_wbr];
	lv_wbr		= 0;
	g_wxf		= new G_WXF[mx_wxf];
	lv_wxf		= 0;
	g_wsh		= new G_WSH[mx_wsh];
	lv_wsh		= 0;
	g_yu		= new G_YU[mx_yu];
	lv_yu		= 0;
	g_jd		= new G_JD[mx_jd];
	lv_jd		= 0;
	g_ju		= new G_JU[mx_ju];
	lv_ju		= 0;
	g_jl		= new G_JL[mx_jl];
	lv_jl		= 0;
	g_jb		= new G_JB[mx_jb];
	lv_jb		= 0;
	g_rx		= new G_RX[mx_rx][mx_rx];
	lv_rx		= 0;
	g_ctl		= new G_CTL[mx_ctl];
	g_iter	    = new G_ITER[mx_iter];

	lv_iter		= 0;
	g_r			= 0.0;
	g_x			= 0.0;
	g_wloss		= 0.0;
	g_rloss		= 0.0;

	g_ld_distr  = 1;

	g_zbr		= new G_ZBR[mx_zbr];
	lv_zbr		= 0;
}

///////////////////////////////////////////////////////////////////////////////
//  destructor
PWRFLOW::~PWRFLOW()
{
	delete[] g_wg;
	delete[] g_wb;
	delete[] g_wld;
	delete[] g_wbr;
	delete[] g_wxf;
	delete[] g_wsh;
	delete[] g_yu;
	delete[] g_jd;
	delete[] g_ju;
	delete[] g_jl;
	delete[] g_jb;
	delete[] g_rx;
	delete[] g_ctl;
	delete[] g_iter;

	delete[] g_zbr;
}

////针对具体的一个岛进行潮流计算
//void PWRFLOW::InitWorkingArray(int island)
//{
//    int    i;
// 
//    //initialize source working array
//    for(i = 0; i < mx_wg; i++)
//    {
//		g_wg[i].logic = 0;
//		g_wg[i].ibs = 0;
//		g_wg[i].regbs = 0;
//		g_wg[i].slackavr = 1;//set generate node as pq node 
//		g_wg[i].vtarget = 1.05;
//		g_wg[i].w = 0.0;
//		g_wg[i].r = 0.0;
//		g_wg[i].v = 1.0;
//		g_wg[i].a = 0.0;
//
//		g_wg[i].ss = 0.0;
//		g_wg[i].r0 = 0.0;
//		g_wg[i].x0 = 0.0;
//	}
//    //initialize bus working array  mx_wb
//	//
//    for(i = 0; i < mx_wb; i++)
//    {
//        g_wb[i].tio = 0;
//        g_wb[i].toi = 0;
//        g_wb[i].ja = 0;
//        g_wb[i].jb = 0;
//        g_wb[i].jc = 0;
//        g_wb[i].v = 1.05;
//        g_wb[i].a = 0.0;
//        g_wb[i].nw = 0.0;
//        g_wb[i].nr = 0.0;
//        g_wb[i].dv = 0.0;
//        g_wb[i].da = 0.0;
//        g_wb[i].dw = 0.0;
//        g_wb[i].dr = 0.0;
//
//        g_wb[i].wloss = 0.0;//节点流入线段的网损
//        g_wb[i].rloss = 0.0;//节点流入线段的网损
//    }
//    lv_wb = 0;
//
//    //initialize load working array
//    for(i = 0; i < mx_wld; i++)
//    {
//        g_wld[i].logic = 0;
//        g_wld[i].ibs = 0;
//        g_wld[i].w = 0.0;
//        g_wld[i].r = 0.0;
//    }
//    lv_wld = 0;
//
//    //initialize branch working array
//    for(i = 0; i < mx_wbr; i++)
//    {
//        g_wbr[i].type = 0;
//        g_wbr[i].logic = 0;
//        g_wbr[i].ibs = 0;
//        g_wbr[i].zbs = 0;
//        g_wbr[i].r = 0.0;
//        g_wbr[i].x = 0.0;
//        g_wbr[i].b = 0.0;
//    }
//    lv_wbr = 0;
//
//	//initialize non-rating ratio winding array
//	for(i = 0; i < mx_wxf; i++)
//	{
//		g_wxf[i].ibr = 0;
//		g_wxf[i].t = 1.0;
//		g_wxf[i].g1 = 0.0;
//		g_wxf[i].b1 = 0.0;
//		g_wxf[i].g2 = 0.0;
//		g_wxf[i].b2 = 0.0;
//	}
//	lv_wxf = 0;
//
//    //initialize shunt working array
//    for(i = 0; i < mx_wsh; i++)
//    {
//        g_wsh[i].logic = 0;
//        g_wsh[i].ibs = 0;
//        g_wsh[i].r = 0.0;
//        g_wsh[i].rnom = 0.0;
//	}
//    lv_wsh = 0;
//
//    //initialize branch working array
//    for(i = 0; i < mx_zbr; i++)
//    {
//        g_zbr[i].ibs = 0;
//        g_zbr[i].w = 0.0;
//        g_zbr[i].r = 0.0;
//        g_zbr[i].v = 0.0;
//        g_zbr[i].a = 0.0;
//        g_zbr[i].i = 0.0;
//    }
//    lv_zbr = 0;
//}

//int PWRFLOW::GetNetworkData(int island)
//{
//	int i;
/*
//////////////////////////////////////////////////////////////////////////
	//电源点
    g_wg[0].logic = i;
    g_wg[0].ibs = ibs;
    g_wg[0].slackavr = 1; //表明是松弛节点
            
	//配电网的电源根节点就是平衡节点，默认是10.5
	g_wg[0].v = 1.05;
	
	//电源点的首端有功和无功，主要用于负荷点的处理
	g_wg[0].w = (so+i)->w / g_wbase;	//有功
	g_wg[0].r = (so+i)->r / g_wbase;	//无功
				
//////////////////////////////////////////////////////////////////////////
	//负荷点,把每个负荷点的数据都读出来				
    for(i = 0; i < lv_ld; i++)
    {
        ibs = (ld+i)->ibs;

		//set load working array 
        g_wld[lv_wld].logic = i;
        g_wld[lv_wld].ibs = ibs;


		if ((ld+i)->w  == 0)//只有在没有实际的量测的时候，才能用计算出来的进行替代
			(ld+i)->w = g_scale_P * ((ld+i)->wm);

		if ((ld+i)->r  == 0)
			(ld+i)->r = g_scale_Q * ((ld+i)->rm);

		g_wld[lv_wld].w = (ld+i)->w / g_wbase ;
		g_wld[lv_wld].r = (ld+i)->r / g_wbase ;
		
		lv_wld++;
    }

//////////////////////////////////////////////////////////////////////////
	//电容器
    for(i = 0; i < lv_cp; i++)
    {
        ibs = (cp+i)->ibs;

        //set shunt working array 
        g_wsh[lv_wsh].logic = i;
        g_wsh[lv_wsh].ibs = ibs;
        g_wsh[lv_wsh].r = 0;
        g_wsh[lv_wsh].rnom = ((cp+i)->r/1000) / g_wbase;
        lv_wsh++;
    }

//////////////////////////////////////////////////////////////////////////
	//支路
    for(i = 0; i < lv_sec; i++)
    {
        ibs = (sec+i)->ibs;
        zbs = (sec+i)->zbs;

        g_wbr[lv_wbr].type = _SEC;
        g_wbr[lv_wbr].logic = i;
        g_wbr[lv_wbr].ibs = ibs;
        g_wbr[lv_wbr].zbs = zbs;
		
		g_wbr[lv_wbr].r = (sec+i)->r0 * (g_wbase/100);
		g_wbr[lv_wbr].x = (sec+i)->x0 * (g_wbase/100);
		g_wbr[lv_wbr].b = (sec+i)->b0 * (g_wbase/100);//配电网的b一般为0

		lv_wbr++;
    }
*/	
//	return 1;
//}

int PWRFLOW::ExecuteIslandPwrflow(int iisland,int type)
{
	g_island = iisland;

	int		ret = 0, lv_island;
	bool isFail = true;

	char out_error[200];
	char so_id[80];
	int  soid;

	so_id[0] = 0;
	if (type == 0)
	{
		soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
		if (soid != -1)
			strcpy(so_id, dsbob2->so_tb->GetDataByLog(soid)->id);
	}

	InitWorkingArray();

	//read network data		2.读入网络数据
	if (GetNetworkData(type) == PWRFLOW_FAIL)
	{
		sprintf(out_error, "getNetworkData error. fdid=%s\n", so_id);
		lld->WriteLog(lld->logfile, out_error);
		dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

		PutFlowResult(2);
		ret--;
	}
		//g_method = 0;//测试gaosi
	switch (g_method)
	{
		case 0:  //sweep前推回代方法
			if (g_loopnum > 0)
			{
				sprintf(out_error, "loop network error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;
			}
			//order bus number by BFS	3.通过广度搜索进行母线编号优化
			if (OrderBusNumberA() == PWRFLOW_FAIL)
			{
				//add by lld 2011-11-02
				sprintf(out_error, "OrderBusNumberA error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				if(strlen(so_id)>0)
					dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;
			}

			if (ExecuteSweep() == PWRFLOW_FAIL)
			{
				sprintf(out_error, "ExecuteSweep error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				if (strlen(so_id) > 0)
					dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;
			}
			break;

		case 1:  //newton

			//order bus number
			if (OrderBusNumberB() == PWRFLOW_FAIL)
			{
				sprintf(out_error, "OrderBusNumberB error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				if (strlen(so_id) > 0)
					dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;
			}

			//execute Newton-Raphson method
			if (ExecuteNewton() == PWRFLOW_FAIL)
			{
				sprintf(out_error, "ExecuteNewton error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				if (strlen(so_id) > 0)
					dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;
			}
			break;
	}

	//如果不收敛，不需要填写
	//write result into database，具体针对一个计算岛
	if (PutFlowResult(type) == PWRFLOW_FAIL)
	{
		return PWRFLOW_FAIL;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
//  ExecutePwrflow(),默认是对所有的岛进行处理
//单独对一个岛进行处理，也就是取一个岛的数据出来进行处理
//有一个岛不收敛，可以不用管，但要记录下来
int PWRFLOW::ExecutePwrflow()
{
	int		ret=0, lv_island;
	// clock_t start, finish;
	bool isFail = true;

	char out_error[200];
	char so_id[80];
	int  soid;

	lv_island = dsbob2->island_tb->GetCount();

	//execute calculations 对每一个岛进行计算
	//岛所关联的应该是馈线
	sprintf(out_error, "\n潮流计算3\n");
	lld->WriteLog(lld->logfile, out_error);

	for(g_island = 0; g_island < lv_island; g_island++)
	{
		soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
		strcpy(so_id, dsbob2->so_tb->GetDataByLog(soid)->id);
		//if (strcmp("SO_1036863808", so_id) == 0)
		//{
		//	isFail = false;
		//}


//		isFail = false;
//lld->TimeHere("end prep data start order.");
		//只对本岛内的数据进行初始化
		//initialize working arrays		1.对母线进行初始化
		InitWorkingArray();

		//read network data		2.读入网络数据
		if(GetNetworkData() == PWRFLOW_FAIL)
		{
			soid=dsbob2->island_tb->GetDataByLog(g_island)->reffd;
			strcpy(so_id , dsbob2->fd_tb->GetDataByLog(soid)->id);
			sprintf(out_error,"getNetworkData error. fdid=%s\n",so_id);
			lld->WriteLog(lld->logfile,out_error);
			dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

			PutFlowResult(2);
			ret--;

			continue;
		}

		//采用什么方法计算，这里需要有一个基本的判断，如果是分布式，用1，否则用0
		//execute selected method

		//g_method = 0;//测试gaosi
		switch(g_method)
		{

		case 0:  //sweep前推回代方法
			if (g_loopnum > 0)
			{
				soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id, dsbob2->fd_tb->GetDataByLog(soid)->id);
				sprintf(out_error, "loop network error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;

				continue;
			}
			//order bus number by BFS	3.通过广度搜索进行母线编号优化
			if(OrderBusNumberA() == PWRFLOW_FAIL)
			{
				//add by lld 2011-11-02
				soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id, dsbob2->fd_tb->GetDataByLog(soid)->id);
				sprintf(out_error, "OrderBusNumberA error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;

//				printf(str);
				continue;
			}

			//OnSearchXFMR("102_200219338",str);

//lld->TimeHere("end order data start sweep.");
			//execute backward and forward sweep	4.执行前推回代
			if(ExecuteSweep() == PWRFLOW_FAIL)
			{
				soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id, dsbob2->fd_tb->GetDataByLog(soid)->id);
				sprintf(out_error, "ExecuteSweep error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;
				
				//				printf(str);
				continue;
			}
			break;
		
		case 1:  //newton
 
			//order bus number
			if(OrderBusNumberB() == PWRFLOW_FAIL)
			{
				soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id, dsbob2->fd_tb->GetDataByLog(soid)->id);
				sprintf(out_error, "OrderBusNumberB error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;

				continue;
			}

			//execute Newton-Raphson method
			if(ExecuteNewton() == PWRFLOW_FAIL)
			{
				soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id, dsbob2->fd_tb->GetDataByLog(soid)->id);
				sprintf(out_error, "ExecuteNewton error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//潮流不收敛

				PutFlowResult(2);
				ret--;

				continue;
			}
			break;

		//default:  //seidel & newton

			//order bus number母线编号
			//if(OrderBusNumberB() == PWRFLOW_FAIL)
			//{
			//	return PWRFLOW_FAIL;
			//}

			////form admittance matrix形成导纳矩阵
			//if(FormAdmittance() == PWRFLOW_FAIL)
			//{
			//	return PWRFLOW_FAIL;
			//}

			////execute Gauss-Seidel method
			////形成导纳矩阵后做高斯迭代
			//if(ExecuteSeidel() == PWRFLOW_FAIL)
			//{
			//	return PWRFLOW_FAIL;
			//}

			////execute Newton-Raphson method
			//if(ExecuteNewton() == PWRFLOW_FAIL)
			//{
			//	return PWRFLOW_FAIL;
			//}
		}

		//如果不收敛，不需要填写
		//write result into database，具体针对一个计算岛
		if (PutFlowResult() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

//lld->TimeHere("end sweep.");
//printf(lld->OutputAllTime());


//增加无功优化模块，可能需要反复计算潮流
//		//计算无功二次精确矩，对每一个母线进行计算
//		//1.分析本条线路是否需要进行无功补偿，如果需要的话，才进行下一步的分析，否则直接跳过
//		//判据,负荷侧的功率因素和电压
//		if ( mathtype == 1 )
//		{
//			//启动无功优化程序，对本馈线进行无功优化，需要反复计算潮流
//			//对当前的电源点（单个的电气岛）进行优化补偿，得出补偿结论。
//			//isNeedRPO();
//			
//		}
	}

	//sprintf(out_error, "\n潮流计算4\n");
	//lld->WriteLog(lld->logfile, out_error);

	//将对侧的裕度和最低电压也考虑

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
//  InitWorkingArray()
void PWRFLOW::InitWorkingArray()
{
    int    i;
 
    //initialize source working array
    for(i = 0; i < mx_wg; i++)
    {
        g_wg[i].logic = 0;
        g_wg[i].ibs = 0;
        g_wg[i].regbs = 0;
        g_wg[i].slackavr = 1;
        g_wg[i].kswitch = 0;
        g_wg[i].vtarget = 1.0;
        g_wg[i].wmx = 0.0;
        g_wg[i].rmx = 0.0;
        g_wg[i].wmn = 0.0;
        g_wg[i].rmn = 0.0;
        g_wg[i].w = 0.0;
        g_wg[i].r = 0.0;
        g_wg[i].v = 1.0;
        g_wg[i].a = 0.0;

		//初始化短路容量
        g_wg[i].ss = 0.0;
    }
    lv_wg = 0;

    //initialize bus working array  mx_wb
	//
    for(i = 0; i < lld->mx_bs+MXELE; i++)
    {
        g_wb[i].tio = 0;
        g_wb[i].toi = 0;
        g_wb[i].ja = 0;
        g_wb[i].jb = 0;
        g_wb[i].jc = 0;
        g_wb[i].v = 1.05;
        g_wb[i].a = 0.0;
        g_wb[i].nw = 0.0;
        g_wb[i].nr = 0.0;
        g_wb[i].dv = 0.0;
        g_wb[i].da = 0.0;
        g_wb[i].dw = 0.0;
        g_wb[i].dr = 0.0;

        g_wb[i].wloss = 0.0;//节点流入线段的网损
        g_wb[i].rloss = 0.0;//节点流入线段的网损

//        g_wb[i].rdi = 0.0;//二次无功精确矩
//        g_wb[i].qv = 0.0;//需要补偿的电容器容量
//        g_wb[i].b_v = 0.0;//补偿前的电压
//        g_wb[i].b_cosfi = 0.0;//补偿前的功率因素
    }
    lv_wb = 0;

    //initialize load working array
    for(i = 0; i < mx_wld; i++)
    {
        g_wld[i].logic = 0;
        g_wld[i].ibs = 0;
        g_wld[i].w = 0.0;
        g_wld[i].r = 0.0;
    }
    lv_wld = 0;

    //initialize branch working array
    for(i = 0; i < mx_wbr; i++)
    {
        g_wbr[i].type = 0;
        g_wbr[i].logic = 0;
        g_wbr[i].ibs = 0;
        g_wbr[i].zbs = 0;
        g_wbr[i].r = 0.0;
        g_wbr[i].x = 0.0;
        g_wbr[i].b = 0.0;

		g_wbr[i].fp = 0;//无功优化时需要

    }
    lv_wbr = 0;

	//initialize non-rating ratio winding array
	for(i = 0; i < mx_wxf; i++)
	{
		g_wxf[i].ibr = 0;
		g_wxf[i].t = 1.0;
		g_wxf[i].g1 = 0.0;
		g_wxf[i].b1 = 0.0;
		g_wxf[i].g2 = 0.0;
		g_wxf[i].b2 = 0.0;
	}
	lv_wxf = 0;

    //initialize shunt working array
    for(i = 0; i < mx_wsh; i++)
    {
        g_wsh[i].logic = 0;
        g_wsh[i].ibs = 0;
        g_wsh[i].r = 0.0;
        g_wsh[i].rnom = 0.0;
	}
    lv_wsh = 0;

    //initialize branch working array
    for(i = 0; i < mx_zbr; i++)
    {
        g_zbr[i].logic = 0;
        g_zbr[i].ibs = 0;
        g_zbr[i].w = 0.0;
        g_zbr[i].r = 0.0;
        g_zbr[i].v = 0.0;
        g_zbr[i].a = 0.0;
        g_zbr[i].i = 0.0;

        g_zbr[i].flag = 0;
    }
    lv_zbr = 0;

	g_eps_last = 100;
}

///////////////////////////////////////////////////////////////////////////////
//  GetNetworkData()
int PWRFLOW::GetNetworkData(int type)
{
//	//get bs table
//	BS* bs = dsbob2->bs_tb->GetTable();

    //read source data from database
	//在风险计算的时候这里可能需要读取负荷预测的数据
    if(GetSourceData(type) == PWRFLOW_FAIL)
    {
        return PWRFLOW_FAIL;
    }

    //read branch data from database
    if(GetBranchData() == PWRFLOW_FAIL)
    {
        return PWRFLOW_FAIL;
    }

//		//先做负荷分配
//		if(OnDistributeLoadData() == PWRFLOW_FAIL)
//		{
//			return PWRFLOW_FAIL;
//		}	
//

    //read load data from database
    if(GetLoadData() == PWRFLOW_FAIL)
    {
        return PWRFLOW_FAIL;
    }

    //read shunt data from database
    if(GetShuntData() == PWRFLOW_FAIL)
    {
        return PWRFLOW_FAIL;
    }

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  GetSourceData()
int PWRFLOW::GetSourceData(int type)
{
	int		i;
	int		ibs, dead;
//	float	vl;//电压等级，在做标幺值的时候有用
	int slackavr;
	double	ratio;
	double  sumP,sumQ;
	double P_value, Q_value, I_value, U_value;

	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();

	//get so table
	int lv_so = dsbob2->so_tb->GetCount();
	SO* so = dsbob2->so_tb->GetTable();

	//get load table
	int lv_ld = dsbob2->ld_tb->GetCount();
	LD* ld = dsbob2->ld_tb->GetTable();
	g_scale_P = g_scale_Q = 1;

//	so_cnt = 0;//一般的岛只含有一个分布式电源
	for(i = 0; i < lv_so; i++)
	{
		g_loopnum = 0;
		ibs = (so+i)->ibs;//只找到本岛内的电源点，一般只有一个
		dead = (so+i)->q&SO_OPEN || (bs+ibs)->iisland!=g_island || (bs+ibs)->q&BS_OPEN;

		if (!dead)
		{
			g_wg[lv_wg].logic = i;
			g_wg[lv_wg].ibs = ibs;
			//            g_wg[lv_wg].wmx = (so+i)->wmx / g_wbase;	最大有功
			//            g_wg[lv_wg].rmx = (so+i)->rmx / g_wbase;	最大无功
			//            g_wg[lv_wg].wmn = (so+i)->wmn / g_wbase;	最小有功
			//            g_wg[lv_wg].rmn = (so+i)->rmn / g_wbase;	最小无功
			g_wg[lv_wg].wmx = 1.0e6;//(so+i)->wmx / g_wbase;	最大有功
			g_wg[lv_wg].rmx = 1.0e6;//(so+i)->rmx / g_wbase;	最大无功
			g_wg[lv_wg].wmn = -1.0e6;//(so+i)->wmn / g_wbase;	最小有功
			g_wg[lv_wg].rmn = -1.0e6;//(so+i)->rmn / g_wbase;	最小无功

			if (lld->MATH_VOLTAGELEVEL == 10)
			{
				//配电网的电源根节点就是平衡节点，默认是10.5,目标电压
				//默认的电压等级为10kV，如果填写了其它的电压等级，需要在标幺值里面进行处理
				//电压的标幺值10，功率100；
				slackavr = (so + i)->slackavr;
				if (slackavr == 4)//主平衡节点
				{
					if (type != 0)
						continue;//做残余岛的潮流，不能有首端电源点

					g_vl = atof((so + i)->vl);//电源点的首端电压值;
				}

				if ((slackavr == 0) || (slackavr == 4))
				{
					if ((so + i)->vtarget == 0)
						(so + i)->vtarget = g_vl;

					g_wg[lv_wg].v = ((so + i)->vtarget) / 10;//电源点的首端电压值;
					g_wg[lv_wg].slackavr = slackavr;//平衡节点 

					ratio = (so + i)->vtarget / atof((so + i)->vl);
					(bs + ibs)->v = ratio;//1.06;
					g_wg[lv_wg].vtarget = ratio;//(so+i)->vtarget/g_wbase;//平衡节点的目标电压
				}
				else
					if (slackavr == 1)//PQ节点
					{
						g_wg[lv_wg].w = (so + i)->w / g_wbase;	//有功
						g_wg[lv_wg].r = (so + i)->r / g_wbase;	//无功
					}
					else
						if (slackavr == -1)//PV节点
						{
							g_wg[lv_wg].w = (so + i)->w / g_wbase;	//有功
							g_wg[lv_wg].v = (so + i)->v / atof((so + i)->vl);
						}
						else
							if (slackavr == -2)//PI节点
							{
								g_wg[lv_wg].w = (so + i)->w / g_wbase;	//有功
								g_wg[lv_wg].v = g_wg[lv_wg].w / (1.732 * (so + i)->i * ((atof((so + i)->vl)*0.01732)));
							}

				//只有首端有量测的情况下，才能用这个负荷分配法则，如果没有量测，严格用负荷值来做
				//首端有量测和负荷端都有量测的情况下，因为有线损和误差的存在，可能存在部分数据部一致

				//如果首端和末端都没有量测，如何处理？
				//如果首端有量测，负债率自动失效了，因为首端的量测就是负载率
				if ((so + i)->isAnalog_P)//首端有功有量测,只有在有量测的情况下进行折算才有意义
				{
					I_value = (so + i)->I_value;//实测的出口电流
					U_value = (so + i)->vtarget;//收敛电压,有可能不是10kV

					P_value = 1.732 * 0.98 * I_value * U_value / 1000;
					Q_value = 1.732 * 0.2 * I_value * U_value / 1000;

					//这个数据准备再后面进行负荷分配的时候用
					g_wg[lv_wg].w = P_value / g_wbase;	//有功

					//计算这个电源点所有的负荷的折算结果
					//将这个写到电源列表里面，以后在计算的时候就可以用这个进行负荷大小的折算
					sumP = 0;//负荷总加
					for (int j = 0; j < lv_ld; j++)
					{
						ibs = (ld + j)->ibs;
						dead = (ld + j)->q&LD_OPEN || (bs + ibs)->iisland != g_island || (bs + ibs)->q&BS_OPEN;

						if (!dead)
						{
							//变压器容量*负债率，负债率在读入数据的时候已经处理了
							sumP += (ld + j)->wm / 1000 / g_wbase;//kva
						}
					}
					if (sumP == 0)//整条线都没有负荷
					{
						g_wg[lv_wg].w = 0;	//有功
						g_scale_P = 0;
					}
					else
					{
						//如果有实测的负荷值，用实测的替代，假定10%的线损
						//计算缩放因子
						g_scale_P = (g_wg[lv_wg].w / sumP) * (1 - lld->LINELOSS);
					}
				}
				//根据实测的负荷总加和负荷的额定容量折算出负荷的比例
				(so + i)->fScaleP = g_scale_P;

				if ((so + i)->isAnalog_Q)//首端无功有量测
				{
					//这个数据准备再后面进行负荷分配的时候用
					//g_wg[lv_wg].r = (so + i)->r / g_wbase;	
					g_wg[lv_wg].r = Q_value / g_wbase;	
					
					//计算这个电源点所有的负荷的折算结果
					//将这个写到电源列表里面，以后在计算的时候就可以用这个进行负荷大小的折算
					sumQ = 0;//负荷总加
					for (int j = 0; j < lv_ld; j++)
					{
						ibs = (ld + j)->ibs;
						dead = (ld + j)->q&LD_OPEN || (bs + ibs)->iisland != g_island || (bs + ibs)->q&BS_OPEN;

						if (!dead)
						{
							//变压器容量*负债率，负债率在读入数据的时候已经处理了
							sumQ += (ld + j)->rm / 1000 / g_wbase;//kva
						}
					}

					if (sumQ == 0)//整条线都没有负荷
					{
						g_wg[lv_wg].r = 0;	//有功
						g_scale_Q = 0;
					}
					else
					{
						//如果有实测的负荷值，用实测的替代，假定3.8%的线损
						g_scale_Q = (g_wg[lv_wg].r / sumQ) * (1- lld->LINELOSS);
					}
				}
				//根据实测的负荷总加和负荷的额定容量折算出负荷的比例
				(so + i)->fScaleQ = g_scale_Q;

				//g_wg[lv_wg].ss = (so+i)->ss / g_wbase;//电源点的短路容量
				if (slackavr == 4)
				{
					//没有填写相关的接地值
					if ((so + i)->ss == 0)
					{
						g_wg[lv_wg].ss = 360 / g_wbase;//电源点的短路容量
						g_wg[lv_wg].x0 = 2;//接地变电抗
						g_wg[lv_wg].r0 = 10;//接地电阻，一般为10欧姆，地网电阻忽略，金属接地
					}
					else
					{
						g_wg[lv_wg].ss = (so + i)->ss / g_wbase;
						g_wg[lv_wg].x0 = 3 * (so + i)->sw;//接地变电抗
						g_wg[lv_wg].r0 = 3 * (so + i)->sr + (so + i)->pLoss;//接地电阻，一般为10欧姆，地网电阻忽略，金属接地
					}
				}
				else
					g_wg[lv_wg].ss = (so + i)->ss;

				lv_wg++;
				if (lv_wg > 1)//出现多个电源点
				{
					//可能出现了分布式电源
					g_loopnum = lv_wg - 1;//环网出现,此为环网的个数		
					this->g_method = 1;//需要用牛顿法来解
				}
			}
			else
			{
				//110kV的母线都是电源点
				g_wg[lv_wg].slackavr = (so + i)->slackavr;//平衡节点 
				g_wg[lv_wg].x1 = (so + i)->sw;//电抗
				g_wg[lv_wg].r1 = (so + i)->sr;//电阻
				if (((so + i)->slackavr != 0) && ((so + i)->slackavr != 4))
				{
					g_wg[lv_wg].w = (so + i)->w;//有功P
					g_wg[lv_wg].r = (so + i)->r;//无功Q
				}
				else
				{
					//通过电压等级，得到需要收敛的值


					g_wg[lv_wg].vtarget = (bs + ibs)->v = g_wg[lv_wg].v = (so + i)->vtarget / 10;//电源点的首端电压值;
				}

				lv_wg++;
			}
		}
	}

	if (type == 1)
	{
		//模拟联络
		int lv_bs = dsbob2->bs_tb->GetCount();
		for (i = 0; i < lv_bs; i++)
		{
			ibs = i;
			dead = ((bs + i)->iisland != g_island) 
			//	|| ((bs + i)->q&BS_OPEN) 
				|| !((bs + i)->q & BS_UNION);

			if (!dead)
			{
				g_wg[lv_wg].logic = i;
				g_wg[lv_wg].ibs = ibs;
				g_wg[lv_wg].wmx = 1.0e6;//(so+i)->wmx / g_wbase;	最大有功
				g_wg[lv_wg].rmx = 1.0e6;//(so+i)->rmx / g_wbase;	最大无功
				g_wg[lv_wg].wmn = -1.0e6;//(so+i)->wmn / g_wbase;	最小有功
				g_wg[lv_wg].rmn = -1.0e6;//(so+i)->rmn / g_wbase;	最小无功

				if (lld->MATH_VOLTAGELEVEL == 10)
				{
					g_wg[lv_wg].v = 1.0;//电源点的首端电压值;
					g_wg[lv_wg].slackavr = 0;//平衡节点 

					(bs + ibs)->v = 1.0;//1.06;
					g_wg[lv_wg].vtarget = 1.0;//平衡节点的目标电压
					lv_wg++;
					if (lv_wg > 1)//出现多个电源点
					{
						//可能出现了分布式电源
						g_loopnum = lv_wg - 1;//环网出现,此为环网的个数		
						this->g_method = 1;//需要用牛顿法来解
					}
				}
			}
		}
	}
	
    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  GetBranchData()
int PWRFLOW::GetBranchData()
{
	int		i;
	int		ibs, zbs;
	int		iout, zout, dead;
	int		itapty,iztapty;
	double	r, x, t;

	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();

	//get lnty table
//	LNTY* lnty = dsbob->lnty_tb->GetTable();

	//read sec data into working branch array		读取线路表
	int lv_sec = dsbob2->sec_tb->GetCount();
	if(lv_sec != 0)
	{
		SEC* sec = dsbob2->sec_tb->GetTable();

        for(i = 0; i < lv_sec; i++)
        {
            ibs = (sec+i)->ibs;
            zbs = (sec+i)->zbs;

            iout = (sec+i)->topflags & SEC_IOPEN || (bs+ibs)->iisland != g_island;
            zout = (sec+i)->topflags & SEC_ZOPEN || (bs+zbs)->iisland != g_island;
            dead = iout && zout;
			
			(sec+i)->ialgzi = 0;
            
			if(!dead)
            {
                //get section parameters
				if ( (ibs == zbs)//0阻抗支路
					|| (((sec + i)->r1 == 0) && ((sec + i)->x1 == 0)) )
				{
					//将其节点加入特殊处理的行列
	                g_zbr[lv_zbr].logic = i;
	                g_zbr[lv_zbr++].ibs = ibs;

					continue;
				}

				//线路的参数在之前已经处理过了
/*               ilnty = (sec+i)->ilnty;
                len = (sec+i)->len / 100.0;
                r = len * (lnty + ilnty)->r0;
                x = len * (lnty + ilnty)->x0;
                b = len * (lnty + ilnty)->b0;
*/
                //set branch working array
                g_wbr[lv_wbr].type = _SEC;
                g_wbr[lv_wbr].logic = i;
                g_wbr[lv_wbr].ibs = ibs;
                g_wbr[lv_wbr].zbs = zbs;
				g_wbr[lv_wbr].r = (sec+i)->r1;//正序
				g_wbr[lv_wbr].x = (sec+i)->x1;
				g_wbr[lv_wbr].b = (sec+i)->b1;//配电网的b一般为0

				//if (((sec + i)->r0 == 0) && ((sec + i)->x0 == 0))
				//{//缺省用4倍正序处理
				//	g_wbr[lv_wbr].r0 = 4*(sec + i)->r1;
				//	g_wbr[lv_wbr].x0 = 4*(sec + i)->x1;
				//}
				//else
				{
					g_wbr[lv_wbr].r0 = (sec + i)->r0;
					g_wbr[lv_wbr].x0 = (sec + i)->x0;
				}
				g_wbr[lv_wbr].b0 = (sec + i)->b0;//配电网的b一般为0

//				if (strcmp((sec+i)->fdid,"CIRCUIT_1017995888")==0)
//				{
//					CString sss;
//					sss.Format("%d %s",lv_wbr,(sec+i)->id);
//					lld->addLogLine(sss);
//					//lv_wbr++;
//				}
				lv_wbr++;
            }
        }
	}

	if (lld->mx_xf > 0)
	{
		int lv_xf = dsbob2->xf_tb->GetCount();
		if (lv_xf != 0)
		{
			XF* xf = dsbob2->xf_tb->GetTable();

			for (i = 0; i < lv_xf; i++)
			{
				ibs = (xf + i)->ibs;
				zbs = (xf + i)->zbs;
				iout = (xf + i)->topflags & XF_IOPEN || (bs + ibs)->iisland != g_island;
				zout = (xf + i)->topflags & XF_ZOPEN || (bs + zbs)->iisland != g_island;
				dead = iout && zout;

				if (!dead)
				{
					r = (xf + i)->r1;//根据绕组的电压等级换算表幺值
					x = (xf + i)->x1;
									//set branch working array
					g_wbr[lv_wbr].type = _XF;
					g_wbr[lv_wbr].logic = i;
					g_wbr[lv_wbr].ibs = ibs;
					g_wbr[lv_wbr].zbs = zbs;
					g_wbr[lv_wbr].r =  0.0001;
					g_wbr[lv_wbr].x =  0.0001;
					g_wbr[lv_wbr].b = 0.0;
					lv_wbr++;
				}
			}
		}
	}

	//get cp table
	if (lld->mx_cp > 0)
	{
		int lv_cp = dsbob2->cp_tb->GetCount();
		CP* cp = dsbob2->cp_tb->GetTable();

		//set shunt working array
		for (i = 0; i < lv_cp; i++)
		{
			ibs = (cp + i)->ibs;
			zbs = (cp + i)->zbs;
			if ((ibs == -1) && (zbs == -1))
				continue;

			if ((ibs == zbs)//0阻抗支路
				|| ((cp + i)->r == 0))
			{
				//将其节点加入特殊处理的行列
				g_zbr[lv_zbr].logic = i;
				g_zbr[lv_zbr++].ibs = ibs;

				continue;
			}

			//需要判断是那种电容器，串联和并联的作用不一样
			if ((ibs != -1) && (zbs != -1))
			{	//属于并联电容器
				dead = (cp + i)->q&LD_OPEN || (bs + ibs)->iisland != g_island || (bs + ibs)->q&BS_OPEN;

				if (!dead)
				{
					g_wbr[lv_wbr].type = _CP;
					g_wbr[lv_wbr].logic = i;
					g_wbr[lv_wbr].ibs = ibs;
					g_wbr[lv_wbr].zbs = zbs;

					//计算容抗
					g_wbr[lv_wbr].r = 0;
					g_wbr[lv_wbr].x = -1 / (((cp + i)->r / 1000) / g_wbase);
					g_wbr[lv_wbr].b = 0;//配电网的b一般为0

					lv_wbr++;
				}
			}
		}
	}



    //read xf data into branch working array	读取变压器绕组支路表
/* lld edit for 10kV 2020-8-4
	if ( (lld->mx_xf > 0) && (lld->mx_tapty > 0))
	{
		//get tapty table
		TAPTY* tapty = dsbob2->tapty_tb->GetTable();

		int lv_xf = dsbob2->xf_tb->GetCount();
		if (lv_xf != 0)
		{
			XF* xf = dsbob2->xf_tb->GetTable();

			for (i = 0; i < lv_xf; i++)
			{
				ibs = (xf + i)->ibs;
				zbs = (xf + i)->zbs;
				iout = (xf + i)->topflags & XF_IOPEN || (bs + ibs)->iisland != g_island;
				zout = (xf + i)->topflags & XF_ZOPEN || (bs + zbs)->iisland != g_island;
				dead = iout && zout;

				if (!dead)
				{
					r = (xf + i)->r1;//根据绕组的电压等级换算表幺值
					x = (xf + i)->x1;

					//determine actual ratio by tap position
					t = 1.0;
					itapty = (xf + i)->itapty;
					iztapty = (xf + i)->iztapty;

					//有抽头量测的情况下需要计算变比
					//变比仅仅影响到绕组支路的r,x,g1,b1,g2,b2的值，与节点的顺序无关
					if (itapty != -1)
					{
						if (((xf + i)->tap != (tapty + itapty)->nom//高压侧抽头不在中间点 
							&& (xf + i)->tap <= (tapty + itapty)->mx //抽头在允许的范围内
							&& (xf + i)->tap >= (tapty + itapty)->mn)
							|| ((xf + i)->ztap != (tapty + iztapty)->nom//低压侧抽头不在中间点 
								&& (xf + i)->ztap <= (tapty + iztapty)->mx //抽头在允许的范围内
								&& (xf + i)->ztap >= (tapty + iztapty)->mn))
						{
							//变比=（中间档位对应的电压+(当前档位-中间档位)*抽头调整步长）/中间档位对应的电压
							t = (xf + i)->kvnom + (xf + i)->kvnom * ((xf + i)->tap - (tapty + itapty)->nom) * (tapty + itapty)->step;
							t /= (xf + i)->zkvnom + (xf + i)->zkvnom * ((xf + i)->ztap - (tapty + iztapty)->nom) * (tapty + iztapty)->step;
							t *= (xf + i)->zkvnom / (xf + i)->kvnom;

							//set non-rating ratio winding array
							//档位调整后相关的绕组的其他参数也要调整
							g_wxf[lv_wxf].ibr = lv_wbr;
							g_wxf[lv_wxf].t = t;
							g_wxf[lv_wxf].g1 = r / (r * r + x * x) / (t * t) * (1.0 - t);
							g_wxf[lv_wxf].b1 = -x / (r * r + x * x) / (t * t) * (1.0 - t);
							g_wxf[lv_wxf].g2 = r / (r * r + x * x) / t * (t - 1.0);
							g_wxf[lv_wxf].b2 = -x / (r * r + x * x) / t * (t - 1.0);
							lv_wxf++;
						}
					}

					///////// add by lld 2009.12.10
					//				r = (xf+i)->r * g_wbase / 10 / 10;
					//				x = (xf+i)->x * g_wbase / 10 / 10;
					///////// add end by lld 2009.12.10

									//set branch working array
					g_wbr[lv_wbr].type = _XF;
					g_wbr[lv_wbr].logic = i;
					g_wbr[lv_wbr].ibs = ibs;
					g_wbr[lv_wbr].zbs = zbs;
					g_wbr[lv_wbr].r = t * r;
					g_wbr[lv_wbr].x = t * x;
					g_wbr[lv_wbr].b = 0.0;
					lv_wbr++;
				}
			}
		}
	}
	
		//get cp table
	if (lld->mx_cp > 0)
	{
		int lv_cp = dsbob2->cp_tb->GetCount();
		CP* cp = dsbob2->cp_tb->GetTable();

		//set shunt working array
		for (i = 0; i < lv_cp; i++)
		{
			ibs = (cp + i)->ibs;
			zbs = (cp + i)->zbs;
			if ((ibs == -1) && (zbs == -1))
				continue;

			if ( (ibs == zbs)//0阻抗支路
				|| ((cp + i)->r==0) )
			{
				//将其节点加入特殊处理的行列
				g_zbr[lv_zbr].logic = i;
				g_zbr[lv_zbr++].ibs = ibs;

				continue;
			}

			//需要判断是那种电容器，串联和并联的作用不一样
			if ((ibs != -1) && (zbs != -1))
			{	//属于并联电容器
				dead = (cp + i)->q&LD_OPEN || (bs + ibs)->iisland != g_island || (bs + ibs)->q&BS_OPEN;

				if (!dead)
				{
					g_wbr[lv_wbr].type = _CP;
					g_wbr[lv_wbr].logic = i;
					g_wbr[lv_wbr].ibs = ibs;
					g_wbr[lv_wbr].zbs = zbs;

					//计算容抗
					g_wbr[lv_wbr].r = 0;
					g_wbr[lv_wbr].x = - 1/ (((cp + i)->r/1000)/ g_wbase);
					g_wbr[lv_wbr].b = 0;//配电网的b一般为0

					lv_wbr++;
				}
			}
		}
	}
*/

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//负荷分配表
int PWRFLOW::OnDistributeLoadData()
{
	int		i/*,j,k,m,n*/;
	int		ibs, dead;
//	int     lv_p,lv_q;
	//int     flag;//开关节点标志
//	int     num[10];
//	char    abb1[5],abb2[9],abbtemp[15];
//	char    abb3[9],abb4[9],abbtemp1[13];
//	double sumPso,sumQso,sumP,sumQ;
//	double sumPcb,sumQcp;
    // get several tables
	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();

	//get load table
	int lv_ld = dsbob2->ld_tb->GetCount();
	LD* ld = dsbob2->ld_tb->GetTable();

	//get cb table
	CB* cb = dsbob2->cb_tb->GetTable();
	int lv_cb = dsbob2->cb_tb->GetCount();
	
	//get digital table
	DIGITAL* digital = dsbob2->digital_tb->GetTable();
	int lv_digital = dsbob2->digital_tb->GetCount();

	//get analog table
	ANALOG* analog = dsbob2->analog_tb->GetTable();
	int lv_analog = dsbob2->analog_tb->GetCount();
	
	//get sec table
	SEC* sec = dsbob2->sec_tb->GetTable();
	int lv_sec = dsbob2->sec_tb->GetCount();

/*
    //通过首端开关量测分配负荷
	//电源总加
    sumPso=sumQso=0;
    for(i = 0; i < lv_wg; i++)
    {
		sumPso += g_wbase * g_wg[i].w;	//有功,mw
		sumQso += g_wbase * g_wg[i].r;	//无功
	}

    sumP=sumQ=0;//负荷总加
    for(i = 0; i < lv_ld; i++)
    {
        ibs = (ld+i)->ibs;
        dead = (ld+i)->q&LD_OPEN || (bs+ibs)->iisland!=g_island || (bs+ibs)->q&BS_OPEN;

        if(!dead)
        {
            //set load working array 
			(ld+i)->w = 0;
			(ld+i)->r = 0;

            sumP += (ld+i)->wm;//kva
            sumQ += (ld+i)->rm;
        }
    }
    
	//根据实测的负荷总加和负荷的额定容量折算出负荷的比例
	//如果有实测的负荷值，用实测的替代，假定5%的线损
	g_scale_P = (sumPso/sumP) * 0.95;
	g_scale_Q = (sumQso/sumQ) * 0.95;
	//将这个写到电源列表里面，以后在计算的时候就可以用这个进行负荷大小的折算
*/

    for(i = 0; i < lv_ld; i++)
    {
        ibs = (ld+i)->ibs;
        dead = (ld+i)->q&LD_OPEN || (bs+ibs)->iisland!=g_island || (bs+ibs)->q&BS_OPEN;

        if(!dead)
        {
			if (lld->g_userid == "main110")
			{
					(ld+i)->w = (ld+i)->wm;
					(ld+i)->r = (ld+i)->rm;
			}
			else
			{
				if ((ld+i)->w  == 0)//只有在没有实际的量测的时候，才能用计算出来的进行替代
					(ld+i)->w = g_scale_P * ((ld+i)->wm);

				if ((ld+i)->r  == 0)
					(ld+i)->r = g_scale_Q * ((ld+i)->rm);
			}
        }
    }
 //高压用户负荷校正
 /*   for (i = 0; i < lv_analog; i++)
    {
       //judge 0204 or 0205 from table analog	
		strncpy(abb1,(analog+i)->id,4);
		abb1[4]='\0';
		if ( strncmp(abb1,"0204",4) == 0)
		{
		   //得到高压负荷编号abb2及p/q指示flag,flag=0表示有功，flag=1表示无功
			int flag1;
			strncpy(abbtemp,(analog+i)->id,14);
            abbtemp[14]='\0';
			for (int k = 5; k < 14; k++)
			{
				abb2[k-5] = abbtemp[k];
			}
			abb2[9]='\0';
			if ((analog+i)->analogType == 0 )
			{
				flag1 = 0;
			}
			else if ((analog+i)->analogType == 1)
			{
				flag1 = 1;
			}
			for ( j = 0; j < lv_ld; j++)
			{
				//得到负荷编号
				strncpy(abbtemp1,(ld+j)->id,13);
					abbtemp1[13]='\0';
                for ( k = 4; k < 13; k++)
                {
					abb3[k-4] = abbtemp1[k];
                }
               //将高压负荷对应有功、无功对应写入
				if (strncmp(abb2,abb3,9) == 0 )
				{
					if ( flag1 == 0 & (analog+i)->quality == 1)
					{
						(ld+j) -> w = (analog+i)->value;
					}
					else if ( flag1 == 1 & (analog+i)->quality == 1)
					{
						(ld+j) -> r = (analog+i)->value;
					}
				}

			}//负荷循环
		}//高压负荷循环
    }
	
   //开关量测校正:通过负荷节点搜索sec表，得到另一端节点,搜索sec表中该节点是否还连有其他负荷，将负荷节点及个数进行统计
   //通过该节点搜索开关表，得到开关编号，进入analog表搜索相关量测信息
	for ( i = 0; i < lv_ld; i++)
	{
		int count = 0;
		char cbno[13],cbtemp[9];
		int ldno = (ld+i)->ind;//负荷节点
		num[count] = (ld+i)->ind;//将负荷节点进行存储，防止开关连接多个负荷情况
		for ( j = 0; j < lv_sec; j++)
		{
			if ( ldno == (sec+j)->ind )
			{
                flag = (sec+j)->iznd; //开关节点
				for (k = 0; k < lv_sec; k++)
                 {
					int dind = (sec+k)->ind;
					int diznd = (sec+k)->iznd;
					if ( flag == dind & diznd != ldno )
					{
						count++;
						num[count] = diznd;
					}
					else if ( flag == diznd & dind != ldno)
					{
						count++;
						num[count] = dind;
					}
                 }//寻找同一开关上负荷节点
			}
		}
		//搜索开关列表，得到编号
       for ( m = 0; m < lv_cb; m++ )
       {
		   if ( flag == (cb+m)->ind || flag == (cb+m)->iznd)
		   {
			  strncmp(cbtemp,(cb+m)->id,13);
			  cbtemp[13] = '\0';
			  for ( n = 4; n < 13; n++)
			  {
				  cbno[n-4] = cbtemp[n];
			  }
			  break;
		   }
       }
	   //搜索量测量
      for ( m = 0; m < lv_analog; m++)
      {
		  strncpy(abbtemp,(analog+i)->id,14);
		  abbtemp[14]='\0';
		  for ( k = 5; k < 14; k++)
		  {
				abb4[k-5] = abbtemp[k];
		  }
			abb4[9]='\0';
		  if ( strncmp(abb4,cbno,9) == 0 & (analog+m)->analogType == 0 & (analog+m)->quality == 1)
		  {
              	  sumPcb = (analog+m)->value;
		  }
		  else if (strncmp(abb4,cbno,9) == 0 & (analog+m)->analogType == 1 & (analog+m)->quality == 1)
		  {
				   sumQcp = (analog+m)->value;
		  }
      }
	  //分配负荷
	  //1、开关只连接一个负荷
	  if ( count == 0)
	  {
		  (ld+i)->w = sumPcb;
		  (ld+i)->r = sumQcp;
	  }
	  //2、开关连接多个负荷
	  else  if ( count != 0)
	  {
		 for ( j = 0; j < count; j++)
		 {
	      for ( n = 0; n < lv_ld; n++)
		  {
			  if ( (ld+n)->ind == num[count] )
			  {
				  (ld+n)->w = sumPcb/count;
				  (ld+n)->r = sumQcp/count;
			  }
		  }
		 }
		  
	  }

	}//首端开关量测校正结束*/
    

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  GetLoadData()				读取负荷表
//如果有量测，就用真实的量测量，如果没有量测，用首端量测进行估计
//如果首末端量测是全的话，在潮流迭代的时候还要处理负荷的分配问题
int PWRFLOW::GetLoadData()
{
	int		i;
	int		ibs, dead,soid;
	char	sLinelog[200],fdid[120];

	double sum_P,sum_Q;

	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();

	//get load table
	int lv_ld = dsbob2->ld_tb->GetCount();
	LD* ld = dsbob2->ld_tb->GetTable();

	sum_P=sum_Q=0;
    //set load working array
    for(i = 0; i < lv_ld; i++)
    {
        ibs = (ld+i)->ibs;
        dead = (ld+i)->q & LD_OPEN || (bs+ibs)->iisland!=g_island || (bs+ibs)->q & BS_OPEN;

        if(!dead)
        {
            //set load working array 
            g_wld[lv_wld].logic = i;
            g_wld[lv_wld].ibs = ibs;
//			sprintf(fdid,"%s",(ld+i)->fdid);

			if (lld->MATH_VOLTAGELEVEL == 10)
			{
				g_wld[lv_wld].w = g_scale_P * ((ld + i)->wm / 1000) / g_wbase;
				sum_P += g_wld[lv_wld].w;

				g_wld[lv_wld].r = g_scale_Q * ((ld + i)->rm / 1000) / g_wbase;
				sum_Q += g_wld[lv_wld].r;
			}
			else
			{
				g_wld[lv_wld].w = (ld + i)->w / g_wbase;
				g_wld[lv_wld].r = (ld + i)->r /g_wbase;
			}

			lv_wld++;
        }
    }
	
	//累计负荷超过20MW，说明重载了，潮流不收敛
	if (g_vl == 0)
		g_vl = 10;
	if ((lld->MATH_VOLTAGELEVEL == 10) && (( ((10/g_vl)*sum_P>0.2) || ((10/g_vl)*sum_Q > 0.05) )))
	{
		soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
		strcpy(fdid , dsbob2->fd_tb->GetDataByLog(soid)->id);		

		sprintf(sLinelog, "error: %s馈线负荷有功累加值超过20MVA，无功累加超过5MVar，请检查。sum_P=%f MW,sum_Q=%f MVar\n",fdid,100*sum_P,100*sum_Q);
		lld->WriteLog(lld->logfile,sLinelog);
	
		//lld->add_globalexception(sLinelog);

		return PWRFLOW_FAIL;
	}

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  GetShuntData()	单端支路，如电容器，电抗器等
int PWRFLOW::GetShuntData()
{
	int		i;
	//虚拟无功节点
	if (cpbss.size() > 0)
	{
		for (i = 0;i < cpbss.size();i++)
		{
			g_wsh[lv_wsh].ibs = cpbss.at(i);
			g_wsh[lv_wsh].r = 0;
			g_wsh[lv_wsh].rnom = (cps.at(i) / 1000) / g_wbase;//输入的时候已经是KVar
			lv_wsh++;
		}
	}

	if (lld->mx_cp == 0)
		return 1;

	int		ibs, zbs,dead;

	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();

	//get cp table
	int lv_cp = dsbob2->cp_tb->GetCount();
	CP* cp = dsbob2->cp_tb->GetTable();

    //set shunt working array
    for(i = 0; i < lv_cp; i++)
    {
        ibs = (cp+i)->ibs;
		zbs = (cp + i)->zbs;
		if ((ibs == -1) && (zbs == -1))
			continue;//属于孤立电容器

		//需要判断是那种电容器，串联和并联的作用不一样
		if ((ibs != -1) && (zbs != -1))
			continue;//属于并联电容器
		
		dead = (cp+i)->q&LD_OPEN || (bs+ibs)->iisland!=g_island || (bs+ibs)->q&BS_OPEN;

        if(!dead)
        {
            //set shunt working array 
            g_wsh[lv_wsh].logic = i;
            g_wsh[lv_wsh].ibs = ibs;
            g_wsh[lv_wsh].r = 0;
            g_wsh[lv_wsh].rnom = ((cp+i)->r/1000) / g_wbase;//输入的时候已经是KVar
            lv_wsh++;
        }
    }

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  OrderBusNumberA()
//前推回推法计算辐射网的潮流
//对母线进行编号
//	Overview:	This subroutine optimizes bus number by a tree structure. If
//				the network is not a radial one, a dummy bus or a loop
//				breakpoint is added for each loop, so as to convert it into
//				a radial network.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::OrderBusNumberA()
{
    int    i, j/*, k*/;
    int    ibs, zbs;
    int    current, total, total0, flag;
    int    wbr[mx_wbr];

    //select slack source and slack bus
	//找到松弛节点(平衡节点),对辐射网而言，无论有多少个发电机，都只有一个平衡节点
    g_slackbs = 0;
    g_slackgen = 0;
    for(i = 0; i < lv_wg; i++)
    {
        if( (g_wg[i].slackavr == 0) || (g_wg[i].slackavr == 4) )
        {
            g_slackgen = i;
            break;
        }
        else if(g_wg[g_slackgen].wmx < g_wg[i].wmx)
        {
            g_slackgen = i;
        }
    }
    g_wg[g_slackgen].slackavr = 0;

    //set out-to-in and in-to-out index for slack bus
	//out为节点优化前，in为节点间优化后,tio得到优化前的编号，toi得到优化后的编号
	//g_wb既存放优化前的编号，也存放优化后的编号
	
	//以辐射网的边为中心来处理
	g_bsmx = lv_wb = lv_wbr + 1;//n=m+1
    
    ibs = g_wg[g_slackgen].ibs;
    g_wb[g_slackbs].tio = ibs;
    g_wb[ibs].toi = g_slackbs;

    //initialize working array
    for(i = 0; i < lv_wbr; i++)
    {
        wbr[i] = 0;//将每个馈线段设为未处理
    }

    //initialize current bus number and total bus number
    current = 1;//得到的新的层所包含的节点总数
    total = 1;//搜索到的所有节点，每找到一个节点，都要将其加上1

    //order bus number from root, layer by layer
	//对母线进行编号，从根节点开始得到逐步的编号
	//搜索主要还是采用二叉树的模式进行
    while(total < lv_wb)//只要还有没有找到的节点，都要一直找下去
    {
        total0 = total;//本层的第一个节点，准备扩展的第一个节点

        //find buses in this new layer
		//每一层扩展都要把所有的支路过一遍，只要是没有处理过的都要进行处理
		//到最后，没有处理的支路越来越少，速度会越来越快
        for( i = 0; i < lv_wbr; i++)
        {
            if(!wbr[i])//如果馈线段未处理
            {
                ibs = g_wbr[i].ibs;
                zbs = g_wbr[i].zbs;

                flag = 0;

				//这个循环表示从本层开始进行节点的循环，也有可能一个也找不到
				//找不到后开挂循环下一个支路
                for(j = total0 - current; j < total0; j++)
                {//j是上一层扩展的节点号
                    if(ibs == g_wb[j].tio)
                    {
                        flag = 1;
                        break;
                    }
                    else if(zbs == g_wb[j].tio)
                    {
                        zbs = ibs;
                        flag = 1;
                        break;
                    }
                }

                if(flag)//有可能一个节点会发现很多下游节点
                {
                    wbr[i] = 1;

					//辐射型配电网不可能形成环
                    //if(loop == loop0)  //this branch does not form a loop
                    {
                        g_wb[total].tio = zbs;	//优化前的编号
                        g_wb[zbs].toi = total;	//优化后的编号
                        g_wb[total].jb = i;		//节点的上级支路
                        g_wb[total].jc = g_wb[ibs].toi;	//节点的上级节点
                        total++;//扩展的节点
                    }
                }
            }
        }

        //get bus number in this new layer
		//每次做一个层，这个层需要循环所有的支路才能得到
        current = total - total0;//得到的新的图层的共有的节点数，用这些节点去扩展其它层
        if(current == 0)
        {
            return PWRFLOW_FAIL;
        }

        //set number of lines connected to the root bus
        if(total0 == 1) g_wb[0].jb = current;
    }

    //reset source, load and line connected buses
    for(i = 0; i < lv_wg; i++)
    {
        ibs = g_wg[i].ibs;
        g_wg[i].ibs = g_wb[ibs].toi;
    }

    for(i = 0; i < lv_wld; i++)
    {
        ibs = g_wld[i].ibs;
        g_wld[i].ibs = g_wb[ibs].toi;
    }

    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;
        g_wbr[i].ibs = g_wb[ibs].toi;
        g_wbr[i].zbs = g_wb[zbs].toi;
    }

    for(i = 0; i < lv_wsh; i++)
    {
        ibs = g_wsh[i].ibs;
        g_wsh[i].ibs = g_wb[ibs].toi;
    }

    //set bus injection power
	//特别注意，电容器不能恒定无功出力，和电压有关，不能在这里进行设置
    for(i = 0; i < lv_wg; i++)
    {
        ibs = g_wg[i].ibs;
        g_wb[ibs].nw += g_wg[i].w;
        g_wb[ibs].nr += g_wg[i].r;
    }

    for(i = 0; i < lv_wld; i++)
    {
        ibs = g_wld[i].ibs;
        g_wb[ibs].nw -= g_wld[i].w;//将负荷变成-号，表示反注入
        g_wb[ibs].nr -= g_wld[i].r;
    }

    //initialize slack and pv bus voltage
	
    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].slackavr==0)//松弛母线，平衡节点
        {
            ibs = g_wg[i].ibs;
			g_wb[ibs].v = g_wg[i].v;
            //g_wb[ibs].v = dsbob2->bs_tb->GetDataByPh(g_wb[ibs].tio)->v;
//			if(g_wb[ibs].v < 0.8 || g_wb[ibs].v > 1.2)
//			{
//				g_wb[ibs].v = 1.05;
//			}
			g_wg[i].vtarget = g_wb[ibs].v;
        }	
    }
	
	//initialize other bus voltage
	for(i = 0; i < lv_wb; i++)
	{
		if(g_wb[i].ja == 0)
		{
			g_wb[i].v = g_wb[g_slackbs].v;
		}
	}

	//g_wb 母线的全局变量

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  OrderBusNumberB()
//母线优化编号，主要是减少消去时的注入元，也就是节点的度最小的先编号，大的后编号
//**静态优化编号：节点度最小的最先编号，相同的随机编号，直到最后，最简单；
//**半动态优化编号：先找一个度最小的节点编号，将这个节点消去，再找最小的度编号，再
//消去,以此类推，直到把所有的节点全部编写完毕。应用最多。
//
//	Overview:	This subroutine optimizes bus number by the static ordering
//				method, i.e, the bus which has fewer connected lines will be
//				given a smaller bus number, the bus which has more connected
//				lines will be given a larger bus number.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::OrderBusNumberB()
{
	int    i, j, k;
	int    ibs, zbs, regbs, nextbs, flag;

	g_bsmx = 0;//在环网中，不存在 n=m+1的判断

	//count number of branches at each bus, get max bus number
	//对每个分支线进行循环，计算首末节点的链接支路数
	for (i = 0; i < lv_wbr; i++)
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;

		g_wb[ibs].ja++;//首节点连接支路数加1
		g_wb[zbs].ja++;

		if (g_bsmx < ibs)//得到最大的母线编号，母线编号都是严格从1开始编号
		{
			g_bsmx = ibs;
		}
		if (g_bsmx < zbs)
		{
			g_bsmx = zbs;
		}
	}

	//count number of buses which have same branches
	//计算节点的度，也就是每个节点连接的支路数量
	//不要在这里把节点的度ja和节点度为X的节点个数混淆jb
	//可能节点1的度为3，g_wb[1].ja=3;度为1的节点个数为100，那么g_wb[1].jb=100；
	for (i = 0; i <= g_bsmx; i++)
	{
		j = g_wb[i].ja;
		if (j > 0)//没有连接的不需要计算
		{
			g_wb[j].jb++;//节点的度为j的节点个数增加一个，ja节点度，jb度为j的节点个数
		}
	}

	//get total number of buses
	//将各个度的节点数进行累加，当然要除去节点度为0的节点
	for (i = 1; i < MXELE; i++)
	{
		lv_wb += g_wb[i].jb;
	}

	if (lv_wb == 0) lv_wb++;//节点一个都没有

	//first position of buses which have same branches，节点最大的度为27
	//有相同度的节点的第一个位置
	for (i = 0; i < MXELE; i++)
	{
		g_wb[i + 1].jb += g_wb[i].jb;
	}

	//select slack bus, slack and regulate generator
	nextbs = lv_wb - 1;
	g_slackbs = nextbs--;//松弛母线，倒数第一个一定要给电源点，剩下的从后向前依次给PQ,PV节点
	g_slackbsnum = 0;//平衡节点的个数

	//如果设置成平衡节点，退出。否则选择最大裕度的发电机设置为平衡节点。
	g_slackgen = 0;//松弛发动机，主电源节点
	//可能存在多个平衡节点
	for (i = 0; i < lv_wg; i++)
	{
		if (g_wg[i].slackavr == 0)//主平衡节点
		{
			g_slackgen = i;
			g_slackbsnum++;
			break;
		}
		else if (g_wg[g_slackgen].wmx < g_wg[i].wmx)
		{
			g_slackgen = i;
		}
	}

	//set out-to-in and in-to-out index for slack bus
	ibs = g_wg[g_slackgen].ibs;
	g_wb[g_slackbs].tio = ibs;//从内到外，从优化后看优化前，也就是优化前的编号
	g_wb[ibs].toi = g_slackbs;
	k = g_wb[ibs].ja;
	if (k == 0)//电源点的度为0
	{
		return 0;
	}

	for (j = k; j < MXELE; j++)
	{
		g_wb[j].jb--;//将度的个数减掉1
	}
	g_wb[ibs].ja = 0;//电源点的度归0，借用结束

	//其它的平衡节点
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		if (ibs == g_wg[g_slackgen].ibs)//主平衡节点
			continue;

		k = g_wb[ibs].ja;//ja为0表示是电源点（松弛节点）
		if (g_wg[i].slackavr == 0)//pv节点需要在计算前就指定，这样就不用判断了
		{
			g_wb[nextbs].tio = ibs;
			g_wb[ibs].toi = nextbs--;
			for (j = k; j < MXELE; j++)
			{
				g_wb[j].jb--;
			}
			g_wb[ibs].ja = 0;
			g_slackbsnum++;
		}
	}

	//set out-to-in and in-to-out index for p-v bus
	//一般情况下都没有p-v节点，加分布式电源后可能会有，但也不一定
	g_pvbsnum = 0;
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		k = g_wb[ibs].ja;//ja为0表示是电源点（松弛节点）

		//if(k != 0 && g_wg[i].slackavr == -1)
		if (g_wg[i].slackavr == -1)//pv节点需要在计算前就指定，这样就不用判断了
		{
			flag = 0;
			for (j = 0; j < lv_wg; j++)
			{
				//不能是自己，不能是平衡节点，不能是一个母线上的设备
				if (j != i && g_wg[j].slackavr != 0 && g_wg[j].ibs == g_wg[i].ibs)
				{
					flag = 1;
					break;
				}
			}
			if (flag)
			{
				g_wg[i].slackavr = 1; //设为PQ节点，lld 2018-7-9
			}
			else
			{
				g_wb[nextbs].tio = ibs;
				g_wb[ibs].toi = nextbs--;
				for (j = k; j < MXELE; j++)
				{
					g_wb[j].jb--;
				}
				g_wb[ibs].ja = 0;
				g_pvbsnum++;
			}
		}
	}

	//set out-to-in and in-to-out index for p-q bus
	//对PQ节点进行编号
	for (i = 0; i <= g_bsmx; i++)
	{
		k = g_wb[i].ja;//电源点在前面已经将度归0了，pv节点也是
		if (k != 0)
		{
			nextbs = g_wb[k - 1].jb++;//前面k加过1，这里回归，每一个度的节点都在前面预留了空位
			g_wb[nextbs].tio = i;
			g_wb[i].toi = nextbs;
			g_wb[i].ja = 0;//已经编过号了，不需要再处理
		}
	}

	//reset g_wb[i].jb to zero，jb是度的个数，借用变量，回归
	for (i = 0; i <= g_bsmx; i++)
	{
		g_wb[i].jb = 0;
	}

	//reset generator, load and line connected buses
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		regbs = g_wg[i].regbs;//参考母线，准备回写的时候用
		g_wg[i].ibs = g_wb[ibs].toi;//新的发电机（电源点）编号
		g_wg[i].regbs = g_wb[regbs].toi;//新的发电机在原来母线表中的物理位置
	}

	for (i = 0; i < lv_wld; i++)
	{
		ibs = g_wld[i].ibs;
		g_wld[i].ibs = g_wb[ibs].toi;
	}

	for (i = 0; i < lv_wbr; i++)
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;
		g_wbr[i].ibs = g_wb[ibs].toi;
		g_wbr[i].zbs = g_wb[zbs].toi;
	}

	for (i = 0; i < lv_wsh; i++)
	{
		ibs = g_wsh[i].ibs;
		g_wsh[i].ibs = g_wb[ibs].toi;
	}

	//mark slack, p-v and p-q bus in bus working array
	for (i = 0; i < lv_wb; i++)
	{
		g_wb[i].ja = 0;  //p-q
	}

	for (i = 0; i < lv_wg; i++)  //p-v，将以前p-q一起给设的值进行修改
	{
		if (g_wg[i].slackavr == -1)
		{
			ibs = g_wg[i].ibs;
			g_wb[ibs].ja = -1;
		}
	}
	//其它平衡节点
	for (i = 0; i < lv_wg; i++)  //p-v，将以前p-q一起给设的值进行修改
	{
		if (g_wg[i].slackavr == 0)
		{
			ibs = g_wg[i].ibs;
			g_wb[ibs].ja = 1;
		}
	}

	if (g_wg[g_slackgen].slackavr == 4)
		g_wg[g_slackgen].slackavr = 0;

	g_wb[g_slackbs].ja = 1;  //slack，将以前p-q一起给设的值进行修改

	//set bus injection power
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;//编号后的
		g_wb[ibs].nw += g_wg[i].w;
		g_wb[ibs].nr += g_wg[i].r;
	}

	for (i = 0; i < lv_wld; i++)
	{
		ibs = g_wld[i].ibs;
		g_wb[ibs].nw -= g_wld[i].w;//潮流计算时的负荷注入为-
		g_wb[ibs].nr -= g_wld[i].r;
	}

	//initialize slack and pv bus voltage
	for (i = 0; i < lv_wg; i++)
	{
		if ((g_wg[i].slackavr == 0) || (g_wg[i].slackavr == -1))//-1也是要通过的
		{
			ibs = g_wg[i].ibs;
			g_wb[ibs].v = g_wg[i].v;
			//            g_wb[ibs].v = dsbob2->bs_tb->GetDataByPh(g_wb[ibs].tio)->v;
			//			if(g_wb[ibs].v < 0.8 || g_wb[ibs].v > 1.2) //有可能出现20kV和35kV的专线情况
			//			{
			//				g_wb[ibs].v = 1.05;
			////				g_wb[ibs].v = 1.0;
			//			}
			g_wg[i].vtarget = g_wb[ibs].v;
		}
	}

	//initialize other bus voltage
	for (i = 0; i < lv_wb; i++)
	{
		if (g_wb[i].ja == 0)//电源点已经==1，pv已经==-1
		{
			g_wb[i].v = g_wb[g_slackbs].v;//和电源点享有同样的初值
		}
	}

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  OrderBusNumberC()
//母线优化编号，主要是减少消去时的注入元，也就是节点的度最小的先编号，大的后编号
//**静态优化编号：节点度最小的最先编号，相同的随机编号，直到最后，最简单；
//**半动态优化编号：先找一个度最小的节点编号，将这个节点消去，再找最小的度编号，再
//消去,以此类推，直到把所有的节点全部编写完毕。应用最多。
//
//	Overview:	This subroutine optimizes bus number by the static ordering
//				method, i.e, the bus which has fewer connected lines will be
//				given a smaller bus number, the bus which has more connected
//				lines will be given a larger bus number.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::OrderBusNumberC()//半动态优化编号法
{
//    int    i, j, k;
//    int    ibs, zbs, regbs, nextbs, flag;
//
//    g_bsmx = 0;//在环网中，不存在 n=m+1的判断
//
//    //count number of branches at each bus, get max bus number
//	//对每个分支线进行循环，计算首末节点的链接支路数,也就是节点的度
//    for(i = 0; i < lv_wbr; i++)
//    {
//        ibs = g_wbr[i].ibs;
//        zbs = g_wbr[i].zbs;
//
//        g_wb[ibs].ja++;//首节点连接支路数加1
//        g_wb[zbs].ja++;
//
//        if(g_bsmx < ibs)//得到最大的母线编号，母线编号都是严格从1开始编号
//        {
//            g_bsmx = ibs;
//        }
//        if(g_bsmx < zbs)
//        {
//            g_bsmx = zbs;
//        }
//    }
//
//    //count number of buses which have same branches
//	//计算节点的度，也就是每个节点连接的支路数量
//    for(i = 0; i <= g_bsmx; i++)
//    {
//        j = g_wb[i].ja;
//        if(j > 0)//没有连接的不需要计算
//        {
//            g_wb[j].jb++;//节点的度为j的节点个数增加一个，ja节点度，jb度为j的节点个数
//        }
//    }
//
//    //get total number of buses
//    for(i = 1; i < MXELE; i++)
//    {
//        lv_wb += g_wb[i].jb;
//    }
//
//	if(lv_wb == 0) lv_wb++;//节点一个都没有
//




    return PWRFLOW_SUCCEED;

}

///////////////////////////////////////////////////////////////////////////////
//  FormAdmittance()
//  导纳矩阵形成：上三角阵，存元素的行和列，对角元素，非对角元素，行号，列号，列连接的数组
//对角元素相当于做偏移量
//  Overview:	This subroutine forms the upper trangular admittance matrix.
//				It sets up the diagonal position array (g_yu[i].d), the row
//				position array (g_yu[i].i), the column position array
//				(g_yu[i].j), and the column linking array (g_yu[i].p).
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::FormAdmittance()
{
    int    i, j, k, m, n;
    int    jmx, jmn;
    int    ibs, zbs;
    int    jw[mx_wb];
    double r, x, b;
//    double so_r,so_x;

    //initialize upper trangular matrix
    for(i = 0; i < mx_yu; i++)
    {
        g_yu[i].i = 0;//上三角的行
        g_yu[i].j = 0;//上三角的列
        g_yu[i].p = 0;//元素对应的链
        g_yu[i].d = 0;//对角元素位置
        g_yu[i].g = 0.0;//导纳的实部
        g_yu[i].b = 0.0;//导纳的虚部
    }
    lv_yu = 0;

    //determine positions of diagonal elements，矩阵的阶数和支路数一样多
	//确定对角元素的位置
	for(i = 0; i < lv_wbr; i++)//确定对角元素的个数
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;

		//取最小的母线编号出来，此编号不是分层编号
        if(ibs > zbs) ibs = zbs;

        g_yu[ibs+1].d++;//确定节点连接其他节点的数量，相当于互导纳的数量
    }

    for(i = 1; i < lv_wb; i++)//确定对角元素在数组中的位置
    {
		//下一个对角元素的位置，后面在查找的时候可以用两个对角元素之间的差值来读取
        j = g_yu[i].d + g_yu[i-1].d + 1;
        g_yu[i].d = j;
        g_yu[j].i = i;//每个节点对应一行
        g_yu[j].j = i;//对角元素的行和列相等
    }

    //determine number of elements in the matrix
	//最后的那个对角元素就是整个数组的个数，其中不含0元素
    lv_yu = g_yu[lv_wb-1].d + 1;

    //determine row positions of elements
	//确定每个元素行的位置
    for(i = 0; i < lv_wb; i++)
    {
        jmn = g_yu[i].d + 1;//下一个对角元素
        jmx = g_yu[i+1].d;

		//本行到下一行之间所有列元素非0元素
        for(k = jmn; k < jmx; k++)//两个对角元素之间的所有元素都与这个对角享有同样的行号
        {
            g_yu[k].i = i;
        }
    }

    //determine column positions of elements
	//确定每个元素列的位置，非对角元素
	//循环支路
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;

		//交换节点号，从小到大，这里只需要处理上三角，所以可以这样排列
        if(zbs < ibs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

        jmn = g_yu[ibs].d + 1;
        jmx = g_yu[ibs+1].d;

		//检索本行全部的非对角元素，线索为两对角元素之间
        for(k = jmn; k < jmx; k++)
        {
            j = g_yu[k].j;//初始的为0

            if(j == 0)//第一次出现
            {
                g_yu[k].j = zbs;//将末端节点直接给
                break;
            }
            else if(j > zbs)//修改其他的节点列
            {
                m = jmx - 1;
                while(m > k)
                {
                    g_yu[m].j = g_yu[m-1].j;
                    m--;
                }
                g_yu[k].j = zbs;
                break;
            }
        }
    }

    //add the column linking to upper trangular matrix
	//列的链表，每一列从第一个开始，一直到对角元素的排列顺序
	
	//初始化
    for(k = 0; k < lv_wb; k++)
    {
        j = g_yu[k].d;//对角元素的位置
        jw[k] = j;      //working array，临时数组，存节点的对角位置
        g_yu[j].p = j;  //point to itself，每列都暂时指向自己，从对角元素开始进行链表的维护
    }

    i = 0;//逐行进行处理
    for(k = 0; k < lv_yu; k++)
    {
        if(k >= g_yu[i+1].d) i++;  //next row

        j = g_yu[k].j;

        if(j != i)  //j > i 上三角的非对角元素的列一定大于行
        {
            m = jw[j];
            n = g_yu[m].p;
            g_yu[m].p = k;//做链表，便于检索
            g_yu[k].p = n;
            jw[j] = k;
        }
    }

    //set elements of upper trangular matrix
	//前面处理每个元素的位置，链表，下面处理元素的内容
	//lv_wg = 1;
	for(i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		jmn = g_yu[ibs].d;//
		
		//用短路容量来估计电源点的等效阻抗
		//得到故障点的等值电阻和电抗,需要成首端电压的标幺值

		//上级电网的阻抗的等值，线电压和线电流
		//16kA ~ 25kA  2.91~4.55
		//z=v^2 / ss; x = 0.96 * z; r=0.3 * x;
//		r = 0.3 * 0.96 * g_wg[i].v * g_wg[i].v * 1.732 / g_wg[i].ss;
//		x =       0.96 * g_wg[i].v * g_wg[i].v * 1.732 / g_wg[i].ss;
		if (g_wg[i].ss > 0)
		{
			//g_wg[i].ss *= (10 * 1.732)/100;
			//x = 0.957826 * g_wg[i].v * g_wg[i].v / (g_wg[i].ss);
			//r = 0.1 * x;
			////r = 0.3 * 0.957826 * g_wg[i].v * g_wg[i].v / (g_wg[i].ss);
			//g_yu[jmn].g += r / (r * r + x * x);
			//g_yu[jmn].b += -x / (r * r + x * x);
		
			//只考虑电抗
			r = 0;
			x = 1/g_wg[i].ss;
			g_yu[jmn].g += r / (r * r + x * x);
			g_yu[jmn].b += -x / (r * r + x * x);
			//g_yu[jmn].g += 0;
			//g_yu[jmn].b += -(1.732*g_wg[i].ss)/10;	
		
		}
		else
		{
			r = 0;
			x = 0;
			g_yu[jmn].g += 0;
			g_yu[jmn].b += 0;
		}



//		r = 0.1162;
//		x = 0.3872;
//
	}
	
	for(i = 0; i < lv_wsh; i++)
	{
		ibs = g_wsh[i].ibs;
		jmn = g_yu[ibs].d;//无功补偿挂在节点上，直接修改节点的b值
		g_yu[jmn].b += g_wsh[i].rnom;
	}

	//对变压器进行处理，对每个变压器支路，需要将其阻抗加到相应的节点上
//	for(i = 0; i < lv_wxf; i++)
//	{
//        ibs = g_wbr[g_wxf[i].ibr].ibs;
//        zbs = g_wbr[g_wxf[i].ibr].zbs;
//
//        jmn = g_yu[ibs].d;
//        jmx = g_yu[zbs].d;
//
//		//首末节点都要加上绕组的
//        g_yu[jmn].g += g_wxf[i].g1;
//        g_yu[jmn].b += g_wxf[i].b1;
//        g_yu[jmx].g += g_wxf[i].g2;
//        g_yu[jmx].b += g_wxf[i].b2;
//	}


//	//直接对负荷进行处理
//	for(i = 0; i < lv_wld; i++)
//	{
//        ibs = g_wld[i].ibs;//负荷点的计算母线号
//        jmn = g_yu[ibs].d;
//
//		//通过负荷点的有功和无功可以计算出对应的导纳
//		e = g_wb[ibs].v * cos(g_wb[ibs].a);//实部
//		f = g_wb[ibs].v * sin(g_wb[ibs].a);
//		c = g_wld[i].w;
//		d = g_wld[i].r;
//
//		//load impedance,节点上有负荷
//		//仅仅在负荷点处短路才有可能
//		//恒阻抗 S / v^2
//		if(c != 0.0 || d != 0.0)
//		{
////			g_yu[jmn].g += c / (a * a + e * e);
////			g_yu[jmn].b += -d / (a * a + e * e);
//
//			g_yu[jmn].g += (c*(e*e - f*f) + 2*d*e*f) / (e * e - f * f + 4*e*e*f*f);
//			g_yu[jmn].b += (d*(e*e - f*f) - 2*c*e*f) / (e * e - f * f + 4*e*e*f*f);
//		}
//	}

    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;
        r = g_wbr[i].r;
        x = g_wbr[i].x;
        b = g_wbr[i].b;

        if(ibs > zbs)//处理上三角
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

        jmn = g_yu[ibs].d;
        jmx = g_yu[zbs].d;

		//自导纳
        g_yu[jmn].g +=  r / (r * r + x * x);
        g_yu[jmn].b += -x / (r * r + x * x) + b;
        g_yu[jmx].g +=  r / (r * r + x * x);
        g_yu[jmx].b += -x / (r * r + x * x) + b;

        jmn++;
        jmx = g_yu[ibs+1].d;

		//互导纳
        for(k = jmn; k < jmx; k++)
        {
            if(g_yu[k].j == zbs)
            {
                g_yu[k].g -=  r / (r * r + x * x);
                g_yu[k].b -= -x / (r * r + x * x);
                break;
            }
        }
    }

//发电机的对侧节点进行处理
//	g_yu[8].g += -0.51;
//	g_yu[8].b += 1.7;

	//ofstream outfile("positive.txt", ios::out);

	//outfile << "   " << endl;
	//outfile << "i " << "d   " << "i   " << "j   " << "p    " << "g   " << "b     " << endl;
	//for (int i = 0; i < 10; i++)
	//{
	//	outfile << i << " " << g_yu[i].d << "   " << g_yu[i].i << "   " << g_yu[i].j << "   " << g_yu[i].p << "   " << g_yu[i].g << "   " << g_yu[i].b << endl;
	//}

	//outfile.close();

    return PWRFLOW_SUCCEED;
}

//零序
int PWRFLOW::FormAdmittance0()
{
	int    i, j, k, m, n;
	int    jmx, jmn;
	int    ibs, zbs;
	int    jw[mx_wb];
	double r, x, b,e,f,d,c;
	//    double so_r,so_x;

		//initialize upper trangular matrix
	for (i = 0; i < mx_yu; i++)
	{
		g_yu[i].i = 0;//上三角的行
		g_yu[i].j = 0;//上三角的列
		g_yu[i].p = 0;//元素对应的链
		g_yu[i].d = 0;//对角元素位置
		g_yu[i].g = 0.0;//导纳的实部
		g_yu[i].b = 0.0;//导纳的虚部
	}
	lv_yu = 0;

	//determine positions of diagonal elements，矩阵的阶数和支路数一样多
	//确定对角元素的位置
	for (i = 0; i < lv_wbr; i++)//确定对角元素的个数
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;

		//取最小的母线编号出来，此编号不是分层编号
		if (ibs > zbs) ibs = zbs;

		g_yu[ibs + 1].d++;//确定节点连接其他节点的数量，相当于互导纳的数量
	}

	for (i = 1; i < lv_wb; i++)//确定对角元素在数组中的位置
	{
		//下一个对角元素的位置，后面在查找的时候可以用两个对角元素之间的差值来读取
		j = g_yu[i].d + g_yu[i - 1].d + 1;
		g_yu[i].d = j;
		g_yu[j].i = i;//每个节点对应一行
		g_yu[j].j = i;//对角元素的行和列相等
	}

	//determine number of elements in the matrix
	//最后的那个对角元素就是整个数组的个数，其中不含0元素
	lv_yu = g_yu[lv_wb - 1].d + 1;

	//determine row positions of elements
	//确定每个元素行的位置
	for (i = 0; i < lv_wb; i++)
	{
		jmn = g_yu[i].d + 1;//下一个对角元素
		jmx = g_yu[i + 1].d;

		//本行到下一行之间所有列元素非0元素
		for (k = jmn; k < jmx; k++)//两个对角元素之间的所有元素都与这个对角享有同样的行号
		{
			g_yu[k].i = i;
		}
	}

	//determine column positions of elements
	//确定每个元素列的位置，非对角元素
	//循环支路
	for (i = 0; i < lv_wbr; i++)
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;

		//交换节点号，从小到大，这里只需要处理上三角，所以可以这样排列
		if (zbs < ibs)
		{
			k = ibs;
			ibs = zbs;
			zbs = k;
		}

		jmn = g_yu[ibs].d + 1;
		jmx = g_yu[ibs + 1].d;

		//检索本行全部的非对角元素，线索为两对角元素之间
		for (k = jmn; k < jmx; k++)
		{
			j = g_yu[k].j;//初始的为0

			if (j == 0)//第一次出现
			{
				g_yu[k].j = zbs;//将末端节点直接给
				break;
			}
			else if (j > zbs)//修改其他的节点列
			{
				m = jmx - 1;
				while (m > k)
				{
					g_yu[m].j = g_yu[m - 1].j;
					m--;
				}
				g_yu[k].j = zbs;
				break;
			}
		}
	}

	//add the column linking to upper trangular matrix
	//列的链表，每一列从第一个开始，一直到对角元素的排列顺序

	//初始化
	for (k = 0; k < lv_wb; k++)
	{
		j = g_yu[k].d;//对角元素的位置
		jw[k] = j;      //working array，临时数组，存节点的对角位置
		g_yu[j].p = j;  //point to itself，每列都暂时指向自己，从对角元素开始进行链表的维护
	}

	i = 0;//逐行进行处理
	for (k = 0; k < lv_yu; k++)
	{
		if (k >= g_yu[i + 1].d) i++;  //next row

		j = g_yu[k].j;

		if (j != i)  //j > i 上三角的非对角元素的列一定大于行
		{
			m = jw[j];
			n = g_yu[m].p;
			g_yu[m].p = k;//做链表，便于检索
			g_yu[k].p = n;
			jw[j] = k;
		}
	}

	//变电站母线的中心点需要有接地
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		jmn = g_yu[ibs].d;//

		if (g_wg[i].r0 > 0)//低电阻接地
		{
			//只考虑电抗
			r = g_wg[i].r0;
			x = g_wg[i].x0;
			g_yu[jmn].g += r / (r * r + x * x);
			g_yu[jmn].b += -x / (r * r + x * x);
		}
		else
		{
			r = 0;
			x = 0;
			g_yu[jmn].g += 0;
			g_yu[jmn].b += 0;
		}
	}

	//set elements of upper trangular matrix
	for (i = 0; i < lv_wbr; i++)
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;
		r = g_wbr[i].r0;
		x = g_wbr[i].x0;
		b = g_wbr[i].b0;

		if (ibs > zbs)//处理上三角
		{
			k = ibs;
			ibs = zbs;
			zbs = k;
		}

		jmn = g_yu[ibs].d;
		jmx = g_yu[zbs].d;

		//自导纳
		g_yu[jmn].g += r / (r * r + x * x);
		g_yu[jmn].b += -x / (r * r + x * x) + b;
		g_yu[jmx].g += r / (r * r + x * x);
		g_yu[jmx].b += -x / (r * r + x * x) + b;

		//对地支路和变压器支路没有互导纳
		if (g_wbr[i].type == _GRDSEC)
			continue;

		jmn++;
		jmx = g_yu[ibs + 1].d;

		//互导纳
		for (k = jmn; k < jmx; k++)
		{
			if (g_yu[k].j == zbs)
			{
				g_yu[k].g -= r / (r * r + x * x);
				g_yu[k].b -= -x / (r * r + x * x);
				break;
			}
		}
	}

	//发电机的对侧节点进行处理
	//	g_yu[8].g += -0.51;
	//	g_yu[8].b += 1.7;

	//ofstream outfile("zero.txt", ios::out);

	//outfile << "   " << endl;
	//outfile << "i " << "d   " << "i   " << "j   " << "p    " << "g   " << "b     " << endl;
	//for (int i = 0; i < 10; i++)
	//{
	//	outfile << i << " " << g_yu[i].d << "   " << g_yu[i].i << "   " << g_yu[i].j << "   " << g_yu[i].p << "   " << g_yu[i].g << "   " << g_yu[i].b << endl;
	//}

	//outfile.close();

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  ConstructSM()
//
//  Overview:	This subroutine constructs senstivity matrix for loop breakpoints
//				and pv bus breakpoints. More than one loop breakpoint or one pv
//				bus breakpoint is considered.
//
///////////////////////////////////////////////////////////////////////////////
/*
int PWRFLOW::ConstructSM()
{

    int    i, j, k, kk, m, n, p1, p2, flag;
    int    ibs, zbs;
    int    k1[mx_rx], k2[mx_rx];
    int    wb1[mx_rx][mx_wb], wb2[mx_rx][mx_wb];
    double r, x;

    //determine reactance matrix size
    lv_rx = g_loopnum + g_pvbsnum;
    if(lv_rx > mx_rx)
    {
        return PWRFLOW_FAIL;
    }

    //find constituent bus and branch for each loop breakpoint
    for(i = 0; i < g_loopnum; i++)
    {
        ibs = g_wb[i].jc;
        zbs = g_wb[ibs].ja;

        wb1[i][0] = ibs;
        wb2[i][0] = zbs;

        k1[i] = 0;
        k2[i] = 0;

        while(wb1[i][k1[i]] != 0)  //g_slackbs == 0
        {
            ibs = wb1[i][k1[i]];
            j = g_wb[ibs].jb;
            if(ibs == g_wbr[j].ibs)
            {
                k1[i]++;
                wb1[i][k1[i]] = g_wbr[j].zbs;
            }
            else
            {
                k1[i]++;
                wb1[i][k1[i]] = g_wbr[j].ibs;
            }
        }

        while(wb2[i][k2[i]] != 0)
        {
            ibs = wb2[i][k2[i]];
            j = g_wb[ibs].jb;
            if(ibs == g_wbr[j].ibs)
            {
                k2[i]++;
                wb2[i][k2[i]] = g_wbr[j].zbs;
            }
            else
            {
                k2[i]++;
                wb2[i][k2[i]] = g_wbr[j].ibs;
            }
        }

        while(wb1[i][k1[i]-1] == wb2[i][k2[i]-1])
        {
            k1[i]--;
            k2[i]--;
        }
    }

    //find constituent bus and branch for each pv breakpoint
    for(i = g_loopnum; i < g_loopnum + g_pvbsnum; i++)
    {
        ibs = g_wb[i].jc;

        wb1[i][0] = ibs;

        k1[i] = 0;
        k2[i] = 0;

        while(wb1[i][k1[i]] != 0)
        {
            ibs = wb1[i][k1[i]];
            j = g_wb[ibs].jb;
            if(ibs == g_wbr[j].ibs)
            {
                k1[i]++;
                wb1[i][k1[i]] = g_wbr[j].zbs;
            }
            else
            {
                k1[i]++;
                wb1[i][k1[i]] = g_wbr[j].ibs;
            }
        }
    }

    //form reactance matrix
    for(i = 0; i < lv_rx; i++)
    {
        r = 0.0;
        x = 0.0;

        for(k = 0; k < k1[i]; k++)
        {
            ibs = wb1[i][k];
            j = g_wb[ibs].jb;
            r += g_wbr[j].r;
            x += g_wbr[j].x;
        }

        for(k = 0; k < k2[i]; k++)
        {
            ibs = wb2[i][k];
            j = g_wb[ibs].jb;
            r += g_wbr[j].r;
            x += g_wbr[j].x;
        }

        g_rx[i][i].r = r;
        g_rx[i][i].x = x;

        for(j = i + 1; j < lv_rx; j++)
        {
            r = 0.0;
            x = 0.0;

            flag = 0;
            for(k = 0; k < k1[j]; k++)
            {
                ibs = wb1[j][k];

                for(kk = 0; kk < k1[i]; kk++)
                {
                    if(ibs == wb1[i][kk])
                    {
                        flag = 1;
                        p1 = k;
                        p2 = kk;
                        while(p1 < k1[j] && p2 < k1[i] && wb1[j][p1+1] == wb1[i][p2+1])
                        {
                            m = wb1[j][p1];
                            n = g_wb[m].jb;
                            r += g_wbr[n].r;
                            x += g_wbr[n].x;
                            p1++;
                            p2++;
                        }
                        break;
                    }
                }
                if(flag) break;
            }

            flag = 0;
            for(k = 0; k < k1[j]; k++)
            {
                ibs = wb1[j][k];

                for(kk = 0; kk < k2[i]; kk++)
                {
                    if(ibs == wb2[i][kk])
                    {
                        flag = 1;
                        p1 = k;
                        p2 = kk;
                        while(p1 < k1[j] && p2 < k2[i] && wb1[j][p1+1] == wb2[i][p2+1])
                        {
                            m = wb1[j][p1];
                            n = g_wb[m].jb;
                            r -= g_wbr[n].r;
                            x -= g_wbr[n].x;
                            p1++;
                            p2++;
                        }
                        break;
                    }
                }
                if(flag) break;
            }

            flag = 0;
            for(k = 0; k < k2[j]; k++)
            {
                ibs = wb2[j][k];

                for(kk = 0; kk < k2[i]; kk++)
                {
                    if(ibs == wb2[i][kk])
                    {
                        flag = 1;
                        p1 = k;
                        p2 = kk;
                        while(p1 < k2[j] && p2 < k2 [i] && wb2[j][p1+1] == wb2[i][p2+1])
                        {
                            m = wb2[j][p1];
                            n = g_wb[m].jb;
                            r += g_wbr[n].r;
                            x += g_wbr[n].x;
                            p1++;
                            p2++;
                        }
                        break;
                    }
                }
                if(flag) break;
            }

            flag = 0;
            for(k = 0; k < k2[j]; k++)
            {
                ibs = wb2[j][k];

                for(kk = 0; kk < k1[i]; kk++)
                {
                    if(ibs == wb1[i][kk])
                    {
                        flag = 1;
                        p1 = k;
                        p2 = kk;
                        while(p1 < k2[j] && p2 < k1[i] && wb2[j][p1+1] == wb1[i][p2+1])
                        {
                            m = wb2[j][p1];
                            n = g_wb[m].jb;
                            r -= g_wbr[n].r;
                            x -= g_wbr[n].x;
                            p1++;
                            p2++;
                        }
                        break;
                    }
                }
                if(flag) break;
            }

            g_rx[i][j].r = r;
            g_rx[i][j].x = x;
            g_rx[j][i].r = r;
            g_rx[j][i].x = x;
        }
    }

    return PWRFLOW_SUCCEED;
}*/


///////////////////////////////////////////////////////////////////////////////
//  CorrectBPPower()
//
//	Overview:	This subroutine corrects breakpoint power injection by the
//				breakpoint voltage mismatch and the sensivity matrix.
//
///////////////////////////////////////////////////////////////////////////////
/*
int PWRFLOW::CorrectBPPower()
{
    int    i, j, k, m;
    int    lv_sm, p[mx_rx];
    double wa[2 * mx_rx], a;
    double sm[(1 + 2 * mx_rx) * mx_rx], dx[2 * mx_rx], dv[2 * mx_rx];

    //determine sensivity matrix size
    lv_sm = 2 * g_loopnum + g_pvbsnum;

    if(lv_sm == 0)
    {
        return PWRFLOW_SUCCEED;
    }

    for(k = 0; k < g_loopnum; k++)
    {
        i = g_wb[k].jc;
        j = g_wb[i].ja;

        dv[k] = g_wb[j].v - g_wb[i].v;
        dv[k+g_loopnum+g_pvbsnum] = g_wb[j].a - g_wb[i].a;
    }
 
    for(k = g_loopnum; k < g_loopnum + g_pvbsnum; k++)
    {
        i = g_wb[k].jc;
        j = g_wb[i].ja;
        
        if(g_wg[j].kswitch == 0)
        {
            dv[k] = g_wg[j].vtarget - g_wb[i].v;
        }
        else
        {
            dv[k] = 0.0;
        }
    }

    p[0] = 0;
    for(i = 0; i < lv_sm; i++)
    {
        p[i+1] = p[i] + lv_sm - i;
    }

    for(i = 0; i < lv_sm; i++)
    {
        //read sensivity matrix into working array
        if(i < g_loopnum + g_pvbsnum)
        {
            for(j = 0; j < g_loopnum + g_pvbsnum; j++)
            {
                wa[j] = g_rx[i][j].x;
            }
            for(j = g_loopnum + g_pvbsnum; j < lv_sm; j++)
            {
                wa[j] = g_rx[i][j - g_loopnum - g_pvbsnum].r;
            }
        }
        else
        {
            for(j = 0; j < g_loopnum + g_pvbsnum; j++)
            {
                wa[j] = -g_rx[i - g_loopnum - g_pvbsnum][j].r;
            }
            for(j = g_loopnum + g_pvbsnum; j < lv_sm; j++)
            {
                wa[j] = g_rx[i - g_loopnum - g_pvbsnum][j - g_loopnum - g_pvbsnum].x;
            }
        }

        //Gauss elimitation
        for(j = 0; j < i; j++)
        {
            if(wa[j] != 0.0)
            {
                k = p[j];
                a = wa[j] / sm[k];
                wa[j] = 0.0;
                for(m = j + 1; m < lv_sm; m++)
                {
                    wa[m] -= a * sm[m-j+k];
                }
                dv[i] -= a * dv[j];
            }
        }

        //write upper trangular matrix into working array
        k = p[i];
        for(j = i; j < lv_sm; j++)
        {
            sm[j-i+k] = wa[j];
        }
    }

    for(i = lv_sm - 1; i >= 0; i--)
    {
        dx[i] = dv[i];

        k = p[i];

        for(j = i + 1; j < lv_sm; j++)
        {
            dx[i] -= sm[j-i+k] * dx[j];
        }

        dx[i] /= sm[k];
    }

    for(k = 0; k < g_loopnum; k++)
    {
        i = g_wb[k].jc;
        j = g_wb[i].ja;

        g_wb[i].nw += dx[k+g_loopnum+g_pvbsnum];
        g_wb[i].nr += dx[k];
        g_wb[j].nw -= dx[k+g_loopnum+g_pvbsnum];
        g_wb[j].nr -= dx[k];
    }

    for(k = g_loopnum; k < g_loopnum + g_pvbsnum; k++)
    {
        i = g_wb[k].jc;
        j = g_wb[i].ja;
        
        if(g_wg[j].kswitch == 0)
        {
            if(g_wg[j].r + dx[k] > g_wg[j].rmx)
            {
                g_wb[i].nr += g_wg[j].rmx - g_wg[j].r;
                g_wg[j].r = g_wg[j].rmx;
                g_wg[j].kswitch = 1; 
            }
            else if(g_wg[j].r + dx[k] < g_wg[j].rmn)
            {
                g_wb[i].nr += g_wg[j].rmn - g_wg[j].r;
                g_wg[j].r = g_wg[j].rmn;
                g_wg[j].kswitch = 1;
            }
            else 
            {
                g_wb[i].nr += dx[k];
                g_wg[j].r += dx[k];
            }  
        }
    }

    return PWRFLOW_SUCCEED;
}*/



///////////////////////////////////////////////////////////////////////////////
//  CheckConvergence()
//
//	Overview:	This subroutine checks if the forward and backward sweep is
//				converged. When all bus voltage mismatch and breakpoint voltage
//				mismatch are below at tolerance, the sweep is converged.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::CheckConvergence()
{
	int    i, j, k;
	double dv, da;

	if (g_eps >= g_epsmx) 
	{
		return 0;
	}

	for (k = 0; k < g_loopnum; k++)
	{
		i = g_wb[k].jc;
		j = g_wb[i].ja;

		dv = g_wb[i].v - g_wb[j].v;
		da = g_wb[i].a - g_wb[j].a;

		if (fabs(dv) >= g_epsmx || fabs(da) >= g_epsmx)
		{
			return 0;
		}
	}

	for (k = g_loopnum; k < g_loopnum + g_pvbsnum; k++)
	{
		i = g_wb[k].jc;
		j = g_wb[i].ja;

		if (g_wg[j].kswitch == 0)
		{
			dv = g_wg[j].vtarget - g_wb[i].v;

			if (fabs(dv) >= g_epsmx)
			{
				return 0;
			}
		}
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  ExecuteSweep()
//
//	Overview:	This subroutine execute backward and forward sweep for a radial
//				network power flow solutions. The backward sweep serves to sum
//				the load powers and the power losses from the end buses to the
//				root. The forward sweep establishes the bus voltages from the
//				root to the end buses based on the power flow obtained in the
//				backward sweep.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::ExecuteSweep()
{
	int ret=1;
    int    i, j, k,m;
    int    ibs, zbs;
    double g, b, vi, ai, vj, aj, w, r, x, y;
	double deltap,deltaq,deltac/*电容器导致的误差*/;//计算功率差值
//    //construct sensitivity matrix
//    if(ConstructSM() == PWRFLOW_FAIL)
//    {
//        return PWRFLOW_FAIL;
//    }

    g_niter = 0;

	BS* bs = dsbob2->bs_tb->GetTable();
    //execute back and forawrd sweep
	//initialize deltap and deltaq
	deltap = deltaq = 0;
    while(g_niter < g_mxiter)
    {
        g_niter++;
        g_eps = 0.0;
		deltac = 0;//每一次迭代电容器导致的误差都一样

		//lv_iter++;

		//if ( (g_wg[0].w>0) && g_ld_distr)//首端有功有量测
		//{
		//	//为了保证首端的功率完全分配，需要动态调整末端负荷的大小
		//	//每次迭代到首端后，会累计一个总的负荷值，这个负荷值和现有的存在一定的差值，
		//	//将这个差值平均分配到每一个负荷上面
		//	for ( k = 0; k < lv_wld; k++)
		//	{
		//		for ( m = 1; m < lv_wb;m++)
		//		{
		//			 if ( g_wld[k].ibs == g_wb[m].toi )
		//			 {
		//				 g_wb[g_wb[m].toi].nw = g_wb[g_wb[m].toi].nw + deltap/lv_wld;

		//				 break;//不可能一个负荷挂在多个母线上
		//			 }
		//		}				   
		//	}
		//}

		//if ( (g_wg[0].r>0) && g_ld_distr)//首端无功有量测
		//{
		//	//为了保证首端的功率完全分配，需要动态调整末端负荷的大小
		//	//每次迭代到首端后，会累计一个总的负荷值，这个负荷值和现有的存在一定的差值，
		//	//将这个差值平均分配到每一个负荷上面
		//	for ( k = 0; k < lv_wld; k++)
		//	{
		//		for ( m = 1; m < lv_wb;m++)
		//		{
		//			 if ( g_wld[k].ibs == g_wb[m].toi )
		//			 {
		//				 g_wb[g_wb[m].toi].nr = g_wb[g_wb[m].toi].nr + deltaq/lv_wld;

		//				 break;//不可能一个负荷挂在多个母线上
		//			 }
		//		}				   
		//	}
		//}
		
        //initialize power flow into buses from upstream lines母线的个数
		//这里实际上只有负荷和电容器有注入，其它的都没有注入，0注入
        for(i = 0; i < lv_wb; i++)
        {
            g_wb[i].dw = -g_wb[i].nw ;
            g_wb[i].dr = -g_wb[i].nr ;
        }

        //take into account of shunt injection power
		//电容器的出力与当前的电压有关，在迭代时这里是动态调整的
        for(i = 0; i < lv_wsh; i++)
        {
			//if (r == 0)
			//	continue;

            ibs = g_wsh[i].ibs;
            r = g_wsh[i].rnom;

            vi = g_wb[ibs].v;
            r = vi * vi * r;

            g_wsh[i].r = r;
            g_wb[ibs].dr -= r;
			//deltac += r;//如果存在首端功率调整，需要加上无功的折扣
        }

		//take into account of non-rating ratio transformer
		for(i = 0; i < lv_wxf; i++)
		{
			ibs = g_wbr[g_wxf[i].ibr].ibs;
			zbs = g_wbr[g_wxf[i].ibr].zbs;

			//first end
			vi = g_wb[ibs].v;
			g = g_wxf[i].g1;
			b = g_wxf[i].b1;

			w =  vi * vi * g;
			r = -vi * vi * b;

			g_wb[ibs].dw += w;
			g_wb[ibs].dr += r;

			//second end
			vj = g_wb[zbs].v;
			g = g_wxf[i].g2;
			b = g_wxf[i].b2;

			w =  vj * vj * g;
			r = -vj * vj * b;

			g_wb[zbs].dw += w;
			g_wb[zbs].dr += r;
		}

        //backward sweep
		//从末端负荷节点开始向前推进
		//这里已经进行了节点优化编号,节点号从0开始从电源点一层层增加
		//p1=p2+deltaP
        for(i = lv_wb - 1; i > 0; i--)
        {
            j = g_wb[i].jb;//节点的上游支路编号

            ibs = g_wbr[j].ibs;
            zbs = g_wbr[j].zbs;

            g = g_wbr[j].r;
            b = g_wbr[j].x;

            if(ibs > zbs)
            {
                k = ibs;
                ibs = zbs;
                zbs = k;
            }

            vj = g_wb[zbs].v;
            w  = g_wb[zbs].dw;//馈线段末端的累加功率值
            r  = g_wb[zbs].dr;

            //calculate power losses，计算末端到首端的网损
			if (vj == 0)
			{
				ret = -1; 
			}
            x = (w * w + r * r) / (vj * vj);
            w = x * g;
            r = x * b;//从这条支路上折算到上级节点的无功值

            //increase power flow into upstream bus
            g_wb[ibs].dw += g_wb[zbs].dw + w;//首端的功率值
            g_wb[ibs].dr += g_wb[zbs].dr + r;
           
            //store line power for forward sweep
			//支路上的首端功率流动，在回代的时候计算压降需要考虑这个首端流动功率
            g_wb[zbs].dw += w;//记录下这个线段参与计算时，末端导致的网损值
            g_wb[zbs].dr += r;
            g_wb[zbs].wloss = w;//本支路消耗的无功值
            g_wb[zbs].rloss = r;//本支路消耗的无功值

			//if ( (g_wg[0].w>0) && g_ld_distr)//首端有量测
			//{
			//	//calculate delta p,q，在电源点处计算累加的负荷值和电源点实测值之间的差值
			//	//每迭代一次这个差值都会变的很小，最后完全一样,这个也是要加快收敛速度
			//	if ( i == 1)
			//	{
			//		 deltap = -(g_wg[0].w - g_wb[i].dw);
			//	}
			//}
			//
			//if ( (g_wg[0].r>0) && g_ld_distr)//首端有量测
			//{
			//	//calculate delta p,q，在电源点处计算累加的负荷值和电源点实测值之间的差值
			//	//每迭代一次这个差值都会变的很小，最后完全一样,这个也是要加快收敛速度
			//	if ( i == 1)
			//	{
			//		 deltaq = -(g_wg[0].r - g_wb[i].dr);
			//	}
			//}
		}

        //forward sweep
		//这里是从1开始进行回代,U2=U1+deltaU(p1*)
        for(i = 1; i < lv_wb; i++)
        {
            j = g_wb[i].jb;//找到上级节点

            ibs = g_wbr[j].ibs;
            zbs = g_wbr[j].zbs;

            g = g_wbr[j].r;
            b = g_wbr[j].x;

            if(ibs > zbs)
            {
                k = ibs;
                ibs = zbs;
                zbs = k;
            }

            vi = g_wb[ibs].v;
            ai = g_wb[ibs].a;
            w  = g_wb[zbs].dw;//支路上首端流动的功率，只能用这个来计算压降
            r  = g_wb[zbs].dr;

			if (vi == 0)
			{
				ret = -1; 
			}
            x = vi - (g * w + b * r) / vi;
            y = (b * w - g * r) / vi;

            vj = sqrt(x * x + y * y);

			if (x == 0)
			{
				ret = -1; 
			}
            aj = ai - atan(y / x);

            g_wb[zbs].dv = g_wb[zbs].v - vj;//节点的电压损耗，用来进行收敛检验
            g_wb[zbs].da = g_wb[zbs].a - aj;
            g_wb[zbs].v = vj;
            g_wb[zbs].a = aj;
			if (isnormal(vj) == 0)
			{
				return 0;
			}
			//判断电压精度,选出两代之间电压差最大的节点
            if(g_eps < fabs(g_wb[zbs].dv))
            {
                g_eps = fabs(g_wb[zbs].dv);
				if (g_eps == 0)
				{
					return PWRFLOW_FAIL;
				}
				//g_iter[lv_iter-1].mxv = (double)g_eps;
				//g_iter[lv_iter-1].bsv = g_wb[zbs].tio;
            }
        }
		
        //if convergence, then return succeed
        if(CheckConvergence())
        {
            SetSlackPowerA();
            return ret;
        }	

        //if not convergence, correct breakpoint power
        //CorrectBPPower();
    }

//	if (g_niter >= 25)
//	{
//		return -1;
//	}

    return PWRFLOW_FAIL;
}

///////////////////////////////////////////////////////////////////////////////
//  ExecuteSeidel()
//
//	Overview:	This subroutine executes Gauss-Seidel power flow iterations.
//				It works on the admittance matrix and serves mainly to
//				provide the suitable initial voltages for Newton-Raphson
//				power flow solution.
//负荷是刚性的，初值仅仅只是给定的启动电压是否合适
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::ExecuteSeidel()
{
    int    i, j, k, m, p, jmn, jmx;
    double g, b, e, f, w, r, x, y, z, rmn, rmx;

    g_niter0 = 0;

    //execute Gauss-Seidel iterations
    while(g_niter0 < g_mxiter0)
    {
        g_niter0++;
        g_eps0 = 0.0;

        //calculate pq and pv bus voltage
        for(i = 0; i < lv_wb; i++)
        {
            if(i == g_slackbs) continue;

            e = g_wb[i].v;
            f = g_wb[i].a;
            w = g_wb[i].nw;
            r = g_wb[i].nr;
            z = e * e + f * f;
            x = (w * e + r * f) / z;
            y = (w * f - r * e) / z;

            k = g_yu[i].d;

            jmn = k + 1;
            jmx = g_yu[i+1].d;//两个对角之间的元素，也就是非对角元素的

            for(m = jmn; m < jmx; m++)
            {
                j = g_yu[m].j;
                g = g_yu[m].g;
                b = g_yu[m].b;
                e = g_wb[j].v;
                f = g_wb[j].a;
                x -= g * e - b * f;
                y -= g * f + b * e;
            }

            p = g_yu[k].p;

            while(p != k)//等于自己说明链表到头了
            {
                j = g_yu[p].i;
                g = g_yu[p].g;
                b = g_yu[p].b;
                p = g_yu[p].p;
                e = g_wb[j].v;
                f = g_wb[j].a;
                x -= g * e - b * f;
                y -= g * f + b * e;
            }

            g = g_yu[k].g;
            b = g_yu[k].b;

            z = g * g + b * b;
            e = x;
            f = y;
            x = (e * g + f * b) / z;
            y = (f * g - e * b) / z;

            if(g_wb[i].ja == 0)  //pq
            {
                e = g_wb[i].v;
                f = g_wb[i].a;
                z = sqrt((x - e) * (x - e) + (y - f) * (y - f));
                if(g_eps0 < z) g_eps0 = z;
                g_wb[i].v = x;
                g_wb[i].a = y;
            }
            else if(g_wb[i].ja == -1)  //pv
            {
                //modify pv bus voltage
                e = g_wb[i].v;
                f = g_wb[i].a;
                z = sqrt((e * e + f * f) / (x * x + y * y));
                e = z * x;
                f = z * y;
                g_wb[i].v = e;
                g_wb[i].a = f;

                //calculate pv bus reactive power
                GetBusPower(i, &w, &r);

                //calculate pv bus reactive power limit
                for(m = 0; m < lv_wg; m++)
                {
                    if(g_wg[m].slackavr == -1 && g_wg[m].ibs == i)
                    {
                        rmn = g_wb[i].nr - g_wg[m].r + g_wg[m].rmn;
                        rmx = g_wb[i].nr - g_wg[m].r + g_wg[m].rmx;
                        break;
                    }
                }

                //check pv bus reactive power limit
                if(r < rmn)
                {
                    r = rmn;
                    g_wb[i].ja = 0;
                    g_wg[m].kswitch = 1;
                }
                else if(r > rmx)
                {
                    r = rmx;
                    g_wb[i].ja = 0;
                    g_wg[m].kswitch = 1;
                }

                //set pv bus and generator reactive power
                g_wg[m].r += r - g_wb[i].nr;
                g_wb[i].nr = r;

                //recalculate pv bus voltage
                w = g_wb[i].nw;
                z = e * e + f * f;
                x = (w * e + r * f) / z;
                y = (w * f - r * e) / z;

                k = g_yu[i].d;

                jmn = k + 1;
                jmx = g_yu[i+1].d;

                for(m = jmn; m < jmx; m++)
                {
                    j = g_yu[m].j;
                    g = g_yu[m].g;
                    b = g_yu[m].b;
                    e = g_wb[j].v;
                    f = g_wb[j].a;
                    x -= g * e - b * f;
                    y -= g * f + b * e;
                }

                p = g_yu[k].p;

                while(p != k)
                {
                    j = g_yu[p].i;
                    g = g_yu[p].g;
                    b = g_yu[p].b;
                    p = g_yu[p].p;
                    e = g_wb[j].v;
                    f = g_wb[j].a;
                    x -= g * e - b * f;
                    y -= g * f + b * e;
                }

                g = g_yu[k].g;
                b = g_yu[k].b;

                z = g * g + b * b;
                e = x;
                f = y;
                x = (e * g + f * b) / z;
                y = (f * g - e * b) / z;

                //set pv bus voltage
                e = g_wb[i].v;
                f = g_wb[i].a;
                if(g_wb[i].ja == -1)
                {
                    z = sqrt((e * e + f * f) / (x * x + y * y));
                    x *= z;
                    y *= z;
                }
                z = sqrt((x - e) * (x - e) + (y - f) * (y - f));
                if(g_eps0 < z) g_eps0 = z;
                g_wb[i].v = x;
                g_wb[i].a = y; 
            }
        }

        //check convergence
        if(g_eps0 < g_epsmx0)
        {
            //convert to polar coordinates
            for(j = 0; j < lv_wb; j++)
            {
                e = g_wb[j].v;
                f = g_wb[j].a;
                g_wb[j].v = sqrt(e * e + f * f);
                g_wb[j].a = atan(f / e);
            }

            return PWRFLOW_SUCCEED;
        }
    }

    return PWRFLOW_FAIL;
}

///////////////////////////////////////////////////////////////////////////////
//  ExecuteNewton()
//
//	Overview:	This subroutine executes Newton-Raphson power flow iterations.
//				The Jacobian matrix is represented by (2, 2) blocks, and the
//				Gauss elimination method is used to solve the power flow
//				equation.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::ExecuteNewton()
{
    //initialize Jacobian matrix
    if(g_method == 1)
    {
        InitJacobianA();//牛顿法和高斯法对雅可比矩阵的处理方法是不一样的
    }
    else
    {
        InitJacobianB();
    }

    g_niter = 0;

    //execute Newton-Raphson iterations
    while(g_niter < g_mxiter)
    {
        g_niter++;
        g_eps = 0.0;

		lv_iter++;

        //form Jacobian matrix
        FormJacobian();//这个雅可比矩阵后面要用到无功优化里面

        //Gauss elimination method
        GaussElimination();

        //solve power flow equation
		if (SolveEquation() == 0)//不收敛
			return 0;

		//收敛不到合适的精度,两次精度都一样，不再进行计算
		if (g_eps < g_epsmx)
		{
			if (g_method == 1)
			{
				SetSlackPowerB();
			}
			else
			{
				SetSlackPowerC();
			}
			return PWRFLOW_SUCCEED;
		}
		
		//if (g_eps_last > g_eps)
		//{
		//	g_eps_last = g_eps; 
		//}

  //      //check convergence
  //      if(g_eps < g_epsmx)
  //      //if(g_eps < 0.00000000001)
  //      {
  //          if(g_method == 1)
  //          {
  //              SetSlackPowerB();
  //          }
  //          else
  //          {
  //              SetSlackPowerC();
  //          }
  //          return PWRFLOW_SUCCEED;
  //      }
    }

    return PWRFLOW_FAIL;
}

///////////////////////////////////////////////////////////////////////////////
//  InitJacobianA()   求雅可比矩阵中元素在上三角和下三角中的位置
//
//	Overview:	This subroutine initializes element values and positions of
//				Jacobian matrix directly from branch connections. As we
//				represent Jacobian matrix by (2, 2) blocks, so it has the
//				same structure as admittance matrix.

//	It sets up the diagonal position array (g_yu[i].d), the row
//				position array (g_yu[i].i), the column position array
//				(g_yu[i].j), and the column linking array (g_yu[i].p)
///////////////////////////////////////////////////////////////////////////////
//U、aij是每次迭代算出来的，G和B是支路的导纳，
//把每个节点的这4个值都算出来，就可以组成一个小的矩阵块
//用上次迭代的这几个值来形成新的雅可比矩阵
//有了雅可比矩阵，就可以用来解方程了，变成了纯数学问题

//雅可比矩阵每个块都含有以下4个元素，采用极坐标形式进行求解
//H： Hij=Ui*Uj*(Gijsin(aij)-Bijcos(aij));   Hii=-Ui * 累加(Uj*Gijsin(aij)-Bijcos(aij)) i<>j
//J： Jij=-Ui*Uj*(Gijcos(aij)-Bijsin(aij));  Jii=Ui * 累加(Uj*Gijcos(aij)+Bijsin(aij))
//N： Nij=Ui*Uj*(Gijcos(aij)+Bijsin(aij));   Nii=-Ui * 累加(Uj*Gijcos(aij)+Bijsin(aij))+2Ui*Ui*Gii
//L： Lij=Ui*Uj*(Gijsin(aij)-Bijcos(aij));   Hii=-Ui * 累加(Uj*Gijsin(aij)-Bijcos(aij))-2Ui*Ui*Bii
int PWRFLOW::InitJacobianA()
{
    int    i, j, k, m, jmn, jmx;
    int    ibs, zbs;

    //initialize data vaules of Jacobian matrix
	//对角元素，最大1000个节点，也就是1000 X 1000的矩阵
    for(i = 0; i < mx_jd; i++)
    {
        g_jd[i].H = 0.0;//每个小2 X 2矩阵组成了和导纳矩阵同阶的矩阵
        g_jd[i].N = 0.0;
        g_jd[i].J = 0.0;
        g_jd[i].L = 0.0;
    }

	//上三角
    for(i = 0; i < mx_ju; i++)
    {
        g_ju[i].j = 0;	//在矩阵中的列的位置
        g_ju[i].d = 0;	//
        g_ju[i].k = 0;	//
        g_ju[i].H = 0.0;
        g_ju[i].N = 0.0;
        g_ju[i].J = 0.0;
        g_ju[i].L = 0.0;
    }

	//下三角,雅可比矩阵不是对称阵
    for(i = 0; i < mx_jl; i++)
    {
        g_jl[i].j = -1;
        g_jl[i].d = 0;
        g_jl[i].k = 0;
        g_jl[i].H = 0.0;
        g_jl[i].N = 0.0;
        g_jl[i].J = 0.0;
        g_jl[i].L = 0.0;
    }

    //initialize size of Jacobian matrix
    lv_jd = lv_wb;//对角元素的个数，也就是矩阵的阶数
    lv_ju = 0;
    lv_jl = 0;

    //count block number of upper and lower Jacobian matrix
	//统计矩阵块的数量，一个块对应一个导纳矩阵的元素
	//每增加一个支路，将会在矩阵的上三角和下三角中分别增加一个元素
	//根据元素的编号大小，小的编号出现在上三角，大的编号在下三角
	//在这里矩阵还是一个对称阵，因为仅仅只是块的个数，和导纳阵相统一

//网络结构
//3      0
// \(0) /(1)
//   2 /
//   |(2) 支路编号
//	 1
//矩阵形式：
//   0   1   2   3
//0  D1      X1
//1      D2  X2
//2  X4  X5  D3  X3
//3          X6  D4 

//表面上看ju数组是存上三角元素，但其中的字段有不同的含义
//...ju是实体元素，角标从0~最后上三角元素。总数可能比阶数要大，因为可能一个节点有几个连接；
//...j 存列号，角标从0~最后上三角元素。和ju的角标一一对应，如X2对应的列是2
//...d 存本行非对角元素在ju中的位置，角标从1~n。如X2在角标[1]里面，本行共有2-1个
//...k 存支路对应的上三角元素在ju中的角标，角标从1~(n-1)。如（2）支路对应角标1，在ju[1]里找到X2

//角标(行) ju    j   d  k(支路编号对应的上三角列)    jl  j  d  k(支路编号对应的下三角列)      H N J K //雅可比矩阵的内容，和角标行对应 
// 0       X1    2   0  2                            X4  0  0  2
// 1       X2    2   1  0							 X5	 1	0  0
// 2       X3    3   2  1							 X6  2	0  1
// 3             0   3  0								 -1	2  0
// 4             0   3  0								 -1	3  0

//角标对应的d表示该行的第一个非0元素在ju中的位置


//从这个里面可以看出每一行有几个非0元素，用首地址将前面的减一下就可以了，如第3行有6-3=3个非零元素

    //通过这个循环可以得到每一行非零元素的总个数g_ju[ibs+1].d，以及全部非零元素的个数lv_ju
	for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;//首节点,这个已经是节点优化过后的编号了，不是原始的编号
        zbs = g_wbr[i].zbs;//末节点，编号是从0开始的，每个到一次编号，一般不会超过1000个

        if(ibs > zbs)//首末节点需要排序，大的在上三角部分，小的在下三角部分
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		//节点对应的行中非对角元素的个数
		//这里的物理意义是矩阵的一行中非0元素的个数就是这个节点和其它有连接的个数
		//+1的意义是为了和其前面的想减，得到本行的非零元素个数
        g_ju[ibs+1].d++;
        g_jl[zbs+1].d++;

        lv_ju++;//上下各增加一个元素
        lv_jl++;
    }

    //determine first block position of each row
	//每一行的第一个块的位置，主要指主对角为基准,非对角列的第一个位置，知道行后，+1取d就知道列位
	//每一行的 第一个 非零元素在数组g_ju[]中的位置，也就是角标号或索引号
    for(i = 0; i < lv_wb; i++)//每个节点对应一行
    {
		//角标再后移一位，现在g_ju[i+1].d保存的是该行的非零元素在g_ju[]的位置
		//g_ju[]数组是挨个存入元素，从最头的那个元素一直到最后的那个元素，中间没有空白
        g_ju[i+1].d += g_ju[i].d;
        g_jl[i+1].d += g_jl[i].d;
    }

    //determine block positions in Jacobian matrix
	//定位每一个块的位置，也就是每一个非0元素的位置，用前面的进行检索即可
	//将每个非0元素的列写到j里面
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		//通过前后两个的差值可以得到本行有几个非0元素
        jmn = g_ju[ibs].d;//在数组g_ju[]中本行的元素的下标
        jmx = g_ju[ibs+1].d;

		//找到g_ju[]中本行的几个实际元素的列的位置
        for(k = jmn; k < jmx; k++)
        {
            j = g_ju[k].j;

            if(j == 0)//本行第一个非0元素
            {
                g_ju[k].j = zbs;//本行第一个非对角元素所对应的列是连接的行号
                break;
            }
            else if(j > zbs)//如果列比原来的列还要更靠前面，需要重新安排一下，插入
            {
                m = jmx - 1;
                while(m > k)
                {
                    g_ju[m].j = g_ju[m-1].j;
                    m--;
                }
                g_ju[k].j = zbs;//把现在的元素的
                break;
            }
        }

		//做下三角
        jmn = g_jl[zbs].d;
        jmx = g_jl[zbs+1].d;

        for(k = jmn; k < jmx; k++)
        {
            j = g_jl[k].j;

            if(j == -1)
            {
                g_jl[k].j = ibs;
                break;
            }
            else if(j > ibs)
            {
                m = jmx - 1;
                while(m > k)
                {
                    g_jl[m].j = g_jl[m-1].j;
                    m--;
                }
                g_jl[k].j = ibs;
                break;
            }
        }
    }

    //determine branch positions in Jacobian matrix
	//确定分支位置，只有确定了每个元素的列才能算出来这个值
	//k值是追踪有用的记录，在后面的计算中要用到
	//将一条支路对应的上（下）三角列的位置选出来，一条支路只对应一个上三角和一个下三角
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		//将节点所连接支路支路编号写到这个下三角数组里面
		//支路号在进行潮流注入的时候有用
        jmn = g_ju[ibs].d;
        jmx = g_ju[ibs+1].d;
        for(j = jmn; j < jmx; j++)
        {
             if(g_ju[j].j == zbs)//对本行的上三角进行循环，找到和这个支路匹配的列的索引（下角标g_ju[j].j）
             {
                 g_ju[i].k = j;//支路对应的上三角元素在ju中的角标，其实就是索引号，只能有一个，找到后就退出
                 break;
             }
        }

        jmn = g_jl[zbs].d;
        jmx = g_jl[zbs+1].d;
        for(j = jmn; j < jmx; j++)
        {
            if(g_jl[j].j == ibs)
            {
                g_jl[i].k = j;//支路对应的下三角列
                break;
            }
        }
    }

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  InitJacobianB()
//
//	Overview::	This subroutine initializes element values and positions of
//				Jacobian matrix based on the admittance matrix structure.
//				As we represent Jacobian matrix by (2, 2) blocks, so it has
//				the same structure as admittance matrix.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::InitJacobianB()
{
    int    i, j, k, p, jmn, jmx;
    int    ibs, zbs;

    //initialize data vaules of Jacobian matrix
    for(i = 0; i < mx_jd; i++)//对角
    {
        g_jd[i].H = 0.0;
        g_jd[i].N = 0.0;
        g_jd[i].J = 0.0;
        g_jd[i].L = 0.0;
    }

    for(i = 0; i < mx_ju; i++)//上三角非对角
    {
        g_ju[i].j = 0;
        g_ju[i].d = 0;
        g_ju[i].H = 0.0;
        g_ju[i].N = 0.0;
        g_ju[i].J = 0.0;
        g_ju[i].L = 0.0;
    }

    for(i = 0; i < mx_jl; i++)//下三角非对角
    {
        g_jl[i].j = 0;
        g_jl[i].d = 0;
        g_jl[i].H = 0.0;
        g_jl[i].N = 0.0;
        g_jl[i].J = 0.0;
        g_jl[i].L = 0.0;
    }

    //initialize size of Jacobian matrix
    lv_jd = lv_wb;
    lv_ju = lv_yu - lv_wb;//含对角元素的上三角
    lv_jl = lv_yu - lv_wb;

    //initialize element positions of Jacobian Matrix
    for(i = 0; i < lv_wb; i++)
    {
        //upper blocks of Jacobian matrix
        g_ju[i+1].d = g_ju[i].d;

        jmn = g_yu[i].d + 1;
        jmx = g_yu[i+1].d;

        for(j = jmn; j < jmx; j++)
        {
            g_ju[g_ju[i+1].d].j = g_yu[j].j;
            g_ju[i+1].d++;
        }

        //lower blocks of Jacobian matrix
		//下三角在列的方面是对称的，可以通过上三角直接得到下三角的元素
        g_jl[i+1].d = g_jl[i].d;

        k = g_yu[i].d;
        p = g_yu[k].p;

        while(p != k)
        {
            g_jl[g_jl[i+1].d].j = g_yu[p].i;
            g_jl[i+1].d++;
            p = g_yu[p].p;
        }
    }

    //initialize branch positions in Jacobian matrix
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

        jmn = g_jl[zbs].d;
        jmx = g_jl[zbs+1].d;
        for(j = jmn; j < jmx; j++)
        {
            if(g_jl[j].j == ibs)
            {
                g_jl[i].k = j;
                break;
            }
        }

        jmn = g_ju[ibs].d;
        jmx = g_ju[ibs+1].d;
        for(j = jmn; j < jmx; j++)
        {
             if(g_ju[j].j == zbs)
             {
                 g_ju[i].k = j;
                 break;
             }
        }
    }

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  FormJacobian()
//
//	Overview:	This subroutine calculates active and reactive bus mismatch.
//              It also calculates Jacobian matrix.
//每个节点代表节点导纳矩阵的1行，逐个节点进行处理后，可以得到每行的对角非对角元素
//节点有重叠，每次都需要进行累加才能得到全部的值，导纳值都和对侧节点有关
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::FormJacobian()
{
    int    i, j, k;
    int    ibs, zbs;
    double g, b, vi, ai, vj, aj, vvi, vvj, vij, csa, sna, r;
    double vvi_g, vvi_b, vvj_g, vvj_b, vij_g, vij_b;

    //initialize bus mismatch and Jacobian matrix
    for(i = 0; i < lv_wb; i++)
    {
        g_wb[i].dw = 0.0;
        g_wb[i].dr = 0.0;
    }

    for(i = 0; i < lv_wld; i++)
    {
        ibs = g_wld[i].ibs;
        g_wb[ibs].dw -= g_wld[i].w;
        g_wb[ibs].dr -= g_wld[i].r;
    }

    for(i = 0; i < lv_wsh; i++)
    {
        ibs = g_wsh[i].ibs;
        r = g_wsh[i].rnom;
        vi = g_wb[ibs].v;
        r = vi * vi * r;
        g_wsh[i].r = r;
        g_wb[ibs].dr += r;
    }

	for(i = 0; i < lv_wxf; i++)
	{
		ibs = g_wbr[g_wxf[i].ibr].ibs;
		zbs = g_wbr[g_wxf[i].ibr].zbs;

		vi = g_wb[ibs].v;
		g = g_wxf[i].g1;
		b = g_wxf[i].b1;

		g_wb[ibs].dw -=  vi * vi * g;
		g_wb[ibs].dr -= -vi * vi * b;

		vj = g_wb[zbs].v;
		g = g_wxf[i].g2;
		b = g_wxf[i].b2;

		g_wb[zbs].dw -=  vj * vj * g;
		g_wb[zbs].dr -= -vj * vj * b;
	}

	for(i = 0; i < lv_jd; i++)  //lv_jd == lv_wb 对角元素的个数和节点数一致
    {
        g_jd[i].H = 0.0;
        g_jd[i].N = 0.0;
        g_jd[i].J = 0.0;
        g_jd[i].L = 0.0;
    }

    for(i = 0; i < lv_ju; i++)  //lv_ju == lv_jl == lv_yu - lv_wb上三角和下三角个数一样
    {
        g_ju[i].H = 0.0;
        g_ju[i].N = 0.0;
        g_ju[i].J = 0.0;
        g_ju[i].L = 0.0;
    }

    for(i = 0; i < lv_jl; i++)
    {
        g_jl[i].H = 0.0;
        g_jl[i].N = 0.0;
        g_jl[i].J = 0.0;
        g_jl[i].L = 0.0;
    }

    for(i = 0; i < lv_wsh; i++)
    {
        ibs = g_wsh[i].ibs;
        r = g_wsh[i].rnom;
        vi = g_wb[ibs].v;
        g_jd[ibs].L += 2.0 * vi * vi * r;
    }

	for(i = 0; i < lv_wxf; i++)
	{
		ibs = g_wbr[g_wxf[i].ibr].ibs;
		zbs = g_wbr[g_wxf[i].ibr].zbs;

		vi = g_wb[ibs].v;
		vj = g_wb[zbs].v;

		g_jd[ibs].N += -2.0 * vi * vi * g_wxf[i].g1;
		g_jd[ibs].L +=  2.0 * vi * vi * g_wxf[i].b1;
		g_jd[zbs].N += -2.0 * vj * vj * g_wxf[i].g2;
		g_jd[zbs].L +=  2.0 * vj * vj * g_wxf[i].b2;
	}

    //calculate line flow and fill-in Jacobian
	//对每一条支路进行处理，一上一下，其中重叠的部分累加得到增量
	//最后就是总的导纳值
	//节点处理时，上三角可以找到对应的j（非0位置列）
	//可以看做本节点对其它节点的扰动项，加到其它节点上去
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

        g = g_wbr[i].r;
        b = g_wbr[i].x;

        vi = g * g + b * b;
		if (vi == 0)
		{
			return 0;
		}

        g /= vi;//得到导纳的实部
        b /= vi;//导纳虚部-

        vi = g_wb[ibs].v;//首节点的电压幅值，电压是待求量，相当于x向量，J*x=b
        ai = g_wb[ibs].a;//
        vj = g_wb[zbs].v;//
        aj = g_wb[zbs].a;//末节点的电压相角，每次迭代都不一样

        vvi = vi * vi;
        vvj = vj * vj;
        vij = vi * vj;

        vvi_g = vvi * g;
        vvi_b = vvi * b;
        vvj_g = vvj * g;
        vvj_b = vvj * b;
        vij_g = vij * g;
        vij_b = vij * b;

        csa = cos(ai - aj);
        sna = sin(ai - aj);

		//计算每个节点的功率不平衡量，注入，相当于方程式的右边项b
		//通过行的导纳和 电压、电压的相角差计算得到
        g_wb[ibs].dw -= vvi_g - vij_g * csa + vij_b * sna;
        g_wb[ibs].dr -= vvi_b - vij_b * csa - vij_g * sna;
        g_wb[zbs].dw -= vvj_g - vij_g * csa - vij_b * sna;
        g_wb[zbs].dr -= vvj_b - vij_b * csa + vij_g * sna;

		//计算每个节点的雅可比矩阵元素中的对角量，两个节点互相扰动
		//一个元素对应4个小元素（2X2）组成的块
        g_jd[ibs].H += -vij_g * sna - vij_b * csa;
        g_jd[ibs].N +=  vij_g * csa - vij_b * sna - 2 * vvi_g;
        g_jd[ibs].J +=  vij_g * csa - vij_b * sna;
        g_jd[ibs].L +=  vij_g * sna + vij_b * csa - 2 * vvi_b;
        g_jd[zbs].H +=  vij_g * sna - vij_b * csa;
        g_jd[zbs].N +=  vij_g * csa + vij_b * sna - 2 * vvj_g;
        g_jd[zbs].J +=  vij_g * csa + vij_b * sna;
        g_jd[zbs].L += -vij_g * sna + vij_b * csa - 2 * vvj_b;

		//本节点和其它节点邻接在上三角中的位置（列）
		//此处的i是支路编号，这个对应了本支路的对上三角的扰动位置
        k = g_ju[i].k; //节点所连接的支路编号，也就是在支路中的位置
		//将这个扰动量算完后，放到对应的上三角元素对应的角标底下，下次可以重新取到
        g_ju[k].H +=  vij_g * sna + vij_b * csa;
        g_ju[k].N +=  vij_g * csa - vij_b * sna;
        g_ju[k].J += -vij_g * csa + vij_b * sna;
        g_ju[k].L +=  vij_g * sna + vij_b * csa;

        k = g_jl[i].k;
        g_jl[k].H += -vij_g * sna + vij_b * csa;
        g_jl[k].N +=  vij_g * csa + vij_b * sna;
        g_jl[k].J += -vij_g * csa - vij_b * sna;
        g_jl[k].L += -vij_g * sna + vij_b * csa;
    }

    //first check pq type generator，1-pq；-1 pv；0-平衡
    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].slackavr == 1 || g_wg[i].kswitch == 1)
        {
            ibs = g_wg[i].ibs;//一个节点有几个发电机的问题
            g_wb[ibs].dw += g_wg[i].w;
            g_wb[ibs].dr += g_wg[i].r;
        }
    }

    //then check slack and pv type generator
    for(i = 0; i < lv_wg; i++)
    {
        ibs = g_wg[i].ibs;//得到发电机的母线编号

        if(g_wg[i].slackavr == 0)  //slack
        {
            g_wb[ibs].nw -= g_wg[i].w;
            g_wb[ibs].nr -= g_wg[i].r;
            g_wg[i].w = -g_wb[ibs].dw;
            g_wg[i].r = -g_wb[ibs].dr;
            g_wb[ibs].nw += g_wg[i].w;
            g_wb[ibs].nr += g_wg[i].r;
            g_wb[ibs].dw += g_wg[i].w;
            g_wb[ibs].dr += g_wg[i].r;
        }
        else if(g_wg[i].slackavr == -1 && g_wg[i].kswitch == 0) //pv 无越界
        {
            g_wb[ibs].nr -= g_wg[i].r;
            g_wg[i].r = -g_wb[ibs].dr; 
            if(g_wg[i].r > g_wg[i].rmx)//无功出力大于发电机最大无功
            {
                g_wg[i].r = g_wg[i].rmx;
                g_wg[i].kswitch = 1;
                g_wb[ibs].ja = 0;
            }
            else if(g_wg[i].r < g_wg[i].rmn)//无功出力小于发电机最小无功
            {
                g_wg[i].r = g_wg[i].rmn;
                g_wg[i].kswitch = 1;
                g_wb[ibs].ja = 0;
            }
            g_wb[ibs].nr += g_wg[i].r;
            g_wb[ibs].dw += g_wg[i].w;
            g_wb[ibs].dr += g_wg[i].r;
        }
		//lld edit
        //else  //pq
        //{
        //    g_wb[ibs].dw += g_wg[i].w;
        //    g_wb[ibs].dr += g_wg[i].r;
        //}  
    }

	//set terms for slack and pv bus
	g_jd[g_slackbs].H = 1.0;
	g_jd[g_slackbs].N = 0.0;
	g_jd[g_slackbs].J = 0.0;
	g_jd[g_slackbs].L = 1.0;

	for(j = g_ju[g_slackbs].d; j < g_ju[g_slackbs+1].d; j++)
	{
		g_ju[j].H = 0.0;
		g_ju[j].N = 0.0;
		g_ju[j].J = 0.0;
		g_ju[j].L = 0.0;
	}

	for(j = g_jl[g_slackbs].d; j < g_jl[g_slackbs+1].d; j++)
	{
		g_jl[j].H = 0.0;
		g_jl[j].N = 0.0;
		g_jl[j].J = 0.0;
		g_jl[j].L = 0.0;
	}

	//其它的平衡节点，lld add 2018-7-23
	for (i = lv_wb - 2; i > lv_wb - 2 - (g_slackbsnum-1); i--)
	{
		if (g_wb[i].ja != -1)
		{
			g_jd[i].H = 1.0;
			g_jd[i].N = 0.0;
			g_jd[i].J = 0.0;
			g_jd[i].L = 1.0;

			for (j = g_ju[i].d; j < g_ju[i + 1].d; j++)
			{
				g_ju[j].H = 0.0;
				g_ju[j].N = 0.0;
				g_ju[j].J = 0.0;
				g_ju[j].L = 0.0;
			}

			for (j = g_jl[i].d; j < g_jl[i + 1].d; j++)
			{
				g_jl[j].H = 0.0;
				g_jl[j].N = 0.0;
				g_jl[j].J = 0.0;
				g_jl[j].L = 0.0;
			}
		}
	}
	//其它的平衡节点，lld add 2018-7-23

	//PV节点
	for(i = lv_wb - 2 - (g_slackbsnum - 1); i > lv_wb - 2 - (g_slackbsnum - 1) - g_pvbsnum; i--)
    {
        if(g_wb[i].ja == -1)
        {
            g_jd[i].J = 0.0;
            g_jd[i].L = 1.0;

            for(j = g_ju[i].d; j < g_ju[i+1].d; j++)
            {
                g_ju[j].J = 0.0;
                g_ju[j].L = 0.0;
            }

            for(j = g_jl[i].d; j < g_jl[i+1].d; j++)
            {
                g_jl[j].J = 0.0;
                g_jl[j].L = 0.0;
            }
        }
    }

	//做完这个后，雅可比矩阵的jd[],ju[],jl[]全部形成了
	//每个节点的注入有功不平衡量g_wb[ibs].dw，无功注入不平衡量g_wb[ibs].dr也形成了
	//剩下的就是解方程了
    return PWRFLOW_SUCCEED;

}

///////////////////////////////////////////////////////////////////////////////
//  GaussElimination()
//
//  Overview:	This subroutine converts Jacobian matrix into upper trangluar
//				matrix by Gauss elimination method. It reads the row of the
//				Jacobian matrix to be eliminated and stores in the working
//				arrays. After elimination, the upper trangular matrix is
//				stored in the global array g_jb[mx_jb].
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::GaussElimination()
{
    int    i, j, k, m, jmn, jmx;

    double wH[1000], wN[1000], wJ[1000], wL[1000], a;
    int    wj[1000];

    //initialize upper matrix
	//上三角矩阵，这个是最后的结果
    for(i = 0; i < mx_jb; i++)
    {
        g_jb[i].j = 0;//列号
        g_jb[i].d = 0;//非0元素首地址
        g_jb[i].H = 0.0;
        g_jb[i].N = 0.0;
        g_jb[i].J = 0.0;
        g_jb[i].L = 0.0;
    }
    lv_jb = 0;

    //Gauss elimination
	//每次处理一行，取一个计算母线出来就是代表一行
    for(i = 0; i < lv_wb; i++)
    {
        //initialize working array
        for(j = 0; j < lv_wb; j++)
        {
            wH[j] = 0.0;
            wN[j] = 0.0;
            wJ[j] = 0.0;
            wL[j] = 0.0;
            wj[j] = 0;
        }

        //read Jacobian into working array
        jmn = g_jl[i].d;
        jmx = g_jl[i+1].d;
        for(j = jmn; j < jmx; j++)
        {
            k = g_jl[j].j;

            wH[k] = g_jl[j].H;
            wN[k] = g_jl[j].N;
            wJ[k] = g_jl[j].J;
            wL[k] = g_jl[j].L;
            wj[k] = 1;
        }

        wH[i] = g_jd[i].H;
        wN[i] = g_jd[i].N;
        wJ[i] = g_jd[i].J;
        wL[i] = g_jd[i].L;
        wj[i] = 1;

        jmn = g_ju[i].d;
        jmx = g_ju[i+1].d;
        for(j = jmn; j < jmx; j++)
        {
            k = g_ju[j].j;

            wH[k] = g_ju[j].H;
            wN[k] = g_ju[j].N;
            wJ[k] = g_ju[j].J;
            wL[k] = g_ju[j].L;
            wj[k] = 1;
        }

        //Gauss elimination
		//逐行进行消除运算
        for(j = 0; j < i; j++)
        {
            if(wj[j] == 1)
            {
                if(wH[j] != 0.0)
                {
                    k = g_jb[j].d;
					if (g_jb[k].H == 0)
					{
						return 0;
					}
                    a = wH[j] / g_jb[k].H;

                    wH[j] = 0.0;
                    wN[j] -= a * g_jb[k].N;

                    for(k = g_jb[j].d + 1; k < g_jb[j+1].d; k++)
                    {
                        m = g_jb[k].j;

                        wH[m] -= a * g_jb[k].H;
                        wN[m] -= a * g_jb[k].N;
                        wj[m] = 1;
                    }
                    g_wb[i].dw -= a * g_wb[j].dw;//b的实部
                }

                if(wJ[j] != 0.0)
                {
                    k = g_jb[j].d;
					if (g_jb[k].H == 0)
					{
						return 0;
					}

                    a = wJ[j] / g_jb[k].H;

                    wJ[j] = 0.0;
                    wL[j] -= a * g_jb[k].N;

                    for(k = g_jb[j].d + 1; k < g_jb[j+1].d; k++)
                    {
                        m = g_jb[k].j;

                        wJ[m] -= a * g_jb[k].H;
                        wL[m] -= a * g_jb[k].N;
                        wj[m] = 1;
                    }
                    g_wb[i].dr -= a * g_wb[j].dw;//b的虚部
                }

                if(wN[j] != 0.0)
                {
                    k = g_jb[j].d;
					if (g_jb[k].L == 0)
					{
						return 0;
					}

                    a = wN[j] / g_jb[k].L;

                    wN[j] = 0.0;

                    for(k = g_jb[j].d + 1; k < g_jb[j+1].d; k++)
                    {
                        m = g_jb[k].j;

                        wH[m] -= a * g_jb[k].J;
                        wN[m] -= a * g_jb[k].L;
                        wj[m] = 1;
                    }
                    g_wb[i].dw -= a * g_wb[j].dr;
                }

                if(wL[j] != 0.0)
                {
                    k = g_jb[j].d;
					if (g_jb[k].L == 0)
					{
						return 0;
					}

                    a = wL[j] / g_jb[k].L;

                    wL[j] = 0.0;

                    for(k = g_jb[j].d + 1; k < g_jb[j+1].d; k++)
                    {
                        m = g_jb[k].j;

                        wJ[m] -= a * g_jb[k].J;
                        wL[m] -= a * g_jb[k].L;
                        wj[m] = 1;
                    }
                    g_wb[i].dr -= a * g_wb[j].dr;
                }
                wj[j] = 0;
            }
        }

        if(wJ[i] != 0.0)
        {
			if (wH[i] == 0)
			{
				return 0;
			}

            a = wJ[i] / wH[i];

            wJ[i] = 0.0;
            wL[i] -= a * wN[i];

            for(k = i + 1; k < lv_wb; k++)
            {
                if(wj[k] == 1)
                {
                    wJ[k] -= a * wH[k];
                    wL[k] -= a * wN[k];
                }
            }
            g_wb[i].dr -= a * g_wb[i].dw;
        }

        //write Jacobian into upper matrix
        g_jb[i].d = lv_jb;

        for(j = i; j < lv_wb; j++)
        {
            if(wj[j] == 1)
            {
                g_jb[lv_jb].j = j;
                g_jb[lv_jb].H = wH[j];
                g_jb[lv_jb].N = wN[j];
                g_jb[lv_jb].J = wJ[j];
                g_jb[lv_jb].L = wL[j];
                lv_jb++;
            }
        }
        g_jb[i+1].d = lv_jb;
    }

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//	SolveEquation()
//
//	Overview:	This subroutine solves linear equation by backward substitution.
//				It is executed after GaussElimination() function.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::SolveEquation()
{
    int    i, j, k, jmn, jmx;

    for(i = lv_wb - 1; i >= 0; i--)
    {
        jmn = g_jb[i].d + 1;
        jmx = g_jb[i+1].d;

        for(j = jmn; j < jmx; j++)
        {
            k = g_jb[j].j;
            g_wb[i].dw -= g_jb[j].H * g_wb[k].da + g_jb[j].N * g_wb[k].dv;
            g_wb[i].dr -= g_jb[j].J * g_wb[k].da + g_jb[j].L * g_wb[k].dv;
        }

        j = g_jb[i].d;
        g_wb[i].dv = g_wb[i].dr / g_jb[j].L;
        g_wb[i].dw -= g_jb[j].N * g_wb[i].dv;
        g_wb[i].da = g_wb[i].dw / g_jb[j].H;
    }

    for(i = 0; i < lv_wb; i++)
    {
        g_wb[i].dv *= g_wb[i].v;

        g_wb[i].a -= g_wb[i].da;//每个节点的电压的角度
        g_wb[i].v -= g_wb[i].dv;//每个节点的电压幅值

		//if (isnormal(g_wb[i].dv) == 0)
		//{
		//	return 0;
		//}

		if (g_eps < fabs(g_wb[i].dv))
        {
            g_eps = fabs(g_wb[i].dv);//将前后两次迭代的最大差值选出来，放到一个全局误差变量

//			g_iter[lv_iter-1].mxv = (double)g_eps;
//			g_iter[lv_iter-1].bsv = g_wb[i].tio;
        }

//		if(g_epsmx < fabs(g_wb[i].dv))
//		{
//			g_iter[lv_iter-1].nbsv++;
//		}
    }

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  GetBusPower()
//	
//	Arguments:   input:   i --- bus number
//                        s --- coordinate system (trangular or polar)
//               output: *w --- active power to be calculated
//                       *r --- reactive power to be calculated
//
//	Overview:    This subroutine calculates power flow out of a bus. It works
//               on admittance matrix and bus voltages. The coordinate system
//               is indicated by the input "s" (0 - trangular, 1 - polar).
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::GetBusPower(int i, double *w, double *r, int s)
{
    int    j, k, m, p, jmn, jmx;
    double g, b, e, f, x, y;

    if(s == 0)
    {
        e = g_wb[i].v;
        f = g_wb[i].a;
        k = g_yu[i].d;
        g = g_yu[k].g;
        b = g_yu[k].b;

        *w = g * e - b * f;
        *r = g * f + b * e;

        jmn = k + 1;
        jmx = g_yu[i+1].d;

        for(m = jmn; m < jmx; m++)
        {
            j = g_yu[m].j;
            g = g_yu[m].g;
            b = g_yu[m].b;
            x = g_wb[j].v;
            y = g_wb[j].a;

            *w += g * x - b * y;
            *r += g * y + b * x;
        }

        p = g_yu[k].p;

        while(p != k)
        {
            j = g_yu[p].i;
            g = g_yu[p].g;
            b = g_yu[p].b;
            p = g_yu[p].p;
            x = g_wb[j].v;
            y = g_wb[j].a;

            *w += g * x - b * y;
            *r += g * y + b * x;
        }

        x = *w;
        y = *r;
        *w = x * e + y * f;
        *r = x * f - y * e;
    }
    else
    {
        e = g_wb[i].v * cos(g_wb[i].a);
        f = g_wb[i].v * sin(g_wb[i].a);
        k = g_yu[i].d;
        g = g_yu[k].g;
        b = g_yu[k].b;

        *w = g * e - b * f;
        *r = g * f + b * e;

        jmn = k + 1;
        jmx = g_yu[i+1].d;

        for(m = jmn; m < jmx; m++)
        {
            j = g_yu[m].j;
            g = g_yu[m].g;
            b = g_yu[m].b;
            x = g_wb[j].v * cos(g_wb[j].a);
            y = g_wb[j].v * sin(g_wb[j].a);

            *w += g * x - b * y;
            *r += g * y + b * x;
        }

        p = g_yu[k].p;

        while(p != k)
        {
            j = g_yu[p].i;
            g = g_yu[p].g;
            b = g_yu[p].b;
            p = g_yu[p].p;
            x = g_wb[j].v * cos(g_wb[j].a);
            y = g_wb[j].v * sin(g_wb[j].a);

            *w += g * x - b * y;
            *r += g * y + b * x;
        }

        x = *w;
        y = *r;
        *w = x * e + y * f;
        *r = x * f - y * e;
    }

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  SetSlackPowerA()
//
//	Overview:    This subroutine calculates the slack (root) bus injection
//               power, slack generator power, and system total loss power.
//               It is executed when the function ExecuteSweep() has got 
//               the converged solutions.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::SetSlackPowerA()
{
    int    i, j, k;
	int	   ibs, zbs;

    g_wb[0].nw = 0.0;
    g_wb[0].nr = 0.0;

    g_wloss = 0.0;
    g_rloss = 0.0;

    //calculate slack bus injection power
    for(i = 1; i < g_wb[0].jb + 1; i++)
    {
        j = g_wb[i].jb;
        k = g_wbr[j].ibs;
        if(k == 0) k = g_wbr[j].zbs;
        g_wb[0].nw += g_wb[k].dw;
        g_wb[0].nr += g_wb[k].dr; 
    }

    //claculate slack generator power and loss power  
    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].slackavr == 0)
        {
            k = i;
            g_wg[k].w = g_wb[0].nw;
            g_wg[k].r = g_wb[0].nr; 
            break;
        }
    }

    for(i = 0; i < lv_wg; i++)
    {
        j = g_wg[i].ibs;
        if(j == 0 && g_wg[i].slackavr != 0)
        {
            g_wg[k].w -= g_wg[i].w;
            g_wg[k].r -= g_wg[i].r;
        }
    }
     
	for(i = 0; i < lv_wxf; i++)
	{
        ibs = g_wbr[g_wxf[i].ibr].ibs;
        zbs = g_wbr[g_wxf[i].ibr].zbs;

        if(ibs == 0)
		{
			g_wg[k].w +=  g_wb[ibs].v * g_wb[ibs].v * g_wxf[i].g1;
			g_wg[k].r += -g_wb[ibs].v * g_wb[ibs].v * g_wxf[i].b1;
		}
		else if(zbs == 0)
		{
			g_wg[k].w +=  g_wb[zbs].v * g_wb[zbs].v * g_wxf[i].g2;
			g_wg[k].r += -g_wb[zbs].v * g_wb[zbs].v * g_wxf[i].b2;
		}
	}

    for(i = 0; i < lv_wsh; i++)
    {
        j = g_wsh[i].ibs;
        if(j == 0)
        {
            g_wg[k].r -= g_wsh[i].r;
            g_wb[0].nr -= g_wsh[i].r;  //NOTE!
        }
        g_rloss += g_wsh[i].r;
    }

    for(i = 0; i < lv_wld; i++)
    {
        j = g_wld[i].ibs;
        if(j == 0)
        {
            g_wg[k].w += g_wld[i].w;
            g_wg[k].r += g_wld[i].r;
        }
        g_wloss -= g_wld[i].w;
        g_rloss -= g_wld[i].r;
    }

    for(i = 0; i < lv_wg; i++)
    {
        g_wloss += g_wg[i].w;
        g_rloss += g_wg[i].r;
    }

    return PWRFLOW_SUCCEED;
}

int PWRFLOW::GetSlackPower(int g_slackbs)
{
	int    i, j;
	int    ibs, zbs;
	double g, b, vi, ai, vj, aj, csa, sna, w, r;

	w = 0.0;
	r = 0.0;
	//主电源节点的功率
	for (i = 0; i < lv_wbr; i++)
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;

		if (ibs > zbs)
		{
			j = ibs;
			ibs = zbs;
			zbs = j;
		}

		//if(ibs == g_slackbs || zbs == g_slackbs)
		if (zbs == g_slackbs)
		{
			g = g_wbr[i].r;
			b = g_wbr[i].x;

			vi = g * g + b * b;
			g /= vi;
			b /= vi;

			vi = g_wb[ibs].v;
			ai = g_wb[ibs].a;
			vj = g_wb[zbs].v;
			aj = g_wb[zbs].a;

			csa = cos(ai - aj);
			sna = sin(ai - aj);

			w += vj * vj * g - vi * vj * g * csa - vi * vj * b * sna;
			r += vj * vj * b - vi * vj * b * csa + vi * vj * g * sna;
		}
	}

	g_wb[g_slackbs].nw = w;
	g_wb[g_slackbs].nr = r;

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  SetSlackPowerB()
//
//  Overview:    This subroutine calculates slack generator power and system
//               total loss power directly from branch connections. It is
//               called when function ExecuteNewton() has got the converged
//               solution.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::SetSlackPowerB()
{
    int    i, j;
    int    ibs, zbs;
    double g, b, vi, ai, vj, aj, csa, sna, w, r;

    w = 0.0;
    r = 0.0;
    g_wloss = 0.0;
    g_rloss = 0.0;

	for (i = g_slackbs; i > g_slackbs - g_slackbsnum; i--)
	{
		GetSlackPower(i);
	}

	////主电源节点的功率
 //   for(i = 0; i < lv_wbr; i++)
 //   {
 //       ibs = g_wbr[i].ibs;
 //       zbs = g_wbr[i].zbs;

 //       if(ibs > zbs)
 //       {
 //           j = ibs;
 //           ibs = zbs;
 //           zbs = j;
 //       }

 //       //if(ibs == g_slackbs || zbs == g_slackbs)
 //       if(zbs == g_slackbs)
 //       {
 //           g = g_wbr[i].r;
 //           b = g_wbr[i].x;

 //           vi = g * g + b * b;
 //           g /= vi;
 //           b /= vi;

 //           vi = g_wb[ibs].v;
 //           ai = g_wb[ibs].a;
 //           vj = g_wb[zbs].v;
 //           aj = g_wb[zbs].a;

 //           csa = cos(ai - aj);
 //           sna = sin(ai - aj);

 //           w += vj * vj * g - vi * vj * g * csa - vi * vj * b * sna;
 //           r += vj * vj * b - vi * vj * b * csa + vi * vj * g * sna;
 //       }
 //   }

 //   g_wb[g_slackbs].nw = w;
 //   g_wb[g_slackbs].nr = r;

    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].slackavr == 0)
        {
            j = i; 
            g_wg[j].w = g_wb[g_slackbs].nw;
            g_wg[j].r = g_wb[g_slackbs].nr;
            //break;
        }
    }

    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].ibs == g_slackbs && g_wg[i].slackavr != 0)
        {
            g_wg[j].w -= g_wg[i].w;
            g_wg[j].r -= g_wg[i].r;
        }
    }
     
	for(i = 0; i < lv_wxf; i++)
	{
        ibs = g_wbr[g_wxf[i].ibr].ibs;
        zbs = g_wbr[g_wxf[i].ibr].zbs;

        if(ibs == g_slackbs)
		{
			g_wg[j].w +=  g_wb[ibs].v * g_wb[ibs].v * g_wxf[i].g1;
			g_wg[j].r += -g_wb[ibs].v * g_wb[ibs].v * g_wxf[i].b1;
		}
		else if(zbs == g_slackbs)
		{
			g_wg[j].w +=  g_wb[zbs].v * g_wb[zbs].v * g_wxf[i].g2;
			g_wg[j].r += -g_wb[zbs].v * g_wb[zbs].v * g_wxf[i].b2;
		}
	}

    for(i = 0; i < lv_wsh; i++)
    {
        if(g_wsh[i].ibs == g_slackbs)
        {
            g_wg[j].r -= g_wsh[i].r;
            g_wb[g_slackbs].nr -= g_wsh[i].r;  //NOTE!
        }
        g_rloss += g_wsh[i].r;
    }

    for(i = 0; i < lv_wld; i++)
    {
        if(g_wld[i].ibs == g_slackbs)
        {
            g_wg[j].w += g_wld[i].w;
            g_wg[j].r += g_wld[i].r;
        }
        g_wloss -= g_wld[i].w;
        g_rloss -= g_wld[i].r;
    }

    for(i = 0; i < lv_wg; i++)
    {
        g_wloss += g_wg[i].w;
        g_rloss += g_wg[i].r;
    }

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  SetSlackPowerC()
//
//	Overview:    This subroutine calculates slack generator power and system
//               total loss power based on admittance matrix. It is called
//               when function ExecuteNewton() has got the converged solution.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::SetSlackPowerC()
{
    int    i, j;
    double w, r;

    w = 0.0;
    r = 0.0;
    g_wloss = 0.0;
    g_rloss = 0.0;

    GetBusPower(g_slackbs, &w, &r, 1);

    g_wb[g_slackbs].nw = w;
    g_wb[g_slackbs].nr = r;

    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].slackavr == 0)
        {
            j = i; 
            g_wg[j].w = g_wb[g_slackbs].nw;
            g_wg[j].r = g_wb[g_slackbs].nr;
            break;
        }
    }

    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].ibs == g_slackbs && g_wg[i].slackavr != 0)
        {
            g_wg[j].w -= g_wg[i].w;
            g_wg[j].r -= g_wg[i].r;
        }
    }
     
    for(i = 0; i < lv_wld; i++)
    {
        if(g_wld[i].ibs == g_slackbs)
        {
            g_wg[j].w += g_wld[i].w;
            g_wg[j].r += g_wld[i].r;
        }
        g_wloss -= g_wld[i].w;
        g_rloss -= g_wld[i].r;
    }

    for(i = 0; i < lv_wsh; i++)
    {
        if(g_wsh[i].ibs == g_slackbs)
        {
            g_wg[j].r -= g_wsh[i].r;
            g_wb[g_slackbs].nr -= g_wsh[i].r;  //NOTE!
        }
        g_rloss += g_wsh[i].r;
    }

    for(i = 0; i < lv_wg; i++)
    {
        g_wloss += g_wg[i].w;
        g_rloss += g_wg[i].r;
    }

    return PWRFLOW_SUCCEED;
}

//////////////////////////////////////////////////////////////////////////////
//  PutFlowResult()
//
//	Overview:    This subroutine calculates the final power flow results by 
//               bus voltages in the working array, and then writes them
//               into the database. It provides a general interface between
//               this program and an outer database.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::PutFlowResult(int type)
{

	if(PutBusVoltage_1(type) == PWRFLOW_FAIL)
	{
		return PWRFLOW_FAIL;
	}

	//write line flow
	if(PutLineFlow_1(type) == PWRFLOW_FAIL)
	{
		return PWRFLOW_FAIL;
	}

	//write system power
	if(PutSystemPower_1(type) == PWRFLOW_FAIL)
	{
		return PWRFLOW_FAIL;
	}

/////////////////////////////////////////////////////

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  PutBusVoltage()
int PWRFLOW::PutBusVoltage_1(int type)
{
    int    i;
    int    ibs, pnd, icp;
    double v, a, vl;
	int			knd;
	int			knd0;

	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();

	//get nd table
	ND* nd = dsbob2->nd_tb->GetTable();

	if (type != 2)//潮流不收敛
	{
		//set bs voltage
		maxVoltage = 0;
		minVoltage = 10000;
		for (i = 0; i < lv_wb; i++)
		{
			if (g_wb[i].tio >= 0)
			{
				v = g_wb[i].v;
				a = g_wb[i].a;
				ibs = g_wb[i].tio;
				pnd = (bs + ibs)->pnd;
				if (pnd == -1)
					vl = 10;
				else
					vl = double((nd + pnd)->vl);
				(bs + ibs)->v = (float)v;
				(bs + ibs)->kv = (float)(v * vl);
				(bs + ibs)->adeg = (float)(a * 57.29578);//将标幺值变成有名值
				(bs + ibs)->w = (float)(g_wb[i].nw * g_wbase);
				(bs + ibs)->r = (float)(g_wb[i].nr * g_wbase);

				if (v < minVoltage)
					minVoltage = v;//把最低电压记录下来
				if (v > maxVoltage)
					maxVoltage = v;//把最高电压记录下来

				//给每个节点都赋上电压值
				knd0 = (bs + ibs)->pnd;//反操作，通过母线得到节点编号
				if (knd0 != -1)
				{
					knd = knd0;
					do
					{
						(nd + knd)->v = (bs + ibs)->v;
						(nd + knd)->adeg = (bs + ibs)->adeg;

						//move to next nd of this bs
						knd = (nd + knd)->fch;
					} while (knd != knd0);
				}
			}
			//g_wb[i].tio < 0 -- 'i' is a dummy bus
		}

		//没必要在这里进行重置，初始化的时候已经做了
		//	for(i = 0; i <= g_bsmx; i++)
		//	{
		//		if( ((bs+i)->iisland < 0) /*|| ((bs+i)->q & BS_OPEN)*/ )
		//		{
		//			(bs+i)->adeg = 0.0;
		//			(bs+i)->v = 0.0;
		//			(bs+i)->kv = 0.0;
		//			(bs+i)->w = 0.0;
		//			(bs+i)->r = 0.0;
		//		}
		//	}
	}

	if (lld->mx_bus>0)
	{
		//set bus voltage
		BUS* bus = dsbob2->bus_tb->GetTable();
		int lv_bus = dsbob2->bus_tb->GetCount();

		for(i = 0; i < lv_bus; i++)
		{
			ibs = (bus+i)->ibs;			
			if((bs+ibs)->iisland == g_island)
			{
				if (type == 2)//潮流不收敛
				{
					(bus + i)->v = -1;
					(bus + i)->adeg = -1;
				}
				else
				{
					(bus + i)->v = (bs + ibs)->v;
					(bus + i)->adeg = (bs + ibs)->adeg;
				}
			}
		}
	}

	if (lld->mx_cp>0)
	{
		//cp
		int lv_cp = dsbob2->cp_tb->GetCount();
		CP* cp = dsbob2->cp_tb->GetTable();

		for(i = 0; i < lv_wsh; i++)
		{
			icp = g_wsh[i].logic;
			ibs = (cp + icp)->ibs;
			if ((bs + ibs)->iisland == g_island)
			{
				if (type != 2)//潮流不收敛
				{
					(cp + icp)->r = (float)(g_wsh[i].r * g_wbase);
				}
				else
					(cp + icp)->r = -1;
			}
		}

		for(i = 0; i < lv_cp; i++)
		{
			ibs = (cp + i)->ibs;
			if (((bs + ibs)->iisland == g_island)
				&&
				((bs + ibs)->q & BS_OPEN || (cp + i)->q & CP_OPEN))
			{
				(cp + i)->r = 0.0;//将本岛的没有接入的电容器赋0值
			}
		}
	}
	
	//残余网络
	if (type & 1)
	{
		//int iwg = 0;
		//for (i = 0; i < lv_wg; i++)
		//{
		//	ibs = iwg = g_wg[i].logic;
		//	//得到母线所属的联络开关起点

		//	if ((bs + ibs)->iisland == g_island)
		//	{
		//		if (type != 2)//潮流不收敛
		//		{
		//			(so + iwg)->w = float(g_wg[i].w * g_wbase);
		//			(so + iwg)->r = float(g_wg[i].r * g_wbase);
		//			(so + iwg)->pLoss = (double)(g_wloss * g_wbase);
		//			(so + iwg)->qLoss = (double)(g_rloss * g_wbase);

		//			(so + iwg)->sr = minVoltage;
		//		}
		//		else
		//		{
		//			(so + iwg)->w = -0.001;
		//			(so + iwg)->r = -0.001;
		//			(so + iwg)->pLoss = 0;
		//			(so + iwg)->qLoss = 0;

		//			(so + iwg)->sr = -1;
		//		}
		//	}
		//}
	}
	else
	{
		//slack so
		SO* so = dsbob2->so_tb->GetTable();
		int iwg = 0;
		for (i = 0; i < lv_wg; i++)
		{
			iwg = g_wg[i].logic;
			ibs = (so + iwg)->ibs;
			if ((bs + ibs)->iisland == g_island)
			{
				if (type != 2)//潮流不收敛
				{
					(so + iwg)->w = float(g_wg[i].w * g_wbase);
					(so + iwg)->r = float(g_wg[i].r * g_wbase);
					(so + iwg)->pLoss = (double)(g_wloss * g_wbase);
					(so + iwg)->qLoss = (double)(g_rloss * g_wbase);

					(so + iwg)->sr = minVoltage;
				}
				else
				{
					(so + iwg)->w = -0.001;
					(so + iwg)->r = -0.001;
					(so + iwg)->pLoss = 0;
					(so + iwg)->qLoss = 0;

					(so + iwg)->sr = -1;
				}
			}
		}
	}
	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  PutLineFlow()
int PWRFLOW::PutLineFlow_1(int type)
{
	int		i,j;
	int		ibs, zbs, isec, ixf, icp;
    int		iout, zout;
    double	r, x, g, b, vi, vj, ai, aj, ei, fi, ej, fj,vi_out;
    double	wi, ri, ci, wj, rj, cj;
    double	wx, wy;

	//get bs table
	BS* bs = dsbob2->bs_tb->GetTable();

	//get sec table
	int lv_sec = dsbob2->sec_tb->GetCount();
	SEC* sec = dsbob2->sec_tb->GetTable();
	//get nd table
	ND* nd = dsbob2->nd_tb->GetTable();

	int lv_xf = 0;
	int lv_cp = 0;
	XF* xf = NULL;
	CP* cp = NULL;

	//lld add 2019-7-30
	//lld->mx_xf = lv_wxf = 0;

	if (lld->mx_xf > 0)
	{
		xf = dsbob2->xf_tb->GetTable();
	}
	if (lld->mx_cp > 0)
	{
		cp = dsbob2->cp_tb->GetTable();
	}

	//initialize xf flow result
	for(i = 0; i < lv_wbr; i++)
	{
		if ((lld->mx_xf > 0) && (g_wbr[i].type == _XF))
		{
            ixf = g_wbr[i].logic;
            (xf+ixf)->w  = 0.0;
            (xf+ixf)->r_ = 0.0;
            (xf+ixf)->wz = 0.0;
            (xf+ixf)->rz = 0.0;
        }
		if ((lld->mx_cp > 0) && (g_wbr[i].type == _CP))
		{
			icp = g_wbr[i].logic;
			(cp + icp)->w = 0.0;
			(cp + icp)->r_ = 0.0;
			(cp + icp)->wz = 0.0;
			(cp + icp)->rz = 0.0;
			(cp + icp)->v_b = 0.0;
			(cp + icp)->adeg_b = 0.0;
			(cp + icp)->v_e = 0.0;
			(cp + icp)->adeg_e = 0.0;
			(cp + icp)->i = 0.0;
		}
	}

	//take into account of non-rating ratio transformer
	for(i = 0; i < lv_wxf; i++)
	{
		ixf = g_wbr[g_wxf[i].ibr].logic;
		ibs = g_wbr[g_wxf[i].ibr].ibs;
		zbs = g_wbr[g_wxf[i].ibr].zbs;

		//first end 
		vi = g_wb[ibs].v;
		g = g_wxf[i].g1;
		b = g_wxf[i].b1;
		wi =  vi * vi * g;
		ri = -vi * vi * b;

		//second end
		vj = g_wb[zbs].v;
		g = g_wxf[i].g2;
		b = g_wxf[i].b2;
		wj =  vj * vj * g;
		rj = -vj * vj * b;

		//set result
		(xf+ixf)->w  += (float)wi;
		(xf+ixf)->r_ += (float)ri;
		(xf+ixf)->wz += (float)wj;
		(xf+ixf)->rz += (float)rj;
	}

	
	//set sec and xf flow result
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;
        r = g_wbr[i].r;
        x = g_wbr[i].x;
        
        vi = g_wb[ibs].v;//电压幅值
        ai = g_wb[ibs].a;//电压相位角，此时还是标幺值
        vj = g_wb[zbs].v;
        aj = g_wb[zbs].a;

 		vi_out = vi;

		ei = vi * cos(ai);//电压的实部
        fi = vi * sin(ai);//电压的虚部
        ej = vj * cos(aj);
        fj = vj * sin(aj);

//		if ((r==0) && (x==0))//0阻抗支路，需要做特殊处理
//		{
//			wi = wj = g_wb[ibs].dw;
//			ri = rj = g_wb[ibs].dr;
//		}
//		else
		{
			wx = ei * ej + fi * fj;
			wy = fi * ej - ei * fj;       
			g = r / (r * r + x * x);
			b = x / (r * r + x * x); 
 
			wi = g * vi * vi - g * wx + b * wy;
			ri = b * vi * vi - b * wx - g * wy;
			wj = g * vj * vj - g * wx - b * wy;
			rj = b * vj * vj - b * wx + g * wy;
		}
		
		if(g_wbr[i].type == _SEC)
        {
			ci = sqrt((wi * wi + ri * ri) / (vi * vi));
			cj = ci;//电流不会发生变化

			//vi = (double)(nd+(bs+g_wb[ibs].tio)->pnd)->vl;
            
			isec = g_wbr[i].logic;
//			lld->delstrright((sec+isec)->id,ZN_ID_LEN);
            (sec+isec)->w  = (float)(wi * g_wbase);
            (sec+isec)->r  = (float)(ri * g_wbase);
			//电流需要标幺值折算，电压是10kV标幺值
            (sec+isec)->i  = (float)(ci * (g_wbase / vi / 1.73205) * 1000);
            (sec+isec)->wz = (float)(wj * g_wbase);
            (sec+isec)->rz = (float)(rj * g_wbase);
            (sec+isec)->iz = (float)(cj * (g_wbase / vi / 1.73205) * 1000);

			(sec+isec)->v_b = vi_out;
			(sec+isec)->adeg_b = ai * 57.29578;//转化为有名值
			
			(sec+isec)->v_e = vj;
			(sec+isec)->adeg_e = aj* 57.29578;

			//节点有属于0阻抗支路的情况
			for (j=0; j<lv_zbr; j++)
			{
				if (g_zbr[j].flag)
					continue;
	
				if (g_zbr[j].ibs == g_wb[ibs].tio)
				{
					g_zbr[j].w = (sec+g_zbr[j].logic)->w = (sec+isec)->w;
					g_zbr[j].r = (sec+g_zbr[j].logic)->r = (sec+isec)->r;
					(sec+g_zbr[j].logic)->wz = -(sec+isec)->w;
					(sec+g_zbr[j].logic)->rz = -(sec+isec)->r;

					g_zbr[j].v = (sec+g_zbr[j].logic)->v_b = (sec+g_zbr[j].logic)->v_e = (sec+isec)->v_b;
					g_zbr[j].a = (sec+g_zbr[j].logic)->adeg_b = (sec+g_zbr[j].logic)->adeg_e = (sec+isec)->adeg_b;
					g_zbr[j].i = (sec+g_zbr[j].logic)->i = (sec+g_zbr[j].logic)->iz = (sec+isec)->i;
					
					g_zbr[j].flag = 1;
				}
				else if (g_zbr[j].ibs == g_wb[zbs].tio) 
				{
					g_zbr[j].w = (sec+g_zbr[j].logic)->wz = (sec+isec)->wz;
					g_zbr[j].r = (sec+g_zbr[j].logic)->rz = (sec+isec)->rz;
					(sec+g_zbr[j].logic)->w = -(sec+isec)->wz;
					(sec+g_zbr[j].logic)->r = -(sec+isec)->rz;
					g_zbr[j].v = (sec+g_zbr[j].logic)->v_b = (sec+g_zbr[j].logic)->v_e = (sec+isec)->v_e;
					g_zbr[j].a = (sec+g_zbr[j].logic)->adeg_b = (sec+g_zbr[j].logic)->adeg_e = (sec+isec)->adeg_e;
					g_zbr[j].i = (sec+g_zbr[j].logic)->i = (sec+g_zbr[j].logic)->iz = (sec+isec)->iz;

					g_zbr[j].flag = 1;
				}
			}
        }
		else if(g_wbr[i].type == _XF)
        {
            ixf = g_wbr[i].logic;
			
			wi += (xf+ixf)->w;
			ri += (xf+ixf)->r_;
			wj += (xf+ixf)->wz;
			rj += (xf+ixf)->rz;
			
			ci = sqrt((wi * wi + ri * ri) / (vi * vi));
			cj = ci;

			vi = (double)(nd+(bs+g_wb[ibs].tio)->pnd)->vl;
			vj = (double)(nd+(bs+g_wb[zbs].tio)->pnd)->vl;

            (xf+ixf)->w  = (float)(wi * g_wbase);
            (xf+ixf)->r_  = (float)(ri * g_wbase);
            (xf+ixf)->i  = (float)(ci * (g_wbase / vi / 1.73205) * 1000); //A
            (xf+ixf)->wz = (float)(wj * g_wbase);
            (xf+ixf)->rz = (float)(rj * g_wbase);
            (xf+ixf)->iz = (float)(cj * (g_wbase / vj / 1.73205) * 1000);
        }
		else if (g_wbr[i].type == _CP)
		{
			ci = sqrt((wi * wi + ri * ri) / (vi * vi));
			cj = ci;//电流不会发生变化

			//vi = (double)(nd+(bs+g_wb[ibs].tio)->pnd)->vl;

			icp = g_wbr[i].logic;
			//			lld->delstrright((cp+icp)->id,ZN_ID_LEN);
			(cp + icp)->w = (float)(wi * g_wbase);
			(cp + icp)->r = (float)(ri * g_wbase);
			//电流需要标幺值折算，电压是10kV标幺值
			(cp + icp)->i = (float)(ci * (g_wbase / vi / 1.73205) * 1000);
			(cp + icp)->wz = (float)(wj * g_wbase);
			(cp + icp)->rz = (float)(rj * g_wbase);

			(cp + icp)->v_b = vi_out;
			(cp + icp)->adeg_b = ai * 57.29578;//转化为有名值

			(cp + icp)->v_e = vj;
			(cp + icp)->adeg_e = aj * 57.29578;

			//节点有属于0阻抗支路的情况
			for (j = 0; j < lv_zbr; j++)
			{
				if (g_zbr[j].flag)
					continue;

				if (g_zbr[j].ibs == g_wb[ibs].tio)
				{
					g_zbr[j].w = (cp + g_zbr[j].logic)->w = (cp + icp)->w;
					g_zbr[j].r = (cp + g_zbr[j].logic)->r = (cp + icp)->r;
					(cp + g_zbr[j].logic)->wz = -(cp + icp)->w;
					(cp + g_zbr[j].logic)->rz = -(cp + icp)->r;

					g_zbr[j].v = (cp + g_zbr[j].logic)->v_b = (cp + g_zbr[j].logic)->v_e = (cp + icp)->v_b;
					g_zbr[j].a = (cp + g_zbr[j].logic)->adeg_b = (cp + g_zbr[j].logic)->adeg_e = (cp + icp)->adeg_b;
					g_zbr[j].i = (cp + g_zbr[j].logic)->i = (cp + icp)->i;

					g_zbr[j].flag = 1;
				}
				else if (g_zbr[j].ibs == g_wb[zbs].tio)
				{
					g_zbr[j].w = (cp + g_zbr[j].logic)->wz = (cp + icp)->wz;
					g_zbr[j].r = (cp + g_zbr[j].logic)->rz = (cp + icp)->rz;
					(cp + g_zbr[j].logic)->w = -(cp + icp)->wz;
					(cp + g_zbr[j].logic)->r = -(cp + icp)->rz;
					g_zbr[j].v = (cp + g_zbr[j].logic)->v_b = (cp + g_zbr[j].logic)->v_e = (cp + icp)->v_e;
					g_zbr[j].a = (cp + g_zbr[j].logic)->adeg_b = (cp + g_zbr[j].logic)->adeg_e = (cp + icp)->adeg_e;
					g_zbr[j].i = (cp + g_zbr[j].logic)->i = (cp + icp)->i;

					g_zbr[j].flag = 1;
				}
			}
		}
	}

//	for (j=0; j<lv_zbr; j++)
//	{
//		if (g_zbr[j].ibs == g_wb[ibs].tio)
//		{
//			g_zbr[j].w = (sec+isec)->w;
//			g_zbr[j].r = (sec+isec)->r;
//			g_zbr[j].v = (sec+isec)->v_b;
//			g_zbr[j].a = (sec+isec)->adeg_b;
//			g_zbr[j].i = (sec+isec)->i;
//		}
//	}

    //write open sec flow
    for(i = 0; i < lv_sec; i++)
    {
        ibs = (sec+i)->ibs;
        zbs = (sec+i)->zbs;
        iout = (sec+i)->topflags & SEC_IOPEN || (bs+ibs)->iisland < 0;
        zout = (sec+i)->topflags & SEC_ZOPEN || (bs+zbs)->iisland < 0;

        if(iout || zout)
        {
            (sec+i)->w  = 0.0;
            (sec+i)->r  = 0.0;
            (sec+i)->i  = 0.0;
            (sec+i)->wz = 0.0;
            (sec+i)->rz = 0.0;
            (sec+i)->iz = 0.0;
        }
	}

    //write open xf flow
    for(i = 0; i < lv_xf; i++)
    {
        ibs = (xf+i)->ibs;
        zbs = (xf+i)->zbs;
        iout = (xf+i)->topflags & XF_IOPEN || (bs+ibs)->iisland < 0;
        zout = (xf+i)->topflags & XF_ZOPEN || (bs+zbs)->iisland < 0;

        if(iout || zout)
        {
            (xf+i)->w  = 0.0;
            (xf+i)->r_  = 0.0;
            (xf+i)->i  = 0.0;
            (xf+i)->wz = 0.0;
            (xf+i)->rz = 0.0;
            (xf+i)->iz = 0.0;
        }
    }

	for (i = 0; i < lv_cp; i++)
	{
		ibs = (cp + i)->ibs;
		zbs = (cp + i)->zbs;
		iout = (cp + i)->topflags & CP_IOPEN || (bs + ibs)->iisland < 0;
		zout = (cp + i)->topflags & CP_ZOPEN || (bs + zbs)->iisland < 0;

		if (iout || zout)
		{
			(cp + i)->w = 0.0;
			(cp + i)->r_ = 0.0;
			(cp + i)->i = 0.0;
			(cp + i)->wz = 0.0;
			(cp + i)->rz = 0.0;
		}
	}

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  PutSystemPower()
int PWRFLOW::PutSystemPower_1(int type)
{
    int			i;
    double		wso, rso, wload, rload, cap, reac;
//	ChildPtr1	*so_p, *so_p0, *ld_p, *ld_p0;

    wso = 0.0;
    rso = 0.0;
    wload = 0.0;
    rload = 0.0;
    cap = 0.0;
    reac = 0.0;

    for(i = 0; i < lv_wg; i++)
    {
		if (type != 2)
		{
			wso += g_wg[i].w;
			rso += g_wg[i].r;
		}
    }

	int ild;
	LD* ld = dsbob2->ld_tb->GetTable();
    for(i = 0; i < lv_wld; i++)
    {
		//给单个负荷赋值
		ild = g_wld[i].logic;
		if (type != 2)
		{
			wload += g_wld[i].w;
			rload += g_wld[i].r;

			//		lld->delstrright((ld+ild)->id,ZN_ID_LEN);
			(ld + ild)->w = float(g_wbase * g_wld[i].w);
			(ld + ild)->r = float(g_wbase * g_wld[i].r);
		}
		else
		{
			(ld + ild)->w = -0.001;
			(ld + ild)->r = -0.001;
		}
    }

    for(i = 0; i < lv_wsh; i++)
    {
		if (type != 2)
		{
			if (g_wsh[i].r >= 0.0) //电容
				cap += g_wsh[i].r;
			else //电抗
				reac += -g_wsh[i].r;
		}
    }

	if (type & 4)//残余网络不需要
		return 1;

	//island
	ISLAND* island = dsbob2->island_tb->GetTable();
	
	(island+g_island)->refbs = g_wb[g_slackbs].tio;//只有一个是主松弛节点

	FD* fd1 = new FD();
	strcpy(fd1->id, dsbob2->so_tb->GetDataByLog(g_wg[g_slackgen].logic)->fdid);
	(island + g_island)->reffd = dsbob2->fd_tb->FindPh(*fd1);//so的物理号给这个岛
	delete fd1;
	(island + g_island)->refminv = minVoltage;
	(island + g_island)->refmaxv = maxVoltage;
	//(island+g_island)->refso = g_wg[g_slackgen].logic;
	//(island+g_island)->vref = float(g_wb[g_slackbs].v);


	(island+g_island)->wload = float(wload * g_wbase);
	(island+g_island)->rload = float(rload * g_wbase);
	(island+g_island)->wso = float(wso * g_wbase);
	(island+g_island)->rso = float(rso * g_wbase);
//网损在迭代的时候已经写入了
	//加入到馈线的裕度里面
	
	//co
	int lv_co = dsbob2->co_tb->GetCount();
	CO* co = dsbob2->co_tb->GetTable();

	if ( ((g_wloss * g_wbase) < 0) || ( (g_wloss * g_wbase) > 5) )//潮流不收敛导致
	{
		g_wloss = g_rloss = 0;
	}

    //if(lv_co != 0)
    {
        if(g_island == 0)
        {
            co->wload = (float)(wload * g_wbase);
            co->rload = (float)(rload * g_wbase);
            co->wso = (float)(wso * g_wbase);
            co->rso = (float)(rso * g_wbase);
            co->wloss = (float)(g_wloss * g_wbase);
            co->rloss = (float)(g_rloss * g_wbase);
            co->cap = (float)(cap * g_wbase);
            co->reac = (float)(reac * g_wbase);
        }
        else
        {
			if (type != 2)
			{
				co->wload += (float)(wload * g_wbase);
				co->rload += (float)(rload * g_wbase);
				co->wso += (float)(wso * g_wbase);
				co->rso += (float)(rso * g_wbase);
				co->wloss += (float)(g_wloss * g_wbase);
				co->rloss += (float)(g_rloss * g_wbase);
				co->cap += (float)(cap * g_wbase);
				co->reac += (float)(reac * g_wbase);
			}
        }
    }

    return PWRFLOW_SUCCEED;
} 

///////////////////////////////////////////////////////////////////////////////
//  PutBusVoltage()
int PWRFLOW::PutUnResult()
{
	if (lld->mx_un==0)
		return 0;
	
	int i,ph;
//	ChildPtr1	*un_p, *un_p0;
	ChildPtr1	*so_p, *so_p0;
	char left[64],right[64];

	double dW,dR,abunW,abunR;//对侧的电源的有功和无功
	SO* so = dsbob2->so_tb->GetTable();
	FD* fd = dsbob2->fd_tb->GetTable();
	int lv_fd = dsbob2->fd_tb->GetCount();

	UN* un = dsbob2->un_tb->GetTable();
	int lv_un = dsbob2->un_tb->GetCount();
	for(i = 0; i < lv_un; i++)
	{
		strcpy(left,(un+i)->left);
		//找left侧的裕度
		ph = dsbob2->OnGetPhByfdid(left);//dsbob2->fd_tb->FindPh(FD(left));//在馈线表中的位置
		so_p = dsbob2->r_fd_so_tb->FindChildPtr1((fd+ph)->r_so);
		so_p0 = so_p;//进行指针的临时保存，指针是整型的数字
		while(so_p)
		{
			dW = ((so+so_p->Ptr)->w - (so_p->Ptr+so)->pLoss)/100;//电源点功率
			dR = ((so+so_p->Ptr)->r - (so_p->Ptr+so)->qLoss)/100;//电源点功率
			
			abunR = (so+so_p->Ptr)->maxcurrent;
			
			if (abunR <= 0)
			{
				(un+i)->l_abund_P = 0;//将视在功率的折成有功裕度
				(un+i)->l_abund_Q =	0;//无功裕度	
				//找最低电压
				(un+i)->l_abund_V =	(so+so_p->Ptr)->sr;//无功裕度
			}
			else
			{
				//将最大电流折合成有功和无功
				abunW = 100*(((1.732*abunR/1000)/10)  - sqrt(dW*dW+dR*dR));//视在功率
				
				//如果对侧的联络开关裕度为0，给一个基本值，大概是5
				(un+i)->l_abund_P = abunW * 0.98;//将视在功率的折成有功裕度
				(un+i)->l_abund_Q =	abunW * 0.2;//无功裕度	
				//找最低电压
				(un+i)->l_abund_V =	(so+so_p->Ptr)->sr;//无功裕度
			}

			so_p = so_p->Next;
		}
		::FreeChildPtr(so_p0);

		strcpy(right,(un+i)->right);
		//找left侧的裕度
		ph = dsbob2->OnGetPhByfdid(right);//dsbob2->fd_tb->FindPh(FD(right));//在馈线表中的位置
		so_p = dsbob2->r_fd_so_tb->FindChildPtr1((fd+ph)->r_so);
		so_p0 = so_p;//进行指针的临时保存，指针是整型的数字
		while(so_p)
		{
			dW = ((so+so_p->Ptr)->w - (so_p->Ptr+so)->pLoss)/100;//电源点功率
			dR = ((so+so_p->Ptr)->r - (so_p->Ptr+so)->qLoss)/100;//电源点功率
			
			abunR = (so+so_p->Ptr)->maxcurrent;
			if (abunR <= 0)
			{
				(un+i)->r_abund_P = 0;//将视在功率的折成有功裕度
				(un+i)->r_abund_Q =	0;//无功裕度	
				//找最低电压
				(un+i)->r_abund_V =	(so+so_p->Ptr)->sr;//无功裕度	
			}
			else
			{
				//将最大电流折合成有功和无功
				abunW = 100*(((1.732*abunR/1000)/10)  - sqrt(dW*dW+dR*dR));//视在功率
				
				//如果对侧的联络开关裕度为0，给一个基本值，大概是5
				(un+i)->r_abund_P = abunW * 0.98;//将视在功率的折成有功裕度
				(un+i)->r_abund_Q =	abunW * 0.2;//无功裕度	
				//找最低电压
				(un+i)->r_abund_V =	(so+so_p->Ptr)->sr;//无功裕度	
			}

			so_p = so_p->Next;
		}
		::FreeChildPtr(so_p0);

	}

	return PWRFLOW_SUCCEED;
}