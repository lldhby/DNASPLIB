#pragma once
#include "../DNASPLIB/pwrflow.h"

#define MaxNodeNumber 1000	//���ڵ���
#define MaxBranchNumber 750	//���֧·��
#define MaxGeneratorNmber 100	//��󷢵����
#define MaxPVNodeNumber 30	//���PV�ڵ���
#define MaxBranchPerNode 50	//ÿ���ڵ��Ϲҵ����֧·��

struct SimpleFaultDataStruct
{
	//�ڵ㡢֧·��Ϣ
	int N, M, IZA[MaxBranchNumber]/*֧·״̬��֧·״̬Ϊ���ʾͣ��*/,
		IZ1[MaxBranchNumber], IZ2[MaxBranchNumber]/*��ĩ�ڵ�*/;
	double Z1[MaxBranchNumber]/*�������*/, Z2[MaxBranchNumber]/*����翹*/,
		Z3[MaxBranchNumber]/*�������*/,
		Z4[MaxBranchNumber]/*�������*/, Z5[MaxBranchNumber]/*����翹*/,
		Z6[MaxBranchNumber]/*�������*/, Z9[MaxBranchNumber], Z10[MaxBranchNumber]/*�ڵ�ĵ������裬��Ҫ�ǳ�վ�ڵĵ�����ҪУ��粽��ѹ*/;

	int SO;//��Դĸ�߽ڵ���
	double Z7;//�ϼ���Դ��ĵ翹
	double Z8;//�ӵص���ɵ���ֵ
	double Z11;//�ӵر�͵������ܵ���
	double Z12;//�ӵص���
	double capI;//���ݵ���

	//�������Ϣ
	int IWGA[MaxGeneratorNmber]/*��������*/, IWG[MaxGeneratorNmber]/*������ڵ�*/;
	double W3[MaxGeneratorNmber]/*����翹*/, W4[MaxGeneratorNmber]/*����̬�翹*/;
	//��������ԳƵĶ�·�������㣬������ĸ���ʹ���̬�翹�����ò�����
	//��������
	int N0, IQ/*���������*/, IT;

	//�ڲ�����;//
	//�ڵ��Ż������
	int IX[MaxNodeNumber][MaxBranchPerNode], ID[MaxNodeNumber];

	double YZ2[MaxBranchNumber * 2];
	int IY1[MaxBranchNumber * 2], IY2[MaxBranchNumber * 2];

	int INB[MaxNodeNumber], INA[MaxNodeNumber], INN[MaxNodeNumber], IN0[MaxNodeNumber];
	double D12[MaxNodeNumber], D22[MaxNodeNumber], D02[MaxNodeNumber];

	int IY[MaxBranchNumber * 2], IY0[MaxBranchNumber * 2];
	double Y2[MaxBranchNumber * 2], Y02[MaxBranchNumber * 2];

	double DK1[MaxNodeNumber], DK2[MaxNodeNumber], DK0[MaxNodeNumber];
	double CK1[MaxNodeNumber], CK2[MaxNodeNumber], CK0[MaxNodeNumber];

	int IJ1[MaxBranchNumber * 20], JF1[MaxNodeNumber], IJ2[MaxBranchNumber * 20], JF2[MaxNodeNumber],
		IJ0[MaxBranchNumber * 20], JF0[MaxNodeNumber];
	double AJ1[MaxBranchNumber * 20], AJ2[MaxBranchNumber * 20], AJ0[MaxBranchNumber * 20];

	int* INt, *IYt, *IJt, *JFt, JK[MaxBranchNumber / 4];
	double *Dt, *Yt, *AJt, *DKt, *CKt, AK[MaxBranchNumber / 4];

	int NF0, IFA;	//���ϵ�͹������ͣ�����NF0�����ϵ�N0!

					//���Ʋ���
	int iErrorCode;
};

class CSimpleSC :
	public PWRFLOW
{
public:
	CSimpleSC();
	CSimpleSC(DSBOB2* m_pDb, CLLD* m_pLLD);
	
	virtual ~CSimpleSC();
	
	bool isFind(std::vector<int> tts, int tt, int& pos);
	vector<int> newbs;//��·����������Ҫ�Ĵ�1��ʼ�������ڵ���
//	vector<double> netImps;//�ӵ����ĵ���ֵ�����ϱ����Ҳ�У��ͽڵ�һһ��Ӧ
	vector<string> newbs_str;//ͼ���ϵĽڵ���

	bool ReadData(SimpleFaultDataStruct& m_dataStruct);
	int OnMathSC(char * eFile);




};

