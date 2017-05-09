/*
 *  Wire Connections
 *
 *  ============================
 *  PWM Driver
 *  ----------------------------
 *  Device Port     Arduino Port
 *  ----------------------------
 *  SCL            A5
 *  SDA            A4
 *
 *  ============================
 *  OLED Display
 *  ----------------------------
 *  Device Port     Arduino Port
 *  ----------------------------
 *  GND
 *  VDD
 *  SCK             D13
 *  SDA             D11
 *  RES             D9
 *  DC              D8
 *  CS              D10
 *
 *  ============================
 *  RFID
 *  ----------------------------
 *  Device Port     Arduino Port
 *  ----------------------------
 *  VDD
 *  RST             D7
 *  GND
 *  x
 *  MISO            D12
 *  MOSI            D11
 *  SCK             D13
 *  SDA             D6
 *
 *  ============================
 *  Ultrasonic
 *  ----------------------------
 *  Device Port     Arduino Port
 *  ----------------------------
 *  Trig            D3
 *  Echo            D2
 */

#include <Arduino.h>
#include <math.h>
#include <Wire.h>

#include <Adafruit_PWMServoDriver.h>

#define leg1hipServoNumber     5
#define leg1thighServoNumber   4
#define leg1tibiaServoNumber   3
#define leg2hipServoNumber     6
#define leg2thighServoNumber   7
#define leg2tibiaServoNumber   8
#define leg3hipServoNumber     11
#define leg3thighServoNumber   10
#define leg3tibiaServoNumber   9
#define leg4hipServoNumber     0
#define leg4thighServoNumber   1
#define leg4tibiaServoNumber   2

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// service pose
//uint16_t leg1hipServoSetValue = 550;
//uint16_t leg1thighServoSetValue = 550;
//uint16_t leg1tibiaServoSetValue = 450;
//uint16_t leg2hipServoSetValue = 225;
//uint16_t leg2thighServoSetValue = 200;
//uint16_t leg2tibiaServoSetValue = 300;
//uint16_t leg3hipServoSetValue = 550;
//uint16_t leg3thighServoSetValue = 550;
//uint16_t leg3tibiaServoSetValue = 450;
//uint16_t leg4hipServoSetValue = 225;
//uint16_t leg4thighServoSetValue = 250;
//uint16_t leg4tibiaServoSetValue = 300;

// stand pose
// uint16_t leg1hipServoSetValue = 550;
// uint16_t leg1thighServoSetValue = 250;
// uint16_t leg1tibiaServoSetValue = 300;
// uint16_t leg2hipServoSetValue = 225;
// uint16_t leg2thighServoSetValue = 450;
// uint16_t leg2tibiaServoSetValue = 450;
// uint16_t leg3hipServoSetValue = 550;
// uint16_t leg3thighServoSetValue = 300;
// uint16_t leg3tibiaServoSetValue = 300;
// uint16_t leg4hipServoSetValue = 225;
// uint16_t leg4thighServoSetValue = 500;
// uint16_t leg4tibiaServoSetValue = 450;

// service pose
uint16_t leg1hipServoSetValue = 550;
uint16_t leg1thighServoSetValue = 500;
uint16_t leg1tibiaServoSetValue = 300;
uint16_t leg2hipServoSetValue = 225;
uint16_t leg2thighServoSetValue = 150;
uint16_t leg2tibiaServoSetValue = 450;
uint16_t leg3hipServoSetValue = 550;
uint16_t leg3thighServoSetValue = 540;
uint16_t leg3tibiaServoSetValue = 300;
uint16_t leg4hipServoSetValue = 225;
uint16_t leg4thighServoSetValue = 200;
uint16_t leg4tibiaServoSetValue = 450;

uint16_t leg1hipServoCurrentValue = leg1hipServoSetValue;
uint16_t leg1thighServoCurrentValue = leg1thighServoSetValue;
uint16_t leg1tibiaServoCurrentValue = leg1tibiaServoSetValue;
uint16_t leg2hipServoCurrentValue = leg2hipServoSetValue;
uint16_t leg2thighServoCurrentValue = leg2thighServoSetValue;
uint16_t leg2tibiaServoCurrentValue = leg2tibiaServoSetValue;
uint16_t leg3hipServoCurrentValue = leg3hipServoSetValue;
uint16_t leg3thighServoCurrentValue = leg3thighServoSetValue;
uint16_t leg3tibiaServoCurrentValue = leg3tibiaServoSetValue;
uint16_t leg4hipServoCurrentValue = leg4hipServoSetValue;
uint16_t leg4thighServoCurrentValue = leg4thighServoSetValue;
uint16_t leg4tibiaServoCurrentValue = leg4tibiaServoSetValue;

const double hipLength = 52.0;
const double thighLength = 44.0;
const double tibiaLength = 97.0;

const double L = (hipLength + thighLength + tibiaLength) * 0.95; // max leg length

void setServo(uint8_t servoNumber, uint16_t oldValue, uint16_t newValue, bool force = false);
void printServoStatus();
void setServos(bool force = false);
void computeXYZToAngles(uint16_t x, uint16_t y, uint16_t z, uint16_t &alpha, uint16_t &beta, uint16_t &theta);
void translateAnglesToLegServoPulses(double alpha, double beta, double theta, int legNumber, int &hipPulse, int &thighPulse, int &tibiaPulse);

void setServo(uint8_t servoNumber, uint16_t oldValue, uint16_t newValue, bool force = false) {
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

void computeXYZToAngles(double x, double y, double z, double &alpha, double &beta, double &theta) {
    z = -z;

    alpha = atan2(y, x);

    double L1 = sqrt(pow(x, 2) + pow(y, 2));
    double L = sqrt(pow(L1 - hipLength, 2) + pow(z, 2));
    beta = acos(z / L) + acos((pow(thighLength, 2) + pow(L, 2) - pow(tibiaLength, 2)) / (2 * thighLength * L));
    theta = acos((pow(tibiaLength, 2) + pow(thighLength, 2) - pow(L, 2)) / (2 * tibiaLength * thighLength));

   if (isnan(alpha) || isnan(beta) || isnan(theta)) {
       alpha = NAN;
       beta = NAN;
       theta = NAN;
   } else {
        alpha = alpha * RAD_TO_DEG;
        beta = beta * RAD_TO_DEG - 90;
        theta = theta * RAD_TO_DEG - 180;
   }
}

void translateAnglesToLegServoPulses(double alpha, double beta, double theta, int legNumber, int &hipPulse, int &thighPulse, int &tibiaPulse) {
    if (legNumber == 2 || legNumber == 4) {
        // mirrored legs
        alpha -= 90;
    }
    hipPulse = 390.0 + alpha * 2.62;
    thighPulse = 350.0 + beta * 2.33;
    tibiaPulse = 260.0 + theta * -2.33;
    
}

// void test_valid() {
//     double x = 100;
//     double y = 100;
//     double z = -50;
//     double alpha, beta, theta;
//     computeXYZToAngles(x, y, z, alpha, beta, theta);
//     Serial.print("Test computeXYZToAngles(");
//     Serial.print(x);
//     Serial.print(", ");
//     Serial.print(y);
//     Serial.print(", ");
//     Serial.print(z);
//     Serial.print(") -> ");
//     Serial.print(alpha); // 45
//     Serial.print(", ");
//     Serial.print(beta); // 40
//     Serial.print(", ");
//     Serial.println(theta); // -100
// }

// void test_NaN() {
//     double x = 100;
//     double y = 100;
//     double z = 140;
//     double alpha, beta, theta;
//     computeXYZToAngles(x, y, z, alpha, beta, theta);
//     Serial.print("Test computeXYZToAngles(");
//     Serial.print(x);
//     Serial.print(", ");
//     Serial.print(y);
//     Serial.print(", ");
//     Serial.print(z);
//     Serial.print(") -> ");
//     Serial.print(alpha); // NAN
//     Serial.print(", ");
//     Serial.print(beta); // NAN
//     Serial.print(", ");
//     Serial.println(theta); // NAN
// }

void setup() {
    Serial.begin(115200);
    Serial.println("Accepted commands: LEG_NUMBER,JOINT_NUMBER,VALUE");
    Serial.println("   LEG_NUMBER   1...4");
    Serial.println("   JOINT_NUMBER 1 - hip, 2 - thigh, 3 - tibia");
    Serial.println("   VALUE 0...180");
    Serial.println("   * Example: 1,1,90");
    Serial.println();


    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

    printServoStatus();
    setServos(/*force: */true);

    double x = 190;
    double y = 0;
    double z = 0;
    double alpha, beta, theta;
    computeXYZToAngles(x, y, z, alpha, beta, theta);
    Serial.print("computeXYZToAngles(");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.print(z);
    Serial.print(") -> ");
    Serial.print(alpha); // 45
    Serial.print(", ");
    Serial.print(beta); // 40
    Serial.print(", ");
    Serial.println(theta); // -100

    int hipPulse, thighPulse, tibiaPulse;
    translateAnglesToLegServoPulses(alpha, beta, theta, 1, hipPulse, thighPulse, tibiaPulse);
    Serial.print("translateAnglesToLegServoPulses -> ");
    Serial.print(hipPulse); // 45
    Serial.print(", ");
    Serial.print(thighPulse); // 40
    Serial.print(", ");
    Serial.println(tibiaPulse); // -100
}

void loop() {
    if (Serial.available()) {
        while (Serial.available()) {
            String legString = Serial.readStringUntil(',');
            int leg = legString.toInt();
            String jointString = Serial.readStringUntil(',');
            int joint = jointString.toInt();
            String valueString = Serial.readStringUntil(',');
            int value = valueString.toInt();

            Serial.print("> ");
            Serial.print(legString);
            Serial.print(",");
            Serial.print(jointString);
            Serial.print(",");
            Serial.println(valueString);
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
            } else if (leg == 11) {
                double x = joint;
                double y = value;
                String zString = Serial.readStringUntil(',');
                double z = zString.toInt();

                double alpha, beta, theta;
                computeXYZToAngles(x, y, z, alpha, beta, theta);
                Serial.print("computeXYZToAngles(");
                Serial.print(x);
                Serial.print(", ");
                Serial.print(y);
                Serial.print(", ");
                Serial.print(z);
                Serial.print(") -> ");
                Serial.print(alpha);
                Serial.print(", ");
                Serial.print(beta);
                Serial.print(", ");
                Serial.println(theta);

                int hipPulse, thighPulse, tibiaPulse;
                translateAnglesToLegServoPulses(alpha, beta, theta, 1, hipPulse, thighPulse, tibiaPulse);
                Serial.print("translateAnglesToLegServoPulses -> ");
                Serial.print(hipPulse);
                Serial.print(", ");
                Serial.print(thighPulse);
                Serial.print(", ");
                Serial.println(tibiaPulse);

                leg1hipServoSetValue = hipPulse;
                leg1thighServoSetValue = thighPulse;
                leg1tibiaServoSetValue = tibiaPulse;
            } else if (leg == 12) {
                double x = joint;
                double y = value;
                String zString = Serial.readStringUntil(',');
                double z = zString.toInt();

                double alpha, beta, theta;
                computeXYZToAngles(x, y, z, alpha, beta, theta);
                Serial.print("computeXYZToAngles(");
                Serial.print(x);
                Serial.print(", ");
                Serial.print(y);
                Serial.print(", ");
                Serial.print(z);
                Serial.print(") -> ");
                Serial.print(alpha);
                Serial.print(", ");
                Serial.print(beta);
                Serial.print(", ");
                Serial.println(theta);

                int hipPulse, thighPulse, tibiaPulse;
                translateAnglesToLegServoPulses(alpha, beta, theta, 2, hipPulse, thighPulse, tibiaPulse);
                Serial.print("translateAnglesToLegServoPulses -> ");
                Serial.print(hipPulse);
                Serial.print(", ");
                Serial.print(thighPulse);
                Serial.print(", ");
                Serial.println(tibiaPulse);

                leg2hipServoSetValue = hipPulse;
                leg2thighServoSetValue = thighPulse;
                leg2tibiaServoSetValue = tibiaPulse;
            } else if (leg == 13) {
                double x = joint;
                double y = value;
                String zString = Serial.readStringUntil(',');
                double z = zString.toInt();

                double alpha, beta, theta;
                computeXYZToAngles(x, y, z, alpha, beta, theta);
                Serial.print("computeXYZToAngles(");
                Serial.print(x);
                Serial.print(", ");
                Serial.print(y);
                Serial.print(", ");
                Serial.print(z);
                Serial.print(") -> ");
                Serial.print(alpha);
                Serial.print(", ");
                Serial.print(beta);
                Serial.print(", ");
                Serial.println(theta);

                int hipPulse, thighPulse, tibiaPulse;
                translateAnglesToLegServoPulses(alpha, beta, theta, 3, hipPulse, thighPulse, tibiaPulse);
                Serial.print("translateAnglesToLegServoPulses -> ");
                Serial.print(hipPulse);
                Serial.print(", ");
                Serial.print(thighPulse);
                Serial.print(", ");
                Serial.println(tibiaPulse);

                leg3hipServoSetValue = hipPulse;
                leg3thighServoSetValue = thighPulse;
                leg3tibiaServoSetValue = tibiaPulse;
            } else if (leg == 14) {
                double x = joint;
                double y = value;
                String zString = Serial.readStringUntil(',');
                double z = zString.toInt();

                double alpha, beta, theta;
                computeXYZToAngles(x, y, z, alpha, beta, theta);
                Serial.print("computeXYZToAngles(");
                Serial.print(x);
                Serial.print(", ");
                Serial.print(y);
                Serial.print(", ");
                Serial.print(z);
                Serial.print(") -> ");
                Serial.print(alpha);
                Serial.print(", ");
                Serial.print(beta);
                Serial.print(", ");
                Serial.println(theta);

                int hipPulse, thighPulse, tibiaPulse;
                translateAnglesToLegServoPulses(alpha, beta, theta, 4, hipPulse, thighPulse, tibiaPulse);
                Serial.print("translateAnglesToLegServoPulses -> ");
                Serial.print(hipPulse);
                Serial.print(", ");
                Serial.print(thighPulse);
                Serial.print(", ");
                Serial.println(tibiaPulse);

                leg4hipServoSetValue = hipPulse;
                leg4thighServoSetValue = thighPulse;
                leg4tibiaServoSetValue = tibiaPulse;
            }
        }

        printServoStatus();

        setServos();
    }
}
