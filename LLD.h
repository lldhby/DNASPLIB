// LLD.h: interface for the CLLD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LLD_H__EE931DDF_40A2_46BA_B38B_7C54AB8CED32__INCLUDED_)
#define AFX_LLD_H__EE931DDF_40A2_46BA_B38B_7C54AB8CED32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "afxtempl.h"
//#include <afxcoll.h>
//#include <io.h>
#include <limits.h>
#include <sys/types.h>
#include <time.h>
//#include <unistd.h>

//#include <sys/stat.h> lld 2019-9-25


//#include <dirent.h>
#include <string.h>
//#include <libgen.h>

#include <vector>
#include <fstream>
#include <string>
using namespace std;

class CLLD 
{
public:
	CLLD();
	virtual ~CLLD();

//程序运行的时间戳
typedef struct stru_timeHere
{
	int id;			//时间戳的序号
	char CurTime[128];//以字符串表示的时间
	long timehere;	//时间
	char descr[128];	//时间戳描述
} stru_timeHere;

//////////////////////////////////////////////////////////////////////////
//多线程可靠性计算用到的全局变量
//	int g_begin_so,g_end_so,g_ld_ra;//需要计算的起止馈线,可靠性结果的索引（负荷）  
	//信号量与关键段  
//	HANDLE            g_hThreadParameter;  
//	CRITICAL_SECTION  g_csThreadCode; 

//////////////////////////////////////////////////////////////////////////


public:
//////////////////////////////////////////////////////////////////////////
//const


//////////////////////////////////////////////////////////////////////////
//全局的严重错误，需要抛出
	char globalexception[5000];
	int  num_globalexception;//严重错误的个数
	int  init_globalexception();//初始化
	int  add_globalexception(char* add);
//	int  out_globalexception(CStdioFile* file);
	int g_mathType;
	int MATH_VOLTAGELEVEL;//计算用的电压等级，10，110两个
		//type：
		//1:-用装见容量进行计算
		//2：-纯潮流计算
		//4：-一线一案（分段开关优化）
		//8：-短路电流
		//16：-N-1
		//32：-可靠性
		//64：-无功优化
		//128-用实时数据进行计算
	//const int CALCTYPE_LABEL = 1;
	const int CALCTYPE_FLOW = 2;
	const int CALCTYPE_OPTSECT = 4;
	const int CALCTYPE_SC = 8;
	const int CALCTYPE_NM1 = 16;
	const int CALCTYPE_RA = 32;
	const int CALCTYPE_RO = 64;
	const int CALCTYPE_REAL = 128;

	//int CALCTYPE;//默认用装见容量来做

//////////////////////////////////////////////////////////////////////////
	//每次计算所开的数组
	int		mx_digital;	//最多只有50000个遥信点
	int		mx_analog;
	int		mx_co;
	int		mx_st;//000
	int		mx_xfmr;
	int		mx_tapty;
	int		mx_xf;
	int		mx_cb;
	int		mx_so;
	int		mx_ld;
	int		mx_cp;
	int		mx_bus;
	int		mx_zn;
	int		mx_eq;

	int		mx_sec;
	int		mx_secp;
	int		mx_nd;
	int		mx_bs;
	int		mx_island;
	int		mx_global;
	int		mx_r_zn_ld;
	int		mx_r_zn_sec;
	int		mx_r_zn_cp;
	int		mx_r_zn_cb;
	int		mx_r_nd_cb;
	int		mx_r_nd_ld;
	int		mx_r_xfmr_xf;
	int		mx_r_nd_xf;
	int		mx_r_nd_zn;
	int		mx_r_nd_sec;
	int		mx_vl;

	//lld add 2016-6-3
	int		mx_r_co_st;
	int		mx_r_st_bus;
	int		mx_r_st_cb;
	int		mx_r_st_cp;
	int		mx_r_nd_cp;
	int		mx_r_st_ld;
	int		mx_r_st_so;
	int		mx_r_st_xfmr;
	//lld add end 2016-6-3


//lld add 2016-1-5
	int		mx_fd;	
	int		mx_un;	
	int		mx_r_fd_un;	
	int		mx_r_fd_zn;	
	int		mx_r_fd_eq;	
	int		mx_r_fd_so;
	int		mx_r_nd_eq;
	int		mx_r_fd_ld;
//lld add 2016-1-8
	int		mx_sql_char;
	int     mx_minpath_len;

//////////////////////////////////////////////////////////////////////////
	char g_userid[64];
//	char g_pwd[64];
//	char g_ver[32];
	char g_error[128];
	int g_ZeroSerialGrounding;
	const static int numlen = 16;
//////////////////////////////////////////////////////////////////////////
//可靠性需要的全局变量
	int  isGroupParam;
	double LINELOSS;//用首段电流折算时的线损率，需要根据情况调整，默认5%
	double UNIFACTOR;//统一功率因素一般设为0.95
	char trans_loadRate[numlen]/*负载率，用来计算运行方式。电源点和负荷点都要加上这个比例*/;

	char fault_ohline_prob[numlen]/*架空线故障率，次/年/100km*/, fault_ohline_rtime[numlen]/*修复时间（分钟）*/;
	char fault_ohline_ins_prob[numlen]/*架空绝缘线故障率，次/年/100km*/, fault_ohline_ins_rtime[numlen]/*修复时间（分钟）*/;
	char fault_cable_prob[numlen]/*电缆*/, fault_cable_rtime[numlen]/**/;
	char fault_breaker_prob[numlen]/*断路器，次/年/100台*/, fault_breaker_rtime[numlen];
	char fault_loadbreakswitch_prob[numlen]/*负荷开关*/, fault_loadbreakswitch_rtime[numlen]/**/;
	char fault_disconnector_prob[numlen]/*隔离开关*/, fault_disconnector_rtime[numlen]/**/;
	char fault_fuse_prob[numlen]/*熔断器*/, fault_fuse_rtime[numlen]/**/;
	char fault_transformer_prob[numlen]/*变压器*/, fault_transformer_rtime[numlen]/**/;

	char fault_ltime[numlen]/*馈线故障定位时间*/, plan_ltime[numlen]/*预安排定位时间*/;
	char fault_stime[numlen]/*馈线切换时间*/,     plan_stime[numlen]/*计划停电切换*/;
	char fault_ttime[numlen]/*馈线转供时间*/,     plan_ttime[numlen];/*计划停电转供时间*/

	char plan_prob[numlen]/*计划停电率*/, plan_rtime[numlen];/*计划停电时间*/

	int g_cnt_nm1_fd,g_cnt_ro_fd,g_ld_ra,g_begin_so,g_end_so,g_cnt_sc_fd;

///无功优化需要
	double alpha_a/*折旧维修率*/;
	double gamma_a/*投资回收率*/;
	double beta_a/*电价*/;
	double Kc_a/*单位容量补偿设备投资*/;
	double tau_a/*全网最大负荷损耗小时数*/;


//////////////////////////////////////////////////////////////////////////


public:
//////////////////////////////////////////////////////////////////////////


	int copyfile(char *SourceFile, char *NewFile);
	bool isSame(int *a, int *b, size_t lenA, size_t lenB);
	int isInInt(int src, int* Int, int len);

	//判断一个vector里面是否有某个元素，如果有返回该元素的个数
	int isFind(std::vector<string> tts, string tt);
	int isFind(std::vector<int> tts, int tt);

	//从一个矩阵里面获得新的子矩阵
	//JudgeMatrix-原矩阵；subJudgeMatrix-子矩阵；i_b,j_b,i_e,j_e-开始位置和结束位置
	void OnGetSubMatrix(vector<vector<double>> JudgeMatrix, vector<vector<double>>& subJudgeMatrix, int i_b, int j_b, int i_e, int j_e);
	//给定一个矩阵，得到最大特征向量
	//JudgeMatrix-给定的矩阵；t_col-返回值，最大特征向量
	void OnGetLamdaMax(vector<vector<double>> JudgeMatrix, vector<double>& t_col);
	
	int isInArray(int** Int, int x, int y, int* pic);
	//给定一个整形的数组，得到其中的不重复的随机数
	//sect整型数组的范围,比如[0-200], lenPic需要获得的位数，比如3位数276
	//int** retInt 返回的数组, int retNum 返回的个数
	//int** intlist = NULL;
	//intlist = (int**)calloc(retNum, sizeof(int*));
	//for (ii = 0; ii < retNum; ++ii)
	//	intlist[ii] = (int*)calloc(lenPic, sizeof(int));
	//for (ii = 0; ii < retNum; ++ii)
	//	for (jj = 0; jj < lenPic; ++jj)
	//		intlist[ii][jj] = -1;
	//ret = OnGetRandom(200, lenPic, intlist, retNum);
	//int a, b, c;
	//for (ii = 0; ii < retNum; ++ii)
	//{
	//	a = *(*(intlist + ii) + 0);
	//	b = *(*(intlist + ii) + 1);
	//	c = *(*(intlist + ii) + 2);
	//	printf("%d %d %d\n", a, b, c);
	//}
	//for (ii = 0; ii < retNum; ++ii)
	//{
	//	if (NULL != intlist[ii])
	//		free(intlist[ii]);
	//}
	//free(intlist);
	//intlist = 0;
	int OnGetRandom(int sect, int lenPic, int** retInt, int retNum);


    void splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);

	//将一个vector转换成以" "分开的字符串
	void OnVector2String(vector<string> vstring, char* qota,char* out);

	double OnGetStandardDev(double* dList, int cnt, bool isOnlyNormalization = false);

	bool isFind(std::vector<string> tts, char* tt, int& pos);
	bool isFind(std::vector<int> tts, int tt, int& pos);
	void getRightByleft(std::vector<std::string> src, std::vector<std::string> dst, char* left, char* key);
	void getRightByleft(std::vector<std::string> src, char* left, char* right, char* key);
	std::vector<std::string> splitWithStl(const std::string &str, const std::string &pattern);

	////字符串2整形 atoi();atof;
	char* IntToString(int iValue);		//整形转字符串
	char* IntToString(int iValue,int wide); 

	char* FloatToString(float iValue,int wide=0);	//浮点形转字符串
	char* DoubleToString(double iValue);	//双精度形转字符串
	char* DoubleToString(double iValue,int wide) ;


	//简单的写日志文件，将文件打开，写东西
	char logfile[128];
	void WriteLog(char* logfile, char* szLog);
/*
//logfile
	//在程序的运行目录处得到日志文件
	//如果文件名不为空，说明是指定的文件，日志在后面追加就可以了
	//如果文件名为空，创建新的文件，直接用日期取代，默认是写入时间
	int openLogFile(* fileName=NULL);
	//加入一行日志
	int addLogLine(* str,bool addtime=false);
	//关闭日志文件
	int closeLogFile();
*/
//////////////////////////////////////////////////////////////////////////
//file
	//将一个文件附加到另一个文件的末尾
	void filecat(char* srcfile,char* catfile);
	int ReNameFile(char *source,char *newname);
	int DelFile(char *source);
	int CopyFile(char *SourceFile,char *NewFile);

	void getCurTime(char* curTime,int flag=0);

	//给定一个文件名，解析其中的目录和文件名
	int getFileDir_Name(char* sFileName, char* dir, char* fname);

	int getExecutePath(char* exePath);
	//获取环境变量的路径
	int getEnvironmentVariable(char* sEnvironmentName,char* outEnvVar);
	//获得一个给定目录的全部文件，放到一个结构里面
	void getFilesall( string path, vector<string>& files);  

	//将一行用空格隔开的文本，拆成相应的字段
	// 1  3  2  4  //行中的文本
	// 0  0  1  0  //0表示字符串；1整数
	//fil[4];datatype[4];n_fil=4;src=12
	//返回值为： "%s%s%d%s"
	void getFilter(int* fil,int* datatype,char* ret);

//////////////////////////////////////////////////////////////////////////
//integer
	//获得一个整型数据在最后一个有效的数字-5，以前都用-1填充
	//{2,3,1,2,-2,-5,-1,-1,-1}
	int getLastValid(int* src,int len);
	int sortInt(int* dd,int cnt);//整形排序
	int sortint_Desc(int* dd,int cnt);//整形排序
	int sortDouble(double* dd,int cnt);
	int sortDouble_Desc(double* dd,int cnt);//降序
	void dsort(double& a,double& b);
//////////////////////////////////////////////////////////////////////////
//string
	void insertchar(char* src,int pos,char* dst);//在src指定的pos位置后插入字符串dst
	void substr(char* src,int pos,int len,char* dst);//在src字符串中的第pos的后面开始取len个字符给dst
	//删掉字符串左边的n个字符
	void delstrleft(char* src,int leftlen,char* dst);
	//删掉字符串右边的n个字符
	void delstrright(char* src,int rightlen,char* dst);
	void delstrright(char* src,int rightlen);
	//判断一个字符串是否给定位置含有某个元素，0-全部找，+左边数位置，-右边数位置
	int isContainor(char* src,char substr,int pos=0);

	void trim_return(char* src);
	//将一个字符串中的空格转化成-
	//void trimsapce(char* s);
	char* trimsapce(char* s);
	//字符串src中间用关键字key隔开很多字符,对他进行解析，将结果放到strList里面
	int strparse(char* src,char key,char** strlist);

	void delstr(char* src,int start,int end,char* dst);

	//取src的左边的第一个以qota分隔的部分，放到dst里面，getleftstr("10.22/10.33","/",dst),dst为10.22
	int getleftstr(char* src,char qota,char* dst); 
	int getrightstr(char* src,char qota,char* dst);

	//根据解析出来的字符串数组，设定计算参数paramtype-0为潮流，1短路，2可靠性，4 n-1
	void setMathParm(char** param,int num,int paramtype=2);
	void getstrByIndex(char** strlist,int index,char* dst);

	//sSrc 源字符串，sMatchStr 需要替换的字符串，sReplaceStr 替换完后的字符串
	int ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr);
	char *replacestr(char *strbuf, char *sstr, char *dstr);

//////////////////////////////////////////////////////////////////////////

	//专门为四川做一个日期解析函数,是昨天就返回1，否则返回0
	int get_SICHUAN_date(char* filename,char* yyyymmdd,int& time_index);
	int get_SICHUAN_ftpfilename(char* filename,char* yestDt1,char* pre,char* posx);

//////////////////////////////////////////////////////////////////////////
	//向一个整形的数组中加入一个节点，如果没有，加到后面，返回数组的位置
	//如果已经有，返回0; pos插入位置
	int addNode(int* nodelist,int node,int& pos);
	
	//找到节点,默认是不需要排序（已经排好序）
	//nodelist 整型的数组,node整型值,int& pos整型个数，返回在整型中的位置,order=0默认排好序
	//返回值-1 没有找到，其它为位置，一定是大于或等于0
	int findNode(int* nodelist,int node,int pos,int order=0);

	//一个节点在整型数组里面存在  intList整型数组,node查找的节点,cnt数组中元素的个数
	int IsExistInArray(int* intList,int node,int cnt);
	//在一个数组中删掉元素，返回为最后剩下的元素个数
	int removeInArray(int* intList,int node,int cnt);

	int getLR_Bykey(char* src,char* key,char* left,char* right);
//////////////////////////////////////////////////////////////////////////
	//对一个向量进行转置,其中分全部转置和有效元素转置
	//src需要转置的向量；len向量长度；dst转换后的向量；type默认全转置>0
	//只有小于等于0的时候才是有效元素的转置，2015-3-10
	void converse_vector(int* src,int len,int* dst,int type=1);




	//src源向量，comp1,返回分叉的位置（从源开始数）,ele为元素
	int comp_vector(int* src,int* comp,int& ele);//比较2个向量
	//int ele_vector(int* src, int ele, int len=0);//ele是否在向量中，是返回位置，否返回-1
	//向量中两个元素进行比较，先出现返回1，后出现返回2，都没有，返回-1
	int ele_comp(int* src, int ele1, int ele2);
	//给定设备的到电源点最小路，负荷点的最小路，求取它们的交点,cross_nd节点
	//负荷点：4,2,1,0,-1,-1,-1;电源点：5,1,0,-1,-1,-1,-1
	//返回2，cross_nd=1
	int getCrossPoint(int* src,int srclen,int* cmp,int cmplen,int& cross_nd);
	int ele_vector(int* src, int ele, int len);

//////////////////////////////////////////////////////////////////////////
//lld add 2018-4-21
//	int getRandomCB(int cbNum,CStringList& cbSerial);
//	//int getRandomCB(int cbNum,char** cbSerial);
//	
//	CString getStringByIndex(CStringList* strList,int ii);
//	int getLR_Bykey(CString src,CString key,CString& left,CString& right);
//	int trim(CString s,BOOL upper=TRUE);


//////////////////////////////////////////////////////////////////////////

	long getCurMill();//输出所有的时间差
	void TimeHere(char* here);
	int  OutputAllTime(char* outall);//输出所有的时间差

private:
	int i_here;
	//最多50处记录当前的时间
	stru_timeHere t_here[500];
	//CList<CString,CString&> m_list;

	//打时标，here标注当前的时间的说明

};

#endif // !defined(AFX_LLD_H__EE931DDF_40A2_46BA_B38B_7C54AB8CED32__INCLUDED_)
