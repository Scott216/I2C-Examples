/*
  This libray makes getting the sensor data simpler by creating simple function calls you use to specify which
  sensor you want data from and what data you want.
*/

#include "Arduino.h"

class Leaksensor
{
  public:
    Leaksensor();  // constructor
    byte SensorStatus(byte SensorID);           // Sensor status: 0 if online, 1 wireless sensor is offline, 2 I2C error
    bool isWet(byte SensorID);                  // Returns true if water is detected
    float remoteBattVolts(byte SensorID);       // Battery voltage of wireless sensor in millivolts
    uint16_t RemoteTemperature(byte SensorID);  // Temperature of wireless sensor
    byte RemoteSignalStrength(byte SensorID);   // Wireless signal strength of wireless sensor 0-100
    byte SensorType(byte SensorID);             // Sensor type: 1 = wired, 2 = wireless
    
  private:
    int getData(uint8_t sensorID);

    typedef struct sensorData_t {
      byte stat;
      byte sensorId;
      byte sensortype;
      byte isWet;
      uint16_t temp;
      float volts;
      byte signal;
    };

    typedef union I2C_Packet_t {
      sensorData_t sensor;
      byte I2CPacket[sizeof(sensorData_t)];
    };

    I2C_Packet_t leakinfo;
};



