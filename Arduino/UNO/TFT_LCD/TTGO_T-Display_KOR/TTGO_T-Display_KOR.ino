#include <TFT_eSPI.h>
#include <SPI.h>

#include "Fonts/ASCFont.h"  //영어폰트  
#include "Fonts/KSFont.h"   //한글폰트

// #define TFT_MOSI            19
// #define TFT_SCLK            18
// #define TFT_CS              5
// #define TFT_DC              16
// #define TFT_RST             23
// #define TFT_BL          4  // Display backlight control pin

// #define TFT_RST 8
// #define TFT_RS  9
// #define TFT_CS  10  // SS
// #define TFT_SDI 11  // MOSI
// #define TFT_CLK 13  // SCK
// #define TFT_LED 3   // 0 if wired to +5V directly
// #define TFT_BRIGHTNESS 100

byte HANFontImage[32] = {0, };
String inputString;

TFT_eSPI tft = TFT_eSPI(); // Invoke custom libraryTFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library
// TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED,TFT_BRIGHTNESS);
int menu=0;
long long int current_time=0;
long long int past_time=0;
float fCnt=0;
boolean isCheck=true;

//한글 조합형으로 글자 이미지를 만든다 
byte *getHAN_font(byte HAN1, byte HAN2, byte HAN3){

  const byte cho[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0 };
  const byte cho2[] = { 0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5 };
  const byte jong[] = { 0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1 };

  uint16_t utf16;
  byte first, mid, last;
  byte firstType, midType, lastType;
  byte i;
  byte *pB, *pF;

  /*------------------------------
    UTF-8 을 UTF-16으로 변환한다.

    UTF-8 1110xxxx 10xxxxxx 10xxxxxx
  */------------------------------
  utf16 = (HAN1 & 0x0f) << 12 | (HAN2 & 0x3f) << 6 | HAN3 & 0x3f;

  /*------------------------------
    초,중,종성 코드를 분리해 낸다.

    unicode = {[(초성 * 21) + 중성] * 28}+ 종성 + 0xAC00

          0   1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 
    초성 ㄱ   ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ
    중성 ㅏ   ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ  
    종성 없음 ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅄ ㅅ ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ
  ------------------------------*/
  utf16 -= 0xac00;
  last = utf16 % 28;
  utf16 /= 28;
  mid = utf16 % 21;
  first = utf16 / 21;

  first++;
  mid++;

  /*------------------------------
    초,중,종성 해당 폰트 타입(벌)을 결정한다.
  ------------------------------*/

  /*
   초성 19자:ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ
   중성 21자:ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ
   종성 27자:ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅆㅇㅈㅊㅋㅌㅍㅎ

   초성
      초성 1벌 : 받침없는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
      초성 2벌 : 받침없는 'ㅗㅛㅡ'
      초성 3벌 : 받침없는 'ㅜㅠ'
      초성 4벌 : 받침없는 'ㅘㅙㅚㅢ'
      초성 5벌 : 받침없는 'ㅝㅞㅟ'
      초성 6벌 : 받침있는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
      초성 7벌 : 받침있는 'ㅗㅛㅜㅠㅡ'
      초성 8벌 : 받침있는 'ㅘㅙㅚㅢㅝㅞㅟ'

   중성
      중성 1벌 : 받침없는 'ㄱㅋ' 와 결합
      중성 2벌 : 받침없는 'ㄱㅋ' 이외의 자음
      중성 3벌 : 받침있는 'ㄱㅋ' 와 결합
      중성 4벌 : 받침있는 'ㄱㅋ' 이외의 자음

   종성
      종성 1벌 : 중성 'ㅏㅑㅘ' 와 결합
      종성 2벌 : 중성 'ㅓㅕㅚㅝㅟㅢㅣ'
      종성 3벌 : 중성 'ㅐㅒㅔㅖㅙㅞ'
      종성 4벌 : 중성 'ㅗㅛㅜㅠㅡ'

  */
  if(!last){  //받침 없는 경우
    firstType = cho[mid];
    if(first == 1 || first == 24) midType = 0;
    else midType = 1;
  }
  else{       //받침 있는 경우
    firstType = cho2[mid];
    if(first == 1 || first == 24) midType = 2;
    else midType = 3;
    lastType = jong[mid];
  }
  memset(HANFontImage, 0, 32);

  //초성 
  pB = HANFontImage;
  pF = (byte*)KSFont + (firstType*20 + first)*32;

  i = 32;
  while(i--) *pB++ = pgm_read_byte(pF++); 

  //중성
  pB = HANFontImage;
  pF = (byte*)KSFont + (8*20 + midType*22 + mid)*32;

  i = 32;
  while(i--) *pB++ |= pgm_read_byte(pF++); 

  //종성
  if(last){
    pB = HANFontImage;
    pF = (byte*)KSFont + (8*20 + 4*22 + lastType*28 + last)*32;

    i = 32;
    while(i--) *pB++ |= pgm_read_byte(pF++); 
  }

  return HANFontImage;
}


//정해진 위치에 입력된 문자를 출력한다 
void matrixPrint(int XPOS,int YPOS,char *pChar){
  byte rg = 3;   //<b1> red, <b0> green
  byte *pFs;
  byte i, b;
  byte c, c2, c3;

  while(*pChar){ 
    c = *(byte*)pChar++;

    // /r 개행 문자인지 확인
    if(c == 0x2F && *(byte*)pChar++ == 0x72){
      XPOS = 0;
      YPOS = YPOS+16;
    }
    //---------- 한글 ---------
    else if(c >= 0x80){
      c2 = *(byte*)pChar++;
      c3 = *(byte*)pChar++;
      
      pFs = getHAN_font(c, c2, c3);
      tft.drawBitmap(XPOS, YPOS,  pFs, 16, 16, 1);
      XPOS = XPOS+16;
      if(XPOS > 240){
        XPOS = 0;
        YPOS = YPOS+16;
      }
    }
    //---------- ASCII ---------
    else{
      pFs = (byte*)ASCfontSet + ((byte)c - 0x20) * 16;
      tft.drawBitmap(XPOS, YPOS,  pFs, 8, 16, 1);
      XPOS = XPOS+8;
      if(XPOS > 240){
        XPOS = 0;
        YPOS = YPOS+16;
      }   
    }   
  }  
}  


void setup(void)
{
    // Serial.begin(115200);
    // Serial.println("Start");
    // tft.init();  //tft 초기화
    // tft.setRotation(1); //화면 전환 0=세로, 1= 세로
    // tft.fillScreen(TFT_WHITE); //배경색상 흰색 지정
    // current_time=millis();
    // past_time=current_time+1000;
    //TFT 백라이트 켜기 
    // if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    //      pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
    //      digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    // }
    //출력할 문자 설정 
    
    inputString = "우리나라 좋은 나라/r한글 만세";
    // //조합형 한글 이미지 만들어 출력
    char paramChar[inputString.length()+1];
    inputString.toCharArray(paramChar, inputString.length() + 1);
    matrixPrint(0,10,paramChar);
}


void loop()
{
  
}
