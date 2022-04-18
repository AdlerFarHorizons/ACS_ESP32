//////////////////////////////////////////////////////////////////////////////////////////////
//                                      OLED                                             ///
//////////////////////////////////////////////////////////////////////////////////////////////
#include "Adafruit_GFX.h"     //OLED libary
#include "Adafruit_SSD1306.h" //OLED library

//OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
void oled_setup(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}
void OLEDScrollText(String text) {
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(text);
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  for (int i=0; i<1; i++){
    display.startscrollleft(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
    delay(1000);
    display.startscrollright(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
    delay(1000);
  }
}

