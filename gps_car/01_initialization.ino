// This version is to match the hardware of v7.0 - Updated battery monitor
// and temperature sensor

// Arduino Code for GPS Car
// Slick Science SSAV (Small Scale Autonomous Vehicle)

// Setup program-level stuff, i.e. timers, etc.
int disp_freq = 2;  // in hz
long disp_delay = 1000 / disp_freq;
long disp_time = 0;

//float calc_dist_freq = 0.5;  // in hz
//long calc_dist_delay = 1000 / calc_dist_freq;
//float calc_dist_freq = 0.5;  // in hz
long calc_dist_delay = 2000;
long calc_dist_time = 0;

//=================  Pin definitions  =================//
// try to define each pins' use
//=====================================================//
// D-pins ...
// Pins D0, D1, D7 unused
int Clk = 2;                // Encoder Clk pin
int Dt = A0;                // Encoder DT pin
int Sw = 13;                // Encoder SW (button) pin
//NOTE!!!  Clk and Sw pin are labeled incorrectly on board 7.0!!!!  This (the code) is correct!
int TXPin = 3;              // SoftwareSerial TX pin
int RXPin = 4;              // SoftwareSerial RX pin
int red_led_pin = 5;        //
int green_led_pin = 6;      //
int button_pin = 7;         // future hardware connected to button
int esc_servo_pin = 9;      // gps signal to ESC
int steering_servo_pin = 10;// gps signal to steering servo
int blue_led_pin = 11;
int test_platform_pin = 8;  // pin for platform identification
int buzzer_pin = 12;        // Piezo electric buzzer pin
//int buzzer_pin = 1;        // Piezo electric buzzer pin

// A-pins...
// Pins A7 unused
int temperature_pin = A1;   // analog input for TMP36 temperature sensor
int ir_pin = A2;            // analog read to determine distance with IR sensor
int batt_cell_1_pin = A3;   // analog input pin for cell 1 of main battery
int batt_volt_pin = A6;     // analog input pin for voltage divide input of main battery
// pins A4,A5 used for I2C protocol...
//SDA = A4;
//SCL = A5;

// Voltage divider constants and battery monitor outputs
const int R1 = 100;
const int R2 = 47;
float batt_volts_total;
float volts_cell_1;
float volts_cell_2;

// setup LED stuff ...
long timer[] = {0, 0, 0};
const int R = 0;
const int G = 1;
const int B = 2;

float dist_ir;
bool test_platform = 0;   // flag to determine if we are on the test platform or the car - based on reading pin 8
bool armed = 0;  // don't arm until arive at 1st gps location
int ind_gps;   // This is the index to which of the gps lat/long points we are currently going for ...
int gps_heading, heading_error, dist_to_target;
int compass_heading;
int compass_offset_car = 90;   // This is because the compass is mounted x° off straight
int compass_offset_platform = -90;   // This is because the compass is mounted x° off straight
int compass_offset;   // This is because the compass is mounted x° off straight

// Setup GPS ...
#include <TinyGPS++.h>
// The TinyGPS++ object
TinyGPSPlus gps;

// Using NeoSWSerial - supposed to be better than SoftwareSerial, and also compatable with servo library
#include <NeoSWSerial.h>
NeoSWSerial ss(TXPin, RXPin);  // The software-serial connection to the GPS device

// so as to be able to use I2C communication
#include <Wire.h>

// Setup Compass ...
#include <Adafruit_HMC5883_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

//Setup LCD Screen
#include <LiquidCrystal_I2C.h>
// next 2 lines for old LiquidCrystal_I2C library...
LiquidCrystal_I2C lcd_0x27 (0x27, 20, 4);  // set the LCD address for a 20 chars and 4 line display
LiquidCrystal_I2C lcd_0x3F (0x3F, 20, 4);  // set the LCD address for a 20 chars and 4 line display
LiquidCrystal_I2C lcd = lcd_0x27;

// Initialize servo stuff ...
#include <Servo.h>
Servo steering_servo, esc_servo;  // create servo objects to control a servo

int  servo_write_freq = 100;  // in hz
long servo_write_delay = 1000 / servo_write_freq;
long servo_write_time = 0;
int  servo_command = 0;
int  throttle_command = 0;

int servo_straight = 90;
int straight_offset = servo_straight - 90;
int servo_left = 60;
int servo_right = 127;
int esc_full_forward = 170;
int esc_slow_grass = 109;
int esc_slow_pavement = 104;
int esc_fast_forward = 140;
int esc_full_reverse = 20;
int esc_slow_reverse = 80;
int esc_fast_reverse = 40;
int esc_stop = 90;

bool beeped = 0;

volatile int LCD_screen = 0;
int num_LCD_screens = 5;
