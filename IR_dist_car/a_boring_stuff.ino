// Setup program-level stuff, i.e. timers, etc.
int disp_freq = 2;  // in hz
long disp_delay = 1000 / disp_freq;
long disp_time = 0;

//=================  Pin definitions  =================//
// try to define each pins' use
//=====================================================//

int esc_servo_pin = 9;      // gps signal to ESC
int steering_servo_pin = 10;// gps signal to steering servo
int buzzer_pin = 12;        // Piezo electric buzzer pin

// A-pins...
int ir_pin = A2;            // analog read to determine distance with IR sensor
// pins A4,A5 used for I2C protocol, SDA to A4, SCL to A5
//SDA = A4;
//SCL = A5;
int pot_pin = A6;       // analog input for potentiometer


float dist_ir;

//Setup LCD Screen
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_0x27 (0x27, 20, 4);  // set the LCD address for a 20 chars and 4 line display
LiquidCrystal_I2C lcd_0x3F (0x3F, 20, 4);  // set the LCD address for a 20 chars and 4 line display
LiquidCrystal_I2C lcd = lcd_0x27;

// Initialize servo stuff ...
// Old, servo.h library stuff - updated so we can use softwareSerial library simultaneously
#include <Servo.h>
Servo steering_servo, esc_servo;  // create servo objects to control a servo

int  servo_write_freq = 100;  // in hz
long servo_write_delay = 1000 / servo_write_freq;
long servo_write_time = 0;
int esc_val = 90;

///////////////////////////////////////////////////////////////////////////
//
//                                void setup
//
///////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);

  pinMode (pot_pin, INPUT);
  pinMode (buzzer_pin, OUTPUT);
  pinMode (esc_servo_pin, OUTPUT);
  pinMode (steering_servo_pin, OUTPUT);


  // test to see which LCD screen is attached - only ones I am aware of are 0x27 and 0x3F
  Wire.begin();
  byte address = 63; // This is 0x3F
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  if (error == 0)  // this means there is a 0x3F - if it is there, assume it is the only one there
    lcd = lcd_0x3F;

  // Initialize LCD
  //  Serial.println(lcd.begin(20, 4));
  lcd.begin ( 20, 4 );
  lcd.setBacklight(HIGH);
  lcd.clear ();
  lcd.setCursor(0, 0);
  lcd.print("IR Distance Race");
  lcd.setCursor(0, 1);
  lcd.print(team_name);
  lcd.setCursor(0, 2);
  lcd.print(sub_name);
//  lcd.setCursor(0, 3);
//  lcd.print(course_name);
  delay(4000);  // make sure to write above for at least some time ...

  // Servo setup
  steering_servo.attach(steering_servo_pin);
  esc_servo.attach(esc_servo_pin);
  steering_servo.write(90);


  countdown();



}    // end setup
// ************************   END SETUP   ************************//
