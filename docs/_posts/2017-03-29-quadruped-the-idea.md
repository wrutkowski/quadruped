---
layout: post
title:  "Quadruped - The Idea"
date:   2017-03-29 12:00:00
---
I have build couple of robots in the past but I always wanted to build a walking robot. I decided that it'd be nice to have small pet-like robot that can sit and walk on the desk, has some human like emotions and express them using body language and face.

By body language I mean waving with one of the legs, shaking, dancing. For a face I decided to use small **colour OLED display** on which I'll be showing different faces to mimic joy, anger, curiosity etc. For interactions (possibly simple games) I'll use **RFID** so robot will be able to recognise object by their rfid markers. On top of that I'm thinking about **ultrasound sensor** so it can detect objects and avoid collisions. I'm still thinking how to stop it from falling off from desk - some connectors in legs should be fine.

I was thinking about material and as I don't have 3D printer nor workshop I decided to use something simple. For now I'm experimenting with *balsa wood* 5mm and 6.8mm thick. When servo motors will arrive I'll test rigidity of balsa wood and in case it won't work I have plastic sheet as a backup plan.

I ordered parts from China so it'll take a while for them to arrive. In the meantime I cut one leg and a body from balsa having only one servo motor for sizing and testing. I can start writing some code though! I'm going to make robot simulator to work out inverse kinematics and easily test movements of the robot: walking, turning and so on.