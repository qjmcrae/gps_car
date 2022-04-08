#include <Wire.h>

// Setup program-level stuff, i.e. timers, etc.
int disp_freq = 5;  // in hz
long disp_delay = 1000 / disp_freq;
long disp_time = 0;

//=================  Pin definitions  =================//
// try to define each pins' use
//=====================================================//
int servo_pin = 8;
int motor_pin = 3;
int joystick_x = A0;
int joystick_y = A1;
int joystick_z = A2;

//Setup LCD Screen
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_0x27 (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // set the LCD address for a 20 chars and 4 line display
LiquidCrystal_I2C lcd_0x3F (0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // set the LCD address for a 20 chars and 4 line display
LiquidCrystal_I2C lcd = lcd_0x27;



// Initialize servo stuff ...
#include <Servo.h>
Servo servo; // create servo objects to control a servo
int servo_command = 0;
int servo_left = 140;
int servo_right = 40;

///////////////////////////////////////////////////////////////////////////
//
//                                void setup
//
///////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);

  pinMode (servo_pin, OUTPUT);
  pinMode (motor_pin, OUTPUT);
  pinMode (joystick_x, INPUT);
  pinMode (joystick_y, INPUT);
  pinMode (joystick_z, INPUT_PULLUP);


  // test to see which LCD screen is attached - only ones I am aware of are 0x27 and 0x3F
  Wire.begin();
  byte address = 63; // This is 0x3F
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  if (error == 0)  // this means there is a 0x3F - if it is there, assume it is the only one there
    lcd = lcd_0x3F;

  // LCD setup
  lcd.begin ( 20, 4 );
  lcd.clear ();
  lcd.setCursor(0, 0);
  lcd.print("Servo and Motor");
  lcd.setCursor(0, 1);
  lcd.print("   Test Bed    ");
  delay(2000);  // make sure to write above for at least some time ...
  //  /* Initialise the sensor */

  // Servo setup
  servo.attach(servo_pin);
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
  int x_command = analogRead(joystick_x);
  int y_command = analogRead(joystick_y);
  int btn = digitalRead(joystick_z);

  static bool servo_rev = 0;
  if (!btn)
  {
    servo_rev = !servo_rev;
    delay(250);
  }
  Serial.print("Servo_rev = ");
  Serial.print(servo_rev);
  Serial.print("   btn  = ");
  Serial.print(btn);
  Serial.println();



  
  int servo_command = map(x_command, 0, 1023, servo_right, servo_left);
  int motor_command = map(y_command, 0, 1023, -255, 255);
  servo.write(servo_command);
  analogWrite(motor_pin, abs(motor_command));

  if (millis() > disp_time)
  {
    disp_time = millis() + disp_delay;
    lcd.clear ();
    lcd.setCursor(0, 0);
    lcd.print("Servo: ");
    lcd.print(x_command);
    lcd.print(" / ");
    lcd.print(servo_command);
    lcd.setCursor(0, 1);
    lcd.print("Motor: ");
    lcd.print(motor_command);

  }
}

