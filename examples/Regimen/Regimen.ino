/*
 * Sunrise - Sunset
 *
 * example of setting the mode sunrise - sunset
 */
 
#include <DS1307RTC.h>
#include <SunSimulation.h>

#define LIGHT_PIN 3

SunSimulation *sun;
tmElements_t tm;
byte brightness;

void setupBlink() {  
  analogWrite(LIGHT_PIN, 255);
  delay(300);
  analogWrite(LIGHT_PIN, 0);
  delay(300);
}

void setup() {   
  pinMode(LIGHT_PIN, OUTPUT);  
  Serial.begin(9600);    
  RTC.read(tm);
  Serial.println(tmYearToCalendar(tm.Year));
  
  // if the clock is not set (LED will blink twice)
  if (tm.Year < 2018 - 1970) {
    // set clock time - hour, minute, second, day, month, year
    setTime(10, 30, 0, 12, 11, 2018);  
    RTC.set(now());     
    setupBlink();
  }
  setupBlink();
   
  // sunrise - hour, minute, duration
  TimeSet sunRise(6, 45, 50);
  // sunset - hour, minute, duration
  TimeSet sunSet(21, 15, 58);

  sun = new SunSimulation(&sunRise, &sunSet, &brightness);
  // second way
//  sun = new SunSimulation(&sunRise, &sunSet);

  // initialization
  String message = sun->regimenInit();
  Serial.println(message);
}

void loop() {
  RTC.read(tm);
  sun->changeBrightness(tm.Hour, tm.Minute, tm.Second);
  // second way
//  sun->changeBrightness(tm.Hour, tm.Minute, tm.Second, &brightness);

    TimeState timeState = sun->getCurrentTimeState(tm.Hour, tm.Minute, tm.Second);
  
  Serial.println(brightness);

  Serial.print("Current time state is = ");
  Serial.println(timeState);
  analogWrite(LIGHT_PIN, brightness);

  delay(2000);
}
