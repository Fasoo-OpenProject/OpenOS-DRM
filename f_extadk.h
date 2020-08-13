#include <stdio.h>

typedef unsigned char BYTE;
typedef unsigned char *LPBYTE;
typedef void *LPVOID;
typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef unsigned long (*_LPGETVERSION)();
typedef unsigned int (*_LPGETAUTHENTICATE)
(
    char* szFilePath
);
typedef unsigned long (*_LPSETNOTIFYMESSAGE)();
typedef unsigned int (*_LPSETMENU)
(
    unsigned int uiMenuID
);

typedef struct DocumentInterfaceManager
{
    _LPGETVERSION pfngetVersion;
    _LPGETAUTHENTICATE pfngetAuthenticate;
    _LPSETNOTIFYMESSAGE pfnsetNotifyMessage;
    _LPSETMENU pfnsetMenu;
} DIM;
typedef DIM *LPDIM;

typedef int (*_LPADKLISLICENSEVALIDBYPATH)
(
    char *szFilePath, int iPurpose
);

typedef int (*_LPADKISSECURECONTENTBYPATH)
(
    char *szFilePath
);

typedef int (*_LPADKGETDOMAINIDBYPATH)
(
    char *szFilePath, char *szDomainID, int iBufferSize
);

typedef void* (*_LPADKOPENCONTENT)
(
    char *szFilePath, int iWritable, int iTruncate
);

typedef int (*_LPADKISSECURECONTENT)
(
    void *pContentInfo
);

typedef int (*_LPADKISLICENSEVALID)
(
    void *pContentInfo, WORD wPurpose
);

typedef int (*_LPADKSETCONTENTPOINTER)
(
    void *pContentInfo, long IOffset, int iOrigin
);

typedef int (*_LPADKREADCONTENT)
(
    void *pContentInfo, void *pBuffer, long lNumberOfBytesToRead, long lNumberOfBytesRead
);

typedef int (*_LPADKWRITECONTENT)
(
    void *pContentInfo, void *pBuffer, long lNumberOfBytesToWrite, long lNumberOfBytesWritten
);

typedef int (*_LPADKSETENDOFCONTENT)
(
    void *pContentInfo
);

typedef int (*_LPADKCLOSECONTENT)
(
    void *pContentInfo
);

typedef int (*_LPADKGETLASTERROR)();

typedef int (*_LPADKSETLASTERROR)
(
    long lError
);

typedef int (*_LPADKGETDOMAINID)
(
    void *pContentInfo, char *szDomainID, int iBufferSize
);

typedef int (*_LPADKGETCONTENTTYPE)
(
    void *pContentInfo
);

typedef int (*_LPADKPACKCONTENT)
(
    void *pContentInfoTarget, void *pContentInfoTemplate
);

typedef int (*_LPADKSETUSAGE)
(
    void *pContentInfoTarget, WORD wPurpose                
);

typedef int (*_LPADKISLOGONSTATUS)
(
    char *szDomainID
);

typedef struct SystemInterfaceManager
{
    _LPADKLISLICENSEVALIDBYPATH pfnADKIsLicenseVaildByPath;
    _LPADKISSECURECONTENTBYPATH pfnADKIsSecureContentByPath;
    _LPADKGETDOMAINIDBYPATH pfnADKGetDomainIdByPath;
    _LPADKOPENCONTENT pfnADKOpenContent;
    _LPADKISSECURECONTENT pfnADKIsSecureContent;
    _LPADKISLICENSEVALID pfnADKIsLicenseValid;
    _LPADKSETCONTENTPOINTER pfnADKSetContentPointer;
    _LPADKREADCONTENT pfnADKReadContent;
    _LPADKWRITECONTENT pfnADKWriteContent;
    _LPADKSETENDOFCONTENT pfnADKSetEndOfContent;
    _LPADKCLOSECONTENT pfnADKCloseContent;
    _LPADKGETLASTERROR pfnADKGetLastError;
    _LPADKSETLASTERROR pfnADKSetLastError;
    _LPADKGETDOMAINID pfnADKGetDomainId;
    _LPADKGETCONTENTTYPE pfnADKGetContentType;
    _LPADKPACKCONTENT pfnADKPackContent;
    _LPADKSETUSAGE pfnADKSetUsage;
    _LPADKISLOGONSTATUS pfnADKIsLogonStatus;
} SIM;
typedef SIM *LPSIM;

typedef struct FVerInfo
{
    char szname[256];   
    unsigned long ulVersion;
} FVI;
typedef FVI* LPFVI;

int DRMInterfaceInitialize(void **ppDocumentInterface, void **ppSystemInterface, void *pOSInformation, void* pOfficeInformation);

