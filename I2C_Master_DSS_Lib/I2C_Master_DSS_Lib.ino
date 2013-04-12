/*
I2C Test - Master
I2C communication between two Arduinos.  Use this with I2C_Slave_struct.ino
This version creates a library to make the main sketch simpler

Uses DSS's I2C.h library, not the Wire.h library.
Uses union and typedef struct to convert byte array from I2C back to usable varible

*/ 

#include "LeakSensors.h"
#include <I2C.h>   // https://github.com/rambo/I2C

Leaksensor sensorInfo;  // initialize Leaksensor class

void setup () 
{
  Serial.begin(9600);
  Serial.println(F("Initialize library for I2C Master Test"));
} // end setup()


void loop()
{
  static byte sID; 
  
  // Alternate between sensors
  if (sID == 22)
  { sID = 23;}
  else
  { sID = 22;}

  byte stat = sensorInfo.SensorStatus(sID);
  if (stat == 0 )
  {
    Serial.print(sID);
    Serial.print(F("\t"));
    Serial.print(sensorInfo.SensorStatus(sID));
    Serial.print(F("\t"));
    Serial.print(sensorInfo.SensorType(sID));
    Serial.print(F("\t"));
    Serial.print(sensorInfo.isWet(sID) );
    Serial.print(F("\t"));
    Serial.print(sensorInfo.RemoteTemperature(sID));
    Serial.print(F("\t"));
    Serial.print(sensorInfo.remoteBattVolts(sID));
    Serial.print(F("\t"));
    Serial.println(sensorInfo.RemoteSignalStrength(sID)); 
  }
  else
  {
    Serial.print(F("Error getting sensor data. Err: "));
    Serial.print(stat);
    Serial.print(F(" for sensor # "));
    Serial.println(sID);
  }
  delay(2000);

}  // end loop()





