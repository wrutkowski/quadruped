---
layout: post
title:  "Quadruped Simulator - Let's Build It: Inverse Kinematics and Movement"
date:   2017-03-30 20:00:00
---

Inverse Kinematics will be implemented to make controlling the robot easier. Instead of providing each servo state in degrees the program will ask the end of the leg to be in X, Y, Z coordinate. The angles will be computed.

To make sure the calculations are right and then work on movement sets I designed a robot simulator. The options include:

- 3D environment with possibility to move the camera around the robot
- Adjusting dimensions of the robot and leg parts
- Setting X, Y and Z for each leg
- Creating movements (or animations) as a looped sequence to work out walking, turning, etc.

# Tools ⚒

Firstly I wanted to build the simulator based on Swift Playground, but the refresh ratio for 3D (SceneKit) was unusable for a more advanced project.
I watched couple of [The Coding Train videos by Daniel Shiffman](https://www.youtube.com/user/shiffman) and those project where based on [p5 JavaScript library](https://p5js.org). I looked through the examples and doc reference and after 10 minutes I had 3D world with a box representing robot body displayed. I must admit that this tool is great for prototyping and it's kind of addictive 😄

# The Simulator 🚀

I implemented first leg with a hip, a thigh and a tibia making sure that alpha, beta and theta variables are easily accessible so I can change them later. First iteration was just to display the robot: body and four legs, control the 3D camera and change angles in legs' joints.

The next step involved coding Inverse Kinematics. I based the calculation on the [3DOF Inverse Kinematics For Arm/Leg of Robot Using Arduino](http://te.unib.ac.id/lecturer/indraagustian/2014/05/3dof-inverse-kinematic-for-armleg-of-robot-use-arduino/) by Indra Agustian. It's funny that I found the Indra's image on StackOverflow and I coded my solution based on that to later by searching by image realising that the calculations were ready and waiting on his website 🤔

![3DOF Inverse Kinematics For Arm/Leg of Robot Using Arduino](http://i2.wp.com/te.unib.ac.id/lecturer/indraagustian/wp-content/uploads/2014/05/Inverse-Kinematics.jpg)

The animation I added first was just **standing** - it's more a pose than animation, but I had to start somewhere 🙃 Second one, by just changing Z value was **sitting**. Next on the list: **waving** is a standing position with one leg up and moving in hip joint back and forth.
Walking and turning were more difficult so I'll describe them in a separate post.

Finally I've added some additional features like preset views, changing the animation speed, showing and hiding axles and _ground_ plane.

![Quadruped Simulator - Movements demo](/quadruped/assets/simulator-movements.gif)
