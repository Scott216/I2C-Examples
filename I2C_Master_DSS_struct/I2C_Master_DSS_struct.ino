/*
I2C Test - Master
I2C communication between two Arduinos.  Use this with I2C_Slave_struct.ino

Uses DSS's I2C.h library, not the Wire.h library.
Uses union and typedef struct to convert byte array from I2C back to usable varible

*/ 


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

#define PACKET_SIZE sizeof(sensorData_t)

I2C_Packet_t leakinfo;

#include <I2C.h>   // https://github.com/rambo/I2C
#define PACKET_SIZE 11     // I2C packet size
const uint8_t addrSlaveI2C = 21;    // ID of I2C slave.  Don't use #define

// Function prototype
int getData();

void setup () 
{
  Serial.begin(9600);
  Serial.println(F("Initialize library for I2C Master Test"));
  I2c.begin();  // enables the I2C hardware
  I2c.timeOut(10000);
  Serial.println(F("Finished setup"));
  
} // end setup()


void loop()
{
  static byte getSensorID; 
  
  // Alternate between sensors
  if (getSensorID == 22)
  { getSensorID = 23;}
  else
  { getSensorID = 22;}
  

  int i2cStatus = getData(getSensorID);  // get data for sensor # 22
  if( i2cStatus == 0)
  {
    Serial.print(leakinfo.sensor.stat);
    Serial.print("\t");
    Serial.print(leakinfo.sensor.sensorId);
    Serial.print("\t");
    Serial.print(leakinfo.sensor.sensortype);
    Serial.print("\t");
    Serial.print(leakinfo.sensor.isWet );
    Serial.print("\t");
    Serial.print(leakinfo.sensor.temp);
    Serial.print("\t");
    Serial.print(leakinfo.sensor.volts);
    Serial.print("\t");
    Serial.println(leakinfo.sensor.signal);
  }
  else
  {
    Serial.print(F("No packet received, error: "));
    Serial.println(i2cStatus);
  }
 
  delay(2000);
}  // end loop()


// I2C Request data from slave
int getData(uint8_t sensorID)
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
    return 0;
  }  // end got packet
  else
  { return readstatus; } // No Packet received
  
} // end getData()



