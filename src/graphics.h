#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "font.h"

extern Adafruit_NeoPixel pixels;

#define WHITE pixels.Color(255, 255, 255)
#define RED pixels.Color(255, 0, 0)
#define GREEN pixels.Color(0, 255, 0)
#define BLUE pixels.Color(0, 0, 255)
#define YELLOW pixels.Color(255, 255, 0)
#define MAGENTA pixels.Color(255, 0, 255)
#define CYAN pixels.Color(0, 255, 255)
#define ORANGE pixels.Color(255, 50, 0)
#define PURPLE pixels.Color(100, 0, 100)
#define BLACK pixels.Color(0, 0, 0)

uint32_t getColorFromTemperature(float currentTemp, float minTemp, float maxTemp);
uint32_t hsl(float h, float s, float l);

void initDisplay();

void applyBuffer();
void clearScreen();

void drawPixel(int8_t x, int8_t y, uint32_t color);
void drawLine(int8_t x1, int8_t y1, int8_t x2, int8_t y2, uint32_t color);
void drawRect(int8_t x, int8_t y, int8_t w, int8_t h, uint32_t color);
void fillRect(int8_t x, int8_t y, int8_t w, int8_t h, uint32_t color);
void fillScreen(uint32_t color);
void drawTriangle(int8_t x1, int8_t y1, int8_t x2, int8_t y2, int8_t x3, int8_t y3, uint32_t color);
void fillTriangle(int8_t x1, int8_t y1, int8_t x2, int8_t y2, int8_t x3, int8_t y3, uint32_t color);
void drawCircle(int8_t x, int8_t y, float r, uint32_t color);
void fillCircle(int8_t x, int8_t y, float r, uint32_t color);

void drawChar(int8_t x, int8_t y, character c, uint32_t color);
void drawChar(int8_t x, int8_t y, char c, uint32_t color, bool rotate = false);
void drawString(int8_t x, int8_t y, const char *s, uint32_t color, bool rotate = false);