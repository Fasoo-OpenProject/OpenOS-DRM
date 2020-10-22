#include "DRMUtil.h"

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
	ADK_PURPOSE_MACRO					= 20,
};

enum ADK_ERROR
{
	E_ADK_OK							= 0x0,		// 성공
	E_ADK_LOAD_NXL_FAIL					= 0x1,		// NXL 모듈 로드 실패
	E_ADK_NXL_INIT_FAIL					= 0x2,		// NXL 모듈 초기화 실패
	E_ADK_WIN32_CREATEFILE_FAIL			= 0x3,		// fppen 실패
	E_ADK_INVALID_ARGS					= 0x4,		// 유효하지 않은 파라미터

	E_ADK_NOT_SECURE_CONTENT			= 0xFF,		// 암호화되지 않은 파일

	E_ADK_CONTENT						= 0x300,
	E_ADK_CONTENT_OPEN_FAIL				= 0x301,	// OpenContent 실패
	E_ADK_CONTENT_READ_FAIL				= 0x302,	// ReadContent 실패
	E_ADK_CONTENT_WRITE_FAIL			= 0x303,	// WriteContent 실패
	E_ADK_CONTENT_INVALID_LICENSE		= 0x304,	// 권한이 유효하지 않음
	E_ADK_CONTENT_PACK_FAIL				= 0x306,	// 암호화 실패
	E_ADK_CONTENT_SET_POINTER_FAIL		= 0x307,	// SetContentPointer 실패
	E_ADK_CONTENT_SET_END_FAIL			= 0x308,	// SetEndOfContent 실패
	E_ADK_INTERNAL_NXL_GET_DOMAINID_FAIL= 0x403,

	E_ADK_GENERAL						= 0xABCD,
};

///////////////// TYPEDEF FUNC
typedef unsigned long(*_LPGETVERSION)();

typedef unsigned int(*_LPGETAUTHENTICATE)
(
char* szFilePath
);

typedef unsigned long(*_LPSETNOTIFYMESSAGE)
(
void* pContentInfo, const char* UTF8FilePath, TO_OFFICE_DRM_EventIDEnum ID, void* param1, void* param2
);

typedef unsigned int(*_LPSETMENU)
(
void* pContentInfo, const char* uiMenuID
);

typedef struct FasooDocumentInterfaceManager
{
	_LPGETVERSION pfngetVersion;
	_LPGETAUTHENTICATE pfngetAuthenticate;
	_LPSETNOTIFYMESSAGE pfnsetNotifyMessage;
	_LPSETMENU pfnsetMenu;
} DIM;
typedef DIM *LPDIM;

typedef int(*_LPADKLISLICENSEVALIDBYPATH)
(
char *szFilePath, unsigned short wPurpose
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
char *szFilePath, char *szFileMode, bool bWritable, bool bTruncate
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

typedef struct FasooSystemInterfaceManager
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

#ifdef __cplusplus
extern "C"{
#endif
	int DRMInterfaceInitialize(void **ppDocumentInterface, void **ppSystemInterface, void *pOSInformation, void* pOfficeInformation);
#ifdef __cplusplus
}
#endif