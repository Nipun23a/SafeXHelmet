#define trigPin1 4           // Trig Pin Of HC-SR04
#define echoPin1 5        // Echo Pin Of HC-SR04
#define trigPin2 6           // Trig Pin Of HC-SR04
#define echoPin2 7        // Echo Pin Of HC-SR04
#define trigPin3 8           // Trig Pin Of HC-SR04
#define echoPin3 9        // Echo Pin Of HC-SR04
#define buzzerPin 10     // Buzzer Pin
long duration1, distance1, duration2, distance2, duration3, distance3;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);  // Set Buzzer Pin as Output
  pinMode(trigPin1, OUTPUT);       // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin1, INPUT);        // Set Echo Pin As I/P To Receive Reflected Waves
  pinMode(trigPin2, OUTPUT);       // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin2, INPUT);        // Set Echo Pin As I/P To Receive Reflected Waves
  pinMode(trigPin3, OUTPUT);       // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin3, INPUT);        // Set Echo Pin As I/P To Receive Reflected Waves
}

void loop() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);       // Transmit Waves For 10us
  delayMicroseconds(10);
  duration1 = pulseIn(echoPin1, HIGH);        // Receive Reflected Waves
  distance1 = duration1 * 0.034 / 2;                       // Get Distance
  Serial.print("Distance 1: ");
  Serial.println(distance1);

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);       // Transmit Waves For 10us
  delayMicroseconds(10);
  duration2 = pulseIn(echoPin2, HIGH);        // Receive Reflected Waves
  distance2 = duration2 * 0.034 / 2;                       // Get Distance
  Serial.print("Distance 2: ");
  Serial.println(distance2);

  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);       // Transmit Waves For 10us
  delayMicroseconds(10);
  duration3 = pulseIn(echoPin3, HIGH);        // Receive Reflected Waves
  distance3 = duration3 * 0.034 / 2;                       // Get Distance
  Serial.print("Distance 3: ");
  Serial.println(distance3);

  if (distance1 < 10 || distance2 < 10 || distance3 < 10) {
    // If any sensor detects an edge
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delay(1000); // Wait for a second
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
  }
}
