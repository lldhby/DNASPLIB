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

//�������е�ʱ���
typedef struct stru_timeHere
{
	int id;			//ʱ��������
	char CurTime[128];//���ַ�����ʾ��ʱ��
	long timehere;	//ʱ��
	char descr[128];	//ʱ�������
} stru_timeHere;

//////////////////////////////////////////////////////////////////////////
//���߳̿ɿ��Լ����õ���ȫ�ֱ���
//	int g_begin_so,g_end_so,g_ld_ra;//��Ҫ�������ֹ����,�ɿ��Խ�������������ɣ�  
	//�ź�����ؼ���  
//	HANDLE            g_hThreadParameter;  
//	CRITICAL_SECTION  g_csThreadCode; 

//////////////////////////////////////////////////////////////////////////


public:
//////////////////////////////////////////////////////////////////////////
//const


//////////////////////////////////////////////////////////////////////////
//ȫ�ֵ����ش�����Ҫ�׳�
	char globalexception[5000];
	int  num_globalexception;//���ش���ĸ���
	int  init_globalexception();//��ʼ��
	int  add_globalexception(char* add);
//	int  out_globalexception(CStdioFile* file);
	int g_mathType;
	int MATH_VOLTAGELEVEL;//�����õĵ�ѹ�ȼ���10��110����
		//type��
		//1:-��װ���������м���
		//2��-����������
		//4��-һ��һ�����ֶο����Ż���
		//8��-��·����
		//16��-N-1
		//32��-�ɿ���
		//64��-�޹��Ż�
		//128-��ʵʱ���ݽ��м���
	//const int CALCTYPE_LABEL = 1;
	const int CALCTYPE_FLOW = 2;
	const int CALCTYPE_OPTSECT = 4;
	const int CALCTYPE_SC = 8;
	const int CALCTYPE_NM1 = 16;
	const int CALCTYPE_RA = 32;
	const int CALCTYPE_RO = 64;
	const int CALCTYPE_REAL = 128;

	//int CALCTYPE;//Ĭ����װ����������

//////////////////////////////////////////////////////////////////////////
	//ÿ�μ�������������
	int		mx_digital;	//���ֻ��50000��ң�ŵ�
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
//�ɿ�����Ҫ��ȫ�ֱ���
	int  isGroupParam;
	double LINELOSS;//���׶ε�������ʱ�������ʣ���Ҫ�������������Ĭ��5%
	double UNIFACTOR;//ͳһ��������һ����Ϊ0.95
	char trans_loadRate[numlen]/*�����ʣ������������з�ʽ����Դ��͸��ɵ㶼Ҫ�����������*/;

	char fault_ohline_prob[numlen]/*�ܿ��߹����ʣ���/��/100km*/, fault_ohline_rtime[numlen]/*�޸�ʱ�䣨���ӣ�*/;
	char fault_ohline_ins_prob[numlen]/*�ܿվ�Ե�߹����ʣ���/��/100km*/, fault_ohline_ins_rtime[numlen]/*�޸�ʱ�䣨���ӣ�*/;
	char fault_cable_prob[numlen]/*����*/, fault_cable_rtime[numlen]/**/;
	char fault_breaker_prob[numlen]/*��·������/��/100̨*/, fault_breaker_rtime[numlen];
	char fault_loadbreakswitch_prob[numlen]/*���ɿ���*/, fault_loadbreakswitch_rtime[numlen]/**/;
	char fault_disconnector_prob[numlen]/*���뿪��*/, fault_disconnector_rtime[numlen]/**/;
	char fault_fuse_prob[numlen]/*�۶���*/, fault_fuse_rtime[numlen]/**/;
	char fault_transformer_prob[numlen]/*��ѹ��*/, fault_transformer_rtime[numlen]/**/;

	char fault_ltime[numlen]/*���߹��϶�λʱ��*/, plan_ltime[numlen]/*Ԥ���Ŷ�λʱ��*/;
	char fault_stime[numlen]/*�����л�ʱ��*/,     plan_stime[numlen]/*�ƻ�ͣ���л�*/;
	char fault_ttime[numlen]/*����ת��ʱ��*/,     plan_ttime[numlen];/*�ƻ�ͣ��ת��ʱ��*/

	char plan_prob[numlen]/*�ƻ�ͣ����*/, plan_rtime[numlen];/*�ƻ�ͣ��ʱ��*/

	int g_cnt_nm1_fd,g_cnt_ro_fd,g_ld_ra,g_begin_so,g_end_so,g_cnt_sc_fd;

///�޹��Ż���Ҫ
	double alpha_a/*�۾�ά����*/;
	double gamma_a/*Ͷ�ʻ�����*/;
	double beta_a/*���*/;
	double Kc_a/*��λ���������豸Ͷ��*/;
	double tau_a/*ȫ����󸺺����Сʱ��*/;


//////////////////////////////////////////////////////////////////////////


public:
//////////////////////////////////////////////////////////////////////////


	int copyfile(char *SourceFile, char *NewFile);
	bool isSame(int *a, int *b, size_t lenA, size_t lenB);
	int isInInt(int src, int* Int, int len);

	//�ж�һ��vector�����Ƿ���ĳ��Ԫ�أ�����з��ظ�Ԫ�صĸ���
	int isFind(std::vector<string> tts, string tt);
	int isFind(std::vector<int> tts, int tt);

	//��һ�������������µ��Ӿ���
	//JudgeMatrix-ԭ����subJudgeMatrix-�Ӿ���i_b,j_b,i_e,j_e-��ʼλ�úͽ���λ��
	void OnGetSubMatrix(vector<vector<double>> JudgeMatrix, vector<vector<double>>& subJudgeMatrix, int i_b, int j_b, int i_e, int j_e);
	//����һ�����󣬵õ������������
	//JudgeMatrix-�����ľ���t_col-����ֵ�������������
	void OnGetLamdaMax(vector<vector<double>> JudgeMatrix, vector<double>& t_col);
	
	int isInArray(int** Int, int x, int y, int* pic);
	//����һ�����ε����飬�õ����еĲ��ظ��������
	//sect��������ķ�Χ,����[0-200], lenPic��Ҫ��õ�λ��������3λ��276
	//int** retInt ���ص�����, int retNum ���صĸ���
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

	//��һ��vectorת������" "�ֿ����ַ���
	void OnVector2String(vector<string> vstring, char* qota,char* out);

	double OnGetStandardDev(double* dList, int cnt, bool isOnlyNormalization = false);

	bool isFind(std::vector<string> tts, char* tt, int& pos);
	bool isFind(std::vector<int> tts, int tt, int& pos);
	void getRightByleft(std::vector<std::string> src, std::vector<std::string> dst, char* left, char* key);
	void getRightByleft(std::vector<std::string> src, char* left, char* right, char* key);
	std::vector<std::string> splitWithStl(const std::string &str, const std::string &pattern);

	////�ַ���2���� atoi();atof;
	char* IntToString(int iValue);		//����ת�ַ���
	char* IntToString(int iValue,int wide); 

	char* FloatToString(float iValue,int wide=0);	//������ת�ַ���
	char* DoubleToString(double iValue);	//˫������ת�ַ���
	char* DoubleToString(double iValue,int wide) ;


	//�򵥵�д��־�ļ������ļ��򿪣�д����
	char logfile[128];
	void WriteLog(char* logfile, char* szLog);
/*
//logfile
	//�ڳ��������Ŀ¼���õ���־�ļ�
	//����ļ�����Ϊ�գ�˵����ָ�����ļ�����־�ں���׷�ӾͿ�����
	//����ļ���Ϊ�գ������µ��ļ���ֱ��������ȡ����Ĭ����д��ʱ��
	int openLogFile(* fileName=NULL);
	//����һ����־
	int addLogLine(* str,bool addtime=false);
	//�ر���־�ļ�
	int closeLogFile();
*/
//////////////////////////////////////////////////////////////////////////
//file
	//��һ���ļ����ӵ���һ���ļ���ĩβ
	void filecat(char* srcfile,char* catfile);
	int ReNameFile(char *source,char *newname);
	int DelFile(char *source);
	int CopyFile(char *SourceFile,char *NewFile);

	void getCurTime(char* curTime,int flag=0);

	//����һ���ļ������������е�Ŀ¼���ļ���
	int getFileDir_Name(char* sFileName, char* dir, char* fname);

	int getExecutePath(char* exePath);
	//��ȡ����������·��
	int getEnvironmentVariable(char* sEnvironmentName,char* outEnvVar);
	//���һ������Ŀ¼��ȫ���ļ����ŵ�һ���ṹ����
	void getFilesall( string path, vector<string>& files);  

	//��һ���ÿո�������ı��������Ӧ���ֶ�
	// 1  3  2  4  //���е��ı�
	// 0  0  1  0  //0��ʾ�ַ�����1����
	//fil[4];datatype[4];n_fil=4;src=12
	//����ֵΪ�� "%s%s%d%s"
	void getFilter(int* fil,int* datatype,char* ret);

//////////////////////////////////////////////////////////////////////////
//integer
	//���һ���������������һ����Ч������-5����ǰ����-1���
	//{2,3,1,2,-2,-5,-1,-1,-1}
	int getLastValid(int* src,int len);
	int sortInt(int* dd,int cnt);//��������
	int sortint_Desc(int* dd,int cnt);//��������
	int sortDouble(double* dd,int cnt);
	int sortDouble_Desc(double* dd,int cnt);//����
	void dsort(double& a,double& b);
//////////////////////////////////////////////////////////////////////////
//string
	void insertchar(char* src,int pos,char* dst);//��srcָ����posλ�ú�����ַ���dst
	void substr(char* src,int pos,int len,char* dst);//��src�ַ����еĵ�pos�ĺ��濪ʼȡlen���ַ���dst
	//ɾ���ַ�����ߵ�n���ַ�
	void delstrleft(char* src,int leftlen,char* dst);
	//ɾ���ַ����ұߵ�n���ַ�
	void delstrright(char* src,int rightlen,char* dst);
	void delstrright(char* src,int rightlen);
	//�ж�һ���ַ����Ƿ����λ�ú���ĳ��Ԫ�أ�0-ȫ���ң�+�����λ�ã�-�ұ���λ��
	int isContainor(char* src,char substr,int pos=0);

	void trim_return(char* src);
	//��һ���ַ����еĿո�ת����-
	//void trimsapce(char* s);
	char* trimsapce(char* s);
	//�ַ���src�м��ùؼ���key�����ܶ��ַ�,�������н�����������ŵ�strList����
	int strparse(char* src,char key,char** strlist);

	void delstr(char* src,int start,int end,char* dst);

	//ȡsrc����ߵĵ�һ����qota�ָ��Ĳ��֣��ŵ�dst���棬getleftstr("10.22/10.33","/",dst),dstΪ10.22
	int getleftstr(char* src,char qota,char* dst); 
	int getrightstr(char* src,char qota,char* dst);

	//���ݽ����������ַ������飬�趨�������paramtype-0Ϊ������1��·��2�ɿ��ԣ�4 n-1
	void setMathParm(char** param,int num,int paramtype=2);
	void getstrByIndex(char** strlist,int index,char* dst);

	//sSrc Դ�ַ�����sMatchStr ��Ҫ�滻���ַ�����sReplaceStr �滻�����ַ���
	int ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr);
	char *replacestr(char *strbuf, char *sstr, char *dstr);

//////////////////////////////////////////////////////////////////////////

	//ר��Ϊ�Ĵ���һ�����ڽ�������,������ͷ���1�����򷵻�0
	int get_SICHUAN_date(char* filename,char* yyyymmdd,int& time_index);
	int get_SICHUAN_ftpfilename(char* filename,char* yestDt1,char* pre,char* posx);

//////////////////////////////////////////////////////////////////////////
	//��һ�����ε������м���һ���ڵ㣬���û�У��ӵ����棬���������λ��
	//����Ѿ��У�����0; pos����λ��
	int addNode(int* nodelist,int node,int& pos);
	
	//�ҵ��ڵ�,Ĭ���ǲ���Ҫ�����Ѿ��ź���
	//nodelist ���͵�����,node����ֵ,int& pos���͸����������������е�λ��,order=0Ĭ���ź���
	//����ֵ-1 û���ҵ�������Ϊλ�ã�һ���Ǵ��ڻ����0
	int findNode(int* nodelist,int node,int pos,int order=0);

	//һ���ڵ������������������  intList��������,node���ҵĽڵ�,cnt������Ԫ�صĸ���
	int IsExistInArray(int* intList,int node,int cnt);
	//��һ��������ɾ��Ԫ�أ�����Ϊ���ʣ�µ�Ԫ�ظ���
	int removeInArray(int* intList,int node,int cnt);

	int getLR_Bykey(char* src,char* key,char* left,char* right);
//////////////////////////////////////////////////////////////////////////
	//��һ����������ת��,���з�ȫ��ת�ú���ЧԪ��ת��
	//src��Ҫת�õ�������len�������ȣ�dstת�����������typeĬ��ȫת��>0
	//ֻ��С�ڵ���0��ʱ�������ЧԪ�ص�ת�ã�2015-3-10
	void converse_vector(int* src,int len,int* dst,int type=1);




	//srcԴ������comp1,���طֲ��λ�ã���Դ��ʼ����,eleΪԪ��
	int comp_vector(int* src,int* comp,int& ele);//�Ƚ�2������
	//int ele_vector(int* src, int ele, int len=0);//ele�Ƿ��������У��Ƿ���λ�ã��񷵻�-1
	//����������Ԫ�ؽ��бȽϣ��ȳ��ַ���1������ַ���2����û�У�����-1
	int ele_comp(int* src, int ele1, int ele2);
	//�����豸�ĵ���Դ����С·�����ɵ����С·����ȡ���ǵĽ���,cross_nd�ڵ�
	//���ɵ㣺4,2,1,0,-1,-1,-1;��Դ�㣺5,1,0,-1,-1,-1,-1
	//����2��cross_nd=1
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

	long getCurMill();//������е�ʱ���
	void TimeHere(char* here);
	int  OutputAllTime(char* outall);//������е�ʱ���

private:
	int i_here;
	//���50����¼��ǰ��ʱ��
	stru_timeHere t_here[500];
	//CList<CString,CString&> m_list;

	//��ʱ�꣬here��ע��ǰ��ʱ���˵��

};

#endif // !defined(AFX_LLD_H__EE931DDF_40A2_46BA_B38B_7C54AB8CED32__INCLUDED_)
