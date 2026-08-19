#include <Adafruit_NeoPixel.h>

#define PIN 2	 // input pin Neopixel is attached to

#define NUMPIXELS 66 // number of neopixels in strip
#define WHEEL_CENTER (NUMPIXELS / 2) // integer division automatically rounds down

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN);

uint8_t  mode   = 1, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = 0x00ff96; // Start red
uint32_t prevTime;

void setup() {
  pixels.begin();
  pixels.setBrightness(50); // 1/3 brightness
  prevTime = millis();
}

void loop() {
  uint8_t  i;
  uint32_t t;

  switch(mode) {

   case 0: // Random sparks - just one LED on at a time!
    i = random(5);
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(20);
    pixels.setPixelColor(i, 0);
    break;
 
   case 1: // Spinny wheels (8 LEDs on at a time)
    for(i=0; i<NUMPIXELS; i++) {
      uint32_t c = 0;
      if(((offset + i) & 7) < 4) c = color; // 4 pixels on...
      
            // FORWARD half
      pixels.setPixelColor(   i, c); // First eye
      // BACKWARD half (now correct)
      pixels.setPixelColor(NUMPIXELS - i, c); // Second eye (flipped and backward running)

    }
    pixels.show();
    offset--; //Decreasing offset moves the pattern backward.
    //offset++; //Increasing offset moves the pattern forward.
    delay(90);
    break;
  }

  t = millis();
  if((t - prevTime) > 8000) {      // Every 8 seconds...
    mode++;                        // Next mode
    if(mode > 1) {                 // End of modes?
      mode = 1;                    // Start modes over
      color >>= 0  ;                 // Next color R->G->B
      if(!color) color = 0x00ff96; // Reset to red
    }
    for(i=0; i<NUMPIXELS; i++) pixels.setPixelColor(i, 0);
    prevTime = t;
  }
}