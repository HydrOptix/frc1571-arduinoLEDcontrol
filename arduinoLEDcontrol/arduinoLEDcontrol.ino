#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

char incomingChar;
int ledMode = 0, incomingMode, index = 0;
uint32_t r=0,g=0,b=0,incomingR,incomingG,incomingB,lastR,lastG,lastB,currentColor;
uint8_t wait = 100,incomingWait;
bool receiving;

void setup() {
  receiving = false;
  Serial.begin(9600);
  while(!Serial) {}
  Serial.println("arduinoLEDcontrol by Samuel Frazee");
  Serial.println("    Send \'H\' for help dialog");
  strip.begin();
  strip.show();
}

void loop() {
  /*
   * Variables I need to receive
   * Start character 'S'
   * LED mode 'M' int
   * Red, Green, and Blue values (if applicable) 'R' int,'G' int,'B' int 
   * wait time till next loop 'W' int
   * finish signal 'F'
   */
  while(Serial.available() > 0) { //Loop while there are values in the serial buffer
    incomingChar = Serial.read();
    if(!receiving) {
      if(incomingChar == 'S') {
        Serial.println("Start signal received");
        receiving = true;
      } else if(incomingChar == 'H') {
        printHelpDialog();
      } else {
        Serial.print("Unknown command \'");
        Serial.print(incomingChar);
        Serial.println("\', send \'H\' for help dialog");
      }
    } else {
      if(incomingChar == 'F') {
        Serial.println("End Signal received");
        receiving = false;
        updateFromIncoming();
        index = 0;
      } else if(incomingChar == 'M') {
        Serial.println("Receiving mode variable");
        if(nextSerialIsNumber()) {
          incomingMode = Serial.parseInt();
          Serial.print("Received ");
          Serial.println(incomingMode);
        } else {
          Serial.println("Next char is not an int");
        }
      } else if(incomingChar == 'R') {
        Serial.println("Receiving r variable");
        if(nextSerialIsNumber()) {
          incomingR = Serial.parseInt();
          Serial.print("Received ");
          Serial.println(incomingR);
        } else {
          Serial.println("Next char is not an int");
        }
      } else if(incomingChar == 'G') {
        Serial.println("Receiving g variable");
        if(nextSerialIsNumber()) {
          incomingG = Serial.parseInt();
          Serial.print("Received ");
          Serial.println(incomingG);
        } else {
          Serial.println("Next char is not an int");
        }
      } else if(incomingChar == 'B') {
        Serial.println("Receiving b variable");
        if(nextSerialIsNumber()) {
          incomingB = Serial.parseInt();
          Serial.print("Received ");
          Serial.println(incomingB);
        } else {
          Serial.println("Next char is not an int");
        }
      } else if(incomingChar == 'W') {
        Serial.println("Receiving wait variable");
        if(nextSerialIsNumber()) {
          incomingWait = Serial.parseInt();
          Serial.print("Received ");
          Serial.println(incomingWait);
        } else {
          Serial.println("Next char is not an int");
        }
      } else if(incomingChar != ' ') {
        printHelpDialog();
      }
    }
  }
  //Start actually doing things with the LEDs

  if(ledMode == 0) {
    solidColor(currentColor);
  } else if(ledMode == 1) {
    colorWipe(currentColor);
  } else if(ledMode == 2) {
    continuousWipe(currentColor);
  } else if(ledMode == 3) {
    oscillatingWipe(currentColor);
  } else if(ledMode == 4) {
    colorFade(r,g,b);
  } else if(ledMode == 5) {
    oscillatingFade(lastR,lastG,lastB,r,g,b);
  } else if(ledMode == 6) {
    theaterChase(currentColor);
  } else if(ledMode == 7) {
    rainbow();
  } else if(ledMode == 8) {
    rainbowCycle();
  } else if(ledMode == 9) {
    rainbowTheaterChase();
  }
  
  delay(wait);
}

void printHelpDialog() {
  Serial.println("  arduinoLEDcontrol developed by Samuel Frazee for FRC Team 1571");
  Serial.println("  View documentation at https://github.com/HydrOptix/frc1571-arduinoLEDcontrol");
}

void updateFromIncoming() {
  ledMode = incomingMode;
  lastR = r;
  lastG = g;
  lastB = b;
  r = incomingR;
  g = incomingG;
  b = incomingB;
  currentColor = strip.Color(incomingR, incomingG, incomingB);
  wait = incomingWait;
  Serial.print("Saved Values: Mode=");
  Serial.print(ledMode);
  Serial.print(", Color=");
  Serial.print(currentColor);
  Serial.print(", Wait=");
  Serial.println(wait);
}

bool nextSerialIsNumber() {
  int nextChar = Serial.peek();
  return(nextChar > 47 && nextChar < 58);
}

uint8_t splitColor ( uint32_t c, char value ) {
  switch ( value ) {
    case 'r': return (uint8_t)(c >> 16);
    case 'g': return (uint8_t)(c >>  8);
    case 'b': return (uint8_t)(c >>  0);
    default:  return 0;
  }
}

void solidColor(uint32_t color) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void colorWipe(uint32_t color) {
    strip.setPixelColor(index, color);
    strip.show();
    if(index < strip.numPixels()) {
      index++;
    } else {
      index = 0;
    }
}

void continuousWipe(uint32_t color) {
  if(index < strip.numPixels()) {
    strip.setPixelColor(index,color);
  } else {
    strip.setPixelColor(index-strip.numPixels(), strip.Color(0,0,0));
  }
  strip.show();
  if(index < strip.numPixels()*2) {
    index++;
  } else {
    index = 0;
  }
}

void oscillatingWipe(uint32_t color) {
  if(index < strip.numPixels()) {
    strip.setPixelColor(index,color);
  } else {
    strip.setPixelColor(strip.numPixels()*2-index, strip.Color(0,0,0));
  }
  strip.show();
  if(index < strip.numPixels()*2) {
    index++;
  } else {
    index = 0;
  }
}

void colorFade(uint8_t newR, uint8_t newG, uint8_t newB) {
  for(int i = 0; i < strip.numPixels(); i++) {
    uint32_t pixelColor = strip.getPixelColor(i);
    uint8_t pixelR = splitColor(pixelColor, 'r');
    uint8_t pixelG = splitColor(pixelColor, 'g');
    uint8_t pixelB = splitColor(pixelColor, 'b');

    if(pixelR > newR) {
      pixelR--;
    } else if(pixelR < newR) {
      pixelR++;
    }
    if(pixelG > newG) {
      pixelG--;
    } else if(pixelG < newG) {
      pixelG++;
    }
    if(pixelB > newB) {
      pixelB--;
    } else if(pixelB < newB) {
      pixelB++;
    }

    strip.setPixelColor(i, strip.Color(pixelR,pixelG,pixelB));
  }
  strip.show();
}

void oscillatingFade(uint8_t rFrom, uint8_t gFrom, uint8_t bFrom, uint8_t rTo, uint8_t gTo, uint8_t bTo) {
  uint8_t difference = abs(rTo-rFrom);
  if(abs(gTo-gFrom) > difference) {
    difference = abs(gTo-gFrom);
  }
  if(abs(bTo-bFrom) > difference) {
    difference = abs(bTo-bFrom);
  }

  if(index < difference) {
    colorFade(rTo,gTo,bTo);
  } else {
    colorFade(rFrom,gFrom,bFrom);
  }

  if(index < difference*2) {
      index++;
    } else {
      index = 0;
    }
}

void theaterChase(uint32_t color) {
  for(uint16_t i=0; i < strip.numPixels(); i=i+3) {
    strip.setPixelColor(index+i,color);
  }
  strip.show();

  for(uint16_t i=0; i < strip.numPixels(); i=i+3) {
    strip.setPixelColor(index+i,0);
  }

  if(index < 3) {
      index++;
    } else {
      index = 0;
    }
}

void rainbow() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel((i+index) & 255));
  }
  strip.show();

  if(index < 256) {
    index++;
  } else {
    index = 0;
  }
}

void rainbowCycle() {
  for(uint16_t i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + index) & 255));
  }
  strip.show();
  
  if(index < 256) {
    index++;
  } else {
    index = 0;
  }
}

void rainbowTheaterChase() {
    for(uint16_t i=0; i < strip.numPixels(); i=i+3) {
      strip.setPixelColor(i+(index%3), Wheel((i+index) % 255));
    }
    strip.show();

    for(uint16_t i=0; i < strip.numPixels(); i=i+3) {
      strip.setPixelColor(i+index%3, 0);
    }
  
  if(index < 256) {
    index++;
  } else {
    index = 0;
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
