///////////////////////////////////////////////////////////////////////////
//
//                                void setup
//
///////////////////////////////////////////////////////////////////////////
void setup()
{
  ss.begin(9600);

//  pinMode (pot_pin, INPUT);
  pinMode (red_led_pin, OUTPUT);
  pinMode (green_led_pin, OUTPUT);
  pinMode (blue_led_pin, OUTPUT);
  pinMode (buzzer_pin, OUTPUT);
  pinMode (esc_servo_pin, OUTPUT);
  pinMode (steering_servo_pin, OUTPUT);
  pinMode (test_platform_pin, INPUT_PULLUP);
  pinMode (Clk, INPUT);
  pinMode (Dt, INPUT);
  pinMode (Sw, INPUT_PULLUP);
  pinMode (temperature_pin, INPUT);
  pinMode (batt_cell_1_pin, INPUT);
  pinMode (batt_volt_pin, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(Clk), isr, LOW);

  // check for student entered craziness and fix
  min_dist_to_tgt = constrain(min_dist_to_tgt, 1, 10);    // in case students do anything crazy!
  delay_at_target = constrain(delay_at_target, 1, 10);    // same
  servo_wag_speed = constrain(servo_wag_speed, 100, 500);  // same

  // Determine if we are in car or test platform, based on test_platform_pin
  test_platform = digitalRead(test_platform_pin);  // ground pin 8 on car to indicate test platform

  //  Set compass offset based on car vs. test platform
  compass_offset = compass_offset_car;
  if (test_platform) compass_offset = compass_offset_platform;

  // test to see which LCD screen is attached - only ones I am aware of are 0x27 and 0x3F
  Wire.begin();
  byte address = 63; // This is 0x3F
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  if (error == 0)  // this means there is a 0x3F - if it is there, assume it is the only one there
    lcd = lcd_0x3F;

  // Initialize LCD
  //  Serial.println(lcd.begin(20, 4));
  lcd.init ();
  lcd.begin ( 20, 4 );
  lcd.setBacklight(HIGH);
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
  delay(2000);  // make sure to write above for at least some time ...
  
  //  /* Initialize the sensor */
  if (!mag.begin()) // This seems to be checking if you called begin ...  There was a problem detecting the HMC5883 ... check your connections
  stop_no_compass();

  disp_time = millis() + 1500;   // Don't change this display for extra 1.5 seconds
  LCD_screen = 1;  // go to the next LCD Screen

  // Servo setup
  steering_servo.attach(steering_servo_pin);
  esc_servo.attach(esc_servo_pin);
}    // end setup
// ************************   END SETUP   ************************//




///////////////////////////////////////////////////////////////////////////
//
//                                void loop
//
///////////////////////////////////////////////////////////////////////////
// ************************   BEGIN LOOP   ************************//
void loop()
{
  float target_lat = target_lats[ind_gps];
  float target_lon = target_longs[ind_gps];
  int esc_forward = esc_stop;
  int esc_brake = esc_stop;
  if (armed)
  {
    //    esc_forward = esc_slow_grass;
    esc_forward = esc_slow_pavement;
    esc_brake = esc_full_reverse;
  }

  // if gps not there after 10 seconds, stop the program ...
  if (millis() > 10000 && gps.charsProcessed() < 10) stop_no_gps();  // Stop the program, display "Check Wiring" error

  // get distance from IR sensor
  get_ir_data();

  // encode new information
  //  software serial method ...
  while (ss.available() > 0)
  {
    gps.encode(ss.read());
  }

  // get compass data ...
  
  get_compass_data(target_lat, target_lon);

  // check if have GPS lock yet - if not, let user know
  if (!gps.location.isValid())
  {
    flash(red_led_pin, 250, 250, 255, 0, R);   // flash red lights until GPS acquired
  }
  else   // have gps
  {
    if (beeped != 1) beep();  // beep first time it acquires GPS

    //  update servo command every so often ...
    if (millis() > servo_write_time)
    {
      static long brake_time;

      if (abs(dist_to_target)  < min_dist_to_tgt)  // check if we're there!
      {
        ind_gps++;
        if (!armed)  // 1st target - arm after "getting in the box"
        {
          armed = 1;
          // arming warning ...
          for (int i = 0; i < 5; i++)
          {
            steering_servo.write(servo_left);
            delay(100);
            steering_servo.write(servo_right);
            delay(100);
          }
          steering_servo.write(servo_straight);
          countdown();  // blink, beep, display countdown ...
        }

        else   // Armed, or in other words, going for it and found target ...
        {
          if (throttle_command >= 90) esc_servo.write(esc_full_reverse);   //make sure not adding to reverse
          long time_delay = millis() + delay_at_target * 1000; // define how long to wait at target before moving on to next target
          while (millis() < time_delay)
          {
            steering_servo.write(servo_right);
            digitalWrite(buzzer_pin, 1);
            delay(servo_wag_speed);
            steering_servo.write(servo_left);
            digitalWrite(buzzer_pin, 0);
            delay(servo_wag_speed);
          }
          steering_servo.write(servo_straight);
          digitalWrite(buzzer_pin, 0);
          esc_servo.write(esc_stop);

          if (ind_gps == num_gps_tgts)    // This means it got to the last one, so STOP!!!!
          {
            lcd.begin ( 20, 4 );
            lcd.clear ();
            lcd.setCursor(0, 1);
            lcd.print("COURSE COMPLETE!!!");
            esc_servo.write(esc_stop);
            for (int i = 1; i < 5; i++)
            {
              steering_servo.write(servo_right);
              delay(100);
              steering_servo.write(servo_left);
              delay(100);
            }
            steering_servo.write(servo_straight);
            while (1)   // make sure you never go back!!!
            {
              flash(red_led_pin,   random(50, 250), random(50, 250), 0, 255, R);
              flash(green_led_pin, random(50, 250), random(50, 250), 0, 255, G);
              flash(blue_led_pin,  random(50, 250), random(50, 250), 0, 255, B);
            }
          }
          servo_command = servo_straight;
          throttle_command = esc_stop;
          steering_servo.write(servo_command);
          esc_servo.write(throttle_command);
          servo_write_time = servo_write_time + servo_write_delay;

          digitalWrite(red_led_pin,   LOW);
          digitalWrite(green_led_pin, LOW);
          for (int i = 0; i < 10; i++)    // flash blue when at target
          {
            digitalWrite(blue_led_pin,  HIGH);
            delay(100);
            digitalWrite(blue_led_pin,  LOW);
            delay(100);
            //  flash(led_left, 100, 100, 0, 255, RIGHT);
            //  flash(led_right,  100, 100, 255, 0, LEFT);
          }  // end flash blue lights
        }  // end Armed / not armed
      }  // end within 2m of target ...

      else if (dist_ir < 20 && dist_ir > 2)   // check for stuff in the way ...
        //      else if (dist_ir < 5 && dist_ir > 2)   // check for stuff in the way ...
      { // if we came close to something, 1st hit the brakes for 2 seconds, then just set to 0 ...
        int flash_on;
        servo_command = servo_straight;
        if (millis() > brake_time)
        {
          throttle_command = esc_stop;
          servo_command = servo_straight;
          flash_on = 255;    // dim flashing to tell under brakes
        }
        else
        {
          throttle_command = esc_brake;
          servo_command = servo_straight;
          flash_on = 30;  // bright flashing to tell just sittin' there
        }
        flash(red_led_pin,   250, 150, 0, flash_on, R);  // flash lights on/off in Purple (red and blue)
        // flash(green_led_pin, 250, 150, 0, flash_on, G);
        digitalWrite(green_led_pin, LOW);
        flash(blue_led_pin,  250, 150, 0, flash_on, B);
      }

      else   // steer to gps target, lights on white
      {
        int kp = 1;  // default to gain of 1 for GPS Car
        if (test_platform) kp = -1;  // if test platform (With goofy servo's), reverse servo direction
        servo_command = constrain(servo_straight - kp * heading_error, servo_left, servo_right);
        throttle_command = esc_forward;
        brake_time = millis() + 2000;
        digitalWrite(red_led_pin,   HIGH);
        digitalWrite(green_led_pin, HIGH);
        digitalWrite(blue_led_pin,  HIGH);

        //  beep progressively quicker as we get closer - this may not work given the delay of how often this is called ...
        int beep_delay = constrain(map(dist_to_target, 0, 40, 50, 5000), 0, 5000); // beep faster as it gets closer to the target
        long now = millis();
        static long beep_on = 0;
        static long beep_off;
        if (now > beep_on)
        {
          beep_on = now + beep_delay;
          digitalWrite(buzzer_pin, 1);
          beep_off = now + 40;  // the 40 is how long it beeps every time, the "beep_delay" is how long it waits between beeps
        }
        if (now > beep_off) digitalWrite(buzzer_pin, 0);


      }

      steering_servo.write(servo_command);
      esc_servo.write(throttle_command);
      servo_write_time = servo_write_time + servo_write_delay;
    }
  }    // end have gps

  if (millis() > disp_time) disp_lcd_info();      // display info to LCD screen

}  // end loop
// ************************   END LOOP   ************************//
