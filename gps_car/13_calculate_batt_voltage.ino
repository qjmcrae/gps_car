//  This code should calculate the battery voltage using the voltage divider on pin A6,
//  and cell 1 connected to pin A3.  It assume the voltage divider has values of R1 
//  and R2, defined elsewhere

void calculate_batt_voltage() 
{

  float tmp = analogRead(batt_volt_pin) * 5.0 / 1024.0;
  batt_volts_total = tmp * (R1 + R2) / R2;
  volts_cell_1 = analogRead(batt_cell_1_pin) * 5.0/1024.0;
  volts_cell_2 = batt_volts_total - volts_cell_1;

} //End of calculate_temperature function
