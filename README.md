# Script for multiple MPU6050s on an Arduino
While working on a personal project I realized there weren't any ready made scripts for working with multiple MPU6050s on the Arduino. So I decided to share the one I wrote, it utilizes the fantastic MPU6050 library by Jeff Rowberg: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050.  

## Wiring diagram
Here's a picture of how I wired up multiple MPU6050s to the Arduino. If you've done some reading you would know that this sensor has an address select pin on AD0. So when HIGH is applied to it, it's address changes to 0x69 instead of 0x68. This behaviour allows for multiple MPU6050s to work concurrently. I have tested up to 5 for simple gyroscope reading.  
1. AD0 - Digital pins
1. SCL - SCL
1. SDA - SDA
1. Vin - 5v (I'm using the GY-521 breakout board which accepts 5v)
1. GND - GND

// pic here


