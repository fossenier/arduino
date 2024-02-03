/*
 */
#include <Servo.h>

Servo myServo;

int potentiometer_pin = A0;
int servo_pin = 9;
int potentiometer_value;
int servo_angle;

void setup()
{
    myServo.attach(servo_pin);

    Serial.begin(9600);
}

void loop()
{
    potentiometer_value = analogRead(potentiometer_pin);
    Serial.print("Potentiometer value: ");
    Serial.print(potentiometer_value);

    servo_angle = map(potentiometer_value, 0, 1023, 0, 180);
    Serial.print("Servo angle: ");
    Serial.println(servo_angle);

    myServo.write(servo_angle);

    delay(15);
}
