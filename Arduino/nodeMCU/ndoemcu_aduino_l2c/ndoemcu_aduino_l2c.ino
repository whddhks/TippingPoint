#include <Wire.h>  //통신과 관련된 라이브러리
#include <MiniCom.h>

MiniCom com;
#define NOMAL_CAR 0
#define DISABLE_CAR 1
#define PERSON 2



void setup() {
  com.init();
  Serial.begin(115200);
  Wire.begin(D1, D2); 
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
  Wire.beginTransmission(6); 
  Wire.write(0); 
  Wire.endTransmission();
  String dstr= "";
  Wire.requestFrom(6, 13);
  while(Wire.available()){
    char c = Wire.read();  
    dstr=dstr+c;
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
