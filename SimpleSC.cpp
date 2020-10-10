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

//电源点的等值阻抗直接加到发电机正序节点上
//接地变和接地电阻直接加到零序网上
bool CSimpleSC::ReadData(SimpleFaultDataStruct& m_dataStruct)
{
//获取电气岛内的节点支路数量
	int		i, cntSec=0,sc_index=1;
	int		ibs, zbs;
	int		iout, zout, dead;
	double rr, xx;
	BS* bs = dsbob2->bs_tb->GetTable();

	std::vector<int> islandbs;//电气岛内的母线数量
	int iFind = 0;

	//传统的短路电流程序 节点编号只能是从1开始进行编号
	int newbsid = 1;
	int pos = -1;
	//get lnty table
	//	LNTY* lnty = dsbob->lnty_tb->GetTable();

	//get tapty table
	//	TAPTY* tapty = dsbob2->tapty_tb->GetTable();

	//需要修改和电源点相连的母线节点号，变成一个统一的号
	vector<int> sos;
	char  st_b[64], st_e[64];
	int so_bs = 20000;
	int bus_bs = 10000;//这个是认为增加的一个节点，表示对地支路和上级来的支路
	int lv_so = dsbob2->so_tb->GetCount();
	SO* so = dsbob2->so_tb->GetTable();
	for (i = 0; i < lv_so; i++)
		sos.push_back((so + i)->ibs);

	//read sec data into working branch array		读取线路表
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

				if (isFind(newbs, ibs, pos))//在新的号已经有了
				{
					m_dataStruct.IZ1[sc_index] = pos+1;//首节点 
				}
				else
				{
					m_dataStruct.IZ1[sc_index] = newbsid++;
					newbs.push_back(ibs);
				}
				if (isFind(newbs, zbs, pos))//在新的号已经有了
				{
					m_dataStruct.IZ2[sc_index] = pos + 1;//首节点 
				}
				else
				{
					m_dataStruct.IZ2[sc_index] = newbsid++;
					newbs.push_back(zbs);
				}

				m_dataStruct.IZA[sc_index] = 1;	//支路类型

				rr = (sec + i)->r0 * g_wbase / atof((sec + i)->vl) / atof((sec + i)->vl);
				xx = (sec + i)->x0 * g_wbase / atof((sec + i)->vl) / atof((sec + i)->vl);

				m_dataStruct.Z1[sc_index] = 0;
				m_dataStruct.Z2[sc_index] = sqrt(rr*rr+xx*xx);
				m_dataStruct.Z3[sc_index] = 0;//正序
				m_dataStruct.Z4[sc_index] = m_dataStruct.Z1[sc_index];//零序
				m_dataStruct.Z5[sc_index] = m_dataStruct.Z2[sc_index];//零序
				m_dataStruct.Z6[sc_index] = 0;//零序
				if ((strcmp(st_b, "-") == 0))
					m_dataStruct.Z9[sc_index] = 0;
				else
					m_dataStruct.Z9[sc_index] = atof((sec + i)->st_b);//接地点首段厂站的地网电阻
				if ((strcmp(st_e, "-") == 0))
					m_dataStruct.Z10[sc_index] = 0;
				else
					m_dataStruct.Z10[sc_index] = atof((sec + i)->st_e);//接地点首段厂站的地网电阻
				sc_index++;
			}
		}
	}

	m_dataStruct.N = islandbs.size()+1;//节点个数
	m_dataStruct.M = 2+cntSec;//支路个数，增加一个电源支路和对地支路

	//上级电源支路
	//找到电源点
	m_dataStruct.IZA[sc_index] = 1;	//支路类型
	if (isFind(newbs, so_bs, pos))//在新的号已经有了
	{
		m_dataStruct.IZ1[sc_index] = pos + 1;//首节点 
	}
	else
	{
		m_dataStruct.IZ1[sc_index] = newbsid++;//首节点 
		newbs.push_back(so_bs);
	}
	if (isFind(newbs, bus_bs, pos))//在新的号已经有了
	{
		m_dataStruct.IZ2[sc_index] = pos + 1;//首节点 
	}
	else
		m_dataStruct.IZ2[sc_index] = newbsid++;//末节点
	m_dataStruct.Z2[sc_index] = 0.0001;//正序，增加的非常小的支路,只要电抗
	m_dataStruct.Z3[sc_index] = 0;//正序
	m_dataStruct.Z4[sc_index] = 0;//
	m_dataStruct.Z5[sc_index] = 0.0001;//零序
	m_dataStruct.Z6[sc_index] = 0;//零序

	m_dataStruct.SO = m_dataStruct.IZ1[sc_index++];
	//xx = dsbob2->so_tb->GetDataByLog(0)->ss / 1.732;//上级等效，变成标么值
	xx = 10 / (1.732 * dsbob2->so_tb->GetDataByLog(0)->ss);//上级等效，变成标么值
	m_dataStruct.Z7 = xx;

	//增加一个对地支路
	m_dataStruct.IZA[sc_index] = 4;	//支路类型
	//ibs = dsbob2->so_tb->GetDataByLog(0)->ibs;//首节点 
	if (isFind(newbs, so_bs, pos))//在新的号已经有了
	{
		m_dataStruct.IZ1[sc_index] = pos + 1;//首节点 
	}
	else
	{
		m_dataStruct.IZ1[sc_index] = newbsid;//首节点 
		newbs.push_back(so_bs);
	}
	m_dataStruct.IZ2[sc_index] = m_dataStruct.IZ1[sc_index];//末节点，对地的虚拟点
	
	m_dataStruct.Z2[sc_index] = 0;//正序阻抗非常小
	m_dataStruct.Z11 = 3*dsbob2->so_tb->GetDataByLog(0)->sw;//接地变
	rr = 3*dsbob2->so_tb->GetDataByLog(0)->sr;//接地电阻
	if (rr==300000)
		m_dataStruct.Z12 = -1;//用模替代阻抗值
	else
		m_dataStruct.Z12 = rr;//用模替代阻抗值

	xx = 3*dsbob2->so_tb->GetDataByLog(0)->pLoss;//过渡电阻
	if (xx == 300000)
		m_dataStruct.Z8 = -1;//
	else
		m_dataStruct.Z8 = xx;//用模替代阻抗值

	m_dataStruct.capI = dsbob2->so_tb->GetDataByLog(0)->cap_I;//电容电流

	m_dataStruct.IQ = 0;//没有小电源

	for (i = 1; i <= m_dataStruct.IQ; i++)
	{//发电机类型，发电机节点编号，发电机的负序电抗，发电机的次暂态电抗
		m_dataStruct.IWGA[i] = 1;
		m_dataStruct.IWG[i] = dsbob2->so_tb->GetDataByLog(i-1)->ibs;
		m_dataStruct.W3[i] = 0;
		m_dataStruct.W4[i] =0;
	}

	//对应的图形上的节点
	int j,kk,ifind,index;
	char  nd[512];
	char nd_t[512];
	for (j = 0;j < newbs.size();j++)
	{
		kk = newbs[j];
		if ( (kk == 20000) || (kk == 10000))
		{
			newbs_str.push_back((so + 0)->nd);//都写到一个电源点上面*******
			continue;
		}
		ifind = 0;
		strcpy(nd , "");
		//一个计算母线对应多个节点号，尤其是T接的情况
		for (i = 0; i < lv_sec; i++)
		{
			if ((sec + i)->ibs == (sec + i)->zbs)//去掉连接线
				continue;

			ibs = (sec + i)->ibs;
			if (ibs==kk)
			{ 
				//如果里面有了，不需要再增加
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
