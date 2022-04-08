//This code is meant to be called if the compass is not connected
//  It gives the "Compass NOT detected" error on LCD
void stop_no_compass() {

    lcd.clear ();
    lcd.setCursor(0, 1);
    lcd.print("Compass NOT detected");
    lcd.setCursor(0, 2);
    lcd.print("Program will not");
    lcd.setCursor(0, 3);
    lcd.print("continue ...");
    while (1)  Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");

} //End of stopProgram function
