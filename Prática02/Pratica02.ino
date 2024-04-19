#include <Servo.h> // Library for servo
#include <LiquidCrystal_I2C.h> // Library for display
#include <Keypad.h> // Library for keypad

/* ---=======Display=======--- */
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

/* ---=======Keypad=======--- */
const byte lines = 4; // Number of keypad rows
const byte columns = 4; // Number of keypad columns

const char KEYPAD_MATRIX[lines][columns] = { // Keypad character matrix (keypad mapping)
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const byte ROW_PINS[lines] = {9, 8, 7, 6}; // Connection pins for keypad rows
const byte COLUMN_PINS[columns] = {5, 4, 3, 2}; // Connection pins for keypad columns

Keypad custom_keypad = Keypad(makeKeymap(KEYPAD_MATRIX), ROW_PINS, COLUMN_PINS, lines, columns); // Initialize keypad

/* ---=======Servo=======--- */
Servo myservo;

#define SERVO_PIN 10 // Pin connected to the servo
#define PWM_FREQ 50 // PWM frequency in Hertz (50Hz for servos)

/* ---=======Setup=======--- */
void setup() {
  /* Display */
  // Initialize the LCD display, backlight off
  lcd.init();

  /* Keyboard */
  Serial.begin(9600); // Initialize serial port

  /* Servo */
  myservo.attach(10);  // attaches the servo on pin 10 to the servo object
  myservo.write(0); // position 0
}

String senha = "1234"; // Defines initial password
String lock_key = "A"; // Defines key to lock. "A" chosen for acessibility
String pwd_key = "B"; // Defines key to change password. "B" chosen for acessibility

/* ---=======Loop=======--- */
void loop() {
  // Assign the keypad input to a variable, this gets displayed with "Serial.println(key_pressed);"
  char key_pressed = custom_keypad.getKey();

  // This will store the input for passwords
  String tentativa = "0000";
  
  // This will store the input for changing passwords or locking
  String lock_input = "0";

  /* Start */
  lcd.setCursor(1, 0);
  lcd.print("Enter password");

  char value; // For the input from the keypad
  
  /* Gets 4 characters from the keypad
    The backlight is initially off, turns on once a key is pressed */
  for(int i=0; i < 4; i++){
    value = custom_keypad.waitForKey();
    tentativa.setCharAt(i, value);

    lcd.backlight();
    
    lcd.setCursor(i+1, 1);
    lcd.print("*");
    //Serial.println(key_pressed);
  }

  //Serial.println(tentativa);
  //Serial.println(senha.equals(tentativa));
  
  /* Checking Password */
  if(!(senha.compareTo(tentativa))){
    // Right password turns the servo
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Right password");
    lcd.setCursor(1, 1);
    lcd.print("Opening");
    delay(2000);
    myservo.write(90);

    // It's now open, waiting for either "close" or "change password"
    // This menu will display until "close" is chosen
    while((lock_key.compareTo(lock_input))){
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("A: to close");
      lcd.setCursor(1, 1);
      lcd.print("B: change pwd"); 
      lock_input = custom_keypad.waitForKey();

      // If (not close) is chosen, waits for a new password input
      if((lock_key.compareTo(lock_input))){
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("New password:");

        for(int i=0; i < 4; i++){
          value = custom_keypad.waitForKey();
          senha.setCharAt(i, value);
          
          lcd.setCursor(i+1, 1);
          lcd.print("*");
          // Serial.println(key_pressed);
        }        
      }
    }

    // After "close" is chosen, turns the servo again
    // And turns off backlight
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Closing");
    delay(2000);
    myservo.write(0);
    delay(5000);
    lcd.noBacklight();
    
  } else {
    // Wrong password does nothing
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Wrong password");
    delay(5000);
  }
}