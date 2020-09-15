# Microprocessor-Application-Design
### Overview
* Achro I.MX6Q Hardware <br>
<img src="https://user-images.githubusercontent.com/70748985/93182691-f4969e00-f774-11ea-8a5b-eee9adbd0b5d.png" width="40%">

   * I/O Board <br>
      <img src="https://user-images.githubusercontent.com/70748985/93183290-c6658e00-f775-11ea-8725-8b66577e994b.png" width="30%">
      * LED <br>
      물리 주소 0x0800_0016에 쓰는 값에 따라 binary encoding으로 LED들이 켜짐 <br>
      <img src="https://user-images.githubusercontent.com/70748985/93184064-c1eda500-f776-11ea-8060-3e3df04367bf.png" width="30%"> <br>
      * Dot matrix <br>
      <img src="https://user-images.githubusercontent.com/70748985/93185007-e007d500-f777-11ea-8019-4458af05ce75.png" width="40%"> <br>
         * Adress Map <br>
         <img src="https://user-images.githubusercontent.com/70748985/93185059-f01fb480-f777-11ea-988d-6e27f5739e3b.png" width="40%"> <br>
         * 출력 방법 <br> 
         <img src="https://user-images.githubusercontent.com/70748985/93185117-00379400-f778-11ea-9e59-82359efcbe15.png" width="40%"> <br>
         * 출력 예시 <br>
         <img src="https://user-images.githubusercontent.com/70748985/93185195-12193700-f778-11ea-8be6-0ff8f37a469a.png" width="40%"> <br>
      * Push Switch <br>
      <img src="https://user-images.githubusercontent.com/70748985/93184512-4c360900-f777-11ea-94a3-b135212725cd.png" width="30%"> <br>
      
<hr>

#### lab1_module
모듈 컴파일 시 출력되는 경고 메시지 없애기

<hr>

#### lab1_simple_device_driver
디바이스 드라이버 적재, 제거시 메시지 출력

<hr>

#### lab2_led
* 1-255 사이의 숫자를 명령어의 인자로 입력 받아 이진수에 해당하는 LED 점등
* LED port로부터 값을 읽어 출력 <br>
<img src="https://user-images.githubusercontent.com/70748985/93183475-fe6cd100-f775-11ea-8c99-af2f29fa6150.png" width="30%"> <br>

<hr>

#### lab3_push_switch  
push switch 9개의 값을 주기적으로 읽어 들여 현재 어떤 스위치가 눌렸는지 저장하고 그 값을 화면에 출력

<hr>

#### lab4_dot 
첫번째 인자로 0-9 사이의 정수를 인자로 받아 dot matrix에출력

<hr>

#### lab5_tetris  
dot matrix와 push switch를 이용하여 만든 tetris
