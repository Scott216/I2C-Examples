I2C Testing between two Arduinos. 

The slave sends 14 bytes of data.  It sends a several different data types: bets, integer, long integer, and float

I2C_Slave.ino is the sketch for the slave arduino that sends the data to the master when requested.
I2C_Master.ino uses the wire.h library to request data from the slave.  
I2C_Master_DSS.ino uses a different library (https://github.com/rambo/I2C) for I2C communication.  I wanted to try it because it uses less RAM then wire.h.

When wiring two Arduinos that are both 5 volts, I2C lines both use a 1.5k pull-up resistor to +5v.  When using one 5 volt arduino and one 3.3 volt, use 4.7k pull-up resistors to +3.3v line.

More info on the i2c.h library is can be found here: http://dsscircuits.com/articles/arduino-i2c-master-library.html


                          Expecting   Actual
leakinfo.sensor.stat          0        231
leakinfo.sensor.sensorId     22          0
leakinfo.sensor.sensortype    0          1
leakinfo.sensor.isWet         0          0 
leakinfo.sensor.temp         75      25344
leakinfo.sensor.volts      3.14          0
leakinfo.sensor.signal       88         155
