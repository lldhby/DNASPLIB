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

#define ZN_ID_LEN		7  //���ε�ǰ׺z999999

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
		g_wb		= new G_WB[lld->mx_bs+MXELE];//��Ҫ����ĸ������������������ڵ����ٵ�ʱ������
	else
		g_wb		= new G_WB[mx_wb];//ǰ�ƻش����õ�ĸ�߱����Ϊ�Ǳ�
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

////��Ծ����һ�������г�������
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
//        g_wb[i].wloss = 0.0;//�ڵ������߶ε�����
//        g_wb[i].rloss = 0.0;//�ڵ������߶ε�����
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
	//��Դ��
    g_wg[0].logic = i;
    g_wg[0].ibs = ibs;
    g_wg[0].slackavr = 1; //�������ɳڽڵ�
            
	//������ĵ�Դ���ڵ����ƽ��ڵ㣬Ĭ����10.5
	g_wg[0].v = 1.05;
	
	//��Դ����׶��й����޹�����Ҫ���ڸ��ɵ�Ĵ���
	g_wg[0].w = (so+i)->w / g_wbase;	//�й�
	g_wg[0].r = (so+i)->r / g_wbase;	//�޹�
				
//////////////////////////////////////////////////////////////////////////
	//���ɵ�,��ÿ�����ɵ�����ݶ�������				
    for(i = 0; i < lv_ld; i++)
    {
        ibs = (ld+i)->ibs;

		//set load working array 
        g_wld[lv_wld].logic = i;
        g_wld[lv_wld].ibs = ibs;


		if ((ld+i)->w  == 0)//ֻ����û��ʵ�ʵ������ʱ�򣬲����ü�������Ľ������
			(ld+i)->w = g_scale_P * ((ld+i)->wm);

		if ((ld+i)->r  == 0)
			(ld+i)->r = g_scale_Q * ((ld+i)->rm);

		g_wld[lv_wld].w = (ld+i)->w / g_wbase ;
		g_wld[lv_wld].r = (ld+i)->r / g_wbase ;
		
		lv_wld++;
    }

//////////////////////////////////////////////////////////////////////////
	//������
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
	//֧·
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
		g_wbr[lv_wbr].b = (sec+i)->b0 * (g_wbase/100);//�������bһ��Ϊ0

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

	//read network data		2.������������
	if (GetNetworkData(type) == PWRFLOW_FAIL)
	{
		sprintf(out_error, "getNetworkData error. fdid=%s\n", so_id);
		lld->WriteLog(lld->logfile, out_error);
		dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

		PutFlowResult(2);
		ret--;
	}
		//g_method = 0;//����gaosi
	switch (g_method)
	{
		case 0:  //sweepǰ�ƻش�����
			if (g_loopnum > 0)
			{
				sprintf(out_error, "loop network error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

				PutFlowResult(2);
				ret--;
			}
			//order bus number by BFS	3.ͨ�������������ĸ�߱���Ż�
			if (OrderBusNumberA() == PWRFLOW_FAIL)
			{
				//add by lld 2011-11-02
				sprintf(out_error, "OrderBusNumberA error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				if(strlen(so_id)>0)
					dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

				PutFlowResult(2);
				ret--;
			}

			if (ExecuteSweep() == PWRFLOW_FAIL)
			{
				sprintf(out_error, "ExecuteSweep error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				if (strlen(so_id) > 0)
					dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

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
					dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

				PutFlowResult(2);
				ret--;
			}

			//execute Newton-Raphson method
			if (ExecuteNewton() == PWRFLOW_FAIL)
			{
				sprintf(out_error, "ExecuteNewton error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				if (strlen(so_id) > 0)
					dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

				PutFlowResult(2);
				ret--;
			}
			break;
	}

	//���������������Ҫ��д
	//write result into database���������һ�����㵺
	if (PutFlowResult(type) == PWRFLOW_FAIL)
	{
		return PWRFLOW_FAIL;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
//  ExecutePwrflow(),Ĭ���Ƕ����еĵ����д���
//������һ�������д���Ҳ����ȡһ���������ݳ������д���
//��һ���������������Բ��ùܣ���Ҫ��¼����
int PWRFLOW::ExecutePwrflow()
{
	int		ret=0, lv_island;
	// clock_t start, finish;
	bool isFail = true;

	char out_error[200];
	char so_id[80];
	int  soid;

	lv_island = dsbob2->island_tb->GetCount();

	//execute calculations ��ÿһ�������м���
	//����������Ӧ��������
	sprintf(out_error, "\n��������3\n");
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
		//ֻ�Ա����ڵ����ݽ��г�ʼ��
		//initialize working arrays		1.��ĸ�߽��г�ʼ��
		InitWorkingArray();

		//read network data		2.������������
		if(GetNetworkData() == PWRFLOW_FAIL)
		{
			soid=dsbob2->island_tb->GetDataByLog(g_island)->reffd;
			strcpy(so_id , dsbob2->fd_tb->GetDataByLog(soid)->id);
			sprintf(out_error,"getNetworkData error. fdid=%s\n",so_id);
			lld->WriteLog(lld->logfile,out_error);
			dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

			PutFlowResult(2);
			ret--;

			continue;
		}

		//����ʲô�������㣬������Ҫ��һ���������жϣ�����Ƿֲ�ʽ����1��������0
		//execute selected method

		//g_method = 0;//����gaosi
		switch(g_method)
		{

		case 0:  //sweepǰ�ƻش�����
			if (g_loopnum > 0)
			{
				soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id, dsbob2->fd_tb->GetDataByLog(soid)->id);
				sprintf(out_error, "loop network error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

				PutFlowResult(2);
				ret--;

				continue;
			}
			//order bus number by BFS	3.ͨ�������������ĸ�߱���Ż�
			if(OrderBusNumberA() == PWRFLOW_FAIL)
			{
				//add by lld 2011-11-02
				soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id, dsbob2->fd_tb->GetDataByLog(soid)->id);
				sprintf(out_error, "OrderBusNumberA error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

				PutFlowResult(2);
				ret--;

//				printf(str);
				continue;
			}

			//OnSearchXFMR("102_200219338",str);

//lld->TimeHere("end order data start sweep.");
			//execute backward and forward sweep	4.ִ��ǰ�ƻش�
			if(ExecuteSweep() == PWRFLOW_FAIL)
			{
				soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id, dsbob2->fd_tb->GetDataByLog(soid)->id);
				sprintf(out_error, "ExecuteSweep error. fdid=%s\n", so_id);
				lld->WriteLog(lld->logfile, out_error);
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

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
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

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
				dsbob2->fd_tb->GetDataByLog(soid)->q = -1;//����������

				PutFlowResult(2);
				ret--;

				continue;
			}
			break;

		//default:  //seidel & newton

			//order bus numberĸ�߱��
			//if(OrderBusNumberB() == PWRFLOW_FAIL)
			//{
			//	return PWRFLOW_FAIL;
			//}

			////form admittance matrix�γɵ��ɾ���
			//if(FormAdmittance() == PWRFLOW_FAIL)
			//{
			//	return PWRFLOW_FAIL;
			//}

			////execute Gauss-Seidel method
			////�γɵ��ɾ��������˹����
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

		//���������������Ҫ��д
		//write result into database���������һ�����㵺
		if (PutFlowResult() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

//lld->TimeHere("end sweep.");
//printf(lld->OutputAllTime());


//�����޹��Ż�ģ�飬������Ҫ�������㳱��
//		//�����޹����ξ�ȷ�أ���ÿһ��ĸ�߽��м���
//		//1.����������·�Ƿ���Ҫ�����޹������������Ҫ�Ļ����Ž�����һ���ķ���������ֱ������
//		//�о�,���ɲ�Ĺ������غ͵�ѹ
//		if ( mathtype == 1 )
//		{
//			//�����޹��Ż����򣬶Ա����߽����޹��Ż�����Ҫ�������㳱��
//			//�Ե�ǰ�ĵ�Դ�㣨�����ĵ������������Ż��������ó��������ۡ�
//			//isNeedRPO();
//			
//		}
	}

	//sprintf(out_error, "\n��������4\n");
	//lld->WriteLog(lld->logfile, out_error);

	//���Բ��ԣ�Ⱥ���͵�ѹҲ����

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

		//��ʼ����·����
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

        g_wb[i].wloss = 0.0;//�ڵ������߶ε�����
        g_wb[i].rloss = 0.0;//�ڵ������߶ε�����

//        g_wb[i].rdi = 0.0;//�����޹���ȷ��
//        g_wb[i].qv = 0.0;//��Ҫ�����ĵ���������
//        g_wb[i].b_v = 0.0;//����ǰ�ĵ�ѹ
//        g_wb[i].b_cosfi = 0.0;//����ǰ�Ĺ�������
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

		g_wbr[i].fp = 0;//�޹��Ż�ʱ��Ҫ

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
	//�ڷ��ռ����ʱ�����������Ҫ��ȡ����Ԥ�������
    if(GetSourceData(type) == PWRFLOW_FAIL)
    {
        return PWRFLOW_FAIL;
    }

    //read branch data from database
    if(GetBranchData() == PWRFLOW_FAIL)
    {
        return PWRFLOW_FAIL;
    }

//		//�������ɷ���
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
//	float	vl;//��ѹ�ȼ�����������ֵ��ʱ������
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

//	so_cnt = 0;//һ��ĵ�ֻ����һ���ֲ�ʽ��Դ
	for(i = 0; i < lv_so; i++)
	{
		g_loopnum = 0;
		ibs = (so+i)->ibs;//ֻ�ҵ������ڵĵ�Դ�㣬һ��ֻ��һ��
		dead = (so+i)->q&SO_OPEN || (bs+ibs)->iisland!=g_island || (bs+ibs)->q&BS_OPEN;

		if (!dead)
		{
			g_wg[lv_wg].logic = i;
			g_wg[lv_wg].ibs = ibs;
			//            g_wg[lv_wg].wmx = (so+i)->wmx / g_wbase;	����й�
			//            g_wg[lv_wg].rmx = (so+i)->rmx / g_wbase;	����޹�
			//            g_wg[lv_wg].wmn = (so+i)->wmn / g_wbase;	��С�й�
			//            g_wg[lv_wg].rmn = (so+i)->rmn / g_wbase;	��С�޹�
			g_wg[lv_wg].wmx = 1.0e6;//(so+i)->wmx / g_wbase;	����й�
			g_wg[lv_wg].rmx = 1.0e6;//(so+i)->rmx / g_wbase;	����޹�
			g_wg[lv_wg].wmn = -1.0e6;//(so+i)->wmn / g_wbase;	��С�й�
			g_wg[lv_wg].rmn = -1.0e6;//(so+i)->rmn / g_wbase;	��С�޹�

			if (lld->MATH_VOLTAGELEVEL == 10)
			{
				//������ĵ�Դ���ڵ����ƽ��ڵ㣬Ĭ����10.5,Ŀ���ѹ
				//Ĭ�ϵĵ�ѹ�ȼ�Ϊ10kV�������д�������ĵ�ѹ�ȼ�����Ҫ�ڱ���ֵ������д���
				//��ѹ�ı���ֵ10������100��
				slackavr = (so + i)->slackavr;
				if (slackavr == 4)//��ƽ��ڵ�
				{
					if (type != 0)
						continue;//�����ൺ�ĳ������������׶˵�Դ��

					g_vl = atof((so + i)->vl);//��Դ����׶˵�ѹֵ;
				}

				if ((slackavr == 0) || (slackavr == 4))
				{
					if ((so + i)->vtarget == 0)
						(so + i)->vtarget = g_vl;

					g_wg[lv_wg].v = ((so + i)->vtarget) / 10;//��Դ����׶˵�ѹֵ;
					g_wg[lv_wg].slackavr = slackavr;//ƽ��ڵ� 

					ratio = (so + i)->vtarget / atof((so + i)->vl);
					(bs + ibs)->v = ratio;//1.06;
					g_wg[lv_wg].vtarget = ratio;//(so+i)->vtarget/g_wbase;//ƽ��ڵ��Ŀ���ѹ
				}
				else
					if (slackavr == 1)//PQ�ڵ�
					{
						g_wg[lv_wg].w = (so + i)->w / g_wbase;	//�й�
						g_wg[lv_wg].r = (so + i)->r / g_wbase;	//�޹�
					}
					else
						if (slackavr == -1)//PV�ڵ�
						{
							g_wg[lv_wg].w = (so + i)->w / g_wbase;	//�й�
							g_wg[lv_wg].v = (so + i)->v / atof((so + i)->vl);
						}
						else
							if (slackavr == -2)//PI�ڵ�
							{
								g_wg[lv_wg].w = (so + i)->w / g_wbase;	//�й�
								g_wg[lv_wg].v = g_wg[lv_wg].w / (1.732 * (so + i)->i * ((atof((so + i)->vl)*0.01732)));
							}

				//ֻ���׶������������£�������������ɷ��䷨�����û�����⣬�ϸ��ø���ֵ����
				//�׶�������͸��ɶ˶������������£���Ϊ����������Ĵ��ڣ����ܴ��ڲ������ݲ�һ��

				//����׶˺�ĩ�˶�û�����⣬��δ���
				//����׶������⣬��ծ���Զ�ʧЧ�ˣ���Ϊ�׶˵�������Ǹ�����
				if ((so + i)->isAnalog_P)//�׶��й�������,ֻ���������������½��������������
				{
					I_value = (so + i)->I_value;//ʵ��ĳ��ڵ���
					U_value = (so + i)->vtarget;//������ѹ,�п��ܲ���10kV

					P_value = 1.732 * 0.98 * I_value * U_value / 1000;
					Q_value = 1.732 * 0.2 * I_value * U_value / 1000;

					//�������׼���ٺ�����и��ɷ����ʱ����
					g_wg[lv_wg].w = P_value / g_wbase;	//�й�

					//���������Դ�����еĸ��ɵ�������
					//�����д����Դ�б����棬�Ժ��ڼ����ʱ��Ϳ�����������и��ɴ�С������
					sumP = 0;//�����ܼ�
					for (int j = 0; j < lv_ld; j++)
					{
						ibs = (ld + j)->ibs;
						dead = (ld + j)->q&LD_OPEN || (bs + ibs)->iisland != g_island || (bs + ibs)->q&BS_OPEN;

						if (!dead)
						{
							//��ѹ������*��ծ�ʣ���ծ���ڶ������ݵ�ʱ���Ѿ�������
							sumP += (ld + j)->wm / 1000 / g_wbase;//kva
						}
					}
					if (sumP == 0)//�����߶�û�и���
					{
						g_wg[lv_wg].w = 0;	//�й�
						g_scale_P = 0;
					}
					else
					{
						//�����ʵ��ĸ���ֵ����ʵ���������ٶ�10%������
						//������������
						g_scale_P = (g_wg[lv_wg].w / sumP) * (1 - lld->LINELOSS);
					}
				}
				//����ʵ��ĸ����ܼӺ͸��ɵĶ������������ɵı���
				(so + i)->fScaleP = g_scale_P;

				if ((so + i)->isAnalog_Q)//�׶��޹�������
				{
					//�������׼���ٺ�����и��ɷ����ʱ����
					//g_wg[lv_wg].r = (so + i)->r / g_wbase;	
					g_wg[lv_wg].r = Q_value / g_wbase;	
					
					//���������Դ�����еĸ��ɵ�������
					//�����д����Դ�б����棬�Ժ��ڼ����ʱ��Ϳ�����������и��ɴ�С������
					sumQ = 0;//�����ܼ�
					for (int j = 0; j < lv_ld; j++)
					{
						ibs = (ld + j)->ibs;
						dead = (ld + j)->q&LD_OPEN || (bs + ibs)->iisland != g_island || (bs + ibs)->q&BS_OPEN;

						if (!dead)
						{
							//��ѹ������*��ծ�ʣ���ծ���ڶ������ݵ�ʱ���Ѿ�������
							sumQ += (ld + j)->rm / 1000 / g_wbase;//kva
						}
					}

					if (sumQ == 0)//�����߶�û�и���
					{
						g_wg[lv_wg].r = 0;	//�й�
						g_scale_Q = 0;
					}
					else
					{
						//�����ʵ��ĸ���ֵ����ʵ���������ٶ�3.8%������
						g_scale_Q = (g_wg[lv_wg].r / sumQ) * (1- lld->LINELOSS);
					}
				}
				//����ʵ��ĸ����ܼӺ͸��ɵĶ������������ɵı���
				(so + i)->fScaleQ = g_scale_Q;

				//g_wg[lv_wg].ss = (so+i)->ss / g_wbase;//��Դ��Ķ�·����
				if (slackavr == 4)
				{
					//û����д��صĽӵ�ֵ
					if ((so + i)->ss == 0)
					{
						g_wg[lv_wg].ss = 360 / g_wbase;//��Դ��Ķ�·����
						g_wg[lv_wg].x0 = 2;//�ӵر�翹
						g_wg[lv_wg].r0 = 10;//�ӵص��裬һ��Ϊ10ŷķ������������ԣ������ӵ�
					}
					else
					{
						g_wg[lv_wg].ss = (so + i)->ss / g_wbase;
						g_wg[lv_wg].x0 = 3 * (so + i)->sw;//�ӵر�翹
						g_wg[lv_wg].r0 = 3 * (so + i)->sr + (so + i)->pLoss;//�ӵص��裬һ��Ϊ10ŷķ������������ԣ������ӵ�
					}
				}
				else
					g_wg[lv_wg].ss = (so + i)->ss;

				lv_wg++;
				if (lv_wg > 1)//���ֶ����Դ��
				{
					//���ܳ����˷ֲ�ʽ��Դ
					g_loopnum = lv_wg - 1;//��������,��Ϊ�����ĸ���		
					this->g_method = 1;//��Ҫ��ţ�ٷ�����
				}
			}
			else
			{
				//110kV��ĸ�߶��ǵ�Դ��
				g_wg[lv_wg].slackavr = (so + i)->slackavr;//ƽ��ڵ� 
				g_wg[lv_wg].x1 = (so + i)->sw;//�翹
				g_wg[lv_wg].r1 = (so + i)->sr;//����
				if (((so + i)->slackavr != 0) && ((so + i)->slackavr != 4))
				{
					g_wg[lv_wg].w = (so + i)->w;//�й�P
					g_wg[lv_wg].r = (so + i)->r;//�޹�Q
				}
				else
				{
					//ͨ����ѹ�ȼ����õ���Ҫ������ֵ


					g_wg[lv_wg].vtarget = (bs + ibs)->v = g_wg[lv_wg].v = (so + i)->vtarget / 10;//��Դ����׶˵�ѹֵ;
				}

				lv_wg++;
			}
		}
	}

	if (type == 1)
	{
		//ģ������
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
				g_wg[lv_wg].wmx = 1.0e6;//(so+i)->wmx / g_wbase;	����й�
				g_wg[lv_wg].rmx = 1.0e6;//(so+i)->rmx / g_wbase;	����޹�
				g_wg[lv_wg].wmn = -1.0e6;//(so+i)->wmn / g_wbase;	��С�й�
				g_wg[lv_wg].rmn = -1.0e6;//(so+i)->rmn / g_wbase;	��С�޹�

				if (lld->MATH_VOLTAGELEVEL == 10)
				{
					g_wg[lv_wg].v = 1.0;//��Դ����׶˵�ѹֵ;
					g_wg[lv_wg].slackavr = 0;//ƽ��ڵ� 

					(bs + ibs)->v = 1.0;//1.06;
					g_wg[lv_wg].vtarget = 1.0;//ƽ��ڵ��Ŀ���ѹ
					lv_wg++;
					if (lv_wg > 1)//���ֶ����Դ��
					{
						//���ܳ����˷ֲ�ʽ��Դ
						g_loopnum = lv_wg - 1;//��������,��Ϊ�����ĸ���		
						this->g_method = 1;//��Ҫ��ţ�ٷ�����
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

	//read sec data into working branch array		��ȡ��·��
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
				if ( (ibs == zbs)//0�迹֧·
					|| (((sec + i)->r1 == 0) && ((sec + i)->x1 == 0)) )
				{
					//����ڵ�������⴦�������
	                g_zbr[lv_zbr].logic = i;
	                g_zbr[lv_zbr++].ibs = ibs;

					continue;
				}

				//��·�Ĳ�����֮ǰ�Ѿ��������
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
				g_wbr[lv_wbr].r = (sec+i)->r1;//����
				g_wbr[lv_wbr].x = (sec+i)->x1;
				g_wbr[lv_wbr].b = (sec+i)->b1;//�������bһ��Ϊ0

				//if (((sec + i)->r0 == 0) && ((sec + i)->x0 == 0))
				//{//ȱʡ��4��������
				//	g_wbr[lv_wbr].r0 = 4*(sec + i)->r1;
				//	g_wbr[lv_wbr].x0 = 4*(sec + i)->x1;
				//}
				//else
				{
					g_wbr[lv_wbr].r0 = (sec + i)->r0;
					g_wbr[lv_wbr].x0 = (sec + i)->x0;
				}
				g_wbr[lv_wbr].b0 = (sec + i)->b0;//�������bһ��Ϊ0

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
					r = (xf + i)->r1;//��������ĵ�ѹ�ȼ��������ֵ
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

			if ((ibs == zbs)//0�迹֧·
				|| ((cp + i)->r == 0))
			{
				//����ڵ�������⴦�������
				g_zbr[lv_zbr].logic = i;
				g_zbr[lv_zbr++].ibs = ibs;

				continue;
			}

			//��Ҫ�ж������ֵ������������Ͳ��������ò�һ��
			if ((ibs != -1) && (zbs != -1))
			{	//���ڲ���������
				dead = (cp + i)->q&LD_OPEN || (bs + ibs)->iisland != g_island || (bs + ibs)->q&BS_OPEN;

				if (!dead)
				{
					g_wbr[lv_wbr].type = _CP;
					g_wbr[lv_wbr].logic = i;
					g_wbr[lv_wbr].ibs = ibs;
					g_wbr[lv_wbr].zbs = zbs;

					//�����ݿ�
					g_wbr[lv_wbr].r = 0;
					g_wbr[lv_wbr].x = -1 / (((cp + i)->r / 1000) / g_wbase);
					g_wbr[lv_wbr].b = 0;//�������bһ��Ϊ0

					lv_wbr++;
				}
			}
		}
	}



    //read xf data into branch working array	��ȡ��ѹ������֧·��
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
					r = (xf + i)->r1;//��������ĵ�ѹ�ȼ��������ֵ
					x = (xf + i)->x1;

					//determine actual ratio by tap position
					t = 1.0;
					itapty = (xf + i)->itapty;
					iztapty = (xf + i)->iztapty;

					//�г�ͷ������������Ҫ������
					//��Ƚ���Ӱ�쵽����֧·��r,x,g1,b1,g2,b2��ֵ����ڵ��˳���޹�
					if (itapty != -1)
					{
						if (((xf + i)->tap != (tapty + itapty)->nom//��ѹ���ͷ�����м�� 
							&& (xf + i)->tap <= (tapty + itapty)->mx //��ͷ������ķ�Χ��
							&& (xf + i)->tap >= (tapty + itapty)->mn)
							|| ((xf + i)->ztap != (tapty + iztapty)->nom//��ѹ���ͷ�����м�� 
								&& (xf + i)->ztap <= (tapty + iztapty)->mx //��ͷ������ķ�Χ��
								&& (xf + i)->ztap >= (tapty + iztapty)->mn))
						{
							//���=���м䵵λ��Ӧ�ĵ�ѹ+(��ǰ��λ-�м䵵λ)*��ͷ����������/�м䵵λ��Ӧ�ĵ�ѹ
							t = (xf + i)->kvnom + (xf + i)->kvnom * ((xf + i)->tap - (tapty + itapty)->nom) * (tapty + itapty)->step;
							t /= (xf + i)->zkvnom + (xf + i)->zkvnom * ((xf + i)->ztap - (tapty + iztapty)->nom) * (tapty + iztapty)->step;
							t *= (xf + i)->zkvnom / (xf + i)->kvnom;

							//set non-rating ratio winding array
							//��λ��������ص��������������ҲҪ����
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

			if ( (ibs == zbs)//0�迹֧·
				|| ((cp + i)->r==0) )
			{
				//����ڵ�������⴦�������
				g_zbr[lv_zbr].logic = i;
				g_zbr[lv_zbr++].ibs = ibs;

				continue;
			}

			//��Ҫ�ж������ֵ������������Ͳ��������ò�һ��
			if ((ibs != -1) && (zbs != -1))
			{	//���ڲ���������
				dead = (cp + i)->q&LD_OPEN || (bs + ibs)->iisland != g_island || (bs + ibs)->q&BS_OPEN;

				if (!dead)
				{
					g_wbr[lv_wbr].type = _CP;
					g_wbr[lv_wbr].logic = i;
					g_wbr[lv_wbr].ibs = ibs;
					g_wbr[lv_wbr].zbs = zbs;

					//�����ݿ�
					g_wbr[lv_wbr].r = 0;
					g_wbr[lv_wbr].x = - 1/ (((cp + i)->r/1000)/ g_wbase);
					g_wbr[lv_wbr].b = 0;//�������bһ��Ϊ0

					lv_wbr++;
				}
			}
		}
	}
*/

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//���ɷ����
int PWRFLOW::OnDistributeLoadData()
{
	int		i/*,j,k,m,n*/;
	int		ibs, dead;
//	int     lv_p,lv_q;
	//int     flag;//���ؽڵ��־
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
    //ͨ���׶˿���������为��
	//��Դ�ܼ�
    sumPso=sumQso=0;
    for(i = 0; i < lv_wg; i++)
    {
		sumPso += g_wbase * g_wg[i].w;	//�й�,mw
		sumQso += g_wbase * g_wg[i].r;	//�޹�
	}

    sumP=sumQ=0;//�����ܼ�
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
    
	//����ʵ��ĸ����ܼӺ͸��ɵĶ������������ɵı���
	//�����ʵ��ĸ���ֵ����ʵ���������ٶ�5%������
	g_scale_P = (sumPso/sumP) * 0.95;
	g_scale_Q = (sumQso/sumQ) * 0.95;
	//�����д����Դ�б����棬�Ժ��ڼ����ʱ��Ϳ�����������и��ɴ�С������
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
				if ((ld+i)->w  == 0)//ֻ����û��ʵ�ʵ������ʱ�򣬲����ü�������Ľ������
					(ld+i)->w = g_scale_P * ((ld+i)->wm);

				if ((ld+i)->r  == 0)
					(ld+i)->r = g_scale_Q * ((ld+i)->rm);
			}
        }
    }
 //��ѹ�û�����У��
 /*   for (i = 0; i < lv_analog; i++)
    {
       //judge 0204 or 0205 from table analog	
		strncpy(abb1,(analog+i)->id,4);
		abb1[4]='\0';
		if ( strncmp(abb1,"0204",4) == 0)
		{
		   //�õ���ѹ���ɱ��abb2��p/qָʾflag,flag=0��ʾ�й���flag=1��ʾ�޹�
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
				//�õ����ɱ��
				strncpy(abbtemp1,(ld+j)->id,13);
					abbtemp1[13]='\0';
                for ( k = 4; k < 13; k++)
                {
					abb3[k-4] = abbtemp1[k];
                }
               //����ѹ���ɶ�Ӧ�й����޹���Ӧд��
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

			}//����ѭ��
		}//��ѹ����ѭ��
    }
	
   //��������У��:ͨ�����ɽڵ�����sec���õ���һ�˽ڵ�,����sec���иýڵ��Ƿ������������ɣ������ɽڵ㼰��������ͳ��
   //ͨ���ýڵ��������ر��õ����ر�ţ�����analog���������������Ϣ
	for ( i = 0; i < lv_ld; i++)
	{
		int count = 0;
		char cbno[13],cbtemp[9];
		int ldno = (ld+i)->ind;//���ɽڵ�
		num[count] = (ld+i)->ind;//�����ɽڵ���д洢����ֹ�������Ӷ���������
		for ( j = 0; j < lv_sec; j++)
		{
			if ( ldno == (sec+j)->ind )
			{
                flag = (sec+j)->iznd; //���ؽڵ�
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
                 }//Ѱ��ͬһ�����ϸ��ɽڵ�
			}
		}
		//���������б��õ����
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
	   //����������
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
	  //���为��
	  //1������ֻ����һ������
	  if ( count == 0)
	  {
		  (ld+i)->w = sumPcb;
		  (ld+i)->r = sumQcp;
	  }
	  //2���������Ӷ������
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

	}//�׶˿�������У������*/
    

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  GetLoadData()				��ȡ���ɱ�
//��������⣬������ʵ�������������û�����⣬���׶�������й���
//�����ĩ��������ȫ�Ļ����ڳ���������ʱ��Ҫ�����ɵķ�������
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
	
	//�ۼƸ��ɳ���20MW��˵�������ˣ�����������
	if (g_vl == 0)
		g_vl = 10;
	if ((lld->MATH_VOLTAGELEVEL == 10) && (( ((10/g_vl)*sum_P>0.2) || ((10/g_vl)*sum_Q > 0.05) )))
	{
		soid = dsbob2->island_tb->GetDataByLog(g_island)->reffd;
		strcpy(fdid , dsbob2->fd_tb->GetDataByLog(soid)->id);		

		sprintf(sLinelog, "error: %s���߸����й��ۼ�ֵ����20MVA���޹��ۼӳ���5MVar�����顣sum_P=%f MW,sum_Q=%f MVar\n",fdid,100*sum_P,100*sum_Q);
		lld->WriteLog(lld->logfile,sLinelog);
	
		//lld->add_globalexception(sLinelog);

		return PWRFLOW_FAIL;
	}

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  GetShuntData()	����֧·������������翹����
int PWRFLOW::GetShuntData()
{
	int		i;
	//�����޹��ڵ�
	if (cpbss.size() > 0)
	{
		for (i = 0;i < cpbss.size();i++)
		{
			g_wsh[lv_wsh].ibs = cpbss.at(i);
			g_wsh[lv_wsh].r = 0;
			g_wsh[lv_wsh].rnom = (cps.at(i) / 1000) / g_wbase;//�����ʱ���Ѿ���KVar
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
			continue;//���ڹ���������

		//��Ҫ�ж������ֵ������������Ͳ��������ò�һ��
		if ((ibs != -1) && (zbs != -1))
			continue;//���ڲ���������
		
		dead = (cp+i)->q&LD_OPEN || (bs+ibs)->iisland!=g_island || (bs+ibs)->q&BS_OPEN;

        if(!dead)
        {
            //set shunt working array 
            g_wsh[lv_wsh].logic = i;
            g_wsh[lv_wsh].ibs = ibs;
            g_wsh[lv_wsh].r = 0;
            g_wsh[lv_wsh].rnom = ((cp+i)->r/1000) / g_wbase;//�����ʱ���Ѿ���KVar
            lv_wsh++;
        }
    }

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  OrderBusNumberA()
//ǰ�ƻ��Ʒ�����������ĳ���
//��ĸ�߽��б��
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
	//�ҵ��ɳڽڵ�(ƽ��ڵ�),�Է��������ԣ������ж��ٸ����������ֻ��һ��ƽ��ڵ�
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
	//outΪ�ڵ��Ż�ǰ��inΪ�ڵ���Ż���,tio�õ��Ż�ǰ�ı�ţ�toi�õ��Ż���ı��
	//g_wb�ȴ���Ż�ǰ�ı�ţ�Ҳ����Ż���ı��
	
	//�Է������ı�Ϊ����������
	g_bsmx = lv_wb = lv_wbr + 1;//n=m+1
    
    ibs = g_wg[g_slackgen].ibs;
    g_wb[g_slackbs].tio = ibs;
    g_wb[ibs].toi = g_slackbs;

    //initialize working array
    for(i = 0; i < lv_wbr; i++)
    {
        wbr[i] = 0;//��ÿ�����߶���Ϊδ����
    }

    //initialize current bus number and total bus number
    current = 1;//�õ����µĲ��������Ľڵ�����
    total = 1;//�����������нڵ㣬ÿ�ҵ�һ���ڵ㣬��Ҫ�������1

    //order bus number from root, layer by layer
	//��ĸ�߽��б�ţ��Ӹ��ڵ㿪ʼ�õ��𲽵ı��
	//������Ҫ���ǲ��ö�������ģʽ����
    while(total < lv_wb)//ֻҪ����û���ҵ��Ľڵ㣬��Ҫһֱ����ȥ
    {
        total0 = total;//����ĵ�һ���ڵ㣬׼����չ�ĵ�һ���ڵ�

        //find buses in this new layer
		//ÿһ����չ��Ҫ�����е�֧·��һ�飬ֻҪ��û�д�����Ķ�Ҫ���д���
		//�����û�д����֧·Խ��Խ�٣��ٶȻ�Խ��Խ��
        for( i = 0; i < lv_wbr; i++)
        {
            if(!wbr[i])//������߶�δ����
            {
                ibs = g_wbr[i].ibs;
                zbs = g_wbr[i].zbs;

                flag = 0;

				//���ѭ����ʾ�ӱ��㿪ʼ���нڵ��ѭ����Ҳ�п���һ��Ҳ�Ҳ���
				//�Ҳ����󿪹�ѭ����һ��֧·
                for(j = total0 - current; j < total0; j++)
                {//j����һ����չ�Ľڵ��
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

                if(flag)//�п���һ���ڵ�ᷢ�ֺܶ����νڵ�
                {
                    wbr[i] = 1;

					//������������������γɻ�
                    //if(loop == loop0)  //this branch does not form a loop
                    {
                        g_wb[total].tio = zbs;	//�Ż�ǰ�ı��
                        g_wb[zbs].toi = total;	//�Ż���ı��
                        g_wb[total].jb = i;		//�ڵ���ϼ�֧·
                        g_wb[total].jc = g_wb[ibs].toi;	//�ڵ���ϼ��ڵ�
                        total++;//��չ�Ľڵ�
                    }
                }
            }
        }

        //get bus number in this new layer
		//ÿ����һ���㣬�������Ҫѭ�����е�֧·���ܵõ�
        current = total - total0;//�õ����µ�ͼ��Ĺ��еĽڵ���������Щ�ڵ�ȥ��չ������
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
	//�ر�ע�⣬���������ܺ㶨�޹��������͵�ѹ�йأ������������������
    for(i = 0; i < lv_wg; i++)
    {
        ibs = g_wg[i].ibs;
        g_wb[ibs].nw += g_wg[i].w;
        g_wb[ibs].nr += g_wg[i].r;
    }

    for(i = 0; i < lv_wld; i++)
    {
        ibs = g_wld[i].ibs;
        g_wb[ibs].nw -= g_wld[i].w;//�����ɱ��-�ţ���ʾ��ע��
        g_wb[ibs].nr -= g_wld[i].r;
    }

    //initialize slack and pv bus voltage
	
    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].slackavr==0)//�ɳ�ĸ�ߣ�ƽ��ڵ�
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

	//g_wb ĸ�ߵ�ȫ�ֱ���

    return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  OrderBusNumberB()
//ĸ���Ż���ţ���Ҫ�Ǽ�����ȥʱ��ע��Ԫ��Ҳ���ǽڵ�Ķ���С���ȱ�ţ���ĺ���
//**��̬�Ż���ţ��ڵ����С�����ȱ�ţ���ͬ�������ţ�ֱ�������򵥣�
//**�붯̬�Ż���ţ�����һ������С�Ľڵ��ţ�������ڵ���ȥ��������С�Ķȱ�ţ���
//��ȥ,�Դ����ƣ�ֱ�������еĽڵ�ȫ����д��ϡ�Ӧ����ࡣ
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

	g_bsmx = 0;//�ڻ����У������� n=m+1���ж�

	//count number of branches at each bus, get max bus number
	//��ÿ����֧�߽���ѭ����������ĩ�ڵ������֧·��
	for (i = 0; i < lv_wbr; i++)
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;

		g_wb[ibs].ja++;//�׽ڵ�����֧·����1
		g_wb[zbs].ja++;

		if (g_bsmx < ibs)//�õ�����ĸ�߱�ţ�ĸ�߱�Ŷ����ϸ��1��ʼ���
		{
			g_bsmx = ibs;
		}
		if (g_bsmx < zbs)
		{
			g_bsmx = zbs;
		}
	}

	//count number of buses which have same branches
	//����ڵ�Ķȣ�Ҳ����ÿ���ڵ����ӵ�֧·����
	//��Ҫ������ѽڵ�Ķ�ja�ͽڵ��ΪX�Ľڵ��������jb
	//���ܽڵ�1�Ķ�Ϊ3��g_wb[1].ja=3;��Ϊ1�Ľڵ����Ϊ100����ôg_wb[1].jb=100��
	for (i = 0; i <= g_bsmx; i++)
	{
		j = g_wb[i].ja;
		if (j > 0)//û�����ӵĲ���Ҫ����
		{
			g_wb[j].jb++;//�ڵ�Ķ�Ϊj�Ľڵ��������һ����ja�ڵ�ȣ�jb��Ϊj�Ľڵ����
		}
	}

	//get total number of buses
	//�������ȵĽڵ��������ۼӣ���ȻҪ��ȥ�ڵ��Ϊ0�Ľڵ�
	for (i = 1; i < MXELE; i++)
	{
		lv_wb += g_wb[i].jb;
	}

	if (lv_wb == 0) lv_wb++;//�ڵ�һ����û��

	//first position of buses which have same branches���ڵ����Ķ�Ϊ27
	//����ͬ�ȵĽڵ�ĵ�һ��λ��
	for (i = 0; i < MXELE; i++)
	{
		g_wb[i + 1].jb += g_wb[i].jb;
	}

	//select slack bus, slack and regulate generator
	nextbs = lv_wb - 1;
	g_slackbs = nextbs--;//�ɳ�ĸ�ߣ�������һ��һ��Ҫ����Դ�㣬ʣ�µĴӺ���ǰ���θ�PQ,PV�ڵ�
	g_slackbsnum = 0;//ƽ��ڵ�ĸ���

	//������ó�ƽ��ڵ㣬�˳�������ѡ�����ԣ�ȵķ��������Ϊƽ��ڵ㡣
	g_slackgen = 0;//�ɳڷ�����������Դ�ڵ�
	//���ܴ��ڶ��ƽ��ڵ�
	for (i = 0; i < lv_wg; i++)
	{
		if (g_wg[i].slackavr == 0)//��ƽ��ڵ�
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
	g_wb[g_slackbs].tio = ibs;//���ڵ��⣬���Ż����Ż�ǰ��Ҳ�����Ż�ǰ�ı��
	g_wb[ibs].toi = g_slackbs;
	k = g_wb[ibs].ja;
	if (k == 0)//��Դ��Ķ�Ϊ0
	{
		return 0;
	}

	for (j = k; j < MXELE; j++)
	{
		g_wb[j].jb--;//���ȵĸ�������1
	}
	g_wb[ibs].ja = 0;//��Դ��Ķȹ�0�����ý���

	//������ƽ��ڵ�
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		if (ibs == g_wg[g_slackgen].ibs)//��ƽ��ڵ�
			continue;

		k = g_wb[ibs].ja;//jaΪ0��ʾ�ǵ�Դ�㣨�ɳڽڵ㣩
		if (g_wg[i].slackavr == 0)//pv�ڵ���Ҫ�ڼ���ǰ��ָ���������Ͳ����ж���
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
	//һ������¶�û��p-v�ڵ㣬�ӷֲ�ʽ��Դ����ܻ��У���Ҳ��һ��
	g_pvbsnum = 0;
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		k = g_wb[ibs].ja;//jaΪ0��ʾ�ǵ�Դ�㣨�ɳڽڵ㣩

		//if(k != 0 && g_wg[i].slackavr == -1)
		if (g_wg[i].slackavr == -1)//pv�ڵ���Ҫ�ڼ���ǰ��ָ���������Ͳ����ж���
		{
			flag = 0;
			for (j = 0; j < lv_wg; j++)
			{
				//�������Լ���������ƽ��ڵ㣬������һ��ĸ���ϵ��豸
				if (j != i && g_wg[j].slackavr != 0 && g_wg[j].ibs == g_wg[i].ibs)
				{
					flag = 1;
					break;
				}
			}
			if (flag)
			{
				g_wg[i].slackavr = 1; //��ΪPQ�ڵ㣬lld 2018-7-9
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
	//��PQ�ڵ���б��
	for (i = 0; i <= g_bsmx; i++)
	{
		k = g_wb[i].ja;//��Դ����ǰ���Ѿ����ȹ�0�ˣ�pv�ڵ�Ҳ��
		if (k != 0)
		{
			nextbs = g_wb[k - 1].jb++;//ǰ��k�ӹ�1������ع飬ÿһ���ȵĽڵ㶼��ǰ��Ԥ���˿�λ
			g_wb[nextbs].tio = i;
			g_wb[i].toi = nextbs;
			g_wb[i].ja = 0;//�Ѿ�������ˣ�����Ҫ�ٴ���
		}
	}

	//reset g_wb[i].jb to zero��jb�Ƕȵĸ��������ñ������ع�
	for (i = 0; i <= g_bsmx; i++)
	{
		g_wb[i].jb = 0;
	}

	//reset generator, load and line connected buses
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		regbs = g_wg[i].regbs;//�ο�ĸ�ߣ�׼����д��ʱ����
		g_wg[i].ibs = g_wb[ibs].toi;//�µķ��������Դ�㣩���
		g_wg[i].regbs = g_wb[regbs].toi;//�µķ������ԭ��ĸ�߱��е�����λ��
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

	for (i = 0; i < lv_wg; i++)  //p-v������ǰp-qһ������ֵ�����޸�
	{
		if (g_wg[i].slackavr == -1)
		{
			ibs = g_wg[i].ibs;
			g_wb[ibs].ja = -1;
		}
	}
	//����ƽ��ڵ�
	for (i = 0; i < lv_wg; i++)  //p-v������ǰp-qһ������ֵ�����޸�
	{
		if (g_wg[i].slackavr == 0)
		{
			ibs = g_wg[i].ibs;
			g_wb[ibs].ja = 1;
		}
	}

	if (g_wg[g_slackgen].slackavr == 4)
		g_wg[g_slackgen].slackavr = 0;

	g_wb[g_slackbs].ja = 1;  //slack������ǰp-qһ������ֵ�����޸�

	//set bus injection power
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;//��ź��
		g_wb[ibs].nw += g_wg[i].w;
		g_wb[ibs].nr += g_wg[i].r;
	}

	for (i = 0; i < lv_wld; i++)
	{
		ibs = g_wld[i].ibs;
		g_wb[ibs].nw -= g_wld[i].w;//��������ʱ�ĸ���ע��Ϊ-
		g_wb[ibs].nr -= g_wld[i].r;
	}

	//initialize slack and pv bus voltage
	for (i = 0; i < lv_wg; i++)
	{
		if ((g_wg[i].slackavr == 0) || (g_wg[i].slackavr == -1))//-1Ҳ��Ҫͨ����
		{
			ibs = g_wg[i].ibs;
			g_wb[ibs].v = g_wg[i].v;
			//            g_wb[ibs].v = dsbob2->bs_tb->GetDataByPh(g_wb[ibs].tio)->v;
			//			if(g_wb[ibs].v < 0.8 || g_wb[ibs].v > 1.2) //�п��ܳ���20kV��35kV��ר�����
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
		if (g_wb[i].ja == 0)//��Դ���Ѿ�==1��pv�Ѿ�==-1
		{
			g_wb[i].v = g_wb[g_slackbs].v;//�͵�Դ������ͬ���ĳ�ֵ
		}
	}

	return PWRFLOW_SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////
//  OrderBusNumberC()
//ĸ���Ż���ţ���Ҫ�Ǽ�����ȥʱ��ע��Ԫ��Ҳ���ǽڵ�Ķ���С���ȱ�ţ���ĺ���
//**��̬�Ż���ţ��ڵ����С�����ȱ�ţ���ͬ�������ţ�ֱ�������򵥣�
//**�붯̬�Ż���ţ�����һ������С�Ľڵ��ţ�������ڵ���ȥ��������С�Ķȱ�ţ���
//��ȥ,�Դ����ƣ�ֱ�������еĽڵ�ȫ����д��ϡ�Ӧ����ࡣ
//
//	Overview:	This subroutine optimizes bus number by the static ordering
//				method, i.e, the bus which has fewer connected lines will be
//				given a smaller bus number, the bus which has more connected
//				lines will be given a larger bus number.
//
///////////////////////////////////////////////////////////////////////////////
int PWRFLOW::OrderBusNumberC()//�붯̬�Ż���ŷ�
{
//    int    i, j, k;
//    int    ibs, zbs, regbs, nextbs, flag;
//
//    g_bsmx = 0;//�ڻ����У������� n=m+1���ж�
//
//    //count number of branches at each bus, get max bus number
//	//��ÿ����֧�߽���ѭ����������ĩ�ڵ������֧·��,Ҳ���ǽڵ�Ķ�
//    for(i = 0; i < lv_wbr; i++)
//    {
//        ibs = g_wbr[i].ibs;
//        zbs = g_wbr[i].zbs;
//
//        g_wb[ibs].ja++;//�׽ڵ�����֧·����1
//        g_wb[zbs].ja++;
//
//        if(g_bsmx < ibs)//�õ�����ĸ�߱�ţ�ĸ�߱�Ŷ����ϸ��1��ʼ���
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
//	//����ڵ�Ķȣ�Ҳ����ÿ���ڵ����ӵ�֧·����
//    for(i = 0; i <= g_bsmx; i++)
//    {
//        j = g_wb[i].ja;
//        if(j > 0)//û�����ӵĲ���Ҫ����
//        {
//            g_wb[j].jb++;//�ڵ�Ķ�Ϊj�Ľڵ��������һ����ja�ڵ�ȣ�jb��Ϊj�Ľڵ����
//        }
//    }
//
//    //get total number of buses
//    for(i = 1; i < MXELE; i++)
//    {
//        lv_wb += g_wb[i].jb;
//    }
//
//	if(lv_wb == 0) lv_wb++;//�ڵ�һ����û��
//




    return PWRFLOW_SUCCEED;

}

///////////////////////////////////////////////////////////////////////////////
//  FormAdmittance()
//  ���ɾ����γɣ��������󣬴�Ԫ�ص��к��У��Խ�Ԫ�أ��ǶԽ�Ԫ�أ��кţ��кţ������ӵ�����
//�Խ�Ԫ���൱����ƫ����
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
        g_yu[i].i = 0;//�����ǵ���
        g_yu[i].j = 0;//�����ǵ���
        g_yu[i].p = 0;//Ԫ�ض�Ӧ����
        g_yu[i].d = 0;//�Խ�Ԫ��λ��
        g_yu[i].g = 0.0;//���ɵ�ʵ��
        g_yu[i].b = 0.0;//���ɵ��鲿
    }
    lv_yu = 0;

    //determine positions of diagonal elements������Ľ�����֧·��һ����
	//ȷ���Խ�Ԫ�ص�λ��
	for(i = 0; i < lv_wbr; i++)//ȷ���Խ�Ԫ�صĸ���
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;

		//ȡ��С��ĸ�߱�ų������˱�Ų��Ƿֲ���
        if(ibs > zbs) ibs = zbs;

        g_yu[ibs+1].d++;//ȷ���ڵ����������ڵ���������൱�ڻ����ɵ�����
    }

    for(i = 1; i < lv_wb; i++)//ȷ���Խ�Ԫ���������е�λ��
    {
		//��һ���Խ�Ԫ�ص�λ�ã������ڲ��ҵ�ʱ������������Խ�Ԫ��֮��Ĳ�ֵ����ȡ
        j = g_yu[i].d + g_yu[i-1].d + 1;
        g_yu[i].d = j;
        g_yu[j].i = i;//ÿ���ڵ��Ӧһ��
        g_yu[j].j = i;//�Խ�Ԫ�ص��к������
    }

    //determine number of elements in the matrix
	//�����Ǹ��Խ�Ԫ�ؾ�����������ĸ��������в���0Ԫ��
    lv_yu = g_yu[lv_wb-1].d + 1;

    //determine row positions of elements
	//ȷ��ÿ��Ԫ���е�λ��
    for(i = 0; i < lv_wb; i++)
    {
        jmn = g_yu[i].d + 1;//��һ���Խ�Ԫ��
        jmx = g_yu[i+1].d;

		//���е���һ��֮��������Ԫ�ط�0Ԫ��
        for(k = jmn; k < jmx; k++)//�����Խ�Ԫ��֮�������Ԫ�ض�������Խ�����ͬ�����к�
        {
            g_yu[k].i = i;
        }
    }

    //determine column positions of elements
	//ȷ��ÿ��Ԫ���е�λ�ã��ǶԽ�Ԫ��
	//ѭ��֧·
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;
        zbs = g_wbr[i].zbs;

		//�����ڵ�ţ���С��������ֻ��Ҫ���������ǣ����Կ�����������
        if(zbs < ibs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

        jmn = g_yu[ibs].d + 1;
        jmx = g_yu[ibs+1].d;

		//��������ȫ���ķǶԽ�Ԫ�أ�����Ϊ���Խ�Ԫ��֮��
        for(k = jmn; k < jmx; k++)
        {
            j = g_yu[k].j;//��ʼ��Ϊ0

            if(j == 0)//��һ�γ���
            {
                g_yu[k].j = zbs;//��ĩ�˽ڵ�ֱ�Ӹ�
                break;
            }
            else if(j > zbs)//�޸������Ľڵ���
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
	//�е�����ÿһ�дӵ�һ����ʼ��һֱ���Խ�Ԫ�ص�����˳��
	
	//��ʼ��
    for(k = 0; k < lv_wb; k++)
    {
        j = g_yu[k].d;//�Խ�Ԫ�ص�λ��
        jw[k] = j;      //working array����ʱ���飬��ڵ�ĶԽ�λ��
        g_yu[j].p = j;  //point to itself��ÿ�ж���ʱָ���Լ����ӶԽ�Ԫ�ؿ�ʼ���������ά��
    }

    i = 0;//���н��д���
    for(k = 0; k < lv_yu; k++)
    {
        if(k >= g_yu[i+1].d) i++;  //next row

        j = g_yu[k].j;

        if(j != i)  //j > i �����ǵķǶԽ�Ԫ�ص���һ��������
        {
            m = jw[j];
            n = g_yu[m].p;
            g_yu[m].p = k;//���������ڼ���
            g_yu[k].p = n;
            jw[j] = k;
        }
    }

    //set elements of upper trangular matrix
	//ǰ�洦��ÿ��Ԫ�ص�λ�ã��������洦��Ԫ�ص�����
	//lv_wg = 1;
	for(i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		jmn = g_yu[ibs].d;//
		
		//�ö�·���������Ƶ�Դ��ĵ�Ч�迹
		//�õ����ϵ�ĵ�ֵ����͵翹,��Ҫ���׶˵�ѹ�ı���ֵ

		//�ϼ��������迹�ĵ�ֵ���ߵ�ѹ���ߵ���
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
		
			//ֻ���ǵ翹
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
		jmn = g_yu[ibs].d;//�޹��������ڽڵ��ϣ�ֱ���޸Ľڵ��bֵ
		g_yu[jmn].b += g_wsh[i].rnom;
	}

	//�Ա�ѹ�����д�����ÿ����ѹ��֧·����Ҫ�����迹�ӵ���Ӧ�Ľڵ���
//	for(i = 0; i < lv_wxf; i++)
//	{
//        ibs = g_wbr[g_wxf[i].ibr].ibs;
//        zbs = g_wbr[g_wxf[i].ibr].zbs;
//
//        jmn = g_yu[ibs].d;
//        jmx = g_yu[zbs].d;
//
//		//��ĩ�ڵ㶼Ҫ���������
//        g_yu[jmn].g += g_wxf[i].g1;
//        g_yu[jmn].b += g_wxf[i].b1;
//        g_yu[jmx].g += g_wxf[i].g2;
//        g_yu[jmx].b += g_wxf[i].b2;
//	}


//	//ֱ�ӶԸ��ɽ��д���
//	for(i = 0; i < lv_wld; i++)
//	{
//        ibs = g_wld[i].ibs;//���ɵ�ļ���ĸ�ߺ�
//        jmn = g_yu[ibs].d;
//
//		//ͨ�����ɵ���й����޹����Լ������Ӧ�ĵ���
//		e = g_wb[ibs].v * cos(g_wb[ibs].a);//ʵ��
//		f = g_wb[ibs].v * sin(g_wb[ibs].a);
//		c = g_wld[i].w;
//		d = g_wld[i].r;
//
//		//load impedance,�ڵ����и���
//		//�����ڸ��ɵ㴦��·���п���
//		//���迹 S / v^2
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

        if(ibs > zbs)//����������
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

        jmn = g_yu[ibs].d;
        jmx = g_yu[zbs].d;

		//�Ե���
        g_yu[jmn].g +=  r / (r * r + x * x);
        g_yu[jmn].b += -x / (r * r + x * x) + b;
        g_yu[jmx].g +=  r / (r * r + x * x);
        g_yu[jmx].b += -x / (r * r + x * x) + b;

        jmn++;
        jmx = g_yu[ibs+1].d;

		//������
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

//������ĶԲ�ڵ���д���
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

//����
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
		g_yu[i].i = 0;//�����ǵ���
		g_yu[i].j = 0;//�����ǵ���
		g_yu[i].p = 0;//Ԫ�ض�Ӧ����
		g_yu[i].d = 0;//�Խ�Ԫ��λ��
		g_yu[i].g = 0.0;//���ɵ�ʵ��
		g_yu[i].b = 0.0;//���ɵ��鲿
	}
	lv_yu = 0;

	//determine positions of diagonal elements������Ľ�����֧·��һ����
	//ȷ���Խ�Ԫ�ص�λ��
	for (i = 0; i < lv_wbr; i++)//ȷ���Խ�Ԫ�صĸ���
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;

		//ȡ��С��ĸ�߱�ų������˱�Ų��Ƿֲ���
		if (ibs > zbs) ibs = zbs;

		g_yu[ibs + 1].d++;//ȷ���ڵ����������ڵ���������൱�ڻ����ɵ�����
	}

	for (i = 1; i < lv_wb; i++)//ȷ���Խ�Ԫ���������е�λ��
	{
		//��һ���Խ�Ԫ�ص�λ�ã������ڲ��ҵ�ʱ������������Խ�Ԫ��֮��Ĳ�ֵ����ȡ
		j = g_yu[i].d + g_yu[i - 1].d + 1;
		g_yu[i].d = j;
		g_yu[j].i = i;//ÿ���ڵ��Ӧһ��
		g_yu[j].j = i;//�Խ�Ԫ�ص��к������
	}

	//determine number of elements in the matrix
	//�����Ǹ��Խ�Ԫ�ؾ�����������ĸ��������в���0Ԫ��
	lv_yu = g_yu[lv_wb - 1].d + 1;

	//determine row positions of elements
	//ȷ��ÿ��Ԫ���е�λ��
	for (i = 0; i < lv_wb; i++)
	{
		jmn = g_yu[i].d + 1;//��һ���Խ�Ԫ��
		jmx = g_yu[i + 1].d;

		//���е���һ��֮��������Ԫ�ط�0Ԫ��
		for (k = jmn; k < jmx; k++)//�����Խ�Ԫ��֮�������Ԫ�ض�������Խ�����ͬ�����к�
		{
			g_yu[k].i = i;
		}
	}

	//determine column positions of elements
	//ȷ��ÿ��Ԫ���е�λ�ã��ǶԽ�Ԫ��
	//ѭ��֧·
	for (i = 0; i < lv_wbr; i++)
	{
		ibs = g_wbr[i].ibs;
		zbs = g_wbr[i].zbs;

		//�����ڵ�ţ���С��������ֻ��Ҫ���������ǣ����Կ�����������
		if (zbs < ibs)
		{
			k = ibs;
			ibs = zbs;
			zbs = k;
		}

		jmn = g_yu[ibs].d + 1;
		jmx = g_yu[ibs + 1].d;

		//��������ȫ���ķǶԽ�Ԫ�أ�����Ϊ���Խ�Ԫ��֮��
		for (k = jmn; k < jmx; k++)
		{
			j = g_yu[k].j;//��ʼ��Ϊ0

			if (j == 0)//��һ�γ���
			{
				g_yu[k].j = zbs;//��ĩ�˽ڵ�ֱ�Ӹ�
				break;
			}
			else if (j > zbs)//�޸������Ľڵ���
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
	//�е�����ÿһ�дӵ�һ����ʼ��һֱ���Խ�Ԫ�ص�����˳��

	//��ʼ��
	for (k = 0; k < lv_wb; k++)
	{
		j = g_yu[k].d;//�Խ�Ԫ�ص�λ��
		jw[k] = j;      //working array����ʱ���飬��ڵ�ĶԽ�λ��
		g_yu[j].p = j;  //point to itself��ÿ�ж���ʱָ���Լ����ӶԽ�Ԫ�ؿ�ʼ���������ά��
	}

	i = 0;//���н��д���
	for (k = 0; k < lv_yu; k++)
	{
		if (k >= g_yu[i + 1].d) i++;  //next row

		j = g_yu[k].j;

		if (j != i)  //j > i �����ǵķǶԽ�Ԫ�ص���һ��������
		{
			m = jw[j];
			n = g_yu[m].p;
			g_yu[m].p = k;//���������ڼ���
			g_yu[k].p = n;
			jw[j] = k;
		}
	}

	//���վĸ�ߵ����ĵ���Ҫ�нӵ�
	for (i = 0; i < lv_wg; i++)
	{
		ibs = g_wg[i].ibs;
		jmn = g_yu[ibs].d;//

		if (g_wg[i].r0 > 0)//�͵���ӵ�
		{
			//ֻ���ǵ翹
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

		if (ibs > zbs)//����������
		{
			k = ibs;
			ibs = zbs;
			zbs = k;
		}

		jmn = g_yu[ibs].d;
		jmx = g_yu[zbs].d;

		//�Ե���
		g_yu[jmn].g += r / (r * r + x * x);
		g_yu[jmn].b += -x / (r * r + x * x) + b;
		g_yu[jmx].g += r / (r * r + x * x);
		g_yu[jmx].b += -x / (r * r + x * x) + b;

		//�Ե�֧·�ͱ�ѹ��֧·û�л�����
		if (g_wbr[i].type == _GRDSEC)
			continue;

		jmn++;
		jmx = g_yu[ibs + 1].d;

		//������
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

	//������ĶԲ�ڵ���д���
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
	double deltap,deltaq,deltac/*���������µ����*/;//���㹦�ʲ�ֵ
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
		deltac = 0;//ÿһ�ε������������µ���һ��

		//lv_iter++;

		//if ( (g_wg[0].w>0) && g_ld_distr)//�׶��й�������
		//{
		//	//Ϊ�˱�֤�׶˵Ĺ�����ȫ���䣬��Ҫ��̬����ĩ�˸��ɵĴ�С
		//	//ÿ�ε������׶˺󣬻��ۼ�һ���ܵĸ���ֵ���������ֵ�����еĴ���һ���Ĳ�ֵ��
		//	//�������ֵƽ�����䵽ÿһ����������
		//	for ( k = 0; k < lv_wld; k++)
		//	{
		//		for ( m = 1; m < lv_wb;m++)
		//		{
		//			 if ( g_wld[k].ibs == g_wb[m].toi )
		//			 {
		//				 g_wb[g_wb[m].toi].nw = g_wb[g_wb[m].toi].nw + deltap/lv_wld;

		//				 break;//������һ�����ɹ��ڶ��ĸ����
		//			 }
		//		}				   
		//	}
		//}

		//if ( (g_wg[0].r>0) && g_ld_distr)//�׶��޹�������
		//{
		//	//Ϊ�˱�֤�׶˵Ĺ�����ȫ���䣬��Ҫ��̬����ĩ�˸��ɵĴ�С
		//	//ÿ�ε������׶˺󣬻��ۼ�һ���ܵĸ���ֵ���������ֵ�����еĴ���һ���Ĳ�ֵ��
		//	//�������ֵƽ�����䵽ÿһ����������
		//	for ( k = 0; k < lv_wld; k++)
		//	{
		//		for ( m = 1; m < lv_wb;m++)
		//		{
		//			 if ( g_wld[k].ibs == g_wb[m].toi )
		//			 {
		//				 g_wb[g_wb[m].toi].nr = g_wb[g_wb[m].toi].nr + deltaq/lv_wld;

		//				 break;//������һ�����ɹ��ڶ��ĸ����
		//			 }
		//		}				   
		//	}
		//}
		
        //initialize power flow into buses from upstream linesĸ�ߵĸ���
		//����ʵ����ֻ�и��ɺ͵�������ע�룬�����Ķ�û��ע�룬0ע��
        for(i = 0; i < lv_wb; i++)
        {
            g_wb[i].dw = -g_wb[i].nw ;
            g_wb[i].dr = -g_wb[i].nr ;
        }

        //take into account of shunt injection power
		//�������ĳ����뵱ǰ�ĵ�ѹ�йأ��ڵ���ʱ�����Ƕ�̬������
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
			//deltac += r;//��������׶˹��ʵ�������Ҫ�����޹����ۿ�
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
		//��ĩ�˸��ɽڵ㿪ʼ��ǰ�ƽ�
		//�����Ѿ������˽ڵ��Ż����,�ڵ�Ŵ�0��ʼ�ӵ�Դ��һ�������
		//p1=p2+deltaP
        for(i = lv_wb - 1; i > 0; i--)
        {
            j = g_wb[i].jb;//�ڵ������֧·���

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
            w  = g_wb[zbs].dw;//���߶�ĩ�˵��ۼӹ���ֵ
            r  = g_wb[zbs].dr;

            //calculate power losses������ĩ�˵��׶˵�����
			if (vj == 0)
			{
				ret = -1; 
			}
            x = (w * w + r * r) / (vj * vj);
            w = x * g;
            r = x * b;//������֧·�����㵽�ϼ��ڵ���޹�ֵ

            //increase power flow into upstream bus
            g_wb[ibs].dw += g_wb[zbs].dw + w;//�׶˵Ĺ���ֵ
            g_wb[ibs].dr += g_wb[zbs].dr + r;
           
            //store line power for forward sweep
			//֧·�ϵ��׶˹����������ڻش���ʱ�����ѹ����Ҫ��������׶���������
            g_wb[zbs].dw += w;//��¼������߶β������ʱ��ĩ�˵��µ�����ֵ
            g_wb[zbs].dr += r;
            g_wb[zbs].wloss = w;//��֧·���ĵ��޹�ֵ
            g_wb[zbs].rloss = r;//��֧·���ĵ��޹�ֵ

			//if ( (g_wg[0].w>0) && g_ld_distr)//�׶�������
			//{
			//	//calculate delta p,q���ڵ�Դ�㴦�����ۼӵĸ���ֵ�͵�Դ��ʵ��ֵ֮��Ĳ�ֵ
			//	//ÿ����һ�������ֵ�����ĺ�С�������ȫһ��,���Ҳ��Ҫ�ӿ������ٶ�
			//	if ( i == 1)
			//	{
			//		 deltap = -(g_wg[0].w - g_wb[i].dw);
			//	}
			//}
			//
			//if ( (g_wg[0].r>0) && g_ld_distr)//�׶�������
			//{
			//	//calculate delta p,q���ڵ�Դ�㴦�����ۼӵĸ���ֵ�͵�Դ��ʵ��ֵ֮��Ĳ�ֵ
			//	//ÿ����һ�������ֵ�����ĺ�С�������ȫһ��,���Ҳ��Ҫ�ӿ������ٶ�
			//	if ( i == 1)
			//	{
			//		 deltaq = -(g_wg[0].r - g_wb[i].dr);
			//	}
			//}
		}

        //forward sweep
		//�����Ǵ�1��ʼ���лش�,U2=U1+deltaU(p1*)
        for(i = 1; i < lv_wb; i++)
        {
            j = g_wb[i].jb;//�ҵ��ϼ��ڵ�

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
            w  = g_wb[zbs].dw;//֧·���׶������Ĺ��ʣ�ֻ�������������ѹ��
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

            g_wb[zbs].dv = g_wb[zbs].v - vj;//�ڵ�ĵ�ѹ��ģ�����������������
            g_wb[zbs].da = g_wb[zbs].a - aj;
            g_wb[zbs].v = vj;
            g_wb[zbs].a = aj;
			if (isnormal(vj) == 0)
			{
				return 0;
			}
			//�жϵ�ѹ����,ѡ������֮���ѹ�����Ľڵ�
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
//�����Ǹ��Եģ���ֵ����ֻ�Ǹ�����������ѹ�Ƿ����
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
            jmx = g_yu[i+1].d;//�����Խ�֮���Ԫ�أ�Ҳ���ǷǶԽ�Ԫ�ص�

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

            while(p != k)//�����Լ�˵������ͷ��
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
        InitJacobianA();//ţ�ٷ��͸�˹�����ſɱȾ���Ĵ������ǲ�һ����
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
        FormJacobian();//����ſɱȾ������Ҫ�õ��޹��Ż�����

        //Gauss elimination method
        GaussElimination();

        //solve power flow equation
		if (SolveEquation() == 0)//������
			return 0;

		//�����������ʵľ���,���ξ��ȶ�һ�������ٽ��м���
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
//  InitJacobianA()   ���ſɱȾ�����Ԫ���������Ǻ��������е�λ��
//
//	Overview:	This subroutine initializes element values and positions of
//				Jacobian matrix directly from branch connections. As we
//				represent Jacobian matrix by (2, 2) blocks, so it has the
//				same structure as admittance matrix.

//	It sets up the diagonal position array (g_yu[i].d), the row
//				position array (g_yu[i].i), the column position array
//				(g_yu[i].j), and the column linking array (g_yu[i].p)
///////////////////////////////////////////////////////////////////////////////
//U��aij��ÿ�ε���������ģ�G��B��֧·�ĵ��ɣ�
//��ÿ���ڵ����4��ֵ����������Ϳ������һ��С�ľ����
//���ϴε������⼸��ֵ���γ��µ��ſɱȾ���
//�����ſɱȾ��󣬾Ϳ��������ⷽ���ˣ�����˴���ѧ����

//�ſɱȾ���ÿ���鶼��������4��Ԫ�أ����ü�������ʽ�������
//H�� Hij=Ui*Uj*(Gijsin(aij)-Bijcos(aij));   Hii=-Ui * �ۼ�(Uj*Gijsin(aij)-Bijcos(aij)) i<>j
//J�� Jij=-Ui*Uj*(Gijcos(aij)-Bijsin(aij));  Jii=Ui * �ۼ�(Uj*Gijcos(aij)+Bijsin(aij))
//N�� Nij=Ui*Uj*(Gijcos(aij)+Bijsin(aij));   Nii=-Ui * �ۼ�(Uj*Gijcos(aij)+Bijsin(aij))+2Ui*Ui*Gii
//L�� Lij=Ui*Uj*(Gijsin(aij)-Bijcos(aij));   Hii=-Ui * �ۼ�(Uj*Gijsin(aij)-Bijcos(aij))-2Ui*Ui*Bii
int PWRFLOW::InitJacobianA()
{
    int    i, j, k, m, jmn, jmx;
    int    ibs, zbs;

    //initialize data vaules of Jacobian matrix
	//�Խ�Ԫ�أ����1000���ڵ㣬Ҳ����1000 X 1000�ľ���
    for(i = 0; i < mx_jd; i++)
    {
        g_jd[i].H = 0.0;//ÿ��С2 X 2��������˺͵��ɾ���ͬ�׵ľ���
        g_jd[i].N = 0.0;
        g_jd[i].J = 0.0;
        g_jd[i].L = 0.0;
    }

	//������
    for(i = 0; i < mx_ju; i++)
    {
        g_ju[i].j = 0;	//�ھ����е��е�λ��
        g_ju[i].d = 0;	//
        g_ju[i].k = 0;	//
        g_ju[i].H = 0.0;
        g_ju[i].N = 0.0;
        g_ju[i].J = 0.0;
        g_ju[i].L = 0.0;
    }

	//������,�ſɱȾ����ǶԳ���
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
    lv_jd = lv_wb;//�Խ�Ԫ�صĸ�����Ҳ���Ǿ���Ľ���
    lv_ju = 0;
    lv_jl = 0;

    //count block number of upper and lower Jacobian matrix
	//ͳ�ƾ�����������һ�����Ӧһ�����ɾ����Ԫ��
	//ÿ����һ��֧·�������ھ���������Ǻ��������зֱ�����һ��Ԫ��
	//����Ԫ�صı�Ŵ�С��С�ı�ų����������ǣ���ı����������
	//�����������һ���Գ�����Ϊ����ֻ�ǿ�ĸ������͵�������ͳһ

//����ṹ
//3      0
// \(0) /(1)
//   2 /
//   |(2) ֧·���
//	 1
//������ʽ��
//   0   1   2   3
//0  D1      X1
//1      D2  X2
//2  X4  X5  D3  X3
//3          X6  D4 

//�����Ͽ�ju�����Ǵ�������Ԫ�أ������е��ֶ��в�ͬ�ĺ���
//...ju��ʵ��Ԫ�أ��Ǳ��0~���������Ԫ�ء��������ܱȽ���Ҫ����Ϊ����һ���ڵ��м������ӣ�
//...j ���кţ��Ǳ��0~���������Ԫ�ء���ju�ĽǱ�һһ��Ӧ����X2��Ӧ������2
//...d �汾�зǶԽ�Ԫ����ju�е�λ�ã��Ǳ��1~n����X2�ڽǱ�[1]���棬���й���2-1��
//...k ��֧·��Ӧ��������Ԫ����ju�еĽǱ꣬�Ǳ��1~(n-1)���磨2��֧·��Ӧ�Ǳ�1����ju[1]���ҵ�X2

//�Ǳ�(��) ju    j   d  k(֧·��Ŷ�Ӧ����������)    jl  j  d  k(֧·��Ŷ�Ӧ����������)      H N J K //�ſɱȾ�������ݣ��ͽǱ��ж�Ӧ 
// 0       X1    2   0  2                            X4  0  0  2
// 1       X2    2   1  0							 X5	 1	0  0
// 2       X3    3   2  1							 X6  2	0  1
// 3             0   3  0								 -1	2  0
// 4             0   3  0								 -1	3  0

//�Ǳ��Ӧ��d��ʾ���еĵ�һ����0Ԫ����ju�е�λ��


//�����������Կ���ÿһ���м�����0Ԫ�أ����׵�ַ��ǰ��ļ�һ�¾Ϳ����ˣ����3����6-3=3������Ԫ��

    //ͨ�����ѭ�����Եõ�ÿһ�з���Ԫ�ص��ܸ���g_ju[ibs+1].d���Լ�ȫ������Ԫ�صĸ���lv_ju
	for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;//�׽ڵ�,����Ѿ��ǽڵ��Ż�����ı���ˣ�����ԭʼ�ı��
        zbs = g_wbr[i].zbs;//ĩ�ڵ㣬����Ǵ�0��ʼ�ģ�ÿ����һ�α�ţ�һ�㲻�ᳬ��1000��

        if(ibs > zbs)//��ĩ�ڵ���Ҫ���򣬴���������ǲ��֣�С���������ǲ���
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		//�ڵ��Ӧ�����зǶԽ�Ԫ�صĸ���
		//��������������Ǿ����һ���з�0Ԫ�صĸ�����������ڵ�����������ӵĸ���
		//+1��������Ϊ�˺���ǰ���������õ����еķ���Ԫ�ظ���
        g_ju[ibs+1].d++;
        g_jl[zbs+1].d++;

        lv_ju++;//���¸�����һ��Ԫ��
        lv_jl++;
    }

    //determine first block position of each row
	//ÿһ�еĵ�һ�����λ�ã���Ҫָ���Խ�Ϊ��׼,�ǶԽ��еĵ�һ��λ�ã�֪���к�+1ȡd��֪����λ
	//ÿһ�е� ��һ�� ����Ԫ��������g_ju[]�е�λ�ã�Ҳ���ǽǱ�Ż�������
    for(i = 0; i < lv_wb; i++)//ÿ���ڵ��Ӧһ��
    {
		//�Ǳ��ٺ���һλ������g_ju[i+1].d������Ǹ��еķ���Ԫ����g_ju[]��λ��
		//g_ju[]�����ǰ�������Ԫ�أ�����ͷ���Ǹ�Ԫ��һֱ�������Ǹ�Ԫ�أ��м�û�пհ�
        g_ju[i+1].d += g_ju[i].d;
        g_jl[i+1].d += g_jl[i].d;
    }

    //determine block positions in Jacobian matrix
	//��λÿһ�����λ�ã�Ҳ����ÿһ����0Ԫ�ص�λ�ã���ǰ��Ľ��м�������
	//��ÿ����0Ԫ�ص���д��j����
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

		//ͨ��ǰ�������Ĳ�ֵ���Եõ������м�����0Ԫ��
        jmn = g_ju[ibs].d;//������g_ju[]�б��е�Ԫ�ص��±�
        jmx = g_ju[ibs+1].d;

		//�ҵ�g_ju[]�б��еļ���ʵ��Ԫ�ص��е�λ��
        for(k = jmn; k < jmx; k++)
        {
            j = g_ju[k].j;

            if(j == 0)//���е�һ����0Ԫ��
            {
                g_ju[k].j = zbs;//���е�һ���ǶԽ�Ԫ������Ӧ���������ӵ��к�
                break;
            }
            else if(j > zbs)//����б�ԭ�����л�Ҫ����ǰ�棬��Ҫ���°���һ�£�����
            {
                m = jmx - 1;
                while(m > k)
                {
                    g_ju[m].j = g_ju[m-1].j;
                    m--;
                }
                g_ju[k].j = zbs;//�����ڵ�Ԫ�ص�
                break;
            }
        }

		//��������
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
	//ȷ����֧λ�ã�ֻ��ȷ����ÿ��Ԫ�ص��в�����������ֵ
	//kֵ��׷�����õļ�¼���ں���ļ�����Ҫ�õ�
	//��һ��֧·��Ӧ���ϣ��£������е�λ��ѡ������һ��֧·ֻ��Ӧһ�������Ǻ�һ��������
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

		//���ڵ�������֧·֧·���д�������������������
		//֧·���ڽ��г���ע���ʱ������
        jmn = g_ju[ibs].d;
        jmx = g_ju[ibs+1].d;
        for(j = jmn; j < jmx; j++)
        {
             if(g_ju[j].j == zbs)//�Ա��е������ǽ���ѭ�����ҵ������֧·ƥ����е��������½Ǳ�g_ju[j].j��
             {
                 g_ju[i].k = j;//֧·��Ӧ��������Ԫ����ju�еĽǱ꣬��ʵ���������ţ�ֻ����һ�����ҵ�����˳�
                 break;
             }
        }

        jmn = g_jl[zbs].d;
        jmx = g_jl[zbs+1].d;
        for(j = jmn; j < jmx; j++)
        {
            if(g_jl[j].j == ibs)
            {
                g_jl[i].k = j;//֧·��Ӧ����������
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
    for(i = 0; i < mx_jd; i++)//�Խ�
    {
        g_jd[i].H = 0.0;
        g_jd[i].N = 0.0;
        g_jd[i].J = 0.0;
        g_jd[i].L = 0.0;
    }

    for(i = 0; i < mx_ju; i++)//�����ǷǶԽ�
    {
        g_ju[i].j = 0;
        g_ju[i].d = 0;
        g_ju[i].H = 0.0;
        g_ju[i].N = 0.0;
        g_ju[i].J = 0.0;
        g_ju[i].L = 0.0;
    }

    for(i = 0; i < mx_jl; i++)//�����ǷǶԽ�
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
    lv_ju = lv_yu - lv_wb;//���Խ�Ԫ�ص�������
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
		//���������еķ����ǶԳƵģ�����ͨ��������ֱ�ӵõ������ǵ�Ԫ��
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
//ÿ���ڵ����ڵ㵼�ɾ����1�У�����ڵ���д���󣬿��Եõ�ÿ�еĶԽǷǶԽ�Ԫ��
//�ڵ����ص���ÿ�ζ���Ҫ�����ۼӲ��ܵõ�ȫ����ֵ������ֵ���ͶԲ�ڵ��й�
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

	for(i = 0; i < lv_jd; i++)  //lv_jd == lv_wb �Խ�Ԫ�صĸ����ͽڵ���һ��
    {
        g_jd[i].H = 0.0;
        g_jd[i].N = 0.0;
        g_jd[i].J = 0.0;
        g_jd[i].L = 0.0;
    }

    for(i = 0; i < lv_ju; i++)  //lv_ju == lv_jl == lv_yu - lv_wb�����Ǻ������Ǹ���һ��
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
	//��ÿһ��֧·���д���һ��һ�£������ص��Ĳ����ۼӵõ�����
	//�������ܵĵ���ֵ
	//�ڵ㴦��ʱ�������ǿ����ҵ���Ӧ��j����0λ���У�
	//���Կ������ڵ�������ڵ���Ŷ���ӵ������ڵ���ȥ
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

        g /= vi;//�õ����ɵ�ʵ��
        b /= vi;//�����鲿-

        vi = g_wb[ibs].v;//�׽ڵ�ĵ�ѹ��ֵ����ѹ�Ǵ��������൱��x������J*x=b
        ai = g_wb[ibs].a;//
        vj = g_wb[zbs].v;//
        aj = g_wb[zbs].a;//ĩ�ڵ�ĵ�ѹ��ǣ�ÿ�ε�������һ��

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

		//����ÿ���ڵ�Ĺ��ʲ�ƽ������ע�룬�൱�ڷ���ʽ���ұ���b
		//ͨ���еĵ��ɺ� ��ѹ����ѹ����ǲ����õ�
        g_wb[ibs].dw -= vvi_g - vij_g * csa + vij_b * sna;
        g_wb[ibs].dr -= vvi_b - vij_b * csa - vij_g * sna;
        g_wb[zbs].dw -= vvj_g - vij_g * csa - vij_b * sna;
        g_wb[zbs].dr -= vvj_b - vij_b * csa + vij_g * sna;

		//����ÿ���ڵ���ſɱȾ���Ԫ���еĶԽ����������ڵ㻥���Ŷ�
		//һ��Ԫ�ض�Ӧ4��СԪ�أ�2X2����ɵĿ�
        g_jd[ibs].H += -vij_g * sna - vij_b * csa;
        g_jd[ibs].N +=  vij_g * csa - vij_b * sna - 2 * vvi_g;
        g_jd[ibs].J +=  vij_g * csa - vij_b * sna;
        g_jd[ibs].L +=  vij_g * sna + vij_b * csa - 2 * vvi_b;
        g_jd[zbs].H +=  vij_g * sna - vij_b * csa;
        g_jd[zbs].N +=  vij_g * csa + vij_b * sna - 2 * vvj_g;
        g_jd[zbs].J +=  vij_g * csa + vij_b * sna;
        g_jd[zbs].L += -vij_g * sna + vij_b * csa - 2 * vvj_b;

		//���ڵ�������ڵ��ڽ����������е�λ�ã��У�
		//�˴���i��֧·��ţ������Ӧ�˱�֧·�Ķ������ǵ��Ŷ�λ��
        k = g_ju[i].k; //�ڵ������ӵ�֧·��ţ�Ҳ������֧·�е�λ��
		//������Ŷ�������󣬷ŵ���Ӧ��������Ԫ�ض�Ӧ�ĽǱ���£��´ο�������ȡ��
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

    //first check pq type generator��1-pq��-1 pv��0-ƽ��
    for(i = 0; i < lv_wg; i++)
    {
        if(g_wg[i].slackavr == 1 || g_wg[i].kswitch == 1)
        {
            ibs = g_wg[i].ibs;//һ���ڵ��м��������������
            g_wb[ibs].dw += g_wg[i].w;
            g_wb[ibs].dr += g_wg[i].r;
        }
    }

    //then check slack and pv type generator
    for(i = 0; i < lv_wg; i++)
    {
        ibs = g_wg[i].ibs;//�õ��������ĸ�߱��

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
        else if(g_wg[i].slackavr == -1 && g_wg[i].kswitch == 0) //pv ��Խ��
        {
            g_wb[ibs].nr -= g_wg[i].r;
            g_wg[i].r = -g_wb[ibs].dr; 
            if(g_wg[i].r > g_wg[i].rmx)//�޹��������ڷ��������޹�
            {
                g_wg[i].r = g_wg[i].rmx;
                g_wg[i].kswitch = 1;
                g_wb[ibs].ja = 0;
            }
            else if(g_wg[i].r < g_wg[i].rmn)//�޹�����С�ڷ������С�޹�
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

	//������ƽ��ڵ㣬lld add 2018-7-23
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
	//������ƽ��ڵ㣬lld add 2018-7-23

	//PV�ڵ�
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

	//����������ſɱȾ����jd[],ju[],jl[]ȫ���γ���
	//ÿ���ڵ��ע���й���ƽ����g_wb[ibs].dw���޹�ע�벻ƽ����g_wb[ibs].drҲ�γ���
	//ʣ�µľ��ǽⷽ����
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
	//�����Ǿ�����������Ľ��
    for(i = 0; i < mx_jb; i++)
    {
        g_jb[i].j = 0;//�к�
        g_jb[i].d = 0;//��0Ԫ���׵�ַ
        g_jb[i].H = 0.0;
        g_jb[i].N = 0.0;
        g_jb[i].J = 0.0;
        g_jb[i].L = 0.0;
    }
    lv_jb = 0;

    //Gauss elimination
	//ÿ�δ���һ�У�ȡһ������ĸ�߳������Ǵ���һ��
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
		//���н�����������
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
                    g_wb[i].dw -= a * g_wb[j].dw;//b��ʵ��
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
                    g_wb[i].dr -= a * g_wb[j].dw;//b���鲿
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

        g_wb[i].a -= g_wb[i].da;//ÿ���ڵ�ĵ�ѹ�ĽǶ�
        g_wb[i].v -= g_wb[i].dv;//ÿ���ڵ�ĵ�ѹ��ֵ

		//if (isnormal(g_wb[i].dv) == 0)
		//{
		//	return 0;
		//}

		if (g_eps < fabs(g_wb[i].dv))
        {
            g_eps = fabs(g_wb[i].dv);//��ǰ�����ε���������ֵѡ�������ŵ�һ��ȫ��������

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
	//����Դ�ڵ�Ĺ���
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

	////����Դ�ڵ�Ĺ���
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

	if (type != 2)//����������
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
				(bs + ibs)->adeg = (float)(a * 57.29578);//������ֵ�������ֵ
				(bs + ibs)->w = (float)(g_wb[i].nw * g_wbase);
				(bs + ibs)->r = (float)(g_wb[i].nr * g_wbase);

				if (v < minVoltage)
					minVoltage = v;//����͵�ѹ��¼����
				if (v > maxVoltage)
					maxVoltage = v;//����ߵ�ѹ��¼����

				//��ÿ���ڵ㶼���ϵ�ѹֵ
				knd0 = (bs + ibs)->pnd;//��������ͨ��ĸ�ߵõ��ڵ���
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

		//û��Ҫ������������ã���ʼ����ʱ���Ѿ�����
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
				if (type == 2)//����������
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
				if (type != 2)//����������
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
				(cp + i)->r = 0.0;//��������û�н���ĵ�������0ֵ
			}
		}
	}
	
	//��������
	if (type & 1)
	{
		//int iwg = 0;
		//for (i = 0; i < lv_wg; i++)
		//{
		//	ibs = iwg = g_wg[i].logic;
		//	//�õ�ĸ�����������翪�����

		//	if ((bs + ibs)->iisland == g_island)
		//	{
		//		if (type != 2)//����������
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
				if (type != 2)//����������
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
        
        vi = g_wb[ibs].v;//��ѹ��ֵ
        ai = g_wb[ibs].a;//��ѹ��λ�ǣ���ʱ���Ǳ���ֵ
        vj = g_wb[zbs].v;
        aj = g_wb[zbs].a;

 		vi_out = vi;

		ei = vi * cos(ai);//��ѹ��ʵ��
        fi = vi * sin(ai);//��ѹ���鲿
        ej = vj * cos(aj);
        fj = vj * sin(aj);

//		if ((r==0) && (x==0))//0�迹֧·����Ҫ�����⴦��
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
			cj = ci;//�������ᷢ���仯

			//vi = (double)(nd+(bs+g_wb[ibs].tio)->pnd)->vl;
            
			isec = g_wbr[i].logic;
//			lld->delstrright((sec+isec)->id,ZN_ID_LEN);
            (sec+isec)->w  = (float)(wi * g_wbase);
            (sec+isec)->r  = (float)(ri * g_wbase);
			//������Ҫ����ֵ���㣬��ѹ��10kV����ֵ
            (sec+isec)->i  = (float)(ci * (g_wbase / vi / 1.73205) * 1000);
            (sec+isec)->wz = (float)(wj * g_wbase);
            (sec+isec)->rz = (float)(rj * g_wbase);
            (sec+isec)->iz = (float)(cj * (g_wbase / vi / 1.73205) * 1000);

			(sec+isec)->v_b = vi_out;
			(sec+isec)->adeg_b = ai * 57.29578;//ת��Ϊ����ֵ
			
			(sec+isec)->v_e = vj;
			(sec+isec)->adeg_e = aj* 57.29578;

			//�ڵ�������0�迹֧·�����
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
			cj = ci;//�������ᷢ���仯

			//vi = (double)(nd+(bs+g_wb[ibs].tio)->pnd)->vl;

			icp = g_wbr[i].logic;
			//			lld->delstrright((cp+icp)->id,ZN_ID_LEN);
			(cp + icp)->w = (float)(wi * g_wbase);
			(cp + icp)->r = (float)(ri * g_wbase);
			//������Ҫ����ֵ���㣬��ѹ��10kV����ֵ
			(cp + icp)->i = (float)(ci * (g_wbase / vi / 1.73205) * 1000);
			(cp + icp)->wz = (float)(wj * g_wbase);
			(cp + icp)->rz = (float)(rj * g_wbase);

			(cp + icp)->v_b = vi_out;
			(cp + icp)->adeg_b = ai * 57.29578;//ת��Ϊ����ֵ

			(cp + icp)->v_e = vj;
			(cp + icp)->adeg_e = aj * 57.29578;

			//�ڵ�������0�迹֧·�����
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
		//���������ɸ�ֵ
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
			if (g_wsh[i].r >= 0.0) //����
				cap += g_wsh[i].r;
			else //�翹
				reac += -g_wsh[i].r;
		}
    }

	if (type & 4)//�������粻��Ҫ
		return 1;

	//island
	ISLAND* island = dsbob2->island_tb->GetTable();
	
	(island+g_island)->refbs = g_wb[g_slackbs].tio;//ֻ��һ�������ɳڽڵ�

	FD* fd1 = new FD();
	strcpy(fd1->id, dsbob2->so_tb->GetDataByLog(g_wg[g_slackgen].logic)->fdid);
	(island + g_island)->reffd = dsbob2->fd_tb->FindPh(*fd1);//so������Ÿ������
	delete fd1;
	(island + g_island)->refminv = minVoltage;
	(island + g_island)->refmaxv = maxVoltage;
	//(island+g_island)->refso = g_wg[g_slackgen].logic;
	//(island+g_island)->vref = float(g_wb[g_slackbs].v);


	(island+g_island)->wload = float(wload * g_wbase);
	(island+g_island)->rload = float(rload * g_wbase);
	(island+g_island)->wso = float(wso * g_wbase);
	(island+g_island)->rso = float(rso * g_wbase);
//�����ڵ�����ʱ���Ѿ�д����
	//���뵽���ߵ�ԣ������
	
	//co
	int lv_co = dsbob2->co_tb->GetCount();
	CO* co = dsbob2->co_tb->GetTable();

	if ( ((g_wloss * g_wbase) < 0) || ( (g_wloss * g_wbase) > 5) )//��������������
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

	double dW,dR,abunW,abunR;//�Բ�ĵ�Դ���й����޹�
	SO* so = dsbob2->so_tb->GetTable();
	FD* fd = dsbob2->fd_tb->GetTable();
	int lv_fd = dsbob2->fd_tb->GetCount();

	UN* un = dsbob2->un_tb->GetTable();
	int lv_un = dsbob2->un_tb->GetCount();
	for(i = 0; i < lv_un; i++)
	{
		strcpy(left,(un+i)->left);
		//��left���ԣ��
		ph = dsbob2->OnGetPhByfdid(left);//dsbob2->fd_tb->FindPh(FD(left));//�����߱��е�λ��
		so_p = dsbob2->r_fd_so_tb->FindChildPtr1((fd+ph)->r_so);
		so_p0 = so_p;//����ָ�����ʱ���棬ָ�������͵�����
		while(so_p)
		{
			dW = ((so+so_p->Ptr)->w - (so_p->Ptr+so)->pLoss)/100;//��Դ�㹦��
			dR = ((so+so_p->Ptr)->r - (so_p->Ptr+so)->qLoss)/100;//��Դ�㹦��
			
			abunR = (so+so_p->Ptr)->maxcurrent;
			
			if (abunR <= 0)
			{
				(un+i)->l_abund_P = 0;//�����ڹ��ʵ��۳��й�ԣ��
				(un+i)->l_abund_Q =	0;//�޹�ԣ��	
				//����͵�ѹ
				(un+i)->l_abund_V =	(so+so_p->Ptr)->sr;//�޹�ԣ��
			}
			else
			{
				//���������ۺϳ��й����޹�
				abunW = 100*(((1.732*abunR/1000)/10)  - sqrt(dW*dW+dR*dR));//���ڹ���
				
				//����Բ�����翪��ԣ��Ϊ0����һ������ֵ�������5
				(un+i)->l_abund_P = abunW * 0.98;//�����ڹ��ʵ��۳��й�ԣ��
				(un+i)->l_abund_Q =	abunW * 0.2;//�޹�ԣ��	
				//����͵�ѹ
				(un+i)->l_abund_V =	(so+so_p->Ptr)->sr;//�޹�ԣ��
			}

			so_p = so_p->Next;
		}
		::FreeChildPtr(so_p0);

		strcpy(right,(un+i)->right);
		//��left���ԣ��
		ph = dsbob2->OnGetPhByfdid(right);//dsbob2->fd_tb->FindPh(FD(right));//�����߱��е�λ��
		so_p = dsbob2->r_fd_so_tb->FindChildPtr1((fd+ph)->r_so);
		so_p0 = so_p;//����ָ�����ʱ���棬ָ�������͵�����
		while(so_p)
		{
			dW = ((so+so_p->Ptr)->w - (so_p->Ptr+so)->pLoss)/100;//��Դ�㹦��
			dR = ((so+so_p->Ptr)->r - (so_p->Ptr+so)->qLoss)/100;//��Դ�㹦��
			
			abunR = (so+so_p->Ptr)->maxcurrent;
			if (abunR <= 0)
			{
				(un+i)->r_abund_P = 0;//�����ڹ��ʵ��۳��й�ԣ��
				(un+i)->r_abund_Q =	0;//�޹�ԣ��	
				//����͵�ѹ
				(un+i)->r_abund_V =	(so+so_p->Ptr)->sr;//�޹�ԣ��	
			}
			else
			{
				//���������ۺϳ��й����޹�
				abunW = 100*(((1.732*abunR/1000)/10)  - sqrt(dW*dW+dR*dR));//���ڹ���
				
				//����Բ�����翪��ԣ��Ϊ0����һ������ֵ�������5
				(un+i)->r_abund_P = abunW * 0.98;//�����ڹ��ʵ��۳��й�ԣ��
				(un+i)->r_abund_Q =	abunW * 0.2;//�޹�ԣ��	
				//����͵�ѹ
				(un+i)->r_abund_V =	(so+so_p->Ptr)->sr;//�޹�ԣ��	
			}

			so_p = so_p->Next;
		}
		::FreeChildPtr(so_p0);

	}

	return PWRFLOW_SUCCEED;
}