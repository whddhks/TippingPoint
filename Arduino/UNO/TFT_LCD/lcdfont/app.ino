#include <SPI.h>
#include <TFT_22_ILI9225.h>
#include <../fonts/ASCFont.h>
#include <../fonts/KSFont.h>

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly
// #define TFT_BRIGHTNESS 100
byte HANFontImage[32] = {0, };
String inputString;

// TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);
byte *getHAN_font(byte HAN1, byte HAN2, byte HAN3){

  const byte cho[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0 };
  const byte cho2[] = { 0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5 };
  const byte jong[] = { 0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1 };

  uint16_t utf16;
  byte first, mid, last;
  byte firstType, midType, lastType;
  byte i;
  byte *pB, *pF;


  utf16 = (HAN1 & 0x0f) << 12 | (HAN2 & 0x3f) << 6 | HAN3 & 0x3f;

  utf16 -= 0xac00;
  last = utf16 % 28;
  utf16 /= 28;
  mid = utf16 % 21;
  first = utf16 / 21;

  first++;
  mid++;

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
      tft.drawBitmap(XPOS, YPOS,  pFs, 16, 16,COLOR_WHITE,COLOR_BLACK);
      XPOS = XPOS+16;
      if(XPOS > 240){
        XPOS = 0;
        YPOS = YPOS+16;
      }
    }
    //---------- ASCII ---------
    else{
      pFs = (byte*)ASCfontSet + ((byte)c - 0x20) * 16;
      tft.drawBitmap(XPOS, YPOS,  pFs, 8, 16,COLOR_WHITE,COLOR_BLACK);
      XPOS = XPOS+8;
      if(XPOS > 240){
        XPOS = 0;
        YPOS = YPOS+16;
      }   
    }   
  }  
}  

void setup(){
    // tft.init();  //tft 초기화
    // tft.setRotation(1); //화면 전환 0=세로, 1= 세로
    // tft.fillScreen(TFT_WHITE); //배경색상 흰색 지정
    tft.begin();
    tft.setOrientation(1);

    //TFT 백라이트 켜기 
    // if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    //      pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
    //      digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    // }
    
    //출력할 문자 설정 
    inputString = "우리나라좋은나라";
    
    //조합형 한글 이미지 만들어 출력
    char paramChar[inputString.length()+1];
    inputString.toCharArray(paramChar, inputString.length() + 1);
    matrixPrint(0,10,paramChar);


    String msg= "남조선동무들";
    char paramChar1[msg.length()+1];
    msg.toCharArray(paramChar1, msg.length() + 1);
    matrixPrint(0,50,paramChar1);
    
    String msg2= "남조선동무들";
    char paramChar2[msg2.length()+1];
    msg2.toCharArray(paramChar2, msg2.length() + 1);
    matrixPrint(0,90,paramChar2);
}
void loop(){
    
}