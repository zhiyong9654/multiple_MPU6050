#include "I2Cdev.h"
#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;

class Gyro {
  private:
  // stores state of gyro
  bool dmpReady = false;
  uint8_t devStatus;
  bool print_out = false;

  // stores pin out information of gyro
  int add_sel_pin;          // AD0 pin of MPU6050
  int *other_add_sel_pins;  // AD0 pins of other MPU6050s
  int no_other_add_sel_pins;// Number of other MPU6050s
  
  // data from gyro
  Quaternion q;
  VectorFloat gravity;
  VectorInt16 aa;         // [x, y, z]
  VectorInt16 aaReal;     // [x, y, z]
  float ypr[3];
  uint16_t packetSize;
  uint16_t fifoCount;
  uint8_t fifoBuffer[64];
  
  public:
  void make_active() {
    for (int i = 0; i < no_other_add_sel_pins; i++) {
      digitalWrite(other_add_sel_pins[i], HIGH);
    digitalWrite(add_sel_pin, LOW);
    }
  }
  void init(int address_select_pin, int *other_address_select_pins, int no_other_address_select_pins, bool printout = false) {
    add_sel_pin = address_select_pin;
    other_add_sel_pins = other_address_select_pins;
    no_other_add_sel_pins = no_other_address_select_pins;
    print_out = printout;
    
    make_active();
    Serial.print(add_sel_pin);
    Serial.println(F(" sensor: Initializing I2C devices..."));
    mpu.initialize();
    Serial.print(add_sel_pin);
    Serial.println(mpu.testConnection() ? F(" sensor: MPU6050 connection successful") : F(" sensor: MPU6050 connection failed"));
    devStatus = mpu.dmpInitialize();
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
    
    if (devStatus == 0) {
      Serial.print(add_sel_pin);
      Serial.println(F(" sensor: DMP Initialization successful"));
      dmpReady = true;
      delay(1000);    // wait for sensor to stabilize
      }
    else {
      Serial.print(add_sel_pin);
      Serial.print(F(" sensor: DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
    }
  }
  
  void get_reading() {
    make_active();
    fifoCount = mpu.getFIFOCount();
    if (fifoCount >= packetSize && fifoCount < 1024) {
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      mpu.dmpGetAccel(&aa, fifoBuffer);
      mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
      if (print_out) {
        Serial.print(add_sel_pin);
        Serial.print(" Yaw:");
        Serial.print(ypr[0] * 180/M_PI);
        Serial.print("\t Pitch:");
        Serial.print(ypr[1] * 180/M_PI);
        Serial.print("\t Roll:");
        Serial.print(ypr[2] * 180/M_PI);
        Serial.print("\t aareal_X:");
        Serial.print(aaReal.x);
        Serial.print("\t aareal_Y:");
        Serial.print(aaReal.y);
        Serial.print("\t aareal_Z:");
        Serial.println(aaReal.z);
      }
    }
    else if (fifoCount >= 1024) {
      mpu.resetFIFO();
      Serial.print(add_sel_pin);
      Serial.println(F("th sensor pin: FIFO OF"));
    }
  }
};

const int no_add_sel_pins = 4;
int add_sel_pins[] = {2,3,4,5};
const int no_other_pins = no_add_sel_pins - 1;

Gyro gyro1;
int other_pins1[3] = {3,4,5};
Gyro gyro2;
int other_pins2[3] = {2,4,5};
Gyro gyro3;
int other_pins3[3] = {2,3,5};
Gyro gyro4;
int other_pins4[3] = {2,3,4};

void setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  Serial.begin(115200);
  for (int i = 0; i < no_add_sel_pins; i++) {
    pinMode(add_sel_pins[i], OUTPUT);
  }
  
  gyro1.init(2, other_pins1, no_other_pins, true);
  gyro2.init(3, other_pins2, no_other_pins, true);
  gyro3.init(4, other_pins3, no_other_pins);
  gyro4.init(5, other_pins4, no_other_pins);
}

void loop() {
  gyro1.get_reading();
  gyro2.get_reading();
  gyro3.get_reading();
  gyro4.get_reading();
}
