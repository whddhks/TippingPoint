#include <TFT_22_ILI9225.h>
#include <SPI.h>
#include <AimHangul.h>
#include <../fonts/FreeSans12pt7b.h>
#include <../fonts/FreeSans24pt7b.h>

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);

uint16_t x, y;
boolean flag = false;

void setup() {
  tft.begin();
  String mag="한글";
  AimHangul_h2(0,20+20,msg,COLOR_GREEN);
}

void loop() {

}
