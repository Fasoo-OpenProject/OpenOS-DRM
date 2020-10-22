#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdarg.h>
#include "DRMUtil.h"
#include "f_extadk.h"

#define EXTADKPATH "/usr/local/fasoo/libfasooextadk.so"
#define LOGPATH "/usr/local/fasoo/log/libfasooextadk.log"

typedef int(*_LPDRMInterfaceInitialize)(void **ppDocumentInterface, void **ppSystemInterface, void *pOSInformation, void* pOfficeInformation);

TO_OFFICE_DRM_SIM *g_lpTOSIM = NULL;
TO_OFFICE_DRM_DIM *g_lpTODIM = NULL;

LPSIM g_lpSIM = NULL;
LPDIM g_lpDIM = NULL;
FVI g_fOSVI;
FVI g_fOFFICEVI;

bool WriteLogFile(char* pszMessage)
{
	if (pszMessage == NULL)
	{
		return false;
	}
	bool bResult = true;
	FILE* hFile = fopen(LOGPATH, "ab+");
	if (hFile == NULL)
	{
		return false;
	}
	unsigned int dwPos = fseek(hFile, 0, SEEK_END);
	if (dwPos != -1)
	{
		long dwBytes = fwrite(pszMessage, sizeof(char), strlen(pszMessage), hFile);
		if (strlen(pszMessage) > dwBytes)
			bResult = false;
	}
	fclose(hFile);
	return bResult;
}

void _TRACEA(unsigned long ltype, const char* format, ...)
{
	if (format == NULL)
		return;
	char buffer[8192] = { 0 };
	strcpy(buffer, "[OfficeDRMUtil] ");
	va_list		vaMarker;
	va_start(vaMarker, format);
	vsprintf(buffer + strlen("[OfficeDRMUtil] "), format, vaMarker);
	va_end(vaMarker);
	if (buffer[strlen(buffer) - 1] != L'\n')
		strcat(buffer, "\n");
	WriteLogFile(buffer);
	printf(buffer);
}

void __attribute__((constructor))OfficeDRMUtil_init(void)
{

}

void __attribute__((destructor))OfficeDRMUtil_fini(void)
{
	if (g_lpTODIM)
		free(g_lpTODIM);

	if (g_lpTOSIM)
		free(g_lpTOSIM);

	if (g_lpDIM)
		free(g_lpDIM);

	if (g_lpSIM)
		free(g_lpSIM);
}

bool FasooDrmInitialize()
{
	void* hDll = NULL;
	hDll = dlopen(EXTADKPATH, RTLD_LAZY);
	if (!hDll)
	{
		_TRACEA(0, "[FasooDrmInitialize] ExtADK dlopen failed. '%s'", dlerror());

		return false;
	}

	_LPDRMInterfaceInitialize pfnDRMInterfaceInitialize = (_LPDRMInterfaceInitialize)dlsym(hDll, "DRMInterfaceInitialize");
	if (!pfnDRMInterfaceInitialize)
	{
		_TRACEA(0, "[FasooDrmInitialize] ExtADK dlsym failed. '%s'", dlerror());
	}

	if (pfnDRMInterfaceInitialize((void**)&g_lpDIM, (void**)&g_lpSIM, (void*)&g_fOSVI, (void*)&g_fOFFICEVI) == 0)
	{
		_TRACEA(0, "[FasooDrmInitialize] DRMInterfaceInitialize failed.");

		return false;
	}

	_TRACEA(0, "[FasooDrmInitialize] DRMInterfaceInitialize success.");

	return true;
}

//////////////////////////////////////////////////////////
// Func for DIM 
//////////////////////////////////////////////////////////

unsigned long _TO_OFFICE_DRM_PFN_getVersion()
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_getVersion] Enter..");

	return 1;
}

int _TO_OFFICE_DRM_PFN_isExtractable(const char* UTF8filePath)
{
	return 0;
}

int _TO_OFFICE_DRM_PFN_isPrintable(const char* UTF8filePath)
{
	return 0;
}

int _TO_OFFICE_DRM_PFN_isEditable(const char* UTF8filePath)
{
	return 0;
}

int _TO_OFFICE_DRM_PFN_isViewable(const char* UTF8filePath)
{
	return 0;
}

int _TO_OFFICE_DRM_PFN_setNotifyMessage(void* pContentInfo, const char* UTF8FilePath, TO_OFFICE_DRM_EventIDEnum ID, void* param1, void* param2)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_setNotifyMessage] Enter.. '%s' '%d'", UTF8FilePath, ID);

	return g_lpDIM->pfnsetNotifyMessage(pContentInfo, UTF8FilePath, ID, param1, param2);
}

int _TO_OFFICE_DRM_PFN_setMenu(void* pContentInfo, const char* menuID)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_setMenu] Enter.. '%s'", menuID);

	return g_lpDIM->pfnsetMenu(pContentInfo, menuID);
}

//////////////////////////////////////////////////////////
// Func for SIM 
//////////////////////////////////////////////////////////

int _TO_OFFICE_DRM_PFN_DRMIsLicenseValidByPath(const char* UTF8FilePath, TO_OFFICE_DRM_PurposeEnum purpose)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMIsLicenseValidByPath] Enter.. '%s' '%d'", UTF8FilePath, purpose);

	return g_lpSIM->pfnADKIsLicenseVaildByPath((char*)UTF8FilePath, (int)purpose);
}

int _TO_OFFICE_DRM_PFN_DRMIsSecureContentByPath(const char* UTF8FilePath)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMIsSecureContentByPath] Enter.. '%s'", UTF8FilePath);

	return g_lpSIM->pfnADKIsSecureContentByPath((char*)UTF8FilePath);
}

void* _TO_OFFICE_DRM_PFN_DRMOpenContent(const char* UTF8FilePath, const char* fileMode)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMOpenContent] Enter.. '%s' '%s'", UTF8FilePath, fileMode);
	
	bool bWritable = false;
	bool bTruncate = false;

	if (NULL == fileMode || '\0' == fileMode[0])
	{
		bWritable = true;
		bTruncate = true;
	}
	else if (NULL != strstr(fileMode, "r") ||
		NULL != strstr(fileMode, "R"))
	{
		if (NULL != strstr(fileMode, "+"))
		{
			bWritable = true;
		}
		else
		{
			bWritable = false;
		}
	}
	else if (NULL != strstr(fileMode, "w") ||
		NULL != strstr(fileMode, "W") ||
		NULL != strstr(fileMode, "a") ||
		NULL != strstr(fileMode, "A"))
	{
		bWritable = true;
		if (NULL != strstr(fileMode, "w") ||
			NULL != strstr(fileMode, "W"))
		{
			bTruncate = true;
		}
	}

	return g_lpSIM->pfnADKOpenContent((char*)UTF8FilePath, (char*)fileMode, bWritable, bTruncate);
}

int _TO_OFFICE_DRM_PFN_DRMIsSecureContent(void* pContentInfo)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMIsSecureContent] Enter..");

	return g_lpSIM->pfnADKIsSecureContent(pContentInfo);
}

int _TO_OFFICE_DRM_PFN_DRMIsLicenseValid(void* pContentInfo, TO_OFFICE_DRM_PurposeEnum purpose)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMIsLicenseValid] Enter.. '%d'", purpose);

	return g_lpSIM->pfnADKIsLicenseValid(pContentInfo, (long)purpose);
}

long _TO_OFFICE_DRM_PFN_DRMGetContentSize(void* pContentInfo)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMGetContentSize] Enter..");
	
	return g_lpSIM->pfnADKGetContentSize(pContentInfo);
}

int _TO_OFFICE_DRM_PFN_DRMSetContentPointer(void* pContentInfo, long lOffset, int iOrigin)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMSetContentPointer] Enter..");

	return g_lpSIM->pfnADKSetContentPointer(pContentInfo, lOffset, iOrigin);
}

int _TO_OFFICE_DRM_PFN_DRMReadContent(void* pContentInfo, void* pBuffer, long lNumberOfBytesToRead, long* pNumberOfBytesRead)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMReadContent] Enter..");
	
	return g_lpSIM->pfnADKReadContent(pContentInfo, pBuffer, (unsigned long)lNumberOfBytesToRead, (unsigned long*)pNumberOfBytesRead);
}

int _TO_OFFICE_DRM_PFN_DRMWriteContent(void* pContentInfo, void* pBuffer, long lNumberOfBytesToWrite, long* pNumberOfBytesWritten)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMWriteContent] Enter..");

	return g_lpSIM->pfnADKWriteContent(pContentInfo, pBuffer, (unsigned long)lNumberOfBytesToWrite, (unsigned long*)pNumberOfBytesWritten);
}

int _TO_OFFICE_DRM_PFN_DRMSetEndOfContent(void* pContentInfo)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMSetEndOfContent] Enter..");

	return g_lpSIM->pfnADKSetEndOfContent(pContentInfo);
}

int _TO_OFFICE_DRM_PFN_DRMCloseContent(void* pContentInfo)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMCloseContent] Enter..");

	return g_lpSIM->pfnADKCloseContent(pContentInfo);
}

TO_OFFICE_DRM_ErrorEnum _TO_OFFICE_DRM_PFN_DRMGetLastError()
{
	return (TO_OFFICE_DRM_ErrorEnum)g_lpSIM->pfnADKGetLastError();
}

void _TO_OFFICE_DRM_PFN_DRMSetLastError(TO_OFFICE_DRM_ErrorEnum error)
{
	g_lpSIM->pfnADKSetLastError(error);

	return;
}

int _TO_OFFICE_DRM_PFN_DRMPackContent(void* pContentInfoTarget, void* pContentInfoTemplate)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMPackContent] Enter..");

	return g_lpSIM->pfnADKPackContent(pContentInfoTarget, pContentInfoTemplate);
}

int _TO_OFFICE_DRM_PFN_DRMAutoPackContent(const char* UTF8FilePath)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMAutoPackContent] Enter.. '%s'", UTF8FilePath);

	return g_lpSIM->pfnADKAutoPackContent((char*)UTF8FilePath);
}

int _TO_OFFICE_DRM_PFN_DRMSetUsage(void* pContentInfoTarget, TO_OFFICE_DRM_PurposeEnum purpose)
{
	_TRACEA(0, "[_TO_OFFICE_DRM_PFN_DRMSetUsage] Enter.. '%d'", purpose);

	return g_lpSIM->pfnADKSetUsage(pContentInfoTarget, (unsigned short)purpose);
}

#pragma GCC visibility push(default)
int TO_OFFICE_DRM_DRMInterfaceInitialize(TO_OFFICE_DRM_DIM **ppDocumentInterface,
	TO_OFFICE_DRM_SIM **ppSystemInterface,
	TO_OFFICE_DRM_VI* pOSInformation,
	TO_OFFICE_DRM_VI* pOfficeInformation)
{
	_TRACEA(0, "[DRMInterfaceInitialize] Enter..");

	g_lpTODIM = (TO_OFFICE_DRM_DIM *)malloc(sizeof(TO_OFFICE_DRM_DIM));
	g_lpTOSIM = (TO_OFFICE_DRM_SIM *)malloc(sizeof(TO_OFFICE_DRM_SIM));

	g_lpDIM = (LPDIM)malloc(sizeof(DIM));
	g_lpSIM = (LPSIM)malloc(sizeof(SIM));

	if (!FasooDrmInitialize())
		return 0;

	g_lpTOSIM->pfnDRMIsLicenseValidByPath = _TO_OFFICE_DRM_PFN_DRMIsLicenseValidByPath;
	g_lpTOSIM->pfnDRMIsSecureContentByPath = _TO_OFFICE_DRM_PFN_DRMIsSecureContentByPath;
	g_lpTOSIM->pfnDRMOpenContent = _TO_OFFICE_DRM_PFN_DRMOpenContent;
	g_lpTOSIM->pfnDRMIsSecureContent = _TO_OFFICE_DRM_PFN_DRMIsSecureContent;
	g_lpTOSIM->pfnDRMIsLicenseValid = _TO_OFFICE_DRM_PFN_DRMIsLicenseValid;
	g_lpTOSIM->pfnDRMGetContentSize = _TO_OFFICE_DRM_PFN_DRMGetContentSize;
	g_lpTOSIM->pfnDRMSetContentPointer = _TO_OFFICE_DRM_PFN_DRMSetContentPointer;
	g_lpTOSIM->pfnDRMReadContent = _TO_OFFICE_DRM_PFN_DRMReadContent;
	g_lpTOSIM->pfnDRMWriteContent = _TO_OFFICE_DRM_PFN_DRMWriteContent;
	g_lpTOSIM->pfnDRMSetEndOfContent = _TO_OFFICE_DRM_PFN_DRMSetEndOfContent;
	g_lpTOSIM->pfnDRMCloseContent = _TO_OFFICE_DRM_PFN_DRMCloseContent;
	g_lpTOSIM->pfnDRMGetLastError = _TO_OFFICE_DRM_PFN_DRMGetLastError;
	g_lpTOSIM->pfnDRMSetLastError = _TO_OFFICE_DRM_PFN_DRMSetLastError;
	g_lpTOSIM->pfnDRMPackContent = _TO_OFFICE_DRM_PFN_DRMPackContent;
	g_lpTOSIM->pfnDRMAutoPackContent = _TO_OFFICE_DRM_PFN_DRMAutoPackContent;
	g_lpTOSIM->pfnDRMSetUsage = _TO_OFFICE_DRM_PFN_DRMSetUsage;

	*(TO_OFFICE_DRM_SIM**)ppSystemInterface = g_lpTOSIM;

	g_lpTODIM->pfngetVersion = _TO_OFFICE_DRM_PFN_getVersion;
	g_lpTODIM->pfnisExtractable = _TO_OFFICE_DRM_PFN_isExtractable;
	g_lpTODIM->pfnisPrintable = _TO_OFFICE_DRM_PFN_isPrintable;
	g_lpTODIM->pfnisEditable = _TO_OFFICE_DRM_PFN_isEditable;
	g_lpTODIM->pfnisViewable = _TO_OFFICE_DRM_PFN_isViewable;
	g_lpTODIM->pfnsetNotifyMessage = _TO_OFFICE_DRM_PFN_setNotifyMessage;
	g_lpTODIM->pfnsetMenu = _TO_OFFICE_DRM_PFN_setMenu;

	*(TO_OFFICE_DRM_DIM**)ppDocumentInterface = g_lpTODIM;

	return 1;
}
#pragma GCC visibility pop