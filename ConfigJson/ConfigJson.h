#ifndef __ConfigJson_H__
#define __ConfigJson_H__

#include <iostream> 
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/utsname.h>

#define CONFIGFILE_SUCCESS					0	// 성공
#define CONFIGFILE_OPEN_ERR					1	// FEDConfig.json 파일 열기 실패 
#define CONFIGFILE_KEYPATH_ERR					2	// Target 경로에 해당 Key가 존재하지 않음 
#define CONFIGFILE_VALUENAME_NO_EXIST				3	// 해당 Key에 추출하고자 하는 Value Name이 존재하지 않음
#define CONFIGFILE_VALUE_NO_EXIST				4	// 해당 Value Name에 값이 없음
#define CONFIGFILE_VALUETYPE_NO_SZ				5	// 설정한 Value Type REG_SZ (문자열)이 아님 
#define CONFIGFILE_VALUETYPE_NO_DWORD				6	// 설정한 Value Type REG_DWORD (숫자)이 아님 
#define CONFIGFILE_INPUTVALUE_ERR				7	// Output Value 할당을 잘못하였음 

// Config 파일 명: FEDConfig.json
// Config 파일 위치: /volume/c:/Program Files (x86)/Fasoo DRM/FEDConfig.json

extern "C"
{
	bool _ReadConfig(char* pTarget, char* pName, char* pFlag, char* pType, void* pValue, unsigned long size);
	// 1st 파라미터 pTarget: 추출하고자 하는 레지스트리 Key 경로 ex) "Software\Fasoo DRM\Policy"

	// 2nd 파라미터 pName: 추출하고자 하는 레지스트리 Key에서의 값 이름 ex) Default

	// 3rd 파라미터 pFlag: Default는 빈 문자열. (64bit 환경일때, RegQueryValue의 Flag로 KEY_WOW64_64KEY를 넣어서 사용하던 것이 있을때 추가) ex) 기존에 pfnRegOpenKeyExW(HKEY_LOCAL_MACHINE, wcsCrypto, 0, KEY_READ | KEY_WOW64_64KEY, &hKey)를 통해서 사용하였다면 "KEY_WOW64_64KEY" 입력
	// * 추가설명: 64bit 환경에서 컴퓨터\HKEY_LOCAL_MACHINE\SOFTWARE\Fasoo DRM 하위의 Key-value를 가져오고 싶을때 활용 
	// 기존에 RegQueryValue 함수를 통해서 가져올 때 4번째 파라미터 Flage 값에 KEY_WOW64_64KEY가 있을때만 기입하면 됨

	// 4th 파라미터 pType: 추출하고자하는 레지스트리 Value의 Type - (1) REG_SZ - 문자열 (2) REG_DWORD - 숫자
	// 4th 파라미터 pValue: 추출하고자하는 레지스트리 Key에서의 Value ex) 0100000000001001
	// 5th 파라미터 size: 추출하고자하는 레지스트리 Key에서의 Value 크기

	// 사용 예제1 - REG_SZ 데이터타입 값 가져오기위한 예제
	// char* strValue = (char*)malloc(sizeof(char) * 1028 + 1);
	//	bool bResult = _ReadConfig("Software\\Fasoo DRM\\Policy", "Default", "", "REG_SZ", strValue, sizeof(char)*1028+1);

	// 사용 예제2 - REG_DWORD 데이터타입 값 가져오기위한 예제
	// 	int strValue = 0;
	//	bool bResult = _ReadConfig("Software\\Fasoo DRM", "InstallPath", "KEY_WOW64_64KEY", "REG_DWORD", &strValue, sizeof(int));

	int _ReadConfigErr(char* pTarget, char* pName, char* pFlag, char* pType, void* pValue, unsigned long size);
	// 위의 _ReadConfig 함수와 파라미터는 동일.
	// Return 값이 Error Code Return (0이 성공)

	bool _GetConfQueryKey(char* pTarget, char* pFlag, int target_index, char* pKeyName);
	// 1st 파라미터 pTarget: 추출하고자 하는 레지스트리 Key 경로 ex) "Software\Fasoo DRM\Policy"
	// 2nd 파라미터 pFlag: Default는 빈 문자열. (64bit 환경일때, RegQueryValue의 Flag로 KEY_WOW64_64KEY를 넣어서 사용하던 것이 있을때 추가) ex) 기존에 pfnRegOpenKeyExW(HKEY_LOCAL_MACHINE, wcsCrypto, 0, KEY_READ | KEY_WOW64_64KEY, &hKey)를 통해서 사용하였다면 "KEY_WOW64_64KEY" 입력
	// * 추가설명: 64bit 환경에서 컴퓨터\HKEY_LOCAL_MACHINE\SOFTWARE\Fasoo DRM 하위의 Key-value를 가져오고 싶을때 활용 
	// 기존에 RegQueryValue 함수를 통해서 가져올 때 4번째 파라미터 Flage 값에 KEY_WOW64_64KEY가 있을때만 기입하면 됨
	// 3rd 파라미터 target_index: 추출하고자하는 레지스트리 Key Index
	// 4th 파라미터 pKeyName: 추출하고자하는 레지스트리 Key Name

	//	사용 예제
	// int target_index = 0;
	// 
	// while (1)
	// {
	// 	char* strValue = (char*)malloc(sizeof(char*) * 1028 + 1);
	// 	bool bResult = _GetConfQueryKey("SOFTWARE\\Fasoo DRM\\Client", "", target_index, strValue);
	// 	target_index++;
	// 
	// 	if (!bResult)
	// 		break;
	// 
	// 	free(strValue);
	// }

	bool _GetConfQueryValue(char* pTarget, char* pFlag, int target_index, char* pValueName);
	// 활용 가이드는 위의 함수 _GetConfQueryKey와 일치 (단지, Target이 Key가 아닌 Value로 바뀐 것)

}
#endif //__ConfigJson_H__
