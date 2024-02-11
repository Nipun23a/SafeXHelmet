#ifndef MPU6050_TOCKN_H
#define MPU6050_TOCKN_H

#include "Arduino.h"
#include "Wire.h"

#define MPU6050_ADDR         0x68
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG       0x1a
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_TEMP_H       0x41
#define MPU6050_TEMP_L       0x42

class MPU6050{
  public:
    MPU6050(TwoWire &w);
    MPU6050(TwoWire &w, float aC, float gC);
    void begin();
    void setGyroOffsets(float x, float y, float z);
    void writeMPU6050(byte reg, byte data);
    byte readMPU6050(byte reg);
    int16_t getRawAccX();
    int16_t getRawAccY();
    int16_t getRawAccZ();
    int16_t getRawTemp();
    int16_t getRawGyroX();
    int16_t getRawGyroY();
    int16_t getRawGyroZ();
    float getTemp();
    float getAccX();
    float getAccY();
    float getAccZ();
    float getGyroX();
    float getGyroY();
    float getGyroZ();
    void calcGyroOffsets(bool console = false, uint16_t delayBefore = 1000, uint16_t delayAfter = 3000);
    float getGyroXoffset();
    float getGyroYoffset();
    float getGyroZoffset();
    void update();
    float getAccAngleX();
    float getAccAngleY();
    float getGyroAngleX();
    float getGyroAngleY();
    float getGyroAngleZ();
    float getAngleX();
    float getAngleY();
    float getAngleZ();

  private:
    TwoWire *wire;
    int16_t rawAccX, rawAccY, rawAccZ, rawTemp,
    rawGyroX, rawGyroY, rawGyroZ;
    float gyroXoffset, gyroYoffset, gyroZoffset;
    float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;
    float angleGyroX, angleGyroY, angleGyroZ,
    angleAccX, angleAccY, angleAccZ;
    float angleX, angleY, angleZ;
    float interval;
    long preInterval;
    float accCoef, gyroCoef;
};

#endif


