# frc1571-arduinoLEDcontrol
An Arduino sketch using the NeoPixel library to allow for serial control of a single pin LED strip

##Valid serial commands

###'H' - Display the help dialog

###'S' - Start the command stream

###'F' - End the command stream and update working variables

###'M' + * *mode* * - Set the LED animation mode<br />
  0 - Solid Color<br />
  1 - Color Wipe<br />
  2 - Continuous Wipe<br />
  3 - Oscillating Wipe<br />
  4 - Color Fade<br />
  5 - Oscillating Fade<br />
      Note: Will fade to most recently set colors and from last set colors. E.G. send "S M5 R0 G255 B0 W20 F S R255 G0 B0 F" to oscillate between green and red. <br />
  6 - Theater Chase<br />
  7 - Rainbow<br />
  8 - Rainbow Cycle<br />
  9 - Theater Chase Rainbow<br />
  E.G. Send "M3" to set LED mode to Theater Chase
  
###'R' + * *value* * - Set the red intensity value for applicable modes
  0-255<br />
  E.G. Send "R255" to set full red intensity
  
###'G' + * *value* * - Set the green intensity value for applicable modes
  0-255<br />
  E.G. Send "G128" to set half green intensity
  
###'B' + * *value* * - Set the blue intensity value for applicable modes
  0-255
  E.G. Send "B0" to set no blue intensity
  
###'W' + * *value* * - Set the number of milliseconds to delay before next update
  No. of milliseconds<br />
  E.G. Send "W100" to delay 100 milliseconds between each update of the LEDs
  
**Full Serial Command Example**

  In order to set a continuous cyan color wipe with a one-tenth second delay between each update:
  Send "SM2R80G255B255W100F" or "S M2 R80 G255 B255 W100 F" (Spaces and other unused character will be discarded)
  
  Serial commands are interchangeable and can be set more than once. For example the command "S M2 W30 G10 R255 B45 M3 G250 F" will set   the Arduino to mode 3 (Oscillating Wipe), red 255, green 250, blue 45, wait 30ms (the last commands are the ones saved).
