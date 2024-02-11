#define trigPin4 12          // Trig Pin Of HC-SR04 - Upwards
#define echoPin4 13          // Echo Pin Of HC-SR04 - Upwards
#define buzzerPin 10         // Buzzer Pin

long duration4, distance4;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);  // Set Buzzer Pin as Output
  pinMode(trigPin4, OUTPUT);   // Set Trig Pin As Output To Transmit Waves
  pinMode(echoPin4, INPUT);    // Set Echo Pin As Input To Receive Reflected Waves
}

void loop() {
  // Upwards sensor
  digitalWrite(trigPin4, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin4, HIGH);  // Transmit Waves For 10us
  delayMicroseconds(10);
  duration4 = pulseIn(echoPin4, HIGH);  // Receive Reflected Waves
  distance4 = duration4 * 0.034 / 2;    // Convert duration to distance

  // Print distance for debug purposes
  Serial.print("Upwards: ");
  Serial.println(distance4);

  // If the upward sensor detects an object at least 2 meters away, activate the buzzer
  if (distance4 >= 200) {
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delay(1000);                   // Wait for a second
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
  }
}