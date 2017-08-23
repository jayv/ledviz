#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Adafruit_NeoPixel.h"
#include <iostream>

uint16_t Adafruit_NeoPixel::numPixels() {
    return 106;
}

void Adafruit_NeoPixel::setPixelColor(uint16_t i, uint32_t color) {
    if (i < 0 || i > numPixels() -1) {
        std::cout << "pixel out of bounds " << std::to_string(i) << std::endl;
        return; // ignore out of bounds
    }
    leds[i] = color;
}

uint8_t r(uint32_t color) {
    return (color >> 24) & 0xff;
}
uint8_t g(uint32_t color) {
    return (color >> 16) & 0xff;
}
uint8_t b(uint32_t color) {
    return (color >> 8) & 0xff;
}

inline double radians(double degrees) {
    return (M_PI / 180) * degrees;
}

void Adafruit_NeoPixel::show() {

    if (done) return;

    SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren);

    for (uint8_t x = 0; x < 27; x++) { // Draw top to bottom

        uint8_t al = 81 + x-2; // arrow left segment
        uint8_t ar = 80 - x; // arrow right segment
        uint8_t cl = 27 + x; // cross left segment
        uint8_t cr = 26 - x; // cross right segment

        if (x != 0)  {
            thickLineRGBA(ren, 300, 100 + x * 20, 400, 100 + x * 20, 3, r(leds[al]), g(leds[al]), b(leds[al]), SDL_ALPHA_OPAQUE);
            thickLineRGBA(ren, 400, 100 + x * 20, 500, 100 + x * 20, 3, r(leds[ar]), g(leds[ar]), b(leds[ar]), SDL_ALPHA_OPAQUE);
        }

        thickLineRGBA(ren, 700, 100 + x * 20, 800, 100 + x * 20, 3, r(leds[cl]), g(leds[cl]), b(leds[cl]), SDL_ALPHA_OPAQUE);
        thickLineRGBA(ren, 800, 100 + x * 20, 900, 100 + x * 20, 3, r(leds[cr]), g(leds[cr]), b(leds[cr]), SDL_ALPHA_OPAQUE);


    }

    // Experiment with angled lines, TBC

    double radius = 300;

    int CX = 512;
    int CY = 384;

    double segments = 47;
    double angle = 360/segments;

    for (int x=0; x <= segments; x++) {

        double cx = cos(radians(angle * x)) * radius; //convert angle to radians for x and y coordinates
        double cy = sin(radians(angle * x)) * radius;
        double cx2 = cos(radians(angle * x)) * (2*radius); //convert angle to radians for x and y coordinates
        double cy2 = sin(radians(angle * x)) * (2*radius);

        thickLineRGBA(ren, cx+CX, cy+CY, CX+cx2, CY+cy2, 3, 140, 150, 50, SDL_ALPHA_OPAQUE);

    }

    SDL_RenderPresent(ren);

    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            done = SDL_TRUE;
        }
    }
}


Adafruit_NeoPixel::Adafruit_NeoPixel() {

    const int SCREEN_WIDTH = 1024;
    const int SCREEN_HEIGHT = 768;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    SDL_Window *win = SDL_CreateWindow("Burning Man 2017!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE /* | SDL_WINDOW_ALLOW_HIGHDPI */ );
    if (win == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

}

uint32_t Adafruit_NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b) {
    return (((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + SDL_ALPHA_OPAQUE);
}

void Adafruit_NeoPixel::delay(uint16_t wait) {
    if (!done) SDL_Delay(wait);
}

