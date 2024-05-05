#include <LiquidCrystal_I2C.h>

/*****************************************************************************
* HC-SR04 Ultrasonic Sensor
* Code to turn a LED on when an object is within 50cm range
* Author: Kelen C. Teixeira Vivaldini
******************************************************************************/

/* Buzzer */
#define echo_PIN 2
#define trigger_PIN 3
float distance;

/* Display */
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

void setup() {
  /* Buzzer */
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigger_PIN, OUTPUT);
  pinMode(echo_PIN, INPUT);

  /* Display */
  // Initialize the LCD display and turn on backlight
  lcd.init();
  lcd.backlight();
}

/* Sonar */
float readDistance() {
  digitalWrite(trigger_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_PIN, LOW);
  int duration = pulseIn(echo_PIN, HIGH);
  distance = duration * 0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance; //Constante obtida com calibração
}

void loop() {
  distance = readDistance();

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Distancia:");

  lcd.setCursor(1, 1);
  lcd.print(distance);

  lcd.setCursor(9, 1);
  lcd.print("cm");
  
  delay(500);
}

