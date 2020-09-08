#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#include <cstring>

#include "f_extadk.h"
#include "NXAPI.h"
#include "ContentInfo.h"

LPDIM g_lpDIM = NULL;
LPSIM g_lpSIM = NULL;

int ADK_SetLastError(DWORD dwError);
NXLINTERFACE *g_pNxlInterface;

#define NXLPATH "./libf_nxl.so"
typedef PNXLINTERFACE(*_LPNXINITIALIZE)(void);

wchar_t* ConvertCtoW(char* szInput)
{
	const size_t iSize = strlen(szInput) + 1;
	wchar_t* wcsOutput = new wchar_t[iSize];
	mbstowcs(wcsOutput, szInput, iSize);

	return wcsOutput;
}

void __attribute__((constructor))ADK_init(void)
{
	g_lpDIM = (LPDIM)malloc(sizeof(DIM));
	g_lpSIM = (LPSIM)malloc(sizeof(SIM));

	g_pNxlInterface = (PNXLINTERFACE)malloc(sizeof(NXLINTERFACE));
}

void __attribute__((destructor))ADK_fini(void)
{
	if (g_lpDIM)
	{
		free(g_lpDIM);
	}

	if (g_lpSIM)
	{
		free(g_lpSIM);
	}

	if (g_pNxlInterface)
	{
		free(g_pNxlInterface);
	}
}

unsigned long get_Version()
{
	//Implementation
	printf(" get_Version called!!..\n");
	return 1;
}

unsigned int get_Authenticate(char *szFilePath)
{
	//Implementation
	printf(" get_Authenticate called!!..\n");
	return 1;
}

unsigned long set_NotifyMessage()
{
	//Implementation
	printf(" set_NotifyMessage called!!..\n");
	return 1;
}

unsigned int set_Menu(unsigned int uiMenuID)
{
	//Implementation
	printf(" set_Menu called!!..\n");
	return 1;
}

bool ADK_NxInitialize()
{
//	printf("[ADK_NxInitialize] Enter..\n");
	static bool bNxlInitialized = false;
	
	if (!bNxlInitialized)
	{
		void* pDllHandle = NULL;
		
		// https://www.joinc.co.kr/w/Site/C++/Documents/Dynamic_Class_Loading

		pDllHandle = dlopen(NXLPATH, RTLD_LAZY);
		if (!pDllHandle)
		{
//			printf("dlopen failed..\n");
			fputs(dlerror(), stderr);
			printf("%s\n", dlerror());
			
			return false;
		}

		_LPNXINITIALIZE pfnNxInitialize = (_LPNXINITIALIZE)dlsym(pDllHandle, "NxInitialize");
		
		g_pNxlInterface = pfnNxInitialize();

		bNxlInitialized = true;
	}

	return true;
}

int ADK_IsLicenseVaildByPath(char *szFilePath, int iPurpose)
{
	printf("[ADK_IsLicenseVaildByPath] Enter..\n");

	if (NULL == szFilePath || '\0' == szFilePath[0])
	{
		printf("[ADK_IsLicenseVaildByPath] Invalid param.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	if (!ADK_NxInitialize())
	{
		printf("[ADK_IsLicenseVaildByPath] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);	
	
		return 0;
	}

	FILE* fp = fopen(szFilePath, "r");
	if (NULL == fp)
	{
		printf("[ADK_IsLicenseVaildByPath] File open failed.\n");

		ADK_SetLastError(E_ADK_WIN32_CREATEFILE_FAIL);

		return 0;
	}

	wchar_t* wcsFilePath = ConvertCtoW(szFilePath);
	if (NULL == wcsFilePath || '\0' == wcsFilePath[0])
	{
		printf("[ADK_IsLicenseVaildByPath] Convert fail.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	LPCONTENTINFO lpContentInfo = g_pNxlInterface->OpenContentByHandle(fp, wcsFilePath, TRUE, FALSE, FALSE);
	if (NULL == lpContentInfo)
	{
		printf("[ADK_IsLicenseValidByPath] OpenContent failed.\n");

		ADK_SetLastError(E_ADK_CONTENT_OPEN_FAIL);
	
		fclose(fp);

		return 0;
	}

	if (!g_pNxlInterface->IsSecure(lpContentInfo))
	{
		printf("[ADK_IsLicenseValidByPath] Not secure content.\n");

		g_pNxlInterface->CloseContentInfo(lpContentInfo);

		ADK_SetLastError(E_ADK_NOT_SECURE_CONTENT);

		fclose(fp);

		return 0;
	}

	bool bRet = g_pNxlInterface->IsLicenseValid(lpContentInfo, iPurpose);
	if (!bRet)
	{
		printf("[ADK_IsLicenseValidByPath] License invalid.\n");

		ADK_SetLastError(E_ADK_CONTENT_INVALID_LICENSE);

		fclose(fp);

		return 0;
	}

	printf("[ADK_IsLicenseValidByPath] License valid. '%s'\n", szFilePath);
	
	ADK_SetLastError(E_ADK_OK);

 	fclose(fp);

	return 1;
}

int ADK_IsSecureContentByPath(char *szFilePath)
{
	printf("[ADK_IsSecureContentByPath] Enter..\n");

	if (NULL == szFilePath || '\0' == szFilePath[0])
	{
		printf("[ADK_IsSecureContentByPath] Invalid param.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	printf("[ADK_IsSecureContentByPath] '%s'\n", szFilePath);

	if (!ADK_NxInitialize())
	{
		printf("[ADK_IsSecureContentByPath] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	FILE* fp = fopen(szFilePath, "r");
	if (NULL == fp)
	{
		printf("[ADK_IsSecureContentByPath] File open failed.\n");

		ADK_SetLastError(E_ADK_WIN32_CREATEFILE_FAIL);

		return 0;
	}
	
	if (!g_pNxlInterface->IsSecureFileByHandle(fp))
	{
		printf("[ADK_IsSecureContentByPath] Not secure file.\n");

		ADK_SetLastError(E_ADK_NOT_SECURE_CONTENT);

		fclose(fp);

		return 0;
	}

	printf("[ADK_IsSecureContentByPath] Secure file. '%s'\n", szFilePath);
	
	ADK_SetLastError(E_ADK_OK);

	fclose(fp);

	return 1;
}

// TODO::
int ADK_GetDomainIdByPath(char *szFilePath, char *szDomainID, int iBufferSize)
{
	printf("[ADK_GetDomainIdByPath] Enter..");

/*	if (NULL == szFilePath || "")*/

	return 1;
}

void* ADK_OpenContent(char *szFilePath, int iWritable, int iTruncate)
{
	printf("[ADK_OpenContent] Enter..\n");

	if (NULL == szFilePath || '\0' == szFilePath[0])
	{
		printf("[ADK_OpenContent] Invalid param.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return NULL;
	}
		
	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_OpenContent] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	FILE* fp;
	if (0 == iWritable)
	{
		fp = fopen(szFilePath, "r");
	}
	else
	{
		if (0 == iTruncate)
		{
			fp = fopen(szFilePath, "r+");
		}
		else
		{
			// 파일을 새로 생성하며, 이미 파일이 존재할 경우 파일의 내용을 모두 삭제하고 연다.
			fp = fopen(szFilePath, "w+");
		}
	}

	if (NULL == fp)
	{
		printf("[ADK_OpenContent] File open failed.\n");

		ADK_SetLastError(E_ADK_WIN32_CREATEFILE_FAIL);

		return 0;
	}

	wchar_t* wcsFilePath = ConvertCtoW(szFilePath);
	if (NULL == wcsFilePath || '\0' == wcsFilePath[0])
	{
		printf("[ADK_OpenContent] Convert fail.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	LPCONTENTINFO lpContentInfo = g_pNxlInterface->OpenContentByHandle(fp, wcsFilePath, true, false, false);
	if (NULL == lpContentInfo)
	{
		printf("[ADK_OpenContent] OpenContent failed.\n");

		ADK_SetLastError(E_ADK_CONTENT_OPEN_FAIL);

		fclose(fp);

		return NULL;
	}

	if (!g_pNxlInterface->IsLicenseValid(lpContentInfo, ADK_PURPOSE_VIEW))
	{
		printf("[ADK_OpenContent] VIEW license invalid.\n");

		ADK_SetLastError(E_ADK_CONTENT_INVALID_LICENSE);

		fclose(fp);

		return NULL;
	}

	if (1 == iWritable && !g_pNxlInterface->IsLicenseValid(lpContentInfo, ADK_PURPOSE_SECURE_SAVE))
	{
		printf("[ADK_OpenContent] SECURE_SAVE license invalid.\n");

		ADK_SetLastError(E_ADK_CONTENT_INVALID_LICENSE);

		fclose(fp);

		return NULL;
	}

	printf("[ADK_OpenContent] OpenContent success. '%s'\n", szFilePath);

	ADK_SetLastError(E_ADK_OK);

	return (void *)lpContentInfo;
}

int ADK_IsSecureContent(void *pContentInfo)
{
	printf("[ADK_IsSecureContent] Enter..\n");

	if (NULL == pContentInfo)
	{
		printf("[ADK_IsSecureContent] Invalid param.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;
	
	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_IsSecureContent] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	bool bSecure = g_pNxlInterface->IsSecure(lpContentInfo);
	if (!bSecure)
	{
		printf("[ADK_IsSecureContent] Not secure content.\n");

		ADK_SetLastError(E_ADK_NOT_SECURE_CONTENT);

		return 0;
	}

	printf("[ADK_IsSecureContent] Secure content.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

int ADK_IsLicenseValid(void *pContentInfo, WORD wPurpose)
{
	printf("[ADK_IsLicenseValid] Enter..\n");

	if (NULL == pContentInfo)
	{
		printf("[ADK_IsLicenseValid] Invalid param.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_IsLicenseValid] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	bool bRet = g_pNxlInterface->IsLicenseValid(lpContentInfo, wPurpose);
	if (!bRet)
	{
		printf("[ADK_IsLicenseValid] License invalid.\n");

		ADK_SetLastError(E_ADK_CONTENT_INVALID_LICENSE);

		return 0;
	}

	printf("[ADK_IsLicenseValid] License valid.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

int ADK_SetContentPointer(void *pContentInfo, long lOffset, int iOrigin)
{
	printf("[ADK_SetContentPointer] Enter..\n");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_SetContentPointer] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	int ibRet = g_pNxlInterface->SetContentPointer(lpContentInfo, lOffset, NULL, iOrigin);
	if (ibRet == -1) // INVALID_SET_CONTENTPOINTER
	{
		printf("[ADK_SetContentPointer] SetContentPointer failed.\n");

		ADK_SetLastError(E_ADK_CONTENT_SET_POINTER_FAIL);

		return 0;
	}

	printf("[ADK_SetContentPointer] SetContentPointer success.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

// TODO::여기서부터 define된 값으로 변경. 위에 변경 필요
int ADK_ReadContent(void *pContentInfo, void* pBuffer, unsigned long dwNumberOfBytesToRead, unsigned long* lpdwNumberOfBytesToRead)
{
	printf("[ADK_ReadContent] Enter..\n");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_ReadContent] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	bool bRet = g_pNxlInterface->ReadContent(lpContentInfo, pBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesToRead);
	if (!bRet)
	{
		printf("[ADK_ReadContent] ReadContent failed.\n");

		ADK_SetLastError(E_ADK_CONTENT_READ_FAIL);

		return 0;
	}
	
	printf("[ADK_ReadContent] ReadContent success.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

int ADK_WriteContent(void *pContentInfo, void* pBuffer, unsigned long dwNumberOfBytesToWrite, unsigned long* lpdwNumberOfBytesToWrite)
{
	printf("[ADK_WriteContent] Enter..\n");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_WriteContent] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	bool bRet = g_pNxlInterface->WriteContent(lpContentInfo, pBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesToWrite);
	if (!bRet)
	{
		printf("[ADK_WriteContent] WriteContent failed.\n");

		ADK_SetLastError(E_ADK_CONTENT_WRITE_FAIL);

		return 0;
	}

	printf("[ADK_WriteContent] WriteContent success.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

int ADK_SetEndOfContent(void *pContentInfo)
{
	printf("[ADK_SetEndOfContent] Enter..\n");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_SetEndOfContent] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	bool bRet = g_pNxlInterface->SetEndOfContent(lpContentInfo);
	if (!bRet)
	{
		printf("[ADK_SetEndOfContent] SetEndOfContent failed.\n");

		ADK_SetLastError(E_ADK_CONTENT_SET_POINTER_FAIL);

		return 0;
	}

	printf("[ADK_SetEndOfContent] SetEndofContent success.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

int ADK_CloseContent(void *pContentInfo)
{
	printf("[ADK_CloseContent] Enter..\n");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_CloseContent] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	FILE* fp = lpContentInfo->hFile;
	g_pNxlInterface->CloseContentInfo(lpContentInfo);

	if (NULL != fp)
	{
		fclose(fp);
	}

	printf("[ADK_CloseContent] CloseContentInfo success.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

DWORD ADK_GetLastError()
{
	return LASTERROR;
}

int ADK_SetLastError(unsigned long dwError)
{
	g_dwTlsError = dwError;

	return 1;
}

DWORD ADK_GetDomainId(void *pContentInfo, char *szDomainID, unsigned int cchDomainId)
{
	printf("[ADK_GetDomainId] Enter..\n");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_GetDomainId] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	if (!g_pNxlInterface->IsSecure(lpContentInfo))
	{
		printf("[ADK_GetDomainId] Not secure content.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	if (NULL == lpContentInfo->lpContentLicense)
	{
		printf("[ADK_GetDomainId] Can't get domainID. lpContentLicense is NULL.\n");

		ADK_SetLastError(E_ADK_GENERAL);

		return 0;
	}

	const wchar_t* pwcsDomainIdFromContent = lpContentInfo->lpContentLicense->GetCPID();

	if (NULL == pwcsDomainIdFromContent)
	{
		printf("[ADK_GetDomainId] Can't get domainID. GetCPID failed.\n");

		ADK_SetLastError(E_ADK_INTERNAL_NXL_GET_DOMAINID_FAIL);

		return 0;
	}

	unsigned long dwLength;

	printf("[ADK_GetDomainId] GetDomainId success.\n");

	ADK_SetLastError(E_ADK_OK);

	return dwLength;
}

// FSN만 지원하므로 GetContentType은 오로지 FSN만 리턴
int ADK_GetContentType(void *pContentInfo)
{
	printf("[ADK_GetContentType] Enter..");

	return 3;	// ADK_CONTENT_FSN
}

int ADK_PackContent(void *pContentInfoTarget, void *pContentInfoTemplate)
{
	printf("[ADK_PackContent] Enter..\n");

	LPCONTENTINFO lpContentInfoTarget = (LPCONTENTINFO)pContentInfoTarget;
	LPCONTENTINFO lpContentInfoTemplate = (LPCONTENTINFO)pContentInfoTemplate;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_PackContent] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	if (!g_pNxlInterface->IsSecure(lpContentInfoTemplate))
	{
		printf("[ADK_PackContent] Not secure file.\n");

		ADK_SetLastError(E_ADK_NOT_SECURE_CONTENT);

		return 0;
	}

	if (!g_pNxlInterface->IsLicenseValid(lpContentInfoTemplate, ADK_PURPOSE_VIEW) ||
		!g_pNxlInterface->IsLicenseValid(lpContentInfoTemplate, ADK_PURPOSE_SECURE_SAVE))
	{
		printf("[ADK_PackContent] License invalid.\n");

		ADK_SetLastError(E_ADK_CONTENT_INVALID_LICENSE);

		return 0;
	}

	if (!g_pNxlInterface->PackByHandleTemplate(lpContentInfoTarget->hFile, lpContentInfoTarget->pwcsFilePath, lpContentInfoTemplate))
	{
		printf("[ADK_PackContent] PackByHandleTemplate failed.\n");

		ADK_SetLastError(E_ADK_CONTENT_PACK_FAIL);

		return 0;
	}

	printf("[ADK_PackContent] PackByHandleTemplate success.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

int ADK_AutoPackContent(char *szFilePath)
{
	printf("[ADK_AutoPackContent] Enter..\n");

	if (NULL == szFilePath || '\0' == szFilePath[0])
	{
		printf("[ADK_AutoPackContent] Invalid param.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_AutoPackContent] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	FILE* fp = fopen(szFilePath, "r+");
	if (NULL == fp)
	{
		printf("[ADK_AutoPackContent] File open failed.\n");

		ADK_SetLastError(E_ADK_WIN32_CREATEFILE_FAIL);

		return 0;
	}

	wchar_t* wcsFilePath = ConvertCtoW(szFilePath);
	if (NULL == wcsFilePath || '\0' == wcsFilePath[0])
	{
		printf("[ADK_AutoPackContent] Convert fail.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}
	
	if (!g_pNxlInterface->PackByHandle(fp, wcsFilePath, NULL))
	{
		printf("[ADK_AutoPackContent] PackByHandle failed.\n");

		ADK_SetLastError(E_ADK_CONTENT_PACK_FAIL);

		fclose(fp);

		return 0;
	}

	printf("[ADK_AutoPackContent] PackByHandle success.\n");

	ADK_SetLastError(E_ADK_OK);

	fclose(fp);

	return 1;
}

int ADK_SetUsage(void *pContentInfoTarget, unsigned short wPurpose)
{
	printf("[ADK_SetUsage] Enter..\n");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfoTarget;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_SetUsage] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

// 	bool bRet = g_pNxlInterface->SetUsage(lpContentInfo, wPurpose, FALSE);
// 	if (!bRet)
// 	{
// 		printf("[ADK_SetUsage] SetUsage failed.\n");
// 
// 		ADK_SetLastError(E_ADK_GENERAL);
// 
// 		return 0;
// 	}

	printf("[ADK_SetUsage] SetUsage success.\n");

	ADK_SetLastError(E_ADK_OK);

	return 1;
}

int ADK_IsLogonStatus(char *szDomainID)
{
	printf("[ADK_IsLogonStatus] Enter..\n");

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_IsLogonStatus] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	if (NULL == szDomainID || '\0' == szDomainID[0])
	{
// 		bool bLogin = g_pNxlInterface->GetLoginStatus();
// 
// 		return bLogin;
	}

	wchar_t* wcsDomainID = ConvertCtoW(szDomainID);
	if (NULL == wcsDomainID || '\0' == wcsDomainID[0])
	{
		printf("[ADK_IsLogonStatus] Convert fail.\n");

		ADK_SetLastError(E_ADK_INVALID_ARGS);

		return 0;
	}

	char *szStatus = NULL;
//	szStatus = g_pNxInterface->GetLogonStatusW(wcsDomainID);
	if (szStatus != NULL &&
		(strcasecmp(szStatus, "ONLINE") == 0 ||
		strcasecmp(szStatus, "OFFLINE") == 0 ||
		strcasecmp(szStatus, "TAKEOUT") == 0 ||
		strcasecmp(szStatus, "LOGON") == 0))
	{
		printf("[ADK_IsLogonStatus] Logon status.\n");

		ADK_SetLastError(E_ADK_OK);

		return 1;
	}
	
	printf("[ADK_IsLogonStatus] Not logon status.\n");

	ADK_SetLastError(E_ADK_GENERAL);

	return 0;
}

unsigned long ADK_GetContentSize(void *pContentInfo)
{
	printf("[ADK_GetContentSize] Enter..\n");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_GetContentSize] NxInitialize failed.\n");

		ADK_SetLastError(E_ADK_NXL_INIT_FAIL);

		return 0;
	}

	unsigned long dwContentSize;
	if (g_pNxlInterface->IsSecure(lpContentInfo))
	{
		dwContentSize = g_pNxlInterface->GetEncryptedContentSize(lpContentInfo, NULL);
	}
	else
	{
		dwContentSize = g_pNxlInterface->GetOriginalContentSize(lpContentInfo, NULL);
	}

	return dwContentSize;
}

#pragma GCC visibility push(default)
int DRMInterfaceInitialize(void **ppDocumentInterface, void **ppSystemInterface, void *pOSInformation, void* pOfficeInformation)
{
	g_lpDIM->pfngetVersion = get_Version;
	g_lpDIM->pfngetAuthenticate = get_Authenticate;
	g_lpDIM->pfnsetNotifyMessage = set_NotifyMessage;
	g_lpDIM->pfnsetMenu = set_Menu;

	*(LPDIM *)ppDocumentInterface = g_lpDIM;

	g_lpSIM->pfnADKIsLicenseVaildByPath = ADK_IsLicenseVaildByPath;
	g_lpSIM->pfnADKIsSecureContentByPath = ADK_IsSecureContentByPath;
	g_lpSIM->pfnADKGetDomainIdByPath = ADK_GetDomainIdByPath;
	g_lpSIM->pfnADKOpenContent = ADK_OpenContent;
	g_lpSIM->pfnADKIsSecureContent = ADK_IsSecureContent;
	g_lpSIM->pfnADKIsLicenseValid = ADK_IsLicenseValid;
	g_lpSIM->pfnADKSetContentPointer = ADK_SetContentPointer;
	g_lpSIM->pfnADKReadContent = ADK_ReadContent;
	g_lpSIM->pfnADKWriteContent = ADK_WriteContent;
	g_lpSIM->pfnADKSetEndOfContent = ADK_SetEndOfContent;
	g_lpSIM->pfnADKCloseContent = ADK_CloseContent;
	g_lpSIM->pfnADKGetLastError = ADK_GetLastError;
	g_lpSIM->pfnADKSetLastError = ADK_SetLastError;
	g_lpSIM->pfnADKGetDomainId = ADK_GetDomainId;
	g_lpSIM->pfnADKGetContentType = ADK_GetContentType;
	g_lpSIM->pfnADKPackContent = ADK_PackContent;
	g_lpSIM->pfnADKAutoPackContent = ADK_AutoPackContent;
	g_lpSIM->pfnADKSetUsage = ADK_SetUsage;
	g_lpSIM->pfnADKIsLogonStatus = ADK_IsLogonStatus;
	g_lpSIM->pfnADKGetContentSize = ADK_GetContentSize;

	*(LPSIM *)ppSystemInterface = g_lpSIM;

	return 1;
}
#pragma GCC visibility pop