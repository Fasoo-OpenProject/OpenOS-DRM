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
	
// 1st �Ķ���� strTarget: �����ϰ��� �ϴ� ������Ʈ�� Key ��� ex) "Software\Fasoo DRM\Policy"
// 2nd �Ķ���� strName: �����ϰ��� �ϴ� ������Ʈ�� Key������ �� �̸� ex) Default
// 3rd �Ķ���� Flag: Default�� Null. (64bit ȯ���϶�, RegQueryValue�� Flag�� KEY_WOW64_64KEY�� �־ ����ϴ� ���� ������ �߰�) ex) "KEY_WOW64_64KEY"
// 4st �Ķ���� strValue: �����ϰ����ϴ� ������Ʈ�� Key������ Value ex) 0100000000001001

// ��� ����
// 	string* strValue = new string[512];
//	bool bResult = _ReadConfig("Software\Fasoo DRM\Policy", "Default", "", strValue);

// Config ���� ��: FEDConfig.json
// Config ���� ��ġ: /volume/c:/Program Files (x86)/Fasoo DRM/FEDConfig.json

#endif	// __ConfigJson_H__