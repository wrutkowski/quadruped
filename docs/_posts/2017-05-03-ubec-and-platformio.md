---
layout: post
title:  "PlatformIO to the rescue + UBEC fixes shakes"
date:   2017-05-03 20:00:00
---
I was tired of using Arduino IDE for more than a couple lines of code. I was tweaking settings but I think it's just not for me and the perspective of typing robot source code moved me to my standard editor [Atom](https://atom.io) and I went looking for command line tool to compile and upload to Arduino. And I found: [PlatformIO](http://platformio.org), which is a plugin for Atom making it a great solution for my needs. Everything I like about Atom plus building, uploading and serial terminal. I played with it for a few hours rewriting Inverse Kinematics functions from [Simulator](https://wrutkowski.github.io/quadruped/2017/03/30/quadruped-simulator-lets-build-it.html).

![PlatformIO IDE for Atom](/quadruped/assets/ide-atom-platformio.png)

I cannot recommend it enough 😊 The only disadvantage I discovered was behavior of serial terminal which is sending everything that I type. I'd like to see, modify and confirm the command with _return_ before sending. It may be possible to do it, but I haven't figure it out yet.

UBEC arrived so I soldered it to battery plug XT60, power switch and screwed it to power ports on PWM Driver. I connected 3S LiPo battery and __servos started working!__ 🏆

![Quadruped with UBEC and 3S battery pack and working servos](/quadruped/assets/quadruped-working-servos.gif)

I combined existing _servo-commander_ code with Inverse Kinematics implementation and I've written a few tests to make sure that results are the same.

Here is the Inverse Kinematics function:
```
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
```

And helper function to translate angle to servo pulse width:
```
void translateAnglesToLegServoPulses(double alpha, double beta, double theta, int legNumber, int &hipPulse, int &thighPulse, int &tibiaPulse) {
    if (legNumber == 2 || legNumber == 4) {
        // mirrored legs
        alpha -= 90;
    }
    hipPulse = 390.0 + alpha * 2.62;
    thighPulse = 350.0 + beta * 2.33;
    tibiaPulse = 260.0 + theta * -2.33;
}
```
It still needs a tuning as every servo is a bit different and they are mirrored 1 to 2 and 3 to 4. So 90° on leg 1 hip is 0° for leg 2 and 4.
