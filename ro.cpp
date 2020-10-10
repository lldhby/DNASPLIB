// RO.cpp: implementation of the CRO class.
//
//////////////////////////////////////////////////////////////////////
#include "../DNASPLIB/ro.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double CRO::getCosfi(double w,double r)//��������
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

	alpha = 0;/*�۾�ά����*/
	garma = 0;/*Ͷ�ʻ�����*/
	belta = 0;/*���*/
	Kc = 0;/*��λ���������豸Ͷ��*/
	tiao = 0;/*ȫ����󸺺����Сʱ��*/
}

CRO::~CRO()
{
//	delete[] g_jd_t;
//	delete[] g_ju_t;
//	delete[] g_jl_t;

	delete[] g_rdi;
}

//���޹��Ż���һ�Ρ����ξؽ�������
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
			g_rdi[i].b_v = -1;//����ǰ��ѹ�����ʱ������������ж�
		}
		g_rdi[i].a_cosfi = 0;
		g_rdi[i].a_v = 0;
		g_rdi[i].Qv = 0;//��������

		//����Ԫ�س�ʼ��
		for(j = 0; j < MXELE; j++)
		{
			g_rdi[i].downstream[j] = -1;
		}
		g_rdi[i].n_downstream = 0;
	}
	lv_rdi = 0;
//		//�Խ�
//		for(i = 0; i < mx_jd; i++)
//		{
//			g_jd_t[i].H = 0.0;
//			g_jd_t[i].N = 0.0;
//			g_jd_t[i].J = 0.0;
//			g_jd_t[i].L = 0.0;
//		}
//	
//		//������
//		for(i = 0; i < mx_ju; i++)
//		{
//			g_ju_t[i].j = 0;	//�ھ����е��е�λ��
//			g_ju_t[i].d = 0;	//
//			g_ju_t[i].k = 0;	//
//			g_ju_t[i].H = 0.0;
//			g_ju_t[i].N = 0.0;
//			g_ju_t[i].J = 0.0;
//			g_ju_t[i].L = 0.0;
//		}
//
//		//������,�ſɱȾ����ǶԳ���
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

//��ȡ����㣬�������һ������
int CRO::getPivot(int* pivot)
{
	int i,j,k,t,t1;
	int n_path,n_step;
	int path[MAX_MINPATH_LEN][MAX_LOW_VOLTAGE_LEN];//����100���ڵ�͵�ѹ����С·MAX_MINPATH_LEN���ڵ�
	int path_t[MAX_MINPATH_LEN],vol_t[MAX_MINPATH_LEN];

    for(i = 0; i < MAX_MINPATH_LEN; i++)
    {
		pivot[i] = -1;
	    for(j = 0; j < MAX_LOW_VOLTAGE_LEN; j++)
			path[i][j] = -1;			
	}

	//��ԭ��д�����������
    for(i = 0; i < lv_wb; i++)
    {
		//�ҵ����ڵ���ϼ��������¼��ڵ㣬�����ϼ��ڵ�����֪�ģ��¼��ڵ���Ҫѭ��
   		g_rdi[i].pivot = -1;
    }

	//Ѱ�Ҳ��ϸ��ѹ
	n_path=0;
    for(i = 1; i < lv_wb; i++)
    {
		//ֻ��Ҫ���Ǹ��ɵ�ĵ͵�ѹ
		if (g_rdi[i].n_downstream > 0)
			continue;

		//�ҵ����ڵ���ϼ��������¼��ڵ㣬�����ϼ��ڵ�����֪�ģ��¼��ڵ���Ҫѭ��
		if (g_rdi[i].v < 0.93)
		{
			for(j = 0; j < MAX_MINPATH_LEN; j++)
			{
				path_t[j] = -1; 
				vol_t[j] = -1;
			}

			n_step = 0;
			//���ҵ���Դ�����С·
			k = g_rdi[i].ibs;//���ڵ�
			while(1)
			{
				if (k==0) 
					break;

				path_t[n_step++] = k;//���ڵ��д�뵽����

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
	
			n_path++;//����һ��
		}
    }

	if ( n_path==0 )//��ѹȫ�ϸ�
		return -1;

	if ( n_path==1 )//ֻ��һ�����ϸ�
	{
		for(i = 0; i < n_step; i++)
		{
			pivot[i] = path[i][0];//ǰ�����еĶ����������
		}
		
		return 1;
	}

	//ͨ����С·�ҽ���
	//�ӵ�Դ�㿪ʼ������һ��������һ���㲻������һ������С·�ϣ�˵���Ѿ����˷ֲ��

	//ת�þ���
    for(i = 0; i < MAX_MINPATH_LEN; i++)//��С·�����һ��Ԫ�أ��Ӹ���ʼ
    {
		t = path[i][0];
	    for(j = 0; j < MAX_LOW_VOLTAGE_LEN; j++)//ѭ�����е���С·
		{
			t1 = path[i][j];
			if (t1<0)//����С·һ��ʼ���ǿ�
				break;

			if (t != t1)//���ֲ�һ�µ������˵���Ƕ��˵ĵ�
			{
				for(k = 0; k < i; k++)
				{
					pivot[k] = path[k][0];//ǰһ������
				}
				return i;
			}
		}
    }

	return -1;//û�������
}

//ÿ�������������ʱ����Ҫ�γ��µ��ſɱȾ������Բ���Ҫ����ԭ�е�����
// H N
// J L
//1.�Խ����ϵ�N,J����
//2.�ſɱȾ���ĵ�����j��k���������е�N��J����
//����΢����=P/Q / (1-Q/Q)
//ÿһ�е�H ��N�����ۼӣ��õ�P/Q��J ��L�ۼӵõ�Q/Q 
int CRO::getTmattrice()//�õ��ſɱȾ����ת�þ���
{
	int i,ibs,zbs,k,k1,k2;
	double H,N,J,L,dw,dr;

	//�ſɱȾ���jd[],ju[],jl[]��ju[]��jl[]��Ҫ��������
	for (i=0; i<mx_jd; i++)
	{
//		g_jd_t[i].H = g_jd[i].H;
//		g_jd_t[i].N = g_jd[i].N;
//		g_jd_t[i].J = g_jd[i].J;
//		g_jd_t[i].L = g_jd[i].L;

		//N,J����
		N = g_jd[i].N;
		g_jd[i].N = g_jd[i].J;
		g_jd[i].J = N;
		
	}

	for (i=0; i<mx_wb; i++)
	{
		//b�����㣬���ڽ���󷽳�
		g_wb[i].dw = 0;
		g_wb[i].dr = 0;
	}

//	for (i=0; i<mx_ju; i++)
//	{
//		g_ju_t[i].j = g_ju[i].j;	//�ھ����е��е�λ��
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
//		g_jl_t[i].j = g_jl[i].j;	//�ھ����е��е�λ��
//		g_jl_t[i].d = g_jl[i].d;	//
//		g_jl_t[i].k = g_jl[i].k;	//
//		g_jl_t[i].H = g_jl[i].H;
//		g_jl_t[i].N = g_jl[i].N;
//		g_jl_t[i].J = g_jl[i].J;
//		g_jl_t[i].L = g_jl[i].L;
//	}

	//ֻ��Ҫ��H N J L��һ�¾Ϳ�����
	//������Ҫֱ�Ӽ���֧·�Ŷ����е�H��N���ۼ�ֵ

	dw = dr = 0;
    for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;//�ھ����е��к�
        zbs = g_wbr[i].zbs;

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		//���ڵ�������ڵ��ڽ����������е�λ�ã��У�
		//�˴���i��֧·��ţ������Ӧ�˱�֧·�Ķ������ǵ��Ŷ�λ��
        k1 = g_ju[i].k; //�ڵ������ӵ�֧·��ţ�Ҳ�������������еĽǱ�
        H = g_ju[k1].H;
        N = g_ju[k1].N;
        J = g_ju[k1].J;
        L = g_ju[k1].L;
		//װ��֮ǰ��H,N�����ۼӵ�b��
		g_wb[ibs].dw += H;
		g_wb[ibs].dr += N;

        k2 = g_jl[i].k;//���������е��к�
		g_wb[zbs].dw += g_jl[k2].H;
		g_wb[zbs].dr += g_jl[k2].N;
        
		g_ju[k1].H = g_jl[k2].H;
        g_ju[k1].J = g_jl[k2].N;//N,J����
        g_ju[k1].N = g_jl[k2].J;
        g_ju[k1].L = g_jl[k2].L;

        g_jl[k2].H = H;
        g_jl[k2].J = N;
        g_jl[k2].N = J;
        g_jl[k2].L = L;
	}

	return 1;
}

//DEL int CRO::reTmattrice()//�õ��ſɱȾ����ת�þ���
//DEL {
//DEL //	int i;
//DEL //
//DEL ////�˳�ǰҪ�ָ�ԭ��
//DEL //	for (i=0; i<mx_ju; i++)
//DEL //	{
//DEL //		g_ju[i].j = g_ju_t[i].j;	//�ھ����е��е�λ��
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
//DEL //		g_jl[i].j = g_jl_t[i].j;	//�ھ����е��е�λ��
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


int CRO::getArrayQ()//�õ��޹�����΢���ʣ�������
{
	int i;

    GaussElimination();
    SolveEquation();
	
	//����Ҫ���е�����ֱ�ӳ��������һ��ȡһ����������
    for(i = 0; i < lv_wb; i++)
    {
        g_Q_inc[i]= g_wb[i].dv;//ÿ���ڵ�ĵ�ѹ��ֵ
    }

	//����
	lld->sortDouble_Desc(g_Q_inc,lv_wb);

	return 1;
}

double CRO::getEconomy()//���㾭�õ���
{
	//���۾�ά����+Ͷ�ʻ����ʣ�* ��λ���������豸Ͷ�ʣ�Ԫ/Kvar��/ 
	// ��λ������ļ۸�Ԫ/kW.h��* ȫ����󸺺����Сʱ��
	
	//��λ���������豸Ͷ��:100Ԫ/kVar;��ѣ�0.5Ԫ/kW.h��ȫ����󸺺����Сʱ����2000
	

	return 1;
}

//���޹��Ż�ֻ��Ӧ��10kV����״�����
//�޹��Ż�ֻ�����ڵ�ѹ�ϸ������½����޹��Ż�����������᱾��ĩ
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
	//execute calculations ��ÿһ�������м���
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
//��Ҫ�ڴ��ж��Ƿ���������зֲ�ʽ��Դ

		//ֻ�Ա����ڵ����ݽ��г�ʼ��
		//initialize working arrays		1.��ĸ�߽��г�ʼ��
		InitWorkingArray();
		InitWorkingArray_1();//ÿ��һ��������Ҫ���������޹���

		//read network data		2.������������
		if(GetNetworkData() == PWRFLOW_FAIL)
		{
			continue;
			//return PWRFLOW_FAIL;
		}

		//////////////////////////////////////////////////////////////////////////
		//g_loopnum=1;//=======
		if (g_loopnum > 0)
		{
			//�����˻�������Ҫ�����ſɱȾ�������
			//1.����ţ�ٷ����㳱��
			if(OrderBusNumberB() == PWRFLOW_FAIL)
			{
				soid=dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->id);
				sprintf(out_error,"OrderBusNumberB error in soid %s\n",so_id);
				lld->WriteLog(lld->logfile,out_error);
				lld->add_globalexception(out_error);
				continue;
			}

			g_method = 1;//��ţ�ٷ����г�ʼ��//=========
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
			//������ȷ�����޹���Ҳȷ����
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
			//execute backward and forward sweep	4.ִ��ǰ�ƻش�
			if(ExecuteSweep() == -1)
			{
				soid=dsbob2->island_tb->GetDataByLog(g_island)->reffd;
				strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->fdid);
				
				sprintf(out_error,"OrderBusNumberA error in fdid %s\n",so_id);
				lld->WriteLog(lld->logfile,out_error);

				//�����߶��������޹��Ż�
				dsbob2->ro_result[lld->g_cnt_ro_fd].id=lld->g_cnt_ro_fd;
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].ndid,"����������");
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
		//order bus number by BFS	3.ͨ�������������ĸ�߱���Ż�
//lld->TimeHere("end sweep.");
		//write result into database���������һ�����㵺

//�����޹��Ż�ģ�飬������Ҫ�������㳱��
		//�����޹����ξ�ȷ�أ���ÿһ��ĸ�߽��м���
		//���ξ�ȷ�ؾ��������ص㣬һ�ξ�ȷ�ؾ�����������

		//1.����������·�Ƿ���Ҫ�����޹������������Ҫ�Ļ����Ž�����һ���ķ���������ֱ������
			//�����޹��Ż����򣬶Ա����߽����޹��Ż�����Ҫ�������㳱��
			//�Ե�ǰ�ĵ�Դ�㣨�����ĵ������������Ż��������ó��������ۡ�
	//0-������ѹ�������ѹ��̫�ͣ���ǰ��Ľڵ��޷�֧�ţ�ֱ���˳�
		if ( (ret = isVoltageViolate(1)) < 0 )//��ȫ���貹��
		{
//			g_wloss_b = g_wloss; 
//			g_rloss_b = g_rloss;

			//����ǰ�ĵ�ѹ��¼�������жԱ�
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

//�жϲ����������μ���õ��Ľ�����������Խ��
//0 ������û��Խ�ޣ�-1 �������ѹԽ���ޣ�-2 ��ѹԽ���ޣ�-3 �ߵ��޶�Խ
int CRO::isVoltageViolate(int oragin)
{
	int i,ibs;
	int ret=0;
	int l_flag,h_flag;

	l_flag = h_flag = 0;
	//���㲹����ĵ�ѹ��ֵ������ǰ��Ա�,����ǰ��״̬
	//���в��ϸ�ڵ�ȫ����ʾ����+7%�� -10%
	for(i = 0; i < lv_wb; i++)
	{
		ibs = g_wb[i].tio;
		if ( ibs >= 0 ) //����������  
		{
			//��Ҫ�жϵ�ѹ�Ƿ�Խ��
			//g_wb[i].b_v = g_wb[i].v;//����ǰ�ĵ�ѹ
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

	//һ��������ж�
	
	//ȫ������Խ���ޣ�û��Խ���޵��������
	if ((l_flag<0) && (h_flag==0))
		ret = -1;

	//ֻԽ����,û��Խ���޵����
	if ((l_flag==0) && (h_flag<0))
		ret = -2;
	
	//�ߵ��߶������㣬��Խ���ޣ���Խ����
	if ((l_flag<0) && (h_flag<0))
		ret = -3;

	return ret;
}

//�Ƿ���Ҫ�����޹�����
//0-���貹����-1 ��ѹ̫�ͣ�������Ч��-2 ������λ�� -3 ��������λ
int CRO::isNeedRPO()
{	
	//1.��һ�׶Σ���ĩ�˵Ĺ�������Ҫ������λ��0.95
	if (firstRPO() == 1)
		return 1;

	if (secondRPO() == 1)
		return 1;
	else
		putRPO(3);//��ʹ�޽⣬ҲҪ������׶ε��Ż����

//	g_wloss_a = g_wloss;
//	g_rloss_a = g_rloss; 

	return 1;
}

//��һ�׶β���
//�Ƚ��е�һ�׶ε��Ż���������Ĺ������ص���0.9����Ҫ���䲹��0.95��ˮƽ
//ֻ�е�ѹ�ಹ����λ��ĩ���޹�ƽ���˲��ܽ��и��ϵĲ���
//0 ��һ�׶�û�гɹ�����Ҫ�ڶ��ڵ㣬1 �ɹ� 
int CRO::firstRPO()
{
	int i,ret,cos_flag,v_flag;//�Ƿ������Ҫ��ѹ�������͵͵�ѹ���
	double w,r,cosfi,Qv,v_low;

	ret=cos_flag=v_flag=0;
	int ibs;
	LD* ld = dsbob2->ld_tb->GetTable();
    for(i = 0; i < lv_wld; i++)
    {
        w = g_wld[i].w;
        r = g_wld[i].r;

		ibs = g_wld[i].ibs;//���ɵ���Ż�����
		
		v_low = g_wb[ibs].v;
		cosfi = w / sqrt(w*w + r*r);
		if ( (cosfi < 0.9) )
		{
			//���е�һ�׶εĲ���
			//ֱ���ڱ�ѹ���෢�޹������ܲ���̫�ֻ࣬�ܵ�0.95
			Qv = w * (sqrt(1/(cosfi * cosfi) -1) - sqrt(1/(0.95 * 0.95) -1));
			g_wb[ibs].nr += Qv;//��������������һ�׶εļ���	

			//******��Ҫ�����������Ͳ���������¼����
			g_rdi[ibs].Qv = Qv;//��Ҫ����������						
			g_rdi[ibs].b_cosfi =cosfi;//����ǰ�Ĺ�������						
			//g_rdi[ibs].b_v = v_low;//����ǰ�ĵ�ѹ						

			cos_flag = 1;//�����˵�һ�׶β�������
		}
    }

	//�Ѿ������˵�һ�׶εĲ���
	if (cos_flag > 0) 
	{
		g_ld_distr=0;//�޹��Ż�����ʱ���ܽ��и��ɷ���
		//1.�ȼ���һ�鳱��
		if (ExecuteSweep() == -1)//����������
		{
			return -4;
		}
		g_ld_distr=1;//���ɷ��俪�ش�
	
		//���������㳱����������û�е͵�ѹ����
		ret = isVoltageViolate();
		if ( ret<0 )//���ڵ�ѹԽ�޵�����
		{
			ret = 0;
		}
		else//��ѹ��Խ�ޣ�����ֱ�������һ�׶εĲ������
 		{
			for(i = 0; i < lv_wld; i++)
			{
				ibs = g_wld[i].ibs;//���ɵ���Ż�����

				if (g_rdi[ibs].Qv >0)//�в����ĵ�
				{
					g_rdi[ibs].a_cosfi = 0.95;//������Ĺ�������						
					g_rdi[ibs].a_v = g_wb[ibs].v;//������ĵ�ѹ						
				}
			}

			//��ѹ����ѹ����޹������Ѿ���λ��ͬʱû�е�ѹԽ�޵�����
			putRPO(1);//���һ�׶ε��Ż����
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
	{		//��������΢����

//		double lamanda;

		getTmattrice();//ת���ſɱȾ���

		//�õ�����΢���ʣ�������
		getArrayQ();

		//�õ������㣬���3��

		//�޹����õ������󲹳�����
//		lamanda = getEconomy();	

	}
	else
	{
		getRdi();
		i = 0;
		while (1)
		{
			if (i==3)//��ಹ�������㣬������һ������ٲ��ڶ���
				break;

			//2.׼���ڶ��׶�
			//����ÿ���ڵ��һ�ξ�rdi_1�Ͷ��ξ�rdi_2

			//����ÿ���ڵ���޹���ģ��أ��������-������
			getQuadQ();

			//���ݵ�ѹ�����Ͷ��ξصõ�һ��������λ��
			site = getSite();
			if (site == -1)//�Ҳ����µĵ�
			{
				break;
			}

			//���㲹������
			comp_Q = get_Q(site);
			
			if (Sweep(site,comp_Q) <0)//����������
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
			if (ret == 0 )			//�����ɹ�
			{
				break;
			}

			i++;
		}
		
		//����,��׼�������ȫ����3���㶼ѡ������
		if (rdi_bs1!=-1)
		{
			g_rdi[rdi_bs1].Qv = Q1;//��������
			g_wb[rdi_bs1].nr -= Q1;//�ڵ���޹������ָ���ԭ״
		}
		if (rdi_bs2!=-1)
		{
			g_rdi[rdi_bs2].Qv = Q2;//��������
			g_wb[rdi_bs2].nr -= Q2;//�ڵ���޹������ָ���ԭ״
		}
		if (rdi_bs3!=-1)
		{
			g_rdi[rdi_bs3].Qv = Q3;//��������
			g_wb[rdi_bs3].nr -= Q3;//�ڵ���޹������ָ���ԭ״
		}

	//����������
		//�ڵ��ѹ�������ȫ����Ҫ���
		for(i = 0; i < lv_wb; i++)
		{
			if (g_rdi[i].b_v > 0)
			{
				g_rdi[i].a_v = g_wb[i].v;
			}
		}		

		putRPO(2);//������׶ε��Ż����


return 1;



//		int t_rdi_bs;
//		double t_rdi,rdi1/*�����޹���ȷ�����Ĳ�����1*/,rdi2,rdi3;
//		double Qv,rdi1_v/*����ǰ��ѹ*/,rdi2_v,rdi3_v,Qv1,Qv2,Qv3,Qv1_b,Qv2_b,Qv3_b;
//
//		//ȡ�������޹������������ڵ�
//		rdi1=rdi2=rdi3=0;
//		rdi_bs1=rdi_bs2=rdi_bs3=0;
//		for(i = 1; i < lv_wb; i++)
//		{	
//			//�����ĩ�ˣ�һ�㲻���Ž��в���
//
//
//			//ѡ��3�������޹����ɾ�
//			//��1����
//			if (g_rdi[i].quanQ > rdi1)
//			{
//				//���������Ļ���
//				//1.�����ĸ���ʱ
//				t_rdi = g_rdi[i].quanQ;//�����޹����ξ�ȷ��
//				t_rdi_bs = i;//�����޹����ξ�ȷ�� ���ֵ� ĸ��
//
//				//2.��˳��Ų��
//				rdi3 = rdi2;//�����޹����ξ�ȷ��
//				rdi_bs3 = rdi_bs2;//�����޹����ξ�ȷ�� ���ֵ� ĸ��
//				
//				rdi2 = rdi1;//�����޹����ξ�ȷ��
//				rdi_bs2 = rdi_bs1;//�����޹����ξ�ȷ�� ���ֵ� ĸ��
//				
//				//3.�����ĸ�1
//				rdi1 = t_rdi;//�����޹����ξ�ȷ��
//				rdi_bs1 = t_rdi_bs;//�����޹����ξ�ȷ�� ���ֵ� ĸ��
//			}
//
//			//2-1֮��
//			if ( (g_rdi[i].quanQ > rdi2) && (g_rdi[i].quanQ < rdi1) )
//			{
//				//1.��2-1֮��ĸ���ʱ
//				t_rdi = g_rdi[i].quanQ;//�����޹����ξ�ȷ��
//				t_rdi_bs = i;//�����޹����ξ�ȷ�� ���ֵ� ĸ��
//
//				//2.ԭ����1������3ȥ��������2
//				rdi3 = rdi2;//�����޹����ξ�ȷ��
//				rdi_bs3 = rdi_bs2;//�����޹����ξ�ȷ�� ���ֵ� ĸ��
//
//				//3.��2-1֮��ĸ�2
//				rdi2 = t_rdi;//�����޹����ξ�ȷ��
//				rdi_bs2 = t_rdi_bs;//�����޹����ξ�ȷ�� ���ֵ� ĸ��
//			}
//
//			//3-2֮��
//			if ( (g_rdi[i].quanQ > rdi3) && (g_rdi[i].quanQ < rdi2) )
//			{
//				rdi3 = g_rdi[i].quanQ;//��������޹����ξ�ȷ��
//				rdi_bs3 = i;
//			}
//		}
//
//		//׼�����޹����ɾؽ��в�������һ���޹����ɾ�������������
//		int k=0;
//		Qv1 = Qv2 = Qv3 = Qv1_b = Qv2_b = Qv3_b = 0;
//		while (1)
//		{
//			if ( (rdi1>0) && (rdi_bs1 > -1) )
//			{
//				Qv1_b = Qv1;//��ǰһ������������׼��Խ����ʱ����
//				ret = setCP(rdi_bs1,Qv1,k);
//
//				if (ret == 1)//��ȫ�ϸ�
//				{
//					g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//					break;
//				}
//
//				if ( (ret == 2) || (ret == 3) )//����Խ���޵����
//				{
//					g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//					//������һ�鳱��
//					ret = Sweep(rdi_bs1,Qv1_b);
//					g_wb[rdi_bs1].nr -= Qv1_b;//�ڵ���޹������ָ���ԭ״
//					break;
//				}
//			}
//
//			if ( (rdi2>0) && (rdi_bs2 > -1) )
//			{
//				Qv2_b = Qv2;//��ǰһ������������׼��Խ����ʱ����
//				ret = setCP(rdi_bs2,Qv2,k);
//
//				if (ret == 1)//��ȫ�ϸ�
//				{
//					g_wb[rdi_bs2].nr -= Qv2;//�ڵ���޹������ָ���ԭ״
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//					
//					break;
//				}
//
//				if ( (ret == 2) || (ret == 3) )//����Խ���޵����
//				{
//					g_wb[rdi_bs2].nr -= Qv2;//�ڵ���޹������ָ���ԭ״
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//
//					//������һ�鳱��
//					ret = Sweep(rdi_bs2,Qv1_b);
//					g_wb[rdi_bs2].nr -= Qv1_b;//�ڵ���޹������ָ���ԭ״
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//					
//					break;
//				}
//			}
//
//			if ( (rdi3>0) && (rdi_bs3 > -1) )
//			{
//				Qv3_b = Qv3;//��ǰһ������������׼��Խ����ʱ����
//				ret = setCP(rdi_bs3,Qv3,k);
//
//				if (ret == 1)//��ȫ�ϸ�
//				{
//					g_wb[rdi_bs3].nr -= Qv3;//�ڵ���޹������ָ���ԭ״
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//					if ( (rdi2>0) && (rdi_bs2 > -1) )
//						g_wb[rdi_bs2].nr -= Qv2;//�ڵ���޹������ָ���ԭ״
//					
//					break;
//				}
//
//				if ( (ret == 2) || (ret == 3) )//����Խ���޵����
//				{
//					g_wb[rdi_bs3].nr -= Qv3;//�ڵ���޹������ָ���ԭ״
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//					if ( (rdi2>0) && (rdi_bs2 > -1) )
//						g_wb[rdi_bs2].nr -= Qv2;//�ڵ���޹������ָ���ԭ״
//
//					//������һ�鳱��
//					ret = Sweep(rdi_bs3,Qv3_b);
//					g_wb[rdi_bs3].nr -= Qv3_b;//�ڵ���޹������ָ���ԭ״
//
//					if ( (rdi1>0) && (rdi_bs1 > -1) )
//						g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//					if ( (rdi2>0) && (rdi_bs2 > -1) )
//						g_wb[rdi_bs2].nr -= Qv2;//�ڵ���޹������ָ���ԭ״
//					
//					break;
//				}
//			}
//
//			if ( (rdi1>0) && (rdi_bs1 > -1) )
//				g_wb[rdi_bs1].nr -= Qv1;//�ڵ���޹������ָ���ԭ״
//		
//			if ( (rdi2>0) && (rdi_bs2 > -1) )
//				g_wb[rdi_bs2].nr -= Qv2;//�ڵ���޹������ָ���ԭ״
//
//			if ( (rdi3>0) && (rdi_bs3 > -1) )
//				g_wb[rdi_bs3].nr -= Qv3;//�ڵ���޹������ָ���ԭ״
//
//			if (k++ >100)
//				return 0;
//		}
//
	}


//����������
	//�ڵ��ѹ�������ȫ����Ҫ���
	for(i = 0; i < lv_wb; i++)
	{
		if (g_rdi[i].b_v > 0)
		{
			g_rdi[i].a_v = g_wb[i].v;
		}
	}		

	putRPO(2);//������׶ε��Ż����

	return 1;
}

//��ȡ�����ڵ㣬����������������
int CRO::getSite()
{
	int i,j,k;
	int pivot[MAX_MINPATH_LEN];
	double* quan_t = new double[lv_wb-1];//���ξ�
	double* quan = new double[lv_wb-1];//���ξ�
	int* ibs = new int[lv_wb-1];		//�ڵ��
	double quan_max;
	int max_ibs=-1;
	int ret_ibs=-1;

	int site;
	site = getPivot(pivot);
	
	//������ξ�
	for(i=1; i<lv_wb; i++)//������0�ĸ��ڵ�ȥ��
    {
		quan_t[i-1] = quan[i-1] = g_rdi[i].quanQ;
		ibs[i-1] =  g_rdi[i].ibs;
	}

	//��quan_t����
//	lld->sortDouble(quan_t,lv_wb-1);

	lld->sortDouble_Desc(quan_t,lv_wb-1);

	for(i=0; i<lv_wb; i++)
    {
		quan_max = 	quan_t[i];//�����Ŀ�ʼȡ
		if (quan_max <=0)//�����޹���һ��Ҫ����0
			continue;
		
		for(j=0; j<lv_wb; j++)
		{		
			if (quan[j] == quan_max)//�ҵ������ĵ�
			{
				max_ibs = ibs[j];
				//�Ƿ����������
				for(k=0; k<lv_wb; k++)
				{		
					if (pivot[k] == max_ibs)//���ξ���󣬶������������
					{
						//������ҵ��Ĳ�����Ϊ���е��κε㣬������
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
		if (ret == 0 )			//����������Ϳ�����
		{
			Q = Qv;
			return 1;
		}

		if ((ret == -2 ) ||	(ret == -3 ))		//ֻԽ����
		{
			Q = Qv;
			return 2;
		}
	}

	Q = Qv;
	return 0;
}

//����У��
int CRO::Sweep(int rdi_bs2, double Qv )
{
	//g_rdi[rdi_bs2].b_v = g_wb[rdi_bs2].v;//����ǰ�ĵ�ѹ
	//Qv = -Qv;

	g_rdi[rdi_bs2].Qv = Qv;
	g_wb[rdi_bs2].nr += Qv;
	
	//�����������������Ҫ�ټ���һ�³�����������û��Խ�ޣ�
	//�ر����·�ʽ����û���޹����͵�����
	//g_wb[rdi_bs2].b_v = g_wb[rdi_bs2].v;//����ǰ��ѹ

	g_ld_distr=0;//�޹��Ż�����ʱ���ܽ��и��ɷ���
	//1.�ȼ���һ�鳱��
	if (ExecuteSweep() == -1)//����������
	{
//		Q = Qv;
		return -4;
	}
	g_ld_distr=1;//���ɷ��俪�ش�

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
		g_rdi[i].Qv = 0;//��������
	}
}


int CRO::getRdi()
{
	int		i, j,k,m;
	int		ibs, zbs;
	double	rdi_2,r;//�������ڵ㿪ʼ�������������ڵ��ȫ��r��x�ۼ�

	int wb[mx_rdi];//���ٽڵ����������趨Ϊ1000��
    for(i = 0; i < mx_rdi; i++)
    {
        wb[i] = 0;
    }

	//��ÿ���ڵ㶼Ҫ���������ڶ�·��������
	//��ĩ��һֱ׷�ٵ��׶ˣ�һ��һ���׷
	for(i=1; i<lv_wb; i++)//������0�ĸ��ڵ�ȥ��
    {
        j = g_wb[i].jb;//�ϼ�֧·
		m = g_wb[i].jc;//�ϼ��ڵ�

		if (wb[i] == 1)//�Ѿ������ �����ϼ��ǵ�Դ��
			continue;

        ibs = g_wbr[j].ibs;//�׽ڵ�
        zbs = g_wbr[j].zbs;//ĩ�ڵ�
		r = g_wbr[j].r;

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		if (wb[zbs] == 0)//û����
		{
			g_rdi[zbs].ibs = zbs;
			g_rdi[zbs].ibr = j;
			g_rdi[zbs].ibsp = ibs;
			g_rdi[zbs].r = r;//�ڵ�����֧·���迹
			
			//���ε�Ԫ������һ��
			g_rdi[ibs].downstream[g_rdi[ibs].n_downstream++] = zbs;

			//�޹��ۿ������			
			//m=0;
			//��·��
			k = zbs;
			rdi_2=0;//֮ǰ��������̽ڵ�ĵ���
			while(1)
			{
				if (k == 0) //root (source) bus���ҵ���Դ�㣬����ֹͣ
				{
					break;
				}

				//׷����ǰ�����Ľڵ�Ϳ����ˣ��������ڵ��·��Ψһ
				if (wb[k] == 1)
				{
					rdi_2 += g_rdi[k].quanRdi;

					break;
				}

				rdi_2 += r;
				
				if (k==g_wb[k].jc)//������ѭ��
					break;

				k = g_wb[k].jc;//�ڵ�ĸ��ڵ㣬�Ż���
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

	//����buildRds�õ��Ľ��g_rds[]���д���
	//��Ҫ����ÿһ��ĸ�ߵ�Ķ��ξأ�0�ڵ�����
    for(i = 1; i < lv_wb; i++)
    {
		//�ҵ����ڵ���ϼ��������¼��ڵ㣬�����ϼ��ڵ�����֪�ģ��¼��ڵ���Ҫѭ��
		v = g_rdi[i].v = g_wb[i].v;
		dr = g_rdi[i].dr = g_wb[i].dr-g_wb[i].rloss;//����ýڵ���޹�ֵ

		//����
		quadQi = (dr*dr) / (v*v);
		
		//����
		quadQj = 0;
		for(j=0; j<g_rdi[i].n_downstream; j++)
		{
			k=g_rdi[i].downstream[j];
			t_q = g_rdi[k].dr = g_wb[k].dr-g_wb[k].rloss;
			t_v = g_rdi[k].v = g_wb[k].v;
			quadQj += (t_q*t_q)/(t_v*t_v);
		}


		//�������·��ĩ�ˣ��ⲻ��Ҫ���и��ϲ�����ֱ�ӽ��е�ѹ�ಹ��
		if (quadQj == 0)
			continue;

		g_rdi[i].quanQ = g_rdi[i].quanRdi * (quadQi - quadQj);
    }

	return 1;
}

//�õ��ڵ������
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
	
	//�ҵ����ڵ���ϼ��������¼��ڵ㣬�����ϼ��ڵ�����֪�ģ��¼��ڵ���Ҫѭ��
	origin = g_rdi[g_wb_id].dr;//ԭ�����޹�����,Ҳ����ԭ�����޹�����
	
	H1=H2=0;
	k = g_wb_id;//���ڵ���ϼ��ڵ�
	while(1)
	{
		if (k==0) 
			break;

		dr = g_rdi[k].dr;//�ڵ�������޹�
		r  = g_rdi[k].r;
		v  = g_rdi[k].v;
		
		H1 += (2 * dr * r) / (v * v);
		H2 += r  / (v * v);

		if (k==g_rdi[k].ibsp)
			break;
		else
			k = g_rdi[k].ibsp;
	}

	//��Ҫ����������
	rdi = (beta * tau * H1) / ( (2 * beta * tau * H2) + (alpha + gamma) * Kc );

	if (rdi > origin)
		return origin;
	else
		return rdi;
}

//��ÿ���ڵ���޹���
int CRO::buildRdi()
{
	int		i, j,k;
	int		ibs, zbs;
	double	rdi_1,rdi_2,v,r;//�������ڵ㿪ʼ�������������ڵ��ȫ��r��x�ۼ�

	int wb[mx_rdi];//���ٽڵ����������趨Ϊ1000��
    for(i = 0; i < mx_rdi; i++)
    {
        wb[i] = 0;
    }

	//��rdi����
	InitWorkingArray_1(0);

	//��ÿ���ڵ㶼Ҫ���������ڶ�·��������
	//��ĩ��һֱ׷�ٵ��׶ˣ�һ��һ���׷
	for(i=1; i<lv_wb; i++)//������0�ĸ��ڵ�ȥ��
    {
        j = g_wb[i].jb;//�ϼ�֧·
		//m = g_wb[i].jc;//�ϼ��ڵ�

		if (wb[i] == 1)//�Ѿ������ �����ϼ��ǵ�Դ��
			continue;

        ibs = g_wbr[j].ibs;//�׽ڵ�
        zbs = g_wbr[j].zbs;//ĩ�ڵ�

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		if (wb[zbs] == 0)//û����
		{
			g_rdi[zbs].ibs = zbs;
			g_rdi[zbs].ibr = j;
			g_rdi[zbs].ibsp = ibs;
			g_rdi[zbs].v = g_wb[zbs].v;
			g_rdi[zbs].dr = g_wb[zbs].dr-g_wb[zbs].rloss;//����ýڵ���޹�ֵ
			g_rdi[zbs].r = g_wbr[j].r;
			
			//����ǰ�ĵ�ѹ
//			g_rdi[zbs].b_v = g_wb[zbs].v;
//			if ( fabs(g_wb[zbs].nw)>0 || fabs(g_wb[zbs].nr)>0 )//����ǰ�Ĺ�������
//				g_rdi[zbs].b_cosfi = getCosfi(fabs(g_wb[zbs].nw),fabs(g_wb[zbs].nr));
			
			//���ε�Ԫ������һ��
			g_rdi[ibs].downstream[g_rdi[ibs].n_downstream++] = zbs;

			//�޹��ۿ������			
			//m=0;
			//��·��
			k = zbs;
			rdi_1=rdi_2=0;//֮ǰ��������̽ڵ�ĵ���
			while(1)
			{
				if (k == 0) //root (source) bus���ҵ���Դ�㣬����ֹͣ
				{
					break;
				}

				//׷����ǰ�����Ľڵ�Ϳ����ˣ��������ڵ��·��Ψһ
				if (wb[k] == 1)
				{
					rdi_1 += g_rdi[k].linearRdi;
					rdi_2 += g_rdi[k].quanRdi;

					break;
				}

				v = g_wb[k].v;
				r = g_wbr[g_wb[k].jb].r;//����ýڵ���޹�ֵ

				rdi_1 += r/(v*v);
				rdi_2 += r;
				
				if (k==g_wb[k].jc)//������ѭ��
					break;

				k = g_wb[k].jc;//�ڵ�ĸ��ڵ㣬�Ż���
			}
			g_rdi[zbs].linearRdi = rdi_1;
			g_rdi[zbs].quanRdi = rdi_2;
				
			wb[zbs] = 1;
		}
	}

	return 1;
}


//����ÿ���ڵ���޹����ξ�ȷ��
int  CRO::PutND_QuadQ()
{
	int i,j;
	int ibs,ibr,ibsp;
	double v,dr;
	double quadQi,quadQj,t_q,t_v;

	//����buildRds�õ��Ľ��g_rds[]���д���
	//��Ҫ����ÿһ��ĸ�ߵ�Ķ��ξأ�0�ڵ�����
    for(i = 1; i < lv_wb; i++)
    {
		//�ҵ����ڵ���ϼ��������¼��ڵ㣬�����ϼ��ڵ�����֪�ģ��¼��ڵ���Ҫѭ��
   		ibs = g_rdi[i].ibs;
		ibr = g_rdi[i].ibr;
		ibsp = g_rdi[i].ibsp;
		v = g_rdi[i].v;
		dr = g_rdi[i].dr;//����ýڵ���޹�ֵ
		//r = g_rdi[i].r;

		//����
		quadQi = (dr*dr) / (v*v);
		
		//����
		quadQj = 0;
		for(j=0; j<g_rdi[i].n_downstream; j++)
		{
			t_q = g_rdi[g_rdi[i].downstream[j]].dr;
			t_v = g_rdi[g_rdi[i].downstream[j]].v;
			quadQj += (t_q*t_q)/(t_v*t_v);
		}

		//�������·��ĩ�ˣ��ⲻ��Ҫ���и��ϲ�����ֱ�ӽ��е�ѹ�ಹ��
		if (quadQj == 0)
			continue;

		g_rdi[i].quanQ = g_rdi[i].quanRdi * (quadQi - quadQj);
    }

/*
    int    i, j, k,m;
    int    ibs, zbs;
//    double g, b, vi, ai, vj, aj, w, r, x, y;
//    double vi, ri, vj, rj, quadQi, quadQj/*, x, y;
    double t_quadQj;//��ʱ���������޹��������ۼ�;
//	double deltap,deltaq;//���㹦�ʲ�ֵ

	int wb[mx_rdi];
//�����еĽڵ���Ϊδ����
    for(i = 0; i < mx_rdi; i++)
    {
        wb[i] = 0;
    }

	//�޹����ξ�ȷ��ֻ�����ڵ�Դ��͸��ɵ�֮�⣬Ҳ����0�ڵ�
	//�޹����ξ�ȷ��ҲӦ�ð���ĩ�˽ڵ㣬��������
	for(i = lv_wb - 1; i > 0; i--)
    {
        j = g_wb[i].jb;//�ϼ�֧·
		m = g_wb[i].jc;//�ϼ��ڵ�

		if ((wb[m] == 1) || (m == 0))//�Ѿ������ �����ϼ��ǵ�Դ��
			continue;

        ibs = g_wbr[j].ibs;//�׽ڵ�
        zbs = g_wbr[j].zbs;//ĩ�ڵ�

        if(ibs > zbs)
        {
            k = ibs;
            ibs = zbs;
            zbs = k;
        }

		//��ĩ�ڵ�ĵ�ѹ���޹�
        vi = g_wb[ibs].v;
        ri  = g_wb[ibs].dr-g_wb[ibs].dloss;//����ýڵ���޹�ֵ
        vj = g_wb[zbs].v;
        rj  = g_wb[zbs].dr-g_wb[zbs].dloss;//drΪ�ڵ��ǰ�����빦�ʣ�lossΪ���߶ε����

		quadQi = (ri*ri) / (vi*vi);
		quadQj = (rj*rj) / (vj*vj);

		//�ҵ����ϼ��ڵ������������֧·
		//�ص㣺�ڵ㹲��һ���ϼ��ڵ�
		for(k = lv_wb - 1; k > 0; k--)
		{
			if ((m == g_wb[k].jc) && (k != i)) //�����ϼ��ڵ�
			{
				vj = g_wb[k].v;
				rj = g_wb[k].dr-g_wb[k].dloss;//����ýڵ���޹�ֵ

				t_quadQj = (rj*rj) / (vj*vj);	
				quadQj += t_quadQj;
			}
		}
		g_wb[m].rdi = quadQi-quadQj;
        wb[m] = 1;
    }

	//ֻ��û���ҵ��Ĳ���ĩ��
	for(i = lv_wb - 1; i > 0; i--)
    {
		if ((wb[i] == 1) || (i == 0))//�Ѿ������ �����ϼ��ǵ�Դ��
			continue;
		else
		{
			vj = g_wb[i].v;
			rj  = g_wb[i].dr-g_wb[i].dloss;//drΪ�ڵ��ǰ�����빦�ʣ�lossΪ���߶ε����
			quadQj = (rj*rj) / (vj*vj);
			g_wb[i].rdi = quadQj;

			wb[i] = 1;
		}
	}
*/
	return 1;
}

//��������ڵ��һ���޹��أ��������ȷ������ڵ�Ĳ�������
double CRO::getND_DelQ(int g_wb_id)
{
	int	 k;
//	int ibsp;
	double dr;
	double	ret=0;
	double	Q1,last,rdi;
	double	a,H2,H1;
	a = 0;
	//����һ�α�,��ǰ���ȽϺü���

	//����ڵ�
	k = g_wb_id;
	H2 = g_rdi[k].linearRdi;//ĩ�ڵ�
	k = g_rdi[k].ibsp;
	H1 = g_rdi[k].linearRdi;//ĩ�ڵ����һ���ڵ�
	Q1 = g_rdi[k].dr;	

	//
	rdi = 0;
	last = Q1;
	k = g_rdi[k].ibsp;
	while(1)
	{
		if (k==0) 
			break;

		dr = g_rdi[k].dr;//�ڵ�������޹�
		rdi += g_rdi[k].linearRdi * (dr-last);
		last = dr;

		if (k==g_rdi[k].ibsp)
			break;
		else
			k = g_rdi[k].ibsp;
	}
	
	rdi = (rdi+H1*Q1)/(H1-H2);//�������Ժ���޹�������
	
	if (fabs(rdi) < a)//�����ı���Ҫ����
	{
		a = fabs(rdi)-a;	
	}
	else//�ýڵ�һ����Ҫ���޹�����+�����޹���������������������޹�����
	{
		a = -a;
	}

	return a;//������Ҫ�������޹�����������0

/*
	rdi = 0;
	//��Ҫ�������õ���0��ʼ��·��
	for(i=n_Y; i>0; i--)
	{
		ibs = Y[i-1];//���νڵ�

		v = g_rdi[i].v;
		dr = g_rdi[i].dr;//����ýڵ���޹�ֵ
		r = g_rdi[i].r;//�ϼ��ĵ���

		rdi = r/(v*v);
		rdi *= (dr - g_rdi[ibs].dr);
	}


//�ڵ�ǰ������·���ϵ�һ�ξؽ����ۼ�
	//��������׽ڵ㣨��Դ�ڵ㣩 �ĳ�������
	while(1)
	{
		i = g_rdi[g_wb_id].ibs;//��ѡ���ĵ㿪ʼ�����������Ż���

		if(i == 0)  //root (source) bus���ҵ���Դ�㣬����ֹͣ
		{
			g_rdi[lv_rdi-1].dr = g_wb[i].nr;//���ڵ������֧·�޹�
			g_rdi[lv_rdi-1].v = g_wb[i].v;//���ڵ��ѹ
			break;
		}

		j = g_wb[i].jb;//�ڵ�ĸ�֧·���Ż���

		g_rdi[lv_rdi-1].r = g_wbr[j].r;//��֧·
		g_rdi[lv_rdi-1].ibr = j;//��֧·
		g_rdi[lv_rdi-1].dr = g_wb[i].dr-g_wb[i].dloss;//���ڵ������֧·�޹�
		g_rdi[lv_rdi-1].v = g_wb[i].v;//���ڵ��ѹ

		ibs = g_wbr[j].ibs;//����һ����������������߶�
        zbs = g_wbr[j].zbs;
	}
	
	for (i=0; i<=lv_rdi-2; i++)
	{
		//����i>1�������ÿ�ζ���Ҫ�ѽڵ�ǰ����·����R / U^2 �����ۼ�
		//��Ҫ����ÿһ���ڵ��һ�ξغ��޹����
		for (j=i; j<=lv_rdi-2; j++)
		{// R / U^2 
			Y[i] += g_rdi[j].r/ (g_rdi[j].v * g_rdi[j].v);
		}
	}


	lv_rdi = 0;
	g_rdi[lv_rdi++].ibs = g_wb_id;//�ڵ��Ż���ı�ţ���������ȥ�������ڵ� 
	//��������ڵ������������ĵ��������������Ӧ����0
	while(1)
	{
		i = g_rdi[lv_rdi-1].ibs;//��ѡ���ĵ㿪ʼ�����������Ż���

		if(i == 0)  //root (source) bus���ҵ���Դ�㣬����ֹͣ
		{
			g_rdi[lv_rdi-1].dr = g_wb[i].nr;//���ڵ������֧·�޹�
			g_rdi[lv_rdi-1].v = g_wb[i].v;//���ڵ��ѹ
			break;
		}

		j = g_wb[i].jb;//�ڵ�ĸ�֧·���Ż���

		g_rdi[lv_rdi-1].r = g_wbr[j].r;//��֧·
		g_rdi[lv_rdi-1].ibr = j;//��֧·
		g_rdi[lv_rdi-1].dr = g_wb[i].dr-g_wb[i].dloss;//���ڵ������֧·�޹�
		g_rdi[lv_rdi-1].v = g_wb[i].v;//���ڵ��ѹ

		ibs = g_wbr[j].ibs;//����һ����������������߶�
        zbs = g_wbr[j].zbs;
		if(ibs == i)
		{
			//�ڵ�ĸ��ڵ�
			g_rdi[lv_rdi].ibs = zbs;//�ҵ����ϵ����һ������Ϊ�Ѿ��ֲ��Ż���ţ�����һ������̵�·��
			g_rdi[lv_rdi-1].ibsp = zbs;//�ҵ����ϵ����һ������Ϊ�Ѿ��ֲ��Ż���ţ�����һ������̵�·��
		}
		else
		{
			g_rdi[lv_rdi].ibs = ibs;
			g_rdi[lv_rdi-1].ibsp = ibs;
		}

		lv_rdi++;
	}

	if (lv_rdi<=2)//��Դ�㱾��
		return 0;

//������ɺ���Ҫ�����ݽ���һ�ξ�ȷ�صļ���
	//��������׽ڵ㣨��Դ�ڵ㣩 �ĳ�������
	for (i=0; i<=lv_rdi-2; i++)
	{
		//����i>1�������ÿ�ζ���Ҫ�ѽڵ�ǰ����·����R / U^2 �����ۼ�
		//��Ҫ����ÿһ���ڵ��һ�ξغ��޹����
		for (j=i; j<=lv_rdi-2; j++)
		{// R / U^2 
			Y[i] += g_rdi[j].r/ (g_rdi[j].v * g_rdi[j].v);
		}
	}

	a = 0;
	//Y[0]�� Y[1]֮ǰ��ֱ�ӽ����ۼӾͿ�����
	for (i=2; i<=lv_rdi-2; i++)
	{// (R / U^) * (Qi-Qj)
		a += -(g_rdi[i].dr - g_rdi[i-1].dr) * Y[i];
	}
	
	//����Y[0] - Y[1]Ҫ���⴦����Ҫ���нⷽ��
	if ( Y[0] - Y[1] > 0)
		ret = (a + (Y[1] * g_rdi[1].dr)) / (Y[0] - Y[1]);

	if (ret > 0.006)
		ret = 0.006;

	return ret;
*/
}

//������ѹ 0 ��ѹ��ȫ�ϸ񣬲��貹����-1 �����ܲ�����λ -2 ���Բ�����λ -3 ��ѹԽ����
//DEL int CRO::analyseVoltage()
//DEL {
//DEL 	int i,j,m,ibs;
//DEL 	int ret=0;
//DEL 	int l_flag,h_flag;
//DEL 	double dw,dr,r,x,delta;
//DEL 
//DEL 	l_flag = h_flag = 0;
//DEL 	//���㲹����ĵ�ѹ��ֵ������ǰ��Ա�,����ǰ��״̬
//DEL 	//���в��ϸ�ڵ�ȫ����ʾ����+7%�� -10%
//DEL 	for(i = 0; i < lv_wb; i++)
//DEL 	{
//DEL 		ibs = g_wb[i].tio;
//DEL 		if ( ibs >= 0 ) //����������  
//DEL 		{
//DEL 			//��Ҫ�жϵ�ѹ�Ƿ�Խ��
//DEL 			//g_wb[i].b_v = g_wb[i].v;//����ǰ�ĵ�ѹ
//DEL 			if ( g_wb[i].v < 0.93 )
//DEL 			{
//DEL 				l_flag = -1;
//DEL 				//���ϼ��ڵ�ĵ�ѹ���бȽ�
//DEL 
//DEL 				j = g_wb[i].jb;//�ϼ�֧·
//DEL 				m = g_wb[i].jc;//�ϼ��ڵ�
//DEL 				
//DEL 				dw = g_wb[m].dw-g_wb[i].wloss;//����ýڵ���޹�ֵ
//DEL 				dr = g_wb[m].dr-g_wb[i].rloss;//����ýڵ���޹�ֵ
//DEL 				r = g_wbr[j].r;//�ڵ��ϼ��ĵ翹
//DEL 				x = g_wbr[j].x;//�ڵ��ϼ��ĵ翹
//DEL 
//DEL 				//���ε���ģ����޹�ȫ���͵ز���������
//DEL 				delta = 0.93+(dw*r)/0.93;//��һ����ѹ
//DEL 
//DEL 				//delta = (0.93*(0.93-g_wb[i].v))/x;
//DEL 
//DEL 				//�������޹����ͣ�������ѹ����
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
//DEL 	//һ��������ж�
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



//����޹����,0--����Ҫ���в�����lev==1 һ�׶Σ�2--���׶�;3--���ϸ��ѹ�ĸ���
int CRO::putRPO(int lev)
{
	int i;
	int ibs,pnd;
	char fdid[80];//������������ID
//	char ndid[80];//������������ID

	int lv_ld = dsbob2->ld_tb->GetCount();
	LD* ld = dsbob2->ld_tb->GetTable();
	ND* nd = dsbob2->nd_tb->GetTable();
	BS* bs = dsbob2->bs_tb->GetTable();//��ĸ���ϵĲ�����Ͳ���������¼����


	//slack so
	SO* so = dsbob2->so_tb->GetTable();
//	 so_id[80];
//	lld->delstr(so->id,7,11,so_id);
	i = g_wg[g_slackgen].logic;
	strcpy(fdid,(so+i)->fdid);
	
	if (lev == 1)//ֻ���һ�׶ε��Ż����
	{
		for(i = 0; i < lv_wb; i++)
		{
			//�κ��е�ѹ����Ľڵ㶼��Ҫ���
			if ( (g_rdi[i].b_v > 0) || ((g_rdi[i].Qv > 0) && (g_wb[i].tio>=0)) ) //��һ�׶�ֻ�и��ɵ�
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

	if (lev == 2)//������׶ε��Ż����
	{
		for(i = 0; i < lv_wb; i++)
		{

			//ֻҪ���޹���ȷ�ص���������㶼�г���
			if ( 
				(g_rdi[i].b_v < 0.93) //ǰ��ѹ������
				||(g_rdi[i].b_v > 10.07) //ǰ��ѹ������
				||(g_rdi[i].a_v < 0.93) //���ѹ������
				||(g_rdi[i].a_v > 10.07) //���ѹ������
				|| ((i == rdi_bs1) && (g_rdi[i].Qv > 0)) 
				|| ((i == rdi_bs2) && (g_rdi[i].Qv > 0))
				|| ((i == rdi_bs3) && (g_rdi[i].Qv > 0)) 
				)
			{
				//�ҵ���صĽڵ㣬����ĸ��
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
					
					//����ǰ��Ĺ�������
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

	if (lev == 3)//������׶ε��Ż����
	{
		for(i = 0; i < lv_wb; i++)
		{
			ibs = g_wb[i].tio;
			pnd = (bs+ibs)->pnd;
			if (
				((g_rdi[i].b_v > 0) && (pnd>=0)) 
				||(g_rdi[i].Qv > 0)
				) //��һ�׶�ֻ�и��ɵ�
			{
				dsbob2->ro_result[lld->g_cnt_ro_fd].id=lld->g_cnt_ro_fd;
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].ndid,(nd+pnd)->id);
				strcpy(dsbob2->ro_result[lld->g_cnt_ro_fd].fdid,fdid);

				if (g_rdi[i].Qv >0)
					dsbob2->ro_result[lld->g_cnt_ro_fd].Qv = g_rdi[i].Qv;
				else
					dsbob2->ro_result[lld->g_cnt_ro_fd].Qv = 0;

				//����ǰ��Ĺ�������
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

//��������޹���ȷ�صġ��ۡ������ء�Ϊ�ýڵ���޹����
double CRO::getNDRDI(int g_wb_id/*�ڵ��Ż���ı��*/)
{
	int		i, j;
	int		ibs, zbs;
	double	total_r;//,total_x;//�������ڵ㿪ʼ�������������ڵ��ȫ��r��x�ۼ�

	//i = fault->ibs;//���ϵ㿪ʼ�����������õ����ϵ㵽��Դ�����·��
	//i= 5;//�Ż�ǰ�ļ���ĸ�߱�ţ�ito�Ż�ǰ�ı��
//	g_rdi[lv_rdi].ibs = g_wb[i].toi;//�ڵ��Ż���ı�ţ���������ȥ�������ڵ� 
//	lv_rdi++;

	lv_rdi=total_r=0;
	g_rdi[lv_rdi++].ibs = g_wb_id;//�ڵ��Ż���ı�ţ���������ȥ�������ڵ� 
	//��������ڵ������������ĵ��������������Ӧ����0
	while(1)
	{
		i = g_rdi[lv_rdi-1].ibs;//�ӹ��ϵ㿪ʼ�����������Ż���

		if(i == 0)  //root (source) bus���ҵ���Դ�㣬����ֹͣ
		{
			break;
		}

		j = g_wb[i].jb;//�ڵ�ĸ��ڵ㣬�Ż���

		ibs = g_wbr[j].ibs;//����һ����������������߶�
        zbs = g_wbr[j].zbs;
		total_r += g_wbr[j].r;
		//�޹���ȷ��ֻ��Ҫ���裬����Ҫ�翹������ֻ��Ҫ��С·�ϵĵ��裬��·�������Ժ���
		//total_x += g_wbr[j].x;

		if(ibs == i)
		{
			g_rdi[lv_rdi].ibs = zbs;//�ҵ����ϵ����һ������Ϊ�Ѿ��ֲ��Ż���ţ�����һ������̵�·��
		}
		else
		{
			g_rdi[lv_rdi].ibs = ibs;
		}
//
//		g_rdi[lv_rdi-1].ibr = j;//���ڵ�
//		
//		g_wbr[j].fp = 1;  //��ע�ҵ������߶�
//
		lv_rdi++;
	}

	return total_r;
}



