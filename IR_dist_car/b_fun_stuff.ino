

void loop()
{
  // 2 functions:
  //  1 - get_ir_distance - sets variable "dist_ir" based on IR sensor
  //  2 - set_speed - sets speed from -100% (full brake) to +100% (full forward)
  //  use these 2 functions in any order you want to get as close to the wall
  //    as you can without touching

  get_ir_distance();
  set_speed(75);

  if (dist_ir < 20) set_speed (35);
  if (dist_ir < 10) set_speed(-100);




  if (millis() > disp_time) disp_lcd_info();      // display info to LCD screen

}  // end loop
// ************************   END LOOP   ************************//
