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
/*********************************************************************
 * RS-WS-N01-TR temperature and humidity sensor
// (Sensor Device) black wire <->  RS485 A+ (RS485-RS232)
// (Sensor Device) gray  wire <->  RS485 B- (RS485-RS232)
// (RS485-RS232)   RxD        <->  0 -> RX  (Arduino UNO R3)
// (RS485-RS232)   TxD        <->  1 -> TX  (Arduino UNO R3)
*********************************************************************/

#include <SimpleModbusMaster.h>

#define baud 4800
#define timeOut 1000
#define polling 200

Packet packetsRS_WS_N01_TR[1];
packetPointer pointerPacket=&packetsRS_WS_N01_TR[0];
unsigned int sensorValues[9];

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
{ 
  B00000000, B11000000,
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
  B00000000, B00110000
};

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {                
  Serial.begin(4800);
  //init sensor RS_WS_N01_TR
  pointerPacket->id=1;
  pointerPacket->function=READ_HOLDING_REGISTERS;
  pointerPacket->address=0;
  pointerPacket->no_of_registers=2;
  pointerPacket->register_array=sensorValues;

  modbus_configure(baud,timeOut,polling,packetsRS_WS_N01_TR);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  // initialize with the I2C addr 0x3C (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  // init done
  
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
   // text display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Data Collector");
}

  
void loop() {
  //get the sensors value
  modbus_update(pointerPacket);
  // text display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,16);
  display.print("Temperature is: ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(sensorValues[5],HEX);
  display.println(sensorValues[6],HEX);
  display.setTextColor(WHITE);
  display.setCursor(0,32);
  display.print("Humidity is: ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(sensorValues[3],HEX);
  display.println(sensorValues[4],HEX);
  display.display();
  delay(2000);
}
