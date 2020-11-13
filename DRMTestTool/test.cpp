#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <time.h>
#include <utime.h>
#include "DRMUtil.h"
#include "NXAPI.h"
#include <unistd.h>

#define EXTADK_PATH "/usr/lib/libofficedrmutil.so"

typedef int(*_TO_OFFICE_DRM_DRMInterfaceInitialize)(TO_OFFICE_DRM_DIM **ppDocumentInterface,
	TO_OFFICE_DRM_SIM **ppSystemInterface,
	TO_OFFICE_DRM_VI* pOSInformation,
	TO_OFFICE_DRM_VI* pOfficeInformation);

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

	_TO_OFFICE_DRM_DRMInterfaceInitialize pfnDRMInterfaceInitialize;
	if ((pfnDRMInterfaceInitialize = (_TO_OFFICE_DRM_DRMInterfaceInitialize)dlsym(dlHandle, "TO_OFFICE_DRM_DRMInterfaceInitialize")) == NULL)
	{
		printf("dlsym error '%s'\n", dlerror());
		dlclose(dlHandle);
		return -1;
	}

	TO_OFFICE_DRM_DIM* pTI = NULL;
	TO_OFFICE_DRM_SIM* pSI = NULL;
	TO_OFFICE_DRM_VI fOSVI;
	TO_OFFICE_DRM_VI fOFFICEVI;
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
	pfnDRMInterfaceInitialize((TO_OFFICE_DRM_DIM **)&pTI, (TO_OFFICE_DRM_SIM **)&pSI, (TO_OFFICE_DRM_VI *)&fOSVI, (TO_OFFICE_DRM_VI *)&fOFFICEVI);

	LPCONTENTINFO lpContentInfo = NULL;

	if (strcasecmp(argv[2], "pack") == 0)
	{
		if (pSI->pfnDRMAutoPackContent(szFilePath) > 0)
			printf("PackContent success.\n");
		else
			printf("PcakContent failed.\n");
	}
	else if (strcasecmp(argv[2], "open") == 0)
	{
		lpContentInfo = (LPCONTENTINFO)pSI->pfnDRMOpenContent(szFilePath, argv[3]);
		if (lpContentInfo != NULL)
			printf("OpenContent success.\n");
		else
			printf("OpenContent failed.\n");
	}
	else if (strcasecmp(argv[2], "issecure") == 0)
	{
		if (pSI->pfnDRMIsSecureContentByPath(szFilePath) > 0)
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
			if (pSI->pfnDRMIsLicenseValidByPath(szFilePath, kDRMPurposeView) > 0)
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
			if (pSI->pfnDRMIsLicenseValidByPath(szFilePath, kDRMPurposeEdit) > 0)
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
			if (pSI->pfnDRMIsLicenseValidByPath(szFilePath, kDRMPurposePrint) > 0)
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
			if (pSI->pfnDRMIsLicenseValidByPath(szFilePath, kDRMPurposeSave) > 0)
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
			if (pSI->pfnDRMIsLicenseValidByPath(szFilePath, kDRMPurposeExtract) > 0)
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
		lpContentInfo = (LPCONTENTINFO)pSI->pfnDRMOpenContent(szFilePath, "r");
		if (lpContentInfo != NULL)
		{
			char szBuffer[1024] = { 0 };
			long lBytesReaden;
			if (pSI->pfnDRMReadContent(lpContentInfo, &szBuffer, 50, &lBytesReaden) > 0)
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
		unsigned long lSize = 0;
		lpContentInfo = (LPCONTENTINFO)pSI->pfnDRMOpenContent(szFilePath, "r");
		if (lpContentInfo != NULL)
		{
			lSize = pSI->pfnDRMGetContentSize(lpContentInfo);
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
		if (pSI->pfnADKUnpackByContentPath(szFilePath, NULL))
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