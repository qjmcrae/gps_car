// ************************   DISP_LCD_INFO   ************************//
void disp_lcd_info()
{
  // clear LCD 1st time through only
  //  static bool has_run = 0;
  static int LCD_screen_old = -1;
  //  if (has_run == 0)
  //  {
  //    lcd.clear();
  //    has_run = 1;
  //  }
  disp_time = disp_time + disp_delay;

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  //                     stuff that is on many screens                       //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////

  // estimate how far traveled total...
  static int dist_traveled = 0;
  if (millis() > calc_dist_time)
  {
    static float old_lat;
    static float old_lon;
    calc_dist_time = calc_dist_time + calc_dist_delay;
    if (gps.location.isValid() && old_lat != 0 && old_lon != 0)
      dist_traveled = dist_traveled + gps.distanceBetween(gps.location.lat(), gps.location.lng(), old_lat, old_lon);
    old_lat = gps.location.lat();
    old_lon = gps.location.lng();
  }

  //figure out GPS Date/Time for many of the different screens:
  int gps_month = gps.date.month();
  int gps_day = gps.date.day();
  int gps_year = gps.date.year() - 2000;  // put it in a shorter format
  int gps_hour = gps.time.hour();
  int gps_minute = gps.time.minute();
  int gps_second = gps.time.second();
  int ampm = 0;
  //    Knowing that Salt Lake is 6-7 hours behind the standard GPS time (depending on DST) ...
  if (gps_hour < 6) gps_day = gps_day - 1;
  gps_hour = gps_hour - 6;
  if (gps_hour < 0) gps_hour = gps_hour + 24;
  if (gps_hour > 12)
  {
    gps_hour = gps_hour - 12;
    ampm = 1;
  }
  char gps_time[9];
  char gps_date[11];
  sprintf(gps_time, "%2d:%02d:%02d", gps_hour, gps_minute, gps_second);
  sprintf(gps_date, "%0d/%0d/%0d", gps_month, gps_day, gps_year);

  // calculate GPS-calculated speed
  char gps_speed[3];
  sprintf(gps_speed, "%2d", round(gps.speed.mph()));

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  //                   Start doing screen-specific stuff                     //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////

  if (LCD_screen == 0)  //    Title screen
  {
    if (LCD_screen_old != LCD_screen) lcd.clear();
    LCD_screen_old = LCD_screen;
    lcd.clear ();
    lcd.setCursor(0, 0);
    if (test_platform) lcd.print(" GPS Test Platform ");
    else lcd.print("   GPS Guided Car ");
    lcd.setCursor(0, 1);
    lcd.print(team_name);
    lcd.setCursor(0, 2);
    lcd.print(sub_name);
    lcd.setCursor(0, 3);
    lcd.print(course_name);
  }
  else if (LCD_screen == 1)  // Main Screen
  {
    if (LCD_screen_old != LCD_screen) lcd.clear();
    LCD_screen_old = LCD_screen;

    //  if GPS not valid, blink "Acquiring GPS"
    lcd.setCursor(0, 0);
    if (gps.location.isValid())    // valid GPS - just display location
    {
      lcd.print(gps_speed);
      lcd.print("mph ");

      lcd.print("Alt ");
      lcd.print(round(gps.altitude.feet()));

      lcd.print(" D ");
      lcd.print(dist_traveled);

    }
    else   // invalid gps - blink "Acquiring GPS..."
    {
      blink_acquiring();
    }
    lcd.setCursor(0, 1);
    lcd.print("IR:      ");
    lcd.setCursor(3, 1);
    lcd.print(dist_ir);

    lcd.setCursor(8, 1);
    if (gps.location.isValid())    // valid GPS - just display location
    {
      if (ind_gps == 0)
      {
        lcd.print(" Starting Box");
      }
      else
      {
        lcd.print(" Tgt ");
        lcd.print(ind_gps + 1);
        lcd.print("/");
        lcd.print(num_gps_tgts);
      }
    }

    // clear heading and gps data, so i don't have to clear the whole line every time
    lcd.setCursor(0, 2);
    lcd.print("Hdg ");
    int i = 4; if (compass_heading < 0) i = i - 1;
    lcd.setCursor(4, 2);
    lcd.print("    ");
    lcd.setCursor(i, 2);
    lcd.print(compass_heading);
    lcd.setCursor(8, 2);
    lcd.print("GPS ");
    i = 12; if (gps_heading < 0) i = i - 1;
    lcd.setCursor(12, 2);
    lcd.print("    ");
    lcd.setCursor(i, 2);
    lcd.print(gps_heading);
    lcd.setCursor(15, 2);
    lcd.print("Sat");
    lcd.print(gps.satellites.value());

    lcd.setCursor(0, 3);
    lcd.print("Dist");
    lcd.setCursor(5, 3);
    lcd.print("    ");
    lcd.setCursor(5, 3);
    lcd.print(constrain(dist_to_target, -5, 99));
    lcd.print("m");
    lcd.setCursor(8, 3);

    lcd.setCursor(10, 3);
    lcd.print(gps_time);
    lcd.print(ampm ? "pm" : "am");
  }
  else if (LCD_screen == 2)
  {
    //   01234567890123456789
    //0  GPS Info:
    //1  Acquiring GPS...       or...
    //1  XXmph Alt:XXXXft
    //2
    //3  Sat:XX    hh:mm:sspm
    if (LCD_screen_old != LCD_screen) lcd.clear();
    LCD_screen_old = LCD_screen;
    lcd.setCursor(0, 0);
    if (gps.location.isValid())    // valid GPS - just display location
    {
      lcd.print(gps_speed);
      lcd.print("mph ");

      lcd.print("Alt ");
      lcd.print(round(gps.altitude.feet()));

    }
    else   // invalid gps - blink "Acquiring GPS..."
    {
      blink_acquiring();
    }

    lcd.setCursor(8, 1);
    if (gps.location.isValid())    // valid GPS - just display location
    {
      if (ind_gps == 0)
      {
        lcd.print(" Starting Box");
      }
      else
      {
        lcd.print(" Tgt ");
        lcd.print(ind_gps + 1);
        lcd.print("/");
        lcd.print(num_gps_tgts);
      }
    }

    // clear heading and gps data, so i don't have to clear the whole line every time
    lcd.setCursor(0, 2);
    lcd.print("Hdg ");
    int i = 4; if (compass_heading < 0) i = i - 1;
    lcd.setCursor(4, 2);
    lcd.print("    ");
    lcd.setCursor(i, 2);
    //    lcd.print(compass_heading);
    lcd.setCursor(8, 2);
    lcd.print("GPS ");
    i = 12; if (gps_heading < 0) i = i - 1;
    lcd.setCursor(12, 2);
    lcd.print("    ");
    lcd.setCursor(i, 2);
    lcd.print(gps_heading);
    lcd.setCursor(15, 2);
    lcd.print("Sat");
    lcd.print(gps.satellites.value());

    lcd.setCursor(0, 3);
    lcd.print("Dist");
    lcd.setCursor(5, 3);
    lcd.print("    ");
    lcd.setCursor(5, 3);
    lcd.print(constrain(dist_to_target, -5, 999));
    lcd.print("m");
    lcd.setCursor(8, 3);

    lcd.setCursor(10, 3);
    lcd.print(gps_time);
    lcd.print(ampm ? "pm" : "am");

  }
  else if (LCD_screen == 3)
  {
    //   01234567890123456789
    //0  Tgt Lat: ##.######
    //1  GPS Lat: ##.######
    //2  Tgt Lng:-###.######
    //3  GPS Lng:-###.######
    if (LCD_screen_old != LCD_screen) lcd.clear();
    LCD_screen_old = LCD_screen;

    lcd.setCursor(0, 0);
    lcd.print("Tgt ");
    lcd.print(ind_gps + 1);
    lcd.print("/");
    lcd.print(num_gps_tgts);
    lcd.print(": ");
    lcd.print(target_lats[ind_gps], 6);
    lcd.setCursor(0, 1);
    lcd.print("GPS Lat: ");
    lcd.print(gps.location.lat(), 6);
    lcd.setCursor(0, 2);
    lcd.print("Tgt Lng:");
    lcd.print(target_longs[ind_gps], 6);
    lcd.setCursor(0, 3);
    lcd.print("GPS Lng:");
    lcd.print(gps.location.lng(), 6);
  }
  else if (LCD_screen == 4)  // radio information
  {
    //   01234567890123456789
    //0     Servo/ESC Info
    //1  ESC Cmd: ###
    //2  Steer Cmd: ###
    //3  Heading Error:  ###
    if (LCD_screen_old != LCD_screen) lcd.clear();
    LCD_screen_old = LCD_screen;

    lcd.setCursor(0, 0);
    lcd.print("   Servo/ESC Info   ");
    lcd.setCursor(0, 1);
    lcd.print("ESC Cmd:            ");
    lcd.setCursor(10, 1);
    lcd.print(throttle_command);
    lcd.setCursor(0, 2);
    lcd.print("Steer Cmd:          ");
    lcd.setCursor(10, 2);
    lcd.print(servo_command);
    lcd.setCursor(0, 3);
    lcd.print("Heading Error:      ");
    int i = 14; if (heading_error < 0) i = i - 1;
    lcd.setCursor(i, 3);
    lcd.print(heading_error);

  }
  else
  {
    if (LCD_screen_old != LCD_screen) lcd.clear();
    LCD_screen_old = LCD_screen;
    lcd.setCursor(0, 1);
    lcd.print("**Undefined Screen**");
    lcd.setCursor(5, 2);
    lcd.print(LCD_screen);
  }

  //  lcd.print("Esc");
  //  lcd.setCursor(11, 3);
  //  lcd.print("    ");
  //  lcd.setCursor(11, 3);
  //  lcd.print(throttle_command);
  //  lcd.setCursor(15, 3);
  //  lcd.print("SV");
  //  lcd.setCursor(17, 3);
  //  lcd.print("   ");
  //  lcd.setCursor(17, 3);
  //  lcd.print(servo_command);
}



void blink_acquiring()
{
  static bool acq_disp_flag = 0;
  if (acq_disp_flag)
  {
    lcd.print("                ");
    acq_disp_flag = 0;
  }
  else
  {
    lcd.print("Acquiring GPS...");
    acq_disp_flag = 1;
  }
}
