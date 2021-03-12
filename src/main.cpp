#include <Arduino.h>

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include "WiFiManager.h" //https://github.com/tzapu/WiFiManager
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <ESP8266Ping.h>

RTC_DS3231 rtc; //Adafruits fork of Jeelabs RTC.

//Timezone Library
#include <Timezone.h>
TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240}; //Daylight time = UTC - 4 hours
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};  //Standard time = UTC - 5 hours
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr; //pointer to the time change rule, use to get TZ abbrev
time_t utc, local;

// u8g2 Constructor
#include <U8g2lib.h>
U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0,
  /* clock=*/D3, /* data=*/D7, /* cs=*/D8, /* dc=*/D6, /* reset=*/D0);

#include "clock_draw.h"


unsigned long previousMillis;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{

  delay(500); //added to prevent current spike reboots
  Serial.begin(115200);
    while (!Serial) {
    delay(100); // wait for serial port to connect. Needed for Native USB only
  }

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    // Serial.flush();
    // abort();
  }

  // if (rtc.lostPower())
  // {
    // Serial.println("RTC lost power, let's set the time!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // }


  u8g2.begin();
  u8g2.firstPage();
  do
  {
    u8g2.setContrast(120);
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(0, 0, 84, 48);
    u8g2.setFont(u8g2_font_ncenB14_tr); /* select Font */
    u8g2.setDrawColor(2);
    u8g2.drawStr(18, 20, "WiFi");
    u8g2.drawStr(12, 42, "Check");
  } while (u8g2.nextPage());
  delay(50);

  WiFiManager wifiManager;
  wifiManager.resetSettings();
  wifiManager.setTimeout(5);
  wifiManager.autoConnect("NokiaClock");
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("Wifi Connected; IP = %s\n", WiFi.localIP().toString().c_str());
  }
  else
  {
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(1);
    Serial.println("Wifi Off\n");
  }
}

void loop()
{
  DateTime now = rtc.now();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000)
  { //update the display only if time has changed
    previousMillis = currentMillis;
    Serial.print("Temperature: ");
    float fahrenheit = ((rtc.getTemperature() * 9 / 5) + 32); //Convert Temp to Fahrenheit
    Serial.print(fahrenheit);
    Serial.printf("\t %02d %02d %04d\n", now.month(), now.day(), now.year());
    Serial.println();
    digitalClockDisplay(now, fahrenheit);
  }
}