#ifndef OLED_H
#define OLED_H
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h>
#include <Speed_read.h>
void OLED_init(Adafruit_ST7789 tft);
void OLED_print(float voltage, float current, Adafruit_ST7789 tft);
#endif