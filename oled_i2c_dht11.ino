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

const int dataPin=8;
int temperature=-1;
int humidity=-1;

void setup()   {                
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.    
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
}

int readDHT11()
{
    uint8_t recvBuffer[5];
    uint8_t cnt=7;
    uint8_t idx=0;
    for(int i=0; i<5; i++) recvBuffer[i]=0;

    //发送低电平脉冲开始通信
    pinMode(dataPin, OUTPUT);
    digitalWrite(dataPin, LOW);
    delay(18);
    digitalWrite(dataPin, HIGH);
    delayMicroseconds(40);
    pinMode(dataPin, INPUT);
    pulseIn(dataPin, HIGH);
    //读取数据包
    unsigned int timeout=10000;
    for(int i=0; i<40; i++)
    {
        timeout=10000;
        while(digitalRead(dataPin)==LOW)
        {
             if(timeout==0) return -1;
             timeout--;
         }
         unsigned long t=micros();

         timeout=10000;
         while(digitalRead(dataPin)==HIGH)
         {
               if(timeout==0) return -1;
               timeout--; 
          }

         if((micros()-t)>40) recvBuffer[idx] |= (1<<cnt);
         if(cnt==0)
         {
              cnt=7;
              idx++;
          }
          else cnt--;
    }

    humidity=recvBuffer[0];  //%
    temperature=recvBuffer[2];  //C
    uint8_t sum=recvBuffer[0]+recvBuffer[2];
    if(recvBuffer[4] != sum) return -2;
    return 0;
}

void loop() {
  int ret=readDHT11();
  if(ret != 0)
  {
    Serial.println(ret);
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  // text display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Data Collector");
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,16);
  display.print("Humidity: ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(humidity);
  display.setTextColor(WHITE);
  display.setCursor(100,16);
  display.print(" %");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,32);
  display.print("Temperature: ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(temperature);
  display.setTextColor(WHITE);
  display.setCursor(100,32);
  display.print(" C");
  
  display.display();
  display.clearDisplay();   // clears the screen and buffer
  delay(2000);  //ms
}
