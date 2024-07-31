// Motor A connections
int enA = 10;
int in1 = 9;
int in2 = 8;
// Motor B connections
int enB = 5;
int in3 = 6;
int in4 = 7;

// Sensores da linha
int sensorPin0 = A0;   // select the input pin for the sensor
int sensorPin1 = A1;
int sensorPin2 = A2;
int sensorValue0 = 0;   // variable to store the value coming from the sensor
int sensorValue1 = 0;
int sensorValue2 = 0;

// Sensores de proximidade
#define IRSensorL 2
#define IRSensorR 3
int sensorStatusL = LOW;
int sensorStatusR = LOW;

void setup() {
	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
    
    // proximidade
    pinMode(IRSensorL, INPUT);
    pinMode(IRSensorR, INPUT);

  //sensores
  Serial.begin(9600);
}

void loop() {
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  sensorStatusL = digitalRead(IRSensorL);
  sensorStatusR = digitalRead(IRSensorR);

  int threshold = 1000;
    if ((sensorStatusL == LOW) || (sensorStatusR == LOW)){
        int turn = 300;
        int turnspeed = 90;
        int straightdelay = 600;
        stop();
        delay(straightdelay);
        turnRight(turnspeed);
        delay(turn+60);
        stop();
        delay(straightdelay);

        
        sensorStatusL = digitalRead(IRSensorL);
        sensorStatusR = digitalRead(IRSensorR);
        while((sensorStatusL == LOW) || (sensorStatusR == LOW)){
          turnRight(turnspeed);
          delay(10);
          sensorStatusL = digitalRead(IRSensorL);
          sensorStatusR = digitalRead(IRSensorR);
        }
        goStraight(turnspeed);
        delay(straightdelay);
        stop();
        delay(straightdelay);

        turnLeft(turnspeed);
        delay(turn);
        stop();
        delay(straightdelay);

        goStraight(turnspeed);
        delay(straightdelay+400);
        stop();
        delay(straightdelay);

        turnLeft(turnspeed);
        delay(turn-50);
        stop();
        delay(straightdelay);

        sensorValue1 = analogRead(sensorPin1);
        while(sensorValue1 <= threshold){
          goStraight(turnspeed);
          delay(10);
          sensorValue1 = analogRead(sensorPin1);
        }

        turnRight(turnspeed);
        delay(turn-100);
        stop();
        delay(straightdelay);
    } else {
      if ((sensorValue0 <= threshold) && (sensorValue1 >= threshold) && (sensorValue2 <= threshold)){
        goStraight(75);
      } else if ((sensorValue0 > threshold)){
        turnRight(75);
      } else if ((sensorValue2 > threshold)){
        turnLeft(75);
      } else if ((sensorValue0 <= threshold) && (sensorValue1 <= threshold) && (sensorValue2 <= threshold)){
        turnRight(70);
      }
    }

  Serial.print((sensorValue0 > threshold ? "black ": "white "));
  Serial.print(sensorValue0);
  Serial.print(" || ");
  Serial.print((sensorValue1 > threshold ? "black ": "white "));
  Serial.print(sensorValue1);
  Serial.print(" || ");
  Serial.print((sensorValue2 > threshold ? "black ": "white "));
  Serial.println(sensorValue2);

  delay(45);
}

// Turn Right
void turnRight(int speed){
  Serial.print(" - Right - ");
  analogWrite(enA, speed);
	analogWrite(enB, speed);
	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
}

// Turn Left
void turnLeft(int speed){
  Serial.print(" - Left - ");
  analogWrite(enA, speed);
	analogWrite(enB, speed);
	// Now change motor directions
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
}

// Go Straight
void goStraight(int speed){
  Serial.print(" - Straight - ");
  analogWrite(enA, speed);
	analogWrite(enB, speed);
  // Turn on motors
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
}

// Stop
void stop(){
  Serial.println("Stop");
	// Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
