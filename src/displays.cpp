#include "Arduino.h"

#include "displays.h"
#include "graphics.h"
#include "data.h"

#define DEG2RAD 0.0174532925

uint j = 0;

void RainbowTime(PerformanceData data)
{
    for (int i = 0; i < 36; i++)
    {
        // drawCircle(3, 14, i, hsl(j + (i * 10), 1, 0.5));
        int32_t c = hsl((((i * 2) * (360 / 60)) + j) % 360, 1, 0.5);
        drawLine(0, i, 3, i - 3, c);
        drawLine(3, i - 3, 6, i, c);
        // drawLine(0, i, 6, i - 7, c);
    }

    j = (j + 1) % 360;

    setFont(smallFont);

    int ofs = 0;

    if (data.hour >= 10)
    {
        drawChar(2, 3, '0' + data.hour / 10, BLACK, false);
        ofs = 2;
    }

    drawChar(2, 7 + ofs, '0' + data.hour % 10, BLACK, false);
    drawChar(2, 14 + ofs, '0' + data.minute / 10, BLACK, false);
    drawChar(2, 20 + ofs, '0' + data.minute % 10, BLACK, false);
}

void GladOS(PerformanceData data)
{
    fillRect(0, 0, 7, 29, WHITE);
    fillRect(1, 8, 4, 14, BLACK);

    fillRect(2, 13, 2, 4, ORANGE);
    fillRect(1, 14, 4, 2, ORANGE);
}

void CoolTime(PerformanceData data)
{
    setFont(smallFont);

    drawChar(1, 2, '0' + data.hour / 10, WHITE, false);
    drawChar(1, 8, '0' + data.hour % 10, WHITE, false);
    drawChar(3, 18, '0' + data.minute / 10, WHITE, false);
    drawChar(3, 24, '0' + data.minute % 10, WHITE, false);

    // drawPixel(5, 2, WHITE);
    drawLine(6, 3, 6, 12, WHITE);
    drawLine(6, 12, 0, 18, WHITE);
    drawLine(0, 18, 0, 27, WHITE);
    // drawPixel(1, 28, WHITE);
}

void TempDisplay(PerformanceData data)
{
    for (int i = 0; i < (25 * (data.cpuUsage / 100)); i++)
    {
        drawPixel(0, 27 - i, data.cpuTemp == 0 ? ORANGE : getColorFromTemperature((((data.cpuTemp - CPU_MIN_TEMP) / (25 * (data.cpuUsage / 100))) * i) + CPU_MIN_TEMP, CPU_MIN_TEMP, CPU_MAX_TEMP));
    }

    for (int i = 0; i < (25 * (data.gpuUsage / 100)); i++)
    {
        drawPixel(6, 27 - i, getColorFromTemperature((((data.gpuTemp - GPU_MIN_TEMP) / (25 * (data.gpuUsage / 100))) * i) + GPU_MIN_TEMP, GPU_MIN_TEMP, GPU_MAX_TEMP));
    }

    setFont(smallFont);

#define DELAY 2000

    if (data.gpuUsage >= data.cpuTemp)
    {
        drawChar(1, 0, gpuIcon, WHITE);

        String s = (millis() / DELAY) % 2 ? String((int)data.gpuTemp) + "C" : String((int)data.gpuUsage) + "%";

        for (int i = 0; i < s.length(); i++)
        {
            drawChar(2, 9 + 6 * i, s.charAt(i), WHITE, false);
        }
    }
    else
    {
        drawChar(1, 0, cpuIcon, WHITE);

        String s = (millis() / DELAY) % 2 ? String((int)data.cpuTemp) + "C" : String((int)data.cpuUsage) + "%";

        for (int i = 0; i < s.length(); i++)
        {
            drawChar(2, 9 + 6 * i, s.charAt(i), WHITE, false);
        }
    }

    // String s = "CPU " + String(data.cpuTemp) + "C " + String(data.cpuUsage) + "% GPU " + String(data.gpuTemp) + "C " + String(data.gpuUsage) + "%";
    // char s[32] = "CPU 12C 34% GPU 56C 78%";
    // sprintf(s, "CPU %dC %d%% GPU %dC %d%%", (int)data.cpuTemp, (int)data.cpuUsage, (int)data.gpuTemp, (int)data.gpuUsage);
    // size_t len = strlen(s);

    // String spacedString;
    // for (size_t i = 0; i < len; i++)
    // {
    //     spacedString += s[i];
    //     if (i < len - 1)
    //     {
    //         spacedString += '\n';
    //     }
    // }

    // uint8_t x = (millis() / 200) % (len * 6);
    // drawString(2, -x, spacedString.c_str(), WHITE, false);
    // drawString(2, ((len * 6) + 5) - x, spacedString.c_str(), WHITE, false);
}

void drawFan(int x, int y, int r)
{
    drawLine(x, y, x + (3 * cos(DEG2RAD * r)), y + (3 * sin(DEG2RAD * r)), WHITE);
}

void TestDisplay(PerformanceData data)
{
    drawFan(3, 14, millis() / 10);
}