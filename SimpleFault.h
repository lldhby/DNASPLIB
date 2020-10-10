// SimpleFault.h: interface for the CSimpleFault class.
//
//////////////////////////////////////////////////////////////////////
#include "../DNASPLIB/SimpleSC.h"

#if !defined(AFX_SIMPLEFAULT_H__23CD423C_3072_4D8E_B7B8_448B17AAFBE2__INCLUDED_)
#define AFX_SIMPLEFAULT_H__23CD423C_3072_4D8E_B7B8_448B17AAFBE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimpleFault  
{
public:
	CSimpleFault();
	virtual ~CSimpleFault();

private:
	bool SCOP(SimpleFaultDataStruct& m_dataStruct);	//节点优化编号
	bool SCKP(SimpleFaultDataStruct& m_dataStruct);	//用新节点号替换旧节点号

	bool SCDP(SimpleFaultDataStruct& m_dataStruct);
	bool SCCP(SimpleFaultDataStruct& m_dataStruct);

	bool SCJP(SimpleFaultDataStruct& m_dataStruct);
	bool SCWP(SimpleFaultDataStruct& m_dataStruct);

	double SCRP(SimpleFaultDataStruct& m_dataStruct, vector<int> newbs, vector<string> newbs_str, double gImp, double pointImp);	//计算结果输出

public:
	bool Run(SimpleFaultDataStruct& m_dataStruct, char * eFile,vector<int> newbs, vector<string> newbs_str);	//短路电流计算主程序
	double RunSingle(SimpleFaultDataStruct& m_dataStruct, char * eFile, vector<int> newbs, vector<string> newbs_str, int l, double gImp, double pointImp);	//短路电流计算主程序
	bool IsValid(SimpleFaultDataStruct& m_dataStruct, double pI, double& Us, int& UsNd);
	//输出计算结果
	vector<string> scResult;

};

#endif // !defined(AFX_SIMPLEFAULT_H__23CD423C_3072_4D8E_B7B8_448B17AAFBE2__INCLUDED_)
