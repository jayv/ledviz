#include <iostream>
#include <SDL.H>
#include "Adafruit_NeoPixel.h"
#include "LedAssignments.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel();

void delay(uint8_t wait) {
    strip.delay(wait);
}

// Fill the dots one after the other with a color
void colorWipeUp(uint32_t color, uint8_t wait) {
    for (uint16_t i = 0; i < GetMaxColHeight(); i++) {
        for(uint16_t pixel_idx = 0; pixel_idx < strip.numPixels(); pixel_idx++) {
            uint8_t row_idx, col_idx;
            GetRowAndColIdx(pixel_idx, &row_idx, &col_idx);
            if (row_idx == i) {
                strip.setPixelColor(pixel_idx, color);
            }
        }
        strip.show();
        delay(wait);
    }
}

// Fill the dots one after the other with a color
void circleLineChase(uint32_t color_background, uint32_t color_line, uint32_t line_width, uint8_t wait) {
    for (int i = 0; i < GetFullCircleTotal(); i++) {
        for(int pixel_idx = 0; pixel_idx < strip.numPixels(); pixel_idx++) {
            uint8_t circle_idx = GetFullCircleIndex(pixel_idx);


            if (abs(i - circle_idx) < line_width) {
                strip.setPixelColor(pixel_idx, color_line);
            } else {
                strip.setPixelColor(pixel_idx, color_background);
            }
        }

        strip.show();
        delay(wait);
    }
}

void loop() {
    colorWipeUp(strip.Color(25, 0, 0), 50); // Red
    colorWipeUp(strip.Color(0, 25, 0), 50); // Green
    colorWipeUp(strip.Color(0, 0, 25), 50); // Blue
    circleLineChase(strip.Color(0, 0, 0), strip.Color(250, 0, 0), 2, 40);
    circleLineChase(strip.Color(0, 0, 0), strip.Color(0, 250, 0), 2, 40);
    circleLineChase(strip.Color(0, 0, 0), strip.Color(0, 0, 250), 2, 40);
}

int main() {

    while (!strip.done) {
        loop();
    }
    SDL_Quit();
    return 0;
}


