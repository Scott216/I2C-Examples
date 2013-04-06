// I2C master using DSS library

#include <I2C.h>

const byte addrSlaveI2C =  21;
#define PACKET_SIZE 14
byte i2CData[PACKET_SIZE];

typedef struct {
  byte slaveID;
  byte sensorID;
  byte misc;
  int temp;
  int mV;
  int val1;
  long longnum;
  byte stuff;
} RemoteSensorData_t;

bool getData(RemoteSensorData_t* sensorInfo);

void setup () 
{
  Serial.begin(9600);
  I2c.begin();

}

void loop()
{

 RemoteSensorData_t sensor1; 
 getData( &sensor1 );
 

 delay(2000);
 
}



bool getData(RemoteSensorData_t* sensorInfo)
{

  I2c.read(addrSlaveI2C, PACKET_SIZE , i2CData); //request data and store directly to array

  // If we got an I2C packet, we can process it
  if(I2c.available())
  {
     Serial.println();
    for (int k=0; k<PACKET_SIZE; k++)
    { 
      Serial.print(i2CData[k]); 
      Serial.print("\t");
    }
    Serial.println();
  }
  
  return true;
}

