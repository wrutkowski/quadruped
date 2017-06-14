---
layout: post
title:  "Walking movement: calibration of legs and first tests"
date:   2017-05-16 20:00:00
---
I draw points on a sheet of paper to mark 10x10 cm square with dots every centimeter. I put the robot on a 5cm book and equipped myself with a ruler - that way I'm able to measure X, Y and Z points and calibrate servos.

![Calibration Sheet](/quadruped/assets/calibration-sheet.jpg)

To make things easier I downloaded mac repository for serial communication [ORSSerialPort](https://github.com/armadsen/ORSSerialPort) and I modified example app for my needs: I added sliders for each servo for each leg and a few buttons with preset values: sit, stand, service.

![Mac Control App](/quadruped/assets/mac-serial-app.png)

The process I end up with is to create a table witch each leg as a column and two settings for each servo as rows: hip 0° and 90°, thigh 90° and 0° and tibia -90° and 0° using the created mac app I was able to set the servos to required positions and writing down pulse width.

![Calibration Table](/quadruped/assets/calibration-table.jpg)

I had the starting points and per degree pulse width change for each of the servos so implementing small translate function was quite easy:

```
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
```

As a test I set three points on calibration sheet and then I tested each leg commanding the end position to be in exact same places as marked point - all working good. One problem I noticed was that calculations were correct when robot body was resting on a book. Under its own weight Z axis changed almost +5cm, that means when robot thought it's 5cm above ground (standing position) in fact it was laying on the ground still.

# Walking

Firstly I wanted to implement movements as an array of preset steps which then is iterated over to set servo values. It compiled nicely and stand and sit positions were working perfectly. When it came to walking only first walk position worked and then Arduino reset. I spend a long time on debugging the problem finally with a conclusion that I run out of the RAM and watchdog reseted Arduino.

I changed the implementation to set each step directly in code mimicking simulator code.

**Robot started moving legs!**

![Robot walking upside down](/quadruped/assets/robot-walking-upside-down.gif)

The problem I mentioned earlier is even more obvious when robot tries to actually walk:

![Robot failing to walk](/quadruped/assets/robot-failing-to-walk.gif)

It looks like servos are too weak for the total weight of the robot and / or legs's parts are too long and momentum is too big.
