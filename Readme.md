I2C Testing between two Arduinos. 

First test using wire library. Then try DSS Circuits I2C library to see if it uses less RAM.  https://github.com/rambo/I2C

When wiring two Arduinos that are both 5 volts, I2C lines are both use a 1.5k pull-up resistor.  When using one 5v arduino and one 3 volt, use 4.7k pull-up resistors to 3.3v line.


