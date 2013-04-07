/*
I2C Test - this is the slave
I2C communication between two Arduinos.  Use this with I2C_Master_DSS_struct.ino

Sends 14 bytes to Master.  Put register map in a typedef struct and use a union to put in a byte array

Scenario for register map - Connected to the slave arduino are some wireless water detectors and wired water detector.  
Both have a wet/dry status.  But the wireless also have temperature, voltage of wireless Tx battery, and wireless signal strength

Register Map:
byte  Desc 
 0    status (slave status)
 1    sensor Id
 2    sensor type: 0=wireless, 1=wired
 3    Wet/Dry Status 
 4    temperature (unisgned int), (wireless only)
 5    temperature
 6    volts (float) (wireless only)
 7    volts
 8    volts
 9    volts
10    Signal Strengh (wireless only)

*/

#include <Wire.h>     // http://arduino.cc/it/Reference/Wire

typedef struct sensorData_t{
  byte stat;
  byte sensorId;
  byte sensortype;
  byte isWet;
  uint16_t temp;
  float volts;
  byte signal;
};

typedef union I2C_Packet_t{
 sensorData_t sensor;
 byte I2CPacket[sizeof(sensorData_t)];
};

#define PACKET_SIZE sizeof(sensorData_t)

I2C_Packet_t leakinfo;  


const byte addrSlaveI2C =  21;  // I2C Slave address of this device
bool printDataflag = false;

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Initialize wire library for slave I2C"));
  Wire.begin(addrSlaveI2C);    // Initiate the Wire library and join the I2C bus 
  Wire.onRequest(wireRequestEvent); // Register a function to be called when a master requests data from this slave device. 
}


void loop()
{

  leakinfo.sensor.stat = 0; 
  leakinfo.sensor.sensorId = 22; 
  leakinfo.sensor.sensortype = 0; 
  leakinfo.sensor.isWet = 0; 
  leakinfo.sensor.temp = 75; 
  leakinfo.sensor.volts = 3141 / 1000.0; 
  leakinfo.sensor.signal = 88; 
  
  
  // Print out data every time the Master makes a request
  if (printDataflag)
  {
    PrintI2CByteArray();
    printDataflag = false;
  }
  
}

// Send data to Master.  This is an interrupt driven event
void wireRequestEvent()
{
  // Send byte array from panStamp. Main Arduino will decode bytes
  Wire.write(leakinfo.I2CPacket, PACKET_SIZE); 
  printDataflag = true;
  
} 

// Print out data that will be sent to the master
void PrintSensorData()
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

// Print the byt array going out I2C
void PrintI2CByteArray()
{
  for(int i=0; i<PACKET_SIZE; i++)
  {
    Serial.print(leakinfo.I2CPacket[i]);
    Serial.print("\t");
  }
  Serial.println();  
}




