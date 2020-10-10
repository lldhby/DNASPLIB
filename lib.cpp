
#if _WIN32
#include "../../DNASP/DNASPLIB/lib.h"
#elif __linux__
#else
#endif

#include "../../DNASP/DNASPDLLLIB/Nmd.h"
#include "../../DNASP/DNASPDLLLIB/Tp.h"

//�����������ݺ�������Ҫ�ļ���ģ��
int InitMath(DSBOB2* dsbob2, CLLD* lld,int isErrLog,int isPrintMidResult)
{
	int ret = 0;
	lld->mx_bs = lld->mx_nd = lld->mx_cb + lld->mx_sec + lld->mx_so + 1000;

	//ֻ����������־
	char logLine[128];
	strcpy(lld->logfile, "dllerr.log");
	FILE *fp;
	fp = fopen("dllerr.log", "w");
	if (fp != NULL)
	{
		lld->getCurTime(logLine);
		fprintf(fp, logLine);
		fprintf(fp, "\n");
		fclose(fp);
	}

	NMD* nmd = new NMD(dsbob2, lld);

	if ((ret = nmd->DoValidate()) > 0)//���˼��飬���˼���ȫ�������ڴ��������
	{
	}
	else
	{
		sprintf(logLine, "\n...nmd...û�гɹ�");
		lld->WriteLog(lld->logfile, logLine);

		delete nmd;
		return -2;
	}
	if (!dsbob2->ReadDigitalData())
	{
		sprintf(logLine, "\n...ң����...û�ж���");
		lld->WriteLog(lld->logfile, logLine);

		delete nmd;
		return -3;
	}

	TP* tp = new TP(dsbob2, lld);
	if (tp->tp_allnew() == TP_FAIL)
	{
		sprintf(logLine, "\n...����...û�гɹ�");
		lld->WriteLog(lld->logfile, logLine);

		delete tp;
		delete nmd;
		return -4;
	}

	return 0;
}