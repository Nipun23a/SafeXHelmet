int buz = 10;            // Buzzer connected to pin 10
const int aqsensor = A0;  // Output of MQ135 connected to A0 pin of Arduino
int threshold = 400;    // Threshold level for Air Quality

void setup() {
  pinMode(buz, OUTPUT);     // Buzzer is connected as Output from Arduino
  pinMode(aqsensor, INPUT); // MQ135 is connected as INPUT to Arduino

  Serial.begin(9600);      // Begin serial communication with baud rate of 9600
}

void loop() {
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