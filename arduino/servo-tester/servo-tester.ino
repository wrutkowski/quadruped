#include <Servo.h>

#define HIP_MODE    0
#define THIGH_MODE  1
#define TIBIA_MODE  2

const int buttonPin = 3;
const int analogInPin = A2;
const int hipServoPin = 5;
const int thighServoPin = 6;
const int tibiaServoPin = 7;

Servo hipServo;
Servo thighServo;
Servo tibiaServo;

int hipServoValue = 0;
int thighServoValue = 0;
int tibiaServoValue = 0;

int mode = HIP_MODE;

void setup() {
    Serial.begin(9600);

    pinMode(buttonPin, INPUT);
    
    hipServo.attach(hipServoPin);
    thighServo.attach(thighServoPin);
    tibiaServo.attach(tibiaServoPin);
}

void loop() {
    int sensorValue = analogRead(analogInPin);
    if (mode == HIP_MODE) {
        hipServoValue = map(sensorValue, 0, 1023, 0, 180);
    } else if (mode == THIGH_MODE) {
        thighServoValue = map(sensorValue, 0, 1023, 0, 180);
    } else if (mode == TIBIA_MODE) {
        tibiaServoValue = map(sensorValue, 0, 1023, 0, 180);
    }

    hipServo.write(hipServoValue);
    thighServo.write(thighServoValue);
    tibiaServo.write(tibiaServoValue);
    
    Serial.print("hipServoValue: ");
    Serial.print(hipServoValue);
    Serial.print(", thighServoValue: ");
    Serial.print(thighServoValue);
    Serial.print(", tibiaServoValue: ");
    Serial.println(tibiaServoValue);

    if (digitalRead(buttonPin) == HIGH) {
        mode++;
        if (mode > TIBIA_MODE) mode = HIP_MODE;

        if (mode == HIP_MODE) {
            Serial.println("Mode: HIP");
        } else if (mode == THIGH_MODE) {
            Serial.println("Mode: THIGH");
        } else if (mode == TIBIA_MODE) {
            Serial.println("Mode: TIBIA");
        }

        // waiting for release
        while (digitalRead(buttonPin) == HIGH) {
            delay(10);
        }
    }

    delay(200);
}
