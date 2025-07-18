#include <Servo.h>

// Define IR sensor pins (left, right, middle)
const int irLedPinL = 10;
const int irReceivePinL = 11;
const int redLedPinL = A2;

const int irLedPinR = 2;
const int irReceivePinR = 3;
const int redLedPinR = A0;

const int irLedPinM = 6;
const int irReceivePinM = 7;
const int redLedPinM = A1;

// Servo objects
Servo servoLeft;
Servo servoRight;

// Configuration parameters
const int maxDistance = 4;        // Maximum distance to avoid hitting the wall (increased for safety)
const int minDistance = 2;        // Minimum distance threshold
const int turnDelay = 800;        // Delay for 90-degree turns
const int slightTurnDelay = 200;  // Delay for slight turns
const int moveDelay = 150;        // Delay for forward/backward movement
const int sensorDelay = 50;       // Delay for sensor readings

// State variables
bool isStuck = false;
int stuckCounter = 0;
const int maxStuckCount = 10;     // Maximum times to try getting unstuck
int noPathCounter = 0;
const int maxNoPathCount = 5;     // Maximum consecutive no-path situations

// Servo control values
const int servoStop = 1500;
const int servoForwardLeft = 1400;
const int servoForwardRight = 1600;
const int servoBackward = 1400;
const int servoTurnLeft = 1400;
const int servoTurnRight = 1600;

void setup() {
  Serial.begin(9600);
  Serial.println("Robot Maze Runner Starting...");

  // Attach servos to motor control pins
  servoLeft.attach(8);   // Left servo on pin 8
  servoRight.attach(9);  // Right servo on pin 9

  // Set up IR sensor pins
  pinMode(irReceivePinL, INPUT);
  pinMode(irLedPinL, OUTPUT);
  pinMode(redLedPinL, OUTPUT);

  pinMode(irReceivePinR, INPUT);
  pinMode(irLedPinR, OUTPUT);
  pinMode(redLedPinR, OUTPUT);

  pinMode(irReceivePinM, INPUT);
  pinMode(irLedPinM, OUTPUT);
  pinMode(redLedPinM, OUTPUT);

  // Initialize LED indicators
  digitalWrite(redLedPinL, LOW);
  digitalWrite(redLedPinR, LOW);
  digitalWrite(redLedPinM, LOW);

  // Start with the robot stopped
  stopRobot();
  
  // Calibration delay
  delay(2000);
  Serial.println("Robot ready to start maze navigation!");
}

void loop() {
  // Measure distance using the irDistance function with averaging
  int distL = getAverageDistance(irLedPinL, irReceivePinL, 3);  // Left IR distance
  int distR = getAverageDistance(irLedPinR, irReceivePinR, 3);  // Right IR distance
  int distM = getAverageDistance(irLedPinM, irReceivePinM, 3);  // Middle IR distance

  // Update LED indicators based on sensor readings
  updateLEDs(distL, distR, distM);

  // Print sensor values for debugging
  Serial.print("Distances - L:");
  Serial.print(distL);
  Serial.print(" M:");
  Serial.print(distM);
  Serial.print(" R:");
  Serial.println(distR);

  // Check if robot is completely stuck (all sensors detect walls)
  if (distL <= minDistance && distR <= minDistance && distM <= minDistance) {
    handleCompleteStuck();
    return;
  }

  // Check if there's no clear path forward
  if (distL <= maxDistance && distR <= maxDistance && distM <= maxDistance) {
    noPathCounter++;
    if (noPathCounter >= maxNoPathCount) {
      Serial.println("No path available - stopping robot");
      stopRobot();
      return; // End the program
    }
    handleNoPath();
    return;
  } else {
    noPathCounter = 0; // Reset counter if path is available
  }

  // Main navigation logic
  if (distM <= maxDistance) {
    // Middle sensor detects a wall, need to turn
    Serial.println("Middle wall detected - turning");
    stopRobot();
    delay(300);
    moveBack();
    delay(200);
    
    // Decide turn direction based on side sensors
    if (distL > distR && distL > maxDistance) {
      Serial.println("Turning left");
      turnLeft();
    } else if (distR > distL && distR > maxDistance) {
      Serial.println("Turning right");
      turnRight();
    } else {
      // If both sides are blocked, try left first
      Serial.println("Both sides blocked - trying left");
      turnLeft();
    }
  } 
  else if (distL <= maxDistance && distR > maxDistance) {
    // Left sensor detects a wall, turn slightly right
    Serial.println("Left wall - slight right turn");
    moveSlightRight();
  } 
  else if (distR <= maxDistance && distL > maxDistance) {
    // Right sensor detects a wall, turn slightly left
    Serial.println("Right wall - slight left turn");
    moveSlightLeft();
  } 
  else {
    // Clear path forward
    Serial.println("Moving forward");
    moveForward();
  }

  delay(sensorDelay);
}

// Function to get average distance reading for better accuracy
int getAverageDistance(int irLedPin, int irReceivePin, int samples) {
  int total = 0;
  for (int i = 0; i < samples; i++) {
    total += irDistance(irLedPin, irReceivePin);
    delay(10);
  }
  return total / samples;
}

// Function to measure distance using frequency sweeps
int irDistance(int irLedPin, int irReceivePin) {
  int distance = 0;
  for (long f = 38000; f <= 42000; f += 1000) {
    distance += irDetect(irLedPin, irReceivePin, f);
  }
  return distance;
}

// IR detection function
int irDetect(int ledPin, int receivePin, long frequency) {
  tone(ledPin, frequency);
  delay(1);
  noTone(ledPin);
  return digitalRead(receivePin);  // Return 1 if no object, 0 if an object is detected
}

// Function to update LED indicators
void updateLEDs(int distL, int distR, int distM) {
  digitalWrite(redLedPinL, (distL <= maxDistance) ? HIGH : LOW);
  digitalWrite(redLedPinR, (distR <= maxDistance) ? HIGH : LOW);
  digitalWrite(redLedPinM, (distM <= maxDistance) ? HIGH : LOW);
}

// Function to handle complete stuck situation
void handleCompleteStuck() {
  Serial.println("Robot completely stuck - attempting recovery");
  stuckCounter++;
  
  if (stuckCounter >= maxStuckCount) {
    Serial.println("Maximum stuck attempts reached - stopping");
    stopRobot();
    return;
  }
  
  // Try to get unstuck by moving backward and turning
  moveBack();
  delay(500);
  turnRight();
  delay(300);
}

// Function to handle no clear path situation
void handleNoPath() {
  Serial.println("No clear path - attempting to find alternative");
  
  // Try different turning strategies
  if (stuckCounter % 2 == 0) {
    turnLeft();
  } else {
    turnRight();
  }
  
  stuckCounter++;
}

// Function to stop the robot
void stopRobot() {
  servoLeft.writeMicroseconds(servoStop);
  servoRight.writeMicroseconds(servoStop);
  Serial.println("Robot stopped");
}

// Function to move backward
void moveBack() {
  servoLeft.writeMicroseconds(servoBackward);
  servoRight.writeMicroseconds(servoBackward);
  delay(moveDelay);
}

// Function to move forward
void moveForward() {
  servoLeft.writeMicroseconds(servoForwardLeft);
  servoRight.writeMicroseconds(servoForwardRight);
  delay(moveDelay);
}

// Function to slightly turn right
void moveSlightRight() {
  servoLeft.writeMicroseconds(servoForwardLeft);
  servoRight.writeMicroseconds(servoStop);
  delay(slightTurnDelay);
}

// Function to slightly turn left
void moveSlightLeft() {
  servoLeft.writeMicroseconds(servoStop);
  servoRight.writeMicroseconds(servoForwardRight);
  delay(slightTurnDelay);
}

// Function to turn right (90-degree turn)
void turnRight() {
  servoLeft.writeMicroseconds(servoTurnRight);
  servoRight.writeMicroseconds(servoTurnLeft);
  delay(turnDelay);
}

// Function to turn left (90-degree turn)
void turnLeft() {
  servoLeft.writeMicroseconds(servoTurnLeft);
  servoRight.writeMicroseconds(servoTurnRight);
  delay(turnDelay);
} 