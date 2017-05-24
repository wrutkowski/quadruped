#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define leg1hipServoNumber     0
#define leg1thighServoNumber   1
#define leg1tibiaServoNumber   2
#define leg2hipServoNumber     4
#define leg2thighServoNumber   5
#define leg2tibiaServoNumber   6
#define leg3hipServoNumber     8
#define leg3thighServoNumber   9
#define leg3tibiaServoNumber   10
#define leg4hipServoNumber     12
#define leg4thighServoNumber   13
#define leg4tibiaServoNumber   14

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// service pose
// 1,1,305,1,2,500,1,3,222,2,1,416,2,2,185,2,3,508,3,1,344,3,2,544,3,3,168,4,1,378,4,2,188,4,3,531
uint16_t leg1hipServoSetValue = 300;
uint16_t leg1thighServoSetValue = 500;
uint16_t leg1tibiaServoSetValue = 220;
uint16_t leg2hipServoSetValue = 425;
uint16_t leg2thighServoSetValue = 200;
uint16_t leg2tibiaServoSetValue = 500;
uint16_t leg3hipServoSetValue = 350;
uint16_t leg3thighServoSetValue = 550;
uint16_t leg3tibiaServoSetValue = 170;
uint16_t leg4hipServoSetValue = 380;
uint16_t leg4thighServoSetValue = 200;
uint16_t leg4tibiaServoSetValue = 530;

// stand pose
//uint16_t leg1hipServoSetValue = 550;
//uint16_t leg1thighServoSetValue = 250;
//uint16_t leg1tibiaServoSetValue = 300;
//uint16_t leg2hipServoSetValue = 225;
//uint16_t leg2thighServoSetValue = 450;
//uint16_t leg2tibiaServoSetValue = 450;
//uint16_t leg3hipServoSetValue = 550;
//uint16_t leg3thighServoSetValue = 300;
//uint16_t leg3tibiaServoSetValue = 300;
//uint16_t leg4hipServoSetValue = 225;
//uint16_t leg4thighServoSetValue = 500;
//uint16_t leg4tibiaServoSetValue = 450;

uint16_t leg1hipServoCurrentValue = leg1hipServoSetValue - 1;
uint16_t leg1thighServoCurrentValue = leg1thighServoSetValue - 1;
uint16_t leg1tibiaServoCurrentValue = leg1tibiaServoSetValue - 1;
uint16_t leg2hipServoCurrentValue = leg2hipServoSetValue - 1;
uint16_t leg2thighServoCurrentValue = leg2thighServoSetValue - 1;
uint16_t leg2tibiaServoCurrentValue = leg2tibiaServoSetValue - 1;
uint16_t leg3hipServoCurrentValue = leg3hipServoSetValue - 1;
uint16_t leg3thighServoCurrentValue = leg3thighServoSetValue - 1;
uint16_t leg3tibiaServoCurrentValue = leg3tibiaServoSetValue - 1;
uint16_t leg4hipServoCurrentValue = leg4hipServoSetValue - 1;
uint16_t leg4thighServoCurrentValue = leg4thighServoSetValue - 1;
uint16_t leg4tibiaServoCurrentValue = leg4tibiaServoSetValue - 1;

void setServo(uint8_t servoNumber, int oldValue, int newValue, bool force = false) {
    if (newValue == oldValue && !force) return;

    if (newValue > oldValue) {
        for (uint16_t pulselen = oldValue; pulselen <= newValue; pulselen++) {
            pwm.setPWM(servoNumber, 0, pulselen);
        }
    } else {
        for (uint16_t pulselen = oldValue; pulselen >= newValue; pulselen--) {
            pwm.setPWM(servoNumber, 0, pulselen);
        }
    }
}

void printServoStatus() {
    Serial.print("Leg1: ");
    Serial.print(leg1hipServoSetValue);
    Serial.print(", ");
    Serial.print(leg1thighServoSetValue);
    Serial.print(", ");
    Serial.println(leg1tibiaServoSetValue);
    Serial.print("Leg2: ");
    Serial.print(leg2hipServoSetValue);
    Serial.print(", ");
    Serial.print(leg2thighServoSetValue);
    Serial.print(", ");
    Serial.println(leg2tibiaServoSetValue);
    Serial.print("Leg3: ");
    Serial.print(leg3hipServoSetValue);
    Serial.print(", ");
    Serial.print(leg3thighServoSetValue);
    Serial.print(", ");
    Serial.println(leg3tibiaServoSetValue);
    Serial.print("Leg4: ");
    Serial.print(leg4hipServoSetValue);
    Serial.print(", ");
    Serial.print(leg4thighServoSetValue);
    Serial.print(", ");
    Serial.println(leg4tibiaServoSetValue);
    Serial.println();
}

void setServos(bool force = false) {
    setServo(leg1hipServoNumber, leg1hipServoCurrentValue, leg1hipServoSetValue, force);
    setServo(leg1thighServoNumber, leg1thighServoCurrentValue, leg1thighServoSetValue, force);
    setServo(leg1tibiaServoNumber, leg1tibiaServoCurrentValue, leg1tibiaServoSetValue, force);
    setServo(leg2hipServoNumber, leg2hipServoCurrentValue, leg2hipServoSetValue, force);
    setServo(leg2thighServoNumber, leg2thighServoCurrentValue, leg2thighServoSetValue, force);
    setServo(leg2tibiaServoNumber, leg2tibiaServoCurrentValue, leg2tibiaServoSetValue, force);
    setServo(leg3hipServoNumber, leg3hipServoCurrentValue, leg3hipServoSetValue, force);
    setServo(leg3thighServoNumber, leg3thighServoCurrentValue, leg3thighServoSetValue, force);
    setServo(leg3tibiaServoNumber, leg3tibiaServoCurrentValue, leg3tibiaServoSetValue, force);
    setServo(leg4hipServoNumber, leg4hipServoCurrentValue, leg4hipServoSetValue, force);
    setServo(leg4thighServoNumber, leg4thighServoCurrentValue, leg4thighServoSetValue, force);
    setServo(leg4tibiaServoNumber, leg4tibiaServoCurrentValue, leg4tibiaServoSetValue, force);

    leg1hipServoCurrentValue = leg1hipServoSetValue;
    leg1thighServoCurrentValue = leg1thighServoSetValue;
    leg1tibiaServoCurrentValue = leg1tibiaServoSetValue;
    leg2hipServoCurrentValue = leg2hipServoSetValue;
    leg2thighServoCurrentValue = leg2thighServoSetValue;
    leg2tibiaServoCurrentValue = leg2tibiaServoSetValue;
    leg3hipServoCurrentValue = leg3hipServoSetValue;
    leg3thighServoCurrentValue = leg3thighServoSetValue;
    leg3tibiaServoCurrentValue = leg3tibiaServoSetValue;
    leg4hipServoCurrentValue = leg4hipServoSetValue;
    leg4thighServoCurrentValue = leg4thighServoSetValue;
    leg4tibiaServoCurrentValue = leg4tibiaServoSetValue;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Accepted commands: LEG_NUMBER,JOINT_NUMBER,VALUE");
    Serial.println("   LEG_NUMBER   1...4");
    Serial.println("   JOINT_NUMBER 1 - hip, 2 - thigh, 3 - tibia");
    Serial.println("   VALUE 150...~650");
    Serial.println("   * Example: 1,1,500");
    Serial.println();

    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

    printServoStatus();
    setServos(/*force: */true);
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
                leg1hipServoSetValue = value;
            } else if (joint == 2) {
                leg1thighServoSetValue = value;
            } else if (joint == 3) {
                leg1tibiaServoSetValue = value;
            }
        } else if (leg == 2) {
            if (joint == 1) {
                leg2hipServoSetValue = value;
            } else if (joint == 2) {
                leg2thighServoSetValue = value;
            } else if (joint == 3) {
                leg2tibiaServoSetValue = value;
            }
        } else if (leg == 3) {
            if (joint == 1) {
                leg3hipServoSetValue = value;
            } else if (joint == 2) {
                leg3thighServoSetValue = value;
            } else if (joint == 3) {
                leg3tibiaServoSetValue = value;
            }
        } else if (leg == 4) {
            if (joint == 1) {
                leg4hipServoSetValue = value;
            } else if (joint == 2) {
                leg4thighServoSetValue = value;
            } else if (joint == 3) {
                leg4tibiaServoSetValue = value;
            }
        }

        printServoStatus();

        setServos();
    }
}
