#include <MiniCom.h>
#include <Ultra.h>
#include <LedControl.h>
#include <SimpleTimer.h>
#include <Servo.h>
SimpleTimer timer;
MiniCom com;
int pir_pin=5;
int pirState=LOW;
int val = 0;
LedControl Dote = LedControl(10, 8, 9, 2);
Ultra ultra(7,6);
Servo myservo;
Servo myservo2;
const int servo_pin = 4;
const int servo_pin2 = 3;

//장애인
byte digits1[8] = 
{ 
  B00001000,
  B00010100,
  B11001000,
  B10001111,
  B10001000,
  B10001111,
  B01000001,
  B00111110,
};
//자동차
byte digits2[8] = 
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

void check() {
    int distance = ultra.getDistance();
    com.print(1, "Dist.(cm)=", distance);
}
d
void move_angle(int angle,int angle2, int delay_time) {
    myservo.write(angle);
    myservo2.write(angle2);
    delay(delay_time); 
}



void setup(){
    
    com.init();
    
    pinMode(pir_pin,INPUT);
    
    com.setInterval(1000, check);
    
    Dote.shutdown(0, false);
    Dote.shutdown(1, false);
    Dote.setIntensity(0, 7);
    Dote.setIntensity(1, 7);
    Dote.clearDisplay(0);
    Dote.clearDisplay(1);

    myservo.attach(servo_pin);
    myservo2.attach(servo_pin2);
}

void loop(){
    com.run();
    int distance = ultra.getDistance();
    val=digitalRead(pir_pin);
    
    if(val==HIGH){
        if(pirState==LOW){
            com.print(0,"Motion Detected!");
            pirState=HIGH;
        }

    }else {
        if(pirState==HIGH){
            com.print(0,"Motion Ended!");
            pirState = LOW;
        }
    }

    int i;
    for (i = 0; i < 8; i++){
        Dote.setRow(0, i, digits1[i]);
        // Dote.setRow(1, i, digits2[i]);
    }
    
    if (distance<10){
        Dote.shutdown(0,true);
        move_angle(0,90,500);

    }else{
        Dote.shutdown(0,false);
        move_angle(90,0,500);
    }

}