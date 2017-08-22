#ifndef BURNINGMAN_ADAFRUIT_NEOPIXEL_H
#define BURNINGMAN_ADAFRUIT_NEOPIXEL_H
#include <cstdint>

class Adafruit_NeoPixel {

public:
    Adafruit_NeoPixel();
    uint32_t Color(uint8_t, uint8_t, uint8_t);
    uint16_t numPixels();
    void setPixelColor(uint16_t i, uint32_t color);
    void show();
    SDL_bool done = SDL_FALSE;
    void delay(uint8_t);
private:
    uint32_t leds[110];
    SDL_Renderer *ren;
    SDL_Event event;
};


#endif //BURNINGMAN_ADAFRUIT_NEOPIXEL_H
