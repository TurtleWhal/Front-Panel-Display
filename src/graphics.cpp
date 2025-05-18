#include "graphics.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(195, 2, NEO_GRB + NEO_KHZ800);

uint32_t buf[7][29] = {};

void initDisplay()
{
    pixels.begin();
    pixels.setBrightness(30);

    init_font();

    clearScreen();
    applyBuffer();
}

void applyBuffer()
{
    pixels.clear();

    for (int i = 27; i > 2; i--)
    {
        pixels.setPixelColor(27 - i, buf[6][i]);
        pixels.setPixelColor((27 - i) + 29 + 29 + 29 + 29 + 29 + 25, buf[0][i]);
    }

    for (int i = 0; i < 29; i++)
    {
        pixels.setPixelColor(25 + i, buf[5][i]);
        pixels.setPixelColor(25 + 29 + 29 + i, buf[3][i]);
        pixels.setPixelColor(25 + 29 + 29 + 29 + 29 + i, buf[1][i]);
    }

    for (int i = 28; i >= 0; i--)
    {
        pixels.setPixelColor((28 - i) + 29 + 25, buf[4][i]);
        pixels.setPixelColor((28 - i) + 29 + 29 + 29 + 25, buf[2][i]);
    }

    pixels.show();
}

void drawPixel(int8_t x, int8_t y, uint32_t color)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 29)
        return;

    buf[x][y] = color;
}

uint32_t hsl(float h, float s, float l)
{
    float r, g, b;

    if (s == 0)
    {
        r = g = b = l; // achromatic
    }
    else
    {
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        float hk = h / 360.0;

        float t[3];
        t[0] = hk + 1.0 / 3.0; // r
        t[1] = hk;             // g
        t[2] = hk - 1.0 / 3.0; // b

        for (int i = 0; i < 3; i++)
        {
            if (t[i] < 0)
                t[i] += 1.0;
            if (t[i] > 1)
                t[i] -= 1.0;

            if (t[i] < 1.0 / 6.0)
            {
                t[i] = p + (q - p) * 6.0 * t[i];
            }
            else if (t[i] < 1.0 / 2.0)
            {
                t[i] = q;
            }
            else if (t[i] < 2.0 / 3.0)
            {
                t[i] = p + (q - p) * (2.0 / 3.0 - t[i]) * 6.0;
            }
            else
            {
                t[i] = p;
            }
        }

        r = t[0];
        g = t[1];
        b = t[2];
    }

    return pixels.Color(int(r * 255.0), int(g * 255.0), int(b * 255.0));
}

uint32_t getColorFromTemperature(float currentTemp, float minTemp, float maxTemp)
{
    // Clamp current temperature within the min and max range
    currentTemp = constrain(currentTemp, minTemp, maxTemp);

    // Normalize the temperature to a range of 0 to 1
    float normalizedTemp = (currentTemp - minTemp) / (maxTemp - minTemp);

    // Calculate the color components
    int red = ceil(normalizedTemp * 255); // Red increases with temperature
    int blue = 255 - red;                 // Blue decreases with temperature

    // Return the color as a 32-bit integer (ARGB format)
    return (red << 16) | (0 << 8) | blue; // Assuming 0 for green
}

void clearScreen()
{
    for (int8_t i = 0; i < 7; i++)
    {
        for (int8_t j = 0; j < 29; j++)
        {
            drawPixel(i, j, 0);
        }
    }
}

void drawLine(int8_t x1, int8_t y1, int8_t x2, int8_t y2, uint32_t color)
{
    int8_t dx = abs(x2 - x1);       // Difference in x
    int8_t dy = abs(y2 - y1);       // Difference in y
    int8_t sx = (x1 < x2) ? 1 : -1; // Step in x direction
    int8_t sy = (y1 < y2) ? 1 : -1; // Step in y direction
    int8_t err = dx - dy;           // Error term

    while (true)
    {
        drawPixel(x1, y1, color); // Draw the pixel at the current position
        if (x1 == x2 && y1 == y2)
            break;           // Stop if we've reached the end point
        int8_t e2 = err * 2; // Temporary variable to hold 2 * error
        if (e2 > -dy)
        { // Check if we need to move in the x direction
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        { // Check if we need to move in the y direction
            err += dx;
            y1 += sy;
        }
    }
}

void drawRect(int8_t x, int8_t y, int8_t width, int8_t height, uint32_t color)
{
    drawLine(x, y, x + width - 1, y, color);
    drawLine(x, y + height - 1, x + width - 1, y + height - 1, color);
    drawLine(x, y, x, y + height - 1, color);
    drawLine(x + width - 1, y, x + width - 1, y + height - 1, color);
}

void fillRect(int8_t x, int8_t y, int8_t width, int8_t height, uint32_t color)
{
    for (int8_t i = 0; i < width; i++)
    {
        for (int8_t j = 0; j < height; j++)
        {
            drawPixel(x + i, y + j, color);
        }
    }
}

void fillScreen(uint32_t color)
{
    fillRect(0, 0, 7, 29, color);
}

void drawTriangle(int8_t x1, int8_t y1, int8_t x2, int8_t y2, int8_t x3, int8_t y3, uint32_t color)
{
    drawLine(x1, y1, x2, y2, color); // Draw side 1
    drawLine(x2, y2, x3, y3, color); // Draw side 2
    drawLine(x3, y3, x1, y1, color); // Draw side 3
}

void fillTriangle(int8_t x1, int8_t y1, int8_t x2, int8_t y2, int8_t x3, int8_t y3, uint32_t color)
{
    // Find the bounding box of the triangle
    int8_t minY = min(y1, min(y2, y3));
    int8_t maxY = max(y1, max(y2, y3));

    for (int8_t y = minY; y <= maxY; y++)
    {
        // Initialize min and max x values for the current scanline
        int16_t xMin = 255, xMax = 0;

        // Helper lambda to calculate the x-coordinate of intersection
        auto getX = [](int8_t x1, int8_t y1, int8_t x2, int8_t y2, int8_t y) -> int16_t
        {
            if (y1 == y2)
                return x1; // Avoid division by zero for vertical lines
            return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        };

        // Determine xMin and xMax for each side of the triangle
        if (y >= min(y1, y2) && y <= max(y1, y2))
        {
            xMin = min(xMin, getX(x1, y1, x2, y2, y));
            xMax = max(xMax, getX(x1, y1, x2, y2, y));
        }
        if (y >= min(y2, y3) && y <= max(y2, y3))
        {
            xMin = min(xMin, getX(x2, y2, x3, y3, y));
            xMax = max(xMax, getX(x2, y2, x3, y3, y));
        }
        if (y >= min(y3, y1) && y <= max(y3, y1))
        {
            xMin = min(xMin, getX(x3, y3, x1, y1, y));
            xMax = max(xMax, getX(x3, y3, x1, y1, y));
        }

        // Draw the horizontal line from xMin to xMax
        for (int16_t x = xMin; x <= xMax; x++)
        {
            drawPixel(x, y, color);
        }
    }
}

void drawCircle(int8_t xCenter, int8_t yCenter, float radius, uint32_t color)
{
    float x = radius;
    int16_t y = 0;
    int16_t decisionOver2 = 1 - x; // Decision criterion

    while (y <= x)
    {
        drawPixel(xCenter + x, yCenter + y, color); // Octant 1
        drawPixel(xCenter + y, yCenter + x, color); // Octant 2
        drawPixel(xCenter - y, yCenter + x, color); // Octant 3
        drawPixel(xCenter - x, yCenter + y, color); // Octant 4
        drawPixel(xCenter - x, yCenter - y, color); // Octant 5
        drawPixel(xCenter - y, yCenter - x, color); // Octant 6
        drawPixel(xCenter + y, yCenter - x, color); // Octant 7
        drawPixel(xCenter + x, yCenter - y, color); // Octant 8

        y++;
        if (decisionOver2 <= 0)
        {
            decisionOver2 += 2 * y + 1; // East
        }
        else
        {
            x--;
            decisionOver2 += 2 * (y - x) + 1; // South-East
        }
    }
}

void fillCircle(int8_t xCenter, int8_t yCenter, float radius, uint32_t color)
{
    for (float y = -radius; y <= radius; y++)
    {
        int16_t xMax = sqrt(radius * radius - y * y); // Calculate the maximum x value
        for (int16_t x = -xMax; x <= xMax; x++)
        {
            drawPixel(xCenter + x, yCenter + y, color); // Draw each pixel
        }
    }
}

void drawChar(int8_t x, int8_t y, character c, uint32_t color)
{
    bool *data = c.data;

    // Serial.printf("ch width: %d, height: %d\n", ch.width, ch.height);

    for (int8_t i = 0; i < c.height; i++)
    {
        for (int8_t j = 0; j < c.width; j++)
        {
            // Serial.print(data[i * ch.width + j] ? "X" : " ");

            if (data[i * c.width + j])
            {
                // if (rotate)
                //     // drawPixel((x + 6) - i, (y + 2) - j, color);
                //     drawPixel((x + ch.height - 1) - i, y + j, color);
                // else
                drawPixel(x + j, y + i, color);
            }
        }
        // Serial.println();
    }
}

void drawChar(int8_t x, int8_t y, char c, uint32_t color, bool rotate)
{
    // Serial.printf("Drawing character %c at %d, %d\n", c, x, y);

    auto ch = characters[c];

    drawChar(x, y, ch, color);

    return;

    bool *data = ch.data;

    // Serial.printf("ch width: %d, height: %d\n", ch.width, ch.height);

    for (int8_t i = 0; i < ch.height; i++)
    {
        for (int8_t j = 0; j < ch.width; j++)
        {
            // Serial.print(data[i * ch.width + j] ? "X" : " ");

            if (data[i * ch.width + j])
            {
                if (rotate)
                    // drawPixel((x + 6) - i, (y + 2) - j, color);
                    drawPixel((x + ch.height - 1) - i, y + j, color);
                else
                    drawPixel(x + j, y + i, color);
            }
        }
        // Serial.println();
    }
}

void drawString(int8_t x, int8_t y, const char *str, uint32_t color, bool rotate)
{
    int8_t oldX = x;
    while (*str)
    {

        if (*str == '\n')
        {
            if (rotate)
            {
                y = oldX;
                x += characters[*str].height + 1;
            }
            else
            {
                x = oldX;
                y += characters[*str].height + 1;
            }

            str++;
            continue;
        }

        drawChar(x, y, *str, color, rotate);

        if (rotate)
            y += characters[*str].width + 1;
        else
            x += characters[*str].width + 1;

        str++;
    }
}