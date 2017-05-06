#include <Servo.h>

const int leg1hipServoPin =   4;
const int leg1thighServoPin = 3;
const int leg1tibiaServoPin = 2;
const int leg2hipServoPin =   7;
const int leg2thighServoPin = 6;
const int leg2tibiaServoPin = 5;
const int leg3hipServoPin =   8;
const int leg3thighServoPin = 9;
const int leg3tibiaServoPin = 10;
const int leg4hipServoPin =   11;
const int leg4thighServoPin = 12;
const int leg4tibiaServoPin = 13;

Servo leg1hipServo;
Servo leg1thighServo;
Servo leg1tibiaServo;
Servo leg2hipServo;
Servo leg2thighServo;
Servo leg2tibiaServo;
Servo leg3hipServo;
Servo leg3thighServo;
Servo leg3tibiaServo;
Servo leg4hipServo;
Servo leg4thighServo;
Servo leg4tibiaServo;

// sit, legs up
//int leg1hipServoValue = 160;
//int leg1thighServoValue = 160;
//int leg1tibiaServoValue = 90;
//int leg2hipServoValue = 50;
//int leg2thighServoValue = 60;
//int leg2tibiaServoValue = 90;
//int leg3hipServoValue = 50;
//int leg3thighServoValue = 40;
//int leg3tibiaServoValue = 90;
//int leg4hipServoValue = 180;
//int leg4thighServoValue = 170;
//int leg4tibiaServoValue = 90;

// stand
int leg1hipServoValue = 160;
int leg1thighServoValue = 70;
int leg1tibiaServoValue = 90;
int leg2hipServoValue = 30;
int leg2thighServoValue = 130;
int leg2tibiaServoValue = 90;
int leg3hipServoValue = 50;
int leg3thighServoValue = 120;
int leg3tibiaServoValue = 90;
int leg4hipServoValue = 180;
int leg4thighServoValue = 50;
int leg4tibiaServoValue = 90;

void setup() {
    Serial.begin(115200);
    Serial.println("Accepted commands: LEG_NUMBER,JOINT_NUMBER,VALUE");
    Serial.println("   LEG_NUMBER   1...4");
    Serial.println("   JOINT_NUMBER 1 - hip, 2 - thigh, 3 - tibia");
    Serial.println("   VALUE 0...180");
    Serial.println("   * Example: 1,1,90");
    Serial.println();
    
    leg1hipServo.attach(leg1hipServoPin);
    leg1thighServo.attach(leg1thighServoPin);
    leg1tibiaServo.attach(leg1tibiaServoPin);
    leg2hipServo.attach(leg2hipServoPin);
    leg2thighServo.attach(leg2thighServoPin);
    leg2tibiaServo.attach(leg2tibiaServoPin);
    leg3hipServo.attach(leg3hipServoPin);
    leg3thighServo.attach(leg3thighServoPin);
    leg3tibiaServo.attach(leg3tibiaServoPin);
    leg4hipServo.attach(leg4hipServoPin);
    leg4thighServo.attach(leg4thighServoPin);
    leg4tibiaServo.attach(leg4tibiaServoPin);

    printServoStatus();

    setServos();
}

void loop() {
    if (Serial.available()) {
        String legString = Serial.readStringUntil(',');
        int leg = legString.toInt();
        String jointString = Serial.readStringUntil(',');
        int joint = jointString.toInt();
        String valueString = Serial.readString();
        int value = valueString.toInt();

        Serial.print("> ");
        Serial.print(legString);
        Serial.print(",");
        Serial.print(jointString);
        Serial.print(",");
        Serial.print(valueString);
        Serial.println("OK");

        if (leg == 1) {
            if (joint == 1) {
                leg1hipServoValue = value;
            } else if (joint == 2) {
                leg1thighServoValue = value;
            } else if (joint == 3) {
                leg1tibiaServoValue = value;
            } 
        } else if (leg == 2) {
            if (joint == 1) {
                leg2hipServoValue = value;
            } else if (joint == 2) {
                leg2thighServoValue = value;
            } else if (joint == 3) {
                leg2tibiaServoValue = value;
            } 
        } else if (leg == 3) {
            if (joint == 1) {
                leg3hipServoValue = value;
            } else if (joint == 2) {
                leg3thighServoValue = value;
            } else if (joint == 3) {
                leg3tibiaServoValue = value;
            } 
        } else if (leg == 4) {
            if (joint == 1) {
                leg4hipServoValue = value;
            } else if (joint == 2) {
                leg4thighServoValue = value;
            } else if (joint == 3) {
                leg4tibiaServoValue = value;
            } 
        }
        
        printServoStatus();

        setServos();
    }
}

void setServos() {
    leg1hipServo.write(leg1hipServoValue);
    leg1thighServo.write(leg1thighServoValue);
    leg1tibiaServo.write(leg1tibiaServoValue);
    leg2hipServo.write(leg2hipServoValue);
    leg2thighServo.write(leg2thighServoValue);
    leg2tibiaServo.write(leg2tibiaServoValue);
    leg3hipServo.write(leg3hipServoValue);
    leg3thighServo.write(leg3thighServoValue);
    leg3tibiaServo.write(leg3tibiaServoValue);
    leg4hipServo.write(leg4hipServoValue);
    leg4thighServo.write(leg4thighServoValue);
    leg4tibiaServo.write(leg4tibiaServoValue);
}

void printServoStatus() {
    Serial.print("Leg1: ");
    Serial.print(leg1hipServoValue);
    Serial.print(", ");
    Serial.print(leg1thighServoValue);
    Serial.print(", ");
    Serial.println(leg1tibiaServoValue);
    Serial.print("Leg2: ");
    Serial.print(leg2hipServoValue);
    Serial.print(", ");
    Serial.print(leg2thighServoValue);
    Serial.print(", ");
    Serial.println(leg2tibiaServoValue);
    Serial.print("Leg3: ");
    Serial.print(leg3hipServoValue);
    Serial.print(", ");
    Serial.print(leg3thighServoValue);
    Serial.print(", ");
    Serial.println(leg3tibiaServoValue);
    Serial.print("Leg4: ");
    Serial.print(leg4hipServoValue);
    Serial.print(", ");
    Serial.print(leg4thighServoValue);
    Serial.print(", ");
    Serial.println(leg4tibiaServoValue);
    Serial.println();
}

