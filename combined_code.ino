#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 0;  // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPin1 = 18;
int servoPin2 = 19;

#define RXD2 16
#define TXD2 17
int dist;     /*----actual distance measurements of LiDAR---*/
int strength; /*----signal strength of LiDAR----------------*/
float temperature;
unsigned char check; /*----save check value------------------------*/
int i;
unsigned char uart[9];       /*----save data measured by LiDAR-------------*/
const int HEADER = 0x59;     /*----frame header of data package------------*/
int rec_debug_state = 0x01;  //receive state for frame
bool motorRotated = false; // Flag to track motor rotation 

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("\nBenewake TFmini-S UART LiDAR Program");
  Serial2.begin(115200);

  // Servo setup
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(300);           // standard 50 hz servo
  myservo.attach(servoPin1, 100, 200000);  // attaches the servo on pin 18 to the servo object
  myservo.attach(servoPin2, 100, 200000);  // attaches the servo on pin 19 to the servo object
  Serial.println(pos);
}


void Get_Lidar_data() {
  if (Serial2.available()) {  //check if serial port has data input
    if (rec_debug_state == 0x01) {
      //the first byte
      uart[0] = Serial2.read();
      if (uart[0] == 0x59) {
        check = uart[0];
        rec_debug_state = 0x02;
      }
    } else if (rec_debug_state == 0x02) {
      //the second byte
      uart[1] = Serial2.read();
      if (uart[1] == 0x59) {
        check += uart[1];
        rec_debug_state = 0x03;
      } else {
        rec_debug_state = 0x01;
      }
    } else if (rec_debug_state == 0x03) {
      uart[2] = Serial2.read();
      check += uart[2];
      rec_debug_state = 0x04;
    } else if (rec_debug_state == 0x04) {
      uart[3] = Serial2.read();
      check += uart[3];
      rec_debug_state = 0x05;
    } else if (rec_debug_state == 0x05) {
      uart[4] = Serial2.read();
      check += uart[4];
      rec_debug_state = 0x06;
    } else if (rec_debug_state == 0x06) {
      uart[5] = Serial2.read();
      check += uart[5];
      rec_debug_state = 0x07;
    } else if (rec_debug_state == 0x07) {
      uart[6] = Serial2.read();
      check += uart[6];
      rec_debug_state = 0x08;
    } else if (rec_debug_state == 0x08) {
      uart[7] = Serial2.read();
      check += uart[7];
      rec_debug_state = 0x09;
    } else if (rec_debug_state == 0x09) {
      uart[8] = Serial2.read();
      if (uart[8] == check) {
        dist = uart[2] + uart[3] * 256;         //the distance
        strength = uart[4] + uart[5] * 256;     //the strength
        temperature = uart[6] + uart[7] * 256;  //calculate chip temprature
        temperature = temperature / 8 - 256;
        Serial.print("dist = ");
        Serial.print(dist);  //output measure distance value of LiDAR
        Serial.print('\n');
        Serial.print("strength = ");
        Serial.print(strength);  //output signal strength value
        Serial.print('\n');
        Serial.print("\t Chip Temperature = ");
        Serial.print(temperature);
        Serial.println(" celcius degree");               //output chip temperature of Lidar
        while (Serial2.available()) { Serial2.read(); }  // This part is added becuase some previous packets are
        //there in the buffer so to clear serial buffer and get fresh data.
        delay(100);
      }
      rec_debug_state = 0x01;
    }
  }
}



void loop() {
  Get_Lidar_data();

  // Check the LiDAR reading and control the motors
  if (dist <= 100) {
    if (!motorRotated) {
      for (pos = 0; pos <= 180; pos += 50) {
        myservo.write(pos);
        delay(0);
      }
      motorRotated = true; // Set the flag to indicate motor rotation
    }
  } else {
    myservo.write(0); // Reset the motor to 0 degrees when LiDAR range is not detected
    motorRotated = false; // Reset the flag
  }
}


