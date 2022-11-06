#include <Wire.h>


void setup() {
 Wire.begin(8);                 /* 슬레이브의 자신의 주소 8 */
 Wire.onReceive(receiveEvent); /* 응답 이벤트 함수 설정 */
 Wire.onRequest(requestEvent); /* 요청 이벤트 함수 설정 */
 Serial.begin(115200);           /* 시리얼 통신 속도 설정 */
}

void loop() {
 delay(1000);
}

// 마스터의 요청 이벤트가 발생 할때 항상 호출되는 함수
void receiveEvent(int howMany) {
 String dstr = ""; 
 while (0 <Wire.available()) {    //메세지가 들어왔다면
    char c = Wire.read();      /* 1byte 읽음 */
    dstr= dstr+c;
 }
 String fir_num=dstr.substring(0,6);
 String snd_num=dstr.substring(6,10);
 String car_type1=dstr.substring(10,11);
 String car_type2=dstr.substring(11,12);
 String hour=dstr.substring(12,14);
 String minute=dstr.substring(14,16);
 String money=dstr.substring(16,20);
 
// Serial.println(fir_num);
// Serial.println(snd_num);
// Serial.println(car_type1);
// Serial.println(car_type2);
// Serial.println(hour);
// Serial.println(minute);
// Serial.println(money+"0");
 
}

// 마스터로 요청 메세지 작성 함수
void requestEvent() {
 Wire.write("Hello NodeMCU");  /*send string on request */
}
