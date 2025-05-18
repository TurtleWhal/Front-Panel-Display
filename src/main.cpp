#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include "movingAvg.h"

#include <Ticker.h>

#include "graphics.h"
#include "displays.h"
#include "data.h"

bool debug = false;

uint32_t lastMsg = 0;
uint32_t msgTimeout = 3000;

uint32_t lastStress = 0;
uint32_t stressTimeout = 6000;

PerformanceData lastData;

Ticker ticker;

void drawTask();
void handleSerialInput();

void setup()
{
  pinMode(4, INPUT_PULLUP);

  Serial.begin(115200);

  ticker.attach_ms(10, drawTask);

  initDisplay();
}

void loop()
{

  // Handle serial data in a non-blocking way
  handleSerialInput();

  // The main loop can perform other tasks without blocking
  // You can include other non-blocking tasks here

  // Let the system run its background tasks
  yield();
}

void drawTask()
{
  clearScreen();

  if (debug)
  {
    // debug code here

    // TestDisplay(lastData);
    // fillScreen(pixels.Color(128, 128, 128));

    // lastData.cpuTemp = 50;
    // lastData.cpuUsage = 60;
    // lastData.gpuTemp = 70;
    // lastData.gpuUsage = 80;

    TempDisplay(lastData);

    applyBuffer();
    return;
  }

  if (millis() - lastMsg < msgTimeout && millis() > msgTimeout)
  {

    // if (lastData.cpuTemp < CPU_MIN_TEMP + 10 && lastData.gpuTemp < GPU_MIN_TEMP + 10)
    if (lastData.cpuUsage < CPU_THRESHOLD && lastData.gpuUsage < GPU_THRESHOLD)
      lastStress = 0;
    else if (lastStress == 0)
      lastStress = millis();

    if (lastStress + stressTimeout > millis() || lastStress == 0)
    {

      // for (int i = 0; i < 36; i++)
      // {
      //   // drawCircle(3, 14, i, hsl(j + (i * 10), 1, 0.5));
      //   int32_t c = hsl((((i * 2) * (360 / 60)) + j) % 360, 1, 0.5);
      //   drawLine(0, i, 3, i - 3, c);
      //   drawLine(3, i - 3, 6, i, c);
      //   // drawLine(0, i, 6, i - 7, c);
      // }

      // j = (j + 1) % 360;

      // setFont(shortNumbers);

      // int ofs = 0;

      // if (hour >= 10)
      // {
      //   drawChar(2, 3, '0' + hour / 10, BLACK, false);
      //   ofs = 2;
      // }

      // drawChar(2, 7 + ofs, '0' + hour % 10, BLACK, false);
      // drawChar(2, 14 + ofs, '0' + minute / 10, BLACK, false);
      // drawChar(2, 20 + ofs, '0' + minute % 10, BLACK, false);

      // fillRect(0, 3, 2, 28, pixels.Color(128, 128, 128));
      // drawLine(0, 28, 0, 28 - , WHITE);

      // RainbowTime(lastData);
      // GladOS(lastData);
      CoolTime(lastData);
    }
    else // Performance Display
    {
      TempDisplay(lastData);
    }
  }

  applyBuffer();
}

void handleSerialInput()
{
  if (Serial.available() > 0)
  {
    lastMsg = millis();

    String input = Serial.readString();

    JsonDocument doc;
    deserializeJson(doc, input);

    // Get and Print Cpu Usage
    // String cpu_usage = doc["cpu_usage"];
    // String cpu_temp = doc["cpu_temp"];
    // cpu_usage = cpu_usage.substring(0, cpu_usage.length() - 1);
    // cpu_temp = cpu_temp.substring(0, cpu_temp.length() - 1);

    // lastData.cpuUsage = cpu_usage.toFloat();
    // lastData.cpuTemp = cpu_temp.toFloat();

    float cpu_usage = doc["cpu_usage"];
    float cpu_temp = doc["cpu_temp"];

    lastData.cpuUsage = cpu_usage;
    lastData.cpuTemp = cpu_temp;

    // drawLine(0, 28, 0, 27 - (25 * (cpu / 100)), cpu_temp.toFloat() == 0 ? ORANGE : getColorFromTemperature(cpu_temp.toFloat(), CPU_MIN_TEMP, CPU_MAX_TEMP));

    // Get and Print Cpu Usage
    // String gpu_usage = doc["gpu_usage"];
    // String gpu_temp = doc["gpu_temp"];
    // gpu_usage = gpu_usage.substring(0, gpu_usage.length() - 1);
    // gpu_temp = gpu_temp.substring(0, gpu_temp.length() - 1);

    // lastData.gpuUsage = gpu_usage.toFloat();
    // lastData.gpuTemp = gpu_temp.toFloat();

    float gpu_usage = doc["gpu_usage"];
    float gpu_temp = doc["gpu_temp"];

    lastData.gpuUsage = gpu_usage;
    lastData.gpuTemp = gpu_temp;

    // drawLine(6, 28, 6, 27 - (25 * (gpu / 100)), getColorFromTemperature(gpu_temp.toFloat(), GPU_MIN_TEMP, GPU_MAX_TEMP));

    // Get and Print Time
    String utcTimeStr = doc["time"];

    struct tm timeinfo;
    strptime(utcTimeStr.c_str(), "%Y-%m-%dT%H:%M:%S", &timeinfo);

    lastData.minute = timeinfo.tm_min;
    lastData.second = timeinfo.tm_sec;

    if (timeinfo.tm_hour > 12)
      lastData.hour = timeinfo.tm_hour - 12;
    else
      lastData.hour = timeinfo.tm_hour;
  }
}