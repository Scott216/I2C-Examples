/*
I2C Test - Master
I2C communication between two Arduinos. The master will make a request from the slave.  The slave will respond.
In this example, the slave will send 14 bytes of data.  I have a combination of bytes, integer, long unisgned integer and float data

Testing library from DSS circuits http://github.com/rambo/I2C to see if it uses less RAM then wire.h library, and it does. 168 fewer bytes
Slave arduino still needs to run wire.h library.  DSS library doesn't support slave communication (as of 4/6/13)

Reference: http://dsscircuits.com/articles/arduino-i2c-master-library.html

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

#include <I2C.h>   // https://github.com/rambo/I2C
#define PACKET_SIZE 14     // I2C packet size, slave will send 14 bytes to master
#define addrSlaveI2C 21    // ID of i@C slave

// Function prototype
int getData(RemoteSensorData_t* sensorInfo);

void setup () 
{
  Serial.begin(9600);
  delay(2000);
  Serial.print("RAM = ");
  Serial.println(freeRam());
  Serial.println(F("Initialize library for I2C Master Test"));
  I2c.begin();
  I2c.timeOut(30000);
  Serial.println(F("Finished setup"));
  Serial.print("RAM = ");
  Serial.println(freeRam());
  
} // end setup()


void loop()
{
  RemoteSensorData_t sensor1; 

  int i2cStatus = getData( &sensor1 );
  if( i2cStatus == 0)
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
    Serial.print("    ");
    Serial.print(sensor1.mS);

    Serial.print("   RAM:");
    Serial.print(freeRam());
    Serial.println();
  }
  else
  {
    Serial.print(F("No packet received, error: "));
    Serial.println(i2cStatus);
  }
 
  delay(2000);
}  // end loop()


// I2C Request data from slave
int getData(RemoteSensorData_t* sensorInfo)
{
  float2bytes_t b2f;
  byte i2CData[PACKET_SIZE];

  int readstatus = I2c.read(addrSlaveI2C, PACKET_SIZE , i2CData); //request data and store directly to i2CData array
  
  // If we got an I2C packet, we can extact the values
  if(readstatus == 0)
  {
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
    
    return 0;
  }  // end got packet
  else
  { return readstatus; } // No Packet received
  
} // end getData



//====================================================================================================
// Displays the amount of freem SRAM
//====================================================================================================
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

