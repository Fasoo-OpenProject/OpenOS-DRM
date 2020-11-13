#ifndef TMAXOFFICE_COMMON_UTIL_DRMUTIL_H_
#define TMAXOFFICE_COMMON_UTIL_DRMUTIL_H_

// 사용자가 DRM관련 관리를 받아야 하는 이벤트에 대한 목록
typedef enum TO_OFFICE_DRM_EventIDEnum
{
    kDRMEventIDDocumentBeforeOpen,     // 문서가 열리기 직전				0
    kDRMEventIDDocumentAfterOpen,      // 문서가 열린 후					1
    kDRMEventIDDocumentBeforeSave,     // 문서가 저장되기 직전				2
    kDRMEventIDDocumentAfterSave,      // 문서가 저장 된 후					3
    kDRMEventIDDocumentBeforeEdit,     // 문서의 Content(e.g, 이미지, 테이블 내용)이 변경되기 전	4
    kDRMEventIDDocumentPreview,        // 문서 미리보기 내용 표시 전(e.g, 인쇄 시)					5
    kDRMEventIDAutoSaveBefore,         // 자동저장 파일 생성 전				6
    kDRMEventIDTempFileCreate,         // 임시 파일 생성 전					7
    kDRMEventIDTempFileDelete,         // 임시 파일 삭제					8
    kDRMEventIDInitApp,                // 프로그램 실행 시					9
    kDRMEventIDBeforeMacroFunction     // 매크로 기능 수행 전				10
} TO_OFFICE_DRM_EventIDEnum;

// getVersion
// purpose   : 하위버전 호환 
// parameter : N/A 
// return    : DRM모듈 version 정보
// e.g, DRM 모듈의 버전이 1.2.3.4인 경우, 0x01020304을 반환
typedef unsigned long (*TO_OFFICE_DRM_PFN_getVersion)();

// isExtractable
// purpose   : 복사 권한 확인
// parameter : filePath
// return    : int
//             > 0  : 권한 있음
//             == 0 : 권한 없음
typedef int (*TO_OFFICE_DRM_PFN_isExtractable)
(
    const char* UTF8FilePath
);

// isPrintable
// purpose   : 인쇄 권한 확인
// parameter : filePath
// return    : int
//             > 0  : 권한 있음
//             == 0 : 권한 없음
typedef int (*TO_OFFICE_DRM_PFN_isPrintable)
(
    const char* UTF8FilePath
);

// isEditable
// purpose   : 편집 권한 확인
// parameter : filePath
// return    : int
//             > 0  : 권한 있음
//             == 0 : 권한 없음
typedef int (*TO_OFFICE_DRM_PFN_isEditable)
(
    const char* UTF8FilePath
);

// isViewable
// purpose   : 열람 권한 확인
// parameter : filePath
// return    : int
//             > 0  : 권한 있음
//             == 0 : 권한 없음
typedef int (*TO_OFFICE_DRM_PFN_isViewable)
(
    const char* UTF8FilePath
);

// setNotifyMessage
// purpose   : 각종 Event 발생 시 DRM 권한 확인
// parameter : ContentInfo, filePath, eventID, wparam, lparam
// return    : int
//             > 0  : 권한 있음
//             == 0 : 권한 없음
typedef int (*TO_OFFICE_DRM_PFN_setNotifyMessage)
(
    void* pContentInfo, const char* UTF8FilePath, TO_OFFICE_DRM_EventIDEnum ID, void* param1, void* param2
);

// setMenu
// purpose   : 특정 메뉴 사용 권한 확인
// parameter : ContentInfo, menuID
// return    : int (default : > 0)
//             > 0  : 권한 있음
//             == 0 : 권한 없음
typedef int (*TO_OFFICE_DRM_PFN_setMenu)
(
    void* pContentInfo,
    const char* menuID
);

typedef struct DocumentInterfaceManager
{
    TO_OFFICE_DRM_PFN_getVersion pfngetVersion;
    TO_OFFICE_DRM_PFN_isExtractable pfnisExtractable;
    TO_OFFICE_DRM_PFN_isPrintable pfnisPrintable;
    TO_OFFICE_DRM_PFN_isEditable pfnisEditable;
    TO_OFFICE_DRM_PFN_isViewable pfnisViewable;
    TO_OFFICE_DRM_PFN_setNotifyMessage pfnsetNotifyMessage;
    TO_OFFICE_DRM_PFN_setMenu pfnsetMenu;
} TO_OFFICE_DRM_DIM;

// 문서마다 허용할 수 있는 용도 목록
typedef enum TO_OFFICE_DRM_PurposeEnum
{
    kDRMPurposeView,         // 열람
    kDRMPurposeSave,         // 일반 문서로 저장
    kDRMPurposeEdit,         // 편집
    kDRMPurposePrint,        // 워터마크 없이 인쇄
    kDRMPurposePrintScreen,  // 화면 캡쳐
    kDRMPurposeExtract,      // raw 데이터 외부공유 (e.g, 클립보드 내 데이터를 암호화 하지 않은 형태로 사용)
    kDRMPurposeTransfer,     // 문서 권한을 변경
    kDRMPurposeSecureSave,   // DRM 형태의 문서를 저장
    kDRMPurposeSecurePrint,  // 워터마크 넣어 인쇄
    kDRMPurposeMacro
}TO_OFFICE_DRM_PurposeEnum;

// DRM module이 설정해주는 에러 종류
// getLastError, setLastError에서 사용
typedef enum TO_OFFICE_DRM_ErrorEnum
{
    kDRMErrorOK,                          // 정상
    kDRMErrorModuleLoadFail,              // DRM 모듈 로드 실패
    kDRMErrorModuleInitFail,              // DRM 모듈 초기화 실패
    kDRMErrorCreateFileFail,              // 파일 생성 실패
    kDRMErrorInvalidArgs,                 // 유효하지 않은 인자 사용
    kDRMErrorAlreadyPrinting,             // 이미 인쇄 중
    kDRMErrorPrintingNotStarted,          // 인쇄가 시작되지 않음
    kDRMErrorMemoryError,                 // 메모리 할당 오류
    kDRMErrorInsufficientBuffer,          // 버퍼 크기가 너무 작음
    kDRMErrorConversionFail,              // 형 변환 오류
    kDRMErrorDRMClientNotFound,           // DRM Client가 확인되지 않음
    kDRMErrorNotSecureFile,               // 일반 문서 파일
    kDRMErrorCertNotAllowedFile,          // 인증되지 않은 모듈
    kDRMErrorCertLoadFail,                // 모듈에 맞는 fac 로드 실패
    kDRMErrorContentOpenFail,             // 컨텐츠 열기 실패
    kDRMErrorContentReadFail,             // 컨텐츠 읽기 실패
    kDRMErrorContentWriteFail,            // 컨텐츠 쓰기 실패
    kDRMErrorContentInvalidLicense,       // 허가되지 않은 동작
    kDRMErrorContentNotEnoughInfo,        // 보안문서에 대한 정보를 얻을 수 없음
    kDRMErrorContentPackFail,             // 보안파일 기반으로 새 보안파일 생성 실패
    kDRMErrorContentSetPointerFail,       // SetContentPointer 실패
    kDRMErrorContentEndFail,              // SetEndOfContent 실패
    kDRMErrorContentDifferFromStart,      // StartPrint와 다른 컨텐츠
    kDRMErrorContentInitRightsFail,       // Content ID가 잘못됨
    kDRMErrorContentLicenseDelFail,       // 라이선스 파일 삭제 실패
    kDRMErrorInternalGetCLSIDFail,        // 보안문서 관련 정보 획득 실패
    kDRMErrorInternalEncryptionFail,      // 암호화 오류
    kDRMErrorInternalFncPointerFail,      // 내부 함수포인터 획득 실패
    kDRMErrorGeneral                      // 미정의 오류
}TO_OFFICE_DRM_ErrorEnum;

// DRMIsLicenseValidByPath
// purpose   : 파일에 purpose 권한이 있는 지 확인
// parameter : filePath, purpose
// return    : int (default: > 0)
//             > 0  : 권한 있음
//             == 0 : ErrorCode가 kContentInvalidLicense인 경우 권한 없음, ErrorCode가 다른 경우 DRM 시스템에 이상이 생긴 것
typedef int (*TO_OFFICE_DRM_PFN_DRMIsLicenseValidByPath)
(
    const char* UTF8FilePath, TO_OFFICE_DRM_PurposeEnum purpose
);

// DRMIsSecureContentByPath
// purpose   : 보안문서인지 확인
// parameter : filePath
// return    : int (default: == 0)
//             > 0  : 보안 문서 
//             == 0 : ErrorCode가 kNotSecureFile인 경우 일반 문서, ErrorCode가 다른 경우 DRM 시스템에 이상이 생긴 것
typedef int (*TO_OFFICE_DRM_PFN_DRMIsSecureContentByPath)
(
    const char* UTF8FilePath
);

// DRMOpenContent
// purpose   : fopen
// parameter : filePath, fileMode
// return    : ContentInfo ptr ( nullptr 이면 실패 )
typedef void* (*TO_OFFICE_DRM_PFN_DRMOpenContent)
(
    const char* UTF8FilePath, const char* fileMode
);

// DRMIsSecureContent
// purpose   : 보안문서인지 확인
// parameter : ContentInfo ptr 
// return    : int (default: == 0)
//             > 0  : 보안 문서 
//             == 0 : ErrorCode가 kNotSecureFile인 경우 일반 문서, ErrorCode가 다른 경우 DRM 시스템에 이상이 생긴 것
typedef int (*TO_OFFICE_DRM_PFN_DRMIsSecureContent)
(
    void *pContentInfo
);

// DRMIsLicenseValid
// purpose   : 문서 권한 확인
// parameter : ContentInfo ptr, purpose 
// return    : int (default: == 0)
//             > 0  : 보안 문서 
//             == 0 : ErrorCode가 kContentInvalidLicense인 경우 권한 없음, ErrorCode가 다른 경우 DRM 시스템에 이상이 생긴 것
typedef int (*TO_OFFICE_DRM_PFN_DRMIsLicenseValid)
(
    void *pContentInfo, TO_OFFICE_DRM_PurposeEnum purpose
);

// DRMGetContentSize
// purpose   : 컨텐츠 크기 확인(fseek(...SEEK_END); ftell();)
// parameter : ContentInfo ptr
// return    : contents 크기 (최대 4gb)
typedef long (*TO_OFFICE_DRM_PFN_DRMGetContentSize)
(
    void *pContentInfo
);

// DRMSetContentPointer
// purpose   : fseek(..)
// parameter : ContentInfo ptr, offset, origin
// return    : 바뀐 contentpointer 위치
typedef int (*TO_OFFICE_DRM_PFN_DRMSetContentPointer)
(
    void *pContentInfo, long lOffset, int iOrigin
);

// DRMReadContent
// purpose   : fread(..)
// parameter : ContentInfo ptr, buffer, bytesToRead, numberofBytesRead ptr
// return    : 성공 여부
typedef int (*TO_OFFICE_DRM_PFN_DRMReadContent)
(
    void *pContentInfo, void *pBuffer, long lNumberOfBytesToRead, long *pNumberOfBytesRead
);

// DRMWriteContent
// purpose   : fwrite(..)
// parameter : ContentInfo ptr, buffer, bytesToWrite, numberofBytesWritten ptr
// return    : int (default: > 0)
//             > 0  : 성공 
//             == 0 : 실패
typedef int (*TO_OFFICE_DRM_PFN_DRMWriteContent)
(
    void *pContentInfo, void *pBuffer, long lNumberOfBytesToWrite, long *pNumberOfBytesWritten
);

// DRMSetEndOfContent
// purpose   : ftruncate with fileno
// parameter : ContentInfo ptr
// return    : int (default: > 0)
//             > 0  : 성공 
//             == 0 : 실패
typedef int (*TO_OFFICE_DRM_PFN_DRMSetEndOfContent)
(
    void *pContentInfo
);

// DRMCloseContent
// purpose   : fclose
// parameter : ContentInfo ptr
// return    : int (default: > 0)
//             > 0  : 성공 
//             == 0 : 실패
typedef int (*TO_OFFICE_DRM_PFN_DRMCloseContent)
(
    void *pContentInfo
);

// DRMGetLastError
// purpose   : errno
// parameter : N/A
// return    : ErrorEnum (default: kOK)
typedef TO_OFFICE_DRM_ErrorEnum (*TO_OFFICE_DRM_PFN_DRMGetLastError)();

// DRMSetLastError
// purpose   : seterrno
// parameter : ErrorEnum
// return    : N/A
typedef void (*TO_OFFICE_DRM_PFN_DRMSetLastError)
(
    TO_OFFICE_DRM_ErrorEnum error
);

// DRMPackContent
// purpose   : 파일의 DRM 권한을, DRM문서와 동일하게 변경
// parameter : targetContentInfo ptr, templateContentInfo ptr
// return    : int
//             > 0  : 성공
//             == 0 : 실패
typedef int (*TO_OFFICE_DRM_PFN_DRMPackContent)
(
    void *pContentInfoTarget, void *pContentInfoTemplate
);

// DRMAutoPackContent
// purpose   : 일반문서를 DRM 문서로 변경 또는 유지 (DRM모듈이 판단)
// parameter : filePath
// return    : int
//             > 0  : 성공
//             == 0 : 실패
typedef int (*TO_OFFICE_DRM_PFN_DRMAutoPackContent)
(
    const char* UTF8FilePath
);

// DRMSetUsage
// purpose   : DRM 모듈에게 문서 사용 완료 보고
// parameter : targetContentInfo ptr, templateContentInfo ptr
// return    : int
//             > 0  : 성공
//             == 0 : 실패
typedef int (*TO_OFFICE_DRM_PFN_DRMSetUsage)
(
    void *pContentInfoTarget, TO_OFFICE_DRM_PurposeEnum purpose                
);

typedef int (*TO_OFFICE_DRM_PFN_DRMUnpackByContentPath)
(
	char *szSrcFilePath, char *szDstFilePath
);

typedef struct SystemInterfaceManager
{
    TO_OFFICE_DRM_PFN_DRMIsLicenseValidByPath pfnDRMIsLicenseValidByPath;
    TO_OFFICE_DRM_PFN_DRMIsSecureContentByPath pfnDRMIsSecureContentByPath;
    TO_OFFICE_DRM_PFN_DRMOpenContent pfnDRMOpenContent;
    TO_OFFICE_DRM_PFN_DRMIsSecureContent pfnDRMIsSecureContent;
    TO_OFFICE_DRM_PFN_DRMIsLicenseValid pfnDRMIsLicenseValid;
    TO_OFFICE_DRM_PFN_DRMGetContentSize pfnDRMGetContentSize;
    TO_OFFICE_DRM_PFN_DRMSetContentPointer pfnDRMSetContentPointer;
    TO_OFFICE_DRM_PFN_DRMReadContent pfnDRMReadContent;
    TO_OFFICE_DRM_PFN_DRMWriteContent pfnDRMWriteContent;
    TO_OFFICE_DRM_PFN_DRMSetEndOfContent pfnDRMSetEndOfContent;
    TO_OFFICE_DRM_PFN_DRMCloseContent pfnDRMCloseContent;
    TO_OFFICE_DRM_PFN_DRMGetLastError pfnDRMGetLastError;
    TO_OFFICE_DRM_PFN_DRMSetLastError pfnDRMSetLastError;
    TO_OFFICE_DRM_PFN_DRMPackContent pfnDRMPackContent;
    TO_OFFICE_DRM_PFN_DRMAutoPackContent pfnDRMAutoPackContent;
    TO_OFFICE_DRM_PFN_DRMSetUsage pfnDRMSetUsage;
	TO_OFFICE_DRM_PFN_DRMUnpackByContentPath pfnADKUnpackByContentPath;
} TO_OFFICE_DRM_SIM;

typedef struct VerInfo
{
    char* name;   
    unsigned long ulVersion;
} TO_OFFICE_DRM_VI;

// DRMInterfaceInitialize
// purpose   : DRM 모듈 초기화
// OS version, Office version 정보를 바탕으로 DIM, SIM을 생성 & 할당
// return    : int
//             > 0  : 성공
//             == 0 : 실패
int TO_OFFICE_DRM_DRMInterfaceInitialize(TO_OFFICE_DRM_DIM **ppDocumentInterface,
        TO_OFFICE_DRM_SIM **ppSystemInterface,
        TO_OFFICE_DRM_VI* pOSInformation,
        TO_OFFICE_DRM_VI* pOfficeInformation);

#endif
