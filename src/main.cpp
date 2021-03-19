/* 
REFACTOR 2021-03
  setup_wifi_config.h employs WiFiManager only when pin 10 is pulled low.  
    File contains code normally in startup
  

*/
#include <Arduino.h>

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

#include "WiFiManager.h" //https://github.com/tzapu/WiFiManager
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <ESP8266Ping.h>

//Timezone Library
#include <Timezone.h>
TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240}; //Daylight time = UTC - 4 hours
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};  //Standard time = UTC - 5 hours
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr; //pointer to the time change rule, use to get TZ abbrev
time_t utc, local;

//NTPClient
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org" /*, 3600, 60000*/);

// u8g2 Constructor
#include <U8g2lib.h>
U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0,
                                    /* clock=*/0, /* data=*/13, /* cs=*/15, /* dc=*/12, /* reset=*/16);

#include "clock_draw.h"
#include "setup_wifi_config.h"
#include "i2c_clear_bus.h"

unsigned long previousMillis;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{

  Serial.begin(115200);
  while (!Serial)
  {
    delay(100); // wait for serial port to connect. Needed for Native USB only
  }
  I2C_CheckBus();
  u8g2.begin();
  pinMode(10, INPUT);
  Rtc.Begin();

  if (digitalRead(10) == LOW)
  {
    setup_wifi();
  }
  else
  {
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(1);
    Serial.println("Wifi Off\n");
  }
  pinMode(10, OUTPUT);
  analogWrite(10,200);  //256 bit pwm
  // digitalWrite(2,LOW);
}

void loop()
{
  // Deprecating to use UTC
  RtcDateTime now = Rtc.GetDateTime();
  time_t utc = now.Epoch64Time();
  time_t local = myTZ.toLocal(utc); //convert to local time

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 500)
  { //update the display only if time has changed
    previousMillis = currentMillis;
    RtcTemperature temp = Rtc.GetTemperature();
    Serial.print("Temperature: ");
    Serial.println(temp.AsFloatDegF());
    // Serial.print("Local Time: ");
    // Serial.println(myTZ.toLocal(timeClient.getEpochTime()));
    Serial.print("NTP Epoch: ");
    Serial.println(timeClient.getEpochTime());

    Serial.print("RTC Epoch: ");
    char buffer[100];
    sprintf(buffer, "%llu", now.Epoch64Time());
    Serial.println(buffer);

    // Serial.printf("\t %02d %02d %04d\n", now.month(), now.day(), now.year());
    // Serial.println();
    digitalClockDisplay(local, temp.AsFloatDegF());
  }
}