#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif
uint8_t dataPin  = 9;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 7;    // Green wire on Adafruit Pixels
// Number of pixels/leds
int num_leds = 28;
Adafruit_WS2801 strip = Adafruit_WS2801(num_leds, dataPin, clockPin);

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


int mode = 0; // Mode will be set by input from serial
int moving_index = -1; // Keeps track of which pixel/led is lit
uint32_t blue; // Blue color
uint32_t red; // Red color
uint32_t white; // White color
bool is_red = false; // is the strip showing all red?
bool is_red_alert = false; // is red alert mode activated?
int lower_bound = 0; // moving led's lower bound
int upper_bound = num_leds - 1; // moving led's upper bound

void moving(int delay_time = 200);

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

  blue = Color(0, 0, 255);
  red = Color(255, 0, 0);
  white = Color(255, 255, 255);
}
void loop() 
{
  if (stringComplete) 
  {
    Serial.println(inputString);
    if (inputString.toInt() == 6)
    {
      // When red alert is activated, the mode retains its value
      is_red_alert = true;
      Serial.println("Red Alert Activated");
      if (num_leds >= 7)
      {
        // The first 3 and last 3 leds will be constantly red
        // only the inner leds will change        
        lower_bound = 3;
        upper_bound = num_leds - 4;

        // When red alert mode is activated
        // Set the first 3 and last 3 leds red
        for (uint8_t i = 0; i < lower_bound; i++)
        {
          strip.setPixelColor(i, 255, 0, 0);
          strip.setPixelColor(i + upper_bound + 1, 255, 0, 0);
        }
        strip.show();
      }
    }
    else if (inputString.toInt() == 7)
    {
      // When red alert is deactivated, the mode retains its value
      is_red_alert = false;
      Serial.println("Red Alert Deactivated");
      lower_bound = 0;
      upper_bound = num_leds - 1;
      // turn off the first 3 and last 3 leds
      for (uint8_t i = 0; i < 3; i++)
      {
        strip.setPixelColor(i, 0, 0, 0);
        strip.setPixelColor(upper_bound - i, 0, 0, 0);
      }
      strip.show();
    }
    else
    {
      if (inputString.toInt() != mode)
      {
        moving_index = -1;
        mode = inputString.toInt();
      }      
    }
    
    
    // clear the string:
    inputString = "";
    stringComplete = false;    
  }

  switch(mode)
  {
    // 0 - Off
    case 0:
    {
      off();
      lower_bound = 0;
      upper_bound = num_leds - 1;
      break;
    }
    
    // 1 - Eject
    case 1:
    {
      eject_sys();
      delay(990);
      break;
    }    

    // 2 - Rest
    case 2:
    {
      moving(200);
      break;
    }

    // 3 - Moving
    case 3:
    {
      moving(100);
      break;
    }
    

    // 4 - Warp
    case 4:
    {
      moving(50);
      break;
    }

    // 5 - Super Warp
    case 5:
    {
      moving(25);
      break;
    }
    
    default:
    {
      off();
    }
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
  if (is_red == false)
  {
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, red);
    }
    strip.show();
    is_red = true;
  }
  else
  {
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, white);
    }
    strip.show();
    is_red = false;
  }
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

void moving(int delay_time = 200)
{
  if ((moving_index == -1) || (moving_index == lower_bound) || (moving_index > upper_bound))
  {
    // Clear the strip from the lower bound to the upper bound
    for (uint8_t i = lower_bound; i <= upper_bound; i++)
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    moving_index = lower_bound;
    // turn off last pixel/led
    strip.setPixelColor(upper_bound, 0, 0, 0);
  }
  else
  {
    // turn off previous pixel/led
    strip.setPixelColor(moving_index - 1, 0, 0, 0);
  }

  // turn on moving_index pixel/led
  strip.setPixelColor(moving_index, blue);
  strip.show();
  
  // increment moving_index
  moving_index++;
  
  delay(delay_time);
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
