# 잭블랙이블랙잭을한다 - ProtoType Release 0.1

##개요

누구나 아는 그 게임 : 블랙잭

멀티 채널 지원, 빠른 입장, 한글 닉네임, 칩 무료충전

###클라이언트

Cocos2d-x 3.12 기반

타겟 플랫폼 - Windows PC (추후 Android 지원 예정)

###서버

로그인 서버 : C#_Asp.Net 기반

채널 서버 : C++, IOCP 기반

##How to Build

###클라이언트
    1. cocos2d-x 3.12버전을 sogalbi\cocos2d 폴더에 풀어주세요
      http://www.cocos2d-x.org/filedown/cocos2d-x-3.12.zip
    2. 압축을 풀었으면 sogalbi\cocos2d\download-deps.py를 실행하여 추가 구성요소를 다운 받아 주세요.
    3. sogalbi\proj.win32\sogalbi.sln 파일을 visual studio 2015 버전으로 열어서 작업하시면 됩니다.
    4. 이상의 내용은 sogalbi 폴더 안의 "cocos2d폴더는 알아서.txt"에도 적혀있습니다.

###로그인 서버
    1. 로그인 서버 sln 파일을 열어 빌드합니다.
    2. 실행 시 exe 파일과 같은 폴더 내에 LoginServerConfig.xml 파일이 필요합니다.
    3. LoginServerConfig.xml 파일 구성 형식은 로그인 서버 폴더 내를 참고하세요.
    4. 실행 시 관리자 권한을 필요로 합니다.

### 채널 서버
    1. 그냥 빌드하면 됩니다.
    2. 실행 시 exe 파일과 같은 폴더 내에 config.ini 파일이 필요합니다.
    3. config.ini 파일 구성 형식은 채널 서버 폴더 내를 참고하세요.

##스크린샷

![image](/sc1.png)
![image](/sc2.png)
![image](/sc3.png)
