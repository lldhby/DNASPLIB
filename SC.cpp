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

//	PWRFLOW(dsbob2);//潮流计算初始化

	g_fu		= new G_FU[mx_fu];
	lv_fu		= 0;

	fault		= new FAULT[1];
}

CSC::~CSC()
{
	delete[] g_fu;
	delete[] fault;
}

//执行短路电流计算
//需要计算完整的潮流，以便得到故障点到电源点的等效阻抗
//如果不计算潮流的话，需要知道全部的矩阵
int CSC::OnMathSC(char * Efile_name,int type)
{
	char str[500];
	//lldapi lld_t;
	CLLD lld_t;
	//get island number
	int lv_island = dsbob2->island_tb->GetCount();

	outfile.open(Efile_name, ios::out);
	outfile << "<!System= Version=1.0 Code=UTF-8 Data=!>" << endl;

	//输出三相短路计算结果,输出首端和末端的短路电流值,单相，两相，三相，两相接地
	outfile << "<sec::sc name=" << "\"短路电流计算结果\"" << ">" << endl;
	outfile << "@\tsec_id\ti_b1_A/B/C_phase(kA)\tu_b1_A/B/C_phase(kV)\t"
		<< "i_b2_A/B/C_phase(kA)\tu_b2_A/B/C_phase(kV)\t" 
		<< "i_b3_A/B/C_phase(kA)\tu_b3_A/B/C_phase(kV)\t"
		<< "i_b21_A/B/C_phase(kA)\tu_b21_A/B/C_phase(kV)\t"
		<< "i_e1_A/B/C_phase(kA)\tu_e1_A/B/C_phase(kV)\t"
		<< "i_e2_A/B/C_phase(kA)\tu_e2_A/B/C_phase(kV)\t"
		<< "i_e3_A/B/C_phase(kA)\tu_e3_A/B/C_phase(kV)\t"
		<< "i_e21_A/B/C_phase(kA)\tu_e21_A/B/C_phase(kV)"
		<< endl;
	outfile << "/@\t馈线段id\t(首)单相短路电流_A相/B相/C相(kA)\t(首)单相短路电压_A相/B相/C相(kV)\t"
		<< "(首)两相短路电流_A相/B相/C相(kA)\t(首)两相短路电压_A相/B相/C相(kV)\t"
		<< "(首)三相短路电流_A相/B相/C相(kA)\t(首)三相短路电压_A相/B相/C相(kV)\t"
		<< "(首)两相短路接地电流_A相/B相/C相(kA)\t(首)两相短路接地电压_A相/B相/C相(kV)\t"
		<< "(末)单相短路电流_A相/B相/C相(kA)\t(末)单相短路电压_A相/B相/C相(kV)\t"
		<< "(末)两相短路电流_A相/B相/C相(kA)\t(末)两相短路电压_A相/B相/C相(kV)\t"
		<< "(末)三相短路电流_A相/B相/C相(kA)\t(末)三相短路电压_A相/B相/C相(kV)\t"
		<< "(末)两相短路接地电流_A相/B相/C相(kA)\t(末)两相短路接地电压_A相/B相/C相(kV)"
		<< endl;

	//execute calculations 对每一个岛进行计算
	for(g_island = 0; g_island < lv_island; g_island++)
	{
		//g_island = 0;
		//1.先计算潮流
//				 so_id[80];
//				int soid=dsbob2->island_tb->GetDataByLog(g_island)->refso;
//				strcpy(so_id , dsbob2->so_tb->GetDataByLog(soid)->id);

		//只对本岛内的数据进行初始化
		//initialize working arrays		1.对母线进行初始化
		InitWorkingArray();

		//read network data		2.读入网络数据
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
			//零序导纳矩阵
			if (FormAdmittance0() == PWRFLOW_FAIL)
			{
				return PWRFLOW_FAIL;
			}
			//做零序的因子表
			if (FormFactorTable() == PWRFLOW_FAIL)
			{
				return PWRFLOW_FAIL;
			}
			SolveStatic(0);
		}

		//2.再计算短路电流
		//form admittance matrix形成导纳矩阵
		if(FormAdmittance() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

		//因子表正序和零序都得做
		if (FormFactorTable() == PWRFLOW_FAIL)
		{
			return PWRFLOW_FAIL;
		}

		SolveStatic(1);

		//计算三个虚网的值
		Calc3Series();

		//输出本岛的计算结果
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

//1.假定对角元素均大于0，将其存入专门的数组
//2.非对角元素的存储到一个数组g_fu里面
//3.一个导纳矩阵对应一个因子矩阵
int CSC::FormFactorTable()
{
    int    i, k, k_f/*因子表的对角位置*/, m, n, f/*因子表计数*/,p/*列的链表*/;
    int    jmx, jmn;
    double yg, yb, g, b;
	double d11_g,d11_b,d22_g,d22_b,u12_g,u12_b;//对角元素和非对角元素
	int i1;
	int find;
	int irow;//对角数组对应的行
	double aij1,aij2;//导纳矩阵里面关联列的元素

	//存对角元素的实部和虚部，对角元素是全序列
	double* GD = new double[lv_wb];//只有对角才可以这么定义，因为对角元素个数是确定的
	double* BD = new double[lv_wb];

	double* GJ = new double[lv_wb];//计算主列
	double* BJ = new double[lv_wb];
	double* GK = new double[lv_wb];//参与计算列
	double* BK = new double[lv_wb];

    //initialize upper trangular matrix
	//因为非对角元素的个数不能确定，所以只能取一个最大值
    for(i = 0; i < mx_fu; i++)
    {
        g_fu[i].d = 0;//对角元素位置，对角准备放GD+jBD的倒数
        g_fu[i].i = 0;//上三角的行
        g_fu[i].j = 0;//上三角的列
        g_fu[i].p = 0;//列对应的链表,通过该链表把所有的该列元素找到
        g_fu[i].g = 0.0;//因子的实部
        g_fu[i].b = 0.0;//因子的虚部
    }
    lv_fu = 0;

	//如果已经知道导纳矩阵，实际上可以直接用公式进行计算
	//dii = aii - (u1i^2 * d11 + u2i^2 * d22 + u3i^2 * d33 + ... + u(i-1)i^2 * d(i-1)(i-1))
	//uij = (aij - (u1i*u1j*d11 + u2i*u2j*d22 + u3i*u3j*d33 + ... +u(i-1)i*u(i-1)j*d(i-1)(i-1))/dii 
	//对每个节点进行处理，从第一行开始处理

	f = 0; //因子表元素从0开始计数
	//jf = 0;//非对角元素个数
    for(i = 0; i < lv_wb; i++)
    {
		//先做位置，再做内容

		//第一步，做对角元素
		k = g_yu[i].d;//节点的对角,导纳矩阵的元素

		//对角元素一定会有
		//做位置
 		g_fu[i].d = f;//上一个对角的位置+上一行的非对角元素个数，可以不用
		k_f = f;//因子表的对角位置，因子表的对角位置和导纳矩阵有可能不一样，因为可能有新的注入
		g_fu[f].i = i;
		g_fu[f].j = i;//对角的行和列是一样的
		g_fu[f].p = f;//先默认自己为链表起始

		//修改链表
		//上一个对应的列，循环前面一行的所有元素
		//需要对因子数组从后面向前查找，找上一个,找到一个就可以了
		for(m = f-1; m >=0 ; m--)
		{
			if (g_fu[m].j == i)//找到了前面的那一列，修改链表
			{
				n = g_fu[m].p;
				g_fu[f].p = n;//上一个
				g_fu[m].p = f;

				break;//只要找到一个就可以了，如果一个都没有，就不要修改链表
			}
		}

		//做内容
		//自己的内容
		yg = g_yu[k].g;//对角的实部
		yb = g_yu[k].b;//对角的虚部
		
		//实际上以列为线索就可以了，对角可以默认一直都存在
		d22_g = d22_b = 0;
		p = g_fu[k_f].p;//自己的下一个，实际是最头上的那个
		while(p != k_f)//等于自己说明链表到头了
        {
            g = g_fu[p].g;
            b = g_fu[p].b;
            irow = g_fu[p].i;//列所在的行
			
			//从对角数组里面取出这个行的对角元素
			d11_g = GD[irow];
			d11_b = BD[irow];

			//用对角元素和所在的非对角元素进行运算
			//(g+jb)^2 * (d11_g+jd11_b)
			d22_g += (g*g - b*b) * d11_g - 2*g*b*d11_b;
			d22_b += (g*g - b*b) * d11_b + 2*g*b*d11_g;

			p = g_fu[p].p;
        }
		d22_g = yg - d22_g; 
		d22_b = yb - d22_b;
		//给对角数组赋值
		GD[i] = d22_g;
		BD[i] = d22_b;
		//给因子表赋值
		g_fu[f].g =  d22_g / (d22_g*d22_g + d22_b*d22_b); //将倒数存入因子表
		g_fu[f].b = -d22_b / (d22_g*d22_g + d22_b*d22_b); //因子数累加

		//本列(以行为固定，也就是对角元素对应的列)
		//1.先把自己对应的两列的前面的链表都找出来，对应的对角找出来
		for (m=0; m<i; m++)//只需把本行之前的数组清空就可以了
		{
			GJ[m]=BJ[m]=0;
		}
		p = g_fu[k_f].p;//自己的上一个
		while(p != k_f)//等于自己说明链表到头了
		{
			g = g_fu[p].g;
			b = g_fu[p].b;
			
			GJ[g_fu[p].i] = g;
			BJ[g_fu[p].i] = b;

			p = g_fu[p].p;//自己的上一个
		}
		f++;

//第二步，做非对角元素
		//关联列（因子表元素对应的列，准备写入的列）
		//无论本列元素是否为0，都需要维护链表，因为前面可能有元素导致新的注入
		for(i1 = i+1; i1 < lv_wb; i1++)//第1行开始，一直到本行i
		{
			for (m=0; m<i; m++)//只需把本行之前的数组清空就可以了
			{
				GK[m]=BK[m]=0;
			}

			//从导纳矩阵里面把关联列的元素拿到(自己的列)
			aij1 = aij2 = 0;
			jmn = k + 1;//对角元素的起始位置
			jmx = g_yu[i+1].d;//两个对角之间的元素，也就是非对角元素的
			for(m = jmn; m < jmx; m++)
			{
				if (i1 == g_yu[m].j)//导纳矩阵里面有列，说明不为0
				{
					aij1 = g_yu[m].g;
					aij2 = g_yu[m].b;

					break;
				}
			}	
			
			//考虑新的注入元
			//本列为空，但它的前面的列有元素，产生新的注入
			find=-1;
			for(m = f-1; m >=0 ; m--)
			{
				if (g_fu[m].j == i1)//找到了前面的那一列，修改链表
				{
					//表示和上一个列接上了
					find = m; //将n保留下来备用，后面用来维护链表
					g = g_fu[m].g;
					b = g_fu[m].b;
					
					GK[g_fu[m].i] = g;
					BK[g_fu[m].i] = b;

					p = g_fu[m].p;//自己的上一个
					while(p != m)//等于自己说明链表到头了
					{
						g = g_fu[p].g;
						b = g_fu[p].b;
						
						GK[g_fu[p].i] = g;
						BK[g_fu[p].i] = b;

						p = g_fu[p].p;//自己的上一个
					}
					break;//找到一个就可以了
				}
			}
		
			//一个关联列做完后，写到临时结果表里面
			u12_g=u12_b=0;
			for (m=0; m<i; m++)
			{//三个列相乘得到(GD+jBD)*(GJ+jBJ)*(GK+jBK)
				u12_g += GD[m]*GJ[m]*GK[m] - GD[m]*BJ[m]*BK[m] - BD[m]*GJ[m]*BK[m] - BD[m]*BJ[m]*GK[m];
				u12_b += GD[m]*GJ[m]*BK[m] + GD[m]*BJ[m]*GK[m] + BD[m]*GJ[m]*GK[m] - BD[m]*BJ[m]*BK[m];
			}

			//( (aij1+jaij2) - (u12_g+ju12_b) )/ (d22_g + jd22_b)
			g = ( (aij1 - u12_g)*d22_g + (aij2 - u12_b)*d22_b ) / (d22_g*d22_g + d22_b*d22_b);
			b = ( -(aij1 - u12_g)*d22_b + (aij2 - u12_b)*d22_g ) / (d22_g*d22_g + d22_b*d22_b);
			
			if ( (g != 0) || (b != 0) )
			{
				//不为0，需要维护列链表
				if (find != -1)
				{
					n = g_fu[find].p;
					g_fu[f].p = n;//上一个
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

//矩阵求逆,三个序网都可以调用，ibs为短路点的计算母线编号
//通过逆阵求取任意给定节点的阻抗值,假设要求节点k的短路电流
//(1)求f
//fi=0; i<k
//fi=1; i==k;
//假设短路点为2，f3=-u23*f2,f4=-u24*f2-u34*f3,f5=-u25*f2-u35*f3-u45*f4,f6=-u26*f2-u36f3-...-f56*f5;

//(2)求h
//hi=fi/dii;
//(3)求Z 从最后的那个h开始向前进行推

int CSC::SolveStatic(int type)
{
	int    i, j, f, m, k, p/*列的链表*/;
	int    jmx, jmn;
	double f11_g, f11_b, f22_g, f22_b, f12_g, f12_b;//对角元素和非对角元素

	double* ff_g = new double[lv_wb];
	double* ff_b = new double[lv_wb];
	double* hh_g = new double[lv_wb];
	double* hh_b = new double[lv_wb];
	double* zz_g = new double[lv_wb];
	double* zz_b = new double[lv_wb];

	if (type == 1)
	{//正序
		fr.clear();
		fx.clear();
	}
	else
	{//零序
		fr0.clear();
		fx0.clear();
	}

	//每个节点扫描,得到每个节点的短路电流
	//第i个点发生故障,相当于做第i列
	for (i = 0; i < lv_wb; i++)
	{
		//初始化
		for (m = 0; m < lv_wb; m++)
		{
			ff_g[m] = ff_b[m] = 0;//将所有的设为0
			hh_g[m] = hh_b[m] = 0;//将所有的设为0
			zz_g[m] = zz_b[m] = 0;//将所有的设为0
		}

		//等于自己要进行特殊处理
		ff_g[i] = 1;//将自己的列为1
		ff_b[i] = 0;//将自己的列为1
		f = g_fu[i].d;
		f11_g = g_fu[f].g;//本行
		f11_b = g_fu[f].b;
		hh_g[i] = f11_g * ff_g[i] - f11_b * ff_b[i];
		hh_b[i] = f11_g * ff_b[i] + f11_b * ff_g[i];

		//大于自己的列需要进行计算
		for (j = i + 1; j < lv_wb; j++)
		{
			f12_g = f12_b = 0;
			//自己的前面的列 * 列所在行的ff
			f = g_fu[j].d;//对角元素所在的物理号

			//本列之后（相当于本行之前所有的行的前面），不包括本列
			//从短路行开始（包括本行），到计算行结束（不包括本行）
			p = g_fu[f].p;
			while (p != f)//等于自己说明链表到头了
			{
				if (g_fu[p].i >= i)
				{
					f11_g = ff_g[g_fu[p].i];//本列的u
					f11_b = ff_b[g_fu[p].i];
					f22_g = g_fu[p].g;//本行的f
					f22_b = g_fu[p].b;

					f12_g += -(f11_g*f22_g - f11_b * f22_b);
					f12_b += -(f11_b*f22_g - f11_g * f22_b);
				}

				p = g_fu[p].p;//自己的上一个
			}
			ff_g[j] = f12_g;
			ff_b[j] = f12_b;

			//做完一个ff，马上做hh
			f11_g = g_fu[f].g;
			f11_b = g_fu[f].b;
			hh_g[j] = f11_g * f12_g - f11_b * f12_b;
			hh_b[j] = f11_g * f12_b + f11_b * f12_g;
		}

		//zz需要反过来做，从最后一个开始
		//以需要做的行为基准，扫描每一列
		for (j = lv_wb - 1; j >= 0; j--)
		{
			//找到该行的对角
			f12_g = f12_b = 0;
			k = g_fu[j].d;
			jmn = k + 1;//对角元素的起始位置
			jmx = g_fu[j + 1].d;//两个对角之间的元素，也就是非对角元素的
			for (m = jmn; m < jmx; m++)
			{
				f11_g = g_fu[m].g;//非对角元素
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
		{//正序
			fr.push_back(zz_g[i]);
			fx.push_back(zz_b[i]);
		}
		else
		{//零序
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

//计算所有的节点的三个序网的短路电流值
int CSC::Calc3Series()
{
	char  output[180];//馈线段的短路电流输出
	char  output_1[80];//单相接地短路电流
	char  output_2[80];//两相短路电流
	char  output_3[80];//三相短路电流
	char  output_4[80];//两相短路接地电流

	output[0] = 0;
	strcpy(output_1, "-\t-");
	strcpy(output_2, "-\t-");
	strcpy(output_3, "-\t-");
	strcpy(output_4, "-\t-");

	int    i;

	//zz做完后，可以计算短路点的短路电流了
//短路点的节点电压值
	//每个节点扫描,得到每个节点的短路电流
	for (i = 0; i < lv_wb; i++)
	{
		fault->ibs = i;//计算母线
		fault->v = 1;//g_wb[i].v;//故障点的稳态电压
		fault->a = 0;//g_wb[i].a;
		//正序
		fault->r = fr[i];//故障点到电源点的等值阻抗Zff(1)
		fault->x = fx[i];
	
		if (lld->g_ZeroSerialGrounding == 1)
		{
			//零序
			fault->r0 = fr0[i];//故障点到电源点的等值阻抗Zff(1)
			fault->x0 = fx0[i];

			CalculateFaultCurrent3(1, output_1);//单相
			CalculateFaultCurrent3(4, output_4);//两相接地
		}
		CalculateFaultCurrent3(2, output_2);//两相
		CalculateFaultCurrent3(3, output_3);//三相
		

		//得到该节点的短路电流值
		sprintf(output, "%s\t%s\t%s\t%s", output_1, output_2,output_3, output_4);
		bs_sc[i] = output;//将计算母线的短路电流存起来 
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
        ibs = g_wbr[i].ibs;//优化编号后
        zbs = g_wbr[i].zbs;
//SEG_1018153683

		isec = g_wbr[i].logic;//由里到外

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
				//需要调整首末节点的顺序，使其看起来更象配网
				//辐射网，只有辐射网有这个规律
				//只能取/前面的几个字符进行比较
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
				//无论哪种算法，最后都要到这里来处理
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
				//需要调整首末节点的顺序，使其看起来更象配网
				//辐射网，只有辐射网有这个规律
				//只能取/前面的几个字符进行比较
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

							//ixf = g_wbr[i].logic;//由里到外
				sprintf(line, "# %s\t%s\t%s", (xf + isec)->id, result_b, result_e);
				//无论哪种算法，最后都要到这里来处理
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
	double  i1, a1, i2, a2, i0, a0;//电流
	double  e1, f1, e2, f2, e0, f0;//电压
	double  e, f;
	double  ea, fa, eb, fb, ec, fc;
	//结果输出
	double ia_3, ib_3, ic_3, va_3, vb_3, vc_3;

	//6. calculate fault current
	i1 = 0.0;//正序电流
	a1 = 0.0;//正序电流的复角
	i2 = 0.0;//负序电流
	a2 = 0.0;//
	i0 = 0.0;//零序电流
	a0 = 0.0;
	e1 = 0.0;//正序电压实部
	f1 = 0.0;//正序电压虚部
	e2 = 0.0;//
	f2 = 0.0;//
	e0 = 0.0;//零序
	f0 = 0.0;

	a = (fault->v) * cos(fault->a);//故障点的稳态电压
	b = (fault->v) * sin(fault->a);
	c = fault->r;//故障点到电源点的等值阻抗Zff(1)
	d = fault->x;

	switch (faulttype)
	{
		//总的公式：
		//Uf(1)=Uf(0)-Zff(1)*If(1)
		//Uf(2)=-Zff(2)*If(2)
		//Uf(0)=-Zff(0)*If(0)
		//Zff(0)=0;Zff(1)=Zff(2);配电系统特有，中性点不接地或消弧线圈接地，一般没有零序
	case 1:  //single phase short to ground	单相短路接地
		//If(1)=If(2)=If(0)；Uf|0|/(Zff(1)+Zff(2)+Zff(0))
		//Uf(1)+Uf(2)+Uf(0)=0;
		c = 2*c + fault->r0;//负序和正序相等，三个序网的阻抗累加
		d = 2*d + fault->x0;

		i1 = (a * c + b * d) / (c * c + d * d);
		a1 = (-a * d + b * c) / (c * c + d * d);
		i2 = i1;//负序电流
		a2 = a1;
		i0 = i1;
		a0 = a1;

		//负序电压
		c = fault->r;//负序电阻，正序=负序
		d = fault->x;//负序电抗
		e2 = -i2 * c + a2 * d;//负序电压实部
		f2 = -i2 * d - a2 * c;//负序电压虚部
		//零序电压
		c = fault->r0;
		d = fault->x0;
		e0 = -i0 * c + a0 * d;//正序电压实部
		f0 = -i0 * d - a0 * c;//正序电压虚部
		//正序电压
		e1 = -e2 - e0;
		f1 = -f2 - f0;	//正序角度

		break;

	case 2:  //two phase short circuit	两相短路，正负序相等，直接加
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
		e1 = i1 * c - a1 * d;//正序电压实部
		f1 = i1 * d + a1 * c;//正序电压虚部
		e2 = e1;//正序和负序的电压相等
		f2 = f1;
		break;

	case 3:	 //three phase short circuit	
		//三相短路，只有正序存在
		//If(1)=Uf|0|/Zff(1); If(2)=If(0)=0; Uf(2)=Uf(0)=0;
		i1 = (a * c + b * d) / (c * c + d * d);//正序短路电流的幅值
		a1 = (-a * d + b * c) / (c * c + d * d);//正序短路电流的相角
		break;

	case 4:  //two phase short to ground	两相接地短路
		//If(1)+If(2)+If(0)=0;
		//Uf(1)=Uf(2)=Uf(0);
		//If(1)=Uf|0|/(Zff(1)+(Zff(2)*Zff(0)/(Zff(2)+Zff(0)))
		//If(2)=-If(1)*Zff(0)/(Zff(2)+Zff(0));
		//If(0)=-If(1)-If(2);
		
		//先求负序和零序阻抗
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

		c = fault->r;//前面已经改变了c的值，需要重新指定
		d = fault->x;
		
		e1 = -i1*e + a1*f;
		f1 = i1*f + a1*e;
		e2 = c + e;
		f2 = d + f;

		i2 = (e1* e2 - e1 * f2) / (e2 * e2 + f2 * f2);
		a2 = (e1* f2 + e1 * e2) / (e2 * e2 + f2 * f2);

		i0 = -i1-i2;
		a0 = -a1-a2;

		//负序电压
		e2 = -i2 * c + a2 * d;//正序电压实部
		f2 = -i2 * d - a2 * c;//正序电压虚部
		e0 = e1 = e2;
		f0 = f1 = f2;	//正序角度

		break;
	}

	fault->i1 = float(i1);//短路点的正序电流
	fault->a1 = float(a1);//角度
	fault->i2 = float(i2);//负序
	fault->a2 = float(a2);
	fault->i0 = float(i0);//零序
	fault->a0 = float(a0);

	//bus voltage at fault location
	ibs = fault->ibs;
	g_wb[ibs].v1 = e1;//正序电压
	g_wb[ibs].a1 = f1;//正序角度
	g_wb[ibs].v2 = e2;
	g_wb[ibs].a2 = f2;	
	g_wb[ibs].v0 = e0;
	g_wb[ibs].a0 = f0;

	//2. fault current，故障点的计算结果
	//将序变成相电流、相电压
	//3个序网的电流
	e1 = fault->i1;
	f1 = fault->a1;
	e2 = fault->i2;
	f2 = fault->a2;
	e0 = fault->i0;
	f0 = fault->a0;

	//故障点的电压值
	//e1 = 1 - m_dataStruct.CK1[m_dataStruct.NF0] * ai1;
	//e2 = -m_dataStruct.CK2[m_dataStruct.NF0] * ai2;
	//e0 = -m_dataStruct.CK0[m_dataStruct.NF0] * ai0;

	//r1 = e0 + e1 + e2;
	//r2 = e0 - 0.5 * e1 - 0.5 * e2;
	//x2 = -b * e1 + b * e2;

	//r1 *= 10;//变有名值
	//r3 = 0;
	//if (fabs(r2) > 1e-10 || fabs(x2) > 1e-10)
	//{
	//	r3 = 10 * sqrt(r2 * r2 + x2 * x2);
	//}
	//r1 = fabs(r1);
	//e1 = r1;
	//e0 = r3;

	////故障点的电流值
	//r1 = ai0 + ai1 + ai2;//三个序网的电流相加
	//r2 = ai0 - 0.5 * ai1 - 0.5 * ai2;
	//x2 = -b * ai1 + b * ai2;

	//r1 *= 10 / 1.732;//变有名值
	//r3 = 0;
	//if (fabs(r2) > 1e-10 || fabs(x2) > 1e-10)
	//{
	//	r3 = (10 / 1.732) * sqrt(r2 * r2 + x2 * x2);
	//}

	//由序到相，电流和电压是一样计算公式
	//Ia=Ia1+Ia2+Ia0
	//Ib=a^2*Ia1+a*Ia2+Ia0
	//Ic=a*Ia1+a^2*Ia2+Ia0
	//a=-0.5+j0.8660254;a^2=-0.5-j0.8600254
	ea = e1 + e2 + e0;//A相电流为三个序网的电流之和
	fa = f1 + f2 + f0;
	//B相电流
	eb = -0.5 * e1 + 0.8660254 * f1 - 0.5 * e2 - 0.8660254 * f2 + e0;
	fb = -0.5 * f1 - 0.8660254 * e1 - 0.5 * f2 + 0.8660254 * e2 + f0;
	//C相电流
	ec = -0.5 * e1 - 0.8660254 * f1 - 0.5 * e2 + 0.8660254 * f2 + e0;
	fc = -0.5 * f1 + 0.8660254 * e1 - 0.5 * f2 - 0.8660254 * e2 + f0;

	//序网的模和角度
	e = sqrt(e1 * e1 + f1 * f1);//正序短路电流的模
	f = atan2(e1, f1) * 57.2957795;//正序短路电流的相角
	e1 = e;
	f1 = f;
	e = sqrt(e2 * e2 + f2 * f2);
	f = atan2(e2, f2) * 57.2957795;
	e2 = e;
	f2 = f;
	//相的模和角度
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

	fault->ia = float(ea);//A相电流
	fault->aa = float(fa);
	fault->ib = float(eb);//B相电流
	fault->ab = float(fb);
	fault->ic = float(ec);//C相电流
	fault->ac = float(fc);

	//故障点的短路电流计算出来了
	ia_3 = (10 / 1.732) * fault->ia;
	ib_3 = (10 / 1.732) * fault->ib;
	ic_3 = (10 / 1.732) * fault->ic;

	//计算三相的电压值
	e1 = g_wb[ibs].v1;//正序电压
	f1 = g_wb[ibs].a1;//正序角度
	e2 = g_wb[ibs].v2;
	f2 = g_wb[ibs].a2;
	e0 = g_wb[ibs].v0;
	f0 = g_wb[ibs].a0;

	//2. fault current，故障点的计算结果
	//将序变成相电流、相电压
	//由序到相，电流和电压是一样计算公式
	//Ia=Ia1+Ia2+Ia0
	//Ib=a^2*Ia1+a*Ia2+Ia0
	//Ic=a*Ia1+a^2*Ia2+Ia0
	ea = e1 + e2 + e0;//A相电流为三个序网的电流之和
	fa = f1 + f2 + f0;
	//B相电流
	eb = -0.5 * e1 + 0.8660254 * f1 - 0.5 * e2 - 0.8660254 * f2 + e0;
	fb = -0.5 * f1 - 0.8660254 * e1 - 0.5 * f2 + 0.8660254 * e2 + f0;
	//C相电流
	ec = -0.5 * e1 - 0.8660254 * f1 - 0.5 * e2 + 0.8660254 * f2 + e0;
	fc = -0.5 * f1 + 0.8660254 * e1 - 0.5 * f2 - 0.8660254 * e2 + f0;

	//序网的模和角度
	e = sqrt(e1 * e1 + f1 * f1);//正序短路电流的模
	f = atan2(e1, f1) * 57.2957795;//正序短路电流的相角
	e1 = e;
	f1 = f;
	e = sqrt(e2 * e2 + f2 * f2);
	f = atan2(e2, f2) * 57.2957795;
	e2 = e;
	f2 = f;
	//相的模和角度
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

	//故障点的短路电压计算出来了
	va_3 = 10 * ea;
	vb_3 = 10 * eb;
	vc_3 = 10 * ec;

	sprintf(output, "i:%02.3f/%02.3f/%02.3f\tv:%02.3f/%02.3f/%02.3f", ia_3, ib_3, ic_3, va_3, vb_3, vc_3);

	return 1;
}





