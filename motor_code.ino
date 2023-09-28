#include <ESP32Servo.h>

Servo myservo; // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int servoPin1 = 18;
int servoPin2 = 19;
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
    myservo.attach(servoPin1, 100, 200000); // attaches the servo on pin 18 to the servo object
     myservo.attach(servoPin2, 100, 200000); // attaches the servo on pin 18 to the servo object
}

void loop()
{
    // Set the servo to a neutral position
    myservo.write(90);

    // Read an angle from the serial port
    int angle;
    Serial.println("Enter an angle between 0 and 270: ");
    while (!Serial.available()) {
        delay(10);
    }
    angle = Serial.parseInt();

    // Move the servo to the desired angle
    myservo.write(angle);

    // Wait for 1 second
    delay(1000);
}