# warp_core_led
LED lights for the Warp Core

0) Off
1) Eject  - blink "all red" to "all white" 1 second interval
2) Rest   - ship engines are not engauged, slowly moving from bottom to top one pixel at a time ... 
3) Moving - ship engines are engaged, medium moving from bottom to top one pixel at a time ...
4) warp -
5) super warp -
6) red alert - 

- - - - - - -
0 1 2 3 4 5 6
lower_bound = 3
upper_bound = 3

- - - - - - - - - - -  -  -  -  -
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
lower_bound = 3
upper_bound = 11

void moving(int delay_time = 200)
{
  if (is_red_alert == true)
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
  }
  else
  {
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
  }
  
  delay(delay_time);
}