// NXAPI.h
//

#ifndef __NXAPI_H__
#define __NXAPI_H__

#include <stdio.h>
//#include "../include/typedef.h"
#include "ContentInfo.h"


////////////////////////////////////////////////////////
// defined in f_nxl.dll


typedef void*(*_LPALLOCATEMEMORY)(unsigned long dwSize, bool bZeroMemory);

typedef bool(*_LPFREEMEMORY)(void* pv);

typedef LPCONTENTINFO(*_LPOPENCONTENTINFOBYHANDLE)(FILE* fp, wchar_t* pwcsFilePath, bool bPack, bool bInsertIntoMap, bool bCheckLicense);

typedef bool (*_LPISSECURE)(LPCONTENTINFO lpContentInfo);

typedef bool (*_LPCLOSECONTENTINFO)(PCONTENTINFO lpContentInfo);

typedef bool(*_LPISSECUREFILEBYHANDLE)(FILE* fp);

typedef bool(*_LPISSECUREFORMAT)(unsigned char* phPart, unsigned int dwBytes);

typedef bool(*_LPREADCONTENT)(LPCONTENTINFO lpContentInfo, void* lpBuffer, unsigned long nNumberOfBytesToRead, unsigned long* lpNumberOfBytesRead);

typedef bool(*_LPWRITECONTENT)(LPCONTENTINFO lpContentInfo,	void* lpBuffer, unsigned long nNumberOfBytesToWrite, unsigned long* lpNumberOfBytesWritten);

typedef bool(*_LPISLICENSEVALID)(LPCONTENTINFO lpContentInfo, unsigned short guidPurpose);

typedef unsigned long(*_LPGETENCRYPTEDCONTENTSIZE)(LPCONTENTINFO lpContentInfo, unsigned long* lpContentSizeHigh);

typedef bool(*_LPSETENDOFCONTENT)(LPCONTENTINFO lpContentInfo);

typedef unsigned long(*_LPSETCONTENTPOINTER)(LPCONTENTINFO lpContentInfo, long lDistanceToMove, long* lpDistanceToMoveHigh, unsigned long dwMoveMethod);

typedef bool(*_LPPACKBYHANDLE)(FILE* hTargetFile, const wchar_t* pwcsFilePath, const wchar_t* pwcsDomain);

typedef bool(*_LPPACKBYHANDLETEMPLATE)(FILE* hTargetFile, const wchar_t* pwcsFilePath, LPCONTENTINFO lpTemplate);

typedef unsigned long(*_LPGETORIGINALCONTENTSIZE)(LPCONTENTINFO lpContentInfo, unsigned long* lpContentSizeHigh);

typedef bool(*_LPSETUSAGEEX)(LPCONTENTINFO, unsigned short, bool, LPUSAGEINFORMATION, LPUSAGESTATUS, unsigned long);

typedef unsigned long(*_LPUNPACKBYCONTENTPATH)(const wchar_t* pwcsSrc, const wchar_t* pwcsDst, bool bCheckLicense, bool bLog, bool bNX);

typedef LPCONTENTINFO(*_LPDUPLICATECONTENTINFO)(LPCONTENTINFO lpInfoSrc, bool bInheritContext);

typedef bool(*_LPUNPACKBYCONTENTINFO)(LPCONTENTINFO lpContentInfo);

typedef bool(*_LPSENDNOTIFICATIONTOMESSAGESERVER)(unsigned int dwMsg, void* hWnd);

/*

typedef unsigned int (*_LPGETPACKAGESIZE)(LPCONTENTINFO lpContentInfo, unsigned int* lpContentSizeHigh);

typedef unsigned int(*_LPGETORIGINALCONTENTSIZE)(LPCONTENTINFO lpContentInfo,unsigned int* lpContentSizeHigh);

typedef LPCONTENTINFO (*_LPDUPLICATECONTENTINFO)(
	LPCONTENTINFO lpInfoSrc,
	bool bInheritContext
);

typedef LPCONTENTINFO (*_LPALLOCATECONTENTINFO)(
	LPCWSTR pwcsFilePath
);

typedef bool (*_LPREPLACECONTENTHEADERBYNAME)(
	LPCONTENTINFO lpContentInfo,
	LPCWSTR pwcsPath
);

typedef unsigned long (*_LPPACKBYFILEPATH)(
	LPCWSTR pwcsSrc,
	LPCWSTR pwcsDst,
	bool bTypeIsNX,		// = TRUE
	bool bCheckLicense	// = TRUE
);

typedef unsigned long (*_LPUNPACKBYFILEPATH)(
	LPCWSTR pwcsSrc,
	LPCWSTR pwcsDst,
	bool bCheckLicense	// = TRUE
);

typedef unsigned long (*_LPUNPACKBYCONTENTPATH)(
	LPCWSTR pwcsSrc,
	LPCWSTR pwcsDst,
	bool bCheckLicense,
	bool bLog,
	bool bNX
);

typedef unsigned long (*_LPPACKBYCONTENTPATH)(
	LPCWSTR pwcsSrc,
	LPCWSTR pwcsDst,
	bool bCheckLicense,
	bool bLog,
	bool bNX
);

typedef bool (*_LPPACKBYHANDLE)(
	HANDLE hTargetFile,
	LPCWSTR pwcsFilePath,
	LPCWSTR pwcsDomain
);

typedef bool (*_LPPACKBYHANDLETEMPLATE)(
	HANDLE hTargetFile,
	LPCWSTR pwcsFilePath,
	LPCONTENTINFO lpTemplate
);

typedef bool (*_LPPACKBYCONTENTINFO)(
	PCONTENTINFO lpContentInfo
);

typedef bool (*_LPUNPACKBYHANDLE)(
	HANDLE hFile,
	LPCWSTR pwcsFilePath
);

typedef bool (*_LPCOPYCONTENT)(
	LPCONTENTINFO lpInfoDest,
	LPCONTENTINFO lpInfoSrc
);

typedef bool (*_LPSETUSAGE)(
	LPCONTENTINFO lpContentInfo,
	WORD wPurpose,
	bool bFilter // = TRUE
);

typedef LPTLS (*_LPGETTHREADSTORAGE)();

typedef bool (*_LPLOGINSSO)();

typedef bool (*_LPGETLOGINSTATUS)();

typedef LPPOLICY (*_LPGETPOLICY)();

typedef DWORD (*_LPGETDOMAINPOLICYPATH)(LPWSTR, DWORD);

typedef DWORD (*_LPGETUSERPOLICYPATH)(LPWSTR, DWORD, bool);

typedef DWORD (*_LPGETLICENSEPOLICYPATH)(LPWSTR, DWORD, bool);

typedef DWORD (*_LPENABLENOTIFICATIONMESSAGEBOX)(bool);

typedef bool (*_LPREPLACECONTENTHEADERITEM)(LPCONTENTINFO, LPCWSTR, LPCWSTR);

typedef DWORD (*_LPREVOKECONTENT)(LPCONTENTINFO);

typedef void (*_LPCHECKREVOKECONTENT)(bool);

typedef bool (*_LPPACKBYCONTENTINFOTEMPLATE)(LPCONTENTINFO, LPCONTENTINFO);

typedef bool (*_LPSETACTIVECONTENTINFO)(LPCONTENTINFO);

typedef void (*_LPCHECKDOMAINOVER)(bool);

typedef void (*_LPCALLINGMODULECSDRM)(bool);

typedef bool (*_LPPACKBYHANDLETEMPORARYFILE)(HANDLE, LPCWSTR);

typedef DWORD (*_LPGETCONTAINERTYPE)(LPCWSTR);

typedef bool (*_LPCONVERTFORMAT)(LPCWSTR);

typedef DWORD (*_LPGETSECURITYLEVELRIGHTS)(LPCWSTR, LPCWSTR, LPWSTR*);

typedef LPCWSTR (*_LPGETLOGONSTATUSW)(LPCWSTR);

typedef DWORD (*_LPGETSECURITYFOLDERPATH)(LPCONTENTINFO, LPWSTR, DWORD);

typedef bool (*_LPISCONTENTWINDOW)(HWND);

typedef bool (*_LPSETCONTENTINFO)(LPCWSTR, LPCONTENTINFO);

typedef bool (*_LPDOWNLOADPROVLICENSE)(LPCONTENTINFO, DWORD, LPCSTR);

typedef void (*_LPCHECKSESSIONID)(bool);

typedef DWORD (*_LPGETAUTOPACKTYPE)();

typedef bool (*_LPUNPACKBYCONTENTINFOTOHANDLE)(LPCONTENTINFO, HANDLE);

typedef DWORD (*_LPPACKBYCONTENTPATHSLV)(LPCWSTR, LPCWSTR, LPCWSTR, bool, bool);

typedef bool (*_LPDELETELICENSE)(LPCWSTR);

typedef bool (*_LPDELETELICENSEFILE)(LPCWSTR);

typedef bool (*_LPREPLACECONTENTHEADERCSDRM)(LPCONTENTINFO);

typedef DWORD (*_LPGETCONTAINERTYPEBYHANDLE)(HANDLE, LPCWSTR);

typedef bool (*_LPGETPMTIME)(LPWSTR, DWORD);

typedef void (*_LPDISABLEREVOKECHECK)(bool);

typedef bool (*_LPISAUTOPACKPROCESS)(LPCWSTR, bool*);

typedef bool (*_LPOPTIONQUERYVALUE)(LPOPTIONINFO);

typedef bool (*_LPDELETESECURITYKEY)(LPCWSTR);

typedef bool (*_LPOPTIONQUERYVALUEBYDEFAULTPOLICY)(LPOPTIONINFO);

typedef DWORD (*_LPLOGINSSOEX)(LPCWSTR, DWORD, LPWSTR, DWORD, DWORD_PTR, DWORD_PTR);

typedef DWORD (*_LPQUERYLICENSEFILE)(LPCWSTR, QUERY_LICENSEFILE, DWORD_PTR, DWORD_PTR);

typedef bool (*_LPQUERYSSOINFO)(LPCWSTR, DWORD, PVOID, DWORD_PTR, DWORD_PTR);

typedef DWORD(*_LPGETSECURITYLEVELNAME)(LPCWSTR, LPCWSTR, WCHAR*, DWORD, DWORD_PTR, DWORD_PTR);

typedef bool(*_LPPACKBYHANDLEFED5)(LPPACKINFO_FED5, LPSECTIONINFO, DWORD_PTR, DWORD_PTR);

typedef bool(*_LPGETCONTENTKEY)(LPCWSTR, unsigned char**, DWORD*, DWORD_PTR, DWORD_PTR);

typedef bool(*_LPSETPRODUCTID)(LPCONTENTINFO, DWORD, DWORD_PTR, DWORD_PTR);

typedef bool(*_LPGETSECURITYLEVELINFO)(LPCWSTR, PSECURITYLEVELINFO, DWORD_PTR, DWORD_PTR);

typedef DWORD(*_LPNXLGETLASTERROR)();

typedef void(*_LPSETCALLINGMODULETYPE)(DWORD);

typedef DWORD(*_LPPACKBYCONTENTPATHEX)(PACK_INFORMATION_CLASS, PVOID, DWORD_PTR, DWORD_PTR);

typedef DWORD(*_LPREPLACETRACESECTIONBYPROPERTYINFO)(LPCWSTR, LPCWSTR, LPCWSTR, DWORD_PTR, DWORD_PTR);

typedef bool(*_LPREPLACECONTENTINFO)(DWORD, LPCONTENTINFO, DWORD_PTR, DWORD_PTR);

typedef bool(*_LPREMOVESECTION)(LPCWSTR, unsigned int, unsigned int*, unsigned int, DWORD_PTR, DWORD_PTR);

typedef bool(*_LPGETPMTIMEEX)(DWORD, LPWSTR, DWORD, DWORD_PTR, DWORD_PTR);

*/
////////////////////////////////////////////////////////////////////////////////////

typedef struct _NXLINTERFACE
{
	_LPALLOCATEMEMORY AllocateMemory;
	_LPFREEMEMORY FreeMemory;
	_LPOPENCONTENTINFOBYHANDLE OpenContentByHandle;
	_LPISSECURE IsSecure;
	_LPCLOSECONTENTINFO CloseContentInfo;
	_LPISSECUREFILEBYHANDLE IsSecureFileByHandle;
	_LPREADCONTENT ReadContent;
	_LPWRITECONTENT WriteContent;
	_LPISLICENSEVALID IsLicenseValid;
	_LPGETENCRYPTEDCONTENTSIZE GetEncryptedContentSize;
	_LPSETENDOFCONTENT SetEndOfContent;
	_LPSETCONTENTPOINTER SetContentPointer;
	_LPPACKBYHANDLE	PackByHandle;
	_LPPACKBYHANDLETEMPLATE PackByHandleTemplate;
	_LPGETORIGINALCONTENTSIZE GetOriginalContentSize;
	_LPSETUSAGEEX SetUsageEx;
	_LPUNPACKBYCONTENTPATH UnpackByContentPath;
	_LPDUPLICATECONTENTINFO DuplicateContentInfo;
	_LPUNPACKBYCONTENTINFO UnpackByContentInfo;
	_LPSENDNOTIFICATIONTOMESSAGESERVER SendNotificationToMessageServer;

	/*
	
	_LPGETPACKAGESIZE _GetPackageSize;
	_LPGETORIGINALCONTENTSIZE _GetOriginalContentSize;	
	_LPSETCONTENTPOINTER _SetContentPointer;
	_LPDUPLICATECONTENTINFO _DuplicateContentInfo;
	_LPALLOCATECONTENTINFO _AllocateContentInfo;
	_LPREPLACECONTENTHEADERBYNAME _ReplaceContentHeaderByName;
		
	_LPPACKBYHANDLE _PackByHandle;
	_LPPACKBYHANDLETEMPLATE _PackByHandleTemplate;
	_LPPACKBYCONTENTINFO _PackByContentInfo;
	_LPUNPACKBYHANDLE _UnpackByHandle;
	
	_LPREADCONTENT _ReadContent;
	_LPWRITECONTENT _WriteContent;
	_LPISLICENSEVALID _IsLicenseValid;
	_LPISSECUREFORMAT _IsSecureFormat;
	_LPCOPYCONTENT _CopyContent;
	_LPPACKBYFILEPATH _PackByFilePath;
	_LPUNPACKBYFILEPATH _UnpackByFilePath;
	_LPUNPACKBYCONTENTPATH _UnpackByContentPath;
	_LPPACKBYCONTENTPATH _PackByContentPath;
	_LPSETUSAGE _SetUsage;
	_LPSETUSAGEEX _SetUsageEx;	
	_LPGETTHREADSTORAGE _GetThreadStorage;
	_LPLOGINSSO _LoginSSO;
	_LPGETLOGINSTATUS _GetLoginStatus;
	_LPGETUSERPOLICYPATH _GetUserPolicyPath;
	_LPGETLICENSEPOLICYPATH _GetLicensePolicyPath;
	_LPENABLENOTIFICATIONMESSAGEBOX _EnableNotificationMessageBox;
	_LPREVOKECONTENT _RevokeContent;
	_LPCHECKREVOKECONTENT _CheckRevokeContent;
	_LPPACKBYCONTENTINFOTEMPLATE _PackByContentInfoTemplate;
	_LPSETACTIVECONTENTINFO _SetActiveContentInfo;
	_LPCHECKDOMAINOVER _CheckDomainOver;
	_LPCALLINGMODULECSDRM _CallingModuleCSDRM;
	_LPPACKBYHANDLETEMPORARYFILE _PackByHandleTemporaryFile;
	_LPGETCONTAINERTYPE _GetContainerType;
	_LPCONVERTFORMAT _ConvertFormat;
	_LPGETSECURITYLEVELRIGHTS _GetSecurityLevelRights;
	_LPGETLOGONSTATUSW _GetLogonStatusW;
	_LPGETSECURITYFOLDERPATH _GetSecurityFolderPath;
	_LPISCONTENTWINDOW _IsContentWindow;
	_LPSETCONTENTINFO _SetContentInfo;
	_LPDOWNLOADPROVLICENSE _DownloadProvLicense;
	_LPCHECKSESSIONID _CheckSessionId;
	_LPGETAUTOPACKTYPE _GetAutoPackType;
	_LPUNPACKBYCONTENTINFOTOHANDLE _UnpackByContentInfoToHandle;
	_LPPACKBYCONTENTPATHSLV _PackByContentPathSLV;
	_LPDELETELICENSE _DeleteLicense;
	_LPDELETELICENSEFILE _DeleteLicenseFile;
	_LPREPLACECONTENTHEADERCSDRM _ReplaceContentHeaderCsDRM;
	_LPGETCONTAINERTYPEBYHANDLE _GetContainerTypeByHandle;
	_LPGETPMTIME _GetPMTime;
	_LPDISABLEREVOKECHECK _DisableRevokeCheck;
	_LPISAUTOPACKPROCESS _IsAutoPackProcess;
	_LPOPTIONQUERYVALUE _OptionQueryValue;
	_LPDELETESECURITYKEY _DeleteSecurityKey;
	_LPOPTIONQUERYVALUEBYDEFAULTPOLICY _OptionQueryValueByDefaultPolicy;
	_LPLOGINSSOEX _LoginSSOEx;
	_LPQUERYLICENSEFILE _QueryLicenseFile;
	_LPQUERYSSOINFO _QuerySSOInfo;
	_LPGETSECURITYLEVELNAME _GetSecurityLevelName;
	_LPPACKBYHANDLEFED5 _PackByHandleFED5;
	_LPGETCONTENTKEY _GetContentKey;
	_LPSETPRODUCTID _SetProductId;
	_LPGETSECURITYLEVELINFO _GetSecurityLevelInfo;
	_LPNXLGETLASTERROR _NxlGetLastError;
	_LPSETCALLINGMODULETYPE _SetCallingModuleType;
	_LPPACKBYCONTENTPATHEX _PackByContentPathEx;
	_LPREPLACETRACESECTIONBYPROPERTYINFO _ReplaceTraceSectionByPropertyInfo;
	_LPREPLACECONTENTINFO _ReplaceContentInfo;
	_LPREMOVESECTION _RemoveSection;
	_LPGETPMTIMEEX _GetPMTimeEx;
	*/
} NXLINTERFACE;

typedef NXLINTERFACE *PNXLINTERFACE;

//////////////////////////////////////////////////////////////////////////
//

void* __AllocateMemory(
	unsigned long dwSize,
	bool bZeroMemory
	);

bool __FreeMemory(
	void* pv
	);

LPCONTENTINFO __OpenContentByHandle(
	FILE* fp, 
	wchar_t* pwcsFilePath,
	bool bPack, 
	bool bInsertIntoMap, 
	bool bCheckLicense
);

bool __IsSecure(
	LPCONTENTINFO lpContentInfo
);

bool __CloseContentInfo(
	PCONTENTINFO lpContentInfo
);

bool __IsSecureFileByHandle(
	FILE* hFile
);


bool __IsSecureFormat(
	unsigned char* phPart,
	unsigned long dwBytes
);

bool __IsSecureFormatEx(
	unsigned char* phPart,
	unsigned long dwBytes,
	unsigned long* pdwType
);


bool __ReadContent(
	LPCONTENTINFO lpContentInfo,
	void* lpBuffer,
	unsigned long nNumberOfBytesToRead,
	unsigned long* lpNumberOfBytesRead
	);

bool __WriteContent(
	LPCONTENTINFO lpContentInfo,
	void* lpBuffer,
	unsigned long nNumberOfBytesToWrite,
	unsigned long* lpNumberOfBytesWritten
);

bool __IsLicenseValid(
	LPCONTENTINFO lpContentInfo,
	unsigned short guidPurpose
);

unsigned long __GetEncryptedContentSize(
	LPCONTENTINFO lpContentInfo,
	unsigned long* lpContentSizeHigh
);

bool __SetEndOfContent(
	LPCONTENTINFO lpContentInfo
);

unsigned long __SetContentPointer(
	LPCONTENTINFO lpContentInfo,
	long lDistanceToMove,
	long* lpDistanceToMoveHigh,
	unsigned long dwMoveMethod
);

bool __PackByHandle(
	FILE* hTargetFile,
	const wchar_t* pwcsFilePath,
	const wchar_t* pwcsDomain	// = NULL
);

bool __PackByHandleTemplate(
	FILE* hTargetFile,
	const wchar_t* pwcsFilePath,
	LPCONTENTINFO lpTemplate
);

unsigned long __GetOriginalContentSize(
	LPCONTENTINFO lpContentInfo,
	unsigned long* lpContentSizeHigh
);

bool __SetUsageEx(
	LPCONTENTINFO lpContentInfo,
	unsigned short wPurpose,
	bool bFilter,
	LPUSAGEINFORMATION lpUsageInfo,
	LPUSAGESTATUS lpUsageStatus,
	unsigned long dwReserved2
);

unsigned long __GetSecurityLevelName(
	const wchar_t* pwcsDomain,
	const wchar_t* pwcsSecurityLevel, // in
	wchar_t* pwcsSecurityLevelName,  // out
	unsigned int dwBufSize,			   // in
	unsigned int dwReserved1,
	unsigned int dwReserved2
);

unsigned long __UnpackByContentPath(
	const wchar_t* pwcsSrc,
	const wchar_t* pwcsDst,
	bool bCheckLicense,
	bool bLog,
	bool bNX
);

LPCONTENTINFO __DuplicateContentInfo(
	LPCONTENTINFO lpInfoSrc,
	bool bInheritContext
);

bool __UnpackByContentInfo(
	LPCONTENTINFO lpContentInfo
);

bool __SendNotificationToMessageServer(
	unsigned int dwNotification,
	void* hWnd
);

//////////////////////////////////////////////////////////////////////////
//

extern "C" { PNXLINTERFACE NxInitialize(); }

#endif	// __NXAPI_H__
