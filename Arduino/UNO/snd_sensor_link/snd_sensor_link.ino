#include <Wire.h>
#include <Ultra.h>
#include <LedControl.h>
#include <Servo.h>

Ultra ultra1(7, 6);
Ultra ultra2(5, 4);
LedControl Dote(10, 8, 9, 2);
Servo myservo1;
Servo myservo2;
const int servo_pin1 = 3;
const int servo_pin2 = 2;
int b =2;
int request_type;

byte digits[8] = {
  B00001000,
  B00010100,
  B11001000,
  B10001111,
  B10001000,
  B10001111,
  B01000001,
  B00111110,
};

void setup() {
  Wire.begin(7);                /* 슬레이브의 자신의 주소 8 */
  Wire.onReceive(receiveEvent); /* 응답 이벤트 함수 설정 */
  Wire.onRequest(requestEvent); /* 요청 이벤트 함수 설정 */
  // Serial.begin(115200);           /* 시리얼 통신 속도 설정 */
  myservo1.attach(servo_pin1);
  myservo2.attach(servo_pin2);
  Dote.shutdown(0, false);
  Dote.shutdown(1, false);
  Dote.setIntensity(0, 5);
  Dote.setIntensity(1, 5);
  Dote.clearDisplay(0);
  Dote.clearDisplay(1);

}

void loop() {
  int distance1 = ultra1.getDistance();
  int distance2 = ultra2.getDistance();

  for (int i = 0; i < 8; i++) {
    Dote.setRow(0, i, digits[i]);
    Dote.setRow(1, i, digits[i]);
  }
  
  if (distance1 < 10) {
    Dote.shutdown(0, true);
  } else {
    Dote.shutdown(0, false);
  }

  if (distance2 < 10) {
    Dote.shutdown(1, true);
  } else {
    Dote.shutdown(1, false);
  }

  if (distance1<10 and distance2<10){b=0;}
  else if(distance1<10){b=1;}
  else if(distance2<10){b=1;}
  else{b=2;}

}

// 마스터의 요청 이벤트가 발생 할때 항상 호출되는 함수
void receiveEvent(int howMany) {
 while (0 <Wire.available()) {    //메세지가 들어왔다면
    request_type = Wire.read();      /* 1byte 읽음 */
    
    // if (request_type==2){
    //   myservo1.write(90);
    // }else if(request_type==3){
    //   myservo1.write(0);
    // }
    // if (request_type==4){
    //   myservo2.write(90);
    // }else if(request_type==5){
    //   myservo2.write(0);
    // }
    
  }

}

// 마스터로 요청 메세지 작성 함수
void requestEvent() {
  if (request_type==1){
    char buff[3];
    dtostrf(b,1,0,buff); //원래 실수임 을 char 배열형태로 만들기
    Wire.write(buff);
  }
//  Wire.write("Hello NodeMCU");  /*send string on request */
}
