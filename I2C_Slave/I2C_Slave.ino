/*
I2C Test - this is the slave
Sends 14 bytes to Master.  Has a mix of data formats

*/

#include <Wire.h>     // http://arduino.cc/it/Reference/Wire
#define PACKET_SIZE 14

const byte addrSlaveI2C =  21;  // I2C Slave address of this device
byte I2C_Packet[PACKET_SIZE];            // Array to hold data sent over I2C to main Arduino

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Initialize wire library for slave I2C"));
  Wire.begin(addrSlaveI2C);    // Initiate the Wire library and join the I2C bus 
  Wire.onRequest(wireRequestEvent); // Register a function to be called when a master requests data from this slave device. 
}


void loop()
{
 uint32_t mS = millis();
 uint16_t mV = 3000;
 uint16_t temp = 75; 

 // convert float to byte array  source: http://mbed.org/forum/helloworld/topic/2053/
 typedef union float2bytes_t   // union consists of one variable represented in a number of different ways 
 { 
    float f; 
    byte b[sizeof(float)]; 
 }; 
 
 I2C_Packet[0] = addrSlaveI2C;
 I2C_Packet[1] = 2;  // command ID

 I2C_Packet[2] = mV >> 8 & 0xff; ; // millivolts
 I2C_Packet[3] = mV & 0xff;

 I2C_Packet[4] = temp >> 8 & 0xff; // Temperature
 I2C_Packet[5] = temp & 0xff;

  // Convert float to 4-byte array
  float2bytes_t f2b; 
  f2b.f = 1234.567;
  I2C_Packet[6] = f2b.b[0];
  I2C_Packet[7] = f2b.b[1];
  I2C_Packet[8] = f2b.b[2];
  I2C_Packet[9] = f2b.b[3];

 // convert from an unsigned long int to a 4-byte array
 I2C_Packet[10] = (int)((mS >> 24) & 0xFF) ;
 I2C_Packet[11] = (int)((mS >> 16) & 0xFF) ;
 I2C_Packet[12] = (int)((mS >> 8) & 0XFF);
 I2C_Packet[13] = (int)((mS & 0XFF));

  Serial.print(I2C_Packet[0]);
  Serial.print("\t");
  Serial.print(I2C_Packet[1]);
  Serial.print("\t");
  Serial.print(mV);
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print(f2b.f);
  Serial.print("\t");
  Serial.println(mS);
  delay(1000);
}

// Send data to Master.  This is an interrupt driven event
void wireRequestEvent()
{
  // Send byte array from panStamp. Main Arduino will decode bytes
  Wire.write(I2C_Packet, PACKET_SIZE); 
} // wireRequestEvent()


