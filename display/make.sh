#!/bin/bash

echo "" > arduino.txt
for i in emotions/*.png ; do ./png2arduino.swift "$i" >> arduino.txt ; done
echo "DONE"