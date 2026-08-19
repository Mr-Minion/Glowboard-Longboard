#include <Adafruit_NeoPixel.h>

#define PIN 2	 // input pin Neopixel is attached to

#define NUMPIXELS 66 // number of neopixels in strip
#define WHEEL_CENTER (NUMPIXELS / 2) // integer division automatically rounds down

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN);

uint8_t  mode   = 1, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = 0x00ff96; // Start red
uint32_t PRIDE_RED     = pixels.Color(255, 0, 0);
uint32_t PRIDE_ORANGE  = pixels.Color(255, 127, 0);
uint32_t PRIDE_YELLOW  = pixels.Color(255, 255, 0);
uint32_t PRIDE_GREEN   = pixels.Color(0, 255, 0);
uint32_t PRIDE_BLUE    = pixels.Color(0, 0, 255);
uint32_t PRIDE_PURPLE  = pixels.Color(148, 0, 211);
uint32_t prevTime;

uint32_t prideColors[] = {
  pixels.Color(255, 0, 0),      // Red
  pixels.Color(255, 127, 0),    // Orange
  pixels.Color(255, 255, 0),    // Yellow
  pixels.Color(0, 255, 0),      // Green
  pixels.Color(0, 0, 255),      // Blue
  pixels.Color(148, 0, 211)     // Purple
};

uint8_t prideIndex = 0;


void setup() {
  pixels.begin();
  pixels.setBrightness(50); // 1/3 brightness
  prevTime = millis();
}

void loop() {
  uint8_t  i;
  uint32_t t;

  switch(mode) {
 
case 1: // Spinny wheels (8 LEDs on at a time)
    for(i=0; i<NUMPIXELS; i++) {
      uint32_t c = 0;
      if(((offset + i) & 7) < 4) c = color; // 4 pixels on...

      // FORWARD half
      pixels.setPixelColor(i, c);

      // BACKWARD half (now correct)
      pixels.setPixelColor((NUMPIXELS - 1) - i, c);
    }
    pixels.show();
    offset--; //Decreasing offset moves the pattern backward.
    //offset++; //Increasing offset moves the pattern forward.
    delay(90);
    break;
  }

  t = millis();
  if((t - prevTime) > 300) {      // Every 8 seconds...
    mode++;                        // Next mode
    if(mode > 1) {                 // End of modes?
      mode = 1;                    // Start modes over
      color >>= 0  ;                 // Next color R->G->B

// Cycle through PRIDE colors
prideIndex++;
if (prideIndex >= 6) prideIndex = 0;
color = prideColors[prideIndex];


    }
    for(i=0; i<NUMPIXELS; i++) pixels.setPixelColor(i, 0);
    prevTime = t;
  }
}