#!/bin/sh

# copy this platform project to an arduino sketch at ~/arduino

mkdir ./arduino
cp -a ./include/* ./arduino
cp -a ./src/* ./arduino
mv ./arduino/main.cpp ./arduino/arduino.ino