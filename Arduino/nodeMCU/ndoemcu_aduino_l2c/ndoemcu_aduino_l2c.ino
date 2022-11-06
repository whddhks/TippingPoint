#include <Wire.h>  //통신과 관련된 라이브러리
#include <MiniCom.h>

MiniCom com;
#define NOMAL_CAR 0
#define DISABLE_CAR 1
#define INTRO_OPEN 2
#define INTRO_CLOSE 3
#define OUTRO_OPEN 4
#define OUTRO_CLOSE 5


void setup() {
  com.init();
  Serial.begin(115200); /* 시리얼 통신 속도 */
  Wire.begin(D1, D2); /* SDA=D1 and SCL=D2 of NodeMCU */

}

void loop() {
  com.run();

  Wire.beginTransmission(6); /* 슬래이브 주소번호 8 */
  Wire.write(0);  /* 슬레이브에게 보내는 메세지 */
  Wire.endTransmission();/* 전송 */
  String dstr= "";
  Wire.requestFrom(6, 13); /* 슬래이브 8번으로 부터 데이터 13byte 받음 */
  while(Wire.available()){  //데이터가 들어왔다면
    char c = Wire.read();  //1byte  씩 읽음
    dstr=dstr+c;
//  Serial.println(c);         //1byte 시리얼 모니터 창에 출력> 개행이 아니라 붙어서나옴 
  }
  int a= dstr.toInt();
  com.print(0,"<=Nomal_Car: ",a);
  delay(500);
  
  Wire.beginTransmission(7);
  Wire.write(1);
  Wire.endTransmission();
  String dstr2= "";
  Wire.requestFrom(7, 13); 
  while(Wire.available()){
    char c1 = Wire.read(); 
    dstr2=dstr2+c1;
  }
  int b = dstr2.toInt();                         
  com.print(1,"Disable_Car=>",b);        //
  delay(500);

  String car_info="000가000000122502000";
    if (car_info != ""){
      char buff[100];
      car_info.toCharArray(buff,car_info.length());
      Wire.beginTransmission(8);
      Wire.write(buff);
      Wire.endTransmission();
      Wire.requestFrom(8, 13);
      while(Wire.available()){
        char c2 = Wire.read();
        Serial.print(c2);
        car_info="";
        buff[100]={};
      }
      Serial.println();
  }
  delay(1000);
}
