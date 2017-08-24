#ifndef BURNINGMAN_ADAFRUIT_NEOPIXEL_H
#define BURNINGMAN_ADAFRUIT_NEOPIXEL_H
#include <cstdint>
typedef uint8_t byte;

class Adafruit_NeoPixel {

public:
    Adafruit_NeoPixel();
    uint32_t Color(uint8_t, uint8_t, uint8_t);
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    uint16_t numPixels() const;
    void setPixelColor(uint16_t i, uint32_t color);
    uint32_t getPixelColor(uint16_t i) const;
    void show();
    SDL_bool done = SDL_FALSE;
    void delay(uint16_t);
private:
    uint32_t leds[106];
    SDL_Renderer *ren;
    SDL_Event event;
};


#endif //BURNINGMAN_ADAFRUIT_NEOPIXEL_H
