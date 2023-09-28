#include <ESP32Servo.h>

Servo myservo; // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 0; // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPin1 = 18;
int servoPin2 = 19;

void setup()
{
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    myservo.setPeriodHertz(50); // standard 50 Hz servo
    myservo.attach(servoPin1, 1000, 2000); // attaches the servo on pin 18 to the servo object
    myservo.attach(servoPin2, 1000, 2000); // attaches the servo on pin 19 to the servo object
    // using default min/max of 1000us and 2000us
    // different servos may require different min/max settings
    // for an accurate 0 to 180 sweep
}

void loop()
{
    for (pos = 0; pos <= 180; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.writeMicroseconds(map(pos, 0, 180, 1000, 2000)); // set servo position in microseconds
        delay(1); // waits 1ms for the servo to reach the position
    }

    for (pos = 180; pos >= 0; pos -= 1)
    { // goes from 180 degrees to 0 degrees
        myservo.writeMicroseconds(map(pos, 0, 180, 1000, 2000)); // set servo position in microseconds
        delay(1); // waits 1ms for the servo to reach the position
    }
}
