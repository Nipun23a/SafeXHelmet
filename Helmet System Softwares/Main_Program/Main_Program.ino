#define TINY_GSM_MODEM_SIM800 // Define your GSM modem model here
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <TinyGsmClient.h>
#include <MPU6050_tockn.h> // Include the MPU6050_tockn library for MPU6050

#define GSM_TX 2  // Arduino digital pin connected to GSM module TX
#define GSM_RX 3  // Arduino digital pin connected to GSM module RX
#define BUTTON_PIN 4 // Arduino digital pin connected to the button
#define sensor A0
#define Highpulse 540
// Trig and Echo Pin Setting to Edge Detection System
#define trigPin1 4           // Trig Pin Of HC-SR04
#define echoPin1 5        // Echo Pin Of HC-SR04
#define trigPin2 6           // Trig Pin Of HC-SR04
#define echoPin2 7        // Echo Pin Of HC-SR04
#define trigPin3 8           // Trig Pin Of HC-SR04
#define echoPin3 9        // Echo Pin Of HC-SR04
// Trig and Echo Pin Setting to Object Detection From Above
#define trigPin4 12          // Trig Pin Of HC-SR04 - Upwards to Object Detection from above.
#define echoPin4 13          // Echo Pin Of HC-SR04 - Upwards to Object Detection from above.

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

int buz = 10;            // Buzzer connected to pin 10
int buzzerPin = 10; // Define buzzerPin here

//Air Quality Checking System Variabes
const int aqsensor = A1;  // Output of MQ135 connected to A1 pin of Arduino
int threshold = 400;    // Threshold level for Air Quality

//Heart Beat Checking System Variables
int value;
int Svalue;
long Stime = 0;
long Ltime = 0;
int count = 0;
int Bpm = 0;
//Edge Detection System Variable
long duration1, distance1, duration2, distance2, duration3, distance3;
// Object Detection From Above System Variable
long duration4, distance4;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  //Set Pin To Edge Detection System
  pinMode(trigPin1, OUTPUT);       // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin1, INPUT);        // Set Echo Pin As I/P To Receive Reflected Waves
  pinMode(trigPin2, OUTPUT);       // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin2, INPUT);        // Set Echo Pin As I/P To Receive Reflected Waves
  pinMode(trigPin3, OUTPUT);       // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin3, INPUT);        // Set Echo Pin As I/P To Receive Reflected Waves
  //Set Pins to Object Detection System
  pinMode(trigPin4, OUTPUT);   // Set Trig Pin As Output To Transmit Waves
  pinMode(echoPin4, INPUT);    // Set Echo Pin As Input To Receive Reflected Waves
  //Set Pin to Buzzzer
  pinMode(buz, OUTPUT);     // Buzzer is connected as Output from Arduino
  // Set Pin to Air Quality Checking
  pinMode(aqsensor, INPUT); // MQ135 is connected as INPUT to Arduino
  gsmSerial.begin(9600);  // Set GSM baud rate
  Serial.begin(9600);     // Set Arduino serial monitor baud rate
}

void loop() {
  airQuality();
  callingBPM();
  ObjectDetection();
  EdgeDetection();
  CallingEmergency();
}

// Emergency System 
void sendSMS(String message) {
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(100);
  gsmSerial.print("AT+CMGS=\"Phone Number\"\r"); // Replace Phone number with your destination phone number
  delay(100);
  gsmSerial.println(message);
  delay(100);
  gsmSerial.println((char)26); // End SMS by sending Ctrl+Z
  delay(1000);
}

// Calling Emergency System to Loop Method
void CallingEmergency() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    sendSMS("Emergency Button Pressed!");
  }
}

// Airquality checking system
void airQuality() {
  int ppm = analogRead(aqsensor); // Read MQ135 analog outputs at A0 and store it in ppm

  Serial.print("Air Quality: ");  // Print message in serial monitor
  Serial.println(ppm);            // Print value of ppm in serial monitor

  if (ppm > threshold) {          // Check if ppm is greater than threshold or not
    Serial.println("AQ Level HIGH");
    tone(buz, 1000, 200);         // Sound the buzzer
  } else {
    noTone(buz);                  // Turn off the buzzer if air quality is good
    Serial.println("AQ Level Good");
  }
  delay(500);
}

// Heartbeat Checking System
void BPM() {
  if (Svalue > Highpulse) {
    Stime = millis() - Ltime;
    count++;

    if (Stime / 1000 >= 60) {
      Ltime = millis();
      Serial.println(count);

      // Send data to web application using GSM module
      gsmSerial.println("AT"); // Check if GSM module is responding
      delay(1000);
      gsmSerial.println("AT+HTTPINIT"); // Initialize HTTP Service
      delay(2000);
      gsmSerial.println("AT+HTTPPARA=\"CID\",1"); // Set CID parameter
      delay(1000);
      gsmSerial.println("AT+HTTPPARA=\"URL\",\"your_web_application_url\""); // Set URL
      delay(1000);
      gsmSerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\""); // Set content type
      delay(1000);
      gsmSerial.println("AT+HTTPDATA=100,5000"); // Set data size and timeout
      delay(1000);
      gsmSerial.print("{\"BPM\":"); // Send BPM data
      gsmSerial.print(count);
      gsmSerial.println("}");
      delay(1000);
      gsmSerial.println("AT+HTTPACTION=1"); // POST data
      delay(5000); // Wait for response, adjust this based on your server's response time
      gsmSerial.println("AT+HTTPTERM"); // Terminate HTTP Service
      delay(1000);

      count = 0;
    }
  }
}

// Calling Heartbeat System to Loop Method
void callingBPM() {
  Svalue = analogRead(sensor); // Update Svalue
  Serial.println(Svalue);
  value = map(Svalue, 0, 1024, 0, 45);
  BPM();
  delay(1000);
}

// Object Detection From Above System
void ObjectDetection() {
  digitalWrite(trigPin4, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin4, HIGH);  // Transmit Waves For 10us
  delayMicroseconds(10);
  duration4 = pulseIn(echoPin4, HIGH);  // Receive Reflected Waves
  distance4 = duration4 * 0.034 / 2;    // Convert duration to distance

  // Print distance for debug purposes
  Serial.print("Danger Object Detected From Above. Distance:");
  Serial.print(distance4);

  // If the upward sensor detects an object at least 2 meters away, activate the buzzer
  if (distance4 >= 200) {
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delay(1000);                   // Wait for a second
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
  }
}

// Edge Detection System
void EdgeDetection() {
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

// Fall Detection System
void FallDetection() {
  // Needed to Develop
}

// Camera System
void CameraSystem() {
  // Needed to Develop
}

// Location Tracking System
void LocationTracking() {
  //Needed to Develop
}