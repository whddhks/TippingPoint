#include <MiniCom.h>
MiniCom com;
int echoPin = 7;
int triggerPin = 6;

void check() {
    // trigger 핀으로 10us의 펄스를 발생
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    // echo 핀 입력으로부터 거리르 cm 단위로 계산
    int distance = pulseIn(echoPin, HIGH) / 58;
    com.print(1, "Dist.(cm)=", distance);
}

void setup() {
    com.init();
    com.setInterval(1000, check);
    com.print(0, "[[Distance]]");
    pinMode(echoPin, INPUT);
    pinMode(triggerPin, OUTPUT);
}

void loop() {
    com.run();
}