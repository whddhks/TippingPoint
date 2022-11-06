#include <Ultra.h>
#include <LedControl.h>

Ultra ultra1(2,3);
Ultra ultra2(4,5);
Ultra ultra3(6,7);
LedControl Dote(13,11,12,3);

byte digits[8] = 
{ 
  B00000000,
  B01111100,
  B10101010,
  B11101111,
  B10000001,
  B11111111,
  B01100110,
  B00000000
};


void setup() {

    for (int i= 0; i<3 ; i++){
        Dote.shutdown(i, false);
        Dote.setIntensity(i, 7);
        Dote.clearDisplay(i);
    }

}

void loop() {
    int distance1 = ultra1.getDistance();
    int distance2 = ultra2.getDistance();
    int distance3 = ultra3.getDistance();

    for (int j =0;j<3;j++){
        for (int i = 0; i < 8; i++){
            Dote.setRow(j, i, digits[i]);
        }
    }
    int dist[3]={distance1,distance2,distance3};
    for (int i = 0; i<3 ; i++){
        if (dist[i]<10){
            Dote.shutdown(i,true);
        }else{
            Dote.shutdown(i,false);
        }
    }


    //모션감지 센서
    // int pir_pins[3]={pir_pin1,pir_pin2,pir_pin3};

    // for (int i=0;i<3;i++){
    //     val=digitalRead(pir_pins[i]);
    //     if (val==HIGH){
    //         if(pirState==LOW){
    //             Serial.println(i+"Motion Detected!");
    //             pirState=HIGH;
    //         }
    //     }else{
    //         if(pirState==HIGH){
    //         Serial.println(i+"Motion Ended!!");
    //         pirState=LOW;
    //         }
    //     }
    // }

}