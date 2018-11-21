#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif
uint8_t dataPin  = 9;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 7;    // Green wire on Adafruit Pixels
int num_leds = 28;
Adafruit_WS2801 strip = Adafruit_WS2801(num_leds, dataPin, clockPin);
void setup() {
  // put your setup code here, to run once:
  strip.begin();
  // Update LED contents, to start they are all 'off'
  strip.show();
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  // off();
  // eject_sys();
  power_up();
  
}
///////////////////////////////////////////////
// 0. Off
///////////////////////////////////////////////
void off()
{
  for (uint8_t i; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}
///////////////////////////////////////////////
// 1. Warp Core Eject
///////////////////////////////////////////////
// Red to white then white to red. Sudden change in color.
void eject_sys()
{
  uint8_t i;
  uint32_t color = Color(255, 0, 0); // Red
  for (i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(990);
  color = Color(255, 255, 255); // White
  for (i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(990);
}
///////////////////////////////////////////////
// 2. Warp power up
///////////////////////////////////////////////
// Pulse and fade from dark blue to bright blue
void power_up()
{
  uint8_t t = 40;
  colorWipeIndividual(Color(0, 0, 0), t);
  colorWipeIndividual(Color(0, 0, 1), t);
  colorWipeIndividual(Color(0, 0, 25), t);
  colorWipeIndividual(Color(0, 0, 120), t);
  colorWipeIndividual(Color(0, 0, 255), t);
}
// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint16_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
void colorWipeIndividual(uint32_t c, uint16_t wait)
{
  for (int i = 0; i < strip.numPixels(); i++) 
  {
    off();
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
