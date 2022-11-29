#include <Ultra.h>
#include <Servo.h>
#include <SimpleTimer.h>
Servo servo1;
Servo servo2;
const int servo_pin1 = 12;
const int servo_pin2 = 13;
Ultra ultra1(9,8);
Ultra ultra2(11,10);
SimpleTimer timer;

void setup() {
    servo1.attach(servo_pin1);
    servo2.attach(servo_pin2);
    Serial.begin(115200);
}
void intro_open() {
    servo1.write(90);

}
void outro_open() {
    servo2.write(90);
    
}

void intro_sensor() {
    int distance1=ultra1.getDistance();
    if (distance1 < 10) {
      String in_close_car="in_1";
      Serial.println(in_close_car);
      timer.setTimeout(3000,intro_open);
    }else{servo1.write(0);}
}
    
void outro_sensor() {
    int distance2=ultra2.getDistance();
    if (distance2 < 10) {
      String out_close_car="out_1";
      Serial.println(out_close_car);
      timer.setTimeout(3000,outro_open);
    }else{servo2.write(0);}
}

void loop() {
    timer.run();
    delay(1000);
    outro_sensor();
    delay(1000);
    intro_sensor();
    
}
