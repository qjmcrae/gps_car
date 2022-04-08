//This code is meant to be called if the GPS signal isn't found
//  It gives the "Check Wriring" error on LCD
void stop_no_gps() {

  while (1)
  {
    static long t_write = 0;
    static bool write_flag = 1;
    if (millis() > t_write) //THIS HAS NO BRACKETS? What is it doing?
      if (write_flag)
      {
        lcd.clear ();
        lcd.setCursor(0, 0);
        lcd.print("No GPS");
        lcd.setCursor(0, 1);
        lcd.print("Check Wiring");
        write_flag = 0;
        t_write = millis() + 500;
      }
      else
      {
        t_write = millis() + 250;
        lcd.clear ();
        write_flag = 1;
      }
  }
} //End of stopProgram function
