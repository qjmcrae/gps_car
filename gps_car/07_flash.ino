// ************************   FLASH   ************************//
void flash(int pin, int on_time, int off_time, int high_level, int low_level, int LED)
{
  static bool pin_state[] = {0, 0, 0, 0};

  if (pin_state[LED])
  {
    if (millis() - timer[LED] > on_time)
    {
      analogWrite(pin, low_level);
      pin_state[LED] = 0;
      timer[LED] = millis();
    }
  }
  else
  {
    if (millis() - timer[LED] > off_time)
    {
      analogWrite(pin, high_level);
      timer[LED] = millis();
      pin_state[LED] = 1;
    }
  }
}

