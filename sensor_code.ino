#include <ESP32Servo.h>

Servo myservo; // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int servoPin = 18;

void setup()
{
    // Initialize Serial communication
    Serial.begin(115200);

    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    myservo.setPeriodHertz(330); // standard 50 Hz servo
    myservo.attach(servoPin, 100, 200000); // attaches the servo on pin 18 to the servo object
}

void loop()
{
    // Move the servo to its minimum position
    myservo.write(0);
    delay(1000);

    // Move the servo to its maximum position
    myservo.write(270);
    delay(1000);

    // Set the servo to a neutral position
    myservo.write(90);

    // Print the current servo angle to the serial port
    Serial.print("Servo angle: ");
    Serial.println(myservo.read());
}
