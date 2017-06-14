---
layout: post
title:  "New Walking algorithm and battery strap"
date:   2017-05-25 20:00:00
---
I cut a small plate of acrylic and mounted it on M2 threaded bar on top of the body to keep UBEC below and the battery on top holden by velcro strap that came with the LiPo. I forgot to take pictures of the process. 😔
It's very rigid so I think I'll do the same with a plate for sensors.

The robot is falling while walking because the center of gravity is moving outside of a triangle that three legs on the ground do. I thought how to change the walk to make sure that CoG stays in. I found this article [How to Program a Quadruped Robot with Arduino](http://makezine.com/2016/11/22/robot-quadruped-arduino-program/) by Josh Elijah describing a walk that makes sure CoG is in the triangle. I implemented it and run on the robot. The battery strap also helps with keeping the CoG in the predicted place.

![Quadruped walking with a new algorithm](/quadruped/assets/quadruped-walking-with-a-new-algorithm.gif)
