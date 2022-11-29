#include <Wire.h>
#include <Ultra.h>
#include <LedControl.h>

Ultra ultra1(3,2);
Ultra ultra2(5,4);
Ultra ultra3(7,6);
LedControl Dote(13,11,12,3);

int a=3;
int request_type;

byte digits[8] = 
{ 
  B01111100,
  B10001010,
  B10010001,
  B10111101,
  B10001001,
  B10010001,
  B11111111,
  B01100110,
};
void setup() {
  com.init();
  Wire.begin(6);
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent); 
  Serial.begin(115200);
  for (int i= 0; i<3 ; i++){
  Dote.shutdown(i, false);
  Dote.setIntensity(i, 7);
  Dote.clearDisplay(i);
  }
}

void loop() {
    for (int j =0;j<3;j++){
        for (int i = 0; i < 8; i++){
            Dote.setRow(j, i, digits[i]);
        }
    }
    int distance1 = ultra1.getDistance();
    int distance2 = ultra2.getDistance();
    int distance3 = ultra3.getDistance();

    int dist[3]={distance3,distance2,distance1};
    for (int i = 0; i<3 ; i++){
        if (dist[i]<10){
            Dote.shutdown(i,true);
        }else{
            Dote.shutdown(i,false);
        }
    } 
    if(distance1<10 and distance2<10 and distance3<10){a=0;}
    else if (distance1<10 and distance2<10){a=1;}
    else if (distance2<10 and distance3<10){a=1;}
    else if (distance1<10 and distance3<10){a=1;}
    else if(distance1<10){a=2;}
    else if(distance2<10){a=2;}
    else if(distance3<10){a=2;}
    else{a=3;}
  
}

// 마스터의 요청 이벤트가 발생 할때 항상 호출되는 함수
void receiveEvent(int howMany) {
 while (0 <Wire.available()) {    //메세지가 들어왔다면
    request_type = Wire.read();      /* 1byte 읽음 */
//    Serial.println(request_type);           /* 출력 */
  }
//  Serial.println();             /* to newline */
}

// 마스터로 요청 메세지 작성 함수
void requestEvent() {
 if (request_type==0){
    char buff[3];
    dtostrf(a,1,0,buff); 
    Wire.write(buff); 
    Serial.println(buff);
 }
}
