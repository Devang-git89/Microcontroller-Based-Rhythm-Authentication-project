#Arduino-Based-Rhythm-Authentication-System
Microcontroller-Based Rhythm Authentication System

Overview
RhythmLock is an Arduino Leonardo project that authenticates users based on unique clap patterns.
The system analyzes the ratios between gaps in claps, making it tempo-invariant.
It can work with any user-defined clap phrase, as long as the pattern has been tested for gaps and ration and hardcoded with reliable thresholds to minimize false or postives or negatives
The system provides LED/LCD feedback and unlock a computer via USB.

Features
- Clap detection with microphone and MAX9814 amplifier
- Merge window to collapse mutiple sound inputs from one clap into a singular datapoint using sliding windows
- Gap ratio analysis for tempo-invariant rhythm recognition 
- Works with any calibrated clap sequence
- RGB LED and LCD1602 feedback
- Can unlock a computer using USB HID (specifc to microcontroller model of Lenoardo not possible with UNO)

Hardware
- Arduino Leonardo
- MAX9814 microphone amplifier
- RGB LEDs
- LCD1602 display
- Breadboard and resistors
- Software / Logic

Written in C++ for Arduino IDE
Pipeline:
- Merge window for handling many datapoints for one clap
- Gap ratio calculation
- Pattern matching against stored and tested ratio list for decided rythm

please watch vedio demo of my project linked below:
https://youtu.be/kSgmm0SC4Us



