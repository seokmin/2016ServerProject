# 2016ServerProject
프로젝트명 : 잭블랙잭블랙잭블랙잭블랙잭블랙잭블랙잭  
다들 아는 그 카드게임  

## 패킷 구조
/common/packet.h
먼저 찜한사람이 수정하고 common폴더에 추가합시당  
[링크(구글닥스)](https://docs.google.com/spreadsheets/d/13zOo826LMDAvX_KQ-mffD9NehRGVJEXf8I36mpPwZKs/edit)  

## 코딩 규칙  
- 공통
Common폴더의 구현은 전부 namespace COMMON에 등록.  

- (채널)
struct의 멤버는 _로 시작  
class의 멤버는 m_로 시작  
우리가 구현한 메서드는 대문자로 시작  
대충 컴파일되게만 해놓고 세부 구현 안한곳은 ```// [TODO] 구현할 내용``` 입력해놓기
