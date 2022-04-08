void isr()
{
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5)
  {
    if (digitalRead(Dt) == HIGH) LCD_screen--;
    else  LCD_screen++;
//    wrap lcd screen around...
    if (LCD_screen > num_LCD_screens) LCD_screen = 0;
    if (LCD_screen < 0) LCD_screen = num_LCD_screens;
//    LCD_screen = constrain(LCD_screen, 1, num_LCD_screens);
  }
  lastInterruptTime = interruptTime;
}
