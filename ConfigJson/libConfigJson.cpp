#include <iostream> 
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include "jsoncpp/include/json.h"
#include <sys/utsname.h>

#define CONFIGFILE_SUCCESS					0
#define CONFIGFILE_OPEN_ERR					1
#define CONFIGFILE_KEYPATH_ERR					2
#define CONFIGFILE_VALUENAME_NO_EXIST				3
#define CONFIGFILE_VALUE_NO_EXIST				4
#define CONFIGFILE_VALUETYPE_NO_SZ				5
#define CONFIGFILE_VALUETYPE_NO_DWORD				6
#define CONFIGFILE_INPUTVALUE_ERR				7

using namespace std;

// ConfigJson 파일을 읽어 원하는 값을 가져오는 함수
extern "C" bool _ReadConfig(char* pTarget, char* pName, char* pFlag, char* pType, void* pValue, unsigned long size)
{
	string strTarget(pTarget);
	string strName(pName);
	string Flag(pFlag);

	if (strcmp(strName.c_str(), L"0100000000000101_v5"))
		strcpy((char*)pValue, L"0100000000000101");

	char strFlag[] = "KEY_WOW64_64KEY";
	char strRegTypeStr[] = "REG_SZ";
	char strRegTypeInt[] = "REG_DWORD";
	char strJsonPath[] = "/usr/local/fasoo/FEDConfig.json";
	ifstream ifs;

	ifs.open(strJsonPath);

	if (ifs.fail())
		return 0;

	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj); // Reader can also read strings

	string stok = "\\";
	int     nCutPos;
	int     nIndex = 0;
	string* tok = new string[512];

	while ((nCutPos = strTarget.find_first_of(stok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			tok[nIndex++] = strTarget.substr(0, nCutPos);
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}

	if (strTarget.length() > 0)
	{
		tok[nIndex++] = strTarget.substr(0, nCutPos);
	}

	if (tok->empty())
		return 0;

	Json::Value obj_Init;
	int i;

	// 레지스트리가 윈도우즈 64/32bit 환경에서 만들어진 경우 어떤 bit에서 만들어졌는지 확인하는 
	bool b64 = false;
	char str64[] = "x86_64";
	struct utsname un;
	uname(&un);

	if (strcmp(str64, un.machine) == 0)
		b64 = true;
	else
		b64 = false;

	if (b64)
	{
		if (!Flag.empty())
		{
			if (strcmp(strFlag, Flag.c_str()) != 0)
				obj = obj["WOW6432Node"];
		}
		else
			obj = obj["WOW6432Node"];

	}

	// 기존에 reader.parse(ifs, obj)로 가져온 obj를 활용하여 원하는 경로의 json 데이터를 가져오려고 포인터를 제어하였지만, 기존에 읽은 원본의 json
	// 데이터가 망가지기 때문에 obj_init에 넣어놓고 
	obj_Init = obj[tok[1]];
	i = 2;

	while (1)
	{
		if (tok[i] == "\0")
		{
			delete tok;
			break;
		}
		
		// 원하는 경로의 json 데이터까지 접근하기 위해 recursive로 포인터를 이어나가는 로직
		Json::Value tmp;
		tmp = obj_Init[tok[i]];
		obj_Init = tmp;
		i++;
	}

	if (pValue != NULL)
	{
		if (obj_Init[strName].isString() && strcmp(strRegTypeStr, pType) == 0)
		{
			string ctmp = obj_Init[strName].asString().c_str();
			strcpy((char*)pValue, ctmp.c_str());
		}
		else if (obj_Init[strName].isUInt() && strcmp(strRegTypeInt, pType) == 0)
		{
			int itmp = obj_Init[strName].asUInt();
			if (size >= sizeof(int))
				*(int*)pValue = itmp;
		}
		else
			return 0;
	}
	else
		return 0;

	return 1;
}

// ConfigJson 파일을 읽어 원하는 값을 가져오는 함수 + Error 났을 시 어떤 Error로 인하여 문제가 생겼는지 return 해주는 
extern "C" int _ReadConfigErr(char* pTarget, char* pName, char* pFlag, char* pType, void* pValue, unsigned long size)
{
	string strTarget(pTarget);
	string strName(pName);
	string Flag(pFlag);

	char strFlag[] = "KEY_WOW64_64KEY";
	char strRegTypeStr[] = "REG_SZ";
	char strRegTypeInt[] = "REG_DWORD";
	char strJsonPath[] = "/usr/local/fasoo/FEDConfig.json";
	ifstream ifs;

	ifs.open(strJsonPath);

	if (ifs.fail())
		return CONFIGFILE_OPEN_ERR;

	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj); // Reader can also read strings

	string stok = "\\";
	int     nCutPos;
	int     nIndex = 0;
	string* tok = new string[512];

	while ((nCutPos = strTarget.find_first_of(stok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			tok[nIndex++] = strTarget.substr(0, nCutPos);
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}

	if (strTarget.length() > 0)
	{
		tok[nIndex++] = strTarget.substr(0, nCutPos);
	}

	if (tok->empty())
		return CONFIGFILE_KEYPATH_ERR;

	Json::Value obj_Init;
	int i;

	bool b64 = false;
	char str64[] = "x86_64";
	struct utsname un;
	uname(&un);

	if (strcmp(str64, un.machine) == 0)
		b64 = true;
	else
		b64 = false;

	if (b64)
	{
		if (!Flag.empty())
		{
			if (strcmp(strFlag, Flag.c_str()) != 0)
				obj = obj["WOW6432Node"];
		}
		else
			obj = obj["WOW6432Node"];

	}

	obj_Init = obj[tok[1]];
	i = 2;

	while (1)
	{
		if (tok[i] == "\0")
		{
			delete tok;
			break;
		}
		Json::Value tmp;
		tmp = obj_Init[tok[i]];
		obj_Init = tmp;
		i++;
	}

	if (obj_Init[strName].empty())
		return CONFIGFILE_VALUENAME_NO_EXIST;

	if (pValue != NULL)
	{
		if (obj_Init[strName].isString() && strcmp(strRegTypeStr, pType) == 0)
		{
			string ctmp = obj_Init[strName].asString().c_str();
			strcpy((char*)pValue, ctmp.c_str());
		}
		else if (obj_Init[strName].isUInt() && strcmp(strRegTypeInt, pType) == 0)
		{
			int itmp = obj_Init[strName].asUInt();
			if (size >= sizeof(int))
				*(int*)pValue = itmp;
		}
		else
		{
			if (obj_Init[strName].isNull())
				return CONFIGFILE_VALUE_NO_EXIST;

			if (obj_Init[strName].isString() && strcmp(strRegTypeStr, pType) != 0)
				return CONFIGFILE_VALUETYPE_NO_SZ;

			if (obj_Init[strName].isUInt() && strcmp(strRegTypeInt, pType) != 0)
				return CONFIGFILE_VALUETYPE_NO_DWORD;
		}
	}
	else
		return CONFIGFILE_INPUTVALUE_ERR;

	return CONFIGFILE_SUCCESS;
}

// ConfigJson 파일을 읽어 Key에 존재하는 Value의 이름을 모두 가져올 수 있는 함수
extern "C" bool _GetConfQueryKey(char* pTarget, char* pFlag, int target_index, char* pKeyName)
{
	string strTarget(pTarget);
	string Flag(pFlag);

	char strFlag[] = "KEY_WOW64_64KEY";
	char strJsonPath[] = "/usr/local/fasoo/FEDConfig.json";
	ifstream ifs;

	ifs.open(strJsonPath);

	if (ifs.fail())
		return 0;

	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj); // Reader can also read strings

	string stok = "\\";

	int     nCutPos;
	int     nIndex = 0;
	string* tok = new string[512];

	while ((nCutPos = strTarget.find_first_of(stok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			tok[nIndex++] = strTarget.substr(0, nCutPos);
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}

	if (strTarget.length() > 0)
	{
		tok[nIndex++] = strTarget.substr(0, nCutPos);
	}

	if (tok->empty())
		return 0;

	Json::Value obj_Init;
	int i;

	bool b64 = false;
	char str64[] = "x86_64";
	struct utsname un;
	uname(&un);

	if (strcmp(str64, un.machine) == 0)
		b64 = true;
	else
		b64 = false;

	if (b64)
	{
		if (!Flag.empty())
		{
			if (strcmp(strFlag, Flag.c_str()) != 0)
				obj = obj["WOW6432Node"];
		}
		else
			obj = obj["WOW6432Node"];

	}

	obj_Init = obj[tok[1]];
	i = 2;

	while (1)
	{
		if (tok[i] == "\0")
		{
			delete tok;
			break;
		}

		Json::Value tmp;
		tmp = obj_Init[tok[i]];
		obj_Init = tmp;
		i++;
	}

	if (pKeyName != NULL)
	{
		int objsize = 0;
		auto member = obj_Init.getMemberNames();
		for (std::string s : member)
		{
			if (obj_Init[s].isObject())
			{
				if (objsize == target_index)
				{
					strcpy((char*)pKeyName, s.c_str());
					return 1;
				}
				objsize++;
			}
		}
	}

	return 0;
}

// ConfigJson 파일을 읽어 Key에 존재하는 Value 값을 모두 가져올 수 있는 함수 
extern "C" bool _GetConfQueryValue(char* pTarget, char* pFlag, int target_index, char* pValueName)
{
	string strTarget(pTarget);
	string Flag(pFlag);

	char strFlag[] = "KEY_WOW64_64KEY";
	char strJsonPath[] = "/usr/local/fasoo/FEDConfig.json";
	ifstream ifs;

	ifs.open(strJsonPath);

	if (ifs.fail())
		return 0;

	Json::Reader reader;
	Json::Value obj;
	reader.parse(ifs, obj); // Reader can also read strings

	string stok = "\\";

	int     nCutPos;
	int     nIndex = 0;
	string* tok = new string[512];

	while ((nCutPos = strTarget.find_first_of(stok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			tok[nIndex++] = strTarget.substr(0, nCutPos);
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}

	if (strTarget.length() > 0)
	{
		tok[nIndex++] = strTarget.substr(0, nCutPos);
	}

	if (tok->empty())
		return 0;

	Json::Value obj_Init;
	int i;

	bool b64 = false;
	char str64[] = "x86_64";
	struct utsname un;
	uname(&un);

	if (strcmp(str64, un.machine) == 0)
		b64 = true;
	else
		b64 = false;

	if (b64)
	{
		if (!Flag.empty())
		{
			if (strcmp(strFlag, Flag.c_str()) != 0)
				obj = obj["WOW6432Node"];
		}
		else
			obj = obj["WOW6432Node"];

	}

	obj_Init = obj[tok[1]];
	i = 2;

	while (1)
	{
		if (tok[i] == "\0")
		{
			delete tok;
			break;
		}

		Json::Value tmp;
		tmp = obj_Init[tok[i]];
		obj_Init = tmp;
		i++;
	}

	if (pValueName != NULL)
	{
		int valuesize = 0;
		auto member = obj_Init.getMemberNames();
		for (std::string s : member)
		{
			if (!obj_Init[s].isObject())
			{
				if (valuesize == target_index)
				{
					strcpy((char*)pValueName, s.c_str());
					return 1;
				}
				valuesize++;
			}
		}
	}

	return 0;
}
