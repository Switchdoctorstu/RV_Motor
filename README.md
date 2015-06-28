# RV_Motor
Ardunio Pro Mini APM to L298 driver

This code is for an arduino pro mini used to interface an ardupilot apm to a set of L298 motor controllers.

It takes the rc as inputs and drives up to 6 motors.

For my tracked vehicle with robotic arm, I used two channels for the main tracks and a further 4 for the robotic arm

The L298 wants 2 x enable pins which set the direction and a PWM speed

I used a 7404 invertor to drive the opposite EN pin for each controller to save on pins.

 Using Arduino Pro Mini 
 
  Arduino pro Pins

   D0

   D1

   D2 - rc1
   D3 - EN Track 0

   D4 - rc2
 D5 - EN track 1
   D6 - EN Arm 0
   D7 - rc 3

   D8 - RC Chan 4 left stick
   D9 - EN Arm 1
   D10 - EN arm 2
   D11 - EN arm 3
   D12 - Dir Track 0
   D13 - Dir Track 1
   A0 - D Arm 0
   A1 - D Arm 2

   A3 - D Arm 3
   A3 - D Arm 3

   A4 - Gyro
   A5 - Gyro

   A6 - Free (analog in only)
   A7 - Free       "
