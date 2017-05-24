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

#define DEBUG 0

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

#define Z_DOWN      -0.473
#define Z_NEUTRAL   -0.15
#define Z_UP        0.273
#define XY_NEUTRAL  0.55

bool standby = false;

const double hipLength = 57.0;
const double thighLength = 40.0;
const double tibiaLength = 47.0;
const double L = (hipLength + thighLength + tibiaLength) * 0.95;

struct LegRatio {
    double x;
    double y;
    double z;
    boolean set;

    LegRatio(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
        set = true;
    }
    LegRatio() {
        set = false;
    }
};

struct LegsRatio {
    LegRatio leg1;
    LegRatio leg2;
    LegRatio leg3;
    LegRatio leg4;

    LegsRatio(LegRatio leg1, LegRatio leg2, LegRatio leg3, LegRatio leg4) : leg1(leg1), leg2(leg2), leg3(leg3), leg4(leg4) {}
    LegsRatio(uint8_t legNumber, LegRatio leg) {
        if (legNumber == 1) {
            leg1 = leg;
        } else if (legNumber == 2) {
            leg2 = leg;
        } else if (legNumber == 3) {
            leg3 = leg;
        } else if (legNumber == 4) {
            leg4 = leg;
        }
    }
    LegsRatio() {}
};

struct Leg {
    int16_t x;
    int16_t y;
    int16_t z;

    Leg(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {}
    Leg() : Leg(0, 0, 0) {}
};

struct Legs {
    Leg leg1;
    Leg leg2;
    Leg leg3;
    Leg leg4;

    Legs(Leg leg1, Leg leg2, Leg leg3, Leg leg4) : leg1(leg1), leg2(leg2), leg3(leg3), leg4(leg4) {}
    Legs() {}
};

struct LegServoPulse {
    uint16_t hip;
    uint16_t thigh;
    uint16_t tibia;

    LegServoPulse(uint16_t hip, uint16_t thigh, uint16_t tibia) : hip(hip), thigh(thigh), tibia(tibia) {}
    LegServoPulse() : hip(0), thigh(0), tibia(0) {}
};

struct LegsServoPulse {
    LegServoPulse leg1;
    LegServoPulse leg2;
    LegServoPulse leg3;
    LegServoPulse leg4;

    LegsServoPulse(LegServoPulse leg1, LegServoPulse leg2, LegServoPulse leg3, LegServoPulse leg4) : leg1(leg1), leg2(leg2), leg3(leg3), leg4(leg4) {}
    LegsServoPulse() {}
};

struct LegAngles {
    double alpha;
    double beta;
    double theta;

    LegAngles() : alpha(NAN), beta(NAN), theta(NAN) {}
};

LegsRatio legsRatio;
Legs legs;
LegsServoPulse legsServoPulse;

enum Mode { None, Sit, Stand, WalkForward };
Mode mode = None;
uint8_t modeStepsCount = 0;
uint8_t modeStep = 0;
uint8_t currentTimeStep = 0;
unsigned long lastMillis;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setServo(uint8_t servoNumber, uint16_t value);
void printServoStatus();
void setServos();
Leg ratioToXYZ(LegRatio legRatio);
LegAngles computeXYZToAngles(Leg leg);
LegServoPulse translateAnglesToLegServoPulses(uint8_t legNumber, LegAngles legAngles);
void setMode(Mode mode);
void progressMode();
void updateServoValues();
void updateServoValues(uint8_t legNumber);

void setServo(uint8_t servoNumber, uint16_t value) {
    pwm.setPWM(servoNumber, 0, value);
    if (DEBUG) {
        Serial.print("pwm.setPWM(");
        Serial.print(servoNumber);
        Serial.print(", 0, ");
        Serial.print(value);
        Serial.println(")");
    }
}

void printServoStatus() {
    Serial.print("X Y Z - "); Serial.print("Leg1: "); Serial.print(legs.leg1.x); Serial.print(", "); Serial.print(legs.leg1.y); Serial.print(", "); Serial.println(legs.leg1.z); Serial.print("Leg2: "); Serial.print(legs.leg2.x); Serial.print(", "); Serial.print(legs.leg2.y); Serial.print(", "); Serial.println(legs.leg2.z); Serial.print("Leg3: "); Serial.print(legs.leg3.x); Serial.print(", "); Serial.print(legs.leg3.y); Serial.print(", "); Serial.println(legs.leg3.z); Serial.print("Leg4: "); Serial.print(legs.leg4.x); Serial.print(", "); Serial.print(legs.leg4.y); Serial.print(", "); Serial.println(legs.leg4.z); Serial.println();
    Serial.print("Servo - "); Serial.print("Leg1: "); Serial.print(legsServoPulse.leg1.hip); Serial.print(", "); Serial.print(legsServoPulse.leg1.thigh); Serial.print(", "); Serial.println(legsServoPulse.leg1.tibia); Serial.print("Leg2: "); Serial.print(legsServoPulse.leg2.hip); Serial.print(", "); Serial.print(legsServoPulse.leg2.thigh); Serial.print(", "); Serial.println(legsServoPulse.leg2.tibia); Serial.print("Leg3: "); Serial.print(legsServoPulse.leg3.hip); Serial.print(", "); Serial.print(legsServoPulse.leg3.thigh); Serial.print(", "); Serial.println(legsServoPulse.leg3.tibia); Serial.print("Leg4: "); Serial.print(legsServoPulse.leg4.hip); Serial.print(", "); Serial.print(legsServoPulse.leg4.thigh); Serial.print(", "); Serial.println(legsServoPulse.leg4.tibia); Serial.println();
}

void setServos() {
    updateServoValues();

    if (DEBUG) {
        Serial.print("LegsServoPulse.leg1: [");
        Serial.print(legsServoPulse.leg1.hip);
        Serial.print(", ");
        Serial.print(legsServoPulse.leg1.thigh);
        Serial.print(", ");
        Serial.print(legsServoPulse.leg1.tibia);
        Serial.println("]");
        Serial.print("LegsServoPulse.leg2: [");
        Serial.print(legsServoPulse.leg2.hip);
        Serial.print(", ");
        Serial.print(legsServoPulse.leg2.thigh);
        Serial.print(", ");
        Serial.print(legsServoPulse.leg2.tibia);
        Serial.println("]");
        Serial.print("LegsServoPulse.leg3: [");
        Serial.print(legsServoPulse.leg3.hip);
        Serial.print(", ");
        Serial.print(legsServoPulse.leg3.thigh);
        Serial.print(", ");
        Serial.print(legsServoPulse.leg3.tibia);
        Serial.println("]");
        Serial.print("LegsServoPulse.leg4: [");
        Serial.print(legsServoPulse.leg4.hip);
        Serial.print(", ");
        Serial.print(legsServoPulse.leg4.thigh);
        Serial.print(", ");
        Serial.print(legsServoPulse.leg4.tibia);
        Serial.println("]");
    }

    setServo(leg1hipServoNumber, legsServoPulse.leg1.hip);
    setServo(leg1thighServoNumber, legsServoPulse.leg1.thigh);
    setServo(leg1tibiaServoNumber, legsServoPulse.leg1.tibia);
    setServo(leg2hipServoNumber, legsServoPulse.leg2.hip);
    setServo(leg2thighServoNumber, legsServoPulse.leg2.thigh);
    setServo(leg2tibiaServoNumber, legsServoPulse.leg2.tibia);
    setServo(leg3hipServoNumber, legsServoPulse.leg3.hip);
    setServo(leg3thighServoNumber, legsServoPulse.leg3.thigh);
    setServo(leg3tibiaServoNumber, legsServoPulse.leg3.tibia);
    setServo(leg4hipServoNumber, legsServoPulse.leg4.hip);
    setServo(leg4thighServoNumber, legsServoPulse.leg4.thigh);
    setServo(leg4tibiaServoNumber, legsServoPulse.leg4.tibia);
}

void updateServoValues() {
    updateServoValues(1);
    yield();
    updateServoValues(2);
    yield();
    updateServoValues(3);
    yield();
    updateServoValues(4);
    yield();
}

void updateServoValues(uint8_t legNumber) {
    if (DEBUG) {
        Serial.print("updateServoValues(");
        Serial.print(legNumber);
        Serial.print(") leg");
    }

    if (legNumber == 1) {
        if (DEBUG) {
            Serial.print("1: [");
            Serial.print(legsRatio.leg1.x);
            Serial.print(", ");
            Serial.print(legsRatio.leg1.y);
            Serial.print(", ");
            Serial.print(legsRatio.leg1.z);
            Serial.println("]");
        }
        LegAngles leg1Angles = computeXYZToAngles(ratioToXYZ(legsRatio.leg1));
        legsServoPulse.leg1 = translateAnglesToLegServoPulses(1, leg1Angles);
    }
    if (legNumber == 2) {
        if (DEBUG) {
            Serial.print("2: [");
            Serial.print(legsRatio.leg2.x);
            Serial.print(", ");
            Serial.print(legsRatio.leg2.y);
            Serial.print(", ");
            Serial.print(legsRatio.leg2.z);
            Serial.println("]");
        }
        LegAngles leg2Angles = computeXYZToAngles(ratioToXYZ(legsRatio.leg2));
        legsServoPulse.leg2 = translateAnglesToLegServoPulses(2, leg2Angles);
    }
    if (legNumber == 3) {
        if (DEBUG) {
            Serial.print("3: [");
            Serial.print(legsRatio.leg3.x);
            Serial.print(", ");
            Serial.print(legsRatio.leg3.y);
            Serial.print(", ");
            Serial.print(legsRatio.leg3.z);
            Serial.println("]");
        }
        LegAngles leg3Angles = computeXYZToAngles(ratioToXYZ(legsRatio.leg3));
        legsServoPulse.leg3 = translateAnglesToLegServoPulses(3, leg3Angles);
    }
    if (legNumber == 4) {
        if (DEBUG) {
            Serial.print("4: [");
            Serial.print(legsRatio.leg4.x);
            Serial.print(", ");
            Serial.print(legsRatio.leg4.y);
            Serial.print(", ");
            Serial.print(legsRatio.leg4.z);
            Serial.println("]");
        }
        LegAngles leg4Angles = computeXYZToAngles(ratioToXYZ(legsRatio.leg4));
        legsServoPulse.leg4 = translateAnglesToLegServoPulses(4, leg4Angles);
    }
}

Leg ratioToXYZ(LegRatio legRatio) {
    return Leg(legRatio.x * L, legRatio.y * L, legRatio.z * L);
}

LegAngles computeXYZToAngles(Leg leg) {
    double x = leg.x;
    double y = leg.y;
    double z = -leg.z;

    LegAngles legAngles = LegAngles();
    legAngles.alpha = atan2(y, x);

    double L1 = sqrt(pow(x, 2) + pow(y, 2));
    double L = sqrt(pow(L1 - hipLength, 2) + pow(z, 2));
    legAngles.beta = acos(z / L) + acos((pow(thighLength, 2) + pow(L, 2) - pow(tibiaLength, 2)) / (2 * thighLength * L));
    legAngles.theta = acos((pow(tibiaLength, 2) + pow(thighLength, 2) - pow(L, 2)) / (2 * tibiaLength * thighLength));

    if (isnan(legAngles.alpha) || isnan(legAngles.beta) || isnan(legAngles.theta)) {
        legAngles.alpha = NAN;
        legAngles.beta = NAN;
        legAngles.theta = NAN;
    } else {
        legAngles.alpha = legAngles.alpha * RAD_TO_DEG;
        legAngles.beta = legAngles.beta * RAD_TO_DEG - 90;
        legAngles.theta = legAngles.theta * RAD_TO_DEG - 180;
    }

    if (DEBUG) {
        Serial.print("computeXYZToAngles(");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.print(", ");
        Serial.print(z);
        Serial.print(") -> ");
        Serial.print(legAngles.alpha);
        Serial.print(", ");
        Serial.print(legAngles.beta);
        Serial.print(", ");
        Serial.println(legAngles.theta);
    }

    return legAngles;
}

LegServoPulse translateAnglesToLegServoPulses(uint8_t legNumber, LegAngles legAngles) {
    LegServoPulse legServoPulse = LegServoPulse();

    if (legNumber == 1) {
        legServoPulse.hip = 430.0 + legAngles.alpha * -2.22;
        legServoPulse.thigh = 340.0 + legAngles.beta * 2.44;
        legServoPulse.tibia = 270.0 + legAngles.theta * -2.33;
    } else if (legNumber == 2) {
        legServoPulse.hip = 315.0 + legAngles.alpha * 2.22;
        legServoPulse.thigh = 405.0 + legAngles.beta * -2.28;
        legServoPulse.tibia = 460.0 + legAngles.theta * 2.39;
    } else if (legNumber == 3) {
        legServoPulse.hip = 370.0 + legAngles.alpha * -1.78;
        legServoPulse.thigh = 360.0 + legAngles.beta * 2.56;
        legServoPulse.tibia = 265.0 + legAngles.theta * -2.33;
    } else if (legNumber == 4) {
        legServoPulse.hip = 315.0 + legAngles.alpha * 2.33;
        legServoPulse.thigh = 405.0 + legAngles.beta * -2.28;
        legServoPulse.tibia = 455.0 + legAngles.theta * 2.44;
    }

    if (DEBUG) {
        Serial.print("translateAnglesToLegServoPulses(");
        Serial.print(legAngles.alpha); Serial.print(", ");
        Serial.print(legAngles.beta); Serial.print(", ");
        Serial.print(legAngles.theta); Serial.print(") -> ");
        Serial.print(legServoPulse.hip); Serial.print(", ");
        Serial.print(legServoPulse.thigh); Serial.print(", ");
        Serial.println(legServoPulse.tibia);
    }

    return legServoPulse;
}

void setMode(Mode newMode) {
    switch (newMode) {
        case None:
            modeStepsCount = 0;
            break;
        case Sit:
            modeStepsCount = 1;
            break;
        case Stand:
            modeStepsCount = 1;
            break;
        case WalkForward:
            modeStepsCount = 14;
            break;
    }
    mode = newMode;
    modeStep = 0;

    if (DEBUG) {
        Serial.print("Mode: ");
        switch (mode) {
            case None:          Serial.print("None"); break;
            case Sit:           Serial.print("Sit"); break;
            case Stand:         Serial.print("Stand"); break;
            case WalkForward:   Serial.print("WalkForward"); break;
        }
        Serial.println();
    }
}

void progressMode() {
    Serial.print("timer tick > "); Serial.println(millis());

    switch (mode) {
        case Sit:
            legsRatio = LegsRatio(
                LegRatio(XY_NEUTRAL, XY_NEUTRAL, Z_NEUTRAL),
                LegRatio(XY_NEUTRAL, XY_NEUTRAL, Z_NEUTRAL),
                LegRatio(XY_NEUTRAL, XY_NEUTRAL, Z_NEUTRAL),
                LegRatio(XY_NEUTRAL, XY_NEUTRAL, Z_NEUTRAL)
            );
            break;

        case Stand:
            legsRatio = LegsRatio(
                LegRatio(XY_NEUTRAL, XY_NEUTRAL, Z_DOWN),
                LegRatio(XY_NEUTRAL, XY_NEUTRAL, Z_DOWN),
                LegRatio(XY_NEUTRAL, XY_NEUTRAL, Z_DOWN),
                LegRatio(XY_NEUTRAL, XY_NEUTRAL, Z_DOWN)
            );
            break;

        case WalkForward:
            {
                uint8_t stepTime = 0;
                const double movementMaxFront = 0.425;
                const double movementMaxBack = 0.6;
                const double movementStep = 0.30;

                if (modeStep == 0) {
                    // stepTime = 4;
                    legsRatio = LegsRatio(
                        LegRatio(XY_NEUTRAL, movementMaxFront - 2 * movementStep, Z_DOWN),
                        LegRatio(XY_NEUTRAL, movementMaxFront - 1 * movementStep, Z_DOWN),
                        LegRatio(XY_NEUTRAL, movementMaxBack - 1 * movementStep, Z_DOWN),
                        LegRatio(XY_NEUTRAL, movementMaxBack - 0 * movementStep, Z_DOWN)
                    );

                } else if (modeStep == 1) {
                    legsRatio.leg4 = LegRatio(XY_NEUTRAL, movementMaxBack - 0 * movementStep, Z_NEUTRAL); // up
                } else if (modeStep == 2) {
                    legsRatio.leg4 = LegRatio(XY_NEUTRAL, movementMaxBack - 2 * movementStep, Z_NEUTRAL); // move forward
                } else if (modeStep == 3) {
                    legsRatio.leg4 = LegRatio(XY_NEUTRAL, movementMaxBack - 2 * movementStep, Z_DOWN); // down

                } else if (modeStep == 4) {
                    legsRatio.leg1 = LegRatio(XY_NEUTRAL, movementMaxFront - 2 * movementStep, Z_NEUTRAL); // up
                } else if (modeStep == 5) {
                    legsRatio.leg1 = LegRatio(XY_NEUTRAL, movementMaxFront - 0 * movementStep, Z_NEUTRAL); // move forward
                } else if (modeStep == 6) {
                    legsRatio.leg1 = LegRatio(XY_NEUTRAL, movementMaxFront - 0 * movementStep, Z_DOWN); // down

                } else if (modeStep == 7) {
                    legsRatio = LegsRatio(
                        LegRatio(XY_NEUTRAL, movementMaxFront - 1 * movementStep, Z_DOWN),
                        LegRatio(XY_NEUTRAL, movementMaxFront - 2 * movementStep, Z_DOWN),
                        LegRatio(XY_NEUTRAL, movementMaxBack - 0 * movementStep, Z_DOWN),
                        LegRatio(XY_NEUTRAL, movementMaxBack - 1 * movementStep, Z_DOWN)
                    );

                } else if (modeStep == 8) {
                    legsRatio.leg3 = LegRatio(XY_NEUTRAL, movementMaxBack - 0 * movementStep, Z_NEUTRAL); // up
                } else if (modeStep == 9) {
                    legsRatio.leg3 = LegRatio(XY_NEUTRAL, movementMaxBack - 2 * movementStep, Z_NEUTRAL); // move forward
                } else if (modeStep == 10) {
                    legsRatio.leg3 = LegRatio(XY_NEUTRAL, movementMaxBack - 2 * movementStep, Z_DOWN); // down

                } else if (modeStep == 11) {
                    legsRatio.leg2 = LegRatio(XY_NEUTRAL, movementMaxFront - 2 * movementStep, Z_NEUTRAL); // up
                } else if (modeStep == 12) {
                    legsRatio.leg2 = LegRatio(XY_NEUTRAL, movementMaxFront - 0 * movementStep, Z_NEUTRAL); // move forward
                } else if (modeStep == 13) {
                    legsRatio.leg2 = LegRatio(XY_NEUTRAL, movementMaxFront - 0 * movementStep, Z_DOWN); // down
                }

                if (DEBUG) {
                    Serial.print("modeStep: "); Serial.print(modeStep); Serial.print("/"); Serial.println(modeStepsCount);
                    Serial.print("currentTimeStep: "); Serial.print(currentTimeStep); Serial.print("/"); Serial.println(stepTime);
                }
                if (currentTimeStep >= stepTime) {
                    currentTimeStep = 0;
                    modeStep++;
                    if (modeStep >= modeStepsCount) {
                        modeStep = 0;
                    }
                } else {
                    currentTimeStep++;
                }
            }
            break;
    }
    yield();
    setServos();
}

void setup() {
    Serial.begin(115200);
    Serial.println("Accepted commands:");
    Serial.println(" - 1 - Sit");
    Serial.println(" - 2 - Stand");
    Serial.println(" - 3 - WalkForward");
    Serial.println();

    // Initialize Variables
    legsRatio = LegsRatio(LegRatio(), LegRatio(), LegRatio(), LegRatio());
    legs = Legs(Leg(), Leg(), Leg(), Leg());
    legsServoPulse = LegsServoPulse(LegServoPulse(), LegServoPulse(), LegServoPulse(), LegServoPulse());

    // Initialize PWM Driver
    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz update

    // Set Default Position
    setMode(Sit);
    progressMode();

    // Start "timer"
    lastMillis = millis();
}

void loop() {
    if (Serial.available()) {
        String commandString = Serial.readStringUntil(',');
        uint8_t command = commandString.toInt();
        Serial.print("> "); Serial.println(command); Serial.println("OK");

        if (command == 0) {
            standby = !standby;
            Serial.print("Standby mode: ");
            if (standby) {
                Serial.println("on");
            } else {
                Serial.println("off");
            }
        } else {
            if (command == 1) {
                if (mode != Sit) setMode(Sit);

            } else if (command == 2) {
                if (mode != Stand) setMode(Stand);

            } else if (command == 3) {
                if (mode != WalkForward) setMode(WalkForward);
                String subcommandString = Serial.readStringUntil(',');
                if (subcommandString.length() > 0) {
                    uint8_t subcommand = subcommandString.toInt();
                    modeStep = subcommand;
                }
            }
            Serial.print("Mode: ");
            switch (mode) {
                case None:          Serial.print("None"); break;
                case Sit:           Serial.print("Sit"); break;
                case Stand:         Serial.print("Stand"); break;
                case WalkForward:   Serial.print("WalkForward"); break;
            }
            Serial.println();
        }

        Serial.print("Benchmark "); Serial.print(millis()); Serial.print(" > ");
        progressMode();
        Serial.println(millis());
    }

    if (!standby && millis() - lastMillis >= 400) {
        lastMillis = millis();
        progressMode();
    }
}
