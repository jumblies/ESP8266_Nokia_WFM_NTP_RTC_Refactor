void setup_wifi()
{

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
    wifiManager.setTimeout(10);
    // wifiManager.startConfigPortal("NokiaClockAP");
    wifiManager.autoConnect("NokiaClockAP");
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.printf("Wifi Connected; IP = %s\n", WiFi.localIP().toString().c_str());
    }
    timeClient.begin();
    timeClient.update();
    RtcDateTime utc;
    utc.InitWithEpoch64Time(timeClient.getEpochTime());
    Rtc.SetDateTime(utc);
};