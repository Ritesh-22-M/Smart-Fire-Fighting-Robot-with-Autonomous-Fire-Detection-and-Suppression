// Define motor control pins
const int IN1 = 7;  // Left motors forward
const int IN2 = 6;  // Left motors backward
const int IN3 = 8;  // Right motors forward
const int IN4 = 9;  // Right motors backward

// Define motor driver enable pins
const int ENA = 10;  // Enable pin for left motors (PWM pin)
const int ENB = 11;  // Enable pin for right motors (PWM pin)

// Define flame sensor pins
const int flameSensorRight = A0; // Flame sensor right
const int flameSensorCenter = A1; // Flame sensor center
const int flameSensorLeft = A2; // Flame sensor left

// Define servo and pump pin
#include <Servo.h>
Servo myServo;
const int servoPin = A4;
const int pumpPin = A5;

// Variables for flame detection
int flameRightStatus;
int flameCenterStatus;
int flameLeftStatus;

void setup() {
  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Set enable pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Initialize servo
  myServo.attach(servoPin);
  
  // Initialize flame sensor pins as inputs
  pinMode(flameSensorRight, INPUT);
  pinMode(flameSensorCenter, INPUT);
  pinMode(flameSensorLeft, INPUT);
  
  // Initialize pump pin
  pinMode(pumpPin, OUTPUT);

  // Start with motors stopped, pump off, and servo at initial position
  stopMotors();
  myServo.write(90); // Initial position of the servo (center)
  digitalWrite(pumpPin, LOW); // Pump off

  Serial.begin(9600);
}

void loop() {
  // Read flame sensor values
  flameRightStatus = analogRead(flameSensorRight);
  flameCenterStatus = analogRead(flameSensorCenter);
  flameLeftStatus = analogRead(flameSensorLeft);

  // Print sensor values for debugging
  Serial.print("Right: "); Serial.print(flameRightStatus);
  Serial.print(" Center: "); Serial.print(flameCenterStatus);
  Serial.print(" Left: "); Serial.println(flameLeftStatus);

  // Check flame sensor readings and control robot accordingly
  if (flameCenterStatus < 300) {  // Flame detected in the center
    extinguishFlame(90);            // Move servo to spray covering angles around the center
  } 
  else if (flameLeftStatus < 300) { // Flame detected on the left
    extinguishFlame(45);           // Move servo to spray left
    moveBackward(150);             // Move backward with moderate speed before turning
    delay(300);
    turnLeft(200);                 // Turn left with speed control
    delay(500);
  } 
  else if (flameRightStatus < 300) { // Flame detected on the right
    extinguishFlame(135);          // Move servo to spray right
    moveBackward(150);             // Move backward with moderate speed before turning
    delay(300);
    turnRight(200);                // Turn right with speed control
    delay(500);
  } 
  else if (flameCenterStatus >= 300 && flameCenterStatus <= 700) {
    // Flame is in front but far away, move forward to get closer
    moveForward(200);              // Move forward with speed control
  } 
  else if (flameLeftStatus >= 300 && flameLeftStatus <= 700) {
    // Flame is on the left but far, turn left slightly
    moveBackward(150);             // Move backward with moderate speed
    delay(300);
    turnLeft(150);                 // Turn left with moderate speed
    delay(300);
  } 
  else if (flameRightStatus >= 300 && flameRightStatus <= 700) {
    // Flame is on the right but far, turn right slightly
    moveBackward(150);             // Move backward with moderate speed
    delay(300);
    turnRight(150);                // Turn right with moderate speed
    delay(300);
  } 
  else {
    // No flame detected, stop the bot and turn off the pump
    stopMotors();
    digitalWrite(pumpPin, LOW);
  }

  delay(100); // Small delay to avoid rapid switching
}

// Function to extinguish flame
void extinguishFlame(int startAngle) {
  stopMotors();                    // Stop the motors before spraying
  for (int angle = startAngle - 20; angle <= startAngle + 20; angle += 3) { // 20 degrees left and right
    myServo.write(angle);    // Move the servo gradually across angles
    delay(100);              // Smooth movement delay
  }
  digitalWrite(pumpPin, HIGH); // Turn on the pump to spray water
  delay(2000);                // Spray for 2 seconds
  digitalWrite(pumpPin, LOW);  // Turn off the pump
  myServo.write(90);           // Reset servo to the center position
}

// Function to move motors forward with speed control
void moveForward(int speed) {
  analogWrite(ENA, speed);  // Control speed of left motors
  analogWrite(ENB, speed);  // Control speed of right motors
  digitalWrite(IN1, HIGH);  // Left motors forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Right motors forward
  digitalWrite(IN4, LOW);
}

// Function to stop the motors
void stopMotors() {
  analogWrite(ENA, 0);      // Stop left motors
  analogWrite(ENB, 0);      // Stop right motors
}

// Function to move motors backward with speed control
void moveBackward(int speed) {
  analogWrite(ENA, speed);  // Control speed of left motors
  analogWrite(ENB, speed);  // Control speed of right motors
  digitalWrite(IN1, LOW);   // Left motors backward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);   // Right motors backward
  digitalWrite(IN4, HIGH);
}

// Function to turn left with speed control
void turnLeft(int speed) {
  analogWrite(ENA, speed);  // Control speed of left motors
  analogWrite(ENB, speed);  // Control speed of right motors
  digitalWrite(IN1, LOW);   // Left motors backward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);  // Right motors forward
  digitalWrite(IN4, LOW);
}

// Function to turn right with speed control
void turnRight(int speed) {
  analogWrite(ENA, speed);  // Control speed of left motors
  analogWrite(ENB, speed);  // Control speed of right motors
  digitalWrite(IN1, HIGH);  // Left motors forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Right motors backward
  digitalWrite(IN4, HIGH);
}