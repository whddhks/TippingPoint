#include <Ultra.h>
#include <MiniCom.h>
#include <LedControl.h>
#include <Servo.h>

Ultra ultra1(7, 6);
Ultra ultra2(5, 4);
MiniCom com;
LedControl Dote(10, 8, 9, 2);
Servo myservo1;
Servo myservo2;
const int servo_pin1 = 3;
const int servo_pin2 = 2;

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

  myservo1.attach(servo_pin1);
  myservo2.attach(servo_pin2);
  com.init();
  Dote.shutdown(0, false);
  Dote.shutdown(1, false);
  Dote.setIntensity(0, 5);
  Dote.setIntensity(1, 5);
  Dote.clearDisplay(0);
  Dote.clearDisplay(1);
  
}

void loop() {
  com.run();
  int distance1 = ultra1.getDistance();
  int distance2 = ultra2.getDistance();
  int i;
  for (i = 0; i < 8; i++) {
    Dote.setRow(0, i, digits[i]);
    Dote.setRow(1, i, digits[i]);
  }
  
  if (distance1 < 10) {
    Dote.shutdown(0, true);
    myservo1.write(0);

  } else {
    Dote.shutdown(0, false);
    myservo1.write(90);
  }

  if (distance2 < 10) {
    Dote.shutdown(1, true);
    myservo2.write(0);

  } else {
    Dote.shutdown(1, false);
    myservo2.write(90);
  }


  
  
  
}

