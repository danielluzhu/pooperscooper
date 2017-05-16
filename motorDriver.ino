// Right wheel
#define MOTORPIN1 22
#define MOTORPIN2 23
// Left Wheel
#define MOTOR2PIN1 24
#define MOTOR2PIN2 25
int turnDelay = 7000;   // obviously just estimated time

void setup() {
  Serial.begin(1000000);
  delay(2000);
  pinMode(MOTORPIN1, OUTPUT);
  pinMode(MOTORPIN2, OUTPUT);
  pinMode(MOTOR2PIN1, OUTPUT);
  pinMode(MOTOR2PIN2, OUTPUT);
  digitalWrite(MOTORPIN1, LOW);
  digitalWrite(MOTORPIN2, LOW);
  digitalWrite(MOTOR2PIN1, LOW);
  digitalWrite(MOTOR2PIN2, LOW);
  Serial.println("Initialized");
}

void loop() {
//    serialMode();
    driveMode();
}

void serialMode() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    //    char cmd = '1';
    switch (cmd) {
      case '0':
        wheelRight(0);  wheelLeft(0);
        Serial.println("Stop");
        break;
      case '1':
        wheelRight(1);  wheelLeft(1);
        Serial.println("Forward");
        break;
      case '2':
        wheelRight(2);  wheelLeft(2);
        Serial.println("Backward");
        break;
      case '3':
        turnLeft90();
        Serial.println("Turn Left");
        break;
      case '4':
        turnRight90();
        Serial.println("Turn Right");
        break;
      case '5':
        wheelRight(0);  wheelLeft(1);
        Serial.println("Left Wheel Only");
        break;
      case '6':
        wheelRight(1);  wheelLeft(0);
        Serial.println("Right Wheel Only");
        break;
      default:
        wheelRight(0);  wheelLeft(0);
        Serial.println("Stop");
        break;
    }
  }
}

void driveMode() {
  //char seq[7] = "0124356";
  char seq[7] = "1111111
  ";
  for (int i = 0; i < 7; i++) {
    char cmd = seq[i];
    switch (cmd) {
      case '0':
        wheelRight(0);  wheelLeft(0);
        break;
      case '1':
        wheelRight(1);  wheelLeft(1);
        break;
      case '2':
        wheelRight(2);  wheelLeft(2);
        break;
      case '3':
        turnLeft90();
        break;
      case '4':
        turnRight90();
        break;
      case '5':
        wheelRight(0);  wheelLeft(1);
        break;
      case '6':
        wheelRight(1);  wheelLeft(0);
        break;
      default:
        wheelRight(0);  wheelLeft(0);
        break;
    }
    int ti = millis();
    while (millis() - ti < turnDelay) {
      // do nothing, go forard
    }
    wheelRight(0);  wheelLeft(0);
  }
}

void wheelRight(int right) {
  //   0 == stop 0 0; 1 = forward 1 0; 2 = backward 0 1
  digitalWrite(MOTORPIN1, bitRead(right, 0));
  digitalWrite(MOTORPIN2, bitRead(right, 1));
}

void wheelLeft(int left) {
  //   0 == stop 0 0; 1 = forward 1 0; 2 = backward 0 1
  digitalWrite(MOTOR2PIN1, bitRead(left, 0));
  digitalWrite(MOTOR2PIN2, bitRead(left, 1));
}

// distance in cm; 200 cm in 10 sec
// 30 rpm = 1 round every 2 seconds
// 40 cm a rotation.
// 40 = r2pi => r = 6.37 cm ~ 2.5 inches
void forward(int distance) {
  int timeDelay = distance / 20 * 1000;
  wheelRight(1);  wheelLeft(1);
  if (distance != 0) {
    int ti = millis();
    while (millis() - ti < timeDelay) {
      // do nothing, go forard
    }
  }
}

void turnRight90() {
  wheelRight(2);  wheelLeft(1);
  delay(turnDelay);
}

void turnLeft90() {
  wheelRight(1);  wheelLeft(2);
  delay(turnDelay);
}

void uturn() {
  // if edge is detected, uturn
  turnRight90();
  forward(20);
  turnRight90();
}
