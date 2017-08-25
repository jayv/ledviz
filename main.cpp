#include <iostream>
#include <SDL.h>
#include "Adafruit_NeoPixel.h"
#include "LedAssignments.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel();

void delay(uint16_t wait) {
    strip.delay(wait);
}

// Simulating Arduino APIs /////////////////////////////

inline int random(int max) {
    return random() % max;
}

inline unsigned long millis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

inline double abs(double val) {
    return fabs(val);
}

// ////////////////////////////////////////////////////

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

uint32_t ColorScale(uint32_t color, float scale) {
    return strip.Color(((color >> 16) & 0xff) * scale, ((color >> 8) & 0xff) * scale, (color & 0xff) * scale);
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

void rodsWipeUp() {

    Segment segments;

    for (int x=0; x <=26; x++) {

        for (int y=0; y < strip.numPixels(); y++) {
            strip.setPixelColor(y, strip.Color(150,150,150));
        }

        GetIdxForRow(x, segments);
        strip.setPixelColor(segments.al, strip.Color(0,100,200));
        strip.setPixelColor(segments.ar, strip.Color(0,100,200));
        strip.setPixelColor(segments.cl, strip.Color(0,100,200));
        strip.setPixelColor(segments.cr, strip.Color(0,100,200));
        strip.show();

        delay(100);
    }
}

void pulseBeam() {

    Segment segments;

    unsigned long start = millis();
    double progress;
    bool done = false;
    while(!done) {

        // clear rods
        for (int y=0; y < strip.numPixels(); y++) {
            strip.setPixelColor(y, strip.Color(0,0,0));
        }
        //bool bothLocked = random(10) < 4; // 40% chance of both locked to same color
        bool topLocked = random(10) < 6; // 60% chance of locked top part
        bool topOffset = random(1000) * 13; // color offsets
        int horizon = topLocked ? 15 : 13;
        double speedup = random(3) + 1;
        for (int x = 0; x < 100; x++) {
    
            double magnitude = sin(pow(speedup*x/5.0,2));
            double amagnitude = abs(magnitude);
            double magnitudeRight = sin(pow(2*x/5.0,2));
            double amagnitudeRight = abs(magnitudeRight);

            unsigned long now = millis();
            unsigned long elapsed = now - start;
            if (elapsed > 100000) done = true;
            progress = (elapsed % 9000) / 9000.0;

            double rows = amagnitude * horizon;
            double rowsRight = amagnitudeRight * horizon;
            double cutoffHead = horizon - rows;
            double cutoffTail = horizon + rows;
            double cutoffHeadRight = horizon - rowsRight;
            double cutoffTailRight = horizon + rowsRight;

            uint32_t color = strip.Color(0,0,0);

            uint32_t baseColor = Wheel((int)(255 * progress * (1 + speedup/10.0)) % 255);//Wheel(255 * progress);

            for (int y = 0; y < (topLocked ? 15 : 26); y++) {

                uint32_t leftColor = color;
                uint32_t rightColor = color;
                if (rows != 0 && y > cutoffHead && y < cutoffTail) {
                    double distScale = 1.0 - (abs(horizon-y)/rows);
                    leftColor = ColorScale(baseColor, distScale);
                }
                if (rowsRight != 0 && y > cutoffHeadRight && y < cutoffTailRight) {
                    double distScale = 1.0 - (abs(horizon-y)/rowsRight);
                    rightColor = ColorScale(baseColor, distScale);
                }

                GetIdxForRow(y, segments);
                if (y != 26) {
                    strip.setPixelColor(segments.al, leftColor);
                    strip.setPixelColor(segments.ar, leftColor);
                }
                strip.setPixelColor(segments.cl, rightColor);
                strip.setPixelColor(segments.cr, rightColor);
    
            }

            if (topLocked) {

                for (int y = 15; y < 27; y++) {

                    uint32_t leftCol = Wheel((int)(200.0 * (elapsed+topOffset)/10000) % 255);
                    uint32_t rightCol = Wheel((int)(200.0 * (elapsed+topOffset)/6666) % 255);

                    GetIdxForRow(y, segments);
                    if (y != 26) {
                        strip.setPixelColor(segments.al, leftCol);
                        strip.setPixelColor(segments.ar, leftCol);
                    }
                    strip.setPixelColor(segments.cl, rightCol);
                    strip.setPixelColor(segments.cr, rightCol);

                }

            }

            strip.show();
            delay(30+20*speedup);
    
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

//    rainbow(20);

//    rodsWipeUp();

    pulseBeam();
}

int main() {
    srandom(millis());
    while (!strip.done) {
        loop();
    }
    SDL_Quit();
    return 0;
}


