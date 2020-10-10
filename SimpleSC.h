#pragma once
#include "../DNASPLIB/pwrflow.h"

#define MaxNodeNumber 1000	//最大节点数
#define MaxBranchNumber 750	//最大支路数
#define MaxGeneratorNmber 100	//最大发电机数
#define MaxPVNodeNumber 30	//最大PV节点数
#define MaxBranchPerNode 50	//每个节点上挂的最大支路数

struct SimpleFaultDataStruct
{
	//节点、支路信息
	int N, M, IZA[MaxBranchNumber]/*支路状态，支路状态为零表示停运*/,
		IZ1[MaxBranchNumber], IZ2[MaxBranchNumber]/*首末节点*/;
	double Z1[MaxBranchNumber]/*正序电阻*/, Z2[MaxBranchNumber]/*正序电抗*/,
		Z3[MaxBranchNumber]/*正序电纳*/,
		Z4[MaxBranchNumber]/*零序电阻*/, Z5[MaxBranchNumber]/*零序电抗*/,
		Z6[MaxBranchNumber]/*零序电纳*/, Z9[MaxBranchNumber], Z10[MaxBranchNumber]/*节点的地网电阻，主要是厂站内的地网，要校验跨步电压*/;

	int SO;//电源母线节点编号
	double Z7;//上级电源点的电抗
	double Z8;//接地点过渡电阻值
	double Z11;//接地变和地网的总电阻
	double Z12;//接地电阻
	double capI;//电容电流

	//发电机信息
	int IWGA[MaxGeneratorNmber]/*发电机类别*/, IWG[MaxGeneratorNmber]/*发电机节点*/;
	double W3[MaxGeneratorNmber]/*负序电抗*/, W4[MaxGeneratorNmber]/*次暂态电抗*/;
	//对于三相对称的短路电流计算，发电机的负序和次暂态电抗好像用不到？
	//其他数据
	int N0, IQ/*发电机数量*/, IT;

	//内部数据;//
	//节点优化编号用
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

	int NF0, IFA;	//故障点和故障类型，其中NF0即书上的N0!

					//控制参数
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
	vector<int> newbs;//短路电流程序需要的从1开始的连续节点编号
//	vector<double> netImps;//接地网的电阻值，柱上变可能也有，和节点一一对应
	vector<string> newbs_str;//图形上的节点编号

	bool ReadData(SimpleFaultDataStruct& m_dataStruct);
	int OnMathSC(char * eFile);




};

