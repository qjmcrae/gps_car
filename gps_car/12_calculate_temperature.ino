//This code should calculate the temperature based on the TMP36 sensor attached to "temperature_pin"
//  It returns the temperature as a floating point in °C

float calculate_temperature() 
{

  float volt = analogRead(temperature_pin) * 5.0 / 1024.0;
  float temp_C = (volt - 0.5) * 100;
  return temp_C;

} //End of calculate_temperature function
