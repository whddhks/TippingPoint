
#include <LedControl.h>

LedControl Dote = LedControl(7, 5, 6, 2);

byte digits[8] = 
{ 
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
};

void setup()
{  
  Dote.shutdown(0, false);
  Dote.shutdown(1, false);
  Dote.setIntensity(0, 7);
  Dote.setIntensity(1, 7);
  Dote.clearDisplay(0);
  Dote.clearDisplay(1);
}

void loop() 
{
  int i;
  for (i = 0; i < 8; i++)
  {
   Dote.setRow(0, i, digits[i]);
   delay(500);
  }
  for (i = 0; i < 8; i++)
  {
   Dote.setRow(1, i, digits[i]);
   delay(500);
  }
  delay(500);
  Dote.clearDisplay(0);
  Dote.clearDisplay(1);
  delay(500);
}
