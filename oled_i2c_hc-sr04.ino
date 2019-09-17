/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using SPI to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

//libs for OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
const static unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int trigPin=8;
int echoPin=7;
float v=331.5+0.6*20;  // m/s

void setup()   {                
  Serial.begin(115200);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  // initialize with the I2C addr 0x3C (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.    
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
}

float distanceM() {
  //发送声音脉冲
  digitalWrite(trigPin,LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  //监听回声
  float tUs=pulseIn(echoPin,HIGH); //ms
  float t=tUs/1000.0/1000.0/2;  //s
  float d=t*v;  //m
  return d*100;  //cm
}
 
void loop() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  int d=distanceM();
  Serial.println(d,DEC);
  // text display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Data Collector");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,16);
  display.print("distance is: ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(d);
  display.setTextColor(WHITE);
  display.setCursor(100,16);
  display.print(" cm");
  display.display();
  display.clearDisplay();   // clears the screen and buffer
  delay(200);
}
