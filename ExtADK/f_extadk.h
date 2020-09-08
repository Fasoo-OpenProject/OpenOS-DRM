#include <stdio.h>
#include <strings.h>
#include <pthread.h>

///////////////// TYPEDEF DATA TYPE & STRUCT
typedef unsigned char					BYTE;
typedef unsigned char					*LPBYTE;
typedef void							*LPVOID;
typedef unsigned short					WORD;
typedef unsigned long					DWORD;
typedef DWORD							*LPDWORD;
typedef unsigned int					UINT;

__thread DWORD g_dwTlsError __attribute__((tls_model("initial-exec")));
DWORD *__tlsError_location(void)
{
	return &g_dwTlsError;
}
#define LASTERROR (*__tlsError_location())

///////////////// ENUM
enum ADK_PURPOSE
{
	ADK_PURPOSE_VIEW					= 2,
	ADK_PURPOSE_SAVE					= 3,
	ADK_PURPOSE_EDIT					= 4,
	ADK_PURPOSE_EXTRACT					= 5,
	ADK_PURPOSE_PRINT					= 6,
	ADK_PURPOSE_PRINT_SCREEN			= 8,
	ADK_PURPOSE_TRANSFER				= 10,
	ADK_PURPOSE_SECURE_SAVE				= 12,
	ADK_PURPOSE_SECURE_PRINT			= 13,
};

enum ADK_ERROR
{
	E_ADK_OK							= 0x0,
	E_ADK_LOAD_NXL_FAIL					= 0x1,
	E_ADK_NXL_INIT_FAIL					= 0x2,
	E_ADK_WIN32_CREATEFILE_FAIL			= 0x3,
	E_ADK_INVALID_ARGS					= 0x4,
	E_ADK_ALREADY_PRINTING				= 0x5,
	E_ADK_PRINTING_NOT_STARTED			= 0x6,
	E_ADK_MEMORY_ERROR					= 0x7,
	E_ADK_INSUFFICIENT_BUFFER			= 0x8,
	E_ADK_CONVERSION_FAIL				= 0x9,

	E_ADK_DRM_CLIENT_NOT_FOUND			= 0xD,

	E_ADK_NOT_SECURE_CONTENT			= 0xFF,

	E_ADK_CERT_START					= 0x100,
	E_ADK_CERT_NOT_ALLOWED_CONTENT		= 0x101,	// 사용하려는 파일이 fac 파일로부터 인증되지 않음.

	E_ADK_FAC_LOAD						= 0x200,
	E_ADK_FAC_LOAD_FAIL					= 0x201,	// 현재 모듈을 인증하는 fac 파일이 없다.

	E_ADK_CONTENT						= 0x300,
	E_ADK_CONTENT_OPEN_FAIL				= 0x301,
	E_ADK_CONTENT_READ_FAIL				= 0x302,
	E_ADK_CONTENT_WRITE_FAIL			= 0x303,
	E_ADK_CONTENT_INVALID_LICENSE		= 0x304,
	E_ADK_CONTENT_NOT_ENOUGH_INFO		= 0x305,	// 보안파일의 고유 정보를 얻을 수 없다.
	E_ADK_CONTENT_PACK_FAIL				= 0x306,
	E_ADK_CONTENT_SET_POINTER_FAIL		= 0x307,
	E_ADK_CONTENT_SET_END_FAIL			= 0x308,
	E_ADK_CONTENT_DIFFER_FROM_START		= 0x309,
	E_ADK_CONTENT_INIT_RIGHTS_FAIL		= 0x30A,
	E_ADK_CONTENT_LICENSE_DEL_FAIL		= 0x30B,
	E_ADK_CONTENT_NOT_AUTOPACK_PROCESS	= 0x30C,

	E_ADK_INTERNAL						= 0x400,
	E_ADK_INTERNAL_GET_CLSID_FAIL		= 0x401,
	E_ADK_INTERNAL_ENCRYPTION			= 0x402,
	E_ADK_INTERNAL_NXL_GET_DOMAINID_FAIL= 0x403,
	E_ADK_INTERNAL_FNC_POINTER_FAIL		= 0x404,

	E_ADK_LIGHT_VERSION					= 0x500,

	E_ADK_GENERAL						= 0xABCD,
};

///////////////// TYPEDEF FUNC
typedef unsigned long(*_LPGETVERSION)();

typedef unsigned int(*_LPGETAUTHENTICATE)
(
char* szFilePath
);

typedef unsigned long(*_LPSETNOTIFYMESSAGE)();

typedef unsigned int(*_LPSETMENU)
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

typedef int(*_LPADKLISLICENSEVALIDBYPATH)
(
char *szFilePath, int iPurpose
);

typedef int(*_LPADKISSECURECONTENTBYPATH)
(
char *szFilePath
);

typedef int(*_LPADKGETDOMAINIDBYPATH)
(
char *szFilePath, char *szDomainID, int iBufferSize
);

typedef void* (*_LPADKOPENCONTENT)
(
char *szFilePath, int iWritable, int iTruncate
);

typedef int(*_LPADKISSECURECONTENT)
(
void *pContentInfo
);

typedef int(*_LPADKISLICENSEVALID)
(
void *pContentInfo, unsigned short wPurpose
);

typedef int(*_LPADKSETCONTENTPOINTER)
(
void *pContentInfo, long IOffset, int iOrigin
);

typedef int(*_LPADKREADCONTENT)
(
void *pContentInfo, void* pBuffer, unsigned long dwNumberOfBytesToRead, unsigned long* lpdwNumberOfBytesToRead
);

typedef int(*_LPADKWRITECONTENT)
(
void *pContentInfo, void* pBuffer, unsigned long dwNumberOfBytesToWrite, unsigned long* lpdwNumberOfBytesToWrite
);

typedef int(*_LPADKSETENDOFCONTENT)
(
void *pContentInfo
);

typedef int(*_LPADKCLOSECONTENT)
(
void *pContentInfo
);

typedef unsigned long(*_LPADKGETLASTERROR)();

typedef int(*_LPADKSETLASTERROR)
(
unsigned long dwError
);

typedef unsigned long(*_LPADKGETDOMAINID)
(
void *pContentInfo, char *szDomainID, unsigned int cchDomainId
);

typedef int(*_LPADKGETCONTENTTYPE)
(
void *pContentInfo
);

typedef int(*_LPADKPACKCONTENT)
(
void *pContentInfoTarget, void *pContentInfoTemplate
);

typedef int(*_LPADKAUTOPACKCONTENT)
(
char *szFilePath
);

typedef int(*_LPADKSETUSAGE)
(
void *pContentInfoTarget, unsigned short wPurpose
);

typedef int(*_LPADKISLOGONSTATUS)
(
char *szDomainID
);

typedef unsigned long(*_LPADKGETCONTENTSIZE)
(
void *pContentInfo
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
	_LPADKAUTOPACKCONTENT pfnADKAutoPackContent;
	_LPADKSETUSAGE pfnADKSetUsage;
	_LPADKISLOGONSTATUS pfnADKIsLogonStatus;
	_LPADKGETCONTENTSIZE pfnADKGetContentSize;
} SIM;
typedef SIM *LPSIM;

typedef struct FVerInfo
{
	char szname[256];
	unsigned long ulVersion;
} FVI;
typedef FVI* LPFVI;

extern "C"{
	int DRMInterfaceInitialize(void **ppDocumentInterface, void **ppSystemInterface, void *pOSInformation, void* pOfficeInformation);
}