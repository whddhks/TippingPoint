#include <Wire.h>  //통신과 관련된 라이브러리
#include <MiniCom.h>

MiniCom com;
#define NOMAL_CAR 0
#define DISABLE_CAR 1
#define PERSON 2



void setup() {
  com.init();
  Serial.begin(115200); /* 시리얼 통신 속도 */
  Wire.begin(D1, D2); /* SDA=D1 and SCL=D2 of NodeMCU */
}

void loop() {

  com.run();
  //Database
  String sig="";
  while(Serial.available()){
    char wait = Serial.read();
    sig=sig+wait;
  }
  
  String person_type=sig.substring(1,2);
  String car_info=sig.substring(3,23);

  
  //Slave 6
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
  

  //Slave 7
  Wire.beginTransmission(7);
  if (person_type=="1"){
      Wire.write(2);
  }
  Wire.write(1);
  Wire.endTransmission();
  String dstr2= "";
  Wire.requestFrom(7, 13); 
  while(Wire.available()){
    char c1 = Wire.read(); 
    dstr2=dstr2+c1;
  }
  if(dstr2.charAt(1)=='1'){
    String illegal_car="illegal";
    Serial.println(illegal_car);
  }
    int b =dstr2.substring(0,1).toInt();
    com.print(1,"Disable_Car=>",b);
    
    delay(500);

  
  


  //Slave 8,9
  if (car_info != ""){
    char buff[100];
    car_info.toCharArray(buff,car_info.length());
    if(sig.charAt(0)=='0'){
      Wire.beginTransmission(8);
      Wire.write(buff);
      Wire.endTransmission();
      Wire.requestFrom(8, 13);
      
    }else if(sig.charAt(0)=='1'){
      Wire.beginTransmission(9);
      Wire.write(buff);
      Wire.endTransmission();
      Wire.requestFrom(9, 13);
    }
    while(Wire.available()){
      char c2 = Wire.read();
    }
    car_info="";
    buff[100]={};
    }
    delay(500);
  
}
