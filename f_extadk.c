#include <stdio.h>
#include <stdlib.h>
#include "f_extadk.h"

LPDIM g_lpDIM = NULL;
LPSIM g_lpSIM = NULL;

void __attribute__((constructor))ADK_init(void)
{
    //printf("ADK_init called!!..\n");
    g_lpDIM = (LPDIM)malloc(sizeof(DIM));
    g_lpSIM = (LPSIM)malloc(sizeof(SIM));
}

void __attribute__((destructor))ADK_fini(void)
{
    
    if (g_lpDIM)
    {
         //printf("ADK_fini called!!..#1\n");
         free(g_lpDIM);
    }
    
    if (g_lpSIM)
    { 
         //printf("ADK_fini called!!..#2\n");
         free(g_lpSIM);
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

int ADK_IsLicenseVaildByPath(char *szFilePath, int iPurpose)
{
    //Implementation
    printf(" ADK_IsLicenseVaildByPath called!!..\n");
    return 1;
}

int ADK_IsSecureContentByPath(char *szFilePath)
{
    //Implementation
    printf(" ADK_IsSecureContentByPath called!!..\n");
    return 1;
}

int ADK_GetDomainIdByPath( char *szFilePath, char *szDomainID, int iBufferSize)
{
    //Implementation
    printf(" ADK_GetDomainIdByPath called!!..\n");
    return 1;
}

void* ADK_OpenContent(char *szFilePath, int iWritable, int iTruncate)
{
    static char szBuf[128] ="Test Buf";
    //Implementation
    printf(" ADK_OpenContent called!!..\n");
    
    return (void *)szBuf;
}

int ADK_IsSecureContent(void *pContentInfo)
{
    //Implementation
    printf(" ADK_IsSecureContent called!!..\n");
    return 1;
}

int ADK_IsLicenseValid(void *pContentInfo, WORD wPurpose)
{
    //Implementation
    printf(" ADK_IsLicenseValid called!!..\n");
    return 1;
}

int ADK_SetContentPointer(void *pContentInfo, long IOffset, int iOrigin)
{
    //Implementation
    printf(" ADK_SetContentPointer called!!..\n");
    return 1;
}

int ADK_ReadContent(void *pContentInfo, void *pBuffer, long lNumberOfBytesToRead, long lNumberOfBytesRead)
{
    //Implementation
    printf(" ADK_ReadContent called!!..\n");
    return 1; 
}

int ADK_WriteContent(void *pContentInfo, void *pBuffer, long lNumberOfBytesToWrite, long lNumberOfBytesWritten)
{
    //Implementation
    printf(" ADK_WriteContent called!!..\n");
    return 1;     
}
int ADK_SetEndOfContent(void *pContentInfo)
{
    //Implementation
    printf(" ADK_SetEndOfContent called!!..\n");
    return 1;      
}

int ADK_CloseContent(void *pContentInfo)
{
    //Implementation
    printf(" ADK_CloseContent called!!..\n");
    return 1;          
}

int ADK_GetLastError()
{
    //Implementation
    printf(" ADK_GetLastError called!!..\n");
    return 1;      
}

int ADK_SetLastError(long lError)
{
    //Implementation
    printf(" ADK_SetLastError called!!..\n");
    return 1;     
}

int ADK_GetDomainId(void *pContentInfo, char *szDomainID, int iBufferSize)
{
    //Implementation
    // This function does not use in normal case.
    printf(" ADK_GetDomainId called!!..\n");
    return 1;        
}

int ADK_GetContentType(void *pContentInfo)
{
    //Implementation
    printf(" ADK_GetContentType called!!..\n");
    return 1;     
}

int ADK_PackContent(void *pContentInfoTarget, void *pContentInfoTemplate)
{
    //Implementation
    printf(" ADK_PackContent called!!..\n");
    return 1;     
}

int ADK_SetUsage(void *pContentInfoTarget, WORD wPurpose )
{
    //Implementation
    printf(" ADK_SetUsage called!!..\n");
    return 1;    
}

int ADK_IsLogonStatus(char *szDomainID)
{
    //Implementation
    printf(" ADK_IsLogonStatus called!!..\n");
    return 1;
}

#pragma GCC visibility push(default)
int DRMInterfaceInitialize(void **ppDocumentInterface, void **ppSystemInterface, void *pOSInformation, void* pOfficeInformation)
{
    printf(" DRMInterfaceInitialize start!!..\n");
    //printf(" DRMInterfaceInitialize start!!..(DIM)\n");
    
    g_lpDIM->pfngetVersion = get_Version;
    g_lpDIM->pfngetAuthenticate = get_Authenticate;
    g_lpDIM->pfnsetNotifyMessage = set_NotifyMessage;
    g_lpDIM->pfnsetMenu = set_Menu;

    *(LPDIM *)ppDocumentInterface = g_lpDIM;
    //printf(" DRMInterfaceInitialize end!!..(DIM)\n");
    
    //printf(" DRMInterfaceInitialize start!!..(SIM)\n");
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
    g_lpSIM->pfnADKSetUsage = ADK_SetUsage;
    g_lpSIM->pfnADKIsLogonStatus = ADK_IsLogonStatus;

    *(LPSIM *)ppSystemInterface = g_lpSIM;
    //printf(" DRMInterfaceInitialize end!!..(SIM)\n");
    
    printf(" DRMInterfaceInitialize end!!..\n");
    return 1;
}
#pragma GCC visibility pop
