// ************************   COUNTDOWN   ************************//
// Countdown code - blink the light, beep and show the count-down before car takes off ...
void countdown()
{
  digitalWrite(red_led_pin, LOW);
  digitalWrite(blue_led_pin, LOW);

  long t_stop = millis() + 10500;
  lcd.clear ();

  while (millis() < t_stop)
  {
    long now = millis();
    int light_delay = 250;
    static long light_on = 0;
    static long light_off;
    long t_minus = (t_stop - now );
    int beep_delay = map(t_minus, 0, 10000, 50, 350);   // beep faster as it gets closer to 0 ...
    static long beep_on = 0;
    static long beep_off;

    if (now > light_on)
    {
      light_on = now + 2 * light_delay;
      digitalWrite(green_led_pin, 1);
      light_off = now + light_delay;
    }

    if (now > light_off) digitalWrite(green_led_pin, 0);

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

