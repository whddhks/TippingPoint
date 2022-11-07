void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  String s_var = "hi";
  String a_var = "bye";

  Serial.println(s_var);
  Serial.println(a_var);
  delay(500);
}
