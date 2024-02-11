#include <SoftwareSerial.h>

#define GSM_TX 2  // Arduino digital pin connected to GSM module TX
#define GSM_RX 3  // Arduino digital pin connected to GSM module RX
#define BUTTON_PIN 4 // Arduino digital pin connected to the button

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  gsmSerial.begin(9600);  // Set GSM baud rate
  Serial.begin(9600);     // Set Arduino serial monitor baud rate
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    sendSMS("Emergency Button Pressed!");
    delay(1000); // Debounce delay
  }
}

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
