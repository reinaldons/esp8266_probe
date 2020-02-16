#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#include "config.h"

#define TFT_GREY 0x5AEB

// Zoll SPI 1.77 inch TFT Display 128x160 Pixels
TFT_eSPI tft = TFT_eSPI();
setup_t user;

void setup() {
  // Main setup
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');

  // TFT
  tft.init();
  tft.setRotation(1);
  tft_info();

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  Serial.println('\n');
}

void tft_info() {
  tft.getSetup(user);

  Serial.printf("\n[code]\n");

  Serial.print ("TFT_eSPI ver = " + user.version + "\n");
  Serial.printf("Processor    = ESP%i\n", user.esp, HEX);
  Serial.printf("Frequency    = %i MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Voltage      = %2.2f V\n", ESP.getVcc() / 918.0); // 918 empirically determined
  Serial.printf("Transactions = %s \n",   (user.trans  ==  1) ? "Yes" : "No");
  Serial.printf("Interface    = %s \n",   (user.serial ==  1) ? "SPI" : "Parallel");
  if (user.serial ==  1)
  Serial.printf("SPI overlap  = %s \n\n", (user.overlap == 1) ? "Yes" : "No");
  if (user.tft_driver != 0xE9D) { // For ePaper displays the size is defined in the sketch
    Serial.printf("Display driver = "); Serial.println(user.tft_driver, HEX); // Hexadecimal code
    Serial.printf("Display width  = %i \n",   user.tft_width);  // Rotation 0 width and height
    Serial.printf("Display height = %i \n\n", user.tft_height);
  }
  else if (user.tft_driver == 0xE9D) Serial.printf("Display driver = ePaper\n\n");

  if (user.r0_x_offset  != 0)  Serial.printf("R0 x offset = %i \n",   user.r0_x_offset); // Offsets, not all used yet
  if (user.r0_y_offset  != 0)  Serial.printf("R0 y offset = %i \n",   user.r0_y_offset);
  if (user.r1_x_offset  != 0)  Serial.printf("R1 x offset = %i \n",   user.r1_x_offset);
  if (user.r1_y_offset  != 0)  Serial.printf("R1 y offset = %i \n",   user.r1_y_offset);
  if (user.r2_x_offset  != 0)  Serial.printf("R2 x offset = %i \n",   user.r2_x_offset);
  if (user.r2_y_offset  != 0)  Serial.printf("R2 y offset = %i \n",   user.r2_y_offset);
  if (user.r3_x_offset  != 0)  Serial.printf("R3 x offset = %i \n",   user.r3_x_offset);
  if (user.r3_y_offset  != 0)  Serial.printf("R3 y offset = %i \n\n", user.r3_y_offset);
  
  if (user.pin_tft_mosi != -1) Serial.printf("MOSI    = D%i (GPIO %i)\n",   getPinName(user.pin_tft_mosi), user.pin_tft_mosi);
  if (user.pin_tft_miso != -1) Serial.printf("MISO    = D%i (GPIO %i)\n",   getPinName(user.pin_tft_miso), user.pin_tft_miso);
  if (user.pin_tft_clk  != -1) Serial.printf("SCK     = D%i (GPIO %i)\n",   getPinName(user.pin_tft_clk), user.pin_tft_clk);

  if (user.overlap == true) {
    Serial.printf("Overlap selected, following pins MUST be used:\n");
    Serial.printf("MOSI     = SD1 (GPIO 8)\n");
    Serial.printf("MISO     = SD0 (GPIO 7)\n");
    Serial.printf("SCK      = CLK (GPIO 6)\n");
    Serial.printf("TFT_CS   = D3  (GPIO 0)\n\n");
    Serial.printf("TFT_DC and TFT_RST pins can be user defined\n");
  }

  if (user.pin_tft_cs   != -1) Serial.printf("TFT_CS   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_cs), user.pin_tft_cs);
  if (user.pin_tft_dc   != -1) Serial.printf("TFT_DC   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_dc), user.pin_tft_dc);
  if (user.pin_tft_rst  != -1) Serial.printf("TFT_RST  = D%i (GPIO %i)\n\n", getPinName(user.pin_tft_rst), user.pin_tft_rst);
  if (user.pin_tch_cs   != -1) Serial.printf("TOUCH_CS = D%i (GPIO %i)\n\n", getPinName(user.pin_tch_cs), user.pin_tch_cs);
  if (user.pin_tft_wr   != -1) Serial.printf("TFT_WR   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_wr), user.pin_tft_wr);
  if (user.pin_tft_rd   != -1) Serial.printf("TFT_RD   = D%i (GPIO %i)\n\n", getPinName(user.pin_tft_rd), user.pin_tft_rd);
  if (user.pin_tft_d0   != -1) Serial.printf("TFT_D0   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_d0), user.pin_tft_d0);
  if (user.pin_tft_d1   != -1) Serial.printf("TFT_D1   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_d1), user.pin_tft_d1);
  if (user.pin_tft_d2   != -1) Serial.printf("TFT_D2   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_d2), user.pin_tft_d2);
  if (user.pin_tft_d3   != -1) Serial.printf("TFT_D3   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_d3), user.pin_tft_d3);
  if (user.pin_tft_d4   != -1) Serial.printf("TFT_D4   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_d4), user.pin_tft_d4);
  if (user.pin_tft_d5   != -1) Serial.printf("TFT_D5   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_d5), user.pin_tft_d5);
  if (user.pin_tft_d6   != -1) Serial.printf("TFT_D6   = D%i (GPIO %i)\n",   getPinName(user.pin_tft_d6), user.pin_tft_d6);
  if (user.pin_tft_d7   != -1) Serial.printf("TFT_D7   = D%i (GPIO %i)\n\n", getPinName(user.pin_tft_d7), user.pin_tft_d7);
  
  uint16_t fonts = tft.fontsLoaded();
  if (fonts & (1 << 1))        Serial.printf("Font GLCD   loaded\n");
  if (fonts & (1 << 2))        Serial.printf("Font 2      loaded\n");
  if (fonts & (1 << 4))        Serial.printf("Font 4      loaded\n");
  if (fonts & (1 << 6))        Serial.printf("Font 6      loaded\n");
  if (fonts & (1 << 7))        Serial.printf("Font 7      loaded\n");
  if (fonts & (1 << 9))        Serial.printf("Font 8N     loaded\n");
  if (fonts & (1 << 8))        Serial.printf("Font 8      loaded\n");
  if (fonts & (1 << 15))       Serial.printf("Smooth font enabled\n");
  Serial.printf("\n");
  
  if (user.serial==1)          Serial.printf("Display SPI frequency = %2.1f MHz \n", user.tft_spi_freq/10.0);
  if (user.pin_tch_cs != -1)   Serial.printf("Touch SPI frequency   = %2.1f MHz \n", user.tch_spi_freq/10.0);
  
  Serial.printf("[/code]\n");
}

// Get pin name for ESP8266
int8_t getPinName(int8_t pin) {
  // For ESP32 pin labels on boards use the GPIO number
  if (user.esp == 32) return pin;

  // For ESP8266 the pin labels are not the same as the GPIO number
  // These are for the NodeMCU pin definitions:
  //        GPIO       Dxx
  if (pin == 16) return 0;
  if (pin ==  5) return 1;
  if (pin ==  4) return 2;
  if (pin ==  0) return 3;
  if (pin ==  2) return 4;
  if (pin == 14) return 5;
  if (pin == 12) return 6;
  if (pin == 13) return 7;
  if (pin == 15) return 8;
  if (pin ==  3) return 9;
  if (pin ==  1) return 10;
  if (pin ==  9) return 11;
  if (pin == 10) return 12;

  return -1; // Invalid pin
}

void loop() {
  // Fill screen with grey so we can see the effect of printing with and without 
  // a background colour defined
  tft.fillScreen(TFT_GREY);
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.println("Hello World!");
  
  // Set the font colour to be yellow with no background, set to font 7
  tft.setTextColor(TFT_YELLOW); tft.setTextFont(2);
  tft.println(1234.56);
  
  // Set the font colour to be red with black background, set to font 4
  tft.setTextColor(TFT_RED,TFT_BLACK);    tft.setTextFont(4);
  tft.println((long)3735928559, HEX); // Should print DEADBEEF

  // Set the font colour to be green with black background, set to font 2
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextFont(2);
  tft.println("Groop");

  // Test some print formatting functions
  float fnumber = 123.45;
   // Set the font colour to be blue with no background, set to font 2
  tft.setTextColor(TFT_BLUE);    tft.setTextFont(2);
  tft.print("Float = "); tft.println(fnumber);           // Print floating point number
  tft.print("Binary = "); tft.println((int)fnumber, BIN); // Print as integer value in binary
  tft.print("Hexadecimal = "); tft.println((int)fnumber, HEX); // Print as integer number in Hexadecimal

  while(1) yield(); // We must yield() to stop a watchdog timeout.
}
