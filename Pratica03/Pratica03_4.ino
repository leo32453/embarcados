#include <Servo.h>
//6 e 7 para sonar
// Define os pinos dos servos
#define PinRight 3
#define PinLeft 2

Servo servoRight;
Servo servoLeft;

/* Sonar */
#define echo_PIN 7
#define trigger_PIN 6
float distance;
float menor_dist;

void setup() {
  // Configura os pinos dos servos como saídas
  pinMode(PinRight, OUTPUT);
  pinMode(PinLeft, OUTPUT);
  
  // Inicializa os objetos dos servos
  servoRight.attach(PinRight);
  servoLeft.attach(PinLeft);

  /* Sonar */
  Serial.begin(9600);
  pinMode(trigger_PIN, OUTPUT);
  pinMode(echo_PIN, INPUT);
}

void moveForward(int speed) {
  // Define a velocidade para frente
  servoRight.writeMicroseconds(1500 + speed);
  servoLeft.writeMicroseconds(1500 - speed);
}

void stop() {
  // Para ambos os motores
  servoRight.writeMicroseconds(1500);
  servoLeft.writeMicroseconds(1500);
}

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
  if(distance > 30){
    // Move para frente com velocidade 100
    moveForward(100);
    // Espere por um segundo
    delay(500);  
    // Pare
    stop(); 
    // Espere por um segundo
    delay(500);  
  }
}