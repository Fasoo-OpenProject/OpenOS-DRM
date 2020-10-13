// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "XL_TimeStampManager.h"
#include <wchar.h>
#include <cstring>
#include <stdarg.h>

#define PM_MODULEPATH		"/usr/local/fasoo/f_cmh_pm.dll"
#define ERROR_SUCCESS       0L
#define WAIT_TIMEOUT        258L
#define STATUS_WAIT_0       ((unsigned long)0x00000000L)    
#define WAIT_FAILED			((unsigned long)0xFFFFFFFF)
#define WAIT_OBJECT_0       ((STATUS_WAIT_0) + 0)

LPXLTSMFUNCSTRUCT g_lpXLTSMFuncStruct = NULL;

bool g_bInitialized = false;
bool g_bTimeSetByServer = false;

unsigned long g_dwSyncPeriod = 60;
unsigned long g_dwReconnectTime2Server = 60;		// Unit : second
unsigned long g_dwTimeLag4ShowOptionDlg = 60;		// Unit : Min.

static time_t g_timeFirstServer = -1;
static time_t g_timeFirstLocalTimeForServer = -1;
static time_t g_timeLastServerTryTime = -1;

sem_t g_semEventExit;

void* TimerProcThread(void* lpParam);
unsigned long WaitForSingleObject(unsigned long dwMilliseconds);
time_t _GetLocalTime();
time_t GetTSFile(time_t *pltime);
bool SetTSFile(time_t t);

void __attribute__((constructor))f_xltsm_init(void)
{
	g_lpXLTSMFuncStruct = (LPXLTSMFUNCSTRUCT)malloc(sizeof(XLTSMFuncStruct));
}

void __attribute__((destructor))f_xltsm_fini(void)
{
	sem_destroy(&g_semEventExit);

	if (g_lpXLTSMFuncStruct)
	{
		free(g_lpXLTSMFuncStruct);
	}
}

unsigned long WaitForSingleObject(unsigned long dwMilliseconds)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += (dwMilliseconds / 1000);
	ts.tv_nsec += (dwMilliseconds % 1000) * 1000000;
	if (ts.tv_nsec > 1000000000)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}

	if (sem_timedwait(&g_semEventExit, &ts))
	{
		int nErrNo = errno;
		if (ETIMEDOUT == errno)
		{
			return WAIT_TIMEOUT;
		}
	
		return WAIT_FAILED;
	}

	return WAIT_OBJECT_0;
}

bool _initXltsm(void)
{
	if (!g_bInitialized)
	{
		sem_init(&g_semEventExit, 0, 0);
		pthread_t tId;
		pthread_create(&tId, NULL, TimerProcThread, NULL);
		
		g_bInitialized = true;
	}

	return true;
}

bool _Msg(void)
{
	return true;
}

bool _Term(void)
{
	return true;
}

bool _SetServerInfo(const char* szProtocol, const char* szURL, const char* szPort, const char* szSubPath)
{
	return true;
}

bool _SetServerInfoFED5(char* szVersion, char* szTimeServerURL)
{
	return true;
}

bool _SetTimeStamp(time_t gmt)
{
	if (gmt == 0xffffffff)
		return false;

	SetTSFile(gmt);

	return true;
}

bool _SetTimeStamp_ServerTime(const char* szProtocol, const char* szServer, const char* szPort, const char* szSubUrl)
{
	return true;
}

bool _OneTimeSynchronization(void)
{
	return true;
}

int _GetTimeByXltsm(time_t *pTime)
{
	FILE *fp = NULL;
	fp = fopen(PM_MODULEPATH, "a+");
	if (fp == NULL)
	{
		return 0;
	}
	fclose(fp);
	
	struct stat st;
	if (stat(PM_MODULEPATH, &st) != 0)
	{
		return 0;
	}

	*pTime = st.st_mtime;

	return 1;
}

void* TimerProcThread(void* lpParam)
{
	void* t;

	time_t iTime = -1;
	time_t iLocalTime = -1;

	bool bReconnect = true;
	
	do 
	{
		unsigned long dwResult = WAIT_TIMEOUT;
		if (bReconnect == false)
		{
			dwResult = WaitForSingleObject(g_dwSyncPeriod * 1000);
		}
		else
		{
			bReconnect = false;
			dwResult = WAIT_TIMEOUT;
		}

		if (dwResult == WAIT_OBJECT_0)
		{
			printf("break.\n");
			break;
		}
		else if (dwResult == WAIT_TIMEOUT)
		{
			GetTSFile(&iTime);

			iTime += g_dwSyncPeriod;
			iLocalTime = _GetLocalTime();

			if (iLocalTime > iTime)
				iTime = iLocalTime;

			SetTSFile(iTime);
		}

	} while (1);

	return 0;
}

time_t _GetLocalTime()
{
	time_t _llocaltime = -1;
	struct timeb tstruct;

	time(&_llocaltime);
	ftime(&tstruct);

	_llocaltime += (tstruct.timezone * 60);

	return _llocaltime;
}

time_t GetTSFile(time_t *pltime)
{
	struct stat st;
	if (stat(PM_MODULEPATH, &st) != 0)
	{
		return false;
	}

	*pltime = st.st_mtime;

	struct tm *tmConvert;
	tmConvert = localtime(pltime);

	return *pltime;
}

bool SetTSFile(time_t t)
{
	struct tm *tmConvert;
	tmConvert = localtime(&t);

	struct utimbuf uti;
	uti.actime = t;
	uti.modtime = t;
	
	utime(PM_MODULEPATH, &uti);

	return true;
}

#pragma GCC visibility push(default)
int XLTSMInitialize(void **ppXLTSMFuncStruct)
{
	g_lpXLTSMFuncStruct->pfnInitXltsm = _initXltsm;
	g_lpXLTSMFuncStruct->pfnMsg = _Msg;
	g_lpXLTSMFuncStruct->pfnTerm = _Term;
	g_lpXLTSMFuncStruct->pfnSetServerInfo = _SetServerInfo;
	g_lpXLTSMFuncStruct->pfnSetServerInfoFED5 = _SetServerInfoFED5;
	g_lpXLTSMFuncStruct->pfnSetTimeStamp = _SetTimeStamp;
	g_lpXLTSMFuncStruct->pfnSetTimeStamp_ServerTime = _SetTimeStamp_ServerTime;
	g_lpXLTSMFuncStruct->pfnOneTimeSyncronization = _OneTimeSynchronization;
	g_lpXLTSMFuncStruct->pfnGetTimeByXltsm = _GetTimeByXltsm;

	*(LPXLTSMFUNCSTRUCT*)ppXLTSMFuncStruct = g_lpXLTSMFuncStruct;
	
	return 1;
}
#pragma GCC visibility pop