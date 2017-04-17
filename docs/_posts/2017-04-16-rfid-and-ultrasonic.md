---
layout: post
title:  "RFID and Ultrasonic Display Control"
date:   2017-04-16 14:00:00
---
RFID arrived before the weekend! I checked documentation and some tutorials and after connecting a few wires it was all running fine. Included card and keyfob were recognised correctly. I edited the code to display message depending on card id:

```
byte keyringRFID[4] = {0xF5, 0x68, 0xCD, 0x65};
byte cardRFID[4] = {0x68, 0x4D, 0xEC, 0x10};
if (checkRFID(readCard, keyringRFID)) {
    Serial.println("keyring detected");
} else if (checkRFID(readCard, cardRFID)) {
    Serial.println("card detected");
}
```

I connected both RFID and OLED sketches so when one of the rfid enabled items is detected Arduino will display different emotion.

![RFID detecting card and keyfob](/quadruped/assets/rfid-oled.gif)

For an experiment I used ultrasonic sensor to make the robot happy when detecting an obstacle close-by (me approaching 😄). It's enough to import the library and then call `ultrasonic.distanceRead()` to get reading in centimeters. When below a set threshold I'm going into the loop checking for another bigger threshold to break the loop and go into idle mode again. The current setup is that the _looking around_ face expression in shown in the loop and for rfid card I'm showing **sad** emotion and for keyfob **angry** face.

![RFID, Ultrasonic sensor controlling emotions on an OLED Display](/quadruped/assets/rfid-ultrasonic-oled.gif)

Code from this post and previous posts is available  [here](https://github.com/wrutkowski/quadruped/blob/c01af33010721998d79208610dbcb35690a00a36/arduino/rfid-ultrasonic-controlled-oled/rfid-ultrasonic-controlled-oled.ino).
