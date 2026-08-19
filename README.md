Neopixel Longboard Project
Inspiration and source code originated from the the project **Trinket NeoPixel LED Longboard**, created by Ruiz Brothers
https://learn.adafruit.com/trinket-neopixel-led-longboard/overview

**Improvement alterations**:
1) Controller: Switching from Adafruit Trinket - Mini Microcontroller - 5V Logic to Arduino UNO.
  Connection to Adafruit Trinket - Mini Microcontroller - 5V Logic with USB 2.0 is no longer supported. No Serial Monitor available in Adafruit Trinket that is required for the NFC reader part.
2) Powerboost 500C and LiPo Battery are replaced with a Powerbank that has an output of 5.0V. Therefore no Powerboost 500C is needed anymore
3) On/Off button is replaced by a NFC reader and NFC tags to switch colors and sequences.

**Codes**:
1) The codes created are listed in the evolving pattern that they occurred
2) Longboard_green_flow.ino
   This is the adjusted code from the project Trinket NeoPixel LED Longboard adjusted for Arduino UNO
3) Longgboard_SwissFlag.ino
   Created for Swiss National day. Change two-color to match your requirement
4) Longboard_PRIDE_RAINBOW.ino
   For PRIDE or Fasnacht/Fasching/German Carnival or Party on the Longboard
5) NFC_tag_ID_identifier.ino
   Identify the ID of your NFC tag in order to use in in Code 7.
7) NFC_switch_mode.ino
   Changing the mode and colors through NFC tags

**Circuit setup**
Arduino UNO  -  Neopixel LED strip
1.       5V  -  5V
        GND  -  GND
        PIN 2  -  DIN

