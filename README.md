# SlotMachine-Using-achro-em
라즈베리파이와 achro-em 보드를 활용한 슬롯머신 게임 구현

기본기능
--

Dot matrix : 1부터 9까지 랜덤한 숫자를 번갈아가며 출력후 멈춤

FND : Dot matrix 에서 멈춘 숫자를 차례대로 출력

Text LCD : 환영 , 성공  , 실패 메시지 출력

Step Motor : 슬롯머신이 돌아가는 모양을표현

Dip Switch : 슬롯머신 시작기능

부저 : 슬롯머신이 끝나면 팡파레 및 우울한 효과음 발생

LED : 슬롯머신이 끝나면 화려한 점등효과 발생 , 시작을 알리는 점등효과


기본기능 출력화면
--
### 시스템 시나리오 
<img src="https://user-images.githubusercontent.com/38147253/71610696-90c44900-2bd6-11ea-9786-e11e3b10c61e.jpg"></img>

### 시작화면
<img src="https://user-images.githubusercontent.com/38147253/71610723-ca954f80-2bd6-11ea-841f-11cf3fe0c38a.png"></img>
### 주요 개발 이슈 & 방법

(1) 쓰레드를 사용해서 동시에 여러 개의 디바이스들을 제어

(2) 모듈의 결합 및 테스트코드 작성을 위해 다른 조들의 테스트코드 리뷰

(3) 랜덤숫자를 표현하기 위해 Dot-matrix와 rand() 함수의 연동 코드 작성

(4) 성공과 실패를 표현하기 위해 부저출력의 빈도를 바꿔서 2가지로 표현

(5) Dot-matrix 에 표시된 숫자를 FND에 표현하는 코드 작성

(6) 성공시 시각표현을 부각하기위해 LED에 다양한 패턴을 주어 화려한 효과 표현


### 기본 출력화면

<img src="https://user-images.githubusercontent.com/38147253/71610753-f9abc100-2bd6-11ea-83bc-4f39de64fc3e.png"></img> <br>

<img src="https://user-images.githubusercontent.com/38147253/71610765-15af6280-2bd7-11ea-8d77-e30d6011aee2.png"></img>


성공화면

<img src="https://user-images.githubusercontent.com/38147253/71610775-311a6d80-2bd7-11ea-80b1-831289bfe58d.png"></img>

실패화면

<img src="https://user-images.githubusercontent.com/38147253/71610783-4c857880-2bd7-11ea-8edd-c5b3b562f424.png"></img>

역할분담
--
<table>
	<th>이름</th>
	<th>내용</th>
	<tr>
	    <td>문준혁</td>
	    <td>FND 기능 구현, Text lcd 기능 구현</td>
	</tr>
	<tr>
	    <td>박현욱</td>
	    <td>Step Moter 기능 구현, Dot matrix 기능 구현, Step motor 기능 구현, Qt GUI 구현</td>
	</tr>
	<tr>
	    <td>이세민</td>
	    <td>발표 및 PPT 제작, Buzzer 기능 구현, Push Switch기능 구현</td>
	</tr>
	<tr>
	    <td>이왕규</td>
	    <td>LED 기능 구현, Qt GUI 구현</td>
	</tr>
	
</table>
	
	
	
참고자료
--
소스코드: <br>
 1)“휴인스 공식홈페이지”, 검색어 : 휴인스 achro-EM_kit V1.2 <br>
http://www.huins.com/new/sub/goods_view.php?it_id=1529394147&ca_id=70&ca_id2=70&n=1&sn=5 <br>
https://www.winpcap.org/docs/docs_412/html/main.html<br>
https://www.binarytides.com/code-packet-sniffer-c-winpcap/<br>
https://toughrogrammer.tistory.com/165<br><br>



--
