void digitalClockDisplay(RtcDateTime now, float fahrenheit)
{

  u8g2.firstPage();
  do
  {
    
    u8g2.setFontDirection(0);
    //Little numerals for date across top
    u8g2.setFont(u8g2_font_prospero_bold_nbp_tf);
    
    char temp[3];
    u8g2.setCursor(1, 9);
    u8g2.printf("%s", (dtostrf(fahrenheit, 2, 0, temp)));
    // u8g2.drawStr(16, 9, "F");
    u8g2.drawGlyph(16, 11, 176);
    u8g2.setCursor(25, 9);
    u8g2.printf("%02d %02d %04d\n", now.Month(), now.Day(), now.Year());

    // seconds
    u8g2.setCursor(64, 47);
    u8g2.printf("%02d", now.Second());

    //Big Numerals for time
    u8g2.setFont(u8g2_font_helvB24_tn); //u8g2_font_logisoso24_tf Second Choice
    u8g2.setCursor(1, 35);
    u8g2.printf("%02d", now.Hour());

    u8g2.setCursor(35, 32);
    u8g2.print(":"); //Raising the colon higher than the bottom line

    u8g2.setCursor(45, 35);
    u8g2.printf("%02d", now.Minute());

    // Graphical Seconds
    u8g2.setDrawColor(1); /* color  for the box */
    u8g2.drawFrame(0, 38, 61, 10);
    u8g2.drawBox(1, 38, now.Second(), 10);

    if (myTZ.locIsDST(local))
    {
      u8g2.setFont(u8g2_font_blipfest_07_tr);
      u8g2.setFontDirection(1);
      u8g2.drawStr(77, 37, "DST");
    };

  } while (u8g2.nextPage());
  delay(50);
}