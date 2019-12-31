# SlotMachine-Using-achro-em
라즈베리파이와 achro-em 보드를 활용한 슬롯머신 게임 구현

기본기능
--
Application Layer
* HTTP, P2P, DNS

Transport Layer(TCP / UDP)
* Source Port, Destination Port
* (TCP) Sequence number, Acknowledgment number

Network Layer
* Source IP, Destination IP

* 패킷 길이, Source MAC address, Destination MAC address, EtherType, Header 길이, TTL

기본기능 출력화면
--
### 네트워크 카드 선택
<img src="https://user-images.githubusercontent.com/37360089/70724357-ce535600-1d3d-11ea-96c2-1b99771d1988.png"></img>

### 필터링
<img src="https://user-images.githubusercontent.com/37360089/70724401-e4611680-1d3d-11ea-896f-13df04daeafa.png"></img>

### 기본 출력화면
<img src="https://user-images.githubusercontent.com/37360089/70724391-dc08db80-1d3d-11ea-90aa-e28e8c804f2e.png"></img> <br>

<img src="https://user-images.githubusercontent.com/37360089/70725416-aebd2d00-1d3f-11ea-8e7d-9aee8bb4b99c.png"></img>

HTTP
--
포트번호가 80 -> http, 443 -> https 
<div>
	<img src="https://user-images.githubusercontent.com/37360089/70724568-2ee29300-1d3e-11ea-822a-01a2b4466149.png"></img>

<img src="https://user-images.githubusercontent.com/37360089/70724588-34d87400-1d3e-11ea-9cfd-99184eda7dcf.png"></img>

</div>

P2P
--
torrent 사용, ip 주소로 확인
<div>
	<img src="https://user-images.githubusercontent.com/37360089/70724854-b4664300-1d3e-11ea-8112-28baa5947a1f.png"></img>
	<img src="https://user-images.githubusercontent.com/37360089/70725028-05763700-1d3f-11ea-88f4-906ee42582e0.png"></img>
</div>

DNS
--
포트번호 53번
Transaction_ID와 DomainName을 출력

<img src="https://user-images.githubusercontent.com/37360089/70726269-2d669a00-1d41-11ea-8dbd-8ca9effb417e.png"></img>

FTP
--
### FileZilla Server 화면
<img src="https://user-images.githubusercontent.com/37360089/70725503-d9a78100-1d3f-11ea-8773-7c4c430744da.png"></img>

### 접속하려는 컴퓨터 cmd 화면
<img src="https://user-images.githubusercontent.com/37360089/70725578-00fe4e00-1d40-11ea-95fa-ef91ae640e35.png"></img>

### 콘솔 출력 화면 
<div>
<img src="https://user-images.githubusercontent.com/37360089/70725795-5f2b3100-1d40-11ea-81c4-34f09cc250ed.png"></img>

<img src="https://user-images.githubusercontent.com/37360089/70725814-66ead580-1d40-11ea-845d-1f5c730a0d34.png"></img>

<img src="https://user-images.githubusercontent.com/37360089/70725830-6e11e380-1d40-11ea-98cc-605ca22b4c73.png"></img>

<img src="https://user-images.githubusercontent.com/37360089/70725839-72d69780-1d40-11ea-9962-6b04d432ea86.png"></img>

<img src="https://user-images.githubusercontent.com/37360089/70725874-81bd4a00-1d40-11ea-924d-dbb6bd74ec93.png"></img>

<img src="https://user-images.githubusercontent.com/37360089/70725896-897cee80-1d40-11ea-9eaa-f1c662eb1d13.png"></img>
</div>


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
