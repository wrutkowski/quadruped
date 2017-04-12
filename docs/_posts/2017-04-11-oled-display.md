---
layout: post
title:  "OLED Display and Emotions"
date:   2017-04-11 22:00:00
---
I received OLED display as a first parcel, so I decided to give it a go.
I tested couple of libraries and I managed to run u8g2 and SH1103_SPI. The memory footprint is very high for both as just with test program it sets global variable at 51%.

![Picture of the OLED Display](quadruped/assets/display-oled.jpg)

![Arduino memory stats for demo](quadruped/assets/display-arduino-sketch-memory-stats)

I was thinking how to display emotions: should it be eyes only, eyes and mouth, what about some artefacts like a bulb 💡 for an idea, heart ❤️ for love?
For all of those I don't need graphics library as I can export frames directly.

## Emotions

According to Paul Ekman there are six distinct emotions: anger 😠, disgust 😩, happiness 😄, sadness ☹️ and surprise 😮. I'm not sure how robot could go into disgust mode, so I'll skip it for now. For available face expressions I'm adding some idle states: idle 🙂 (no particular expression), looking around 😏, and sleeping 😴.

I created small app based on JS p5 (the same as for Inverse Kinematics). It has a 128x64 squares, which can be turned on or off by mouse press or dragging. There is also a save button that exports the whole frame into an array of 0 and 1 - representing each pixel on the display.

![p5 display frame app](quadruped/assets/display-p5-editor.png)

It didn't work ☹️ The amount of data produced by this method was too big to be handled by global variable. The program wouldn't compile. I thought that maybe providing only coordinates of pixels that are on and making it part of the program, not variable will help. And it did. Exported frame could be copy-pasted into Arduino IDE and run directly on the display - all working just fine.

But I thought that using this app will be time consuming - creating a few frames for each face expression without any tool like rectangle, circle drawing... That was 💡 the idea - why not using graphics program to design the face expression, export it to png and then make a script that will output on pixels for the display. I implemented in Swift as a script and it worked perfectly. To automate the process even more I made a bash script that feeds all png files from the folder to the script which creates functions with each face expression for Arduino code to use.

{% gist 2e505b739932a99f94aba0c7a25e3335 %}

Few improvements later it was working perfectly. I've added a segmentation: a check for R, G and B being under 50% and alpha component above 50% as the graphics program was using shades of grey and alpha channels for better representation of the drawings.

For designing the expressions I used Sketch as it's my almost-every-day work tool. Easy shape creation, vector graphics, automatic export 👌

![Designed face expressions](quadruped/assets/emotions.png)

Running everything together on the display

![GIF showing display with face expressions](quadruped/assets/display-working-face-expressions.gif)
