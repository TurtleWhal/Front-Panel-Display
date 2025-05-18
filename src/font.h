#include <Arduino.h>

struct character
{
    uint8_t width;
    uint8_t height;
    bool data[32];
};

extern character *characters;
extern character *defaultFont;
extern character *smallFont;

extern character gpuIcon;
extern character cpuIcon;

void init_font();
void setFont(character *font);