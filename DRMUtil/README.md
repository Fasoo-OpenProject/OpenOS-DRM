DRM 기능 Utility입니다.
권한이 없는 경우, 동작 하지 않습니다.
반드시 문서에 권한이 있는지 확인하고 사용 하시기 바랍니다. 

[환경구성]
1. FasooDRM이 설치되어 있어야 합니다.
2. 본 툴을 직접 빌드할 시, ExtADK 폴더에 존재하는 f_extadk.h 파일이 있어야 합니다.

[MakeFile]
g++ -o DRMUtil DRMUtil.cpp -ldl

[Command]
DRMTestTool 문서경로 기능명 [옵션]
ex) ./DRMTestTool /home/fasoo/Desktop/test.docx islicensevalid edit

[기능 설명]
1. 암호화 (pack)
 - ./DRMTestTool /home/fasoo/Desktop/test.docx pack
 - 해당 문서를 암호화합니다.
 - 테스트 툴에 대해 서버에서 자동 암호화 옵션을 세팅해주어야 합니다.
 
2. 열람 (open)
 - ./DRMTestTool /home/fasoo/Desktop/test.docx open r+
 - 세 번쨰 파라미터의 경우 fopen에서 사용되는 FileMode와 동일한 사양입니다.
 
3. 암호화 파일 여부 확인 (issecure)
 - ./DRMTestTool /home/fasoo/Desktop/test.docx issecure
 - 해당 문서가 암호화 파일인지 확인합니다.
 
4. 권한 확인 (islicensevalid)
 - ./DRMTestTool /home/fasoo/Desktop/test.docx islicensevalid 권한
 - 해당 문서가 파라미터로 입력받은 권한을 가지고 있는지 확인합니다.
 - 현재는 view(열람) / edit(편집) / print(인쇄) / save(암호화해제) / extract(추출) 권한에 대해 확인할 수 있습니다.

5. 파일 읽기 (read)
 - ./DRMTestTool /home/fasoo/Desktop/test.docx read
 - 파일의 일부(50바이트)가 정상적으로 read되는지 확인합니다.
 
6. 파일 사이즈 확인 (size)
 - ./DRMTestTool /home/fasoo/Desktop/test.docx size
 - 파일의 사이즈를 확인합니다.
 
7. 암호화 해제 (unpack)
 - ./DRMTestTool /home/fasoo/Desktop/test.docx unpack
 - 암호화된 문서를 암호화 해제하여 일반 문서로 변경합니다.
