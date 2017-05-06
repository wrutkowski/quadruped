---
layout: post
title:  "Upper Body building"
date:   2017-04-27 20:00:00
---
While I wait for UBEC which will provide enough power for the servos I started working on enclosing for the robot. The requirement for it is to hold the display, ultrasonic sonar and RFID scanner. At the beginning I was thinking of creating a scaffolding to mount mentioned devices to but finally I designed a cuboid with one edge cut on 45°.

The curved wall was meant to hold the display so it's easily readable when looking at the robot from the front.

After measuring the parts I started cutting balsa wood.

Piece with holes for ultrasonic sonar, display mounted on another piece.
![Piece with holes for ultrasonic sonar, display mounted on another piece](/quadruped/assets/wood-pieces-for-upper-body.jpg)

RFID piece glued and the display from the other side. I checked whether RFID will work through 6mm balsa wood, but it is 👌
![RFID piece glued and display from the other side](/quadruped/assets/wood-pieces-rfid-display-for-upper-body.jpg)

Everything glued together looks like this:
![Upper body](/quadruped/assets/upper-body-from-the-outside.jpg)

Inside:
![Upper body from the inside](/quadruped/assets/upper-body-from-the-inside.jpg)

Time to connect all wires. I stuck small breadboard with Arduino Nano on it next to PWM Driver, then I connected all devices using jumper wires and uploaded _emotions_ program to verify all works as expected.
![Wiring: Arduino, PWM Driver, RFID, Display and Ultrasonic Sonar](/quadruped/assets/wiring-arduino-pwm-driver-rfid-display-ultrasonic-sonar.jpg)

I cut a hole for a power switch on the side and small holes for servo cables to go through. I mounted the upper body on the robot by glueing small fragments of one-by-one with holes for M3 screw.

It's tight inside, but there is enough room for UBEC and probably another Arduino if one is not enough to handle all tasks.
![Robot inside wiring with all external devices](/quadruped/assets/quadruped-inside-wiring-and-devices-placement.jpg)

And with uploaded emotions program:
![Quadruped with emotions](/quadruped/assets/quadruped-with-emotions.gif)

I'm still waiting for UBEC to arrive. When it will the robot should stand for the first time 👏
