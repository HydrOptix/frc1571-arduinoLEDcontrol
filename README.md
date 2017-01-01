# frc1571-arduinoLEDcontrol
An Arduino sketch to allow for serial control of a single pin LED strip

**Valid serial commands**

'S' - Start the command stream

'F' - End the command stream and update working variables

'M' + * *mode* * - Set the LED animation mode
  0 - Off
  1 - Solid Color
  2 - Color Wipe
  3 - Theater Chase
  4 - Rainbow
  5 - Rainbow Cycle
  6 - Theater Chase Rainbow
  E.G. Send "M3" to set LED mode to Theater Chase
  
'R' + * *value* * - Set the red intensity value for applicable modes
  0-255
  E.G. Send "R255" to set full red intensity
  
'G' + * *value* * - Set the green intensity value for applicable modes
  0-255
  E.G. Send "G128" to set half green intensity
  
'B' + * *value* * - Set the blue intensity value for applicable modes
  0-255
  E.G. Send "B0" to set no blue intensity
  
'W' + * *value* * - Set the number of milliseconds to delay before next update
  No. of milliseconds
  E.G. Send "W100" to delay 100 milliseconds between each update of the LEDs
  
**Full Serial Command Example**

  In order to set a cyan color wipe with a one-tenth second delay between each update:
  Send "SM2R80G255B255W100F" or "S M2 R80 G255 B255 W100 F" (Spaces and other unused character will be discarded)
  
  Serial commands are interchangeable and can be set more than once. For example the command "S M2 W30 G10 R255 B45 M3 G250 F" will set   the Arduino to mode 3 (Theater Chase), red 255, green 250, blue 45, wait 30ms (the last commands are the ones saved).
