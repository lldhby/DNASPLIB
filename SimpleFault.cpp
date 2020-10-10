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

bool CSimpleFault::SCOP(SimpleFaultDataStruct& m_dataStruct)	//�ڵ��Ż����
{
	int i, i1, i2, i3, i5, j, j1, j2, j3, j4, k, k1, k2, k3, k4;

	for(i = 1; i <= m_dataStruct.N; i++)
	{
		m_dataStruct.ID[i] = 0;	//�ڵ�i����������
	}

	for(k = 1; k <= m_dataStruct.M; k++)	//����֧·������һ��
	{
Loop30:	
		if(m_dataStruct.IZA[k] == 0 || m_dataStruct.IZA[k] >= 4)
			continue;	//4�ǽӵ�֧·
		i = m_dataStruct.IZ1[k];	//֧·�ڵ��1����1��ʼ
		j = m_dataStruct.IZ2[k];	//֧·�ڵ��2����1��ʼ
		i1 = m_dataStruct.ID[i];	//�ڵ�i��������֧·������1��ʼ
		j1 = m_dataStruct.ID[j];	//�ڵ�j��������֧·������1��ʼ
		for(k1 = 1; k1 <= i1; k1++)	//�ж���i�ڵ������Ľڵ���Ƿ���j�ڵ�
		{
			if(m_dataStruct.IX[i][k1] == j) 
			{
				for(k1 = 1; k1 <= j1; k1++)
				{
					if(m_dataStruct.IX[j][k1] == i)	//��i�ڵ������Ľڵ�����j�ڵ�
					{
						goto Loop30;
					}
				}
			}
			else
			{
				i1++;	//i�ڵ�֧·����1
				j1++;	//j�ڵ�֧·����1
				m_dataStruct.ID[i] = i1;
				m_dataStruct.ID[j] = j1;
				m_dataStruct.IX[i][i1] = j;		//��¼i�ڵ��ϵ�j���ӽڵ�ţ���1��ʼ
				m_dataStruct.IX[j][j1] = i; 	//��¼j�ڵ��ϵ�i���ӽڵ�ţ���1��ʼ
			}
		}
	}
	
	for(i5 = 1; i5 <= m_dataStruct.N; i5++)	//i5�½ڵ�ŵļĴ浥Ԫ
	{
		i1 = MaxBranchNumber;	//����������1��ʼ
		for(i =1; i <= m_dataStruct.N; i++)	//�ҵ����������ٵĽڵ�
		{
			i2 = m_dataStruct.ID[i];	//�ڵ�i����������1��ʼ
			if(i2 <= 1)	//i2=1��ʾ�ڵ�iֻ��һ�����ߣ����Ѿ������ٵ��ˣ�i2������С��1
			{
				i1 = i2;	//��ǰ���ٳ���������1��ʼ
				j = i;	//���ٳ��߽ڵ�ż�1
				break;	//����forѭ��
			}
			if(i2 >= i1) continue;
			i1 = i2;	//��ǰ���ٳ���������1��ʼ
			j = i;	//���ٳ��߽ڵ�ż�1
		}

		m_dataStruct.INA[i5] = j;	//�½ڵ�ţ���1��ʼ
		m_dataStruct.INB[j] = i5;			//�ɽڵ�ţ���1��ʼ��j��0��ʼ

		m_dataStruct.ID[j] = MaxBranchNumber;	//�ڵ��Ѿ����

		for(j1 = 1; j1 <= i1; j1++)	//��ȥ�ڵ�j+1��i1�ǵ�ǰ���ٳ���������1��ʼ
		{
			i3 = m_dataStruct.IX[j][j1];	//�ڵ�j�ĶԶ���·��
			j2 = m_dataStruct.ID[i3];	//�ڵ�j�ĶԶ���·������
			for(j3 = 1; j3 <= j2; j3++)
			{
				j4 = m_dataStruct.IX[i3][j3];	//�ڵ�j�ĶԶ���·�ĶԶ���·��
				if(j4 == j) break;
			}
			m_dataStruct.IX[i3][j3] = m_dataStruct.IX[i3][j2];	//��ȥ�ڵ�
			m_dataStruct.ID[i3] = j2 - 1;	//�����ڵ�����1
		}

		for(j1 = 1; j1 <= i1 - 1; j1++)	//���ע��Ԫ��
		{
			k1 = m_dataStruct.IX[j][j1];	//�ڵ�j+1�ĶԶ���·��
			k3 = m_dataStruct.ID[k1];	//�ڵ�j+1�ĶԶ���·������
			for(j2 = j1 + 1; j2 <= i1; j2++)
			{
				k2 = m_dataStruct.IX[j][j2];	//��k1��k2֮���������·
				for(j3 = 1; j3 <= k3; j3++)	//�������·�Ƿ��Ѿ�����
				{
					if(m_dataStruct.IX[k1][j3] == k2) 
						break;	//��·�Ѿ�����
				}
				
				if(j3 == k3+1)	//��·������
				{
					k3++;
					m_dataStruct.IX[k1][k3] = k2;	//k1�Զ˽ڵ��Ϊk2
					m_dataStruct.ID[k1] = k3;
					k4 = m_dataStruct.ID[k2] + 1;
					m_dataStruct.IX[k2][k4] = k1;	//k2�Զ˽ڵ��Ϊk1
					m_dataStruct.ID[k2] = k4;
				}
			}
		}
	}

	return true;
}

bool CSimpleFault::SCKP(SimpleFaultDataStruct& m_dataStruct)	//���½ڵ���滻�ɽڵ��
{
	int i, j, k;

	for(k = 1; k < m_dataStruct.M; k++)	//֧·
	{
		i = m_dataStruct.IZ1[k];	//�ڵ��1
		j = m_dataStruct.IZ2[k];	//�ڵ��2

		m_dataStruct.IZ1[k] = m_dataStruct.INB[i];	//�½ڵ��1
		if(j == 0) 
			continue;	//�Ե�֧·����һ�ֱ�ʾ����
		
		m_dataStruct.IZ2[k] = m_dataStruct.INB[j];	//�½ڵ��2
	}

	for(k = 1; k <= m_dataStruct.IQ; k++)	//�����
	{
		i = m_dataStruct.IWG[k];	//�ڵ��
		m_dataStruct.IWG[k] = m_dataStruct.INB[i];	//�½ڵ��
	}

	return true;
}

//��Ҫ�ѷ�������Ǹ��ڵ��ҵ���������Ӷ�Ӧ�ĵ�ֵ����
bool CSimpleFault::SCDP(SimpleFaultDataStruct& m_dataStruct)	//�γ������ɾ���
{
	int i, j, j1, j3, k, k0, k1, k2, l, ig;
	double b, x;

	for(i = 1; i <= m_dataStruct.N; i++)	//������Խ�Ԫ������
	{
		m_dataStruct.D12[i] = 0;
	}

	l = 0;	//���㻥����Ԫ�صļ�����Ԫ
	for(k = 1; k <= m_dataStruct.M; k++)	//ÿ��֧·��Ҫ��
	{
		ig = m_dataStruct.IZA[k];	//֧·״̬
		if(ig == 0) 
			continue;	//֧·״̬Ϊ���ʾͣ��

		i = m_dataStruct.IZ1[k];	//�ڵ��1
		j = m_dataStruct.IZ2[k];	//�ڵ��2
		x = m_dataStruct.Z2[k];	//�翹���򵥶�·����������Ե���ͷǱ�׼���
		if(fabs(x) < 1e-10) 
			continue;	//0�迹֧·
		b = -1.0 / x;

		switch(ig)	//�����Ͳ���
		{
		case 1:	//��·
		case 2:	//��ѹ�����Ǳ�׼�����i��
		case 3:	//��ѹ�����Ǳ�׼�����i��
			m_dataStruct.D12[i] += b;
			m_dataStruct.D12[j] += b;

			//����ǵ�Դ�㣨ĸ�߽ڵ㣬����Ҫ�����ϼ��ĵ�Ч�翹��
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
		case 4:	//�Ե�֧·

			//����ǵ�Դ�ڵ㣬��Ҫ���⴦��

			m_dataStruct.D12[i] += b;
			break;	//����forѭ��
		}
	}

	//���к���С���󽫷��㻥����Ԫ��������Y1��Y2��

	j = 0;	//���㻥����Ԫ�صļ�����Ԫ������ۼ�
	k0 = 0;	//���㻥����Ԫ�صļ�����Ԫ�������ۼ�

	for(i = 1; i <= m_dataStruct.N; i++)	//���нڵ㶼ɨ��һ��
	{
		j1 = 0;	//��ǰ��i����Ԫ�صļ�����Ԫ
		for(k = 1; k <= l; k++)	//ÿ���ڵ㶼��Ҫ����ɨ������֧·
		{
			if(m_dataStruct.IY1[k] != i) continue;
			j3 = m_dataStruct.IY2[k];
			for(k1 = 1; k1 <= j1; k1++)
			{
				k2 = k0 + k1;
				if(j3 == m_dataStruct.IY[k2]) break;	//֧·�Ѿ�����
			}

			if(k1 == j1+1)	//��֧·
			{
				j1++;
				j++;
				m_dataStruct.Y2[j] = m_dataStruct.YZ2[k];
				m_dataStruct.IY[j] = j3;
			}
			else	//��֧·
			{
				m_dataStruct.Y2[k2] += m_dataStruct.YZ2[k];
			}
		}

		m_dataStruct.INN[i] = j1;
		k0 += j1;
	}

	return true;
}

//��Ҫ�ѷ�������Ǹ��ڵ��ҵ���������Ӵ����Ľӵر�͵͵���
bool CSimpleFault::SCCP(SimpleFaultDataStruct& m_dataStruct)	//�γ������ɾ���
{
	int i, j, j1, j3, k, k0, k1, k2, l, ig;
	double b, x;

	for(i = 1; i <= m_dataStruct.N; i++)	//������Խ�Ԫ������
	{
		m_dataStruct.D02[i] = 0;
	}

	l = 0;	//���㻥����Ԫ�صļ�����Ԫ
	for(k = 1; k <= m_dataStruct.M; k++)	//ÿ��֧·��Ҫ��
	{
		ig = m_dataStruct.IZA[k];	//֧·״̬
		if(ig == 0) 
			continue;	//֧·״̬Ϊ���ʾͣ��

		i = m_dataStruct.IZ1[k];	//�ڵ��1
		j = m_dataStruct.IZ2[k];	//�ڵ��2
		x = m_dataStruct.Z5[k];	//�翹
		if(fabs(x) < 1e-10) 
			continue;	//0�迹֧·
		
		b = -1 / x;

		switch(ig)	//�����Ͳ���
		{
		case 1:	//��·
		case 2:	//��ѹ�����Ǳ�׼�����i��
			m_dataStruct.D02[i] += b;
			m_dataStruct.D02[j] += b;

			//if (i == m_dataStruct.NF0)
			//{
			//	//����һ���ӵص���ɵ���ֵ
			//	//if (fabs(m_dataStruct.Z8) >= 1e-10)
			//	//	m_dataStruct.D02[i] += -1 / m_dataStruct.Z8;

			//	//���ӵ�������
			//	//if (fabs(m_dataStruct.Z9[k]) >= 1e-10)
			//	//	m_dataStruct.D02[i] += -1 / m_dataStruct.Z9[k];
			//}
			//if (j == m_dataStruct.NF0)
			//{
			//	//����һ���ӵص���ɵ���ֵ
			//	//if (fabs(m_dataStruct.Z8) >= 1e-10)
			//	//	m_dataStruct.D02[j] += -1 / m_dataStruct.Z8;

			//	//���ӵ�������
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
		case 3:	//��ѹ��
		case 4:	//�Ե�֧·
			m_dataStruct.D02[i] += b;
			continue;	//����forѭ��
		}
	}

	//���к���С���󽫷��㻥����Ԫ��������Y1��Y2��
	
	j = 0;	//���㻥����Ԫ�صļ�����Ԫ������ۼ�
	k0 = 0;	//���㻥����Ԫ�صļ�����Ԫ�������ۼ�

	for(i = 1; i <= m_dataStruct.N; i++)	//���нڵ㶼ɨ��һ��
	{
		j1 = 0;	//��ǰ��i����Ԫ�صļ�����Ԫ
		for(k = 1; k <= l; k++)	//ÿ���ڵ㶼��Ҫ����ɨ������֧·
		{
			if(m_dataStruct.IY1[k] != i)
				continue;
			j3 = m_dataStruct.IY2[k];
			for(k1 = 1; k1 <= j1; k1++)
			{
				k2 = k0 + k1;
				if(j3 == m_dataStruct.IY0[k2]) 
					break;	//֧·�Ѿ�����
			}

			if(k1 == j1+1)	//��֧·
			{
				j1++;
				j++;
				m_dataStruct.Y02[j] = m_dataStruct.YZ2[k];
				m_dataStruct.IY0[j] = j3;
			}
			else	//��֧·
			{
				m_dataStruct.Y02[k2] += m_dataStruct.YZ2[k];
			}
		}

		m_dataStruct.IN0[i] = j1;
		k0 += j1;
	}

	return true;
}

bool CSimpleFault::SCJP(SimpleFaultDataStruct& m_dataStruct)	//���ӱ�ֽ�ͨ�ú���
{
	int i, ig, i1, i2, i3, k, k0, j, j5, k5;
	double a;

	k0 = 1;	//���ɾ�����㻥����Ԫ�ص�ָ��
	for(i = 1; i <= m_dataStruct.N; i++)	//���нڵ㶼��һ��
	{
		m_dataStruct.AK[1] = m_dataStruct.Dt[i];	//�Խ�Ԫ��
		m_dataStruct.JK[1] = i;	//�Խ�Ԫ���кţ����к���ͬ��
		j5 = m_dataStruct.INt[i] + 1;	//��ǰ�е��ɾ������Ԫ�صĸ�����1
		for(ig = 2; ig <= j5; ig++)	//������Ԫ�ؽ���������
		{
			m_dataStruct.AK[ig] = m_dataStruct.Yt[k0];	//����Ԫ��ֵ
			m_dataStruct.JK[ig] = m_dataStruct.IYt[k0];	//����Ԫ���к�
			k0++;
		}

		k = 1;	//���ӱ����UԪ�ص�λ��ָ��
		for(i1 = 1; i1 <= i - 1; i1++)	//ǰi-1�еķ���Ԫ�ض���Ҫ��ȥ
		{
			for(i3 = 2; i3 <= j5; i3++)	//���ҵ�ǰ�з���Ԫ�����Ƿ��е�i1�е�
			{
				if(m_dataStruct.JK[i3] == i1) break;	//��
			}

			if(i3 == j5+1)	//û�ҵ�
			{
				k += m_dataStruct.JFt[i1];	//��һ��
				continue;
			}

			for(ig = 1; ig <= m_dataStruct.JFt[i1]; ig++)	//���ӱ��i1+1�з���Ԫ�صĸ���
			{
				for(i2 = 1; i2 <= j5; i2++)
				{
					if(m_dataStruct.JK[i2] == m_dataStruct.IJt[k]) 
						break;	//��ע��Ԫ��
				}

				if(i2 == j5+1)	//ע����Ԫ��
				{
					j5 = i2;
					m_dataStruct.JK[j5] = m_dataStruct.IJt[k];
					m_dataStruct.AK[j5] = -m_dataStruct.AK[i3] * m_dataStruct.AJt[k];
				}
				else	//��ע��Ԫ��
				{
					m_dataStruct.AK[i2] += -m_dataStruct.AK[i3] * m_dataStruct.AJt[k];
				}

				k++;
			}
		}

		if(fabs(m_dataStruct.AK[1]) < 1e-10)	//�սڵ�
		{
			m_dataStruct.JFt[i] = 0;
			m_dataStruct.DKt[i] = 0;
			continue;
		}

		k5 = k;
		a = 1 / m_dataStruct.AK[1];
		for(j = 2; j <= j5; j++)	//���
		{
			if(m_dataStruct.JK[j] <= i) 
				continue;	//���½�Ԫ�ز���
			m_dataStruct.AJt[k] = m_dataStruct.AK[j] * a;
			m_dataStruct.IJt[k] = m_dataStruct.JK[j];
			k++;
		}

		m_dataStruct.DKt[i] = a;	//�Խ�Ԫ��
		m_dataStruct.JFt[i] = k - k5;
	}

	return true;
}

bool CSimpleFault::SCWP(SimpleFaultDataStruct& m_dataStruct)	//�������ͨ�ú���
{
	int i, j, k, l, ig;
	double a;

	l = 1;	//���ɾ������ӱ�UԪ�ص�ָ��
	for(k = 1; k <= m_dataStruct.N; k++)	//��ȥ
	{
		a = m_dataStruct.CKt[k];
		for(j = 1; j <= m_dataStruct.JFt[k]; j++)	//��k��U����Ԫ����
		{
			i = m_dataStruct.IJt[l];
			m_dataStruct.CKt[i] += - a * m_dataStruct.AJt[l];
			l++;
		}
		m_dataStruct.CKt[k] = a * m_dataStruct.DKt[k];	//���
	}

	for(i = m_dataStruct.N - 1; i >= 1; i--)	//�ش�
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

//��Ҫ�����еĽڵ㶼��һ�飬�������е��������͹���
double CSimpleFault::SCRP(SimpleFaultDataStruct& m_dataStruct, vector<int> newbs, vector<string> newbs_str,double gImp, double pointImp)	//���������
{
	double pointI = 0, Us;
	int UsND;
	double b, ai1, ai2, ai0, r1, r2, r3, x2,e1,e2,e0;

	b = 3.0;
	b = sqrt(b) / 2.0;

	switch(m_dataStruct.IFA)
	{
	case 1:	//�����·�ӵ�
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
	case 2://�����·
		ai0 = 0;
		ai1 = 1 / (m_dataStruct.CK1[m_dataStruct.NF0] + m_dataStruct.CK2[m_dataStruct.NF0]);
		ai2 = -ai1;
		
		break;
	case 3://�����·
		ai0 = 0;
		if (m_dataStruct.CK1[m_dataStruct.NF0] != 0)
		{
			ai1 = 1 / m_dataStruct.CK1[m_dataStruct.NF0];//ֻ������������ҷ�ֵһ��
		}
		else
		{
			ai1 = 0;
		}
		ai2 = 0;

		break;
	case 4://�����·�ӵ�
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

	//���ϵ�ĵ�ѹֵ
	e1 = 1 - m_dataStruct.CK1[m_dataStruct.NF0] * ai1;
	e2 = -m_dataStruct.CK2[m_dataStruct.NF0] * ai2;
	e0 = -m_dataStruct.CK0[m_dataStruct.NF0] * ai0;

	r1 = e0 + e1 + e2;
	r2 = e0 - 0.5 * e1 - 0.5 * e2;
	x2 = -b * e1 + b * e2;

	r1 *= 10;//������ֵ
	r3 = 0;
	if (fabs(r2) > 1e-10 || fabs(x2) > 1e-10)
	{
		r3 = 10 * sqrt(r2 * r2 + x2 * x2);
	}
	r1 = fabs(r1);
	e1 = r1;
	e0 = r3;

	//���ϵ�ĵ���ֵ
	r1 = ai0 + ai1 + ai2;//���������ĵ������
	r2 = ai0 - 0.5 * ai1 - 0.5 * ai2;
	x2 = -b * ai1 + b * ai2;

	r1 *= 10 / 1.732;//������ֵ
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
	if (m_dataStruct.IFA == 1)//����ӵ�
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
	if (m_dataStruct.IFA == 2)//����
		sprintf(sResult, "2	%s	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	-	-	-	-	-	-\n", newbs_str[ndt].c_str(), r1, r3, r3, ai1, ai2, ai0, e1, e0, e0);
	if (m_dataStruct.IFA == 3)
		sprintf(sResult, "3	%s	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	%8.4f	-	-	-	-	-	-\n", newbs_str[ndt].c_str(), r1, r3, r3, (10 / 1.732)*ai1, (10 / 1.732)*ai2, (10 / 1.732)*ai0, e1, e0, e0);
	if (m_dataStruct.IFA == 4)//�����·�ӵ�
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




	fprintf(fp, "\n                       ****  ���ϵ����  ****\n\n");
	fprintf(fp, "    NODE    VOLTAGE     PHASE     G-P       G-Q       L-P       L-Q\n");
	fprintf(fp, "\n      NF0=%4i  IFA=%4i  ai1=%8.4f  ai2=%8.4f  ai0=%8.4f  r1=%8.4f  r2=%8.4f  r3=%8.4f\n\n",
		iNF0,	m_dataStruct.IFA, ai1, ai2, ai0, r1, r3, r3);

	if(fp != NULL) fclose(fp);
*/
	return pointI;
}

//�������м���,����ֻ�������ɾ������˱仯
//���룺l-�ڵ��
double CSimpleFault::RunSingle(SimpleFaultDataStruct& m_dataStruct, char  * eFile, vector<int> newbs, vector<string> newbs_str,int l, double gImp, double pointImp)	//��·��������������
{
	int i, j, k, m;
	double pointI = 0, maxpointI=0;

	scResult.clear();//������������

	for (m = 1; m <= 4; m++)
	{
		m_dataStruct.NF0 = l;//���Ͻڵ�
		m_dataStruct.IFA = m;//����ӵ�

		SCCP(m_dataStruct);	//�γ������ɾ���

		for (i = 1; i <= m_dataStruct.N; i++)	//�γɸ����ɾ���Խ�Ԫ��
		{
			m_dataStruct.D22[i] = m_dataStruct.D12[i];
		}

		//lld commit 2019-1-18,����û��������õ�ֵ�迹���
		for (i = 1; i <= m_dataStruct.IQ; i++)	//�����������緢���
		{
			if (m_dataStruct.IWGA[i] == 0) continue;
			j = m_dataStruct.IWG[i];//������Ľڵ��
			m_dataStruct.D12[j] += -1 / m_dataStruct.W4[i];	//����̬�翹
			m_dataStruct.D22[j] += -1 / m_dataStruct.W3[i];	//����翹
		}

		m_dataStruct.INt = m_dataStruct.INN;
		m_dataStruct.Dt = m_dataStruct.D12;
		m_dataStruct.Yt = m_dataStruct.Y2;
		m_dataStruct.IYt = m_dataStruct.IY;
		m_dataStruct.AJt = m_dataStruct.AJ1;
		m_dataStruct.IJt = m_dataStruct.IJ1;
		m_dataStruct.JFt = m_dataStruct.JF1;
		m_dataStruct.DKt = m_dataStruct.DK1;
		SCJP(m_dataStruct);	//���������ӷֽ�

		m_dataStruct.Dt = m_dataStruct.D22;
		m_dataStruct.Yt = m_dataStruct.Y2;
		m_dataStruct.IYt = m_dataStruct.IY;
		m_dataStruct.AJt = m_dataStruct.AJ2;
		m_dataStruct.IJt = m_dataStruct.IJ2;
		m_dataStruct.JFt = m_dataStruct.JF2;
		m_dataStruct.DKt = m_dataStruct.DK2;
		SCJP(m_dataStruct);	//���������ӷֽ�

		m_dataStruct.INt = m_dataStruct.IN0;
		m_dataStruct.Dt = m_dataStruct.D02;
		m_dataStruct.Yt = m_dataStruct.Y02;
		m_dataStruct.IYt = m_dataStruct.IY0;
		m_dataStruct.AJt = m_dataStruct.AJ0;
		m_dataStruct.IJt = m_dataStruct.IJ0;
		m_dataStruct.JFt = m_dataStruct.JF0;
		m_dataStruct.DKt = m_dataStruct.DK0;
		SCJP(m_dataStruct);	//���������ӷֽ�

		m_dataStruct.NF0 = m_dataStruct.INB[m_dataStruct.NF0];
		for (k = 1; k <= m_dataStruct.N; k++)
		{
			m_dataStruct.CK1[k] = 0;
			m_dataStruct.CK2[k] = 0;
			m_dataStruct.CK0[k] = 0;
		}

		switch (m_dataStruct.IFA)	//��·����
		{
		case 1:	//�����·
		case 4:	//�����·�ӵء����ﲻҪbreak���!
			m_dataStruct.CK0[m_dataStruct.NF0] = 1;

			m_dataStruct.AJt = m_dataStruct.AJ0;
			m_dataStruct.IJt = m_dataStruct.IJ0;
			m_dataStruct.JFt = m_dataStruct.JF0;
			m_dataStruct.CKt = m_dataStruct.CK0;
			m_dataStruct.DKt = m_dataStruct.DK0;
			SCWP(m_dataStruct);
		case 2:	//�����·
			m_dataStruct.CK2[m_dataStruct.NF0] = 1;

			m_dataStruct.AJt = m_dataStruct.AJ2;
			m_dataStruct.IJt = m_dataStruct.IJ2;
			m_dataStruct.JFt = m_dataStruct.JF2;
			m_dataStruct.CKt = m_dataStruct.CK2;
			m_dataStruct.DKt = m_dataStruct.DK2;
			SCWP(m_dataStruct);
		case 3:	//�����·
			m_dataStruct.CK1[m_dataStruct.NF0] = 1;

			m_dataStruct.AJt = m_dataStruct.AJ1;
			m_dataStruct.IJt = m_dataStruct.IJ1;
			m_dataStruct.JFt = m_dataStruct.JF1;
			m_dataStruct.CKt = m_dataStruct.CK1;
			m_dataStruct.DKt = m_dataStruct.DK1;
			SCWP(m_dataStruct);
		}

		//�����·��ĵ���ֵ
		pointI = SCRP(m_dataStruct, newbs, newbs_str,  gImp,  pointImp);	//���������
		if (pointI > maxpointI)
			maxpointI = pointI;
	}
	FILE *fp;
	fp = NULL;
	if ((fp = fopen(eFile, "a")) == NULL)
		return 0;
	//�ڵ�  ����ӵأ�A��	B��	C��	����	����	����
	//		fprintf(fp, "�ӵ�����	�ڵ�	A��	B��	C��	����	����	����	A���ѹ	B���ѹ	C���ѹ\n");
	for (i = 0; i < scResult.size(); i++)//v.size() ��ʾvector����Ԫ�صĸ���
		fprintf(fp, scResult[i].c_str());
	if (fp != NULL) fclose(fp);

	return maxpointI;
}

//�ж��Ƿ񳬱�
bool CSimpleFault::IsValid(SimpleFaultDataStruct& m_dataStruct,double pI, double& Us,int& UsNd)
{
	bool ret = false;
	int i, j, k, m, l, n;
	double pointI = 0;//�����ص���
	//double gImp = 10;//�͵���
	double pointImp = 0;//�ӵص���ɵ���
	Us = 0;//�粽��ѹ
	UsNd = -1;
	double maxUs = 0;
	bool find = false;

	pI *= 1000;//���A
	pointI = sqrt(pI* pI + m_dataStruct.capI*m_dataStruct.capI);
	if ((pointI < 30) || (pointI > 1000))
	{
		find = true;
//		return false;
	}
	//Ҫ��1.��ص����������ݵ�������ò�Ҫ����700A�����ܴ���1000A����Ϊ��ͨ�Ÿ���
	//2.��ص�������̫С��̫С�����ڱ�������������30A
	//3.�ӵص�Ŀ粽�������ܳ���2000V��R*I<2000
	//��ȡ�ýڵ�ĳ�վ�ӵ�������
	//int ndt = m_dataStruct.NF0 - 1;
	//string nds = newbs_str[ndt];
	// nd[64];
	//strcpy(nd , nds.c_str());

	for (int k = 1; k <= m_dataStruct.M; k++)	//ÿ��֧·��Ҫ��
	{
		if (m_dataStruct.IZA[k] == 0)
			continue;	//֧·״̬Ϊ���ʾͣ��

		i = m_dataStruct.IZ1[k];	//�ڵ��1
		j = m_dataStruct.IZ2[k];	//�ڵ��2
		double x = m_dataStruct.Z5[k];	//�翹
		if (fabs(x) < 1e-10)
			continue;	//0�迹֧·

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

bool CSimpleFault::Run(SimpleFaultDataStruct& m_dataStruct, char * eFile, vector<int> newbs, vector<string> newbs_str)	//��·��������������
{
	int i,j,k,m,l,n;
	double pointI = 0;//�����ص���
	double gImp = 10;//�͵���
	double pointImp=0;//�ӵص���ɵ���
	double Us = 0;//�粽��ѹ
	int UsNd = -1;
	bool find = false;
	
	double maxpointI = 0;//�����ص���
	double maxUs = 0;//�粽��ѹ
	int maxUsNd = -1;

	bool varImp=false, varPoint=false;
	if (m_dataStruct.Z8 == -1)
		varPoint = true;
		
	if (m_dataStruct.Z12 == -1)//����������Ҫ����
		varImp = true;

	FILE *fp;
	fp = NULL;
	if ((fp = fopen(eFile, "a")) == NULL)
		return false;
	//�ڵ�  ����ӵأ�A��	B��	C��	����	����	����
	fprintf(fp, "�ӵ�����	�ڵ�	A��	B��	C��	����	����	����	A���ѹ	B���ѹ	C���ѹ �ӵص��� �ӵص���ɵ��� ���ݵ��� ��ص��� �粽��ѹ �粽��ѹ�ڵ��\n");
	if (fp != NULL) fclose(fp);

	SCOP(m_dataStruct);	//�ڵ��Ż����
	SCKP(m_dataStruct);	//���½ڵ���滻�ɽڵ��
	SCDP(m_dataStruct);	//�γ������ɾ���

	//��Ҫ���Ӷ�ÿ���ڵ�ĶԵع��ɵ��������
	//�����￪ʼ���м��㣬��ÿ���ڵ����ɨ��
		//1.�ж��Ƿ���㲻ͬ�Ľӵص���ɵ���
		//2.�ж��Ƿ��������
	if ((!varPoint) && (varImp))//����������Ҫ����
	{
		bool isExit = false;
		for (m = 10;m > 3;m--)
		{
			find = false;
			for (l = 1; l <= m_dataStruct.N; l++)
			{
				gImp = 3 * m;
				m_dataStruct.Z5[m_dataStruct.M] = sqrt(gImp * gImp + m_dataStruct.Z11*m_dataStruct.Z11);//�����Ǹ�֧·
				pointImp = m_dataStruct.Z8;
				pointI = RunSingle(m_dataStruct, eFile, newbs, newbs_str, l, gImp, pointImp);

//				if (!IsValid(m_dataStruct, pointI, Us, UsNd))
//				{
//					isExit = true;//ֻҪһ���ڵ㲻�ϸ񣬾Ͳ�Ҫ������
//					break;
//				}
			}
//			if (isExit)
//				break;
		}
	}
	else
	if ((varPoint) && (!varImp))//�����ӵص���ɵ�����Ҫ����
	{
		bool isExit = false;
		for (m = 80;m >= 0;m = m - 20)
		{
			find = false;
			for (l = 1; l <= m_dataStruct.N; l++)
			{
				pointImp = m_dataStruct.Z8 = 3 * m;
				gImp = m_dataStruct.Z12;
				m_dataStruct.Z5[m_dataStruct.M] = sqrt((gImp + pointImp)*(gImp + pointImp) + m_dataStruct.Z11*m_dataStruct.Z11);//�����Ǹ�֧·
				pointI = RunSingle(m_dataStruct, eFile, newbs, newbs_str, l, gImp, pointImp);
			}
			//	if (!IsValid(m_dataStruct, pointI, Us, UsNd))
			//	{
			//		isExit = true;//ֻҪһ���ڵ㲻�ϸ񣬾Ͳ�Ҫ������
			//		break;
			//	}
			//}
			//if (isExit)
			//	break;
		}
	}
	else
	if ((varPoint) && (varImp))//ȫ����Ҫ����
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
					m_dataStruct.Z5[m_dataStruct.M] = sqrt((gImp + pointImp)*(gImp + pointImp) + m_dataStruct.Z11*m_dataStruct.Z11);//�����Ǹ�֧·
					pointI = RunSingle(m_dataStruct, eFile, newbs, newbs_str, l, gImp, pointImp);
				}
			}
			//		if (!IsValid(m_dataStruct, pointI, Us, UsNd))
			//		{
			//			isExit = true;//ֻҪһ���ڵ㲻�ϸ񣬾Ͳ�Ҫ������
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
	else//������
	{
		gImp = m_dataStruct.Z12 + m_dataStruct.Z8;//�����Ǹ�֧·
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

	//���ѡ����С����ֵ
	//fp = NULL;
	//if ((fp = fopen(eFile, "a")) == NULL)
	//	return false;
	// nd[64];
	//nd[0] = 0;
	//if (maxUsNd >= 1)
	//	strcpy(nd,newbs_str[maxUsNd].c_str());
	//else
	//	strcpy(nd, "-");

	//fprintf(fp, "\n����С����ֵ��%f\t�ӵص���ɵ���ֵ��%f\t�����ص���ֵ��%f\t���粽��ѹֵ��%f\t���粽��ѹ�ڵ㣺%s", gImp/3, pointImp, maxpointI, maxUs, nd);
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

for(i = 0; i < m_dataStruct.M; i++)//����֧·��Ŀ
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
{//��������ͣ�������ڵ��ţ�������ĸ���翹��������Ĵ���̬�翹
fprintf(fp, "\n      IWGA=%4i  IWG=%4i  W3=%8.4f  W4=%8.4f \n",
m_dataStruct.IWGA[i],m_dataStruct.IWG[i],m_dataStruct.W3[i],m_dataStruct.W4[i]);
}
if(fp != NULL) fclose(fp);
*/




		//for (m = 1; m <= 4; m++)
		//{
		//	m_dataStruct.NF0 = l;//���Ͻڵ�
		//	m_dataStruct.IFA = m;//����ӵ�

		//	SCCP(m_dataStruct);	//�γ������ɾ���

		//	for (i = 1; i <= m_dataStruct.N; i++)	//�γɸ����ɾ���Խ�Ԫ��
		//	{
		//		m_dataStruct.D22[i] = m_dataStruct.D12[i];
		//	}

		//	//lld commit 2019-1-18,����û��������õ�ֵ�迹���
		//	for (i = 1; i <= m_dataStruct.IQ; i++)	//�����������緢���
		//	{
		//		if (m_dataStruct.IWGA[i] == 0) continue;
		//		j = m_dataStruct.IWG[i];//������Ľڵ��
		//		m_dataStruct.D12[j] += -1 / m_dataStruct.W4[i];	//����̬�翹
		//		m_dataStruct.D22[j] += -1 / m_dataStruct.W3[i];	//����翹
		//	}

		//	m_dataStruct.INt = m_dataStruct.INN;
		//	m_dataStruct.Dt = m_dataStruct.D12;
		//	m_dataStruct.Yt = m_dataStruct.Y2;
		//	m_dataStruct.IYt = m_dataStruct.IY;
		//	m_dataStruct.AJt = m_dataStruct.AJ1;
		//	m_dataStruct.IJt = m_dataStruct.IJ1;
		//	m_dataStruct.JFt = m_dataStruct.JF1;
		//	m_dataStruct.DKt = m_dataStruct.DK1;
		//	SCJP(m_dataStruct);	//���������ӷֽ�

		//	m_dataStruct.Dt = m_dataStruct.D22;
		//	m_dataStruct.Yt = m_dataStruct.Y2;
		//	m_dataStruct.IYt = m_dataStruct.IY;
		//	m_dataStruct.AJt = m_dataStruct.AJ2;
		//	m_dataStruct.IJt = m_dataStruct.IJ2;
		//	m_dataStruct.JFt = m_dataStruct.JF2;
		//	m_dataStruct.DKt = m_dataStruct.DK2;
		//	SCJP(m_dataStruct);	//���������ӷֽ�

		//	m_dataStruct.INt = m_dataStruct.IN0;
		//	m_dataStruct.Dt = m_dataStruct.D02;
		//	m_dataStruct.Yt = m_dataStruct.Y02;
		//	m_dataStruct.IYt = m_dataStruct.IY0;
		//	m_dataStruct.AJt = m_dataStruct.AJ0;
		//	m_dataStruct.IJt = m_dataStruct.IJ0;
		//	m_dataStruct.JFt = m_dataStruct.JF0;
		//	m_dataStruct.DKt = m_dataStruct.DK0;
		//	SCJP(m_dataStruct);	//���������ӷֽ�

		//	m_dataStruct.NF0 = m_dataStruct.INB[m_dataStruct.NF0];
		//	for (k = 1; k <= m_dataStruct.N; k++)
		//	{
		//		m_dataStruct.CK1[k] = 0;
		//		m_dataStruct.CK2[k] = 0;
		//		m_dataStruct.CK0[k] = 0;
		//	}

		//	switch (m_dataStruct.IFA)	//��·����
		//	{
		//	case 1:	//�����·
		//	case 4:	//�����·�ӵء����ﲻҪbreak���!
		//		m_dataStruct.CK0[m_dataStruct.NF0] = 1;

		//		m_dataStruct.AJt = m_dataStruct.AJ0;
		//		m_dataStruct.IJt = m_dataStruct.IJ0;
		//		m_dataStruct.JFt = m_dataStruct.JF0;
		//		m_dataStruct.CKt = m_dataStruct.CK0;
		//		m_dataStruct.DKt = m_dataStruct.DK0;
		//		SCWP(m_dataStruct);
		//	case 2:	//�����·
		//		m_dataStruct.CK2[m_dataStruct.NF0] = 1;

		//		m_dataStruct.AJt = m_dataStruct.AJ2;
		//		m_dataStruct.IJt = m_dataStruct.IJ2;
		//		m_dataStruct.JFt = m_dataStruct.JF2;
		//		m_dataStruct.CKt = m_dataStruct.CK2;
		//		m_dataStruct.DKt = m_dataStruct.DK2;
		//		SCWP(m_dataStruct);
		//	case 3:	//�����·
		//		m_dataStruct.CK1[m_dataStruct.NF0] = 1;

		//		m_dataStruct.AJt = m_dataStruct.AJ1;
		//		m_dataStruct.IJt = m_dataStruct.IJ1;
		//		m_dataStruct.JFt = m_dataStruct.JF1;
		//		m_dataStruct.CKt = m_dataStruct.CK1;
		//		m_dataStruct.DKt = m_dataStruct.DK1;
		//		SCWP(m_dataStruct);
		//	}

		//	//�����·��ĵ���ֵ
		//	SCRP(m_dataStruct, newbs, newbs_str);	//���������
		//	//��Ҫ�����ѹֵ
		//}

		//FILE *fp;
		//fp = NULL;
		//if ((fp = fopen(eFile, "a")) == NULL)
		//	return false;
		////�ڵ�  ����ӵأ�A��	B��	C��	����	����	����
		//fprintf(fp, "�ӵ�����	�ڵ�	A��	B��	C��	����	����	����	A���ѹ	B���ѹ	C���ѹ\n");
		//for (i = 0; i < scResult.size(); i++)//v.size() ��ʾvector����Ԫ�صĸ���
		//	fprintf(fp, scResult[i].c_str());
		//if (fp != NULL) fclose(fp);
	//}
