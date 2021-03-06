# Script for multiple MPU6050s on an Arduino
While working on a personal project I realized there weren't any ready made scripts for working with multiple MPU6050s on the Arduino. So I decided to share the one I wrote, it utilizes the fantastic MPU6050 library by Jeff Rowberg: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050.  

With this script and 4 sensors, on a 16mhz Arduino nano, I was getting roughly 18hz per sensor (18 readings per sensor, per second). This can be sped up significantly if you don't require all of the DMP values listed below.

It uses the trick detailed on the MPU6050 arduino page to achieve functionality with multiple MPU6050s. (https://playground.arduino.cc/Main/MPU-6050/)  

Since multiple MPU6050s are being used, this should be used in dynamic projects, like robots or quadcopters, hence I did not implement support for the interrupt pin.

## Get started
1. Install Jeff Rowberg's MPU6050 library. I did this by using Arduino's IDE and searching for MPU6050.
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

## Issues
If you run into FIFO overflow very often, it is likely that the FIFO buffer is being filled up way faster than it's being read. This can occur when you have many MPU6050s working at the same time. This can be addressed by tweaking the MPU6050 library itself, edit the file in ../MPU6050/src/MPU6050_6Axis_MotionApps20.h, and look for:  
``` c++
0x02,   0x16,   0x02,   0x00, 0x04 // D_0_22 inv_set_fifo_rate
```
The value you're interested in is the last value before the comment, 0x04.  
or Updated 2 Mar 2020:
``` c++
#define MPU6050_DMP_FIFO_RATE_DIVISOR 0x01
```

By tweaking this number, you can set the desired frequency for your MPU6050s. For example, at 0x04, frequency = (200/(1+4)) = 40hz. At 0x03, frequency is (200/(1+3)) = 50 hz.  
This is described here: (https://www.i2cdevlib.com/forums/topic/27-fifo-overflow/)
