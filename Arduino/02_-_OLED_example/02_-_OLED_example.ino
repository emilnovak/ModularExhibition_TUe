#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define WIDTH SSD1306_LCDWIDTH // = 128
#define HEIGHT SSD1306_LCDHEIGHT // = 64

uint8_t frameBuffer[WIDTH];

void setup() {
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  for (uint8_t i = 0; i < WIDTH; i++) {
    frameBuffer[i] = 0;  
  }
}

void loop() {
  
  for (uint8_t i = 1; i < WIDTH; i++) {
    frameBuffer[i - 1] = frameBuffer[i];
  }
  frameBuffer[WIDTH - 1] = map(analogRead(0), 0, 1023, 0, 63);


  display.clearDisplay();
  for (uint8_t i = 0; i < WIDTH; i++) {
    display.drawPixel(i, HEIGHT - 1 - frameBuffer[i], WHITE);
  }
  display.display();
  
}

