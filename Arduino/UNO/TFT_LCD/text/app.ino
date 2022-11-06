void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  String s= "11가1111";
  for (int i =0; i<s.length();i++ ){
    char a=s[i];
    a=(a&0xFF);
    Serial.println(a,HEX);
    Serial.println(utf8tokor("%EA%B0%80"));
  }
}
String utf8tokor(String utf8){
  String outputdata = "";
  int tmp = 0;
  int ttmp = 0;
  char utf8data;
  
  tmp = utf8.indexOf('%',tmp);//처음 %찾기
 
  while(tmp > -1){//%가 없으면
    String temp = utf8;
    utf8data = utf8c(temp.substring(tmp+1,tmp+3));
    temp = utf8;
      if(ttmp != tmp){//다음 단어의 시작 %가 이번 단어의 끝과 같지 않을땐 사이에 일반 값이 있으므로
         outputdata += temp.substring(ttmp,tmp);//%--%--%--와%--%--%-- 사이의 값을 잘라내어 저장
      }
    ttmp = tmp+3;
    tmp = utf8.indexOf('%',tmp+3);
    outputdata += utf8data;
    }
    
    outputdata += utf8.substring(ttmp,utf8.length());//맨뒤 남은것 저장
    
    return outputdata;
  }
int utf8c(String utf8data){
  int returndata = 0;
  int i=0;
  Serial.print(utf8data);
  for(i=0;i<=1;i++){
    if((utf8data[i] > 47) && (utf8data[i] < 58)){ //0~9
      returndata += (((int)utf8data[i]-48) * (int)(pow(16,(1-i))+0.1));
      }
    if((utf8data[i] > 64) && (utf8data[i] < 71)){ //A~F
      returndata += (((int)utf8data[i]-55) * (int)(pow(16,(1-i))+0.1));
      }
    }
    return returndata;
  }

void loop() {
  // put your main code here, to run repeatedly:

}