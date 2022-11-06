#include <AimHangul.h>
#include <Servo.h>
#include "SPI.h"
#include <Wire.h>

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 12
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST,TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);
#define TFT_CS2  7  // SS
#define TFT_RST2 6
TFT_22_ILI9225 tft2 = TFT_22_ILI9225(TFT_RST2,TFT_RS, TFT_CS2, TFT_SDI, TFT_CLK, TFT_LED);

String request_type;

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setOrientation(1);
  AimHangul_v2(tft,60,0,"어서오십시오",COLOR_CYAN);
  String name[3]={"차번호","입차시간","시간당금액"};
  display(tft,name);
  total_money(tft,"1000");
  tft2.begin();
  tft2.setOrientation(1);
  AimHangul_v2(tft2,60,0,"안녕히가십시오",COLOR_CYAN);
  String name2[3]={"차번호","입차시간","지불할금액"};
  display(tft2,name2);
  
  Wire.begin(8);   
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}


void loop() {
 delay(1000);
}



void display(TFT_22_ILI9225 tft, String name[]){
  for (int i = 0; i<3;i++){
    int sum=45*(i+1);
    AimHangul_v2(tft,5,sum,name[i],COLOR_WHITE);
  } 
  for (int i=1;i<4;i++){
    tft.setFont(Terminal12x16);
    int x=35+18*i;
    int y=5+45*i;
    tft.drawText(x, y, ":");
  }

}
void intro_time(TFT_22_ILI9225 tft, String time) {
  String h=time.substring(0,2);
  String m=time.substring(2,4);
  tft.setFont(Terminal12x16);
  tft.drawText(90,95,h,COLOR_WHITE);
  tft.drawText(115,95,":",COLOR_WHITE);
  tft.drawText(125,95,m,COLOR_WHITE);
}
void car_num(TFT_22_ILI9225 tft,String fir_num,String snd_num) {
  String n1;
  if(fir_num.charAt(0)=='0'){
    n1=fir_num.substring(1,3);
  }else{n1=fir_num.substring(0,3);}

  String s1=fir_num.substring(3,6);
  tft.setFont(Terminal12x16);
  tft.drawText(70,50,n1,COLOR_GREEN);
  tft.drawText(140,50,snd_num,COLOR_GREEN);
  AimHangul(tft,120,50,s1,COLOR_GREEN);
}

void total_money(TFT_22_ILI9225 tft, String m) {
  String m1;
  if(m.charAt(0)=='0'){
    m1=m.substring(1,5);
  }else{m1=m.substring(0,5);}
  AimHangul_v2(tft,170,135,"원",COLOR_WHITE);
  tft.setFont(Terminal12x16);
  tft.drawText(100,140,m1);
}


void receiveEvent(int howMany) {
 String dstr="";
 while (0 <Wire.available()) {    //메세지가 들어왔다면
    char c = Wire.read();      /* 1byte 읽음 */
    dstr=dstr+c;
 }
    String fir_num=dstr.substring(0,6);
    String snd_num=dstr.substring(6,10);
    String car_type1=dstr.substring(10,11);
    String car_type2=dstr.substring(11,12);
    String i_time=dstr.substring(12,16);
    String money=dstr.substring(16,20);
//    Serial.println(fir_num);
    car_num(tft,fir_num,snd_num);
    intro_time(tft,i_time);
    intro_time(tft2,i_time);
    car_num(tft2,fir_num,snd_num);
    total_money(tft2,money+"0");
  
}
// 마스터로 요청 메세지 작성 함수
void requestEvent() {
  Wire.write("seccese!");  /*send string on request */
}
