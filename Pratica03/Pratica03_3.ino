#include <LiquidCrystal_I2C.h>
#include <Servo.h>

/* Sonar */
#define echo_PIN 2
#define trigger_PIN 3
float distance;
float menor_dist;

/* Display */
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

/* Buzzer */
int buzzer = 6; // Pin connected to the buzzer 

/* Servo */
Servo myservo;

#define SERVO_PIN 10 // Pin connected to the servo
#define PWM_FREQ 50 // PWM frequency in Hertz (50Hz for servos)

// int posicao[18] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 130, 140, 150, 160, 170, 180};
int menor_pos = 0;

void setup() {
  /* Sonar */
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigger_PIN, OUTPUT);
  pinMode(echo_PIN, INPUT);

  /* Display */
  // Initialize the LCD display and turn on backlight
  lcd.init();
  lcd.backlight();

  /* Buzzer */
  pinMode(buzzer, OUTPUT); //Defines the buzzer pin as Output

  /* Servo */
  myservo.attach(5);  // attaches the servo on pin 5 to the servo object
  myservo.write(0); 
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
  menor_dist = 4000;
  myservo.write(0);
  delay(400);
  
  for(int i=0; i<180; i++){
    myservo.write(i); 
    delay(30);

    distance = readDistance();
    if((distance < menor_dist) && (distance > 2)){
      menor_dist = distance;
      menor_pos = i;
    }
    delay(30);

    if((distance < 50) && (distance > 2)){
      digitalWrite(LED_BUILTIN, 1);
      tone(buzzer, distance*300); // Activate the buzzer at the frequency relative to C in Hz (Dó)
      delay(20);   // Wait for a while to deactivate 
      noTone(buzzer); //Deactivate the buzzer 
      digitalWrite(LED_BUILTIN, 0);
    }
  }

  myservo.write(menor_pos);

  delay(5000);
}

