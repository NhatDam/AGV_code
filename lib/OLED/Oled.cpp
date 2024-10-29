#include <Oled.h>
void OLED_init(Adafruit_ST7789 tft){
    
    tft.init(240, 320,SPI_MODE2);
    tft.fillScreen(ST77XX_BLACK);

    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
    tft.setTextSize(4);
    tft.print("V: ");

    tft.setCursor(0, 80);
    tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
    tft.setTextSize(4);
    tft.print("I: ");

    tft.setCursor(0, 180);
    tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
    tft.setTextSize(4);
    tft.print("L: ");

    tft.setCursor(0, 280);
    tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
    tft.setTextSize(4);
    tft.print("R: ");
}

void OLED_print(float voltage, float current, Adafruit_ST7789 tft){
char _buffer[11];
// sprintf(_buffer,"%02u.%02u",(int)(voltage),(int)(voltage*100)%100);
tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
tft.setCursor(18*3, 0);
tft.setTextSize(4);
tft.print (voltage);
  
// sprintf(_buffer,"%02u.%02u",(int)(current),(int)(current*100)%100);
tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
tft.setCursor(18*3, 80);
tft.setTextSize(4);
tft.print (current);

tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
tft.setCursor(18*3, 180);
tft.setTextSize(4);

tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
tft.setCursor(18*3, 280);
tft.setTextSize(4);
}