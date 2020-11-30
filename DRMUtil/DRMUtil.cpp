#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <time.h>
#include <utime.h>
#include "f_extadk.h"
#include "NXAPI.h"
#include <unistd.h>

#define EXTADK_PATH "/usr/local/fasoo/libfasooextadk.so"

typedef int(*_LPDRMInterfaceInitialize)(void **ppDocumentInterface, void **ppSystemInterface, void *pOSInformation, void* pOfficeInformation);

LPSIM g_lpSIM = NULL;
LPDIM g_lpDIM = NULL;
FVI g_fOSVI;
FVI g_fOFFICEVI;

int main(int argc, char**argv)
{
	void *dlHandle = NULL;

	dlHandle = dlopen(EXTADK_PATH, RTLD_LAZY);
	if (!dlHandle)
	{
		fputs(dlerror(), stderr);
		printf("dlopen error '%s'\n", dlerror());
		return -1;
	}
	else
	{
		printf("dlopen success.\n");
	}

	_LPDRMInterfaceInitialize pfnDRMInterfaceInitialize = (_LPDRMInterfaceInitialize)dlsym(dlHandle, "DRMInterfaceInitialize");
	if (!pfnDRMInterfaceInitialize)
	{
		printf("dlsym error '%s'\n", dlerror());
		dlclose(dlHandle);
		return -1;
	}

	char *szFilePath;
	szFilePath = (char*)malloc(1024);

	szFilePath = argv[1];
	if (szFilePath == NULL)
	{
		printf("szFilePath is NULL.\n");
		dlclose(dlHandle);
		return -1;
	}
		
	printf("filepath '%s'\n", szFilePath);

	//Initialize..
	if (pfnDRMInterfaceInitialize((void**)&g_lpDIM, (void**)&g_lpSIM, (void*)&g_fOSVI, (void*)&g_fOFFICEVI) == 0)
	{
		printf("DRMInterfaceInitialize failed.");

		return false;
	}

	LPCONTENTINFO lpContentInfo = NULL;

	if (strcasecmp(argv[2], "pack") == 0)
	{
		if (g_lpSIM->pfnADKAutoPackContent(szFilePath) > 0)
			printf("PackContent success.\n");
		else
			printf("PcakContent failed.\n");
	}
	else if (strcasecmp(argv[2], "open") == 0)
	{
		// 아래 두 플래그는 FileMode에 따라 세팅되어야 함.
		// bWritable = 쓰기 가능. FileMode 'r+' 'w' 'a' 등이 해당.
		// bTruncate = 파일을 지우고 다시 씀. FileMode 'w'가 해당.
		bool bWritable = true;
		bool bTruncate = false;

		lpContentInfo = (LPCONTENTINFO)g_lpSIM->pfnADKOpenContent(szFilePath, argv[3], bWritable, bTruncate);
		if (lpContentInfo != NULL)
			printf("OpenContent success.\n");
		else
			printf("OpenContent failed.\n");
	}
	else if (strcasecmp(argv[2], "issecure") == 0)
	{
		if (g_lpSIM->pfnADKIsSecureContentByPath(szFilePath) > 0)
		{
			printf("'%s' is secure file.\n", szFilePath);
		}
		else
		{
			printf("'%s' is not secure file.\n", szFilePath);
		}
	}
	else if (strcasecmp(argv[2], "islicensevalid") == 0)
	{
		if (strcasecmp(argv[3], "view") == 0)
		{
			if (g_lpSIM->pfnADKIsLicenseVaildByPath(szFilePath, kDRMPurposeView) > 0)
			{
				printf("'%s' VIEW license valid.\n", szFilePath);
			}
			else
			{
				printf("'%s' VIEW license invalid.\n", szFilePath);
			}

		}
		else if (strcasecmp(argv[3], "edit") == 0)
		{
			if (g_lpSIM->pfnADKIsLicenseVaildByPath(szFilePath, kDRMPurposeEdit) > 0)
			{
				printf("'%s' EDIT license valid.\n", szFilePath);
			}
			else
			{
				printf("'%s' EDIT license invalid.\n", szFilePath);
			}
		}
		else if (strcasecmp(argv[3], "print") == 0)
		{
			if (g_lpSIM->pfnADKIsLicenseVaildByPath(szFilePath, kDRMPurposePrint) > 0)
			{
				printf("'%s' PRINT license valid.\n", szFilePath);
			}
			else
			{
				printf("'%s' PRINT license invalid.\n", szFilePath);
			}
		}
		else if (strcasecmp(argv[3], "save") == 0)
		{
			if (g_lpSIM->pfnADKIsLicenseVaildByPath(szFilePath, kDRMPurposeSave) > 0)
			{
				printf("'%s' SAVE license valid.\n", szFilePath);
			}
			else
			{
				printf("'%s' SAVE license invalid.\n", szFilePath);
			}
		}
		else if (strcasecmp(argv[3], "extract") == 0)
		{
			if (g_lpSIM->pfnADKIsLicenseVaildByPath(szFilePath, kDRMPurposeExtract) > 0)
			{
				printf("'%s' EXTRACT license valid.\n", szFilePath);
			}
			else
			{
				printf("'%s' EXTRACT license invalid.\n", szFilePath);
			}
		}
	}
	else if (strcasecmp(argv[2], "read") == 0)
	{
		const char *FileMode = "r";
		lpContentInfo = (LPCONTENTINFO)g_lpSIM->pfnADKOpenContent(szFilePath, (char*)FileMode, false, false);
		if (lpContentInfo != NULL)
		{
			char szBuffer[1024] = { 0 };
			unsigned long lBytesReaden;
			if (g_lpSIM->pfnADKReadContent(lpContentInfo, &szBuffer, 50, &lBytesReaden) > 0)
			{
				printf("ReadContent success. '%s'", szBuffer);
			}
			else
			{
				printf("ReadContent failed.\n");
			}
		}
	}
	else if (strcasecmp(argv[2], "size") == 0)
	{
		const char *FileMode = "r";
		unsigned long lSize = 0;
		lpContentInfo = (LPCONTENTINFO)g_lpSIM->pfnADKOpenContent(szFilePath, (char*)FileMode, false, false);
		if (lpContentInfo != NULL)
		{
			lSize = g_lpSIM->pfnADKGetContentSize(lpContentInfo);
			if (lSize > 0)
			{
				printf("GetContentSize success. '%ld'\n", lSize);
			}
			else
			{
				printf("GetContentSize failed.\n");
			}
		}
	}
	else if (strcasecmp(argv[2], "unpack") == 0)
	{
		if (g_lpSIM->pfnADKUnpackByContentPath(szFilePath, NULL))
		{
			printf("UnpackbyContentInfo success.\n");
		}
		else
		{
			printf("UnpackbyContentInfo failed.\n");
		}
	}

	dlclose(dlHandle);
	return 0;
}