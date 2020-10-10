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
	bool SCOP(SimpleFaultDataStruct& m_dataStruct);	//�ڵ��Ż����
	bool SCKP(SimpleFaultDataStruct& m_dataStruct);	//���½ڵ���滻�ɽڵ��

	bool SCDP(SimpleFaultDataStruct& m_dataStruct);
	bool SCCP(SimpleFaultDataStruct& m_dataStruct);

	bool SCJP(SimpleFaultDataStruct& m_dataStruct);
	bool SCWP(SimpleFaultDataStruct& m_dataStruct);

	double SCRP(SimpleFaultDataStruct& m_dataStruct, vector<int> newbs, vector<string> newbs_str, double gImp, double pointImp);	//���������

public:
	bool Run(SimpleFaultDataStruct& m_dataStruct, char * eFile,vector<int> newbs, vector<string> newbs_str);	//��·��������������
	double RunSingle(SimpleFaultDataStruct& m_dataStruct, char * eFile, vector<int> newbs, vector<string> newbs_str, int l, double gImp, double pointImp);	//��·��������������
	bool IsValid(SimpleFaultDataStruct& m_dataStruct, double pI, double& Us, int& UsNd);
	//���������
	vector<string> scResult;

};

#endif // !defined(AFX_SIMPLEFAULT_H__23CD423C_3072_4D8E_B7B8_448B17AAFBE2__INCLUDED_)
