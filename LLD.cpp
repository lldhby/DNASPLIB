// LLD.cpp: implementation of the CLLD class.
//
//////////////////////////////////////////////////////////////////////

//#include <boost/algorithm/string.hpp>
#include <stdio.h>
//#include <fcntl.h>
//#include<unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include<tchar.h>
#else
//#include "D:/lld_common/DPCAS4.1/LLD.h"
//#include <sys/time.h> //lld 2019-9-25
#endif

#include "../DNASPLIB/LLD.h"

#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#include "time.h"
#include <cstdio>
#include <algorithm>
#include <vector>
#include <fstream>
#include "string.h"
using namespace std;

fstream logFile;	//文件流

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLLD::CLLD()
{
	mx_co=1;
	mx_global=1;

	mx_digital=0;	//最多只有50000个遥信点
	mx_analog=0;
	mx_st=0;//000
	mx_xfmr=0;
	mx_xf=0;
	mx_cb=0;
	mx_so=0;
	mx_ld=0;
	mx_cp=0;
	mx_bus=0;
	mx_zn=0;
	mx_eq=0;
	mx_tapty = 0;

	mx_sec=0;
	mx_secp=0;
	mx_nd=0;
	mx_bs=0;
	mx_island=0;
	mx_global=0;
	mx_r_zn_ld=0;
	mx_r_zn_sec=0;
	mx_r_zn_cb=0;
	mx_r_zn_cp = 0;
	mx_r_nd_cb=0;
	mx_r_nd_ld=0;
	mx_r_xfmr_xf=0;
	mx_r_nd_xf=0;
	mx_r_nd_zn=0;
	mx_r_nd_sec=0;
	mx_r_nd_cp = 0;
//lld add 2016-1-5
	mx_fd=0;
	mx_un=0;
	mx_r_fd_un=0;
	mx_r_fd_zn=0;
	mx_r_fd_eq=0;
	mx_r_fd_so=0;
	mx_r_nd_eq=0;
	mx_r_fd_ld=0;
	mx_r_co_st = 0;
	mx_r_st_xfmr = 0;

	mx_minpath_len = 200;//最大的最小路长度
	mx_sql_char = 128;

	LINELOSS = 0.05;
}

CLLD::~CLLD()
{

}

int CLLD::isFind(std::vector<string> tts, string tt)
{
	return std::count(tts.begin(), tts.end(), tt);
}

int CLLD::isFind(std::vector<int> tts, int tt)
{
	return std::count(tts.begin(), tts.end(), tt);
}

bool CLLD::isFind(std::vector<string> tts, char* tt, int& pos)
{
	int iFind = 0;
	int index = 0;
	vector<string>::iterator t;
	for (t = tts.begin(); t != tts.end(); t++)
	{
		if (strcmp((*t).c_str(), tt) == 0)
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

bool CLLD::isFind(std::vector<int> tts, int tt, int& pos)
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

//从一个矩阵里面获得新的子矩阵
void CLLD::OnGetSubMatrix(vector<vector<double>> JudgeMatrix, vector<vector<double>>& subJudgeMatrix,
	int i_b, int j_b, int i_e, int j_e)
{
	int i, j;
	vector<double> row;

	subJudgeMatrix.clear();
	for (i = i_b;i < i_e;i++)
	{
		row.clear();
		for (j = j_b;j < j_e;j++)
		{
			row.push_back(JudgeMatrix[i][j]);
		}
		subJudgeMatrix.push_back(row);
	}
}

//给定一个矩阵，得到最大特征向量
void CLLD::OnGetLamdaMax(vector<vector<double>> JudgeMatrix, vector<double>& t_col)
{
	int i, j, k;
	int n_sch, n_dim;

	//存放中间过程
	vector<vector<double>> t_JudgeMatrix;//二维数组,判断矩阵

	//使用矩阵进行计算
	//1.用和积法计算判断矩阵的特征向量
	//1-列归一化，2-按行相加，3-加后归一化得最大特征向量，即为近似解
	n_sch = JudgeMatrix[0].size();
	n_dim = JudgeMatrix.size();
	OnGetSubMatrix(JudgeMatrix, t_JudgeMatrix, 0, 0, n_dim, n_dim);

	//求W

	//对列进行归一化
	double sumCol = 0;
	for (j = 0; j < n_dim; j++)
	{
		sumCol = 0;
		for (k = 0; k < n_dim; k++)
		{
			sumCol += JudgeMatrix[k][j];
		}
		for (k = 0; k < n_dim; k++)
		{
			t_JudgeMatrix[k][j] = JudgeMatrix[k][j] / sumCol;
		}
	}
	//行加
	t_col.clear();
	for (i = 0; i < n_dim; i++)
	{
		sumCol = 0;
		for (j = 0; j < n_dim; j++)
		{
			sumCol += t_JudgeMatrix[i][j];
		}
		t_col.push_back(sumCol);
	}
	//行归一化
	sumCol = 0;
	for (i = 0; i < n_dim; i++)
	{
		sumCol += t_col[i];
	}
	for (i = 0; i < n_dim; i++)
	{
		t_col[i] /= sumCol;
	}
}

void CLLD::splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
    //从后面开始定位
    drive[0] = 0;
    int ii = 0;
    int len = 0;
    int pos = 0;
    len = strlen(path);
    char* temp = new char[len];
    for (ii = 0;ii < len; ii++)
        temp[0] = 0;
    for (ii = len;ii > 0;ii--)
    {
        if ((path[ii] == '\\') || (path[ii] == '/'))
        {
            pos = ii;
            break;
        }
    }

    for (ii = 0;ii < pos; ii++)
    {
        temp[ii] = path[ii];
    }
    temp[ii] = '\0';
    strcpy(dir, temp);
    temp[0] = 0;
    for (ii = pos + 1;ii < len; ii++)
    {
        temp[ii - pos - 1] = path[ii];
    }
    temp[ii - pos - 1] = '\0';
    strcpy(fname, temp);

    pos = 0;
    len = strlen(fname);
    for (ii = 0;ii < len; ii++)
    {
        if (fname[ii] == '.')
        {
            pos = ii;
            break;
        }
    }
    temp[0] = 0;
    for (ii = pos + 1;ii < len; ii++)
    {
        temp[ii - pos - 1] = fname[ii];
    }
    temp[ii - pos - 1] = '\0';
    strcpy(ext, temp);

    delete[] temp;
}

//目前暂时还不支持汉字路径或文件名
int CLLD::getFileDir_Name(char* sFileName, char* dir, char* fname)
{
//	char temp[128];
	char drive[16];
	char name[64];
	char ext[8];
	char path[128];
	dir[0] = 0;
	fname[0] = 0;

#if _WIN32
	//将'\\'改成'/'
	_splitpath(sFileName, drive, path, name, ext);
	strcat(dir, drive);
#elif __linux__
	if (strstr(sFileName, "\\") != NULL)
	{
		for (size_t i = 0; i < strlen(sFileName); i++)
		{
			if (sFileName[i] == '\\')
				sFileName[i] = '/';
		}
	}
	splitpath(sFileName, drive, path, name, ext);

	//给目录加上/,lld 2018-12-18
	strcat(path, "/");

#else
	printf("http://c.biancheng.net\n");
#endif

	strcat(dir, path);
	strcat(fname, name);

	//linux
	//delstrright(dir, 1);

	//windows
	//delstrright(dir, 1);

	return 1;
}

bool CLLD::isSame(int *a, int *b, size_t lenA, size_t lenB)
{
	//if (lenA != lenB)
	//{
	//	return false;
	//}

	//sort(a, a + lenA);
	//sort(b, b + lenB);

	//return memcmp((void*)a, (void*)b, lenA * sizeof(int)) == 0;


	//int a[] =
	//{
	//	1,2,3,2
	//};

	//int b[] =
	//{
	//	1,3,2,2
	//};

	//cout << isSame(a, b, sizeof(a) / sizeof(int), sizeof(b) / sizeof(int)) << endl;

	return true;
}

int CLLD::isInInt(int src, int* Int, int len)
{
	for (int ii = 0; ii < len; ii++)
	{
		if (src == Int[ii])
			return ii;
	}

	return -1;
}

int CLLD::isInArray(int** Int, int x, int y, int* pic)
{
	int ii, jj, find;
	for (ii = 0;ii < x; ii++)
	{
		find = 1;
		for (jj = 0;jj < y; jj++)
		{
			if (*(*(Int + ii) + jj) != *(pic + jj))
			{
				find = 0;
				break;
			}
		}
		if (find == 1)
			return 1;
	}

	return 0;
}

//给定一个整形的数组，得到其中的不重复的随机数
//sect整型数组的范围,比如[0-200], lenPic需要获得的位数，比如3位数276
//int** retInt 返回的数组, int retNum 返回的个数
int CLLD::OnGetRandom(int sect, int lenPic, int** retInt, int retNum)
{
	int i, j, k;
	int ii = 0;
	int jj = 0;
	int* pic = new int[lenPic];
	int success = 0;
	std::vector<int> foo;
	while (true)
	{
		foo.clear();
		for (int i = 0;i < sect;i++)
			foo.push_back(i);
		// obtain a time-based seed:
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle(foo.begin(), foo.end(), std::default_random_engine(seed));
		for (j = 0;j < foo.size();j = j + lenPic)
		{
			if ((j + lenPic) > foo.size())
				break;

			for (k = 0;k < lenPic;k++)
			{
				*(pic + k) = foo.at(j + k);
			}
			if (isInArray(retInt, retNum, lenPic, pic) == 0)
			{
				for (jj = 0;jj < lenPic;jj++)
					*(*(retInt + ii) + jj) = *(pic + jj);
			}
			if (ii++ == retNum - 1)
			{
				success = 1;
				break;
			}
		}

		if (success)
			break;
	}
	delete[] pic;
	if (success)
		return 1;
	else
		return 0;
}

void CLLD::OnVector2String(vector<string> vstring,char* qota, char* out)
{
	if (vstring.size() == 0)
	{
		out[0] = 0;
		return;
	}

	char ret[2000];
	string st;
	char t[64];
	ret[0] = 0;
	for (vector<string>::iterator it = vstring.begin();it != vstring.end(); it++) ///第二种调用方法  
	{
		st = *it;
		strcpy(t , st.c_str());
		strcat(ret, t);
		strcat(ret, qota);
	}
	strcpy(out, ret);
	substr(out, 0, strlen(out) - strlen(qota), ret);
	//strncpy(ret,out,strlen(out)-strlen(qota)-1);
	strcpy(out, ret);
}

//归一化,并标准方差
//输入一个数组，求取其均方差，isOnlyNormalization仅仅归一化
double CLLD::OnGetStandardDev(double* dList, int cnt,bool isOnlyNormalization)
{
	int ii = 0;
	double ret = 0;
	//归一化操作，将其全部变成0-1之间的小数
	double* temp = new double[cnt];//长度
	double u = 0;
	for(ii=0;ii<cnt;ii++)
		u += dList[ii];
	if (u == 0)
		return 100;//全部都是0.很平整,但不是想要的

	for (ii = 0;ii<cnt;ii++)
		temp[ii] = (dList[ii] / u);

	if (isOnlyNormalization)
	{
		dList[ii] = temp[ii];
	}
	else
	{
		//计算均方差
		u = 0;
		for (ii = 0;ii<cnt;ii++)
			u += temp[ii];
		u = u / cnt;

		double ruo = 0;
		for (ii = 0;ii<cnt;ii++)
			ruo += (temp[ii] - u) * (temp[ii] - u);
		ret = sqrt(ruo / cnt);
	}
	return ret;
}

std::vector<std::string> CLLD::splitWithStl(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> resVec;

	if ("" == str)
	{
		return resVec;
	}
	//方便截取最后一段数据
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}


//对一个向量进行转置,其中分全部转置和有效元素转置
void CLLD::converse_vector(int* src,int len,int* dst,int type)
{
	int i,pos;

	if (type < 0) //有效转置
	{
		for (i=0; i<len; i++)
		{
			dst[i] = type;
		}

		//找到最后那个元素
		for (i=0; i<len; i++)
		{
			if (src[i]==type)
			{
				pos = i;
				break;
			}
		}
		
		for (i=pos-1; i>=0; i--)
		{
			dst[pos-1-i] = src[i];
		}

	}
	else//全部转置
	{
		for (i=len; i>0; i--)
		{
			dst[len-i] = src[i];
		}
	}
}

char* CLLD::IntToString(int iValue) 
{
	char str[512];
	sprintf(str,"%d",iValue);
	
	return str;
}

char* CLLD::IntToString(int iValue,int wide) 
{
	char str[512];
	switch( wide )
	{
	case 2:
		sprintf(str,"%02d",iValue);
		break;
	case 3:
		sprintf(str,"%03d",iValue);
		break;
	case 4:
		sprintf(str,"%04d",iValue);
		break;
	case 5:
		sprintf(str,"%05d",iValue);
		break;
	case 6:
		sprintf(str,"%06d",iValue);
		break;
	case 7:
		sprintf(str,"%07d",iValue);
		break;
	case 8:
		sprintf(str,"%08d",iValue);
		break;

	default:
		sprintf(str,"%d",iValue);

	}	
	return str;
}

char* CLLD::FloatToString(float iValue,int wide) 
{
//	char ss[100];
//	float k=0.02132113;
//	sprintf(ss,"%03f",k);

	char str[512];
	switch( wide )
	{
	case 2:
		sprintf(str,"%02f",iValue);
		break;
	case 3:
		sprintf(str,"%03f",iValue);
		break;
	case 4:
		sprintf(str,"%04f",iValue);
		break;
	case 5:
		sprintf(str,"%05f",iValue);
		break;
	case 6:
		sprintf(str,"%06f",iValue);
		break;
	case 7:
		sprintf(str,"%07f",iValue);
		break;
	case 8:
		sprintf(str,"%08f",iValue);
		break;
		
	default:
		sprintf(str,"%f",iValue);
		
	}		
	return str;
}

char* CLLD::DoubleToString(double iValue) 
{
	char str[512];
	sprintf(str,"%f",iValue);
	
	return str;
}

char* CLLD::DoubleToString(double iValue,int wide) 
{
	return FloatToString(iValue,wide);	
}

//
////根据开关的个数，穷举开关的可能性,返回各种组合
//int CLLD::getRandomCB(int cbNum,CStringList& cbSerial)
//{
//	int ret; 
//	CString strT;
//	char s[1];
//	char sss[30];
//
//	//cbSerial.RemoveAll();
//	if (cbNum == 0)
//		return  0;
////#define N 24
//
//	int N = cbNum;
//	int i, j;
//	int Total;
//	int Temp;
//
//	ret = 0;
//	Total = (int)(1<<N);
//	for (i=0; i<Total; i++) 
//	{
//		strT = "";
//	    for (j=0; j<N; j++) 
//		{
//			Temp = (i>>(N-j-1))&1;
//
//			itoa(Temp,s,10);
//
//			sss[j]=s[0];
//		  //cout<<Temp;//写一位
//			//printf(sss);
//	
//		}
//		sss[N]='\0';
//		//strcpy(cbSerial[N],sss);
//		cbSerial.AddTail(sss);
//		ret++;
//    //cout<<endl;//一个字符串结束
//	 }
//
//	return ret;
//}
//
//CString CLLD::getStringByIndex(CStringList* strList,int ii)
//{
//	//char ret[];
//	CString ss;
//	int jj;
//	
//	POSITION p = strList->GetHeadPosition();
//	jj = 0;
//	while (p != NULL)
//	{
//		ss = strList->GetNext(p);
//		if (ii == jj)
//		{
//			return ss;
//		}
//		jj++;
//	}
//	
//	return "";
//}
//
//int CLLD::trim(CString s,BOOL upper)
//{
//	CString str;
//
//	str = s;
//	str.TrimLeft();
//	str.TrimRight();
//	
//	if (upper)
//		str.MakeUpper();
//
//	s = str;
//
//	return 1;
//}
//
//int CLLD::getLR_Bykey(CString src,CString key,CString& left,CString& right)
//{
//	int pos;
//	CString get,other;
//
//	trim(src);
//	trim(key);
//	pos = src.Find(key);
//	if (pos <0)
//		return -1;//没有找到
//
//	get = src.Left(pos);
//	other = src.Right(src.GetLength()-pos-1);
//		
//	get.Remove(';');
//	other.Remove(';');
//	trim(get);
//	trim(other);
//
//	left = get;
//	right = other;
//
//	return pos;
//}

int CLLD::copyfile(char *SourceFile,char *NewFile)
{
	ifstream in;
	ofstream out;
	in.open(SourceFile,ios::binary);//打开源文件
	if(in.fail())//打开源文件失败
	{
//	   cout<<"Error 1: Fail to open the source file."<<endl;
	   in.close();
	   out.close();
	   return 0;
	}
	out.open(NewFile,ios::binary);//创建目标文件 
	if(out.fail())//创建文件失败
	{
//	   cout<<"Error 2: Fail to create the new file."<<endl;
	   out.close();
	   in.close();
	   return 0;
	}
	else//复制文件
	{
	   out<<in.rdbuf();
	   out.close();
	   in.close();
	   return 1;
	}
}

int CLLD::DelFile(char *source)
{
	/* _access(char *,int) 判断文件是否存在
	存在 返回0;不存在 返回-1.
	_access(const char *path,int mode)
	mode的值:
	00 是否存在
	02 写权限
	04 读权限
	06 读写权限
	*/
//	if(!_access(source,0))//如果文件存在:文件为只读无法删除
//	{
//		if (remove(source) == 0)
//		{
//			return 1;
//		}
//		else
//		{
//			return 0;
//		}
//
//
//
//	//去掉文件只读属性		
//		// lld comment 2018-5-23
//		//SetFileAttributes(source,0);		
//		
//		//if(DeleteFile(source))//删除成功
//		{
////		   cout<<source<<" 已成功删除."<<endl;
//		}
////		else//无法删除:文件只读或无权限执行删除
//		{
////		   cout<<source<<" 无法删除:文件为只读属性或无删除权限."<<endl;
//		}
//	}
//	else//文件不存在
//	{
////		cout<<source<<" 不存在,无法删除."<<endl;
//	}
//
	return 0;
}

int CLLD::ReNameFile(char *source,char *newname)
{
//	if(!_access(source,0))//如果文件存在:
//	{
//		if(!rename(source,newname))//删除成功
//		{
//			//cout<<source<<" 成功重命名为: "<<newname<<endl;
//		}
//		else//无法重命名:文件打开或无权限执行重命名
//		{
////	   cout<<"文件无法重命名(可能原因如下):"<<endl;
////	   cout<<"\t"<<"1. "<<newname<<" 已存在"<<endl
////		<<"\t"<<"2. "<<newname<<" 正在使用,未关闭."<<endl
////		<<"\t"<<"3. "<<"你没有权限重命名此文件."<<endl;
//		}
//	}
//	else//文件不存在
//	{
//		//cout<<source<<" 不存在,无法重命名."<<endl;
//	}
	return 0;

}


int CLLD::get_SICHUAN_ftpfilename(char* filename,char* yestDt1,char* pre,char* posx)
{
	char yestDt[9];
    time_t now = time(NULL);
    struct tm *ts = localtime(&now);
    ts->tm_mday--;
    mktime(ts);
    strftime(yestDt, sizeof(yestDt), "%Y%m%d", ts);
	strcpy(yestDt1,yestDt);
 //   printf("Yesterday is \"%s\"\n", yestDt);

	char left[100];
//    time_t now = time(NULL);
//    struct tm *ts = localtime(&now);
//    ts->tm_mday--;
//    mktime(ts);
//    strftime(t, sizeof(t), "%4Y%2m%2d", ts);
//    printf("Yesterday is \"%s\"\n", t);
//	strcpy(yestDt,t);
//
	posx[0]=0;
	getrightstr(filename,'.',posx);
	
	pre[0]=0;
	getleftstr(filename,'.',left);
	substr(left,0,strlen(left)-12,pre);

	return 1;
}

int CLLD::get_SICHUAN_date(char* filename,char* yyyymmdd,int& time_index)
{
	char yestDt[9]; 
    time_t now = time(NULL); 
    struct tm *ts = localtime(&now); 
    ts->tm_mday--; 
    mktime(ts); 
    strftime(yestDt, sizeof(yestDt), "%Y%m%d", ts); 
    printf("Yesterday is \"%s\"\n", yestDt);

	int len;
	char t[128],date[8],hh[2],mi[2];

	strcpy(t,filename);
	trim_return(t);
	len = strlen(t);
	substr(t,len-17,8,date);
	substr(t,len-9,2,hh);
	substr(t,len-7,2,mi);

	strcpy(yyyymmdd,date);
	time_index = 1+12*atoi(hh)+atoi(mi)/5;

	if (strcmp(date,yestDt)==0)
		return 1;
	else
		return 0;
}

int  CLLD::init_globalexception()//初始化
{
	globalexception[0] = 0;
	num_globalexception = 0;

	return 1;
}

int  CLLD::add_globalexception(char* add)
{
	char str[500];

	//先判断长度，如果大于5000，退出
	if (strlen(globalexception) >= 5000)
	{
		sprintf(str,"%s","错误太多，不能增加输出信息!");
		WriteLog(logfile,str);

		return 0;
	}

	strcat(globalexception,add);
	strcat(globalexception,";\n");
	num_globalexception++;

	return 1;
}

/*
int  CLLD::out_globalexception(CStdioFile* file)
{
	if (num_globalexception>0)
		file->WriteString(globalexception);

	return 1;
}
*/

void CLLD::getFilesall( string path, vector<string>& files)  
{  
	////文件句柄  
	//long   hFile   =   0;  
	////文件信息  
	//struct _finddata_t fileinfo;  //很少用的文件信息读取结构
	//string p;  //string类很有意思的一个赋值函数:assign()，有很多重载版本
	//if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
	//{  
	// do  
	// {   
	//	 if((fileinfo.attrib &  _A_SUBDIR))  //比较文件类型是否是文件夹
	//	 {  
	//		 if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
	//		 {
	//			 files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
	//			 getFilesall( p.assign(path).append("\\").append(fileinfo.name), files ); 
	//		 }
	//	 }  
	//	 else  
	//	 {  
	//		 files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
	//	 }  
	// }while(_findnext(hFile, &fileinfo)  == 0);  //寻找下一个，成功返回0，否则-1
	// _findclose(hFile); 
	//} 
}  

/*
	//在程序的运行目录处得到日志文件,如果文件名为空，直接用日期取代，默认是写入时间
int CLLD::openLogFile(char* fileName)
{
	char curTime[128];
	char sExePath[128];
	char sfileName[64];

	getCurTime(curTime);				
	if (fileName==NULL)
	{
		getExecutePath(sExePath);
		sprintf(sfileName,"%s\\%s.log",sExePath , curTime);
		logFile.open(sfileName,ios::out);	//系统接线数据文件
		if(logFile.is_open() == 0)	//没有打开
		{
			return 0;
		}
	}
	else
	{
		//strcpy(curTime,fileName);
		//getExecutePath(sExePath);
		//sprintf(sfileName,"%s_%s.log",fileName , curTime);
		strcpy(sfileName,fileName);
		if(GetFileAttributes(sfileName)!=0xFFFFFFFF) //文件存在
		{ 
			logFile.open(sfileName,ios::out);	//系统接线数据文件
			if(logFile.is_open() == 0)	//没有打开
			{
				return 0;
			}
		}
		else
		{
			sprintf(sfileName,"%s_%s.log",fileName , curTime);
			logFile.open(sfileName,ios::out);	//系统接线数据文件
			if(logFile.is_open() == 0)	//没有打开
			{
				return 0;
			}
		}
	}

	return 1;
}
*/
void CLLD::getFilter(int* fil,int* datatype,char* ret)
{
	//string ss;
//	BOOL bFind;
	int i,j,pos,start;
	ret[0]=0;

	start = 0;
	for(i=0; i<100; i++)
	{
		if (-1 == fil[i])//出现-1，退出
			break;
		
		pos = fil[i];

		//严格按顺序来处理
		for (j=start; j<pos; j++)
		{
			strcat(ret,"%*s");//先不空缺
		}

		start = pos+1;//重新开始 

		//直接取对应的数据类型	
		if (datatype[i]==1)
			strcat(ret,"%d");
		else
			strcat(ret,"%s");
	}
}

int CLLD::getLastValid(int* src,int len)
{
	return src[len-1]+1;


	int i,pos;
	pos = len;

	for (i=pos-1; i>=0; i--)
	{
		if (-1 == src[i])
			return src[i];
	}

}

void CLLD::WriteLog(char* logfile,char * szLog)  
{  
/*
	struct timeval tv;  
	char buf[64];  
  
	gettimeofday(&tv, NULL);  
	strftime(buf, sizeof(buf)-1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));  
	fprintf(stderr, "%s ", buf);  
*/




//     SYSTEMTIME st;  
//     GetLocalTime(&st);  
     FILE *fp;  
     fp=fopen(logfile,"at");  
     fprintf(fp,szLog);  
     fprintf(fp,"\t");  
//     fprintf(fp,"MyLogInfo: %d:%d:%d:%d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);  
     fprintf(fp,"\n");  
     fclose(fp);  
     //OutputDebugStringA(szLog);  
	 
}  

/*
//加入一行日志
int CLLD::addLogLine(char* str,bool addtime)
{
	char ss[128];
	char cur[32];

	if (strlen(str) > 128)
	{
		logFile<<str;
		return 1;
	}

	if (addtime)
	{
		getCurTime(cur);
		sprintf(ss,"%s   %s\n",cur);
	}
	else
		sprintf(ss,"%s\n",str);

//	m_list.AddTail(ss);
	logFile<<ss;
	return 1;
}

//关闭日志文件
int CLLD::closeLogFile()
{
	logFile.close();

	return 1;
}
*/

void CLLD::getCurTime(char* curTime,int flag)
{
 	char str[128];
	int hour;

#ifdef _WIN32
 	SYSTEMTIME st;
 	GetSystemTime(&st);
 
	hour = st.wHour+8;
 	if (hour > 24)
 		hour = hour-24;
 	if (hour < 0)
 		hour = 24 + hour;
 
	//默认，要全部带毫秒 yyyymmdd hhmiss ms
	if (flag==0)
	{
		sprintf(str,"%04d-%02d-%02d %02d-%02d-%02d %04d",st.wYear,st.wMonth,st.wDay,hour,st.wMinute,st.wSecond,st.wMilliseconds);
		strcpy(curTime,str);
	}

	//只要年月日yyyymmdd
	if (flag==1)
	{
		sprintf(str,"%04d%02d%02d",st.wYear,st.wMonth,st.wDay);
		strcpy(curTime,str);
	}

	//yyyymmdd hhmiss
	if (flag==2)
	{
		sprintf(str,"%04d%02d%02d %02d%02d%02d",st.wYear,st.wMonth,st.wDay,hour,st.wMinute,st.wSecond);
		strcpy(curTime,str);
	}
#else
	time_t now = time(0);// 基于当前系统的当前日期/时间
	tm *ltm = localtime(&now);

	char iyear[50], imonth[50], iday[50], ihour[50], imin[50], isec[50];
	sprintf(iyear, "%d", 1900 + ltm->tm_year);
	sprintf(imonth, "%02d", 1 + ltm->tm_mon);
	sprintf(iday, "%02d", ltm->tm_mday);
	sprintf(ihour, "%02d", ltm->tm_hour);
	sprintf(imin, "%02d", ltm->tm_min);
	sprintf(isec, "%02d", ltm->tm_sec);

	//vector<string> sDate{ iyear, imonth, iday };
	//vector<string> sTime{ ihour, imin, isec };
	//string myDate = boost::algorithm::join(sDate, "-");
	//string myTime = boost::algorithm::join(sTime, ":");
	//vector<string> sDateTime{ myDate, myTime };
	//string myDateTime = boost::algorithm::join(sDateTime, " ");
	sprintf(curTime, "%s-%s-%s %s:%s:%s", iyear, imonth,iday,ihour,imin,isec);

#endif


}

int CLLD::getExecutePath(char* exePath)
{
	bool ret = false;

#if _WIN32
	int i, len,index;
	TCHAR   szPath[MAX_PATH];
	char retPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);

	//int iLen = 2 * wcslen(szPath);
	//::wcstombs(retPath, szPath, iLen + 1);

	len = strlen(retPath);
	for (i = len; i >= 0; i--)
	{
		if (retPath[i] == ('\\'))
		{
			index = i;
			break;
		}
	}

	for (i = 0; i < index; i++)
	{
		exePath[i] = retPath[i];
	}
	exePath[index] = '\0';
	return true;
	//GetModuleFileName(NULL, ExePath, MAX_PATH);
#elif __linux__
	//std::string strCfgName;// = "logger_import_db.conf";
	//bool fGetCfgFileName(std::string& paraStr_CfgFileName)
	//{
	//	paraStr_CfgFileName.clear();
	//	char szWorkDir[MAX_PATH] = { 0 };
	//	char szCfgFileNameTemp[MAX_PATH] = { 0 };
	//	if (!getcwd(szWorkDir, 260))
	//	{
	//		return false;
	//	}

	//	paraStr_CfgFileName = szWorkDir;
	//	paraStr_CfgFileName.append("/");
	//	paraStr_CfgFileName.append(strCfgName);

	//	strcpy(filename, paraStr_CfgFileName.c_str());

	//	return true;
	//}



#endif



	return ret;
	
//	int i,index;
	/*
	TCHAR   szPath[MAX_PATH];
	::GetModuleFileName(NULL,szPath,MAX_PATH);
	
	len = strlen(szPath);
	for (i=len; i>=0; i--)
	{
		if(szPath[i] == _T('\\'))
		{
			index=i;
			break;
		}
	}

	for (i=0; i<index; i++)
	{
		exePath[i]=szPath[i];
	}
	exePath[index]='\0';
*/
	return 1;
}

void CLLD::trim_return(char* src)
{
	char ret[256];
	int len = strlen(src);
	if (src[len-1] == '\n')
	{

		substr(src,0,len-1,ret);
		ret[len]='\0';

		strcpy(src,ret);
	}
}

void CLLD::delstr(char* src,int start,int end,char* dst)
{
	int len,i,k,s,e;
	len = strlen(src);

	s = start-1;
	e=end-1;
	k=0;
	for (i=0; i<len; i++)
	{
		if ((i<s) || (i>e))
		{
			dst[k++] = src[i];
		}
	}
	dst[k] ='\0';
}

char* CLLD::trimsapce(char* s)
{
	int len,i;
	len = strlen(s);
	
	for (i=0; i<len; i++)
	{
		if ((s[i] == '\n')  ||  (s[i] == ' '))
		{
			s[i] = '-';
		}
	}
	s[len] ='\0';

	return s; 
}

void CLLD::insertchar(char* src,int pos,char* dst)
{
	int len,lenr,lend;
	char right[100];

	len = strlen(src);
	lenr = len-pos;
	for(int ii=0;ii<lenr;ii++)
		right[ii] = src[pos+ii];

	right[lenr] = '\0';
	
	lend=strlen(dst);
	strncpy(src+pos,dst,lend);

	strncpy(src+pos+lend,right,lenr);
	src[len+lend] ='\0';
}

void CLLD::substr(char* src,int pos,int len,char* dst)
{	
	dst[0] = 0;
	
	for(int ii=0;ii<len;ii++)
		dst[ii] = src[pos+ii];
	
	dst[len] = '\0';
}

//取src的左边的第一个以qota分隔的部分，放到dst里面，getleftstr("10.22/10.33","/",dst),dst为10.22
int CLLD::getleftstr(char* src,char qota,char* dst)
{
	dst[0] = 0;
	
	int pos;
	//先定位长度
	pos = isContainor(src,qota);
	if (pos == -1)
		return -1;

	substr(src,0,pos,dst);

	return pos;
}

//目前暂时还不支持汉字路径或文件名
//int CLLD::getFileDir_Name(char* sFileName, char* dir, char* fname)
//{
//	char temp[128];
//	char drive[16];
//	char name[16];
//	char ext[8];
//	char path[64];
//	dir[0] = 0;
//	fname[0] = 0;
//
//#if _WIN32
//	//将'\\'改成'/'
//	_splitpath(sFileName, drive, path, name, ext);
//	strcat(dir, drive);
//#elif __linux__
//	if (strstr(sFileName, "\\") != NULL)
//	{
//		for (size_t i = 0; i < strlen(sFileName); i++)
//		{
//			if (sFileName[i] == '\\')
//				sFileName[i] = '/';
//		}
//	}
//	splitpath(sFileName, drive, path, name, ext);
//
//	//给目录加上/,lld 2018-12-18
//	strcat(path, "/");
//
//#else
//	printf("http://c.biancheng.net\n");
//#endif
//
//	strcat(dir, path);
//	strcat(fname, name);
//
//	//linux
//	//delstrright(dir, 1);
//
//	//windows
//	//delstrright(dir, 1);
//
//	return 1;
//}
//取src的左边的第一个以qota分隔的部分，放到dst里面，getleftstr("10.22/10.33","/",dst),dst为10.22
int CLLD::getrightstr(char* src,char qota,char* dst)
{
	int pos;
	//先定位长度
	pos = isContainor(src,qota);
	substr(src,pos+1,strlen(src)-pos-1,dst);
	return pos;
}

int CLLD::isContainor(char* src,char substr,int pos)
{
	if (src==NULL)
	{
		return -1;
	}

	int ret=-1;
	int sumlen = strlen(src);

	if (pos == 0)
	{
		for(int ii=0;ii<sumlen;ii++)
		{
			if (src[ii] == substr) 
				return ii;
		}
	}
	else if (pos > 0)
	{
		if (src[pos] == substr)
			return 1;
		else
			return 0;
	}
	else
	{
		if (src[sumlen+pos] == substr)
			return 1;
		else
			return 0;
	}

	return ret;
}

void CLLD::getRightByleft(std::vector<std::string> src, char* left, char* right,char* key)
{
	//将;隔开的字符串提取出来
	char substring[500];
	char l[200];
	char r[200];
	right[0] = 0;

	size_t len = src.size();
	for (size_t i = 0; i < len; i++)
	{
		strcpy(substring, src[i].c_str());
		getLR_Bykey(substring, key, l, r);
		if (strcmp(l, left) == 0)
		{
			strcpy(right, r);
			break;
		}
	}
}

void CLLD::getRightByleft(std::vector<std::string> src, std::vector<std::string> dst, char* left, char* key)
{
	//将;隔开的字符串提取出来
	char substring[500];
	char l[200];
	char r[200];

	size_t len = src.size();
	dst.empty();
	for (size_t i = 0; i < len; i++)
	{
		strcpy(substring, src[i].c_str());
		getLR_Bykey(substring, key, l, r);
		if (strcmp(l, left) == 0)
		{
			dst.push_back(r);
		}
	}
}

//删掉字符串左边的n个字符
void CLLD::delstrleft(char* src,int leftlen,char* dst)
{
	char dd[400];
	int sumlen = strlen(src);

	for(int ii=0;ii<sumlen;ii++)
		dd[ii] = src[leftlen+ii];
	
	dd[sumlen-leftlen] = '\0';

	strcpy(dst,dd);
}

//删掉字符串右边的n个字符
void CLLD::delstrright(char* src,int rightlen,char* dst)
{
	int sumlen = strlen(src);

	for(int ii=0;ii<sumlen-rightlen;ii++)
		dst[ii] = src[ii];
	
	dst[sumlen-rightlen] = '\0';
}

//删掉字符串右边的n个字符
void CLLD::delstrright(char* src,int rightlen)
{
	int sumlen = strlen(src);
	char* dst = new char[sumlen];

	delstrright(src,rightlen,dst);
	strcpy(src,dst);
	delete dst;
}


int CLLD::strparse(char* src,char key,char** strlist)
{
	//不包含指定字符,全部都是要解析的字段
	if (isContainor(src,key)<0)
	{
		strcpy(strlist[0],src);
		return 1;	
	}

	int num;
	char left[80];
	char right[80000];
	char t[80000];
	
	strcpy(t,src);
	
	//从一个数组里面找到对应的字符串	
	num = 0;
	while (1)
	{
		if (isContainor(t,key)<0)
		{
			//t后面的回车和空格去掉
			if (src[strlen(right)] == '\n')
			{
				substr(right,0,strlen(right)-1,strlist[num++]);
			}
			else
				strcpy(strlist[num++],right);

			//strcpy(strlist[num++],right);
			return num;	
		}

		getleftstr(t,key,left);
		getrightstr(t,key,right);
		
		strcpy(t,right);
		strcpy(strlist[num++],left);
	}
}

//////////////////////////////////////////////////////////////////////////
//如果里面已经有了，就不需要加了
int CLLD::addNode(int* nodelist,int node,int& pos)
{
	for (int i=0;i<pos;i++)
	{
		if (nodelist[i] == node)
		{
			return 0;
		}
	}
	nodelist[pos++] = node;
	return 1;
}



//特别注意，pos必须要是数组的全部大小,同时nodelist对角标不敏感
int CLLD::findNode(int* nodelist,int node,int pos,int order)
{

	//此处的排序改变了nodelist的位置，下次再用的时候可能就出现了问题
	if (order == 1)
		this->sortInt(nodelist,pos);	

//二分法
	int high,middle,low;

	int ndCount = pos;

	high = ndCount - 1;
	low = 0;
    while(low <= high)
	{
        middle = (low + high) / 2;
		
		if (node == nodelist[middle])
		{  //找到,返回下标索引值
			return middle;
		}
		else if(node < nodelist[middle])
		{  //查找值在低半区
            high = middle - 1;
        }
		else
		{  //查找值在高半区
            low = middle + 1;
        }
    }

	return -1;
}

int CLLD::removeInArray(int* intList,int node,int cnt)
{
	int ii,leftcnt;
	int* my = new int[cnt];

	for(ii=0; ii<cnt; ii++)
	{
		if (intList[ii] == node)
		{
			//先标记出来
			intList[ii] = -10000;
		}
	}

	leftcnt=0;
	for(ii=0; ii<cnt; ii++)
	{
		if (intList[ii] != -10000)
		{
			//将标记的元素去掉
			my[leftcnt++] = intList[ii];
		}
	}

	for(ii=0; ii<leftcnt; ii++)
	{
		intList[ii] = my[ii];
	}

	delete []my;

	return leftcnt;
}

int CLLD::IsExistInArray(int* intList,int node,int cnt)
{
	//先传出来，再做
	for(int ii=0; ii<cnt; ii++)
	{
		if (intList[ii] == node)
			return ii;
	}

	return -1;
}


int CLLD::getLR_Bykey(char* src,char* key,char* left,char* right)
{
	getleftstr(src,key[0],left);
	getrightstr(src,key[0],right);

	return 1;	
}

void CLLD::filecat(char* srcfile,char* catfile)
{
	char* f1 = srcfile;
	char* f2 = catfile;
	char aa[128];

//	getEnvironmentVariable("DP_HOME",aa);
	char f3[256];
	
	sprintf(f3,"%s\\temp_lld_temp.txt",aa);
	ifstream inFile1(f1);
	ifstream inFile2(f2);
	ofstream outFile(f3);
	outFile << inFile1.rdbuf();
	outFile << inFile2.rdbuf();
	inFile1.close();
	inFile2.close();
	outFile.close();
	
	ifstream inFile3(f3);
	ofstream outFile1(f1);
	outFile1 << inFile3.rdbuf();
	inFile3.close();
	outFile1.close();
	//删掉临时文件
	char line[256];
	sprintf(line,"del %s",f3);
	system(line);
}

//根据环境变量名得到路径，环境变量修改后需要重启计算机
int CLLD::getEnvironmentVariable(char* sEnvironmentName,char* outEnvVar)
{
/*

//获取环境变量
//待操作的环境变量名
	char cBuffer[256];//读取环境变量信息
	cBuffer[0]=0;
	//CString sDir;

	DWORD dwRet = GetEnvironmentVariable(sEnvironmentName, cBuffer, sizeof(cBuffer));
	if (dwRet == 0)
	{
	//    CString sEnvironmentValue("D:\\DP");
	//    BOOL bRet = SetEnvironmentVariable(sEnvironmentName, sEnvironmentValue);
	//    if (bRet == TRUE)
	//           AfxMessageBox("环境变量设置完毕！");
	//    else
	//           AfxMessageBox("环境变量设置失败！");
	}
	else
	{
	//如果环境变量存在，显示环境变量内容
	//    CString sMessage("环境变量QTDIR值为：");
	//    sMessage += CString(cBuffer);
	//    AfxMessageBox(sMessage);

//		return cBuffer;
	}	

	strcpy(outEnvVar,cBuffer);
*/
	return 1;
}


//整形升序
int Compare_int(const void *elem1, const void *elem2)
{
    return *((int *)(elem1)) - *((int *)(elem2));
}

//整形升序
int  CLLD::sortInt(int* dd,int cnt)
{
//    int a[100];
    qsort(dd, cnt, sizeof(int), Compare_int);
    return 0;
} 

//整形降序
int Compare_int_desc(const void *elem1, const void *elem2)
{
    return *((int *)(elem2)) - *((int *)(elem1));
}

//整形降序
int  CLLD::sortint_Desc(int* dd,int cnt)
{
//    int a[100];
    qsort(dd, cnt, sizeof(int), Compare_int_desc);
    return 0;
} 

int cmp_double( const double &a, const double &b ){
    if( a < b )
       return 1;
    else
       return 0;
}

//浮点升序
int  CLLD::sortDouble(double* dd,int cnt)
{
//    int a[100];
//    qsort(dd, cnt, sizeof(dd[0]), Compare_double);
	sort(dd,dd+cnt,cmp_double);

    return 0;
} 

int cmp_double_desc( const double &a, const double &b )
{
    if( a > b )
       return 1;
    else
       return 0;
}

//对数字进行排序，大的在前面
int  CLLD::sortDouble_Desc(double* dd,int cnt)
{
	sort(dd,dd+cnt,cmp_double_desc);

    
	//qsort(dd, cnt, sizeof(dd[0]), Compare_double_desc);


    return 1;
} 

void CLLD::dsort(double& a,double& b)
{
	double c;
	if (a>b)
	{
		c = a;
		a=b;
		b=c;
	}
	
} 

void CLLD::setMathParm(char** param,int num,int paramtype)
{
//	int ii;
//	char sTemp[128];
//	char sL[128];
//	char sR[128];
//
//	if (paramtype == 0)//潮流
//	{	
//		for (ii=1; ii<num; ii++)
//		{
//			getstrByIndex(param,ii,sTemp);
//			getleftstr(sTemp,'=',sL);
//			getrightstr(sTemp,'=',sR);
//			//TRACE(sTemp+"\n");
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"trans_loadRate")==0))
//			{
//			   strcpy(trans_loadRate , sR);
//			   return;
//			}
//		}
//	}
//
//	//默认是可靠性，将读出来的参数写到全局变量
//	if (paramtype == 2)
//	{	
//		for (ii=1; ii<num; ii++)
//		{
//			getstrByIndex(param,ii,sTemp);
//			getleftstr(sTemp,'=',sL);
//			getrightstr(sTemp,'=',sR);
//			//TRACE(sTemp+"\n");
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_ohline_prob")==0))
//			{
//			   strcpy(fault_ohline_prob , sR);
//			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_ohline_rtime")==0))
//			{
//			   strcpy(fault_ohline_rtime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_ohline_ins_prob")==0))
//			{
//			   strcpy(fault_ohline_ins_prob , sR);
//			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_ohline_ins_rtime")==0))
//			{
//			   strcpy(fault_ohline_ins_rtime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_cable_prob")==0))
//			{
//			   strcpy(fault_cable_prob , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_cable_rtime")==0))
//			{
//			   strcpy(fault_cable_rtime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_breaker_prob")==0))
//			{
//			   strcpy(fault_breaker_prob , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_breaker_rtime")==0))
//			{
//			   strcpy(fault_breaker_rtime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_loadbreakswitch_prob")==0))
//			{
//			   strcpy(fault_loadbreakswitch_prob , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_loadbreakswitch_rtime")==0))
//			{
//			   strcpy(fault_loadbreakswitch_rtime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_disconnector_prob")==0))
//			{
//			   strcpy(fault_disconnector_prob , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_disconnector_rtime")==0))
//			{
//			   strcpy(fault_disconnector_rtime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_fuse_prob")==0))
//			{
//			   strcpy(fault_fuse_prob , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_fuse_rtime")==0))
//			{
//			   strcpy(fault_fuse_rtime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_transformer_prob")==0))
//			{
//			   strcpy(fault_transformer_prob , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_transformer_rtime")==0))
//			{
//			   strcpy(fault_transformer_rtime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"up_store")==0))
//			{
//			   strcpy(fault_stime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_stime")==0))
//			{
//			   strcpy(fault_detect , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_up_store")==0))
//			{
//			   strcpy(plan_detect , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"fault_ttime")==0))
//			{
//			   strcpy(fault_ttime , sR);
//  			   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_stime")==0))
//			{
//			   strcpy(plan_stime , sR);
//       		   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_ttime")==0))
//			{
//			   strcpy(plan_ttime , sR);
//       		   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_ohline_prob")==0))
//			{
//			   strcpy(plan_prob , sR);
//       		   continue;
//			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_ohline_rtime")==0))
//			{
//			   strcpy(plan_rtime , sR);
//       		   continue;
//			}
//
//
////			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_ohline_prob")==0))
////			{
////			   strcpy(plan_ohline_prob , sR);
////       		   continue;
////			}
////
////			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_ohline_rtime")==0))
////			{
////			   strcpy(plan_ohline_rtime , sR);
////       		   continue;
////			}
////
////			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_ohline_ins_prob")==0))
////			{
////			   strcpy(plan_ohline_ins_prob , sR);
////       		   continue;
////			}
////
////			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_ohline_ins_rtime")==0))
////			{
////			   strcpy(plan_ohline_ins_rtime , sR);
////       		   continue;
////			}
////
////			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_cable_prob")==0))
////			{
////			   strcpy(plan_cable_prob , sR);
////       		   continue;
////			}
////
////			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"plan_ohline_rtime")==0))
////			{
////			   strcpy(plan_ohline_rtime , sR);
////       		   continue;
////			}
//
//	//fault_ohline_ins_prob=8.3|fault_ohline_ins_rtime=2.6
////plan_ohline_prob=6|plan_ohline_rtime=4|plan_ohline_ins_prob=6|plan_ohline_ins_rtime=4|plan_cable_prob=6|plan_cable_rtime=4
//
//
////			if ((isContainor(sTemp,'=')>0) && (strcmp(sR,"sel_feeders")==0))
////			{
////			   strcpy(sel_feeders , sR);
////       		   continue;
////			}
//
//			if ((isContainor(sTemp,'=')>0) && (strcmp(sL,"trans_loadRate")==0))
//			{
//			   strcpy(trans_loadRate , sR);
//       		   continue;
//			}
//
//		}		
//	}
}

long CLLD::getCurMill()
{
/*	SYSTEMTIME st;
	GetSystemTime(&st);
	return st.wMinute*60*1000 + st.wSecond * 1000 + st.wMilliseconds;  
	*/

	return 0;
}


void CLLD::getstrByIndex(char** strlist,int index,char* dst)
{
	strcpy(dst,strlist[index]);
}

void CLLD::TimeHere(char* here)
{
/*	char	str[128];
	SYSTEMTIME st;
	GetSystemTime(&st);
	long ddd= st.wMinute*60*1000 + st.wSecond * 1000 + st.wMilliseconds;  

	sprintf(str,"%04d%02d%02d %02d%02d%02d %04d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);

	t_here[i_here].id = i_here;
	strcpy(t_here[i_here].descr , here);
	strcpy(t_here[i_here].CurTime ,str) ;
	t_here[i_here].timehere = ddd;
	i_here++;
*/
}

int  CLLD::OutputAllTime(char* outall)//输出所有的时间差
{
	int iB,iE;
	//CString ret,str;
	char sout[128],sBDescr[128],sEDescr[128],sB[128],sE[128];
	long del;
	
	outall[0]=0;
	
	for (int ii=0; ii<i_here-1; ii++)
	{
		iB = ii;
		iE = ii+1;
		strcpy(sBDescr , t_here[iB].descr);
		strcpy(sEDescr , t_here[iE].descr);
		strcpy(sB , t_here[iB].CurTime); 
		strcpy(sE , t_here[iE].CurTime);
		del = t_here[iE].timehere - t_here[iB].timehere;
		sprintf(sout,"%s到%s,时间间隔%d 毫秒\n",sBDescr,sEDescr,del);
		strcat(outall,sout);
	}

	return 1;
}

//比较2个相似向量,返回分叉点的序号，不是分叉点的编号
int CLLD::comp_vector(int* src,int* comp,int& ele)
{
	for(int i=0; i<mx_minpath_len; i++)
	{
		if ((src[i] == -1))
		{
			ele=src[i-1];//最后的那个返回
			return i-1;
		}
			//return  -1;

		if (src[i] != comp[i])//两个开始不一样
		{
			ele=src[i-1];
			return i-1;
		}
	}
	
	return -1;
}

//向量中两个元素进行比较，先出现返回1，后出现返回2，都没用，返回-1
int CLLD::ele_comp(int* src, int ele1, int ele2)
{
	if (ele1==-1)
		return -1;

	for(int i=0; i<mx_minpath_len; i++)
	{
		if (src[i] == ele1)
		{
			return 1;
		}
		if (src[i] == ele2)
		{
			return 2;
		}
	}
	
	return -1;
}

int CLLD::getCrossPoint(int* src,int srclen,int* cmp,int cmplen,int& cross_nd)
{
	int i,len,diff;
	diff = srclen - cmplen; 
	if (diff < 0)
	{
		diff = -diff;
		len = cmplen;
		for (i=len-1; i>=0; i--)
		{
			if (cmp[i] < 0)
				continue;

			if (i<diff)
			{
				cross_nd = cmp[i+1];
				return i+1-diff;
			}
			if (cmp[i] != src[i-diff])
			{
				cross_nd = cmp[i+1];
				return i+1-diff;
			}
		}
	}
	else
	{
		len = srclen;
		for (i=len-1; i>=0; i--)
		{
			if (src[i] < 0)
				continue;

			if (i<diff)
			{
				cross_nd = src[i+1];
				return i+1;
			}
			if (src[i] != cmp[i-diff])
			{
				cross_nd = src[i+1];
				return i+1;
			}
		}
	}

	return -1;
}


int CLLD::ele_vector(int* src, int ele, int len)
{
	for(int i=0; i<len; i++)
	{
		if (src[i] == ele)
		{
			return i;
		}
	}
	
	return -1;
}

int CLLD::ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr)
{
    int  StringLen;
    char* caNewString = new char[mx_sql_char];

    char *FindPos = strstr(sSrc, sMatchStr);
    if ((!FindPos) || (!sMatchStr))
    {
        delete[] caNewString;
        return -1;
    }

    while (FindPos)
    {
        memset(caNewString, 0, sizeof(caNewString));
        StringLen = FindPos - sSrc;
        strncpy(caNewString, sSrc, StringLen);
        caNewString[StringLen] = '\0';
        strcat(caNewString, sReplaceStr);
        strcat(caNewString, FindPos + strlen(sMatchStr));
        strcpy(sSrc, caNewString);

        FindPos = strstr(sSrc, sMatchStr);
    }

    delete[] caNewString;

    return 0;
}

char* CLLD::replacestr(char *strbuf, char *sstr, char *dstr)
{  
//	char *p,*p1;
//	pl = new char[10];
//    int len;
//
//    if ((strbuf == NULL)||(sstr == NULL)||(dstr == NULL))
//            return NULL;
//
//    p = strstr(strbuf, sstr);       //返回字符串第一次出现的地址,否则返回NULL
//    if (p == NULL)  /*not found*/
//            return NULL;
//
//    len = strlen(strbuf) + strlen(dstr) - strlen(sstr);
//    p1 = malloc(len);
//    bzero(p1, len);
//    strncpy(p1, strbuf, p-strbuf);
//    strcat(p1, dstr);
//    p += strlen(sstr);
//    strcat(p1, p);
    return NULL;
}