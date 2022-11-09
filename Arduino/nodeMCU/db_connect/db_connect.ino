//mysql 연결관련 헤더
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#include <ESP8266WiFi.h> //와이파이
#include <stdio.h>




//wifi 정보
const char* ssid = "KT_GiGA_2G_6E0F";
const char* password = "2dib1bc850";

//sql 연결셋팅부
IPAddress server_addr(172,30,1,57); // I여기에 IP를 넣어주세요
//char hostname[] = "#######################"; // 도메인 주소가 있다면 넣어주세요  !! 둘중 하나만 사용해야합니다!
char user[] = "root"; //계정정보 ID입니다
char password_[] = "multi123";        // DB password입니다

WiFiClient client;
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

//char INSERT_SQL[] = "INSERT INTO test_arduino.hello_arduino (message) VALUES ('Hello, Arduino!')"; //실제 넣어줄 쿼리문입니다
char SELECT_SQL[]= "select * from  "
void setup() {
  Serial.begin(115200); //전송속도

  Wifi_connect(); //와이파이 연결

//  setupDateTime();//시간설정

  Serial.print("Connecting to SQL...  ");
  if (conn.connect(server_addr, 3306, user, password_)) {
    Serial.println("OK.");
  }
  else {
    Serial.println("FAILED.");
  }
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
  
}


void loop() {
  
  if (WiFi.status() != WL_CONNECTED) { //와이파이 연결이 이상할시에 재접속 하는 코드
    Serial.println("try reconnect");
    Wifi_connect();
  }


  sprintf(INSERT_SQL, "INSERT INTO test.test VALUES (NOW(),%d,%d)", 22, 23); //쿼리문 온습도
 
  if (conn.connected()) {
    cursor->execute(INSERT_SQL); //실제 excute되는 시점
  }

  delay(300000);

}



void Wifi_connect() {

  Serial.println("---------------------------------------");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // 와이파이 이름과 비밀번호를 통해 WIFI연결을 시작 // WL_CONNECTED라는 값을 돌려준다
  while (WiFi.status() != WL_CONNECTED) {
    // 네트워크의 연결 상태, 8개의 리턴값
    // STATUS와 WL_CONNECTED 값이 같은지를 통해 제대로 연결이 되있는지를 확인할 수 있다
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wifi connected!");
  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());
}
