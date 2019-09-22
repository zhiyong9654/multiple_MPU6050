# Script for multiple MPU6050s on an Arduino
While working on a personal project I realized there weren't any ready made scripts for working with multiple MPU6050s on the Arduino. So I decided to share the one I wrote, it utilizes the fantastic MPU6050 library by Jeff Rowberg: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050.  

It uses the trick detailed on the MPU6050 arduino page to achieve functionality with multiple MPU6050s. (https://playground.arduino.cc/Main/MPU-6050/)  

Since multiple MPU6050s are being used, this should be used in dynamic projects, like robots or quadcopters, hence I did not implement support for the interrupt pin.

## Get started
1. Wire up your gyroscopes as per the wiring schema detailed below.
1. Define how many gyroscopes are in use and their pin outs (see how to use).
1. Initialize them in setup() as per the sample script.
1. Run get_reading() within the loop and extend upon it as you like.

## Wiring schema
1. AD0 - Digital pins (My example uses pins 2,3,4,5, for a total of 4 gyroscopes.)
1. SCL - SCL
1. SDA - SDA
1. Vin - 5v (I'm using the GY-521 breakout board which accepts 5v)
1. GND - GND

## How to use?
Define your setup in the global scope
``` c++
const int no_add_sel_pins = 4; 	// How many MPU6050s are you using?
int add_sel_pins[] = {2,3,4,5};	// What pins are their AD0s connected to?
//...
// Define the Gyro object
Gyro gyro1;
int other_pins1[3] = {3,4,5};	// gyro1 AD0 pin is connected to pin 2, so pin 3,4,5 is considered other_pins
Gyro gyro2;
int other_pins2[3] = {2,4,5};	// gyro2 AD0 pin is connected to pin 3, so pin 2,4,5 is considered other_pins
//..
```

Initialize the gyroscopes in setup()
``` c++
gyro1.init(2, other_pins1, true); // (AD0 pin for this sensor, array containing AD0 pins for other sensors, print out sensor values if true)
//..
```

Get reading in loop()
``` c++
gyro1.get_reading();
```


