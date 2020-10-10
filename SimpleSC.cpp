#include "../DNASPLIB/SimpleSC.h"
#include "../DNASPLIB/SimpleFault.h"
#include "math.h"

CSimpleSC::CSimpleSC()
{
}

CSimpleSC::CSimpleSC(DSBOB2* m_pDb, CLLD* m_pLLD)
{
	dsbob2 = m_pDb;
	lld = m_pLLD;
}

CSimpleSC::~CSimpleSC()
{
}

bool CSimpleSC::isFind(std::vector<int> tts, int tt,int& pos)
{
	int iFind = 0;
	int index = 0;
	vector<int>::iterator t;
	for (t = tts.begin(); t != tts.end(); t++)
	{
		if (*t == tt)
		{
			pos = index;
			iFind = 1;
			break;
		}
		index++;
	}
	if (iFind == 0)
		return false;
	else
		return true;
}

//��Դ��ĵ�ֵ�迹ֱ�Ӽӵ����������ڵ���
//�ӵر�ͽӵص���ֱ�Ӽӵ���������
bool CSimpleSC::ReadData(SimpleFaultDataStruct& m_dataStruct)
{
//��ȡ�������ڵĽڵ�֧·����
	int		i, cntSec=0,sc_index=1;
	int		ibs, zbs;
	int		iout, zout, dead;
	double rr, xx;
	BS* bs = dsbob2->bs_tb->GetTable();

	std::vector<int> islandbs;//�������ڵ�ĸ������
	int iFind = 0;

	//��ͳ�Ķ�·�������� �ڵ���ֻ���Ǵ�1��ʼ���б��
	int newbsid = 1;
	int pos = -1;
	//get lnty table
	//	LNTY* lnty = dsbob->lnty_tb->GetTable();

	//get tapty table
	//	TAPTY* tapty = dsbob2->tapty_tb->GetTable();

	//��Ҫ�޸ĺ͵�Դ��������ĸ�߽ڵ�ţ����һ��ͳһ�ĺ�
	vector<int> sos;
	char  st_b[64], st_e[64];
	int so_bs = 20000;
	int bus_bs = 10000;//�������Ϊ���ӵ�һ���ڵ㣬��ʾ�Ե�֧·���ϼ�����֧·
	int lv_so = dsbob2->so_tb->GetCount();
	SO* so = dsbob2->so_tb->GetTable();
	for (i = 0; i < lv_so; i++)
		sos.push_back((so + i)->ibs);

	//read sec data into working branch array		��ȡ��·��
	int lv_sec = dsbob2->sec_tb->GetCount();
	SEC* sec = dsbob2->sec_tb->GetTable();
	if (lv_sec != 0)
	{
		for (i = 0; i < lv_sec; i++)
		{
			ibs = (sec + i)->ibs;
			zbs = (sec + i)->zbs;
			strcpy(st_b, (sec + i)->st_b);
			strcpy(st_e, (sec + i)->st_e);

			if (ibs == zbs)
				continue;

			//if (strstr((sec + i)->id, "PD_20100000_187180") != NULL)
			//{
			//	int deu = 0;
			//}

			iout = (sec + i)->topflags & SEC_IOPEN;
			zout = (sec + i)->topflags & SEC_ZOPEN;
		
			dead = iout && zout;

			if ((sec + i)->r_zn == -1)
				dead = 1;

			(sec + i)->ialgzi = 0;

			if (!dead)
			{
				if (isFind(sos, ibs, pos))
					ibs = bus_bs;
				if (!isFind(islandbs, ibs, pos))
					islandbs.push_back(ibs);

				if (isFind(sos, zbs, pos))
					zbs = bus_bs;
				if (!isFind(islandbs, zbs, pos))
					islandbs.push_back(zbs);

				cntSec++;

				if (isFind(newbs, ibs, pos))//���µĺ��Ѿ�����
				{
					m_dataStruct.IZ1[sc_index] = pos+1;//�׽ڵ� 
				}
				else
				{
					m_dataStruct.IZ1[sc_index] = newbsid++;
					newbs.push_back(ibs);
				}
				if (isFind(newbs, zbs, pos))//���µĺ��Ѿ�����
				{
					m_dataStruct.IZ2[sc_index] = pos + 1;//�׽ڵ� 
				}
				else
				{
					m_dataStruct.IZ2[sc_index] = newbsid++;
					newbs.push_back(zbs);
				}

				m_dataStruct.IZA[sc_index] = 1;	//֧·����

				rr = (sec + i)->r0 * g_wbase / atof((sec + i)->vl) / atof((sec + i)->vl);
				xx = (sec + i)->x0 * g_wbase / atof((sec + i)->vl) / atof((sec + i)->vl);

				m_dataStruct.Z1[sc_index] = 0;
				m_dataStruct.Z2[sc_index] = sqrt(rr*rr+xx*xx);
				m_dataStruct.Z3[sc_index] = 0;//����
				m_dataStruct.Z4[sc_index] = m_dataStruct.Z1[sc_index];//����
				m_dataStruct.Z5[sc_index] = m_dataStruct.Z2[sc_index];//����
				m_dataStruct.Z6[sc_index] = 0;//����
				if ((strcmp(st_b, "-") == 0))
					m_dataStruct.Z9[sc_index] = 0;
				else
					m_dataStruct.Z9[sc_index] = atof((sec + i)->st_b);//�ӵص��׶γ�վ�ĵ�������
				if ((strcmp(st_e, "-") == 0))
					m_dataStruct.Z10[sc_index] = 0;
				else
					m_dataStruct.Z10[sc_index] = atof((sec + i)->st_e);//�ӵص��׶γ�վ�ĵ�������
				sc_index++;
			}
		}
	}

	m_dataStruct.N = islandbs.size()+1;//�ڵ����
	m_dataStruct.M = 2+cntSec;//֧·����������һ����Դ֧·�ͶԵ�֧·

	//�ϼ���Դ֧·
	//�ҵ���Դ��
	m_dataStruct.IZA[sc_index] = 1;	//֧·����
	if (isFind(newbs, so_bs, pos))//���µĺ��Ѿ�����
	{
		m_dataStruct.IZ1[sc_index] = pos + 1;//�׽ڵ� 
	}
	else
	{
		m_dataStruct.IZ1[sc_index] = newbsid++;//�׽ڵ� 
		newbs.push_back(so_bs);
	}
	if (isFind(newbs, bus_bs, pos))//���µĺ��Ѿ�����
	{
		m_dataStruct.IZ2[sc_index] = pos + 1;//�׽ڵ� 
	}
	else
		m_dataStruct.IZ2[sc_index] = newbsid++;//ĩ�ڵ�
	m_dataStruct.Z2[sc_index] = 0.0001;//�������ӵķǳ�С��֧·,ֻҪ�翹
	m_dataStruct.Z3[sc_index] = 0;//����
	m_dataStruct.Z4[sc_index] = 0;//
	m_dataStruct.Z5[sc_index] = 0.0001;//����
	m_dataStruct.Z6[sc_index] = 0;//����

	m_dataStruct.SO = m_dataStruct.IZ1[sc_index++];
	//xx = dsbob2->so_tb->GetDataByLog(0)->ss / 1.732;//�ϼ���Ч����ɱ�ôֵ
	xx = 10 / (1.732 * dsbob2->so_tb->GetDataByLog(0)->ss);//�ϼ���Ч����ɱ�ôֵ
	m_dataStruct.Z7 = xx;

	//����һ���Ե�֧·
	m_dataStruct.IZA[sc_index] = 4;	//֧·����
	//ibs = dsbob2->so_tb->GetDataByLog(0)->ibs;//�׽ڵ� 
	if (isFind(newbs, so_bs, pos))//���µĺ��Ѿ�����
	{
		m_dataStruct.IZ1[sc_index] = pos + 1;//�׽ڵ� 
	}
	else
	{
		m_dataStruct.IZ1[sc_index] = newbsid;//�׽ڵ� 
		newbs.push_back(so_bs);
	}
	m_dataStruct.IZ2[sc_index] = m_dataStruct.IZ1[sc_index];//ĩ�ڵ㣬�Եص������
	
	m_dataStruct.Z2[sc_index] = 0;//�����迹�ǳ�С
	m_dataStruct.Z11 = 3*dsbob2->so_tb->GetDataByLog(0)->sw;//�ӵر�
	rr = 3*dsbob2->so_tb->GetDataByLog(0)->sr;//�ӵص���
	if (rr==300000)
		m_dataStruct.Z12 = -1;//��ģ����迹ֵ
	else
		m_dataStruct.Z12 = rr;//��ģ����迹ֵ

	xx = 3*dsbob2->so_tb->GetDataByLog(0)->pLoss;//���ɵ���
	if (xx == 300000)
		m_dataStruct.Z8 = -1;//
	else
		m_dataStruct.Z8 = xx;//��ģ����迹ֵ

	m_dataStruct.capI = dsbob2->so_tb->GetDataByLog(0)->cap_I;//���ݵ���

	m_dataStruct.IQ = 0;//û��С��Դ

	for (i = 1; i <= m_dataStruct.IQ; i++)
	{//��������ͣ�������ڵ��ţ�������ĸ���翹��������Ĵ���̬�翹
		m_dataStruct.IWGA[i] = 1;
		m_dataStruct.IWG[i] = dsbob2->so_tb->GetDataByLog(i-1)->ibs;
		m_dataStruct.W3[i] = 0;
		m_dataStruct.W4[i] =0;
	}

	//��Ӧ��ͼ���ϵĽڵ�
	int j,kk,ifind,index;
	char  nd[512];
	char nd_t[512];
	for (j = 0;j < newbs.size();j++)
	{
		kk = newbs[j];
		if ( (kk == 20000) || (kk == 10000))
		{
			newbs_str.push_back((so + 0)->nd);//��д��һ����Դ������*******
			continue;
		}
		ifind = 0;
		strcpy(nd , "");
		//һ������ĸ�߶�Ӧ����ڵ�ţ�������T�ӵ����
		for (i = 0; i < lv_sec; i++)
		{
			if ((sec + i)->ibs == (sec + i)->zbs)//ȥ��������
				continue;

			ibs = (sec + i)->ibs;
			if (ibs==kk)
			{ 
				//����������ˣ�����Ҫ������
				strcpy(nd_t,(sec + i)->nd);
				if (strstr(nd,nd_t ) == NULL)
				{
					strcat(nd, (sec + i)->nd);
					strcat(nd, "+");
				}
			}

			zbs = (sec + i)->zbs;
			if (zbs == kk)
			{
				strcpy(nd_t, (sec + i)->znd);
				if (strstr(nd, nd_t) == NULL)
				{
					strcat(nd, (sec + i)->znd);
					strcat(nd, "+");
				}
			}
		}
		lld->delstrright(nd, 1);
		newbs_str.push_back(nd);
	}
	return true;
}

int CSimpleSC::OnMathSC(char * eFile)
{
	SimpleFaultDataStruct m_dataStruct;
	ReadData(m_dataStruct);
	CSimpleFault m_formYMatrix;
	m_formYMatrix.Run(m_dataStruct,eFile, newbs,newbs_str);

	return 1;
}
