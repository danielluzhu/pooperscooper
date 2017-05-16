#define LED_RED 9
#define LED_BLUE 10
#define SCOOPPIN1 26
#define SCOOPPIN2 27
#define LEDTime 50
#define ScoopTime 1000

// https://www.arduino.cc/en/Tutorial/SimpleAudioPlayer
// https://startingelectronics.org/beginners/start-electronics-now/tut13-555-wailing-siren/

void setup() {
  Serial.begin(1000000);
  delay(2000);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(SCOOPPIN1, OUTPUT);
  pinMode(SCOOPPIN2, OUTPUT);
  Serial.println("Initialized");
}

int sc = 0;
void loop() {
  sc = 1;
  scoop();
  Serial.println("scoop?");
}

// 360 degrees in 6 seconds
void scoop() {
  scoopmotor(2); // move scoop down
  int ti = millis();
  while (millis() - ti < 3000) {
    // do nothing, go forard
  }
  scoopmotor(1);
}

void scoopmotor(int sc) {
  //   0 == stop 0 0; 1 = forward 1 0; 2 = backward 0 1
  int ti = millis();
  while (ti > millis() - ScoopTime) {
    digitalWrite(SCOOPPIN1, bitRead(sc, 0));
    digitalWrite(SCOOPPIN2, bitRead(sc, 1));
  }
  digitalWrite(SCOOPPIN1, 0);
  digitalWrite(SCOOPPIN2, 0);
}

void lit(boolean det) {
  while (det) {
    digitalWrite(LED_RED, HIGH);
    delay(LEDTime);
    digitalWrite(LED_RED, LOW);
    delay(LEDTime);
    digitalWrite(LED_RED, HIGH);
    delay(LEDTime);
    digitalWrite(LED_RED, LOW);

    digitalWrite(LED_BLUE, HIGH);
    delay(LEDTime);
    digitalWrite(LED_BLUE, LOW);
    delay(LEDTime);
    digitalWrite(LED_BLUE, HIGH);
    delay(LEDTime);
    digitalWrite(LED_BLUE, LOW);
  }
}

