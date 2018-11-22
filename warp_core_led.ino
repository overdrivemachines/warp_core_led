#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif
uint8_t dataPin  = 9;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 7;    // Green wire on Adafruit Pixels
int num_leds = 28;
Adafruit_WS2801 strip = Adafruit_WS2801(num_leds, dataPin, clockPin);

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

int mode = 0;
int moving_index = -1;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 8 bytes for the inputString:
  inputString.reserve(8);
  
  strip.begin();
  // Update LED contents, to start they are all 'off'
  strip.show();
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}
void loop() 
{
  if (stringComplete) 
  {
    Serial.println(inputString);
    mode = inputString.toInt();
    // clear the string:
    inputString = "";
    stringComplete = false;    
  }

  switch(mode)
  {
    case 0:
    off();
    moving_index = -1;
    break;

    case 1:
    eject_sys();
    moving_index = -1;
    break;

    case 2:
    uint32_t blue = Color(0, 0, 255);
    if (moving_index == -1)
    {
      off();
    }
    moving_index++;
    if (moving_index >= strip.numPixels())
    {
      strip.setPixelColor(strip.numPixels() - 1, 0, 0, 0);
      moving_index = 0;
    }
    strip.setPixelColor(moving_index - 1, 0, 0, 0);
    strip.setPixelColor(moving_index, blue);
    strip.show();
    delay(200);
    break;
  }
  // put your main code here, to run repeatedly:
  // off();
  // eject_sys();
  //power_up();
  
}
///////////////////////////////////////////////
// 0. Off
///////////////////////////////////////////////
void off()
{
  for (uint8_t i = 0; i < strip.numPixels(); i++)
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


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
