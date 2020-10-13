#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <utime.h>

typedef bool(*_LPINITXLTSM)
(
);

typedef bool(*_LPMSG)
(
);

typedef bool(*_LPTERM)
(
);

typedef bool(*_LPSETSERVERINFO)
(
const char* szProtocol, const char* szURL, const char* szPort, const char* szSubPath
);

typedef bool(*_LPSETSERVERINFOFED5)
(
char* szVersion, char* szTimeServerURL
);

typedef bool(*_LPSETTIMESTAMP)
(
time_t gmt
);

typedef bool(*_LPSETTIMESTAMP_SERVERTIME)
(
const char* szProtocol, const char* szServer, const char* szPort, const char* szSubUrl
);

typedef bool(*_LPONETIMESYNCRONIZATION)
(
);

typedef int(*_LPGETTIMEBYXLTSM)
(
time_t *pTime
);

typedef struct XLTSMFuncStruct
{
	_LPINITXLTSM pfnInitXltsm;
	_LPMSG pfnMsg;
	_LPTERM pfnTerm;
	_LPSETSERVERINFO pfnSetServerInfo;
	_LPSETSERVERINFOFED5 pfnSetServerInfoFED5;
	_LPSETTIMESTAMP pfnSetTimeStamp;
	_LPSETTIMESTAMP_SERVERTIME pfnSetTimeStamp_ServerTime;
	_LPONETIMESYNCRONIZATION pfnOneTimeSyncronization;
	_LPGETTIMEBYXLTSM pfnGetTimeByXltsm;
}XLTSMFuncStruct;
typedef XLTSMFuncStruct* LPXLTSMFUNCSTRUCT;

extern "C"
{
	int XLTSMInitialize(void** ppXLTSMFuncStruct);
}