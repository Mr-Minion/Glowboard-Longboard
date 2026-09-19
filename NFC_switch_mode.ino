#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>

#define PIN 2	 // input pin Neopixel is attached to

#define NUMPIXELS 66 // number of neopixels in strip
#define WHEEL_CENTER (NUMPIXELS / 2) // integer division automatically rounds down
#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN);

uint8_t  mode   = 1, // Current animation effect
         offset = 0; // Position of spinny eyes
// uint32_t color  = 0x00ff96; // Start red
uint32_t GREEN_FLOW = pixels.Color(0, 255, 150);
uint32_t TSCHUGGER_ROT  = pixels.Color(255, 0, 0);
uint32_t color = TSCHUGGER_ROT;   // start with red
uint32_t TSCHUGGER_BLAU = pixels.Color(0, 0, 255);
uint32_t SWISS_WHITE = pixels.Color(255, 255, 255);
uint32_t prevTime;

// PRIDE colors
uint32_t PRIDE_RED     = pixels.Color(255, 0, 0);
uint32_t PRIDE_ORANGE  = pixels.Color(255, 127, 0);
uint32_t PRIDE_YELLOW  = pixels.Color(255, 255, 0);
uint32_t PRIDE_GREEN   = pixels.Color(0, 255, 0);
uint32_t PRIDE_BLUE    = pixels.Color(0, 0, 255);
uint32_t PRIDE_PURPLE  = pixels.Color(148, 0, 211);

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
//   pixels.setBrightness(20); // 1/3 brightness
  prevTime = millis();
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  uint8_t  i;
  uint32_t t;

  // --- READ RFID TAGS HERE ---
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      String tag = "";
      for (byte i = 0; i < rfid.uid.size; i++) {
          tag += String(rfid.uid.uidByte[i], HEX);
      }
// write the letters of the UID all in lowercase!
      if (tag == "c678167e") mode = 1; // Green flow for blue tag
      if (tag == "8b308261") mode = 2; // Tschugger - white card
      if (tag == "4e6642f27580") mode = 3; // Swiss Cross - by Lonza
      // NFC Stickers
      if (tag == "1df9c699211080") mode = 1; // Green flow
      if (tag == "1d61d299211080") mode = 2; // Tschugger
      if (tag == "1da1d299211080") mode = 4; // Rainbow
      if (tag == "1d7cb399211080") mode = 5; // OFF
  }

  switch(mode) {

// ---------------------------------------------------------
// CASE 1 — GREEN FLOW (uses green color only)
// ---------------------------------------------------------
   case 1: // Green Flow
    for(i = 0; i < NUMPIXELS; i++) {
        uint32_t c = 0;

        // Correct 4‑on / 4‑off pattern
        if(((offset + i) & 7) < 4) c = GREEN_FLOW;

        // Forward half
        pixels.setPixelColor(i, c);

        // Backward half (mirrored)
        pixels.setPixelColor(NUMPIXELS - i, c);
    }
    pixels.setBrightness(50);   // adjust freely
    pixels.show();
    offset--;   // backward movement
    delay(90); //lower delay number == faster; higher delay number == slower
    break;

// ---------------------------------------------------------
// CASE 2 — TSCHUGGER FLOW (now uses SAME alternating color)
// ---------------------------------------------------------
case 2:  // Tschugger flow
    for(i = 0; i < NUMPIXELS; i++) {

        uint8_t phase = (offset + i) & 15;   // 16‑pixel cycle
        uint32_t c = 0;

if (phase < 4) c = color;
else if (phase < 8) c = 0;
else if (phase < 12) c = color;
else c = 0;

        // if (phase < 4) c = TSCHUGGER_BLAU;        // BLUE
        // else if (phase < 8) c = 0;                // OFF
        // else if (phase < 12) c = TSCHUGGER_ROT;   // RED
        // else c = 0;                               // OFF

        pixels.setPixelColor(i, c);
        pixels.setPixelColor(NUMPIXELS - i, c);
    }
    pixels.setBrightness(150);   // adjust freely
    pixels.show();
    offset--;
    delay(90);
    break;

// ---------------------------------------------------------
// CASE 3 — SWISS CROSS (white + red)
// ---------------------------------------------------------
case 3:  // Tschugger flow
    for(i = 0; i < NUMPIXELS; i++) {

        uint8_t phase = (offset + i) & 15;   // 16‑pixel cycle
        uint32_t c = 0;

        if (phase < 4) c = SWISS_WHITE;        // BLUE
        else if (phase < 8) c = 0;                // OFF
        else if (phase < 12) c = TSCHUGGER_ROT;   // RED
        else c = 0;                               // OFF

        pixels.setPixelColor(i, c);
        pixels.setPixelColor(NUMPIXELS - i, c);
    }
    pixels.setBrightness(10);   // adjust freely
    pixels.show();
    offset--;
    delay(90);
    break;

// ---------------------------------------------------------
// CASE 4 — Rainbow colors
// ---------------------------------------------------------
// ---------------------------------------------------------
// CASE 4 — PRIDE RAINBOW FLOW
// ---------------------------------------------------------
case 4:
{
    static uint32_t prideTimer = 0;   // independent timer for rainbow cycling
    uint32_t now = millis();

    // Change color every 400 ms (adjust if needed)
    if (now - prideTimer > 200) {
        prideIndex++;
        if (prideIndex >= 6) prideIndex = 0;
        prideTimer = now;
    }

    uint32_t prideColor = prideColors[prideIndex];

    for (i = 0; i < NUMPIXELS; i++) {
        uint32_t c = 0;

        // 4‑on / 4‑off pattern
        if (((offset + i) & 7) < 4)
            c = prideColor;

        // Forward half
        pixels.setPixelColor(i, c);

        // Backward half (mirrored)
        pixels.setPixelColor(NUMPIXELS - i, c);
    }

    pixels.setBrightness(80);   // adjust freely
    pixels.show();
    offset--;                   // movement
    delay(90);
}
break;

// ---------------------------------------------------------
// CASE 5 — OFF
// ---------------------------------------------------------
    case 5:  // OFF mode
    pixels.clear();
    pixels.show();
    break;
  }


t = millis();

// ---------------------------------------------------------
// CASE 1 — no color switching (GREEN stays constant)
// ---------------------------------------------------------
if (mode == 1) {
    // Nothing here — case 1 does not alternate colors
}

// ---------------------------------------------------------
// CASE 2 — alternate RED ↔ BLUE every X ms
// ---------------------------------------------------------
static uint32_t prevTimeCase2 = 0;   // independent timer for case 2

if (mode == 2) {
    if ((t - prevTimeCase2) > 500) {   // <<< adjust this value

        if (color == TSCHUGGER_ROT)
            color = TSCHUGGER_BLAU;
        else
            color = TSCHUGGER_ROT;

        prevTimeCase2 = t;
    }
}

// ---------------------------------------------------------
// CASE 3 — alternate RED ↔ BLUE every Y ms
// ---------------------------------------------------------
static uint32_t prevTimeCase3 = 0;   // independent timer for case 3

if (mode == 3) {
    if ((t - prevTimeCase3) > 1000) {   // <<< adjust this value


        prevTimeCase3 = t;
    }
}
}