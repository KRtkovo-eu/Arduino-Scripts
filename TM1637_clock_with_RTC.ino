#include "TM1637.h" // source: https://github.com/Seeed-Studio/Grove_4Digital_Display
#include <StopWatch.h>  // source: https://playground.arduino.cc/Code/StopWatchClass/
#include <Wire.h>
#include "RTClib.h" // source: https://github.com/adafruit/RTClib

RTC_DS1307 rtc;

int8_t TimeDisp[] = {0x00, 0x00, 0x00, 0x00};

StopWatch stopWatchClock;
byte runHours = 0;
byte runMinutes = 0;
byte runSeconds = 0;
byte runHoursShift = 0;
byte runMinutesShift = 0;
byte currentHours = 0;
byte currentMinutes = 0;

#define CLK 2//pins definitions for TM1637 and can be changed to other ports
#define DIO 3
TM1637 tm1637(CLK, DIO);

void setup() {
  rtc.begin();
  //rtc.adjust(DateTime(2022,03,18,12,43,0)); // YEAR, MONTH, DAY, HOURS, MINUTES, SECONDS - manually set current time to adjust RTC and then comment this line to prevent RTC time shift on each device restart
  DateTime now = rtc.now();
  runHoursShift = now.hour(), DEC;
  runMinutesShift = now.minute(), DEC;
  
  // start stop watch
  stopWatchClock.start();
  
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


unsigned long startMillisClock;
static void UpdateDeviceTime() {
  unsigned long runMillis = stopWatchClock.elapsed();
  
  // Update time variables only once per second
  if(runMillis - startMillisClock >= 1000) {
    unsigned long allSeconds = runMillis/1000;
    runHours = allSeconds/3600;
    int secsRemaining = allSeconds%3600;
    runMinutes = secsRemaining/60;
    runSeconds = secsRemaining%60;

    // Handle time shift set by user or rtc
    currentHours = runHours + runHoursShift;
    currentMinutes = runMinutes + runMinutesShift;

    // Handle shifted time overflow
    if(currentMinutes >= 60) {
      currentMinutes = currentMinutes - 60;
      currentHours++;
    }
    currentHours = currentHours % 24;

    startMillisClock = runMillis;
  }
}

static void DrawClockTime() {
  int8_t TimeDisp[] = {0x00, 0x00, 0x00, 0x00};
  TimeDisp[0] = currentHours / 10;
  TimeDisp[1] = currentHours % 10;
  TimeDisp[2] = currentMinutes / 10;
  TimeDisp[3] = currentMinutes % 10;

  if (runSeconds % 2) {
      tm1637.point(POINT_ON);
  } else {
      tm1637.point(POINT_OFF);
  }

  tm1637.display(TimeDisp);
}
