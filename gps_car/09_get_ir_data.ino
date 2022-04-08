void get_ir_data() {
  //  if GPS Car, use IR sensor, if test platform, no IR sensor exists, so just make up a distance that
  //  makes everything work.
  if (test_platform) dist_ir = 99;
  else
  {
    // Using front IR sensor, guestimate the distance to any object in front
    int tmp = analogRead(ir_pin);
    int tmp2 = (6762 / (tmp - 9)) - 4;
    dist_ir = float(tmp2) / 100.0 / 0.3048 * 12.0; // This returns the distance in inches
  }

} //End function
