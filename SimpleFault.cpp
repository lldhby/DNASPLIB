// SimpleFault.cpp: implementation of the CSimpleFault class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "math.h"

//#include "SimpleFaultDataStruct.h"
#include "../DNASPLIB/SimpleFault.h"
//#include "../handledatabase.h"

#include <iostream>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static  char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimpleFault::CSimpleFault()
{

}

CSimpleFault::~CSimpleFault()
{

}

bool CSimpleFault::SCOP(SimpleFaultDataStruct& m_dataStruct)	//节点优化编号
{
	int i, i1, i2, i3, i5, j, j1, j2, j3, j4, k, k1, k2, k3, k4;

	for(i = 1; i <= m_dataStruct.N; i++)
	{
		m_dataStruct.ID[i] = 0;	//节点i出线数归零
	}

	for(k = 1; k <= m_dataStruct.M; k++)	//所有支路都搜索一遍
	{
Loop30:	
		if(m_dataStruct.IZA[k] == 0 || m_dataStruct.IZA[k] >= 4)
			continue;	//4是接地支路
		i = m_dataStruct.IZ1[k];	//支路节点号1，从1开始
		j = m_dataStruct.IZ2[k];	//支路节点号2，从1开始
		i1 = m_dataStruct.ID[i];	//节点i上所连的支路数，从1开始
		j1 = m_dataStruct.ID[j];	//节点j上所连的支路数，从1开始
		for(k1 = 1; k1 <= i1; k1++)	//判断与i节点相连的节点号是否有j节点
		{
			if(m_dataStruct.IX[i][k1] == j) 
			{
				for(k1 = 1; k1 <= j1; k1++)
				{
					if(m_dataStruct.IX[j][k1] == i)	//与i节点相连的节点中有j节点
					{
						goto Loop30;
					}
				}
			}
			else
			{
				i1++;	//i节点支路数加1
				j1++;	//j节点支路数加1
				m_dataStruct.ID[i] = i1;
				m_dataStruct.ID[j] = j1;
				m_dataStruct.IX[i][i1] = j;		//记录i节点上的j连接节点号，从1开始
				m_dataStruct.IX[j][j1] = i; 	//记录j节点上的i连接节点号，从1开始
			}
		}
	}
	
	for(i5 = 1; i5 <= m_dataStruct.N; i5++)	//i5新节点号的寄存单元
	{
		i1 = MaxBranchNumber;	//出线数，从1开始
		for(i =1; i <= m_dataStruct.N; i++)	//找到出线数最少的节点
		{
			i2 = m_dataStruct.ID[i];	//节点i出线数，从1开始
			if(i2 <= 1)	//i2=1表示节点i只有一条出线，这已经是最少的了，i2不可能小于1
			{
				i1 = i2;	//当前最少出线数，从1开始
				j = i;	//最少出线节点号减1
				break;	//跳出for循环
			}
			if(i2 >= i1) continue;
			i1 = i2;	//当前最少出线数，从1开始
			j = i;	//最少出线节点号减1
		}

		m_dataStruct.INA[i5] = j;	//新节点号，从1开始
		m_dataStruct.INB[j] = i5;			//旧节点号，从1开始，j从0开始

		m_dataStruct.ID[j] = MaxBranchNumber;	//节点已经编号

		for(j1 = 1; j1 <= i1; j1++)	//消去节点j+1，i1是当前最少出线数，从1开始
		{
			i3 = m_dataStruct.IX[j][j1];	//节点j的对端线路号
			j2 = m_dataStruct.ID[i3];	//节点j的对端线路出线数
			for(j3 = 1; j3 <= j2; j3++)
			{
				j4 = m_dataStruct.IX[i3][j3];	//节点j的对端线路的对端线路号
				if(j4 == j) break;
			}
			m_dataStruct.IX[i3][j3] = m_dataStruct.IX[i3][j2];	//消去节点
			m_dataStruct.ID[i3] = j2 - 1;	//关联节点数减1
		}

		for(j1 = 1; j1 <= i1 - 1; j1++)	//添加注入元素
		{
			k1 = m_dataStruct.IX[j][j1];	//节点j+1的对端线路号
			k3 = m_dataStruct.ID[k1];	//节点j+1的对端线路出线数
			for(j2 = j1 + 1; j2 <= i1; j2++)
			{
				k2 = m_dataStruct.IX[j][j2];	//在k1和k2之间添加新线路
				for(j3 = 1; j3 <= k3; j3++)	//检查新线路是否已经存在
				{
					if(m_dataStruct.IX[k1][j3] == k2) 
						break;	//线路已经存在
				}
				
				if(j3 == k3+1)	//线路不存在
				{
					k3++;
					m_dataStruct.IX[k1][k3] = k2;	//k1对端节点号为k2
					m_dataStruct.ID[k1] = k3;
					k4 = m_dataStruct.ID[k2] + 1;
					m_dataStruct.IX[k2][k4] = k1;	//k2对端节点号为k1
					m_dataStruct.ID[k2] = k4;
				}
			}
		}
	}

	return true;
}

bool CSimpleFault::SCKP(SimpleFaultDataStruct& m_dataStruct)	//用新节点号替换旧节点号
{
	int i, j, k;

	for(k = 1; k < m_dataStruct.M; k++)	//支路
	{
		i = m_dataStruct.IZ1[k];	//节点号1
		j = m_dataStruct.IZ2[k];	//节点号2

		m_dataStruct.IZ1[k] = m_dataStruct.INB[i];	//新节点号1
		if(j == 0) 
			continue;	//对地支路的另一种表示方法
		
		m_dataStruct.IZ2[k] = m_dataStruct.INB[j];	//新节点号2
	}

	for(k = 1; k <= m_dataStruct.IQ; k++)	//发电机
	{
		i = m_dataStruct.IWG[k];	//节点号
		m_dataStruct.IWG[k] = m_dataStruct.INB[i];	//新节点号
	}

	return true;
}

//需要把发电机的那个节点找到，在上面加对应的等值电阻
bool CSimpleFault::SCDP(SimpleFaultDataStruct& m_dataStruct)	//形成正序导纳矩阵
{
	int i, j, j1, j3, k, k0, k1, k2, l, ig;
	double b, x;

	for(i = 1; i <= m_dataStruct.N; i++)	//导纳阵对角元素清零
	{
		m_dataStruct.D12[i] = 0;
	}

	l = 0;	//非零互导纳元素的计数单元
	for(k = 1; k <= m_dataStruct.M; k++)	//每个支路都要做
	{
		ig = m_dataStruct.IZA[k];	//支路状态
		if(ig == 0) 
			continue;	//支路状态为零表示停运

		i = m_dataStruct.IZ1[k];	//节点号1
		j = m_dataStruct.IZ2[k];	//节点号2
		x = m_dataStruct.Z2[k];	//电抗，简单短路电流计算忽略电阻和非标准变比
		if(fabs(x) < 1e-10) 
			continue;	//0阻抗支路
		b = -1.0 / x;

		switch(ig)	//按类型操作
		{
		case 1:	//线路
		case 2:	//变压器，非标准变比在i侧
		case 3:	//变压器，非标准变比在i侧
			m_dataStruct.D12[i] += b;
			m_dataStruct.D12[j] += b;

			//如果是电源点（母线节点，还需要加上上级的等效电抗）
			if (i == m_dataStruct.SO)
				m_dataStruct.D12[i] += -1.0 / m_dataStruct.Z7;
			if (j == m_dataStruct.SO)
				m_dataStruct.D12[j] += -1.0 / m_dataStruct.Z7;

			l++;
			m_dataStruct.YZ2[l] = -b;
			m_dataStruct.IY1[l] = i;
			m_dataStruct.IY2[l] = j;
			l++;
			m_dataStruct.YZ2[l] = -b;
			m_dataStruct.IY1[l] = j;
			m_dataStruct.IY2[l] = i;
			break;
		case 4:	//对地支路

			//如果是电源节点，需要特殊处理

			m_dataStruct.D12[i] += b;
			break;	//继续for循环
		}
	}

	//按行号由小到大将非零互导纳元素排列在Y1、Y2中

	j = 0;	//非零互导纳元素的计数单元，逐个累计
	k0 = 0;	//非零互导纳元素的计数单元，按行累计

	for(i = 1; i <= m_dataStruct.N; i++)	//所有节点都扫描一遍
	{
		j1 = 0;	//当前行i非零元素的计数单元
		for(k = 1; k <= l; k++)	//每个节点都需要重新扫描所有支路
		{
			if(m_dataStruct.IY1[k] != i) continue;
			j3 = m_dataStruct.IY2[k];
			for(k1 = 1; k1 <= j1; k1++)
			{
				k2 = k0 + k1;
				if(j3 == m_dataStruct.IY[k2]) break;	//支路已经存在
			}

			if(k1 == j1+1)	//新支路
			{
				j1++;
				j++;
				m_dataStruct.Y2[j] = m_dataStruct.YZ2[k];
				m_dataStruct.IY[j] = j3;
			}
			else	//旧支路
			{
				m_dataStruct.Y2[k2] += m_dataStruct.YZ2[k];
			}
		}

		m_dataStruct.INN[i] = j1;
		k0 += j1;
	}

	return true;
}

//需要把发电机的那个节点找到，在上面加串联的接地变和低电阻
bool CSimpleFault::SCCP(SimpleFaultDataStruct& m_dataStruct)	//形成零序导纳矩阵
{
	int i, j, j1, j3, k, k0, k1, k2, l, ig;
	double b, x;

	for(i = 1; i <= m_dataStruct.N; i++)	//导纳阵对角元素清零
	{
		m_dataStruct.D02[i] = 0;
	}

	l = 0;	//非零互导纳元素的计数单元
	for(k = 1; k <= m_dataStruct.M; k++)	//每个支路都要做
	{
		ig = m_dataStruct.IZA[k];	//支路状态
		if(ig == 0) 
			continue;	//支路状态为零表示停运

		i = m_dataStruct.IZ1[k];	//节点号1
		j = m_dataStruct.IZ2[k];	//节点号2
		x = m_dataStruct.Z5[k];	//电抗
		if(fabs(x) < 1e-10) 
			continue;	//0阻抗支路
		
		b = -1 / x;

		switch(ig)	//按类型操作
		{
		case 1:	//线路
		case 2:	//变压器，非标准变比在i侧
			m_dataStruct.D02[i] += b;
			m_dataStruct.D02[j] += b;

			//if (i == m_dataStruct.NF0)
			//{
			//	//增加一个接地点过渡电阻值
			//	//if (fabs(m_dataStruct.Z8) >= 1e-10)
			//	//	m_dataStruct.D02[i] += -1 / m_dataStruct.Z8;

			//	//增加地网电阻
			//	//if (fabs(m_dataStruct.Z9[k]) >= 1e-10)
			//	//	m_dataStruct.D02[i] += -1 / m_dataStruct.Z9[k];
			//}
			//if (j == m_dataStruct.NF0)
			//{
			//	//增加一个接地点过渡电阻值
			//	//if (fabs(m_dataStruct.Z8) >= 1e-10)
			//	//	m_dataStruct.D02[j] += -1 / m_dataStruct.Z8;

			//	//增加地网电阻
			//	//if (fabs(m_dataStruct.Z10[k]) >= 1e-10)
			//	//	m_dataStruct.D02[j] += -1 / m_dataStruct.Z10[k];
			//}

			l++;
			m_dataStruct.YZ2[l] = -b;
			m_dataStruct.IY1[l] = i;
			m_dataStruct.IY2[l] = j;
			l++;
			m_dataStruct.YZ2[l] = -b;
			m_dataStruct.IY1[l] = j;
			m_dataStruct.IY2[l] = i;
			break;
		case 3:	//变压器
		case 4:	//对地支路
			m_dataStruct.D02[i] += b;
			continue;	//继续for循环
		}
	}

	//按行号由小到大将非零互导纳元素排列在Y1、Y2中
	
	j = 0;	//非零互导纳元素的计数单元，逐个累计
	k0 = 0;	//非零互导纳元素的计数单元，按行累计

	for(i = 1; i <= m_dataStruct.N; i++)	//所有节点都扫描一遍
	{
		j1 = 0;	//当前行i非零元素的计数单元
		for(k = 1; k <= l; k++)	//每个节点都需要重新扫描所有支路
		{
			if(m_dataStruct.IY1[k] != i)
				continue;
			j3 = m_dataStruct.IY2[k];
			for(k1 = 1; k1 <= j1; k1++)
			{
				k2 = k0 + k1;
				if(j3 == m_dataStruct.IY0[k2]) 
					break;	//支路已经存在
			}

			if(k1 == j1+1)	//新支路
			{
				j1++;
				j++;
				m_dataStruct.Y02[j] = m_dataStruct.YZ2[k];
				m_dataStruct.IY0[j] = j3;
			}
			else	//旧支路
			{
				m_dataStruct.Y02[k2] += m_dataStruct.YZ2[k];
			}
		}

		m_dataStruct.IN0[i] = j1;
		k0 += j1;
	}

	return true;
}

bool CSimpleFault::SCJP(SimpleFaultDataStruct& m_dataStruct)	//因子表分解通用函数
{
	int i, ig, i1, i2, i3, k, k0, j, j5, k5;
	double a;

	k0 = 1;	//导纳矩阵非零互导纳元素的指针
	for(i = 1; i <= m_dataStruct.N; i++)	//所有节点都做一次
	{
		m_dataStruct.AK[1] = m_dataStruct.Dt[i];	//对角元素
		m_dataStruct.JK[1] = i;	//对角元素列号（与行号相同）
		j5 = m_dataStruct.INt[i] + 1;	//当前行导纳矩阵非零元素的个数加1
		for(ig = 2; ig <= j5; ig++)	//互导纳元素进工作数组
		{
			m_dataStruct.AK[ig] = m_dataStruct.Yt[k0];	//非零元素值
			m_dataStruct.JK[ig] = m_dataStruct.IYt[k0];	//非零元素列号
			k0++;
		}

		k = 1;	//因子表非零U元素的位置指针
		for(i1 = 1; i1 <= i - 1; i1++)	//前i-1列的非零元素都需要消去
		{
			for(i3 = 2; i3 <= j5; i3++)	//查找当前行非零元素中是否有第i1列的
			{
				if(m_dataStruct.JK[i3] == i1) break;	//有
			}

			if(i3 == j5+1)	//没找到
			{
				k += m_dataStruct.JFt[i1];	//下一行
				continue;
			}

			for(ig = 1; ig <= m_dataStruct.JFt[i1]; ig++)	//因子表第i1+1行非零元素的个数
			{
				for(i2 = 1; i2 <= j5; i2++)
				{
					if(m_dataStruct.JK[i2] == m_dataStruct.IJt[k]) 
						break;	//非注入元素
				}

				if(i2 == j5+1)	//注入新元素
				{
					j5 = i2;
					m_dataStruct.JK[j5] = m_dataStruct.IJt[k];
					m_dataStruct.AK[j5] = -m_dataStruct.AK[i3] * m_dataStruct.AJt[k];
				}
				else	//非注入元素
				{
					m_dataStruct.AK[i2] += -m_dataStruct.AK[i3] * m_dataStruct.AJt[k];
				}

				k++;
			}
		}

		if(fabs(m_dataStruct.AK[1]) < 1e-10)	//空节点
		{
			m_dataStruct.JFt[i] = 0;
			m_dataStruct.DKt[i] = 0;
			continue;
		}

		k5 = k;
		a = 1 / m_dataStruct.AK[1];
		for(j = 2; j <= j5; j++)	//规格化
		{
			if(m_dataStruct.JK[j] <= i) 
				continue;	//左下角元素不存
			m_dataStruct.AJt[k] = m_dataStruct.AK[j] * a;
			m_dataStruct.IJt[k] = m_dataStruct.JK[j];
			k++;
		}

		m_dataStruct.DKt[i] = a;	//对角元素
		m_dataStruct.JFt[i] = k - k5;
	}

	return true;
}

bool CSimpleFault::SCWP(SimpleFaultDataStruct& m_dataStruct)	//常数项处理通用函数
{
	int i, j, k, l, ig;
	double a;

	l = 1;	//导纳矩阵因子表U元素的指针
	for(k = 1; k <= m_dataStruct.N; k++)	//消去
	{
		a = m_dataStruct.CKt[k];
		for(j = 1; j <= m_dataStruct.JFt[k]; j++)	//第k行U非零元素数
		{
			i = m_dataStruct.IJt[l];
			m_dataStruct.CKt[i] += - a * m_dataStruct.AJt[l];
			l++;
		}
		m_dataStruct.CKt[k] = a * m_dataStruct.DKt[k];	//规格化
	}

	for(i = m_dataStruct.N - 1; i >= 1; i--)	//回代
	{
		for(ig = 1; ig <= m_dataStruct.JFt[i]; ig++)
		{
			l--;
			j = m_dataStruct.IJt[l];
			m_dataStruct.CKt[i] += - m_dataStruct.AJt[l] * m_dataStruct.CKt[j];
		}
	}

	return true;
}

//需要把所有的节点都走一遍，计算所有的四种类型故障
double CSimpleFault::SCRP(SimpleFaultDataStruct& m_dataStruct, vector<int> newbs, vector<string> newbs_str,double gImp, double pointImp)	//计算结果输出
{
	double pointI = 0, Us;
	int UsND;
	double b, ai1, ai2, ai0, r1, r2, r3, x2,e1,e2,e0;

	b = 3.0;
	b = sqrt(b) / 2.0;

	switch(m_dataStruct.IFA)
	{
	case 1:	//单相短路接地
		if(fabs(m_dataStruct.CK0[m_dataStruct.NF0]) < 1e-10)
		{
			ai1 = 0;
			ai2 = 0;
			ai0 = 0;
		}
		else
		{
			ai1 = 1 / (m_dataStruct.CK1[m_dataStruct.NF0]
				+ m_dataStruct.CK2[m_dataStruct.NF0] + m_dataStruct.CK0[m_dataStruct.NF0]);
			ai2 = ai1;
			ai0 = ai1;
		}

		break;
	case 2://两相短路
		ai0 = 0;
		ai1 = 1 / (m_dataStruct.CK1[m_dataStruct.NF0] + m_dataStruct.CK2[m_dataStruct.NF0]);
		ai2 = -ai1;
		
		break;
	case 3://三相短路
		ai0 = 0;
		if (m_dataStruct.CK1[m_dataStruct.NF0] != 0)
		{
			ai1 = 1 / m_dataStruct.CK1[m_dataStruct.NF0];//只有正序电流，且幅值一样
		}
		else
		{
			ai1 = 0;
		}
		ai2 = 0;

		break;
	case 4://两相短路接地
		if(fabs(m_dataStruct.CK0[m_dataStruct.NF0]) < 1e-10)
		{
			ai0 = 0;
			ai1 = 1 / (m_dataStruct.CK1[m_dataStruct.NF0] + m_dataStruct.CK2[m_dataStruct.NF0]);
			ai2 = -ai1;
		}
		else
		{
			ai1 = 1 /(m_dataStruct.CK1[m_dataStruct.NF0] + m_dataStruct.CK2[m_dataStruct.NF0] * m_dataStruct.CK0[m_dataStruct.NF0]
				/ (m_dataStruct.CK2[m_dataStruct.NF0] + m_dataStruct.CK0[m_dataStruct.NF0]));
			ai2 = -ai1 * m_dataStruct.CK0[m_dataStruct.NF0] / (m_dataStruct.CK2[m_dataStruct.NF0] + m_dataStruct.CK0[m_dataStruct.NF0]);
			ai0 = -ai1 * m_dataStruct.CK2[m_dataStruct.NF0] / (m_dataStruct.CK2[m_dataStruct.NF0] + m_dataStruct.CK0[m_dataStruct.NF0]);
		}

		break;
	}

	//故障点的电压值
	e1 = 1 - m_dataStruct.CK1[m_dataStruct.NF0] * ai1;
	e2 = -m_dataStruct.CK2[m_dataStruct.NF0] * ai2;
	e0 = -m_dataStruct.CK0[m_dataStruct.NF0] * ai0;

	r1 = e0 + e1 + e2;
	r2 = e0 - 0.5 * e1 - 0.5 * e2;
	x2 = -b * e1 + b * e2;

	r1 *= 10;//变有名值
	r3 = 0;
	if (fabs(r2) > 1e-10 || fabs(x2) > 1e-10)
	{
		r3 = 10 * sqrt(r2 * r2 + x2 * x2);
	}
	r1 = fabs(r1);
	e1 = r1;
	e0 = r3;

	//故障点的电流值
	r1 = ai0 + ai1 + ai2;//三个序网的电流相加
	r2 = ai0 - 0.5 * ai1 - 0.5 * ai2;
	x2 = -b * ai1 + b * ai2;

	r1 *= 10 / 1.732;//变有名值
	r3 = 0;
	if(fabs(r2) > 1e-10 || fabs(x2) > 1e-10)
	{
		r3 = (10/1.732) * sqrt(r2 * r2 + x2 * x2);
	}

	//r1,r1,r3
	double ret = 0;
	r1 = fabs(r1);
	char  sResult[512];
	int ndt = m_dataStruct.NF0 - 1;
	string nds = newbs_str[ndt];
	char  nd[512];
	if (m_dataStruct.IFA == 1)//单相接地
	{
		pointI = r1;
		Us = 0;
		UsND = -1;
		IsValid(m_dataStruct, pointI, Us, UsND);
		if (UsND == -1)
			strcpy(nd, "-");
		else
			strcpy(nd, newbs_str[UsND].c_str());

		sprintf(sResult, "1	%s	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%s\n", nds.c_str(), r1, r3, r3, (10/1.732)*ai1, (10 / 1.732)*ai2, (10 / 1.732)*ai0, e1, e0, e0, gImp, pointImp, m_dataStruct.capI, pointI,Us, nd);
	}
	if (m_dataStruct.IFA == 2)//两相
		sprintf(sResult, "2	%s	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	-	-	-	-	-	-\n", newbs_str[ndt].c_str(), r1, r3, r3, ai1, ai2, ai0, e1, e0, e0);
	if (m_dataStruct.IFA == 3)
		sprintf(sResult, "3	%s	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	-	-	-	-	-	-\n", newbs_str[ndt].c_str(), r1, r3, r3, (10 / 1.732)*ai1, (10 / 1.732)*ai2, (10 / 1.732)*ai0, e1, e0, e0);
	if (m_dataStruct.IFA == 4)//两相短路接地
	{
		//pointI = r3;
		IsValid(m_dataStruct, pointI, Us, UsND);
		if (UsND == -1)
			strcpy(nd, "-");
		else
			strcpy(nd, newbs_str[UsND].c_str());
		sprintf(sResult, "4	%s	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%s\n", newbs_str[ndt].c_str(), r1, r3, r3, ai1, ai2, ai0, e1, e0, e0, gImp, pointImp, m_dataStruct.capI, pointI, Us, nd);
	}	//printf(sResult);
	scResult.push_back(sResult);

/*
	FILE *fp;
	fp = NULL;

	if((fp = fopen("SimpleFault_result.dat", "w")) == NULL)
	{
		return false;
	}




	fprintf(fp, "\n                       ****  故障点电流  ****\n\n");
	fprintf(fp, "    NODE    VOLTAGE     PHASE     G-P       G-Q       L-P       L-Q\n");
	fprintf(fp, "\n      NF0=%4i  IFA=%4i  ai1=%8.4f  ai2=%8.4f  ai0=%8.4f  r1=%8.4f  r2=%8.4f  r3=%8.4f\n\n",
		iNF0,	m_dataStruct.IFA, ai1, ai2, ai0, r1, r3, r3);

	if(fp != NULL) fclose(fp);
*/
	return pointI;
}

//单次运行计算,仅仅只是零序导纳矩阵发生了变化
//传入：l-节点号
double CSimpleFault::RunSingle(SimpleFaultDataStruct& m_dataStruct, char  * eFile, vector<int> newbs, vector<string> newbs_str,int l, double gImp, double pointImp)	//短路电流计算主程序
{
	int i, j, k, m;
	double pointI = 0, maxpointI=0;

	scResult.clear();//计算结果先清零

	for (m = 1; m <= 4; m++)
	{
		m_dataStruct.NF0 = l;//故障节点
		m_dataStruct.IFA = m;//单相接地

		SCCP(m_dataStruct);	//形成零序导纳矩阵

		for (i = 1; i <= m_dataStruct.N; i++)	//形成负序导纳矩阵对角元素
		{
			m_dataStruct.D22[i] = m_dataStruct.D12[i];
		}

		//lld commit 2019-1-18,配网没发电机，用等值阻抗替代
		for (i = 1; i <= m_dataStruct.IQ; i++)	//正、负序网络发电机
		{
			if (m_dataStruct.IWGA[i] == 0) continue;
			j = m_dataStruct.IWG[i];//发电机的节点号
			m_dataStruct.D12[j] += -1 / m_dataStruct.W4[i];	//次暂态电抗
			m_dataStruct.D22[j] += -1 / m_dataStruct.W3[i];	//负序电抗
		}

		m_dataStruct.INt = m_dataStruct.INN;
		m_dataStruct.Dt = m_dataStruct.D12;
		m_dataStruct.Yt = m_dataStruct.Y2;
		m_dataStruct.IYt = m_dataStruct.IY;
		m_dataStruct.AJt = m_dataStruct.AJ1;
		m_dataStruct.IJt = m_dataStruct.IJ1;
		m_dataStruct.JFt = m_dataStruct.JF1;
		m_dataStruct.DKt = m_dataStruct.DK1;
		SCJP(m_dataStruct);	//正序网因子分解

		m_dataStruct.Dt = m_dataStruct.D22;
		m_dataStruct.Yt = m_dataStruct.Y2;
		m_dataStruct.IYt = m_dataStruct.IY;
		m_dataStruct.AJt = m_dataStruct.AJ2;
		m_dataStruct.IJt = m_dataStruct.IJ2;
		m_dataStruct.JFt = m_dataStruct.JF2;
		m_dataStruct.DKt = m_dataStruct.DK2;
		SCJP(m_dataStruct);	//负序网因子分解

		m_dataStruct.INt = m_dataStruct.IN0;
		m_dataStruct.Dt = m_dataStruct.D02;
		m_dataStruct.Yt = m_dataStruct.Y02;
		m_dataStruct.IYt = m_dataStruct.IY0;
		m_dataStruct.AJt = m_dataStruct.AJ0;
		m_dataStruct.IJt = m_dataStruct.IJ0;
		m_dataStruct.JFt = m_dataStruct.JF0;
		m_dataStruct.DKt = m_dataStruct.DK0;
		SCJP(m_dataStruct);	//零序网因子分解

		m_dataStruct.NF0 = m_dataStruct.INB[m_dataStruct.NF0];
		for (k = 1; k <= m_dataStruct.N; k++)
		{
			m_dataStruct.CK1[k] = 0;
			m_dataStruct.CK2[k] = 0;
			m_dataStruct.CK0[k] = 0;
		}

		switch (m_dataStruct.IFA)	//短路类型
		{
		case 1:	//单相短路
		case 4:	//两相短路接地、这里不要break语句!
			m_dataStruct.CK0[m_dataStruct.NF0] = 1;

			m_dataStruct.AJt = m_dataStruct.AJ0;
			m_dataStruct.IJt = m_dataStruct.IJ0;
			m_dataStruct.JFt = m_dataStruct.JF0;
			m_dataStruct.CKt = m_dataStruct.CK0;
			m_dataStruct.DKt = m_dataStruct.DK0;
			SCWP(m_dataStruct);
		case 2:	//两相短路
			m_dataStruct.CK2[m_dataStruct.NF0] = 1;

			m_dataStruct.AJt = m_dataStruct.AJ2;
			m_dataStruct.IJt = m_dataStruct.IJ2;
			m_dataStruct.JFt = m_dataStruct.JF2;
			m_dataStruct.CKt = m_dataStruct.CK2;
			m_dataStruct.DKt = m_dataStruct.DK2;
			SCWP(m_dataStruct);
		case 3:	//三相短路
			m_dataStruct.CK1[m_dataStruct.NF0] = 1;

			m_dataStruct.AJt = m_dataStruct.AJ1;
			m_dataStruct.IJt = m_dataStruct.IJ1;
			m_dataStruct.JFt = m_dataStruct.JF1;
			m_dataStruct.CKt = m_dataStruct.CK1;
			m_dataStruct.DKt = m_dataStruct.DK1;
			SCWP(m_dataStruct);
		}

		//计算短路点的电流值
		pointI = SCRP(m_dataStruct, newbs, newbs_str,  gImp,  pointImp);	//计算结果输出
		if (pointI > maxpointI)
			maxpointI = pointI;
	}
	FILE *fp;
	fp = NULL;
	if ((fp = fopen(eFile, "a")) == NULL)
		return 0;
	//节点  单相接地：A相	B相	C相	正序	负序	零序
	//		fprintf(fp, "接地类型	节点	A相	B相	C相	正序	负序	零序	A相电压	B相电压	C相电压\n");
	for (i = 0; i < scResult.size(); i++)//v.size() 表示vector存入元素的个数
		fprintf(fp, scResult[i].c_str());
	if (fp != NULL) fclose(fp);

	return maxpointI;
}

//判断是否超标
bool CSimpleFault::IsValid(SimpleFaultDataStruct& m_dataStruct,double pI, double& Us,int& UsNd)
{
	bool ret = false;
	int i, j, k, m, l, n;
	double pointI = 0;//最大入地电流
	//double gImp = 10;//低电阻
	double pointImp = 0;//接地点过渡电阻
	Us = 0;//跨步电压
	UsNd = -1;
	double maxUs = 0;
	bool find = false;

	pI *= 1000;//变成A
	pointI = sqrt(pI* pI + m_dataStruct.capI*m_dataStruct.capI);
	if ((pointI < 30) || (pointI > 1000))
	{
		find = true;
//		return false;
	}
	//要求：1.入地电流（含电容电流）最好不要大于700A，不能大于1000A，因为有通信干扰
	//2.入地电流不能太小，太小不利于保护启动，至少30A
	//3.接地点的跨步电流不能超过2000V，R*I<2000
	//获取该节点的厂站接地网电阻
	//int ndt = m_dataStruct.NF0 - 1;
	//string nds = newbs_str[ndt];
	// nd[64];
	//strcpy(nd , nds.c_str());

	for (int k = 1; k <= m_dataStruct.M; k++)	//每个支路都要做
	{
		if (m_dataStruct.IZA[k] == 0)
			continue;	//支路状态为零表示停运

		i = m_dataStruct.IZ1[k];	//节点号1
		j = m_dataStruct.IZ2[k];	//节点号2
		double x = m_dataStruct.Z5[k];	//电抗
		if (fabs(x) < 1e-10)
			continue;	//0阻抗支路

		if (i == m_dataStruct.NF0)
		{
			if (fabs(m_dataStruct.Z9[k]) >= 1e-10)
			{
				Us = pointI * m_dataStruct.Z9[k];
				if (Us > maxUs)
				{
					maxUs = Us;
					UsNd = i;
				}

				if (Us > 2000)
				{
					find = true;
					break;
				}
			}
		}
		if (j == m_dataStruct.NF0)
		{
			if (fabs(m_dataStruct.Z10[k]) >= 1e-10)
			{
				Us = pointI * m_dataStruct.Z10[k];
				if (Us > maxUs)
				{
					maxUs = Us;
					UsNd = j;
				}
				if (Us > 2000)
				{
					find = true;
					break;
				}
			}
		}
	}
	Us = maxUs;

	if (find)
		return false;

	return true;
}

bool CSimpleFault::Run(SimpleFaultDataStruct& m_dataStruct, char * eFile, vector<int> newbs, vector<string> newbs_str)	//短路电流计算主程序
{
	int i,j,k,m,l,n;
	double pointI = 0;//最大入地电流
	double gImp = 10;//低电阻
	double pointImp=0;//接地点过渡电阻
	double Us = 0;//跨步电压
	int UsNd = -1;
	bool find = false;
	
	double maxpointI = 0;//最大入地电流
	double maxUs = 0;//跨步电压
	int maxUsNd = -1;

	bool varImp=false, varPoint=false;
	if (m_dataStruct.Z8 == -1)
		varPoint = true;
		
	if (m_dataStruct.Z12 == -1)//仅仅电阻需要迭代
		varImp = true;

	FILE *fp;
	fp = NULL;
	if ((fp = fopen(eFile, "a")) == NULL)
		return false;
	//节点  单相接地：A相	B相	C相	正序	负序	零序
	fprintf(fp, "接地类型	节点	A相	B相	C相	正序	负序	零序	A相电压	B相电压	C相电压 接地电阻 接地点过渡电阻 电容电流 入地电流 跨步电压 跨步电压节点号\n");
	if (fp != NULL) fclose(fp);

	SCOP(m_dataStruct);	//节点优化编号
	SCKP(m_dataStruct);	//用新节点号替换旧节点号
	SCDP(m_dataStruct);	//形成正序导纳矩阵

	//需要增加对每个节点的对地过渡电阻的设置
	//从这里开始进行计算，对每个节点进行扫描
		//1.判断是否计算不同的接地点过渡电阻
		//2.判断是否迭代计算
	if ((!varPoint) && (varImp))//仅仅电阻需要迭代
	{
		bool isExit = false;
		for (m = 10;m > 3;m--)
		{
			find = false;
			for (l = 1; l <= m_dataStruct.N; l++)
			{
				gImp = 3 * m;
				m_dataStruct.Z5[m_dataStruct.M] = sqrt(gImp * gImp + m_dataStruct.Z11*m_dataStruct.Z11);//最后的那个支路
				pointImp = m_dataStruct.Z8;
				pointI = RunSingle(m_dataStruct, eFile, newbs, newbs_str, l, gImp, pointImp);

//				if (!IsValid(m_dataStruct, pointI, Us, UsNd))
//				{
//					isExit = true;//只要一个节点不合格，就不要迭代了
//					break;
//				}
			}
//			if (isExit)
//				break;
		}
	}
	else
	if ((varPoint) && (!varImp))//仅仅接地点过渡电阻需要迭代
	{
		bool isExit = false;
		for (m = 80;m >= 0;m = m - 20)
		{
			find = false;
			for (l = 1; l <= m_dataStruct.N; l++)
			{
				pointImp = m_dataStruct.Z8 = 3 * m;
				gImp = m_dataStruct.Z12;
				m_dataStruct.Z5[m_dataStruct.M] = sqrt((gImp + pointImp)*(gImp + pointImp) + m_dataStruct.Z11*m_dataStruct.Z11);//最后的那个支路
				pointI = RunSingle(m_dataStruct, eFile, newbs, newbs_str, l, gImp, pointImp);
			}
			//	if (!IsValid(m_dataStruct, pointI, Us, UsNd))
			//	{
			//		isExit = true;//只要一个节点不合格，就不要迭代了
			//		break;
			//	}
			//}
			//if (isExit)
			//	break;
		}
	}
	else
	if ((varPoint) && (varImp))//全部需要迭代
	{
		bool isExit = false;
		for (m = 10;m > 3;m--)
		{
			for (n = 80;n >= 0;n = n - 20)
			{
				find = false;
				for (l = 1; l <= m_dataStruct.N; l++)
				{
					gImp = 3 * m;
					pointImp = m_dataStruct.Z8 = 3 * n;
					m_dataStruct.Z5[m_dataStruct.M] = sqrt((gImp + pointImp)*(gImp + pointImp) + m_dataStruct.Z11*m_dataStruct.Z11);//最后的那个支路
					pointI = RunSingle(m_dataStruct, eFile, newbs, newbs_str, l, gImp, pointImp);
				}
			}
			//		if (!IsValid(m_dataStruct, pointI, Us, UsNd))
			//		{
			//			isExit = true;//只要一个节点不合格，就不要迭代了
			//			break;
			//		}
			//	}
			//	if (isExit)
			//		break;
			//}
			//if (isExit)
			//	break;
		}
	}
	else//不迭代
	{
		gImp = m_dataStruct.Z12 + m_dataStruct.Z8;//最后的那个支路
		m_dataStruct.Z5[m_dataStruct.M] = sqrt(gImp*gImp + m_dataStruct.Z11*m_dataStruct.Z11);
		pointImp = m_dataStruct.Z8;
		for (l = 1; l <= m_dataStruct.N; l++)
		{
			pointI = RunSingle(m_dataStruct, eFile, newbs, newbs_str, l, gImp, pointImp);
		}
		//IsValid(m_dataStruct, pointI, Us, UsNd);
		//if (pointI > maxpointI)
		//	maxpointI = pointI;
		//if (Us > maxUs)
		//{
		//	maxUs = Us;
		//	maxUsNd = UsNd;
		//}
	}

	//输出选到的小电阻值
	//fp = NULL;
	//if ((fp = fopen(eFile, "a")) == NULL)
	//	return false;
	// nd[64];
	//nd[0] = 0;
	//if (maxUsNd >= 1)
	//	strcpy(nd,newbs_str[maxUsNd].c_str());
	//else
	//	strcpy(nd, "-");

	//fprintf(fp, "\n建议小电阻值：%f\t接地点过渡电阻值：%f\t最大入地电流值：%f\t最大跨步电压值：%f\t最大跨步电压节点：%s", gImp/3, pointImp, maxpointI, maxUs, nd);
	//if (fp != NULL) fclose(fp);

	return true;
}

/*
FILE *fp;
fp = NULL;

if((fp = fopen("branch1.dat", "w")) == NULL)
{
return false;
}

for(i = 0; i < m_dataStruct.M; i++)//读入支路数目
{
fprintf(fp, "\n      IZA=%4i  IZ1=%4i  IZ2=%4i  Z2=%8.5f  Z5=%8.5f\n",
m_dataStruct.IZA[i],m_dataStruct.IZ1[i],m_dataStruct.IZ2[i],m_dataStruct.Z2[i],
m_dataStruct.Z5[i]);
}
if(fp != NULL) fclose(fp);



if((fp = fopen("gen.dat", "w")) == NULL)
{
return false;
}

for(i = 0; i < m_dataStruct.IQ; i++)
{//发电机类型，发电机节点编号，发电机的负序电抗，发电机的次暂态电抗
fprintf(fp, "\n      IWGA=%4i  IWG=%4i  W3=%8.4f  W4=%8.4f \n",
m_dataStruct.IWGA[i],m_dataStruct.IWG[i],m_dataStruct.W3[i],m_dataStruct.W4[i]);
}
if(fp != NULL) fclose(fp);
*/




		//for (m = 1; m <= 4; m++)
		//{
		//	m_dataStruct.NF0 = l;//故障节点
		//	m_dataStruct.IFA = m;//单相接地

		//	SCCP(m_dataStruct);	//形成零序导纳矩阵

		//	for (i = 1; i <= m_dataStruct.N; i++)	//形成负序导纳矩阵对角元素
		//	{
		//		m_dataStruct.D22[i] = m_dataStruct.D12[i];
		//	}

		//	//lld commit 2019-1-18,配网没发电机，用等值阻抗替代
		//	for (i = 1; i <= m_dataStruct.IQ; i++)	//正、负序网络发电机
		//	{
		//		if (m_dataStruct.IWGA[i] == 0) continue;
		//		j = m_dataStruct.IWG[i];//发电机的节点号
		//		m_dataStruct.D12[j] += -1 / m_dataStruct.W4[i];	//次暂态电抗
		//		m_dataStruct.D22[j] += -1 / m_dataStruct.W3[i];	//负序电抗
		//	}

		//	m_dataStruct.INt = m_dataStruct.INN;
		//	m_dataStruct.Dt = m_dataStruct.D12;
		//	m_dataStruct.Yt = m_dataStruct.Y2;
		//	m_dataStruct.IYt = m_dataStruct.IY;
		//	m_dataStruct.AJt = m_dataStruct.AJ1;
		//	m_dataStruct.IJt = m_dataStruct.IJ1;
		//	m_dataStruct.JFt = m_dataStruct.JF1;
		//	m_dataStruct.DKt = m_dataStruct.DK1;
		//	SCJP(m_dataStruct);	//正序网因子分解

		//	m_dataStruct.Dt = m_dataStruct.D22;
		//	m_dataStruct.Yt = m_dataStruct.Y2;
		//	m_dataStruct.IYt = m_dataStruct.IY;
		//	m_dataStruct.AJt = m_dataStruct.AJ2;
		//	m_dataStruct.IJt = m_dataStruct.IJ2;
		//	m_dataStruct.JFt = m_dataStruct.JF2;
		//	m_dataStruct.DKt = m_dataStruct.DK2;
		//	SCJP(m_dataStruct);	//负序网因子分解

		//	m_dataStruct.INt = m_dataStruct.IN0;
		//	m_dataStruct.Dt = m_dataStruct.D02;
		//	m_dataStruct.Yt = m_dataStruct.Y02;
		//	m_dataStruct.IYt = m_dataStruct.IY0;
		//	m_dataStruct.AJt = m_dataStruct.AJ0;
		//	m_dataStruct.IJt = m_dataStruct.IJ0;
		//	m_dataStruct.JFt = m_dataStruct.JF0;
		//	m_dataStruct.DKt = m_dataStruct.DK0;
		//	SCJP(m_dataStruct);	//零序网因子分解

		//	m_dataStruct.NF0 = m_dataStruct.INB[m_dataStruct.NF0];
		//	for (k = 1; k <= m_dataStruct.N; k++)
		//	{
		//		m_dataStruct.CK1[k] = 0;
		//		m_dataStruct.CK2[k] = 0;
		//		m_dataStruct.CK0[k] = 0;
		//	}

		//	switch (m_dataStruct.IFA)	//短路类型
		//	{
		//	case 1:	//单相短路
		//	case 4:	//两相短路接地、这里不要break语句!
		//		m_dataStruct.CK0[m_dataStruct.NF0] = 1;

		//		m_dataStruct.AJt = m_dataStruct.AJ0;
		//		m_dataStruct.IJt = m_dataStruct.IJ0;
		//		m_dataStruct.JFt = m_dataStruct.JF0;
		//		m_dataStruct.CKt = m_dataStruct.CK0;
		//		m_dataStruct.DKt = m_dataStruct.DK0;
		//		SCWP(m_dataStruct);
		//	case 2:	//两相短路
		//		m_dataStruct.CK2[m_dataStruct.NF0] = 1;

		//		m_dataStruct.AJt = m_dataStruct.AJ2;
		//		m_dataStruct.IJt = m_dataStruct.IJ2;
		//		m_dataStruct.JFt = m_dataStruct.JF2;
		//		m_dataStruct.CKt = m_dataStruct.CK2;
		//		m_dataStruct.DKt = m_dataStruct.DK2;
		//		SCWP(m_dataStruct);
		//	case 3:	//三相短路
		//		m_dataStruct.CK1[m_dataStruct.NF0] = 1;

		//		m_dataStruct.AJt = m_dataStruct.AJ1;
		//		m_dataStruct.IJt = m_dataStruct.IJ1;
		//		m_dataStruct.JFt = m_dataStruct.JF1;
		//		m_dataStruct.CKt = m_dataStruct.CK1;
		//		m_dataStruct.DKt = m_dataStruct.DK1;
		//		SCWP(m_dataStruct);
		//	}

		//	//计算短路点的电流值
		//	SCRP(m_dataStruct, newbs, newbs_str);	//计算结果输出
		//	//需要计算电压值
		//}

		//FILE *fp;
		//fp = NULL;
		//if ((fp = fopen(eFile, "a")) == NULL)
		//	return false;
		////节点  单相接地：A相	B相	C相	正序	负序	零序
		//fprintf(fp, "接地类型	节点	A相	B相	C相	正序	负序	零序	A相电压	B相电压	C相电压\n");
		//for (i = 0; i < scResult.size(); i++)//v.size() 表示vector存入元素的个数
		//	fprintf(fp, scResult[i].c_str());
		//if (fp != NULL) fclose(fp);
	//}
