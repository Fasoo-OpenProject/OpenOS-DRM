#include <stdio.h>
#include <strings.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#include <cstring>
#include <stdarg.h>
#include <iconv.h>
#include <vector>
#include <locale>
#include <algorithm>
#include "f_extadk.h"
#include "NXAPI.h"
#include "ContentInfo.h"
#include "Trace.h"

LPDIM g_lpDIM;
LPSIM g_lpSIM;

int ADK_SetLastError(DWORD dwError);
NXLINTERFACE *g_pNxlInterface;

#define NXLPATH		"/usr/local/fasoo/libf_nxl.so"
#define CP_ACP		0
#define CP_UTF8		65001

typedef PNXLINTERFACE(*_LPNXINITIALIZE)(void);

inline bool ChangeCharset(char *szSrcCharset, char *szDstCharset, char *szSrc, int nSrcLength, char *szDst, int* pnDstLength)
{
	iconv_t it = iconv_open(szDstCharset, szSrcCharset);
	if (it == (iconv_t)(-1))
	{
		return false;
	}
	bool result = true;
	size_t nSrcStrLen = nSrcLength;
	size_t cc = iconv(it, &szSrc, &nSrcStrLen, &szDst, (size_t*)pnDstLength);
	if (cc == (size_t)(-1))
	{
		result = false;
	}

	if (iconv_close(it) == -1)
		result = false;
	return result;
};

inline std::wstring mbs_to_wcs(std::string const& str,
	std::locale const& loc = std::locale())
{
	std::wstring ret = L"";
	unsigned long dwLen = str.length();
	if (dwLen < 1)
		return ret;

	char* pcBuf = (char*)malloc((dwLen + 1) * sizeof(char));
	strcpy(pcBuf, str.c_str());

	char* pcOutBuf = (char*)malloc((dwLen + 1) * sizeof(wchar_t));
	int iOut = (dwLen + 1) * sizeof(wchar_t);

	char szSrcCharset[16] = "EUC-KR";
	char szDstCharset[16] = "UTF-32LE";

	if (ChangeCharset(szSrcCharset, szDstCharset, pcBuf, strlen(pcBuf), pcOutBuf, &iOut))
	{
		ret = (wchar_t*)pcOutBuf;
	}

	if (pcBuf != nullptr)
		free(pcBuf);
	if (pcOutBuf != nullptr)
		free(pcOutBuf);

	return ret;
};

unsigned short PurposeMapping(unsigned short wTmaxPurpose)
{
	unsigned short wADKPurpose;
	switch (wTmaxPurpose)
	{
	case kDRMPurposeView:
		wADKPurpose = ADK_PURPOSE_VIEW;
		break;
	case kDRMPurposeSave:
		wADKPurpose = ADK_PURPOSE_SAVE;
		break;
	case kDRMPurposeEdit:
		wADKPurpose = ADK_PURPOSE_EDIT;
		break;
	case kDRMPurposePrint:
		wADKPurpose = ADK_PURPOSE_PRINT;
		break;
	case kDRMPurposePrintScreen:
		wADKPurpose = ADK_PURPOSE_PRINT_SCREEN;
		break;
	case kDRMPurposeExtract:
		wADKPurpose = ADK_PURPOSE_EXTRACT;
		break;
	case kDRMPurposeTransfer:
		wADKPurpose = ADK_PURPOSE_TRANSFER;
		break;
	case kDRMPurposeSecureSave:
		wADKPurpose = ADK_PURPOSE_SECURE_SAVE;
		break;
	case kDRMPurposeSecurePrint:
		wADKPurpose = ADK_PURPOSE_SECURE_PRINT;
		break;
	case kDRMPurposeMacro:
		wADKPurpose = ADK_PURPOSE_MACRO;
		break;
	}

	return wADKPurpose;
}

void __attribute__((constructor))ADK_init(void)
{
}

void __attribute__((destructor))ADK_fini(void)
{
}

unsigned long get_Version()
{
	return 1;
}

unsigned int get_Authenticate(char *szFilePath)
{
	return 1;
}

unsigned long set_NotifyMessage(void* pContentInfo, const char* UTF8FilePath, TO_OFFICE_DRM_EventIDEnum ID, void* param1, void* param2)
{
	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	_FTRACEW(2, L"[set_NotifyMessage] Enter.. '%s' '%d'", lpContentInfo->pwcsFilePath, ID);

	unsigned long dwPurpose = 0;
	switch (ID)
	{
	case kDRMEventIDDocumentAfterOpen:
		{
			dwPurpose = ADK_PURPOSE_VIEW;
			break;
		}
	case kDRMEventIDDocumentAfterSave:
		{
			dwPurpose = ADK_PURPOSE_SECURE_SAVE;
			break;
		}
	}

	_FTRACEW(2, L"[set_NotifyMessage] purpose '%ld'", dwPurpose);

	if (0 != dwPurpose)
		g_pNxlInterface->SetUsageEx(lpContentInfo, dwPurpose, TRUE, NULL, NULL, 0);

	return 1;
}

unsigned int set_Menu(void* pContentInfo, const char* szMenuID)
{	
	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;
	
	_FTRACEW(0, L"[set_Menu] called!!.. '%ls' '%s'", lpContentInfo->pwcsFilePath, szMenuID);
	
	if (!g_pNxlInterface->IsSecure(lpContentInfo))
	{
		_FTRACEA(2, "[_TO_OFFICE_DRM_PFN_setMenu] Is not secure file.");

		return 1;
	}

	if (strcasecmp(szMenuID, "NEW_FILE") == 0 ||
		strcasecmp(szMenuID, "OPEN_FILE") == 0 ||
		strcasecmp(szMenuID, "CURRENT_FILE") == 0)
	{
		_FTRACEA(2, "[_TO_OFFICE_DRM_PFN_setMenu] Menus related to File View.");

		return 1;
	}
	else if (strcasecmp(szMenuID, "SAVE_FILE") == 0 ||
		strcasecmp(szMenuID, "SAVE_FILE_AS") == 0 ||
		strcasecmp(szMenuID, "TH_SAVE_FILE") == 0 ||
		strcasecmp(szMenuID, "TH_SAVE_FILE_AS") == 0 ||
		strcasecmp(szMenuID, "TH_SAVE_FILE_AS_PDF") == 0 ||
		strcasecmp(szMenuID, "TH_SAVE_INSERTED_PICTURE") == 0 ||
		strcasecmp(szMenuID, "TH_DIALOG_SAVE_INSERTED_PICTURE") == 0)
	{
		_FTRACEA(2, "[_TO_OFFICE_DRM_PFN_setMenu] SECURE_SAVE invalid.");
		
		if (!g_pNxlInterface->IsLicenseValid(lpContentInfo, ADK_PURPOSE_SECURE_SAVE))
		{
			return 0;
		}
	}
	else if (strcasecmp(szMenuID, "MACRO_DIALOG_VBA_ITEM") == 0 ||
		strcasecmp(szMenuID, "MACRO_DIALOG_JS_ITEM") == 0 ||
		strcasecmp(szMenuID, "VBA_MACRO") == 0 ||
		strcasecmp(szMenuID, "VBA_MACRO_VIEW") == 0 ||
		strcasecmp(szMenuID, "VBA_MACRO_RUN") == 0)
	{
		if (!g_pNxlInterface->IsLicenseValid(lpContentInfo, ADK_PURPOSE_MACRO))
		{
			_FTRACEA(2, "[_TO_OFFICE_DRM_PFN_setMenu] MACRO invalid.");

			return 0;
		}
	}
	else if (strcasecmp(szMenuID, "CONTEXTUAL_TAB_TH_PRINT_PREVIEW") == 0 ||
		strcasecmp(szMenuID, "TM_MAIL_PRINT") == 0 ||
		strcasecmp(szMenuID, "GROUP_PRINT_TH") == 0 ||
		strcasecmp(szMenuID, "GROUP_PAGE_PRINT_TH") == 0 ||
		strcasecmp(szMenuID, "GROUP_PREVIEW_PAGE_PRINT_TH") == 0 ||
		strcasecmp(szMenuID, "PRINT") == 0 ||
		strcasecmp(szMenuID, "PRINT_TO_DOCUMENT_ADD_BUTTON") == 0 ||
		strcasecmp(szMenuID, "TW_DOCUMENT_PRINT") == 0 ||
		strcasecmp(szMenuID, "TH_PRINT") == 0 ||
		strcasecmp(szMenuID, "TH_PRINT_IN_PREVIEW") == 0)
	{
		if (!g_pNxlInterface->IsLicenseValid(lpContentInfo, ADK_PURPOSE_MACRO))
		{
			_FTRACEA(2, "[_TO_OFFICE_DRM_PFN_setMenu] PRINT invalid.");

			return 0;
		}
	}
	else
	{
		if (!g_pNxlInterface->IsLicenseValid(lpContentInfo, ADK_PURPOSE_EDIT))
		{
			_FTRACEA(2, "[_TO_OFFICE_DRM_PFN_setMenu] EDIT invalid.");

			return 0;
		}
	}

	return 1;
}

bool ADK_NxInitialize()
{
	_FTRACEA(2, "[ADK_NxInitialize] Enter..");
	
	static bool bNxlInitialized = false;
	
	if (!bNxlInitialized)
	{
		void* hDll = NULL;
		
		// https://www.joinc.co.kr/w/Site/C++/Documents/Dynamic_Class_Loading

		hDll = dlopen(NXLPATH, RTLD_LAZY);
		if (!hDll)
		{
			_FTRACEA(2, "[ADK_NxInitialize] dlopen failed. '%s'", dlerror());

			return false;
		}

		_LPNXINITIALIZE pfnNxInitialize = (_LPNXINITIALIZE)dlsym(hDll, "NxInitialize");
		if (!pfnNxInitialize)
		{
			_FTRACEA(2, "[ADK_NxInitialize] dlsym failed. '%s'", dlerror());

			return false;
		}
		
		g_pNxlInterface = pfnNxInitialize();

		bNxlInitialized = true;
	}

	return true;
}

int ADK_IsLicenseVaildByPath(char *szFilePath, unsigned short wPurpose)
{
	unsigned short wPurposeNew = PurposeMapping(wPurpose);

	_FTRACEA(2, "[ADK_IsLicenseVaildByPath] Enter.. '%s' '%d' -> '%d'", szFilePath, wPurpose, wPurposeNew);

	if (NULL == szFilePath || '\0' == szFilePath[0])
	{
		_FTRACEA(2, "[ADK_IsLicenseVaildByPath] Invalid param.");

		ADK_SetLastError(kDRMErrorInvalidArgs);

		return 0;
	}

	if (!ADK_NxInitialize())
	{
		_FTRACEA(2, "[ADK_IsLicenseVaildByPath] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);	
	
		return 0;
	}
	
	FILE* fp = fopen(szFilePath, "r");
	if (NULL == fp)
	{
		_FTRACEA(2, "[ADK_IsLicenseVaildByPath] File open failed. '%s'", strerror(errno));

		ADK_SetLastError(kDRMErrorCreateFileFail);

		return 0;
	}

	std::wstring wstrFilePath = mbs_to_wcs(szFilePath);
	wchar_t* wcsFilePath = (wchar_t*)malloc((wstrFilePath.length() + 1) * sizeof(wchar_t));
	wcscpy(wcsFilePath, wstrFilePath.c_str());

	LPCONTENTINFO lpContentInfo = g_pNxlInterface->OpenContentByHandle(fp, wcsFilePath, TRUE, FALSE, FALSE);
	if (NULL == lpContentInfo)
	{
		_FTRACEA(2, "[ADK_IsLicenseVaildByPath] OpenContentByHandle failed.");

		ADK_SetLastError(kDRMErrorContentOpenFail);
	
		fclose(fp);
		free(wcsFilePath);

		return 0;
	}
	free(wcsFilePath);

	if (!g_pNxlInterface->IsSecure(lpContentInfo))
	{
		_FTRACEA(2, "[ADK_IsLicenseVaildByPath] Not secure file.");

		g_pNxlInterface->CloseContentInfo(lpContentInfo);

		ADK_SetLastError(kDRMErrorNotSecureFile);

		fclose(fp);

		return 1;
	}

	bool bRet = g_pNxlInterface->IsLicenseValid(lpContentInfo, wPurposeNew);
	if (!bRet)
	{
		_FTRACEA(2, "[ADK_IsLicenseVaildByPath] License invalid.");

		ADK_SetLastError(kDRMErrorContentInvalidLicense);

		fclose(fp);

		return 0;
	}

	_FTRACEA(2, "[ADK_IsLicenseVaildByPath] License valid.");
	
	ADK_SetLastError(kDRMErrorOK);

 	fclose(fp);

	return 1;
}

int ADK_IsSecureContentByPath(char *szFilePath)
{
	_FTRACEA(2, "[ADK_IsSecureContentByPath] Enter.. '%s'", szFilePath);

	if (NULL == szFilePath || '\0' == szFilePath[0])
	{
		_FTRACEA(2, "[ADK_IsSecureContentByPath] Invalid param.");

		ADK_SetLastError(kDRMErrorInvalidArgs);

		return 0;
	}

	if (!ADK_NxInitialize())
	{
		_FTRACEA(2, "[ADK_IsSecureContentByPath] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	FILE* fp = fopen(szFilePath, "r");
	if (NULL == fp)
	{
		_FTRACEA(2, "[ADK_IsSecureContentByPath] File open failed. '%s'", strerror(errno));

		ADK_SetLastError(kDRMErrorCreateFileFail);

		return 0;
	}
	
	if (!g_pNxlInterface->IsSecureFileByHandle(fp))
	{
		_FTRACEA(2, "[ADK_IsSecureContentByPath] Not secure file.");

		ADK_SetLastError(kDRMErrorOK);

		fclose(fp);

		return 0;
	}

	_FTRACEA(2, "[ExtADK][ADK_IsSecureContentByPath] Secure file.");
	
	ADK_SetLastError(kDRMErrorOK);

	fclose(fp);

	return 1;
}

int ADK_GetDomainIdByPath(char *szFilePath, char *szDomainID, int iBufferSize)
{
	return 1;
}

void* ADK_OpenContent(char *szFilePath, char *szFileMode, bool bWritable, bool bTruncate)
{
	_FTRACEA(2, "[ADK_OpenContent] Enter.. '%s' '%s' '%d' '%d'", szFilePath, szFileMode, bWritable, bTruncate);

	if (NULL == szFilePath || '\0' == szFilePath[0])
	{
		_FTRACEA(2, "[ADK_OpenContent] Invalid param.");

		ADK_SetLastError(kDRMErrorInvalidArgs);

		return NULL;
	}

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_OpenContent] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return NULL;
	}

	std::wstring wstrFilePath = mbs_to_wcs(szFilePath);
	wchar_t* wcsFilePath = (wchar_t*)malloc((wstrFilePath.length() + 1) * sizeof(wchar_t));
	wcscpy(wcsFilePath, wstrFilePath.c_str());

	LPCONTENTINFO lpContentInfoOld = NULL;
	FILE* fpOld = NULL;
	if (bTruncate)
	{	
		_FTRACEA(2, "[ADK_OpenContent] Truncate file. call OpenCotentByHandle.");
		fpOld = fopen(szFilePath, "r+");
		if (NULL != fpOld)
		{
			lpContentInfoOld = g_pNxlInterface->OpenContentByHandle(fpOld, wcsFilePath, true, false, false);
			fclose(fpOld);
		}
	}

	char szFileModeTemp[10] = { 0 };
	strcpy(szFileModeTemp, szFileMode);

	if (strstr(szFileModeTemp, "w") != NULL ||
		strstr(szFileModeTemp, "W") != NULL)
	{
		if (strstr(szFileModeTemp, "+") == NULL)
		{
			strcat(szFileModeTemp, "+");
		}
	}

	FILE* fp;
	fp = fopen(szFilePath, szFileModeTemp);
	if (NULL == fp)
	{
		_FTRACEA(2, "[ADK_OpenContent] File open failed. '%s'", strerror(errno));

		ADK_SetLastError(kDRMErrorCreateFileFail);

		free(wcsFilePath);

		return 0;
	}

	if (g_pNxlInterface->IsSecure(lpContentInfoOld))
	{
		_FTRACEA(2, "[ADK_OpenContent] Truncate file. call PackByHandleTemplate.");

		g_pNxlInterface->PackByHandleTemplate(fp, wcsFilePath, lpContentInfoOld);

		g_pNxlInterface->CloseContentInfo(lpContentInfoOld);
	}

	LPCONTENTINFO lpContentInfo = g_pNxlInterface->OpenContentByHandle(fp, wcsFilePath, true, false, false);
	if (NULL == lpContentInfo)
	{
		_FTRACEA(2, "[ADK_OpenContent] OpenContentByHandle failed.");

		ADK_SetLastError(kDRMErrorContentOpenFail);

		fclose(fp);
		free(wcsFilePath);

		return NULL;
	}
	free(wcsFilePath);

	if (!g_pNxlInterface->IsLicenseValid(lpContentInfo, ADK_PURPOSE_VIEW))
	{
		_FTRACEA(2, "[ADK_OpenContent] VIEW license invalid.");

		ADK_SetLastError(kDRMErrorContentInvalidLicense);

		fclose(fp);

		return NULL;
	}

	if ((bWritable || bTruncate) && !g_pNxlInterface->IsLicenseValid(lpContentInfo, ADK_PURPOSE_SECURE_SAVE))
	{
		_FTRACEA(2, "[ADK_OpenContent] SECURE_SAVE license invalid.");

		ADK_SetLastError(kDRMErrorContentInvalidLicense);

		fclose(fp);

		return NULL;
	}

	_FTRACEA(2, "[ADK_OpenContent] OpenContent success.");

	return (void *)lpContentInfo;
}

int ADK_IsSecureContent(void *pContentInfo)
{
	_FTRACEA(2, "[ADK_IsSecureContent] Enter..");

	if (NULL == pContentInfo)
	{
		_FTRACEA(2, "[ADK_IsSecureContent] Invalid param.");

		ADK_SetLastError(kDRMErrorInvalidArgs);

		return 0;
	}

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;
	
	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_IsSecureContent] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	bool bSecure = g_pNxlInterface->IsSecure(lpContentInfo);
	if (!bSecure)
	{
		_FTRACEA(2, "[ADK_IsSecureContent] Not secure content.");

		ADK_SetLastError(kDRMErrorNotSecureFile);

		return 0;
	}

	_FTRACEA(2, "[ADK_IsSecureContent] Secure content.");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

int ADK_IsLicenseValid(void *pContentInfo, unsigned short wPurpose)
{
	unsigned short wPurposeNew = PurposeMapping(wPurpose);

	_FTRACEA(2, "[ADK_IsLicenseValid] Enter.. '%d' -> '%d'", wPurpose, wPurposeNew);
	
	if (NULL == pContentInfo)
	{
		_FTRACEA(2, "[ADK_IsLicenseValid] Invalid param.");

		ADK_SetLastError(kDRMErrorInvalidArgs);

		return 0;
	}

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_IsLicenseValid] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	bool bRet = g_pNxlInterface->IsLicenseValid(lpContentInfo, wPurposeNew);
	if (!bRet)
	{
		_FTRACEA(2, "[ADK_IsLicenseValid] License invalid.");

		ADK_SetLastError(kDRMErrorContentInvalidLicense);

		return 0;
	}

	_FTRACEA(2, "[ADK_IsLicenseValid] License valid.");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

int ADK_SetContentPointer(void *pContentInfo, long lOffset, int iOrigin)
{
	_FTRACEA(2, "[ADK_SetContentPointer] SetContentPointer Enter.. '%ld' '%d'", lOffset, iOrigin);

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_SetContentPointer] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	int ibRet = g_pNxlInterface->SetContentPointer(lpContentInfo, lOffset, NULL, iOrigin);
	if (ibRet == -1) // INVALID_SET_CONTENTPOINTER
	{
		_FTRACEA(2, "[ADK_SetContentPointer] SetContentPointer failed.");

		ADK_SetLastError(kDRMErrorContentSetPointerFail);

		return 0;
	}

	_FTRACEA(2, "[ADK_SetContentPointer] SetContentPointer success.");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

int ADK_ReadContent(void *pContentInfo, void* pBuffer, unsigned long dwNumberOfBytesToRead, unsigned long* lpdwNumberOfBytesToRead)
{
	_FTRACEA(2, "[ADK_ReadContent] Enter.. '%ld'", dwNumberOfBytesToRead);

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;
		
	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_ReadContent] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	bool bRet = g_pNxlInterface->ReadContent(lpContentInfo, pBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesToRead);
	if (!bRet)
	{
		_FTRACEA(2, "[ADK_ReadContent] ReadContent failed.");

		ADK_SetLastError(kDRMErrorContentReadFail);

		return 0;
	}

	_FTRACEA(2, "[ADK_ReadContent] ReadContent success");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

int ADK_WriteContent(void *pContentInfo, void* pBuffer, unsigned long dwNumberOfBytesToWrite, unsigned long* lpdwNumberOfBytesToWrite)
{
	_FTRACEA(2, "[ADK_WriteContent] Enter.. '%ld'", dwNumberOfBytesToWrite);

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_WriteContent] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	bool bRet = g_pNxlInterface->WriteContent(lpContentInfo, pBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesToWrite);
	if (!bRet)
	{
		_FTRACEA(2, "[ADK_WriteContent] WriteContent failed.");

		ADK_SetLastError(kDRMErrorContentWriteFail);

		return 0;
	}

	_FTRACEA(2, "[ADK_WriteContent] WriteContent success.");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

int ADK_SetEndOfContent(void *pContentInfo)
{
	_FTRACEA(2, "[ADK_SetEndOfContent] Enter..");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_SetEndOfContent] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	bool bRet = g_pNxlInterface->SetEndOfContent(lpContentInfo);
	if (!bRet)
	{
		_FTRACEA(2, "[ADK_SetEndOfContent] SetEndOfContent failed.");

		ADK_SetLastError(kDRMErrorContentSetPointerFail);

		return 0;
	}

	_FTRACEA(2, "[ADK_SetEndOfContent] SetEndOfContent success.");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

int ADK_CloseContent(void *pContentInfo)
{
	_FTRACEA(2, "[ADK_CloseContent] Enter..");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_CloseContent] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	_FTRACEA(2, "[ADK_CloseContent] hFile '0x%x'", lpContentInfo->hFile);

	FILE* fp = lpContentInfo->hFile;
	g_pNxlInterface->CloseContentInfo(lpContentInfo);

	if (NULL != fp)
	{
		fclose(fp);
	}

	_FTRACEA(2, "[ADK_CloseContent] CloseContentInfo success.");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

DWORD ADK_GetLastError()
{
	return LASTERROR;
}

int ADK_SetLastError(unsigned long dwError)
{
	LASTERROR = dwError;

	return 1;
}

DWORD ADK_GetDomainId(void *pContentInfo, char *szDomainID, unsigned int cchDomainId)
{
	_FTRACEA(2, "[ADK_GetDomainId] Enter.. '%s, '%d'", szDomainID, cchDomainId);

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_GetDomainId] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	if (!g_pNxlInterface->IsSecure(lpContentInfo))
	{
		_FTRACEA(2, "[ADK_GetDomainId] Not secure content.");

		ADK_SetLastError(kDRMErrorInvalidArgs);

		return 0;
	}

	if (NULL == lpContentInfo->lpContentLicense)
	{
		_FTRACEA(2, "[ADK_GetDomainId] Can't get domainID. lpContentLicense is NULL.");

		ADK_SetLastError(kDRMErrorGeneral);

		return 0;
	}

	const wchar_t* pwcsDomainIdFromContent = lpContentInfo->lpContentLicense->GetCPID();

	if (NULL == pwcsDomainIdFromContent)
	{
		_FTRACEA(2, "[ADK_GetDomainId] Can't get domainID. GetCPID failed.");

		ADK_SetLastError(E_ADK_INTERNAL_NXL_GET_DOMAINID_FAIL);

		return 0;
	}

	unsigned long dwLength;

	_FTRACEA(2, "[ADK_GetDomainId] GetDomainId success.");

	ADK_SetLastError(kDRMErrorOK);

	return dwLength;
}

// FSN만 지원하므로 GetContentType은 오로지 FSN만 리턴
int ADK_GetContentType(void *pContentInfo)
{
	_FTRACEA(2, "[ADK_GetContentType] Enter.."); 

	return 3;	// ADK_CONTENT_FSN
}

int ADK_PackContent(void *pContentInfoTarget, void *pContentInfoTemplate)
{
	_FTRACEA(2, "[ADK_PackContent] Enter..");

	LPCONTENTINFO lpContentInfoTarget = (LPCONTENTINFO)pContentInfoTarget;
	LPCONTENTINFO lpContentInfoTemplate = (LPCONTENTINFO)pContentInfoTemplate;
	
	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_PackContent] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	if (!g_pNxlInterface->IsSecure(lpContentInfoTemplate))
	{
		_FTRACEA(2, "[ADK_PackContent] Not secure template.");

		ADK_SetLastError(kDRMErrorNotSecureFile);

		return 0;
	}

	if (!g_pNxlInterface->IsLicenseValid(lpContentInfoTemplate, ADK_PURPOSE_VIEW) ||
		!g_pNxlInterface->IsLicenseValid(lpContentInfoTemplate, ADK_PURPOSE_SECURE_SAVE))
	{
		_FTRACEA(2, "[ADK_PackContent] License invalid.");

		ADK_SetLastError(kDRMErrorContentInvalidLicense);

		return 0;
	}

	if (!g_pNxlInterface->PackByHandleTemplate(lpContentInfoTarget->hFile, lpContentInfoTarget->pwcsFilePath, lpContentInfoTemplate))
	{
		_FTRACEA(2, "[ADK_PackContent] PackByHandleTemplate failed.");

		ADK_SetLastError(E_ADK_CONTENT_PACK_FAIL);

		return 0;
	}

	_FTRACEA(2, "[ADK_PackContent] PackByHandleTemplate success.");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

int ADK_AutoPackContent(char *szFilePath)
{
	_FTRACEA(2, "[ADK_AutoPackContent] Enter.. '%s'", szFilePath);

	if (NULL == szFilePath || '\0' == szFilePath[0])
	{
		_FTRACEA(2, "[ADK_AutoPackContent] Invalid param.");

		ADK_SetLastError(kDRMErrorInvalidArgs);

		return 0;
	}

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_AutoPackContent] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	FILE* fp = fopen(szFilePath, "r+");
	if (NULL == fp)
	{
		_FTRACEA(2, "[ADK_AutoPackContent] File open failed. '%s'", strerror(errno));

		ADK_SetLastError(kDRMErrorCreateFileFail);

		return 0;
	}

	std::wstring wstrFilePath = mbs_to_wcs(szFilePath);
	wchar_t* wcsFilePath = (wchar_t*)malloc((wstrFilePath.length() + 1) * sizeof(wchar_t));
	wcscpy(wcsFilePath, wstrFilePath.c_str());
	
	if (!g_pNxlInterface->PackByHandle(fp, wcsFilePath, NULL))
	{
		_FTRACEA(2, "[ADK_AutoPackContent] PackByHandle failed.");

		ADK_SetLastError(E_ADK_CONTENT_PACK_FAIL);

		fclose(fp);

		free(wcsFilePath);

		return 0;
	}

	_FTRACEA(2, "[ADK_AutoPackContent] PackByHandle success.");

	ADK_SetLastError(kDRMErrorOK);

	fclose(fp);

	free(wcsFilePath);

	return 1;
}

int ADK_SetUsage(void *pContentInfoTarget, unsigned short wPurpose)
{
	_FTRACEA(2, "[ADK_SetUsage] Enter..");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfoTarget;

	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_SetUsage] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	bool bRet = g_pNxlInterface->SetUsageEx(lpContentInfo, wPurpose, TRUE, NULL, NULL, 0);
	if (!bRet)
	{
		_FTRACEA(2, "[ADK_SetUsage] SetUsage failed.");

		ADK_SetLastError(kDRMErrorGeneral);

		return 0;
	}

	_FTRACEA(2, "[ADK_SetUsage] SetUsage success.");

	ADK_SetLastError(kDRMErrorOK);

	return 1;
}

int ADK_IsLogonStatus(char *szDomainID)
{
	printf("[ADK_IsLogonStatus] Enter..\n");

	if (ADK_NxInitialize() == 0)
	{
		printf("[ADK_IsLogonStatus] NxInitialize failed.\n");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	if (NULL == szDomainID || '\0' == szDomainID[0])
	{
// 		bool bLogin = g_pNxlInterface->GetLoginStatus();
// 
// 		return bLogin;
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

		ADK_SetLastError(kDRMErrorOK);

		return 1;
	}
	
	printf("[ADK_IsLogonStatus] Not logon status.\n");

	ADK_SetLastError(kDRMErrorGeneral);

	return 0;
}

unsigned long ADK_GetContentSize(void *pContentInfo)
{
	_FTRACEA(2, "[ADK_GetContentSize] Enter..");

	LPCONTENTINFO lpContentInfo = (LPCONTENTINFO)pContentInfo;
	
	if (ADK_NxInitialize() == 0)
	{
		_FTRACEA(2, "[ADK_GetContentSize] NxInitialize failed.");

		ADK_SetLastError(kDRMErrorModuleInitFail);

		return 0;
	}

	unsigned long dwContentSize;
	dwContentSize = g_pNxlInterface->GetOriginalContentSize(lpContentInfo, NULL);

	_FTRACEA(2, "[ADK_GetContentSize] size '%lu'", dwContentSize);

	return dwContentSize;
}

#pragma GCC visibility push(default)
int DRMInterfaceInitialize(void **ppDocumentInterface, void **ppSystemInterface, void *pOSInformation, void* pOfficeInformation)
{
	_FTRACEA(2, "[DRMInterfaceInitialize] Enter..");

	g_lpDIM = (LPDIM)malloc(sizeof(DIM));
	g_lpSIM = (LPSIM)malloc(sizeof(SIM));

	g_pNxlInterface = (PNXLINTERFACE)malloc(sizeof(NXLINTERFACE));

	if (!_FTraceInit("f_extadk", NULL))
		printf("[ExtADK] TraceInit failed.\n");

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