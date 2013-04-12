#include "LeakSensors.h"
#include <I2C.h>   // https://github.com/rambo/I2C

#define PACKET_SIZE sizeof(sensorData_t)

uint8_t addrSlaveI2C = 21;    // ID of I2C slave.  Don't use #define

static byte prevSensorID = 0; // sensor ID of previous request
static uint32_t lastRequestTime = 0;  // millis of last request. Used to prevent calling slave too often
const uint32_t requestDelay = 1000; 

Leaksensor::Leaksensor()   // Constructor
{
  // Initialize I2C
  I2c.begin();  // enables the I2C hardware
  I2c.timeOut(10000);
}

byte Leaksensor::SensorStatus(byte SensorID)    // Sensor status
{
  // If requesting a different sensor, or some time has passed since last request, call slave to get fresh data
  if (SensorID != prevSensorID && (long)(millis() - lastRequestTime) > requestDelay )
  {
    int i2cStat = Leaksensor::getData(SensorID);  // Refresh sensor data
  }
  return leakinfo.sensor.stat;  
}

bool Leaksensor::isWet(byte SensorID)
{
  // If requesting a different sensor, or some time has passed since last request, call slave to get fresh data
  if (SensorID != prevSensorID && (long)(millis() - lastRequestTime) > requestDelay )
  {
    int i2cStat = Leaksensor::getData(SensorID);  // Refresh sensor data
  }
  return leakinfo.sensor.isWet;  
}

float Leaksensor::remoteBattVolts(byte SensorID) // Battery voltage of wireless sensor in millivolts
{
  // If requesting a different sensor, or some time has passed since last request, call slave to get fresh data
  if (SensorID != prevSensorID && (long)(millis() - lastRequestTime) > requestDelay )
  {
    int i2cStat = Leaksensor::getData(SensorID);  // Refresh sensor data
  }
  return leakinfo.sensor.volts;  
}

uint16_t Leaksensor::RemoteTemperature(byte SensorID)  // Temperature of wireless sensor
{
  // If requesting a different sensor, or some time has passed since last request, call slave to get fresh data
  if (SensorID != prevSensorID && (long)(millis() - lastRequestTime) > requestDelay )
  {
    int i2cStat = Leaksensor::getData(SensorID);  // Refresh sensor data
  }
  return leakinfo.sensor.temp;  
}

byte Leaksensor::RemoteSignalStrength(byte SensorID)  // Wireless signal strength of wireless sensor 0-100
{
  // If requesting a different sensor, or some time has passed since last request, call slave to get fresh data
  if (SensorID != prevSensorID && (long)(millis() - lastRequestTime) > requestDelay )
  {
    int i2cStat = Leaksensor::getData(SensorID);  // Refresh sensor data
  }
  return leakinfo.sensor.signal;  
}

byte Leaksensor::SensorType(byte SensorID)   // Sensor type: 1 = wired, 2 = wireless
{
  // If requesting a different sensor, or some time has passed since last request, call slave to get fresh data
  if (SensorID != prevSensorID && (long)(millis() - lastRequestTime) > requestDelay )
  {
    int i2cStat = Leaksensor::getData(SensorID);  // Refresh sensor data
  }
  return leakinfo.sensor.sensortype;  
}

// I2C Request data from slave
int Leaksensor::getData(uint8_t sensorID)
{
  // Tell slave which sensor we want data for                        
  I2c.write(addrSlaveI2C, sensorID);  
  
  //get data for sensor defined above in I2c.write() and store in byteArray array
  byte byteArray[PACKET_SIZE];
  int readstatus = I2c.read(addrSlaveI2C, PACKET_SIZE, byteArray ); 
  // If we got an I2C packet, we can copy the values into the union leakinfo
  if(readstatus == 0)
  {
    for (int k=0; k < PACKET_SIZE; k++)
    { 
      leakinfo.I2CPacket[k] = byteArray[k];
    }
    lastRequestTime = millis(); 
    prevSensorID = sensorID;
    return 0;
  }  // end got packet
  else
  { 
    leakinfo.sensor.stat = 2;  // I2C Error 
    return readstatus; 
  } // No Packet received
  
} // end getData()

