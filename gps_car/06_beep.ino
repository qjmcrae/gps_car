// ************************   BEEP   ************************//
void beep()
{
  beeped = 1;
  digitalWrite(buzzer_pin, 1);
  delay(1000);
  digitalWrite(buzzer_pin, 0);
  delay(250);
  for (int i = 1; i < 5; i++)
  {
    digitalWrite(buzzer_pin, 1);
    delay(250);
    digitalWrite(buzzer_pin, 0);
    delay(250);
  }
}


