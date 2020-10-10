// SC.cpp: implementation of the CSC class.
//
//////////////////////////////////////////////////////////////////////
#if _WIN32
#include "../DNASPLIB/SC.h"
#elif __linux__
#include "../DNASPLIB/SC.h"
#else
#endif

#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSC::CSC(DSBOB2* m_pDb, CLLD* m_pLLD) : PWRFLOW(m_pDb, m_pLLD)
{
	dsbob2 = m_pDb;
	lld = m_pLLD;

//	PWRFLOW(dsbob2);//���������ʼ��

	g_fu		= new G_FU[mx_fu];
	lv_fu		= 0;

	fault		= new FAULT[1];
}

CSC::~CSC()
{
	delete[] g_fu;
	delete[] fault;
}

//ִ�ж�·��������
//��Ҫ���������ĳ������Ա�õ����ϵ㵽��Դ��ĵ�Ч�迹
//��������㳱���Ļ�����Ҫ֪��ȫ���ľ���
int CSC::OnMathSC(char * Efile_name,int type)
{
	char str[500];
	//lldapi lld_t;
	CLLD lld_t;
	//get island number
	int lv_island = dsbob2->island_tb->GetCount();

	outfile.open(Efile_name, ios::out);
	outfile << "<!System= Version=1.0 Code=UTF-8 Data=!>" << endl;

	//��������·������,����׶˺�ĩ�˵Ķ�·����ֵ,���࣬���࣬���࣬����ӵ�
	outfile << "<sec::sc name=" << "\"��·����������\"" << ">" << endl;
	outfile << "@\tsec_id\ti_b1_A/B/C_phase(kA)\tu_b1_A/B/C_phase(kV)\t"
		<< "i_b2_A/B/C_phase(kA)\tu_b2_A/B/C_phase(kV)\t" 
		<< "i_b3_A/B/C_phase(kA)\tu_b3_A/B/C_phase(kV)\t"
		<< "i_b21_A/B/C_phase(kA)\tu_b21_A/B/C_phase(kV)\t"
		<< "i_e1_A/B/C_phase(kA)\tu_e1_A/B/C_phase(kV)\t"
		<< "i_e2_A/B/C_phase(kA)\tu_e2_A/B/C_phase(kV)\t"
		<< "i_e3_A/B/C_phase(kA)\tu_e3_A/B/C_phase(kV)\t"
		<< "i_e21_A/B/C_phase(kA)\tu_e21_A/B/C_phase(kV)"
		<< endl;
	outfile << "/@\t���߶�id\t(��)�����·����_A��/B��/C��(kA)\t(��)�����·��ѹ_A��/B��/C��(kV)\t"
		<< "(��)�����·����_A��/B��/C��(kA)\t(��)�����·��ѹ_A��/B��/C��(kV)\t"
		<< "(��)�����·����_A��/B��/C��(kA)\t(��)�����·��ѹ_A��/B��/C��(kV)\t"
		<< "(��)�����·�ӵص���_A��/B��/C��(kA)\t(��)�����·�ӵص�ѹ_A��/B��/C��(kV)\t"
		<< "(ĩ)�����·����_A��/B��/C��(kA)\t(ĩ)�����·��ѹ_A��/B��/C��(kV)\t"
		<< "(ĩ)�����·����_A��/B��/C��(kA)\t(ĩ)�����·��ѹ_A��/B��/C��(kV)\t"
		<< "(ĩ)�����·����_A��/B��/C��(kA)\t(ĩ)�����·��ѹ_A��/B��/C��(kV)\t"
		<< "(ĩ)�����·�ӵص���_A��/B��/C��(kA)\t(ĩ)�����·�ӵص�ѹ_A��/B��/C��(kV)"
		<< endl;

	//execute calculations ��ÿһ�������м���
	for(g_island = 0; g_island < lv_island; g_island++)
	{
		//g_island = 0;
		//1.�ȼ��㳱��
//				 so_id[80];
//				int soid=dsbob2->island_tb->GetDataByLog(g_island)->refso;
//				strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->id);

		//ֻ�Ա����ڵ����ݽ��г�ʼ��
		//initialize working arrays		1.��ĸ�߽��г�ʼ��
		InitWorkingArray();

		//read network data		2.������������
		if(GetNetworkData() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

		if(OrderBusNumberB() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

		if (lld->g_ZeroSerialGrounding == 1)
		{
			//�����ɾ���
			if (FormAdmittance0() == PWRFLOW_FAIL)
			{
				return PWRFLOW_FAIL;
			}
			//����������ӱ�
			if (FormFactorTable() == PWRFLOW_FAIL)
			{
				return PWRFLOW_FAIL;
			}
			SolveStatic(0);
		}

		//2.�ټ����·����
		//form admittance matrix�γɵ��ɾ���
		if(FormAdmittance() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

		//���ӱ���������򶼵���
		if (FormFactorTable() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

		SolveStatic(1);

		//��������������ֵ
		Calc3Series();

		//��������ļ�����
		if(PutResult() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

		//ofstream outfile("sc_test.txt",ios::out);
	 //
	 //	outfile<<"   "<< endl;
		//outfile<<"i "<<"d   "<<"i   "<<"j   "<<"p    "<<"g   "<<"b     "<<endl;
	 // 	for(int i = 0; i <10; i++)
	 // 	{
	 //		outfile<<i<<" "<<g_fu[i].d<<"   "<<g_fu[i].i<<"   "<<g_fu[i].j<<"   "<<g_fu[i].p<<"   "<<g_fu[i].g<<"   "<<g_fu[i].b<< endl;
	 // 	}
	 //	
		//outfile.close();
	}
	outfile<<"</sec::sc>"<< endl;
	outfile<<" "<< endl;
	outfile.close();
	
	return 1;

}

//1.�ٶ��Խ�Ԫ�ؾ�����0���������ר�ŵ�����
//2.�ǶԽ�Ԫ�صĴ洢��һ������g_fu����
//3.һ�����ɾ����Ӧһ�����Ӿ���
int CSC::FormFactorTable()
{
    int    i, k, k_f/*���ӱ�ĶԽ�λ��*/, m, n, f/*���ӱ����*/,p/*�е�����*/;
    int    jmx, jmn;
    double yg, yb, g, b;
	double d11_g,d11_b,d22_g,d22_b,u12_g,u12_b;//�Խ�Ԫ�غͷǶԽ�Ԫ��
	int i1;
	int find;
	int irow;//�Խ������Ӧ����
	double aij1,aij2;//���ɾ�����������е�Ԫ��

	//��Խ�Ԫ�ص�ʵ�����鲿���Խ�Ԫ����ȫ����
	double* GD = new double[lv_wb];//ֻ�жԽǲſ�����ô���壬��Ϊ�Խ�Ԫ�ظ�����ȷ����
	double* BD = new double[lv_wb];

	double* GJ = new double[lv_wb];//��������
	double* BJ = new double[lv_wb];
	double* GK = new double[lv_wb];//���������
	double* BK = new double[lv_wb];

    //initialize upper trangular matrix
	//��Ϊ�ǶԽ�Ԫ�صĸ�������ȷ��������ֻ��ȡһ�����ֵ
    for(i = 0; i < mx_fu; i++)
    {
        g_fu[i].d = 0;//�Խ�Ԫ��λ�ã��Խ�׼����GD+jBD�ĵ���
        g_fu[i].i = 0;//�����ǵ���
        g_fu[i].j = 0;//�����ǵ���
        g_fu[i].p = 0;//�ж�Ӧ������,ͨ������������еĸ���Ԫ���ҵ�
        g_fu[i].g = 0.0;//���ӵ�ʵ��
        g_fu[i].b = 0.0;//���ӵ��鲿
    }
    lv_fu = 0;

	//����Ѿ�֪�����ɾ���ʵ���Ͽ���ֱ���ù�ʽ���м���
	//dii = aii - (u1i^2 * d11 + u2i^2 * d22 + u3i^2 * d33 + ... + u(i-1)i^2 * d(i-1)(i-1))
	//uij = (aij - (u1i*u1j*d11 + u2i*u2j*d22 + u3i*u3j*d33 + ... +u(i-1)i*u(i-1)j*d(i-1)(i-1))/dii 
	//��ÿ���ڵ���д����ӵ�һ�п�ʼ����

	f = 0; //���ӱ�Ԫ�ش�0��ʼ����
	//jf = 0;//�ǶԽ�Ԫ�ظ���
    for(i = 0; i < lv_wb; i++)
    {
		//����λ�ã���������

		//��һ�������Խ�Ԫ��
		k = g_yu[i].d;//�ڵ�ĶԽ�,���ɾ����Ԫ��

		//�Խ�Ԫ��һ������
		//��λ��
 		g_fu[i].d = f;//��һ���Խǵ�λ��+��һ�еķǶԽ�Ԫ�ظ��������Բ���
		k_f = f;//���ӱ�ĶԽ�λ�ã����ӱ�ĶԽ�λ�ú͵��ɾ����п��ܲ�һ������Ϊ�������µ�ע��
		g_fu[f].i = i;
		g_fu[f].j = i;//�Խǵ��к�����һ����
		g_fu[f].p = f;//��Ĭ���Լ�Ϊ������ʼ

		//�޸�����
		//��һ����Ӧ���У�ѭ��ǰ��һ�е�����Ԫ��
		//��Ҫ����������Ӻ�����ǰ���ң�����һ��,�ҵ�һ���Ϳ�����
		for(m = f-1; m >=0 ; m--)
		{
			if (g_fu[m].j == i)//�ҵ���ǰ�����һ�У��޸�����
			{
				n = g_fu[m].p;
				g_fu[f].p = n;//��һ��
				g_fu[m].p = f;

				break;//ֻҪ�ҵ�һ���Ϳ����ˣ����һ����û�У��Ͳ�Ҫ�޸�����
			}
		}

		//������
		//�Լ�������
		yg = g_yu[k].g;//�Խǵ�ʵ��
		yb = g_yu[k].b;//�Խǵ��鲿
		
		//ʵ��������Ϊ�����Ϳ����ˣ��Խǿ���Ĭ��һֱ������
		d22_g = d22_b = 0;
		p = g_fu[k_f].p;//�Լ�����һ����ʵ������ͷ�ϵ��Ǹ�
		while(p != k_f)//�����Լ�˵������ͷ��
        {
            g = g_fu[p].g;
            b = g_fu[p].b;
            irow = g_fu[p].i;//�����ڵ���
			
			//�ӶԽ���������ȡ������еĶԽ�Ԫ��
			d11_g = GD[irow];
			d11_b = BD[irow];

			//�öԽ�Ԫ�غ����ڵķǶԽ�Ԫ�ؽ�������
			//(g+jb)^2 * (d11_g+jd11_b)
			d22_g += (g*g - b*b) * d11_g - 2*g*b*d11_b;
			d22_b += (g*g - b*b) * d11_b + 2*g*b*d11_g;

			p = g_fu[p].p;
        }
		d22_g = yg - d22_g; 
		d22_b = yb - d22_b;
		//���Խ����鸳ֵ
		GD[i] = d22_g;
		BD[i] = d22_b;
		//�����ӱ�ֵ
		g_fu[f].g =  d22_g / (d22_g*d22_g + d22_b*d22_b); //�������������ӱ�
		g_fu[f].b = -d22_b / (d22_g*d22_g + d22_b*d22_b); //�������ۼ�

		//����(����Ϊ�̶���Ҳ���ǶԽ�Ԫ�ض�Ӧ����)
		//1.�Ȱ��Լ���Ӧ�����е�ǰ��������ҳ�������Ӧ�ĶԽ��ҳ���
		for (m=0; m<i; m++)//ֻ��ѱ���֮ǰ��������վͿ�����
		{
			GJ[m]=BJ[m]=0;
		}
		p = g_fu[k_f].p;//�Լ�����һ��
		while(p != k_f)//�����Լ�˵������ͷ��
		{
			g = g_fu[p].g;
			b = g_fu[p].b;
			
			GJ[g_fu[p].i] = g;
			BJ[g_fu[p].i] = b;

			p = g_fu[p].p;//�Լ�����һ��
		}
		f++;

//�ڶ��������ǶԽ�Ԫ��
		//�����У����ӱ�Ԫ�ض�Ӧ���У�׼��д����У�
		//���۱���Ԫ���Ƿ�Ϊ0������Ҫά��������Ϊǰ�������Ԫ�ص����µ�ע��
		for(i1 = i+1; i1 < lv_wb; i1++)//��1�п�ʼ��һֱ������i
		{
			for (m=0; m<i; m++)//ֻ��ѱ���֮ǰ��������վͿ�����
			{
				GK[m]=BK[m]=0;
			}

			//�ӵ��ɾ�������ѹ����е�Ԫ���õ�(�Լ�����)
			aij1 = aij2 = 0;
			jmn = k + 1;//�Խ�Ԫ�ص���ʼλ��
			jmx = g_yu[i+1].d;//�����Խ�֮���Ԫ�أ�Ҳ���ǷǶԽ�Ԫ�ص�
			for(m = jmn; m < jmx; m++)
			{
				if (i1 == g_yu[m].j)//���ɾ����������У�˵����Ϊ0
				{
					aij1 = g_yu[m].g;
					aij2 = g_yu[m].b;

					break;
				}
			}	
			
			//�����µ�ע��Ԫ
			//����Ϊ�գ�������ǰ�������Ԫ�أ������µ�ע��
			find=-1;
			for(m = f-1; m >=0 ; m--)
			{
				if (g_fu[m].j == i1)//�ҵ���ǰ�����һ�У��޸�����
				{
					//��ʾ����һ���н�����
					find = m; //��n�����������ã���������ά������
					g = g_fu[m].g;
					b = g_fu[m].b;
					
					GK[g_fu[m].i] = g;
					BK[g_fu[m].i] = b;

					p = g_fu[m].p;//�Լ�����һ��
					while(p != m)//�����Լ�˵������ͷ��
					{
						g = g_fu[p].g;
						b = g_fu[p].b;
						
						GK[g_fu[p].i] = g;
						BK[g_fu[p].i] = b;

						p = g_fu[p].p;//�Լ�����һ��
					}
					break;//�ҵ�һ���Ϳ�����
				}
			}
		
			//һ�������������д����ʱ���������
			u12_g=u12_b=0;
			for (m=0; m<i; m++)
			{//��������˵õ�(GD+jBD)*(GJ+jBJ)*(GK+jBK)
				u12_g += GD[m]*GJ[m]*GK[m] - GD[m]*BJ[m]*BK[m] - BD[m]*GJ[m]*BK[m] - BD[m]*BJ[m]*GK[m];
				u12_b += GD[m]*GJ[m]*BK[m] + GD[m]*BJ[m]*GK[m] + BD[m]*GJ[m]*GK[m] - BD[m]*BJ[m]*BK[m];
			}

			//( (aij1+jaij2) - (u12_g+ju12_b) )/ (d22_g + jd22_b)
			g = ( (aij1 - u12_g)*d22_g + (aij2 - u12_b)*d22_b ) / (d22_g*d22_g + d22_b*d22_b);
			b = ( -(aij1 - u12_g)*d22_b + (aij2 - u12_b)*d22_g ) / (d22_g*d22_g + d22_b*d22_b);
			
			if ( (g != 0) || (b != 0) )
			{
				//��Ϊ0����Ҫά��������
				if (find != -1)
				{
					n = g_fu[find].p;
					g_fu[f].p = n;//��һ��
					g_fu[find].p = f;
				}
				else
					g_fu[f].p = f;
				
				g_fu[f].i = i; 
				g_fu[f].j = i1;
				g_fu[f].g = g;
				g_fu[f++].b = b;
			}
		}
    }

	delete[] GD;
	delete[] BD;
	delete[] GJ;
	delete[] BJ;
	delete[] GK;
	delete[] BK;

	return 1;
}

//��������,�������������Ե��ã�ibsΪ��·��ļ���ĸ�߱��
//ͨ��������ȡ��������ڵ���迹ֵ,����Ҫ��ڵ�k�Ķ�·����
//(1)��f
//fi=0; i<k
//fi=1; i==k;
//�����·��Ϊ2��f3=-u23*f2,f4=-u24*f2-u34*f3,f5=-u25*f2-u35*f3-u45*f4,f6=-u26*f2-u36f3-...-f56*f5;

//(2)��h
//hi=fi/dii;
//(3)��Z �������Ǹ�h��ʼ��ǰ������

int CSC::SolveStatic(int type)
{
	int    i, j, f, m, k, p/*�е�����*/;
	int    jmx, jmn;
	double f11_g, f11_b, f22_g, f22_b, f12_g, f12_b;//�Խ�Ԫ�غͷǶԽ�Ԫ��

	double* ff_g = new double[lv_wb];
	double* ff_b = new double[lv_wb];
	double* hh_g = new double[lv_wb];
	double* hh_b = new double[lv_wb];
	double* zz_g = new double[lv_wb];
	double* zz_b = new double[lv_wb];

	if (type == 1)
	{//����
		fr.clear();
		fx.clear();
	}
	else
	{//����
		fr0.clear();
		fx0.clear();
	}

	//ÿ���ڵ�ɨ��,�õ�ÿ���ڵ�Ķ�·����
	//��i���㷢������,�൱������i��
	for (i = 0; i < lv_wb; i++)
	{
		//��ʼ��
		for (m = 0; m < lv_wb; m++)
		{
			ff_g[m] = ff_b[m] = 0;//�����е���Ϊ0
			hh_g[m] = hh_b[m] = 0;//�����е���Ϊ0
			zz_g[m] = zz_b[m] = 0;//�����е���Ϊ0
		}

		//�����Լ�Ҫ�������⴦��
		ff_g[i] = 1;//���Լ�����Ϊ1
		ff_b[i] = 0;//���Լ�����Ϊ1
		f = g_fu[i].d;
		f11_g = g_fu[f].g;//����
		f11_b = g_fu[f].b;
		hh_g[i] = f11_g * ff_g[i] - f11_b * ff_b[i];
		hh_b[i] = f11_g * ff_b[i] + f11_b * ff_g[i];

		//�����Լ�������Ҫ���м���
		for (j = i + 1; j < lv_wb; j++)
		{
			f12_g = f12_b = 0;
			//�Լ���ǰ����� * �������е�ff
			f = g_fu[j].d;//�Խ�Ԫ�����ڵ������

			//����֮���൱�ڱ���֮ǰ���е��е�ǰ�棩������������
			//�Ӷ�·�п�ʼ���������У����������н��������������У�
			p = g_fu[f].p;
			while (p != f)//�����Լ�˵������ͷ��
			{
				if (g_fu[p].i >= i)
				{
					f11_g = ff_g[g_fu[p].i];//���е�u
					f11_b = ff_b[g_fu[p].i];
					f22_g = g_fu[p].g;//���е�f
					f22_b = g_fu[p].b;

					f12_g += -(f11_g*f22_g - f11_b * f22_b);
					f12_b += -(f11_b*f22_g - f11_g * f22_b);
				}

				p = g_fu[p].p;//�Լ�����һ��
			}
			ff_g[j] = f12_g;
			ff_b[j] = f12_b;

			//����һ��ff��������hh
			f11_g = g_fu[f].g;
			f11_b = g_fu[f].b;
			hh_g[j] = f11_g * f12_g - f11_b * f12_b;
			hh_b[j] = f11_g * f12_b + f11_b * f12_g;
		}

		//zz��Ҫ���������������һ����ʼ
		//����Ҫ������Ϊ��׼��ɨ��ÿһ��
		for (j = lv_wb - 1; j >= 0; j--)
		{
			//�ҵ����еĶԽ�
			f12_g = f12_b = 0;
			k = g_fu[j].d;
			jmn = k + 1;//�Խ�Ԫ�ص���ʼλ��
			jmx = g_fu[j + 1].d;//�����Խ�֮���Ԫ�أ�Ҳ���ǷǶԽ�Ԫ�ص�
			for (m = jmn; m < jmx; m++)
			{
				f11_g = g_fu[m].g;//�ǶԽ�Ԫ��
				f11_b = g_fu[m].b;

				if ((f11_b > -0.000001) && (f11_b < 0.000001))
					f11_b = 0;

				f22_g = zz_g[g_fu[m].j];
				f22_b = zz_b[g_fu[m].j];

				f12_g += (f11_g*f22_g - f11_b * f22_b);
				f12_b += (f11_b*f22_g + f11_g * f22_b);
			}
			zz_g[j] = hh_g[j] - f12_g;
			zz_b[j] = hh_b[j] - f12_b;
		}

		if (type == 1)
		{//����
			fr.push_back(zz_g[i]);
			fx.push_back(zz_b[i]);
		}
		else
		{//����
			fr0.push_back(zz_g[i]);
			fx0.push_back(zz_b[i]);
		}
	}


	delete[] ff_g;
	delete[] ff_b;
	delete[] hh_g;
	delete[] hh_b;
	delete[] zz_g;
	delete[] zz_b;

	return 1;
}

//�������еĽڵ�����������Ķ�·����ֵ
int CSC::Calc3Series()
{
	char  output[180];//���߶εĶ�·�������
	char  output_1[80];//����ӵض�·����
	char  output_2[80];//�����·����
	char  output_3[80];//�����·����
	char  output_4[80];//�����·�ӵص���

	output[0] = 0;
	strcpy(output_1, "-\t-");
	strcpy(output_2, "-\t-");
	strcpy(output_3, "-\t-");
	strcpy(output_4, "-\t-");

	int    i;

	//zz����󣬿��Լ����·��Ķ�·������
//��·��Ľڵ��ѹֵ
	//ÿ���ڵ�ɨ��,�õ�ÿ���ڵ�Ķ�·����
	for (i = 0; i < lv_wb; i++)
	{
		fault->ibs = i;//����ĸ��
		fault->v = 1;//g_wb[i].v;//���ϵ����̬��ѹ
		fault->a = 0;//g_wb[i].a;
		//����
		fault->r = fr[i];//���ϵ㵽��Դ��ĵ�ֵ�迹Zff(1)
		fault->x = fx[i];
	
		if (lld->g_ZeroSerialGrounding == 1)
		{
			//����
			fault->r0 = fr0[i];//���ϵ㵽��Դ��ĵ�ֵ�迹Zff(1)
			fault->x0 = fx0[i];

			CalculateFaultCurrent3(1, output_1);//����
			CalculateFaultCurrent3(4, output_4);//����ӵ�
		}
		CalculateFaultCurrent3(2, output_2);//����
		CalculateFaultCurrent3(3, output_3);//����
		

		//�õ��ýڵ�Ķ�·����ֵ
		sprintf(output, "%s\t%s\t%s\t%s", output_1, output_2,output_3, output_4);
		bs_sc[i] = output;//������ĸ�ߵĶ�·���������� 
	}


	return 1;
}

int CSC::PutResult()
{
	 char line[500];
	 char result_b[200];
	 char result_e[200];
	 char result_t[200];
	 char t1[200];
	 char t2[200];

	int i,ibs,zbs,isec;
	int iout,zout,dead;
	SEC* sec = dsbob2->sec_tb->GetTable();
	XF* xf = NULL;
	if (dsbob2->xf_tb != NULL)
		XF* xf = dsbob2->xf_tb->GetTable();

	for(i = 0; i < lv_wbr; i++)
    {
        ibs = g_wbr[i].ibs;//�Ż���ź�
        zbs = g_wbr[i].zbs;
//SEG_1018153683

		isec = g_wbr[i].logic;//���ﵽ��

		if (g_wbr[i].type == _SEC)
		{
			iout = (sec + isec)->topflags & SEC_IOPEN;
			zout = (sec + isec)->topflags & SEC_ZOPEN;
			dead = iout && zout;

			if (dead)
			{
				continue;
			}

			strcpy(result_b, bs_sc[ibs].c_str());
			strcpy(result_e, bs_sc[zbs].c_str());

			if ((result_b[0] == 0) || (result_e[0] == 0))
			{
				int debug = 1;
			}

			if (g_wbr[i].type == _SEC)
			{
				//��Ҫ������ĩ�ڵ��˳��ʹ�俴������������
				//��������ֻ�з��������������
				//ֻ��ȡ/ǰ��ļ����ַ����бȽ�
				lld->getleftstr(result_b, '/', t1);
				lld->getleftstr(result_e, '/', t2);
				if (atof(t1) < atof(t2))
				{
					strcpy(result_t, result_b);
					strcpy(result_b, result_e);
					strcpy(result_e, result_t);
				}

				sprintf(line, "%s\t%s", result_b, result_e);
				strcpy((sec + isec)->opencblist, line);

				sprintf(line, "# %s\t%s\t%s", (sec + isec)->id, result_b, result_e);
				//���������㷨�����Ҫ������������
				outfile << line << endl;
				//strcpy((sec + isec)->opencblist, line);

				//			scResult.push_back(line);
			}
		}
		if (g_wbr[i].type == _XF)
		{
			iout = (xf + isec)->topflags & XF_IOPEN;
			zout = (xf + isec)->topflags & XF_ZOPEN;
			dead = iout && zout;

			if (dead)
			{
				continue;
			}

			strcpy(result_b, bs_sc[ibs].c_str());
			strcpy(result_e, bs_sc[zbs].c_str());

			if ((result_b[0] == 0) || (result_e[0] == 0))
			{
				int debug = 1;
			}

			if (g_wbr[i].type == _XF)
			{
				//��Ҫ������ĩ�ڵ��˳��ʹ�俴������������
				//��������ֻ�з��������������
				//ֻ��ȡ/ǰ��ļ����ַ����бȽ�
				lld->getleftstr(result_b, '/', t1);
				lld->getleftstr(result_e, '/', t2);
				if (atof(t1) < atof(t2))
				{
					strcpy(result_t, result_b);
					strcpy(result_b, result_e);
					strcpy(result_e, result_t);
				}

				//			if (bs_sc[ibs] < bs_sc[zbs])
				//			{
				//				ixf = ibs;
				//				ibs = zbs;
				//				zbs = ixf;
				//			}
				sprintf(line, "%s\t%s", result_b, result_e);
				strcpy((xf + isec)->ver, line);

							//ixf = g_wbr[i].logic;//���ﵽ��
				sprintf(line, "# %s\t%s\t%s", (xf + isec)->id, result_b, result_e);
				//���������㷨�����Ҫ������������
				outfile << line << endl;
				//strcpy((xf + isec)->opencblist, line);

				//			scResult.push_back(line);
			}
		}
	}

	return 1;
}

int CSC::CalculateFaultCurrent3(int faulttype,char  * output)
{
	int		ibs;
	double	a, b, c, d;
	double  i1, a1, i2, a2, i0, a0;//����
	double  e1, f1, e2, f2, e0, f0;//��ѹ
	double  e, f;
	double  ea, fa, eb, fb, ec, fc;
	//������
	double ia_3, ib_3, ic_3, va_3, vb_3, vc_3;

	//6. calculate fault current
	i1 = 0.0;//�������
	a1 = 0.0;//��������ĸ���
	i2 = 0.0;//�������
	a2 = 0.0;//
	i0 = 0.0;//�������
	a0 = 0.0;
	e1 = 0.0;//�����ѹʵ��
	f1 = 0.0;//�����ѹ�鲿
	e2 = 0.0;//
	f2 = 0.0;//
	e0 = 0.0;//����
	f0 = 0.0;

	a = (fault->v) * cos(fault->a);//���ϵ����̬��ѹ
	b = (fault->v) * sin(fault->a);
	c = fault->r;//���ϵ㵽��Դ��ĵ�ֵ�迹Zff(1)
	d = fault->x;

	switch (faulttype)
	{
		//�ܵĹ�ʽ��
		//Uf(1)=Uf(0)-Zff(1)*If(1)
		//Uf(2)=-Zff(2)*If(2)
		//Uf(0)=-Zff(0)*If(0)
		//Zff(0)=0;Zff(1)=Zff(2);���ϵͳ���У����Ե㲻�ӵػ�������Ȧ�ӵأ�һ��û������
	case 1:  //single phase short to ground	�����·�ӵ�
		//If(1)=If(2)=If(0)��Uf|0|/(Zff(1)+Zff(2)+Zff(0))
		//Uf(1)+Uf(2)+Uf(0)=0;
		c = 2*c + fault->r0;//�����������ȣ������������迹�ۼ�
		d = 2*d + fault->x0;

		i1 = (a * c + b * d) / (c * c + d * d);
		a1 = (-a * d + b * c) / (c * c + d * d);
		i2 = i1;//�������
		a2 = a1;
		i0 = i1;
		a0 = a1;

		//�����ѹ
		c = fault->r;//������裬����=����
		d = fault->x;//����翹
		e2 = -i2 * c + a2 * d;//�����ѹʵ��
		f2 = -i2 * d - a2 * c;//�����ѹ�鲿
		//�����ѹ
		c = fault->r0;
		d = fault->x0;
		e0 = -i0 * c + a0 * d;//�����ѹʵ��
		f0 = -i0 * d - a0 * c;//�����ѹ�鲿
		//�����ѹ
		e1 = -e2 - e0;
		f1 = -f2 - f0;	//����Ƕ�

		break;

	case 2:  //two phase short circuit	�����·����������ȣ�ֱ�Ӽ�
		//If(1)=-If(2);If(0)=0;
		//Uf(1)=Uf(2);Uf(0)=0;
		//If(1)=Uf|0|/(Zff(1)+Zff(2))
		c += c;
		d += d;
		i1 = (a * c + b * d) / (c * c + d * d);
		a1 = (-a * d + b * c) / (c * c + d * d);
		i2 = -i1;
		a2 = -a1;
		c = fault->r;
		d = fault->x;
		e1 = i1 * c - a1 * d;//�����ѹʵ��
		f1 = i1 * d + a1 * c;//�����ѹ�鲿
		e2 = e1;//����͸���ĵ�ѹ���
		f2 = f1;
		break;

	case 3:	 //three phase short circuit	
		//�����·��ֻ���������
		//If(1)=Uf|0|/Zff(1); If(2)=If(0)=0; Uf(2)=Uf(0)=0;
		i1 = (a * c + b * d) / (c * c + d * d);//�����·�����ķ�ֵ
		a1 = (-a * d + b * c) / (c * c + d * d);//�����·���������
		break;

	case 4:  //two phase short to ground	����ӵض�·
		//If(1)+If(2)+If(0)=0;
		//Uf(1)=Uf(2)=Uf(0);
		//If(1)=Uf|0|/(Zff(1)+(Zff(2)*Zff(0)/(Zff(2)+Zff(0)))
		//If(2)=-If(1)*Zff(0)/(Zff(2)+Zff(0));
		//If(0)=-If(1)-If(2);
		
		//������������迹
		e = fault->r0;
		f = fault->x0;

		e1 = c * e - d * f;
		f1 = d * e + c * f;
		e2 = c + e;
		f2 = d + f;

		c += (e1* e2 - e1 * f2) / (e2 * e2 + f2 * f2);
		d += (e1* f2 + e1 * e2) / (e2 * e2 + f2 * f2);

		i1 = (a * c - b * d) / (c * c + d * d);
		a1 = (a * d + b * c) / (c * c + d * d);

		c = fault->r;//ǰ���Ѿ��ı���c��ֵ����Ҫ����ָ��
		d = fault->x;
		
		e1 = -i1*e + a1*f;
		f1 = i1*f + a1*e;
		e2 = c + e;
		f2 = d + f;

		i2 = (e1* e2 - e1 * f2) / (e2 * e2 + f2 * f2);
		a2 = (e1* f2 + e1 * e2) / (e2 * e2 + f2 * f2);

		i0 = -i1-i2;
		a0 = -a1-a2;

		//�����ѹ
		e2 = -i2 * c + a2 * d;//�����ѹʵ��
		f2 = -i2 * d - a2 * c;//�����ѹ�鲿
		e0 = e1 = e2;
		f0 = f1 = f2;	//����Ƕ�

		break;
	}

	fault->i1 = float(i1);//��·����������
	fault->a1 = float(a1);//�Ƕ�
	fault->i2 = float(i2);//����
	fault->a2 = float(a2);
	fault->i0 = float(i0);//����
	fault->a0 = float(a0);

	//bus voltage at fault location
	ibs = fault->ibs;
	g_wb[ibs].v1 = e1;//�����ѹ
	g_wb[ibs].a1 = f1;//����Ƕ�
	g_wb[ibs].v2 = e2;
	g_wb[ibs].a2 = f2;	
	g_wb[ibs].v0 = e0;
	g_wb[ibs].a0 = f0;

	//2. fault current�����ϵ�ļ�����
	//����������������ѹ
	//3�������ĵ���
	e1 = fault->i1;
	f1 = fault->a1;
	e2 = fault->i2;
	f2 = fault->a2;
	e0 = fault->i0;
	f0 = fault->a0;

	//���ϵ�ĵ�ѹֵ
	//e1 = 1 - m_dataStruct.CK1[m_dataStruct.NF0] * ai1;
	//e2 = -m_dataStruct.CK2[m_dataStruct.NF0] * ai2;
	//e0 = -m_dataStruct.CK0[m_dataStruct.NF0] * ai0;

	//r1 = e0 + e1 + e2;
	//r2 = e0 - 0.5 * e1 - 0.5 * e2;
	//x2 = -b * e1 + b * e2;

	//r1 *= 10;//������ֵ
	//r3 = 0;
	//if (fabs(r2) > 1e-10 || fabs(x2) > 1e-10)
	//{
	//	r3 = 10 * sqrt(r2 * r2 + x2 * x2);
	//}
	//r1 = fabs(r1);
	//e1 = r1;
	//e0 = r3;

	////���ϵ�ĵ���ֵ
	//r1 = ai0 + ai1 + ai2;//���������ĵ������
	//r2 = ai0 - 0.5 * ai1 - 0.5 * ai2;
	//x2 = -b * ai1 + b * ai2;

	//r1 *= 10 / 1.732;//������ֵ
	//r3 = 0;
	//if (fabs(r2) > 1e-10 || fabs(x2) > 1e-10)
	//{
	//	r3 = (10 / 1.732) * sqrt(r2 * r2 + x2 * x2);
	//}

	//�����࣬�����͵�ѹ��һ�����㹫ʽ
	//Ia=Ia1+Ia2+Ia0
	//Ib=a^2*Ia1+a*Ia2+Ia0
	//Ic=a*Ia1+a^2*Ia2+Ia0
	//a=-0.5+j0.8660254;a^2=-0.5-j0.8600254
	ea = e1 + e2 + e0;//A�����Ϊ���������ĵ���֮��
	fa = f1 + f2 + f0;
	//B�����
	eb = -0.5 * e1 + 0.8660254 * f1 - 0.5 * e2 - 0.8660254 * f2 + e0;
	fb = -0.5 * f1 - 0.8660254 * e1 - 0.5 * f2 + 0.8660254 * e2 + f0;
	//C�����
	ec = -0.5 * e1 - 0.8660254 * f1 - 0.5 * e2 + 0.8660254 * f2 + e0;
	fc = -0.5 * f1 + 0.8660254 * e1 - 0.5 * f2 - 0.8660254 * e2 + f0;

	//������ģ�ͽǶ�
	e = sqrt(e1 * e1 + f1 * f1);//�����·������ģ
	f = atan2(e1, f1) * 57.2957795;//�����·���������
	e1 = e;
	f1 = f;
	e = sqrt(e2 * e2 + f2 * f2);
	f = atan2(e2, f2) * 57.2957795;
	e2 = e;
	f2 = f;
	//���ģ�ͽǶ�
	e = sqrt(ea * ea + fa * fa);
	f = atan2(ea, fa) * 57.2957795;
	ea = e;
	fa = f;
	e = sqrt(eb * eb + fb * fb);
	f = atan2(eb, fb) * 57.2957795;
	eb = e;
	fb = f;
	e = sqrt(ec * ec + fc * fc);
	f = atan2(ec, fc) * 57.2957795;
	ec = e;
	fc = f;

	fault->i1 = float(e1);
	fault->a1 = float(f1);
	fault->i2 = float(e2);
	fault->a2 = float(f2);
	fault->i0 = float(e0);
	fault->a0 = float(f0);

	fault->ia = float(ea);//A�����
	fault->aa = float(fa);
	fault->ib = float(eb);//B�����
	fault->ab = float(fb);
	fault->ic = float(ec);//C�����
	fault->ac = float(fc);

	//���ϵ�Ķ�·�������������
	ia_3 = (10 / 1.732) * fault->ia;
	ib_3 = (10 / 1.732) * fault->ib;
	ic_3 = (10 / 1.732) * fault->ic;

	//��������ĵ�ѹֵ
	e1 = g_wb[ibs].v1;//�����ѹ
	f1 = g_wb[ibs].a1;//����Ƕ�
	e2 = g_wb[ibs].v2;
	f2 = g_wb[ibs].a2;
	e0 = g_wb[ibs].v0;
	f0 = g_wb[ibs].a0;

	//2. fault current�����ϵ�ļ�����
	//����������������ѹ
	//�����࣬�����͵�ѹ��һ�����㹫ʽ
	//Ia=Ia1+Ia2+Ia0
	//Ib=a^2*Ia1+a*Ia2+Ia0
	//Ic=a*Ia1+a^2*Ia2+Ia0
	ea = e1 + e2 + e0;//A�����Ϊ���������ĵ���֮��
	fa = f1 + f2 + f0;
	//B�����
	eb = -0.5 * e1 + 0.8660254 * f1 - 0.5 * e2 - 0.8660254 * f2 + e0;
	fb = -0.5 * f1 - 0.8660254 * e1 - 0.5 * f2 + 0.8660254 * e2 + f0;
	//C�����
	ec = -0.5 * e1 - 0.8660254 * f1 - 0.5 * e2 + 0.8660254 * f2 + e0;
	fc = -0.5 * f1 + 0.8660254 * e1 - 0.5 * f2 - 0.8660254 * e2 + f0;

	//������ģ�ͽǶ�
	e = sqrt(e1 * e1 + f1 * f1);//�����·������ģ
	f = atan2(e1, f1) * 57.2957795;//�����·���������
	e1 = e;
	f1 = f;
	e = sqrt(e2 * e2 + f2 * f2);
	f = atan2(e2, f2) * 57.2957795;
	e2 = e;
	f2 = f;
	//���ģ�ͽǶ�
	e = sqrt(ea * ea + fa * fa);
	f = atan2(ea, fa) * 57.2957795;
	ea = e;
	fa = f;
	e = sqrt(eb * eb + fb * fb);
	f = atan2(eb, fb) * 57.2957795;
	eb = e;
	fb = f;
	e = sqrt(ec * ec + fc * fc);
	f = atan2(ec, fc) * 57.2957795;
	ec = e;
	fc = f;

	//���ϵ�Ķ�·��ѹ���������
	va_3 = 10 * ea;
	vb_3 = 10 * eb;
	vc_3 = 10 * ec;

	sprintf(output, "i:%02.3f/%02.3f/%02.3f\tv:%02.3f/%02.3f/%02.3f", ia_3, ib_3, ic_3, va_3, vb_3, vc_3);

	return 1;
}





