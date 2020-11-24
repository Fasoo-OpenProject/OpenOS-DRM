#ifndef __ConfigJson_H__
#define __ConfigJson_H__

#include <iostream> 
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/utsname.h>

#define CONFIGFILE_SUCCESS					0	// ����
#define CONFIGFILE_OPEN_ERR					1	// FEDConfig.json ���� ���� ���� 
#define CONFIGFILE_KEYPATH_ERR				2	// Target ��ο� �ش� Key�� �������� ���� 
#define CONFIGFILE_VALUENAME_NO_EXIST		3	// �ش� Key�� �����ϰ��� �ϴ� Value Name�� �������� ����
#define CONFIGFILE_VALUE_NO_EXIST			4	// �ش� Value Name�� ���� ����
#define CONFIGFILE_VALUETYPE_NO_SZ			5	// ������ Value Type REG_SZ (���ڿ�)�� �ƴ� 
#define CONFIGFILE_VALUETYPE_NO_DWORD		6	// ������ Value Type REG_DWORD (����)�� �ƴ� 
#define CONFIGFILE_INPUTVALUE_ERR			7	// Output Value �Ҵ��� �߸��Ͽ��� 

// Config ���� ��: FEDConfig.json
// Config ���� ��ġ: /volume/c:/Program Files (x86)/Fasoo DRM/FEDConfig.json

extern "C"
{
	bool _ReadConfig(char* pTarget, char* pName, char* pFlag, char* pType, void* pValue, unsigned long size);
	// 1st �Ķ���� pTarget: �����ϰ��� �ϴ� ������Ʈ�� Key ��� ex) "Software\Fasoo DRM\Policy"

	// 2nd �Ķ���� pName: �����ϰ��� �ϴ� ������Ʈ�� Key������ �� �̸� ex) Default

	// 3rd �Ķ���� pFlag: Default�� �� ���ڿ�. (64bit ȯ���϶�, RegQueryValue�� Flag�� KEY_WOW64_64KEY�� �־ ����ϴ� ���� ������ �߰�) ex) ������ pfnRegOpenKeyExW(HKEY_LOCAL_MACHINE, wcsCrypto, 0, KEY_READ | KEY_WOW64_64KEY, &hKey)�� ���ؼ� ����Ͽ��ٸ� "KEY_WOW64_64KEY" �Է�
	// * �߰�����: 64bit ȯ�濡�� ��ǻ��\HKEY_LOCAL_MACHINE\SOFTWARE\Fasoo DRM ������ Key-value�� �������� ������ Ȱ�� 
	// ������ RegQueryValue �Լ��� ���ؼ� ������ �� 4��° �Ķ���� Flage ���� KEY_WOW64_64KEY�� �������� �����ϸ� ��

	// 4th �Ķ���� pType: �����ϰ����ϴ� ������Ʈ�� Value�� Type - (1) REG_SZ - ���ڿ� (2) REG_DWORD - ����
	// 4th �Ķ���� pValue: �����ϰ����ϴ� ������Ʈ�� Key������ Value ex) 0100000000001001
	// 5th �Ķ���� size: �����ϰ����ϴ� ������Ʈ�� Key������ Value ũ��

	// ��� ����1 - REG_SZ ������Ÿ�� �� ������������ ����
	// char* strValue = (char*)malloc(sizeof(char) * 1028 + 1);
	//	bool bResult = _ReadConfig("Software\\Fasoo DRM\\Policy", "Default", "", "REG_SZ", strValue, sizeof(char)*1028+1);

	// ��� ����2 - REG_DWORD ������Ÿ�� �� ������������ ����
	// 	int strValue = 0;
	//	bool bResult = _ReadConfig("Software\\Fasoo DRM", "InstallPath", "KEY_WOW64_64KEY", "REG_DWORD", &strValue, sizeof(int));

	int _ReadConfigErr(char* pTarget, char* pName, char* pFlag, char* pType, void* pValue, unsigned long size);
	// ���� _ReadConfig �Լ��� �Ķ���ʹ� ����.
	// Return ���� Error Code Return (0�� ����)

	bool _GetConfQueryKey(char* pTarget, char* pFlag, int target_index, char* pKeyName);
	// 1st �Ķ���� pTarget: �����ϰ��� �ϴ� ������Ʈ�� Key ��� ex) "Software\Fasoo DRM\Policy"
	// 2nd �Ķ���� pFlag: Default�� �� ���ڿ�. (64bit ȯ���϶�, RegQueryValue�� Flag�� KEY_WOW64_64KEY�� �־ ����ϴ� ���� ������ �߰�) ex) ������ pfnRegOpenKeyExW(HKEY_LOCAL_MACHINE, wcsCrypto, 0, KEY_READ | KEY_WOW64_64KEY, &hKey)�� ���ؼ� ����Ͽ��ٸ� "KEY_WOW64_64KEY" �Է�
	// * �߰�����: 64bit ȯ�濡�� ��ǻ��\HKEY_LOCAL_MACHINE\SOFTWARE\Fasoo DRM ������ Key-value�� �������� ������ Ȱ�� 
	// ������ RegQueryValue �Լ��� ���ؼ� ������ �� 4��° �Ķ���� Flage ���� KEY_WOW64_64KEY�� �������� �����ϸ� ��
	// 3rd �Ķ���� target_index: �����ϰ����ϴ� ������Ʈ�� Key Index
	// 4th �Ķ���� pKeyName: �����ϰ����ϴ� ������Ʈ�� Key Name

	//	��� ����
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
	// Ȱ�� ���̵�� ���� �Լ� _GetConfQueryKey�� ��ġ (����, Target�� Key�� �ƴ� Value�� �ٲ� ��)

}
#endif //__ConfigJson_H__