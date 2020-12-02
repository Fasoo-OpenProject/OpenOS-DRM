# OpenOS DRM Interface

## Overview
개방형 OS (Linux OS)에서 동작하는 문서 저작 도구(Office 류의 Application)에 문서 권한 관리 솔루션(문서 DRM)을 적용 하기 위한 Interface이다.  
문서 저작 도구는 이 Interface를 이용, 문서 권한 관리 솔루션과 통신하여 문서 저작 도구에 문서 권한 관리 솔루션을 적용 한다.
* 문서 저작 도구는 여러 종류/회사의 문서 권한 관리 솔루션과 통신을 위해 매번 새로이 코드를 수정 할 필요 없이 단일 규격 Interface 한 번만 적용하면 되기 때문에 문서 권한 관리 솔루션 적용이 보다 쉬워 지고, 빨라진다.
* 여러 종류/회사의 문서 권한 관리 솔루션은 단일 규격의 Interface에 맞춰 문서 권한 관리 기능을 수행 하면 되기 때문에, 사용자 및 엔지니어는 동일 규격의 Application을 구축할 수 있게 되며, 동일 규격의 문서 권한 관리 시스템을 지원 할 수 있게 된다.
* Interface를 이용하기 때문에 기존 문서 권한 관리 솔루션에서의 단점이 충돌 및 Hooking 등으로 인해 낭비되는 시스템 리소스를 최소화 할 수 있다.

![OpenOS](https://user-images.githubusercontent.com/65989637/99944330-053b4400-2db6-11eb-9cd2-10b44bc4ddde.png)

## OpenOS DRM Interface 구성
OpenOS DRM Interface는 크게 문서 저작 도구 Application의 UI 관련 부분을 위한 'Document Interface'와, 파일의 입/출력 및 암호화된 파일의 정보 확인 등을 위한 'System Interface'로 나뉜다. 

* Document Interface
   * 사용자가 문서 저작 도구를 이용하여 문서를 사용 할 경우 필요한 이벤트 호출 (문서 저작 도구 > 문서 권한 관리 솔루션)
      * 파일 열기 전/후
      * 파일 저장 전/후
      * 문서에 다른 Content(다른 문서, 이미지 등) 삽입 전
      * 문서 미리보기 (인쇄 기능전 미리보기 화면 등)
      * 자동 저장 파일 저장 전
      * 임시 파일 생성 후
      * 임시 파일 삭제 후
      * 프로그램 실행 (초기화)
      * 매크로 실행 전
      * 메뉴/단축키 실행 전
   * 메뉴/단축키 제어
   * 암호화된 문서의 사용 가능 여부
* System Interface
   * 문서의 암호화 API
   * FILE 계열 C Standard API와 1:1 매칭 되는 FILE 관련 API Set
      * Open
      * Read
      * Write
      * close
      * Setting File Pointer
      * Getting File Size
   * DRM 문서의 정보
      * 암호화 된 문서의 권한 정보 확인
      * 암호화 된 문서의 소유 고객사 확인
      * 암호화 문서의 종류
      * 로그온 상태 정보
   * 사용 내역 발송
   * 에러 체크
* 상세 인터페이스는 [개방형 OS 문서 저작 도구 DRM Interface.doc](https://github.com/Fasoo-OpenProject/OpenOS-DRM/blob/master/DOC/%EA%B0%9C%EB%B0%A9%ED%98%95%20OS%20%EB%AC%B8%EC%84%9C%20%EC%A0%80%EC%9E%91%20%EB%8F%84%EA%B5%AC%20DRM%20Interface.docx) 참고

## 지원 환경
   * 개방형 OS 및 Linux 사에서 동작하는 문서 저작 도구 중, C/C++로 Interface 연동이 가능한 제품
   * OS
      * [TMAX OS 3](https://tmaxanc.com/#!/product-introduce/TmaxOS) 
   * 문서 저작 도구
      * [ToOffice 3](https://tmaxanc.com/#!/product-introduce/ToOffice)
   * 컴파일러
      * GCC/G++ 8.2.0

## 적용 프로세스
   * DRM Interface의 적용을 위해서는 반드시 파수와 먼저 협의 필요.
   * 인터페이스 문서에 권한에 대한 설명이 있지만, 문서 저자 도구(Office)의 기능이 Application 별로 특성이 있어 권한과 맞게 설정하기 위해 DRM 개발사와 사전확인 작업이 필요.
   * DRM 적용 프로세스는 아래와 같음.
      1. DRM 개발사 Contact
         * 테스트용 SSO 사용자 ID 전달 및 문의 사항 답변 
      1. DRM Interface API 설명 확인
         * 적용하려는 Application에 DRM Interface 및 DRM 적용이 가능 한가?
      2. Test DRM Client 패키지 설치
         * DRM Interface를 적용 하면서 또는 적용 후, 정상 동작 확인 
      3. DRM Interface 적용 (인터페이스 정의서 및 샘플코드 참조)
         * 인터페이스 정의서 및 샘플코드 참조
         * 이슈 게시판 등을 통해 문의
      4. 권한에 따라 Application이 제어가 되는지 확인 ( 적용자 / DRM 개발사 )
         * 잘못된 권한에 따른 DRM Interface 적용으로 보호 되어야 할 데이터가 유출 방지 차원 
      5. 적용 Application 출시
   

## 적용 가이드 (개발)
   * DRM Interface 연동 초기화
      * f_extadk.h를 소스에 포함(#include)
      * libfasooextadk.so 로드
      * 외부 노출 초기화 함수 실행
      * DRM Interface 획득
      * Interface를 이용하여 상호 통신
   * [인터페이스 정의서](https://github.com/Fasoo-OpenProject/OpenOS-DRM/blob/master/DOC/%EA%B0%9C%EB%B0%A9%ED%98%95%20OS%20%EB%AC%B8%EC%84%9C%20%EC%A0%80%EC%9E%91%20%EB%8F%84%EA%B5%AC%20DRM%20Interface.docx) API 참고, (샘플 코드 포함)
   * [샘플코드](https://github.com/Fasoo-OpenProject/OpenOS-DRM/tree/master/ExtADK)
   * Test DRM Client (TMAX 3.0), 'FasooDRM 패키저 적용하기' 참고
      * DRM Client 설치 및 동작의 간략한 확인
         *API를 이용하여 문서를 암호화 했을 때, SSO 로그인 창이 발생하면 DRM Client 설치 및 동작이 정상
      
## FasooDRM 패키저 적용하기
(1) TestPackage.zip 파일 압축 해제 

        unzip TestPackage.zip

(2) 압축 해제된 pkg-build-fasooDRM.deb 패키저 파일 설치

        sudo dpkg --install pkg-build-fasooDRM.deb
        sudo apt-get update
        sudo apt-get install -f


## 라이선스(License)
Copyright 2020 Fasoo

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
