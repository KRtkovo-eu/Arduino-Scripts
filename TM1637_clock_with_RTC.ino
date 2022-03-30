#include "TM1637.h" // source: https://github.com/Seeed-Studio/Grove_4Digital_Display
#include <Wire.h>
#include "RTClib.h" // source: https://github.com/adafruit/RTClib

RTC_DS1307 rtc;
byte runHours = 0;
byte runMinutes = 0;
byte runSeconds = 0;

#define CLK 2 //pins definitions for TM1637 and can be changed to other ports
#define DIO 3
TM1637 tm1637(CLK, DIO);

void setup() {
  rtc.begin();
  //rtc.adjust(DateTime(2022,03,18,12,43,0)); // YEAR, MONTH, DAY, HOURS, MINUTES, SECONDS - manually set current time to adjust RTC and then comment this line to prevent RTC time shift on each device restart
  
  // do initial time variables update
  UpdateDeviceTime();
  
  // set display
  tm1637.init();
  tm1637.set(5); // set brightness 0-7
}
void loop() {
  DrawClockTime();
  UpdateDeviceTime();
  
  // delay 1 second to prevent displaying weird characters
  delay(1000);
}

static void UpdateDeviceTime() {
  DateTime now = rtc.now();
  runSeconds = now.seconds(), DEC;
  runHours = now.hour(), DEC;
  runMinutes = now.minute(), DEC;
}

static void DrawClockTime() {
  int8_t TimeDisp[] = {0x00, 0x00, 0x00, 0x00};
  TimeDisp[0] = runHours / 10;
  TimeDisp[1] = runHours % 10;
  TimeDisp[2] = runMinutes / 10;
  TimeDisp[3] = runMinutes % 10;

  if (runSeconds % 2) {
      tm1637.point(POINT_ON);
  } else {
      tm1637.point(POINT_OFF);
  }

  tm1637.display(TimeDisp);
}
