#include <Wire.h>
#include <Ultra.h>
#include <LedControl.h>
#include <Servo.h>
#include <Switch.h>

Switch btn(11);
Ultra ultra1(7, 6);
Ultra ultra2(5, 4);
LedControl Dote(10, 8, 9, 2);
Servo servo1;
Servo servo2;
const int servo_pin1 = 3;
const int servo_pin2 = 2;
int b;
int request_type;
int c;
long time;
int v;

//byte digits[8] = {
//  B00001000,
//  B00010100,
//  B11001000,
//  B10001111,
//  B10001000,
//  B10001111,
//  B01000001,
//  B00111110,
//};
byte digits[8] = {
  B01111100,
  B10000010,
  B11110001,
  B00010001,
  B11110001,
  B00010011,
  B00101000,
  B00010000};
void btn_open() {
  v=1;
  servo2.write(90);
  time=millis();
}
void setup() {
  Wire.begin(7);                /* 슬레이브의 자신의 주소 8 */
  Wire.onReceive(receiveEvent); /* 응답 이벤트 함수 설정 */
  Wire.onRequest(requestEvent); /* 요청 이벤트 함수 설정 */
  Serial.begin(115200);           /* 시리얼 통신 속도 설정 */
  servo1.attach(servo_pin1);
  servo2.attach(servo_pin2);
  Dote.shutdown(0, false);
  Dote.shutdown(1, false);
  Dote.setIntensity(0, 5);
  Dote.setIntensity(1, 5);
  Dote.clearDisplay(0);
  Dote.clearDisplay(1);
  servo1.write(0);
  servo2.write(0);
  btn.setCallback(btn_open);
}

void loop() {
  
  btn.check();
  int distance1 = ultra1.getDistance();
  int distance2 = ultra2.getDistance();

  for (int i = 0; i < 8; i++) {
    Dote.setRow(0, i, digits[i]);
    Dote.setRow(1, i, digits[i]);
  }
  
  if (distance1 < 10) {
    Dote.shutdown(0, true);
    servo1.write(0);
  } else {
    Dote.shutdown(0, false);
  }

  if (distance2 < 10) {
    Dote.shutdown(1, true);
      servo2.write(0);
  } else {
    Dote.shutdown(1, false);
  }

  if (distance1<10 and distance2<10){b=0;}
  else if(distance1<10){b=1,c=1;}
  else if(distance2<10){b=1,c=1;}
  else{b=2,c=0;}

  if (v==1){
    long current_time=millis();
    if(current_time-time>=5000){
      servo2.write(0);
      v=0;
    }
  }
}

// 마스터의 요청 이벤트가 발생 할때 항상 호출되는 함수
void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    request_type = Wire.read();
    Serial.println(request_type);
    if (request_type==2){
      servo1.write(90);
      servo2.write(90);
    }
  }
}
void requestEvent() {
  if (request_type==1){
    char buff[3];
    dtostrf(b,1,0,buff);
    Wire.write(buff);
    char buff2[3];
    dtostrf(c,1,0,buff2);
    Wire.write(buff2);
    
  }
}
