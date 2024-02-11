#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define sensor A0
#define Highpulse 540

SoftwareSerial gsm(7, 8); // RX, TX

int value;
int Svalue; // Declare Svalue globally
long Stime = 0;
long Ltime = 0;
int count = 0;
int Bpm = 0;

void setup() {
  Serial.begin(9600);
  gsm.begin(9600); // Set GSM module baud rate
}

void loop() {
  Svalue = analogRead(sensor); // Update Svalue
  Serial.println(Svalue);
  value = map(Svalue, 0, 1024, 0, 45);

  BPM();

  delay(1000); // Adjust delay as per your requirement
}

void BPM() {
  if (Svalue > Highpulse) {
    Stime = millis() - Ltime;
    count++;

    if (Stime / 1000 >= 60) {
      Ltime = millis();
      Serial.println(count);

      // Send data to web application using GSM module
      gsm.println("AT"); // Check if GSM module is responding
      delay(1000);
      gsm.println("AT+HTTPINIT"); // Initialize HTTP Service
      delay(2000);
      gsm.println("AT+HTTPPARA=\"CID\",1"); // Set CID parameter
      delay(1000);
      gsm.println("AT+HTTPPARA=\"URL\",\"your_web_application_url\""); // Set URL
      delay(1000);
      gsm.println("AT+HTTPPARA=\"CONTENT\",\"application/json\""); // Set content type
      delay(1000);
      gsm.println("AT+HTTPDATA=100,5000"); // Set data size and timeout
      delay(1000);
      gsm.print("{\"BPM\":"); // Send BPM data
      gsm.print(count);
      gsm.println("}");
      delay(1000);
      gsm.println("AT+HTTPACTION=1"); // POST data
      delay(5000); // Wait for response, adjust this based on your server's response time
      gsm.println("AT+HTTPTERM"); // Terminate HTTP Service
      delay(1000);

      count = 0;
    }
  }
}
