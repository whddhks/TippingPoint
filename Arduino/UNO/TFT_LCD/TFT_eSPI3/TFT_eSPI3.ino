#include <AimHangul.h>
#include "SPI.h"
#include <Wire.h>

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 12
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST,TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);
String dstr;

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setOrientation(1);
  AimHangul_v2(tft,60,0,"안녕히가십시오",COLOR_CYAN);
  String name[3]={"시간","차번호","주차금액"};
  display(tft,name);
  tft.setFont(Terminal12x16);
  tft.drawText(185,140,"W");
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}


void loop() {
 if (dstr!=""){
    delay(2000);
    tft.clear();
    AimHangul_v2(tft,60,0,"안녕히가십시오",COLOR_CYAN);
    String name[3]={"시간","차번호","주차금액"};
    display(tft,name);
    tft.setFont(Terminal12x16);
    tft.drawText(185,140,"W");
    dstr="";
  }
 
}


void display(TFT_22_ILI9225 tft, String name[]){
  for (int i = 0; i<3;i++){
    int sum=45*(i+1);
    AimHangul_v2(tft,5,sum,name[i],COLOR_WHITE);
  } 
  for (int i=1;i<4;i++){
    tft.setFont(Terminal12x16);
    int x=30+18*i;
    int y=5+45*i;
    tft.drawText(x, y, ":");
  }

}
void intro_time(TFT_22_ILI9225 tft, String time) {
  tft.setFont(Terminal12x16);
  tft.drawText(110,50,time,COLOR_WHITE);
}
void car_num(TFT_22_ILI9225 tft,String fir_num,String snd_num) {
  String n1;
  if(fir_num.charAt(0)=='0'){
    n1=fir_num.substring(1,3);}
  else{
    n1=fir_num.substring(0,3);}

  String s1=fir_num.substring(3,6);
  tft.setFont(Terminal12x16);
  tft.drawText(80,95,n1,COLOR_GREEN);
  tft.drawText(145,95,snd_num,COLOR_GREEN);
  AimHangul(tft,125,95,s1,COLOR_GREEN);
}

void total_money(TFT_22_ILI9225 tft, String m) {
  String m1;
  if(m.charAt(0)=='0'){
    m1=m.substring(1,5);
  }else{m1=m.substring(0,5);}
  
//  AimHangul(tft,180,140,"원",COLOR_WHITE);
  tft.setFont(Terminal12x16);
  tft.drawText(100,140,m1+"0");
}

//0550288파10122505000


void receiveEvent(int howMany) {
 dstr="";
 while (0 <Wire.available()) {
    char c = Wire.read();      
    dstr=dstr+c;
 }
    String fir_num=dstr.substring(4,10);
    String snd_num=dstr.substring(0,4);
    String i_time=dstr.substring(10,15);
    String money=dstr.substring(15,19);
    intro_time(tft,i_time);
    car_num(tft,fir_num,snd_num);
    total_money(tft,money);
}


void requestEvent() {
  Wire.write("");  /*send string on request */
}
