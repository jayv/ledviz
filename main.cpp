#include <iostream>
#include <SDL.h>
#include "Adafruit_NeoPixel.h"
#include "LedAssignments.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel();

void delay(uint16_t wait) {
    strip.delay(wait);
}

inline int random(int max) {
    return random() % max;
}

inline unsigned long millis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int t_gamma[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
        1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
        2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
        5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
        10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
        17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
        25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
        37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
        51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
        69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
        90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
        115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
        144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
        177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
        215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
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

uint8_t red(uint32_t c) {
    return (c >> 8);
}
uint8_t green(uint32_t c) {
    return (c >> 16);
}
uint8_t blue(uint32_t c) {
    return (c);
}


void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
    float fadeMax = 100.0;
    int fadeVal = 0;
    uint32_t wheelVal;
    int redVal, greenVal, blueVal;

    for(int k = 0 ; k < rainbowLoops ; k ++){

        for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

            for(int i=0; i< strip.numPixels(); i++) {

                wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

                redVal = red(wheelVal) * float(fadeVal/fadeMax);
                greenVal = green(wheelVal) * float(fadeVal/fadeMax);
                blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

                strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

            }

            //First loop, fade in!
            if(k == 0 && fadeVal < fadeMax-1) {
                fadeVal++;
            }

                //Last loop, fade out!
            else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
                fadeVal--;
            }

            strip.show();
            delay(wait);
        }

    }



//    delay(500);


//    for(int k = 0 ; k < whiteLoops ; k ++){
//
//        for(int j = 0; j < 256 ; j++){
//
//            for(uint16_t i=0; i < strip.numPixels(); i++) {
//                strip.setPixelColor(i, strip.Color(0,0,0, t_gamma[j] ) );
//            }
//            strip.show();
//        }
//
//        delay(2000);
//        for(int j = 255; j >= 0 ; j--){
//
//            for(uint16_t i=0; i < strip.numPixels(); i++) {
//                strip.setPixelColor(i, strip.Color(0,0,0, t_gamma[j] ) );
//            }
//            strip.show();
//        }
//    }
//
//    delay(500);


}


void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {

    if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

    int head = whiteLength - 1;
    int tail = 0;

    int loops = 3;
    int loopNum = 0;

    static unsigned long lastTime = 0;

    while(true){
        for(int j=0; j<256; j++) {
            for(uint16_t i=0; i<strip.numPixels(); i++) {
                if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
                    strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
                }
                else{
                    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
                }

            }

            if(millis() - lastTime > whiteSpeed) {
                head++;
                tail++;
                if(head == strip.numPixels()){
                    loopNum++;
                }
                lastTime = millis();
            }

            if(loopNum == loops) return;

            head%=strip.numPixels();
            tail%=strip.numPixels();
            strip.show();
            delay(wait);
        }
    }

}


// fades from background color to blink color by blinking more and more rows until all
// rods have the blinking color
void randomBlinkFade(uint32_t color_background, uint32_t color_blink, uint8_t wait, uint8_t n_repeats) {
    byte i_max = 100;
    // Generate a random number between 0 and i_max for each row.
    // Increase the threshold from 0 to i_max, and give all rows with a value below the threshold the blinking color
    for (byte i_threshold = 0; i_threshold < i_max; i_threshold++) {
        for (byte j = 0; j < n_repeats; j++) {

            for (uint16_t row = 0; row < GetMaxColHeight(); row++) {

                byte i = random(i_max);

                for(uint16_t pixel_idx = 0; pixel_idx < strip.numPixels(); pixel_idx++) {
                    byte row_idx, col_idx;
                    GetRowAndColIdx(pixel_idx, &row_idx, &col_idx);

                    if (row_idx == row) {
                        if (i <= i_threshold) {
                            strip.setPixelColor(pixel_idx, color_blink);
                        } else {
                            strip.setPixelColor(pixel_idx, color_background);
                        }
                    }
                }
            }

            strip.show();
            delay(wait);
        }
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i+j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, c);    //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}


void loop() {

    // Some example procedures showing how to display to the pixels:
//    colorWipeUp(strip.Color(25, 0, 0), 50); // Red
//    colorWipeUp(strip.Color(0, 25, 0), 50); // Green
//    colorWipeUp(strip.Color(0, 0, 25), 50); // Blue
//    circleLineChase(strip.Color(0, 0, 0), strip.Color(250, 0, 0), 2, 40);
//    circleLineChase(strip.Color(0, 0, 0), strip.Color(0, 250, 0), 2, 40);
//    circleLineChase(strip.Color(0, 0, 0), strip.Color(0, 0, 250), 2, 40);
//    randomBlinkFade(strip.Color(25, 0, 0), strip.Color(0, 0, 25), 20, 20);
    //delay(500);
//    randomBlinkFade(strip.Color(0, 0, 25), strip.Color(0, 25, 0), 2

//    whiteOverRainbow(0,75,1);

//    rainbowFade2White(30,3,0);

//    rainbowCycle(20);

    rainbow(20);

}


int x = 0;
void loopLedsInSequence() {

    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255));
    }

    strip.setPixelColor(x, strip.Color(0,140,255));

    strip.show();
    delay(100);
    x = x < strip.numPixels() - 1 ? x + 1 : x = 0;
}


int main() {

    while (!strip.done) {
        loop();
//        loopLedsInSequence();
    }
    SDL_Quit();
    return 0;
}


