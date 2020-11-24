#ifndef __ConfigJson_H__
#define __ConfigJson_H__

#include <iostream> 
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <jsoncpp/json/json.h>
#include <sys/utsname.h>

bool _ReadConfig(std::string strTarget, std::string strName, std::string Flag, std::string *strValue);
	
// 1st 파라미터 strTarget: 추출하고자 하는 레지스트리 Key 경로 ex) "Software\Fasoo DRM\Policy"
// 2nd 파라미터 strName: 추출하고자 하는 레지스트리 Key에서의 값 이름 ex) Default
// 3rd 파라미터 Flag: Default는 Null. (64bit 환경일때, RegQueryValue의 Flag로 KEY_WOW64_64KEY를 넣어서 사용하던 것이 있을때 추가) ex) "KEY_WOW64_64KEY"
// 4st 파라미터 strValue: 추출하고자하는 레지스트리 Key에서의 Value ex) 0100000000001001

// 사용 예제
// 	string* strValue = new string[512];
//	bool bResult = _ReadConfig("Software\Fasoo DRM\Policy", "Default", "", strValue);

// Config 파일 명: FEDConfig.json
// Config 파일 위치: /volume/c:/Program Files (x86)/Fasoo DRM/FEDConfig.json

#endif	// __ConfigJson_H__