#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 5
#define PIN_KNOP 7

#define DELAY_DEBOUNCE 20
#define DELAY_FLASH_FAST 25
#define DELAY_FLASH_SLOW 500

#define NUM_LEDS 3

#define BRIGHTNESS 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
bool is_green = false;
void setup() {
  strip.setBrightness(30);
  strip.begin();
  strip.setPixelColor(0, 255, 0, 0);
  strip.setPixelColor(1, 255, 0, 0);
  strip.setPixelColor(2, 255, 0, 0);
  strip.setPixelColor(3, 255, 0, 0);
  strip.setPixelColor(4, 0, 0, 0);
  strip.show();

  pinMode(PIN_KNOP, INPUT_PULLUP);
  
}

void loop() {
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;
  for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(int i=0; i< strip.numPixels(); i++) {
      wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
      strip.setPixelColor(i, strip.Color(red(wheelVal), green(wheelVal), blue(wheelVal)));
    }
    strip.show();
    delay(6);
    
    if(digitalRead(PIN_KNOP) == HIGH) {
      strip.setBrightness(255);
      for(int i=0; i<strip.numPixels(); i++){
        if(is_green) {
          strip.setPixelColor(i, 0, 255, 0);
          is_green = false;
        } else {
          strip.setPixelColor(i, 255, 255, 0);
          is_green = true;
        }
        strip.show();
      }
      delay(350);
    } else {
      strip.setBrightness(30);  
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

