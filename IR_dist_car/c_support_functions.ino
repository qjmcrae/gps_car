// ************************   GET_IR_DISTANCE   ************************//
// calculate IR distance, return floating point in inches
float get_ir_distance()
{
  // Using front IR sensor, guestimate the distance to any object in front
  int tmp = analogRead(ir_pin);
  int tmp2 = (6762 / (tmp - 9)) - 4;
  dist_ir = float(tmp2) / 100.0 / 0.3048 * 12.0; // This returns the distance in inches
  if (dist_ir < 0) dist_ir = 75;
  return dist_ir;
}

// ************************   SET_SPEED   ************************//
//  Set the esc speed - mapped from -100% (full brake) to +100% (full throttle)
void set_speed(int esc_speed)
{
  if (esc_speed >= 0)  // if esc_speed is +, map from 0 to 100% to neutral to full-throttle
    esc_val = constrain(map(esc_speed, 0, 100, 98, 105), 98, 105);// 98 seems to be when it starts moving
  else  // if esc_speed is -, map from -100% to 0 to full brake to 0
    esc_val = constrain(map(esc_speed, -100, 0, 20, 90), 20, 90);

  esc_servo.write(esc_val);
}

// ************************   COUNTDOWN   ************************//
// Countdown code - blink the light, beep and show the count-down before car takes off ...
void countdown()
{
  long t_stop = millis() + 3500;
  lcd.clear ();

  while (millis() < t_stop)
  {
    long now = millis();
    int light_delay = 250;
    static long light_on = 0;
    static long light_off;
    long t_minus = (t_stop - now );
    int beep_delay = map(t_minus, 0, 3000, 50, 350);   // beep faster as it gets closer to 0 ...
    static long beep_on = 0;
    static long beep_off;

    if (now > beep_on)
    {
      beep_on = now + beep_delay;
      digitalWrite(buzzer_pin, 1);
      beep_off = now + 50;  // the 50 is how long it beeps every time, the "beep_delay" is how long it waits between beeps
    }
    if (now > beep_off) digitalWrite(buzzer_pin, 0);

    lcd.setCursor(0, 0);
    lcd.print("Launching in ...");
    lcd.setCursor(3, 1);
    lcd.print("T - ");
    lcd.print(min(t_minus / 1000, 99));
    lcd.print(" Seconds..");
  }

  digitalWrite(buzzer_pin, 0);
}


// ************************   DISP_LCD_INFO   ************************//
void disp_lcd_info()
{
  // clear LCD 1st time through only
  static bool has_run = 0;
  if (has_run == 0)
  {
    lcd.clear();
    has_run = 1;
  }

  disp_time = disp_time + disp_delay;
  lcd.setCursor(0, 0);
  lcd.print("IR:                 ");
  lcd.setCursor(3, 0);
  lcd.print(dist_ir);
  lcd.print(" inches");
  lcd.setCursor(0, 1);
  lcd.print("ESC:     ");
  lcd.setCursor(4, 1);
  lcd.print(esc_val);
  lcd.print(" / ");
  lcd.print(map(esc_val, 98, 105, 0, 100));
//    esc_val = constrain(map(esc_speed, 0, 100, 98, 105), 98, 105);// 98 seems to be when it starts moving
//    esc_val = constrain(map(esc_speed, -100, 0, 20, 90), 20, 90);

}
