## Emulation library of light day regimen (sunrise – sunset) for Arduino.
It supports smooth increase/decrease of brightness according to established duration.
### For what?
This functionality can be useful in projects on birdhouse automatization.
For example: regulation light day in hen-house or wakening parrots.
During smooth change of brightness, birds don’t feel stress.
### How does it work?
Declare regimen:
```c++
SunSimulation *s;
```
First of all we should define time of sunrise and sunset, best place for it is setup() function:
```c++
//sunrise time 6:15 am
// 6 — sunrise hour
// 15 – minutes
// sunrise duration 40 minutes
TimeSet *sunrise = new TimeSet(6, 15, 40);

// sunset time 8:42 pm
TimeSet *sunset = new TimeSet(20, 42, 50);
```
Next step is create regimen and initialize it:
```c++
s = new SunSimulation(sunrise, sunset);
s->regimenInit();
```
Function regimenInit() returns string, which reports about successful or wrong initialization. We can check it:
```c++
String message = s->regimenInit();
Serial.println(message);
```
Now in loop() function we can change variable which responsible for light brightness according to current time.
```c++
// 6 – current hour
// 35 – current minute
// 43 – current second
// br – our brightness value

s->changeBrightness(6, 35, 43, &br);
Serial.println(br);
```
Also we can change our previous regimen during loop() run:
```c++
s->reloadRegimen(newSunrise, newSunset);
```
Enjoy!
