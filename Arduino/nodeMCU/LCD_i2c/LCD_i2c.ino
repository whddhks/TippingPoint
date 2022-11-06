#include <MiniCom.h>

MiniCom com;

void setup() {
    com.init();
    int a=0;
    com.print(0,"<=Nomal");
    com.print(1,"Disable=>");
}

void loop() {
    com.run();
}