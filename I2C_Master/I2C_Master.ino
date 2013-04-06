// 
/*
I2C Test - Master

Note: you can't to bitwise ops on the typedef, don't know why. For example, don't do this:
 sensorInfo->mV   = i2CData[2] << 8;
 sensorInfo->mV  |= i2CData[3];

*/

typedef struct  {
  byte slaveID;
  byte cmdID;
  uint16_t mV;
  uint16_t temp;
  float myFloat;  // some float number
  uint32_t mS;    // millis() from slave
} RemoteSensorData_t;

typedef union float2bytes_t   // union consists of one variable represented in a number of different ways 
{ 
  float f; 
  char b[sizeof(float)]; 
}; 

#include <Wire.h>          // For I2C communication with panStamp, http://www.arduino.cc/en/Reference/Wire
#define PACKET_SIZE 14     // I2C packet size, slave will send 14 bytes to master
#define addrSlaveI2C 21    // ID of i@C slave

// Function prototype
bool getData(RemoteSensorData_t* sensorInfo);

void setup () 
{
  Serial.begin(9600);
  delay(2000);
  Serial.print("RAM = ");
  Serial.println(freeRam());
  Serial.println(F("Initialize library for I2C Master Test"));
  Wire.begin();  // Initialiae wire library for I2C communication
  Serial.println(F("Finished setup"));
  Serial.print("RAM = ");
  Serial.println(freeRam());
  
} // end setup()


void loop()
{
  RemoteSensorData_t sensor1; 
  if( getData( &sensor1 ))
  {
    Serial.print(sensor1.slaveID);
    Serial.print("\t");
    Serial.print(sensor1.cmdID);
    Serial.print("\t");
    Serial.print(sensor1.mV);
    Serial.print("\t");
    Serial.print(sensor1.temp);
    Serial.print("\t");
    Serial.print(sensor1.myFloat);
    Serial.print("\t");
    Serial.print(sensor1.mS);

    Serial.print("   RAM:");
    Serial.print(freeRam());
    Serial.println();
  }
  else
  {
    Serial.println(F("No packet received"));
  }
 
  delay(2000);
}  // end loop()


// I2C Request data from slave
bool getData(RemoteSensorData_t* sensorInfo)
{
  float2bytes_t b2f;
  bool gotI2CPacket = false;    
  byte i=0;
  byte i2CData[PACKET_SIZE];
  
  Wire.requestFrom(addrSlaveI2C, PACKET_SIZE);    // request data from I2C slave
  
  while(Wire.available())    // slave may send less than requested
  { 
    i2CData[i++] = Wire.read(); // receive a byte of data
    gotI2CPacket = true;  // Flag to indicate sketch received I2C packet
  }

  // If we got an I2C packet, we can extact the values
  if(gotI2CPacket)
  {
    gotI2CPacket = false;  // Reset flag

    sensorInfo->slaveID = i2CData[0];  // Slave address
    sensorInfo->cmdID   = i2CData[1];  // some sort of command ID

    sensorInfo->mV    = i2CData[2] << 8;
    sensorInfo->mV   |= i2CData[3];
    sensorInfo->temp  = i2CData[4] << 8;
    sensorInfo->temp |= i2CData[5];

    // Get floating point number    
    b2f.b[0] = i2CData[6];
    b2f.b[1] = i2CData[7];
    b2f.b[2] = i2CData[8];
    b2f.b[3] = i2CData[9];
    sensorInfo->myFloat = b2f.f;
    
    // Get long integer - milliseconds
    sensorInfo->mS = ( ((unsigned long)i2CData[10] << 24)
                     + ((unsigned long)i2CData[11] << 16)
                     + ((unsigned long)i2CData[12] <<  8)
                     + ((unsigned long)i2CData[13] ) );  
    
    return true;
  }  // end got packet
  else
  { return false; } // No Packet received
  
} // end getData



//====================================================================================================
// Displays the amount of freem SRAM
//====================================================================================================
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
