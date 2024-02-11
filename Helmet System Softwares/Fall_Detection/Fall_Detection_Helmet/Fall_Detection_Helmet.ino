#define TINY_GSM_MODEM_SIM800 // Define your GSM modem model here
#include <TinyGsmClient.h>
#include <Wire.h>
#include <SoftwareSerial.h> // Include SoftwareSerial library for ESP32
#include <MPU6050_tockn.h> // Include the MPU6050_tockn library for MPU6050

const char* apn = "your_apn"; // APN provided by your network carrier
const char* gprsUser = "";    // GPRS user
const char* gprsPass = "";    // GPRS password

MPU6050 mpu6050(Wire); // Define an MPU6050 object

#define buzzerPin 10
unsigned long buzzerStartTime = 0;

boolean fallDetected = false;

void setup() {
  Serial.begin(115200);

  // Initialize GSM module
  SoftwareSerial SerialGSM(2, 3); // RX, TX pins for GSM module
  TinyGsm modem(SerialGSM);
  SerialGSM.begin(9600);
  delay(10);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // Connect to GSM network
  Serial.println("Connecting to GSM network");
  if (!modem.init()) {
    Serial.println("Initialization failed. Check your GSM module.");
    while (true);
  }

  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("GPRS connection failed. Check your APN.");
    while (true);
  }
  Serial.println("GSM connected successfully");
}

void loop() {
  mpu_read();
  float ax = (mpu6050.getAccX() - 2050) / 16384.00;
  float ay = (mpu6050.getAccY() - 77) / 16384.00;
  float az = (mpu6050.getAccZ() - 1947) / 16384.00;
  float gx = (mpu6050.getGyroX() + 270) / 131.07;
  float gy = (mpu6050.getGyroY() - 351) / 131.07;
  float gz = (mpu6050.getGyroZ() + 136) / 131.07;
  
  // calculating Amplitude vector for 3 axes
  float Raw_Amp = pow(pow(ax, 2) + pow(ay, 2) + pow(az, 2), 0.5);
  int Amp = Raw_Amp * 10;  // Multiplied by 10 because values are between 0 to 1
  Serial.println(Amp);

  if (Amp <= 2) { // Assuming a threshold for fall detection
    if (!fallDetected) {
      Serial.println("Fall Detected!");
      // You can uncomment the following line to send a notification to the server
      // sendToServer("Fall Detected!");
      fallDetected = true;
    }
  } else {
    fallDetected = false;
  }

  delay(100);
}

void mpu_read() {
  mpu6050.update();
}
