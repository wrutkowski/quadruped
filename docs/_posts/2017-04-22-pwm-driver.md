---
layout: post
title:  "PWM Driver - a cure for shakes?"
date:   2017-04-22 14:00:00
---
I've received PWM Driver. I hoped that it will resolve shakes issue. I wanted to try it as soon as possible. Pins were not soldered so that was I first step.

![PRM Driver soldering](/quadruped/assets/pwm-driver-soldering.jpg)

I placed 4 standoffs for PWM Driver on the robot body and then I connected all servos. I had to adjust servo-commander script as [PWM Driver library from Adafruit](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library) accept _pulse width_ instead of angles as _Servo_ library.

I added for loop to change pulse width gradually hoping it'll help for the shakes as well.

```
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
```

Standoffs ready for the Driver
![Standoffs ready for the Driver](/quadruped/assets/pwm-driver-standoffs.jpg)

PWM Driver with servos connected
![PWM Driver with servos connected](/quadruped/assets/pwm-driver-with-servos-connected.jpg)

You can see PWM driver didn't help - shaking is still there.

![PWM Driver - robot shakes](/quadruped/assets/pwm-driver-shakes.gif)

In the documentation I found the point about [soldering capacitor to the driver](https://learn.adafruit.com/16-channel-pwm-servo-driver?view=all#adding-a-capacitor-to-the-thru-hole-capacitor-slot). For 12 servos it should be 1200uF capacitor. I didn't have one, but I'll grab one from the nearby electronic store tomorrow.
