---
layout: post
title:  "Servos connected - robot with a \"hairstyle\" and shakes..."
date:   2017-04-20 22:00:00
---
I just couldn't wait for PWM Driver for servos so I decided to build my own on a breadboard. I hook up external power source and loads of jumpers to make a proof of concept that I can start working on legs controlling from Arduino.

When I show this picture to my friend he said that the robot has a stylish hairstyle 😂 Judge for yourself:

![Robot with a "hairstyle"](/quadruped/assets/robot-with-a-hairstyle.jpg)

I created an Arduino sketch allowing me to send commands in format `LEG_NUMBER,JOINT_NUMBER,VALUE` to the Arduino using serial communication to change servos' degrees.

_servo-commander.ino_
```
Serial.println("Accepted commands: LEG_NUMBER,JOINT_NUMBER,VALUE");
Serial.println("   LEG_NUMBER   1...4");
Serial.println("   JOINT_NUMBER 1 - hip, 2 - thigh, 3 - tibia");
Serial.println("   VALUE 0...180");
Serial.println("   * Example: 1,1,90");
Serial.println();
```

I used `Serial.readStringUntil()` to read a string that is before `,` and convert it to `int`. When the new value was set program prints leg values to confirm changed setting.

There was a problem though... 🤒

![Quadruped shakes](/quadruped/assets/quadruped-shakes-servos.gif)

When I command servo to adjust the degree the whole robot started shaking, I had no suspect at that point.
