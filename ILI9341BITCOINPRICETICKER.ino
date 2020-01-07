/*********************************************************************
 * Show Bitcoin exchange rates for US dollars, GB pounds, and EU euros
 * from CoinDesk. Display on ILI9341 SPI LCD display.
 * - Adafruit GFX library, no changes required
 *   Use the Arduino IDE Library Manager to get the latest version
 * - Arduino JSON library
 *   Use the Arduino IDE Library Manager to get the latest version
 *
 *********************************************************************/

/*
The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

//#include <WiFi.h>
//#include <ESP8266WiFi.h>
#ifndef min
#define min(x,y) (((x)<(y))?(x):(y))
#endif
#ifndef max
#define max(x,y) (((x)>(y))?(x):(y))
#endif
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h> // Hardware-specific library
  
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library


  #define TFT_GREY        0x5AEB
  #define TFT_BLACK       0x0000      
  #define TFT_NAVY        0x000F
  #define TFT_DARKGREEN   0x03E0      
  #define TFT_DARKCYAN    0x03EF      
  #define TFT_MAROON      0x7800      
  #define TFT_PURPLE      0x780F      
  #define TFT_OLIVE       0x7BE0      
  #define TFT_LIGHTGREY   0xC618      
  #define TFT_DARKGREY    0x7BEF      
  #define TFT_BLUE        0x001F  
  #define TFT_GREEN       0x07E0
  #define TFT_CYAN        0x07FF
  #define TFT_RED         0xF800
  #define TFT_MAGENTA     0xF81F      
  #define TFT_YELLOW      0xFFE0      
  #define TFT_WHITE       0xFFFF    
  #define TFT_ORANGE      0xFDA0      
  #define TFT_GREENYELLOW 0xB7E0  
  #define TFT_PINK        0xFC9F    

  // Don't change!!!  Comment the pinout in User_Setup.h and in the sketch if you want to use it with ESP32
  #define TFT_DC  D3
  #define TFT_CS  D8
  #define TFT_MOSI D7
  #define TFT_SCK D5
  //#define TFT_RST D4

  //For ESP32            // Pls edit User_Setup.h like this to use it with ESP32
  //#define TFT_MISO 19
  //#define TFT_MOSI 23
  //#define TFT_SCLK 18
  //#define TFT_CS    15  // Chip select control pin
  //#define TFT_DC    2  // Data Command control pin
  //#define TFT_RST   4  // Reset pin (could connect to RST pin)
  //#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is 





  const char SSID[]       = "*****************";
  const char PASSWORD[]   = "*****************";

 const unsigned char wifiBitmap [] PROGMEM = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 31, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 128,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 15, 255, 255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 255, 255, 254, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 192, 0, 0,
 0, 0, 0, 0, 0, 3, 255, 255, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 7,
 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 15, 255, 255, 255, 255, 248, 0, 0,
 0, 0, 0, 0, 0, 15, 255, 255, 255, 255, 252, 0, 0, 0, 0, 0, 0, 7, 255, 255,
 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 0, 0,
 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 3, 255, 255, 255, 255,
 255, 255, 255, 255, 224, 0, 0, 0, 7, 255, 255, 255, 255, 255, 128, 0, 1, 240, 0, 0, 
 0, 7, 255, 255, 255, 255, 255, 0, 0, 0, 120, 0, 0, 0, 15, 255, 255, 255, 255, 254,
 0, 0, 0, 60, 0, 0, 0, 31, 255, 255, 255, 255, 252, 0, 0, 0, 28, 0, 0, 0,
 31, 255, 255, 255, 255, 248, 0, 0, 0, 30, 0, 0, 0, 63, 255, 255, 255, 31, 240, 0,
 0, 30, 14, 0, 0, 0, 63, 7, 7, 6, 15, 240, 255, 255, 62, 14, 0, 0, 0, 63, 7,
 7, 6, 15, 224, 255, 255, 62, 7, 0, 0, 0, 127, 7, 7, 14, 15, 224, 255, 255, 62, 7, 0, 0, 0, 127, 135, 7, 15, 
 31, 224, 255, 255, 30, 7, 0, 0, 0, 127, 135, 7, 15, 255, 224, 255, 255, 0, 3, 0, 0, 0, 127,
 134, 6, 15, 255, 224, 252, 0, 30, 3, 0, 0, 0, 127, 130, 2, 14, 31, 224, 252, 0,
 62, 3, 0, 0, 0, 127, 130, 2, 30, 31, 224, 252, 0, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224,
 252, 0, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 194, 34, 30,
 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127,
 192, 32, 62, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 224, 32, 62, 31, 224, 255, 254, 62, 3, 0, 0,
 0, 127, 224, 96, 62, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 224, 112, 62, 31, 224, 252, 0, 62, 3, 0, 0,
 0, 127, 224, 112, 62, 31, 224, 252, 0, 62, 3, 0, 0, 0, 127, 224, 112, 126, 31, 224,
 252, 0, 62, 3, 0, 0, 0, 127, 240, 112, 126, 31, 192, 252, 0, 62, 7, 0, 0, 0, 127, 240, 240, 126, 31, 192,
 252, 0, 62, 7, 0, 0, 0, 63, 255, 255, 255, 255, 192, 248, 0, 62, 7, 0, 0, 0, 63, 255, 255, 255, 255,
 192, 0, 0, 0, 14, 0, 0, 0, 63, 255, 255, 255, 255, 128, 0, 0, 0, 14, 0, 0, 0, 31, 255, 255, 255, 255,
 128, 0, 0, 0, 30, 0, 0, 0, 31, 255, 255, 255, 255, 0, 0, 0, 0, 28, 0, 0, 0, 15, 255, 255, 255,
 255, 0, 0, 0, 0, 56, 0, 0, 0, 7, 255, 255, 255, 254, 0, 0, 0, 0, 248, 0, 0, 0, 7, 255, 255, 255, 252, 
 0, 0, 0, 1, 240, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 255, 255, 255,
 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0,
 0, 0, 7, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 7, 255, 255, 255, 255, 248, 0, 0, 0,
 0, 0, 0, 0, 3, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 255, 255, 224, 0, 0, 0, 0,
 0, 0, 0, 1, 255, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0,
 0, 0, 0, 63, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 255, 255, 254, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 7, 255, 255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 127, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


  // 60 second delay between normal updates
  #define DELAY_NORMAL    (60*1000)
  // 10 minute delay between updates after an error
  #define DELAY_ERROR     (10*60*1000)

  #define COINDESK    "api.coindesk.com"
  const char COINDESK_REQ[] =
    "GET /v1/bpi/currentprice.json HTTP/1.1\r\n"
    "User-Agent: ESP8266/0.1\r\n"
    "Accept: */*\r\n"
    "Host: " COINDESK "\r\n"
    "Connection: close\r\n\r\n";

     

  void setup()
  {
  Serial.begin(115200);
  
  
  tft.init();
  tft.fillScreen(TFT_BLACK );
  tft.setRotation(0); //2
  tft.setTextWrap(0);
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  
  

  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

 
  // Prices provided by CoinDesk so give them credit.
  // See http://www.coindesk.com/api/ for details.
   
  tft.setTextSize(3);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(25,10);
  tft.println(F("Powered by"));
  
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(70,55);
  tft.println(F("CoinDesk"));
  //tft.println();
  //tft.setTextWrap(true);
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(30,100);
  tft.println(F("http://www.coindesk.com/price"));
  //tft.setTextWrap(false);
  tft.drawBitmap(70, 150, wifiBitmap, 100, 70, TFT_WHITE);
  delay(5000);
}

void loop()
{
  char respBuf[1024];

  // http://api.coindesk.com/v1/bpi/currentprice.json
  // TBD https://api.coindesk.com/v1/bpi/currentprice.json
  WiFiClient httpclient;
  Serial.println(F("open socket to " COINDESK " port 80"));
  if (!httpclient.connect(COINDESK, 80)) {
    Serial.println(F("socket connect to coindesk.com failed"));
    //return;
    delay(100);
  }
  Serial.println(httpclient.connected() ?
      F("connected") : F("not connected"));

  Serial.print(F("Requesting "));
  Serial.println(COINDESK_REQ);
  httpclient.print(COINDESK_REQ);
  httpclient.flush();

  // Collect http response headers and content from CoinDesk
  // Headers are discarded.
  // The content is formatted in JSON and is left in respBuf.
  int respLen = 0;
  bool skip_headers = true;
  while (httpclient.connected()) {
    if (skip_headers) {
      String aLine = httpclient.readStringUntil('\n');
      // Blank line denotes end of headers
      if (aLine.length() <= 1) {
        skip_headers = false;
      }
    }
    else {
      int bytesIn;
      bytesIn = httpclient.read((uint8_t *)&respBuf[respLen], sizeof(respBuf) - respLen);
      Serial.print(F("bytesIn ")); Serial.println(bytesIn);
      if (bytesIn > 0) {
        respLen += bytesIn;
        if (respLen > sizeof(respBuf)) respLen = sizeof(respBuf);
      }
      else if (bytesIn < 0) {
        Serial.print(F("read error "));
        Serial.println(bytesIn);
      }
    }
    delay(1);
  }
  httpclient.stop();

  if (respLen >= sizeof(respBuf)) {
    Serial.print(F("respBuf overflow "));
    Serial.println(respLen);
    delay(DELAY_ERROR);
    return;
  }
  // Terminate the C string
  respBuf[respLen++] = '\0';
  Serial.print(F("respLen "));
  Serial.println(respLen);
  Serial.println(respBuf);

  // Parse JSON and show results
  if (showBitcoin(respBuf)) {
    delay(DELAY_NORMAL);
  }
  else {
    delay(DELAY_ERROR);
  }
}

bool showBitcoin(char *json)
{
  StaticJsonBuffer<1024> jsonBuffer;

  // JSON from CoinDesk in json
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println(F("jsonBuffer.parseObject() failed"));
    return false;
  }

  const char *time_updatedBST = root["time"]["updatedISO"];
  Serial.println(time_updatedBST);
  // Display date on 1 line and time on next
  //2015-09-25T09:14:00+00:00
  //012345678901234567890
  //          1         2
  char yyyymmdd[10+1];
  const char *hhmmss;
  strncpy(yyyymmdd, time_updatedBST, 10);
  yyyymmdd[10] = '\0';
  hhmmss = &time_updatedBST[11];
  
  

  JsonObject& bpi = root["bpi"];
   tft.init();
   tft.fillScreen(TFT_BLACK );
  tft.setTextSize(3);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(5,10);
  tft.print("Bitcoin price");

  // Show bitcoin exchange rate in US dollars
  const char *bpi_usd_rate = bpi["USD"]["rate"];
  Serial.println(bpi_usd_rate);
  tft.setTextSize(4);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(90,50);
  tft.print(F("USD"));
  tft.setTextSize(3);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(35,85);
  tft.println(bpi_usd_rate);

  // Show bitcoin exchange rate in Great Britain pounds
  const char *bpi_gbp_rate = bpi["GBP"]["rate"];
   tft.setTextSize(4);
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  tft.setCursor(90,115);
  tft.print(F("GBP"));
  
  tft.setTextSize(3);
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  tft.setCursor(35,150);
  tft.println(bpi_gbp_rate);
 

  //Show bitcoin exchange rate in EU Euros
  const char *bpi_eur_rate = bpi["EUR"]["rate"];
  Serial.println(bpi_eur_rate);
   tft.setTextSize(4);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(90,180);
  tft.print(F("EUR"));
  tft.setTextSize(3);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(35,215);
  tft.println(bpi_eur_rate);

  tft.setTextSize(2);
  tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);
  tft.setCursor(50,245);
  tft.print("Last updated:");
  
  tft.setTextSize(2);
  tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);
  tft.setCursor(60,265);
  tft.println(yyyymmdd);
  tft.setTextSize(2);
  tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);
  tft.setCursor(35,285);
  tft.println(hhmmss);
  tft.println();

  
  return true;
}
